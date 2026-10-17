"""Synthetic removed-helper composition and zero-credit build receipt closure; no private image."""
import json
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


if __name__ == "__main__":
    unittest.main()
