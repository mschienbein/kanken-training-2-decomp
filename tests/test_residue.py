"""Metadata-only residue joining: no ROM, compiler, matcher, or full build needed."""
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import residue  # noqa: E402


class ResidueTest(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        self.names = [f"func_{0x100000 + index * 0x100:08x}" for index in range(16)]
        self.put("config/catalogue.json", {"functions": [
            {"address": 0x100000 + i * 0x100, "size": 32,
             "kind": "library" if i == 1 else "function"}
            for i in range(16)]})
        self.put("config/matches.json", {"units": [{"functions": [self.names[0]]}]})
        self.put("config/compiler_profiles.json", {"profiles": {"game": {}, "sdk": {}, "middleware": {}}})
        self.put("scratch/integrator/census-20261021-r3/LADDER.json", {"rows": []})
        self.put("scratch/integrator/census-20261021-r3/REPORT.md", "metadata census\n")
        self.put("scratch/integrator/escalation/RESIDUE_READY.json", {})
        self.put("scratch/integrator/escalation/QUEUES.json", {})
        self.put("scratch/permute/sweep-10/summary.json", {
            "policy": {"operational_differing_byte_cap": 32}, "rows": {}})

    def put(self, path: str, value: object) -> None:
        target = self.root / path
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_text(value if isinstance(value, str) else json.dumps(value), encoding="utf-8")

    def author(self, batch: str, rows: list[dict], targets: list[str] | None = None) -> None:
        self.put(f"scratch/integrator/batches/{batch}.json", {
            "functions": [{"function": name} for name in
                          (targets if targets is not None else [residue.identity(row) for row in rows])]})
        self.put(f"scratch/authors/{batch}/RESULT.json", {"functions": rows})

    def row(self, index: int, count: int = 10, **extra: object) -> dict:
        return {"address": f"0x{0x100000 + index * 0x100:08x}", "size": 32,
                "status": "mismatch", "profile": "game",
                "residual": {"target_size": 32, "differing_bytes": count}, **extra}

    def census(self, indices: list[int], steps: list[str]) -> None:
        self.put("scratch/integrator/census-20261021-r3/LADDER.json", {
            "denominator": {"revision": "v9-r2"},
            "rows": [{"function": self.names[index], "target_bytes": 32,
                      "completed_steps": steps, "completion_evidence": {}}
                     for index in indices]})

    def supplement(self, delivered: dict | None = None) -> dict:
        batch_path = "scratch/integrator/batches/fa90.json"
        result_path = "scratch/authors/fa90/RESULT.json"
        self.put(batch_path, {"created_by": "Main", "step": "1",
                              "functions": [{"function": self.names[9]}]})
        self.put(result_path, {"functions": [delivered or self.row(9, 6)]})
        doc = {"input_sha256": {
            path: residue.digest((self.root / path).read_bytes())
            for path in (batch_path, result_path)},
            "rows": [{"function": self.names[9], "target_bytes": 32,
                      "completion_evidence": {"1": [{"batch": "fa90", "result_row": 0}]}}]}
        self.put("scratch/integrator/census-supplements/S01.json", doc)
        return doc

    def report(self) -> dict:
        return residue.generate(self.root, pending=[])

    def test_identity_and_selected_residual_normalization(self) -> None:
        name = self.names[3]
        for row in ({"name": name.upper().replace("FUNC", "func")},
                    {"function": name}, {"address": "00100300"},
                    {"name": "NativeOwner", "address": 0x100300}):
            self.assertEqual(residue.identity(row), name)
        self.assertIsNone(residue.identity("mentions func_00100300 in prose"))
        self.assertEqual(residue.residual({"residual": {
            "differing_bytes_including_size_delta": 7, "differing_bytes": 3}})[:2],
            (7, "residual.differing_bytes_including_size_delta"))
        self.assertEqual(residue.residual({"residual": 8})[:2], (8, "residual"))
        self.assertEqual(residue.residual({"differing_bytes": 5})[:2], (5, "differing_bytes"))
        self.assertEqual(residue.residual({"residual_before": 0, "residual_after": {
            "differing_bytes": 9}})[0], 9)
        self.assertEqual(residue.residual({"residual": "unmeasured", "differing_bytes": None})[0], None)
        self.assertIsNone(residue.residual({"residual": True})[0])

    def test_partition_classes_and_actionable_missing_rungs(self) -> None:
        exact = self.row(3, 0, status="match", integration_needs=["native receiver contract"])
        header = self.row(4, 7, header_blocked=True, header_issues=["canonical header declaration conflict"])
        intake = self.row(5, 0, status="match")
        self.author("nm01", [exact, header, intake])
        for batch in ("r301", "r4001", "tu001", "nf001", "np001", "nm02"):
            self.author(batch, [self.row(6, 12), self.row(7, 80)])
        self.census([6, 7], ["initial", "2", "3"])
        self.put("scratch/permute/sweep-10/summary.json", {
            "policy": {"operational_differing_byte_cap": 32},
            "rows": {self.names[6]: {"function": self.names[6], "size": 32, "state": "plateau"}}})
        self.put("scratch/integrator/c010/contract-deferrals.json", [
            {"function": self.names[8], "reason": "awaits user ruling on unresolved policy"}])
        report = self.report()
        by_name = {row["function"]: row for row in report["rows"]}
        self.assertNotIn(self.names[0], by_name)  # accepted
        self.assertNotIn(self.names[1], by_name)  # non-creditable
        self.assertEqual(by_name[self.names[3]]["status_class"], "held_contract")
        self.assertEqual(by_name[self.names[4]]["status_class"], "header_blocked")
        self.assertEqual(by_name[self.names[5]]["status_class"], "pending_intake")
        self.assertEqual(by_name[self.names[6]]["status_class"], "near_miss_exhausted")
        self.assertEqual(by_name[self.names[7]]["status_class"], "never_matched_exhausted")
        self.assertTrue(by_name[self.names[7]]["permuter"]["boundary_not_search"])
        self.assertFalse(by_name[self.names[7]]["permuter"]["search_completed"])
        self.assertEqual(by_name[self.names[8]]["status_class"], "awaits_user_ruling")
        self.assertEqual(by_name[self.names[9]]["status_class"], "ladder_incomplete")
        self.assertNotIn("5c", by_name[self.names[9]]["missing_steps"])
        self.assertIn("4", by_name[self.names[9]]["missing_steps"])
        self.assertEqual(report["denominator"]["functions"], 15)
        self.assertEqual(report["accepted"]["functions"] + report["pending_checkpoint"]["functions"] +
                         report["residue"]["functions"], 15)
        self.assertEqual(sum(value["bytes"] for value in report["summary_by_class"].values()),
                         report["residue"]["bytes"])
        actionable = {row["function"]: row for row in report["incomplete_ladder"]}
        self.assertEqual(actionable[self.names[5]]["actionable_items"], ["checkpoint_intake"])
        self.assertNotIn(self.names[3], actionable)
        self.assertNotIn(self.names[6], actionable)
        self.assertIn(self.names[4], actionable)  # header blockers do not waive missing rungs

    def test_assignment_and_context_rows_do_not_complete_target_rungs(self) -> None:
        self.put("scratch/integrator/batches/nm03.json", {"functions": [{"name": self.names[9]}]})
        self.author("nm04", [self.row(10, 0, status="context_match"), self.row(11, 3)],
                    targets=[self.names[11]])
        self.author("np004", [self.row(12, 0, status="context_match")])
        by_name = {row["function"]: row for row in self.report()["rows"]}
        self.assertEqual(by_name[self.names[9]]["ladder"]["5c"]["assigned_batches"], ["nm03"])
        self.assertFalse(by_name[self.names[9]]["ladder"]["5c"]["completed"])
        self.assertFalse(by_name[self.names[10]]["ladder"]["5c"]["completed"])
        self.assertEqual(by_name[self.names[10]]["best"]["residual_bytes"], 0)
        self.assertEqual(by_name[self.names[10]]["actionable_items"], ["context_match_contract_review"])
        self.assertEqual(by_name[self.names[10]]["status_class"], "ladder_incomplete")
        self.assertFalse(by_name[self.names[12]]["ladder"]["5b"]["completed"])
        self.assertTrue(by_name[self.names[11]]["ladder"]["5c"]["completed"])

    def test_stale_extent_cannot_win_best_or_complete_ladder(self) -> None:
        self.author("np005", [self.row(13, 0, size=28, status="match")])
        self.author("nf005", [self.row(13, 5)])
        report = self.report()
        row = next(row for row in report["rows"] if row["function"] == self.names[13])
        self.assertEqual(row["best"]["residual_bytes"], 5)
        self.assertFalse(row["ladder"]["5b"]["completed"])
        self.assertTrue(row["stale_extent_evidence"])
        self.assertNotEqual(row["status_class"], "pending_intake")

    def test_inside_bound_improvement_reopens_old_operational_boundary(self) -> None:
        self.author("nm05", [self.row(14, 9)])
        self.put("scratch/permute/sweep-10/summary.json", {
            "policy": {"operational_differing_byte_cap": 32},
            "rows": {self.names[14]: {"function": self.names[14],
                "state": "not_applicable_operational_boundary",
                "stop_reason": "source_bound_residual_above_32_differing_bytes"}}})
        row = next(row for row in self.report()["rows"] if row["function"] == self.names[14])
        self.assertEqual(row["permuter"]["state"], "reopened_inside_bound")
        self.assertFalse(row["permuter"]["boundary_not_search"])
        self.assertIn("4", row["missing_steps"])

    def test_adapter_refusal_is_a_boundary_not_completed_search(self) -> None:
        self.author("nm06", [self.row(15, 6)])
        self.put("scratch/permute/step4-nm/refusal.json", {
            "function": self.names[15], "size": 32, "status": "adapter_refused"})
        row = next(row for row in self.report()["rows"] if row["function"] == self.names[15])
        self.assertEqual(row["permuter"]["state"], "adapter_refused")
        self.assertTrue(row["permuter"]["boundary_not_search"])
        self.assertFalse(row["permuter"]["search_completed"])
        self.assertNotIn("4", row["missing_steps"])

    def test_harvest_noted_contract_conflicts_override_pending_label(self) -> None:
        self.author("nm07", [self.row(5, 0, status="match")])
        self.put("scratch/integrator/HARVEST-LIST.json", [
            f"nm07:{self.names[5]}(pending; receiver vs canonical callback contract)"])
        row = next(row for row in self.report()["rows"] if row["function"] == self.names[5])
        self.assertEqual(row["status_class"], "held_contract")
        self.assertTrue(row["harvest"][0]["noted_contract_conflict"])
        self.assertIn("harvest_contract_conflict", [h["basis"] for h in row["holds"]])

    def test_old_adoption_requests_do_not_block_new_unheld_match(self) -> None:
        self.author("nf003", [self.row(5, 0, status="match", needs_headers=["canonical header needed"])])
        self.author("nm03", [self.row(5, 0, status="match")])
        row = next(row for row in self.report()["rows"] if row["function"] == self.names[5])
        self.assertEqual(row["status_class"], "pending_intake")
        self.assertEqual(row["best"]["batch"], "nm03")
        self.assertFalse(row["holds"])
        self.assertTrue(row["historical_integration_needs"])

    def test_scoped_top_level_needs_and_named_deferral_lists_are_joined(self) -> None:
        self.author("nm03", [self.row(5, 0, status="match"), self.row(6, 0, status="match")])
        self.put("scratch/authors/nm03/RESULT.json", {
            "functions": [self.row(5, 0, status="match"), self.row(6, 0, status="match")],
            "integration_needs": {"headers": {self.names[5]: ["native receiver contract"]}}})
        self.put("scratch/integrator/c010/HANDOFF.json", {
            "c009_owned_deferrals_unchanged": [self.names[6]]})
        by_name = {row["function"]: row for row in self.report()["rows"]}
        self.assertEqual(by_name[self.names[5]]["status_class"], "held_contract")
        self.assertEqual(by_name[self.names[6]]["status_class"], "held_contract")

    def test_early_rung_completion_uses_census_not_batch_prefixes(self) -> None:
        for batch in ("r301", "r4001", "tu001", "w01"):
            self.author(batch, [self.row(5, 8)])
        self.author("rl283", [self.row(5, 8)])
        self.put("scratch/integrator/batches/rl283.json", {
            "step": "2", "functions": [{"function": self.names[5]}]})
        row = next(row for row in self.report()["rows"] if row["function"] == self.names[5])
        self.assertFalse(any(row["ladder"][step]["completed"] for step in ("1", "2", "3")))
        self.assertEqual(row["ladder"]["2"]["assigned_batches"], ["rl283"])
        self.census([5], ["initial", "2", "3"])
        row = next(row for row in self.report()["rows"] if row["function"] == self.names[5])
        self.assertTrue(all(row["ladder"][step]["completed"] for step in ("1", "2", "3")))
        self.assertEqual(row["census"]["completed_steps"], ["initial", "2", "3"])
        self.assertTrue(row["ladder"]["1"]["evidence"])

    def test_verified_census_supplements_add_rungs_and_unverifiable_ones_refuse(self) -> None:
        name = self.names[9]
        supplement = "scratch/integrator/census-supplements/S01.json"
        self.put("scratch/integrator/batches/fa90.json", {
            "step": "1", "created_by": "Main", "functions": [{"function": name}]})
        self.put("scratch/authors/fa90/RESULT.json", {"functions": [
            self.row(10, 0, status="context_match"), self.row(9, 6)]})
        unwitnessed = next(row for row in self.report()["rows"] if row["function"] == name)
        self.assertFalse(unwitnessed["ladder"]["1"]["completed"])

        def witness(step: str = "1", row: int = 1, size: int = 32) -> None:
            paths = ("scratch/integrator/batches/fa90.json", "scratch/authors/fa90/RESULT.json")
            self.put(supplement, {"input_sha256": {
                path: residue.digest((self.root / path).read_bytes()) for path in paths},
                "rows": [{"function": name, "target_bytes": size,
                          "completion_evidence": {step: [{"batch": "fa90", "result_row": row}]}}]})

        witness()
        witnessed = next(row for row in self.report()["rows"] if row["function"] == name)
        self.assertTrue(witnessed["ladder"]["1"]["completed"])
        self.assertTrue(any(supplement in item for item in witnessed["ladder"]["1"]["evidence"]))
        for bad in ({"step": "2"}, {"row": 0}, {"row": 5}, {"step": "4"}, {"size": 64}):
            witness(**bad)
            with self.assertRaises(ValueError, msg=str(bad)):
                self.report()
        self.put("scratch/integrator/batches/fa90.json", {"step": "1", "functions": [{"function": name}]})
        witness()
        with self.assertRaises(ValueError):
            self.report()

    def test_resolved_extension_ruling_is_not_an_open_user_question(self) -> None:
        self.put("docs/rulings/2026-10-17-armcc-extensions.md", "Documented extensions are approved.\n")
        self.put("scratch/integrator/c009/contract-deferrals.json", [
            {"function": self.names[8], "reason": "awaiting user ruling on __svc and CP15 extensions"}])
        row = next(row for row in self.report()["rows"] if row["function"] == self.names[8])
        self.assertNotEqual(row["status_class"], "awaits_user_ruling")
        self.assertTrue(row["holds"][0]["resolved_policy_question_by"])

    def test_pending_branches_use_git_show_pinned_commits_and_c012_fallback(self) -> None:
        manifest_path = "config/candidates/c012.json"
        commits = {"c012-r4": "a" * 40, "c012": "b" * 40}
        queried = []
        def git(_inputs: residue.Inputs, *args: str) -> bytes:
            queried.append(args)
            if args[0] == "rev-parse":
                branch = args[-1].rsplit("/", 1)[-1]
                if branch not in commits:
                    raise ValueError("branch absent")
                return (commits[branch] + "\n").encode()
            if args[0] == "ls-tree":
                if args[-1] == "config/candidates" and args[4] == "b" * 40:
                    return (manifest_path + "\0").encode()
                return b""
            if args[0] == "show":
                return json.dumps({"units": [{"functions": [self.names[0], self.names[2]]}]}).encode()
            self.fail(f"unexpected metadata query {args!r}")
        with patch.object(residue.Inputs, "git", git):
            report = residue.generate(self.root, pending=[(("c012-r4", "c012"), manifest_path)])
        self.assertEqual(report["pending_checkpoint"]["functions"], 1)
        self.assertEqual(report["pending_checkpoint"]["rows"][0]["function"], self.names[2])
        self.assertEqual(report["pending_checkpoint"]["rows"][0]["status"], "pending checkpoint")
        self.assertEqual(report["pending_checkpoint"]["sources"][0]["fallback_from"], ["c012-r4"])
        self.assertIn(("show", "b" * 40 + ":" + manifest_path), queried)
        self.assertNotIn(self.names[2], [row["function"] for row in report["rows"]])
        self.assertEqual(report["pending_checkpoint"]["sources"][0]["commit"], "b" * 40)

    def test_determinism_hashes_and_text_allowlist(self) -> None:
        secret = "PRIVATE_TEXT_MUST_NOT_LEAK"
        self.author("nm08", [self.row(5, 6, notes=f"{secret} register allocation and scheduling")])
        first = self.report()
        second = self.report()
        self.assertEqual(residue.canonical(first), residue.canonical(second))
        self.assertEqual(residue.markdown(first), residue.markdown(second))
        self.assertNotIn(secret, residue.canonical(first).decode())
        self.assertNotIn(str(self.root), residue.canonical(first).decode())
        self.assertIn("register_role", next(row for row in first["rows"] if row["function"] == self.names[5])["diagnosis_families"])
        self.assertEqual(first["input_manifest_sha256"], residue.digest(residue.canonical(first["input_hashes"])))
        self.assertEqual(first["input_hashes"]["scratch/authors/nm08/RESULT.json"],
                         residue.digest((self.root / "scratch/authors/nm08/RESULT.json").read_bytes()))
        self.put("scratch/integrator/batches/nm09.json", {"functions": [{"function": self.names[5]}]})
        self.assertNotEqual(first["input_manifest_sha256"], self.report()["input_manifest_sha256"])

    def test_extra_passes_are_not_exhaustion_prerequisites(self) -> None:
        self.census([5, 6], ["initial", "2", "3"])
        self.author("w01", [self.row(5, 12), self.row(6, 80)])
        self.put("scratch/permute/sweep-10/summary.json", {
            "policy": {"operational_differing_byte_cap": 32},
            "rows": {self.names[5]: {"function": self.names[5], "size": 32, "state": "exhausted"}}})
        by_name = {row["function"]: row for row in self.report()["rows"]}
        self.assertEqual(by_name[self.names[5]]["status_class"], "near_miss_exhausted")
        self.assertEqual(by_name[self.names[6]]["status_class"], "never_matched_exhausted")
        for index in (5, 6):
            row = by_name[self.names[index]]
            self.assertEqual(row["missing_steps"], [])
            self.assertTrue(all(not row["ladder"][step]["required"] for step in ("5", "5b", "5c")))
            self.assertTrue(all(not row["ladder"][step]["completed"] for step in ("5", "5b", "5c")))
            self.assertTrue(row["ladder"]["4"]["completed"])
        self.assertEqual(by_name[self.names[5]]["permuter"]["disposition"], "completed")
        self.assertEqual(by_name[self.names[6]]["permuter"]["disposition"], "not_applicable")

    def test_later_sweeps_complete_step4_only_when_searched_and_share_the_bound(self) -> None:
        self.census([7, 8], ["initial", "2", "3"])
        self.author("w02", [self.row(7, 12), self.row(8, 12)])
        self.put("scratch/permute/sweep-11/summary.json", {
            "policy": {"operational_differing_byte_cap": 32},
            "rows": {self.names[7]: {"function": self.names[7], "size": 32, "state": "plateau"},
                     self.names[8]: {"function": self.names[8], "size": 32, "state": "pending"}}})
        by_name = {row["function"]: row for row in self.report()["rows"]}
        searched, pending = by_name[self.names[7]], by_name[self.names[8]]
        self.assertTrue(searched["ladder"]["4"]["completed"])
        self.assertTrue(any("sweep-11" in item for item in searched["ladder"]["4"]["evidence"]))
        self.assertEqual(searched["status_class"], "near_miss_exhausted")
        self.assertFalse(pending["ladder"]["4"]["completed"])
        self.assertEqual(pending["status_class"], "ladder_incomplete")
        self.put("scratch/permute/sweep-11/summary.json", {
            "policy": {"operational_differing_byte_cap": 16}, "rows": {}})
        with self.assertRaises(ValueError):
            self.report()

    def test_census_identity_reconciliation_preserves_mapping_gaps(self) -> None:
        self.put("scratch/integrator/census-20261021-r3/LADDER.json", {
            "rows": [
                {"function": self.names[1], "target_bytes": 32, "completed_steps": ["initial"]},
                {"function": self.names[5], "target_bytes": 28, "completed_steps": ["initial", "2", "3"]},
                {"function": self.names[6], "target_bytes": 32, "completed_steps": ["initial", "2", "3"]}]})
        report = self.report()
        mapping = report["census_reconciliation"]
        self.assertEqual(mapping["exact_identity_matches"], 1)
        self.assertEqual(mapping["census_identities_no_longer_creditable"][0]["current_kind"], "library")
        self.assertEqual(mapping["extent_changed_requires_mapping_review"][0]["function"], self.names[5])
        self.assertIn(self.names[7], [r["function"] for r in mapping["current_identities_missing_from_census"]])
        by_name = {row["function"]: row for row in report["rows"]}
        self.assertTrue(all(by_name[self.names[6]]["ladder"][step]["completed"] for step in ("1", "2", "3")))
        self.assertFalse(any(by_name[self.names[5]]["ladder"][step]["completed"] for step in ("1", "2", "3")))
        self.assertFalse(by_name[self.names[5]]["census"]["extent_agrees"])

    def test_user_rulings_and_documented_resources_have_distinct_dispositions(self) -> None:
        self.put("scratch/integrator/escalation/LEDGER.json", {"functions": [
            {"function": self.names[5], "size": 32, "blockers": [
                {"category": "user-ruling", "code": "thumb_identity_standard",
                 "detail": "Standalone runtime identity needs a ruling."}]},
            {"function": self.names[1], "size": 32, "blockers": [
                {"category": "user-ruling", "detail": "Already classified non-C."}]}]})
        self.put("docs/RESUME.md",
                 f"{self.names[6]} needs the user-provided ARMCC 4.1 b791 runtime library.\n")
        report = self.report()
        by_name = {row["function"]: row for row in report["rows"]}
        self.assertEqual(by_name[self.names[5]]["status_class"], "awaits_user_ruling")
        self.assertEqual(by_name[self.names[6]]["status_class"], "ladder_incomplete")
        self.assertFalse(any(h["awaits_user_ruling"] for h in by_name[self.names[6]]["holds"]))
        for index in (5, 6):
            self.assertIn("user_prerequisite", by_name[self.names[index]]["blocker_classes"])
        self.assertIn("docs/RESUME.md#L1-L1", by_name[self.names[6]]["evidence"])
        self.assertNotIn(self.names[1], by_name)
        self.assertIn("docs/RESUME.md", report["input_hashes"])

    def test_residual_bands_account_for_unknown_and_pending_exact_bodies(self) -> None:
        self.assertEqual([residue.residual_band(v) for v in (0, 1, 16, 17, 64, 65, 256, 257, None)],
                         ["0 (held)", "1-16", "1-16", "17-64", "17-64",
                          "65-256", "65-256", ">256", "unknown"])
        self.author("nm01", [self.row(5, 0, status="match"), self.row(6, 16),
                              self.row(7, 64), self.row(8, 256), self.row(9, 257)])
        report = self.report()
        self.assertEqual(sum(v["functions"] for v in report["summary_by_residual_band"].values()),
                         report["residue"]["functions"])
        for value in report["summary_by_class"].values():
            self.assertEqual(sum(v["bytes"] for v in value["residual_bands"].values()), value["bytes"])
        self.assertEqual(report["summary_by_residual_band"]["0 (held)"]["functions"], 1)
        self.assertIn("Residual bands", residue.markdown(report))

    def test_revision_two_output_preserves_history_and_refuses_overwrite(self) -> None:
        report = self.report()
        directory = self.root / "scratch/integrator/residue"
        self.put("scratch/integrator/residue/RESIDUE-r1.json", "immutable r1\n")
        with patch.object(residue, "generate", return_value=report), patch("builtins.print"):
            self.assertEqual(residue.main(["--root", str(self.root)]), 0)
            self.assertEqual(residue.main(["--root", str(self.root)]), 0)
        self.assertEqual((directory / "RESIDUE-r1.json").read_text(), "immutable r1\n")
        expected = (directory / "RESIDUE-r2.json").read_bytes()
        self.assertIn("RESIDUE-r2.json", (directory / "RESIDUE-r2.md").read_text())
        with patch.object(residue, "generate", return_value={**report, "evidence_scope": "changed"}):
            with self.assertRaises(SystemExit):
                residue.main(["--root", str(self.root)])
        self.assertEqual((directory / "RESIDUE-r2.json").read_bytes(), expected)

    def test_annotated_harvest_prefixes_preserve_holds_without_copying_notes(self) -> None:
        self.author("nm07", [self.row(5, 0, status="match")])
        for annotation in ("(pending; PRIVATE_HANDOFF) -> folded into c015; receiver contract held",
                           "(pending; receiver contract held; PRIVATE_HANDOFF -> folded into c015)"):
            with self.subTest(annotation=annotation):
                self.put("scratch/integrator/HARVEST-LIST.json", [f"nm07:{self.names[5]}{annotation}"])
                report = self.report()
                row = next(r for r in report["rows"] if r["function"] == self.names[5])
                self.assertEqual(row["harvest"][0]["state"], "pending")
                self.assertEqual(row["status_class"], "held_contract")
                self.assertNotIn("PRIVATE_HANDOFF", residue.canonical(report).decode())
        self.put("scratch/integrator/HARVEST-LIST.json", ["nm07:not_a_function(pending) -> c015"])
        with self.assertRaises(ValueError):
            self.report()

    def test_default_pending_uses_current_config_and_manifest_withdrawals(self) -> None:
        self.author("nm01", [self.row(5, 0, status="match"), self.row(6, 0, status="match"),
                             self.row(7, 0, status="match")])
        config = {"manifests": [
            {"branches": ["c012-r6", "c012-r5", "c012"], "path": "config/candidates/c012.json"},
            {"branches": ["c015"], "path": "config/candidates/c015.json"}],
            "hold_manifests": ["config/candidates/c011.json"]}
        self.put("scratch/integrator/residue/PENDING-MANIFESTS.json", config)
        self.put("config/candidates/c011.json", {
            "holds": [{"function": self.names[7], "reason": "native receiver contract held"}]})
        branches = ("c012-r6", "c012-r5", "c012", "c015", "c011-r8",
                    "c012-r4", "types-compose-r3", "cmodels2-r5", "main")
        commits = {branch: f"{index + 1:040x}" for index, branch in enumerate(branches)}
        queried = []
        def git(_inputs: residue.Inputs, *args: str) -> bytes:
            queried.append(args)
            if args[0] == "rev-parse":
                return (commits[args[-1].removeprefix("refs/heads/")] + "\n").encode()
            if args[0] == "ls-tree":
                return (("\0".join("config/candidates/" + name + ".json" for name in
                                  ("c011", "c012", "c015", "ctypes1", "cmodels2")) + "\0").encode()
                        if args[-1] == "config/candidates" else b"")
            if args[0] == "show":
                commit, path = args[1].split(":", 1)
                if commit == commits["c012-r6"]:
                    doc = {"units": [{"functions": [self.names[2], self.names[5]]}],
                           "holds": [{"function": self.names[5], "reason": "native receiver contract held"}]}
                elif commit == commits["c015"]:
                    doc = {"units": [{"functions": [self.names[3]]}],
                           "holds": [{"function": self.names[6], "reason": "canonical header contract held"}]}
                else:
                    doc = {"units": [{"functions": self.names[5:8]}]}
                return json.dumps(doc).encode()
            self.fail(f"unexpected metadata query {args!r}")
        with patch.object(residue.Inputs, "git", git):
            report = residue.generate(self.root)
            self.assertEqual({r["function"] for r in report["pending_checkpoint"]["rows"]},
                             {self.names[2], self.names[3]})
            by_name = {r["function"]: r for r in report["rows"]}
            for index in (5, 6, 7):
                self.assertEqual(by_name[self.names[index]]["status_class"], "held_contract")
            self.assertEqual([r["branch"] for r in report["pending_checkpoint"]["sources"]],
                             ["c012-r6", "c015"])
            self.assertIn("scratch/integrator/residue/PENDING-MANIFESTS.json", report["input_hashes"])
            config["manifests"] = config["manifests"][1:]
            self.put("scratch/integrator/residue/PENDING-MANIFESTS.json", config)
            queried.clear()
            edited = residue.generate(self.root)
        self.assertEqual({r["function"] for r in edited["pending_checkpoint"]["rows"]}, {self.names[3]})
        self.assertFalse(any("c012" in arg for query in queried for arg in query))

    def test_current_c012_fallback_and_finalized_main_snapshot(self) -> None:
        path = "config/candidates/c012.json"
        self.put("scratch/integrator/residue/PENDING-MANIFESTS.json", {"manifests": [
            {"branches": ["c012-r6", "c012-r5", "c012"], "path": path}]})
        self.author("nm01", [self.row(5, 0, status="match")])
        main_commit, branch_commit, prepared_commit = ("a" * 40, "b" * 40, "c" * 40)
        finalized_doc = {"units": [{"functions": [self.names[2]]}],
                         "holds": [{"function": self.names[5], "reason": "native contract held"}]}
        finalized_raw = json.dumps(finalized_doc).encode()
        finalized = False
        queried = []
        def git(_inputs: residue.Inputs, *args: str) -> bytes:
            queried.append(args)
            if args[0] == "rev-parse":
                branch = args[-1].removeprefix("refs/heads/")
                if branch == "main":
                    return (main_commit + "\n").encode()
                if branch == "c012-r5":
                    return (branch_commit + "\n").encode()
                raise ValueError("branch absent")
            if args[0] == "ls-tree":
                if args[-1] == "accept" and finalized:
                    return b"accept/c012-r5/finalize.json\0accept/c012-r6/prepare.json\0accept/c012-r6/finalize.json\0"
                return (path + "\0").encode() if args[-1] == "config/candidates" else b""
            if args[0] == "show":
                commit, source = args[1].split(":", 1)
                if source.endswith("/finalize.json"):
                    return json.dumps({"checkpoint": "c012-r6"}).encode()
                if source.endswith("/prepare.json"):
                    return json.dumps({"commit": prepared_commit, "candidates": path,
                                       "candidates_sha256": residue.digest(finalized_raw)}).encode()
                if commit == prepared_commit:
                    return finalized_raw
                return json.dumps({"units": [{"functions": [self.names[5]]}]}).encode()
            self.fail(f"unexpected metadata query {args!r}")
        with patch.object(residue.Inputs, "git", git):
            fallback = residue.generate(self.root)
            self.assertEqual(fallback["pending_checkpoint"]["sources"][0]["branch"], "c012-r5")
            self.assertEqual(fallback["pending_checkpoint"]["sources"][0]["fallback_from"], ["c012-r6"])
            finalized = True
            report = residue.generate(self.root)
        self.assertEqual({r["function"] for r in report["pending_checkpoint"]["rows"]}, {self.names[2]})
        self.assertEqual(next(r for r in report["rows"] if r["function"] == self.names[5])["status_class"],
                         "held_contract")
        self.assertTrue(any(key.endswith("accept/c012-r6/finalize.json") for key in report["input_hashes"]))
        self.assertIn(("show", f"{prepared_commit}:{path}"), queried)

    def test_scoped_list_needs_do_not_apply_to_unrelated_siblings(self) -> None:
        self.author("nm03", [self.row(i, 0, status="match") for i in (5, 6, 7)])
        self.put("scratch/authors/nm03/RESULT.json", {
            "functions": [self.row(i, 0, status="match") for i in (5, 6, 7)],
            "integration_needs": {
                "headers": [{"functions": [self.names[5]], "summary": "native receiver contract unresolved"}],
                "contract_conflicts": [{"target": self.names[6], "summary": "canonical callback contract conflict"}]}})
        by_name = {r["function"]: r for r in self.report()["rows"]}
        self.assertEqual(by_name[self.names[5]]["status_class"], "held_contract")
        self.assertEqual(by_name[self.names[6]]["status_class"], "held_contract")
        self.assertEqual(by_name[self.names[7]]["status_class"], "pending_intake")
        self.assertNotIn("callback_signature", by_name[self.names[5]]["blocker_classes"])

    def test_nm41_grouped_names_preserves_scoped_headers_and_contract_conflicts(self) -> None:
        rows = [self.row(index, 0, status="match") for index in (5, 6, 7)]
        self.author("nm41", rows)
        headers = [
            {"target": self.names[5], "suggested_header": "include/game/cleanup.h",
             "need": "Reconcile the native resource owner and canonical header declarations"},
            {"target": self.names[5], "scope": "Inherited complete TU",
             "need": "Remove inherited local header declarations after contract reconciliation"},
            {"target": self.names[6], "suggested_header": "include/game/service_state.h",
             "need": "Promote this target's canonical header declarations"}]
        conflicts = [
            {"target": self.names[5], "symbols": ["AppletCallbacks"],
             "existing_header": "include/game/service_callbacks.h",
             "need": "PRIVATE_NM41_GROUPED_NEED reconcile canonical callback signatures"},
            {"target": self.names[5], "symbols": ["ObjectPool"],
             "existing_header": "include/game/object_pool.h",
             "need": "Resolve native owner type identity before integration"},
            {"target": self.names[5], "symbols": ["PlaybackNode", "SoundInstance"],
             "existing_header": "include/game/sound_runtime.h",
             "need": "Reconcile the native receiver and sound owner contract"},
            {"target": self.names[5], "symbols": ["func_00124874"],
             "need": "Select the unique companion owner before granting credit"}]
        for names_group in ([], ["CanonicalSymbol"], [{"symbol": "CanonicalSymbol"}]):
            with self.subTest(names_group=names_group):
                doc = {"functions": rows, "integration_needs": {
                    "headers": headers, "names": names_group, "contract_conflicts": conflicts,
                    "resolved_canonical_adaptations": [
                        {"target": self.names[7], "change": "Canonical native owner contract already resolved"}],
                    "catalogue_issues": [],
                    "unmatched_targets": "Unrelated sibling needs no new header or contract"}}
                self.put("scratch/authors/nm41/RESULT.json", doc)
                self.assertCountEqual(residue.scoped_needs(doc, rows[0], self.names[5]),
                                      [*headers[:2], *conflicts])
                report = self.report()
                by_name = {row["function"]: row for row in report["rows"]}
                self.assertEqual(by_name[self.names[5]]["status_class"], "held_contract")
                self.assertIn("callback_signature", by_name[self.names[5]]["blocker_classes"])
                self.assertEqual(by_name[self.names[6]]["status_class"], "held_contract")
                self.assertNotIn("callback_signature", by_name[self.names[6]]["blocker_classes"])
                self.assertEqual(by_name[self.names[7]]["status_class"], "pending_intake")
                self.assertNotIn("PRIVATE_NM41_GROUPED_NEED", residue.canonical(report).decode())

    def test_need_ids_resolve_unscoped_records_but_respect_explicit_scope(self) -> None:
        self.author("np212", [self.row(i, 0, status="match") for i in (5, 6, 7)])
        self.put("scratch/authors/np212/RESULT.json", {
            "functions": [self.row(5, 0, status="match", integration_needs=["I05", "I06"]),
                          self.row(6, 0, status="match"),
                          self.row(7, 0, status="match", integration_need_ids=["I07"])],
            "integration_needs": [
                {"id": "I05", "functions": [self.names[5]], "summary": "native owner contract unresolved"},
                {"id": "I06", "target": self.names[6], "summary": "callback signature conflict"},
                {"id": "I07", "summary": "canonical header declaration conflict"}]})
        by_name = {r["function"]: r for r in self.report()["rows"]}
        for index in (5, 6, 7):
            self.assertEqual(by_name[self.names[index]]["status_class"], "held_contract")
        self.assertNotIn("callback_signature", by_name[self.names[5]]["blocker_classes"])
        self.assertIn("canonical_header", by_name[self.names[7]]["blocker_classes"])

    def test_supplement_rejects_stale_selected_or_declared_extents(self) -> None:
        for extra in ({"target_bytes": 28}, {"target_size": 28}, {"measured_target_size": 28},
                      {"residual": {"target_size": 28, "differing_bytes": 6}},
                      {"residual": {"target_size": 32, "measured_target_size": 28, "differing_bytes": 6}}):
            with self.subTest(extra=extra):
                self.supplement(self.row(9, 6, **extra))
                with self.assertRaisesRegex(ValueError, "extent"):
                    self.report()

    def test_supplement_refuses_changed_or_missing_input_hash_bindings(self) -> None:
        label = "scratch/integrator/census-supplements/S01.json"
        for source in ("scratch/integrator/batches/fa90.json", "scratch/authors/fa90/RESULT.json"):
            for change in ("changed_bytes", "wrong_hash", "missing_binding"):
                with self.subTest(source=source, change=change):
                    doc = self.supplement()
                    if change == "changed_bytes":
                        current = json.loads((self.root / source).read_bytes())
                        self.put(source, {**current, "notes": "different metadata with unchanged identity"})
                    elif change == "wrong_hash":
                        doc["input_sha256"][source] = "0" * 64
                        self.put(label, doc)
                    else:
                        del doc["input_sha256"][source]
                        self.put(label, doc)
                    with self.assertRaisesRegex(ValueError, "hash|binding"):
                        self.report()

    def test_all_sweeps_must_use_the_frozen_32_byte_cap(self) -> None:
        self.census([9], ["initial", "2", "3"])
        self.author("w01", [self.row(9, 20)])
        for cap in (16, 64, 0, None, True):
            with self.subTest(cap=cap):
                for sweep in ("sweep-10", "sweep-11"):
                    self.put(f"scratch/permute/{sweep}/summary.json", {
                        "policy": {"operational_differing_byte_cap": cap}, "rows": {}})
                with self.assertRaises(ValueError):
                    self.report()

    def test_stale_sweep_extents_cannot_disposition_step4(self) -> None:
        self.census([9], ["initial", "2", "3"])
        self.author("w01", [self.row(9, 6)])
        for state in ("plateau", "exhausted", "match", "adapter_refused"):
            for extent in ({"size": 28}, {"size": 32, "final_functions": [self.row(9, 6, size=28)]},
                           {"size": 32, "applicability": {"residual_evidence": {"target_size": 28}}},
                           {}):
                with self.subTest(state=state, extent=extent):
                    self.put("scratch/permute/sweep-10/summary.json", {
                        "policy": {"operational_differing_byte_cap": 32},
                        "rows": {self.names[9]: {"function": self.names[9], "state": state, **extent}}})
                    row = next(r for r in self.report()["rows"] if r["function"] == self.names[9])
                    self.assertIn("4", row["missing_steps"])
                    self.assertFalse(row["permuter"]["search_completed"])
                    self.assertIsNone(row["permuter"]["disposition"])
                    self.assertTrue(row["stale_extent_evidence"])

    def test_adapter_plans_are_not_explicit_refusals(self) -> None:
        self.census([9], ["initial", "2", "3"])
        self.author("w01", [self.row(9, 6)])
        for status in ("pending", "planned", "awaiting_adapter", "refused"):
            with self.subTest(status=status):
                self.put("scratch/permute/step4-nm/outcome.json", {
                    "function": self.names[9], "size": 32, "status": status,
                    "notes": "PRIVATE_ADAPTER_NOTE adapter preflight planned; no refusal attempt"})
                report = self.report()
                row = next(r for r in report["rows"] if r["function"] == self.names[9])
                self.assertEqual(row["status_class"], "ladder_incomplete")
                self.assertIn("4", row["missing_steps"])
                self.assertNotIn("PRIVATE_ADAPTER_NOTE", residue.canonical(report).decode())

    def test_explicit_adapter_refusals_require_current_extent(self) -> None:
        self.author("w01", [self.row(9, 6)])
        for extent in ({}, {"size": 28}, {"size": 32, "measured_target_size": 28}, {"size": 32}):
            with self.subTest(extent=extent):
                self.put("scratch/permute/step4-nm/outcome.json", {
                    "function": self.names[9], "status": "adapter_refused", **extent})
                row = next(r for r in self.report()["rows"] if r["function"] == self.names[9])
                self.assertEqual(row["permuter"]["disposition"],
                                 "adapter_refused" if extent == {"size": 32} else None)

    def test_user_input_categories_do_not_exempt_missing_ladder_rungs(self) -> None:
        self.put("scratch/integrator/escalation/LEDGER.json", {"functions": [
            {"function": self.names[5], "size": 32, "blockers": [
                {"category": "user-input", "detail": "User-supplied runtime archive missing"}]},
            {"function": self.names[6], "size": 32, "blockers": [
                {"category": "user-prerequisite", "detail": "User-supplied runtime archive missing"}]},
            {"function": self.names[7], "size": 32, "awaits_user_input": True}]})
        by_name = {r["function"]: r for r in self.report()["rows"]}
        for index in (5, 6, 7):
            row = by_name[self.names[index]]
            self.assertEqual(row["status_class"], "ladder_incomplete")
            self.assertEqual(row["missing_steps"], list(residue.REQUIRED_STEPS))
            self.assertFalse(any(h["awaits_user_ruling"] for h in row["holds"]))
            self.assertIn("user_prerequisite", row["blocker_classes"])

    def test_dedicated_user_ruling_holds_are_optional_and_extent_scoped(self) -> None:
        self.assertFalse(next(r for r in self.report()["rows"] if r["function"] == self.names[5])["holds"])
        label = "scratch/integrator/USER-RULING-HOLDS.json"
        self.put(label, {"holds": [
            {"function": self.names[5], "size": 32, "awaits_user_ruling": True,
             "reason": "No frozen Thumb profile; user ruling required", "evidence": ["scratch/authors/fs01/RESULT.json"]},
            {"function": self.names[6], "size": 28, "awaits_user_ruling": True,
             "reason": "Stale extent must not create a current exemption"}]})
        report = self.report()
        by_name = {r["function"]: r for r in report["rows"]}
        self.assertEqual(by_name[self.names[5]]["status_class"], "awaits_user_ruling")
        self.assertIn(label + "#/holds/0", by_name[self.names[5]]["evidence"])
        self.assertNotEqual(by_name[self.names[6]]["status_class"], "awaits_user_ruling")
        self.assertIn(label, report["input_hashes"])

    def test_concurrent_draft_creation_never_overwrites_or_leaves_a_mixed_pair(self) -> None:
        report = self.report()
        directory = self.root / "scratch/integrator/residue"
        original_open = Path.open
        original_link = residue.os.link
        for revision, suffix in ((3, "json"), (4, "md")):
            target = directory / f"RESIDUE-r{revision}.{suffix}"
            marker = b"another immutable metadata draft\n"
            inserted = False
            def insert_competitor() -> None:
                nonlocal inserted
                if not inserted:
                    inserted = True
                    with original_open(target, "xb") as handle:
                        handle.write(marker)
            def interleave(path: Path, mode: str = "r", *args: object, **kwargs: object):
                nonlocal inserted
                if path == target and mode in ("wb", "xb") and not inserted:
                    insert_competitor()
                return original_open(path, mode, *args, **kwargs)
            def interleave_link(source: Path, destination: Path, *args: object, **kwargs: object):
                if destination == target:
                    insert_competitor()
                return original_link(source, destination, *args, **kwargs)
            with self.subTest(suffix=suffix), patch.object(residue, "generate", return_value=report), \
                    patch.object(Path, "open", interleave), patch.object(residue.os, "link", interleave_link), \
                    patch("builtins.print"):
                with self.assertRaises(SystemExit) as caught:
                    residue.main(["--root", str(self.root), "--revision", str(revision)])
                self.assertEqual(caught.exception.code, 1)
            self.assertTrue(inserted)
            self.assertEqual(target.read_bytes(), marker)
            other = directory / f"RESIDUE-r{revision}.{'md' if suffix == 'json' else 'json'}"
            self.assertFalse(other.exists())

    def test_idempotent_competitor_cannot_lose_a_successful_draft_pair(self) -> None:
        report = self.report()
        directory = self.root / "scratch/integrator/residue"
        destination = directory / "RESIDUE-r9.json"
        destination_md = destination.with_suffix(".md")
        argv = ["--root", str(self.root), "--revision", "9"]
        original_open, original_link = Path.open, residue.os.link
        interleaved = False
        competitor = {}
        def compete() -> None:
            nonlocal interleaved
            if interleaved:
                return
            interleaved = True
            try:
                competitor["exit"] = residue.main(argv)
            except SystemExit as exc:
                competitor["exit"] = exc.code
            competitor["complete_pair"] = destination.exists() and destination_md.exists()
        def interleave_open(path: Path, mode: str = "r", *args: object, **kwargs: object):
            if path == destination_md and mode == "xb":
                compete()
            return original_open(path, mode, *args, **kwargs)
        def interleave_link(source: Path, target: Path, *args: object, **kwargs: object):
            if target == destination_md:
                compete()
            return original_link(source, target, *args, **kwargs)
        with patch.object(residue, "generate", return_value=report), patch("builtins.print"):
            with patch.object(Path, "open", interleave_open), \
                    patch.object(residue.os, "link", interleave_link):
                try:
                    outer_exit = residue.main(argv)
                except SystemExit as exc:
                    outer_exit = exc.code
            self.assertTrue(interleaved)
            self.assertIn(0, (outer_exit, competitor["exit"]))
            if competitor["exit"] == 0:
                self.assertTrue(competitor["complete_pair"])
            self.assertEqual(destination.read_bytes(), residue.canonical(report))
            self.assertEqual(destination_md.read_bytes(), residue.markdown(report).encode())
            self.assertEqual(residue.main(argv), 0)  # Completed pairs remain idempotently reusable.

    def test_draft_publication_stages_both_complete_files_before_exposing_either(self) -> None:
        report = self.report()
        directory = self.root / "scratch/integrator/residue"
        expected = {"RESIDUE-r9.json": residue.canonical(report),
                    "RESIDUE-r9.md": residue.markdown(report).encode()}
        original_link = residue.os.link
        published = []
        def inspect_link(source: Path, destination: Path, *args: object, **kwargs: object):
            source, destination = Path(source), Path(destination)
            for filename, data in expected.items():
                self.assertEqual((source.parent / filename).read_bytes(), data)
            if not published:
                self.assertTrue(all(not (directory / filename).exists() for filename in expected))
            result = original_link(source, destination, *args, **kwargs)
            published.append(destination.name)
            return result
        with patch.object(residue, "generate", return_value=report), patch("builtins.print"), \
                patch.object(residue.os, "link", inspect_link):
            self.assertEqual(residue.main(["--root", str(self.root), "--revision", "9"]), 0)
        self.assertCountEqual(published, expected)
        self.assertFalse(any(path.is_dir() for path in directory.iterdir()))

    def test_failed_publication_never_removes_competitor_replacements(self) -> None:
        report = self.report()
        directory = self.root / "scratch/integrator/residue"
        destination = directory / "RESIDUE-r9.json"
        destination_md = destination.with_suffix(".md")
        original_open, original_link = Path.open, residue.os.link
        replaced = False
        marker = b"competing immutable draft\n"
        def replace_pair() -> None:
            nonlocal replaced
            if replaced:
                return
            replaced = True
            destination.unlink()
            for path in (destination, destination_md):
                with original_open(path, "xb") as handle:
                    handle.write(marker)
        def interleave_open(path: Path, mode: str = "r", *args: object, **kwargs: object):
            if path == destination_md and mode == "xb":
                replace_pair()
            return original_open(path, mode, *args, **kwargs)
        def interleave_link(source: Path, target: Path, *args: object, **kwargs: object):
            if target == destination_md:
                replace_pair()
            return original_link(source, target, *args, **kwargs)
        with patch.object(residue, "generate", return_value=report), patch("builtins.print"), \
                patch.object(Path, "open", interleave_open), \
                patch.object(residue.os, "link", interleave_link):
            with self.assertRaises(SystemExit) as caught:
                residue.main(["--root", str(self.root), "--revision", "9"])
        self.assertEqual(caught.exception.code, 1)
        self.assertTrue(replaced)
        self.assertEqual(destination.read_bytes(), marker)
        self.assertEqual(destination_md.read_bytes(), marker)

    def test_missing_required_metadata_refuses_instead_of_silent_empty_residue(self) -> None:
        (self.root / "scratch/integrator/census-20261021-r3/LADDER.json").unlink()
        with self.assertRaisesRegex(ValueError, "required metadata input missing"):
            self.report()


if __name__ == "__main__":
    unittest.main()
