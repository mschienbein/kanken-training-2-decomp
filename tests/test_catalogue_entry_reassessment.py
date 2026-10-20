"""Public producer/consumer reassessment of synthetic unadopted pointer claims."""
from __future__ import annotations

from copy import deepcopy
import hashlib
import json
from pathlib import Path
import struct
import tempfile
import unittest

from tools import analyze, catalogue_closure
from tests.test_analyze import BASE, branch, fixture
from tests.test_catalogue_protection import metadata_sha, run_catalogue, synthetic_project, write_json


def callback_image(state: int) -> tuple[bytes, object]:
    body = struct.pack("<8H", 0xB510, state, 0x4C02, 0x447C, 0xB650, 0xBD10, 0xFFFF, 0xFFFF)
    target = BASE + 16
    constant = (target + 1 - (BASE + 38 + 4)) & 0xFFFFFFFF
    return fixture([0xFA000006, 0xE12FFF1E, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x47702007, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    *struct.unpack("<4I", body), constant, 0xFFFFFFFF])


def reassessment_fixture() -> tuple[bytes, object, dict, dict, dict]:
    # The LE writer supplies a concrete provisional entry record. On the BE
    # scenario the same closed-looking callback has no valid pool-value witness;
    # restoring LE after the load does not authorize the old pointer proposal.
    le_image, layout = callback_image(0xB650)
    reference = analyze.build_catalogue(le_image, layout, closure={})
    image, _ = callback_image(0xB658)
    current = analyze.build_catalogue(image, layout, closure={})
    target = BASE + 16
    before = next(row for row in reference["functions"] if row["address"] == target)
    witness = next(row for row in reference["pc_relative_pointer_witnesses"] if row["target"] == target)
    source = next(row for row in current["functions"] if row["address"] == witness["function"])
    analyzer = analyze.Analyzer(image, layout)
    reasons = []
    for _, decoded in catalogue_closure._decoded_functions({"functions": [source]}, analyzer):
        for site, instruction in decoded.items():
            flow = analyzer.decode(site, source["isa"])
            if catalogue_closure._data_endian_effect(instruction, source["isa"]) is None:
                reasons.append({"site": site, "reason": "uncertified_processor_effect"})
            if flow is None:
                reasons.append({"site": site, "reason": "missing_flow"})
            elif flow.switch is not None or flow.indirect and not flow.call:
                reasons.append({"site": site, "reason": "unresolved_computed_transfer"})
    revision = {
        "image_sha256": current["image_sha256"],
        "prior_proposal": {"sha256": metadata_sha(reference), "status": "synthetic unadopted proposal"},
        "prior_owners": [{key: row[key] for key in ("address", "size", "isa", "kind")}
                         for row in reference["functions"]],
        "prior_entries": [{"owner": row["address"], **entry}
                          for row in reference["functions"] for entry in row["entries"]],
        "prior_unknown_ranges": [[row["start"], row["end"]] for row in reference["ranges"] if row["kind"] == "unknown"],
        "prior_anomalies": reference["anomalies"],
        "withdrawn_entry_proposals": [{
            "address": target, "prior_function": before, "prior_record_sha256": metadata_sha(before),
            "original_extent_sha256": hashlib.sha256(image[16:16 + before["size"]]).hexdigest(),
            "prior_witness": witness, "source_record_sha256": metadata_sha(source),
            "negative_certificate_reasons": reasons,
        }],
    }
    return image, layout, reference, current, revision


class CatalogueEntryReassessmentTests(unittest.TestCase):
    def test_public_builder_preserves_refusal_and_reopens_only_the_provisional_span(self) -> None:
        image, layout, reference, current, revision = reassessment_fixture()
        result = analyze.build_catalogue(image, layout, closure={"entry_revision": revision})
        self.assertEqual(result["functions"], current["functions"])
        self.assertEqual(result["ranges"], current["ranges"])
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        before_entries = {(entry["address"], entry["isa"]) for row in reference["functions"] for entry in row["entries"]}
        after_entries = {(entry["address"], entry["isa"]) for row in result["functions"] for entry in row["entries"]}
        self.assertEqual(before_entries - after_entries, {(BASE + 16, "thumb")})
        self.assertFalse(after_entries - before_entries)
        self.assertTrue(any(row["kind"] == "unknown" and row["start"] <= BASE + 16 < BASE + 20 <= row["end"]
                            for row in result["ranges"]))
        assessment = result["closure_evidence"]["provisional_entry_reassessment"]
        self.assertEqual([row["address"] for row in assessment["withdrawn_proposals"]], [BASE + 16])
        self.assertFalse(assessment["adoption"])
        self.assertEqual(assessment["new_C_credit"], 0)

    def test_public_builder_will_not_withdraw_a_proved_or_protected_entry(self) -> None:
        image, layout, reference, _, revision = reassessment_fixture()
        callback = next(row for row in reference["functions"] if row["address"] == BASE + 16)
        with self.assertRaises(ValueError):
            analyze.build_catalogue(image, layout, protected_functions={BASE + 16: callback},
                                     closure={"entry_revision": revision})
        le_image, _ = callback_image(0xB650)
        revision["image_sha256"] = hashlib.sha256(le_image).hexdigest()
        with self.assertRaises(ValueError):
            analyze.build_catalogue(le_image, layout, closure={"entry_revision": revision})

    def test_public_consumer_refuses_missing_negative_reasons_or_unrelated_delta(self) -> None:
        image, layout, _, current, revision = reassessment_fixture()
        for change in ("missing_veto", "wrong_reasons", "wrong_source_record", "rodata_instead_of_unknown",
                       "drop_unrelated_entry", "change_other_extent"):
            with self.subTest(change=change):
                candidate, evidence = deepcopy(current), deepcopy(revision)
                if change == "missing_veto":
                    candidate["closure_evidence"]["read_vetoes"] = []
                elif change == "wrong_reasons":
                    evidence["withdrawn_entry_proposals"][0]["negative_certificate_reasons"].append(
                        {"site": BASE, "reason": "unproved"})
                elif change == "wrong_source_record":
                    evidence["withdrawn_entry_proposals"][0]["source_record_sha256"] = "0" * 64
                elif change == "rodata_instead_of_unknown":
                    row = next(row for row in candidate["ranges"] if row["start"] <= BASE + 16 < row["end"])
                    row["kind"] = "rodata"
                elif change == "drop_unrelated_entry":
                    evidence["prior_entries"] = [row for row in evidence["prior_entries"] if row["address"] != BASE]
                else:
                    candidate["functions"][0]["size"] += 4
                with self.assertRaises(ValueError):
                    catalogue_closure.check_entry_revision(candidate, analyze.Analyzer(image, layout),
                                                           {"entry_revision": evidence}, {}, [])

    def test_public_builder_preserves_existing_nonordinary_topology(self) -> None:
        image, layout = fixture([branch(BASE, BASE + 16, link=True), branch(BASE + 4, BASE + 20, link=True),
                                 0xE12FFF1E, 0, 0xE320F000, 0xE3A00001, 0xE12FFF1E])
        before = analyze.build_catalogue(image, layout, closure={})
        body = next(row for row in before["functions"] if row["address"] == BASE + 16)
        self.assertEqual(body["kind"], "multi_entry")
        revision = {
            "image_sha256": before["image_sha256"], "prior_proposal": {"sha256": metadata_sha(before)},
            "prior_owners": [{key: row[key] for key in ("address", "size", "isa", "kind")}
                             for row in before["functions"]],
            "prior_entries": [{"owner": row["address"], **entry}
                              for row in before["functions"] for entry in row["entries"]],
            "prior_unknown_ranges": [[row["start"], row["end"]] for row in before["ranges"] if row["kind"] == "unknown"],
            "prior_anomalies": before["anomalies"], "withdrawn_entry_proposals": [],
        }
        record = {"address": body["address"], "size": body["size"], "isa": body["isa"],
                  "image_sha256": before["image_sha256"], "kind": "runtime", "basis": "compiler_abi",
                  "identity": "synthetic", "evidence": "synthetic classification, not native evidence",
                  "compiler_emission": "synthetic topology-preservation control"}
        result = analyze.build_catalogue(image, layout, classifications=[record], closure={"entry_revision": revision})
        actual = next(row for row in result["functions"] if row["address"] == body["address"])
        self.assertEqual(actual["kind"], "multi_entry")
        self.assertEqual(actual["entries"], body["entries"])
        self.assertEqual(actual["classification_evidence"], record)

    def test_real_cli_requires_the_new_hash_bound_successor(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            synthetic_project(root)
            passed = run_catalogue(root)
            self.assertEqual(passed.returncode, 0, passed.stderr)
            candidate = json.loads((root / "proposal.json").read_bytes())
            self.assertEqual(candidate["generator"]["revision"], "catalogue-v13")
            self.assertIn("research/catalogue_v11_r3_evidence.json", candidate["generator"]["input_sha256"])
            self.assertIn("research/catalogue_v12_evidence.json", candidate["generator"]["input_sha256"])
            self.assertIn("research/catalogue_v12_object_proof.json", candidate["generator"]["input_sha256"])
            path = root / "research/catalogue_v11_r3_evidence.json"
            revision = json.loads(path.read_bytes())
            revision["parent_evidence"]["sha256"] = "0" * 64
            write_json(path, revision)
            refused = run_catalogue(root, "refused.json")
            self.assertEqual(refused.returncode, 1)
            self.assertIn("v11-r3 entry evidence differs", refused.stderr)
            self.assertFalse((root / "refused.json").exists())

    def test_real_cli_refuses_unbound_leaf_repair_policy(self) -> None:
        for change in ("parent", "policy"):
            with self.subTest(change=change), tempfile.TemporaryDirectory() as temporary:
                root = Path(temporary)
                synthetic_project(root)
                passed = run_catalogue(root)
                self.assertEqual(passed.returncode, 0, passed.stderr)
                path = root / "research/catalogue_v11_r4_evidence.json"
                revision = json.loads(path.read_bytes())
                if change == "parent":
                    revision["parent_evidence"]["sha256"] = "0" * 64
                else:
                    revision["policy"] = "permit_user_bank_transfers"
                write_json(path, revision)
                refused = run_catalogue(root, "refused.json")
                self.assertEqual(refused.returncode, 1)
                self.assertIn("v11-r4 leaf evidence differs", refused.stderr)
                self.assertFalse((root / "refused.json").exists())


if __name__ == "__main__":
    unittest.main()
