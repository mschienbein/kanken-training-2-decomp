"""V13 public producer/consumer tests with real synthetic image and ELF inputs."""
from copy import deepcopy
import hashlib
import json
from pathlib import Path
import struct
import tempfile
import unittest

from tools import analyze, catalogue_alignment as alignment, catalogue_frozen, catalogue_local_entries as local, catalogue_non_c as non_c
from tests.test_analyze import BASE, ROOT, fixture
from tests.test_catalogue_local_entries import synthetic_archive


def sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def function(address: int, size: int, isa: str, code_size: int | None = None) -> dict:
    code_size = size if code_size is None else code_size
    return {"address": address, "size": size, "isa": isa, "kind": "function", "code_size": code_size,
            "entries": [{"address": address, "isa": isa, "kind": "entry", "evidence": [], "sources": []}],
            "code_ranges": [[address, address + code_size]], "pools": [], "literals": [], "evidence": [],
            "noreturn": False, "noreturn_reasons": []}


def elf_member(section: bytes, size: int) -> bytes:
    strings = b"\0.text\0$t\0$d\0synthetic\0"
    symbols = b"\0" * 16
    for name, value, declared_size, info in ((1, 0, 0, 3), (7, 0, 0, 0), (10, size, 0, 0), (13, 1, size, 0x12)):
        symbols += struct.pack("<IIIBBH", name, value, declared_size, info, 0, 1)
    names = b"\0.text\0.symtab\0.strtab\0.shstrtab\0"
    blobs = [b"", section, symbols, strings, names]
    data, offsets = bytearray(52), []
    for blob in blobs:
        data.extend(b"\0" * (-len(data) % 4))
        offsets.append(len(data))
        data.extend(blob)
    data.extend(b"\0" * (-len(data) % 4))
    headers_offset = len(data)
    for header in ((0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                   (1, 1, 6, 0, offsets[1], len(section), 0, 0, 2, 0),
                   (7, 2, 0, 0, offsets[2], len(symbols), 3, 4, 4, 16),
                   (15, 3, 0, 0, offsets[3], len(strings), 0, 0, 1, 0),
                   (23, 3, 0, 0, offsets[4], len(names), 0, 0, 1, 0)):
        data.extend(struct.pack("<10I", *header))
    data[:52] = struct.pack("<16sHHIIIIIHHHHHH", b"\x7fELF\x01\x01\x01" + b"\0" * 9,
                            1, 40, 1, 0, 0, headers_offset, 0x05000000, 52, 0, 0, 40, 5, 4)
    return bytes(data)


def alignment_fixture(*, disagree: bool = False) -> tuple[dict, object, dict]:
    fill = b"\x35\x7a"
    text = (struct.pack("<H", 0x4770) + fill + struct.pack("<I", 0xE12FFF1E)
            + struct.pack("<H", 0x4770) + (b"\x34\x7a" if disagree else fill) + struct.pack("<I", 0xE12FFF1E)
            + struct.pack("<3H", 0x2000, 0x3001, 0x4770) + b"\0\0" + b"literal!"[:4] + b"\xff" * 4)
    image, layout = fixture(list(struct.unpack("<8I", text)))
    functions = [function(BASE, 2, "thumb"), function(BASE + 4, 4, "arm"), function(BASE + 8, 2, "thumb"),
                 function(BASE + 12, 4, "arm"), function(BASE + 16, 12, "thumb", 6)]
    functions[-1]["pools"] = [[BASE + 24, BASE + 28]]
    ranges = [{"start": row["address"], "end": row["address"] + row["size"], "kind": "function", "owner": row["address"]}
              for row in functions]
    ranges[-1:] = [{"start": BASE + 16, "end": BASE + 22, "kind": "function", "owner": BASE + 16},
                   {"start": BASE + 24, "end": BASE + 28, "kind": "function", "owner": BASE + 16}]
    ranges.extend({"start": BASE + lo, "end": BASE + hi, "kind": "unknown", "owner": None}
                  for lo, hi in ((2, 4), (10, 12), (22, 24), (28, 32)))
    ranges.sort(key=lambda row: row["start"])
    catalogue = {"image_sha256": sha(image), "functions": functions, "ranges": ranges,
                 "anomalies": [{"address": BASE + 16, "issue": "unexplained_function_bytes"}],
                 "closure_evidence": {"read_footprints": []}, "pc_relative_pointer_witnesses": [],
                 "stats": {"bytes_by_range_kind": {"function": 22, "unknown": 10}, "unknown_ranges": 4,
                           "anomalies": 1, "anomalies_by_issue": {"unexplained_function_bytes": 1},
                           "creditable_bytes": 24, "creditable_functions": 5}}
    baseline = {"revision": "catalogue-v12-r2", "adopted": False, "proposal_sha256": local.metadata_sha(catalogue),
                "field_sha256": alignment.baseline_hashes(catalogue), "range_kinds": ["function", "unknown"]}
    sections = []
    for offset, size, declared in ((0, 2, 2), (8, 2, 2), (16, 12, 6)):
        payload = text[offset:offset + size]
        member = elf_member(payload, declared)
        archive = synthetic_archive(member, f"synthetic_{offset}.o")
        symbols = [{"name": ".text", "kind": "STT_SECTION", "bind": "STB_LOCAL", "value": 0, "offset": 0, "size": 0},
                   {"name": "synthetic", "kind": "STT_FUNC", "bind": "STB_GLOBAL", "value": 1, "offset": 0, "size": declared}]
        sections.append({"id": str(offset), "base": BASE + offset, "index": 1, "name": ".text", "size": size,
                         "alignment": 2, "complete": True, "whole_section_reencoded_equal": True, "allocated": True,
                         "executable": True, "writable": False, "nobits": False, "image_sha256": sha(payload),
                         "object_sha256": sha(payload), "relocated_sha256": sha(payload),
                         "composition": [{"offset": 0, "end_offset": size, "object_sha256": sha(payload),
                                          "image_sha256": sha(payload), "relocated_sha256": sha(payload), "reencoded_equal": True}],
                         "mapping": [[0, "$t"], [declared, "$d"]], "symbols": symbols, "relocations": [],
                         "bindings": [{"address": BASE + offset, "record_sha256": local.metadata_sha(functions[(0, 8, 16).index(offset) * 2])}],
                         "bounded_references": [], "witness": {"archive": f"private/{offset}.a", "archive_sha256": sha(archive),
                         "member": f"synthetic_{offset}.o", "member_offset": 8, "member_sha256": sha(member)},
                         "_archive": archive})
    census = [{"start": BASE + offset + 2, "end": BASE + offset + 4, "successor_alignment": 4,
               "successor": BASE + offset + 4, "successor_record_sha256": local.metadata_sha(functions[index * 2 + 1]),
               "alignment_authority": "architectural_arm_state", "predecessor_section": str(offset),
               "image_sha256": sha(text[offset + 2:offset + 4])} for index, offset in enumerate((0, 8))]
    proof = {"image_sha256": sha(image), "baseline": baseline, "sections": sections,
             "external_census": census, "external_census_intervals": [[row["start"], row["end"]] for row in census],
             "internal_padding": [{"start": BASE + 22, "end": BASE + 24, "owner": BASE + 16, "section": "16",
                                   "field_alignment": 4, "field_kind": "retained_literal", "field_end": BASE + 28}]}
    archives = {section["witness"]["archive"]: section.pop("_archive") for section in sections}
    transitions = [{"start": row["start"], "end": row["end"], "kind": "padding", "owner": None,
                    "rule": "linker_alignment_fill", "section": row["predecessor_section"]} for row in census]
    transitions.append({"start": BASE + 22, "end": BASE + 24, "kind": "padding", "owner": BASE + 16,
                        "rule": "in_section_zero_alignment", "section": "16"})
    revision = {"image_sha256": sha(image), "baseline": baseline, "_section_proof": proof,
                "section_proof": {"path": "research/catalogue_v13_section_proof.json", "sha256": local.metadata_sha(proof)},
                "transitions": transitions, "resolved_anomalies": catalogue["anomalies"], "new_C_credit": 0, "adoption": False}
    return catalogue, analyze.Analyzer(image, layout), {"alignment_revision": revision, "archives": archives}


def rebind(evidence: dict) -> None:
    revision = evidence["alignment_revision"]
    revision["section_proof"]["sha256"] = local.metadata_sha(revision["_section_proof"])


class CatalogueAlignmentTests(unittest.TestCase):
    def test_public_consumer_reverses_only_exact_spans_and_preserves_every_function(self) -> None:
        baseline, analyzer, evidence = alignment_fixture()
        candidate = deepcopy(baseline)
        alignment.close_catalogue(candidate, analyzer, evidence, {})
        self.assertEqual(candidate["functions"], baseline["functions"])
        self.assertEqual(candidate["anomalies"], [])
        self.assertEqual(candidate["stats"]["creditable_bytes"], baseline["stats"]["creditable_bytes"])
        self.assertEqual(candidate["stats"]["bytes_by_range_kind"]["padding"], 6)
        restored = alignment.check_catalogue(candidate, analyzer, evidence)
        self.assertEqual(alignment.baseline_hashes(restored), alignment.baseline_hashes(baseline))
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            for path, payload in evidence["archives"].items():
                (root / path).parent.mkdir(exist_ok=True, parents=True)
                (root / path).write_bytes(payload)
            result = alignment.prove_objects(root, baseline, analyzer, evidence["alignment_revision"])
            self.assertEqual(result["complete_section_bytes"], 16)

    def test_one_disagreeing_gap_refuses_all_even_with_rebound_image_and_observation_hashes(self) -> None:
        baseline, analyzer, evidence = alignment_fixture(disagree=True)
        with self.assertRaisesRegex(ValueError, "disagreement"):
            alignment.close_catalogue(baseline, analyzer, evidence, {})
        self.assertFalse(any(row["kind"] == "padding" for row in baseline["ranges"]))

    def test_uncertified_predecessor_stays_unknown_but_still_participates_in_uniformity_veto(self) -> None:
        for disagree in (False, True):
            with self.subTest(disagree=disagree):
                baseline, analyzer, evidence = alignment_fixture(disagree=disagree)
                revision = evidence["alignment_revision"]
                proof = revision["_section_proof"]
                proof["external_census"][1]["predecessor_section"] = None
                revision["transitions"] = [row for row in revision["transitions"] if row["start"] != BASE + 10]
                rebind(evidence)
                if disagree:
                    with self.assertRaisesRegex(ValueError, "disagreement"):
                        alignment.close_catalogue(baseline, analyzer, evidence, {})
                else:
                    alignment.close_catalogue(baseline, analyzer, evidence, {})
                    self.assertIn({"start": BASE + 10, "end": BASE + 12, "kind": "unknown", "owner": None},
                                  baseline["ranges"])
                    alignment.check_catalogue(baseline, analyzer, evidence)

    def test_misaligned_successor_refuses_before_any_mutation(self) -> None:
        baseline, analyzer, evidence = alignment_fixture()
        evidence["alignment_revision"]["_section_proof"]["external_census"][0]["successor_alignment"] = 8
        rebind(evidence)
        with self.assertRaisesRegex(ValueError, "misaligned"):
            alignment.close_catalogue(baseline, analyzer, evidence, {})

    def test_overlapping_direct_read_entry_owner_and_relocation_refuse(self) -> None:
        for kind in ("read", "entry", "owner", "relocation", "sized_declaration"):
            with self.subTest(kind=kind):
                baseline, analyzer, evidence = alignment_fixture()
                proof = evidence["alignment_revision"]["_section_proof"]
                if kind == "read":
                    proof["sections"][0]["bounded_references"].append({"start": BASE + 2, "end": BASE + 4})
                elif kind == "entry":
                    baseline["functions"][1]["entries"].append({"address": BASE + 2})
                elif kind == "owner":
                    baseline["functions"].append(function(BASE + 2, 2, "thumb"))
                elif kind == "relocation":
                    proof["sections"][-1]["relocations"].append({"offset": 6, "width": 4, "type": 3, "target": BASE, "reencoded_equal": True})
                else:
                    proof["sections"][-1]["symbols"].append({"offset": 6, "size": 2, "kind": "STT_OBJECT"})
                # Rebind unrelated catalogue changes so the semantic guard, not
                # merely the immutable baseline hash, must reject the attack.
                evidence["alignment_revision"]["baseline"]["field_sha256"] = alignment.baseline_hashes(baseline)
                rebind(evidence)
                with self.assertRaises(ValueError):
                    alignment.close_catalogue(baseline, analyzer, evidence, {})

    def test_partial_section_missing_tail_and_refused_relocation_never_authorize_padding(self) -> None:
        for kind in ("partial", "missing_tail", "refused_relocation"):
            with self.subTest(kind=kind):
                baseline, analyzer, evidence = alignment_fixture()
                section = evidence["alignment_revision"]["_section_proof"]["sections"][0]
                if kind == "partial":
                    section["whole_section_reencoded_equal"] = False
                elif kind == "missing_tail":
                    section["composition"] = []
                else:
                    section["relocations"] = [{"offset": 0, "width": 2, "type": 102, "reencoded_equal": False}]
                rebind(evidence)
                with self.assertRaises(ValueError):
                    alignment.close_catalogue(baseline, analyzer, evidence, {})

    def test_successor_projection_refuses_extra_owner_split_and_statistic_injections(self) -> None:
        for kind in ("extra_owner", "extra_padding", "split", "statistics", "function_extent", "extra_anomaly"):
            with self.subTest(kind=kind):
                baseline, analyzer, evidence = alignment_fixture()
                alignment.close_catalogue(baseline, analyzer, evidence, {})
                if kind == "extra_owner":
                    baseline["ranges"][-1]["owner"] = BASE + 16
                elif kind == "extra_padding":
                    baseline["ranges"][-1]["kind"] = "padding"
                elif kind == "split":
                    span = next(row for row in baseline["ranges"] if row["kind"] == "padding")
                    index = baseline["ranges"].index(span)
                    baseline["ranges"][index:index + 1] = [{**span, "end": span["start"] + 1}, {**span, "start": span["start"] + 1}]
                elif kind == "statistics":
                    baseline["stats"]["creditable_bytes"] += 1
                elif kind == "function_extent":
                    baseline["functions"][-1]["size"] += 2
                else:
                    baseline["anomalies"].append({"address": BASE, "issue": "forged"})
                with self.assertRaises(ValueError):
                    alignment.check_catalogue(baseline, analyzer, evidence)

    def test_non_c_public_consumer_preserves_extent_and_refuses_projection_injections(self) -> None:
        image, layout = fixture([0xEB000000, 0xE12FFF1E, 0xE12FFF1E])
        analyzer = analyze.Analyzer(image, layout)
        original = analyze.build_catalogue(image, layout, closure={})
        owner = next(row for row in original["functions"] if row["address"] == BASE)
        target = next(row for row in original["functions"] if row["address"] == BASE + 8)
        section = {"index": 1, "base": BASE + 8, "size": 4, "alignment": 4,
                   "whole_section_equal": True, "executable": True, "relocated_sha256": sha(image[8:12]),
                   "image_sha256": sha(image[8:12]), "relocations": []}
        group = {"id": "synthetic", "sections": [section], "placement": [],
                 "incoming": [{"owner": BASE, "site": BASE, "isa": "arm", "target": BASE + 8,
                               "target_isa": "arm", "conditional": False,
                               "record_sha256": local.metadata_sha(owner),
                               "image_sha256": sha(image[:owner["size"]])}]}
        row = {"address": BASE + 8, "size": 4, "isa": "arm", "before_sha256": local.metadata_sha(target),
               "extent_sha256": sha(image[8:12]), "kind": "library", "identity": "synthetic",
               "coverage": [["synthetic", 1, BASE + 8, BASE + 12]], "qualification": "Synthetic certificate"}
        proof = {"image_sha256": sha(image), "baseline": {}, "groups": [group], "classifications": [row]}
        revision = {"image_sha256": sha(image), "baseline": {}, "_non_c_proof": proof,
                    "non_c_proof": {"path": "research/catalogue_v13_non_c_proof.json", "sha256": local.metadata_sha(proof)}}
        candidate = deepcopy(original)
        non_c.apply(candidate, analyzer, revision, {})
        self.assertEqual(candidate["stats"]["creditable_functions"], original["stats"]["creditable_functions"] - 1)
        self.assertEqual(candidate["stats"]["creditable_bytes"], original["stats"]["creditable_bytes"] - 4)
        self.assertEqual(non_c.reverse(candidate, analyzer, revision), original)
        for change in ("extent", "evidence", "entry"):
            with self.subTest(change=change):
                forged = deepcopy(candidate)
                function = next(row for row in forged["functions"] if row["address"] == BASE + 8)
                if change == "extent":
                    function["size"] += 4
                elif change == "evidence":
                    function["classification_evidence"]["identity"] = "unproved_alias"
                else:
                    function["entries"].append({**function["entries"][0], "address": BASE + 10})
                with self.assertRaises(ValueError):
                    non_c.reverse(forged, analyzer, revision)
        group["sections"][0]["whole_section_equal"] = False
        revision["non_c_proof"]["sha256"] = local.metadata_sha(proof)
        with self.assertRaisesRegex(ValueError, "complete section"):
            non_c.apply(deepcopy(original), analyzer, revision, {})

    def test_frozen_generator_inputs_never_resolve_to_live_mutable_paths(self) -> None:
        manifest = catalogue_frozen.load(ROOT)
        snapshot = json.loads((ROOT / "research/catalogue_v11_protection.json").read_bytes())
        for field in ("adopted_catalogue", "ledger", "names", "prepare", "selection"):
            original = snapshot[field]
            retained = manifest["inputs"][original["path"]]
            self.assertEqual((retained["size"], retained["sha256"]), (original["size"], original["sha256"]))
        for label, descriptor in manifest["inputs"].items():
            self.assertTrue(descriptor["path"].startswith(catalogue_frozen.PREFIX))
            self.assertEqual(catalogue_frozen.resolve(ROOT, label), ROOT / descriptor["path"])
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "research").mkdir()
            bad = {"revision": "catalogue-v13", "inputs": {"config/names.json": {"path": "config/names.json"}}}
            (root / catalogue_frozen.MANIFEST).write_text(json.dumps(bad))
            with self.assertRaisesRegex(ValueError, "live mutable"):
                catalogue_frozen.load(root)


if __name__ == "__main__":
    unittest.main()
