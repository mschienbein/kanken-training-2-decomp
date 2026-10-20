"""Build composition and receipts for synthetic images, removed helpers and literal records."""
import contextlib
import io
import json
import struct
import sys
import tempfile
import unittest
from contextlib import ExitStack
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import build  # noqa: E402
import ctrfmt  # noqa: E402
import image  # noqa: E402
import inplace  # noqa: E402
import toolchain  # noqa: E402
from tests import test_inplace as inplace_tests  # noqa: E402
from tests.test_literal_pools import MODEL, POOL, fixture  # noqa: E402

BASE = inplace_tests.BASE


class BuildRemovedCalleeModelTest(unittest.TestCase):
    def synthetic_build(self, model="armlink5-inline-branchnop-v3", retained=False, reverse=False,
                        comdat_alias=None, credit_helper=False, unreferenced=False):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        root = Path(temporary.name)
        fixture = inplace_tests.RemovedCalleeModelTest()
        names = {}
        if unreferenced:
            if unreferenced == "unwind":
                caller, memory, functions, names = inplace_tests.UnreferencedDestructorUnwindModelTest().fixture("extab")
            else:
                caller, memory, functions, names = inplace_tests.UnreferencedDestructorModelTest().fixture()
            helper = inplace.ObjectFile.__new__(inplace.ObjectFile)
            helper.sections, helper.symbols = {}, []
        elif comdat_alias is not None:
            (caller, helper), memory, functions, names = fixture.comdat_alias_fixture(
                contradictory=comdat_alias == "contradictory",
                original_size=12 if comdat_alias == "wrong_original" else 8)
            if comdat_alias == "duplicate_owner":
                caller.symbols.append(inplace.Symbol("bound_variant", 0, 0, "STT_FUNC", "STB_GLOBAL", 2))
            elif comdat_alias == "unbound_alias":
                names = {}
                functions.pop(BASE + 0x100)
        else:
            caller, memory, functions = fixture.fixture()
            helper = inplace.ObjectFile.__new__(inplace.ObjectFile)
            definition = caller.symbols.pop()
            helper.sections, helper.symbols = {2: caller.sections.pop(2)}, [definition]
            external = inplace.Symbol(definition.name, 0, 0, "STT_NOTYPE", "STB_GLOBAL", "SHN_UNDEF")
            caller.symbols.append(external)
            for relocation in caller.sections[1].relocs:
                relocation.symbol = external
        if retained:
            helper.sections[2].retention = ("__tagsym$$used",)
        pairs = [
            ({"source": "src/caller.c", "profile": "game", "functions": ["func_00100000"]}, caller),
            ({"source": "src/helper.c", "profile": "game",
              "functions": ["bound_variant"] if credit_helper else []}, helper),
        ]
        if reverse:
            pairs.reverse()
        units = [unit for unit, _ in pairs]
        objects = {Path(unit["source"]).stem: obj for unit, obj in pairs}
        for unit in units:
            path = root / unit["source"]
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text("/* Independent synthetic build input identity. */\n", encoding="utf-8")
        for name, contents in {
            "config/matches.json": json.dumps({"units": units}).encode(),
            "config/catalogue.json": b"{}",
            "config/names.json": b"{}",
            "config/compiler_profiles.json": b"{}",
            "toolchain.lock.json": b"{}",
            "tools/linker_models.py": Path(inplace.__file__).with_name("linker_models.py").read_bytes(),
            "orig/exheader.bin": b"synthetic-header",
            "orig/exefs/code.blz": b"synthetic-compression",
            "compiler.exe": b"synthetic-compiler",
        }.items():
            path = root / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(contents)
        memory.code = memory.read(BASE, max(address + fn["size"] for address, fn in functions.items()) - BASE)
        memory.sha256 = build.sha256_bytes(memory.code)
        memory.bss = (0x200000, 0x201000)
        memory.layout = SimpleNamespace(file_offset=lambda address: address - BASE)
        catalogue = {"image_sha256": memory.sha256, "functions": list(functions.values())}
        original_combine = inplace.ObjectFile.combine

        def compile_units(units, profiles, out, fresh, jobs):
            for unit in units:
                path = out / "obj" / Path(unit["source"]).with_suffix(".o")
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(("synthetic-object:" + unit["source"]).encode())
            return {unit["source"]: False for unit in units}

        def object_file(path):
            return objects[path.stem]

        object_file.combine = original_combine
        with ExitStack() as stack:
            for target, attribute, value in (
                (build, "ROOT", root), (build, "MATCHES", root / "config/matches.json"),
                (build, "HARNESS", ("tools/linker_models.py",)), (build.inplace, "ObjectFile", object_file),
                (build.toolchain, "PROFILES", root / "config/compiler_profiles.json"),
                (build.image_mod, "CATALOGUE", root / "config/catalogue.json"),
                (build.image_mod, "NAMES", root / "config/names.json"),
            ):
                stack.enter_context(patch.object(target, attribute, value))
            stack.enter_context(patch.object(build.denominator, "verify", return_value=None))
            stack.enter_context(patch.object(build.image_mod.Image, "load", return_value=memory))
            stack.enter_context(patch.object(build.image_mod, "load_catalogue", return_value=catalogue))
            stack.enter_context(patch.object(build.image_mod, "load_names", return_value=names))
            stack.enter_context(patch.object(build, "compile_units", side_effect=compile_units))
            stack.enter_context(patch.object(build.toolchain, "load_profiles", return_value={
                "game": toolchain.Profile("game", "synthetic", ())}))
            stack.enter_context(patch.object(build.toolchain, "armcc_exe", return_value=root / "compiler.exe"))
            stack.enter_context(patch.object(build.ctrfmt, "blz_compress", return_value=b"synthetic-compression"))
            return build.build("synthetic-removed", root / "config/matches.json", linker_model=model), root

    def test_zero_credit_helper_is_hashed_and_reported_with_every_dependent_rewrite(self):
        for reverse in (False, True):
            with self.subTest(reverse=reverse):
                receipt, root = self.synthetic_build(reverse=reverse)
                self.assertEqual(receipt["result"], "PASS")
                self.assertEqual(receipt["inputs"]["linker_model"], "armlink5-inline-branchnop-v3")
                self.assertEqual(receipt["credited"]["functions"], 1)
                self.assertEqual(receipt["credited"]["bytes"], 40)
                self.assertEqual(receipt["credited"]["catalogue_functions"], 1)
                self.assertEqual(receipt["credited"]["catalogue_bytes"], 40)
                caller, helper = ({unit["source"]: unit for unit in receipt["units"]}[source]
                                  for source in ("src/caller.c", "src/helper.c"))
                self.assertEqual(helper["functions"], [])
                self.assertEqual(helper["source_sha256"], build.sha256_file(root / "src/helper.c"))
                self.assertEqual(helper["object_sha256"], build.sha256_file(root / "build/synthetic-removed/obj/src/helper.o"))
                self.assertEqual(len(helper["removed_callees"]), 1)
                self.assertEqual(helper["removed_callees"][0]["object"], "src/helper.c")
                self.assertEqual(helper["linker_rewrites"], {})
                rewrites = caller["linker_rewrites"]["func_00100000"]
                self.assertEqual([row["offset"] for row in rewrites], [12, 28])
                self.assertTrue(all(row["target"] is None for row in rewrites))
                self.assertTrue(all(row["removed_callee"] == helper["removed_callees"][0] for row in rewrites))
                self.assertEqual(caller["removed_callees"], [])

    def test_v4_model_and_removed_helper_closure_are_recorded_without_changing_v3(self):
        model = "armlink5-inline-branchnop-v4"
        receipt, root = self.synthetic_build(model=model)
        self.assertEqual(receipt["result"], "PASS")
        self.assertEqual(receipt["inputs"]["linker_model"], model)
        self.assertIn("tools/linker_models.py", build.HARNESS)
        self.assertEqual(receipt["inputs"]["harness_sha256"]["tools/linker_models.py"],
                         build.sha256_file(root / "tools/linker_models.py"))
        caller, helper = ({unit["source"]: unit for unit in receipt["units"]}[source]
                          for source in ("src/caller.c", "src/helper.c"))
        self.assertEqual(helper["functions"], [])
        self.assertEqual(helper["source_sha256"], build.sha256_file(root / "src/helper.c"))
        self.assertEqual(helper["removed_callees"][0]["object"], "src/helper.c")
        self.assertTrue(all(row["target"] is None for row in caller["linker_rewrites"]["func_00100000"]))

    def test_v2_never_uses_candidate_only_helper_elimination(self):
        with self.assertRaisesRegex(inplace.LinkError, "unresolved symbol"):
            self.synthetic_build(model="armlink5-inline-branchnop-v2")

    def test_retained_helper_cannot_receive_a_successful_build_receipt(self):
        with self.assertRaisesRegex(inplace.LinkError, "retained callee"):
            self.synthetic_build(retained=True)

    def test_asymmetric_bound_comdat_alias_cannot_receive_caller_only_or_helper_credit(self):
        for case in ("contradictory", "wrong_original"):
            for credit_helper in (False, True):
                for reverse in (False, True):
                    with self.subTest(case=case, credit_helper=credit_helper, reverse=reverse):
                        with self.assertRaisesRegex(inplace.LinkError, "mixes original-bound and removable callee copies"):
                            self.synthetic_build(model="armlink5-inline-branchnop-v6", comdat_alias=case,
                                                 credit_helper=credit_helper, reverse=reverse)

    def test_credited_zero_size_alias_requires_one_emitted_comdat_owner(self):
        for reverse in (False, True):
            with self.subTest(reverse=reverse):
                with self.assertRaisesRegex(build.BuildError, "func_00100100: defined by both"):
                    self.synthetic_build(model="armlink5-inline-branchnop-v6", comdat_alias="duplicate_owner",
                                         credit_helper=True, reverse=reverse)

    def test_v6_alias_copies_keep_helper_only_receipts_and_every_removed_owner(self):
        for reverse in (False, True):
            with self.subTest(reverse=reverse):
                receipt, _ = self.synthetic_build(model="armlink5-inline-branchnop-v6",
                                                   comdat_alias="unbound_alias", reverse=reverse)
                self.assertEqual(receipt["result"], "PASS")
                self.assertEqual(receipt["inputs"]["linker_model"], "armlink5-inline-branchnop-v6")
                self.assertEqual(receipt["credited"]["functions"], 1)
                self.assertEqual(receipt["credited"]["bytes"], 40)
                units = {unit["source"]: unit for unit in receipt["units"]}
                self.assertEqual(units["src/helper.c"]["functions"], [])
                for source, unit in units.items():
                    self.assertEqual([row["object"] for row in unit["removed_callees"]], [source])
                self.assertIn("bound_variant", units["src/helper.c"]["removed_callees"][0]["symbols"])
                rewrites = units["src/caller.c"]["linker_rewrites"]["func_00100000"]
                self.assertEqual([row["target"] for row in rewrites], [None, None])
                self.assertTrue(all(row["removed_callee"]["object"] == "src/caller.c" for row in rewrites))

    def test_v9_section_eliminations_are_owned_hashed_and_reported_without_extra_credit(self):
        model = inplace_tests.UnreferencedDestructorModelTest.MODEL
        for reverse in (False, True):
            receipt, root = self.synthetic_build(model=model, reverse=reverse, unreferenced=True)
            self.assertEqual(receipt["result"], "PASS")
            self.assertEqual((receipt["credited"]["functions"], receipt["credited"]["bytes"]), (1, 4))
            units = {unit["source"]: unit for unit in receipt["units"]}
            owner = units["src/caller.c"]
            member, = owner["removed_special_members"]
            self.assertEqual(member["object"], "src/caller.c")
            self.assertEqual(owner["linker_section_rewrites"], [{
                "rule": "unused_deleting_destructor", "target": None, "removed_special_member": member}])
            self.assertEqual(owner["linker_rewrites"], {})
            self.assertEqual(owner["object_sha256"], build.sha256_file(root / "build/synthetic-removed/obj/src/caller.o"))
            self.assertNotIn("removed_special_members", units["src/helper.c"])

    def test_v10_receipts_bind_every_owned_unwind_companion_without_crediting_it(self):
        model = inplace_tests.UnreferencedDestructorUnwindModelTest.MODEL
        for reverse in (False, True):
            receipt, root = self.synthetic_build(model=model, reverse=reverse, unreferenced="unwind")
            self.assertEqual(receipt["result"], "PASS")
            self.assertEqual((receipt["credited"]["functions"], receipt["credited"]["bytes"]), (1, 4))
            owner = next(unit for unit in receipt["units"] if unit["source"] == "src/caller.c")
            member, = owner["removed_special_members"]
            self.assertEqual([row["kind"] for row in member["unwind_sections"]], ["exidx", "extab"])
            self.assertEqual({row["object"] for row in member["unwind_sections"]}, {"src/caller.c"})
            self.assertEqual(owner["linker_section_rewrites"][0]["removed_special_member"], member)
            self.assertEqual(owner["object_sha256"], build.sha256_file(root / "build/synthetic-removed/obj/src/caller.o"))

PROFILE = toolchain.Profile(
    "game", "armcc_504_82",
    ("--cpu=MPCore", "--fpmode=fast", "--apcs=/interwork", "--arm", "--split_sections", "-O3", "-Otime"),
)
HAVE_COMPILER = toolchain.armcc_exe(PROFILE.compiler).is_file()


class SyntheticBuildFixture:
    """Shared environment for build, collect and checkpoint integration tests."""

    def setUp(self):
        super().setUp()
        temp = tempfile.TemporaryDirectory()
        self.addCleanup(temp.cleanup)
        self.root = Path(temp.name)
        stack = ExitStack()
        self.addCleanup(stack.close)
        code = bytearray(0x1000)
        functions = []
        for address, value in ((0x100000, 1), (0x100010, 2), (0x100020, 3)):
            # Independently encoded synthetic ARM: mov r0, #value; bx lr.
            struct.pack_into("<II", code, address - 0x100000, 0xE3A00000 | value, 0xE12FFF1E)
            functions.append({"address": address, "size": 8, "isa": "arm", "kind": "function"})
        layout = ctrfmt.CodeLayout(
            "SYNTH", False, ctrfmt.CodeSegment("text", 0x100000, 1, 0x1000),
            ctrfmt.CodeSegment("rodata", 0x101000, 0, 0), ctrfmt.CodeSegment("data", 0x200000, 0, 0),
            0x1000, 0, (),
        )
        self.image = image.Image(bytes(code), layout)
        catalogue = {"image_sha256": self.image.sha256, "functions": functions}
        for rel, text in {
            "config/matches.json": json.dumps({"schema_version": 1, "units": []}),
            "config/catalogue.json": json.dumps(catalogue),
            "config/compiler_profiles.json": "{}",
            "toolchain.lock.json": "{}",
        }.items():
            self.write_source(rel, text)
        (self.root / "docs").mkdir()
        (self.root / "orig/exefs").mkdir(parents=True)
        (self.root / "orig/exefs/code.blz").write_bytes(ctrfmt.blz_compress(self.image.code))
        (self.root / "orig/exheader.bin").write_bytes(b"synthetic exheader hash input")
        for module, attr, value in (
            (build, "ROOT", self.root), (build, "MATCHES", self.root / "config/matches.json"),
            (build, "HARNESS", ()), (image, "CATALOGUE", self.root / "config/catalogue.json"),
            (image, "NAMES", self.root / "config/names.json"),
            (toolchain, "ROOT", self.root), (toolchain, "PROFILES", self.root / "config/compiler_profiles.json"),
            (toolchain, "CACHE", self.root / ".cache/objcache"),
        ):
            stack.enter_context(patch.object(module, attr, value))
        stack.enter_context(patch.object(image.Image, "load", return_value=self.image))
        stack.enter_context(patch.object(image, "load_catalogue", return_value=catalogue))
        stack.enter_context(patch.object(image, "load_names", return_value={}))
        stack.enter_context(patch.object(toolchain, "load_profiles", return_value={"game": PROFILE}))
        stack.enter_context(patch.object(build.denominator, "verify"))
        stack.enter_context(patch.object(build.denominator, "read_history", return_value=[{}]))

    def write_source(self, source, text):
        path = self.root / source
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8")
        return path

    def unit(self, source, *functions):
        return {"source": source, "profile": "game", "functions": list(functions)}

    def build_units(self, name, units):
        ledger = self.write_source(f"config/{name}.json", json.dumps({"units": units}))
        return build.build(name, ledger, fresh=True, jobs=1)


@unittest.skipUnless(HAVE_COMPILER, "pinned compiler not installed (tools/setup.py)")
class BuildTest(SyntheticBuildFixture, unittest.TestCase):
    def test_multi_function_unit_is_verified_function_by_function(self):
        self.write_source("src/native.c", "int func_00100000(void) { return 1; }\n"
                          "int func_00100010(void) { return 2; }\n")
        receipt = self.build_units("native", [self.unit("src/native.c", "func_00100000", "func_00100010")])
        self.assertEqual(receipt["result"], "PASS")
        self.assertEqual(receipt["credited"]["functions"], 2)
        self.assertFalse(receipt["units"][0]["cache_hit"])

    def test_unclaimed_duplicate_definition_is_refused(self):
        self.write_source("src/old.c", "int func_00100000(void) { return 1; }\n"
                          "int func_00100010(void) { return 2; }\n")
        self.write_source("src/new.c", "int func_00100000(void) { return 1; }\n")
        # The old unit no longer claims credit for the moved function, but still defines it.
        with self.assertRaisesRegex(build.BuildError, "defined by both"):
            self.build_units("duplicate", [self.unit("src/old.c", "func_00100010"),
                                           self.unit("src/new.c", "func_00100000")])

    def test_credited_comdat_copies_are_refused_even_when_identical(self):
        template = "template<int x> int f(void) { return x; }\ntemplate int f<1>(void);\n"
        self.write_source("src/old.cpp", template + 'extern "C" int func_00100010(void) { return 2; }\n')
        self.write_source("src/native.cpp", template)
        names = {"_Z1fILi1EEiv": {"address": 0x100000}}
        with patch.object(image, "load_names", return_value=names):
            with self.assertRaisesRegex(build.BuildError, "func_00100000: defined by both"):
                self.build_units("identical-comdat", [self.unit("src/old.cpp", "func_00100010"),
                                                       self.unit("src/native.cpp", "func_00100000")])

    def test_credited_alias_definition_in_third_unit_is_refused(self):
        self.write_source("src/old.c", "int func_00100010(void) { return 2; }\n")
        self.write_source("src/native.cpp",
                          "template<int x> int f(void) { return x; }\ntemplate int f<1>(void);\n")
        self.write_source("src/third.cpp",
                          "template<int x> int g(void) { return x + 3; }\ntemplate int g<1>(void);\n"
                          'extern "C" int func_00100020(void) { return 3; }\n')
        names = {name: {"address": 0x100000} for name in ("_Z1fILi1EEiv", "_Z1gILi1EEiv")}
        with patch.object(image, "load_names", return_value=names):
            with self.assertRaisesRegex(build.BuildError, "func_00100000: defined by both"):
                self.build_units("third-unit-alias", [self.unit("src/old.c", "func_00100010"),
                                                       self.unit("src/native.cpp", "func_00100000"),
                                                       self.unit("src/third.cpp", "func_00100020")])

    def test_uncredited_comdat_copies_preserve_existing_behavior(self):
        template = "template<int x> int f(void) { return x; }\ntemplate int f<1>(void);\n"
        self.write_source("src/a.cpp", template + 'extern "C" int func_00100010(void) { return 2; }\n')
        self.write_source("src/b.cpp", template + 'extern "C" int func_00100020(void) { return 3; }\n')
        names = {"_Z1fILi1EEiv": {"address": 0x100000}}
        with patch.object(image, "load_names", return_value=names):
            receipt = self.build_units("uncredited-comdat", [self.unit("src/a.cpp", "func_00100010"),
                                                             self.unit("src/b.cpp", "func_00100020")])
        self.assertEqual(receipt["result"], "PASS")
        self.assertEqual(receipt["credited"]["functions"], 2)

    def test_changed_function_cannot_keep_credit(self):
        self.write_source("src/new.c", "int func_00100000(void) { return 4; }\n")
        receipt = self.build_units("mismatch", [self.unit("src/new.c", "func_00100000")])
        self.assertEqual(receipt["result"], "FAIL")
        self.assertFalse(receipt["checks"]["credited_functions_exact"])
        self.assertEqual(receipt["mismatched_functions"], ["0x100000"])


class BuildLiteralModelTest(unittest.TestCase):
    def synthetic_build(self, literal_model=MODEL, linker_model=inplace.DEFAULT_LINKER_MODEL,
                        order=(2, 0, 1)):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        root = Path(temporary.name)
        objects = []
        units = []
        first, memory, linker, *_ = fixture(order=order)
        objects.append(first)
        second, second_memory, _, *_ = fixture(order=order)
        address, second_pool = BASE + 0x40, POOL + 0x10000
        second.symbols[0].name = "func_00100040"
        original_pointers = struct.unpack("<4I", second_memory.read(BASE, 16))
        memory.write(address, struct.pack("<4I", *(p + second_pool - POOL for p in original_pointers)))
        memory.write(second_pool, second_memory.read(POOL, second.sections[2].size))
        linker.functions[address] = {"address": address, "size": 16, "isa": "arm", "code_ranges": []}
        objects.append(second)
        for index, obj in enumerate(objects):
            source = f"src/unit{index}.cpp"
            path = root / source
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text("// Synthetic build input identity; no private source.\n", encoding="utf-8")
            units.append({"source": source, "profile": "game", "functions": [obj.symbols[0].name]})
        for name, contents in {
            "config/matches.json": json.dumps({"units": units}).encode(),
            "config/catalogue.json": b"{}",
            "config/names.json": b"{}",
            "config/compiler_profiles.json": b"{}",
            "toolchain.lock.json": b"{}",
            "orig/exheader.bin": b"synthetic-header",
            "orig/exefs/code.blz": b"synthetic-compression",
            "compiler.exe": b"synthetic-compiler",
        }.items():
            path = root / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(contents)
        memory.code = bytes(memory.data)
        memory.sha256 = build.sha256_bytes(memory.code)
        memory.bss = linker.bss
        memory.layout = SimpleNamespace(file_offset=lambda address: address - BASE)
        catalogue = {"image_sha256": memory.sha256, "functions": list(linker.functions.values())}
        original_combine = inplace.ObjectFile.combine

        def compile_units(units, profiles, out, fresh, jobs):
            for index, unit in enumerate(units):
                path = out / "obj" / Path(unit["source"]).with_suffix(".o")
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(f"synthetic-object-{index}".encode())
            return {unit["source"]: False for unit in units}

        def object_file(path):
            return objects[int(path.stem[-1])]

        object_file.combine = original_combine
        with ExitStack() as stack:
            for target, attribute, value in (
                (build, "ROOT", root), (build, "MATCHES", root / "config/matches.json"),
                (build, "HARNESS", ()), (build.inplace, "ObjectFile", object_file),
                (build.toolchain, "PROFILES", root / "config/compiler_profiles.json"),
                (build.image_mod, "CATALOGUE", root / "config/catalogue.json"),
                (build.image_mod, "NAMES", root / "config/names.json"),
            ):
                stack.enter_context(patch.object(target, attribute, value))
            stack.enter_context(patch.object(build.denominator, "verify", return_value=None))
            stack.enter_context(patch.object(build.image_mod.Image, "load", return_value=memory))
            stack.enter_context(patch.object(build.image_mod, "load_catalogue", return_value=catalogue))
            stack.enter_context(patch.object(build.image_mod, "load_names", return_value={}))
            stack.enter_context(patch.object(build, "compile_units", side_effect=compile_units))
            stack.enter_context(patch.object(build.toolchain, "load_profiles", return_value={
                "game": toolchain.Profile("game", "synthetic", ())}))
            stack.enter_context(patch.object(build.toolchain, "armcc_exe", return_value=root / "compiler.exe"))
            stack.enter_context(patch.object(build.ctrfmt, "blz_compress", return_value=b"synthetic-compression"))
            return build.build("synthetic-literals", root / "config/matches.json",
                               linker_model=linker_model, literal_model=literal_model)

    def test_default_off_preserves_ordinary_combined_build(self):
        receipt = self.synthetic_build(literal_model=inplace.DEFAULT_LITERAL_MODEL, order=(0, 1, 2))
        self.assertEqual(receipt["result"], "PASS")
        self.assertEqual(receipt["inputs"]["linker_model"], inplace.DEFAULT_LINKER_MODEL)
        self.assertEqual(receipt["inputs"]["literal_model"], "none")
        self.assertTrue(all(not u["literal_rebindings"] for u in receipt["units"]))

    def test_combined_build_attributes_each_pool_to_its_own_unit(self):
        for model in ("armlink5-inline-branchnop-v2", "armlink5-inline-branchnop-v3",
                      "armlink5-inline-branchnop-v4", "armlink5-inline-branchnop-v6"):
            with self.subTest(model=model):
                receipt = self.synthetic_build(linker_model=model)
                self.assertEqual(receipt["result"], "PASS")
                self.assertEqual(receipt["inputs"]["linker_model"], model)
                self.assertEqual(receipt["inputs"]["literal_model"], MODEL)
                self.assertEqual(receipt["credited"]["functions"], 2)
                for index, unit in enumerate(receipt["units"]):
                    self.assertEqual(len(unit["literal_rebindings"]), 1)
                    pool = unit["literal_rebindings"][0]
                    self.assertEqual(pool["object"], unit["source"])
                    self.assertEqual(pool["base"], POOL + index * 0x10000)
                    self.assertEqual(pool["record_count"], 3)
                    self.assertEqual(unit["removed_callees"], [])

    def test_default_off_still_refuses_permuted_pools(self):
        with self.assertRaisesRegex(inplace.LinkError, "references disagree"):
            self.synthetic_build(literal_model=inplace.DEFAULT_LITERAL_MODEL)

    def test_cli_forwards_literal_and_linker_models_independently(self):
        receipt = {"result": "PASS", "credited": {"functions": 1, "catalogue_functions": 1,
                   "bytes": 4, "catalogue_bytes": 4}, "checks": {}}
        for model in ("armlink5-inline-branchnop-v3", "armlink5-inline-branchnop-v4", "armlink5-inline-branchnop-v6"):
            with self.subTest(model=model), patch.object(build, "build", return_value=receipt) as run, \
                    contextlib.redirect_stdout(io.StringIO()):
                code = build.main(["--name", "synthetic", "--linker-model", model, "--literal-model", MODEL])
                self.assertEqual(code, 0)
                self.assertEqual(run.call_args.args[-2:], (model, MODEL))

    def test_cli_leaves_literal_model_off_by_default(self):
        receipt = {"result": "PASS", "credited": {"functions": 1, "catalogue_functions": 1,
                   "bytes": 4, "catalogue_bytes": 4}, "checks": {}}
        with patch.object(build, "build", return_value=receipt) as run, contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(build.main(["--name", "synthetic"]), 0)
        self.assertEqual(run.call_args.args[-1], "none")


if __name__ == "__main__":
    unittest.main()
