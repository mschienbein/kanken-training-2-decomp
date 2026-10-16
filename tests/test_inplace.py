"""In-place linker: relocation encodings, interworking, inferred data placement and failure cases.

Compiles a small fixture with the pinned compiler (skipped when the toolchain is not installed),
places it into a synthetic memory image whose expected bytes are encoded independently here, and
checks the linker reproduces them or fails loudly.
"""
import struct
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import inplace  # noqa: E402
import toolchain  # noqa: E402

PROFILE = toolchain.Profile(
    "test", "armcc_504_82",
    ("--cpu=MPCore", "--fpmode=fast", "--apcs=/interwork", "--arm", "--split_sections", "-O3", "-Otime"),
)
HAVE_COMPILER = toolchain.armcc_exe(PROFILE.compiler).is_file()

FIXTURE = r"""
extern void func_00100100(int);
extern void func_00100200(void);
extern int data_00200000;
void func_00100000(int x) { func_00100100(x); func_00100200(); data_00200000 = x; }
void *func_00100040(void) { return (void *)func_00100200; }
static const int table[4] = {11, 22, 33, 44};
int func_00100080(int i) { return table[i]; }
"""
TAILCALL = "extern void func_00100200(void);\nvoid func_00100000(void) { func_00100200(); }\n"
UNRESOLVED = "extern void mystery(void);\nvoid func_00100000(void) { mystery(); mystery(); }\n"

BASE = 0x100000
TABLE_AT = 0x180000
TABLE = struct.pack("<4i", 11, 22, 33, 44)


class Memory:
    def __init__(self):
        self.mem = bytearray(0x110000)

    def read(self, address, size):
        return bytes(self.mem[address - BASE : address - BASE + size])

    def write(self, address, data):
        self.mem[address - BASE : address - BASE + len(data)] = data


def bl(place, target, thumb):
    off = target - (place + 8)
    if thumb:
        return 0xFA000000 | (((off >> 1) & 1) << 24) | ((off >> 2) & 0xFFFFFF)
    return 0xEB000000 | ((off >> 2) & 0xFFFFFF)


CATALOGUE = {
    0x100000: {"address": 0x100000, "size": 0x40, "isa": "arm"},
    0x100040: {"address": 0x100040, "size": 0x40, "isa": "arm"},
    0x100080: {"address": 0x100080, "size": 0x40, "isa": "arm"},
    0x100100: {"address": 0x100100, "size": 0x10, "isa": "arm"},
    0x100200: {"address": 0x100200, "size": 0x10, "isa": "thumb"},
}


@unittest.skipUnless(HAVE_COMPILER, "pinned compiler not installed (tools/setup.py)")
class InPlaceTest(unittest.TestCase):
    def compile(self, text):
        tmp = Path(tempfile.mkdtemp())
        src = tmp / "fixture.c"
        src.write_text(text, encoding="utf-8")
        toolchain.compile_source(PROFILE, src, tmp / "fixture.o")
        return inplace.ObjectFile(tmp / "fixture.o")

    def expected_memory(self, obj):
        """Encode what armlink would produce, independently of the linker under test."""
        mem = Memory()
        mem.write(TABLE_AT, TABLE)
        for fn in obj.defined_functions():
            sec = obj.sections[fn.shndx]
            base = int(fn.name[5:], 16)
            data = bytearray(sec.data)
            for rel in sec.relocs:
                place = base + rel.offset
                name = rel.symbol.name
                if rel.type == inplace.R_ARM_CALL:
                    target = int(name[5:], 16)
                    struct.pack_into("<I", data, rel.offset, bl(place, target, CATALOGUE[target]["isa"] == "thumb"))
                elif rel.type == inplace.R_ARM_ABS32 and name.startswith(("data_", "func_")):
                    target = int(name[5:], 16)
                    thumb = name.startswith("func_") and CATALOGUE[target]["isa"] == "thumb"
                    struct.pack_into("<I", data, rel.offset, target | thumb)
                elif rel.type == inplace.R_ARM_ABS32:  # the const table's section
                    addend = struct.unpack_from("<I", sec.data, rel.offset)[0]
                    struct.pack_into("<I", data, rel.offset, TABLE_AT + addend)
                else:
                    self.fail(f"fixture produced unexpected relocation type {rel.type}")
            mem.write(base, bytes(data))
        return mem

    def link(self, obj, mem):
        linker = inplace.InPlaceLinker(mem, CATALOGUE, inplace.Resolver(CATALOGUE), (0x200000, 0x201000))
        return {p.base: data for p, data in linker.link(obj).values() if p.origin == "catalogue"}

    def test_relocations_reproduce_independent_encoding(self):
        obj = self.compile(FIXTURE)
        mem = self.expected_memory(obj)
        placed = self.link(obj, mem)
        self.assertEqual(sorted(placed), [0x100000, 0x100040, 0x100080])
        for base, data in placed.items():
            self.assertEqual(data, mem.read(base, len(data)), f"function at {base:#x}")
        # the call to the Thumb function became BLX, the pointer to it carries the Thumb bit
        words = struct.unpack_from(f"<{len(placed[0x100000]) // 4}I", placed[0x100000])
        self.assertTrue(any(w >> 25 == 0x7D for w in words))
        self.assertIn(0x100201, struct.unpack_from(f"<{len(placed[0x100040]) // 4}I", placed[0x100040]))

    def test_const_section_must_match_original_bytes(self):
        obj = self.compile(FIXTURE)
        mem = self.expected_memory(obj)
        mem.write(TABLE_AT + 8, struct.pack("<i", 34))
        with self.assertRaisesRegex(inplace.LinkError, "bytes differ"):
            self.link(obj, mem)

    def test_unresolved_symbol_fails(self):
        obj = self.compile(UNRESOLVED)
        with self.assertRaisesRegex(inplace.LinkError, "unresolved symbol 'mystery'"):
            self.link(obj, Memory())

    def test_tail_branch_to_thumb_needs_veneer(self):
        obj = self.compile(TAILCALL)
        rel = obj.sections[obj.defined_functions()[0].shndx].relocs[0]
        self.assertEqual(rel.type, inplace.R_ARM_JUMP24)
        with self.assertRaisesRegex(inplace.LinkError, "veneer"):
            self.link(obj, Memory())

    def test_unknown_function_address_fails(self):
        obj = self.compile("void func_00100004(void) {}\n")
        with self.assertRaisesRegex(inplace.LinkError, "no catalogued function"):
            self.link(obj, Memory())

    def test_distinct_data_objects_cannot_alias(self):
        """Review F01: two C objects inferred at one address must fail, not match."""
        obj = self.compile(
            "__attribute__((section(\".data.first\"))) int first = 17;\n"
            "__attribute__((section(\".data.second\"))) int second = 17;\n"
            "int func_00100000(void) { return first; }\nint func_00100040(void) { return second; }\n"
        )
        mem = Memory()
        mem.write(TABLE_AT, struct.pack("<ii", 17, 17))
        for fn in obj.defined_functions():
            sec = obj.sections[fn.shndx]
            data = bytearray(sec.data)
            for rel in sec.relocs:
                struct.pack_into("<I", data, rel.offset, TABLE_AT)  # both point at the same storage
            mem.write(int(fn.name[5:], 16), bytes(data))
        with self.assertRaisesRegex(inplace.LinkError, "overlap|disagree"):
            self.link(obj, mem)

    def test_unsplit_section_with_several_functions_is_refused(self):
        """Review F09: without --split_sections two functions share .text; never misreport it."""
        profile = toolchain.Profile("nosplit", PROFILE.compiler, tuple(f for f in PROFILE.flags if f != "--split_sections"))
        tmp = Path(tempfile.mkdtemp())
        (tmp / "two.c").write_text("int func_00100000(int x) { return x + 1; }\nint func_00100040(int x) { return x + 2; }\n", encoding="utf-8")
        toolchain.compile_source(profile, tmp / "two.c", tmp / "two.o")
        with self.assertRaisesRegex(inplace.LinkError, "several functions"):
            self.link(inplace.ObjectFile(tmp / "two.o"), Memory())


def synthetic(rtype: int, word: bytes, symbol: inplace.Symbol):
    sec = inplace.Section(1, "i.f", word, len(word), False, True, False)
    rel = inplace.Reloc(0, rtype, symbol)
    sec.relocs.append(rel)
    return sec, rel


class RelocationArithmeticTest(unittest.TestCase):
    """Compiler-free checks of encodings the fixtures above do not reach."""

    def linker(self, mem=None):
        return inplace.InPlaceLinker(mem or Memory(), CATALOGUE, inplace.Resolver(CATALOGUE), (0x200000, 0x201000))

    def test_prel31_keeps_thumb_bit_and_checks_range(self):
        sym = inplace.Symbol("func_00100200", 0, 0, "STT_FUNC", "STB_GLOBAL", "SHN_UNDEF")
        sec, rel = synthetic(inplace.R_ARM_PREL31, b"\0\0\0\x80", sym)
        data = bytearray(sec.data)
        self.linker()._apply(rel, sec, data, 0x100000, inplace.Target(0x100200, True))
        self.assertEqual(struct.unpack("<I", data)[0], 0x80000201)  # bit 31 kept, T bit set
        with self.assertRaisesRegex(inplace.LinkError, "overflows"):
            self.linker()._apply(rel, sec, bytearray(sec.data), 0x100000, inplace.Target(0x80100000, False))

    def test_jump24_bl_to_thumb_needs_veneer(self):
        sym = inplace.Symbol("func_00100200", 0, 0, "STT_FUNC", "STB_GLOBAL", "SHN_UNDEF")
        sec, rel = synthetic(inplace.R_ARM_JUMP24, struct.pack("<I", 0xEBFFFFFE), sym)
        with self.assertRaisesRegex(inplace.LinkError, "veneer"):
            self.linker()._apply(rel, sec, bytearray(sec.data), 0x100000, inplace.Target(0x100200, True))

    def test_thumb_short_branches_infer_their_target(self):
        """Review F05: B/Bcond to a defined Thumb function are decoded during placement."""
        mem = Memory()
        target = inplace.Symbol("local", 1, 4, "STT_FUNC", "STB_LOCAL", 2)
        for rtype, opcode in ((inplace.R_ARM_THM_JUMP11, 0xE000), (inplace.R_ARM_THM_JUMP8, 0xD000)):
            place = 0x100100
            # original: branch from 0x100100 to 0x100140 => imm = (0x140 - 0x100 - 4) >> 1
            imm = (0x100140 - place - 4) >> 1
            mem.write(place, struct.pack("<H", opcode | imm))
            sec, rel = synthetic(rtype, struct.pack("<H", opcode | (0x7FE if rtype == inplace.R_ARM_THM_JUMP11 else 0xFE)), target)
            implied = self.linker(mem)._original_value(rel, place, sec)
            addend = self.linker(mem)._addend(rel, sec)
            self.assertEqual(implied - addend, 0x100140, f"type {rtype}")

    def test_marker_relocations_need_no_symbol(self):
        sec = inplace.Section(1, "i.f", b"\x1e\xff\x2f\xe1", 4, False, True, False)
        sec.relocs.append(inplace.Reloc(0, inplace.R_ARM_V4BX, inplace.Symbol("", 0, 0, "STT_NOTYPE", "STB_LOCAL", "SHN_UNDEF")))

        class Obj:
            sections = {1: sec}

            def defined_functions(self):
                return [inplace.Symbol("func_00100000", 0, 4, "STT_FUNC", "STB_GLOBAL", 1)]

        mem = Memory()
        mem.write(0x100000, sec.data)
        linked = self.linker(mem).link(Obj())
        self.assertEqual(linked[1][1], sec.data)

    def test_reserved_overlay_names_are_refused(self):
        """Review F06: an overlay alias must not redirect a func_XXXXXXXX name."""
        with self.assertRaisesRegex(inplace.LinkError, "reserved"):
            inplace.Resolver(CATALOGUE, {"func_00100000": {"address": 0x100040}})


if __name__ == "__main__":
    unittest.main()
