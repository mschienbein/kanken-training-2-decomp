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
            "__attribute__((section(\".data.first\"))) static int first = 17;\n"
            "__attribute__((section(\".data.second\"))) static int second = 17;\n"
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

    def test_misaligned_placement_is_refused(self):
        """Review R2-01: an 8-byte-aligned data section placed at a 4-mod-8 address must fail."""
        obj = self.compile("static unsigned long long wide = 17ULL;\nunsigned long long *func_00100000(void) { return &wide; }\n")
        mem = Memory()
        mem.write(TABLE_AT + 4, struct.pack("<Q", 17))
        fn = obj.defined_functions()[0]
        sec = obj.sections[fn.shndx]
        data = bytearray(sec.data)
        for rel in sec.relocs:
            struct.pack_into("<I", data, rel.offset, TABLE_AT + 4)
        mem.write(0x100000, bytes(data))
        with self.assertRaisesRegex(inplace.LinkError, "not aligned"):
            self.link(obj, mem)

    def test_adjacent_functions_in_one_section_are_refused(self):
        """Review R2-05: contiguous functions sharing .text must not collapse into one EXTENT row."""
        profile = toolchain.Profile("nosplit", PROFILE.compiler, tuple(f for f in PROFILE.flags if f != "--split_sections"))
        tmp = Path(tempfile.mkdtemp())
        (tmp / "adj.c").write_text("int func_00100000(int x) { return x + 1; }\nint func_00100008(int x) { return x + 2; }\n", encoding="utf-8")
        toolchain.compile_source(profile, tmp / "adj.c", tmp / "adj.o")
        catalogue = {0x100000: {"address": 0x100000, "size": 8, "isa": "arm"}, 0x100008: {"address": 0x100008, "size": 8, "isa": "arm"}}
        linker = inplace.InPlaceLinker(Memory(), catalogue, inplace.Resolver(catalogue), (0x200000, 0x201000))
        with self.assertRaisesRegex(inplace.LinkError, "several functions"):
            linker.link(inplace.ObjectFile(tmp / "adj.o"))

    def global_fixture(self, pointer_at):
        """TU A defines g = 17 and returns &g; TU B uses extern g. The original's literal is pointer_at."""
        a = self.compile("int g = 17;\nint *func_00100000(void) { return &g; }\n")
        b = self.compile("extern int g;\nint *func_00100040(void) { return &g; }\n")
        mem = Memory()
        mem.write(TABLE_AT, struct.pack("<ii", 17, 17))
        for obj in (a, b):
            fn = obj.defined_functions()[0]
            sec = obj.sections[fn.shndx]
            data = bytearray(sec.data)
            for rel in sec.relocs:
                struct.pack_into("<I", data, rel.offset, pointer_at)
            mem.write(int(fn.name[5:], 16), bytes(data))
        return a, b, mem

    def link_with_names(self, obj, mem, names):
        linker = inplace.InPlaceLinker(mem, CATALOGUE, inplace.Resolver(CATALOGUE, names), (0x200000, 0x201000))
        return {p.base: data for p, data in linker.link(obj).values() if p.origin == "catalogue"}

    def test_global_definition_and_extern_resolve_to_one_address(self):
        """Advisory: A's definition (reached via a section symbol) and B's extern must agree."""
        a, b, mem = self.global_fixture(TABLE_AT)
        names = {"g": {"address": TABLE_AT}}
        self.assertEqual(self.link_with_names(a, mem, names)[0x100000], mem.read(0x100000, 12))
        self.assertEqual(self.link_with_names(b, mem, names)[0x100040], mem.read(0x100040, 12))

    def test_global_definition_away_from_its_binding_fails(self):
        """Advisory: A infers g at TABLE_AT from its literal while the overlay binds g 4 bytes later."""
        a, b, mem = self.global_fixture(TABLE_AT)
        names = {"g": {"address": TABLE_AT + 4}}
        # A's definition is placed at its binding, so A's literal becomes TABLE_AT + 4: a mismatch
        linked_a = self.link_with_names(a, mem, names)[0x100000]
        linked_b = self.link_with_names(b, mem, names)[0x100040]
        self.assertNotEqual(linked_a, mem.read(0x100000, 12))
        self.assertNotEqual(linked_b, mem.read(0x100040, 12))
        # and both functions now return the same pointer, as C requires
        self.assertEqual(linked_a[-4:], linked_b[-4:])

    def test_unbound_global_definition_fails(self):
        a, _, mem = self.global_fixture(TABLE_AT)
        with self.assertRaisesRegex(inplace.LinkError, "no address binding"):
            self.link_with_names(a, mem, {})

    def test_bound_data_definition_is_verified_without_references(self):
        """A bound definition no code references is still placed and its bytes checked."""
        obj = self.compile("int data_00180000 = 18;\nint func_00100000(void) { return 1; }\n")
        mem = Memory()
        fn = obj.defined_functions()[0]
        mem.write(0x100000, obj.sections[fn.shndx].data)
        mem.write(TABLE_AT, struct.pack("<i", 17))
        with self.assertRaisesRegex(inplace.LinkError, "bytes differ"):
            self.link(obj, mem)

    def test_switch_table_label_is_not_a_function(self):
        """ARMCC's local `__switch$$` label (jump-table base) relocates within its own section."""
        obj = self.compile(
            "extern void func_00100100(int);\n"
            "void func_00100000(int x) {\n"
            "  switch (x) { case 0: func_00100100(5); break; case 1: func_00100100(9); break;\n"
            "  case 2: func_00100100(13); break; case 3: func_00100100(2); break;\n"
            "  case 4: func_00100100(7); break; case 5: func_00100100(4); break; default: break; }\n"
            "}\n"
        )
        self.assertEqual([f.name for f in obj.defined_functions()], ["func_00100000"])
        fn = obj.defined_functions()[0]
        sec = obj.sections[fn.shndx]
        data = bytearray(sec.data)
        for rel in sec.relocs:
            if rel.type in (inplace.R_ARM_CALL, inplace.R_ARM_JUMP24):
                insn = struct.unpack_from("<I", sec.data, rel.offset)[0]
                off = (0x100100 - (0x100000 + rel.offset + 8)) >> 2
                struct.pack_into("<I", data, rel.offset, (insn & 0xFF000000) | (off & 0xFFFFFF))
            elif rel.type == inplace.R_ARM_ABS32:
                addend = struct.unpack_from("<I", sec.data, rel.offset)[0]
                struct.pack_into("<I", data, rel.offset, 0x100000 + rel.symbol.value + addend)
        mem = Memory()
        mem.write(0x100000, bytes(data))
        catalogue = {0x100000: {"address": 0x100000, "size": len(data), "isa": "arm"},
                     0x100100: {"address": 0x100100, "size": 0x10, "isa": "arm"}}
        linker = inplace.InPlaceLinker(mem, catalogue, inplace.Resolver(catalogue), (0x200000, 0x201000))
        linked = {p.base: d for p, d in linker.link(obj).values()}
        self.assertEqual(linked[0x100000], bytes(data))


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

            defined_globals = defined_functions

        mem = Memory()
        mem.write(0x100000, sec.data)
        linked = self.linker(mem).link(Obj())
        self.assertEqual(linked[1][1], sec.data)

    def test_absolute_data_symbols_keep_their_low_bit(self):
        """Review R2-03: only function symbols carry a Thumb bit."""
        obj_sym = inplace.Symbol("abs_data", 0x102001, 1, "STT_OBJECT", "STB_GLOBAL", "SHN_ABS")
        self.assertEqual(self.linker()._symbol_target(obj_sym, None, {}), inplace.Target(0x102001, False))
        fn_sym = inplace.Symbol("abs_fn", 0x100201, 0, "STT_FUNC", "STB_GLOBAL", "SHN_ABS")
        self.assertEqual(self.linker()._symbol_target(fn_sym, None, {}), inplace.Target(0x100200, True))

    def test_none_dependency_on_an_unplaced_section_fails(self):
        """Review R2-04: R_ARM_NONE keeps a section alive; it cannot silently drop out of the check."""
        dep = inplace.Section(2, ".constdata", b"\x01\x02\x03\x04", 4, False, False, False)
        sec = inplace.Section(1, "i.f", b"\x1e\xff\x2f\xe1", 4, False, True, False)
        sec.relocs.append(inplace.Reloc(0, inplace.R_ARM_NONE, inplace.Symbol(".constdata", 0, 0, "STT_SECTION", "STB_LOCAL", 2)))

        class Obj:
            sections = {1: sec, 2: dep}

            def defined_functions(self):
                return [inplace.Symbol("func_00100000", 0, 4, "STT_FUNC", "STB_GLOBAL", 1)]

            defined_globals = defined_functions

        mem = Memory()
        mem.write(0x100000, sec.data)
        with self.assertRaisesRegex(inplace.LinkError, "depends on"):
            self.linker(mem).link(Obj())

    def test_comdat_copies_share_storage_but_other_sections_do_not(self):
        """Review R2-02: duplicate COMDAT members are one entity; anything else overlapping fails."""
        key = ("_ZX", ".data_X")
        inplace.reject_overlaps([(0x102000, 0x102004, "a.o:.data_X", key), (0x102000, 0x102004, "b.o:.data_X", key)])
        with self.assertRaises(inplace.LinkError):
            inplace.reject_overlaps([(0x102000, 0x102004, "a.o:.data", None), (0x102000, 0x102004, "b.o:.data", None)])
        with self.assertRaises(inplace.LinkError):
            inplace.reject_overlaps([(0x102000, 0x102004, "a.o:.data_X", key), (0x102000, 0x102008, "b.o:.data_X", key)])

    def test_one_comdat_entity_has_one_address(self):
        """Review R3-01: the same COMDAT member at two non-overlapping addresses is a contradiction."""
        key = ("_ZN6HolderILi1EE5valueE", ".data__ZN6HolderILi1EE5valueE")
        with self.assertRaisesRegex(inplace.LinkError, "COMDAT"):
            inplace.reject_overlaps([(0x102000, 0x102004, "a.o", key), (0x102004, 0x102008, "b.o", key)])

    def test_secondary_entries_are_call_targets_but_not_definitions(self):
        """Catalogue v3 `entries`: an interior entry can be called, never defined as a C function."""
        catalogue = {0x100000: {"address": 0x100000, "size": 0x20, "isa": "arm", "kind": "multi_entry",
                                "entries": [{"address": 0x100000, "isa": "arm"}, {"address": 0x10000c, "isa": "arm"}]}}
        resolver = inplace.Resolver(catalogue)
        self.assertEqual(resolver.resolve("func_0010000c"), inplace.Target(0x10000C, False))
        linker = inplace.InPlaceLinker(Memory(), catalogue, resolver, (0x200000, 0x201000))
        with self.assertRaisesRegex(inplace.LinkError, "not a catalogued function start"):
            linker._address_for("func_0010000c")
        with self.assertRaisesRegex(inplace.LinkError, "no catalogued function or entry"):
            resolver.resolve("func_00100008")

    def test_reserved_overlay_names_are_refused(self):
        """Review F06: an overlay alias must not redirect a func_XXXXXXXX name."""
        with self.assertRaisesRegex(inplace.LinkError, "reserved"):
            inplace.Resolver(CATALOGUE, {"func_00100000": {"address": 0x100040}})


# -- armlink output optimisations (docs/LINKER.md) --------------------------------------------

MOV_R0_R0 = 0xE1A00000
BX_LR = struct.pack("<I", 0xE12FFF1E)
# Callees the model examines, with their original bytes.
CALLEES = {
    0x100100: BX_LR,  # empty: inlined as MOV r0, r0
    0x100110: struct.pack("<I", 0xE3A00000) + BX_LR,  # mov r0, #0; bx lr: inlined as mov r0, #0
    0x100120: struct.pack("<I", 0xE28F0000) + BX_LR,  # add r0, pc, #0: reads the PC, kept
    0x100130: struct.pack("<I", MOV_R0_R0) + BX_LR,  # a tail-call B removed by --branchnop: kept
    0x100140: struct.pack("<I", 0xE3A00000) + struct.pack("<I", 0xE3A01000) + BX_LR,  # 12 bytes: kept
    0x100150: struct.pack("<I", 0xE3A00000) + BX_LR,  # qualifying bytes, but a multi-entry owner: kept
    0x100160: struct.pack("<HH", 0x2000, 0x4770),  # Thumb movs r0, #0; bx lr: kept (never modelled)
}
MODEL_CATALOGUE = {
    0x100000: {"address": 0x100000, "size": 0x40, "isa": "arm"},
    0x100004: {"address": 0x100004, "size": 0x3C, "isa": "arm"},  # only for the branch-to-next fixture
    0x100100: {"address": 0x100100, "size": 4, "isa": "arm"},
    0x100110: {"address": 0x100110, "size": 8, "isa": "arm"},
    0x100120: {"address": 0x100120, "size": 8, "isa": "arm"},
    0x100130: {"address": 0x100130, "size": 8, "isa": "arm"},
    0x100140: {"address": 0x100140, "size": 12, "isa": "arm"},
    0x100150: {"address": 0x100150, "size": 8, "isa": "arm", "kind": "multi_entry",
               "entries": [{"address": 0x100150, "isa": "arm"}, {"address": 0x100154, "isa": "arm"}]},
    0x100160: {"address": 0x100160, "size": 4, "isa": "thumb"},
}


@unittest.skipUnless(HAVE_COMPILER, "pinned compiler not installed (tools/setup.py)")
class ArmlinkModelTest(unittest.TestCase):
    """Each rule of the default linker model, and the boundary where the call stays a BL."""

    compile = InPlaceTest.compile

    def memory(self):
        mem = Memory()
        for address, body in CALLEES.items():
            mem.write(address, body)
        return mem

    def expected(self, obj, mem, base, calls, data_at=0x200000):
        """Write what armlink produces for the function at `base` into `mem`, encoded here
        independently: `calls` maps a callee name to the word armlink writes for a call to it
        (None: an ordinary BL/BLX)."""
        fn = next(f for f in obj.defined_functions() if int(f.name[5:], 16) == base)
        sec = obj.sections[fn.shndx]
        data = bytearray(sec.data)
        for rel in sec.relocs:
            if rel.type == inplace.R_ARM_ABS32:
                struct.pack_into("<I", data, rel.offset, data_at)
                continue
            word = calls[rel.symbol.name]
            if word is None:
                target = int(rel.symbol.name[5:], 16)
                word = bl(base + rel.offset, target, MODEL_CATALOGUE[target]["isa"] == "thumb")
            struct.pack_into("<I", data, rel.offset, word)
        mem.write(base, bytes(data))
        return bytes(data)

    def link(self, obj, mem, model=inplace.DEFAULT_LINKER_MODEL, catalogue=MODEL_CATALOGUE):
        linker = inplace.InPlaceLinker(mem, catalogue, inplace.Resolver(catalogue), (0x200000, 0x201000), model)
        linked = {p.base: data for p, data in linker.link(obj).values() if p.origin == "catalogue"}
        return linked, [(w.rule, w.target, w.word) for w in linker.rewrites]

    def test_default_model_is_the_evidenced_armlink_model(self):
        model = inplace.linker_model(inplace.DEFAULT_LINKER_MODEL)
        self.assertTrue(model.inline and model.branchnop)
        self.assertEqual(model.name, "armlink5-inline-branchnop-v2")
        self.assertFalse(inplace.linker_model("strict-v1").inline)
        with self.assertRaisesRegex(inplace.LinkError, "unknown linker model"):
            inplace.linker_model("armlink-guess")

    def test_retired_model_never_runs_again(self):
        """Review F01: v1 judged same-object callees by candidate bytes; its name stays reserved."""
        self.assertNotIn("armlink5-inline-branchnop-v1", inplace.LINKER_MODELS)
        with self.assertRaisesRegex(inplace.LinkError, "retired"):
            inplace.linker_model("armlink5-inline-branchnop-v1")

    def test_calls_of_return_only_and_one_instruction_functions_are_inlined(self):
        """Rules I1/I2: BL to `bx lr` -> MOV r0, r0; BL to `mov r0, #0; bx lr` -> mov r0, #0."""
        obj = self.compile(
            "extern void func_00100100(void);\nextern int func_00100110(void);\nextern int data_00200000;\n"
            "void func_00100000(void) { func_00100100(); data_00200000 = func_00100110(); }\n"
        )
        mem = self.memory()
        want = self.expected(obj, mem, 0x100000, {"func_00100100": MOV_R0_R0, "func_00100110": 0xE3A00000})
        linked, rewrites = self.link(obj, mem)
        self.assertEqual(linked[0x100000], want)
        self.assertEqual(rewrites, [("inline_nop", 0x100100, MOV_R0_R0), ("inline_insn", 0x100110, 0xE3A00000)])
        strict, none = self.link(obj, mem, "strict-v1")
        self.assertEqual(none, [])
        self.assertNotEqual(strict[0x100000], want)  # the strict model keeps both BLs: a mismatch

    def test_conditional_call_of_an_empty_function_becomes_an_unconditional_nop(self):
        """Rule I3 (image evidence): `cmp; blne empty` left `cmp; mov r0, r0` in the original."""
        obj = self.compile(
            "extern void func_00100100(int);\nextern int data_00200000;\n"
            "void func_00100000(int x) { if (x) func_00100100(x); data_00200000 = x; }\n"
        )
        sec = obj.sections[obj.defined_functions()[0].shndx]
        rel = next(r for r in sec.relocs if r.symbol.name == "func_00100100")
        self.assertEqual((rel.type, struct.unpack_from("<I", sec.data, rel.offset)[0] >> 28), (inplace.R_ARM_JUMP24, 0x1))
        mem = self.memory()
        want = self.expected(obj, mem, 0x100000, {"func_00100100": MOV_R0_R0})
        linked, rewrites = self.link(obj, mem)
        self.assertEqual(linked[0x100000], want)
        self.assertEqual(rewrites, [("inline_nop", 0x100100, MOV_R0_R0)])

    def test_condition_rules_for_an_inlined_instruction(self):
        """Rule I3 (manual): the BL's condition goes onto an unconditional instruction; matching
        conditions inline as they are; differing ones, or an unconditional-only instruction under
        BLcond, keep the call."""
        body = lambda insn: struct.pack("<I", insn) + BX_LR  # noqa: E731
        blne, bl_ = 0x1BFFFFFE, 0xEBFFFFFE
        self.assertEqual(inplace.inline_arm_call(blne, body(0xE3A00000)), ("inline_insn", 0x13A00000))
        self.assertEqual(inplace.inline_arm_call(blne, body(0x13A00001)), ("inline_insn", 0x13A00001))
        self.assertIsNone(inplace.inline_arm_call(blne, body(0x03A00001)))
        self.assertIsNone(inplace.inline_arm_call(blne, body(0xF5D0F000)))  # pld [r0]
        self.assertEqual(inplace.inline_arm_call(bl_, body(0xF5D0F000)), ("inline_insn", 0xF5D0F000))
        self.assertEqual(inplace.inline_arm_call(blne, BX_LR), ("inline_nop", MOV_R0_R0))
        self.assertEqual(inplace.inline_arm_call(bl_, struct.pack("<I", 0xE1A0F00E)), ("inline_nop", MOV_R0_R0))
        for refused in (0xEF00003C, 0xE1A0000E, 0xE59F0000, 0xEA000000, 0xE12FFF1E):  # svc, lr, pc, b, bx
            self.assertIsNone(inplace.inline_arm_call(bl_, body(refused)), f"{refused:#x}")
        self.assertIsNone(inplace.inline_arm_call(bl_, struct.pack("<I", 0xE12FFF1E) * 3))
        self.assertIsNone(inplace.inline_arm_call(bl_, struct.pack("<I", 0x012FFF1E)))  # bxeq lr only

    def test_calls_that_do_not_qualify_stay_bl(self):
        """Fail-closed boundary: PC access, a removed tail call (MOV r0, r0), a longer callee, a
        multi-entry owner and a Thumb callee all keep an ordinary relocated BL/BLX."""
        obj = self.compile(
            "extern int func_00100120(void);\nextern int func_00100130(void);\nextern int func_00100140(void);\n"
            "extern int func_00100150(void);\nextern int func_00100160(void);\n"
            "int func_00100000(void) { return func_00100120() + func_00100130() + func_00100140()"
            " + func_00100150() + func_00100160(); }\n"
        )
        mem = self.memory()
        names = ("func_00100120", "func_00100130", "func_00100140", "func_00100150", "func_00100160")
        want = self.expected(obj, mem, 0x100000, dict.fromkeys(names))
        linked, rewrites = self.link(obj, mem)
        self.assertEqual(rewrites, [])
        self.assertEqual(linked[0x100000], want)

    def test_a_non_matching_body_still_mismatches(self):
        """The rewrite is what armlink writes, not what the original holds: an original MOV r0, #1
        at the call of an empty function, or a MOV r0, r0 at a call armlink keeps, differ."""
        obj = self.compile(
            "extern void func_00100100(void);\nextern int func_00100140(void);\nextern int data_00200000;\n"
            "void func_00100000(void) { func_00100100(); data_00200000 = func_00100140(); }\n"
        )
        for calls in ({"func_00100100": 0xE3A00001, "func_00100140": None},
                      {"func_00100100": MOV_R0_R0, "func_00100140": MOV_R0_R0}):
            mem = self.memory()
            want = self.expected(obj, mem, 0x100000, calls)
            linked, _ = self.link(obj, mem)
            self.assertNotEqual(linked[0x100000], want, calls)

    LOCAL_EMPTY = (
        "extern int data_00200000;\n__attribute__((noinline)) void func_00100100(void) {}\n"
        "void func_00100000(void) { func_00100100(); data_00200000 = 1; }\n"
    )

    def test_callee_defined_in_the_same_object_must_be_the_original_function(self):
        """Decided from the original: the local `bx lr` equals the eligible original body."""
        obj = self.compile(self.LOCAL_EMPTY)
        mem = self.memory()
        want = self.expected(obj, mem, 0x100000, {"func_00100100": MOV_R0_R0})
        linked, rewrites = self.link(obj, mem)
        self.assertEqual((linked[0x100000], linked[0x100100]), (want, BX_LR))
        self.assertEqual(rewrites, [("inline_nop", 0x100100, MOV_R0_R0)])

    def test_same_object_callee_cannot_unlock_a_caller_match(self):
        """Review F01 probes: the original at the callee's address is a 12-byte body, the excluded
        MOV r0, r0; BX lr, or a return-only veneer / multi-entry owner. The local helper is `bx lr`
        and the original caller holds the inlined NOP, so trusting the helper would make the caller
        match. The link is refused outright: tools/build.py links the whole object before it reads
        the ledger's function list, so a caller-only ledger cannot get past it either."""
        obj = self.compile(self.LOCAL_EMPTY)
        probes = {
            "12-byte original body": (struct.pack("<3I", 0xE3A00002, 0xE3A01003, 0xE12FFF1E), {}),
            "original MOV r0, r0; BX lr": (struct.pack("<I", MOV_R0_R0) + BX_LR, {}),
            "original veneer": (BX_LR, {"kind": "veneer"}),
            "original multi-entry owner": (BX_LR, {"kind": "multi_entry", "entries": [
                {"address": 0x100100, "isa": "arm"}, {"address": 0x100102, "isa": "thumb"}]}),
        }
        for name, (body, meta) in probes.items():
            with self.subTest(name):
                catalogue = {0x100000: MODEL_CATALOGUE[0x100000],
                             0x100100: {"address": 0x100100, "size": len(body), "isa": "arm", **meta}}
                mem = Memory()
                mem.write(0x100100, body)
                self.expected(obj, mem, 0x100000, {"func_00100100": MOV_R0_R0})
                with self.assertRaisesRegex(inplace.LinkError, "inlining is undecidable"):
                    self.link(obj, mem, catalogue=catalogue)
                # strict-v1 never rewrites: the caller keeps its BL and simply mismatches
                strict, _ = self.link(obj, mem, "strict-v1", catalogue)
                self.assertNotEqual(strict[0x100000], mem.read(0x100000, len(strict[0x100000])))

    def test_same_object_callee_with_other_inlinable_bytes_is_refused(self):
        """Local `mov r0, #1; bx lr` against an original `mov r0, #0; bx lr`: both inline, differently."""
        obj = self.compile(
            "extern int data_00200000;\n__attribute__((noinline)) int func_00100110(void) { return 1; }\n"
            "void func_00100000(void) { data_00200000 = func_00100110(); }\n"
        )
        mem = self.memory()
        self.expected(obj, mem, 0x100000, {"func_00100110": 0xE3A00000})
        with self.assertRaisesRegex(inplace.LinkError, "other bytes than the original"):
            self.link(obj, mem)

    def test_mismatching_local_helper_that_neither_side_inlines_keeps_the_bl(self):
        """No inlining decision, no refusal: an ordinary wrong helper is an ordinary mismatch."""
        obj = self.compile(
            "__attribute__((noinline)) int func_00100140(int x) { return x * 3 + 1; }\n"
            "int func_00100000(int x) { return func_00100140(x) + 2; }\n"
        )
        mem = self.memory()
        want = self.expected(obj, mem, 0x100000, {"func_00100140": None})
        linked, rewrites = self.link(obj, mem)
        self.assertEqual(rewrites, [])
        self.assertEqual(linked[0x100000], want)
        self.assertNotEqual(linked[0x100140], CALLEES[0x100140])

    def test_tail_call_to_the_next_instruction_becomes_a_nop(self):
        """Rule B1 (--branchnop): `b g` with g placed right after it is MOV r0, r0 (a NOP entry)."""
        obj = self.compile("extern void func_00100004(void);\nvoid func_00100000(void) { func_00100004(); }\n")
        linked, rewrites = self.link(obj, self.memory())
        self.assertEqual(linked[0x100000], struct.pack("<I", MOV_R0_R0))
        self.assertEqual(rewrites, [("branchnop", 0x100004, MOV_R0_R0)])
        strict, _ = self.link(obj, self.memory(), "strict-v1")
        self.assertEqual(strict[0x100000], struct.pack("<I", 0xEAFFFFFF))

    def test_tail_call_of_an_empty_function_is_not_inlined(self):
        """Only BL is inlined: B to `bx lr` stays a B (the original keeps 42 of them)."""
        obj = self.compile("extern void func_00100100(void);\nvoid func_00100000(void) { func_00100100(); }\n")
        linked, rewrites = self.link(obj, self.memory())
        self.assertEqual(rewrites, [])
        self.assertEqual(linked[0x100000], struct.pack("<I", 0xEA000000 | ((0x100100 - 0x100008) >> 2)))


class ArmlinkBoundaryTest(unittest.TestCase):
    """Compiler-free: branches to the next instruction whose armlink output is not established."""

    def test_conditional_branch_or_call_to_the_next_instruction_is_refused(self):
        linker = inplace.InPlaceLinker(Memory(), MODEL_CATALOGUE, inplace.Resolver(MODEL_CATALOGUE), (0x200000, 0x201000))
        sym = inplace.Symbol("func_00100004", 0, 0, "STT_FUNC", "STB_GLOBAL", "SHN_UNDEF")
        for rtype, word in ((inplace.R_ARM_JUMP24, 0x1AFFFFFE), (inplace.R_ARM_JUMP24, 0x1BFFFFFE), (inplace.R_ARM_CALL, 0xEBFFFFFE)):
            sec, rel = synthetic(rtype, struct.pack("<I", word), sym)
            with self.assertRaisesRegex(inplace.LinkError, "next instruction"):
                linker._optimised_branch(rel, sec, 0x100000, inplace.Target(0x100004), None, {})
        sec, rel = synthetic(inplace.R_ARM_JUMP24, struct.pack("<I", 0x1AFFFFFE), sym)
        strict = inplace.InPlaceLinker(Memory(), MODEL_CATALOGUE, inplace.Resolver(MODEL_CATALOGUE), (0x200000, 0x201000), "strict-v1")
        self.assertIsNone(strict._optimised_branch(rel, sec, 0x100000, inplace.Target(0x100004), None, {}))


ROOT = Path(__file__).resolve().parents[1]


@unittest.skipUnless((ROOT / "orig/code.bin").is_file() and (ROOT / "orig/exheader.bin").is_file(), "private inputs absent")
class OriginalImageEvidenceTest(unittest.TestCase):
    """The default model must never contradict the original: no BL it would have inlined survives."""

    def test_no_surviving_call_targets_a_callee_the_model_inlines(self):
        import image as image_mod

        image = image_mod.Image.load(ROOT / "orig")
        functions = image_mod.functions_by_address(image_mod.load_catalogue())
        linker = inplace.InPlaceLinker(image, functions, inplace.Resolver(functions), image.bss)
        inlinable = {a for a in functions
                     if (body := linker.eligible_original_callee(a)) and inplace.inline_arm_call(0xEB000000, body)}
        calls = contradictions = 0
        for fn in functions.values():
            if fn["isa"] != "arm":
                continue
            data = [tuple(p) for p in (*fn.get("pools", []), *fn.get("literals", []))]
            for lo, hi in fn["code_ranges"]:
                for place in range(lo, hi, 4):
                    if any(a <= place < b for a, b in data):
                        continue
                    word = struct.unpack("<I", image.read(place, 4))[0]
                    if word >> 28 == 0xF or (word >> 24) & 0xF != 0xB:
                        continue  # not BL/BLcond
                    calls += 1
                    contradictions += inplace._arm_branch_target(word, place) in inlinable
        self.assertGreater(len(inlinable), 100)  # 74 empty + 36 one-instruction functions (v3)
        self.assertGreater(calls, 19000)
        self.assertEqual(contradictions, 0)


if __name__ == "__main__":
    unittest.main()
