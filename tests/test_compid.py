"""tools/compid.py: a grid pair qualifies only when every supplied source compiled and matched."""
import sys
import tempfile
import unittest
from pathlib import Path

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


if __name__ == "__main__":
    unittest.main()
