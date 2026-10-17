"""Relocation-masked discovery against synthetic ELF objects and images only."""
import contextlib
import io
import json
from pathlib import Path
import struct
import sys
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import fingerprint  # noqa: E402
import inplace  # noqa: E402
import toolchain  # noqa: E402

BASE = 0x100000
BODY = struct.pack("<4I", 0xE5900004, 0xE2800007, 0xE5800008, 0xE12FFF1E)


class Memory:
    def __init__(self, code):
        self.code = bytes(code)
        self.sha256 = "synthetic"
        self.layout = SimpleNamespace(text=SimpleNamespace(address=BASE, size=len(code)))

    def read(self, address, size):
        if not BASE <= address <= address + size <= BASE + len(self.code):
            raise ValueError("outside synthetic text")
        return self.code[address - BASE:address - BASE + size]


def symbol(name="library_function", value=0, size=0):
    return inplace.Symbol(name, value, size, "STT_FUNC", "STB_GLOBAL", 1)


def object_fixture(data=BODY, symbols=None, relocs=(), mapping=((0, "$a"),)):
    obj = object.__new__(inplace.ObjectFile)
    obj.sections = {1: inplace.Section(1, ".text", data, len(data), False, True, False,
                                      list(relocs), list(mapping))}
    obj.symbols = symbols or [symbol(size=len(data))]
    return obj


def fn(data=BODY, mask=None, isa="arm"):
    return fingerprint.FunctionFingerprint("library_function", isa, data,
                                           mask if mask is not None else b"\xff" * len(data),
                                           len(data), 0, 0)


def catalogue(*entries):
    return {address: {"address": address, "size": size, "isa": isa}
            for address, size, isa in entries}


def write_elf(path):
    """Minimal AAELF32 fixture, independent of a compiler or the object reader."""
    strings = b"\0$a\0$d\0library_function\0external\0"
    names = b"\0.text\0.rel.text\0.symtab\0.strtab\0.shstrtab\0"
    text = struct.pack("<4I", 0xE59F0004, 0xEBFFFFFE, 0xE12FFF1E, 0)
    symbols = b"\0" * 16
    for name, value, size, info, section in ((1, 0, 0, 0, 1), (4, 12, 0, 0, 1),
                                            (7, 0, 12, 0x12, 1), (24, 0, 0, 0x10, 0)):
        symbols += struct.pack("<IIIBBH", name, value, size, info, 0, section)
    relocs = struct.pack("<4I", 4, (4 << 8) | 28, 12, (4 << 8) | 2)
    blobs = [b"", text, relocs, symbols, strings, names]
    data = bytearray(b"\0" * 52)
    offsets = []
    for blob in blobs:
        data.extend(b"\0" * (-len(data) % 4))
        offsets.append(len(data))
        data.extend(blob)
    data.extend(b"\0" * (-len(data) % 4))
    section_offset = len(data)
    headers = [(0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
               (1, 1, 6, 0, offsets[1], len(text), 0, 0, 4, 0),
               (7, 9, 0, 0, offsets[2], len(relocs), 3, 1, 4, 8),
               (17, 2, 0, 0, offsets[3], len(symbols), 4, 3, 4, 16),
               (25, 3, 0, 0, offsets[4], len(strings), 0, 0, 1, 0),
               (33, 3, 0, 0, offsets[5], len(names), 0, 0, 1, 0)]
    for header in headers:
        data.extend(struct.pack("<10I", *header))
    ident = b"\x7fELF\x01\x01\x01" + b"\0" * 9
    data[:52] = struct.pack("<16sHHIIIIIHHHHHH", ident, 1, 40, 1, 0, 0, section_offset,
                            0x05000000, 52, 0, 0, 40, len(headers), 5)
    path.write_bytes(data)


class ObjectFingerprintTest(unittest.TestCase):
    def test_real_synthetic_elf_includes_relocated_literal_pool(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "synthetic.o"
            write_elf(path)
            fingerprints = fingerprint.function_fingerprints(inplace.ObjectFile(path))
        self.assertEqual(len(fingerprints), 1)
        result = fingerprints[0]
        self.assertEqual((result.name, result.size, result.code_size, result.literal_size),
                         ("library_function", 16, 12, 4))
        self.assertEqual(result.relocation_count, 2)
        self.assertTrue(result.agrees(struct.pack("<4I", 0xE59F0004, 0xFA012345, 0xE12FFF1E, 0x234500)))
        self.assertFalse(result.agrees(struct.pack("<4I", 0xE59F0008, 0xFA012345, 0xE12FFF1E, 0x234500)))

    def test_unrelocated_literal_is_not_a_wildcard(self):
        obj = object_fixture(BODY, [symbol(size=12)], mapping=((0, "$a"), (12, "$d")))
        result = fingerprint.function_fingerprints(obj)[0]
        self.assertEqual((result.size, result.literal_size), (16, 4))
        self.assertFalse(result.agrees(BODY[:12] + b"\0" * 4))

    def test_shared_sections_and_aliases_have_separate_rows(self):
        obj = object_fixture(BODY * 2, [symbol("one", 0, 16), symbol("alias", 0, 16), symbol("two", 16, 16)])
        results = fingerprint.function_fingerprints(obj)
        self.assertEqual([(r.name, r.size) for r in results], [("one", 16), ("alias", 16), ("two", 16)])

    def test_thumb_symbol_bit_and_mapping_are_normalized(self):
        obj = object_fixture(b"\x70\x47\x00\x00", [symbol(value=1, size=2)], mapping=((0, "$t"), (2, "$d")))
        result = fingerprint.function_fingerprints(obj)[0]
        self.assertEqual((result.isa, result.code_size, result.literal_size), ("thumb", 2, 2))

    def test_mapping_before_function_supplies_mode(self):
        obj = object_fixture(BODY, [symbol(value=8, size=8)], mapping=((0, "$t"),))
        self.assertEqual(fingerprint.function_fingerprints(obj)[0].isa, "thumb")

    def test_unknown_relocation_is_not_silently_wildcarded(self):
        obj = object_fixture(relocs=[inplace.Reloc(4, 255, symbol("external"))])
        result = fingerprint.function_fingerprints(obj)[0]
        self.assertIn("unsupported relocation", result.error)
        report = fingerprint.locate_function(result, Memory(BODY), catalogue((BASE, 16, "arm")))
        self.assertEqual(report["status"], "no_hits")
        self.assertIn("unsupported", report["reason"])

    def test_relocation_crossing_boundary_is_rejected(self):
        obj = object_fixture(BODY * 2, [symbol("one", 0, 16), symbol("two", 16, 16)],
                             [inplace.Reloc(14, 2, symbol("external"))])
        self.assertTrue(all(r.error == "relocation crosses function boundary"
                            for r in fingerprint.function_fingerprints(obj)))

    def test_none_and_v4bx_preserve_instruction(self):
        obj = object_fixture(relocs=[inplace.Reloc(0, 0, symbol()), inplace.Reloc(12, 40, symbol())])
        result = fingerprint.function_fingerprints(obj)[0]
        self.assertEqual(result.mask, b"\xff" * 16)
        self.assertEqual(result.relocation_count, 0)

    def test_overlapping_symbols_fail(self):
        with self.assertRaisesRegex(fingerprint.FingerprintError, "overlapping"):
            fingerprint.function_fingerprints(object_fixture(BODY * 2, [symbol("one", 0, 20), symbol("two", 16, 16)]))

    def test_nonexecutable_function_fails(self):
        obj = object_fixture()
        obj.sections[1].executable = False
        with self.assertRaisesRegex(fingerprint.FingerprintError, "executable"):
            fingerprint.function_fingerprints(obj)

    def test_thumb_branch_masks_retain_opcode(self):
        data = bytes.fromhex("00f000f8")
        obj = object_fixture(data, [symbol(value=1, size=4)], [inplace.Reloc(0, 10, symbol())], ((0, "$t"),))
        result = fingerprint.function_fingerprints(obj)[0]
        self.assertTrue(result.agrees(bytes.fromhex("55f355e9")))
        self.assertFalse(result.agrees(bytes.fromhex("55e355e9")))

    def test_prel31_retains_high_bit(self):
        obj = object_fixture(struct.pack("<I", 0x80000000), relocs=[inplace.Reloc(0, 42, symbol())])
        result = fingerprint.function_fingerprints(obj)[0]
        self.assertTrue(result.agrees(struct.pack("<I", 0x81234567)))
        self.assertFalse(result.agrees(struct.pack("<I", 0x01234567)))


class LocateTest(unittest.TestCase):
    def test_unique_equal_size_hit_has_high_confidence(self):
        result = fingerprint.locate_function(fn(), Memory(b"\0" * 16 + BODY), catalogue((BASE + 16, 16, "arm")))
        self.assertEqual((result["status"], result["scope"], result["confidence"]), ("unique", "catalogue_size", "high"))
        self.assertEqual(result["hits"], [{"address": BASE + 16, "size": 16, "catalogue_size": 16}])

    def test_equal_size_ambiguous_hits_are_all_reported(self):
        result = fingerprint.locate_function(fn(), Memory(BODY * 2), catalogue((BASE, 16, "arm"), (BASE + 16, 16, "arm")))
        self.assertEqual((result["status"], result["confidence"]), ("ambiguous", "low"))
        self.assertEqual(len(result["hits"]), 2)

    def test_fallback_finds_uncatalogued_interior_start(self):
        result = fingerprint.locate_function(fn(), Memory(b"\0" * 4 + BODY), catalogue((BASE, 20, "arm")))
        self.assertEqual((result["status"], result["scope"], result["confidence"]), ("unique", "text", "medium"))
        self.assertEqual(result["hits"][0]["address"], BASE + 4)
        self.assertIsNone(result["hits"][0]["catalogue_size"])

    def test_first_pass_is_explicitly_not_global_uniqueness(self):
        result = fingerprint.locate_function(fn(), Memory(BODY * 2), catalogue((BASE, 16, "arm")))
        self.assertEqual(result["status"], "unique")
        self.assertEqual(result["scope"], "catalogue_size")
        self.assertEqual(len(result["hits"]), 1)

    def test_wrong_size_catalogue_start_can_be_a_fallback_hit(self):
        result = fingerprint.locate_function(fn(), Memory(BODY), catalogue((BASE, 12, "arm")))
        self.assertEqual((result["scope"], result["hits"][0]["catalogue_size"]), ("text", 12))

    def test_unaligned_arm_occurrence_does_not_match(self):
        result = fingerprint.locate_function(fn(), Memory(b"\0\0" + BODY + b"\0\0"), {})
        self.assertEqual(result["status"], "no_hits")

    def test_thumb_alignment_allows_halfword_start(self):
        result = fingerprint.locate_function(fn(isa="thumb"), Memory(b"\0\0" + BODY), {})
        self.assertEqual(result["hits"][0]["address"], BASE + 2)

    def test_no_hits_does_not_read_past_text(self):
        result = fingerprint.locate_function(fn(), Memory(BODY[:12]), catalogue((BASE, 16, "arm")))
        self.assertEqual((result["status"], result["confidence"]), ("no_hits", "none"))

    def test_all_wildcards_are_ambiguous_low_confidence(self):
        result = fingerprint.locate_function(fn(b"\0" * 4, b"\0" * 4), Memory(b"\0" * 12), {})
        self.assertEqual((result["status"], result["confidence"], result["invariant_bits"]), ("ambiguous", "low", 0))
        self.assertEqual(len(result["hits"]), 3)

    def test_partially_masked_bytes_without_anchor_are_compared(self):
        result = fingerprint.locate_function(fn(b"\x11" * 4, b"\x0f" * 4), Memory(b"\x21" * 4 + b"\x22" * 4), {})
        self.assertEqual(result["hits"][0]["address"], BASE)
        self.assertEqual(len(result["hits"]), 1)

    def test_object_report_never_contains_bytes(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "synthetic.o"
            write_elf(path)
            memory = Memory(struct.pack("<4I", 0xE59F0004, 0xEB123456, 0xE12FFF1E, 0x234500))
            cat = {"image_sha256": "synthetic", "functions": list(catalogue((BASE, 16, "arm")).values())}
            rows = fingerprint.fingerprint_object(path, memory, cat)
            self.assertEqual(rows[0]["status"], "unique")
            self.assertNotIn("data", rows[0])
            self.assertNotIn("mask", rows[0])
            json.dumps(rows)
            self.assertEqual(memory.code, struct.pack("<4I", 0xE59F0004, 0xEB123456, 0xE12FFF1E, 0x234500))

    def test_image_catalogue_hash_mismatch_fails(self):
        with self.assertRaisesRegex(fingerprint.FingerprintError, "different code.bin"):
            fingerprint.fingerprint_object(Path("unused.o"), Memory(BODY), {"image_sha256": "wrong"})

    def test_cli_compiles_with_selected_profile_includes_and_macros(self):
        profile = toolchain.Profile("game", "armcc_504_82", ("--arm",))
        with patch.object(fingerprint.toolchain, "load_profiles", return_value={"game": profile}), \
                patch.object(fingerprint.image_mod.Image, "load", return_value=Memory(BODY)), \
                patch.object(fingerprint.image_mod, "load_catalogue", return_value={}), \
                patch.object(fingerprint.toolchain, "compile_source") as compile_source, \
                patch.object(fingerprint, "fingerprint_object", return_value=[]), \
                contextlib.redirect_stdout(io.StringIO()) as stdout:
            result = fingerprint.main(["library.cpp", "--profile", "game", "-I", "vendor/include", "-D", "NW_RELEASE"])
        self.assertEqual(result, 0)
        args = compile_source.call_args.args
        self.assertEqual(args[0].flags, ("--arm", "-DNW_RELEASE"))
        self.assertEqual(args[3], (Path("vendor/include"),))
        self.assertEqual(json.loads(stdout.getvalue())["functions"], [])

    def test_unknown_profile_reports_exit_two(self):
        with patch.object(fingerprint.toolchain, "load_profiles", return_value={}), \
                contextlib.redirect_stderr(io.StringIO()) as stderr:
            result = fingerprint.main(["library.cpp", "--profile", "missing"])
        self.assertEqual(result, 2)
        self.assertIn("unknown profile", stderr.getvalue())


if __name__ == "__main__":
    unittest.main()
