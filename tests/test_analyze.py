"""Hand-encoded synthetic ARM/Thumb fixtures; no original executable bytes."""
from __future__ import annotations

import hashlib
import json
from pathlib import Path
import struct
import unittest

from tools import analyze, ctrfmt

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

    def test_shared_adr_string_remains_unknown(self) -> None:
        for second_target in (32, 39):
            with self.subTest(second_target=second_target):
                image, layout = fixture([branch(BASE, BASE + 16, link=True), 0xE28F0014, 0xE12FFF1E, 0,
                                         0xE28F0000 | (second_target - 24), 0xE12FFF1E, 0, 0,
                                         *literal_words(b"shared-value\0")])
                result = analyze.build_catalogue(image, layout)
                self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
                self.assertTrue(all(not function["literals"] and not function["pools"]
                                    for function in result["functions"]))
                self.assertEqual(sum("shared_address_literal" in item["reasons"]
                                     for item in result["literal_conflicts"]), 2)
                self.assertTrue(all(item["kind"] == "unknown" and item["owner"] is None
                                    for item in result["ranges"] if BASE + 32 <= item["start"] < BASE + 48))
                self.assertEqual(result["stats"]["bytes_by_range_kind"]["unknown"], 16)

    def test_string_shared_through_another_functions_pool(self) -> None:
        image, layout = fixture([branch(BASE, BASE + 16, link=True), 0xE28F0014, 0xE12FFF1E, 0,
                                 0xE59F0000, 0xE12FFF1E, BASE + 32, 0, *literal_words(b"shared\0")])
        result = analyze.build_catalogue(image, layout)
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertTrue(all(not function["literals"] for function in result["functions"]))
        self.assertIn("shared_address_literal", result["literal_conflicts"][0]["reasons"])
        self.assertEqual(result["functions"][1]["pools"], [[BASE + 24, BASE + 28]])

    def test_external_data_pointer_prevents_string_absorption(self) -> None:
        image, layout = fixture([0xE28F0000, 0xE12FFF1E, *literal_words(b"external\0")],
                                 rodata=[BASE + 8])
        result = analyze.build_catalogue(image, layout)
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertEqual(result["functions"][0]["literals"], [])
        self.assertEqual(result["functions"][0]["size"], 8)
        self.assertIn("shared_address_literal", result["literal_conflicts"][0]["reasons"])

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

    @unittest.skipUnless((ROOT / "orig/code.bin").is_file() and (ROOT / "orig/exheader.bin").is_file(), "private inputs absent")
    def test_original_invariants_and_stability(self) -> None:
        image = (ROOT / "orig/code.bin").read_bytes()
        layout = ctrfmt.parse_exheader((ROOT / "orig/exheader.bin").read_bytes())
        protected_addresses = {int(name.removeprefix("func_"), 16)
                               for path in ("config/matches.json", "config/candidates/c004.json")
                               for unit in json.loads((ROOT / path).read_text(encoding="utf-8"))["units"]
                               for name in unit["functions"]}
        adopted = json.loads((ROOT / "config/catalogue.json").read_text(encoding="utf-8"))
        adopted_functions = {function["address"]: function for function in adopted["functions"]}
        protected = {address: adopted_functions[address] for address in protected_addresses}
        first = analyze.build_catalogue(image, layout, protected_functions=protected)
        second = analyze.build_catalogue(image, layout, protected_functions=protected)
        self.assertEqual(first["stats"], second["stats"])
        self.assertEqual(analyze.serialize_catalogue(first), analyze.serialize_catalogue(second))
        self.assertEqual(analyze.validate_catalogue(first, image, layout), [])
        self.assertEqual(sum(item["end"] - item["start"] for item in first["ranges"]), len(image))
        self.assertGreater(first["stats"]["functions"], 100)
        strings = next(function for function in first["functions"] if function["address"] == 0x1932E8)
        self.assertEqual(strings["size"], 300)
        self.assertEqual(strings["literals"], [[0x193390, 0x193414]])
        entry_owners = {entry["address"]: (function, entry) for function in first["functions"]
                        for entry in function["entries"]}
        owner, entry = entry_owners[0x18F7F0]
        self.assertEqual((owner["address"], owner["kind"]), (0x18F7F0, "function"))
        self.assertTrue({0x11FA34, 0x11FFDC} <= set(entry["sources"]))
        owner, entry = entry_owners[0x12D1D8]
        self.assertEqual((owner["address"], owner["kind"]), (0x12D1D8, "function"))
        self.assertIn(0x135054, entry["sources"])
        owner, entry = entry_owners[0x1852F4]
        self.assertEqual((owner["kind"], entry["kind"], owner["size"]), ("function", "entry", 4))
        functions = {function["address"]: function for function in first["functions"]}
        self.assertEqual((functions[0x186EEC]["kind"], functions[0x186EEC]["size"]), ("function", 4))
        self.assertIn("pointer", functions[0x186EEC]["evidence"])
        self.assertEqual((functions[0x1EF884]["kind"], functions[0x1EF884]["size"]), ("function", 4))
        self.assertIn("pointer", functions[0x1EF884]["evidence"])
        self.assertEqual(functions[0x149428]["kind"], "function")
        self.assertIn("pointer", functions[0x149428]["evidence"])
        for address in (0x195EC4, 0x180768, 0x180884, 0x149428):
            self.assertEqual(entry_owners[address][1]["isa"], "arm")
        adopted_entries = {(entry["address"], entry["isa"]) for function in adopted["functions"]
                           for entry in function.get("entries", [function])}
        recovered_entries = {(address, entry["isa"]) for address, (_, entry) in entry_owners.items()}
        self.assertEqual(sorted(adopted_entries - recovered_entries), [])
        for address in protected:
            self.assertEqual(functions[address], adopted_functions[address])
        self.assertEqual((functions[0x112C80]["kind"], functions[0x112C80]["size"]), ("function", 24))
        self.assertEqual((functions[0x112C98]["kind"], functions[0x112C98]["size"]), ("function", 48))
        self.assertEqual(len(first["linker_tail_inventory"]), 270)
        expected_splits = {
            0x15274C: (12, 0x152758, 20),
            0x15E474: (12, 0x15E480, 28),
            0x15E49C: (12, 0x15E4A8, 96),
            0x15E5C4: (16, 0x15E5D4, 28),
            0x15E624: (16, 0x15E634, 80),
            0x15E684: (20, 0x15E698, 28),
            0x1D2C44: (52, 0x1D2C78, 12),
            0x1D2CF0: (36, 0x1D2D14, 12),
        }
        for address, (head_size, target, tail_size) in expected_splits.items():
            with self.subTest(address=address):
                self.assertEqual((functions[address]["kind"], functions[address]["size"]), ("function", head_size))
                self.assertEqual((functions[target]["kind"], functions[target]["size"]), ("function", tail_size))
        self.assertEqual({address for address, function in adopted_functions.items() if functions[address] != function},
                         {address for address, row in expected_splits.items()
                          if adopted_functions[address]["size"] != row[0]})
        self.assertEqual(set(functions) - set(adopted_functions),
                         {row[1] for row in expected_splits.values()} - set(adopted_functions))
        self.assertEqual(len(first["linker_tail_extent_splits"]), 137)
        self.assertEqual(first["stats"]["linker_tail_census"], adopted["stats"]["linker_tail_census"])
        self.assertEqual(first["stats"]["linker_nop_words"], 776)
        nop_inventory = first["linker_nop_inventory"]
        self.assertEqual([row["site"] for row in nop_inventory],
                         [layout.text.address + offset for offset in range(0, layout.text.size - 3, 4)
                          if struct.unpack_from("<I", image, offset)[0] == 0xE1A00000])
        self.assertEqual(sum(row["decision"] == "in_frame_not_tail" for row in nop_inventory), 505)
        self.assertEqual(sum(row["decision"] == "not_executed_arm_code" for row in nop_inventory), 1)
        for field in ("code_bytes", "pool_bytes", "literal_bytes", "bytes_by_range_kind", "anomalies_by_issue"):
            self.assertEqual(first["stats"][field], adopted["stats"][field])
        self.assertEqual(sum(function["size"] for function in first["functions"]),
                         sum(function["size"] for function in adopted["functions"]))
        for split in first["linker_tail_splits"]:
            self.assertEqual((functions[split["address"]]["kind"], functions[split["address"]]["size"]),
                             ("function", 4))
            self.assertEqual(functions[split["target"]]["kind"], "function")


if __name__ == "__main__":
    unittest.main()
