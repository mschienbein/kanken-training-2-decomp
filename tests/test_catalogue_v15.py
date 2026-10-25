"""Behavioural v15 entry/protection guards, without ROM, archives or aggregate pins."""
from collections import Counter
from copy import deepcopy
import json
from pathlib import Path
import shutil
from types import SimpleNamespace
import tempfile
import unittest
from unittest.mock import patch

from tools import catalogue_unknowns as unknowns, catalogue_local_entries as local

ROOT = Path(__file__).resolve().parents[1]


def claims(manifest: dict, names: dict) -> set[int]:
    return {(names["symbols"][name]["address"] if name in names["symbols"] else int(name.removeprefix("func_"), 16)) & ~1
            for unit in manifest["units"] for name in unit["functions"]}


def fixture(baseline: dict, revision: dict) -> tuple[dict, dict, object, object]:
    before, rev = deepcopy(baseline), deepcopy(revision)
    marker = b"authored nonexecutable metadata image"
    before["image_sha256"] = rev["image_sha256"] = rev["_proof"]["image_sha256"] = unknowns.runtime._sha(marker)
    proof = rev["_proof"]
    parent, callback = proof["sections"]
    bodies = {}
    functions = {row["address"]: row for row in before["functions"]}
    for address, size in [(section["base"], section["size"]) for section in proof["sections"]] + [
        (proof["parent"]["incoming"]["address"], proof["parent"]["incoming"]["size"])
    ] + [(row["anchor"]["address"], row["anchor"]["size"]) for row in parent["relocations"] if "anchor" in row]:
        text = f"authored metadata body {address} ".encode()
        bodies[address] = (text * (size // len(text) + 1))[:size]
    relative = parent["relocations"][-1]
    word = ((((callback["base"] + relative["addend"]) | 1) - (parent["base"] + relative["offset"])) & unknowns.runtime.MASK).to_bytes(4, "little")
    bodies[parent["base"]] = bodies[parent["base"]][:-4] + word

    def reader(analyzer, address: int, size: int) -> bytes:
        for base, body in bodies.items():
            if base <= address and address + size <= base + len(body):
                return body[address - base:address - base + size]
        raise ValueError("synthetic read escaped the authored body")

    for section in proof["sections"]:
        section["image_sha256"] = section["relocated_sha256"] = unknowns.runtime._sha(reader(None, section["base"], section["size"]))
    callback["object_sha256"] = callback["image_sha256"]
    incoming = proof["parent"]["incoming"]
    incoming["image_sha256"] = unknowns.runtime._sha(reader(None, incoming["address"], incoming["size"]))
    decoded = {incoming["site"]: SimpleNamespace(call=True, conditional=False, target=(parent["base"], "thumb"))}
    for relocation in parent["relocations"]:
        if "anchor" in relocation:
            anchor = relocation["anchor"]
            anchor["image_sha256"] = unknowns.runtime._sha(reader(None, anchor["address"], anchor["size"]))
            decoded[parent["base"] + relocation["offset"]] = SimpleNamespace(call=True, conditional=False, target=(anchor["address"], "thumb"))
    for site in range(callback["base"], callback["base"] + callback["size"], 2):
        decoded[site] = SimpleNamespace(size=2)
    analyzer = SimpleNamespace(image=marker, decode=lambda site, isa: decoded.get(site), decoded=decoded)
    rebind(before, rev)
    return before, rev, analyzer, reader


def rebind(baseline: dict, revision: dict) -> None:
    revision["baseline"]["sha256"] = local.metadata_sha(baseline)
    revision["inputs"]["proof"]["sha256"] = local.metadata_sha(revision["_proof"])


class CatalogueV15Tests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.baseline, cls.revision, cls.inputs = unknowns.load_inputs(ROOT)

    def setUp(self) -> None:
        self.before, self.rev, self.analyzer, reader = fixture(self.baseline, self.revision)
        self.patch = patch.object(unknowns, "_read", side_effect=reader)
        self.patch.start()
        self.addCleanup(self.patch.stop)
        self.sources = {"authored/source": "independently supplied source binding"}

    def propose(self) -> dict:
        return unknowns.propose(self.before, self.analyzer, self.rev, self.inputs, self.sources)

    def refuse(self) -> None:
        rebind(self.before, self.rev)
        with self.assertRaises(ValueError):
            self.propose()

    def test_successor_derives_only_the_new_complete_declared_entry(self) -> None:
        result = self.propose()
        unknowns.validate(result, self.before, self.analyzer, self.rev, self.inputs, self.sources)
        old = {row["address"]: row for row in self.before["functions"]}
        new = {row["address"]: row for row in result["functions"]}
        callback = self.rev["_proof"]["sections"][-1]
        self.assertEqual({address: new[address] for address in old}, old)
        self.assertEqual(new.keys() - old.keys(), {callback["base"]})
        entry = new[callback["base"]]
        declaration = next(row for row in callback["symbols"] if row["kind"] == "STT_FUNC")
        self.assertEqual((entry["address"], entry["size"], entry["classification_evidence"]["identity"]),
                         (callback["base"] + declaration["offset"], declaration["size"], declaration["name"]))
        self.assertEqual(entry["code_ranges"], [[entry["address"], entry["address"] + entry["size"]]])
        self.assertEqual(result["functions"], sorted(result["functions"], key=lambda row: row["address"]))
        for before, after in zip(self.before["ranges"], result["ranges"]):
            self.assertEqual(after, {**before, "kind": "function", "owner": entry["address"]} if before["start"] == entry["address"] else before)
        for key in self.before.keys() - {"functions", "ranges", "stats", "generator", "revision_review", "closure_evidence"}:
            self.assertEqual(result[key], self.before[key])
        self.assertEqual({key: value for key, value in result["closure_evidence"].items() if key != unknowns.RECEIPT}, self.before["closure_evidence"])

    def test_statistics_are_the_exact_entry_and_physical_byte_delta(self) -> None:
        result = self.propose()
        before, after = self.before["stats"], result["stats"]
        old_addresses = {row["address"] for row in self.before["functions"]}
        added = [row for row in result["functions"] if row["address"] not in old_addresses]
        size = sum(row["size"] for row in added)
        counts = Counter(row["kind"] for row in result["functions"])
        self.assertEqual(after["functions_by_kind"], dict(counts))
        self.assertEqual(after["functions_by_isa"], dict(Counter(row["isa"] for row in result["functions"])))
        self.assertEqual(after["functions"], len(result["functions"]))
        self.assertEqual(after["classified_functions"] - before["classified_functions"], len(added))
        self.assertEqual(after["code_bytes"] - before["code_bytes"], size)
        self.assertEqual(after["decoded_instructions"] - before["decoded_instructions"], sum(row["size"] // 2 for row in added))
        self.assertEqual(after["creditable_functions"], before["creditable_functions"])
        self.assertEqual(after["creditable_bytes"], before["creditable_bytes"])
        self.assertEqual(after["starts_by_evidence"], dict(Counter(tag for row in result["functions"] for tag in row["evidence"])))
        self.assertEqual(after["unknown_ranges"], sum(row["kind"] == "unknown" for row in result["ranges"]))
        self.assertEqual(after["bytes_by_range_kind"], {kind: sum(row["end"] - row["start"] for row in result["ranges"] if row["kind"] == kind) for kind in {row["kind"] for row in result["ranges"]}})
        changed = {key for key in before if before[key] != after[key]}
        self.assertEqual(changed, {"functions", "classified_functions", "code_bytes", "decoded_instructions", "unknown_ranges", "functions_by_kind", "functions_by_isa", "starts_by_evidence", "bytes_by_range_kind"})

    def test_protection_resolves_every_frozen_accepted_and_both_candidate_claims(self) -> None:
        protected = unknowns.protected_functions(self.before, self.rev)
        expected = set()
        for manifest, names in (("_ledger", "_names"), ("_c012", "_c012_names"), ("_c015", "_c015_names")):
            expected |= claims(self.rev[manifest], self.rev[names])
        old = {row["address"]: row for row in self.before["functions"]}
        new = {row["address"]: row for row in self.propose()["functions"]}
        self.assertEqual(protected, {address: old[address] for address in expected})
        self.assertEqual({address: new[address] for address in expected}, protected)

    def test_unrelated_successor_changes_and_forged_generator_bindings_refuse(self) -> None:
        original = self.propose()
        for label in ("owner", "range", "statistic", "anomaly", "ancestor", "receipt", "review", "generator", "source_binding"):
            candidate = deepcopy(original)
            if label == "owner":
                candidate["functions"][0]["size"] += 4
            elif label == "range":
                candidate["ranges"][0]["owner"] = None
            elif label == "statistic":
                candidate["stats"]["creditable_bytes"] += 1
            elif label == "anomaly":
                candidate["anomalies"].clear()
            elif label == "ancestor":
                candidate["closure_evidence"].pop(next(iter(self.before["closure_evidence"])))
            elif label == "receipt":
                candidate["closure_evidence"][unknowns.RECEIPT]["runtime_pool_value_admitted"] = True
            elif label == "review":
                candidate["revision_review"]["protected_rows"].pop()
            elif label == "generator":
                candidate["generator"]["input_sha256"] = {}
            else:
                candidate["generator"]["source_sha256"] = {"forged": "source"}
            with self.subTest(label=label), self.assertRaises(ValueError):
                unknowns.validate(candidate, self.before, self.analyzer, self.rev, self.inputs, self.sources)

    def test_missing_or_partial_callback_declaration_refuses_after_rebinding(self) -> None:
        original = deepcopy(self.rev)
        for label in ("size", "mapping", "symbol", "relocation", "object_hash", "whole_section"):
            self.rev = deepcopy(original)
            section = self.rev["_proof"]["sections"][-1]
            if label == "size":
                section["symbols"][-1]["size"] -= 2
            elif label == "mapping":
                section["mapping"].append([2, "$d"])
            elif label == "symbol":
                section["symbols"][-1]["name"] = "unrelated_callback"
            elif label == "relocation":
                section["relocations"].append({"offset": 0})
            elif label == "object_hash":
                section["object_sha256"] = "f" * 64
            else:
                section["whole_section_reencoded_equal"] = False
            with self.subTest(label=label):
                self.refuse()

    def test_parent_tail_mapping_and_complete_inventory_cannot_be_borrowed(self) -> None:
        original = deepcopy(self.rev)
        for label in ("extent", "tail", "declaration", "inventory", "padding_only"):
            self.rev = deepcopy(original)
            section = self.rev["_proof"]["sections"][0]
            if label == "extent":
                section["size"] -= 4
            elif label == "tail":
                section["mapping"][-1][0] += 2
            elif label == "declaration":
                section["symbols"][-1]["size"] = section["size"]
            elif label == "inventory":
                section["relocations"].pop(0)
            else:
                section["whole_section_reencoded_equal"] = False
            with self.subTest(label=label):
                self.refuse()

    def test_parent_incoming_code_entry_cannot_be_missing_or_rewritten(self) -> None:
        incoming = self.rev["_proof"]["parent"]["incoming"]
        instruction = self.analyzer.decoded[incoming["site"]]
        for label in ("call", "conditional", "target"):
            changed = deepcopy(instruction)
            if label == "call":
                changed.call = False
            elif label == "conditional":
                changed.conditional = True
            else:
                changed.target = (self.rev["_proof"]["sections"][-1]["base"], "thumb")
            self.analyzer.decoded[incoming["site"]] = changed
            with self.subTest(label=label), self.assertRaises(ValueError):
                self.propose()
        self.analyzer.decoded[incoming["site"]] = instruction
        self.propose()

    def test_independent_named_callee_authentication_is_not_optional(self) -> None:
        relocation = self.rev["_proof"]["sections"][0]["relocations"][0]
        address = relocation["anchor"]["address"]
        owner = next(row for row in self.before["functions"] if row["address"] == address)
        owner["kind"] = "function"
        relocation["anchor"]["record_sha256"] = local.metadata_sha(owner)
        self.refuse()

    def test_named_import_actual_call_target_and_mode_are_checked(self) -> None:
        parent = self.rev["_proof"]["sections"][0]
        relocation = parent["relocations"][0]
        site = parent["base"] + relocation["offset"]
        original = self.analyzer.decoded[site]
        for target in ((relocation["target"], "arm"), (relocation["target"] + 2, "thumb")):
            self.analyzer.decoded[site] = SimpleNamespace(call=True, conditional=False, target=target)
            with self.subTest(target=target), self.assertRaises(ValueError):
                self.propose()
        self.analyzer.decoded[site] = original
        self.propose()

    def test_rel32_symbol_addend_tag_and_target_are_not_whole_field_masks(self) -> None:
        original = deepcopy(self.rev)
        for field, value in (("symbol", "unknown_name"), ("addend", 0), ("thumb", False), ("target", 0x1005fc), ("reencoded_equal", False)):
            self.rev = deepcopy(original)
            self.rev["_proof"]["sections"][0]["relocations"][-1][field] = value
            with self.subTest(field=field):
                self.refuse()

    def test_unknown_range_must_be_exact_and_old_anomaly_must_remain(self) -> None:
        callback = self.rev["_proof"]["sections"][-1]
        span = next(row for row in self.before["ranges"] if row["start"] == callback["base"])
        span["kind"] = "padding"
        self.refuse()
        span["kind"] = "unknown"
        self.rev["remaining_anomalies"] = []
        self.refuse()

    def test_existing_owner_collision_refuses_before_mutation(self) -> None:
        section = self.rev["_proof"]["sections"][-1]
        row = deepcopy(self.before["functions"][0])
        row.update(address=section["base"], size=section["size"])
        self.before["functions"].append(row)
        before = deepcopy(self.before)
        self.refuse()
        self.assertEqual(self.before, before)

    def test_incomplete_callback_decode_refuses_before_mutation(self) -> None:
        section = self.rev["_proof"]["sections"][-1]
        self.analyzer.decoded.pop(section["base"] + section["size"] - 2)
        before = deepcopy(self.before)
        self.refuse()
        self.assertEqual(self.before, before)

    def retained_root(self) -> Path:
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        root = Path(temporary.name)
        for descriptor in self.revision["inputs"].values():
            path = root / descriptor["path"]
            path.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT / descriptor["path"], path)
        (root / unknowns.INPUT).write_bytes((ROOT / unknowns.INPUT).read_bytes())
        return root

    def load_retained(self, root: Path) -> tuple[dict, dict, dict]:
        with patch.object(unknowns.subprocess, "run", return_value=SimpleNamespace(returncode=0, stdout=unknowns.analyze.serialize_catalogue(self.baseline))):
            return unknowns.load_inputs(root)

    def test_frozen_candidate_manifests_do_not_follow_live_file_mutation(self) -> None:
        root = self.retained_root()
        before, revision, inputs = self.load_retained(root)
        live = root / "config/candidates/c012.json"
        live.parent.mkdir(parents=True)
        live.write_text('{"units": []}')
        after, revised, changed_inputs = self.load_retained(root)
        self.assertEqual((after, revised, changed_inputs), (before, revision, inputs))
        descriptor = self.revision["inputs"]["c012"]
        path = root / descriptor["path"]
        path.write_text('{"units": []}')
        with self.assertRaises(ValueError):
            self.load_retained(root)

    def test_missing_hash_mismatched_or_escaping_retained_input_refuses(self) -> None:
        for label in ("missing", "digest", "escaping"):
            root = self.retained_root()
            evidence_path = root / unknowns.INPUT
            evidence = json.loads(evidence_path.read_text())
            descriptor = evidence["inputs"]["proof"]
            if label == "missing":
                (root / descriptor["path"]).unlink()
            elif label == "digest":
                descriptor["sha256"] = "f" * 64
            else:
                descriptor["path"] = "../outside.json"
            evidence_path.write_bytes(unknowns.analyze.serialize_catalogue(evidence))
            with self.subTest(label=label), self.assertRaises((OSError, ValueError)):
                self.load_retained(root)

    def test_nonimmutable_or_wrong_parent_binding_refuses(self) -> None:
        root = self.retained_root()
        evidence_path = root / unknowns.INPUT
        evidence = json.loads(evidence_path.read_text())
        for field, value in (("blob", "main"), ("sha256", "f" * 64), ("revision", "catalogue-v13-r2")):
            candidate = deepcopy(evidence)
            candidate["baseline"][field] = value
            evidence_path.write_bytes(unknowns.analyze.serialize_catalogue(candidate))
            with self.subTest(field=field), self.assertRaises(ValueError):
                self.load_retained(root)


if __name__ == "__main__":
    unittest.main()
