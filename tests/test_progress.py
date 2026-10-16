"""ci/progress_report.py: measures from catalogue + matches, and refusal of uncatalogued credit."""
import json
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

CATALOGUE = {
    "schema_version": 1,
    "image": "code.bin",
    "image_sha256": "0" * 64,
    "segments": [
        {"name": "text", "address": 0x100000, "size": 0x20000, "mapped_size": 0x20000},
        {"name": "rodata", "address": 0x120000, "size": 0x100, "mapped_size": 0x1000},
        {"name": "data", "address": 0x121000, "size": 0x40, "mapped_size": 0x1000},
    ],
    "functions": [
        {"address": 0x100000, "size": 0x40, "isa": "arm"},
        {"address": 0x100040, "size": 0x20, "isa": "arm"},
        {"address": 0x110000, "size": 0x100, "isa": "thumb"},
    ],
}


class ProgressReportTest(unittest.TestCase):
    def run_report(self, units, names=None):
        tmp = Path(tempfile.mkdtemp())
        (tmp / "ci").mkdir()
        (tmp / "config").mkdir()
        shutil.copy(ROOT / "ci/progress_report.py", tmp / "ci")
        (tmp / "config/catalogue.json").write_text(json.dumps(CATALOGUE))
        (tmp / "config/matches.json").write_text(json.dumps({"units": units}))
        if names is not None:
            (tmp / "config/names.json").write_text(json.dumps({"symbols": names}))
        proc = subprocess.run(
            [sys.executable, "ci/progress_report.py", "--output", "report.json"],
            cwd=tmp, capture_output=True, text=True, check=False,
        )
        report = json.loads((tmp / "report.json").read_text(encoding="utf-8")) if proc.returncode == 0 else None
        return proc, report

    def test_measures_count_credited_bytes_and_blocks(self):
        proc, report = self.run_report(
            [{"source": "src/a.c", "functions": ["func_00100040", "Named"]}],
            {"Named": {"address": 0x110000}},
        )
        self.assertEqual(proc.returncode, 0, proc.stderr)
        m = report["measures"]
        self.assertEqual(report["version"], 2)
        self.assertEqual((m["total_code"], m["matched_code"]), ("352", "288"))
        self.assertEqual((m["total_functions"], m["matched_functions"]), (3, 2))
        self.assertEqual(m["total_data"], str(0x140))
        self.assertEqual([u["name"] for u in report["units"]], ["text_00100000", "text_00110000"])
        self.assertEqual(report["units"][1]["measures"]["complete_units"], 1)
        names = [f["name"] for u in report["units"] for f in u["functions"]]
        self.assertEqual(names, ["func_00100000", "func_00100040", "Named"])

    def test_credit_outside_catalogue_is_refused(self):
        proc, _ = self.run_report([{"source": "src/a.c", "functions": ["func_00100004"]}])
        self.assertNotEqual(proc.returncode, 0)
        self.assertIn("not a catalogued function", proc.stderr)


if __name__ == "__main__":
    unittest.main()
