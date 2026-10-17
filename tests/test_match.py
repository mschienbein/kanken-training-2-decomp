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
        ranges = [(BASE, BASE + 9), (BASE + 12, BASE + 16)]
        literals = [(BASE + 9, BASE + 12)]
        rows = match.render(data, BASE, "arm", ranges, literals)
        self.assertEqual([(a, n) for a, n, _ in rows], [
            (BASE, 4), (BASE + 4, 4), (BASE + 8, 1), (BASE + 9, 3), (BASE + 12, 4)])
        self.assertEqual(rows[2][2], ".word 0x1e")
        self.assertEqual(rows[3][2], ".word 0xe12fff")
        self.assertEqual(rows[4][2], "bx lr")
        fn = {"isa": "arm", "code_ranges": ranges, "literals": literals}
        lines = match.diff_lines(data, data, BASE, fn, 20)
        self.assertIn("00100084  .word 0x1e", lines[2])

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
        rows = match.render(data, BASE, "thumb", [(BASE, BASE + 2)], [(BASE + 2, BASE + 4)])
        self.assertEqual([(a, n) for a, n, _ in rows], [(BASE, 2), (BASE + 2, 2)])
        self.assertTrue(all(text.startswith((".inst", ".word")) for _, _, text in rows))

    def test_candidate_bytes_outside_catalogue_are_data(self):
        data = struct.pack("<2I", 0xE12FFF1E, 0xE12FFF1E)
        rows = match.render(data, BASE, "arm", [(BASE, BASE + 4)], [])
        self.assertEqual(rows[1], (BASE + 4, 4, ".word 0xe12fff1e"))


if __name__ == "__main__":
    unittest.main()
