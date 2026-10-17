"""Hand-encoded synthetic ARM/Thumb fixtures; no original executable bytes."""
from __future__ import annotations

from contextlib import redirect_stdout
import hashlib
import io
import json
from pathlib import Path
import struct
import subprocess
import tempfile
from types import SimpleNamespace
import unittest
from unittest import mock

from tools import analyze, catalogue_closure, ctrfmt

BASE = 0x100000
ROOT = Path(__file__).resolve().parents[1]


def branch(source: int, target: int, *, link: bool = False, condition: int = 14) -> int:
    return (condition << 28) | (0x0B000000 if link else 0x0A000000) | (((target - source - 8) >> 2) & 0xFFFFFF)


def fixture(words: list[int], *, rodata: list[int] | None = None, data: list[int] | None = None) -> tuple[bytes, ctrfmt.CodeLayout]:
    text = struct.pack(f"<{len(words)}I", *words)
    buffers = [text, struct.pack(f"<{len(rodata or [])}I", *(rodata or [])),
               struct.pack(f"<{len(data or [])}I", *(data or []))]
    exheader = bytearray(0x200)
    exheader[:8] = b"SYNTHETC"
    for offset, address, buffer in zip((0x10, 0x20, 0x30), (BASE, BASE + 0x1000, BASE + 0x2000), buffers):
        struct.pack_into("<III", exheader, offset, address, 1, len(buffer))
    image = b"".join(buffer.ljust(0x1000, b"\0") for buffer in buffers)
    return image, ctrfmt.parse_exheader(exheader)


def literal_words(value: bytes) -> list[int]:
    padded = value + b"\0" * (-len(value) % 4)
    return list(struct.unpack(f"<{len(padded) // 4}I", padded))


def walker_bounds(table: int, end: int, first_word: int) -> tuple[list[int], dict]:
    """Two text pool words whose PC-relative arithmetic yields the table bounds."""
    start_pool, end_pool = BASE + 4 * first_word, BASE + 4 * first_word + 4
    start_base, end_base = start_pool + 16, end_pool + 16
    return ([(table - start_base) & 0xFFFFFFFF, (end - end_base) & 0xFFFFFFFF],
            {"start": [start_pool, start_base], "end": [end_pool, end_base]})


# Synthetic swap/store-exclusive reproducers match the review's missing MEM WRITE
# flags; exclusive loads must also be withheld because they change reservation state.
ATOMIC_SETUPS = (
    (0xE1030091, analyze.arm.ARM_INS_SWP),
    (0xE1430091, analyze.arm.ARM_INS_SWPB),
    (0xE1830F91, analyze.arm.ARM_INS_STREX),
    (0xE1C30F91, analyze.arm.ARM_INS_STREXB),
    (0xE1E30F91, analyze.arm.ARM_INS_STREXH),
    (0xE1A30F92, analyze.arm.ARM_INS_STREXD),
    (0xE1930F9F, analyze.arm.ARM_INS_LDREX),
    (0xE1D30F9F, analyze.arm.ARM_INS_LDREXB),
    (0xE1F30F9F, analyze.arm.ARM_INS_LDREXH),
    (0xE1B30F9F, analyze.arm.ARM_INS_LDREXD),
)


class CatalogueTests(unittest.TestCase):
    def catalogue(self, words: list[int], **kwargs) -> dict:
        image, layout = fixture(words, **kwargs)
        result = analyze.build_catalogue(image, layout)
        self.assertEqual(result["image_sha256"], hashlib.sha256(image).hexdigest())
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertEqual(result["anomalies"], [])
        self.assertEqual(sum(item["end"] - item["start"] for item in result["ranges"]), len(image))
        return result

    def test_leaf_and_padding(self) -> None:
        result = self.catalogue([0xE3A00003, 0xE12FFF1E, 0, 0])
        function = result["functions"][0]
        self.assertEqual((function["size"], function["code_size"], function["pools"]), (8, 8, []))
        self.assertEqual(result["stats"]["bytes_by_range_kind"]["padding"], 8)
        self.assertEqual(result["stats"]["bytes_by_range_kind"]["segment_padding"], 0x3000 - 16)

    def test_empty_segments_keep_separate_page_padding(self) -> None:
        result = self.catalogue([0xE12FFF1E])
        padding = [(row["start"], row["end"]) for row in result["ranges"] if row["kind"] == "segment_padding"]
        self.assertEqual(padding, [(BASE + 4, BASE + 0x1000),
                                   (BASE + 0x1000, BASE + 0x2000), (BASE + 0x2000, BASE + 0x3000)])

    def test_return_forms_and_prefetch(self) -> None:
        for terminal in (0xE12FFF1E, 0xE1A0F00E, 0xE8BD8010, 0xE49DF004):
            with self.subTest(terminal=terminal):
                # PLD uses the unconditional encoding space, not a literal LDRB.
                result = self.catalogue([0xF5DFF000, terminal, 0])
                self.assertEqual(result["functions"][0]["code_size"], 8)
                self.assertEqual(result["functions"][0]["pools"], [])

    def test_trailing_literal_pool(self) -> None:
        result = self.catalogue([0xE59F0000, 0xE12FFF1E, 0x12345678, 0])
        function = result["functions"][0]
        self.assertEqual(function["size"], 12)
        self.assertEqual(function["code_size"], 8)
        self.assertEqual(function["pools"], [[BASE + 8, BASE + 12]])

    def test_vfp_double_literal_pool(self) -> None:
        result = self.catalogue([0xED9F0B00, 0xE12FFF1E, 0x12345678, 0x23456789])
        function = result["functions"][0]
        self.assertEqual(function["pools"], [[BASE + 8, BASE + 16]])
        self.assertEqual(function["size"], 16)

    def test_adr_literal_pool(self) -> None:
        result = self.catalogue([0xE28F0000, 0xE12FFF1E, 0x12345678])
        self.assertEqual(result["functions"][0]["pools"], [])
        self.assertEqual(result["functions"][0]["literals"], [[BASE + 8, BASE + 12]])

    def test_function_pool_and_complete_adr_string(self) -> None:
        result = self.catalogue([0xE28F0008, 0xE59F1000, 0xE12FFF1E, 0x12345678,
                                 *literal_words(b"format=%s\0")])
        function = result["functions"][0]
        self.assertEqual(function["code_size"], 12)
        self.assertEqual(function["size"], 28)
        self.assertEqual(function["pools"], [[BASE + 12, BASE + 16]])
        self.assertEqual(function["literals"], [[BASE + 16, BASE + 28]])
        self.assertEqual(result["stats"]["literal_extent_changes"],
                         {"extent_bytes_grown": 8, "functions_grown": 1})
        self.assertEqual(result["stats"]["bytes_by_range_kind"]["function"], 28)

    def test_all_six_strings_are_complete(self) -> None:
        payload = []
        targets = []
        for value in (b"first\0", b"second=%s\0", b"third=%s:%d\0",
                      b"fourth\0", b"fifth=%u\0", b"sixth=%s_%s\0"):
            targets.append(BASE + 36 + 4 * len(payload))
            payload.extend(literal_words(value))
        code = [0xE28F0000 | (target - (BASE + 4 * index + 8)) for index, target in enumerate(targets)]
        result = self.catalogue([*code, 0xE59F1000, 0xE12FFF1E, 0x12345678, *payload])
        function = result["functions"][0]
        self.assertEqual(function["size"], 36 + 4 * len(payload))
        self.assertEqual(function["literals"], [[BASE + 36, BASE + function["size"]]])
        self.assertEqual(result["stats"]["literal_objects_by_kind"], {"string": 6})

    def test_add_sub_and_multibyte_string_literals(self) -> None:
        for instruction in (0xE28F0000, 0xE24F0000):
            for value in (b"\0", "日本語%s\0".encode(), "日本語%s\0".encode("cp932")):
                with self.subTest(instruction=instruction, length=len(value)):
                    payload = literal_words(value)
                    result = self.catalogue([instruction, 0xE12FFF1E, *payload])
                    self.assertEqual(result["functions"][0]["literals"],
                                     [[BASE + 8, BASE + 8 + 4 * len(payload)]])
                    self.assertEqual(result["stats"]["literal_objects_by_kind"], {"string": 1})

    def test_shared_adr_string_becomes_shared_rodata(self) -> None:
        for second_target in (32, 39):
            with self.subTest(second_target=second_target):
                image, layout = fixture([branch(BASE, BASE + 16, link=True), 0xE28F0014, 0xE12FFF1E, 0,
                                         0xE28F0000 | (second_target - 24), 0xE12FFF1E, 0, 0,
                                         *literal_words(b"shared-value\0")])
                result = analyze.build_catalogue(image, layout)
                self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
                self.assertTrue(all(not function["literals"] and not function["pools"]
                                    for function in result["functions"]))
                self.assertEqual(result["literal_conflicts"], [])
                self.assertTrue(all(item["kind"] == "rodata" and item["owner"] is None
                                    for item in result["ranges"] if BASE + 32 <= item["start"] < BASE + 48))
                self.assertEqual(result["stats"]["bytes_by_range_kind"].get("unknown", 0), 0)
                self.assertEqual((result["shared_literals"][0]["start"], result["shared_literals"][0]["end"]),
                                 (BASE + 32, BASE + 48))

    def test_string_shared_through_another_functions_pool(self) -> None:
        image, layout = fixture([branch(BASE, BASE + 16, link=True), 0xE28F0014, 0xE12FFF1E, 0,
                                 0xE59F0000, 0xE12FFF1E, BASE + 32, 0, *literal_words(b"shared\0")])
        result = analyze.build_catalogue(image, layout)
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertTrue(all(not function["literals"] for function in result["functions"]))
        self.assertEqual(result["literal_conflicts"], [])
        self.assertEqual(result["shared_literals"][0]["start"], BASE + 32)
        self.assertEqual(result["functions"][1]["pools"], [[BASE + 24, BASE + 28]])

    def test_external_data_pointer_prevents_string_absorption(self) -> None:
        image, layout = fixture([0xE28F0000, 0xE12FFF1E, *literal_words(b"external\0")],
                                 rodata=[BASE + 8])
        result = analyze.build_catalogue(image, layout)
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertEqual(result["functions"][0]["literals"], [])
        self.assertEqual(result["functions"][0]["size"], 8)
        self.assertEqual(result["literal_conflicts"], [])
        self.assertEqual(result["shared_literals"][0]["start"], BASE + 8)

    def test_reviewed_shared_utf16_ideographic_whitespace_keeps_the_full_object(self) -> None:
        image, layout = fixture([0xE28F0000, 0xE12FFF1E,
                                 *literal_words("\u3000\0".encode("utf-16le"))], rodata=[BASE + 8])
        result = analyze.build_catalogue(image, layout, literal_encodings=[
            {"function": BASE, "target": BASE + 8, "references": [BASE], "encoding": "utf-16le"}])
        self.assertEqual(result["functions"][0]["size"], 8)
        self.assertEqual(result["functions"][0]["literals"], [])
        self.assertEqual(result["shared_literals"][0]["start"], BASE + 8)
        self.assertEqual(result["shared_literals"][0]["end"], BASE + 12)
        self.assertEqual(result["stats"]["bytes_by_range_kind"].get("unknown", 0), 0)
        self.assertEqual(result["anomalies"], [])

    def test_reviewed_utf16_whitespace_does_not_admit_control_data(self) -> None:
        image, layout = fixture([0xE28F0000, 0xE12FFF1E,
                                 *literal_words("\x01\0".encode("utf-16le"))], rodata=[BASE + 8])
        with self.assertRaisesRegex(ValueError, "complete printable bound"):
            analyze.build_catalogue(image, layout, literal_encodings=[
                {"function": BASE, "target": BASE + 8, "references": [BASE], "encoding": "utf-16le"}])

    def test_string_crossing_a_start_is_not_stolen(self) -> None:
        # A hard Thumb entry's synthetic BX bytes are also printable string bytes.
        image, layout = fixture([0xE28F0004, 0xFA000001, 0xE12FFF1E,
                                 *literal_words(b"ABCDpGEF\0")])
        result = analyze.build_catalogue(image, layout)
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertEqual([function["address"] for function in result["functions"]], [BASE, BASE + 16])
        self.assertTrue(all(not function["literals"] for function in result["functions"]))
        self.assertIn("address_literal_crosses_start", result["literal_conflicts"][0]["reasons"])
        self.assertEqual(result["literal_conflicts"][0]["next_start"], BASE + 16)
        self.assertLessEqual(result["functions"][0]["address"] + result["functions"][0]["size"], BASE + 16)

    def test_unaligned_string_does_not_claim_its_prefix(self) -> None:
        image, layout = fixture([0xE28F0009, 0xE12FFF1E, 0, 0, *literal_words(b"\xffhello\0")])
        result = analyze.build_catalogue(image, layout)
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertEqual(result["functions"][0]["literals"], [[BASE + 17, BASE + 24]])
        self.assertEqual(result["stats"]["bytes_by_range_kind"]["unknown"], 1)

    def test_embedded_string_ranges_are_data_not_code(self) -> None:
        image, layout = fixture([0xE28F0008, branch(BASE + 4, BASE + 24), 0, 0,
                                 *literal_words(b"foo\0"), 0, 0xE12FFF1E])
        result = analyze.build_catalogue(image, layout)
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertEqual(result["functions"][0]["literals"], [[BASE + 16, BASE + 20]])
        self.assertEqual(result["functions"][0]["code_ranges"], [[BASE, BASE + 8], [BASE + 24, BASE + 28]])

    def test_thumb_adr_string_is_complete(self) -> None:
        result = self.catalogue([0xFA000002, 0xE12FFF1E, 0, 0,
                                 0x4770A000, *literal_words(b"thumb=%s\0")])
        function = result["functions"][1]
        self.assertEqual(function["literals"], [[BASE + 20, BASE + 32]])
        self.assertEqual(function["pools"], [])
        self.assertEqual(function["code_size"], 4)

    def test_thumb_add_pc_uses_adjacent_proven_constant(self) -> None:
        result = self.catalogue([0xFA000002, 0xE12FFF1E, 0, 0,
                                 0x200046C0, 0x47704478, *literal_words(b"thumbadd\0")])
        function = result["functions"][1]
        self.assertEqual(function["literals"], [[BASE + 24, BASE + 36]])
        self.assertEqual(function["code_size"], 8)
        self.assertEqual(result["stats"]["literal_extent_changes"],
                         {"extent_bytes_grown": 12, "functions_grown": 1})

    def test_thumb_add_pc_does_not_assume_branch_entry_constant(self) -> None:
        # A conditional branch can enter ADD without executing the MOV.
        result = self.catalogue([0xFA000002, 0xE12FFF1E, 0, 0,
                                 0x2000D000, 0x47704478, *literal_words(b"not-owned\0")])
        self.assertEqual(result["functions"][1]["literals"], [])
        self.assertEqual(result["functions"][1]["size"], 8)

    def test_literal_validation_rejects_overlap_and_bad_alignment(self) -> None:
        image, layout = fixture([0xE28F0000, 0xE12FFF1E, *literal_words(b"hello\0")])
        for literal, issue in (([BASE + 8, BASE + 15], "invalid_literal_range"),
                               ([BASE + 4, BASE + 16], "literal_overlaps_code")):
            with self.subTest(issue=issue):
                result = analyze.build_catalogue(image, layout)
                result["functions"][0]["literals"] = [literal]
                self.assertTrue(any(item["issue"] == issue
                                    for item in analyze.validate_catalogue(result, image, layout)))

    def test_bl_to_following_function(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 16, link=True), 0xE12FFF1E, 0, 0, 0xE3A00004, 0xE12FFF1E])
        self.assertEqual([function["address"] for function in result["functions"]], [BASE, BASE + 16])
        self.assertIn("bl_target", result["functions"][1]["evidence"])
        self.assertEqual(result["stats"]["direct_calls"], 1)

    def test_tail_call_branch(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 16), 0, 0, 0, 0xE3A00004, 0xE12FFF1E])
        self.assertEqual([function["address"] for function in result["functions"]], [BASE, BASE + 16])
        self.assertEqual(result["functions"][0]["size"], 4)
        self.assertIn("tail_call", result["functions"][1]["evidence"])

    def test_tail_call_after_setup_and_padding(self) -> None:
        result = self.catalogue([0xE3A00001, branch(BASE + 4, BASE + 16),
                                 0, 0, 0xE3A00004, 0xE12FFF1E])
        self.assertEqual([function["address"] for function in result["functions"]], [BASE, BASE + 16])
        self.assertEqual(result["functions"][0]["size"], 8)

    def test_tail_call_after_single_lr_restore(self) -> None:
        result = self.catalogue([0xE52DE004, 0xE3A00001, 0xE49DE004,
                                 branch(BASE + 12, BASE + 16), 0xE3A00004, 0xE12FFF1E])
        self.assertEqual([function["address"] for function in result["functions"]], [BASE, BASE + 16])
        self.assertEqual(result["functions"][0]["size"], 16)

    def test_linker_nop_entry_and_body_are_separate_functions(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 16, link=True), branch(BASE + 4, BASE + 20, link=True),
                                 0xE12FFF1E, 0, 0xE1A00000, 0xE3A00001, 0xE12FFF1E])
        wrapper, body = result["functions"][1:]
        self.assertEqual((wrapper["address"], wrapper["size"], wrapper["kind"]),
                         (BASE + 16, 4, "function"))
        self.assertEqual((body["address"], body["size"], body["kind"]),
                         (BASE + 20, 8, "function"))
        self.assertEqual(wrapper["entries"][0]["sources"], [BASE])
        self.assertEqual(body["entries"][0]["sources"], [BASE + 4])
        self.assertEqual(result["stats"]["nop_entries"], 0)
        self.assertEqual(result["entry_groups"], [])
        self.assertEqual(result["linker_tail_splits"],
                         [{"address": BASE + 16, "target": BASE + 20, "isa": "arm"}])

    def test_leading_linker_nop_splits_without_a_body_pointer(self) -> None:
        result = self.catalogue([0xE1A00000, 0xE59F0000, 0xE12FFF1E, 0x12345678])
        wrapper, body = result["functions"]
        self.assertEqual((wrapper["size"], wrapper["kind"]), (4, "function"))
        self.assertEqual((body["address"], body["size"], body["kind"]), (BASE + 4, 12, "function"))
        self.assertEqual(body["pools"], [[BASE + 12, BASE + 16]])
        self.assertEqual(body["evidence"], ["linker_branchnop"])
        self.assertEqual(result["stats"]["linker_tail_wrappers"], 1)

    def test_linker_nop_restores_a_merged_weak_body_start(self) -> None:
        image, layout = fixture([0xE1A00000, 0xE92D4010, 0xE8BD8010], rodata=[BASE + 4])
        analyzer = analyze.Analyzer(image, layout)
        analyzer.add_seed(BASE, "arm", "entry")
        analyzer.add_seed(BASE + 4, "arm", "pointer")
        traces = analyzer.group_fallthroughs(analyzer.refine())
        self.assertEqual(len(analyzer.boundary_merges), 1)
        analyzer.record_entries(traces)
        self.assertTrue(analyzer.promote_linker_entries(traces))
        traces = analyzer.group_fallthroughs(analyzer.refine())
        self.assertEqual([trace.address for trace in traces], [BASE, BASE + 4])
        self.assertEqual(analyzer.boundary_merges, [])
        self.assertEqual(analyzer.seeds[BASE + 4].evidence, {"linker_branchnop", "pointer"})

    def test_pointer_referenced_linker_nop_padding_before_a_long_body(self) -> None:
        long_body = [0xE2800001] * 40 + [0xE12FFF1E]
        for segment in ("rodata", "data", "pool"):
            with self.subTest(segment=segment):
                if segment == "pool":
                    words = [0xE59F0004, branch(BASE + 4, BASE + 20, link=True), 0xE12FFF1E,
                             BASE + 16, 0xE1A00000, *long_body]
                    result = self.catalogue(words)
                    address = BASE + 16
                else:
                    words = [branch(BASE, BASE + 16, link=True), 0xE12FFF1E, 0, 0xE1A00000, *long_body]
                    result = self.catalogue(words, **{segment: [BASE + 12]})
                    address = BASE + 12
                functions = {function["address"]: function for function in result["functions"]}
                self.assertEqual((functions[address]["size"], functions[address]["kind"]), (4, "function"))
                self.assertIn("pointer", functions[address]["evidence"])
                self.assertEqual(functions[address + 4]["size"], 164)
                self.assertEqual(functions[address + 4]["kind"], "function")

    def test_pointer_tail_chain_discovers_unseeded_leaf(self) -> None:
        result = self.catalogue([0xE12FFF1E, 0, 0xE3A00001, branch(BASE + 12, BASE + 24),
                                 0, 0, 0xE3A00002, 0xE12FFF1E], rodata=[BASE + 8])
        image, layout = fixture([0xE12FFF1E, 0, 0xE3A00001, branch(BASE + 12, BASE + 24),
                                 0, 0, 0xE3A00002, 0xE12FFF1E], rodata=[BASE + 8])
        probe = analyze.Analyzer(image, layout)
        self.assertFalse(probe.pointer_plausible(BASE + 8, "arm"))
        self.assertTrue(probe.pointer_plausible(BASE + 8, "arm", follow_tail_branches=True))
        functions = {function["address"]: function for function in result["functions"]}
        self.assertEqual(set(functions), {BASE, BASE + 8, BASE + 24})
        self.assertEqual((functions[BASE + 8]["kind"], functions[BASE + 8]["size"]), ("function", 8))
        self.assertIn("pointer", functions[BASE + 8]["evidence"])
        self.assertIn("tail_call", functions[BASE + 24]["evidence"])

    def test_late_pointer_probe_does_not_fall_into_a_published_body(self) -> None:
        image, layout = fixture([0xE12FFF1E, 0, 0xE3A00001, 0xE3A00002, 0xE12FFF1E],
                                 rodata=[BASE + 8])
        analyzer = analyze.Analyzer(image, layout)
        analyzer.add_seed(BASE, "arm", "entry")
        analyzer.add_seed(BASE + 12, "arm", "bl_target")
        traces = analyzer.group_fallthroughs(analyzer.refine())
        analyzer.record_entries(traces)
        self.assertFalse(analyzer.promote_pointer_entries(traces))
        self.assertEqual(set(analyzer.seeds), {BASE, BASE + 12})

    def test_unproved_pointer_tail_cycles_remain_unknown(self) -> None:
        for target in (BASE + 8, BASE + 16):
            with self.subTest(target=target):
                result = self.catalogue([0xE12FFF1E, 0, 0xE3A00001, branch(BASE + 12, target), 0],
                                        rodata=[BASE + 8])
                self.assertEqual([function["address"] for function in result["functions"]], [BASE])
                self.assertEqual(result["stats"]["starts_by_evidence"], {"entry": 1})

    def test_pointer_only_empty_body_hidden_after_linker_nop(self) -> None:
        for terminal in (0xE12FFF1E, 0xE1A0F00E):
            with self.subTest(terminal=terminal):
                result = self.catalogue([0xE1A00000, terminal], rodata=[BASE + 4])
                wrapper, body = result["functions"]
                self.assertEqual((wrapper["size"], body["size"]), (4, 4))
                self.assertEqual((wrapper["kind"], body["kind"]), ("function", "function"))
                self.assertIn("pointer", body["evidence"])

    def test_linker_nop_chain_splits_each_wrapper(self) -> None:
        result = self.catalogue([0xE1A00000, 0xE1A00000, 0xE12FFF1E])
        self.assertEqual([function["size"] for function in result["functions"]], [4, 4, 4])
        self.assertEqual(result["stats"]["linker_tail_wrappers"], 2)

    def test_linker_nop_does_not_erase_other_multi_entry_evidence(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 16, link=True), branch(BASE + 4, BASE + 24, link=True),
                                 0xE12FFF1E, 0, 0xE1A00000, 0xE3A00001, 0xE12FFF1E])
        wrapper, body = result["functions"][1:]
        self.assertEqual((wrapper["address"], wrapper["kind"], wrapper["size"]), (BASE + 16, "function", 4))
        self.assertEqual((body["address"], body["kind"]), (BASE + 20, "multi_entry"))
        self.assertEqual([entry["address"] for entry in body["entries"]], [BASE + 20, BASE + 24])

    def test_secondary_nop_inside_a_body_does_not_establish_a_wrapper(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 16, link=True), branch(BASE + 4, BASE + 20),
                                 0, 0, 0xE3A00001, 0xE1A00000, 0xE12FFF1E])
        body = result["functions"][1]
        self.assertEqual((body["address"], body["kind"], body["size"]), (BASE + 16, "multi_entry", 12))
        self.assertEqual([entry["address"] for entry in body["entries"]], [BASE + 16, BASE + 20])
        self.assertEqual(result["linker_tail_splits"], [])

    def test_architectural_nop_fallthrough_stays_multi_entry(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 16, link=True), branch(BASE + 4, BASE + 20, link=True),
                                 0xE12FFF1E, 0, 0xE320F000, 0xE3A00001, 0xE12FFF1E])
        self.assertEqual(result["functions"][1]["kind"], "multi_entry")
        self.assertEqual(result["stats"]["nop_entries"], 1)
        self.assertEqual(result["linker_tail_splits"], [])

    def test_nonentry_and_unreferenced_linker_nops_are_not_split(self) -> None:
        for words in ([0xE92D4010, 0xE1A00000, 0xE8BD8010],
                      [0xE12FFF1E, 0xE1A00000, 0xE12FFF1E],
                      [0x01A00000, 0xE12FFF1E]):
            with self.subTest(words=words):
                result = self.catalogue(words)
                self.assertEqual(len(result["functions"]), 1)
                self.assertEqual(result["linker_tail_splits"], [])

    def test_tail_nop_after_epilogue_splits_before_a_prologue(self) -> None:
        result = self.catalogue([0xE92D4010, 0xE1A04000, branch(BASE + 8, BASE + 40, link=True),
                                 0xE1A00004, 0xE8BD4010, 0xE1A00000,
                                 0xE92D4010, 0xE3A00002, 0xE8BD8010, 0, 0xE12FFF1E])
        functions = {function["address"]: function for function in result["functions"]}
        self.assertEqual((functions[BASE]["kind"], functions[BASE]["size"]), ("function", 24))
        self.assertEqual((functions[BASE + 24]["kind"], functions[BASE + 24]["size"]), ("function", 12))
        self.assertEqual(result["linker_tail_extent_splits"][0]["site"], BASE + 20)
        self.assertIn("prologue", result["linker_tail_extent_splits"][0]["start_evidence"])

    def test_tail_nop_group_becomes_two_ordinary_functions(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 16, link=True), branch(BASE + 4, BASE + 24, link=True),
                                 0xE12FFF1E, 0, 0xE3A00001, 0xE1A00000,
                                 0xE92D4010, 0xE3A00002, 0xE8BD8010])
        head, tail = result["functions"][1:]
        self.assertEqual((head["address"], head["size"], head["kind"]), (BASE + 16, 8, "function"))
        self.assertEqual((tail["address"], tail["size"], tail["kind"]), (BASE + 24, 12, "function"))
        self.assertEqual(head["entries"][0]["sources"], [BASE])
        self.assertEqual(tail["entries"][0]["sources"], [BASE + 4])
        self.assertEqual(result["entry_groups"], [])

    def test_tail_nop_live_arguments_prove_a_leaf_successor(self) -> None:
        result = self.catalogue([0xE3A00001, 0xE1A00000, 0xE0800001, 0xE12FFF1E])
        self.assertEqual([function["size"] for function in result["functions"]], [8, 8])
        self.assertIn("aapcs_live_arguments", result["linker_tail_extent_splits"][0]["start_evidence"])

    def test_nonstoring_wrapper_splits_a_complete_field_getter(self) -> None:
        for load in (0xE5900004, 0xE5D00004, 0xE1D000B4, 0xE1D000D4, 0xE1D000F4):
            for terminal in (0xE12FFF1E, 0xE1A0F00E):
                with self.subTest(load=load, terminal=terminal):
                    result = self.catalogue([0xE5900008, 0xE1A00000, load, terminal])
                    self.assertEqual([function["size"] for function in result["functions"]], [8, 8])
                    self.assertEqual(result["linker_tail_extent_splits"][0]["start_evidence"],
                                     ["nonstoring_wrapper_field_read"])

    def test_field_getter_may_tail_to_an_independent_returning_owner(self) -> None:
        result = self.catalogue([0xE5900008, 0xE1A00000, 0xE5900004,
                                 branch(BASE + 12, BASE + 20), 0, 0xE12FFF1E])
        self.assertEqual([function["size"] for function in result["functions"]], [8, 8, 4])
        self.assertEqual(result["linker_tail_extent_splits"][0]["start_evidence"],
                         ["nonstoring_wrapper_field_read"])

    def test_field_getter_does_not_tail_to_a_noreturn_owner(self) -> None:
        result = self.catalogue([0xE5900008, 0xE1A00000, 0xE5900004,
                                 branch(BASE + 12, BASE + 20), 0, 0xEF000003])
        self.assertEqual(result["linker_tail_extent_splits"], [])

    def test_field_getter_requires_unconditional_scalar_read_and_plain_return(self) -> None:
        for suffix in (
            [0xE4900004, 0xE12FFF1E],  # Writeback.
            [0x15900004, 0xE12FFF1E],  # Conditional read.
            [0xE5940004, 0xE12FFF1E],  # Not an argument-object field.
            [0xE5800004, 0xE12FFF1E],  # Store, not a read.
            [0xE5900004, 0xE49DF004],  # Stack-dependent return.
            [0xE5900004, 0x012FFF1E],  # Conditional return.
            [0xE5900004, 0xE2800001, 0xE12FFF1E],  # Arbitrary longer body.
        ):
            with self.subTest(suffix=suffix):
                image, layout = fixture([0xE5900008, 0xE1A00000, *suffix])
                result = analyze.build_catalogue(image, layout)
                self.assertEqual(result["linker_tail_extent_splits"], [])
                self.assertNotIn("nonstoring_wrapper_field_read",
                                 result["linker_tail_inventory"][0]["start_evidence"])

    def test_field_getter_requires_nonstoring_stack_and_lr_independent_prefix(self) -> None:
        for prefix in ([0xE5801000], [0xE59D0000], [0xE1A0E000], [0x15900008],
                       *([word] for word, _ in ATOMIC_SETUPS)):
            with self.subTest(prefix=prefix):
                result = self.catalogue([*prefix, 0xE1A00000, 0xE5900004, 0xE12FFF1E])
                self.assertEqual(result["linker_tail_extent_splits"], [])

    def test_field_getter_does_not_override_crossing_control_or_pool_ownership(self) -> None:
        for words, decision in (
            ([branch(BASE, BASE + 12, condition=0), 0xE5900008, 0xE1A00000,
              0xE5900004, 0xE12FFF1E], "control_flow_crosses"),
            ([0xE59F0008, 0xE1A00000, 0xE5900004, 0xE12FFF1E, 0x12345678],
             "literal_ownership_crosses"),
        ):
            with self.subTest(decision=decision):
                image, layout = fixture(words)
                result = analyze.build_catalogue(image, layout)
                self.assertEqual(result["linker_tail_extent_splits"], [])
                self.assertEqual(result["linker_tail_inventory"][0]["decision"], decision)

    def test_tail_nop_vfp_scalar_and_register_lists_are_live_arguments(self) -> None:
        for suffix in (
            [0xED800A00, 0xE12FFF1E],  # VSTR S0,[R0].
            [0xE5901000, 0xEC810A02, 0xE12FFF1E],  # VSTMIA R1,{S0,S1}.
            [0xE5901000, 0xEC810A03, 0xE12FFF1E],  # Three pass-through floats.
            [0xE5901000, 0xEC810B04, 0xE12FFF1E],  # VSTMIA R1,{D0,D1}.
            [0xEDC07A00, 0xE12FFF1E],  # Last single argument lane S15.
            [0xED807B00, 0xE12FFF1E],  # Last double argument register D7.
            [0xED840A00, 0xE5900000, 0xE12FFF1E],  # VFP use before the R0 use.
        ):
            with self.subTest(suffix=suffix):
                result = self.catalogue([0xE3A00001, 0xE1A00000, *suffix])
                self.assertEqual([function["size"] for function in result["functions"]], [8, 4 * len(suffix)])
                row = result["linker_tail_extent_splits"][0]
                self.assertIn("aapcs_live_arguments", row["start_evidence"])
                self.assertTrue(any(name.startswith("s") for name in row["live_argument_registers"]))

    def test_vfp_return_values_and_callee_saved_lanes_are_not_argument_witnesses(self) -> None:
        for suffix in (
            [0xED840A00, 0xE12FFF1E],  # S0 alone may be a scalar result.
            [0xEC840A04, 0xE12FFF1E],  # S0-S3 may be an HFA result.
            [0xEC840B08, 0xE12FFF1E],  # D0-D3 may be an HFA result.
            [0xED808A00, 0xE12FFF1E],  # S16 is callee-saved.
            [0xED808B00, 0xE12FFF1E],  # D8 is callee-saved.
            [0xE3A00001, 0xED800A00, 0xE12FFF1E],  # R0 is initialized after the call.
        ):
            with self.subTest(suffix=suffix):
                result = self.catalogue([0xE3A00001, 0xE1A00000, *suffix])
                self.assertEqual(result["linker_tail_extent_splits"], [])
                self.assertEqual(result["linker_tail_inventory"][0]["as_if_call"], "consistent")

    def test_vfp_alias_writes_and_load_lists_initialize_exact_lanes(self) -> None:
        for load, store, split in (
            (0xED900B00, 0xED800A00, False),  # D0 writes both S0 and S1.
            (0xED900A00, 0xED800B00, True),  # S0 does not initialize all of D0.
            (0xEC900A02, 0xEC800A02, False),  # VLDMIA writes both list registers.
            (0xEC900A01, 0xEC800A02, True),  # Loading S0 leaves S1 live.
        ):
            with self.subTest(load=load, store=store):
                result = self.catalogue([0xE3A00001, 0xE1A00000, load, store, 0xE12FFF1E])
                self.assertEqual(bool(result["linker_tail_extent_splits"]), split)

    def test_live_vfp_arguments_do_not_override_a_saved_lr(self) -> None:
        result = self.catalogue([0xE92D4010, 0xE1A00000, 0xED800A00, 0xE8BD8010])
        self.assertEqual(result["linker_tail_extent_splits"], [])
        self.assertEqual(len(result["functions"]), 1)

    def test_nop_census_keeps_non_tail_words_without_splitting_them(self) -> None:
        result = self.catalogue([0xE92D4010, 0xE1A00000, 0xED800A00, 0xE8BD8010])
        self.assertEqual(len(result["linker_nop_inventory"]), 1)
        row = result["linker_nop_inventory"][0]
        self.assertEqual((row["census_class"], row["decision"]), ("call:in_frame", "in_frame_not_tail"))
        self.assertEqual(row["as_if_call"], "reads_args")
        self.assertTrue(row["lr_saved"])
        self.assertEqual(result["linker_tail_inventory"], [])

    def test_live_vfp_arguments_do_not_override_crossing_control_or_literals(self) -> None:
        for words, decision in (
            ([branch(BASE, BASE + 12, condition=0), 0xE3A00001, 0xE1A00000,
              0xED800A00, 0xE12FFF1E], "control_flow_crosses"),
            ([0xE59F2008, 0xE1A00000, 0xED800A00, 0xE12FFF1E, 0x12345678],
             "literal_ownership_crosses"),
        ):
            with self.subTest(decision=decision):
                result = self.catalogue(words)
                self.assertEqual(result["linker_tail_extent_splits"], [])
                row = result["linker_tail_inventory"][0]
                self.assertIn("aapcs_live_arguments", row["start_evidence"])
                self.assertEqual(row["decision"], decision)

    def test_flags_only_dependency_is_not_a_boundary_witness(self) -> None:
        result = self.catalogue([0xE3500000, 0xE1A00000, 0x12800001, 0xE12FFF1E])
        self.assertEqual(result["linker_tail_extent_splits"], [])
        self.assertEqual(result["linker_tail_inventory"][0]["as_if_call"], "reads_flags")

    def test_restored_frame_proves_short_leaf_setters(self) -> None:
        for store in (0xE5801000, 0xE5C01000, 0xE1C010B0):
            with self.subTest(store=store):
                result = self.catalogue([0xE92D4010, 0xE8BD4010, 0xE1A00000,
                                         0xE3A01001, store, 0xE12FFF1E])
                self.assertEqual([function["size"] for function in result["functions"]], [12, 12])
                row = result["linker_tail_extent_splits"][0]
                self.assertEqual(row["restored_frame_at"], BASE + 4)
                self.assertEqual(row["start_evidence"], ["restored_frame_leaf_setter"])

    def test_post_epilogue_nop_outside_the_frozen_census_is_considered(self) -> None:
        # A non-writeback LDR restores the actual saved LR slot; a later ADD
        # balances SP. The census only understands POP/LDM and misses this tail.
        result = self.catalogue([0xE92D4010, 0xE59DE004, 0xE28DD008, 0xE1A00000,
                                 0xE3A01001, 0xE5801000, 0xE12FFF1E])
        self.assertEqual([function["size"] for function in result["functions"]], [16, 12])
        row = result["linker_tail_extent_splits"][0]
        self.assertEqual(row["census_class"], "call:in_frame")
        self.assertTrue(row["census_lr_saved"])
        self.assertFalse(row["lr_saved"])
        self.assertEqual(row["restored_frame_at"], BASE + 4)
        self.assertEqual(row["start_evidence"], ["restored_frame_leaf_setter"])

    def test_leaf_setter_needs_a_genuine_balanced_lr_restore(self) -> None:
        for prefix in (
            [0xE3A00001],  # A setter shape alone is not a section witness.
            [0xE92D4010, 0x08BD4010],  # A conditional restore is insufficient.
            [0xE92D4010, 0xE59DE000, 0xE28DD008],  # Wrong stack slot.
            [0xE92D4010, 0xE59DE004],  # LR loaded, but SP remains in the frame.
            [0xE92D4010, 0xE8BD4010, 0xE1A0E000],  # Restored LR overwritten.
        ):
            with self.subTest(prefix=prefix):
                result = self.catalogue([*prefix, 0xE1A00000, 0xE3A01001, 0xE5801000, 0xE12FFF1E])
                self.assertEqual(result["linker_tail_extent_splits"], [])
                self.assertEqual(len(result["functions"]), 1)

    def test_restored_frame_does_not_make_an_arbitrary_suffix_a_setter(self) -> None:
        for suffix in (
            [0xE2800001, 0xE12FFF1E],  # No field store.
            [0xE3A01001, 0x05801000, 0xE12FFF1E],  # Conditional store.
            [0xE3A01001, 0xE5841000, 0xE12FFF1E],  # Non-argument base register.
            [0xE3A01001, 0xE5801000, 0xE59DE000, 0xE12FFF1E],  # Stack dependency.
            [0xE3A01001, 0xE5801000, *([0xE3A01001] * 10), 0xE12FFF1E],  # Beyond the probe bound.
        ):
            with self.subTest(suffix=suffix):
                result = self.catalogue([0xE92D4010, 0xE8BD4010, 0xE1A00000, *suffix])
                self.assertEqual(result["linker_tail_extent_splits"], [])
                self.assertNotIn("restored_frame_leaf_setter", result["linker_tail_inventory"][0]["start_evidence"])

    def test_restored_frame_setter_still_rejects_crossing_branches(self) -> None:
        result = self.catalogue([0xE92D4010, branch(BASE + 4, BASE + 20, condition=0),
                                 0xE8BD4010, 0xE1A00000, 0xE3A01001, 0xE5801000, 0xE12FFF1E])
        self.assertEqual(result["linker_tail_extent_splits"], [])
        row = result["linker_tail_inventory"][0]
        self.assertIn("restored_frame_leaf_setter", row["start_evidence"])
        self.assertEqual(row["decision"], "control_flow_crosses")

    def test_new_tail_evidence_keeps_protected_owners_and_adopted_splits(self) -> None:
        for words in (
            [0xE3A00001, 0xE1A00000, 0xED800A00, 0xE12FFF1E],
            [0xE92D4010, 0xE8BD4010, 0xE1A00000, 0xE3A01001, 0xE5801000, 0xE12FFF1E],
            [0xE5900008, 0xE1A00000, 0xE5900004, 0xE12FFF1E],
        ):
            with self.subTest(words=words):
                image, layout = fixture(words)
                split = analyze.build_catalogue(image, layout)
                published = dict(split["functions"][0], size=4 * len(words), code_size=4 * len(words),
                                 code_ranges=[[BASE, BASE + 4 * len(words)]])
                protected = analyze.build_catalogue(image, layout, protected_functions={BASE: published})
                self.assertEqual(protected["functions"], [published])
                self.assertEqual(protected["linker_tail_extent_splits"], [])
                self.assertEqual(protected["linker_tail_inventory"][0]["decision"], "protected_owner")
                adopted = analyze.build_catalogue(
                    image, layout, protected_functions={function["address"]: function for function in split["functions"]})
                self.assertEqual(analyze.serialize_catalogue(adopted), analyze.serialize_catalogue(split))

    def test_tail_nop_without_successor_evidence_is_retained(self) -> None:
        result = self.catalogue([0xE3A00001, 0xE1A00000, 0xE2800002, 0xE12FFF1E])
        self.assertEqual([function["size"] for function in result["functions"]], [16])
        self.assertEqual(result["linker_tail_inventory"][0]["decision"], "no_independent_successor_witness")

    def test_tail_census_lr_base_does_not_discard_a_saved_stack_lr(self) -> None:
        for middle, category in (([], "tail:after_epilogue"), ([0xE3A00001], "tail:in_leaf")):
            with self.subTest(category=category):
                result = self.catalogue([0xE92D4010, 0xE89E0001, *middle,
                                         0xE1A00000, 0xE1A00001, 0xE8BD8010])
                self.assertEqual(result["linker_tail_extent_splits"], [])
                row = result["linker_tail_inventory"][0]
                self.assertEqual(row["census_class"], category)
                self.assertFalse(row["census_lr_saved"])
                self.assertTrue(row["lr_saved"])
                self.assertEqual(row["decision"], "frame_not_restored")

    def test_tail_nop_before_an_empty_successor_is_split(self) -> None:
        result = self.catalogue([0xE3A00001, 0xE1A00000, 0xE12FFF1E])
        self.assertEqual([function["size"] for function in result["functions"]], [8, 4])
        self.assertIn("return_only_successor", result["linker_tail_extent_splits"][0]["start_evidence"])

    def test_tail_nop_crossing_branches_are_not_split(self) -> None:
        for words in (
            [branch(BASE, BASE + 16, condition=0), 0xE3A00001, 0xE1A00000,
             0xE92D4010, 0xE3A00002, 0xE8BD8010],
            [0xE3A00001, 0xE1A00000, 0xE92D4010, branch(BASE + 12, BASE, condition=0), 0xE8BD8010],
        ):
            with self.subTest(words=words):
                result = self.catalogue(words)
                self.assertEqual(result["linker_tail_extent_splits"], [])
                self.assertEqual(result["linker_tail_inventory"][0]["decision"], "control_flow_crosses")

    def test_tail_nop_switch_edges_are_not_split(self) -> None:
        result = self.catalogue([0xE3500001, 0x979FF100, branch(BASE + 8, BASE + 20),
                                 BASE + 20, BASE + 28, 0xE3A00001, 0xE1A00000,
                                 0xE92D4010, 0xE3A00002, 0xE8BD8010])
        self.assertEqual(result["linker_tail_extent_splits"], [])
        row = result["linker_tail_inventory"][0]
        self.assertEqual(row["decision"], "control_flow_crosses")
        self.assertTrue(any("table_word" in edge for edge in row["crossing_branches"]))

    def test_tail_nop_crossing_literal_ownership_is_not_split(self) -> None:
        result = self.catalogue([0xE59F000C, 0xE1A00000, 0xE92D4010, 0xE3A00002, 0xE8BD8010, 0x12345678])
        self.assertEqual(result["linker_tail_extent_splits"], [])
        self.assertEqual(result["linker_tail_inventory"][0]["decision"], "literal_ownership_crosses")

    def test_tail_nop_extra_interior_entries_remain_multi_entry(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 16, link=True), branch(BASE + 4, BASE + 28, link=True),
                                 0xE12FFF1E, 0, 0xE3A00001, 0xE1A00000,
                                 0xE92D4010, 0xE3A00002, 0xE8BD8010])
        self.assertEqual(result["functions"][1]["kind"], "multi_entry")
        self.assertEqual(result["linker_tail_extent_splits"], [])
        self.assertEqual(result["linker_tail_inventory"][0]["decision"], "additional_interior_entries")

    def test_tail_nop_protected_owner_is_not_changed(self) -> None:
        image, layout = fixture([0xE3A00001, 0xE1A00000, 0xE12FFF1E])
        split = analyze.build_catalogue(image, layout)
        published = dict(split["functions"][0], size=12, code_size=12, code_ranges=[[BASE, BASE + 12]])
        result = analyze.build_catalogue(image, layout, protected_functions={BASE: published})
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertEqual((result["functions"][0]["kind"], result["functions"][0]["size"]), ("function", 12))
        self.assertEqual(result["linker_tail_extent_splits"], [])
        self.assertEqual(result["linker_tail_inventory"][0]["decision"], "protected_owner")

    def test_tail_nop_protected_secondary_entry_keeps_the_group(self) -> None:
        image, layout = fixture([branch(BASE, BASE + 16, link=True), branch(BASE + 4, BASE + 24, link=True),
                                 0xE12FFF1E, 0, 0xE3A00001, 0xE1A00000,
                                 0xE92D4010, 0xE3A00002, 0xE8BD8010])
        split = analyze.build_catalogue(image, layout)
        head, tail = split["functions"][1:]
        published = dict(head, size=20, code_size=20, kind="multi_entry",
                         code_ranges=[[BASE + 16, BASE + 36]],
                         entries=[head["entries"][0], dict(tail["entries"][0], evidence=["bl_target"])])
        result = analyze.build_catalogue(image, layout, protected_functions={BASE + 24: published})
        self.assertEqual(result["functions"][1]["kind"], "multi_entry")
        self.assertEqual(result["linker_tail_extent_splits"], [])
        self.assertEqual(result["linker_tail_inventory"][0]["decision"], "protected_owner")

    def test_adopted_tail_split_is_reproduced_when_its_parts_become_protected(self) -> None:
        image, layout = fixture([0xE3A00001, 0xE1A00000, 0xE12FFF1E])
        adopted = analyze.build_catalogue(image, layout)
        protected = {function["address"]: function for function in adopted["functions"]}
        regenerated = analyze.build_catalogue(image, layout, protected_functions=protected)
        self.assertEqual(analyze.serialize_catalogue(regenerated), analyze.serialize_catalogue(adopted))
        self.assertEqual(len(regenerated["linker_tail_extent_splits"]), 1)

    def test_external_body_branch_records_secondary_entry(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 16, link=True), branch(BASE + 4, BASE + 20),
                                 0, 0, 0xE3A00001, 0xE12FFF1E])
        function = result["functions"][1]
        self.assertEqual(function["kind"], "multi_entry")
        entry = function["entries"][1]
        self.assertEqual(entry, {"address": BASE + 20, "isa": "arm", "kind": "secondary_entry",
                                 "evidence": ["tail_call"], "sources": [BASE + 4]})
        self.assertEqual(result["stats"]["secondary_entries"], 1)
        self.assertEqual(result["entry_groups"], [])

    def test_bl_target_can_be_an_explicit_group_entry(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 16, link=True), branch(BASE + 4, BASE + 20, link=True),
                                 0xE12FFF1E, 0, 0xE3A00001, 0xE12FFF1E])
        self.assertEqual([function["address"] for function in result["functions"]], [BASE, BASE + 16])
        self.assertEqual(result["functions"][1]["kind"], "multi_entry")
        self.assertEqual(result["functions"][1]["entries"][1]["address"], BASE + 20)
        self.assertIn("bl_target", result["functions"][1]["entries"][1]["evidence"])

    def test_conditional_return_fallthrough_is_non_c(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 16, link=True), branch(BASE + 4, BASE + 20, link=True),
                                 0xE12FFF1E, 0, 0x012FFF1E, 0xE12FFF1E])
        self.assertEqual(result["functions"][1]["kind"], "multi_entry")
        self.assertEqual(result["functions"][1]["size"], 8)
        self.assertEqual(result["stats"]["absorbed_function_starts"], 1)

    def test_internal_branch_labels_are_not_secondary_entries(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 8, condition=0), 0xE12FFF1E, 0xE12FFF1E])
        self.assertEqual(result["functions"][0]["kind"], "function")
        self.assertEqual(len(result["functions"][0]["entries"]), 1)
        self.assertEqual(result["stats"]["secondary_entries"], 0)

    def test_tail_target_hidden_by_address_word_is_repaired(self) -> None:
        image, layout = fixture([branch(BASE, BASE + 16, link=True), branch(BASE + 4, BASE + 32), 0, 0,
                                 0xE28F0008, 0xE12FFF1E, 0, 0, 0xE12FFF1E])
        result = analyze.build_catalogue(image, layout)
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertEqual([function["address"] for function in result["functions"]],
                         [BASE, BASE + 16, BASE + 32])
        self.assertIn("tail_call", result["functions"][-1]["evidence"])
        self.assertEqual(result["tail_target_repairs"],
                         [{"address": BASE + 32, "isa": "arm", "sources": [BASE + 4]}])

    def test_secondary_entries_are_validated_not_assumed(self) -> None:
        image, layout = fixture([branch(BASE, BASE + 16, link=True), branch(BASE + 4, BASE + 20),
                                 0, 0, 0xE3A00001, 0xE12FFF1E])
        for mutation, issue in (("remove", "branch_target_not_entry"),
                                ("isa", "invalid_entry_point"),
                                ("kind", "invalid_secondary_entry_kind")):
            with self.subTest(mutation=mutation):
                result = analyze.build_catalogue(image, layout)
                function = result["functions"][1]
                if mutation == "remove":
                    function["entries"].pop()
                elif mutation == "isa":
                    function["entries"][1]["isa"] = "thumb"
                else:
                    function["kind"] = "function"
                self.assertTrue(any(item["issue"] == issue
                                    for item in analyze.validate_catalogue(result, image, layout)))

    def test_legacy_catalogue_without_entries_still_validates(self) -> None:
        image, layout = fixture([branch(BASE, BASE + 16, link=True), 0xE12FFF1E, 0, 0,
                                 0xE3A00001, 0xE12FFF1E])
        result = analyze.build_catalogue(image, layout)
        for function in result["functions"]:
            function.pop("entries")
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])

    def test_mid_function_pool_merges_tail_only_start(self) -> None:
        # MOV; B over a zero-valued pool; LDR back from that pool; BX lr.
        # The initial padding-gap heuristic mistakes the B for a tail call.
        result = self.catalogue([0xE3A00001, branch(BASE + 4, BASE + 12),
                                 0, 0xE51F000C, 0xE12FFF1E])
        self.assertEqual([function["address"] for function in result["functions"]], [BASE])
        self.assertEqual(result["functions"][0]["code_ranges"], [[BASE, BASE + 8], [BASE + 12, BASE + 20]])
        self.assertEqual(result["functions"][0]["pools"], [[BASE + 8, BASE + 12]])
        self.assertEqual(result["boundary_merges"], [{
            "address": BASE + 12, "retained_address": BASE, "evidence": ["tail_call"],
            "reasons": ["section_literal"],
            "witnesses": [{"reason": "section_literal", "address": BASE + 12, "target": BASE + 8}],
        }])

    def test_mid_function_pool_merges_prologue_only_start(self) -> None:
        image, layout = fixture([0xE3A00001, branch(BASE + 4, BASE + 12),
                                 0, 0xE92D4010, 0xE51F0010, 0xE8BD8010])
        analyzer = analyze.Analyzer(image, layout)
        analyzer.add_seed(BASE, "arm", "entry")
        analyzer.add_seed(BASE + 12, "arm", "prologue")
        traces = analyzer.refine()
        self.assertEqual([trace.address for trace in traces], [BASE])
        self.assertEqual(traces[0].pools, {BASE + 8})
        self.assertEqual(analyzer.boundary_merges[0]["evidence"], ["prologue"])

    def test_mid_function_pool_does_not_merge_bl_target(self) -> None:
        words = [0xE3A00001, branch(BASE + 4, BASE + 12), 0, 0xE51F000C, 0xE12FFF1E,
                 0xE92D4010, branch(BASE + 24, BASE + 12, link=True), 0xE8BD8010]
        image, layout = fixture(words)
        result = analyze.build_catalogue(image, layout)
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertIn(BASE + 12, [function["address"] for function in result["functions"]])
        self.assertEqual(result["boundary_merges"], [])
        self.assertIn({"address": BASE + 12, "issue": "literal_outside_function"}, result["anomalies"])

    def test_cross_isa_pool_does_not_merge_blx_target(self) -> None:
        image, layout = fixture([0xE59F0010, 0xFA000001, 0xE12FFF1E, 0,
                                 0x47704801, 0, 0x12345678])
        result = analyze.build_catalogue(image, layout)
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        target = next(function for function in result["functions"] if function["address"] == BASE + 16)
        self.assertEqual(target["isa"], "thumb")
        self.assertIn("blx_target", target["evidence"])
        self.assertEqual(result["boundary_merges"], [])

    def test_pointer_tables_protect_weak_starts(self) -> None:
        words = [0xE3A00001, branch(BASE + 4, BASE + 12), 0, 0xE51F000C, 0xE12FFF1E, 0xE12FFF1E]
        for segment in ("rodata", "data"):
            for nulls in ([], [0]):
                with self.subTest(segment=segment, nulls=nulls):
                    image, layout = fixture(words, **{segment: [BASE + 12, *nulls, BASE + 20]})
                    result = analyze.build_catalogue(image, layout)
                    self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
                    self.assertEqual(result["boundary_merges"], [])
                    self.assertEqual(result["stats"]["protected_pointer_table_starts"], 2)
                    self.assertIn(BASE + 12, [function["address"] for function in result["functions"]])

    def test_isolated_pointer_can_name_an_internal_label(self) -> None:
        result = self.catalogue([0xE3A00001, branch(BASE + 4, BASE + 12),
                                 0, 0xE51F000C, 0xE12FFF1E], rodata=[BASE + 12])
        self.assertEqual(len(result["functions"]), 1)
        self.assertEqual(result["boundary_merges"][0]["evidence"], ["pointer", "tail_call"])

    def test_external_tail_call_protects_weak_start(self) -> None:
        words = [0xE3A00001, branch(BASE + 4, BASE + 12), 0, 0xE51F000C, 0xE12FFF1E,
                 0xE92D4010, 0xE8BD4010, branch(BASE + 28, BASE + 12)]
        image, layout = fixture(words)
        result = analyze.build_catalogue(image, layout)
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertEqual(result["boundary_merges"], [])

    def test_branch_into_body_merges_weak_label(self) -> None:
        image, layout = fixture([0xE3A00001, branch(BASE + 4, BASE + 20), 0,
                                 0xE92D4010, 0xE2800001, 0xE8BD8010])
        result = analyze.build_catalogue(image, layout)
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertEqual(len(result["functions"]), 1)
        merges = {merge["address"]: merge for merge in result["boundary_merges"]}
        self.assertEqual(merges[BASE + 12]["reasons"], ["branch_into_body"])
        self.assertTrue(all(merge["retained_address"] == BASE for merge in merges.values()))
        self.assertEqual(result["stats"]["bytes_by_range_kind"]["padding"], 4)

    def test_literal_constraint_merges_multiple_weak_boundaries(self) -> None:
        # A backward pool reference spans two weak entry guesses.
        image, layout = fixture([0xE3A00001, branch(BASE + 4, BASE + 20), 0,
                                 0xE3A00002, branch(BASE + 16, BASE + 20),
                                 0xE51F0014, 0xE12FFF1E])
        analyzer = analyze.Analyzer(image, layout)
        for address, evidence in ((BASE, "entry"), (BASE + 12, "pointer"), (BASE + 20, "tail_call")):
            analyzer.add_seed(address, "arm", evidence)
        traces = analyzer.refine()
        self.assertEqual([trace.address for trace in traces], [BASE])
        self.assertEqual([merge["address"] for merge in analyzer.boundary_merges], [BASE + 12, BASE + 20])
        self.assertTrue(all(merge["retained_address"] == BASE for merge in analyzer.boundary_merges))
        self.assertTrue(all("section_literal" in merge["reasons"] for merge in analyzer.boundary_merges))

    def test_fallthrough_without_return_merges_prologue(self) -> None:
        image, layout = fixture([0xE3A00001, 0xE92D4010, 0xE8BD8010])
        analyzer = analyze.Analyzer(image, layout)
        analyzer.add_seed(BASE, "arm", "entry")
        analyzer.add_seed(BASE + 4, "arm", "prologue")
        self.assertEqual([trace.address for trace in analyzer.refine()], [BASE])
        self.assertEqual(analyzer.boundary_merges[0]["reasons"], ["fallthrough_without_return"])

    def test_switch_labels_are_not_callback_table_entries(self) -> None:
        image, layout = fixture([0xE12FFF1E, 0xE12FFF1E, BASE, BASE + 4])
        analyzer = analyze.Analyzer(image, layout)
        analyzer.add_seed(BASE, "arm", "entry")
        analyzer.add_seed(BASE + 4, "arm", "pointer")
        trace = analyze.Trace(BASE, "arm", pools={BASE + 8, BASE + 12})
        self.assertEqual(analyzer._pointer_table_starts([trace]), {BASE, BASE + 4})
        trace.table_words.update(trace.pools)
        self.assertEqual(analyzer._pointer_table_starts([trace]), set())

    def test_arm_branch_switch_table(self) -> None:
        # CMP r0,#1; ADDLS pc,pc,r0,LSL#2; default branch; two case branches.
        words = [0xE3500001, 0x908FF100, branch(BASE + 8, BASE + 36),
                 branch(BASE + 12, BASE + 20), branch(BASE + 16, BASE + 28),
                 0xE3A00001, 0xE12FFF1E, 0xE3A00002, 0xE12FFF1E, 0xE3A00003, 0xE12FFF1E]
        result = self.catalogue(words)
        self.assertEqual(len(result["functions"]), 1)
        self.assertEqual(result["functions"][0]["size"], 44)
        self.assertEqual(result["stats"]["switch_tables"], 1)

    def test_arm_address_switch_table(self) -> None:
        words = [0xE3500001, 0x979FF100, branch(BASE + 8, BASE + 36),
                 BASE + 20, BASE + 28, 0xE3A00001, 0xE12FFF1E,
                 0xE3A00002, 0xE12FFF1E, 0xE3A00003, 0xE12FFF1E]
        result = self.catalogue(words)
        self.assertEqual(len(result["functions"]), 1)
        self.assertEqual(result["functions"][0]["pools"], [[BASE + 12, BASE + 20]])

    def test_arm_lo_switch_has_exclusive_bound(self) -> None:
        words = [0xE3500002, 0x379FF100, branch(BASE + 8, BASE + 36),
                 BASE + 20, BASE + 28, 0xE3A00001, 0xE12FFF1E,
                 0xE3A00002, 0xE12FFF1E, 0xE3A00003, 0xE12FFF1E]
        result = self.catalogue(words)
        self.assertEqual(result["functions"][0]["pools"], [[BASE + 12, BASE + 20]])

    def test_branch_table_without_nearby_compare(self) -> None:
        words = [0xE08FF100, 0, branch(BASE + 8, BASE + 16),
                 branch(BASE + 12, BASE + 24), 0xE3A00001, 0xE12FFF1E,
                 0xE3A00002, 0xE12FFF1E]
        image, layout = fixture(words)
        result = analyze.build_catalogue(image, layout)
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertEqual(result["stats"]["switch_tables"], 1)
        self.assertNotIn("unresolved_switch", result["stats"]["anomalies_by_issue"])

    def test_arm_literal_veneer(self) -> None:
        result = self.catalogue([0xE51FF004, BASE + 8, 0xE3A00007, 0xE12FFF1E])
        self.assertEqual(result["functions"][0]["kind"], "veneer")
        self.assertEqual(result["functions"][0]["size"], 8)
        self.assertEqual(result["functions"][0]["pools"], [[BASE + 4, BASE + 8]])
        self.assertIn("veneer_target", result["functions"][1]["evidence"])

    def test_thumb_interworking_veneer(self) -> None:
        # ARM BLX to Thumb BX pc; NOP; ARM B to the final ARM function.
        result = self.catalogue([0xFA000002, 0xE12FFF1E, 0, 0,
                                 0x46C04778, branch(BASE + 20, BASE + 24),
                                 0xE3A00007, 0xE12FFF1E])
        veneer = next(function for function in result["functions"] if function["address"] == BASE + 16)
        self.assertEqual((veneer["isa"], veneer["kind"], veneer["size"]), ("thumb", "veneer", 8))

    def test_thumb_function_reached_by_blx(self) -> None:
        # FA000002 encodes BLX to +16; 2003 / 4770 are Thumb MOVS / BX lr.
        result = self.catalogue([0xFA000002, 0xE12FFF1E, 0, 0, 0x47702003, 0])
        function = result["functions"][1]
        self.assertEqual((function["address"], function["isa"], function["size"]), (BASE + 16, "thumb", 4))
        self.assertIn("blx_target", function["evidence"])

    def test_thumb_literal_and_adr_pools(self) -> None:
        for load in (0x4800, 0xA000):
            with self.subTest(load=load):
                result = self.catalogue([0xFA000002, 0xE12FFF1E, 0, 0,
                                         0x47700000 | load, 0x12345678])
                function = next(function for function in result["functions"] if function["address"] == BASE + 16)
                self.assertEqual(function["code_size"], 4)
                self.assertEqual(function["pools"], [[BASE + 20, BASE + 24]] if load == 0x4800 else [])
                self.assertEqual(function["literals"], [[BASE + 20, BASE + 24]] if load == 0xA000 else [])

    def test_thumb_bl_target(self) -> None:
        result = self.catalogue([0xFA000002, 0xE12FFF1E, 0, 0,
                                 0xF802F000, 0x00004770, 0x47702003])
        function = next(function for function in result["functions"] if function["address"] == BASE + 24)
        self.assertEqual(function["isa"], "thumb")
        self.assertIn("bl_target", function["evidence"])

    def test_thumb_blx_to_arm_target(self) -> None:
        result = self.catalogue([0xFA000002, 0xE12FFF1E, 0, 0,
                                 0xE802F000, 0x00004770, 0xE3A00003, 0xE12FFF1E])
        function = next(function for function in result["functions"] if function["address"] == BASE + 24)
        self.assertEqual(function["isa"], "arm")
        self.assertIn("blx_target", function["evidence"])

    def test_thumb_prologue_in_gap(self) -> None:
        result = self.catalogue([0xE12FFF1E, 0, 0xBD10B510])
        function = next(function for function in result["functions"] if function["address"] == BASE + 8)
        self.assertEqual(function["isa"], "thumb")
        self.assertEqual(function["evidence"], ["prologue"])

    def test_pointer_function_and_prologue_gap(self) -> None:
        words = [0xE12FFF1E, 0, 0xE92D4010, 0xE8BD8010, 0xE3A00001, 0xE12FFF1E]
        result = self.catalogue(words, rodata=[BASE + 16])
        by_address = {function["address"]: function for function in result["functions"]}
        self.assertIn("prologue", by_address[BASE + 8]["evidence"])
        self.assertIn("pointer", by_address[BASE + 16]["evidence"])

    def test_unknown_bytes_are_not_absorbed(self) -> None:
        result = self.catalogue([0xE12FFF1E, 0x12345678])
        self.assertEqual(result["functions"][0]["size"], 4)
        self.assertEqual(result["stats"]["bytes_by_range_kind"]["unknown"], 4)

    def test_call_to_infinite_loop_terminates_fallthrough(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 8, link=True), 0xE12FFF1E,
                                 branch(BASE + 8, BASE + 8), 0xE92D4010,
                                 branch(BASE + 16, BASE + 4, link=True), 0xE8BD8010])
        functions = {function["address"]: function for function in result["functions"]}
        self.assertTrue(functions[BASE + 8]["noreturn"])
        self.assertIn("closed_loop", functions[BASE + 8]["noreturn_reasons"])
        self.assertTrue(functions[BASE]["noreturn"])
        self.assertIn("noreturn_call", functions[BASE]["noreturn_reasons"])
        self.assertEqual(functions[BASE]["code_size"], 4)
        self.assertFalse(functions[BASE + 4]["noreturn"])

    def test_noreturn_propagates_through_calls_to_exit_process(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 8, link=True), 0xE12FFF1E,
                                 branch(BASE + 8, BASE + 16, link=True), 0xE12FFF1E, 0xEF000003],
                                rodata=[BASE + 4, BASE + 12])
        functions = {function["address"]: function for function in result["functions"]}
        self.assertEqual({address for address, function in functions.items() if function["noreturn"]},
                         {BASE, BASE + 8, BASE + 16})
        self.assertIn("svc_exit_process", functions[BASE + 16]["noreturn_reasons"])
        self.assertEqual(result["stats"]["noreturn_functions"], 3)

    def test_discovery_and_table_protection_precede_exit_pruning(self) -> None:
        # A double-width pool after ExitProcess contains two entry pointers.
        result = self.catalogue([branch(BASE, BASE + 8, link=True), 0xE12FFF1E,
                                 0xEF000003, 0xED9F0B00, 0xE12FFF1E, BASE + 28, BASE + 36,
                                 0xE3A00001, 0xE12FFF1E, 0xE3A00002, 0xE12FFF1E])
        functions = {function["address"]: function for function in result["functions"]}
        self.assertTrue(functions[BASE + 8]["noreturn"])
        self.assertEqual(functions[BASE + 8]["pools"], [])
        self.assertIn("pointer", functions[BASE + 28]["evidence"])
        self.assertIn("pointer", functions[BASE + 36]["evidence"])
        self.assertEqual(result["stats"]["protected_pointer_table_starts"], 2)

    def test_conditional_noreturn_call_keeps_returning_path(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 12, link=True, condition=1),
                                 0xE12FFF1E, 0, branch(BASE + 12, BASE + 12)])
        self.assertFalse(result["functions"][0]["noreturn"])
        self.assertEqual(result["functions"][0]["code_size"], 8)
        self.assertTrue(result["functions"][1]["noreturn"])

    def test_unknown_indirect_exit_is_not_noreturn(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 8, link=True), 0xE12FFF1E, 0xE12FFF10])
        self.assertFalse(any(function["noreturn"] for function in result["functions"]))

    def test_incomplete_callee_is_not_noreturn(self) -> None:
        for suffix in ([0xE3A00000, 0], [0xE3A00000]):
            with self.subTest(suffix=suffix):
                image, layout = fixture([branch(BASE, BASE + 8, link=True), 0xE12FFF1E, *suffix])
                result = analyze.build_catalogue(image, layout)
                self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
                self.assertFalse(any(function["noreturn"] for function in result["functions"]))

    def test_other_or_conditional_svc_can_return(self) -> None:
        for instruction in (0xEF000001, 0x1F000003):
            with self.subTest(instruction=instruction):
                result = self.catalogue([branch(BASE, BASE + 8, link=True), 0xE12FFF1E,
                                         instruction, 0xE12FFF1E])
                self.assertFalse(any(function["noreturn"] for function in result["functions"]))

    def test_thumb_exit_process_is_noreturn(self) -> None:
        result = self.catalogue([0xFA000002, 0xE12FFF1E, 0, 0, 0x4770DF03])
        target = next(function for function in result["functions"] if function["address"] == BASE + 16)
        self.assertTrue(target["noreturn"])
        self.assertEqual(target["code_size"], 2)
        self.assertIn("svc_exit_process", target["noreturn_reasons"])
        self.assertEqual(result["stats"]["bytes_by_range_kind"]["unknown"], 6)

    def test_closed_tail_cycle_is_noreturn(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 8, link=True), 0xE12FFF1E,
                                 branch(BASE + 8, BASE + 12), branch(BASE + 12, BASE + 8)],
                                rodata=[BASE + 8, BASE + 12])
        self.assertTrue(all(function["noreturn"] for function in result["functions"]))
        self.assertEqual(result["stats"]["bytes_by_range_kind"]["unknown"], 4)

    def test_noreturn_call_does_not_absorb_unreferenced_return_code(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 16, link=True), 0xE3A00001,
                                 0xE12FFF1E, 0, branch(BASE + 16, BASE + 16)])
        self.assertEqual(result["functions"][0]["size"], 4)
        self.assertEqual(result["stats"]["bytes_by_range_kind"]["unknown"], 8)

    def test_nonzero_page_padding_is_anomaly(self) -> None:
        image, layout = fixture([0xE12FFF1E])
        image = image[:4] + b"\1" + image[5:]
        result = analyze.build_catalogue(image, layout)
        self.assertIn({"address": BASE + 4, "issue": "nonzero_segment_padding"}, result["anomalies"])

    def test_call_to_bad_target_is_anomaly(self) -> None:
        image, layout = fixture([branch(BASE, BASE + 0x10000, link=True), 0xE12FFF1E])
        result = analyze.build_catalogue(image, layout)
        self.assertIn({"address": BASE, "issue": "call_target_not_function_start"}, result["anomalies"])

    def test_map_gap_is_detected(self) -> None:
        image, layout = fixture([0xE12FFF1E])
        result = analyze.build_catalogue(image, layout)
        result["ranges"][0]["end"] -= 1
        self.assertTrue(any(issue["issue"] == "ranges_do_not_tile" for issue in analyze.validate_catalogue(result, image, layout)))

    def test_bad_pool_alignment_is_detected(self) -> None:
        image, layout = fixture([0xE59F0000, 0xE12FFF1E, 0x12345678])
        result = analyze.build_catalogue(image, layout)
        result["functions"][0]["pools"][0][0] += 1
        self.assertTrue(any(issue["issue"] == "invalid_pool_range" for issue in analyze.validate_catalogue(result, image, layout)))

    def test_internal_unknown_hole_is_explicit(self) -> None:
        image, layout = fixture([branch(BASE, BASE + 12, condition=0), 0xE12FFF1E, 0x12345678, 0xE12FFF1E])
        result = analyze.build_catalogue(image, layout)
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertEqual(result["stats"]["bytes_by_range_kind"]["unknown"], 4)
        self.assertIn({"address": BASE, "issue": "unexplained_function_bytes"}, result["anomalies"])

    def test_leading_literal_setup_recovers_start_and_trailing_pool(self) -> None:
        result = self.catalogue([0xE12FFF1E, 0xE59F500C, 0xE24DD004, 0xE1A00005,
                                 0xE28DD004, 0xE12FFF1E, 0x12345678])
        function = result["functions"][1]
        self.assertEqual((function["address"], function["size"]), (BASE + 4, 24))
        self.assertEqual(function["pools"], [[BASE + 24, BASE + 28]])
        self.assertEqual(result["leading_instruction_repairs"][0]["old_address"], BASE + 8)

    def test_multiple_leading_instructions_follow_register_dependencies(self) -> None:
        result = self.catalogue([0xE12FFF1E, 0xE3A01003, 0xE2810001,
                                 0xE92D4010, 0xE1A04000, 0xE8BD8010])
        self.assertEqual(result["functions"][1]["address"], BASE + 4)
        self.assertEqual(len(result["leading_instruction_repairs"][0]["instructions"]), 2)

    def test_leading_setup_rejects_unused_predicated_and_unknown_inputs(self) -> None:
        for prefix, body in ((0xE3A05001, 0xE3A00000),
                             (0x13A05001, 0xE1A00005),
                             (0xE1A05006, 0xE1A00005)):
            with self.subTest(prefix=prefix):
                result = self.catalogue([0xE12FFF1E, prefix, 0xE24DD004,
                                         body, 0xE28DD004, 0xE12FFF1E])
                self.assertEqual(result["functions"][1]["address"], BASE + 8)
                self.assertEqual(result["leading_instruction_repairs"], [])

    def test_real_call_entry_is_not_moved_to_a_leading_setup(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 12, link=True), 0xE12FFF1E,
                                 0xE3A05001, 0xE24DD004, 0xE1A00005,
                                 0xE28DD004, 0xE12FFF1E])
        self.assertEqual(result["functions"][1]["address"], BASE + 12)
        self.assertEqual(result["leading_instruction_repairs"], [])

    def test_protected_leading_setup_is_withheld(self) -> None:
        image, layout = fixture([0xE12FFF1E, 0xE3A05001, 0xE24DD004,
                                 0xE1A00005, 0xE28DD004, 0xE12FFF1E])
        analyzer = analyze.Analyzer(image, layout)
        analyzer.discover()
        traces = analyzer.group_fallthroughs(analyzer.refine())
        analyzer.include_address_literals(traces)
        protected = {BASE + 8: {"address": BASE + 8, "size": 16}}
        self.assertFalse(analyzer.promote_leading_instructions(traces, protected))
        self.assertEqual(analyzer.leading_instruction_inventory[BASE + 8]["decision"], "protected_extent")

    def test_leading_setup_scan_does_not_absorb_an_unbounded_gap(self) -> None:
        result = self.catalogue([0xE12FFF1E, *([0xE2800001] * 65),
                                 0xE92D4010, 0xE1A04000, 0xE8BD8010])
        self.assertEqual(result["leading_instruction_repairs"], [])
        self.assertEqual(result["functions"][1]["address"], BASE + 264)

    def test_adopted_leading_repair_is_reproduced_when_protected(self) -> None:
        image, layout = fixture([0xE12FFF1E, 0xE59F500C, 0xE24DD004, 0xE1A00005,
                                 0xE28DD004, 0xE12FFF1E, 0x12345678])
        first = analyze.build_catalogue(image, layout)
        expected = first["functions"][1]
        second = analyze.build_catalogue(image, layout, protected_functions={BASE + 4: expected})
        self.assertEqual(second["functions"][1], expected)
        self.assertEqual(second["leading_instruction_repairs"], first["leading_instruction_repairs"])

    def test_leading_setup_refuses_swap_store_exclusive_and_exclusive_loads(self) -> None:
        for word, expected_id in ATOMIC_SETUPS:
            with self.subTest(instruction_id=expected_id):
                image, layout = fixture([0xE12FFF1E, word, 0xE92D4010, 0xE1A04000, 0xE8BD8010])
                analyzer = analyze.Analyzer(image, layout)
                decoded = analyzer.detailed(BASE + 4)
                self.assertEqual(decoded.id, expected_id)
                self.assertTrue(analyze._memory_side_effect(decoded))
                if expected_id in (analyze.arm.ARM_INS_SWP, analyze.arm.ARM_INS_STREX):
                    self.assertFalse(any(op.type == analyze.arm.ARM_OP_MEM
                                         and op.access & analyze.capstone.CS_AC_WRITE for op in decoded.operands))
                result = analyze.build_catalogue(image, layout)
                self.assertEqual(result["leading_instruction_repairs"], [])
                self.assertEqual(result["functions"][1]["address"], BASE + 8)

    def test_leading_setup_allows_benign_argument_memory_reads(self) -> None:
        result = self.catalogue([0xE12FFF1E, 0xE5910004, 0xE92D4010, 0xE1A04000, 0xE8BD8010])
        self.assertEqual(result["functions"][1]["address"], BASE + 4)

    def test_leading_pool_cannot_cross_a_known_entry(self) -> None:
        image, layout = fixture([0xE12FFF1E, 0xE59F5010, 0xE24DD004,
                                 0xE1A00005, 0xE28DD004, 0xE12FFF1E,
                                 0xE12FFF1E, 0x12345678], rodata=[BASE + 24])
        result = analyze.build_catalogue(image, layout)
        self.assertNotIn(BASE + 4, {function["address"] for function in result["functions"]})
        self.assertEqual(result["leading_instruction_repairs"], [])

    def test_long_pointer_only_closed_cfg_recovers_nonstandard_prologue(self) -> None:
        result = self.catalogue([0xE12FFF1E, 0xE92D0010, *([0xE2800001] * 40),
                                 0xE8BD0010, 0xE12FFF1E], rodata=[BASE + 4])
        function = result["functions"][1]
        self.assertEqual((function["address"], function["size"]), (BASE + 4, 172))
        proof = next(row for row in result["pointer_entry_inventory"] if row["address"] == BASE + 4)
        self.assertEqual(proof["reason"], "closed_pointer_cfg")
        self.assertEqual(proof["open_exits"], [])

    def test_long_pointer_probe_rejects_an_open_or_invalid_graph(self) -> None:
        for terminal in (0, 0xE12FFF10, branch(BASE + 164, BASE + 0x4000)):
            with self.subTest(terminal=terminal):
                image, layout = fixture([0xE12FFF1E, *([0xE2800001] * 40), terminal],
                                         rodata=[BASE + 4])
                result = analyze.build_catalogue(image, layout)
                self.assertNotIn(BASE + 4, {function["address"] for function in result["functions"]})

    def test_pointer_prefix_ends_at_proved_linker_tail_nop(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 16, link=True), 0xE12FFF1E,
                                 0xE2800004, 0xE1A00000, *([0xE2800001] * 40),
                                 0xE12FFF1E], rodata=[BASE + 8])
        functions = {function["address"]: function for function in result["functions"]}
        self.assertEqual(functions[BASE + 8]["size"], 8)
        self.assertEqual(functions[BASE + 16]["size"], 164)
        self.assertEqual(result["pointer_prefix_wrappers"],
                         [{"address": BASE + 8, "target": BASE + 16, "tail_nop": BASE + 12}])

    def test_pointer_prefix_requires_exact_nop_and_local_flags(self) -> None:
        for words, accepted in (([0xE2800004, 0xE320F000], False),
                                ([0x13A00001, 0xE1A00000], False),
                                ([0xE5801000, 0xE1A00000], False),
                                ([0xE24DD004, 0xE1A00000], False),
                                ([0xE3500000, 0x13A00001, 0xE1A00000], True)):
            with self.subTest(words=words):
                image, layout = fixture([*words, 0xE12FFF1E])
                analyzer = analyze.Analyzer(image, layout)
                end = BASE + len(words) * 4
                analyzer.seeds[end] = analyze.Seed("arm", {"bl_target"})
                self.assertEqual(analyzer._pointer_prefix_wrapper(BASE, "arm", end), accepted)

    def test_pointer_prefix_refuses_swap_store_exclusive_and_exclusive_loads(self) -> None:
        for word, expected_id in ATOMIC_SETUPS:
            with self.subTest(instruction_id=expected_id):
                image, layout = fixture([word, 0xE1A00000, 0xE12FFF1E])
                analyzer = analyze.Analyzer(image, layout)
                analyzer.seeds[BASE + 8] = analyze.Seed("arm", {"bl_target"})
                self.assertEqual(analyzer.detailed(BASE).id, expected_id)
                self.assertFalse(analyzer._pointer_prefix_wrapper(BASE, "arm", BASE + 8))

    def test_pointer_prefix_allows_benign_argument_memory_reads(self) -> None:
        image, layout = fixture([0xE5930004, 0xE3A01001, 0xE1A00000, 0xE12FFF1E])
        analyzer = analyze.Analyzer(image, layout)
        analyzer.seeds[BASE + 12] = analyze.Seed("arm", {"bl_target"})
        self.assertTrue(analyzer._pointer_prefix_wrapper(BASE, "arm", BASE + 12))

    def test_reviewed_classification_retains_extent_and_physical_ownership(self) -> None:
        result = self.catalogue([0xE12FFF1E])
        physical_ranges = json.loads(json.dumps(result["ranges"]))
        row = {"address": BASE, "size": 4, "isa": "arm", "image_sha256": result["image_sha256"],
               "kind": "runtime", "basis": "compiler_abi", "identity": "synthetic ABI helper",
               "evidence": "synthetic compiler/ABI witness", "compiler_emission": "synthetic emitted helper"}
        analyze.apply_classifications(result, [row])
        self.assertEqual(result["functions"][0]["kind"], "runtime")
        self.assertEqual(result["functions"][0]["classification_evidence"], row)
        self.assertEqual(result["ranges"], physical_ranges)
        self.assertEqual((result["stats"]["creditable_functions"], result["stats"]["creditable_bytes"]), (0, 0))

    def test_classification_requires_identity_proofs_and_is_transactional(self) -> None:
        result = self.catalogue([0xE12FFF1E])
        row = {"address": BASE, "size": 4, "isa": "arm", "image_sha256": result["image_sha256"],
               "kind": "runtime", "basis": "compiler_generated", "identity": "synthetic outlined helper",
               "evidence": "synthetic identity", "whole_body_identity": "synthetic equal named section"}
        for change in ({"size": 8}, {"image_sha256": "wrong"}, {"whole_body_identity": ""},
                       {"identity": ""}, {"kind": "library"}, {"basis": "semantic_guess"}):
            with self.subTest(change=change):
                before = analyze.serialize_catalogue(result)
                with self.assertRaises(ValueError):
                    analyze.apply_classifications(result, [{**row, **change}])
                self.assertEqual(analyze.serialize_catalogue(result), before)
        with self.assertRaises(ValueError):
            analyze.apply_classifications(result, [row, row])
        self.assertEqual(result["functions"][0]["kind"], "function")

    def test_classification_cannot_mutate_a_complete_protected_record(self) -> None:
        image, layout = fixture([0xE12FFF1E])
        baseline = analyze.build_catalogue(image, layout)
        published = baseline["functions"][0]
        row = {"address": BASE, "size": 4, "isa": "arm", "image_sha256": baseline["image_sha256"],
               "kind": "runtime", "basis": "compiler_abi", "identity": "synthetic ABI helper",
               "evidence": "synthetic compiler/ABI witness", "compiler_emission": "synthetic emitted helper"}
        with self.assertRaisesRegex(ValueError, "protected function"):
            analyze.build_catalogue(image, layout, protected_functions={BASE: published}, classifications=[row])
        self.assertEqual(published["kind"], "function")
        self.assertNotIn("classification_evidence", published)

    def test_protected_comparison_rejects_evidence_only_mutation(self) -> None:
        result = self.catalogue([0xE12FFF1E])
        published = json.loads(json.dumps(result["functions"][0]))
        result["functions"][0]["classification_evidence"] = {"evidence": "synthetic annotation"}
        with self.assertRaisesRegex(ValueError, "protected function"):
            analyze.check_protected_functions(result, {BASE: published})

    def test_manifest_protection_includes_c007_and_pinned_c008_blob(self) -> None:
        baseline = self.catalogue([0xE12FFF1E, 0xE12FFF1E, 0xE12FFF1E])
        # Separate entries are supplied explicitly; this fixture concerns manifest
        # resolution, not speculative discovery of unreachable adjacent returns.
        functions = [dict(baseline["functions"][0], address=BASE + 4 * index, size=4) for index in range(3)]
        baseline["functions"] = functions
        scratch = ROOT / "scratch"
        scratch.mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(dir=scratch) as temporary:
            root = Path(temporary)
            (root / "config/candidates").mkdir(parents=True)
            (root / "config/matches.json").write_text(json.dumps({"units": [{"functions": ["func_00100000"]}]}))
            (root / "config/candidates/c005.json").write_text(
                json.dumps({"units": [{"functions": ["func_00100004"]}]}))
            (root / "config/candidates/c007.json").write_text(
                json.dumps({"units": [{"functions": ["func_00100004"]}]}))
            content = json.dumps({"units": [{"functions": ["func_00100008"]}]}).encode()
            blob = "1" * 40
            descriptor = {"reference": "c008-integrate:config/candidates/c008.json", "blob": blob,
                          "sha256": hashlib.sha256(content).hexdigest()}
            with mock.patch.object(analyze.subprocess, "run", return_value=
                                   subprocess.CompletedProcess([], 0, content, b"")) as run:
                protected, summaries, hashes = analyze.load_protected_functions(
                    root, baseline, [], git_blobs=[descriptor])
            self.assertEqual(protected, {function["address"]: function for function in functions})
            label = f"git:c008-integrate:config/candidates/c008.json@{blob}"
            self.assertEqual(summaries[label], {"functions": 1, "bytes": 4, "changes": []})
            self.assertEqual(hashes[label], hashlib.sha256(content).hexdigest())
            self.assertEqual(run.call_args.args[0], ["git", "show", blob])
            with mock.patch.object(analyze.subprocess, "run", return_value=
                                   subprocess.CompletedProcess([], 0, b"wrong blob", b"")):
                with self.assertRaisesRegex(ValueError, "digest"):
                    analyze.load_protected_functions(root, baseline, [], git_blobs=[descriptor])

    def test_plain_catalogue_command_includes_review_and_reproduces_all_bytes(self) -> None:
        image, layout = fixture([0xE12FFF1E])
        baseline = analyze.build_catalogue(image, layout)
        row = {"address": BASE, "size": 4, "isa": "arm", "image_sha256": baseline["image_sha256"],
               "kind": "runtime", "basis": "compiler_abi", "identity": "synthetic ABI helper",
               "evidence": "synthetic compiler/ABI witness", "compiler_emission": "synthetic emitted helper"}
        evidence = {"revision": "catalogue-v9-r2", "image_sha256": baseline["image_sha256"],
                    "classification_records": [row], "thumb_dispositions": [], "author_dispositions": [],
                    "provenance": {"proof_status": "synthetic"}, "source_candidate_sha256": "synthetic",
                    "source_input_sha256": {}}
        scratch = ROOT / "scratch"
        scratch.mkdir(exist_ok=True)
        for protect in (False, True):
            with self.subTest(protected=protect), tempfile.TemporaryDirectory(dir=scratch) as temporary:
                root = Path(temporary)
                for directory in ("config", "orig", "research"):
                    (root / directory).mkdir()
                (root / "config/catalogue.json").write_bytes(analyze.serialize_catalogue(baseline))
                (root / "config/matches.json").write_text(
                    json.dumps({"units": [{"functions": ["func_00100000"]}]} if protect else {"units": []}))
                (root / "research/catalogue_v9_r2_evidence.json").write_text(json.dumps(evidence))
                baseline_payload = analyze.serialize_catalogue(baseline)
                closure = {"image_sha256": baseline["image_sha256"], "literal_encodings": [],
                           "provenance": {"proof_status": "synthetic"}, "closed_graph_owners": [],
                           "parent_evidence": {"path": "research/catalogue_v9_r2_evidence.json",
                                               "sha256": hashlib.sha256((root / "research/catalogue_v9_r2_evidence.json").read_bytes()).hexdigest()},
                           "baseline_catalogue": {"blob": "1" * 40, "reference": "synthetic",
                                                  "revision": "synthetic-adopted",
                                                  "sha256": hashlib.sha256(baseline_payload).hexdigest()}}
                (root / "research/catalogue_v10_evidence.json").write_text(json.dumps(closure))
                (root / "orig/code.bin").write_bytes(image)
                exheader = bytearray(0x200)
                for offset, segment in zip((0x10, 0x20, 0x30), (layout.text, layout.rodata, layout.data)):
                    struct.pack_into("<III", exheader, offset, segment.address, 1, segment.size)
                (root / "orig/exheader.bin").write_bytes(exheader)
                with mock.patch.object(analyze, "ROOT", root), mock.patch.object(
                    analyze.subprocess, "run", return_value=subprocess.CompletedProcess([], 0, baseline_payload, b"")
                ), redirect_stdout(io.StringIO()):
                    for name in ("first", "second"):
                        self.assertEqual(analyze.main(["catalogue", "--out", str(root / f"{name}.json")]), 0)
                self.assertEqual((root / "first.json").read_bytes(), (root / "second.json").read_bytes())
                result = json.loads((root / "first.json").read_bytes())
                self.assertTrue(result["generator"]["bundled_review_applied"])
                self.assertEqual(result["revision_review"]["name"], "catalogue-v10")
                self.assertEqual(result["generator"]["classification_input"], "research/catalogue_v9_r2_evidence.json")
                self.assertIn("research/catalogue_v10_evidence.json", result["generator"]["input_sha256"])
                self.assertEqual(result["functions"][0]["kind"], "function" if protect else "runtime")
                self.assertEqual(len(result["revision_review"]["withheld_classifications"]), int(protect))

    def test_ordinary_library_identity_is_not_just_a_semantic_name(self) -> None:
        result = self.catalogue([0xE12FFF1E])
        row = {"address": BASE, "size": 4, "isa": "arm", "image_sha256": result["image_sha256"],
               "kind": "library", "basis": "known_library", "identity": "synthetic strlen",
               "evidence": "synthetic string semantics"}
        with self.assertRaises(ValueError):
            analyze.apply_classifications(result, [row])
        row["known_library_identity"] = "synthetic authenticated member equality"
        analyze.apply_classifications(result, [row])
        self.assertEqual(result["functions"][0]["kind"], "library")

    def test_raw_abi_classification_needs_a_hard_compiler_limit(self) -> None:
        result = self.catalogue([0xE12FFF1E])
        row = {"address": BASE, "size": 4, "isa": "arm", "image_sha256": result["image_sha256"],
               "kind": "assembly", "basis": "unrepresentable_abi", "identity": "synthetic raw kernel ABI",
               "evidence": "synthetic wrapper shape"}
        with self.assertRaises(ValueError):
            analyze.apply_classifications(result, [row])
        row["compiler_limit"] = "synthetic rejected r4 argument proof"
        analyze.apply_classifications(result, [row])
        self.assertEqual(result["functions"][0]["kind"], "assembly")

    def test_runtime_evidence_does_not_erase_existing_multi_entry_topology(self) -> None:
        result = self.catalogue([0xE12FFF1E])
        function = result["functions"][0]
        function["kind"] = "multi_entry"
        before = json.loads(json.dumps(function["entries"]))
        row = {"address": BASE, "size": 4, "isa": "arm", "image_sha256": result["image_sha256"],
               "kind": "runtime", "basis": "compiler_abi", "identity": "synthetic shared ABI",
               "evidence": "synthetic ABI witness", "compiler_emission": "synthetic emitted helper"}
        analyze.apply_classifications(result, [row])
        self.assertEqual(function["kind"], "multi_entry")
        self.assertEqual(function["entries"], before)
        self.assertEqual(result["stats"]["creditable_functions"], 0)

    def test_pc_relative_ip_exchange_is_a_code_veneer(self) -> None:
        result = self.catalogue([0xE28FC001, 0xE12FFF1C, 0x47702007])
        veneer, target = result["functions"]
        self.assertEqual((veneer["kind"], veneer["size"], veneer["pools"], veneer["literals"]),
                         ("veneer", 8, [], []))
        self.assertEqual(veneer["code_ranges"], [[BASE, BASE + 8]])
        self.assertEqual((target["address"], target["isa"], target["size"]), (BASE + 8, "thumb", 4))

    def test_pc_relative_veneer_requires_exact_register_condition_and_thumb_target(self) -> None:
        for setup, transfer in ((0xE28FC000, 0xE12FFF1C), (0x128FC001, 0xE12FFF1C),
                                (0xE28F0001, 0xE12FFF10), (0xE28FC001, 0xE12FFF1E)):
            with self.subTest(setup=setup, transfer=transfer):
                image, layout = fixture([setup, transfer, 0x47702007])
                self.assertIsNone(analyze.Analyzer(image, layout).veneer(BASE, "arm"))

    def test_shared_empty_literal_is_data_not_padding_or_extent_growth(self) -> None:
        image, layout = fixture([0xE28F0000, 0xE12FFF1E, 0], rodata=[BASE + 8])
        result = analyze.build_catalogue(image, layout)
        self.assertEqual(result["functions"][0]["size"], 8)
        self.assertIn({"start": BASE + 8, "end": BASE + 12, "kind": "rodata", "owner": None}, result["ranges"])
        self.assertEqual(result["anomalies"], [])

    def test_reviewed_utf16_bounds_require_exact_references(self) -> None:
        image, layout = fixture([0xE28F0000, 0xE12FFF1E, *literal_words("alpha\0".encode("utf-16le"))])
        record = {"function": BASE, "target": BASE + 8, "references": [BASE], "encoding": "utf-16le"}
        result = analyze.build_catalogue(image, layout, literal_encodings=[record])
        self.assertEqual(result["functions"][0]["literals"], [[BASE + 8, BASE + 20]])
        self.assertEqual(result["stats"]["literal_objects_by_kind"], {"utf16_string": 1})
        for change in ({"references": [BASE + 4]}, {"encoding": "guessed-wide"}, {"function": BASE + 4}):
            with self.subTest(change=change), self.assertRaises(ValueError):
                analyze.build_catalogue(image, layout, literal_encodings=[{**record, **change}])
        with self.assertRaises(ValueError):
            analyze.build_catalogue(image, layout, literal_encodings=[record, record])

    def test_reviewed_utf16_type_cannot_override_callable_code(self) -> None:
        image, layout = fixture([0xE28F0004, 0xFA000001, 0xE12FFF1E, *literal_words(b"ABCDpGEF\0\0")])
        result = analyze.build_catalogue(image, layout, literal_encodings=[
            {"function": BASE, "target": BASE + 12, "references": [BASE], "encoding": "utf-16le"}])
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertEqual(result["functions"][0]["literals"], [])
        self.assertIn("address_literal_overlaps_code", result["literal_conflicts"][0]["reasons"])

    def test_reviewed_utf16_requires_a_complete_terminator(self) -> None:
        image, layout = fixture([0xE28F0000, 0xE12FFF1E, *literal_words("abcd".encode("utf-16le"))])
        with self.assertRaisesRegex(ValueError, "complete printable bound"):
            analyze.build_catalogue(image, layout, literal_encodings=[
                {"function": BASE, "target": BASE + 8, "references": [BASE], "encoding": "utf-16le"}])

    def test_section_authenticated_relative_table_recovers_missing_entries(self) -> None:
        table = BASE + 0x1000
        pool_words, bounds = walker_bounds(table, table + 8, 8)
        image, layout = fixture([0xE12FFF1E, 0, 0, 0, 0xE3A00007, 0xE12FFF1E, 0, 0, *pool_words],
                                rodata=[(BASE - table) & 0xFFFFFFFF, (BASE + 16 - table - 4) & 0xFFFFFFFF])
        record = {"start": table, "end": table + 8, "section": ".init_array", "bounds": bounds,
                  "encoding": "self_relative_32", "evidence": "synthetic compiler section-ABI witness"}
        baseline = analyze.build_catalogue(image, layout)
        self.assertEqual([function["address"] for function in baseline["functions"]], [BASE])
        result = analyze.build_catalogue(image, layout, relative_pointer_tables=[record])
        self.assertEqual([function["address"] for function in result["functions"]], [BASE, BASE + 16])
        self.assertEqual(result["functions"][1]["evidence"], ["relative_init_array"])
        self.assertEqual([row["address"] for row in result["relative_pointer_tables"][0]["targets"]], [BASE, BASE + 16])
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])

    def test_relative_table_is_bounded_to_the_proved_section(self) -> None:
        table = BASE + 0x1000
        pool_words, bounds = walker_bounds(table, table + 4, 8)
        image, layout = fixture([0xE12FFF1E, 0, 0, 0, 0, 0, 0, 0, *pool_words], rodata=[(BASE - table) & 0xFFFFFFFF])
        record = {"start": table, "end": table + 4, "section": ".init_array", "bounds": bounds,
                  "encoding": "self_relative_32", "evidence": "synthetic compiler section-ABI witness"}
        shifted = {"start": [bounds["start"][0], bounds["start"][1] + 4], "end": bounds["end"]}
        for change in ({"start": table + 1}, {"end": table + 8}, {"section": ".guessed"},
                       {"evidence": ""}, {"encoding": "absolute"}, {"start": BASE},
                       {"bounds": {"end": bounds["end"]}}, {"bounds": shifted},
                       {"bounds": {"start": [BASE + 0x2000, 0], "end": bounds["end"]}}):
            with self.subTest(change=change), self.assertRaises(ValueError):
                analyze.build_catalogue(image, layout, relative_pointer_tables=[{**record, **change}])
        bad_image, bad_layout = fixture([0xE12FFF1E, 0, 0, 0, 0, 0, 0, 0, *pool_words], rodata=[0])
        with self.assertRaisesRegex(ValueError, "plausible code"):
            analyze.build_catalogue(bad_image, bad_layout, relative_pointer_tables=[record])

    def test_new_initializer_calls_preserve_complete_protected_entry_metadata(self) -> None:
        table = BASE + 0x1000
        pool_words, bounds = walker_bounds(table, table + 4, 9)
        image, layout = fixture([branch(BASE, BASE + 32, link=True), 0xE12FFF1E, 0, 0,
                                branch(BASE + 16, BASE + 32, link=True), 0xE12FFF1E, 0, 0, 0xE12FFF1E, *pool_words],
                                rodata=[(BASE + 16 - table) & 0xFFFFFFFF])
        baseline = analyze.build_catalogue(image, layout)
        expected = next(function for function in baseline["functions"] if function["address"] == BASE + 32)
        result = analyze.build_catalogue(image, layout, protected_functions={BASE + 32: expected},
                                         relative_pointer_tables=[{
                                             "start": table, "end": table + 4, "section": ".init_array",
                                             "bounds": bounds, "encoding": "self_relative_32",
                                             "evidence": "synthetic section proof"}])
        actual = next(function for function in result["functions"] if function["address"] == BASE + 32)
        self.assertEqual(actual, expected)
        self.assertEqual(result["protected_entry_observations"], [
            {"function": BASE + 16, "site": BASE + 16, "target": BASE + 32,
             "reason": "protected_adopted_entry_metadata"}])

    def test_initializer_recovery_preserves_a_legacy_prologue_entry_without_credit(self) -> None:
        table = BASE + 0x1000
        pool_words, bounds = walker_bounds(table, table + 4, 8)
        image, layout = fixture([
            0xE12FFF1E, 0, 0, 0, 0xE3A00007, 0xE92D4010, 0xE3A00000, 0xE8BD8010, *pool_words],
            rodata=[(BASE + 16 - table) & 0xFFFFFFFF])
        baseline = analyze.build_catalogue(image, layout)
        prior = next(function for function in baseline["functions"] if function["address"] == BASE + 20)
        result = analyze.build_catalogue(image, layout, prior_functions=baseline["functions"],
                                         relative_pointer_tables=[{
                                             "start": table, "end": table + 4, "section": ".init_array",
                                             "bounds": bounds, "encoding": "self_relative_32",
                                             "evidence": "synthetic section proof"}])
        recovered = next(function for function in result["functions"] if function["address"] == BASE + 16)
        self.assertEqual(recovered["kind"], "multi_entry")
        self.assertEqual(recovered["entries"], [
            {"address": BASE + 16, "isa": "arm", "kind": "entry",
             "evidence": ["relative_init_array"], "sources": []},
            {"address": BASE + 20, "isa": "arm", "kind": "secondary_entry",
             "evidence": ["legacy_prologue"], "sources": []},
        ])
        self.assertEqual(result["legacy_entry_preservation"], [{
            "old_address": BASE + 20, "address": BASE + 16, "old_size": prior["size"],
            "size": recovered["size"], "isa": "arm", "old_entry_evidence": ["prologue"],
            "entry_evidence": ["legacy_prologue"],
            "reason": "preserved prior prologue-derived label; no independent callability witness or C credit",
        }])
        self.assertEqual(result["stats"]["creditable_functions"], 1)
        self.assertEqual(result["stats"]["secondary_entries"], 1)
        regenerated = analyze.build_catalogue(image, layout, prior_functions=result["functions"],
                                               relative_pointer_tables=[{
                                                   "start": table, "end": table + 4, "section": ".init_array",
                                                   "bounds": bounds, "encoding": "self_relative_32",
                                                   "evidence": "synthetic section proof"}])
        self.assertEqual(regenerated["functions"], result["functions"])
        self.assertEqual(regenerated["stats"]["creditable_functions"], 1)

    def test_adopted_legacy_preservation_refuses_changed_owner_or_stronger_entry(self) -> None:
        result = self.catalogue([0xE3A00007, 0xE3A00000, 0xE12FFF1E])
        result["functions"][0]["evidence"] = ["relative_init_array"]
        previous = json.loads(json.dumps(result["functions"][0]))
        previous["kind"] = "multi_entry"
        previous["entries"].append({"address": BASE + 4, "isa": "arm", "kind": "secondary_entry",
                                     "evidence": ["legacy_prologue"], "sources": []})
        for change in ("owner", "kind", "evidence", "incoming_call", "entry_kind", "lost_span"):
            with self.subTest(change=change):
                candidate, before = json.loads(json.dumps(result)), json.loads(json.dumps(previous))
                legacy = before["entries"][1]
                if change == "owner":
                    before["address"] -= 4
                elif change == "kind":
                    before["kind"] = "function"
                elif change == "evidence":
                    legacy["evidence"] = ["pointer"]
                elif change == "incoming_call":
                    legacy["sources"] = [BASE + 0x100]
                elif change == "entry_kind":
                    legacy["kind"] = "entry"
                else:
                    before["code_ranges"][0][1] += 4
                with self.assertRaisesRegex(ValueError, "prior entry"):
                    analyze.preserve_prior_entries(candidate, [before])

    def test_legacy_preservation_refuses_unproved_or_noninstruction_mappings(self) -> None:
        result = self.catalogue([0xE3A00007, 0xE3A00000, 0xE12FFF1E])
        before = {**result["functions"][0], "address": BASE + 4, "size": 8, "code_size": 8,
                  "code_ranges": [[BASE + 4, BASE + 12]], "evidence": ["prologue"],
                  "entries": [{"address": BASE + 4, "isa": "arm", "kind": "entry",
                               "evidence": ["prologue"], "sources": []}]}
        for change in ("missing_initializer", "incoming_call", "wrong_isa", "unaligned", "lost_span"):
            with self.subTest(change=change):
                candidate, previous = json.loads(json.dumps(result)), json.loads(json.dumps(before))
                candidate["functions"][0]["evidence"] = ["relative_init_array"]
                if change == "missing_initializer":
                    candidate["functions"][0]["evidence"] = ["pointer"]
                elif change == "incoming_call":
                    previous["entries"][0]["sources"] = [BASE + 0x100]
                elif change == "wrong_isa":
                    previous["entries"][0]["isa"] = "thumb"
                elif change == "unaligned":
                    previous["entries"][0]["address"] += 2
                else:
                    previous["code_ranges"][0][1] += 4
                with self.assertRaisesRegex(ValueError, "prior entry"):
                    analyze.preserve_prior_entries(candidate, [previous])

    def test_nonstoring_wrapper_splits_closed_object_access_graph(self) -> None:
        result = self.catalogue([
            0xE5900008, 0xE1A00000, 0xE5901000, 0xE3510000,
            branch(BASE + 16, BASE + 32, condition=0), 0xE5912000, 0xE5812004, 0xE1A00002, 0xE12FFF1E])
        self.assertEqual([function["size"] for function in result["functions"]], [8, 28])
        self.assertIn("nonstoring_wrapper_field_access_graph", result["linker_tail_extent_splits"][0]["start_evidence"])

    def test_nonstoring_wrapper_splits_closed_pointer_count_loop(self) -> None:
        result = self.catalogue([
            0xE5900008, 0xE1A00000, 0xE1A01000, 0xE3A00000, 0xE5912000, 0xE3520000,
            branch(BASE + 24, BASE + 40, condition=0), 0xE2800001, 0xE2811004,
            branch(BASE + 36, BASE + 16), 0xE12FFF1E])
        self.assertEqual([function["size"] for function in result["functions"]], [8, 36])

    def test_accessor_graph_intersects_definitions_at_control_flow_joins(self) -> None:
        image, layout = fixture([
            0xE5900008, 0xE1A00000, 0xE5901000, 0xE3510000,
            branch(BASE + 16, BASE + 24, condition=0), 0xE3A02001, 0xE0820000, 0xE12FFF1E])
        result = analyze.build_catalogue(image, layout)
        self.assertEqual(result["linker_tail_extent_splits"], [])

    def test_accessor_graph_never_admits_an_atomic_prefix(self) -> None:
        for prefix, _ in ATOMIC_SETUPS:
            with self.subTest(prefix=prefix):
                result = self.catalogue([
                    prefix, 0xE1A00000, 0xE5901000, 0xE3510000,
                    branch(BASE + 16, BASE + 24, condition=0), 0xE1A00001, 0xE12FFF1E])
                self.assertEqual(result["linker_tail_extent_splits"], [])

    def test_restored_frame_splits_a_single_closed_tail_wrapper(self) -> None:
        result = self.catalogue([
            0xE92D4010, branch(BASE + 4, BASE + 28, link=True), 0xE8BD4010, 0xE1A00000,
            branch(BASE + 16, BASE + 28), 0, 0, 0xE12FFF1E])
        functions = {function["address"]: function for function in result["functions"]}
        self.assertEqual((functions[BASE]["size"], functions[BASE + 16]["size"]), (16, 4))
        self.assertIn("restored_frame_closed_tail", result["linker_tail_extent_splits"][0]["start_evidence"])

    @unittest.skipUnless((ROOT / "orig/code.bin").is_file() and (ROOT / "orig/exheader.bin").is_file(), "private inputs absent")
    def test_original_invariants_and_stability(self) -> None:
        image = (ROOT / "orig/code.bin").read_bytes()
        layout = ctrfmt.parse_exheader((ROOT / "orig/exheader.bin").read_bytes())
        adopted = json.loads((ROOT / "config/catalogue.json").read_bytes())
        evidence = json.loads((ROOT / "research/catalogue_v9_r2_evidence.json").read_bytes())
        closure, _, _ = catalogue_closure.load_inputs(ROOT, ROOT / "research/catalogue_v9_r2_evidence.json")
        protected, _, _ = analyze.load_protected_functions(
            ROOT, adopted, [], git_blobs=evidence["protected_git_blobs"])
        adopted_functions = {function["address"]: function for function in adopted["functions"]}
        scratch = ROOT / "scratch"
        scratch.mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(dir=scratch) as temporary, redirect_stdout(io.StringIO()):
            paths = [Path(temporary) / name for name in ("first.json", "second.json")]
            for path in paths:
                self.assertEqual(analyze.main(["catalogue", "--out", str(path)]), 0)
            self.assertEqual(paths[0].read_bytes(), paths[1].read_bytes())
            first = json.loads(paths[0].read_bytes())
        self.assertEqual(analyze.validate_catalogue(first, image, layout), [])
        self.assertEqual(sum(item["end"] - item["start"] for item in first["ranges"]), len(image))
        functions = {function["address"]: function for function in first["functions"]}
        for address, expected in protected.items():
            self.assertEqual(functions[expected["address"]], expected, f"{address:#x}")
        self.assertEqual((first["stats"]["functions"], first["stats"]["creditable_functions"],
                          first["stats"]["creditable_bytes"]), (4743, 4662, 1133322))
        self.assertEqual((first["stats"]["bytes_by_range_kind"]["unknown"], first["stats"]["unknown_ranges"],
                          first["stats"]["anomalies"]), (360, 36, 10))
        self.assertEqual(sum(first["stats"][key] for key in ("code_bytes", "pool_bytes", "literal_bytes")),
                         first["stats"]["bytes_by_range_kind"]["function"])
        self.assertEqual((functions[0x1372EC]["size"], functions[0x1372EC]["literals"]),
                         (708, [[0x137520, 0x1375B0]]))
        for address in (0x1BC29C, 0x1B8234, 0x1B54E4):
            item = next(row for row in first["ranges"] if row["start"] <= address < row["end"])
            self.assertEqual((item["kind"], item["owner"], item["end"] - item["start"]), ("rodata", None, 4))
        regenerated = analyze.build_catalogue(
            image, layout, protected_functions=protected,
            classifications=evidence["classification_records"] + closure["classification_records"],
            literal_encodings=evidence["literal_encodings"] + closure["literal_encodings"],
            relative_pointer_tables=evidence["relative_pointer_tables"], prior_functions=first["functions"], closure=closure)
        self.assertEqual(regenerated["functions"], first["functions"])
        for address, (head_size, target, tail_size) in {
            0x131948: (8, 0x131950, 8), 0x1EFCD0: (8, 0x1EFCD8, 8),
            0x10BED4: (32, 0x10BEF4, 4), 0x1D033C: (12, 0x1D0348, 56),
            0x1D222C: (8, 0x1D2234, 52), 0x1F4784: (8, 0x1F478C, 40),
        }.items():
            self.assertEqual((functions[address]["size"], functions[target]["size"]), (head_size, tail_size))
        for address in (0x1008AC, 0x100C80, 0x100C9E, 0x19F6B4, 0x1847C8):
            self.assertEqual(functions[address]["kind"], "runtime")
        for address in (0x11EF2C, 0x11EF4C, 0x1565B0):
            self.assertEqual((functions[address]["kind"], functions[address]["classification_evidence"]["basis"]),
                             ("assembly", "unrepresentable_abi"))
        for address in (0x1109B4, 0x11EF58, 0x12D0EC, 0x12D150, 0x18F7D4):
            self.assertEqual((functions[address]["kind"], functions[address]["classification_evidence"]["basis"]),
                             ("assembly", "unrepresentable_abi"))
        group = functions[0x18EAEC]
        self.assertEqual((group["kind"], group["size"]), ("multi_entry", 924))
        self.assertEqual([entry["address"] for entry in group["entries"]], [0x18EAEC, 0x18EBF8, 0x18EC7C])
        self.assertFalse(first["closure_evidence"]["owner_merges"][0]["ownership_classified"])
        self.assertEqual(len(first["closure_evidence"]["closed_leaves"]), 42)
        self.assertEqual(sum(row["end"] - row["start"] for row in first["closure_evidence"]["closed_leaves"]), 2560)
        self.assertEqual(len(first["pc_relative_pointer_witnesses"]), 3)
        for address in (0x10007C, 0x100114, 0x100520, 0x1008DC, 0x10090C, 0x100A6C, 0x100AF0, 0x100C94,
                        0x19F5BC, 0x19F634, 0x19F6AC, 0x1911C4):
            self.assertEqual((functions[address]["kind"], functions[address]["size"]), ("veneer", 8))
        self.assertEqual(sum(function["kind"] == "veneer" for function in functions.values()), 18)
        self.assertEqual((functions[0x212674]["size"], functions[0x2136C4]["size"]), (84, 56))
        self.assertEqual(functions[0x212674]["pools"], [[0x2126B4, 0x2126C8]])
        self.assertEqual(functions[0x2136C4]["pools"], [[0x2136EC, 0x2136FC]])
        self.assertEqual(first["stats"]["linker_nop_words"], 776)
        self.assertEqual(len(first["linker_tail_extent_splits"]), 143)
        self.assertEqual([row["site"] for row in first["linker_nop_inventory"]],
                         [layout.text.address + offset for offset in range(0, layout.text.size - 3, 4)
                          if struct.unpack_from("<I", image, offset)[0] == 0xE1A00000])
        review = first["revision_review"]
        self.assertEqual(review["structural_issues"], [])
        self.assertEqual(review["preservation"]["protected_changes"], [])
        self.assertEqual(review["preservation"]["unexpected_entry_losses"], [])
        self.assertEqual(review["preservation"]["unexpected_extent_changes"], [])
        prior_entries = {(entry["address"], entry["isa"]) for function in adopted["functions"]
                         for entry in function["entries"]}
        current_entries = {(entry["address"], entry["isa"]) for function in functions.values()
                           for entry in function["entries"]}
        self.assertLessEqual(prior_entries, current_entries)
        self.assertEqual(review["preservation"]["retained_prior_entries"], len(prior_entries))
        expected_legacy = {0x1F5990: 0x1F5980, 0x2069F0: 0x2069EC, 0x212684: 0x212674,
                           0x2136D0: 0x2136C4, 0x214FEC: 0x214FE8}
        self.assertEqual({row["old_address"]: row["address"] for row in first["legacy_entry_preservation"]},
                         expected_legacy)
        for address, target in expected_legacy.items():
            self.assertEqual(functions[target]["kind"], "multi_entry")
            legacy = next(entry for entry in functions[target]["entries"] if entry["address"] == address)
            self.assertEqual((legacy["kind"], legacy["evidence"], legacy["sources"]),
                             ("secondary_entry", ["legacy_prologue"], []))
        self.assertEqual((functions[0x145610]["kind"], functions[0x145610]["size"]), ("function", 28))
        self.assertEqual(len(review["unknown_accounting"]), adopted["stats"]["unknown_ranges"])
        self.assertEqual(len(review["anomaly_accounting"]), adopted["stats"]["anomalies"])
        table_targets = [row for table in first["relative_pointer_tables"] for row in table["targets"]]
        self.assertEqual(len(table_targets), 160)
        entries = {(entry["address"], entry["isa"]) for function in first["functions"] for entry in function["entries"]}
        self.assertTrue(all((row["address"], row["isa"]) in entries for row in table_targets))
        self.assertEqual(len(evidence["initializer_targets_previously_unknown"]), 93)
        residue = review["thumb_v9_residue"]
        self.assertEqual(len(residue), 56)
        self.assertFalse(any(row["ownership_classified"] for row in residue))
        self.assertEqual(sum(row["candidate_owner_kind"] == "function" for row in residue), 54)
        self.assertEqual({row["address"]: row["candidate_owner"] for row in residue
                          if row["candidate_owner_kind"] == "multi_entry"},
                         {0x1000A0: 0x100084, 0x10030C: 0x10030C})
        for address in (0x1932E8, 0x113CA4, 0x110CC8, 0x14B358, 0x112C80, 0x112C98, 0x14E50C, 0x16BDE0):
            self.assertEqual(functions[address]["size"], adopted_functions[address]["size"])
        self.assertNotIn(0x14E510, functions)
        self.assertNotIn(0x16BDE4, functions)


class CatalogueClosureTests(unittest.TestCase):
    def leaf(self, words: list[int], *, record: bool = False) -> dict | None:
        image, layout = fixture(words)
        return catalogue_closure.constant_copy_leaf(
            analyze.Analyzer(image, layout), BASE, BASE + len(words) * 4, allow_record_layout=record)

    def test_text_closure_preserves_each_mapped_segments_range_boundaries(self) -> None:
        for rodata, data in (([], []), ([0], []), ([], [0]), ([0], [0])):
            with self.subTest(rodata=rodata, data=data):
                image, layout = fixture([0xE12FFF1E], rodata=rodata, data=data)
                baseline = analyze.build_catalogue(image, layout)
                closed = analyze.build_catalogue(image, layout, closure={})
                self.assertEqual(closed["ranges"], baseline["ranges"])
                self.assertEqual(analyze.validate_catalogue(closed, image, layout), [])

    def test_returning_svc_discards_all_core_call_clobbered_pointer_facts(self) -> None:
        for register in (0, 1, 2, 3, 12):
            for condition in (1, 14):
                with self.subTest(register=register, condition=condition):
                    image, layout = fixture([
                        0xE59F0008 | register << 12, condition << 28 | 0x0F000028,
                        0xE1D010F0 | register << 16, 0xE12FFF1E, BASE + 24,
                        0xFFFFFFFF, 0x01010101, 0xFFFFFFFF])
                    result = analyze.build_catalogue(image, layout, closure={})
                    row = next(row for row in result["ranges"] if row["start"] <= BASE + 24 < row["end"])
                    self.assertEqual((row["kind"], row["owner"]), ("unknown", None))
                    self.assertFalse(any(row["site"] == BASE + 8 for row in result["closure_evidence"]["read_footprints"]))

    def test_exclusive_pair_write_does_not_leave_a_stale_pointer(self) -> None:
        # LDREXD writes both r0/r1; Capstone 5's aggregate writes/access flags omit them.
        image, layout = fixture([
            0xE59F100C, 0xE3A02000, 0xE1B20F9F, 0xE1D130F0, 0xE12FFF1E,
            BASE + 28, 0xFFFFFFFF, 0x01010101, 0xFFFFFFFF])
        result = analyze.build_catalogue(image, layout, closure={})
        row = next(row for row in result["ranges"] if row["start"] <= BASE + 28 < row["end"])
        self.assertEqual((row["kind"], row["owner"]), ("unknown", None))
        self.assertFalse(any(row["site"] == BASE + 12 for row in result["closure_evidence"]["read_footprints"]))

    def test_coprocessor_and_status_reads_invalidate_their_destinations(self) -> None:
        for instruction, register in ((0xEE100F10, 0), (0xEC510F00, 0), (0xEC510F00, 1), (0xE10F0000, 0)):
            with self.subTest(instruction=instruction, register=register):
                image, layout = fixture([
                    0xE59F0008 | register << 12, instruction, 0xE1D020F0 | register << 16,
                    0xE12FFF1E, BASE + 24, 0xFFFFFFFF, 0x01010101, 0xFFFFFFFF])
                result = analyze.build_catalogue(image, layout, closure={})
                row = next(row for row in result["ranges"] if row["start"] <= BASE + 24 < row["end"])
                self.assertEqual((row["kind"], row["owner"]), ("unknown", None))

    def test_shifted_move_does_not_reintroduce_an_unmodeled_pointer_value(self) -> None:
        image, layout = fixture([
            0xE59F1008, 0xE1A00081, 0xE1D020F0, 0xE12FFF1E, BASE + 24,
            0xFFFFFFFF, 0x01010101, 0xFFFFFFFF])
        result = analyze.build_catalogue(image, layout, closure={})
        row = next(row for row in result["ranges"] if row["start"] <= BASE + 24 < row["end"])
        self.assertEqual((row["kind"], row["owner"]), ("unknown", None))

    def test_postindex_base_write_cannot_reuse_its_old_pointer(self) -> None:
        image, layout = fixture([
            0xE59F0010, 0xE4D01002, 0xE1D030F0, 0xE12FFF1E,
            0xFFFFFFFF, 0xFFFFFFFF, BASE + 32, 0xFFFFFFFF, 0x01010101, 0xFFFFFFFF])
        result = analyze.build_catalogue(image, layout, closure={})
        self.assertTrue(any(row["site"] == BASE + 4 for row in result["closure_evidence"]["read_footprints"]))
        self.assertFalse(any(row["site"] == BASE + 8 for row in result["closure_evidence"]["read_footprints"]))
        row = next(row for row in result["ranges"] if row["start"] <= BASE + 33 < row["end"])
        self.assertEqual((row["kind"], row["owner"]), ("unknown", None))

    def test_register_inventory_covers_actual_multi_outputs_and_writeback(self) -> None:
        for instruction, expected in (
            (0xEF000028, {"r0", "r1", "r2", "r3", "ip", "lr"}),
            (0xE0810392, {"r0", "r1"}), (0xE0C10392, {"r0", "r1"}),
            (0xE1C200D0, {"r0", "r1"}), (0xE0C200D8, {"r0", "r1", "r2"}),
            (0xE8920003, {"r0", "r1"}), (0xE8B20003, {"r0", "r1", "r2"}),
            (0xE1B20F9F, {"r0", "r1"}), (0xE1020091, {"r0"}),
            (0xE10F0000, {"r0"}), (0xEE100F10, {"r0"}), (0xEC510F00, {"r0", "r1"}),
            (0xE92D4010, {"sp"}), (0xE8BD4030, {"sp", "r4", "r5", "lr"})):
            with self.subTest(instruction=instruction):
                image, layout = fixture([instruction, 0xE12FFF1E])
                decoded = analyze.Analyzer(image, layout).detailed(BASE)
                writes = catalogue_closure._register_writes(decoded)
                self.assertIsNotNone(writes)
                self.assertLessEqual(expected, {decoded.reg_name(register) for register in writes})

    def test_register_inventory_uses_implicit_writes_and_refuses_unknown_effects(self) -> None:
        arm = analyze.arm
        register = SimpleNamespace(type=arm.ARM_OP_REG, reg=arm.ARM_REG_R1, access=1)
        unknown = SimpleNamespace(id=arm.ARM_INS_CDP, operands=[register], regs_write=[arm.ARM_REG_R6],
                                  regs_access=lambda: ([], []), writeback=False)
        self.assertEqual(catalogue_closure._register_writes(unknown), {arm.ARM_REG_R1, arm.ARM_REG_R6})
        unknown.operands = []
        unknown.regs_write = []
        self.assertIsNone(catalogue_closure._register_writes(unknown))
        memory = SimpleNamespace(type=arm.ARM_OP_MEM, mem=SimpleNamespace(base=arm.ARM_REG_R0), access=0)
        store = SimpleNamespace(id=arm.ARM_INS_STR, operands=[register, memory], regs_write=[],
                                regs_access=lambda: ([], []), writeback=True)
        self.assertEqual(catalogue_closure._register_writes(store), {arm.ARM_REG_R0})
        for opcode, operands in ((arm.ARM_INS_LDREXD, [register, memory]),
                                 (arm.ARM_INS_LDM, [register]), (arm.ARM_INS_MRRC, [register]),
                                 (arm.ARM_INS_LDR, [memory])):
            malformed = SimpleNamespace(id=opcode, operands=operands, regs_write=[],
                                        regs_access=lambda: ([], []), writeback=False)
            with self.subTest(malformed_opcode=opcode):
                self.assertIsNone(catalogue_closure._register_writes(malformed))

    def test_conditional_calls_discard_every_caller_saved_pointer_fact(self) -> None:
        for register in (0, 1, 2, 3, 12):
            for condition in (1, 14):
                with self.subTest(register=register, condition=condition):
                    image, layout = fixture([
                        0xE92D4010, 0xE59F000C | register << 12, 0xE3500000 | register << 16,
                        branch(BASE + 12, BASE + 32, link=True, condition=condition),
                        0xE5904000 | register << 16, 0xE8BD8010, BASE + 48, 0xFFFFFFFF,
                        0xE3A00000 | register << 12, 0xE12FFF1E, 0xFFFFFFFF, 0xFFFFFFFF, 0x01010101, 0xFFFFFFFF])
                    result = analyze.build_catalogue(image, layout, closure={})
                    row = next(row for row in result["ranges"] if row["start"] <= BASE + 48 < row["end"])
                    self.assertEqual((row["kind"], row["owner"]), ("unknown", None))
                    self.assertFalse(any(row["site"] == BASE + 16 for row in result["closure_evidence"]["read_footprints"]))

    def test_conditional_register_call_also_clobbers_pointer_facts(self) -> None:
        image, layout = fixture([
            0xE92D4030, 0xE59F0014, 0xE59F5014, 0xE3500000, 0x112FFF35, 0xE5904000, 0xE8BD8030,
            0xFFFFFFFF, BASE + 64, BASE + 48, 0xFFFFFFFF, 0xFFFFFFFF,
            0xE3A00000, 0xE12FFF1E, 0xFFFFFFFF, 0xFFFFFFFF, 0x01010101, 0xFFFFFFFF])
        result = analyze.build_catalogue(image, layout, closure={})
        row = next(row for row in result["ranges"] if row["start"] <= BASE + 64 < row["end"])
        self.assertEqual((row["kind"], row["owner"]), ("unknown", None))
        self.assertFalse(any(row["site"] == BASE + 20 for row in result["closure_evidence"]["read_footprints"]))

    def test_conditional_call_preserves_a_callee_saved_pointer_fact(self) -> None:
        image, layout = fixture([
            0xE92D4010, 0xE59F400C, 0xE3540000, branch(BASE + 12, BASE + 32, link=True, condition=1),
            0xE5941000, 0xE8BD8010, BASE + 48, 0xFFFFFFFF, 0xE3A00000, 0xE12FFF1E,
            0xFFFFFFFF, 0xFFFFFFFF, 0x01010101, 0xFFFFFFFF])
        result = analyze.build_catalogue(image, layout, closure={})
        row = next(row for row in result["ranges"] if row["start"] <= BASE + 48 < row["end"])
        self.assertEqual((row["kind"], row["owner"]), ("rodata", None))
        self.assertTrue(any(row["site"] == BASE + 16 for row in result["closure_evidence"]["read_footprints"]))

    def test_exception_return_forms_never_create_closed_copy_owners(self) -> None:
        for terminal in (0xE1B0F00E, 0xE25EF004, 0xE8FD8010, 0xE1A0F08E, 0xE1A0F06E):
            with self.subTest(terminal=terminal):
                image, layout = fixture([0xE12FFF1E, 0xE5912000, 0xE5802000, terminal])
                analyzer = analyze.Analyzer(image, layout)
                self.assertIsNone(catalogue_closure.constant_copy_leaf(analyzer, BASE + 4, BASE + 16))
                result = analyze.build_catalogue(image, layout, closure={})
                self.assertNotIn(BASE + 4, {function["address"] for function in result["functions"]})
        image, layout = fixture([0xE12FFF1E, 0xE5912000, 0xE5802000, 0xE1A0F00E])
        self.assertIsNotNone(catalogue_closure.constant_copy_leaf(analyze.Analyzer(image, layout), BASE + 4, BASE + 16))
        result = analyze.build_catalogue(image, layout, closure={})
        self.assertIn(BASE + 4, {function["address"] for function in result["functions"]})

    def test_signed_byte_and_halfword_reads_veto_alignment(self) -> None:
        for instruction, width in ((0xE1DF10BE, 2), (0xE1DF10FE, 2), (0xE5DF100E, 1), (0xE1DF10DE, 1)):
            with self.subTest(instruction=instruction, width=width):
                image, layout = fixture([instruction, 0xFA000001, 0xE12FFF1E, 0, 0x4801B500, 0x0000BD00, 0x12345678])
                result = analyze.build_catalogue(image, layout, closure={})
                row = next(row for row in result["ranges"] if row["start"] <= BASE + 22 < row["end"])
                self.assertEqual((row["kind"], row["owner"]), ("rodata", None))
                reads = [row for row in result["closure_evidence"]["read_footprints"] if row["site"] == BASE]
                self.assertEqual([(row["start"], row["end"]) for row in reads], [(BASE + 22, BASE + 22 + width)])
                self.assertFalse(any(row["start"] <= BASE + 22 < row["end"] for row in result["closure_evidence"]["alignment"]))

    def test_conditional_reads_veto_alignment_without_claiming_data(self) -> None:
        for instruction in (0x01DF10BE, 0x01DF10FE, 0x01DF10DE):
            with self.subTest(instruction=instruction):
                image, layout = fixture([instruction, 0xFA000001, 0xE12FFF1E, 0, 0x4801B500, 0x0000BD00, 0x12345678])
                result = analyze.build_catalogue(image, layout, closure={})
                row = next(row for row in result["ranges"] if row["start"] <= BASE + 22 < row["end"])
                self.assertEqual((row["kind"], row["owner"]), ("unknown", None))
                self.assertTrue(any(row["site"] == BASE for row in result["closure_evidence"]["read_vetoes"]))

    def test_unmodeled_and_unbounded_reads_do_not_prove_no_reference(self) -> None:
        image, layout = fixture([0xED9F0100, 0xFA000001, 0xE12FFF1E, 0, 0x4801B500, 0x0000BD00, 0x12345678])
        self.assertEqual(analyze.Analyzer(image, layout).detailed(BASE).id, analyze.arm.ARM_INS_LDC)
        result = analyze.build_catalogue(image, layout, closure={})
        row = next(row for row in result["ranges"] if row["start"] <= BASE + 22 < row["end"])
        self.assertEqual((row["kind"], row["owner"]), ("unknown", None))
        self.assertTrue(any(row["site"] == BASE for row in result["closure_evidence"]["read_vetoes"]))
        image, layout = fixture([0xE59F0014, 0xE7901002, 0xFA000000, 0xE12FFF1E, 0x4801B500,
                                 0x0000BD00, 0x12345678, BASE + 22])
        result = analyze.build_catalogue(image, layout, closure={})
        row = next(row for row in result["ranges"] if row["start"] <= BASE + 22 < row["end"])
        self.assertEqual((row["kind"], row["owner"]), ("unknown", None))
        self.assertTrue(any(row["site"] == BASE + 4 for row in result["closure_evidence"]["read_vetoes"]))

    def test_read_inventory_widths_cover_signed_unprivileged_atomic_and_doubleword_forms(self) -> None:
        arm = analyze.arm
        register = SimpleNamespace(type=arm.ARM_OP_REG, reg=arm.ARM_REG_R1)
        memory = SimpleNamespace(type=arm.ARM_OP_MEM, access=0, subtracted=False,
                                 mem=SimpleNamespace(base=arm.ARM_REG_R0, index=0, disp=6),
                                 shift=SimpleNamespace(type=0, value=0))
        known = {arm.ARM_REG_R0: (BASE, BASE, "synthetic")}
        for width, names in ((1, ("LDRB", "LDRBT", "LDRSB", "LDRSBT", "LDREXB", "SWPB")),
                             (2, ("LDRH", "LDRHT", "LDRSH", "LDRSHT", "LDREXH")),
                             (4, ("LDR", "LDRT", "LDREX", "SWP")), (8, ("LDRD", "LDREXD"))):
            for name in names:
                with self.subTest(name=name):
                    operands = [register, register, memory] if width == 8 else [register, memory]
                    instruction = SimpleNamespace(id=getattr(arm, "ARM_INS_" + name), operands=operands)
                    reads, uncertain = catalogue_closure._instruction_reads(instruction, known, BASE, BASE + 8)
                    self.assertEqual(reads, [(BASE + 6, BASE + 6 + width, BASE, "synthetic")])
                    self.assertFalse(uncertain)
        for reg, width in ((arm.ARM_REG_S0, 4), (arm.ARM_REG_D0, 8)):
            instruction = SimpleNamespace(id=arm.ARM_INS_VLDR,
                                          operands=[SimpleNamespace(type=arm.ARM_OP_REG, reg=reg), memory])
            reads, uncertain = catalogue_closure._instruction_reads(instruction, known, BASE, BASE + 8)
            self.assertEqual(reads[0][:2], (BASE + 6, BASE + 6 + width))
            self.assertFalse(uncertain)

    def test_multiple_read_address_modes_use_complete_architectural_spans(self) -> None:
        arm = analyze.arm
        reg = lambda value: SimpleNamespace(type=arm.ARM_OP_REG, reg=value)
        known = {arm.ARM_REG_R0: (BASE + 16, BASE, "synthetic"), arm.ARM_REG_SP: (BASE + 16, BASE, "synthetic")}
        for name, registers, offset, width in (
            ("LDM", [arm.ARM_REG_R0, arm.ARM_REG_R1, arm.ARM_REG_R2], 0, 8),
            ("LDMIB", [arm.ARM_REG_R0, arm.ARM_REG_R1, arm.ARM_REG_R2], 4, 8),
            ("LDMDA", [arm.ARM_REG_R0, arm.ARM_REG_R1, arm.ARM_REG_R2], -4, 8),
            ("LDMDB", [arm.ARM_REG_R0, arm.ARM_REG_R1, arm.ARM_REG_R2], -8, 8),
            ("VLDMIA", [arm.ARM_REG_R0, arm.ARM_REG_S0, arm.ARM_REG_S1], 0, 8),
            ("VLDMDB", [arm.ARM_REG_R0, arm.ARM_REG_D0, arm.ARM_REG_D1], -16, 16),
            ("POP", [arm.ARM_REG_R1, arm.ARM_REG_R2], 0, 8),
            ("VPOP", [arm.ARM_REG_D0, arm.ARM_REG_D1], 0, 16)):
            with self.subTest(name=name):
                instruction = SimpleNamespace(id=getattr(arm, "ARM_INS_" + name), operands=[reg(value) for value in registers])
                reads, uncertain = catalogue_closure._instruction_reads(instruction, known, BASE, BASE + 8)
                self.assertEqual(reads[0][:2], (BASE + 16 + offset, BASE + 16 + offset + width))
                self.assertFalse(uncertain)

    def test_partially_mapped_reads_veto_without_claiming_a_truncated_object(self) -> None:
        image, layout = fixture([0xE12FFF1E])
        catalogue = analyze.build_catalogue(image, layout)
        analyzer = analyze.Analyzer(image, layout)
        vetoes = []
        with mock.patch.object(catalogue_closure, "_instruction_reads",
                               return_value=([(BASE - 2, BASE + 2, BASE, "synthetic")], False)):
            reads = catalogue_closure.read_footprints(catalogue, analyzer, read_vetoes=vetoes)
        self.assertEqual(reads, [])
        self.assertEqual([(row["start"], row["end"]) for row in vetoes], [(BASE, BASE + 2)])
        self.assertEqual(vetoes[0]["evidence"], "partially_mapped_read_veto_only")

    def test_unknown_memory_read_shapes_and_missing_access_flags_fail_closed(self) -> None:
        arm = analyze.arm
        register = SimpleNamespace(type=arm.ARM_OP_REG, reg=arm.ARM_REG_R1)
        memory = SimpleNamespace(type=arm.ARM_OP_MEM, access=0, subtracted=False,
                                 mem=SimpleNamespace(base=arm.ARM_REG_R0, index=0, disp=0),
                                 shift=SimpleNamespace(type=0, value=0))
        known = {arm.ARM_REG_R0: (BASE, BASE, "synthetic")}
        for instruction in (SimpleNamespace(id=arm.ARM_INS_LDC, operands=[register, memory]),
                            SimpleNamespace(id=arm.ARM_INS_CDP, operands=[register, memory]),
                            SimpleNamespace(id=arm.ARM_INS_LDRSH, operands=[register]),
                            SimpleNamespace(id=arm.ARM_INS_LDM, operands=[register])):
            with self.subTest(instruction=instruction.id):
                self.assertEqual(catalogue_closure._instruction_reads(instruction, known, BASE, BASE + 8), ([], True))
        # A known, store-only class does not become a read just because flags are absent.
        instruction = SimpleNamespace(id=arm.ARM_INS_STR, operands=[register, memory])
        self.assertEqual(catalogue_closure._instruction_reads(instruction, known, BASE, BASE + 8), ([], False))
        memory.mem.index = arm.ARM_REG_R2
        instruction = SimpleNamespace(id=arm.ARM_INS_LDR, operands=[register, memory])
        self.assertEqual(catalogue_closure._instruction_reads(instruction, known, BASE, BASE + 8), ([], True))
        known[arm.ARM_REG_R2] = (3, BASE, "synthetic")
        memory.shift = SimpleNamespace(type=arm.ARM_SFT_LSL, value=1)
        reads, uncertain = catalogue_closure._instruction_reads(instruction, known, BASE, BASE + 8)
        self.assertEqual(reads[0][:2], (BASE + 6, BASE + 10))
        self.assertFalse(uncertain)
        memory.shift = SimpleNamespace(type=arm.ARM_SFT_RRX, value=1)
        self.assertEqual(catalogue_closure._instruction_reads(instruction, known, BASE, BASE + 8), ([], True))

    def test_complete_copy_effects_do_not_claim_library_ownership(self) -> None:
        for load, store, width in ((0xE5912000, 0xE5802000, 4), (0xE5D12000, 0xE5C02000, 1)):
            with self.subTest(width=width):
                leaf = self.leaf([load, store, 0xE12FFF1E])
                self.assertIsNotNone(leaf)
                self.assertEqual((leaf["copy_bytes"], leaf["source_bytes"], leaf["instruction_count"]), (width, width, 3))
                self.assertFalse(leaf["ownership_classified"])

    def test_record_permutation_requires_its_explicit_memory_contract(self) -> None:
        words = [0xE5912004, 0xE5802000, 0xE5912000, 0xE5802004, 0xE12FFF1E]
        self.assertIsNone(self.leaf(words))
        leaf = self.leaf(words, record=True)
        self.assertEqual((leaf["source_bytes"], leaf["copy_bytes"]), (8, 8))
        self.assertEqual([field["source_offset"] for field in leaf["fields"]], [4, 0])
        constants = self.leaf([0xE5913000, 0xE5803000, 0xE3A02007, 0xE5802004, 0xE12FFF1E], record=True)
        self.assertEqual([field["kind"] for field in constants["fields"]], ["source", "constant"])

    def test_copy_witness_fails_closed_for_unproved_effects(self) -> None:
        bad = {
            "predicate": [0x05912000, 0xE5802000, 0xE12FFF1E],
            "call": [branch(BASE, BASE + 8, link=True), 0xE5802000, 0xE12FFF1E],
            "indexed": [0xE7912003, 0xE5802000, 0xE12FFF1E],
            "callee_clobber": [0xE5914000, 0xE5804000, 0xE12FFF1E],
            "unbalanced_stack": [0xE92D0010, 0xE5912000, 0xE5802000, 0xE12FFF1E],
            "source_write": [0xE5912000, 0xE5812000, 0xE12FFF1E],
            "flags": [0xE5912000, 0xE5802000, 0xE2900001, 0xE12FFF1E],
            "missing_return": [0xE5912000, 0xE5802000],
            "preserved_vfp_alias": [0xED910B00, 0xED800B00, 0xE12FFF1E],
        }
        for reason, words in bad.items():
            with self.subTest(reason=reason):
                self.assertIsNone(self.leaf(words, record=True))

    def test_nonlocal_string_never_borrows_a_neighbors_pool(self) -> None:
        image, layout = fixture([0xE28F0008, 0xE12FFF1E, 0xE92D4010, 0xE8BD8010, *literal_words(b"foo\0")])
        result = analyze.build_catalogue(image, layout)
        self.assertEqual([function["size"] for function in result["functions"]], [8, 8])
        self.assertIn({"start": BASE + 16, "end": BASE + 20, "kind": "rodata", "owner": None}, result["ranges"])
        image, layout = fixture([0xE28F0010, 0xE12FFF1E, 0xE92D4010, 0xE59F0004, 0xE8BD8010, 0,
                                 *literal_words(b"foo\0")])
        result = analyze.build_catalogue(image, layout)
        row = next(row for row in result["ranges"] if row["start"] <= BASE + 24 < row["end"])
        # Sharing vetoes exclusive pool ownership before optional closure.
        self.assertEqual((row["kind"], row["owner"]), ("unknown", None))
        self.assertEqual(result["functions"][1]["pools"], [])
        self.assertEqual(result["functions"][0]["size"], 8)
        # The actual LDR then proves four unowned data bytes, not a borrowed pool.
        closed = analyze.build_catalogue(image, layout, closure={})
        row = next(row for row in closed["ranges"] if row["start"] <= BASE + 24 < row["end"])
        self.assertEqual((row["kind"], row["owner"]), ("rodata", None))
        self.assertEqual(closed["functions"][1]["pools"], [])

    def test_reviewed_wide_bound_requires_digest_terminator_and_zero_alignment(self) -> None:
        image, layout = fixture([0xE28F0000, 0xE12FFF1E, *literal_words("AB\0".encode("utf-16le"))])
        row = {"function": BASE, "target": BASE + 8, "end": BASE + 14, "references": [BASE],
               "encoding": "utf-16le", "image_interval_sha256": hashlib.sha256(image[8:14]).hexdigest()}
        result = analyze.build_catalogue(image, layout, literal_encodings=[row])
        self.assertEqual(result["functions"][0]["literals"], [[BASE + 8, BASE + 16]])
        for change in ({"end": BASE + 16}, {"image_interval_sha256": "0" * 64}):
            with self.subTest(change=change), self.assertRaises(ValueError):
                analyze.build_catalogue(image, layout, literal_encodings=[{**row, **change}])
        with self.assertRaises(ValueError):
            analyze.build_catalogue(image[:14] + b"\1" + image[15:], layout, literal_encodings=[row])

    def test_dominated_thumb_pc_pointer_requires_the_actual_defining_value(self) -> None:
        image, layout = fixture([0xFA000002, 0xE12FFF1E, 0, 0, 0x4478200B, 0x00004770, 0, 0, 0x47702007])
        result = analyze.build_catalogue(image, layout)
        self.assertIn(BASE + 32, {function["address"] for function in result["functions"]})
        self.assertTrue(any(row["target"] == BASE + 32 for row in result["pc_relative_pointer_witnesses"]))
        image, layout = fixture([0xFA000002, 0xE12FFF1E, 0, 0, 0x2000200B, 0x47704478, 0, 0, 0x47702007])
        result = analyze.build_catalogue(image, layout)
        self.assertNotIn(BASE + 32, {function["address"] for function in result["functions"]})

    def test_only_unreferenced_zero_halfwords_can_align_thumb_pools(self) -> None:
        for fill, expected in ((0, "padding"), (0xFFFF, "unknown")):
            with self.subTest(fill=fill):
                image, layout = fixture([0xFA000002, 0xE12FFF1E, 0, 0, 0x4801B500,
                                         (fill << 16) | 0xBD00, 0x12345678])
                result = analyze.build_catalogue(image, layout, closure={})
                row = next(row for row in result["ranges"] if row["start"] <= BASE + 22 < row["end"])
                self.assertEqual(row["kind"], expected)
        image, layout = fixture([0xE1DF10BE, 0xFA000001, 0xE12FFF1E, 0, 0x4801B500, 0x0000BD00, 0x12345678])
        result = analyze.build_catalogue(image, layout, closure={})
        row = next(row for row in result["ranges"] if row["start"] <= BASE + 22 < row["end"])
        self.assertEqual((row["kind"], row["owner"]), ("rodata", None))

    def graph_fixture(self, first_word: int = 0xE3520000) -> tuple:
        words = [first_word, branch(BASE + 4, BASE + 16, condition=0), 0xE92D4010, 0xE8BD8010,
                 0xE92D4010, 0xE8BD8010]
        image, layout = fixture(words)
        analyzer = analyze.Analyzer(image, layout)
        functions = []
        for address in (BASE + 8, BASE + 16):
            analyzer.add_seed(address, "arm", "prologue")
            functions.append({"address": address, "size": 8, "code_size": 8, "isa": "arm", "kind": "function",
                              "evidence": ["prologue"], "code_ranges": [[address, address + 8]], "pools": [],
                              "literals": [], "noreturn": False, "noreturn_reasons": [],
                              "entries": [{"address": address, "isa": "arm", "kind": "entry",
                                           "evidence": ["prologue"], "sources": []}]})
        catalogue = {"functions": functions, "stats": {}, "ranges": [
            {"start": BASE, "end": BASE + 8, "kind": "unknown", "owner": None},
            *({"start": function["address"], "end": function["address"] + 8,
               "kind": "function", "owner": function["address"]} for function in functions)]}
        record = {"start": BASE, "end": BASE + 24, "legacy_owners": [BASE + 8, BASE + 16],
                  "image_interval_sha256": hashlib.sha256(image[:24]).hexdigest(),
                  "behavior_witness": {"identity": "synthetic"}, "behavior_cases": 1, "behavior_copy_and_abi_passes": 1}
        return catalogue, analyzer, record

    def test_entry_connected_graph_preserves_weak_labels_as_topology_not_ownership(self) -> None:
        catalogue, analyzer, record = self.graph_fixture()
        old = json.loads(json.dumps(catalogue["functions"]))
        rows = catalogue_closure.merge_closed_graphs(catalogue, analyzer, [record], {})
        function = catalogue["functions"][0]
        self.assertEqual((function["kind"], function["size"]), ("multi_entry", 24))
        self.assertEqual([entry["address"] for entry in function["entries"]], [BASE, BASE + 8, BASE + 16])
        self.assertFalse(rows[0]["ownership_classified"])
        analyze.preserve_prior_entries(catalogue, old)
        analyze.preserve_prior_entries(catalogue, json.loads(json.dumps(catalogue["functions"])))
        self.assertEqual(len(catalogue["functions"][0]["entries"]), 3)

    def test_graph_merge_rejects_protected_strong_data_and_unclosed_members(self) -> None:
        for reason in ("protected", "strong_entry", "pool", "digest", "missing_behavior", "system_instruction"):
            with self.subTest(reason=reason):
                catalogue, analyzer, record = self.graph_fixture(0xEF000000 if reason == "system_instruction" else 0xE3520000)
                protected = {}
                if reason == "protected":
                    protected[BASE + 8] = catalogue["functions"][0]
                elif reason == "strong_entry":
                    catalogue["functions"][0]["entries"][0]["sources"] = [BASE + 0x100]
                elif reason == "pool":
                    catalogue["functions"][0]["pools"] = [[BASE + 12, BASE + 16]]
                elif reason == "digest":
                    record["image_interval_sha256"] = "0" * 64
                elif reason == "missing_behavior":
                    record["behavior_cases"] = 0
                with self.assertRaises(ValueError):
                    catalogue_closure.merge_closed_graphs(catalogue, analyzer, [record], protected)

    def test_classification_checks_the_entire_original_extent_identity(self) -> None:
        image, layout = fixture([0xE12FFF1E])
        row = {"address": BASE, "size": 4, "isa": "arm", "image_sha256": hashlib.sha256(image).hexdigest(),
               "kind": "runtime", "basis": "compiler_abi", "identity": "synthetic ABI helper",
               "evidence": "synthetic compiler/ABI witness", "compiler_emission": "synthetic emitted helper",
               "original_extent_sha256": "0" * 64}
        with self.assertRaises(ValueError):
            analyze.build_catalogue(image, layout, classifications=[row])

    def test_frozen_baseline_and_parent_are_independently_authenticated(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "research").mkdir()
            parent = root / "research/parent.json"
            parent.write_text("{}")
            baseline = json.dumps({"image_sha256": "synthetic"}).encode()
            evidence = {"image_sha256": "synthetic",
                        "parent_evidence": {"path": "research/parent.json", "sha256": hashlib.sha256(parent.read_bytes()).hexdigest()},
                        "baseline_catalogue": {"blob": "1" * 40, "reference": "synthetic", "revision": "prior",
                                               "sha256": hashlib.sha256(baseline).hexdigest()}}
            path = root / "research/catalogue_v10_evidence.json"
            path.write_text(json.dumps(evidence))
            with mock.patch.object(catalogue_closure.subprocess, "run",
                                   return_value=subprocess.CompletedProcess([], 0, baseline, b"")):
                _, prior, inputs = catalogue_closure.load_inputs(root, parent)
                self.assertEqual(prior["revision"], "prior")
                self.assertIn("research/catalogue_v10_evidence.json", inputs)
                for field in ("parent_evidence", "baseline_catalogue"):
                    changed = json.loads(json.dumps(evidence))
                    changed[field]["sha256"] = "0" * 64
                    path.write_text(json.dumps(changed))
                    with self.subTest(field=field), self.assertRaises(ValueError):
                        catalogue_closure.load_inputs(root, parent)


if __name__ == "__main__":
    unittest.main()
