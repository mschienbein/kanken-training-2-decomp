"""Build composition and receipts for synthetic removed helpers and anonymous literal records."""
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
import inplace  # noqa: E402
import toolchain  # noqa: E402
from tests import test_inplace as inplace_tests  # noqa: E402
from tests.test_literal_pools import MODEL, POOL, fixture  # noqa: E402

BASE = inplace_tests.BASE


class BuildRemovedCalleeModelTest(unittest.TestCase):
    def synthetic_build(self, model="armlink5-inline-branchnop-v3", retained=False, reverse=False):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        root = Path(temporary.name)
        caller, memory, functions = inplace_tests.RemovedCalleeModelTest().fixture()
        helper = inplace.ObjectFile.__new__(inplace.ObjectFile)
        definition = caller.symbols.pop()
        helper.sections, helper.symbols = {2: caller.sections.pop(2)}, [definition]
        if retained:
            helper.sections[2].retention = ("__tagsym$$used",)
        external = inplace.Symbol(definition.name, 0, 0, "STT_NOTYPE", "STB_GLOBAL", "SHN_UNDEF")
        caller.symbols.append(external)
        for relocation in caller.sections[1].relocs:
            relocation.symbol = external
        pairs = [
            ({"source": "src/caller.c", "profile": "game", "functions": ["func_00100000"]}, caller),
            ({"source": "src/helper.c", "profile": "game", "functions": []}, helper),
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
            "orig/exheader.bin": b"synthetic-header",
            "orig/exefs/code.blz": b"synthetic-compression",
            "compiler.exe": b"synthetic-compiler",
        }.items():
            path = root / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(contents)
        memory.code = memory.read(BASE, 40)
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

    def test_v2_never_uses_candidate_only_helper_elimination(self):
        with self.assertRaisesRegex(inplace.LinkError, "unresolved symbol"):
            self.synthetic_build(model="armlink5-inline-branchnop-v2")

    def test_retained_helper_cannot_receive_a_successful_build_receipt(self):
        with self.assertRaisesRegex(inplace.LinkError, "retained callee"):
            self.synthetic_build(retained=True)


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
        for model in ("armlink5-inline-branchnop-v2", "armlink5-inline-branchnop-v3"):
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
        with patch.object(build, "build", return_value=receipt) as run, contextlib.redirect_stdout(io.StringIO()):
            code = build.main(["--name", "synthetic", "--linker-model", "armlink5-inline-branchnop-v3",
                               "--literal-model", MODEL])
        self.assertEqual(code, 0)
        self.assertEqual(run.call_args.args[-2:], ("armlink5-inline-branchnop-v3", MODEL))

    def test_cli_leaves_literal_model_off_by_default(self):
        receipt = {"result": "PASS", "credited": {"functions": 1, "catalogue_functions": 1,
                   "bytes": 4, "catalogue_bytes": 4}, "checks": {}}
        with patch.object(build, "build", return_value=receipt) as run, contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(build.main(["--name", "synthetic"]), 0)
        self.assertEqual(run.call_args.args[-1], "none")


if __name__ == "__main__":
    unittest.main()
