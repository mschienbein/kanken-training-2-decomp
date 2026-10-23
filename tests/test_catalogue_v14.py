"""Classification-only successor guards; no ROM, compiler, or licensed archive needed."""
from copy import deepcopy
from pathlib import Path
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

    def setUp(self) -> None:
        self.before, self.rev, self.analyzer = fixture(self.baseline, self.revision)
        self.reader = patch.object(non_c, "_read", side_effect=lambda analyzer, address, size: synthetic_body(address, size))
        self.reader.start()
        self.addCleanup(self.reader.stop)

    def propose(self) -> dict:
        return non_c.propose_v14(self.before, self.analyzer, self.rev, self.inputs, {"synthetic": "source"})

    def test_real_inputs_bind_adopted_parent_and_all_2682_complete_accepted_records(self) -> None:
        protected = non_c.v14_protected(self.baseline, self.revision)
        self.assertEqual(len(protected), 2682)
        self.assertEqual(sum(row["size"] for row in protected.values()), 288580)
        self.assertEqual(self.revision["baseline"]["revision"], "catalogue-v13-r2")
        self.assertEqual(local.metadata_sha(self.baseline), self.revision["baseline"]["sha256"])
        self.assertEqual(len(self.revision["_proof"]["classifications"]), 25)
        self.assertEqual(sum(row["size"] for row in self.revision["_proof"]["classifications"]), 1580)
        self.assertEqual(self.revision["_assembly"]["already_classified"], [])

    def test_complete_successor_changes_only_25_kinds_and_evidence(self) -> None:
        result = self.propose()
        non_c.validate_v14(result, self.before, self.analyzer, self.rev, self.inputs, {"synthetic": "source"})
        changed = [(old, new) for old, new in zip(self.before["functions"], result["functions"]) if old != new]
        self.assertEqual(len(changed), 25)
        for old, new in changed:
            projected = {key: value for key, value in new.items() if key != "classification_evidence"}
            projected["kind"] = "function"
            self.assertEqual(projected, old)
        self.assertEqual(result["ranges"], self.before["ranges"])
        self.assertEqual(result["anomalies"], self.before["anomalies"])
        self.assertEqual(result["stats"]["creditable_functions"], 4546)
        self.assertEqual(result["stats"]["creditable_bytes"], 1120094)
        self.assertEqual(result["stats"]["functions_by_kind"]["assembly"], 37)
        self.assertEqual(result["stats"]["functions_by_kind"]["library"], 95)
        protected = non_c.v14_protected(self.before, self.rev)
        actual = {row["address"]: row for row in result["functions"]}
        self.assertTrue(all(actual[address] == record for address, record in protected.items()))

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
                candidate["functions"][100]["kind"] = "synthetic-forbidden-kind"
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
        for index in range(5):
            revision = deepcopy(self.rev)
            revision["_library"]["candidates"][index]["named_identity_witnesses"] = []
            with self.subTest(index=index), self.assertRaisesRegex(ValueError, "incoming references"):
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

    def test_wcsncpy_qualified_anchor_is_not_a_cpp_type_or_source_adoption(self) -> None:
        non_c.check_v14(self.before, self.analyzer, self.rev)
        decision = next(row for row in self.rev["_library"]["adjudications"] if row["function"] == "func_0018651c")
        self.assertEqual(decision["decision"], "admit_library_identity_only")
        self.assertIn("nominal C++ wchar_t", decision["reason"])
        self.assertIn("remain failures", decision["reason"])

    def test_wcslen_refusal_is_retained_and_only_the_separate_tail_may_be_rewritten(self) -> None:
        non_c.check_v14(self.before, self.analyzer, self.rev)
        refusal = self.rev["_library"]["wcslen_prior_refusal"]["row"]
        self.assertEqual(refusal["disposition"], "unresolved")
        candidate = next(row for row in self.rev["_library"]["candidates"] if row["identity"] == "wcslen")
        named = candidate["named_identity_witnesses"][0]
        caller = next(row for row in self.rev["_callers"]["callers"] if row["function"] == named["function"])
        fn = caller["receipt_without_instruction_encodings"]["functions"][0]
        self.assertEqual(fn["linker_rewrites"][0]["offset"], 44)
        fn["linker_rewrites"][0]["offset"] = named["offset"]
        with self.assertRaisesRegex(ValueError, "rewritten at the name"):
            non_c.check_v14(self.before, self.analyzer, self.rev)

    def test_callback_parent_cannot_be_replaced_by_a_shape_only_guess(self) -> None:
        self.rev["_library"]["anchors"]["callback_parent"]["current_classification_identity"] = "guessed callback API"
        with self.assertRaisesRegex(ValueError, "authenticated complete parent"):
            non_c.check_v14(self.before, self.analyzer, self.rev)

    def test_all_twenty_assembly_members_include_78_word_endpoint(self) -> None:
        rows = non_c.check_v14(self.before, self.analyzer, self.rev)
        assembly = [row for row in rows if row["kind"] == "assembly"]
        self.assertEqual(len(assembly), 20)
        self.assertEqual(sum(row["size"] for row in assembly), 1476)
        self.assertEqual(assembly[-1]["address"], 0x1846F8)
        self.assertTrue(assembly[-1]["identity"].endswith("_78"))
        for label in ("declaration", "trace", "extent", "provenance", "already", "missing"):
            revision = deepcopy(self.rev)
            family = revision["_assembly"]["authors"]["fa03"]["record"]["family"]
            if label == "declaration":
                revision["_header"] = revision["_header"].replace("DECL_SUB(78)", "DECL_SUB(40)")
            elif label == "trace":
                family[0]["observed_instruction_sequence_sha256"] = "0" * 64
            elif label == "extent":
                family[0]["expected_instruction_count"] += 1
            elif label == "provenance":
                revision["_assembly"]["authors"]["fa04"]["record"]["public_source"]["sha256"] = "0" * 64
            elif label == "already":
                revision["_assembly"]["already_classified"] = [family[0]["function"]]
            else:
                family.pop()
            with self.subTest(label=label), self.assertRaises(ValueError):
                non_c.check_v14(self.before, self.analyzer, revision)

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

    def test_retained_inputs_refuse_tampering_and_escaping_labels(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            path = root / "proof.json"
            path.write_text("{}")
            descriptor = {"path": "proof.json", "size": 2, "sha256": sha(b"{}")}
            self.assertEqual(non_c._v14_retained(root, descriptor), b"{}")
            path.write_text("[]")
            with self.assertRaisesRegex(ValueError, "digest/size"):
                non_c._v14_retained(root, descriptor)
            for label in ("../outside.json", str(path), "nested/../proof.json"):
                with self.subTest(label=label), self.assertRaisesRegex(ValueError, "confined"):
                    non_c._v14_retained(root, {**descriptor, "path": label})


if __name__ == "__main__":
    unittest.main()
