"""Classification-only library behaviour; authored metadata, no ROM or licensed objects."""
from collections import Counter
from copy import deepcopy
import json
from pathlib import Path
import shutil
from types import SimpleNamespace
import tempfile
import unittest
from unittest.mock import patch

from tools import catalogue_library as library, catalogue_local_entries as local

ROOT = Path(__file__).resolve().parents[1]


def body(address: int, size: int) -> bytes:
    text = f"authored nonexecutable metadata body {address} ".encode()
    return (text * (size // len(text) + 1))[:size]


def rebind(baseline: dict, revision: dict) -> None:
    revision["baseline"]["sha256"] = local.metadata_sha(baseline)
    revision["inputs"]["proof"]["sha256"] = local.metadata_sha(revision["_proof"])


def fixture(baseline: dict, revision: dict) -> tuple[dict, dict, object]:
    before, rev = deepcopy(baseline), deepcopy(revision)
    image = b"authored metadata test image, not executable code"
    before["image_sha256"] = rev["image_sha256"] = rev["_proof"]["image_sha256"] = library.runtime._sha(image)
    proof = rev["_proof"]
    sections = [row["section"] for row in proof["variants"]] + [proof["array_anchor"]["section"], proof["scalar_anchor"]["section"]]
    decoded = {}
    for section in sections:
        section["image_sha256"] = section["relocated_sha256"] = library.runtime._sha(body(section["base"], section["size"]))
        for relocation in section["relocations"]:
            if relocation["type"] in (28, 29):
                decoded[section["base"] + relocation["offset"]] = SimpleNamespace(target=(relocation["target"], "arm"),
                    call=relocation["type"] == 28 or relocation["symbol"] == "_ZdlPv",
                    branch=relocation["type"] == 29 and relocation["symbol"] != "_ZdlPv",
                    conditional=relocation["type"] == 29 and relocation["symbol"] == "_ZdlPv")
    analyzer = SimpleNamespace(image=image, decoded=decoded, decode=lambda site, isa: decoded.get(site))
    rebind(before, rev)
    return before, rev, analyzer


class CatalogueV16Tests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.baseline, cls.revision, cls.inputs = library.load_inputs(ROOT)

    def setUp(self) -> None:
        self.before, self.rev, self.analyzer = fixture(self.baseline, self.revision)
        reader = patch.object(library, "_read", side_effect=lambda analyzer, address, size: body(address, size))
        reader.start()
        self.addCleanup(reader.stop)
        self.sources = {"authored/source": "independent source binding"}

    def propose(self) -> dict:
        return library.propose(self.before, self.analyzer, self.rev, self.inputs, self.sources)

    def refuse(self) -> None:
        rebind(self.before, self.rev)
        with self.assertRaises(ValueError):
            self.propose()

    def test_only_evidenced_kind_and_evidence_change_all_geometry_is_identical(self) -> None:
        result = self.propose()
        library.validate(result, self.before, self.analyzer, self.rev, self.inputs, self.sources)
        old = {row["address"]: row for row in self.before["functions"]}
        new = {row["address"]: row for row in result["functions"]}
        self.assertEqual(new.keys(), old.keys())
        self.assertEqual({a for a in old if old[a] != new[a]}, {library.TARGET})
        for address, before in old.items():
            after = new[address]
            if address != library.TARGET:
                self.assertEqual(after, before)
            else:
                changed = {key for key in before.keys() | after.keys() if before.get(key) != after.get(key)}
                self.assertEqual(changed, {"kind", "classification_evidence"})
                self.assertEqual(after["kind"], "library")
                self.assertEqual(after["classification_evidence"]["identity"], "_ZdaPv")
                self.assertEqual(after["classification_evidence"]["library_witness"], self.rev["_proof"]["variants"][0])
        for key in self.before.keys() - {"functions", "stats", "closure_evidence", "generator", "revision_review"}:
            self.assertEqual(result[key], self.before[key])
        self.assertEqual({k: v for k, v in result["closure_evidence"].items() if k != library.KEY}, self.before["closure_evidence"])

    def test_denominator_and_classification_counters_follow_the_transition(self) -> None:
        result = self.propose()
        old_creditable = {row["address"]: row for row in self.before["functions"] if row["kind"] == "function"}
        expected = {a: row for a, row in old_creditable.items() if a != library.TARGET}
        stats = result["stats"]
        self.assertEqual(stats["creditable_functions"], len(expected))
        self.assertEqual(stats["creditable_bytes"], sum(row["size"] for row in expected.values()))
        self.assertEqual(stats["functions_by_kind"], dict(Counter(row["kind"] for row in result["functions"])))
        self.assertEqual(stats["classified_functions"] - self.before["stats"]["classified_functions"], len(old_creditable.keys() - expected.keys()))
        self.assertEqual({k: v for k, v in stats.items() if k not in ("creditable_functions", "creditable_bytes", "classified_functions", "functions_by_kind")},
                         {k: v for k, v in self.before["stats"].items() if k not in ("creditable_functions", "creditable_bytes", "classified_functions", "functions_by_kind")})

    def test_accepted_and_both_pending_claims_keep_complete_records(self) -> None:
        protected = library.protected_functions(self.before, self.rev)
        expected = set()
        for manifest, names in (("_ledger", "_names"), ("_c015", "_c015_names"), ("_recon", "_recon_names")):
            overlay = self.rev[names]["symbols"]
            expected |= {(overlay[name]["address"] if name in overlay else int(name.removeprefix("func_"), 16)) & ~1
                         for unit in self.rev[manifest]["units"] for name in unit["functions"]}
        new = {row["address"]: row for row in self.propose()["functions"]}
        old = {row["address"]: row for row in self.before["functions"]}
        self.assertNotIn(library.TARGET, expected)
        self.assertEqual(protected, {a: old[a] for a in expected})
        self.assertEqual(protected, {a: new[a] for a in expected})

    def test_refused_and_pending_owners_are_not_promoted(self) -> None:
        result = {row["address"]: row for row in self.propose()["functions"]}
        old = {row["address"]: row for row in self.before["functions"]}
        for row in self.rev["_adjudication"]["owner_dispositions"]:
            if row["verdict"] != "PROVEN":
                self.assertEqual(result[row["address"]], old[row["address"]])
        for row in self.rev["_adjudication"]["pending_linker_v19"]:
            self.assertEqual(result[row["address"]], old[row["address"]])

    def test_unrelated_function_range_statistic_receipt_review_or_generator_changes_refuse(self) -> None:
        original = self.propose()
        for label in ("function", "range", "statistic", "anomaly", "closure", "review", "inputs", "sources"):
            candidate = deepcopy(original)
            if label == "function":
                candidate["functions"][0]["size"] += 4
            elif label == "range":
                candidate["ranges"][0]["owner"] = None
            elif label == "statistic":
                candidate["stats"]["code_bytes"] += 4
            elif label == "anomaly":
                candidate["anomalies"].clear()
            elif label == "closure":
                candidate["closure_evidence"][library.KEY]["address"] += 4
            elif label == "review":
                candidate["revision_review"]["protected_rows"].pop()
            elif label == "inputs":
                candidate["generator"]["input_sha256"] = {}
            else:
                candidate["generator"]["source_sha256"] = {"forged": "source"}
            with self.subTest(label=label), self.assertRaises(ValueError):
                library.validate(candidate, self.before, self.analyzer, self.rev, self.inputs, self.sources)

    def test_partial_generic_or_wrong_isa_definition_refuses_after_rebinding(self) -> None:
        original = deepcopy(self.rev)
        for label in ("declaration", "extent", "mapping", "rewrite", "whole", "hash"):
            self.rev = deepcopy(original)
            section = self.rev["_proof"]["variants"][0]["section"]
            if label == "declaration":
                section["symbols"][-1]["name"] = "unrelated_generic_destructor"
            elif label == "extent":
                section["symbols"][-1]["size"] -= 4
            elif label == "mapping":
                section["mapping"] = [[0, "$t"]]
            elif label == "rewrite":
                section["rewrites"] = [{"offset": 4, "rule": "not_adopted"}]
            elif label == "whole":
                section["whole_section_equal"] = False
            else:
                section["image_sha256"] = section["relocated_sha256"] = "f" * 64
            with self.subTest(label=label):
                self.refuse()

    def test_outgoing_name_target_addend_binding_and_type_are_not_masks(self) -> None:
        original = deepcopy(self.rev)
        for field, value in (("symbol", "inferred_free"), ("target", 0x1852f8), ("addend", 0), ("thumb", True),
                             ("symbol_kind", "STT_OBJECT"), ("symbol_binding", "STB_WEAK"), ("type", 29)):
            self.rev = deepcopy(original)
            self.rev["_proof"]["variants"][0]["section"]["relocations"][0][field] = value
            with self.subTest(field=field):
                self.refuse()

    def test_incoming_natural_name_and_whole_caller_are_required(self) -> None:
        original = deepcopy(self.rev)
        for label in ("name", "partial", "source", "missing"):
            self.rev = deepcopy(original)
            anchor = self.rev["_proof"]["array_anchor"]
            if label == "name":
                anchor["section"]["relocations"][0]["symbol"] = "unrelated_deallocator"
            elif label == "partial":
                anchor["section"]["size"] -= 4
            elif label == "source":
                anchor["source_sha256"] = "f" * 64
            else:
                anchor["section"]["relocations"] = []
            with self.subTest(label=label):
                self.refuse()

    def test_scalar_anchor_requires_a_fresh_successful_whole_native_compile(self) -> None:
        original = deepcopy(self.rev)
        for field, value in (("exit_code", 1), ("cache_used", True), ("source_sha256", "f" * 64), ("temporary_outputs_removed", False)):
            self.rev = deepcopy(original)
            self.rev["_proof"]["scalar_anchor"]["compile"][field] = value
            with self.subTest(field=field):
                self.refuse()

    def test_legacy_jump24_conditional_call_and_tail_branch_both_remain_strict(self) -> None:
        self.propose()
        scalar = self.rev["_proof"]["scalar_anchor"]["section"]
        site = scalar["base"] + scalar["relocations"][0]["offset"]
        self.analyzer.decoded[site].call = False
        self.analyzer.decoded[site].branch = True
        self.propose()
        self.analyzer.decoded[site].branch = False
        self.refuse()
        self.analyzer.decoded[site].call = True
        array = self.rev["_proof"]["array_anchor"]["section"]
        site = array["base"] + array["relocations"][0]["offset"]
        self.analyzer.decoded[site].conditional = True
        self.refuse()

    def test_wrong_native_transfer_target_mode_and_kind_refuse(self) -> None:
        site = library.TARGET + 4
        original = deepcopy(self.analyzer.decoded[site])
        for label in ("mode", "target", "call"):
            instruction = deepcopy(original)
            if label == "mode":
                instruction.target = (instruction.target[0], "thumb")
            elif label == "target":
                instruction.target = (instruction.target[0] + 4, "arm")
            else:
                instruction.call = False
            self.analyzer.decoded[site] = instruction
            with self.subTest(label=label), self.assertRaises(ValueError):
                self.propose()
        self.analyzer.decoded[site] = original
        self.propose()

    def test_cross_build_equality_and_preferred_provenance_are_mandatory(self) -> None:
        original = deepcopy(self.rev)
        for label in ("member", "inventory", "preferred"):
            self.rev = deepcopy(original)
            if label == "member":
                self.rev["_proof"]["variants"][2]["member_sha256"] = "f" * 64
            elif label == "inventory":
                self.rev["_proof"]["variants"].pop()
            else:
                self.rev["_proof"]["variants"].reverse()
            with self.subTest(label=label):
                self.refuse()

    def test_new_pending_claim_on_target_refuses_transactionally(self) -> None:
        self.rev["_recon"]["units"].append({"source": "authored/pending.cpp", "functions": ["func_0019e08c"]})
        before = deepcopy(self.before)
        self.refuse()
        self.assertEqual(self.before, before)

    def test_adjudication_cannot_promote_another_discovery_owner(self) -> None:
        extra = deepcopy(self.rev["_adjudication"]["candidates"][0])
        extra["address"] += 4
        self.rev["_adjudication"]["candidates"].append(extra)
        self.refuse()

    def retained_root(self) -> Path:
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        root = Path(temporary.name)
        for descriptor in self.revision["inputs"].values():
            path = root / descriptor["path"]
            path.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT / descriptor["path"], path)
        (root / library.INPUT).write_bytes((ROOT / library.INPUT).read_bytes())
        return root

    def load_retained(self, root: Path) -> tuple[dict, dict, dict]:
        with patch.object(library.subprocess, "run", return_value=SimpleNamespace(returncode=0, stdout=library.analyze.serialize_catalogue(self.baseline))):
            return library.load_inputs(root)

    def test_live_pending_mutation_cannot_replace_frozen_manifests(self) -> None:
        root = self.retained_root()
        before = self.load_retained(root)
        live = root / "config/candidates/c016.json"
        live.parent.mkdir(parents=True)
        live.write_text('{"units": []}')
        self.assertEqual(self.load_retained(root), before)
        (root / self.revision["inputs"]["recon"]["path"]).write_text('{"units": []}')
        with self.assertRaises(ValueError):
            self.load_retained(root)

    def test_missing_mismatched_and_escaping_frozen_inputs_refuse(self) -> None:
        for label in ("missing", "digest", "escape"):
            root = self.retained_root()
            evidence = json.loads((root / library.INPUT).read_text())
            descriptor = evidence["inputs"]["proof"]
            if label == "missing":
                (root / descriptor["path"]).unlink()
            elif label == "digest":
                descriptor["sha256"] = "f" * 64
            else:
                descriptor["path"] = "../unbound.json"
            (root / library.INPUT).write_bytes(library.analyze.serialize_catalogue(evidence))
            with self.subTest(label=label), self.assertRaises((OSError, ValueError)):
                self.load_retained(root)

    def test_parent_requires_immutable_blob_and_exact_adopted_hash(self) -> None:
        root = self.retained_root()
        path = root / library.INPUT
        evidence = json.loads(path.read_text())
        for field, value in (("blob", "main"), ("sha256", "f" * 64), ("revision", "catalogue-v14")):
            candidate = deepcopy(evidence)
            candidate["baseline"][field] = value
            path.write_bytes(library.analyze.serialize_catalogue(candidate))
            with self.subTest(field=field), self.assertRaises(ValueError):
                self.load_retained(root)


if __name__ == "__main__":
    unittest.main()
