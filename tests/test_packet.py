"""tools/packet.py: PC-relative literal addresses used to annotate author packets."""
import struct
import sys
import unittest
from pathlib import Path

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
        self.assertEqual(packet.pc_literal(insn, "thumb"), 0x10000C)
        insn = decode(struct.pack("<H", 0x4801), 0x100002, CS_MODE_THUMB)  # ldr r0, [pc, #4]
        self.assertEqual(packet.pc_literal(insn, "thumb"), 0x100008)  # Align(0x100006, 4) + 4
if __name__ == "__main__":
    unittest.main()
