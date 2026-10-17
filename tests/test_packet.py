"""tools/packet.py: bounded synthetic disassembly and PC-relative literal annotations."""
import struct
import sys
import unittest
from pathlib import Path
from unittest.mock import Mock

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import packet  # noqa: E402
from capstone import CS_ARCH_ARM, CS_MODE_ARM, CS_MODE_THUMB, Cs  # noqa: E402


def decode(word: bytes, address: int, mode):
    return next(Cs(CS_ARCH_ARM, mode).disasm(word, address, 1))


class PcLiteralTest(unittest.TestCase):
    def test_arm_negative_displacement(self):
        insn = decode(struct.pack("<I", 0xE51F0004), 0x100000, CS_MODE_ARM)  # ldr r0, [pc, #-4]
        self.assertEqual(packet.pc_literal(insn, "arm"), 0x100004)

    def test_arm_positive_displacement(self):
        insn = decode(struct.pack("<I", 0xE59F0018), 0x100000, CS_MODE_ARM)  # ldr r0, [pc, #0x18]
        self.assertEqual(packet.pc_literal(insn, "arm"), 0x100020)

    def test_thumb_literal_uses_aligned_pc(self):
        insn = decode(struct.pack("<H", 0x4801), 0x100002, CS_MODE_THUMB)  # ldr r0, [pc, #4]
        self.assertEqual(packet.pc_literal(insn, "thumb"), 0x100008)  # Align(0x100006, 4) + 4


class LiteralNameTest(unittest.TestCase):
    def namer(self):
        image = Mock()
        image.layout = Mock(text=Mock(address=0x100000, size=0x118000), rodata=Mock(address=0x218000, size=0x1F000),
                            data=Mock(address=0x237000, size=0xD000))
        image.bss = (0x244000, 0x4E4000)
        functions = {0x100100: {"address": 0x100100, "isa": "thumb"}, 0x100200: {"address": 0x100200, "isa": "arm"}}
        return packet.Namer(image, functions, {})

    def test_odd_data_address_keeps_bit_zero(self):
        self.assertEqual(self.namer().literal(0x2232D5), "data_002232d5")
        self.assertIsNone(self.namer().code_target(0x2232D5))

    def test_thumb_code_pointer_names_its_function(self):
        self.assertEqual(self.namer().literal(0x100101), "func_00100100")
        self.assertEqual(self.namer().code_target(0x100101), 0x100100)

    def test_arm_function_and_even_data_are_exact(self):
        self.assertEqual(self.namer().literal(0x100200), "func_00100200")
        self.assertEqual(self.namer().literal(0x2232D4), "data_002232d4")


class DisassembleTest(unittest.TestCase):
    def disassemble(self, data, isa, ranges, pools=(), literals=(), kind="function", entries=()):
        base = 0x10007C
        image = Mock()
        image.read.return_value = data
        fn = {"address": base, "size": len(data), "isa": isa,
              "code_ranges": ranges, "pools": pools, "literals": literals, "kind": kind, "entries": entries}
        rows = packet.disassemble(image, fn)
        image.read.assert_called_once_with(base, len(data))
        return rows

    def test_midword_literal_and_padding(self):
        base = 0x10007C
        data = struct.pack("<4I", *([0xE12FFF1E] * 4))
        for code_end in (base + 8, base + 9):
            with self.subTest(code_end=code_end):
                rows = self.disassemble(data, "arm", [(base, code_end), (base + 12, base + 16)],
                                        literals=[(base + 9, base + 12)])
                self.assertEqual([(kind, addr) for kind, addr, _, _ in rows], [
                    ("insn", base), ("insn", base + 4), ("raw", base + 8),
                    ("raw", base + 9), ("insn", base + 12)])
                self.assertEqual(rows[2][2], "1e")
                self.assertEqual(rows[3][2], "ff2fe1")
                self.assertEqual(rows[4][2].mnemonic, "bx")

    def test_pool_word_and_unlabelled_padding(self):
        base = 0x10007C
        data = struct.pack("<4I", 0xE12FFF1E, 0x12345678, 0xE12FFF1E, 0xE12FFF1E)
        rows = self.disassemble(data, "arm", [(base, base + 4), (base + 12, base + 16)],
                                pools=[(base + 4, base + 8)])
        self.assertEqual(rows[1], ("word", base + 4, 0x12345678, None))
        self.assertEqual(rows[2], ("word", base + 8, 0xE12FFF1E, None))
        self.assertEqual(rows[3][0], "insn")

    def test_partial_pool_does_not_consume_following_code(self):
        base = 0x10007C
        data = b"\x00\x00" + struct.pack("<H", 0x4770)
        rows = self.disassemble(data, "thumb", [(base + 2, base + 4)],
                                pools=[(base, base + 2)])
        self.assertEqual(rows[0], ("raw", base, "0000", None))
        self.assertEqual(rows[1][0:2], ("insn", base + 2))

    def test_thumb_wide_instruction_cannot_cross_boundary(self):
        base = 0x10007C
        data = bytes.fromhex("00f000f8")  # synthetic Thumb BL
        for stop, expected in (
            (1, [("raw", base, "00", None), ("raw", base + 1, "f000f8", None)]),
            (2, [("raw", base, "00f0", None), ("raw", base + 2, "00f8", None)]),
            (3, [("raw", base, "00f0", None), ("raw", base + 2, "00", None),
                 ("raw", base + 3, "f8", None)]),
        ):
            with self.subTest(stop=stop):
                rows = self.disassemble(data, "thumb", [(base, base + stop)],
                                        literals=[(base + stop, base + 4)])
                self.assertEqual(rows, expected)

    def test_trailing_partial_word_is_bounded(self):
        base = 0x10007C
        rows = self.disassemble(b"\x01\x02\x03", "arm", [], pools=[(base, base + 3)])
        self.assertEqual(rows, [("raw", base, "010203", None)])

    def test_adjacent_literal_and_pool_stay_separate(self):
        base = 0x10007C
        data = struct.pack("<5I", *([0xE12FFF1E] * 5))
        rows = self.disassemble(data, "arm", [(base, base + 8), (base + 16, base + 20)],
                                pools=[(base + 12, base + 16)], literals=[(base + 9, base + 12)])
        self.assertEqual(rows[2:5], [
            ("raw", base + 8, "1e", None),
            ("raw", base + 9, "ff2fe1", None),
            ("word", base + 12, 0xE12FFF1E, None)])
        self.assertEqual(rows[-1][0:2], ("insn", base + 16))

    def test_unaligned_code_ranges_only_decode_complete_words(self):
        base = 0x10007C
        data = struct.pack("<4I", *([0xE12FFF1E] * 4))
        rows = self.disassemble(data, "arm", [(base + 1, base + 7), (base + 8, base + 16)])
        self.assertEqual(rows[:4], [
            ("raw", base, "1e", None), ("raw", base + 1, "ff2fe1", None),
            ("raw", base + 4, "1eff2f", None), ("raw", base + 7, "e1", None)])
        self.assertEqual([(kind, addr) for kind, addr, _, _ in rows[4:]],
                         [("insn", base + 8), ("insn", base + 12)])

    def test_thumb_narrow_and_wide_instructions_fit_code_ranges(self):
        base = 0x10007C
        data = bytes.fromhex("704700f000f87047") + struct.pack("<I", 0x12345678) + bytes.fromhex("7047")
        rows = self.disassemble(data, "thumb", [(base, base + 8), (base + 12, base + 14)],
                                pools=[(base + 8, base + 12)])
        self.assertEqual([(kind, addr) for kind, addr, _, _ in rows], [
            ("insn", base), ("insn", base + 2), ("insn", base + 6),
            ("word", base + 8), ("insn", base + 12)])
        self.assertEqual([(row[2].mnemonic, row[2].size) for row in rows if row[0] == "insn"],
                         [("bx", 2), ("bl", 4), ("bx", 2), ("bx", 2)])

    def test_thumb_wide_instruction_cannot_cross_adjacent_code_ranges(self):
        base = 0x10007C
        rows = self.disassemble(bytes.fromhex("00f000f8"), "thumb",
                                [(base, base + 2), (base + 2, base + 4)])
        self.assertEqual(rows, [
            ("raw", base, "00f0", None), ("raw", base + 2, "00f8", None)])

    def test_data_overrides_overlapping_code_range(self):
        base = 0x10007C
        data = struct.pack("<4I", *([0xE12FFF1E] * 4))
        rows = self.disassemble(data, "arm", [(base, base + 16)],
                                pools=[(base + 4, base + 8)], literals=[(base + 9, base + 12)])
        self.assertEqual([(kind, addr) for kind, addr, _, _ in rows], [
            ("insn", base), ("word", base + 4), ("raw", base + 8),
            ("raw", base + 9), ("insn", base + 12)])
        self.assertEqual(rows[2:4], [("raw", base + 8, "1e", None), ("raw", base + 9, "ff2fe1", None)])

    def test_image_end_clips_partial_literal(self):
        base = 0x10007C
        data = struct.pack("<3I", *([0xE12FFF1E] * 3))[:10]
        rows = self.disassemble(data, "arm", [(base, base + 8)], literals=[(base + 9, base + 12)])
        self.assertEqual(rows[-2:], [("raw", base + 8, "1e", None), ("raw", base + 9, "ff", None)])

    def test_multi_entry_function_keeps_secondary_code_island(self):
        base = 0x10007C
        data = struct.pack("<4I", *([0xE12FFF1E] * 4))
        rows = self.disassemble(data, "arm", [(base, base + 4), (base + 12, base + 16)],
                                kind="multi_entry", entries=[{"address": base, "isa": "arm"},
                                                             {"address": base + 12, "isa": "arm"}])
        self.assertEqual([(kind, addr) for kind, addr, _, _ in rows], [
            ("insn", base), ("word", base + 4), ("word", base + 8), ("insn", base + 12)])


if __name__ == "__main__":
    unittest.main()
