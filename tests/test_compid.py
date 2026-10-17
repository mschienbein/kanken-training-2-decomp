"""tools/compid.py: a grid pair qualifies only when every supplied source compiled and matched."""
import json
import sys
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import compid  # noqa: E402
import match  # noqa: E402
import toolchain  # noqa: E402

HAVE_INPUTS = (ROOT / "orig/code.bin").is_file() and toolchain.armcc_exe("armcc_504_82").is_file()
GRID = {
    "compilers": ["armcc_504_82"],
    "flagsets": {"O3-Otime": ["--cpu=MPCore", "--fpmode=fast", "--apcs=/interwork", "--arm", "--split_sections", "-O3", "-Otime"]},
}
GOOD = "struct S { char pad[0x1c]; int x1c; };\nvoid func_0010e664(struct S *s) { s->x1c = 0; }\n"
BROKEN = "void func_0010e664(struct S *s) { s->x1c = ; }\n"
EMPTY = "static int helper(int x) { return x + 1; }\n"
# The v3 NOP entry 0x1852f4 as a C tail-call wrapper: one --branchnop rewrite (EXTENT under v3).
WRAPPER = "extern void func_001852f8(void);\nvoid func_001852f4(void) { func_001852f8(); }\n"


class CompidModelOptionsTest(unittest.TestCase):
    def test_registry_selection_and_json_payload_keep_the_requested_model(self):
        for model in ("armlink5-inline-branchnop-v3", "armlink5-inline-branchnop-v4"):
            with self.subTest(model=model), tempfile.TemporaryDirectory() as directory:
                root = Path(directory)
                source = root / "candidate.c"
                source.write_text(GOOD, encoding="utf-8")
                context = SimpleNamespace(image=SimpleNamespace(sha256="synthetic"), model=SimpleNamespace(name=model))
                empty = {"pairs": ["synthetic"], "functions": [], "results": {}, "failures": {},
                         "matching_all": [], "linker_model": model, "linker_rewrites": {}}
                self.assertEqual(compid.run_grid(context, [], {"compilers": [], "flagsets": {}}, root)["linker_model"], model)
                with patch.object(compid.match, "Context", return_value=context) as selected, \
                        patch.object(compid, "run_grid", return_value=empty), patch("builtins.print"):
                    code = compid.main([str(source), "--out", str(root / "out"), "--linker-model", model])
                self.assertEqual(code, 1)  # no compilation or qualifying grid cell is fabricated
                self.assertEqual(selected.call_args.args[-1], model)
                record = json.loads((root / "out/compid.json").read_text(encoding="utf-8"))
                self.assertEqual(record["linker_model"], model)
                self.assertEqual(record["matching_all"], [])


@unittest.skipUnless(HAVE_INPUTS, "private input or pinned compiler missing")
class CompidFailClosedTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.ctx = match.Context(ROOT / "config/catalogue.json")

    def run_sources(self, *texts):
        tmp = Path(tempfile.mkdtemp())
        sources = []
        for i, text in enumerate(texts):
            path = tmp / f"probe{i}.c"
            path.write_text(text, encoding="utf-8")
            sources.append(path)
        return compid.run_grid(self.ctx, sources, GRID, tmp / "out")

    def test_good_source_qualifies(self):
        self.assertEqual(self.run_sources(GOOD)["matching_all"], ["armcc_504_82:O3-Otime"])

    def test_all_sources_failing_never_qualifies(self):
        result = self.run_sources(BROKEN)
        self.assertEqual(result["functions"], [])
        self.assertEqual(result["matching_all"], [])
        self.assertIn("armcc_504_82:O3-Otime", result["failures"])

    def test_one_failing_source_disqualifies_the_pair(self):
        result = self.run_sources(GOOD, BROKEN)
        self.assertEqual(result["results"]["func_0010e664"]["armcc_504_82:O3-Otime"], "Y")
        self.assertEqual(result["matching_all"], [])

    def test_source_without_catalogued_functions_disqualifies_the_pair(self):
        result = self.run_sources(GOOD, EMPTY)
        self.assertEqual(result["matching_all"], [])
        reasons = list(result["failures"]["armcc_504_82:O3-Otime"].values())
        self.assertEqual(reasons, ["no catalogued function in the object"])

    def test_linker_model_and_rewrites_are_recorded(self):
        """Review F02: a cell must say which linker model produced it and what that model rewrote."""
        result = self.run_sources(WRAPPER)
        self.assertEqual(result["linker_model"], self.ctx.model.name)
        # Catalogue v4 splits the NOP-entry wrapper into its own 4-byte function: with the model it MATCHes.
        self.assertEqual(result["results"]["func_001852f4"]["armcc_504_82:O3-Otime"], "Y")
        [rewrite] = result["linker_rewrites"]["func_001852f4"]["armcc_504_82:O3-Otime"]
        self.assertEqual((rewrite["rule"], rewrite["target"], rewrite["word"]), ("branchnop", 0x1852F8, "0xe1a00000"))
        strict = compid.run_grid(match.Context(ROOT / "config/catalogue.json", "strict-v1"),
                                 [self.write_source(WRAPPER)], GRID, Path(tempfile.mkdtemp()))
        self.assertEqual(strict["linker_model"], "strict-v1")
        self.assertEqual(strict["linker_rewrites"], {})
        self.assertTrue(strict["results"]["func_001852f4"]["armcc_504_82:O3-Otime"].startswith("+"))

    def test_compid_json_binds_the_linker_model(self):
        tmp = Path(tempfile.mkdtemp())
        grid = tmp / "grid.json"
        grid.write_text(json.dumps(GRID), encoding="utf-8")
        code = compid.main([str(self.write_source(WRAPPER)), "--grid", str(grid), "--out", str(tmp / "out"),
                            "--linker-model", "strict-v1"])
        self.assertEqual(code, 1)
        record = json.loads((tmp / "out/compid.json").read_text(encoding="utf-8"))
        self.assertEqual((record["linker_model"], record["linker_rewrites"]), ("strict-v1", {}))

    def write_source(self, text):
        path = Path(tempfile.mkdtemp()) / "probe.c"
        path.write_text(text, encoding="utf-8")
        return path


if __name__ == "__main__":
    unittest.main()
