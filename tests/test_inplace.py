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
        if not BASE <= address <= address + size <= BASE + len(self.mem):
            raise ValueError(f"address {address:#x} is outside code.bin")  # as image.Image.read
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

    def test_reference_implying_an_address_outside_the_image_is_refused(self):
        """Wave4Author2: a candidate's data reference sat where the original holds an instruction
        (0xe1a04000); inferring the section there must be a LinkError, not a crash."""
        obj = self.compile("static const int pair[2] = {0, -1};\nconst int *func_00100000(void) { return pair; }\n")
        mem = Memory()
        for fn in obj.defined_functions():
            sec = obj.sections[fn.shndx]
            data = bytearray(sec.data)
            for rel in sec.relocs:
                struct.pack_into("<I", data, rel.offset, 0xE1A04000)  # an instruction, not an address
            mem.write(int(fn.name[5:], 16), bytes(data))
        with self.assertRaisesRegex(inplace.LinkError, "outside the image"):
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


class ArmCallAbiContextTest(unittest.TestCase):
    """Non-ROM CFG probes for R1's incoming argument and outgoing clobber checks."""

    def context(self, words, site, arguments=None, **kwargs):
        instructions = {insn.address: insn for insn in inplace._ARM.disasm(
            struct.pack(f"<{len(words)}I", *words), BASE)}
        arguments = {inplace.cs_arm.ARM_REG_R0, inplace.cs_arm.ARM_REG_R1} if arguments is None else arguments
        return inplace.arm_call_abi_context(instructions, BASE + site, arguments,
                                           inplace.cs_arm.ARM_CC_AL, **kwargs)

    def test_reviewer_branch_successor_reads_a_clobbered_argument(self):
        # The linear suffix looks clean, but B reaches a read of the pre-call r1.
        words = [0xE92D4010, 0xE1A00004, 0xE3A01007, 0xE5801018, 0xEA000001,
                 0xE3A01000, 0xE12FFF1E, 0xE2812001, 0xE12FFF1E]
        self.assertEqual(self.context(words, 12), "reads_arguments")

    def test_both_conditional_successors_must_kill_caller_saved_values(self):
        for read_in_taken_arm in (False, True):
            words = [0xE92D4010, 0xE1A00004, 0xE3A01007, 0xE5801018, 0xE3500000,
                     0x1A000001, 0xE3A01000, 0xE12FFF1E, 0xE2812001, 0xE12FFF1E]
            if not read_in_taken_arm:
                words[6], words[8] = words[8], words[6]
            with self.subTest(read_in_taken_arm=read_in_taken_arm):
                self.assertEqual(self.context(words, 12), "reads_arguments")

    def test_reviewer_branch_skips_the_nearby_argument_setup(self):
        # r1 was modified at entry; the only reachable incoming edge bypasses the nearby MOV.
        words = [0xE3A01002, 0xEA000002, 0xE320F000, 0xE320F000, 0xE3A01007,
                 0xE5801018, 0xE3A01000, 0xE12FFF1E]
        self.assertEqual(self.context(words, 20), "arguments_not_materialised")

    def test_unmodified_entry_arguments_can_pass_through_without_nearby_moves(self):
        words = [0xE92D4010, 0xE1A04002, 0xE5801000, 0xE3A01000, 0xE5804008, 0xE8BD8010]
        self.assertEqual(self.context(words, 8), "consistent")
        # The new incoming-parameter allowance never waives the post-site clobber check.
        words[3] = 0xE2812001
        self.assertEqual(self.context(words, 8), "reads_arguments")

    def test_entry_arguments_are_unmodified_on_every_cfg_path(self):
        words = [0xE3520000, 0x1A000001, 0xE3A04000, 0xE320F000,
                 0xE5801018, 0xE3A01000, 0xE12FFF1E]
        self.assertEqual(self.context(words, 16), "consistent")
        # One arm now changes r1 too far before the site; the other leaves it untouched.
        words = [0xE3520000, 0x1A000004, 0xE3A01007, 0xE320F000, 0xE320F000,
                 0xE320F000, 0xE320F000, 0xE5801018, 0xE3A01000, 0xE12FFF1E]
        self.assertEqual(self.context(words, 28), "arguments_not_materialised")

    def test_prior_calls_break_entry_argument_provenance(self):
        words = [0xE92D4010, 0xEB000020, 0xE5801018, 0xE3A01000, 0xE8BD8010]
        self.assertEqual(self.context(words, 8), "arguments_not_materialised")
        words[1] = 0xE5801000
        self.assertEqual(self.context(words, 8, call_sites={BASE + 4}), "arguments_not_materialised")

    def test_conditional_definition_does_not_establish_a_missing_argument(self):
        words = [0xE3A01001, 0xE320F000, 0xE320F000, 0xE320F000, 0xE320F000,
                 0xE3500000, 0x13A01007, 0xE5801018, 0xE3A01000, 0xE12FFF1E]
        self.assertEqual(self.context(words, 28), "arguments_not_materialised")

    def test_unknown_successors_fail_closed(self):
        words = [0xE92D4010, 0xE1A00004, 0xE3A01007, 0xE5801018, 0xE12FFF14]
        self.assertEqual(self.context(words, 12), "unresolved_control_flow")


class RemovedCalleeModelTest(unittest.TestCase):
    """V3's candidate-only exception is narrower than ordinary original-image inlining."""

    MODEL = "armlink5-inline-branchnop-v3"
    SITES = (12, 28)

    def fixture(self, body=None, call=0xEBFFFFFE):
        body = body if body is not None else struct.pack("<I", 0xE5801018) + BX_LR
        helper = inplace.Symbol("set_field18", 0, len(body), "STT_FUNC", "STB_GLOBAL", 2)
        caller = inplace.Symbol("func_00100000", 0, 40, "STT_FUNC", "STB_GLOBAL", 1)
        # Independently authored ARM fixture: frame, argument moves, two setter calls, return.
        words = [0xE92D4010, 0xE1A00004, 0xE3A01007, call, 0xE3A01000,
                 0xE1A00004, 0xE3A01008, call, 0xE3500000, 0xE8BD8010]
        decision = inplace.inline_arm_call(call, body)
        if call >> 28 != 0xE or decision is not None and decision[1] >> 28 not in (0xE, 0xF):
            words[4] = 0xE3500000  # establish fresh flags, never rely on a preceding call's flags
        raw = struct.pack("<10I", *words)
        obj = inplace.ObjectFile.__new__(inplace.ObjectFile)
        obj.sections = {
            1: inplace.Section(1, "i.caller", raw, len(raw), False, True, False,
                               [inplace.Reloc(site, inplace.R_ARM_CALL, helper) for site in self.SITES],
                               [(0, "$a")], align=4),
            2: inplace.Section(2, "i.set_field18", body, len(body), False, True, False,
                               mapping=[(0, "$a")], align=4),
        }
        obj.symbols = [caller, helper]
        catalogue = {BASE: {"address": BASE, "size": len(raw), "isa": "arm"}}
        mem = Memory()
        original = bytearray(raw)
        for site in self.SITES:
            struct.pack_into("<I", original, site, decision[1] if decision else 0xE5801018)
        mem.write(BASE, original)
        return obj, mem, catalogue

    def linker(self, mem, catalogue, names=None, model=None):
        return inplace.InPlaceLinker(mem, catalogue, inplace.Resolver(catalogue, names),
                                     (0x200000, 0x201000), model or self.MODEL)

    def test_two_setter_calls_inline_and_the_unreferenced_section_is_dropped(self):
        obj, mem, catalogue = self.fixture()
        linker = self.linker(mem, catalogue)
        linked = linker.link(obj)
        self.assertEqual(set(linked), {1})
        self.assertEqual(linked[1][1], mem.read(BASE, 40))
        self.assertEqual([rewrite.offset for rewrite in linker.rewrites], list(self.SITES))
        self.assertEqual([rewrite.target for rewrite in linker.rewrites], [None, None])
        self.assertEqual([rewrite.rule for rewrite in linker.rewrites], ["inline_insn"] * 2)
        self.assertEqual(linker.removed_callees[0]["symbols"], ["set_field18"])
        self.assertEqual(linker.removed_callees[0]["size"], 8)
        self.assertEqual(linker.rewrites[0].record()["removed_callee"], linker.removed_callees[0])
        with self.assertRaisesRegex(inplace.LinkError, "unresolved symbol"):
            self.linker(mem, catalogue, model="armlink5-inline-branchnop-v2").link(obj)

    def test_entry_argument_forwarding_is_allowed_by_the_full_link(self):
        obj, mem, catalogue = self.fixture()
        candidate, original = bytearray(obj.sections[1].data), bytearray(mem.read(BASE, 40))
        for offset in (4, 8):
            struct.pack_into("<I", candidate, offset, 0xE320F000)
            struct.pack_into("<I", original, offset, 0xE320F000)
        obj.sections[1].data = bytes(candidate)
        mem.write(BASE, original)
        linker = self.linker(mem, catalogue)
        self.assertEqual(linker.link(obj)[1][1], mem.read(BASE, 40))
        self.assertEqual([rewrite.offset for rewrite in linker.rewrites], list(self.SITES))

    def test_a_definition_in_another_candidate_object_is_used_not_an_external_guess(self):
        obj, mem, catalogue = self.fixture()
        helper = obj.symbols[1]
        helper_object = inplace.ObjectFile.__new__(inplace.ObjectFile)
        helper_object.sections, helper_object.symbols = {2: obj.sections.pop(2)}, [helper]
        obj.symbols.remove(helper)
        extern = inplace.Symbol(helper.name, 0, 0, "STT_NOTYPE", "STB_GLOBAL", "SHN_UNDEF")
        obj.symbols.append(extern)
        for rel in obj.sections[1].relocs:
            rel.symbol = extern
        combined = inplace.ObjectFile.combine([obj, helper_object], ["caller.c", "setter.c"])
        linker = self.linker(mem, catalogue)
        linked = linker.link(combined)
        self.assertEqual(len(linked), 1)
        self.assertEqual(next(iter(linked.values()))[1], mem.read(BASE, 40))
        self.assertEqual(linker.removed_callees[0]["object"], "setter.c")
        self.assertEqual(extern.shndx, "SHN_UNDEF")  # combining does not mutate inputs
        with self.assertRaisesRegex(inplace.LinkError, "unresolved symbol"):
            self.linker(mem, catalogue).link(obj)

    @unittest.skipUnless(HAVE_COMPILER, "pinned compiler not installed (tools/setup.py)")
    def test_reviewer_compiler_used_helper_is_retained(self):
        """F01: armcc's actual __attribute__((used)) marker defeats otherwise eligible R1."""
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "used.c"
            source.write_text(
                "__attribute__((used))\n"
                "void set_field18(unsigned *p, unsigned value) { p[6] = value; }\n",
                encoding="utf-8")
            toolchain.compile_source(toolchain.load_profiles()["game"], source, source.with_suffix(".o"))
            helper = inplace.ObjectFile(source.with_suffix(".o"))
            definition = next(sym for sym in helper.defined_functions() if sym.name == "set_field18")
            self.assertEqual(helper.sections[definition.shndx].size, 8)
            self.assertIn("__tagsym$$used", helper.sections[definition.shndx].retention)
            obj, mem, catalogue = self.fixture()
            obj.sections.pop(2)
            obj.symbols[1].shndx = "SHN_UNDEF"
            combined = inplace.ObjectFile.combine([obj, helper], ["caller.c", "used.c"])
            linker = self.linker(mem, catalogue)
            with self.assertRaisesRegex(inplace.LinkError, r"retained callee .*__tagsym\$\$used"):
                linker.link(combined)
            self.assertEqual(linker.removed_callees, [])
            self.assertEqual(linker.rewrites, [])

    @unittest.skipUnless(HAVE_COMPILER, "pinned compiler not installed (tools/setup.py)")
    def test_compiler_exported_helper_is_retained(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "export.c"
            source.write_text(
                "__declspec(dllexport) void set_field18(unsigned *p, unsigned value) { p[6] = value; }\n",
                encoding="utf-8")
            toolchain.compile_source(toolchain.load_profiles()["game"], source, source.with_suffix(".o"))
            helper = inplace.ObjectFile(source.with_suffix(".o"))
            obj, mem, catalogue = self.fixture()
            obj.sections.pop(2)
            obj.symbols[1].shndx = "SHN_UNDEF"
            linker = self.linker(mem, catalogue)
            with self.assertRaisesRegex(inplace.LinkError, "retained callee .*exported visibility"):
                linker.link(inplace.ObjectFile.combine([obj, helper]))
            self.assertEqual(linker.removed_callees, [])

    def test_keep_entry_first_last_and_exported_roots_never_remove_helpers(self):
        for reason in ("KEEP", "entry", "first", "last", "exported", "unknown retention mechanism"):
            for selector in ("set_field18", "i.set_*"):
                with self.subTest(reason=reason, selector=selector):
                    obj, mem, catalogue = self.fixture()
                    linker = inplace.InPlaceLinker(
                        mem, catalogue, inplace.Resolver(catalogue), (0x200000, 0x201000),
                        self.MODEL, retention_roots={selector: reason})
                    with self.assertRaisesRegex(inplace.LinkError, "retained callee"):
                        linker.link(obj)
                    self.assertEqual(linker.removed_callees, [])
        with self.assertRaisesRegex(inplace.LinkError, "uninterpretable retention root"):
            inplace.InPlaceLinker(mem, catalogue, inplace.Resolver(catalogue), (0x200000, 0x201000),
                                  self.MODEL, retention_roots={"missing": "KEEP"}).link(obj)

    def test_bound_retained_helper_keeps_the_call_as_a_bl(self):
        obj, mem, catalogue = self.fixture()
        obj.symbols.append(inplace.Symbol("__tagsym$$used", 0, 0, "STT_NOTYPE", "STB_LOCAL", 2))
        catalogue[0x100100] = {"address": 0x100100, "size": 8, "isa": "arm"}
        mem.write(0x100100, obj.sections[2].data)
        linker = self.linker(mem, catalogue, {"set_field18": {"address": 0x100100}})
        linked = linker.link(obj)
        self.assertEqual(set(linked), {1, 2})
        self.assertEqual(linker.removed_callees, [])
        self.assertEqual(linker.rewrites, [])
        self.assertNotEqual(linked[1][1], mem.read(BASE, 40))
        for site in self.SITES:
            self.assertEqual(struct.unpack_from("<I", linked[1][1], site)[0], bl(BASE + site, 0x100100, False))

    @unittest.skipUnless(HAVE_COMPILER, "pinned compiler not installed (tools/setup.py)")
    def test_elf_entry_retain_and_unknown_processor_metadata_fail_closed(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "root.c"
            source.write_text("void set_field18(unsigned *p, unsigned v) { p[6] = v; }\n", encoding="utf-8")
            path = source.with_suffix(".o")
            toolchain.compile_source(toolchain.load_profiles()["game"], source, path)
            raw = path.read_bytes()  # only the synthetic compiler object, never private image bytes
            with path.open("rb") as stream:
                elf = inplace.ELFFile(stream)
                index = next(i for i, section in enumerate(elf.iter_sections()) if section.name == "i.set_field18")
                flag_offset = elf["e_shoff"] + index * elf["e_shentsize"] + 8  # Elf32_Shdr.sh_flags
            for flags in (0x10000006, 0x200006, 0x80000006):
                with self.subTest(flags=flags):
                    data = bytearray(raw)
                    struct.pack_into("<I", data, flag_offset, flags)
                    path.write_bytes(data)
                    helper = inplace.ObjectFile(path)
                    obj, mem, catalogue = self.fixture()
                    obj.sections.pop(2)
                    obj.symbols[1].shndx = "SHN_UNDEF"
                    with self.assertRaisesRegex(inplace.LinkError, "retained callee"):
                        self.linker(mem, catalogue).link(inplace.ObjectFile.combine([obj, helper]))
            data = bytearray(raw)
            struct.pack_into("<I", data, 24, 4)  # Elf32_Ehdr.e_entry cannot be interpreted as a section identity
            path.write_bytes(data)
            helper = inplace.ObjectFile(path)
            with self.assertRaisesRegex(inplace.LinkError, "retained callee .*ELF entry point"):
                self.linker(mem, catalogue).link(inplace.ObjectFile.combine([obj, helper]))

    def test_retained_comdat_copy_retains_the_whole_entity(self):
        first, mem, catalogue = self.fixture()
        second, _, _ = self.fixture()
        second.sections.pop(1)
        second.symbols.pop(0)
        first.sections[2].comdat = second.sections[2].comdat = "set_field18"
        second.sections[2].retention = ("entry section",)
        with self.assertRaisesRegex(inplace.LinkError, "retained callee"):
            self.linker(mem, catalogue).link(inplace.ObjectFile.combine([first, second]))

    def test_a_pointer_even_in_an_unplaced_section_prevents_removal(self):
        for kind in ("STT_FUNC", "STT_SECTION"):
            with self.subTest(kind):
                obj, mem, catalogue = self.fixture()
                symbol = obj.symbols[1] if kind == "STT_FUNC" else inplace.Symbol("", 0, 0, kind, "STB_LOCAL", 2)
                obj.symbols.append(symbol)
                obj.sections[3] = inplace.Section(3, ".data.pointer", bytes(4), 4, False, False, True,
                                                  [inplace.Reloc(0, inplace.R_ARM_ABS32, symbol)], align=4)
                with self.assertRaisesRegex(inplace.LinkError, "still referenced"):
                    self.linker(mem, catalogue).link(obj)

    def test_a_pointer_in_another_object_prevents_removal(self):
        obj, mem, catalogue = self.fixture()
        pointer = inplace.ObjectFile.__new__(inplace.ObjectFile)
        symbol = inplace.Symbol("set_field18", 0, 0, "STT_NOTYPE", "STB_GLOBAL", "SHN_UNDEF")
        pointer.symbols = [symbol]
        pointer.sections = {1: inplace.Section(1, ".data.pointer", bytes(4), 4, False, False, True,
                                               [inplace.Reloc(0, inplace.R_ARM_ABS32, symbol)], align=4)}
        with self.assertRaisesRegex(inplace.LinkError, "still referenced"):
            self.linker(mem, catalogue).link(inplace.ObjectFile.combine([obj, pointer]))

    def test_tail_branch_dependency_or_interior_call_prevents_removal(self):
        for case in ("B", "dependency", "interior"):
            with self.subTest(case):
                obj, mem, catalogue = self.fixture()
                section = obj.sections[1]
                rel = section.relocs[-1]
                if case == "B":
                    data = bytearray(section.data)
                    struct.pack_into("<I", data, rel.offset, 0xEAFFFFFE)
                    section.data, rel.type = bytes(data), inplace.R_ARM_JUMP24
                elif case == "dependency":
                    rel.type = inplace.R_ARM_NONE
                else:
                    rel.symbol = inplace.Symbol("inside", 4, 0, "STT_NOTYPE", "STB_LOCAL", 2)
                with self.assertRaisesRegex(inplace.LinkError, "still referenced"):
                    self.linker(mem, catalogue).link(obj)

    def test_a_catalogued_address_never_uses_the_candidate_only_exception(self):
        obj, mem, catalogue = self.fixture()
        catalogue[0x100100] = {"address": 0x100100, "size": 12, "isa": "arm"}
        mem.write(0x100100, struct.pack("<3I", 0xE3A00001, 0xE3A01002, 0xE12FFF1E))
        linker = self.linker(mem, catalogue, {"set_field18": {"address": 0x100100}})
        with self.assertRaisesRegex(inplace.LinkError, "inlining is undecidable"):
            linker.link(obj)
        self.assertEqual(linker.removed_callees, [])
        # An interior address binding cannot disguise an existing function either.
        with self.assertRaisesRegex(inplace.LinkError, "not a catalogued function start"):
            self.linker(mem, catalogue, {"set_field18": {"address": 0x100104}}).link(obj)

    def test_an_alias_cannot_hide_a_catalogued_counterpart(self):
        obj, mem, catalogue = self.fixture()
        alias = inplace.Symbol("func_00100100", 0, 8, "STT_FUNC", "STB_GLOBAL", 2)
        obj.symbols.append(alias)
        catalogue[0x100100] = {"address": 0x100100, "size": 8, "isa": "arm"}
        with self.assertRaises(inplace.LinkError):
            self.linker(mem, catalogue).link(obj)
        # Reserved address names remain reserved even when no such catalogue entry exists.
        obj.symbols.pop()
        obj.symbols[1].name = "func_00100300"
        with self.assertRaisesRegex(inplace.LinkError, "no catalogued function"):
            self.linker(mem, catalogue).link(obj)

    def test_pc_lr_svc_and_branchnop_bodies_keep_calls_and_sections(self):
        for word in (0xE59F0000, 0xE1A0000E, 0xEF00003C, MOV_R0_R0):
            with self.subTest(word=word):
                obj, mem, catalogue = self.fixture(struct.pack("<I", word) + BX_LR)
                obj.symbols[1].name = "func_00100100"
                catalogue[0x100100] = {"address": 0x100100, "size": 8, "isa": "arm"}
                mem.write(0x100100, obj.sections[2].data)
                linker = self.linker(mem, catalogue)
                linked = linker.link(obj)
                self.assertEqual(set(linked), {1, 2})
                self.assertEqual(linker.rewrites, [])
                self.assertEqual(linker.removed_callees, [])
                for site in self.SITES:
                    self.assertEqual(struct.unpack_from("<I", linked[1][1], site)[0], bl(BASE + site, 0x100100, False))
                obj.symbols[1].name = "set_field18"
                with self.assertRaisesRegex(inplace.LinkError, "unresolved symbol"):
                    self.linker(mem, catalogue).link(obj)  # an unplaceable callee is not silently erased

    def test_i3_conditions_are_identical_for_removed_callees(self):
        for call, instruction, replacement in (
                (0x1BFFFFFE, 0xE5801018, 0x15801018),
                (0x1BFFFFFE, 0x15801018, 0x15801018),
                (0xEBFFFFFE, 0x15801018, 0x15801018)):
            with self.subTest(call=call, instruction=instruction):
                obj, mem, catalogue = self.fixture(struct.pack("<I", instruction) + BX_LR, call)
                linker = self.linker(mem, catalogue)
                linked = linker.link(obj)
                self.assertEqual(set(linked), {1})
                self.assertEqual([rewrite.word for rewrite in linker.rewrites], [replacement] * 2)
        for instruction in (0x05801018, 0xF5D0F000):
            with self.subTest(incompatible=instruction):
                obj, mem, catalogue = self.fixture(struct.pack("<I", instruction) + BX_LR, 0x1BFFFFFE)
                with self.assertRaisesRegex(inplace.LinkError, "still referenced"):
                    self.linker(mem, catalogue).link(obj)

    def test_conditional_return_only_calls_drop_the_section_and_emit_unconditional_nops(self):
        for body in (BX_LR, struct.pack("<I", 0xE1A0F00E)):
            with self.subTest(body_size=len(body)):
                obj, mem, catalogue = self.fixture(body, 0x1BFFFFFE)
                linker = self.linker(mem, catalogue)
                self.assertEqual(set(linker.link(obj)), {1})
                self.assertEqual([(rewrite.rule, rewrite.word) for rewrite in linker.rewrites],
                                 [("inline_nop", MOV_R0_R0)] * 2)

    def test_non_call_abi_context_or_thumb_caller_is_refused(self):
        for case in ("missing_argument", "live_argument", "live_flags", "conditional_definition", "thumb"):
            with self.subTest(case):
                obj, mem, catalogue = self.fixture()
                data = bytearray(mem.read(BASE, 40))
                if case == "missing_argument":
                    # Entry pass-through is now valid. Remove both setups after the first
                    # candidate call so its clobber, not the caller's entry r1, reaches site two.
                    struct.pack_into("<I", data, 16, 0xE3A02000)
                    struct.pack_into("<I", data, 24, 0xE3A02008)
                elif case == "live_argument":
                    struct.pack_into("<I", data, 16, 0xE2812001)
                elif case == "live_flags":
                    struct.pack_into("<I", data, 16, 0x03A01000)
                elif case == "conditional_definition":
                    struct.pack_into("<I", data, 16, 0xE3500000)
                    struct.pack_into("<I", data, 20, 0x13A01000)
                    struct.pack_into("<I", data, 24, 0xE2812001)
                else:
                    obj.sections[1].mapping = [(0, "$t")]
                mem.write(BASE, data)
                with self.assertRaisesRegex(inplace.LinkError, "still referenced"):
                    self.linker(mem, catalogue).link(obj)

    def test_no_general_unused_section_elimination_or_abi_breaking_helper(self):
        obj, mem, catalogue = self.fixture()
        obj.sections[1].relocs.clear()
        with self.assertRaisesRegex(inplace.LinkError, "no inlined call"):
            self.linker(mem, catalogue).link(obj)
        for word in (0xE1A00004, 0xE3A04000, 0xE24DD004):
            obj, mem, catalogue = self.fixture(struct.pack("<I", word) + BX_LR)
            with self.assertRaisesRegex(inplace.LinkError, "unresolved symbol"):
                self.linker(mem, catalogue).link(obj)

    def test_combining_candidates_rejects_duplicate_non_comdat_definitions(self):
        first, _, _ = self.fixture()
        second, _, _ = self.fixture()
        with self.assertRaisesRegex(inplace.LinkError, "several candidate objects"):
            inplace.ObjectFile.combine([first, second])

    def test_removed_comdat_copies_must_agree_and_are_dropped_as_one_entity(self):
        first, mem, catalogue = self.fixture()
        second, _, _ = self.fixture()
        second.sections.pop(1)
        second.symbols.pop(0)
        first.sections[2].comdat = second.sections[2].comdat = "set_field18"
        linker = self.linker(mem, catalogue)
        self.assertEqual(len(linker.link(inplace.ObjectFile.combine([first, second]))), 1)
        self.assertEqual(len(linker.removed_callees), 2)
        second.sections[2].data = struct.pack("<I", 0xE580101C) + BX_LR
        with self.assertRaisesRegex(inplace.LinkError, "contradictory removed-callee copies"):
            self.linker(mem, catalogue).link(inplace.ObjectFile.combine([first, second]))



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
