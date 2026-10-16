"""tools/accept.py: finalize needs a PASS prepare plus bound reviews, happens once, and the ledger
stays bound to the hash-chained history."""
import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import accept  # noqa: E402

PATHS = ("ROOT", "MATCHES", "HISTORY", "ACCEPT", "PROFILES", "CATALOGUE")


class AcceptGateTest(unittest.TestCase):
    def setUp(self):
        self.saved = {k: getattr(accept, k) for k in PATHS}
        root = Path(tempfile.mkdtemp())
        accept.ROOT = root
        accept.MATCHES = root / "config/matches.json"
        accept.HISTORY = root / "docs/progress-history.jsonl"
        accept.ACCEPT = root / "accept"
        accept.PROFILES = root / "config/compiler_profiles.json"
        accept.CATALOGUE = root / "config/catalogue.json"
        for rel, text in {
            "config/matches.json": json.dumps({"units": []}),
            "config/compiler_profiles.json": "{}",
            "config/catalogue.json": "{}",
            "src/a.c": "int func_00100000(void) { return 1; }\n",
            "notes.md": "re-derived",
        }.items():
            (root / rel).parent.mkdir(parents=True, exist_ok=True)
            (root / rel).write_text(text, encoding="utf-8")
        (root / "docs").mkdir(exist_ok=True)
        self.root = root
        self.make_prepared("c001", result="PASS")

    def tearDown(self):
        for k, v in self.saved.items():
            setattr(accept, k, v)

    def make_prepared(self, name, result):
        """A prepare record as tools/accept.py prepare writes it (builds are exercised elsewhere)."""
        out = self.root / "accept" / name
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        accept.write_exclusive(out / "matches.json", {"schema_version": 1, "units": units})
        record = {
            "checkpoint": name,
            "base_matches_sha256": accept.sha256_file(accept.MATCHES),
            "ledger_sha256": accept.sha256_file(out / "matches.json"),
            "sources": accept.source_hashes(units),
            "new_units": ["src/a.c"],
            "profiles_sha256": accept.sha256_file(accept.PROFILES),
            "catalogue_sha256": accept.sha256_file(accept.CATALOGUE),
            "builds": [{"credited": {"functions": 1, "bytes": 8, "catalogue_functions": 10, "catalogue_bytes": 80},
                        "outputs": {"code_bin_sha256": "x"}}],
            "result": result,
        }
        accept.write_exclusive(out / "prepare.json", record)

    def review(self, verdict="PASS", name="c001", reviewer="rev1", row="PASS", findings=(), qualifications=()):
        record = {
            "reviewer": reviewer, "family": "openai", "verdict": verdict,
            "functions": [{"function": "func_00100000", "source": "src/a.c", "verdict": row,
                           "method": "tools/match.py re-run", "finding_ids": [f["id"] for f in findings]}],
            "findings": list(findings), "qualifications": list(qualifications),
        }
        path = self.root / f"review-{reviewer}-{verdict}.json"
        path.write_text(json.dumps(record), encoding="utf-8")
        return accept.review(name, path)

    def test_finalize_requires_a_bound_passing_review(self):
        with self.assertRaisesRegex(accept.AcceptError, "need 1"):
            accept.finalize("c001", 1)
        self.review()
        entry = accept.finalize("c001", 1)
        self.assertEqual((entry["seq"], entry["credited_functions"]), (1, 1))
        self.assertEqual(accept.sha256_file(accept.MATCHES), entry["matches_sha256"])
        self.assertEqual(len(accept.verify_history()), 1)
        with self.assertRaisesRegex(accept.AcceptError, "already finalized"):
            accept.finalize("c001", 1)

    def test_fail_review_blocks_finalize(self):
        self.review("FAIL")
        with self.assertRaisesRegex(accept.AcceptError, "FAIL"):
            accept.finalize("c001", 1)

    def test_failed_prepare_cannot_finalize(self):
        self.make_prepared("c002", result="FAIL")
        self.review(name="c002")
        with self.assertRaisesRegex(accept.AcceptError, "prepare is FAIL"):
            accept.finalize("c002", 1)

    def test_source_change_after_review_blocks_finalize(self):
        self.review()
        (self.root / "src/a.c").write_text("int func_00100000(void) { return 2; }\n", encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "bound|changed"):
            accept.finalize("c001", 1)

    def test_unaccepted_credit_is_detected(self):
        (self.root / "config/matches.json").write_text(json.dumps({"units": [{"source": "src/a.c"}]}), encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "no checkpoint"):
            accept.verify_history()

    def test_ledger_edited_after_finalize_is_detected(self):
        self.review()
        accept.finalize("c001", 1)
        (self.root / "src/a.c").write_text("int func_00100000(void) { return 3; }\n", encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "accepted hash"):
            accept.verify_history()

    def test_review_must_cover_every_new_function(self):
        record = {"reviewer": "rev1", "family": "openai", "verdict": "PASS", "functions": [], "findings": [], "qualifications": []}
        path = self.root / "empty.json"
        path.write_text(json.dumps(record), encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "does not cover"):
            accept.review("c001", path)

    def test_blocker_finding_forces_fail_verdict(self):
        blocker = {"id": "F01", "severity": "blocker", "subject": "src/a.c", "summary": "wrong call"}
        with self.assertRaisesRegex(accept.AcceptError, "requires overall verdict FAIL"):
            self.review("PASS", findings=[blocker])
        self.review("FAIL", row="FAIL", findings=[blocker])

    def test_qualified_rows_need_stated_qualifications(self):
        with self.assertRaisesRegex(accept.AcceptError, "require PASS_WITH_QUALIFICATIONS"):
            self.review("PASS", row="QUALIFIED")
        with self.assertRaisesRegex(accept.AcceptError, "at least one stated qualification"):
            self.review("PASS_WITH_QUALIFICATIONS", row="QUALIFIED")
        self.review("PASS_WITH_QUALIFICATIONS", row="QUALIFIED", qualifications=["preserved original bug"])
        self.assertEqual(accept.finalize("c001", 1)["reviews"][0]["verdict"], "PASS_WITH_QUALIFICATIONS")

    def test_checkpoint_names_are_restricted(self):
        with self.assertRaises(accept.AcceptError):
            accept.checkpoint_dir("../escape")


if __name__ == "__main__":
    unittest.main()
