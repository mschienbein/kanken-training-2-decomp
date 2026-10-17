"""Catalogue-bounded display of synthetic instructions and data in tools/match.py."""
import struct
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import match  # noqa: E402

BASE = 0x10007C


class RenderTest(unittest.TestCase):
    def test_midword_literal_and_padding(self):
        data = struct.pack("<4I", *([0xE12FFF1E] * 4))
        literals = [(BASE + 9, BASE + 12)]
        for code_end in (BASE + 8, BASE + 9):
            with self.subTest(code_end=code_end):
                ranges = [(BASE, code_end), (BASE + 12, BASE + 16)]
                rows = match.render(data, BASE, "arm", ranges, literals)
                self.assertEqual([(a, n) for a, n, _ in rows], [
                    (BASE, 4), (BASE + 4, 4), (BASE + 8, 1), (BASE + 9, 3), (BASE + 12, 4)])
                self.assertEqual(rows[2][2], ".word 0x1e")
                self.assertEqual(rows[3][2], ".word 0xe12fff")
                self.assertEqual(rows[4][2], "bx lr")

    def test_pool_word_and_unlabelled_padding(self):
        data = struct.pack("<4I", 0xE12FFF1E, 0x12345678, 0xE12FFF1E, 0xE12FFF1E)
        rows = match.render(data, BASE, "arm", [(BASE, BASE + 4), (BASE + 12, BASE + 16)],
                            [(BASE + 4, BASE + 8)])
        self.assertEqual([row[2] for row in rows], [
            "bx lr", ".word 0x12345678", ".word 0xe12fff1e", "bx lr"])

    def test_partial_pool_does_not_consume_following_code(self):
        data = b"\x00\x00" + struct.pack("<H", 0x4770)
        rows = match.render(data, BASE, "thumb", [(BASE + 2, BASE + 4)], [(BASE, BASE + 2)])
        self.assertEqual(rows, [(BASE, 2, ".word 0x0000"), (BASE + 2, 2, "bx lr")])

    def test_thumb_wide_instruction_cannot_cross_boundary(self):
        data = bytes.fromhex("00f000f8")  # synthetic Thumb BL
        for stop, expected in (
            (1, [(BASE, 1), (BASE + 1, 3)]),
            (2, [(BASE, 2), (BASE + 2, 2)]),
            (3, [(BASE, 2), (BASE + 2, 1), (BASE + 3, 1)]),
        ):
            with self.subTest(stop=stop):
                rows = match.render(data, BASE, "thumb", [(BASE, BASE + stop)],
                                    [(BASE + stop, BASE + 4)])
                self.assertEqual([(a, n) for a, n, _ in rows], expected)
                self.assertTrue(all(text.startswith((".inst", ".word")) for _, _, text in rows))

    def test_candidate_bytes_outside_catalogue_are_data(self):
        data = struct.pack("<2I", 0xE12FFF1E, 0xE12FFF1E)
        rows = match.render(data, BASE, "arm", [(BASE, BASE + 4)], [])
        self.assertEqual(rows[1], (BASE + 4, 4, ".word 0xe12fff1e"))

    def test_adjacent_literal_and_pool_stay_separate(self):
        data = struct.pack("<5I", *([0xE12FFF1E] * 5))
        rows = match.render(data, BASE, "arm", [(BASE, BASE + 8), (BASE + 16, BASE + 20)],
                            [(BASE + 12, BASE + 16), (BASE + 9, BASE + 12)])
        self.assertEqual(rows[2:5], [
            (BASE + 8, 1, ".word 0x1e"),
            (BASE + 9, 3, ".word 0xe12fff"),
            (BASE + 12, 4, ".word 0xe12fff1e")])
        self.assertEqual(rows[-1], (BASE + 16, 4, "bx lr"))

    def test_unaligned_code_ranges_only_decode_complete_words(self):
        data = struct.pack("<4I", *([0xE12FFF1E] * 4))
        rows = match.render(data, BASE, "arm", [(BASE + 1, BASE + 7), (BASE + 8, BASE + 16)], [])
        self.assertEqual([(a, n) for a, n, _ in rows], [
            (BASE, 1), (BASE + 1, 3), (BASE + 4, 3), (BASE + 7, 1),
            (BASE + 8, 4), (BASE + 12, 4)])
        self.assertTrue(all(text.startswith(".word") for _, _, text in rows[:4]))
        self.assertEqual([text for _, _, text in rows[4:]], ["bx lr", "bx lr"])

    def test_thumb_narrow_and_wide_instructions_fit_code_ranges(self):
        data = bytes.fromhex("704700f000f87047") + struct.pack("<I", 0x12345678) + bytes.fromhex("7047")
        rows = match.render(data, BASE, "thumb", [(BASE, BASE + 8), (BASE + 12, BASE + 14)],
                            [(BASE + 8, BASE + 12)])
        self.assertEqual([(a, n) for a, n, _ in rows], [
            (BASE, 2), (BASE + 2, 4), (BASE + 6, 2), (BASE + 8, 4), (BASE + 12, 2)])
        self.assertEqual([text.split()[0] for _, _, text in rows], ["bx", "bl", "bx", ".word", "bx"])

    def test_thumb_wide_instruction_cannot_cross_adjacent_code_ranges(self):
        rows = match.render(bytes.fromhex("00f000f8"), BASE, "thumb",
                            [(BASE, BASE + 2), (BASE + 2, BASE + 4)], [])
        self.assertEqual(rows, [(BASE, 2, ".inst 0xf000"), (BASE + 2, 2, ".inst 0xf800")])

    def test_data_overrides_overlapping_code_range(self):
        data = struct.pack("<4I", *([0xE12FFF1E] * 4))
        rows = match.render(data, BASE, "arm", [(BASE, BASE + 16)],
                            [(BASE + 4, BASE + 8), (BASE + 9, BASE + 12)])
        self.assertEqual([(a, n) for a, n, _ in rows], [
            (BASE, 4), (BASE + 4, 4), (BASE + 8, 1), (BASE + 9, 3), (BASE + 12, 4)])
        self.assertEqual([text.split()[0] for _, _, text in rows], ["bx", ".word", ".word", ".word", "bx"])

    def test_image_end_clips_partial_literal(self):
        data = struct.pack("<3I", *([0xE12FFF1E] * 3))[:10]
        rows = match.render(data, BASE, "arm", [(BASE, BASE + 8)], [(BASE + 9, BASE + 12)])
        self.assertEqual(rows[-2:], [(BASE + 8, 1, ".word 0x1e"), (BASE + 9, 1, ".word 0xff")])


class DiffLinesTest(unittest.TestCase):
    def test_midword_literal_is_data_on_both_sides(self):
        original = struct.pack("<3I", *([0xE12FFF1E] * 3))
        candidate = struct.pack("<3I", *([0xE1A00000] * 3))
        fn = {"isa": "arm", "code_ranges": [(BASE, BASE + 8)],
              "pools": [], "literals": [(BASE + 9, BASE + 12)]}
        lines = match.diff_lines(original, candidate, BASE, fn, 20)
        self.assertEqual(len(lines), 4)
        for index, offset, left, right in (
            (2, 8, ".word 0x1e", ".word 0x00"),
            (3, 9, ".word 0xe12fff", ".word 0xe1a000"),
        ):
            self.assertEqual(lines[index], f"! {BASE + offset:08x}  {left:<40} {right}")

    def test_thumb_width_changes_keep_data_aligned_on_both_sides(self):
        narrow = bytes.fromhex("70477047") + struct.pack("<I", 0x12345678) + bytes.fromhex("7047")
        wide = bytes.fromhex("00f000f8") + narrow[4:]
        fn = {"isa": "thumb", "kind": "multi_entry",
              "entries": [{"address": BASE, "isa": "thumb"}, {"address": BASE + 8, "isa": "thumb"}],
              "code_ranges": [(BASE, BASE + 4), (BASE + 8, BASE + 10)],
              "pools": [(BASE + 4, BASE + 8)], "literals": []}
        for original, candidate, left, right in (
            (narrow, wide, "bx lr", ""), (wide, narrow, "", "bx lr"),
        ):
            with self.subTest(left=left):
                lines = match.diff_lines(original, candidate, BASE, fn, 20)
                self.assertEqual([int(line[2:10], 16) for line in lines],
                                 [BASE, BASE + 2, BASE + 4, BASE + 8])
                self.assertEqual(lines[1], f"! {BASE + 2:08x}  {left:<40} {right}")
                pool = ".word 0x12345678"
                self.assertEqual(lines[2], f"  {BASE + 4:08x}  {pool:<40} {pool}")
                self.assertEqual(lines[3], f"  {BASE + 8:08x}  {'bx lr':<40} bx lr")

    def test_extent_bytes_are_data_on_either_side(self):
        short = struct.pack("<I", 0xE12FFF1E)
        long = short * 2
        fn = {"isa": "arm", "code_ranges": [(BASE, BASE + 4)], "pools": [], "literals": []}
        for original, candidate, left, right in (
            (short, long, "", ".word 0xe12fff1e"), (long, short, ".word 0xe12fff1e", ""),
        ):
            with self.subTest(left=left):
                lines = match.diff_lines(original, candidate, BASE, fn, 20)
                self.assertEqual(len(lines), 2)
                self.assertEqual(lines[1], f"! {BASE + 4:08x}  {left:<40} {right}")


if __name__ == "__main__":
    unittest.main()
