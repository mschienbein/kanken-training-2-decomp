"""tools/packet.py: PC-relative literal addresses used to annotate author packets."""
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


class DisassembleTest(unittest.TestCase):
    def disassemble(self, data, isa, ranges, pools=(), literals=()):
        base = 0x10007C
        image = Mock()
        image.read.return_value = data
        fn = {"address": base, "size": len(data), "isa": isa,
              "code_ranges": ranges, "pools": pools, "literals": literals}
        rows = packet.disassemble(image, fn)
        image.read.assert_called_once_with(base, len(data))
        return rows

    def test_midword_literal_and_padding(self):
        base = 0x10007C
        data = struct.pack("<4I", *([0xE12FFF1E] * 4))
        rows = self.disassemble(data, "arm", [(base, base + 9), (base + 12, base + 16)],
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
        rows = self.disassemble(bytes.fromhex("00f000f8"), "thumb", [(base, base + 2)],
                                literals=[(base + 2, base + 4)])
        self.assertEqual(rows, [
            ("raw", base, "00f0", None), ("raw", base + 2, "00f8", None)])

    def test_trailing_partial_word_is_bounded(self):
        base = 0x10007C
        rows = self.disassemble(b"\x01\x02\x03", "arm", [], pools=[(base, base + 3)])
        self.assertEqual(rows, [("raw", base, "010203", None)])


if __name__ == "__main__":
    unittest.main()
