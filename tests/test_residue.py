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
            "rows": {self.names[6]: {"function": self.names[6], "state": "plateau"}}})
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
            "function": self.names[15], "status": "adapter_refused"})
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
                return (commits[args[-1].rsplit("/", 1)[-1]] + "\n").encode()
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
            "rows": {self.names[5]: {"function": self.names[5], "state": "exhausted"}}})
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

    def test_structured_and_documented_user_dependencies_are_not_contract_holds(self) -> None:
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
        for index in (5, 6):
            self.assertEqual(by_name[self.names[index]]["status_class"], "awaits_user_ruling")
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

    def test_missing_required_metadata_refuses_instead_of_silent_empty_residue(self) -> None:
        (self.root / "scratch/integrator/census-20261021-r3/LADDER.json").unlink()
        with self.assertRaisesRegex(ValueError, "required metadata input missing"):
            self.report()


if __name__ == "__main__":
    unittest.main()
