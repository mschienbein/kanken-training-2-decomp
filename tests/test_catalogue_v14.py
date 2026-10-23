"""Classification-only successor guards; no ROM, compiler, or licensed archive needed."""
from collections import Counter
from copy import deepcopy
import json
from pathlib import Path
import shutil
from types import SimpleNamespace
import tempfile
import unittest
from unittest.mock import patch

from tools import catalogue_non_c as non_c, catalogue_local_entries as local

ROOT = Path(__file__).resolve().parents[1]


def sha(content: bytes) -> str:
    return non_c.runtime._sha(content)


def synthetic_body(address: int, size: int) -> bytes:
    """Authored textual stand-in for metadata tests, never executable payload."""
    label = f"synthetic metadata owner {address} ".encode()
    return (label * (size // len(label) + 1))[:size]


def claimed_addresses(manifest: dict, names: dict) -> set[int]:
    return {(names[name]["address"] if name in names else int(name.removeprefix("func_"), 16)) & ~1
            for unit in manifest["units"] for name in unit["functions"]}


def evidenced_owners(baseline: dict, revision: dict) -> dict[int, tuple[dict, str, str]]:
    """Derive expected transitions from the source evidence, not the generated totals."""
    functions = {row["address"]: row for row in baseline["functions"]}
    expected = {row["address"]: (row, "library", row["identity"])
                for row in revision["_library"]["candidates"]}
    for row in revision["_assembly"]["authors"]["fa03"]["record"]["family"]:
        address = int(row["address"], 16)
        if functions[address]["kind"] == "function":
            expected[address] = (row, "assembly", row["public_symbol"])
    return expected


def fixture(baseline: dict, revision: dict) -> tuple[dict, dict, object]:
    baseline, revision = deepcopy(baseline), deepcopy(revision)
    image = b"authored nonexecutable metadata-test image"
    baseline["image_sha256"] = revision["image_sha256"] = sha(image)
    proof = revision["_proof"]
    proof["image_sha256"] = sha(image)
    revision["baseline"]["sha256"] = local.metadata_sha(baseline)
    proof["baseline"] = deepcopy(revision["baseline"])
    targets = {}
    library = revision["_library"]
    library["image_sha256"] = sha(image)
    revision["_assembly"]["image_sha256"] = sha(image)
    for candidate in library["candidates"]:
        digest = sha(synthetic_body(candidate["address"], candidate["size"]))
        candidate["original_extent_sha256"] = digest
        witness = candidate["selected_vendor_witness"]
        witness["object_body_sha256"] = witness["image_body_sha256"] = digest
        witness["strict_comparison"]["relocated_body_sha256"] = digest
        for incoming in candidate["caller_witnesses"]:
            incoming["whole_caller_extent_sha256"] = sha(synthetic_body(incoming["address"], incoming["size"]))
            targets[incoming["site"]] = SimpleNamespace(target=(incoming["target"], incoming["target_isa"]),
                                                       conditional=incoming["conditional"], call=True)
        for named in candidate["named_identity_witnesses"]:
            if "address" in named:
                named["whole_caller_extent_sha256"] = sha(synthetic_body(named["address"], named["size"]))
    parent = library["anchors"]["callback_parent"]
    parent["relocated_body_sha256"] = sha(synthetic_body(parent["address"], parent["size"]))
    # The callback anchor retains exact named-record equality as well.
    callback = library["candidates"][0]
    library["anchors"]["callers"][str(callback["address"])] = deepcopy(callback["named_identity_witnesses"])
    for caller in revision["_callers"]["callers"]:
        caller["receipt_without_instruction_encodings"]["image_sha256"] = sha(image)
    family = revision["_assembly"]["authors"]["fa03"]["record"]["family"]
    for source in family:
        source["original_extent_sha256"] = sha(synthetic_body(int(source["address"], 16), source["size"]))
    by_name = {row["function"]: row for row in family}
    for source in revision["_assembly"]["authors"]["fa04"]["record"]["functions"]:
        source["original_extent_sha256"] = by_name[source["function"]]["original_extent_sha256"]
    for source in revision["_assembly"]["authors"]["fa05"]["record"]["functions"]:
        source["original_owner_sha256"] = by_name[source["function"]]["original_extent_sha256"]
    sources = {row["address"]: row for row in library["candidates"]}
    sources.update({int(row["address"], 16): row for row in family})
    for row in proof["classifications"]:
        source = sources[row["address"]]
        row["extent_sha256"] = source["original_extent_sha256"]
        row["source_sha256"] = local.metadata_sha(source)
    analyzer = SimpleNamespace(image=image, decode=lambda site, isa: targets.get(site))
    return baseline, revision, analyzer


class CatalogueV14Tests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.baseline, cls.revision, cls.inputs = non_c.load_v14_inputs(ROOT)
        cls.live_names = json.loads((ROOT / "config/names.json").read_text())["symbols"]
        cls.live_ledger = json.loads((ROOT / "config/matches.json").read_text())
        cls.proposal = json.loads((ROOT / "config/catalogue.json").read_text())

    def setUp(self) -> None:
        self.before, self.rev, self.analyzer = fixture(self.baseline, self.revision)
        self.reader = patch.object(non_c, "_read", side_effect=lambda analyzer, address, size: synthetic_body(address, size))
        self.reader.start()
        self.addCleanup(self.reader.stop)

    def propose(self) -> dict:
        return non_c.propose_v14(self.before, self.analyzer, self.rev, self.inputs, {"synthetic": "source"})

    def retained_root(self) -> Path:
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        root = Path(temporary.name)
        for descriptor in self.revision["inputs"].values():
            path = root / descriptor["path"]
            path.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT / descriptor["path"], path)
        (root / non_c.V14_INPUT).write_bytes((ROOT / non_c.V14_INPUT).read_bytes())
        return root

    def load_retained(self, root: Path) -> tuple[dict, dict, dict]:
        # Isolate filesystem refusals from Git availability in the temporary root.
        parent = (json.dumps(self.baseline, indent=2, sort_keys=True, ensure_ascii=False) + "\n").encode()
        with patch.object(non_c.subprocess, "run", return_value=SimpleNamespace(returncode=0, stdout=parent)):
            return non_c.load_v14_inputs(root)

    def test_frozen_protection_resolves_exact_parent_records_and_certificate_sources(self) -> None:
        protected = non_c.v14_protected(self.baseline, self.revision)
        parent = {row["address"]: row for row in self.baseline["functions"]}
        expected = claimed_addresses(self.revision["_ledger"], self.revision["_names"]["symbols"])
        self.assertEqual(protected, {address: parent[address] for address in expected})
        self.assertEqual(local.metadata_sha(self.baseline), self.revision["baseline"]["sha256"])
        self.assertEqual(self.baseline["generator"]["revision"], self.revision["baseline"]["revision"])
        sources = evidenced_owners(self.baseline, self.revision)
        certificates = {row["address"]: row for row in self.revision["_proof"]["classifications"]}
        self.assertEqual(certificates.keys(), sources.keys())
        for address, (source, kind, identity) in sources.items():
            certificate = certificates[address]
            self.assertEqual((certificate["kind"], certificate["identity"]), (kind, identity))
            self.assertEqual((certificate["size"], certificate["isa"]), (parent[address]["size"], parent[address]["isa"]))
            self.assertEqual(certificate["before_sha256"], local.metadata_sha(parent[address]))
            self.assertEqual(certificate["source_sha256"], local.metadata_sha(source))

    def test_successor_changes_exactly_the_evidenced_records_and_preserves_geometry(self) -> None:
        result = self.propose()
        non_c.validate_v14(result, self.before, self.analyzer, self.rev, self.inputs, {"synthetic": "source"})
        parent = {row["address"]: row for row in self.before["functions"]}
        actual = {row["address"]: row for row in result["functions"]}
        expected = evidenced_owners(self.before, self.rev)
        self.assertEqual(actual.keys(), parent.keys())
        self.assertEqual([row["address"] for row in result["functions"]],
                         [row["address"] for row in self.before["functions"]])
        self.assertEqual({address for address in parent if parent[address] != actual[address]}, expected.keys())
        for address, before in parent.items():
            after = actual[address]
            with self.subTest(address=address):
                for field in ("address", "size", "isa", "entries", "code_ranges", "pools", "literals"):
                    self.assertEqual(after[field], before[field])
                if address in expected:
                    source, kind, identity = expected[address]
                    changed = {key for key in before.keys() | after.keys() if before.get(key) != after.get(key)}
                    self.assertEqual(changed, {"kind", "classification_evidence"})
                    self.assertEqual(after["kind"], kind)
                    self.assertEqual(after["classification_evidence"]["identity"], identity)
                    self.assertEqual(after["classification_evidence"]["original_extent_sha256"],
                                     source["original_extent_sha256"])
                else:
                    self.assertEqual(after, before)
        self.assertEqual(result["ranges"], self.before["ranges"])
        self.assertEqual([row for row in result["ranges"] if row["kind"] == "unknown"],
                         [row for row in self.before["ranges"] if row["kind"] == "unknown"])
        self.assertEqual(result["anomalies"], self.before["anomalies"])

    def test_creditable_set_and_statistics_are_parent_minus_evidenced_owners(self) -> None:
        result = self.propose()
        parent = {row["address"]: row for row in self.before["functions"]}
        expected = evidenced_owners(self.before, self.rev)
        creditable = {address for address, row in parent.items() if row["kind"] == "function"} - expected.keys()
        self.assertEqual({row["address"] for row in result["functions"] if row["kind"] == "function"}, creditable)
        stats = deepcopy(self.before["stats"])
        stats["creditable_functions"] = len(creditable)
        stats["creditable_bytes"] = sum(parent[address]["size"] for address in creditable)
        stats["classified_functions"] += len(expected)
        stats["functions_by_kind"] = dict(Counter(expected[address][1] if address in expected else row["kind"]
                                                  for address, row in parent.items()))
        self.assertEqual(result["stats"], stats)

    def test_live_accepted_and_candidate_claims_retain_complete_parent_records(self) -> None:
        parent = {row["address"]: row for row in self.baseline["functions"]}
        actual = {row["address"]: row for row in self.proposal["functions"]}
        certified = evidenced_owners(self.baseline, self.revision)
        manifests = {ROOT / "config/matches.json": self.live_ledger}
        manifests.update({path: json.loads(path.read_text()) for path in (ROOT / "config/candidates").glob("*.json")})
        for path, manifest in manifests.items():
            claims = claimed_addresses(manifest, self.live_names)
            with self.subTest(manifest=path.name):
                self.assertFalse(claims & certified.keys())
                self.assertEqual({address: actual[address] for address in claims},
                                 {address: parent[address] for address in claims})
        generated = {row["address"]: row for row in self.propose()["functions"]}
        accepted = claimed_addresses(self.live_ledger, self.live_names)
        self.assertEqual({address: generated[address] for address in accepted},
                         {address: parent[address] for address in accepted})

    def test_reverse_validation_rejects_every_nonclassification_projection_change(self) -> None:
        original = self.propose()
        for label in ("extent", "entry", "status", "physical_range", "unknown", "anomaly", "statistics", "evidence", "receipt", "review", "generator"):
            candidate = deepcopy(original)
            if label == "extent":
                candidate["functions"][0]["size"] += 2
            elif label == "entry":
                entry = candidate["functions"][0]["entries"][0]
                entry["isa"] = "arm" if entry["isa"] == "thumb" else "thumb"
            elif label == "status":
                owner = next(row for row in candidate["functions"] if row["kind"] == "function")
                owner["kind"] = "synthetic-forbidden-kind"
            elif label == "physical_range":
                candidate["ranges"][0]["start"] += 1
            elif label == "unknown":
                candidate["stats"]["bytes_by_range_kind"]["unknown"] -= 1
            elif label == "anomaly":
                candidate["anomalies"] = []
            elif label == "statistics":
                candidate["stats"]["creditable_bytes"] += 1
            elif label == "evidence":
                candidate["revision_review"]["classifications"][0]["identity"] = "unproved alias"
            elif label == "receipt":
                candidate["closure_evidence"][non_c.V14_KEY]["classifications"].pop()
            elif label == "generator":
                candidate["generator"]["source_sha256"]["synthetic"] = "forged"
            else:
                candidate["revision_review"]["protected_rows"].pop()
            with self.subTest(label=label), self.assertRaises(ValueError):
                non_c.validate_v14(candidate, self.before, self.analyzer, self.rev, self.inputs, {"synthetic": "source"})

    def test_short_identical_library_body_without_named_anchor_still_refuses(self) -> None:
        for index, source in enumerate(self.rev["_library"]["candidates"]):
            revision = deepcopy(self.rev)
            revision["_library"]["candidates"][index]["named_identity_witnesses"] = []
            with self.subTest(function=source["function"]), self.assertRaisesRegex(ValueError, "incoming references"):
                non_c.check_v14(self.before, self.analyzer, revision)

    def test_library_partial_masked_or_conflicting_definition_refuses(self) -> None:
        for label in ("partial", "relocation", "conflict", "not_authenticated", "wrong_name", "body"):
            revision = deepcopy(self.rev)
            candidate = revision["_library"]["candidates"][0]
            if label == "partial":
                candidate["selected_vendor_witness"]["section_size"] += 2
            elif label == "relocation":
                candidate["relocation_matching"]["total_relocation_count"] = 1
            elif label == "conflict":
                candidate["body_mask_family_count"] = 2
            elif label == "not_authenticated":
                candidate["identity_authenticated"] = False
            elif label == "wrong_name":
                candidate["selected_vendor_witness"]["declared_symbol"]["name"] = "another API"
            else:
                candidate["selected_vendor_witness"]["object_body_sha256"] = "0" * 64
            with self.subTest(label=label), self.assertRaisesRegex(ValueError, "exact complete"):
                non_c.check_v14(self.before, self.analyzer, revision)

    def test_qualified_caller_requires_bound_source_object_and_whole_match_receipt(self) -> None:
        self.propose()
        target = self.rev["_library"]["authors"]["fa06"]["record"]["target"]
        for label in ("source", "object", "receipt_source", "receipt_object", "partial_match"):
            revision = deepcopy(self.rev)
            candidate = next(row for row in revision["_library"]["candidates"] if row["function"] == target)
            named = candidate["named_identity_witnesses"][0]
            caller = next(row for row in revision["_callers"]["callers"] if row["original_receipt"] == named["receipt"])
            receipt = caller["receipt_without_instruction_encodings"]
            if label == "source":
                caller["source_text"] += "\n"
            elif label == "object":
                caller["object"]["sha256"] = "0" * 64
            elif label == "receipt_source":
                receipt["source_sha256"] = "0" * 64
            elif label == "receipt_object":
                receipt["object_sha256"] = "0" * 64
            else:
                matched = next(row for row in receipt["functions"] if row["address"] == named["address"])
                matched["candidate_size"] += 1
            with self.subTest(label=label), self.assertRaises(ValueError):
                non_c.check_v14(self.before, self.analyzer, revision)

    def test_separate_tail_rewrite_is_allowed_but_rewriting_the_named_call_refuses(self) -> None:
        refusal = deepcopy(self.rev["_library"]["wcslen_prior_refusal"])
        self.propose()
        self.assertEqual(self.rev["_library"]["wcslen_prior_refusal"], refusal)
        candidate = next(row for row in self.rev["_library"]["candidates"]
                         if row["function"] == refusal["row"]["function"])
        named = candidate["named_identity_witnesses"][0]
        caller = next(row for row in self.rev["_callers"]["callers"] if row["original_receipt"] == named["receipt"])
        matched = next(row for row in caller["receipt_without_instruction_encodings"]["functions"]
                       if row["address"] == named["address"])
        self.assertTrue(matched["linker_rewrites"])
        self.assertTrue(all(row["offset"] != named["offset"] for row in matched["linker_rewrites"]))
        matched["linker_rewrites"][0]["offset"] = named["offset"]
        with self.assertRaisesRegex(ValueError, "rewritten at the name"):
            non_c.check_v14(self.before, self.analyzer, self.rev)

    def test_callback_parent_cannot_be_replaced_by_a_shape_only_guess(self) -> None:
        self.rev["_library"]["anchors"]["callback_parent"]["current_classification_identity"] = "guessed callback API"
        with self.assertRaisesRegex(ValueError, "authenticated complete parent"):
            non_c.check_v14(self.before, self.analyzer, self.rev)

    def test_assembly_classifies_exactly_the_declared_family_and_refuses_incomplete_traces(self) -> None:
        rows = non_c.check_v14(self.before, self.analyzer, self.rev)
        family = self.rev["_assembly"]["authors"]["fa03"]["record"]["family"]
        parent = {row["address"]: row for row in self.before["functions"]}
        expected = {int(row["address"], 16): row for row in family
                    if parent[int(row["address"], 16)]["kind"] == "function"}
        actual = {row["address"]: row for row in rows if row["kind"] == "assembly"}
        self.assertEqual(actual.keys(), expected.keys())
        for address, source in expected.items():
            self.assertEqual(actual[address]["identity"], source["public_symbol"])
            self.assertEqual(actual[address]["size"], source["size"])
        already = [row["function"] for row in family if parent[int(row["address"], 16)]["kind"] != "function"]
        self.assertEqual(self.rev["_assembly"]["already_classified"], already)
        for label in ("declaration", "trace", "extent", "provenance", "already", "missing"):
            revision = deepcopy(self.rev)
            family = revision["_assembly"]["authors"]["fa03"]["record"]["family"]
            if label == "declaration":
                declaration = f"DECL_SUB({family[-1]['word_count']})"
                revision["_header"] = revision["_header"].replace(declaration, "")
            elif label == "trace":
                family[0]["observed_instruction_sequence_sha256"] = "0" * 64
            elif label == "extent":
                family[0]["expected_instruction_count"] += 1
            elif label == "provenance":
                revision["_assembly"]["authors"]["fa04"]["record"]["public_source"]["sha256"] = "0" * 64
            elif label == "already":
                revision["_assembly"]["already_classified"] = already + [family[0]["function"]]
            else:
                family.pop()
            with self.subTest(label=label), self.assertRaises(ValueError):
                non_c.check_v14(self.before, self.analyzer, revision)

    def test_nonfamily_neighbour_without_source_evidence_is_not_classified(self) -> None:
        result = self.propose()
        parent = {row["address"]: row for row in self.before["functions"]}
        actual = {row["address"]: row for row in result["functions"]}
        boundary = self.rev["_assembly"]["family_end"]
        neighbour = min(address for address, row in parent.items()
                        if address >= boundary and row["kind"] == "function")
        self.assertNotIn(neighbour, evidenced_owners(self.before, self.rev))
        self.assertEqual(actual[neighbour], parent[neighbour])
        forged = deepcopy(self.rev["_proof"]["classifications"][-1])
        forged.update(address=neighbour, size=parent[neighbour]["size"], isa=parent[neighbour]["isa"],
                      before_sha256=local.metadata_sha(parent[neighbour]))
        self.rev["_proof"]["classifications"].append(forged)
        self.rev["_proof"]["classifications"].sort(key=lambda row: row["address"])
        with self.assertRaisesRegex(ValueError, "omit, duplicate or add"):
            non_c.check_v14(self.before, self.analyzer, self.rev)

    def test_a_newly_protected_target_refuses_without_mutation(self) -> None:
        target = self.rev["_proof"]["classifications"][0]["address"]
        self.rev["_ledger"]["units"].append({"functions": [f"func_{target:08x}"]})
        before = deepcopy(self.before)
        with self.assertRaisesRegex(ValueError, "accepted complete-record snapshot"):
            self.propose()
        self.assertEqual(self.before, before)

    def test_duplicate_and_missing_certificates_and_wrong_extent_refuse(self) -> None:
        for label in ("duplicate", "missing", "extent", "source", "kind"):
            revision = deepcopy(self.rev)
            rows = revision["_proof"]["classifications"]
            if label == "duplicate":
                rows.append(deepcopy(rows[0]))
            elif label == "missing":
                rows.pop()
            elif label == "extent":
                rows[0]["size"] += 2
            elif label == "source":
                rows[0]["source_sha256"] = "0" * 64
            else:
                rows[0]["kind"] = "runtime"
            with self.subTest(label=label), self.assertRaises(ValueError):
                non_c.check_v14(self.before, self.analyzer, revision)

    def test_each_consumed_frozen_input_refuses_tampering_or_missing_content(self) -> None:
        root = self.retained_root()
        baseline, revision, inputs = self.load_retained(root)
        self.assertEqual(baseline, self.baseline)
        self.assertEqual(inputs, self.inputs)
        self.assertEqual(non_c.v14_protected(baseline, revision),
                         non_c.v14_protected(self.baseline, self.revision))
        for label, descriptor in self.revision["inputs"].items():
            path = root / descriptor["path"]
            content = path.read_bytes()
            with self.subTest(label=label, mutation="tampered"):
                path.write_bytes(content + b"\n")
                with self.assertRaisesRegex(ValueError, "digest/size"):
                    self.load_retained(root)
                path.write_bytes(content)
            with self.subTest(label=label, mutation="missing"):
                path.unlink()
                with self.assertRaises(FileNotFoundError):
                    self.load_retained(root)
                path.write_bytes(content)

    def test_fixed_frozen_input_roster_refuses_missing_or_extra_roles(self) -> None:
        root = self.retained_root()
        original = json.loads((root / non_c.V14_INPUT).read_text())
        for label, descriptor in original["inputs"].items():
            for mutation in ("missing", "extra"):
                manifest = deepcopy(original)
                if mutation == "missing":
                    del manifest["inputs"][label]
                else:
                    manifest["inputs"]["unregistered_" + label] = descriptor
                (root / non_c.V14_INPUT).write_text(json.dumps(manifest))
                with self.subTest(label=label, mutation=mutation), self.assertRaisesRegex(ValueError, "fixed review input set"):
                    self.load_retained(root)

    def test_retained_input_resolver_refuses_escaping_labels(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            path = root / "proof.json"
            content = b"{}"
            path.write_bytes(content)
            descriptor = {"path": path.name, "size": len(content), "sha256": sha(content)}
            self.assertEqual(non_c._v14_retained(root, descriptor), content)
            for label in ("../outside.json", str(path), "nested/../proof.json"):
                with self.subTest(label=label), self.assertRaisesRegex(ValueError, "confined"):
                    non_c._v14_retained(root, {**descriptor, "path": label})


if __name__ == "__main__":
    unittest.main()
