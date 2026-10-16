"""tools/denominator.py: the catalogue can only change together with a chained history entry."""
import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import denominator  # noqa: E402


def catalogue(sizes):
    return {
        "image_sha256": "0" * 64,
        "functions": [{"address": 0x100000 + 0x100 * i, "size": s} for i, s in enumerate(sizes)],
        "stats": {"bytes_by_range_kind": {"unknown": 4}},
    }


class DenominatorTest(unittest.TestCase):
    def setUp(self):
        self.tmp = Path(tempfile.mkdtemp())
        self.saved = denominator.CATALOGUE, denominator.HISTORY
        denominator.CATALOGUE = self.tmp / "catalogue.json"
        denominator.HISTORY = self.tmp / "history.jsonl"
        denominator.CATALOGUE.write_text(json.dumps(catalogue([8, 12])), encoding="utf-8")

    def tearDown(self):
        denominator.CATALOGUE, denominator.HISTORY = self.saved

    def test_chain_records_and_verifies_revisions(self):
        first = denominator.record("v1")
        self.assertEqual((first["version"], first["functions"], first["function_bytes"]), (1, 2, 20))
        denominator.CATALOGUE.write_text(json.dumps(catalogue([8, 16])), encoding="utf-8")
        second = denominator.record("v2")
        self.assertEqual(second["prev"], first["entry_sha256"])
        self.assertEqual(len(denominator.verify()), 2)

    def test_unrecorded_catalogue_edit_is_detected(self):
        denominator.record("v1")
        denominator.CATALOGUE.write_text(json.dumps(catalogue([8, 13])), encoding="utf-8")
        with self.assertRaises(SystemExit):
            denominator.verify()

    def test_rewritten_history_is_detected(self):
        denominator.record("v1")
        lines = denominator.HISTORY.read_text(encoding="utf-8").splitlines()
        entry = json.loads(lines[0])
        entry["functions"] = 3
        denominator.HISTORY.write_text(json.dumps(entry) + "\n", encoding="utf-8")
        with self.assertRaises(SystemExit):
            denominator.verify()

    def test_unchanged_catalogue_is_not_rerecorded(self):
        denominator.record("v1")
        with self.assertRaises(SystemExit):
            denominator.record("again")


if __name__ == "__main__":
    unittest.main()
