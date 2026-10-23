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
        """Frozen pre-selector evidence, with full receipts but no selection/model fields."""
        out = self.root / "accept" / name
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        accept.write_exclusive(out / "matches.json", {"schema_version": 1, "units": units})
        builds = []
        for suffix in ("a", "b"):
            receipt = {
                "name": f"{name}-{suffix}", "result": result, "fresh": True, "inputs": {},
                "credited": {"functions": 1, "bytes": 8, "catalogue_functions": 10, "catalogue_bytes": 80},
                "outputs": {"code_bin_sha256": "x"},
            }
            accept.write_exclusive(out / "receipts" / f"{name}-{suffix}.json", receipt)
            builds.append({key: receipt[key] for key in ("name", "result", "credited", "outputs")})
        record = {
            "checkpoint": name,
            "base_matches_sha256": accept.sha256_file(accept.MATCHES),
            "ledger_sha256": accept.sha256_file(out / "matches.json"),
            "sources": accept.source_hashes(units),
            "headers": accept.header_hashes(),
            "new_units": ["src/a.c"],
            "profiles_sha256": accept.sha256_file(accept.PROFILES),
            "catalogue_sha256": accept.sha256_file(accept.CATALOGUE),
            "builds": builds,
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

    def stubbed_prepare(self, name, units, literal_models=None, literal_rebindings=(), receipt_units=None,
                        linker_model="armlink5-inline-branchnop-v2", literal_model="none", linker_models=None):
        """accept.prepare with builds, suite, interfaces and git stubbed out (exercised elsewhere)."""
        import types
        receipt = {"result": "PASS", "outputs": {"code_bin_sha256": "x"},
                   "units": [{"source": u["source"], "functions": u["functions"],
                              "linker_rewrites": {"func_00100000": [{"kind": "inline_nop"}], "func_00100004": []},
                              "literal_rebindings": list(literal_rebindings)} for u in units],
                   "credited": {"functions": 1, "bytes": 8, "catalogue_functions": 10, "catalogue_bytes": 80}}
        if receipt_units is not None:
            receipt["units"] = receipt_units
        literal_choices = iter(literal_models) if literal_models is not None else None
        linker_choices = iter(linker_models) if linker_models is not None else None
        self.build_calls = []

        def build(name, matches, **kwargs):
            # The choice must already be frozen when the first build starts.
            frozen = accept.load_json(matches.parent / "selection.json")
            self.assertEqual(frozen, {"linker_model": kwargs["linker_model"], "literal_model": kwargs["literal_model"]})
            self.build_calls.append((name, kwargs))
            return {**receipt, "name": name, "fresh": kwargs["fresh"], "inputs": {
                "linker_model": next(linker_choices) if linker_choices is not None else kwargs["linker_model"],
                "literal_model": next(literal_choices) if literal_choices is not None else kwargs["literal_model"],
            }}

        stubs = {
            "build": types.SimpleNamespace(build=build, BuildError=RuntimeError),
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
            return accept.prepare(name, cand, linker_model=linker_model, literal_model=literal_model)
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
        self.assertEqual(set(record["checks"]), {
            "build_a_pass", "build_b_pass", "builds_identical", "models_selected", "suite_pass", "interfaces_pass",
        })
        self.assertEqual(record["result"], "PASS")
        self.assertEqual(record["commit"], "0" * 40)
        on_disk = json.loads((self.root / "accept/c009/prepare.json").read_text(encoding="utf-8"))
        self.assertEqual(on_disk["checks"], record["checks"])
        # Gates v1.8: the linker model and every emulated rewrite in new units are recorded.
        self.assertEqual(record["linker_model"], "armlink5-inline-branchnop-v2")
        self.assertEqual(record["linker_rewrites"], {"func_00100000": [{"kind": "inline_nop"}]})
        self.assertEqual(record["literal_model"], "none")
        self.assertEqual(record["literal_rebindings"], {})
        self.assertEqual(accept.load_json(self.root / "accept/c009/selection.json"), {
            "linker_model": "armlink5-inline-branchnop-v2", "literal_model": "none",
        })
        self.assertEqual([call[1] for call in self.build_calls], [
            {"fresh": True, "linker_model": "armlink5-inline-branchnop-v2", "literal_model": "none"},
        ] * 2)
        accept.verify_selection(self.root / "accept/c009", record)

    def test_two_builds_must_use_the_same_literal_model(self):
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        record = self.stubbed_prepare("c011", units, ("none", "anonymous-string-records-v1"))
        self.assertEqual(record["result"], "FAIL")
        self.assertFalse(record["checks"]["builds_identical"])

    def test_prepare_records_independent_literal_model_and_rebindings(self):
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        rebindings = [{"rule": "anonymous-string-records-v1", "record_count": 3, "size": 46}]
        record = self.stubbed_prepare("c012", units, literal_rebindings=rebindings,
                                     literal_model="anonymous-string-records-v1")
        self.assertEqual(record["result"], "PASS")
        self.assertEqual(record["linker_model"], "armlink5-inline-branchnop-v2")
        self.assertEqual(record["literal_model"], "anonymous-string-records-v1")
        self.assertEqual(record["literal_rebindings"], {"src/a.c": rebindings})

    def test_finalize_preserves_the_independent_literal_model(self):
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        self.stubbed_prepare("c015", units, literal_model="anonymous-string-records-v1")
        self.review(name="c015")
        self.assertEqual(accept.finalize("c015", 1)["literal_model"], "anonymous-string-records-v1")

    def test_prepare_cli_passes_explicit_independent_choices(self):
        for model in ("armlink5-inline-branchnop-v3", "armlink5-inline-branchnop-v4"):
            with self.subTest(model=model), patch.object(accept, "prepare", return_value={
                "result": "PASS", "checks": {}, "suite": {"summary": "stub"},
            }) as prepare, patch("builtins.print"):
                self.assertEqual(accept.main([
                    "prepare", "--checkpoint", "c015", "--candidates", "config/candidates.json",
                    "--linker-model", model, "--literal-model", "anonymous-string-records-v1",
                ]), 0)
                prepare.assert_called_once_with("c015", Path("config/candidates.json"), False,
                                                model, "anonymous-string-records-v1")

    def test_explicit_selection_is_bound_in_prepare_receipts_and_history(self):
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        record = self.stubbed_prepare("c015", units, linker_model="armlink5-inline-branchnop-v3",
                                     literal_model="anonymous-string-records-v1")
        out = self.root / "accept/c015"
        self.assertEqual(record["schema_version"], 2)
        self.assertEqual(record["selection_sha256"], accept.sha256_file(out / "selection.json"))
        self.assertEqual([call[1] for call in self.build_calls], [
            {"fresh": True, "linker_model": "armlink5-inline-branchnop-v3",
             "literal_model": "anonymous-string-records-v1"},
        ] * 2)
        for suffix, summary in zip(("a", "b"), record["builds"]):
            self.assertEqual(summary["receipt_sha256"], accept.sha256_file(out / "receipts" / f"c015-{suffix}.json"))
        self.review(name="c015")
        entry = accept.finalize("c015", 1)
        for key in ("selection_sha256", "linker_model", "literal_model"):
            self.assertEqual(entry[key], record[key])
        self.assertEqual(accept.verify_history(), [entry])

    def test_two_builds_must_use_the_same_linker_model(self):
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        record = self.stubbed_prepare("c015", units, linker_models=(
            "armlink5-inline-branchnop-v2", "armlink5-inline-branchnop-v3"))
        self.assertEqual(record["result"], "FAIL")
        self.assertFalse(record["checks"]["builds_identical"])
        self.assertFalse(record["checks"]["models_selected"])
        with self.assertRaisesRegex(accept.AcceptError, "prepare is FAIL"):
            accept.finalize("c015", 1)

    def test_identical_builds_cannot_silently_substitute_another_selected_model(self):
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        record = self.stubbed_prepare("c015", units, linker_model="armlink5-inline-branchnop-v3",
                                     linker_models=("armlink5-inline-branchnop-v2",) * 2)
        self.assertEqual(record["linker_model"], "armlink5-inline-branchnop-v3")
        self.assertTrue(record["checks"]["builds_identical"])
        self.assertFalse(record["checks"]["models_selected"])
        self.assertEqual(record["result"], "FAIL")

    def test_identical_builds_cannot_silently_enable_the_literal_model(self):
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        record = self.stubbed_prepare("c015", units, literal_models=("anonymous-string-records-v1",) * 2)
        self.assertEqual(record["literal_model"], "none")
        self.assertTrue(record["checks"]["builds_identical"])
        self.assertFalse(record["checks"]["models_selected"])
        self.assertEqual(record["result"], "FAIL")

    def test_invalid_selection_is_refused_before_checkpoint_creation(self):
        for kwargs in ({"linker_model": "armlink5-inline-branchnop-v1"}, {"literal_model": "unknown"}):
            with self.assertRaisesRegex(accept.AcceptError, "unknown or retired|unknown literal"):
                accept.prepare("c015", self.root / "unused.json", **kwargs)
        self.assertFalse((self.root / "accept/c015").exists())

    def test_selection_change_after_review_blocks_finalize(self):
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        self.stubbed_prepare("c015", units, literal_model="anonymous-string-records-v1")
        self.review(name="c015")
        path = self.root / "accept/c015/selection.json"
        selection = accept.load_json(path)
        selection["literal_model"] = "none"
        path.write_text(json.dumps(selection), encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "model selection changed"):
            accept.finalize("c015", 1)

    def test_receipt_mapping_change_after_review_blocks_finalize(self):
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        self.stubbed_prepare("c015", units, literal_model="anonymous-string-records-v1",
                             literal_rebindings=[{"records": [{"candidate_offset": 0, "output_offset": 4, "size": 4}]}])
        self.review(name="c015")
        path = self.root / "accept/c015/receipts/c015-b.json"
        receipt = accept.load_json(path)
        receipt["units"][0]["literal_rebindings"] = []
        path.write_text(json.dumps(receipt), encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "build b receipt changed"):
            accept.finalize("c015", 1)

    def test_verify_history_checks_selected_prepare_and_full_receipts(self):
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        self.stubbed_prepare("c015", units, literal_model="anonymous-string-records-v1")
        self.review(name="c015")
        accept.finalize("c015", 1)
        for relative, message in (
            ("prepare.json", "prepare changed"),
            ("selection.json", "model selection changed"),
            ("receipts/c015-a.json", "build a receipt changed"),
            ("receipts/c015-b.json", "build b receipt changed"),
        ):
            with self.subTest(relative=relative):
                path = self.root / "accept/c015" / relative
                original = path.read_text(encoding="utf-8")
                path.write_text(original + "\n", encoding="utf-8")
                with self.assertRaisesRegex(accept.AcceptError, message):
                    accept.verify_history()
                path.write_text(original, encoding="utf-8")

    def test_rehashed_history_cannot_change_the_prepared_model(self):
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        self.stubbed_prepare("c015", units, literal_model="anonymous-string-records-v1")
        self.review(name="c015")
        entry = accept.finalize("c015", 1)
        entry["literal_model"] = "none"
        entry["entry_sha256"] = accept.entry_hash(entry)
        accept.HISTORY.write_text(json.dumps(entry) + "\n", encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "history models differ"):
            accept.verify_history()

    def assert_prepare_schema_downgrade_refused(self, schema_version):
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        record = self.stubbed_prepare("c015", units, linker_model="armlink5-inline-branchnop-v3",
                                     literal_model="anonymous-string-records-v1")
        self.review(name="c015")
        out = self.root / "accept/c015"
        if schema_version is None:
            record.pop("schema_version")
        else:
            record["schema_version"] = schema_version
        record.update(accept.LEGACY_MODELS)
        (out / "prepare.json").write_text(json.dumps(record), encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "selection binding"):
            self.review(name="c015", reviewer="rev2")
        self.assertFalse((out / "reviews/rev2.json").exists())
        # Keep the review bound to the exact downgraded prepare: finalize must refuse
        # the selection inconsistency itself, not merely a stale review hash.
        review_path = out / "reviews/rev1.json"
        reviewed = accept.load_json(review_path)
        reviewed["prepare_sha256"] = accept.sha256_file(out / "prepare.json")
        review_path.write_text(json.dumps(reviewed), encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "selection binding"):
            accept.finalize("c015", 1)

    def test_prepare_schema_downgrade_blocks_review_and_finalize(self):
        self.assert_prepare_schema_downgrade_refused(1)

    def test_prepare_schema_removal_blocks_review_and_finalize(self):
        self.assert_prepare_schema_downgrade_refused(None)

    def assert_history_schema_downgrade_refused(self, schema_version):
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        self.stubbed_prepare("c015", units, linker_model="armlink5-inline-branchnop-v3",
                             literal_model="anonymous-string-records-v1")
        self.review(name="c015")
        entry = accept.finalize("c015", 1)
        self.assertEqual(accept.verify_history(), [entry])
        if schema_version is None:
            entry.pop("schema_version")
        else:
            entry["schema_version"] = schema_version
        entry.pop("selection_sha256")
        entry.update(accept.LEGACY_MODELS)
        entry["entry_sha256"] = accept.entry_hash(entry)
        accept.HISTORY.write_text(json.dumps(entry) + "\n", encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "selection binding"):
            accept.verify_history()

    def test_finalized_history_schema_downgrade_cannot_remove_selection(self):
        self.assert_history_schema_downgrade_refused(1)

    def test_finalized_history_schema_removal_cannot_remove_selection(self):
        self.assert_history_schema_downgrade_refused(None)

    def test_legacy_prepare_cannot_hide_nondefault_receipt_models(self):
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        record = self.stubbed_prepare("c015", units, linker_model="armlink5-inline-branchnop-v3",
                                     literal_model="anonymous-string-records-v1")
        out = self.root / "accept/c015"
        (out / "selection.json").unlink()
        record.pop("schema_version")
        record.pop("selection_sha256")
        record.update(accept.LEGACY_MODELS)
        for summary in record["builds"]:
            summary.pop("receipt_sha256")
            summary.pop("inputs")
        (out / "prepare.json").write_text(json.dumps(record), encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "models differ"):
            self.review(name="c015")
        with self.assertRaisesRegex(accept.AcceptError, "models differ"):
            accept.finalize("c015", 1)

    def test_legacy_history_receipts_must_agree_with_recorded_models(self):
        self.review()
        accept.finalize("c001", 1)
        for suffix in ("a", "b"):
            path = self.root / f"accept/c001/receipts/c001-{suffix}.json"
            original = path.read_text(encoding="utf-8")
            for key, value in (("linker_model", "armlink5-inline-branchnop-v3"),
                               ("literal_model", "anonymous-string-records-v1")):
                with self.subTest(build=suffix, model=key):
                    receipt = json.loads(original)
                    receipt["inputs"][key] = value
                    path.write_text(json.dumps(receipt), encoding="utf-8")
                    with self.assertRaisesRegex(accept.AcceptError, "models differ"):
                        accept.verify_history()
                    path.write_text(original, encoding="utf-8")

    def make_legacy_history(self, count):
        """Construct legacy-shaped evidence, including deliberate post-cutover forgeries."""
        self.review()
        history = [accept.finalize("c001", 1)]
        for seq in range(2, count + 1):
            name = f"c{seq:03}"
            self.make_prepared(name, result="PASS")
            out = self.root / "accept" / name
            entry = {**history[-1], "seq": seq, "checkpoint": name,
                     "prepare_sha256": accept.sha256_file(out / "prepare.json"),
                     "matches_sha256": accept.sha256_file(out / "matches.json"),
                     "prev": history[-1]["entry_sha256"]}
            entry.pop("schema_version", None)
            entry["entry_sha256"] = accept.entry_hash(entry)
            accept.write_exclusive(out / "finalize.json", {
                "checkpoint": name, "entry_sha256": entry["entry_sha256"],
            })
            history.append(entry)
        accept.HISTORY.write_text("".join(json.dumps(e) + "\n" for e in history), encoding="utf-8")
        return history

    def test_sequence_7_legacy_checkpoint_verifies_before_selector_cutover(self):
        self.assertEqual(accept.SELECTION_FIRST_SEQ, 8)
        history = self.make_legacy_history(7)
        self.assertEqual(history[-1]["seq"], 7)
        self.assertNotIn("selection_sha256", history[-1])
        self.assertEqual(accept.verify_history(), history)

    def test_post_selector_history_requires_selection_without_schema_or_artifacts(self):
        self.make_legacy_history(accept.SELECTION_FIRST_SEQ)
        with self.assertRaisesRegex(accept.AcceptError, f"c{accept.SELECTION_FIRST_SEQ:03}:.*selection binding"):
            accept.verify_history()

    def test_post_selector_prepare_requires_selection_without_schema_or_artifacts(self):
        history = self.make_legacy_history(accept.SELECTION_FIRST_SEQ - 1)
        self.assertEqual(accept.verify_history(), history)
        self.make_prepared("c015", result="PASS")
        with self.assertRaisesRegex(accept.AcceptError, "selection binding"):
            self.review(name="c015")
        with self.assertRaisesRegex(accept.AcceptError, "selection binding"):
            accept.finalize("c015", 1)

    def test_model_transition_reopens_unchanged_l1_functions_and_exposes_all_mappings(self):
        self.review()
        accept.finalize("c001", 1)
        mappings = [{"rule": "anonymous-string-records-v1", "records": [
            {"candidate_offset": 0, "output_offset": 4, "size": 4},
            {"candidate_offset": 4, "output_offset": 0, "size": 4},
        ]}]
        receipt_units = [{"source": "src/a.c", "functions": ["func_00100000"], "literal_rebindings": mappings}]
        record = self.stubbed_prepare("c015", [], receipt_units=receipt_units,
                                     literal_model="anonymous-string-records-v1")
        self.assertEqual(record["revised_units"], [])
        self.assertEqual(record["model_changes"], {
            "literal_model": {"previous": "none", "selected": "anonymous-string-records-v1"},
        })
        self.assertEqual(record["literal_rebindings"], {"src/a.c": mappings})
        self.assertEqual(record["model_uses"]["src/a.c"]["literal_rebindings"], mappings)
        self.assertEqual(record["model_reviews"], {"func_00100000": {"source": "src/a.c", "rules": ["L1"]}})
        self.assertEqual(accept.new_functions(self.root / "accept/c015", record), {"func_00100000": "src/a.c"})
        empty = {"reviewer": "rev1", "family": "openai", "verdict": "PASS", "functions": [],
                 "findings": [], "qualifications": []}
        path = self.root / "empty-model-review.json"
        path.write_text(json.dumps(empty), encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "does not cover"):
            accept.review("c015", path)
        self.review("PASS_WITH_QUALIFICATIONS", name="c015", row="QUALIFIED",
                    qualifications=["L1 witnessed candidate span; original pool extent and ownership unknown"])
        self.assertEqual(accept.finalize("c015", 1)["seq"], 2)
        accept.verify_history()

    def u1_model_transition(self, previous, selected, own_unwind=False):
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]
        self.stubbed_prepare("c014", units, linker_model=previous)
        self.review(name="c014")
        accept.finalize("c014", 1)
        member = {"section": "i._ZN6WidgetD0Ev", "section_index": 2, "symbols": ["_ZN6WidgetD0Ev"],
                  "size": 12, "object": "src/a.c", "evidence": {"d0_slot_is_null": True}}
        if own_unwind:
            member["unwind_sections"] = [{
                "section": ".ARM.exidx.D0", "section_index": 3, "kind": "exidx", "size": 8, "object": "src/a.c"}]
        action = {"rule": "unused_deleting_destructor", "target": None, "removed_special_member": member}
        receipts = [{"source": "src/a.c", "functions": ["func_00100000"], "removed_special_members": [member],
                     "linker_section_rewrites": [action]}]
        record = self.stubbed_prepare("c015", [], receipt_units=receipts, linker_model=selected)
        out = self.root / "accept/c015"
        self.assertEqual(record["revised_units"], [])
        self.assertEqual(record["model_changes"], {"linker_model": {"previous": previous, "selected": selected}})
        self.assertEqual(record["model_reviews"], {"func_00100000": {"source": "src/a.c", "rules": ["U1"]}})
        self.assertEqual(record["model_uses"]["src/a.c"]["removed_special_members"], [member])
        self.assertEqual(record["linker_section_rewrites"], {"src/a.c": [action]})
        self.assertEqual(accept.new_functions(out, record), {"func_00100000": "src/a.c"})
        empty = {"reviewer": "empty", "family": "openai", "verdict": "PASS", "functions": [],
                 "findings": [], "qualifications": []}
        path = self.root / "empty-u1-review.json"
        path.write_text(json.dumps(empty), encoding="utf-8")
        with self.assertRaisesRegex(accept.AcceptError, "does not cover"):
            accept.review("c015", path)
        for suffix, summary in zip(("a", "b"), record["builds"]):
            receipt_path = out / "receipts" / f"c015-{suffix}.json"
            self.assertEqual(accept.load_json(receipt_path)["units"][0]["linker_section_rewrites"], [action])
            self.assertEqual(summary["receipt_sha256"], accept.sha256_file(receipt_path))
        self.review("PASS_WITH_QUALIFICATIONS", name="c015", row="QUALIFIED",
                    qualifications=["U1 null-slot and class spelling are bounded witnesses, not original link provenance"])
        self.assertEqual(accept.finalize("c015", 1)["linker_model"], selected)
        accept.verify_history()

    def test_v8_to_v9_transition_reopens_unchanged_u1_owner_and_binds_section_actions(self):
        self.u1_model_transition("armlink5-inline-branchnop-v8", "armlink5-inline-branchnop-v9")

    def test_v9_to_v10_transition_reopens_u1_owner_and_binds_every_unwind_companion(self):
        self.u1_model_transition("armlink5-inline-branchnop-v9", "armlink5-inline-branchnop-v10", own_unwind=True)

    def test_v10_to_v11_transition_reopens_u1_owner_and_binds_corrected_model(self):
        self.u1_model_transition("armlink5-inline-branchnop-v10", "armlink5-inline-branchnop-v11", own_unwind=True)

    def test_v11_to_v12_transition_reopens_u1_owner_and_binds_complete_metadata_model(self):
        self.u1_model_transition("armlink5-inline-branchnop-v11", "armlink5-inline-branchnop-v12", own_unwind=True)

    def test_v12_to_v13_transition_reopens_u1_owner_and_binds_structural_edge_model(self):
        self.u1_model_transition("armlink5-inline-branchnop-v12", "armlink5-inline-branchnop-v13", own_unwind=True)

    def test_v13_to_v14_transition_reopens_u1_owner_and_binds_complete_namespace_model(self):
        self.u1_model_transition("armlink5-inline-branchnop-v13", "armlink5-inline-branchnop-v14", own_unwind=True)

    def test_v14_to_v15_transition_binds_narrow_single_object_scope(self):
        self.u1_model_transition("armlink5-inline-branchnop-v14", "armlink5-inline-branchnop-v15", own_unwind=True)

    def test_u1_zero_credit_owner_remains_visible_without_inventing_credited_functions(self):
        member = {"symbols": ["_ZN6WidgetD0Ev"], "object": "src/helper.cpp", "section_index": 3}
        action = {"rule": "unused_deleting_destructor", "target": None, "removed_special_member": member}
        uses = accept.model_use_summary({"units": [{
            "source": "src/helper.cpp", "functions": [], "removed_special_members": [member],
            "linker_section_rewrites": [action]}]})
        self.assertEqual(uses["src/helper.cpp"]["u1_functions"], [])
        self.assertEqual(uses["src/helper.cpp"]["removed_special_members"], [member])
        self.assertEqual(uses["src/helper.cpp"]["linker_section_rewrites"], [action])

    def test_model_transition_reopens_unchanged_r1_callers_and_keeps_zero_credit_helpers(self):
        receipt_units, rewrite = self.removed_helper_baseline(linker_model="armlink5-inline-branchnop-v2")
        # Exercise a default-model transition rather than a helper/source edit.
        receipt_units[1]["removed_callees"] = [rewrite["removed_callee"]]
        record = self.stubbed_prepare("c015", [], receipt_units=receipt_units,
                                     linker_model="armlink5-inline-branchnop-v3")
        self.assertEqual(record["revised_units"], [])
        self.assertEqual(record["model_uses"]["src/a.c"]["r1_callers"], ["func_00100000"])
        self.assertEqual(record["model_uses"]["src/helper.c"]["removed_callees"], [rewrite["removed_callee"]])
        self.assertEqual(record["model_reviews"], {"func_00100000": {"source": "src/a.c", "rules": ["R1"]}})
        self.assertEqual(record["linker_rewrites"], {"func_00100000": [rewrite]})
        with self.assertRaisesRegex(accept.AcceptError, "identity provenance requires a QUALIFIED"):
            self.review(name="c015")

    def test_v3_to_v4_selection_binds_receipts_and_reopens_unchanged_r1_callers(self):
        receipts, rewrite = self.removed_helper_baseline()
        receipts[1]["removed_callees"] = [rewrite["removed_callee"]]
        model = "armlink5-inline-branchnop-v4"
        record = self.stubbed_prepare("c015", [], receipt_units=receipts, linker_model=model)
        out = self.root / "accept/c015"
        self.assertEqual(record["revised_units"], [])
        self.assertEqual(record["model_changes"], {
            "linker_model": {"previous": "armlink5-inline-branchnop-v3", "selected": model},
        })
        self.assertEqual(accept.load_json(out / "selection.json"), {"linker_model": model, "literal_model": "none"})
        self.assertEqual([call[1]["linker_model"] for call in self.build_calls], [model, model])
        self.assertEqual(record["model_reviews"], {"func_00100000": {"source": "src/a.c", "rules": ["R1"]}})
        self.assertEqual(record["linker_rewrites"], {"func_00100000": [rewrite]})
        for suffix, summary in zip(("a", "b"), record["builds"]):
            receipt_path = out / "receipts" / f"c015-{suffix}.json"
            self.assertEqual(accept.load_json(receipt_path)["inputs"]["linker_model"], model)
            self.assertEqual(summary["receipt_sha256"], accept.sha256_file(receipt_path))
        with self.assertRaisesRegex(accept.AcceptError, "identity provenance requires a QUALIFIED"):
            self.review(name="c015")
        self.review("PASS_WITH_QUALIFICATIONS", name="c015", row="QUALIFIED",
                    qualifications=["R1 helper identity remains inferred under the selected v4 CFG policy"])
        entry = accept.finalize("c015", 1)
        self.assertEqual(entry["linker_model"], model)
        self.assertEqual(entry["selection_sha256"], record["selection_sha256"])
        self.assertEqual(accept.verify_history()[-1], entry)

    def test_history_without_model_selection_keeps_verifying(self):
        self.review()
        entry = accept.finalize("c001", 1)
        self.assertNotIn("selection_sha256", entry)
        self.assertEqual(entry["literal_model"], "none")
        self.assertEqual(accept.verify_history(), [entry])

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

    def removed_helper_baseline(self, linker_model="armlink5-inline-branchnop-v3"):
        """An accepted caller plus a zero-credit helper, with selected synthetic receipts."""
        helper = {"source": "src/helper.c", "profile": "game", "functions": []}
        (self.root / helper["source"]).write_text(
            "void set_field18(unsigned *p, unsigned v) { p[6] = v; }\n", encoding="utf-8")
        units = [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}, helper]
        self.stubbed_prepare("c014", units, linker_model=linker_model)
        self.review(name="c014")
        accept.finalize("c014", 1)
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
        record = self.stubbed_prepare("c011", [], receipt_units=receipts, linker_model="armlink5-inline-branchnop-v3")
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
        record = self.stubbed_prepare("c012", [], receipt_units=receipts, linker_model="armlink5-inline-branchnop-v3")
        self.assertEqual(record["revised_units"], [])
        self.assertEqual(record["linker_rewrites"], {"func_00100000": [rewrite]})
        self.assertEqual(record["dependency_reviews"]["func_00100000"]["revised_headers"], ["include/game/shared.h"])
        self.assertEqual(accept.new_functions(self.root / "accept/c012", record), {"func_00100000": "src/a.c"})

    def test_unchanged_helper_does_not_reopen_unrelated_credited_callers(self):
        receipts, _ = self.removed_helper_baseline()
        record = self.stubbed_prepare("c013", [], receipt_units=receipts, linker_model="armlink5-inline-branchnop-v3")
        self.assertEqual(record["dependency_reviews"], {})
        self.assertEqual(record["linker_rewrites"], {})
        self.assertEqual(accept.new_functions(self.root / "accept/c013", record), {})

    def test_dependency_row_must_name_a_credited_caller_and_its_source(self):
        record = accept.load_json(self.root / "accept/c001/prepare.json")
        record["new_units"] = []
        record["dependency_reviews"] = {"func_00100000": {"source": "src/helper.c"}}
        with self.assertRaisesRegex(accept.AcceptError, "not a credited caller"):
            accept.new_functions(self.root / "accept/c001", record)

    def test_revision_of_any_removed_comdat_copy_reopens_its_callers(self):
        helper = {"object": "src/first.c", "comdat": "set_field18"}
        copies = [{"source": source, "functions": [], "removed_callees": [{**helper, "object": source}]}
                  for source in ("src/first.c", "src/second.c")]
        rows = [{"target": None, "removed_callee": helper}]
        receipt = {"units": [
            {"source": "src/a.c", "linker_rewrites": {"func_00100000": rows}}, *copies,
        ]}
        rewrites, dependencies = accept.linker_review_summary(receipt, {"src/second.c"}, [])
        self.assertEqual(rewrites, {"func_00100000": rows})
        self.assertEqual(dependencies["func_00100000"]["revised_helpers"], ["src/second.c"])

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

    def test_addition_preserves_existing_zero_credit_helper_units(self):
        helper = {"source": "src/helper.c", "profile": "game", "functions": []}
        addition = {"source": "src/new.c", "profile": "game", "functions": ["func_00100030"]}
        units, moves, removed = accept.compose_units(
            self.base + [helper], {"units": [addition]}, "c002")
        self.assertEqual(units, self.base + [helper, addition])
        self.assertEqual(moves, [])
        self.assertEqual(removed, [])

    def test_moves_retire_only_emptied_credited_units_not_zero_credit_helpers(self):
        helper = {"source": "src/helper.c", "profile": "game", "functions": []}
        destination = {"source": "src/native.c", "profile": "game",
                       "functions": ["func_00100000", "func_00100010"]}
        declared = [{"function": fn, "from": "src/a.c", "to": "src/native.c"}
                    for fn in self.base[0]["functions"]]
        units, moves, removed = accept.compose_units(
            self.base + [helper], {"units": [destination], "moves": declared}, "c002")
        self.assertEqual(units, [self.base[1], helper, destination])
        self.assertEqual(moves, [{**move, "checkpoint": "c002"} for move in declared])
        self.assertEqual(removed, ["src/a.c"])

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
        self.assertEqual(entry["selection_sha256"], record["selection_sha256"])
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
