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
            "include/game/shared.h": "extern int data_00200000;\n",
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
            "headers": accept.header_hashes(),
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

    def test_header_change_after_review_blocks_finalize(self):
        """A shared header changes every object that includes it, so it is bound like a source."""
        self.review()
        (self.root / "include/game/shared.h").write_text("extern short data_00200000;\n", encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "bound|headers changed"):
            accept.finalize("c001", 1)

    def test_unrelated_new_header_during_review_does_not_unbind_it_but_blocks_finalize(self):
        """c002-wave1-r2: a header the integrator added after prepare unbound a valid review.
        Reviews bind the prepared files; finalize still refuses any header added since prepare."""
        (self.root / "include/game/next.h").write_text("struct Next;\n", encoding="utf-8")
        entry = self.review()
        self.assertEqual(entry["headers_added_since_prepare"], ["include/game/next.h"])
        with self.assertRaisesRegex(accept.AcceptError, "headers changed since prepare"):
            accept.finalize("c001", 1)
        (self.root / "include/game/next.h").unlink()
        self.assertEqual(accept.finalize("c001", 1)["seq"], 1)

    def test_accepted_header_edit_is_detected_but_new_headers_are_allowed(self):
        self.review()
        accept.finalize("c001", 1)
        (self.root / "include/game/next.h").write_text("struct Next;\n", encoding="utf-8")
        accept.verify_history()
        (self.root / "include/game/shared.h").unlink()
        with self.assertRaisesRegex(accept.AcceptError, "accepted header changed or disappeared: include/game/shared.h"):
            accept.verify_history()

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

    def stubbed_prepare(self, name, units, literal_models=("none", "none"), literal_rebindings=(), receipt_units=None):
        """accept.prepare with builds, suite, interfaces and git stubbed out (exercised elsewhere)."""
        import types
        receipt = {"name": "x", "result": "PASS", "outputs": {"code_bin_sha256": "x"},
                   "inputs": {"linker_model": "armlink5-inline-branchnop-v2"},
                   "units": [{"source": u["source"], "linker_rewrites": {"func_00100000": [{"kind": "inline_nop"}], "func_00100004": []},
                              "literal_rebindings": list(literal_rebindings)} for u in units],
                   "credited": {"functions": 1, "bytes": 8, "catalogue_functions": 10, "catalogue_bytes": 80}}
        if receipt_units is not None:
            receipt["units"] = receipt_units
            receipt["inputs"]["linker_model"] = "armlink5-inline-branchnop-v3"
        models = iter(literal_models)
        stubs = {
            "build": types.SimpleNamespace(
                build=lambda *a, **k: {**receipt, "inputs": {**receipt["inputs"], "literal_model": next(models)}},
                BuildError=RuntimeError),
            "denominator": types.SimpleNamespace(verify=lambda: None, read_history=lambda: [{}]),
            "interfaces": types.SimpleNamespace(
                audit=lambda units, root: {"units": [], "disagreements": [], "policy": []},
                select_units=lambda s, m, r: [], failed=lambda report, allow=False: False),
            "test_suite": types.SimpleNamespace(run=lambda: {"returncode": 0, "tests_run": 5, "skipped": 0,
                                                             "failed_modules": [], "seconds": 1.0}),
        }
        saved = {k: sys.modules.get(k) for k in stubs}
        sys.modules.update(stubs)
        real_clean_commit = accept.clean_commit
        accept.clean_commit = lambda: "0" * 40
        try:
            cand = self.root / f"config/candidates-{name}.json"
            cand.write_text(json.dumps({"units": units}), encoding="utf-8")
            return accept.prepare(name, cand)
        finally:
            accept.clean_commit = real_clean_commit
            for k, v in saved.items():
                if v is None:
                    sys.modules.pop(k, None)
                else:
                    sys.modules[k] = v

    def test_prepare_records_every_gate_result(self):
        """The prepare record must show each gate's outcome, not only the overall result."""
        record = self.stubbed_prepare("c009", [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}])
        self.assertEqual(set(record["checks"]), {"build_a_pass", "build_b_pass", "builds_identical", "suite_pass", "interfaces_pass"})
        self.assertEqual(record["result"], "PASS")
        self.assertEqual(record["commit"], "0" * 40)
        on_disk = json.loads((self.root / "accept/c009/prepare.json").read_text(encoding="utf-8"))
        self.assertEqual(on_disk["checks"], record["checks"])
        # Gates v1.8: the linker model and every emulated rewrite in new units are recorded.
        self.assertEqual(record["linker_model"], "armlink5-inline-branchnop-v2")
        self.assertEqual(record["linker_rewrites"], {"func_00100000": [{"kind": "inline_nop"}]})
        self.assertEqual(record["literal_model"], "none")
        self.assertEqual(record["literal_rebindings"], {})

    def test_two_builds_must_use_the_same_literal_model(self):
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        record = self.stubbed_prepare("c011", units, ("none", "anonymous-string-records-v1"))
        self.assertEqual(record["result"], "FAIL")
        self.assertFalse(record["checks"]["builds_identical"])

    def test_prepare_records_independent_literal_model_and_rebindings(self):
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        rebindings = [{"rule": "anonymous-string-records-v1", "record_count": 3, "size": 46}]
        record = self.stubbed_prepare("c012", units, ("anonymous-string-records-v1",) * 2, rebindings)
        self.assertEqual(record["result"], "PASS")
        self.assertEqual(record["linker_model"], "armlink5-inline-branchnop-v2")
        self.assertEqual(record["literal_model"], "anonymous-string-records-v1")
        self.assertEqual(record["literal_rebindings"], {"src/a.c": rebindings})

    def test_finalize_preserves_the_independent_literal_model(self):
        path = self.root / "accept/c001/prepare.json"
        record = json.loads(path.read_text())
        record["literal_model"] = "anonymous-string-records-v1"
        path.write_text(json.dumps(record), encoding="utf-8")
        self.review()
        self.assertEqual(accept.finalize("c001", 1)["literal_model"], "anonymous-string-records-v1")

    def test_revised_accepted_unit_must_be_reviewed_again(self):
        """Editing an accepted source in a later checkpoint needs review rows like new work."""
        self.review()
        accept.finalize("c001", 1)
        (self.root / "src/a.c").write_text("int func_00100000(void) { return 4; }\n", encoding="utf-8")
        (self.root / "include/game/shared.h").write_text("extern long data_00200000;\n", encoding="utf-8")
        (self.root / "src/b.c").write_text("int func_00100010(void) { return 1; }\n", encoding="utf-8")
        record = self.stubbed_prepare("c010", [{"source": "src/b.c", "profile": "game", "functions": ["func_00100010"]}])
        self.assertEqual(record["revised_units"], ["src/a.c"])
        self.assertEqual(record["revised_headers"], ["include/game/shared.h"])
        self.assertEqual(accept.new_functions(self.root / "accept/c010", record),
                         {"func_00100000": "src/a.c", "func_00100010": "src/b.c"})
        # c003-wave2 F02: finalize must accept exactly the declared revisions...
        rows = [{"function": f, "source": s, "verdict": "PASS", "method": "tools/match.py re-run", "finding_ids": []}
                for f, s in (("func_00100000", "src/a.c"), ("func_00100010", "src/b.c"))]
        review = self.root / "review-c010.json"
        review.write_text(json.dumps({"reviewer": "rev1", "family": "openai", "verdict": "PASS",
                                      "functions": rows, "findings": [], "qualifications": []}), encoding="utf-8")
        accept.review("c010", review)
        self.assertEqual(accept.finalize("c010", 1)["seq"], 2)
        accept.verify_history()
        # ...and nothing else: an undeclared edit to an accepted file still blocks verify.
        (self.root / "src/a.c").write_text("int func_00100000(void) { return 5; }\n", encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "credited source differs"):
            accept.verify_history()

    def removed_helper_baseline(self):
        """An accepted caller plus a zero-credit helper, without running an actual prepare."""
        helper = {"source": "src/helper.c", "profile": "game", "functions": []}
        (self.root / helper["source"]).write_text(
            "void set_field18(unsigned *p, unsigned v) { p[6] = v; }\n", encoding="utf-8")
        out = self.root / "accept/c001"
        ledger = accept.load_json(out / "matches.json")
        ledger["units"].append(helper)
        (out / "matches.json").write_text(json.dumps(ledger), encoding="utf-8")
        prepared = accept.load_json(out / "prepare.json")
        prepared["ledger_sha256"] = accept.sha256_file(out / "matches.json")
        prepared["sources"] = accept.source_hashes(ledger["units"])
        prepared["new_units"].append(helper["source"])
        (out / "prepare.json").write_text(json.dumps(prepared), encoding="utf-8")
        self.review()
        accept.finalize("c001", 1)
        rewrite = {"offset": 12, "rule": "inline_insn", "target": None,
                   "removed_callee": {"object": "src/helper.c", "section": "i.set_field18",
                                      "symbols": ["set_field18"], "size": 8}}
        receipts = [
            {"source": "src/a.c", "functions": ["func_00100000"], "linker_rewrites": {"func_00100000": [rewrite]}},
            {"source": "src/helper.c", "functions": [], "linker_rewrites": {}},
        ]
        return receipts, rewrite

    def test_helper_only_revision_reopens_unchanged_credited_caller_with_metadata(self):
        receipts, rewrite = self.removed_helper_baseline()
        (self.root / "src/helper.c").write_text(
            "/* revised contract */\nvoid set_field18(unsigned *p, unsigned v) { p[6] = v; }\n",
            encoding="utf-8")
        record = self.stubbed_prepare("c011", [], receipt_units=receipts)
        self.assertEqual(record["revised_units"], ["src/helper.c"])
        self.assertEqual(record["linker_rewrites"], {"func_00100000": [rewrite]})
        self.assertEqual(record["dependency_reviews"], {
            "func_00100000": {"source": "src/a.c", "revised_helpers": ["src/helper.c"], "revised_headers": []},
        })
        self.assertEqual(accept.new_functions(self.root / "accept/c011", record), {"func_00100000": "src/a.c"})
        empty = {"reviewer": "rev1", "family": "openai", "verdict": "PASS", "functions": [],
                 "findings": [], "qualifications": []}
        path = self.root / "empty-helper-review.json"
        path.write_text(json.dumps(empty), encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "does not cover"):
            accept.review("c011", path)
        with self.assertRaisesRegex(accept.AcceptError, "identity provenance requires a QUALIFIED"):
            self.review(name="c011")
        reviewed = self.review("PASS_WITH_QUALIFICATIONS", name="c011", row="QUALIFIED",
                              qualifications=["[INFERENCE] removed helper identity; contract and all caller paths reviewed"])
        self.assertEqual(reviewed["functions"][0]["source"], "src/a.c")
        self.assertEqual(accept.finalize("c011", 1)["seq"], 2)

    def test_header_contract_revision_reopens_r1_callers_without_source_changes(self):
        receipts, rewrite = self.removed_helper_baseline()
        (self.root / "include/game/shared.h").write_text("extern const int data_00200000;\n", encoding="utf-8")
        record = self.stubbed_prepare("c012", [], receipt_units=receipts)
        self.assertEqual(record["revised_units"], [])
        self.assertEqual(record["linker_rewrites"], {"func_00100000": [rewrite]})
        self.assertEqual(record["dependency_reviews"]["func_00100000"]["revised_headers"], ["include/game/shared.h"])
        self.assertEqual(accept.new_functions(self.root / "accept/c012", record), {"func_00100000": "src/a.c"})

    def test_unchanged_helper_does_not_reopen_unrelated_credited_callers(self):
        receipts, _ = self.removed_helper_baseline()
        record = self.stubbed_prepare("c013", [], receipt_units=receipts)
        self.assertEqual(record["dependency_reviews"], {})
        self.assertEqual(record["linker_rewrites"], {})
        self.assertEqual(accept.new_functions(self.root / "accept/c013", record), {})

    def test_dependency_row_must_name_a_credited_caller_and_its_source(self):
        record = accept.load_json(self.root / "accept/c001/prepare.json")
        record["new_units"] = []
        record["dependency_reviews"] = {"func_00100000": {"source": "src/helper.c"}}
        with self.assertRaisesRegex(accept.AcceptError, "not a credited caller"):
            accept.new_functions(self.root / "accept/c001", record)

    def test_helper_revision_selects_every_dependent_caller_and_not_other_helpers(self):
        rows = lambda helper: [{"target": None, "removed_callee": {"object": helper}}]
        first, second, other = rows("src/helper.c"), rows("src/helper.c"), rows("src/other.c")
        receipts = {"units": [
            {"source": "src/a.c", "linker_rewrites": {"func_00100000": first}},
            {"source": "src/b.c", "linker_rewrites": {"func_00100010": second, "func_00100020": other}},
            {"source": "src/helper.c", "functions": [], "linker_rewrites": {}},
        ]}
        rewrites, dependencies = accept.linker_review_summary(receipts, {"src/helper.c"}, [])
        self.assertEqual(rewrites, {"func_00100000": first, "func_00100010": second})
        self.assertEqual(set(dependencies), {"func_00100000", "func_00100010"})

    def test_prepare_refuses_a_dirty_tree(self):
        """Gates run on one committed tree; an uncommitted edit anywhere stops prepare."""
        import subprocess
        run = lambda *a: subprocess.run(["git", *a], cwd=self.root, check=True, capture_output=True)  # noqa: E731
        run("init", "-q")
        run("-c", "user.name=t", "-c", "user.email=t@t", "add", "-A")
        run("-c", "user.name=t", "-c", "user.email=t@t", "commit", "-qm", "base")
        self.assertEqual(len(accept.clean_commit()), 40)
        (self.root / "src/a.c").write_text("int func_00100000(void) { return 9; }\n", encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "not clean"):
            accept.clean_commit()
        (self.root / "src/a.c").write_text("int func_00100000(void) { return 1; }\n", encoding="utf-8")
        (self.root / "tools").mkdir()
        (self.root / "tools/new.py").write_text("", encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "not clean"):
            accept.clean_commit()

    def test_checkpoint_names_are_restricted(self):
        with self.assertRaises(accept.AcceptError):
            accept.checkpoint_dir("../escape")


if __name__ == "__main__":
    unittest.main()
