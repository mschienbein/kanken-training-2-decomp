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
        self.assertEqual(result["functions"][0]["pools"], [[BASE + 8, BASE + 12]])

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
                self.assertEqual(function["pools"], [[BASE + 20, BASE + 24]])

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


if __name__ == "__main__":
    unittest.main()
