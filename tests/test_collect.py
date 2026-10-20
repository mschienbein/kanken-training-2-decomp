"""Collection keeps accepted rows opt-in and re-derives every requested move at its destination."""
import io
import json
import sys
import unittest
from contextlib import redirect_stderr, redirect_stdout
from pathlib import Path
from unittest.mock import patch

from tests import test_inplace
from tests.test_build import HAVE_COMPILER, SyntheticBuildFixture

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import collect  # noqa: E402


@unittest.skipUnless(HAVE_COMPILER, "pinned compiler not installed (tools/setup.py)")
class CollectTest(SyntheticBuildFixture, unittest.TestCase):
    def setUp(self):
        super().setUp()
        patcher = patch.object(collect, "ROOT", self.root)
        patcher.start()
        self.addCleanup(patcher.stop)
        self.write_source("src/old.c", "int func_00100000(void) { return 1; }\n")
        self.write_source("config/matches.json", json.dumps({
            "schema_version": 1, "units": [self.unit("src/old.c", "func_00100000")],
        }))

    def author_result(self, name, source, functions):
        rows = [{"status": "match", "source": source, "address": f"0x{int(fn[5:], 16):x}", "profile": "game"}
                for fn in functions]
        return self.write_source(f"scratch/{name}.json", json.dumps({"agent": name, "functions": rows}))

    def run_collect(self, results, *extra):
        candidates = self.root / "config/candidates/test.json"
        stdout, stderr = io.StringIO(), io.StringIO()
        with redirect_stdout(stdout), redirect_stderr(stderr):
            code = collect.main(["--dest", "src/native", "--candidates", str(candidates),
                                 *extra, *(str(p) for p in results)])
        record = json.loads(candidates.read_text(encoding="utf-8")) if candidates.exists() else None
        return code, record, stdout.getvalue() + stderr.getvalue()

    def test_accepted_function_is_still_refused_by_default(self):
        source = "scratch/native.c"
        self.write_source(source, "int func_00100000(void) { return 1; }\n")
        result = self.author_result("author", source, ["func_00100000"])
        code, record, output = self.run_collect([result])
        self.assertEqual(code, 0)
        self.assertEqual(record["units"], [])
        self.assertNotIn("moves", record)
        self.assertIn("already accepted or claimed", output)
        self.assertFalse((self.root / "src/native/native.c").exists())

    def test_candidate_without_moves_keeps_existing_shape(self):
        source = "scratch/new.c"
        self.write_source(source, "int func_00100010(void) { return 2; }\n")
        result = self.author_result("author", source, ["func_00100010"])
        code, record, _ = self.run_collect([result])
        self.assertEqual(code, 0)
        self.assertEqual(record["units"], [{**self.unit("src/native/new.c", "func_00100010"), "author": "author"}])
        self.assertNotIn("moves", record)

    def test_opt_in_move_can_share_a_native_unit_with_new_functions(self):
        source = "scratch/native.c"
        self.write_source(source, "int func_00100000(void) { return 1; }\n"
                          "int func_00100010(void) { return 2; }\n")
        result = self.author_result("author", source, ["func_00100000", "func_00100010"])
        code, record, _ = self.run_collect([result], "--rehome", "func_00100000")
        self.assertEqual(code, 0)
        self.assertEqual(record["units"][0]["functions"], ["func_00100000", "func_00100010"])
        self.assertEqual(record["moves"], [{"function": "func_00100000", "from": "src/old.c", "to": "src/native/native.c"}])
        self.assertTrue((self.root / "src/old.c").is_file(), "collect never edits the accepted source")

    def test_requested_move_is_not_collected_twice(self):
        results = []
        for name in ("one", "two"):
            source = f"scratch/{name}.c"
            self.write_source(source, "int func_00100000(void) { return 1; }\n")
            results.append(self.author_result(name, source, ["func_00100000"]))
        code, record, output = self.run_collect(results, "--rehome", "func_00100000")
        self.assertEqual(code, 0)
        self.assertEqual(len(record["units"]), 1)
        self.assertEqual(len(record["moves"]), 1)
        self.assertIn("already accepted or claimed", output)

    def test_changed_moved_function_is_not_collected(self):
        source = "scratch/wrong.c"
        self.write_source(source, "int func_00100000(void) { return 4; }\n")
        result = self.author_result("author", source, ["func_00100000"])
        code, record, output = self.run_collect([result], "--rehome", "func_00100000")
        self.assertEqual(code, 0)
        self.assertEqual(record["units"], [])
        self.assertNotIn("moves", record)
        self.assertIn("does not re-derive MATCH", output)
        self.assertFalse((self.root / "src/native/wrong.c").exists())

    def test_rehome_requires_an_accepted_function(self):
        source = "scratch/new.c"
        self.write_source(source, "int func_00100010(void) { return 2; }\n")
        result = self.author_result("author", source, ["func_00100010"])
        code, record, output = self.run_collect([result], "--rehome", "func_00100010")
        self.assertEqual(code, 1)
        self.assertIsNone(record)
        self.assertIn("not accepted", output)

    def test_explicit_models_reach_the_existing_matcher_context(self):
        source = "scratch/new.c"
        self.write_source(source, "int func_00100010(void) { return 2; }\n")
        result = self.author_result("author", source, ["func_00100010"])
        with patch.object(collect.match, "Context", wraps=collect.match.Context) as context:
            code, record, _ = self.run_collect(
                [result], "--linker-model", "armlink5-inline-branchnop-v6",
                "--literal-model", "anonymous-string-records-v1")
        self.assertEqual(code, 0)
        self.assertEqual(len(record["units"]), 1)
        context.assert_called_once_with(
            self.root / "config/catalogue.json", model="armlink5-inline-branchnop-v6",
            literal_model="anonymous-string-records-v1")

    def removed_helper_inputs(self, helper=None):
        fixture = test_inplace.SwitchRemovedCalleeTest().fixture()
        original = fixture.original + self.image.code[len(fixture.original):]
        memory = collect.match.image_mod.Image(original, self.image.layout)
        catalogue = {"image_sha256": memory.sha256, "functions": list(fixture.catalogue.values())}
        for target, method, value in (
            (collect.match.image_mod.Image, "load", memory),
            (collect.match.image_mod, "load_catalogue", catalogue),
        ):
            patcher = patch.object(target, method, return_value=value)
            patcher.start()
            self.addCleanup(patcher.stop)
        self.write_source("scratch/caller.c", test_inplace.SwitchRemovedCalleeTest.CALLER)
        self.write_source("scratch/helper.c", helper or test_inplace.SwitchRemovedCalleeTest.HELPER)
        return self.author_result("author", "scratch/caller.c", ["func_00100000"])

    def test_extra_source_is_rederived_and_kept_as_a_zero_credit_unit(self):
        result = self.removed_helper_inputs()
        code, record, output = self.run_collect(
            [result], "--rehome", "func_00100000", "--extra-source", "scratch/helper.c",
            "--linker-model", "armlink5-inline-branchnop-v6",
            "--literal-model", "anonymous-string-records-v1")
        self.assertEqual(code, 0)
        self.assertNotIn("refused", output)
        self.assertEqual(record["units"], [
            {**self.unit("src/native/caller.c", "func_00100000"), "author": "author"},
            self.unit("src/native/helper.c"),
        ])
        self.assertEqual((self.root / "src/native/helper.c").read_text(), test_inplace.SwitchRemovedCalleeTest.HELPER)
        self.assertEqual(record["moves"], [
            {"function": "func_00100000", "from": "src/old.c", "to": "src/native/caller.c"}])

    def test_extra_source_does_not_enable_removed_callees_under_the_default_model(self):
        result = self.removed_helper_inputs()
        code, record, output = self.run_collect(
            [result], "--rehome", "func_00100000", "--extra-source", "scratch/helper.c")
        self.assertEqual(code, 0)
        self.assertEqual(record["units"], [])
        self.assertIn("unresolved symbol", output)
        self.assertFalse((self.root / "src/native/caller.c").exists())
        self.assertFalse((self.root / "src/native/helper.c").exists())

    def test_wrong_extra_source_is_not_credited_and_both_copies_are_removed(self):
        result = self.removed_helper_inputs("void set_field18(unsigned *p, unsigned v) { p[7] = v; }\n")
        code, record, output = self.run_collect(
            [result], "--rehome", "func_00100000", "--extra-source", "scratch/helper.c",
            "--linker-model", "armlink5-inline-branchnop-v6")
        self.assertEqual(code, 0)
        self.assertEqual(record["units"], [])
        self.assertIn("does not re-derive MATCH", output)
        self.assertFalse((self.root / "src/native/caller.c").exists())
        self.assertFalse((self.root / "src/native/helper.c").exists())

    def test_extra_source_cannot_depend_on_an_author_local_header(self):
        result = self.removed_helper_inputs('#include "private_helper.h"\n')
        self.write_source("scratch/private_helper.h", test_inplace.SwitchRemovedCalleeTest.HELPER)
        code, record, output = self.run_collect(
            [result], "--rehome", "func_00100000", "--extra-source", "scratch/helper.c",
            "--linker-model", "armlink5-inline-branchnop-v6")
        self.assertEqual(code, 1)
        self.assertIsNone(record)
        self.assertIn("helper compilation refused", output)
        self.assertFalse((self.root / "src/native/helper.c").exists())

    def test_extra_source_name_clash_is_refused_before_copying(self):
        result = self.removed_helper_inputs()
        self.write_source("scratch/other/helper.c", test_inplace.SwitchRemovedCalleeTest.HELPER)
        code, record, output = self.run_collect(
            [result], "--extra-source", "scratch/helper.c", "--extra-source", "scratch/other/helper.c")
        self.assertEqual(code, 1)
        self.assertIsNone(record)
        self.assertIn("destination name clashes", output)
        self.assertFalse((self.root / "src/native/helper.c").exists())


if __name__ == "__main__":
    unittest.main()
