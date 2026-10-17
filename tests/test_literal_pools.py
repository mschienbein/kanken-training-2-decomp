"""Opt-in whole-record literal placement; synthetic data only, no private inputs."""
import json
import struct
import sys
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import inplace  # noqa: E402
import match  # noqa: E402

BASE = 0x100000
POOL = 0x180000
MODEL = "inplace-literal-records-v1"


class Memory:
    def __init__(self):
        self.data = bytearray(0x110000)

    def read(self, address, size):
        if not BASE <= address <= address + size <= BASE + len(self.data):
            raise ValueError("synthetic address outside image")
        return bytes(self.data[address - BASE:address - BASE + size])

    def write(self, address, data):
        self.data[address - BASE:address - BASE + len(data)] = data


def pool_bytes(records, order, alignment):
    data = bytearray()
    offsets = {}
    for index, record in enumerate(order):
        offsets[record] = len(data)
        data.extend(records[record])
        if index + 1 < len(order):
            data.extend(bytes((-len(data)) % alignment))
    return bytes(data), offsets


def fixture(unit=2, alignment=4, order=(2, 0, 1), lengths=(4, 6, 8)):
    records = [(chr(65 + i) * length).encode("utf-16le" if unit == 2 else "ascii")
               + bytes(unit) for i, length in enumerate(lengths)]
    candidate, offsets = pool_bytes(records, range(3), alignment)
    original, output_offsets = pool_bytes(records, order, alignment)
    section_name = ".constwstring" if unit == 2 else ".conststring"
    pool = inplace.Section(2, section_name, candidate, len(candidate), False, False, False,
                           align=alignment, merge_strings=True, entsize=unit)
    symbol = inplace.Symbol(section_name, 0, len(candidate), "STT_SECTION", "STB_LOCAL", 2)
    refs = [0, 1, 2, 0]
    code = inplace.Section(1, "i.func_00100000", struct.pack("<4I", *(offsets[i] for i in refs)),
                           16, False, True, False)
    code.relocs = [inplace.Reloc(i * 4, inplace.R_ARM_ABS32, symbol) for i in range(4)]
    function = inplace.Symbol("func_00100000", 0, 16, "STT_FUNC", "STB_GLOBAL", 1)
    obj = SimpleNamespace(sections={1: code, 2: pool}, symbols=[function, symbol],
                          defined_functions=lambda: [function], defined_globals=lambda: [function])
    memory = Memory()
    memory.write(POOL, original)
    memory.write(BASE, struct.pack("<4I", *(POOL + output_offsets[i] for i in refs)))
    functions = {BASE: {"address": BASE, "size": 16, "isa": "arm", "code_ranges": []}}
    linker = inplace.InPlaceLinker(memory, functions, inplace.Resolver(functions),
                                   (0x200000, 0x201000), MODEL)
    return obj, memory, linker, offsets, output_offsets, original


class LiteralPoolTest(unittest.TestCase):
    def test_reorders_complete_candidate_records_for_frozen_alignments(self):
        for unit, alignment in ((1, 1), (1, 4), (2, 2), (2, 4)):
            with self.subTest(unit=unit, alignment=alignment):
                # Same remainder at every record end keeps the final extent invariant.
                lengths = (5, 9, 13) if unit == 1 else (4, 6, 8)
                obj, memory, linker, offsets, outputs, original = fixture(unit, alignment, lengths=lengths)
                linked = linker.link(obj)
                self.assertEqual(linked[1][1], memory.read(BASE, 16))
                self.assertEqual(linked[2][1], original)
                self.assertEqual(linked[2][0].literal_offsets,
                                 {offsets[i]: outputs[i] for i in range(3)})
                self.assertEqual(obj.sections[2].data, pool_bytes(
                    [(chr(65 + i) * n).encode("utf-16le" if unit == 2 else "ascii") + bytes(unit)
                     for i, n in enumerate(lengths)], range(3), alignment)[0])
                record = linker.literal_rebindings[0]
                self.assertEqual(record["rule"], "anonymous-string-records-v1")
                self.assertEqual((record["base"], record["record_count"], record["reference_count"]),
                                 (POOL, 3, 4))
                self.assertEqual(set(record["records"][0]), {"candidate_offset", "output_offset", "size"})

    def test_default_and_strict_models_still_refuse_permutations(self):
        for model in ("strict-v1", inplace.DEFAULT_LINKER_MODEL):
            obj, _, linker, *_ = fixture()
            linker.model = inplace.linker_model(model)
            with self.assertRaisesRegex(inplace.LinkError, "references disagree"):
                linker.link(obj)
        self.assertFalse(inplace.linker_model(inplace.DEFAULT_LINKER_MODEL).string_records)

    def test_target1_references_are_supported(self):
        obj, memory, linker, *_ = fixture()
        obj.sections[1].relocs[1].type = inplace.R_ARM_TARGET1
        self.assertEqual(linker.link(obj)[1][1], memory.read(BASE, 16))

    def test_unchanged_order_also_has_an_explicit_receipt(self):
        obj, memory, linker, *_ = fixture(order=(0, 1, 2))
        self.assertEqual(linker.link(obj)[1][1], memory.read(BASE, 16))
        self.assertEqual(len(linker.literal_rebindings), 1)

    def test_section_name_without_merge_flags_does_not_enable_rule(self):
        obj, _, linker, *_ = fixture()
        obj.sections[2].merge_strings = False
        with self.assertRaisesRegex(inplace.LinkError, "references disagree"):
            linker.link(obj)

    def test_pool_attributes_fail_closed(self):
        for field, value in (("writable", True), ("executable", True), ("nobits", True),
                             ("comdat", "synthetic"), ("entsize", 1), ("align", 8),
                             ("size", 1 << 21)):
            with self.subTest(field=field):
                obj, _, linker, *_ = fixture()
                setattr(obj.sections[2], field, value)
                with self.assertRaisesRegex(inplace.LinkError, "eligible anonymous"):
                    linker.link(obj)

    def test_named_pool_definitions_are_refused(self):
        for kind, bind, value in (("STT_OBJECT", "STB_LOCAL", 0),
                                  ("STT_SECTION", "STB_GLOBAL", 0),
                                  ("STT_SECTION", "STB_LOCAL", 4)):
            obj, _, linker, *_ = fixture()
            symbol = obj.symbols[1]
            symbol.kind, symbol.bind, symbol.value = kind, bind, value
            with self.assertRaisesRegex(inplace.LinkError, "named or non-section"):
                linker.link(obj)

    def test_pool_relocations_are_refused(self):
        obj, _, linker, *_ = fixture()
        obj.sections[2].relocs.append(obj.sections[1].relocs[0])
        with self.assertRaisesRegex(inplace.LinkError, "eligible anonymous"):
            linker.link(obj)

    def test_missing_record_witness_is_refused(self):
        obj, _, linker, *_ = fixture()
        obj.sections[1].relocs.pop(2)
        with self.assertRaisesRegex(inplace.LinkError, "every string record"):
            linker.link(obj)

    def test_unplaced_reference_source_is_refused(self):
        obj, _, linker, *_ = fixture()
        extra = inplace.Section(3, ".constdata", bytes(4), 4, False, False, False)
        extra.relocs.append(inplace.Reloc(0, inplace.R_ARM_ABS32, obj.symbols[1]))
        obj.sections[3] = extra
        with self.assertRaisesRegex(inplace.LinkError, "unplaced incoming"):
            linker.link(obj)

    def test_references_through_a_placed_table_are_verified(self):
        obj, memory, linker, *_ = fixture()
        code = obj.sections[1]
        table = inplace.Section(3, ".constdata", code.data, 16, False, False, False,
                                relocs=code.relocs)
        table_symbol = inplace.Symbol(".constdata", 0, 16, "STT_SECTION", "STB_LOCAL", 3)
        obj.sections[3] = table
        obj.symbols.append(table_symbol)
        memory.write(0x190000, memory.read(BASE, 16))
        code.data = bytes(16)
        code.relocs = [inplace.Reloc(0, inplace.R_ARM_ABS32, table_symbol)]
        memory.write(BASE, struct.pack("<I", 0x190000) + bytes(12))
        linked = linker.link(obj)
        self.assertEqual(linked[1][1], memory.read(BASE, 16))
        self.assertEqual(linked[3][1], memory.read(0x190000, 16))

    def test_unsupported_incoming_relocation_is_refused(self):
        obj, _, linker, *_ = fixture()
        obj.sections[1].relocs[0].type = inplace.R_ARM_REL32
        with self.assertRaisesRegex(inplace.LinkError, "unsupported or unplaced"):
            linker.link(obj)

    def test_interior_and_suffix_references_are_refused(self):
        obj, _, linker, *_ = fixture()
        obj.sections[1].data = struct.pack("<I", 2) + obj.sections[1].data[4:]
        with self.assertRaisesRegex(inplace.LinkError, "whole-record start"):
            linker.link(obj)

    def test_repeated_references_must_agree(self):
        obj, memory, linker, *_ = fixture()
        memory.write(BASE + 12, struct.pack("<I", POOL))
        with self.assertRaisesRegex(inplace.LinkError, "repeated string references"):
            linker.link(obj)

    def test_distinct_records_cannot_alias(self):
        obj, memory, linker, *_ = fixture()
        memory.write(BASE + 4, memory.read(BASE, 4))
        with self.assertRaisesRegex(inplace.LinkError, "not bijective"):
            linker.link(obj)

    def test_original_targets_must_be_aligned(self):
        obj, memory, linker, *_ = fixture()
        memory.write(BASE + 4, struct.pack("<I", POOL + 1))
        with self.assertRaisesRegex(inplace.LinkError, "misaligned"):
            linker.link(obj)

    def test_gaps_between_original_records_are_refused(self):
        obj, memory, linker, *_ = fixture()
        pointer = struct.unpack("<I", memory.read(BASE + 4, 4))[0]
        memory.write(BASE + 4, struct.pack("<I", pointer + 4))
        with self.assertRaisesRegex(inplace.LinkError, "contiguous"):
            linker.link(obj)

    def test_original_function_or_bss_overlap_is_refused(self):
        for overlap in ("function", "bss"):
            obj, _, linker, *_ = fixture()
            if overlap == "function":
                linker.functions[POOL] = {"size": 4, "isa": "arm"}
            else:
                linker.bss = (POOL, POOL + 4)
            with self.assertRaisesRegex(inplace.LinkError, "catalogued function or BSS"):
                linker.link(obj)

    def test_other_placed_section_overlap_is_refused(self):
        obj, memory, linker, *_ = fixture()
        extra = inplace.Section(3, ".constdata", bytes(4), 4, False, False, False)
        obj.sections[3] = extra
        symbol = inplace.Symbol("data_00180000", 0, 4, "STT_OBJECT", "STB_GLOBAL", 3)
        obj.symbols.append(symbol)
        obj.defined_globals = lambda: [obj.symbols[0], symbol]
        with self.assertRaisesRegex(inplace.LinkError, "overlap"):
            linker.link(obj)

    def test_original_range_outside_image_is_refused(self):
        obj, memory, linker, *_ = fixture()
        pointers = struct.unpack("<4I", memory.read(BASE, 16))
        memory.write(BASE, struct.pack("<4I", *(p + 0x400000 for p in pointers)))
        with self.assertRaisesRegex(inplace.LinkError, "outside the image"):
            linker.link(obj)

    def test_changed_record_content_is_not_patched_from_original(self):
        obj, _, linker, *_ = fixture()
        sec = obj.sections[2]
        sec.data = b"Z" + sec.data[1:]
        with self.assertRaisesRegex(inplace.LinkError, "differs from the candidate"):
            linker.link(obj)

    def test_original_padding_is_verified(self):
        obj, memory, linker, *_ = fixture()
        memory.write(POOL + 18, b"\x01")
        with self.assertRaisesRegex(inplace.LinkError, "differs from the candidate"):
            linker.link(obj)

    def test_candidate_padding_must_be_zero(self):
        obj, _, linker, *_ = fixture()
        sec = obj.sections[2]
        sec.data = sec.data[:10] + b"\x01" + sec.data[11:]
        with self.assertRaisesRegex(inplace.LinkError, "alignment padding"):
            linker.link(obj)

    def test_duplicate_empty_and_unterminated_records_are_refused(self):
        for data, message in ((b"A\0\0\0A\0\0\0", "duplicate"),
                              (bytes(4), "empty or unterminated"),
                              (b"A\0B\0", "empty or unterminated")):
            obj, _, linker, *_ = fixture()
            sec = obj.sections[2]
            sec.data = data
            sec.size = len(data)
            with self.assertRaisesRegex(inplace.LinkError, message):
                linker.link(obj)

    def test_reordering_cannot_change_total_extent(self):
        obj, _, linker, *_ = fixture(lengths=(3, 4, 5))
        with self.assertRaisesRegex(inplace.LinkError, "changes the section size"):
            linker.link(obj)

    def test_nonrelocated_function_bytes_are_untouched(self):
        obj, memory, linker, *_ = fixture()
        obj.sections[1].data += struct.pack("<I", 7)
        obj.sections[1].size += 4
        linker.functions[BASE]["size"] += 4
        memory.write(BASE + 16, struct.pack("<I", 9))
        linked = linker.link(obj)
        self.assertNotEqual(linked[1][1], memory.read(BASE, 20))
        self.assertEqual(linked[1][1][-4:], struct.pack("<I", 7))

    def test_rebinding_receipt_resets_for_each_link(self):
        obj, _, linker, *_ = fixture()
        linker.link(obj)
        first = json.dumps(linker.literal_rebindings, sort_keys=True)
        linker.link(obj)
        self.assertEqual(json.dumps(linker.literal_rebindings, sort_keys=True), first)

    def test_match_context_exposes_the_same_count_only_receipt(self):
        obj, memory, linker, *_ = fixture()
        context = match.Context.__new__(match.Context)
        context.image = memory
        memory.bss = linker.bss
        context.functions = linker.functions
        context.resolver = linker.resolver
        context.model = linker.model
        with patch.object(inplace, "ObjectFile", return_value=obj):
            results = context.compare(Path("synthetic.o"))
        self.assertEqual(results[0]["status"], "match")
        self.assertEqual(context.literal_rebindings[0]["record_count"], 3)


if __name__ == "__main__":
    unittest.main()
