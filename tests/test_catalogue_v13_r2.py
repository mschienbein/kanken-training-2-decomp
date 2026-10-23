"""Root-cause regressions for the two sealed v13 public-consumer findings."""
from copy import deepcopy
import hashlib
import json
from pathlib import Path
import struct
import tempfile
import unittest

from tools import analyze, catalogue_alignment as alignment, catalogue_closure, catalogue_frozen
from tests.test_analyze import BASE
from tests.test_catalogue_alignment import alignment_fixture, rebind


def write_manifest(root: Path, manifest: dict) -> None:
    (root / "research").mkdir(exist_ok=True)
    (root / catalogue_frozen.MANIFEST).write_text(json.dumps(manifest))


def retained_input(root: Path, label: str, value: dict, name: str | None = None) -> dict:
    content = json.dumps(value).encode()
    path = catalogue_frozen.PREFIX + (name or label)
    (root / path).parent.mkdir(parents=True, exist_ok=True)
    (root / path).write_bytes(content)
    return {"path": path, "size": len(content), "sha256": hashlib.sha256(content).hexdigest()}


def protection_fixture(root: Path) -> tuple[dict, dict]:
    inputs = {label: retained_input(root, label, value) for label, value in (
        ("config/names.json", {"symbols": {}}), ("config/matches.json", {"units": []}))}
    manifest = {"revision": "catalogue-v13", "inputs": inputs,
                "absent_paths": [f"config/candidates/c00{number}.json" for number in range(4, 9)]}
    write_manifest(root, manifest)
    baseline = {"functions": [{"address": BASE, "size": 4, "isa": "arm", "kind": "function", "entries": []}]}
    return manifest, baseline


def conditional_fixture(instruction: int) -> tuple[dict, object, dict, list[dict]]:
    baseline, analyzer, evidence = alignment_fixture()
    # Authored synthetic conditional PC-relative halfword load; no original data.
    image = bytearray(analyzer.image)
    struct.pack_into("<I", image, 4, instruction)
    analyzer = analyze.Analyzer(bytes(image), analyzer.layout)
    digest = hashlib.sha256(image).hexdigest()
    baseline["image_sha256"] = digest
    revision = evidence["alignment_revision"]
    revision["image_sha256"] = digest
    revision["_section_proof"]["image_sha256"] = digest
    vetoes = []
    reads = catalogue_closure.read_footprints(baseline, analyzer, read_vetoes=vetoes)
    baseline["closure_evidence"]["read_footprints"] = reads
    baseline["closure_evidence"]["read_vetoes"] = vetoes
    revision["baseline"]["field_sha256"] = alignment.baseline_hashes(baseline)
    rebind(evidence)
    return baseline, analyzer, evidence, vetoes


class CatalogueV13R2Tests(unittest.TestCase):
    def test_declared_absence_has_a_nonconsuming_positive_probe_only(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            manifest = {"revision": "catalogue-v13", "inputs": {}, "absent_paths": ["extra.json"]}
            write_manifest(root, manifest)
            self.assertEqual(catalogue_frozen.load(root), manifest)
            self.assertFalse(catalogue_frozen.is_present(root, "extra.json"))
            with self.assertRaisesRegex(ValueError, "declared absent input cannot be consumed"):
                catalogue_frozen.resolve(root, "extra.json")

    def test_absolute_declared_absent_extra_manifest_never_changes_protected_identities(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            outer = Path(temporary)
            root = outer / "root"
            root.mkdir()
            manifest, baseline = protection_fixture(root)
            extra = (outer / "external-extra.json").resolve()
            manifest["absent_paths"].append(str(extra))
            write_manifest(root, manifest)
            frozen_digest = hashlib.sha256((root / catalogue_frozen.MANIFEST).read_bytes()).hexdigest()
            for units in ([], [{"source": "synthetic.c", "profile": "game", "functions": ["func_00100000"]}]):
                extra.write_text(json.dumps({"units": units}))
                with self.subTest(units=bool(units)), self.assertRaisesRegex(ValueError, "declared absent"):
                    analyze.load_protected_functions(root, baseline, [extra])
                self.assertEqual(hashlib.sha256((root / catalogue_frozen.MANIFEST).read_bytes()).hexdigest(), frozen_digest)

    def test_registered_absolute_extra_manifest_uses_only_retained_content(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            outer = Path(temporary)
            root = outer / "root"
            root.mkdir()
            manifest, baseline = protection_fixture(root)
            extra = (outer / "external-extra.json").resolve()
            extra.write_text(json.dumps({"units": []}))
            manifest["inputs"][str(extra)] = retained_input(root, str(extra), {"units": []}, "extra.json")
            write_manifest(root, manifest)
            before = analyze.load_protected_functions(root, baseline, [extra])
            extra.write_text(json.dumps({"units": [{"source": "synthetic.c", "profile": "game",
                                                  "functions": ["func_00100000"]}]}))
            after = analyze.load_protected_functions(root, baseline, [extra])
            self.assertEqual(before[0], {})
            self.assertEqual(after, before)
            self.assertEqual(after[2][str(extra)], manifest["inputs"][str(extra)]["sha256"])

    def test_materialized_absent_files_symlinks_and_dangling_symlinks_refuse(self) -> None:
        for location in ("original", "historical_sentinel"):
            for kind in ("file", "symlink", "dangling_symlink"):
                with self.subTest(location=location, kind=kind), tempfile.TemporaryDirectory() as temporary:
                    root = Path(temporary)
                    label = "config/names.json"
                    write_manifest(root, {"revision": "catalogue-v13", "inputs": {}, "absent_paths": [label]})
                    path = root / label if location == "original" else root / catalogue_frozen.PREFIX / "absent" / label
                    path.parent.mkdir(parents=True, exist_ok=True)
                    if kind == "file":
                        path.write_text('{"symbols":{}}')
                    else:
                        target = root / "live-names.json"
                        if kind == "symlink":
                            target.write_text('{"symbols":{}}')
                        path.symlink_to(target)
                    for operation in (lambda: catalogue_frozen.load(root),
                                      lambda: catalogue_frozen.is_present(root, label),
                                      lambda: catalogue_frozen.resolve(root, label),
                                      lambda: analyze.load_protected_functions(root, {"functions": []}, [])):
                        with self.assertRaisesRegex(ValueError, "declared absent.*materialized"):
                            operation()

    def test_relative_declared_absent_consumption_and_unconfined_labels_refuse(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            manifest, baseline = protection_fixture(root)
            manifest["absent_paths"].append("extra.json")
            write_manifest(root, manifest)
            with self.assertRaisesRegex(ValueError, "declared absent input cannot be consumed"):
                analyze.load_protected_functions(root, baseline, [root / "extra.json"])
            for label in ("../extra.json", "config/../extra.json", str(root / "extra.json"), ".", ""):
                invalid = {"revision": "catalogue-v13", "inputs": {}, "absent_paths": [label]}
                write_manifest(root, invalid)
                with self.subTest(label=label), self.assertRaisesRegex(ValueError, "confined"):
                    catalogue_frozen.load(root)

    def test_actual_conditional_halfword_read_veto_blocks_external_and_internal_padding(self) -> None:
        for instruction, interval in ((0x115F00BA, [BASE + 2, BASE + 4]),
                                      (0x11DF00BA, [BASE + 22, BASE + 24])):
            with self.subTest(interval=interval):
                baseline, analyzer, evidence, vetoes = conditional_fixture(instruction)
                self.assertTrue(any(row["evidence"] == "conditional_read_veto_only"
                                    and [row["start"], row["end"]] == interval for row in vetoes))
                with self.assertRaisesRegex(ValueError, "exact bounded read veto"):
                    alignment.close_catalogue(baseline, analyzer, evidence, {})
                self.assertFalse(any(row["kind"] == "padding" for row in baseline["ranges"]))

    def test_public_validation_refuses_actual_conditional_veto_after_exact_baseline_rebinding(self) -> None:
        baseline, analyzer, evidence, vetoes = conditional_fixture(0x115F00BA)
        revision = evidence["alignment_revision"]
        baseline["closure_evidence"]["read_vetoes"] = []
        revision["baseline"]["field_sha256"] = alignment.baseline_hashes(baseline)
        rebind(evidence)
        candidate = deepcopy(baseline)
        alignment.close_catalogue(candidate, analyzer, evidence, {})
        baseline["closure_evidence"]["read_vetoes"] = vetoes
        candidate["closure_evidence"]["read_vetoes"] = vetoes
        revision["baseline"]["field_sha256"] = alignment.baseline_hashes(baseline)
        rebind(evidence)
        candidate["closure_evidence"][alignment.RECEIPT] = alignment._receipt(revision)
        with self.assertRaisesRegex(ValueError, "exact bounded read veto"):
            alignment.check_catalogue(candidate, analyzer, evidence)

    def test_nonoverlapping_conditional_and_explicitly_broad_vetoes_preserve_positive_control(self) -> None:
        baseline, analyzer, evidence, vetoes = conditional_fixture(0x115F00B0)
        self.assertTrue(any(row["evidence"] == "conditional_read_veto_only" for row in vetoes))
        vetoes.extend({"start": analyzer.base, "end": analyzer.end, "evidence": category} for category in (
            "unproved_data_endian_pool_value_veto_only", "unmodeled_or_unbounded_static_read_veto_only"))
        revision = evidence["alignment_revision"]
        revision["baseline"]["field_sha256"] = alignment.baseline_hashes(baseline)
        rebind(evidence)
        alignment.close_catalogue(baseline, analyzer, evidence, {})
        alignment.check_catalogue(baseline, analyzer, evidence)
        self.assertEqual(baseline["stats"]["bytes_by_range_kind"]["padding"], 6)

    def test_other_bounded_veto_categories_fail_closed_on_overlap(self) -> None:
        for category in ("partially_mapped_read_veto_only", "future_bounded_read_veto"):
            with self.subTest(category=category):
                baseline, analyzer, evidence = alignment_fixture()
                baseline["closure_evidence"]["read_vetoes"] = [{"start": BASE + 2, "end": BASE + 4, "evidence": category}]
                evidence["alignment_revision"]["baseline"]["field_sha256"] = alignment.baseline_hashes(baseline)
                rebind(evidence)
                with self.assertRaisesRegex(ValueError, "exact bounded read veto"):
                    alignment.close_catalogue(baseline, analyzer, evidence, {})


if __name__ == "__main__":
    unittest.main()
