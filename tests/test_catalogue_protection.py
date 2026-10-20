"""Exact metadata handoff through public builders/CLI; synthetic inputs only."""
from __future__ import annotations

from copy import deepcopy
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest

from tools import analyze
from tests.test_analyze import BASE, ROOT, fixture, freeze_local_fixture, library_revision_fixture, literal_words


def metadata_sha(value: dict | list) -> str:
    return hashlib.sha256(analyze.serialize_catalogue(value)).hexdigest()


def synthetic_inputs() -> tuple[bytes, object, dict, dict, list[dict], dict]:
    # The trailing independently loaded pool pins the unchanged function extent.
    # Without the typed witness the byte-string reader owns only the first word.
    text = "ABC\0".encode("utf-16le")
    image, layout = fixture([0xE28F0004, 0xE59F1008, 0xE12FFF1E,
                             *literal_words(text), 0x12345678, 0, 0,
                             0xE92D4010, 0xE8BD8010])
    before = analyze.build_catalogue(image, layout)
    literals = [{"function": BASE, "target": BASE + 12, "end": BASE + 20,
                 "references": [BASE], "encoding": "utf-16le",
                 "image_interval_sha256": hashlib.sha256(text).hexdigest(),
                 "source_record_equal": True, "evidence": "synthetic typed inline object"}]
    after = analyze.build_catalogue(image, layout, literal_encodings=literals)
    old, new = before["functions"][0], after["functions"][0]
    transition = {"address": BASE, "before": old, "after": new,
                  "before_sha256": metadata_sha(old), "after_sha256": metadata_sha(new),
                  "original_extent_sha256": hashlib.sha256(image[:old["size"]]).hexdigest(),
                  "literal_witnesses_sha256": metadata_sha(literals),
                  "source": {"path": "src/first.c", "sha256": hashlib.sha256(b"synthetic source\n").hexdigest()},
                  "retained_compatibility": {
                      "status": "match", "size": old["size"],
                      "before_outcome_sha256": "a" * 64, "after_outcome_sha256": "a" * 64,
                      "linker_model": "armlink5-inline-branchnop-v6",
                      "literal_model": "anonymous-string-records-v1"}}
    return image, layout, before, after, literals, transition


def write_json(path: Path, value: dict) -> None:
    path.write_bytes(analyze.serialize_catalogue(value))


def descriptor(root: Path, path: str) -> dict:
    content = (root / path).read_bytes()
    return {"path": path, "sha256": hashlib.sha256(content).hexdigest(), "size": len(content)}


def synthetic_project(root: Path) -> dict:
    """Prepare a tiny real CLI project, including an accepted and prepared-only claim."""
    image, layout, before, after, literals, transition = synthetic_inputs()
    for directory in ("tools", "config", "orig", "research", "src", "accept/c009-r2"):
        (root / directory).mkdir(parents=True, exist_ok=True)
    for name in ("analyze.py", "catalogue_closure.py", "catalogue_review.py", "catalogue_runtime_data.py",
                 "catalogue_local_entries.py", "ctrfmt.py"):
        shutil.copyfile(ROOT / "tools" / name, root / "tools" / name)
    subprocess.run(["git", "init", "-q", str(root)], capture_output=True, check=True)
    write_json(root / "config/catalogue.json", before)
    unit = {"source": "src/first.c", "profile": "game", "functions": [f"func_{BASE:08x}"]}
    prepared_only = {"source": "src/second.c", "profile": "game", "functions": [f"func_{BASE + 32:08x}"]}
    write_json(root / "config/matches.json", {"units": [unit]})
    write_json(root / "config/names.json", {"symbols": {}})
    (root / "src/first.c").write_bytes(b"synthetic source\n")
    (root / "src/second.c").write_bytes(b"synthetic prepared-only source\n")
    write_json(root / "accept/c009-r2/matches.json", {"units": [unit, prepared_only]})
    write_json(root / "accept/c009-r2/prepare.json", {"fixture": "synthetic, not acceptance"})
    write_json(root / "accept/c009-r2/selection.json", {"fixture": "synthetic, not model authorization"})
    blob = subprocess.run(["git", "hash-object", "-w", "config/catalogue.json"], cwd=root,
                          capture_output=True, text=True, check=True).stdout.strip()
    evidence = {"revision": "synthetic-v9-r2", "image_sha256": before["image_sha256"],
                "classification_records": [], "thumb_dispositions": [], "author_dispositions": [],
                "thumb_v9_residue": [], "provenance": {"proof_status": "synthetic"},
                "source_candidate_sha256": "synthetic", "source_input_sha256": {}}
    parent = root / "research/catalogue_v9_r2_evidence.json"
    write_json(parent, evidence)
    closure = {"image_sha256": before["image_sha256"], "literal_encodings": literals,
               "provenance": "synthetic", "closed_graph_owners": [],
               "parent_evidence": descriptor(root, "research/catalogue_v9_r2_evidence.json"),
               "baseline_catalogue": {"blob": blob, "reference": "synthetic", "revision": "synthetic-adopted",
                                      "sha256": hashlib.sha256((root / "config/catalogue.json").read_bytes()).hexdigest()}}
    closure_path = root / "research/catalogue_v10_evidence.json"
    write_json(closure_path, closure)
    library_revision_fixture(root, closure_path, before["image_sha256"])
    identities = []
    for old, new, owner in zip(before["functions"], after["functions"], (unit, prepared_only)):
        identities.append({"address": old["address"], "owner": old["address"], "name": owner["functions"][0],
                           "size": old["size"], "isa": old["isa"], "entries": old["entries"],
                           "source": owner["source"], "profile": owner["profile"],
                           "previous_metadata_changed": old != new, "adopted_record_sha256": metadata_sha(old),
                           "proposal_record_sha256": metadata_sha(new)})
    snapshot = {"identity_count": len(identities), "identities": identities,
                "adopted_catalogue": descriptor(root, "config/catalogue.json"),
                "ledger": descriptor(root, "accept/c009-r2/matches.json"),
                "names": descriptor(root, "config/names.json"),
                "prepare": descriptor(root, "accept/c009-r2/prepare.json"),
                "selection": descriptor(root, "accept/c009-r2/selection.json")}
    snapshot_path = root / "research/catalogue_v11_protection.json"
    write_json(snapshot_path, snapshot)
    runtime_path = root / "research/catalogue_v11_evidence.json"
    runtime = json.loads(runtime_path.read_bytes())
    runtime["prepared_protection"] = descriptor(root, "research/catalogue_v11_protection.json")
    write_json(runtime_path, runtime)
    successor_path = root / "research/catalogue_v11_r2_evidence.json"
    successor = json.loads(successor_path.read_bytes())
    successor.update(parent_evidence=descriptor(root, "research/catalogue_v11_evidence.json"),
                     prepared_protection=runtime["prepared_protection"], protected_record_transitions=[transition])
    write_json(successor_path, successor)
    entry_path = root / "research/catalogue_v11_r3_evidence.json"
    entry_revision = json.loads(entry_path.read_bytes())
    entry_revision["parent_evidence"] = descriptor(root, "research/catalogue_v11_r2_evidence.json")
    # Like the real r4 reference, this prior proposal already contains the typed
    # metadata repair, while the adopted catalogue intentionally still does not.
    entry_revision.update(
        prior_owners=[{key: row[key] for key in ("address", "size", "isa", "kind")} for row in after["functions"]],
        prior_entries=[{"owner": row["address"], **entry} for row in after["functions"] for entry in row["entries"]],
        prior_unknown_ranges=[[row["start"], row["end"]] for row in after["ranges"] if row["kind"] == "unknown"],
        prior_anomalies=after["anomalies"])
    write_json(entry_path, entry_revision)
    leaf_path = root / "research/catalogue_v11_r4_evidence.json"
    leaf_repair = json.loads(leaf_path.read_bytes())
    leaf_repair["parent_evidence"] = descriptor(root, "research/catalogue_v11_r3_evidence.json")
    write_json(leaf_path, leaf_repair)
    (root / "orig/code.bin").write_bytes(image)
    exheader = bytearray(0x200)
    for offset, segment in zip((0x10, 0x20, 0x30), (layout.text, layout.rodata, layout.data)):
        struct.pack_into("<III", exheader, offset, segment.address, 1, segment.size)
    (root / "orig/exheader.bin").write_bytes(exheader)
    freeze_local_fixture(root, image, layout, protected={row["address"]: row for row in before["functions"]})
    return {"before": before, "after": after, "successor": successor, "snapshot": snapshot}


def run_catalogue(root: Path, output: str = "proposal.json", *extra: str) -> subprocess.CompletedProcess:
    return subprocess.run([sys.executable, "-B", "tools/analyze.py", "catalogue", "--out", output, *extra],
                          cwd=root, capture_output=True, text=True, check=False)


class CatalogueProtectionTests(unittest.TestCase):
    def test_public_builder_derives_exact_successor_without_mutating_adopted_records(self) -> None:
        image, layout, before, after, literals, transition = synthetic_inputs()
        protected = {row["address"]: deepcopy(row) for row in before["functions"]}
        original = deepcopy(protected)
        with self.assertRaisesRegex(ValueError, "cannot retain its adopted catalogue record"):
            analyze.build_catalogue(image, layout, protected_functions=protected, literal_encodings=literals)
        result = analyze.build_catalogue(image, layout, protected_functions=protected, literal_encodings=literals,
                                         closure={"protection_revision": {"image_sha256": before["image_sha256"],
                                                                         "protected_record_transitions": [transition]}})
        self.assertEqual(result["functions"], after["functions"])
        self.assertEqual(protected, original)
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertEqual(result["functions"][0]["size"], before["functions"][0]["size"])
        self.assertEqual(result["functions"][0]["entries"], before["functions"][0]["entries"])
        self.assertEqual(result["closure_evidence"]["protected_metadata_transitions"][0]["after_sha256"],
                         metadata_sha(after["functions"][0]))

    def test_public_builder_refuses_nonliteral_complete_record_transitions(self) -> None:
        image, layout, before, _, literals, transition = synthetic_inputs()
        for field, value in (("size", 28), ("isa", "thumb"), ("kind", "runtime"), ("entries", []),
                             ("pools", []), ("code_ranges", []), ("noreturn", True),
                             ("classification_evidence", {"identity": "invented"})):
            changed = deepcopy(transition)
            changed["after"][field] = value
            changed["after_sha256"] = metadata_sha(changed["after"])
            with self.subTest(field=field), self.assertRaises(ValueError):
                analyze.build_catalogue(image, layout, protected_functions={BASE: before["functions"][0]},
                                         literal_encodings=literals, closure={"protection_revision": {
                                             "image_sha256": before["image_sha256"],
                                             "protected_record_transitions": [changed]}})

    def test_public_builder_rejects_arbitrary_literal_records_and_changed_witnesses(self) -> None:
        image, layout, before, _, literals, transition = synthetic_inputs()
        for change in ("invented_literal", "extent_hash", "witness_hash", "duplicate", "compatibility", "references"):
            changed, witnesses = deepcopy(transition), deepcopy(literals)
            rows = [changed]
            if change == "invented_literal":
                changed["after"]["literals"] = [[BASE + 12, BASE + 24]]
                changed["after_sha256"] = metadata_sha(changed["after"])
            elif change == "extent_hash":
                changed["original_extent_sha256"] = "0" * 64
            elif change == "witness_hash":
                changed["literal_witnesses_sha256"] = "0" * 64
            elif change == "duplicate":
                rows.append(changed)
            elif change == "compatibility":
                changed["retained_compatibility"]["after_outcome_sha256"] = "0" * 64
            else:
                witnesses[0]["references"] = [BASE + 4]
                changed["literal_witnesses_sha256"] = metadata_sha(witnesses)
            with self.subTest(change=change), self.assertRaises(ValueError):
                analyze.build_catalogue(image, layout, protected_functions={BASE: before["functions"][0]},
                                         literal_encodings=witnesses, closure={"protection_revision": {
                                             "image_sha256": before["image_sha256"], "protected_record_transitions": rows}})

    def test_public_builder_refuses_a_different_image_for_the_transition(self) -> None:
        image, layout, before, _, literals, transition = synthetic_inputs()
        payload = bytearray(image)
        payload[20] ^= 1
        with self.assertRaisesRegex(ValueError, "belongs to another image"):
            analyze.build_catalogue(bytes(payload), layout, protected_functions={BASE: before["functions"][0]},
                                     literal_encodings=literals, closure={"protection_revision": {
                                         "image_sha256": before["image_sha256"],
                                         "protected_record_transitions": [transition]}})

    def test_public_builder_retains_complete_guards_for_other_protected_owners(self) -> None:
        image, layout, before, _, literals, transition = synthetic_inputs()
        protected = {row["address"]: deepcopy(row) for row in before["functions"]}
        protected[BASE + 32]["evidence"] = ["unapproved"]
        with self.assertRaisesRegex(ValueError, "cannot retain its adopted catalogue record"):
            analyze.build_catalogue(image, layout, protected_functions=protected, literal_encodings=literals,
                                     closure={"protection_revision": {"image_sha256": before["image_sha256"],
                                                                     "protected_record_transitions": [transition]}})

    def test_real_cli_protects_the_current_ledger_and_prepared_only_identity(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            project = synthetic_project(root)
            result = run_catalogue(root)
            self.assertEqual(result.returncode, 0, result.stderr)
            candidate = json.loads((root / "proposal.json").read_bytes())
            self.assertEqual(candidate["functions"], project["after"]["functions"])
            self.assertEqual({row["address"] for row in candidate["revision_review"]["protected_rows"]},
                             {BASE, BASE + 32})
            self.assertIn("accept/c009-r2/matches.json", candidate["revision_review"]["protected_summary"])
            self.assertIn("research/catalogue_v11_r2_evidence.json", candidate["generator"]["input_sha256"])
            self.assertEqual(len(candidate["revision_review"]["protected_metadata_transitions"]), 1)
            self.assertEqual((root / "config/catalogue.json").read_bytes(), analyze.serialize_catalogue(project["before"]))

    def test_real_cli_fails_closed_on_invalid_transition_or_evidence_handoff(self) -> None:
        for change in ("parent", "drop_transitions", "drop_snapshot", "source", "prepared_ledger", "literal_evidence",
                       "unrelated_record", "old_record"):
            with self.subTest(change=change), tempfile.TemporaryDirectory() as temporary:
                root = Path(temporary)
                project = synthetic_project(root)
                revision = project["successor"]
                if change == "parent":
                    revision["parent_evidence"]["sha256"] = "0" * 64
                elif change == "drop_transitions":
                    revision["protected_record_transitions"] = []
                elif change == "drop_snapshot":
                    revision["prepared_protection"] = None
                elif change == "source":
                    (root / "src/first.c").write_bytes(b"unreviewed source\n")
                elif change == "prepared_ledger":
                    write_json(root / "accept/c009-r2/matches.json", {"units": []})
                elif change == "literal_evidence":
                    revision["literal_evidence"]["sha256"] = "0" * 64
                elif change in ("unrelated_record", "old_record"):
                    row = revision["protected_record_transitions"][0]
                    key = "after" if change == "unrelated_record" else "before"
                    row[key]["entries"] = []
                    row[key + "_sha256"] = metadata_sha(row[key])
                write_json(root / "research/catalogue_v11_r2_evidence.json", revision)
                result = run_catalogue(root, "refused.json")
                self.assertEqual(result.returncode, 1, result.stderr)
                self.assertTrue(result.stderr.startswith("catalogue: "), result.stderr)
                self.assertFalse((root / "refused.json").exists())

    def test_real_cli_keeps_explicit_protection_manifests(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            synthetic_project(root)
            write_json(root / "extra.json", {"units": [{"source": "src/second.c", "profile": "game",
                                                       "functions": [f"func_{BASE + 32:08x}"]}]})
            result = run_catalogue(root, "proposal.json", "--protect-manifest", "extra.json")
            self.assertEqual(result.returncode, 0, result.stderr)
            candidate = json.loads((root / "proposal.json").read_bytes())
            self.assertIn("extra.json", candidate["revision_review"]["protected_summary"])


if __name__ == "__main__":
    unittest.main()
