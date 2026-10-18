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
from types import SimpleNamespace

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import inplace  # noqa: E402
import toolchain  # noqa: E402

PROFILE = toolchain.Profile(
    "test", "armcc_504_82",
    ("--cpu=MPCore", "--fpmode=fast", "--apcs=/interwork", "--arm", "--split_sections", "-O3", "-Otime"),
)
HAVE_COMPILER = toolchain.armcc_exe(PROFILE.compiler).is_file()
GAME_PROFILE = toolchain.load_profiles()["game"]

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


@unittest.skipUnless(HAVE_COMPILER, "pinned compiler not installed (tools/setup.py)")
class CppFunctionAliasTest(unittest.TestCase):
    """Real game-profile C1/C2 and virtual/non-virtual D1/D2 ELF aliases."""

    MODEL = "armlink5-inline-branchnop-v6"

    PAIRS = (
        ("_ZN5PlainC1Ev", "_ZN5PlainC2Ev"),
        ("_ZN5PlainD1Ev", "_ZN5PlainD2Ev"),
        ("_ZN7VirtualD1Ev", "_ZN7VirtualD2Ev"),
    )

    @classmethod
    def setUpClass(cls):
        tmp = tempfile.TemporaryDirectory()
        cls.addClassCleanup(tmp.cleanup)
        source = Path(tmp.name) / "aliases.cpp"
        source.write_text(
            "class Plain { public: Plain(); ~Plain(); };\n"
            "Plain::Plain() {}\nPlain::~Plain() {}\n"
            "class Virtual { public: virtual ~Virtual(); };\nVirtual::~Virtual() {}\n",
            encoding="utf-8",
        )
        cls.object_path = source.with_suffix(".o")
        toolchain.compile_source(GAME_PROFILE, source, cls.object_path)

    def fixture(self, binding=0):
        obj = inplace.ObjectFile(self.object_path)
        addresses = {
            index: (BASE if section.executable else TABLE_AT) + index * 0x100
            for index, section in obj.sections.items()
        }
        skipped = {pair[1 - binding] for pair in self.PAIRS}
        names = {
            sym.name: {"address": addresses[sym.shndx] + sym.value}
            for sym in obj.defined_globals() if sym.name not in skipped
        }
        names.update({
            "_ZdlPv": {"address": BASE + 0x8000},
            "_ZTVN10__cxxabiv117__class_type_infoE": {"address": TABLE_AT + 0x8000},
        })
        catalogue = {
            addresses[index]: {"address": addresses[index], "size": sec.size, "isa": "arm"}
            for index, sec in obj.sections.items() if sec.executable
        }
        catalogue[BASE + 0x8000] = {"address": BASE + 0x8000, "size": 16, "isa": "arm"}
        mem, expected = Memory(), {}
        for index, sec in obj.sections.items():
            data = bytearray(sec.data)
            for rel in sec.relocs:
                sym = rel.symbol
                target = (addresses[sym.shndx] + sym.value if isinstance(sym.shndx, int)
                          else names[sym.name]["address"])
                word = struct.unpack_from("<I", data, rel.offset)[0]
                place = addresses[index] + rel.offset
                if rel.type == inplace.R_ARM_JUMP24:
                    addend = word & 0xFFFFFF
                    if addend & 0x800000:
                        addend -= 0x1000000
                    word = (word & 0xFF000000) | (((target + (addend << 2) - place) >> 2) & 0xFFFFFF)
                elif rel.type == inplace.R_ARM_ABS32:
                    word = (target + word) & 0xFFFFFFFF
                elif rel.type == inplace.R_ARM_PREL31:
                    addend = word & 0x7FFFFFFF
                    if addend & 0x40000000:
                        addend -= 0x80000000
                    word = (word & 0x80000000) | ((target + addend - place) & 0x7FFFFFFF)
                else:
                    self.fail(f"unexpected fixture relocation {rel.type}")
                struct.pack_into("<I", data, rel.offset, word)
            expected[index] = bytes(data)
            mem.write(addresses[index], data)
        return obj, mem, catalogue, names, expected

    def linker(self, mem, catalogue, names, model=MODEL):
        return inplace.InPlaceLinker(mem, catalogue, inplace.Resolver(catalogue, names),
                                     (0x200000, 0x201000), model)

    def test_game_compiler_emits_zero_size_constructor_and_destructor_aliases(self):
        obj = inplace.ObjectFile(self.object_path)
        symbols = {sym.name: sym for sym in obj.defined_functions()}
        for complete, base in self.PAIRS:
            with self.subTest(complete=complete):
                full, zero = symbols[complete], symbols[base]
                self.assertEqual((full.shndx, full.value), (zero.shndx, zero.value))
                self.assertEqual((full.size, zero.size), (obj.sections[full.shndx].size, 0))
                self.assertIn(full.shndx, inplace.function_aliases(obj))

    def test_either_abi_name_places_one_unchanged_complete_section(self):
        for model, policy in inplace.LINKER_MODELS.items():
            if not policy.function_entry_aliases:
                continue
            for binding in (0, 1):
                with self.subTest(model=model, binding=binding):
                    obj, mem, catalogue, names, expected = self.fixture(binding)
                    metadata = [vars(sym).copy() for sym in obj.symbols]
                    linker = self.linker(mem, catalogue, names, model)
                    linked = linker.link(obj)
                    for index, (placed, data) in linked.items():
                        self.assertEqual(data, expected[index])
                        self.assertEqual(len(data), placed.section.size)
                    for pair in self.PAIRS:
                        full, zero = (next(sym for sym in obj.symbols if sym.name == name) for name in pair)
                        placements = {index: row[0] for index, row in linked.items()}
                        self.assertEqual(linker._symbol_target(full, obj, placements),
                                         linker._symbol_target(zero, obj, placements))
                    self.assertEqual([vars(sym) for sym in obj.symbols], metadata)
                    self.assertEqual(linker.removed_callees, [])

    def test_frozen_models_keep_their_original_alias_requirements(self):
        for model, policy in inplace.LINKER_MODELS.items():
            if policy.function_entry_aliases:
                continue
            for binding in (0, 1):
                with self.subTest(model=model, binding=binding):
                    obj, mem, catalogue, names, _ = self.fixture(binding)
                    with self.assertRaisesRegex(inplace.LinkError, "unresolved symbol"):
                        self.linker(mem, catalogue, names, model).link(obj)
            obj, mem, catalogue, names, expected = self.fixture()
            for complete, base in self.PAIRS:
                names[base] = dict(names[complete])
            linked = self.linker(mem, catalogue, names, model).link(obj)
            self.assertTrue(all(data == expected[index] for index, (_, data) in linked.items()))

    def test_zero_size_requires_an_identical_full_section_function_sibling(self):
        for case in ("interior", "no_extent", "partial_extent", "other_isa", "not_executable"):
            with self.subTest(case=case):
                obj, mem, catalogue, names, _ = self.fixture()
                full, zero = (next(sym for sym in obj.symbols if sym.name == name) for name in self.PAIRS[1])
                if case == "interior":
                    zero.value = 4
                elif case == "no_extent":
                    full.size = 0
                elif case == "partial_extent":
                    full.size -= 1
                elif case == "other_isa":
                    zero.value = 1
                else:
                    obj.sections[full.shndx].executable = False
                with self.assertRaisesRegex(inplace.LinkError, "zero-size function"):
                    self.linker(mem, catalogue, names).link(obj)

    def test_a_zero_size_data_symbol_cannot_borrow_a_function_binding(self):
        for kind in ("STT_OBJECT", "STT_NOTYPE"):
            for model, policy in inplace.LINKER_MODELS.items():
                if not policy.function_entry_aliases:
                    continue
                with self.subTest(kind=kind, model=model):
                    obj, mem, catalogue, names, _ = self.fixture()
                    zero = next(sym for sym in obj.symbols if sym.name == self.PAIRS[1][1])
                    zero.kind = kind
                    with self.assertRaisesRegex(inplace.LinkError, "global .* has no address binding"):
                        self.linker(mem, catalogue, names, model).link(obj)

    def test_alias_bindings_must_agree_and_name_a_catalogued_start(self):
        for case in ("conflict", "interior", "absent"):
            with self.subTest(case=case):
                obj, mem, catalogue, names, _ = self.fixture()
                complete, base = self.PAIRS[1]
                address = names[complete]["address"]
                names[base] = {"address": address + (0x100 if case == "conflict" else 1 if case == "interior" else 0x9000)}
                reason = "aliases resolve to different addresses" if case == "conflict" else "not a catalogued function start"
                with self.assertRaisesRegex(inplace.LinkError, reason):
                    self.linker(mem, catalogue, names).link(obj)

    def test_swapping_the_abi_name_with_the_recorded_extent_is_equivalent(self):
        obj, mem, catalogue, names, expected = self.fixture(binding=1)
        for complete, base in self.PAIRS:
            full, zero = (next(sym for sym in obj.symbols if sym.name == name) for name in (complete, base))
            full.size, zero.size = zero.size, full.size
        linked = self.linker(mem, catalogue, names).link(obj)
        self.assertTrue(all(data == expected[index] for index, (_, data) in linked.items()))


class CompositeAliasSwitchTest(unittest.TestCase):
    """V6 must exercise the v4 CFG and alias proof together, not select either one."""

    MODEL = "armlink5-inline-branchnop-v6"

    def fixture(self, form="words", alias_call=False, **kwargs):
        fixture = BoundedSwitchModelTest().fixture(form, **kwargs)
        alias = inplace.Symbol("base_variant", 0, 0, "STT_FUNC", "STB_GLOBAL", 2)
        fixture.object.symbols.append(alias)
        if alias_call:
            fixture.object.sections[1].relocs[0].symbol = alias
        return fixture

    def linker(self, fixture, model=MODEL):
        return inplace.InPlaceLinker(fixture.memory, fixture.catalogue, inplace.Resolver(fixture.catalogue),
                                     (0x200000, 0x201000), model)

    def test_v6_composes_both_switch_forms_and_both_alias_call_spellings(self):
        for form in ("words", "branches"):
            for alias_call in (False, True):
                with self.subTest(form=form, alias_call=alias_call):
                    fixture = self.fixture(form, alias_call)
                    metadata = [vars(sym).copy() for sym in fixture.object.symbols]
                    with self.assertRaisesRegex(inplace.LinkError, "unresolved symbol"):
                        self.linker(fixture, "armlink5-inline-branchnop-v4").link(fixture.object)
                    linker = self.linker(fixture)
                    linked = linker.link(fixture.object)
                    self.assertEqual(set(linked), {1})
                    self.assertEqual(linked[1][1], fixture.original)
                    self.assertEqual([row.target for row in linker.rewrites], [None])
                    self.assertEqual(linker.removed_callees[0]["symbols"], ["base_variant", "set_field18"])
                    self.assertEqual([vars(sym) for sym in fixture.object.symbols], metadata)

    def test_v6_aliases_do_not_bypass_v4_endian_state_refusal(self):
        for form in ("words", "branches"):
            for state in (BoundedSwitchModelTest.SETEND_BE, BoundedSwitchModelTest.SETEND_LE):
                with self.subTest(form=form, state=state):
                    fixture = self.fixture(form, alias_call=True, window=(state,))
                    linker = self.linker(fixture)
                    with self.assertRaisesRegex(inplace.LinkError, "still referenced"):
                        linker.link(fixture.object)
                    self.assertEqual(linker.rewrites, [])
                    self.assertEqual(linker.removed_callees, [])

    def test_v6_aliases_refuse_omitted_architectural_guard_writes(self):
        # Independent F03 fixtures: these loads/transfers write the bounded index even
        # when the installed decoder's register-access list omits their destinations.
        for name, word in (("ldrexd", 0xE1B02F9F), ("predicated ldrexd", 0x31B02F9F),
                           ("mrrc", 0xEC532F00)):
            for form in ("words", "branches"):
                with self.subTest(instruction=name, form=form):
                    fixture = self.fixture(form, alias_call=True, window=(word,))
                    linker = self.linker(fixture)
                    with self.assertRaisesRegex(inplace.LinkError, "still referenced"):
                        linker.link(fixture.object)
                    self.assertEqual(linker.rewrites, [])
                    self.assertEqual(linker.removed_callees, [])

    def test_v6_aliases_refuse_all_rfe_forms_and_owner_placements(self):
        # Independently encoded exception returns, with both the review's r4 frame
        # and an sp frame: none proves PC/CPSR or little-endian state preservation.
        variants = (("rfeia", 0xF8BD0A00), ("rfeda", 0xF83D0A00),
                    ("rfedb", 0xF93D0A00), ("rfeib", 0xF9BD0A00))
        for name, encoding in variants:
            for base in (4, 13):
                word = (encoding & ~(0xF << 16)) | base << 16
                for form in ("words", "branches"):
                    for placement in ("window", "before_compare", "one_predecessor", "unreachable"):
                        with self.subTest(instruction=name, base=base, form=form, placement=placement):
                            if placement == "window":
                                kwargs = {"window": (word,)}
                            elif placement == "before_compare":
                                kwargs = {"compare": word, "window": (
                                    JumpTableContextTest.CMP_R2_3, JumpTableContextTest.MOV_R4_R0)}
                            elif placement == "one_predecessor":
                                kwargs = {"compare": JumpTableContextTest.branch(BASE + 4, BASE + 12, 0x1),
                                          "window": (word, JumpTableContextTest.CMP_R2_3,
                                                     JumpTableContextTest.MOV_R4_R0)}
                            else:
                                kwargs = {"suffix": (word,)}
                            fixture = self.fixture(form, alias_call=True, **kwargs)
                            linker = self.linker(fixture)
                            with self.assertRaisesRegex(inplace.LinkError, "still referenced"):
                                linker.link(fixture.object)
                            self.assertEqual(linker.rewrites, [])
                            self.assertEqual(linker.removed_callees, [])


def synthetic(rtype: int, word: bytes, symbol: inplace.Symbol):
    sec = inplace.Section(1, "i.f", word, len(word), False, True, False)
    rel = inplace.Reloc(0, rtype, symbol)
    sec.relocs.append(rel)
    return sec, rel


def placement_failure_fixture(base, section_name=".constdata", relocation_offset=0, nested=True):
    """Synthetic pointer chain: caller -> common data/typeinfo -> anonymous string section."""
    caller = inplace.Symbol("func_00100000", 0, 4, "STT_FUNC", "STB_GLOBAL", 1)
    common = inplace.Symbol(section_name, 0, 0, "STT_SECTION", "STB_LOCAL", 2)
    string = inplace.Symbol(".conststring", 0, 0, "STT_SECTION", "STB_LOCAL", 3)
    size = relocation_offset + 4
    obj = inplace.ObjectFile.__new__(inplace.ObjectFile)
    obj.sections = {
        1: inplace.Section(1, "i.caller", bytes(4), 4, False, True, False,
                           [inplace.Reloc(0, inplace.R_ARM_ABS32, common)]),
        2: inplace.Section(2, section_name, bytes(size), size, False, False, False,
                           [inplace.Reloc(relocation_offset, inplace.R_ARM_ABS32, string)] if nested else []),
        3: inplace.Section(3, ".conststring", b"name\0", 5, False, False, False),
    }
    obj.symbols = [caller, common, string]
    catalogue = {BASE: {"address": BASE, "size": 4, "isa": "arm"}}
    mem = Memory()
    mem.write(BASE, struct.pack("<I", base))
    return obj, mem, catalogue


class PlacementFailureTest(unittest.TestCase):
    def linker(self, mem, catalogue):
        return inplace.InPlaceLinker(mem, catalogue, inplace.Resolver(catalogue), (0x300000, 0x301000))

    def test_common_section_nested_relocation_refuses_instead_of_raising_valueerror(self):
        """r41256: placement follows an instruction-valued pointer into a common initializer."""
        obj, mem, catalogue = placement_failure_fixture(0xE1A04000)
        with self.assertRaisesRegex(inplace.LinkError, r"\.constdata\+0x0:.*outside the image.*redacted"):
            self.linker(mem, catalogue).link(obj)

    def test_typeinfo_nested_relocation_refuses_instead_of_raising_valueerror(self):
        """r41272: the inferred RTTI section starts at zero and its pointer lies at address four."""
        obj, mem, catalogue = placement_failure_fixture(0, ".rodata._ZTI6Layout", 4)
        with self.assertRaisesRegex(inplace.LinkError, r"_ZTI6Layout\+0x4:.*outside the image.*redacted"):
            self.linker(mem, catalogue).link(obj)

    def test_terminal_constdata_read_remains_a_refusal(self):
        obj, mem, catalogue = placement_failure_fixture(0xE1A04000, nested=False)
        with self.assertRaisesRegex(inplace.LinkError, r"\.constdata:.*outside the image.*redacted"):
            self.linker(mem, catalogue).link(obj)

    def test_every_placement_relocation_translates_image_address_failure(self):
        symbol = inplace.Symbol("local", 0, 0, "STT_SECTION", "STB_LOCAL", 2)
        linker = self.linker(Memory(), CATALOGUE)
        for rtype in (inplace.R_ARM_ABS32, inplace.R_ARM_TARGET1, inplace.R_ARM_REL32,
                      inplace.R_ARM_PREL31, inplace.R_ARM_CALL, inplace.R_ARM_JUMP24,
                      inplace.R_ARM_PC24, inplace.R_ARM_THM_CALL,
                      inplace.R_ARM_THM_JUMP11, inplace.R_ARM_THM_JUMP8):
            with self.subTest(rtype=rtype):
                sec, rel = synthetic(rtype, bytes(4), symbol)
                with self.assertRaisesRegex(inplace.LinkError, "placement reference.*witness address redacted"):
                    linker._original_value(rel, 4, sec)
        for rtype in (inplace.R_ARM_NONE, inplace.R_ARM_V4BX):
            sec, rel = synthetic(rtype, bytes(4), symbol)
            self.assertIsNone(linker._original_value(rel, 4, sec))

    def test_catalogue_alias_section_base_is_checked_before_indexing(self):
        obj, mem, catalogue = placement_failure_fixture(0)
        obj.sections[1].relocs = []
        obj.symbols = [
            inplace.Symbol("constructor", 4, 4, "STT_FUNC", "STB_GLOBAL", 1),
            inplace.Symbol("constructor_alias", 4, 4, "STT_FUNC", "STB_GLOBAL", 1),
        ]
        names = {name: {"address": BASE} for name in ("constructor", "constructor_alias")}
        linker = inplace.InPlaceLinker(mem, catalogue, inplace.Resolver(catalogue, names), (0x300000, 0x301000))
        with self.assertRaisesRegex(inplace.LinkError, "section offset 0x4.*section base 0xffffc.*not a catalogued"):
            linker.link(obj)

    def test_same_address_aliases_still_share_one_catalogue_placement(self):
        obj, mem, catalogue = placement_failure_fixture(0)
        obj.sections[1].relocs = []
        obj.symbols = [
            inplace.Symbol("constructor", 0, 4, "STT_FUNC", "STB_GLOBAL", 1),
            inplace.Symbol("constructor_alias", 0, 0, "STT_FUNC", "STB_GLOBAL", 1),
        ]
        names = {name: {"address": BASE} for name in ("constructor", "constructor_alias")}
        linker = inplace.InPlaceLinker(mem, catalogue, inplace.Resolver(catalogue, names), (0x300000, 0x301000))
        placed, data = linker.link(obj)[1]
        self.assertEqual((placed.base, placed.origin, data), (BASE, "catalogue", bytes(4)))


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
        words = [0xE92D4010, 0xEB000020, 0xE5802018, 0xE3A01000, 0xE8BD8010]
        arguments = {inplace.cs_arm.ARM_REG_R0, inplace.cs_arm.ARM_REG_R2}
        self.assertEqual(self.context(words, 8, arguments), "arguments_not_materialised")
        words[1] = 0xE5801000
        self.assertEqual(self.context(words, 8, arguments, call_sites={BASE + 4}), "arguments_not_materialised")

    def test_call_results_are_inputs_without_artificial_argument_moves(self):
        # A 32-bit result in r0, or a two-word result in r0/r1, feeds the removed helper.
        for arguments in ({inplace.cs_arm.ARM_REG_R0},
                          {inplace.cs_arm.ARM_REG_R0, inplace.cs_arm.ARM_REG_R1}):
            with self.subTest(arguments=arguments):
                instruction = 0xE2000102 if len(arguments) == 1 else 0xE0000001
                words = [0xE92D4010, 0xEB000020, instruction, 0xE3A01000, 0xE8BD8010]
                self.assertEqual(self.context(words, 8, arguments), "consistent")
                words[3] = 0xE2812001
                self.assertEqual(self.context(words, 8, arguments), "reads_arguments")

    def test_call_result_provenance_must_hold_on_every_incoming_path(self):
        # Taken arm bypasses the call, retaining an old modified r0 instead of a result.
        words = [0xE3A00001, 0xE3520000, 0x1A000003, 0xE320F000, 0xE320F000,
                 0xEB000020, 0xE320F000, 0xE2000102, 0xE3500000, 0xE12FFF1E]
        self.assertEqual(self.context(words, 28, {inplace.cs_arm.ARM_REG_R0}), "arguments_not_materialised")
        # A conditional BL alone also cannot establish a result on every incoming path.
        words[2], words[5] = 0xE320F000, 0x1B000020
        self.assertEqual(self.context(words, 28, {inplace.cs_arm.ARM_REG_R0}), "arguments_not_materialised")

    def test_conditional_definition_does_not_establish_a_missing_argument(self):
        words = [0xE3A01001, 0xE320F000, 0xE320F000, 0xE320F000, 0xE320F000,
                 0xE3500000, 0x13A01007, 0xE5801018, 0xE3A01000, 0xE12FFF1E]
        self.assertEqual(self.context(words, 28), "arguments_not_materialised")

    def test_unknown_successors_fail_closed(self):
        words = [0xE92D4010, 0xE1A00004, 0xE3A01007, 0xE5801018, 0xE12FFF14]
        self.assertEqual(self.context(words, 12), "unresolved_control_flow")


class JumpTableContextTest(unittest.TestCase):
    """Compiler-free: the bounded ARMCC jump-table forms in R1's call-context CFG.

    Every layout is the pattern armcc 5.04 emits (`cmp rM, #N; ...; ldrlo pc, [pc, rM, lsl #2]; b default;
    <N words>` or `addlo pc, pc, rM, lsl #2; b default; <N branches>`), written out independently."""

    PUSH, POP, NOP, STORE = 0xE92D4010, 0xE8BD8010, 0xE320F000, 0xE5801018  # STORE: str r1, [r0, #0x18]
    CMP_R2_3, MOV_R4_R0, MOV_R1_7 = 0xE3520003, 0xE1A04000, 0xE3A01007
    LDRLO, ADDLO = 0x379FF102, 0x308FF102  # ldrlo pc, [pc, r2, lsl #2] / addlo pc, pc, r2, lsl #2
    ARGUMENTS = {inplace.cs_arm.ARM_REG_R0, inplace.cs_arm.ARM_REG_R1}

    @staticmethod
    def branch(place, target, condition=0xE):
        return (condition << 28) | 0x0A000000 | (((target - place - 8) >> 2) & 0xFFFFFF)

    def caller(self, form="words", compare=CMP_R2_3, window=(MOV_R4_R0,), writer=None, fall=POP, entries=3):
        """Layout: PUSH, compare, window, writer, fall, table, then the cases: case 0 (index 0) loads r1
        and runs the removed setter (the site), every other index returns."""
        writer = writer if writer is not None else (self.LDRLO if form == "words" else self.ADDLO)
        head = [self.PUSH, compare, *window, writer, fall]
        table = 4 * len(head)
        body = table + 4 * entries
        targets = [body, body + 8] + [body + 16] * (entries - 2)
        entry_words = ([BASE + target for target in targets] if form == "words"
                       else [self.branch(BASE + table + 4 * slot, BASE + target) for slot, target in enumerate(targets)])
        words = [*head, *entry_words, self.MOV_R1_7, self.STORE, self.POP, self.NOP, self.POP]
        pool = range(len(head), len(head) + entries) if form == "words" else range(0)
        return SimpleNamespace(words=words, pool=pool, site=body + 4, writer=table - 8, table=table, body=body, targets=targets)

    def decode(self, caller, with_words=True):
        instructions = {BASE + 4 * index: next(inplace._ARM.disasm(struct.pack("<I", word), BASE + 4 * index, 1))
                        for index, word in enumerate(caller.words) if index not in caller.pool}
        pool_words = {BASE + 4 * index: caller.words[index] for index in caller.pool} if with_words else {}
        return instructions, pool_words

    def context(self, caller, with_words=True, **kwargs):
        instructions, pool_words = self.decode(caller, with_words)
        return inplace.arm_call_abi_context(instructions, BASE + caller.site, self.ARGUMENTS, inplace.cs_arm.ARM_CC_AL,
                                            kwargs.pop("entries", None), None, pool_words, bounded_switches=True, **kwargs)

    def tables(self, caller, with_words=True, entries=None):
        instructions, pool_words = self.decode(caller, with_words)
        flows = {address: inplace._arm_call_flow(insn, bounded_switches=True) for address, insn in instructions.items()}
        return inplace._arm_jump_tables(instructions, flows, entries or {BASE}, pool_words)

    def test_word_table_resolves_exactly_the_bounded_entries(self):
        caller = self.caller()
        self.assertEqual(self.context(caller), "consistent")
        writer = BASE + caller.writer
        self.assertEqual(self.tables(caller), {writer: [writer + 4, *(BASE + target for target in dict.fromkeys(caller.targets))]})
        self.assertEqual(self.context(caller, with_words=False), "unresolved_control_flow")

    def test_only_whole_aligned_pool_words_inside_the_extent_are_available(self):
        function = {"address": BASE, "size": 16, "pools": [[BASE - 8, BASE + 5]],
                    "literals": [[BASE + 9, BASE + 32]]}
        self.assertEqual(set(inplace.arm_pool_words(Memory(), function)), {BASE, BASE + 12})
        function["pools"], function["literals"] = [[BASE + 1, BASE + 14]], []
        self.assertEqual(set(inplace.arm_pool_words(Memory(), function)), {BASE + 4, BASE + 8})

    def test_ls_bound_adds_one_entry_and_lo_does_not(self):
        compare, ldrls = 0xE3520002, 0x979FF102  # cmp r2, #2; ldrls: indexes 0..2, three entries
        caller = self.caller(compare=compare, writer=ldrls)
        self.assertEqual(self.context(caller), "consistent")
        self.assertEqual(len(self.tables(caller)[BASE + caller.writer]) - 1, len(set(caller.targets)))
        # cmp r2, #3 with LS reaches four entries: three words are not enough, four resolve.
        three = self.caller(writer=ldrls)
        self.assertEqual(self.context(three), "unresolved_control_flow")
        four = self.caller(writer=ldrls, entries=4)
        self.assertEqual(self.context(four), "consistent")
        self.assertEqual(self.context(self.caller(entries=4)), "consistent")  # LO: the fourth word is never read
        # The bound cannot be read from the table: cmp r2, #4 with LO needs a fourth word the pool lacks.
        self.assertEqual(self.context(self.caller(compare=0xE3520004)), "unresolved_control_flow")

    def test_branch_table_resolves_exactly_the_bounded_entries(self):
        caller = self.caller("branches")
        self.assertEqual(self.context(caller), "consistent")
        writer = BASE + caller.writer
        self.assertEqual(self.tables(caller), {writer: [writer + 4, *(writer + 8 + 4 * slot for slot in range(3))]})
        ls = self.caller("branches", compare=0xE3520002, writer=0x908FF102)  # addls: three entries
        self.assertEqual(len(self.tables(ls)[BASE + ls.writer]) - 1, 3)
        # LO with cmp #4 reads an entry that is plain code (mov r1, #7), not a branch.
        self.assertEqual(self.context(self.caller("branches", compare=0xE3520004)), "unresolved_control_flow")

    def test_branch_table_entries_must_be_unconditional_branches_inside_the_function(self):
        for case in ("nop", "conditional", "leaves", "return"):
            with self.subTest(case):
                caller = self.caller("branches")
                place, index = BASE + caller.table + 4, caller.table // 4 + 1
                caller.words[index] = {
                    "nop": self.NOP,
                    "conditional": self.branch(place, BASE + caller.targets[1], 0x1),
                    "leaves": self.branch(place, BASE + 0x1000),
                    "return": 0xE12FFF1E,
                }[case]
                self.assertEqual(self.context(caller), "unresolved_control_flow")

    def test_word_table_entries_must_name_instructions_of_this_function(self):
        for case, target in (("outside", BASE + 0x1000), ("before", BASE - 4), ("thumb", BASE + 0x21),
                             ("pool word", BASE + 0x14), ("misaligned", BASE + 0x22)):
            with self.subTest(case):
                caller = self.caller()
                caller.words[caller.table // 4 + 1] = target
                self.assertEqual(self.context(caller), "unresolved_control_flow")

    def test_every_table_target_is_checked_after_a_site_before_the_switch(self):
        # The removed setter precedes `cmp r4, #3; ldrlo pc, [pc, r4, lsl #2]`; r1 is clobbered by the call.
        for reads_clobbered_r1 in (False, True):
            with self.subTest(reads_clobbered_r1=reads_clobbered_r1):
                words = [self.PUSH, 0xE1A04002, self.MOV_R1_7, self.STORE, 0xE3540003, 0x379FF104, self.POP,
                         BASE + 0x28, BASE + 0x2c, BASE + 0x34, self.POP, 0xE1A00001 if reads_clobbered_r1 else self.NOP,
                         self.POP, self.POP]
                caller = SimpleNamespace(words=words, pool=range(7, 10), site=12)
                self.assertEqual(self.context(caller), "reads_arguments" if reads_clobbered_r1 else "consistent")

    def test_a_table_successor_cannot_bypass_argument_setup(self):
        for form in ("words", "branches"):
            with self.subTest(form=form):
                caller = self.caller(form, window=(self.MOV_R4_R0, 0xE3A01002))
                self.assertEqual(self.context(caller), "consistent")
                slot = caller.table // 4 + 1
                caller.words[slot] = (BASE + caller.site if form == "words"
                                      else self.branch(BASE + 4 * slot, BASE + caller.site))
                self.assertEqual(self.context(caller), "arguments_not_materialised")

    def test_the_bound_must_come_from_a_plain_immediate_compare_of_the_index(self):
        for case, compare in (("other register", 0xE3530003), ("register operand", 0xE1520003),
                              ("conditional compare", 0x13520003), ("compare negative", 0xE3720003),
                              ("no compare", self.NOP), ("test", 0xE1120003)):
            with self.subTest(case):
                for form in ("words", "branches"):
                    self.assertEqual(self.context(self.caller(form, compare=compare)), "unresolved_control_flow")
        instruction = next(inplace._ARM.disasm(struct.pack("<I", 0xE3520C01), 0, 1))  # cmp r2, #0x100 (rotated)
        self.assertEqual(inplace._arm_compare_bound(instruction), (2, 0x100))

    def test_nothing_between_the_compare_and_the_writer_may_change_the_index_or_flags(self):
        for case, word in (("index written", 0xE1A02002), ("conditional index written", 0x11A02002),
                           ("flags by subs", 0xE2555001), ("flags by tst", 0xE1150005),
                           ("flags by msr", 0xE128F000), ("flags by mrc", 0xEE10FF10),
                           ("flags by vmrs", 0xEEF1FA10), ("flags by call", 0xEB000010),
                           ("branch", 0x1A000000), ("index loaded", 0xE5922000)):
            with self.subTest(case):
                self.assertEqual(self.context(self.caller(window=(self.MOV_R4_R0, word))), "unresolved_control_flow")
        for case, word in (("other register", 0xE1A05000), ("store", 0xE5840000), ("predicated, no flags", 0x13A05001),
                           ("nop", self.NOP)):
            with self.subTest(case, positive=True):
                self.assertEqual(self.context(self.caller(window=(self.MOV_R4_R0, word, word))), "consistent")

    def test_the_writer_may_be_reached_only_by_falling_through_from_the_guard(self):
        caller = self.caller()
        compare, first, writer = BASE + 4, BASE + 8, BASE + caller.writer
        loop = caller.targets[2] // 4  # a case off the site's own path
        for case, target in (("window", first), ("writer", writer)):
            with self.subTest(case):
                patched = self.caller()
                patched.words[loop] = self.branch(BASE + caller.targets[2], target)
                self.assertEqual(self.context(patched), "unresolved_control_flow")
        patched = self.caller()
        patched.words[loop] = self.branch(BASE + caller.targets[2], compare)  # re-running the compare is fine
        self.assertEqual(self.context(patched), "consistent")
        for entry in (first, writer):
            with self.subTest(entry=entry):
                self.assertEqual(self.context(caller, entries={BASE, entry}), "unresolved_control_flow")
        # A table target inside its own guard window is an entry there too.
        again = self.caller()
        again.words[again.table // 4 + 1] = first
        self.assertEqual(self.context(again), "unresolved_control_flow")

    def test_direct_calls_cannot_supply_an_alternate_entry_after_the_guard(self):
        for thumb in (False, True):
            with self.subTest(thumb=thumb):
                caller = self.caller()
                place = BASE + caller.targets[2]
                caller.words[caller.targets[2] // 4] = bl(place, BASE + caller.writer, thumb)
                self.assertEqual(self.context(caller), "unresolved_control_flow")

    def test_another_resolved_table_cannot_enter_after_the_guard(self):
        words = [self.PUSH, 0xE3520001, 0x979FF102, self.POP, BASE + 24, BASE + 36,
                 self.MOV_R1_7, self.STORE, self.POP, 0xE3520001, self.LDRLO, self.POP, BASE + 8]
        caller = SimpleNamespace(words=words, pool={4, 5, 12}, site=28)
        self.assertNotIn(BASE + 8, self.tables(caller))
        self.assertEqual(self.context(caller), "unresolved_control_flow")

    def test_the_fall_through_must_leave_the_table(self):
        for case, fall in (("plain instruction", self.MOV_R1_7), ("conditional branch", self.branch(BASE + 0x10, BASE + 0x30, 0x1)),
                           ("call", 0xEB000010), ("nop", self.NOP)):
            with self.subTest(case):
                for form in ("words", "branches"):
                    self.assertEqual(self.context(self.caller(form, fall=fall)), "unresolved_control_flow")
        branch = self.branch(BASE + 0x10, BASE + 0x30)  # armcc's `b default` is equally fine as a return
        self.assertEqual(self.context(self.caller(fall=branch)), "consistent")

    def test_the_default_branch_must_stay_inside_the_callers_code(self):
        for form in ("words", "branches"):
            for target in (BASE - 4, BASE + 0x1000):
                with self.subTest(form=form, target=target):
                    caller = self.caller(form)
                    fall = caller.writer + 4
                    caller.words[fall // 4] = self.branch(BASE + fall, target)
                    self.assertEqual(self.context(caller), "unresolved_control_flow")
        caller = self.caller()
        caller.words[(caller.writer + 4) // 4] = self.branch(BASE + caller.writer + 4, BASE + caller.table)
        self.assertEqual(self.context(caller), "unresolved_control_flow")  # data, not an exit

    def test_only_the_recognised_writer_encodings_are_resolved(self):
        for case, writer in (("hi", 0x879FF102), ("ne", 0x179FF102), ("always", 0xE79FF102), ("shift 3", 0x379FF182),
                             ("subtracted index", 0x371FF102), ("writeback", 0x37BFF102), ("no shift", 0x379FF002),
                             ("other base", 0x3790F102), ("mov pc", 0x31A0F002), ("bx index", 0x312FFF12),
                             ("add, shift 3", 0x308FF182), ("sub", 0x304FF102), ("add, set flags", 0x309FF102),
                             ("add, register shift", 0x308FF112), ("pc index", 0x379FF10F)):
            with self.subTest(case):
                self.assertEqual(self.context(self.caller(writer=writer)), "unresolved_control_flow")

    def test_unrelated_indirect_transfers_stay_unresolved_next_to_a_resolved_table(self):
        caller = self.caller()
        caller.words[-1] = 0xE12FFF14  # bx r4 in a reachable case
        self.assertEqual(self.context(caller), "unresolved_control_flow")

    def test_computed_pc_writes_are_not_mistaken_for_returns(self):
        for case, word in (("add pc, r4, lr", 0xE084F00E), ("ldr pc, [lr]", 0xE59EF000),
                           ("movs pc, lr", 0xE1B0F00E), ("bx r4", 0xE12FFF14)):
            with self.subTest(case=case):
                caller = self.caller()
                caller.words[-1] = word
                self.assertEqual(self.context(caller), "unresolved_control_flow")
                caller = self.caller(fall=word)
                self.assertEqual(self.context(caller), "unresolved_control_flow")


class BoundedSwitchEffectsTest(unittest.TestCase):
    """Effect proofs use an explicit decoder-class/encoding allowlist, never absent writes."""

    def test_every_decoder_instruction_class_is_allowlisted_or_refused(self):
        classes = {value for name, value in vars(inplace.cs_arm).items()
                   if name.startswith("ARM_INS_") and isinstance(value, int)}
        self.assertTrue(inplace._ARM_SWITCH_MODELED_IDS <= classes)
        self.assertTrue(classes - inplace._ARM_SWITCH_MODELED_IDS)
        for instruction_id in classes:
            with self.subTest(instruction_id=instruction_id):
                if instruction_id not in inplace._ARM_SWITCH_MODELED_IDS:
                    opaque = SimpleNamespace(id=instruction_id)  # no access metadata or bytes to guess from
                    self.assertIsNone(inplace._arm_switch_effects(opaque))
                    self.assertTrue(inplace._arm_may_change(opaque, inplace.cs_arm.ARM_REG_R2))
                    self.assertEqual(inplace._arm_call_flow(opaque, bounded_switches=True), "unknown")
                invalid = SimpleNamespace(id=instruction_id, bytes=struct.pack("<I", 0xFFFFFFFF))
                self.assertIsNone(inplace._arm_switch_effects(invalid))
                self.assertTrue(inplace._arm_may_change(invalid, inplace.cs_arm.ARM_REG_R2))

    def test_modeled_destinations_and_flags_do_not_depend_on_decoder_write_lists(self):
        # Independent non-ROM A32 encodings; R2 is the prospective bounded table index.
        cases = (
            (0xE1A02002, {2}, False), (0x11A02002, {2}, False), (0xE2504000, {4}, True),
            (0xE5902000, {2}, False), (0xE4920004, {0, 2}, False),
            (0xE1C020D0, {2, 3}, False), (0xE1E200F8, {2}, False),
            (0xE8B0000C, {0, 2, 3}, False), (0xE8B20003, {0, 1, 2}, False),
            (0xE8BD8010, {4, 13, 15}, False),
            (0xE0020190, {2}, False), (0xE0832190, {2, 3}, False),
            (0xEE102A10, {2}, False), (0xEC532B10, {2, 3}, False),
            (0xEEF12A10, {2}, False), (0xEEF1FA10, set(), True),
            (0xEE002A10, set(), False), (0xED920A00, set(), False),
            (0xED2D8B04, {13}, False), (0xECBD8B04, {13}, False),
            (0xE320F000, set(), False),
        )
        for word, registers, flags in cases:
            with self.subTest(word=word):
                insn = next(inplace._ARM.disasm(struct.pack("<I", word), BASE, 1), None)
                self.assertIsNotNone(insn)
                opaque = SimpleNamespace(id=insn.id, bytes=insn.bytes)  # regs_access/groups/update_flags absent
                expected = frozenset(inplace._ARM_NUMBERED[index] for index in registers), flags
                self.assertEqual(inplace._arm_switch_effects(opaque), expected)
                self.assertEqual(inplace._arm_may_change(opaque, inplace.cs_arm.ARM_REG_R2), 2 in registers or flags)


class BoundedSwitchModelTest(unittest.TestCase):
    """Public compiler-free linker consumers: v4 is explicit and endian changes fail closed."""

    MODEL = "armlink5-inline-branchnop-v4"
    SETEND_BE, SETEND_LE = 0xF1010200, 0xF1010000

    def fixture(self, form="words", suffix=(), **kwargs):
        layout = JumpTableContextTest().caller(form, **kwargs)
        layout.words.extend(suffix)
        original = struct.pack(f"<{len(layout.words)}I", *layout.words)
        candidate = bytearray(original)
        struct.pack_into("<I", candidate, layout.site, 0xEBFFFFFE)
        helper_body = struct.pack("<2I", JumpTableContextTest.STORE, 0xE12FFF1E)
        caller = inplace.Symbol("func_00100000", 0, len(original), "STT_FUNC", "STB_GLOBAL", 1)
        helper = inplace.Symbol("set_field18", 0, len(helper_body), "STT_FUNC", "STB_GLOBAL", 2)
        mapping = [(0, "$a"), (layout.table, "$d"), (layout.body, "$a")] if form == "words" else [(0, "$a")]
        obj = inplace.ObjectFile.__new__(inplace.ObjectFile)
        obj.symbols = [caller, helper]
        obj.sections = {
            1: inplace.Section(1, "i.caller", bytes(candidate), len(candidate), False, True, False,
                               [inplace.Reloc(layout.site, inplace.R_ARM_CALL, helper)], mapping, align=4),
            2: inplace.Section(2, "i.set_field18", helper_body, len(helper_body), False, True, False,
                               mapping=[(0, "$a")], align=4),
        }
        obj.section_objects = {1: "caller.c", 2: "setter.c"}
        ranges = ([[BASE, BASE + layout.table], [BASE + layout.body, BASE + len(original)]]
                  if form == "words" else [[BASE, BASE + len(original)]])
        catalogue = {BASE: {"address": BASE, "size": len(original), "isa": "arm", "code_ranges": ranges,
                            "pools": [[BASE + layout.table, BASE + layout.body]] if form == "words" else []}}
        memory = Memory()
        memory.write(BASE, original)
        return SimpleNamespace(object=obj, memory=memory, catalogue=catalogue, original=original, layout=layout)

    def linker(self, fixture, model=None):
        return inplace.InPlaceLinker(fixture.memory, fixture.catalogue, inplace.Resolver(fixture.catalogue),
                                     (0x200000, 0x201000), model or self.MODEL)

    def assert_refused(self, fixture, model=None):
        linker = self.linker(fixture, model)
        with self.assertRaisesRegex(inplace.LinkError, "still referenced"):
            linker.link(fixture.object)
        self.assertEqual(linker.rewrites, [])
        self.assertEqual(linker.removed_callees, [])

    def test_each_exact_form_requires_the_new_selected_model(self):
        for form in ("words", "branches"):
            for ls in (False, True):
                with self.subTest(form=form, ls=ls):
                    writer = (0x979FF102 if form == "words" else 0x908FF102) if ls else None
                    fixture = self.fixture(form, compare=0xE3520002 if ls else 0xE3520003, writer=writer)
                    self.assert_refused(fixture, "armlink5-inline-branchnop-v3")
                    linker = self.linker(fixture)
                    linked = linker.link(fixture.object)
                    self.assertEqual(set(linked), {1})
                    self.assertEqual(linked[1][1], fixture.original)
                    self.assertEqual([(row.rule, row.target) for row in linker.rewrites], [("inline_insn", None)])
                    self.assertEqual(linker.removed_callees[0]["object"], "setter.c")
                    for model in ("strict-v1", "armlink5-inline-branchnop-v2"):
                        with self.assertRaisesRegex(inplace.LinkError, "unresolved symbol"):
                            self.linker(fixture, model).link(fixture.object)

    def test_setend_between_the_guard_and_load_is_refused_by_the_public_consumer(self):
        fixture = self.fixture(window=(self.SETEND_BE,))
        self.assertTrue(all(not BASE <= int.from_bytes(struct.pack("<I", BASE + target), "big")
                            < BASE + len(fixture.original) for target in fixture.layout.targets))
        self.assert_refused(fixture)

    def test_endian_state_before_the_compare_is_not_ignored(self):
        for state in (self.SETEND_BE, self.SETEND_LE, 0xE128F000, 0xEE010F10):
            for form in ("words", "branches"):
                with self.subTest(state=state, form=form):
                    fixture = self.fixture(form, compare=state,
                                           window=(JumpTableContextTest.CMP_R2_3, JumpTableContextTest.MOV_R4_R0))
                    self.assert_refused(fixture)

    def test_a_state_change_on_only_one_incoming_path_is_not_ignored(self):
        branch = JumpTableContextTest.branch(BASE + 4, BASE + 12, 0x1)
        fixture = self.fixture(compare=branch,
                               window=(self.SETEND_BE, JumpTableContextTest.CMP_R2_3, JumpTableContextTest.MOV_R4_R0))
        self.assert_refused(fixture)

    def test_unreachable_state_changes_also_require_an_unimplemented_proof(self):
        self.assert_refused(self.fixture(suffix=(self.SETEND_BE,)))

    def test_exclusive_and_coprocessor_destinations_cannot_bypass_guard(self):
        for name, word in (("LDREXD", 0xE1B02F9F), ("LDREXDLO", 0x31B02F9F), ("MRRC", 0xEC532F00)):
            for form in ("words", "branches"):
                with self.subTest(name=name, form=form):
                    self.assert_refused(self.fixture(form, window=(word,)))

    def test_modeled_load_destinations_and_writeback_cannot_change_index(self):
        for word in (0xE1C020D0, 0x31C020D0, 0xE8B0000C, 0xE8B20003, 0xE4920004, 0xE1E200F8,
                     0xEE102A10, 0xEC532B10, 0xEEF12A10):
            for form in ("words", "branches"):
                with self.subTest(word=word, form=form):
                    self.assert_refused(self.fixture(form, window=(word,)))

    def test_rfe_forms_refuse_every_state_path_placement(self):
        for frame_register in (4, 13):
            for word in (0xF8BD0A00, 0xF83D0A00, 0xF93D0A00, 0xF9BD0A00):
                word = (word & ~0xF0000) | (frame_register << 16)
                insn = next(inplace._ARM.disasm(struct.pack("<I", word), BASE, 1))
                self.assertIsNone(inplace._arm_switch_effects(insn))
                self.assertEqual(inplace._arm_call_flow(insn, bounded_switches=True), "unknown")
                branch = JumpTableContextTest.branch(BASE + 4, BASE + 12, 0x1)
                placements = {
                    "window": {"window": (word,)},
                    "before_compare": {"compare": word, "window": (JumpTableContextTest.CMP_R2_3,)},
                    "one_predecessor": {"compare": branch, "window": (word, JumpTableContextTest.CMP_R2_3)},
                    "unreachable": {"suffix": (word,)},
                }
                for form in ("words", "branches"):
                    for placement, kwargs in placements.items():
                        with self.subTest(frame_register=frame_register, word=word, form=form, placement=placement):
                            self.assert_refused(self.fixture(form, **kwargs))

    def test_frozen_models_keep_their_policy_payload_and_return_classification(self):
        for name, flags in (("strict-v1", (False, False, False, False)),
                            ("armlink5-inline-branchnop-v2", (True, True, False, False)),
                            ("armlink5-inline-branchnop-v3", (True, True, True, False)),
                            (self.MODEL, (True, True, True, True))):
            model = inplace.linker_model(name)
            self.assertEqual((model.inline, model.branchnop, model.removed_inline, model.bounded_switches), flags)
        insn = next(inplace._ARM.disasm(struct.pack("<I", 0xE084F00E), BASE, 1))  # legacy LR-reading PC write
        self.assertEqual(inplace._arm_call_flow(insn), "return")
        self.assertEqual(inplace._arm_call_flow(insn, bounded_switches=True), "unknown")
        self.assertEqual(inplace.DEFAULT_LINKER_MODEL, "armlink5-inline-branchnop-v2")


class RemovedCalleeModelTest(unittest.TestCase):
    """V3's candidate-only exception is narrower than ordinary original-image inlining."""

    MODEL = "armlink5-inline-branchnop-v3"
    ALIAS_MODEL = "armlink5-inline-branchnop-v6"
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

    def comdat_alias_fixture(self, contradictory=False, original_size=8):
        """F01: only the second copy supplies the bound zero-size entry alias."""
        first, mem, catalogue = self.fixture()
        second, _, _ = self.fixture()
        second.sections.pop(1)
        second.symbols.pop(0)
        first.sections[2].comdat = second.sections[2].comdat = "set_field18"
        second.symbols.append(inplace.Symbol("bound_variant", 0, 0, "STT_FUNC", "STB_GLOBAL", 2))
        if contradictory:
            second.sections[2].data = struct.pack("<I", 0xE580101C) + BX_LR
        address = BASE + 0x100
        catalogue[address] = {"address": address, "size": original_size, "isa": "arm"}
        mem.write(address, second.sections[2].data + (BX_LR if original_size == 12 else b""))
        return [first, second], mem, catalogue, {"bound_variant": {"address": address}}

    def test_bound_and_removable_comdat_copies_fail_before_caller_only_linking(self):
        for contradictory, original_size in ((True, 8), (False, 12)):
            for reverse in (False, True):
                with self.subTest(contradictory=contradictory, original_size=original_size, reverse=reverse):
                    objects, mem, catalogue, names = self.comdat_alias_fixture(contradictory, original_size)
                    labels = ["caller_and_copy.cpp", "bound_copy.cpp"]
                    if reverse:
                        objects.reverse()
                        labels.reverse()
                    linker = self.linker(mem, catalogue, names, self.ALIAS_MODEL)
                    with self.assertRaisesRegex(inplace.LinkError, "mixes original-bound and removable callee copies"):
                        linker.link(inplace.ObjectFile.combine(objects, labels))
                    self.assertEqual(linker.removed_callees, [])
                    self.assertEqual(linker.rewrites, [])

    def test_removal_checks_even_a_comdat_copy_with_no_full_sized_function(self):
        objects, mem, catalogue, _ = self.comdat_alias_fixture()
        objects[1].symbols[0].size = 0
        linker = self.linker(mem, catalogue, model=self.ALIAS_MODEL)
        with self.assertRaisesRegex(inplace.LinkError, "unsupported retained callee copy"):
            linker.link(inplace.ObjectFile.combine(objects))
        self.assertEqual(linker.removed_callees, [])
        self.assertEqual(linker.rewrites, [])

    def test_roots_through_either_alias_or_another_copy_prevent_removal(self):
        for selector in ("set_field18", "caller_variant", "bound_variant"):
            for alias_call in (False, True):
                with self.subTest(selector=selector, alias_call=alias_call):
                    objects, mem, catalogue, _ = self.comdat_alias_fixture()
                    alias = inplace.Symbol("caller_variant", 0, 0, "STT_FUNC", "STB_GLOBAL", 2)
                    objects[0].symbols.append(alias)
                    if alias_call:
                        objects[0].sections[1].relocs[-1].symbol = alias
                    linker = inplace.InPlaceLinker(
                        mem, catalogue, inplace.Resolver(catalogue), (0x200000, 0x201000),
                        self.ALIAS_MODEL, retention_roots={selector: "KEEP"})
                    with self.assertRaisesRegex(inplace.LinkError, "retained callee"):
                        linker.link(inplace.ObjectFile.combine(objects))
                    self.assertEqual(linker.removed_callees, [])
                    self.assertEqual(linker.rewrites, [])

    def test_bound_zero_size_alias_keeps_original_equality_even_when_rooted(self):
        for model in (inplace.DEFAULT_LINKER_MODEL, self.MODEL, "armlink5-inline-branchnop-v4", self.ALIAS_MODEL):
            for rooted in (False, True):
                for original_size in (8, 12):
                    with self.subTest(model=model, rooted=rooted, original_size=original_size):
                        objects, mem, catalogue, names = self.comdat_alias_fixture(original_size=original_size)
                        obj = objects[0]
                        alias = inplace.Symbol("bound_variant", 0, 0, "STT_FUNC", "STB_GLOBAL", 2)
                        obj.symbols.append(alias)
                        obj.sections[1].relocs[-1].symbol = alias
                        if not inplace.linker_model(model).function_entry_aliases:
                            names["set_field18"] = dict(names["bound_variant"])
                        if rooted:
                            obj.sections[2].retention = ("KEEP",)
                        linker = self.linker(mem, catalogue, names, model)
                        if original_size == 12:
                            with self.assertRaisesRegex(inplace.LinkError, "inlining is undecidable"):
                                linker.link(obj)
                            self.assertEqual(linker.rewrites, [])
                        else:
                            linked = linker.link(obj)
                            self.assertEqual(linked[1][1], mem.read(BASE, 40))
                            self.assertEqual(linked[2][1], mem.read(BASE + 0x100, original_size))
                            self.assertEqual([row.target for row in linker.rewrites], [BASE + 0x100] * 2)
                        self.assertEqual(linker.removed_callees, [])

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

    def test_prior_call_result_can_feed_a_removed_helper_in_the_full_link(self):
        obj, mem, catalogue = self.fixture(struct.pack("<I", 0xE2000102) + BX_LR)
        ordinary = inplace.Symbol("func_00100100", 0, 0, "STT_FUNC", "STB_GLOBAL", "SHN_UNDEF")
        obj.symbols.append(ordinary)
        obj.sections[1].relocs.insert(0, inplace.Reloc(4, inplace.R_ARM_CALL, ordinary))
        candidate, original = bytearray(obj.sections[1].data), bytearray(mem.read(BASE, 40))
        struct.pack_into("<I", candidate, 4, 0xEBFFFFFE)
        struct.pack_into("<I", original, 4, bl(BASE + 4, 0x100100, False))
        obj.sections[1].data = bytes(candidate)
        mem.write(BASE, original)
        catalogue[0x100100] = {"address": 0x100100, "size": 12, "isa": "arm"}
        mem.write(0x100100, struct.pack("<3I", 0xE3A00000, 0xE3A01000, 0xE12FFF1E))
        linker = self.linker(mem, catalogue)
        self.assertEqual(linker.link(obj)[1][1], mem.read(BASE, 40))
        self.assertEqual([rewrite.offset for rewrite in linker.rewrites], list(self.SITES))

    def test_zero_size_entry_aliases_are_the_same_callee_without_changing_r1(self):
        obj, mem, catalogue = self.fixture()
        helper = obj.symbols[1]
        alias = inplace.Symbol("base_variant", 0, 0, "STT_FUNC", "STB_GLOBAL", helper.shndx)
        obj.symbols.append(alias)
        obj.sections[1].relocs[-1].symbol = alias
        linker = self.linker(mem, catalogue, model=self.ALIAS_MODEL)
        self.assertEqual(linker.link(obj)[1][1], mem.read(BASE, 40))
        self.assertEqual(linker.removed_callees[0]["symbols"], ["base_variant", "set_field18"])
        self.assertEqual((helper.size, alias.size), (8, 0))
        self.assertEqual([row.target for row in linker.rewrites], [None, None])
        for model, policy in inplace.LINKER_MODELS.items():
            if policy.function_entry_aliases:
                continue
            with self.subTest(frozen_model=model):
                with self.assertRaisesRegex(inplace.LinkError, "unresolved symbol"):
                    self.linker(mem, catalogue, model=model).link(obj)

    def test_zero_size_aliases_do_not_relax_r1_reference_or_extent_checks(self):
        for case in ("interior", "no_extent", "other_isa", "data_alias", "untyped_alias", "pointer", "unwind"):
            with self.subTest(case=case):
                obj, mem, catalogue = self.fixture()
                helper = obj.symbols[1]
                alias = inplace.Symbol("base_variant", 0, 0, "STT_FUNC", "STB_GLOBAL", helper.shndx)
                obj.symbols.append(alias)
                if case == "interior":
                    alias.value = 4
                elif case == "no_extent":
                    helper.size = 0
                elif case == "other_isa":
                    alias.value = 1
                elif case in ("data_alias", "untyped_alias"):
                    alias.kind = "STT_OBJECT" if case == "data_alias" else "STT_NOTYPE"
                else:
                    symbol = alias if case == "pointer" else inplace.Symbol("i.helper", 0, 0, "STT_SECTION", "STB_LOCAL", 2)
                    rtype = inplace.R_ARM_ABS32 if case == "pointer" else inplace.R_ARM_PREL31
                    obj.sections[3] = inplace.Section(3, ".data.pointer" if case == "pointer" else ".ARM.exidx",
                                                      bytes(4), 4, False, False, False,
                                                      [inplace.Reloc(0, rtype, symbol)], align=4)
                with self.assertRaises(inplace.LinkError):
                    self.linker(mem, catalogue, model=self.ALIAS_MODEL).link(obj)

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

    def test_bound_retained_helper_uses_v2_identity_and_inline_rules(self):
        obj, mem, catalogue = self.fixture()
        obj.symbols.append(inplace.Symbol("__tagsym$$used", 0, 0, "STT_NOTYPE", "STB_LOCAL", 2))
        catalogue[0x100100] = {"address": 0x100100, "size": 8, "isa": "arm"}
        mem.write(0x100100, obj.sections[2].data)
        for model in ("armlink5-inline-branchnop-v2", self.MODEL):
            with self.subTest(model=model):
                linker = self.linker(mem, catalogue, {"set_field18": {"address": 0x100100}}, model)
                linked = linker.link(obj)
                self.assertEqual(set(linked), {1, 2})
                self.assertEqual(linked[1][1], mem.read(BASE, 40))
                self.assertEqual(linked[2][0].origin, "catalogue")
                self.assertEqual(linked[2][1], mem.read(0x100100, 8))
                self.assertEqual(linker.removed_callees, [])
                self.assertEqual([rewrite.target for rewrite in linker.rewrites], [0x100100] * 2)
                self.assertEqual([rewrite.rule for rewrite in linker.rewrites], ["inline_insn"] * 2)

    def compiled_bound_retained_fixture(self, reference_body, reference_size):
        """F08's real game-profile objects against a wholly synthetic v2 reference image."""
        sources = {
            "caller.c": (
                "extern void set_field18(unsigned *p, unsigned v);\n"
                "extern void func_00100100(unsigned v);\n"
                "unsigned func_00100000(unsigned *p, unsigned v) {\n"
                "    set_field18(p, v); func_00100100(0); return 1;\n"
                "}\n"),
            "retained.c": (
                "__attribute__((used))\n"
                "void set_field18(unsigned *p, unsigned v) { p[6] = v; }\n"),
            "reference.c": f"void set_field18(unsigned *p, unsigned v) {{ {reference_body} }}\n",
        }
        objects = {}
        with tempfile.TemporaryDirectory() as directory:
            for name, text in sources.items():
                source = Path(directory) / name
                source.write_text(text, encoding="utf-8")
                toolchain.compile_source(toolchain.load_profiles()["game"], source, source.with_suffix(".o"))
                objects[name] = inplace.ObjectFile(source.with_suffix(".o"))
        caller = objects["caller.c"]
        definition = next(sym for sym in caller.defined_functions() if sym.name == "func_00100000")
        self.assertEqual(caller.sections[definition.shndx].size, 24)
        helper = objects["retained.c"]
        definition = next(sym for sym in helper.defined_functions() if sym.name == "set_field18")
        self.assertEqual(helper.sections[definition.shndx].size, 8)
        self.assertIn("__tagsym$$used", helper.sections[definition.shndx].retention)
        original = objects["reference.c"]
        definition = next(sym for sym in original.defined_functions() if sym.name == "set_field18")
        self.assertEqual(original.sections[definition.shndx].size, reference_size)
        helper_at = 0x100200
        names = {"set_field18": {"address": helper_at}}
        catalogue = {
            BASE: {"address": BASE, "size": 24, "isa": "arm"},
            0x100100: {"address": 0x100100, "size": 12, "isa": "arm"},
            helper_at: {"address": helper_at, "size": reference_size, "isa": "arm"},
        }
        mem = Memory()
        mem.write(helper_at, original.sections[definition.shndx].data)
        reference = inplace.ObjectFile.combine([caller, original], ["caller.c", "reference.c"])
        linker = self.linker(mem, catalogue, names, "armlink5-inline-branchnop-v2")
        for placed, data in linker.link(reference).values():
            mem.write(placed.base, data)
        candidate = inplace.ObjectFile.combine([caller, helper], ["caller.c", "retained.c"])
        return candidate, mem, catalogue, names

    @unittest.skipUnless(HAVE_COMPILER, "pinned compiler not installed (tools/setup.py)")
    def test_reviewer_bound_retained_wrong_helper_is_refused_like_v2(self):
        obj, mem, catalogue, names = self.compiled_bound_retained_fixture("p[6] = v; p[7] = v;", 12)
        # Linking precedes selection of caller-only credit: the zero-credit helper cannot bypass it.
        for model in ("armlink5-inline-branchnop-v2", self.MODEL):
            with self.subTest(model=model):
                linker = self.linker(mem, catalogue, names, model)
                with self.assertRaisesRegex(inplace.LinkError, "inlining is undecidable.*not an ordinary 4/8-byte"):
                    linker.link(obj)
                self.assertEqual(linker.removed_callees, [])
                self.assertEqual(linker.rewrites, [])

    @unittest.skipUnless(HAVE_COMPILER, "pinned compiler not installed (tools/setup.py)")
    def test_compiler_bound_retained_matching_helper_is_compared_as_its_original(self):
        obj, mem, catalogue, names = self.compiled_bound_retained_fixture("p[6] = v;", 8)
        for model in ("armlink5-inline-branchnop-v2", self.MODEL):
            with self.subTest(model=model):
                linker = self.linker(mem, catalogue, names, model)
                linked = {placed.base: (placed, data) for placed, data in linker.link(obj).values()}
                self.assertEqual(set(linked), {BASE, 0x100200})
                for address, (placed, data) in linked.items():
                    self.assertEqual(placed.origin, "catalogue")
                    self.assertEqual(data, mem.read(address, catalogue[address]["size"]))
                self.assertEqual(linker.removed_callees, [])
                self.assertEqual([rewrite.target for rewrite in linker.rewrites], [0x100200])
                self.assertEqual([rewrite.rule for rewrite in linker.rewrites], ["inline_insn"])
                self.assertNotIn("removed_callee", linker.rewrites[0].record())

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
                body = struct.pack("<I", 0xE5802018) + BX_LR if case == "missing_argument" else None
                obj, mem, catalogue = self.fixture(body)
                data = bytearray(mem.read(BASE, 40))
                if case == "live_argument":
                    struct.pack_into("<I", data, 16, 0xE2812001)
                elif case == "live_flags":
                    struct.pack_into("<I", data, 16, 0x03A01000)
                elif case == "conditional_definition":
                    struct.pack_into("<I", data, 16, 0xE3500000)
                    struct.pack_into("<I", data, 20, 0x13A01000)
                    struct.pack_into("<I", data, 24, 0xE2812001)
                elif case == "thumb":
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


class RemovedUnwindModelTest(unittest.TestCase):
    """V8 alone drops a complete, exclusive EHABI/group closure for an R1-removed section."""

    MODEL = "armlink5-inline-branchnop-v8"
    GROUP = "removed_unwind_group"

    def fixture(self, form="cant", alias=False):
        obj, mem, catalogue = RemovedCalleeModelTest().fixture(BX_LR if alias else None)
        helper = obj.symbols[1]
        if alias:
            zero = inplace.Symbol("base_variant", 0, 0, "STT_FUNC", "STB_GLOBAL", 2)
            obj.symbols.append(zero)
            obj.sections[1].relocs[-1].symbol = zero
        key = inplace.Symbol("i.helper", 0, 0, "STT_SECTION", "STB_LOCAL", 2)
        obj.symbols.append(key)
        second = 0x80B0B0B0 if form == "inline" else 1
        relocs = [inplace.Reloc(0, inplace.R_ARM_PREL31, key)]
        obj.sections[3] = inplace.Section(
            3, ".ARM.exidx.helper", struct.pack("<2I", 0, second), 8, False, False, False,
            relocs, align=4, elf_type="SHT_ARM_EXIDX", link_order=True, linked_section=2)
        if form == "extab":
            extab = inplace.Symbol(".ARM.extab.helper", 0, 0, "STT_SECTION", "STB_LOCAL", 4)
            obj.symbols.append(extab)
            obj.sections[3].data = bytes(8)
            relocs.append(inplace.Reloc(4, inplace.R_ARM_PREL31, extab))
            obj.sections[4] = inplace.Section(
                4, ".ARM.extab.helper", struct.pack("<2I", 0x80B0B0B0, 0), 8, False, False, False, align=4)
        return obj, mem, catalogue

    def linker(self, mem, catalogue, **kwargs):
        return inplace.InPlaceLinker(mem, catalogue, inplace.Resolver(catalogue),
                                     (0x200000, 0x201000), self.MODEL, **kwargs)

    def refuse(self, obj, mem, catalogue, pattern=None, **kwargs):
        linker = self.linker(mem, catalogue, **kwargs)
        with self.assertRaisesRegex(inplace.LinkError, pattern or "."):
            linker.link(obj)
        self.assertEqual(linker.rewrites, [])
        self.assertEqual(linker.removed_callees, [])

    def test_v6_refuses_and_v8_removes_only_the_owners_unwind_sections(self):
        for form in ("cant", "inline", "extab"):
            for alias in (False, True):
                with self.subTest(form=form, alias=alias):
                    obj, mem, catalogue = self.fixture(form, alias)
                    with self.assertRaises(inplace.LinkError):
                        RemovedCalleeModelTest().linker(mem, catalogue, model="armlink5-inline-branchnop-v6").link(obj)
                    linker = self.linker(mem, catalogue)
                    linked = linker.link(obj)
                    self.assertEqual(set(linked), {1})
                    self.assertEqual(linked[1][1], mem.read(BASE, 40))
                    removed = linker.removed_callees[0]
                    self.assertEqual([r["kind"] for r in removed["unwind_sections"]],
                                     ["exidx", "extab"] if form == "extab" else ["exidx"])
                    self.assertTrue(all(row.target is None and row.removed_callee == removed
                                        for row in linker.rewrites))

    def test_compact_extab_personalities_and_dependency_markers_are_bounded(self):
        for personality in (0, 1, 2):
            obj, mem, catalogue = self.fixture("extab")
            header = 0x80B0B0B0 if personality == 0 else ((0x80 + personality) << 24) | 0x0001B0B0
            obj.sections[4].data = struct.pack("<I", header) + (struct.pack("<I", 0xB0B0B0B0) if personality else b"") + bytes(4)
            obj.sections[4].size = len(obj.sections[4].data)
            routine = inplace.Symbol(f"__aeabi_unwind_cpp_pr{personality}", 0, 0, "STT_NOTYPE", "STB_GLOBAL", "SHN_UNDEF")
            obj.symbols.append(routine)
            obj.sections[4].relocs = [inplace.Reloc(0, inplace.R_ARM_NONE, routine)]
            linker = self.linker(mem, catalogue)
            self.assertEqual(set(linker.link(obj)), {1})
            self.assertEqual(len(linker.removed_callees[0]["unwind_sections"]), 2)

    def test_complete_compact_extab_counts_terminator_separately_from_unwind_words(self):
        # Sealed F02 refused complete 8/12/12-byte entries under v7. The terminator is
        # required out of line, not part of the encoded unwind-word count.
        for personality in (0, 1, 2):
            for count in ((0,) if personality == 0 else (0, 1, 3, 255)):
                with self.subTest(personality=personality, extra_unwind_words=count):
                    obj, mem, catalogue = self.fixture("extab")
                    header = 0x80B0B0B0 if personality == 0 else ((0x80 + personality) << 24) | (count << 16) | 0xB0B0
                    prefix = struct.pack("<I", header) + struct.pack("<I", 0xB0B0B0B0) * count
                    obj.sections[4].data = prefix + bytes(4)
                    obj.sections[4].size = len(prefix) + 4
                    linker = self.linker(mem, catalogue)
                    self.assertEqual(linker.link(obj)[1][1], mem.read(BASE, 40))
                    self.assertEqual(linker.removed_callees[0]["unwind_sections"][-1]["size"], 4 * (2 + count))

    def test_truncated_descriptor_lists_and_trailing_extab_data_are_refused(self):
        for personality in (0, 1, 2):
            for case in ("missing_terminator", "partial_terminator", "nonempty_descriptor",
                         "extra_zero_word", "second_entry", "too_few_unwind_words", "too_many_unwind_words",
                         "descriptor_relocation"):
                with self.subTest(personality=personality, case=case):
                    obj, mem, catalogue = self.fixture("extab")
                    count = 0 if personality == 0 else 1
                    header = 0x80B0B0B0 if personality == 0 else ((0x80 + personality) << 24) | (count << 16) | 0xB0B0
                    prefix = struct.pack("<I", header) + struct.pack("<I", 0xB0B0B0B0) * count
                    data = prefix + bytes(4)
                    if case == "missing_terminator":
                        data = prefix
                    elif case == "partial_terminator":
                        data = data[:-2]
                    elif case == "nonempty_descriptor":
                        data = prefix + struct.pack("<I", 0x00010001)
                    elif case == "extra_zero_word":
                        data += bytes(4)
                    elif case == "second_entry":
                        data += struct.pack("<2I", 0x80B0B0B0, 0)
                    elif case == "too_few_unwind_words":
                        data = data[:4] + data[8:] if count else bytes(4)
                    elif case == "too_many_unwind_words":
                        data = prefix + struct.pack("<I", 0xB0B0B0B0) + bytes(4)
                    else:
                        target = inplace.Symbol("landing_pad", 0, 0, "STT_NOTYPE", "STB_GLOBAL", "SHN_UNDEF")
                        obj.symbols.append(target)
                        obj.sections[4].relocs.append(inplace.Reloc(len(prefix), inplace.R_ARM_PREL31, target))
                    obj.sections[4].data, obj.sections[4].size = data, len(data)
                    self.refuse(obj, mem, catalogue)

    def group_fixture(self, form="cant", alias=False):
        obj, mem, catalogue = self.fixture(form, alias)
        members = {2, 3, *([4] if form == "extab" else [])}
        for index in members:
            obj.sections[index].comdat = self.GROUP
        obj.section_groups = [(1, self.GROUP, members)]
        return obj, mem, catalogue

    def test_v7_is_retired_instead_of_changing_recorded_rules(self):
        self.assertNotIn("armlink5-inline-branchnop-v7", inplace.LINKER_MODELS)
        with self.assertRaisesRegex(inplace.LinkError, "retired.*COMDAT.*terminator"):
            inplace.linker_model("armlink5-inline-branchnop-v7")

    def test_complete_exclusive_groups_are_removable(self):
        for form in ("cant", "inline", "extab"):
            for alias in (False, True):
                with self.subTest(form=form, alias=alias):
                    obj, mem, catalogue = self.group_fixture(form, alias)
                    linker = self.linker(mem, catalogue)
                    self.assertEqual(linker.link(obj)[1][1], mem.read(BASE, 40))
                    self.assertEqual(set(linker._removed_unwind), {3, *([4] if form == "extab" else [])})

    def test_other_live_or_unmodeled_group_members_prevent_elimination(self):
        # Sealed F01 accepted the kept-caller case under v7; no reference exemption may
        # split that live group, including members that ordinary placement would ignore.
        for case in ("caller", "kept_function", "bound_data", "unplaced_data", "unplaced_bss", "unmodeled_code"):
            with self.subTest(case=case):
                obj, mem, catalogue = self.group_fixture()
                if case == "caller":
                    obj.sections[1].comdat = self.GROUP
                    obj.section_groups[0][2].add(1)
                else:
                    executable = case in ("kept_function", "unmodeled_code")
                    data = BX_LR if executable else bytes(4)
                    if case == "unmodeled_code":
                        data *= 3
                    size, nobits = len(data), case == "unplaced_bss"
                    obj.sections[5] = inplace.Section(5, "i.other" if executable else ".data.other",
                        b"" if nobits else data, size, nobits, executable, nobits, align=4, comdat=self.GROUP)
                    obj.section_groups[0][2].add(5)
                    if case == "kept_function":
                        obj.symbols.append(inplace.Symbol("func_00100100", 0, 4, "STT_FUNC", "STB_GLOBAL", 5))
                        catalogue[BASE + 0x100] = {"address": BASE + 0x100, "size": 4, "isa": "arm"}
                        mem.write(BASE + 0x100, BX_LR)
                    elif case == "unmodeled_code":
                        obj.symbols.append(inplace.Symbol("long_helper", 0, size, "STT_FUNC", "STB_GLOBAL", 5))
                    elif case == "bound_data":
                        obj.symbols.append(inplace.Symbol("data_00180000", 0, 4, "STT_OBJECT", "STB_GLOBAL", 5))
                self.refuse(obj, mem, catalogue, "group-removal proof")

    def test_every_selected_group_code_member_still_needs_an_inlined_call(self):
        obj, mem, catalogue = self.group_fixture()
        obj.sections[5] = inplace.Section(5, "i.uncalled", BX_LR, 4, False, True, False,
                                         mapping=[(0, "$a")], align=4, comdat=self.GROUP)
        obj.symbols.append(inplace.Symbol("uncalled_helper", 0, 4, "STT_FUNC", "STB_GLOBAL", 5))
        obj.section_groups[0][2].add(5)
        self.refuse(obj, mem, catalogue, "no inlined call")

    def test_multiple_independently_used_helpers_and_companions_can_close_a_group(self):
        obj, mem, catalogue = self.group_fixture(alias=True)
        helper = inplace.Symbol("second_empty", 0, 4, "STT_FUNC", "STB_GLOBAL", 5)
        key = inplace.Symbol("i.second_empty", 0, 0, "STT_SECTION", "STB_LOCAL", 5)
        obj.symbols.extend((helper, key))
        obj.sections[5] = inplace.Section(5, "i.second_empty", BX_LR, 4, False, True, False,
                                         mapping=[(0, "$a")], align=4, comdat=self.GROUP)
        obj.sections[6] = inplace.Section(6, ".ARM.exidx.second", struct.pack("<2I", 0, 1),
            8, False, False, False, [inplace.Reloc(0, inplace.R_ARM_PREL31, key)], align=4,
            comdat=self.GROUP, elf_type="SHT_ARM_EXIDX", link_order=True, linked_section=5)
        obj.sections[1].relocs[-1].symbol = helper
        obj.section_groups[0][2].update((5, 6))
        linker = self.linker(mem, catalogue)
        self.assertEqual(linker.link(obj)[1][1], mem.read(BASE, 40))
        self.assertEqual(len(linker.removed_callees), 2)
        self.assertEqual(set(linker._removed_unwind), {3, 6})

    def test_all_comdat_copies_require_group_closure_in_either_object_order(self):
        for kept_member in (False, True):
            for reverse in (False, True):
                with self.subTest(kept_member=kept_member, reverse=reverse):
                    first, mem, catalogue = self.group_fixture(alias=True)
                    second, _, _ = self.group_fixture(alias=True)
                    second.sections.pop(1)
                    second.symbols = [sym for sym in second.symbols if sym.shndx != 1]
                    if kept_member:
                        second.sections[5] = inplace.Section(5, "i.kept_other", BX_LR, 4,
                            False, True, False, mapping=[(0, "$a")], align=4, comdat=self.GROUP)
                        second.symbols.append(inplace.Symbol("func_00100100", 0, 4, "STT_FUNC", "STB_GLOBAL", 5))
                        second.section_groups[0][2].add(5)
                        catalogue[BASE + 0x100] = {"address": BASE + 0x100, "size": 4, "isa": "arm"}
                        mem.write(BASE + 0x100, BX_LR)
                    objects = [second, first] if reverse else [first, second]
                    obj = inplace.ObjectFile.combine(objects)
                    if kept_member:
                        self.refuse(obj, mem, catalogue, "group-removal proof")
                    else:
                        linker = self.linker(mem, catalogue)
                        linked = linker.link(obj)
                        self.assertEqual(next(iter(linked.values()))[1], mem.read(BASE, 40))
                        self.assertEqual(len(linker.removed_callees), 2)
                        self.assertEqual(len(linker._removed_unwind), 2)

    def test_unrepresented_and_unsupported_allocated_group_members_fail_closed(self):
        for flags in (0, 1, 3):
            with self.subTest(flags=flags):
                obj, mem, catalogue = self.group_fixture()
                if flags == 1:
                    obj.section_groups[0][2].add(99)  # zero-size allocated member omitted from placement
                else:
                    obj.section_groups[0] = (flags, self.GROUP if flags & 1 else "", {2, 3})
                    if not flags:
                        obj.sections[2].comdat = obj.sections[3].comdat = None
                self.refuse(obj, mem, catalogue, "group-removal proof|unsupported section group")

    def test_combining_preserves_unrepresented_group_members(self):
        for reverse in (False, True):
            with self.subTest(reverse=reverse):
                first, mem, catalogue = self.group_fixture(alias=True)
                second, _, _ = self.group_fixture(alias=True)
                second.sections.pop(1)
                second.symbols = [sym for sym in second.symbols if sym.shndx != 1]
                second.section_groups[0][2].add(99)
                obj = inplace.ObjectFile.combine([second, first] if reverse else [first, second])
                self.assertTrue(any(isinstance(index, str) for _, _, members in obj.section_groups for index in members))
                self.refuse(obj, mem, catalogue, "unrepresented allocated member.*group-removal proof")

    @unittest.skipUnless(HAVE_COMPILER, "pinned compiler not installed (tools/setup.py)")
    def test_real_elf_groups_preserve_zero_size_allocated_members(self):
        from io import BytesIO

        with tempfile.TemporaryDirectory() as tmp:
            source = Path(tmp) / "group.cpp"
            source.write_text("template<class T> void group_release(T *) {}\n"
                              "template void group_release<int>(int *);\n", encoding="utf-8")
            path = source.with_suffix(".o")
            toolchain.compile_source(GAME_PROFILE, source, path)
            original = path.read_bytes()
            helper = inplace.ObjectFile(path)
            full = next(sym for sym in helper.defined_functions() if sym.size)
            self.assertTrue(helper.sections[full.shndx].comdat)
            self.assertTrue(any(full.shndx in members for _, _, members in helper.section_groups))

            # Append a zero-sized allocated member to the actual compiler group. Only ELF
            # metadata changes; no code, unwind data or relocation is invented or patched.
            elf = inplace.ELFFile(BytesIO(original))
            group_index, group = next(
                (index, section) for index, section in enumerate(elf.iter_sections())
                if section["sh_type"] == "SHT_GROUP"
                and full.shndx in struct.unpack(f"<{section['sh_size'] // 4}I", section.data())[1:])
            count, stride, old_headers = elf.num_sections(), elf["e_shentsize"], elf["e_shoff"]
            self.assertEqual(stride, 40)  # pinned compiler emits ELF32 section headers
            headers = bytearray(original[old_headers:old_headers + count * stride])
            modified = bytearray(original)
            modified.extend(bytes(-len(modified) % 4))
            group_offset = len(modified)
            group_data = group.data() + struct.pack("<I", count)
            modified.extend(group_data)
            modified.extend(bytes(-len(modified) % 4))
            header_offset = len(modified)
            struct.pack_into("<I", headers, group_index * stride + 16, group_offset)
            struct.pack_into("<I", headers, group_index * stride + 20, len(group_data))
            modified.extend(headers)
            modified.extend(struct.pack("<10I", 0, 1, 0x202, 0, 0, 0, 0, 0, 4, 0))
            struct.pack_into("<I", modified, 32, header_offset)
            struct.pack_into("<H", modified, 48, count + 1)
            with_zero = inplace.ObjectFile(path, data=bytes(modified))
            self.assertNotIn(count, with_zero.sections)
            self.assertTrue(any(count in members for _, _, members in with_zero.section_groups))

            caller, mem, catalogue = RemovedCalleeModelTest().fixture(BX_LR)
            caller.sections.pop(2)
            caller.symbols = caller.symbols[:1]
            external = inplace.Symbol(full.name, 0, 0, "STT_FUNC", "STB_GLOBAL", "SHN_UNDEF")
            caller.symbols.append(external)
            for rel in caller.sections[1].relocs:
                rel.symbol = external
            linked = self.linker(mem, catalogue).link(inplace.ObjectFile.combine([caller, helper]))
            self.assertEqual(next(iter(linked.values()))[1], mem.read(BASE, 40))
            for objects in ([caller, with_zero], [with_zero, caller]):
                self.refuse(inplace.ObjectFile.combine(objects), mem, catalogue, "unrepresented allocated member.*group-removal proof")

    def test_incorrect_or_combined_index_metadata_is_refused(self):
        changes = (
            ("elf_type", "SHT_PROGBITS"), ("link_order", False), ("linked_section", 1),
            ("linked_section", 999), ("writable", True), ("executable", True),
            ("nobits", True), ("align", 1), ("entsize", 4), ("comdat", "other"),
            ("name", ".data.imposter"), ("size", 16), ("data", bytes(16)),
        )
        for field, value in changes:
            with self.subTest(field=field, value=value):
                obj, mem, catalogue = self.fixture()
                setattr(obj.sections[3], field, value)
                self.refuse(obj, mem, catalogue)

    def test_keys_for_kept_or_interior_functions_are_not_removed(self):
        for case in ("kept", "interior", "high_bit", "duplicate", "wrong_kind"):
            with self.subTest(case=case):
                obj, mem, catalogue = self.fixture()
                index = obj.sections[3]
                if case == "kept":
                    index.relocs[0].symbol = obj.symbols[0]
                elif case == "duplicate":
                    index.relocs.append(inplace.Reloc(0, inplace.R_ARM_PREL31, obj.symbols[1]))
                elif case == "wrong_kind":
                    index.relocs[0].type = inplace.R_ARM_ABS32
                else:
                    index.data = struct.pack("<2I", 4 if case == "interior" else 0x80000000, 1)
                self.refuse(obj, mem, catalogue)

    def test_any_other_incoming_reference_to_helper_or_companions_is_refused(self):
        for target in (2, 3, 4):
            for rtype in (inplace.R_ARM_ABS32, inplace.R_ARM_PREL31, inplace.R_ARM_NONE, 255):
                with self.subTest(target=target, rtype=rtype):
                    obj, mem, catalogue = self.fixture("extab")
                    symbol = inplace.Symbol("dependency", 0, 0, "STT_SECTION", "STB_LOCAL", target)
                    obj.symbols.append(symbol)
                    obj.sections[5] = inplace.Section(5, ".data.unplaced", bytes(4), 4, False, False, False,
                                                      [inplace.Reloc(0, rtype, symbol)], align=4)
                    self.refuse(obj, mem, catalogue)

    def test_unknown_outgoing_metadata_relocations_are_not_erased(self):
        for target in (3, 4):
            for rtype in (inplace.R_ARM_ABS32, inplace.R_ARM_PREL31, inplace.R_ARM_V4BX, 255):
                with self.subTest(target=target, rtype=rtype):
                    obj, mem, catalogue = self.fixture("extab")
                    unknown = inplace.Symbol("unknown", 0, 0, "STT_NOTYPE", "STB_GLOBAL", "SHN_UNDEF")
                    obj.symbols.append(unknown)
                    obj.sections[target].relocs.append(inplace.Reloc(0, rtype, unknown))
                    self.refuse(obj, mem, catalogue)

    def test_duplicate_indexes_and_shared_or_combined_extab_are_refused(self):
        for case in ("duplicate_index", "shared_extab", "interior_extab", "combined_extab", "generic_extab",
                     "extra_dependency", "wrong_personality", "unsupported_inline"):
            with self.subTest(case=case):
                obj, mem, catalogue = self.fixture("extab")
                if case in ("duplicate_index", "shared_extab"):
                    import copy
                    duplicate = copy.copy(obj.sections[3])
                    duplicate.index, duplicate.relocs = 5, list(duplicate.relocs)
                    if case == "shared_extab":
                        duplicate.linked_section = 1
                        duplicate.relocs[0] = inplace.Reloc(0, inplace.R_ARM_PREL31, obj.symbols[0])
                    obj.sections[5] = duplicate
                elif case == "interior_extab":
                    obj.sections[3].data = struct.pack("<2I", 0, 4)
                elif case == "combined_extab":
                    obj.sections[4].data *= 2
                    obj.sections[4].size = len(obj.sections[4].data)
                elif case == "generic_extab":
                    obj.sections[4].data = bytes(8)
                elif case in ("extra_dependency", "wrong_personality"):
                    name = "__aeabi_unwind_cpp_pr1" if case == "wrong_personality" else "other_root"
                    dep = inplace.Symbol(name, 0, 0, "STT_NOTYPE", "STB_GLOBAL", "SHN_UNDEF")
                    obj.symbols.append(dep)
                    obj.sections[4].relocs.append(inplace.Reloc(0, inplace.R_ARM_NONE, dep))
                else:
                    obj.sections[3].data = struct.pack("<2I", 0, 0x83000000)
                    obj.sections[3].relocs.pop()
                self.refuse(obj, mem, catalogue)

    def test_roots_and_original_bound_unwind_entries_cannot_disappear(self):
        for index in (3, 4):
            for case in ("retained", "named", "original_bound", "link_dependency"):
                with self.subTest(index=index, case=case):
                    obj, mem, catalogue = self.fixture("extab")
                    if case == "retained":
                        self.refuse(obj, mem, catalogue, retention_roots={obj.sections[index].name: "KEEP"})
                        continue
                    if case == "link_dependency":
                        obj.sections[5] = inplace.Section(5, ".linked", bytes(4), 4, False, False, False,
                                                          align=4, link_order=True, linked_section=index)
                    else:
                        name = "data_00180000" if case == "original_bound" else "named_unwind"
                        obj.symbols.append(inplace.Symbol(name, 0, 4, "STT_OBJECT", "STB_GLOBAL", index))
                    self.refuse(obj, mem, catalogue)

    def test_a_kept_functions_separate_index_is_never_eliminated(self):
        obj, mem, catalogue = self.fixture()
        obj.sections[4] = inplace.Section(
            4, ".ARM.exidx.caller", struct.pack("<2I", 0, 1), 8, False, False, False,
            [inplace.Reloc(0, inplace.R_ARM_PREL31, obj.symbols[0])], align=4,
            elf_type="SHT_ARM_EXIDX", link_order=True, linked_section=1)
        linker = self.linker(mem, catalogue)
        self.assertEqual(set(linker.link(obj)), {1})
        self.assertEqual(set(linker._removed_unwind), {3})
        self.assertEqual([r["section_index"] for r in linker.removed_callees[0]["unwind_sections"]], [3])

    def test_v8_inherits_v6_independently_bound_zero_size_placement(self):
        # V5 regressed this v4 placement: independent bindings do not borrow a sibling's
        # identity and therefore do not require the full-section alias proof.
        for model in ("armlink5-inline-branchnop-v4", "armlink5-inline-branchnop-v6", self.MODEL):
            for binding in ("resolver", "explicit"):
                with self.subTest(model=model, binding=binding):
                    obj, mem, catalogue = RemovedCalleeModelTest().fixture()
                    obj.symbols[0].size = 4  # no full-section alias witness in this section
                    bind = "STB_GLOBAL" if binding == "resolver" else "STB_LOCAL"
                    obj.symbols.append(inplace.Symbol("bound_entry", 0, 0, "STT_FUNC", bind, 1))
                    resolver_names = {"bound_entry": {"address": BASE}} if binding == "resolver" else {}
                    explicit_names = {"bound_entry": BASE} if binding == "explicit" else {}
                    linker = inplace.InPlaceLinker(mem, catalogue, inplace.Resolver(catalogue, resolver_names),
                                                   (0x200000, 0x201000), model)
                    self.assertEqual(linker.link(obj, explicit_names)[1][1], mem.read(BASE, 40))
                    self.assertNotIn("unwind_sections", linker.removed_callees[0])

    @unittest.skipUnless(HAVE_COMPILER, "pinned compiler not installed (tools/setup.py)")
    def test_real_d1_d2_elf_metadata_survives_combining_in_either_object_order(self):
        with tempfile.TemporaryDirectory() as tmp:
            source = Path(tmp) / "lifetime.cpp"
            source.write_text("class Lifetime { public: ~Lifetime(); }; Lifetime::~Lifetime() {}\n", encoding="utf-8")
            path = source.with_suffix(".o")
            toolchain.compile_source(GAME_PROFILE, source, path)
            helper = inplace.ObjectFile(path)
            full = next(sym for sym in helper.defined_functions() if sym.size)
            aliases = inplace.function_aliases(helper)[full.shndx]
            self.assertEqual(sorted(sym.size for sym in aliases), [0, 4])
            indexes = [sec for sec in helper.sections.values() if sec.elf_type == "SHT_ARM_EXIDX"]
            self.assertEqual(len(indexes), 1)
            self.assertTrue(indexes[0].link_order)
            self.assertEqual(indexes[0].linked_section, full.shndx)
            for reverse in (False, True):
                caller, mem, catalogue = RemovedCalleeModelTest().fixture(BX_LR)
                caller.sections.pop(2)
                caller.symbols = caller.symbols[:1]
                external = inplace.Symbol(full.name, 0, 0, "STT_FUNC", "STB_GLOBAL", "SHN_UNDEF")
                caller.symbols.append(external)
                for rel in caller.sections[1].relocs:
                    rel.symbol = external
                objects, labels = [caller, helper], ["caller.cpp", "lifetime.cpp"]
                if reverse:
                    objects.reverse()
                    labels.reverse()
                combined = inplace.ObjectFile.combine(objects, labels)
                for sec in combined.sections.values():
                    if sec.elf_type == "SHT_ARM_EXIDX":
                        self.assertEqual(sec.linked_section, sec.relocs[0].symbol.shndx)
                with self.assertRaisesRegex(inplace.LinkError, "still referenced"):
                    RemovedCalleeModelTest().linker(mem, catalogue, model="armlink5-inline-branchnop-v6").link(combined)
                linker = self.linker(mem, catalogue)
                self.assertEqual(next(iter(linker.link(combined).values()))[1], mem.read(BASE, 40))
                self.assertEqual(linker.removed_callees[0]["object"], "lifetime.cpp")
                self.assertEqual(linker.removed_callees[0]["unwind_sections"][0]["object"], "lifetime.cpp")


@unittest.skipUnless(HAVE_COMPILER, "pinned compiler not installed (tools/setup.py)")
class SwitchRemovedCalleeTest(unittest.TestCase):
    """R1 on real armcc switches: a bounded jump table no longer hides a removed setter's call
    context, and an unbounded, escaping or computed transfer is still refused.

    The synthetic original is the compiled caller with each relocation applied independently:
    the setter calls hold the inlined `str r1, [r0, #0x18]`, table words hold absolute addresses."""

    MODEL = "armlink5-inline-branchnop-v4"
    SETTER = 0xE5801018  # str r1, [r0, #0x18]
    HELPER = "void set_field18(unsigned *p, unsigned v) { p[6] = v; }\n"
    CALLER = (
        "extern void set_field18(unsigned *p, unsigned v);\n"
        "void func_00100000(unsigned *p, unsigned v, int x) {\n"
        "    switch (x) {\n"
        "    case 0: set_field18(p, v); p[0] = 9; break;\n"
        "    case 1: p[1] = 1; break;\n"
        "    case 2: p[2] = 2; break;\n"
        "    case 3: set_field18(p, v); p[3] = 3; break;\n"
        "    case 4: p[4] = 4; break;\n"
        "    default: break;\n"
        "    }\n"
        "}\n"
    )
    FUNCTION_POINTER = (
        "extern void set_field18(unsigned *p, unsigned v);\n"
        "void func_00100000(unsigned *p, unsigned v, void (*fn)(unsigned *)) {\n"
        "    set_field18(p, v); p[1] = 2; fn(p);\n"
        "}\n"
    )

    def fixture(self, caller=CALLER, profile="game"):
        with tempfile.TemporaryDirectory() as directory:
            objects = []
            for name, text in (("caller.c", caller), ("helper.c", self.HELPER)):
                source = Path(directory) / name
                source.write_text(text, encoding="utf-8")
                toolchain.compile_source(toolchain.load_profiles()[profile], source, source.with_suffix(".o"))
                objects.append(inplace.ObjectFile(source.with_suffix(".o")))
        definition = next(sym for sym in objects[0].defined_functions() if sym.name == "func_00100000")
        section = objects[0].sections[definition.shndx]
        original = bytearray(section.data)
        for rel in section.relocs:
            if rel.symbol.name == "set_field18" and rel.type == inplace.R_ARM_CALL:
                struct.pack_into("<I", original, rel.offset, self.SETTER)
            elif rel.symbol.name == "__switch$$" and rel.type == inplace.R_ARM_ABS32:
                addend = struct.unpack_from("<I", section.data, rel.offset)[0]
                struct.pack_into("<I", original, rel.offset, BASE + rel.symbol.value + addend)
            else:
                self.fail(f"unexpected relocation {rel.type} against {rel.symbol.name}")
        spans = [(offset, end) for (offset, kind), end in zip(section.mapping, [o for o, _ in section.mapping[1:]] + [section.size])]
        catalogue = {BASE: {
            "address": BASE, "size": section.size, "isa": "arm",
            "code_ranges": [[BASE + lo, BASE + hi] for (lo, hi), (_, kind) in zip(spans, section.mapping) if kind == "$a"],
            "pools": [[BASE + lo, BASE + hi] for (lo, hi), (_, kind) in zip(spans, section.mapping) if kind == "$d"],
        }}
        mem = Memory()
        mem.write(BASE, bytes(original))
        combined = inplace.ObjectFile.combine(objects, ["caller.c", "helper.c"])
        sites = [rel.offset for rel in section.relocs if rel.symbol.name == "set_field18"]
        return SimpleNamespace(object=combined, memory=mem, catalogue=catalogue, original=bytes(original), sites=sites)

    def locate(self, fixture):
        """Offsets of the jump-table writer (and its form), of the guarding compare before it and of
        the first table entry, found from the raw encodings."""
        words = struct.unpack(f"<{len(fixture.original) // 4}I", fixture.original)
        writer = next(4 * i for i, word in enumerate(words)
                      if word & 0x0FFFFFF0 in (0x079FF100, 0x008FF100) and word >> 28 in (0x3, 0x9))
        form = "words" if words[writer // 4] & 0x0FFFFFF0 == 0x079FF100 else "branches"
        compare = max(4 * i for i, word in enumerate(words) if word & 0x0FF0F000 == 0x03500000 and 4 * i < writer)
        return SimpleNamespace(writer=writer, form=form, compare=compare, table=writer + 8)

    def link(self, fixture, model=None):
        linker = inplace.InPlaceLinker(fixture.memory, fixture.catalogue, inplace.Resolver(fixture.catalogue),
                                       (0x200000, 0x201000), model or self.MODEL)
        return linker, linker.link(fixture.object)

    def patch(self, fixture, offset, word):
        fixture.memory.write(BASE + offset, struct.pack("<I", word))

    def assert_refused(self, fixture):
        linker = inplace.InPlaceLinker(fixture.memory, fixture.catalogue, inplace.Resolver(fixture.catalogue),
                                       (0x200000, 0x201000), self.MODEL)
        with self.assertRaisesRegex(inplace.LinkError, "removed callee i.set_field18 is still referenced"):
            linker.link(fixture.object)
        self.assertEqual(linker.removed_callees, [])

    def assert_resolved(self, profile, form):
        fixture = self.fixture(profile=profile)
        self.assertEqual(self.locate(fixture).form, form)
        self.assertEqual(len(fixture.sites), 2)
        linker, linked = self.link(fixture)
        self.assertEqual({placed.base: data for placed, data in linked.values()}, {BASE: fixture.original})
        self.assertEqual([(rewrite.offset, rewrite.rule, rewrite.word) for rewrite in linker.rewrites],
                         [(site, "inline_insn", self.SETTER) for site in fixture.sites])
        self.assertEqual([callee["symbols"] for callee in linker.removed_callees], [["set_field18"]])
        self.assertEqual([rewrite.target for rewrite in linker.rewrites], [None, None])
        self.assertEqual(linker.removed_callees[0]["object"], "helper.c")
        with self.assertRaisesRegex(inplace.LinkError, "unresolved symbol"):
            self.link(fixture, "armlink5-inline-branchnop-v2")
        with self.assertRaisesRegex(inplace.LinkError, "still referenced"):
            self.link(fixture, "armlink5-inline-branchnop-v3")

    def test_a_word_table_switch_no_longer_hides_the_removed_setters_context(self):
        self.assert_resolved("game", "words")  # cmp r2, #5; ldrlo pc, [pc, r2, lsl #2]; b; five words

    def test_a_branch_table_switch_no_longer_hides_the_removed_setters_context(self):
        self.assert_resolved("trial-504-82-O3-Ospace", "branches")  # addlo pc, pc, r2, lsl #2; b; five B

    def test_a_table_entry_outside_the_function_is_refused(self):
        for case, entry in (("beyond the extent", 0x1000), ("before the function", -0x100), ("Thumb entry", 0x21)):
            with self.subTest(case):
                fixture = self.fixture()
                located = self.locate(fixture)
                self.patch(fixture, located.table + 8, (BASE + entry) & 0xFFFFFFFF)
                self.assert_refused(fixture)
        fixture = self.fixture(profile="trial-504-82-O3-Ospace")
        located = self.locate(fixture)
        place = BASE + located.table + 8
        self.patch(fixture, located.table + 8, 0xEA000000 | (((BASE + 0x1000 - place - 8) >> 2) & 0xFFFFFF))
        self.assert_refused(fixture)

    def test_a_code_range_outside_the_callers_extent_cannot_supply_a_table_target(self):
        fixture = self.fixture()
        located = self.locate(fixture)
        outside = BASE + 0x1000
        fixture.catalogue[BASE]["code_ranges"].append([outside, outside + 4])
        fixture.memory.write(outside, struct.pack("<I", 0xE12FFF1E))
        self.patch(fixture, located.table + 8, outside)
        self.assert_refused(fixture)

    def test_the_default_edge_cannot_escape_the_function(self):
        for profile in ("game", "trial-504-82-O3-Ospace"):
            with self.subTest(profile=profile):
                fixture = self.fixture(profile=profile)
                offset = self.locate(fixture).writer + 4
                self.patch(fixture, offset, 0xEA000000 | (((0x1000 - offset - 8) >> 2) & 0xFFFFFF))
                self.assert_refused(fixture)

    def test_a_table_edge_cannot_bypass_its_guard(self):
        for profile in ("game", "trial-504-82-O3-Ospace"):
            with self.subTest(profile=profile):
                fixture = self.fixture(profile=profile)
                located = self.locate(fixture)
                entry = located.table + 8
                target = BASE + located.writer
                word = (target if located.form == "words"
                        else 0xEA000000 | (((target - (BASE + entry) - 8) >> 2) & 0xFFFFFF))
                self.patch(fixture, entry, word)
                self.assert_refused(fixture)

    def test_a_bound_that_the_table_does_not_cover_is_refused(self):
        # cmp r2, #6 would read a sixth word, which is the first instruction after the pool; likewise
        # a sixth branch-table entry would be a non-branch instruction.
        for profile in ("game", "trial-504-82-O3-Ospace"):
            with self.subTest(profile):
                fixture = self.fixture(profile=profile)
                located = self.locate(fixture)
                self.patch(fixture, located.compare, 0xE3520006)
                self.assert_refused(fixture)

    def test_a_switch_without_its_bounding_compare_is_refused(self):
        for profile in ("game", "trial-504-82-O3-Ospace"):
            for case, word in (("no compare", 0xE320F000), ("other register", 0xE3530005), ("register compare", 0xE1520003)):
                with self.subTest(profile=profile, case=case):
                    fixture = self.fixture(profile=profile)
                    self.patch(fixture, self.locate(fixture).compare, word)
                    self.assert_refused(fixture)

    def test_flags_changed_between_the_compare_and_the_writer_are_refused(self):
        for profile in ("game", "trial-504-82-O3-Ospace"):
            with self.subTest(profile):
                fixture = self.fixture(profile=profile)
                located = self.locate(fixture)
                self.assertEqual(located.writer - located.compare, 8)  # `mov r4, r0` sits between them
                self.patch(fixture, located.compare + 4, 0xE2504000)  # subs r4, r0, #0
                self.assert_refused(fixture)

    def test_the_index_changed_after_the_compare_is_refused(self):
        for profile in ("game", "trial-504-82-O3-Ospace"):
            with self.subTest(profile=profile):
                fixture = self.fixture(profile=profile)
                located = self.locate(fixture)
                self.assertEqual(located.writer - located.compare, 8)
                self.patch(fixture, located.compare + 4, 0xE1A02000)  # mov r2, r0
                self.assert_refused(fixture)

    def test_a_computed_branch_after_the_site_is_still_refused(self):
        fixture = self.fixture(self.FUNCTION_POINTER)  # `bx ip` through the passed function pointer
        self.assertEqual(len(fixture.sites), 1)
        self.assert_refused(fixture)


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
