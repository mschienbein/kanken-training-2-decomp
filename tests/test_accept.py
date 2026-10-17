"""tools/accept.py: finalize needs a PASS prepare plus bound reviews, happens once, and the ledger
stays bound to the hash-chained history."""
import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from tests.test_build import HAVE_COMPILER, SyntheticBuildFixture

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import accept  # noqa: E402
import image  # noqa: E402
import inplace  # noqa: E402

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

    def stubbed_prepare(self, name, units, literal_models=("none", "none"), literal_rebindings=()):
        """accept.prepare with builds, suite, interfaces and git stubbed out (exercised elsewhere)."""
        import types
        receipt = {"name": "x", "result": "PASS", "outputs": {"code_bin_sha256": "x"},
                   "inputs": {"linker_model": "armlink5-inline-branchnop-v2"},
                   "units": [{"source": u["source"], "linker_rewrites": {"func_00100000": [{"kind": "inline_nop"}], "func_00100004": []},
                              "literal_rebindings": list(literal_rebindings)} for u in units],
                   "credited": {"functions": 1, "bytes": 8, "catalogue_functions": 10, "catalogue_bytes": 80}}
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


class RehomeCompositionTest(unittest.TestCase):
    def setUp(self):
        self.base = [
            {"source": "src/a.c", "profile": "game", "functions": ["func_00100000", "func_00100010"]},
            {"source": "src/b.c", "profile": "game", "functions": ["func_00100020"]},
        ]

    def test_move_is_explicit_and_preserves_every_accepted_function(self):
        unit = {"source": "src/b.c", "profile": "game", "functions": ["func_00100000", "func_00100020"]}
        move = {"function": "func_00100000", "from": "src/a.c", "to": "src/b.c"}
        units, moves, removed = accept.compose_units(self.base, {"units": [unit], "moves": [move]}, "c002")
        self.assertEqual(units[0]["functions"], ["func_00100010"])
        self.assertEqual(moves, [{**move, "checkpoint": "c002"}])
        self.assertEqual(removed, [])
        self.assertEqual(self.base[0]["functions"], ["func_00100000", "func_00100010"])
        with self.assertRaisesRegex(accept.AcceptError, "already in the accepted ledger"):
            accept.compose_units(self.base, {"units": [unit]}, "c002")

    def test_moved_function_cannot_be_claimed_twice(self):
        one = {"source": "src/native.c", "profile": "game", "functions": ["func_00100000"]}
        two = {**one, "source": "src/duplicate.c"}
        with self.assertRaisesRegex(accept.AcceptError, "credited by both"):
            accept.compose_units(self.base, {"units": [one, two]}, "c002")
        # A new candidate cannot silently steal credit without a move record either.
        with self.assertRaisesRegex(accept.AcceptError, "credited by both"):
            accept.compose_units(self.base, {"units": [one]}, "c002")

    def test_bad_move_records_are_refused(self):
        unit = {"source": "src/native.c", "profile": "game", "functions": ["func_00100000"]}
        move = {"function": "func_00100000", "from": "src/a.c", "to": "src/native.c"}
        for moves, message in (
            ([{**move, "from": "src/wrong.c"}], "accepted source"),
            ([{**move, "to": "src/missing.c"}], "destination"),
            ([move, move], "declared twice"),
            ([{**move, "unexpected": True}], "list of"),
        ):
            with self.subTest(moves=moves), self.assertRaisesRegex(accept.AcceptError, message):
                accept.compose_units(self.base, {"units": [unit], "moves": moves}, "c002")

    def test_existing_destination_must_retain_its_accepted_functions(self):
        unit = {"source": "src/b.c", "profile": "game", "functions": ["func_00100000"]}
        move = {"function": "func_00100000", "from": "src/a.c", "to": "src/b.c"}
        with self.assertRaisesRegex(accept.AcceptError, "accepted functions cannot be removed"):
            accept.compose_units(self.base, {"units": [unit], "moves": [move]}, "c002")


@unittest.skipUnless(HAVE_COMPILER, "pinned compiler not installed (tools/setup.py)")
class RehomeGateTest(SyntheticBuildFixture, unittest.TestCase):
    def setUp(self):
        super().setUp()
        for key, value in (
            ("ROOT", self.root), ("MATCHES", self.root / "config/matches.json"),
            ("HISTORY", self.root / "docs/progress-history.jsonl"), ("ACCEPT", self.root / "accept"),
            ("PROFILES", self.root / "config/compiler_profiles.json"), ("CATALOGUE", self.root / "config/catalogue.json"),
        ):
            patcher = patch.object(accept, key, value)
            patcher.start()
            self.addCleanup(patcher.stop)
        # Only the real compiler, linker and byte verifier run here; do not invoke the full suite
        # recursively or require these temporary fixtures to be committed in git.
        for key, value in (
            ("clean_commit", lambda: "0" * 40),
            ("run_suite", lambda: {"returncode": 0, "summary": "suite isolated by fixture"}),
        ):
            patcher = patch.object(accept, key, value)
            patcher.start()
            self.addCleanup(patcher.stop)
        import interfaces
        patcher = patch.object(interfaces, "audit", return_value={"units": [], "disagreements": [], "policy": [], "unchecked": []})
        patcher.start()
        self.addCleanup(patcher.stop)

    def prepare_checkpoint(self, name, units, moves=()):
        candidate = {"schema_version": 1, "units": units}
        if moves:
            candidate["moves"] = list(moves)
        path = self.write_source(f"config/candidates/{name}.json", json.dumps(candidate))
        return accept.prepare(name, path)

    def finalize_checkpoint(self, name):
        out = self.root / "accept" / name
        prepared = accept.load_json(out / "prepare.json")
        rows = [{"function": fn, "source": source, "verdict": "PASS", "method": "synthetic build re-derived",
                 "finding_ids": []} for fn, source in accept.new_functions(out, prepared).items()]
        record = {"reviewer": "rev1", "family": "test", "verdict": "PASS", "functions": rows,
                  "findings": [], "qualifications": []}
        path = self.write_source(f"review-{name}.json", json.dumps(record))
        accept.review(name, path)
        return accept.finalize(name, 1)

    def accept_initial_units(self, split=False):
        if split:
            self.write_source("src/a.c", "int func_00100000(void) { return 1; }\n")
            self.write_source("src/b.c", "int func_00100010(void) { return 2; }\n")
            units = [self.unit("src/a.c", "func_00100000"), self.unit("src/b.c", "func_00100010")]
        else:
            self.write_source("src/a.c", "int func_00100000(void) { return 1; }\n"
                              "int func_00100010(void) { return 2; }\n")
            self.write_source("src/b.c", "int func_00100020(void) { return 3; }\n")
            units = [self.unit("src/a.c", "func_00100000", "func_00100010"),
                     self.unit("src/b.c", "func_00100020")]
        self.assertEqual(self.prepare_checkpoint("c100", units)["result"], "PASS")
        return self.finalize_checkpoint("c100")

    def test_move_between_existing_units_keeps_continuous_credit(self):
        before = self.accept_initial_units()
        self.write_source("src/a.c", "int func_00100010(void) { return 2; }\n")
        self.write_source("src/b.c", "int func_00100020(void) { return 3; }\n"
                          "int func_00100000(void) { return 1; }\n")
        move = {"function": "func_00100000", "from": "src/a.c", "to": "src/b.c"}
        record = self.prepare_checkpoint("c101", [self.unit("src/b.c", "func_00100020", "func_00100000")], [move])
        self.assertEqual(record["result"], "PASS")
        self.assertEqual(record["new_units"], [])
        self.assertEqual(record["revised_units"], ["src/a.c", "src/b.c"])
        self.assertEqual(accept.new_functions(self.root / "accept/c101", record),
                         {"func_00100010": "src/a.c", "func_00100020": "src/b.c", "func_00100000": "src/b.c"})
        entry = self.finalize_checkpoint("c101")
        self.assertEqual(entry["moves"], [{**move, "checkpoint": "c101"}])
        self.assertEqual((entry["credited_functions"], entry["credited_bytes"]),
                         (before["credited_functions"], before["credited_bytes"]))
        self.assertEqual(len(accept.verify_history()), 2)

    def test_new_native_unit_retires_empty_units_and_preserves_history(self):
        self.accept_initial_units(split=True)
        original = {p.name: p.read_bytes() for p in (self.root / "accept/c100").glob("*.json")}
        self.write_source("src/native.c", "int func_00100000(void) { return 1; }\n"
                          "int func_00100010(void) { return 2; }\nint func_00100020(void) { return 3; }\n")
        for name in ("a", "b"):
            (self.root / f"src/{name}.c").unlink()
        moves = [{"function": fn, "from": source, "to": "src/native.c"}
                 for fn, source in (("func_00100000", "src/a.c"), ("func_00100010", "src/b.c"))]
        record = self.prepare_checkpoint(
            "c101", [self.unit("src/native.c", "func_00100000", "func_00100010", "func_00100020")], moves)
        self.assertEqual(record["result"], "PASS")
        self.assertEqual(record["new_units"], ["src/native.c"])
        self.assertEqual(record["revised_units"], [])
        self.assertEqual(record["removed_units"], ["src/a.c", "src/b.c"])
        entry = self.finalize_checkpoint("c101")
        self.assertEqual(entry["credited_functions"], 3)
        self.assertEqual(len(accept.verify_history()), 2)
        self.assertEqual({p.name: p.read_bytes() for p in (self.root / "accept/c100").glob("*.json")}, original)
        # A second move is validated against its immediate predecessor, not the first acceptance.
        self.write_source("src/native.c", "int func_00100000(void) { return 1; }\n"
                          "int func_00100020(void) { return 3; }\n")
        self.write_source("src/readable.c", "int func_00100010(void) { return 2; }\n")
        self.assertEqual(self.prepare_checkpoint(
            "c102", [self.unit("src/readable.c", "func_00100010")],
            [{"function": "func_00100010", "from": "src/native.c", "to": "src/readable.c"}])["result"], "PASS")
        self.finalize_checkpoint("c102")
        self.assertEqual(len(accept.verify_history()), 3)

    def test_duplicate_definition_in_remaining_old_unit_fails_prepare(self):
        self.accept_initial_units()
        self.write_source("src/native.c", "int func_00100000(void) { return 1; }\n")
        move = {"function": "func_00100000", "from": "src/a.c", "to": "src/native.c"}
        record = self.prepare_checkpoint("c101", [self.unit("src/native.c", "func_00100000")], [move])
        self.assertEqual(record["result"], "FAIL")
        self.assertTrue(all("defined by both" in b["error"] for b in record["builds"]))
        with self.assertRaisesRegex(accept.AcceptError, "prepare is FAIL"):
            accept.finalize("c101", 1)

    def test_conflicting_comdat_definition_in_old_unit_fails_prepare_without_changing_history(self):
        name = "_Z1fILi1EEiv"
        template = "template<int x> int f(void) { return x; }\ntemplate int f<1>(void);\n"
        retained = 'extern "C" int func_00100010(void) { return 2; }\n'
        names = {name: {"address": 0x100000}}
        self.write_source("config/names.json", json.dumps(names))
        with patch.object(image, "load_names", return_value=names):
            self.write_source("src/a.cpp", template + retained)
            initial = self.unit("src/a.cpp", "func_00100000", "func_00100010")
            self.assertEqual(self.prepare_checkpoint("c100", [initial])["result"], "PASS")
            self.finalize_checkpoint("c100")
            canonical = {path: path.read_bytes() for path in (accept.MATCHES, accept.HISTORY)}
            historical = {p.name: p.read_bytes() for p in (self.root / "accept/c100").glob("*.json")}

            self.write_source("src/a.cpp", template.replace("return x;", "return x + 3;") + retained)
            self.write_source("src/native.cpp", template)
            move = {"function": "func_00100000", "from": "src/a.cpp", "to": "src/native.cpp"}
            record = self.prepare_checkpoint("c101", [self.unit("src/native.cpp", "func_00100000")], [move])
            self.assertEqual(record["result"], "FAIL")
            self.assertEqual(len(record["builds"]), 2)
            for receipt in record["builds"]:
                self.assertEqual(receipt["result"], "FAIL")
                self.assertIn("func_00100000: defined by both", receipt["error"])

            # Reproduce the reviewer's real-compiler bypass: same COMDAT key, conflicting old
            # bytes, and a byte-exact destination. Neither credit subtraction nor coalescing
            # may hide the stale old definition.
            sections = []
            for source in ("a", "native"):
                obj = inplace.ObjectFile(self.root / f"build/c101-a/obj/src/{source}.o")
                symbol = next(sym for sym in obj.defined_functions() if sym.name == name)
                section = obj.sections[symbol.shndx]
                self.assertEqual(section.comdat, name)
                sections.append(section)
            expected = self.image.read(0x100000, 8)
            self.assertNotEqual(sections[0].data, sections[1].data)
            self.assertNotEqual(sections[0].data, expected)
            self.assertEqual(sections[1].data, expected)
            with self.assertRaisesRegex(accept.AcceptError, "prepare is FAIL"):
                accept.finalize("c101", 1)
            for path, contents in canonical.items():
                self.assertEqual(path.read_bytes(), contents)
            self.assertEqual({p.name: p.read_bytes() for p in (self.root / "accept/c100").glob("*.json")}, historical)

    def test_changed_moved_function_fails_prepare_without_changing_ledger(self):
        self.accept_initial_units()
        original = accept.sha256_file(accept.MATCHES)
        self.write_source("src/a.c", "int func_00100010(void) { return 2; }\n")
        self.write_source("src/native.c", "int func_00100000(void) { return 4; }\n")
        move = {"function": "func_00100000", "from": "src/a.c", "to": "src/native.c"}
        record = self.prepare_checkpoint("c101", [self.unit("src/native.c", "func_00100000")], [move])
        self.assertEqual(record["result"], "FAIL")
        self.assertTrue(all(not b["checks"]["credited_functions_exact"] for b in record["builds"]))
        self.assertEqual(accept.sha256_file(accept.MATCHES), original)
        self.assertEqual(len(accept.read_history()), 1)
        with self.assertRaisesRegex(accept.AcceptError, "prepare is FAIL"):
            accept.finalize("c101", 1)

    def test_empty_old_source_must_be_deleted_and_stay_deleted_through_finalize(self):
        self.accept_initial_units(split=True)
        self.write_source("src/native.c", "int func_00100000(void) { return 1; }\n")
        move = {"function": "func_00100000", "from": "src/a.c", "to": "src/native.c"}
        with self.assertRaisesRegex(accept.AcceptError, "delete the old source"):
            self.prepare_checkpoint("c101", [self.unit("src/native.c", "func_00100000")], [move])
        (self.root / "src/a.c").unlink()
        self.assertEqual(self.prepare_checkpoint(
            "c102", [self.unit("src/native.c", "func_00100000")], [move])["result"], "PASS")
        self.write_source("src/a.c", "int func_00100000(void) { return 1; }\n")
        with self.assertRaisesRegex(accept.AcceptError, "delete the old source"):
            self.finalize_checkpoint("c102")

    def test_history_cannot_silently_forget_a_move(self):
        self.accept_initial_units(split=True)
        self.write_source("src/native.c", "int func_00100000(void) { return 1; }\n")
        (self.root / "src/a.c").unlink()
        move = {"function": "func_00100000", "from": "src/a.c", "to": "src/native.c"}
        self.prepare_checkpoint("c101", [self.unit("src/native.c", "func_00100000")], [move])
        self.finalize_checkpoint("c101")
        history = accept.read_history()
        history[-1].pop("moves")
        history[-1]["entry_sha256"] = accept.entry_hash(history[-1])
        accept.HISTORY.write_text("".join(json.dumps(e) + "\n" for e in history), encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "move record"):
            accept.verify_history()


if __name__ == "__main__":
    unittest.main()
