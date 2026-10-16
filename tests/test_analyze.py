"""Hand-encoded synthetic ARM/Thumb fixtures; no original executable bytes."""
from __future__ import annotations

import hashlib
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

    def test_nop_entry_and_body_are_one_non_c_group(self) -> None:
        result = self.catalogue([branch(BASE, BASE + 16, link=True), branch(BASE + 4, BASE + 20, link=True),
                                 0xE12FFF1E, 0, 0xE1A00000, 0xE3A00001, 0xE12FFF1E])
        function = result["functions"][1]
        self.assertEqual((function["address"], function["size"], function["kind"]),
                         (BASE + 16, 12, "multi_entry"))
        self.assertEqual([(entry["address"], entry["kind"]) for entry in function["entries"]],
                         [(BASE + 16, "nop_entry"), (BASE + 20, "entry")])
        self.assertEqual(result["stats"]["nop_entries"], 1)
        self.assertEqual(result["entry_groups"],
                         [{"address": BASE + 16, "members": [BASE + 16, BASE + 20], "reason": "fallthrough"}])

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
        first = analyze.build_catalogue(image, layout)
        second = analyze.build_catalogue(image, layout)
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
        self.assertEqual((owner["address"], owner["kind"]), (0x18F7EC, "multi_entry"))
        self.assertTrue({0x11FA34, 0x11FFDC} <= set(entry["sources"]))
        owner, entry = entry_owners[0x12D1D8]
        self.assertEqual(owner["address"], 0x12D1CC)
        self.assertIn(0x135054, entry["sources"])
        owner, entry = entry_owners[0x1852F4]
        self.assertEqual((owner["kind"], entry["kind"]), ("multi_entry", "nop_entry"))


if __name__ == "__main__":
    unittest.main()
