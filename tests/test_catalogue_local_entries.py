"""Complete synthetic ELF section proofs through the real public consumers."""
from __future__ import annotations

from copy import deepcopy
import hashlib
import json
from pathlib import Path
import struct
import tempfile
import unittest

from tools import analyze, catalogue_closure, catalogue_local_entries as local
from tests.test_analyze import BASE, fixture
from tests.test_catalogue_protection import run_catalogue, synthetic_project, write_json


def sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def synthetic_member(section: bytes, local_name: str, anchor_name: str) -> bytes:
    """A real minimal AAELF32 object, not a mocked object-reader result."""
    strings = bytearray(b"\0")
    indices = {}
    for name in (".text", local_name, "$t", "$d", anchor_name, "__printf"):
        indices[name] = len(strings)
        strings.extend(name.encode() + b"\0")
    symbols = b"\0" * 16
    for name, value, size, info, section_index in ((".text", 0, 0, 3, 1), (local_name, 1, 10, 2, 1),
                                                   ("$t", 0, 0, 0, 1), ("$d", 42, 0, 0, 1),
                                                   (anchor_name, 11, 32, 0x12, 1), ("__printf", 0, 0, 0x10, 0)):
        symbols += struct.pack("<IIIBBH", indices[name], value, size, info, 0, section_index)
    names = b"\0.text\0.rel.text\0.symtab\0.strtab\0.shstrtab\0"
    relocs = struct.pack("<4I", 34, (6 << 8) | 10, 44, (2 << 8) | 3)
    blobs = [b"", section, relocs, symbols, bytes(strings), names]
    data, offsets = bytearray(52), []
    for blob in blobs:
        data.extend(b"\0" * (-len(data) % 4))
        offsets.append(len(data))
        data.extend(blob)
    data.extend(b"\0" * (-len(data) % 4))
    section_offset = len(data)
    headers = [(0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
               (1, 1, 6, 0, offsets[1], len(section), 0, 0, 4, 0),
               (7, 9, 0, 0, offsets[2], len(relocs), 3, 1, 4, 8),
               (17, 2, 0, 0, offsets[3], len(symbols), 4, 5, 4, 16),
               (25, 3, 0, 0, offsets[4], len(strings), 0, 0, 1, 0),
               (33, 3, 0, 0, offsets[5], len(names), 0, 0, 1, 0)]
    for header in headers:
        data.extend(struct.pack("<10I", *header))
    ident = b"\x7fELF\x01\x01\x01" + b"\0" * 9
    data[:52] = struct.pack("<16sHHIIIIIHHHHHH", ident, 1, 40, 1, 0, 0, section_offset,
                            0x05000000, 52, 0, 0, 40, len(headers), 5)
    return bytes(data)


def synthetic_archive(member: bytes, name: str) -> bytes:
    payload = name.encode() + member
    header = f"{'#1/' + str(len(name)):<16}{0:<12}{0:<6}{0:<6}{0o100644:<8}{len(payload):<10}`\n".encode()
    return b"!<arch>\n" + header + payload + b"\n" * (len(payload) % 2)


def synthetic_image(state: int) -> tuple[bytes, object, bytes]:
    base, external = BASE + 32, BASE + 96
    delta = base + 10 - BASE - 8
    blx = 0xFA000000 | (((delta >> 1) & 1) << 24) | ((delta >> 2) & 0xFFFFFF)
    root = struct.pack("<8I", blx, *([0xE1A00000] * 6), 0xE12FFF1E)
    body = bytearray(struct.pack("<5H", 0x2007, 0x3001, 0x3002, 0x3003, 0x4770))
    body.extend(struct.pack("<12H", 0xB510, state, 0x4600, 0x4600, 0x4600,
                            0x4C05, 0x4600, 0x447C, 0xB650, 0x4600, 0x4600, 0x4600))
    offset = external - (base + 34) - 4
    body.extend(struct.pack("<HH", 0xF000 | ((offset >> 12) & 0x7FF), 0xF800 | ((offset >> 1) & 0x7FF)))
    body.extend(struct.pack("<3H", 0x4600, 0xBD10, 0xFFFF))
    body.extend(struct.pack("<I", (((base + 16) | 1) - (base + 44)) & 0xFFFFFFFF))
    text = root + body + b"\xff" * 16 + struct.pack("<HH", 0x4770, 0xFFFF)
    image, layout = fixture(list(struct.unpack(f"<{len(text) // 4}I", text)))
    return image, layout, bytes(body)


def local_fixture() -> dict:
    base, local_name, anchor_name = BASE + 32, "synthetic_local", "synthetic_common"
    image, layout, section = synthetic_image(0xB658)
    le_image, _, _ = synthetic_image(0xB650)
    raw = analyze.build_catalogue(image, layout, closure={})
    functions = {row["address"]: row for row in raw["functions"]}
    anchor = functions[base + 10]
    object_section = bytearray(section)
    struct.pack_into("<HH", object_section, 34, 0xF7FF, 0xFFFE)
    struct.pack_into("<I", object_section, 44, 16)
    member_name = "synthetic_common.o"
    member = synthetic_member(bytes(object_section), local_name, anchor_name)
    archive = synthetic_archive(member, member_name)
    archive_name = ".cache/armlib/5.04-b82/lib/armlib/c_5.l"
    classification = {"address": base + 10, "size": 38, "isa": "thumb", "kind": "library", "basis": "known_library",
                      "identity": anchor_name, "image_sha256": sha(image),
                      "original_extent_sha256": sha(section[10:]), "known_library_identity": "Synthetic real ELF declaration",
                      "evidence": "Synthetic complete ELF and independent direct caller",
                      "library_witness": {"archive": archive_name, "archive_member": member_name, "member_offset": 8,
                                          "member_sha256": sha(member), "symbol": anchor_name}}
    le_reference = analyze.build_catalogue(le_image, layout, closure={})
    previous = next(row for row in le_reference["functions"] if row["address"] == base)
    witness = next(row for row in le_reference["pc_relative_pointer_witnesses"] if row["target"] == base)
    current = analyze.build_catalogue(image, layout, classifications=[classification], closure={})
    source = next(row for row in current["functions"] if row["address"] == base + 10)
    # Classification is an independent old owner; no new local entry is reused
    # from the LE reference, which remains only the historical withdrawn claim.
    for row in le_reference["functions"]:
        if row["address"] == base + 10:
            row["kind"] = "library"
    analyzer = analyze.Analyzer(image, layout)
    reasons = []
    for _, decoded in catalogue_closure._decoded_functions({"functions": [source]}, analyzer):
        for site, detail in decoded.items():
            flow = analyzer.decode(site, "thumb")
            if catalogue_closure._data_endian_effect(detail, "thumb") is None:
                reasons.append({"site": site, "reason": "uncertified_processor_effect"})
            if flow is None:
                reasons.append({"site": site, "reason": "missing_flow"})
            elif flow.switch is not None or flow.indirect and not flow.call:
                reasons.append({"site": site, "reason": "unresolved_computed_transfer"})
    reassessment = {"image_sha256": sha(image), "prior_proposal": {"sha256": local.metadata_sha(le_reference)},
                    "prior_owners": [{key: row[key] for key in ("address", "size", "isa", "kind")}
                                     for row in le_reference["functions"]],
                    "prior_entries": [{"owner": row["address"], **entry} for row in le_reference["functions"] for entry in row["entries"]],
                    "prior_unknown_ranges": [[row["start"], row["end"]] for row in le_reference["ranges"] if row["kind"] == "unknown"],
                    "prior_anomalies": le_reference["anomalies"], "withdrawn_entry_proposals": [
                        {"address": base, "prior_function": previous, "prior_record_sha256": local.metadata_sha(previous),
                         "original_extent_sha256": sha(section[:10]), "prior_witness": witness,
                         "source_record_sha256": local.metadata_sha(source), "negative_certificate_reasons": reasons}]}
    baseline = analyze.build_catalogue(image, layout, classifications=[classification], closure={"entry_revision": reassessment})
    authentication = {"address": base + 10, "size": 38, "isa": "thumb", "authenticated": True,
                      "whole_body_identical": True, "selected_alias_groups": [[anchor_name]],
                      "incoming_library_symbol_witnesses": [{"caller": BASE, "offset": 0, "symbol": anchor_name}]}
    parts = []
    for lo, hi, role in ((0, 10, "local_declared_function"), (10, 42, "anchor_declared_function"),
                          (42, 44, "mapped_data_halfword_unclassified"), (44, 48, "same_section_relative_relocation")):
        parts.append({"offset": lo, "end_offset": hi, "role": role, "object_sha256": sha(object_section[lo:hi]),
                      "image_sha256": sha(section[lo:hi]), "relocated_sha256": sha(section[lo:hi]), "reencoded_equal": True})
    record = {"address": base, "size": 10, "isa": "thumb", "symbol": local_name,
              "archive": archive_name, "archive_sha256": sha(archive), "member": member_name,
              "member_offset": 8, "member_sha256": sha(member), "fragment_sha256": sha(section[:10]),
              "unclassified_halfword": [base + 42, base + 44],
              "anchor": {"address": base + 10, "symbol_offset": 10, "declared_size": 32, "size": 38,
                         "isa": "thumb", "symbol": anchor_name, "record_sha256": local.metadata_sha(source),
                         "classification_sha256": local.metadata_sha(classification), "image_sha256": sha(section[10:]),
                         "authentication_sha256": local.metadata_sha(authentication), "incoming_references": [
                             {"caller": BASE, "site": BASE, "isa": "arm", "caller_record_sha256": local.metadata_sha(functions[BASE]),
                              "caller_image_sha256": sha(image[:functions[BASE]["size"]])}]},
              "section": {"index": 1, "name": ".text", "size": 48, "alignment": 4, "allocated": True,
                          "executable": True, "writable": False, "nobits": False,
                          "mapping": [[0, "$t"], [42, "$d"]], "composition": parts,
                          "object_sha256": sha(object_section), "image_sha256": sha(section),
                          "relocated_sha256": sha(section), "whole_section_reencoded_equal": True,
                          "symbols": [{"name": ".text", "bind": "STB_LOCAL", "kind": "STT_SECTION", "value": 0, "offset": 0, "size": 0},
                                      {"name": local_name, "bind": "STB_LOCAL", "kind": "STT_FUNC", "value": 1, "offset": 0, "size": 10},
                                      {"name": anchor_name, "bind": "STB_GLOBAL", "kind": "STT_FUNC", "value": 11, "offset": 10, "size": 32}],
                          "relocations": [{"offset": 34, "type": 10, "symbol": "__printf", "symbol_section": "SHN_UNDEF",
                                           "symbol_value": 0, "symbol_size": 0, "addend": -4, "target": BASE + 96,
                                           "thumb": True, "agreement": "observed_external_target_not_API_identity", "reencoded_equal": True},
                                          {"offset": 44, "type": 3, "symbol": local_name, "symbol_section": 1,
                                           "symbol_value": 1, "symbol_size": 10, "addend": 16, "target": base,
                                           "thumb": True, "agreement": "same_section_definition", "reencoded_equal": True}]}}
    baseline_descriptor = {"revision": "catalogue-v11-r4", "adopted": False, "proposal_sha256": local.metadata_sha(baseline),
                           "field_sha256": local.baseline_hashes(baseline)}
    proof = {"image_sha256": sha(image), "exheader_sha256": sha(b"synthetic layout"),
             "baseline": baseline_descriptor, "records": [record]}
    revision = {"revision": "catalogue-v12", "image_sha256": sha(image), "exheader_sha256": proof["exheader_sha256"],
                "object_proof": {"path": "research/catalogue_v12_object_proof.json", "sha256": local.metadata_sha(proof)},
                "baseline": baseline_descriptor, "new_C_credit": 0, "adoption": False, "policy": local.POLICY,
                "transitions": [{"address": base, "size": 10, "isa": "thumb", "symbol": local_name,
                                 "from": {"kind": "unknown", "owner": None},
                                 "to": {"kind": "library", "physical_kind": "function", "owner": base}}],
                "preserved_unknown_ranges": [[base + 42, base + 44], [base + 48, BASE + 96]],
                "_object_proof": proof, "_anchor_authentications": {str(base + 10): authentication}}
    return {"image": image, "layout": layout, "baseline": baseline, "classification": classification,
            "closure": {"entry_revision": reassessment, "local_entry_revision": revision},
            "member": member, "archive": archive, "previous": previous}


def build(fixture_data: dict) -> dict:
    return analyze.build_catalogue(fixture_data["image"], fixture_data["layout"],
                                   classifications=[fixture_data["classification"]], closure=fixture_data["closure"])


class CatalogueLocalEntryTests(unittest.TestCase):
    def test_pool_independent_public_builder_retains_the_complete_negative_stage(self) -> None:
        data = local_fixture()
        result = build(data)
        base = BASE + 32
        actual = next(row for row in result["functions"] if row["address"] == base)
        self.assertEqual((actual["kind"], actual["size"], actual["evidence"]), ("library", 10, [local.ENTRY_EVIDENCE]))
        self.assertNotEqual(actual, data["previous"])
        self.assertEqual(analyze.validate_catalogue(result, data["image"], data["layout"]), [])
        self.assertEqual(result["closure_evidence"]["provisional_entry_reassessment"],
                         data["baseline"]["closure_evidence"]["provisional_entry_reassessment"])
        self.assertEqual(result["closure_evidence"]["read_vetoes"], data["baseline"]["closure_evidence"]["read_vetoes"])
        self.assertFalse(any(row["target"] == base for row in result["pc_relative_pointer_witnesses"]))
        self.assertEqual(result["stats"]["creditable_bytes"], data["baseline"]["stats"]["creditable_bytes"])
        self.assertEqual(result["stats"]["creditable_functions"], data["baseline"]["stats"]["creditable_functions"])
        restored = local.check_catalogue(result, analyze.Analyzer(data["image"], data["layout"]), data["closure"])
        self.assertEqual(local.baseline_hashes(restored), local.baseline_hashes(data["baseline"]))

    def test_complete_real_synthetic_object_is_rechecked_without_a_search(self) -> None:
        data = local_fixture()
        revision = data["closure"]["local_entry_revision"]
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            path = root / revision["_object_proof"]["records"][0]["archive"]
            path.parent.mkdir(parents=True)
            path.write_bytes(data["archive"])
            result = local.prove_objects(root, data["baseline"], analyze.Analyzer(data["image"], data["layout"]), revision)
            self.assertEqual(result["complete_section_bytes"], 48)
            path.write_bytes(data["archive"] + b"changed")
            with self.assertRaisesRegex(ValueError, "archive digest"):
                local.prove_objects(root, data["baseline"], analyze.Analyzer(data["image"], data["layout"]), revision)

    def test_public_builder_refuses_incomplete_or_misbound_certificates(self) -> None:
        for change in ("member", "member_offset", "member_hash", "anchor", "mapping", "missing_relocation",
                       "relative_symbol", "relative_addend", "external_binding", "size", "tag", "composition", "section_hash"):
            with self.subTest(change=change):
                data = local_fixture()
                revision = data["closure"]["local_entry_revision"]
                record = revision["_object_proof"]["records"][0]
                if change == "member":
                    record["member"] = "other.o"
                elif change == "member_offset":
                    record["member_offset"] += 2
                elif change == "member_hash":
                    record["member_sha256"] = sha(b"wrong member")
                elif change == "anchor":
                    record["anchor"]["symbol"] = "unproved_alias"
                elif change == "mapping":
                    record["section"]["mapping"] = [[0, "$t"]]
                elif change == "missing_relocation":
                    record["section"]["relocations"].pop()
                elif change == "relative_symbol":
                    record["section"]["relocations"][1]["symbol_section"] = "SHN_UNDEF"
                elif change == "relative_addend":
                    record["section"]["relocations"][1]["addend"] = 0
                elif change == "external_binding":
                    record["section"]["relocations"][0]["agreement"] = "authenticated_API_identity"
                elif change == "size":
                    record["size"] = 12
                elif change == "tag":
                    record["section"]["symbols"][1]["value"] = 0
                elif change == "composition":
                    record["section"]["composition"].pop(2)
                else:
                    record["section"]["image_sha256"] = sha(b"wrong section")
                # Exercise the semantic checks, not merely the enclosing digest.
                revision["object_proof"]["sha256"] = local.metadata_sha(revision["_object_proof"])
                with self.assertRaises(ValueError):
                    build(data)

    def test_public_consumer_refuses_old_positive_records_and_unrelated_changes(self) -> None:
        data = local_fixture()
        actual = build(data)
        for change in ("old_pointer_record", "grow_anchor", "classify_hole", "classify_unresolved", "missing_veto", "statistics"):
            with self.subTest(change=change):
                candidate = deepcopy(actual)
                if change == "old_pointer_record":
                    candidate["functions"] = [deepcopy(data["previous"]) if row["address"] == BASE + 32 else row
                                              for row in candidate["functions"]]
                elif change == "grow_anchor":
                    next(row for row in candidate["functions"] if row["address"] == BASE + 42)["size"] += 2
                elif change in ("classify_hole", "classify_unresolved"):
                    address = BASE + (74 if change == "classify_hole" else 80)
                    next(row for row in candidate["ranges"] if row["start"] == address)["kind"] = "rodata"
                elif change == "missing_veto":
                    candidate["closure_evidence"]["read_vetoes"] = []
                else:
                    candidate["stats"]["creditable_bytes"] += 10
                with self.assertRaises(ValueError):
                    local.check_catalogue(candidate, analyze.Analyzer(data["image"], data["layout"]), data["closure"])

    def test_public_consumer_projects_only_the_exact_ten_byte_entry(self) -> None:
        data = local_fixture()
        candidate = build(data)
        base = BASE + 32
        self.assertEqual([row for row in candidate["ranges"] if row["owner"] == base],
                         [{"start": base, "end": base + 10, "kind": "function", "owner": base}])
        restored = local.check_catalogue(candidate, analyze.Analyzer(data["image"], data["layout"]), data["closure"])
        self.assertEqual({key: restored[key] for key in local.BASELINE_FIELDS},
                         {key: data["baseline"][key] for key in local.BASELINE_FIELDS})

    def test_public_consumer_refuses_extra_entry_owned_unknown_intervals(self) -> None:
        data = local_fixture()
        actual = build(data)
        base = BASE + 32
        for interval in ((BASE + 74, BASE + 76), (BASE + 80, BASE + 96)):
            for kind in ("function", "unknown"):
                with self.subTest(interval=interval, kind=kind):
                    candidate = deepcopy(actual)
                    row = next(row for row in candidate["ranges"] if (row["start"], row["end"]) == interval)
                    self.assertEqual((row["kind"], row["owner"]), ("unknown", None))
                    row.update(kind=kind, owner=base)
                    # Keep the real ten-byte span, every complete record and all
                    # statistics unchanged: projection must not erase this fraud.
                    self.assertEqual(candidate["functions"], actual["functions"])
                    self.assertEqual(candidate["stats"], actual["stats"])
                    self.assertIn({"start": base, "end": base + 10, "kind": "function", "owner": base},
                                  candidate["ranges"])
                    with self.assertRaisesRegex(ValueError, "ownership outside its exact declared span"):
                        local.check_catalogue(candidate, analyze.Analyzer(data["image"], data["layout"]), data["closure"])

    def test_inherited_refusal_is_checked_before_new_entries_and_protected_entries_cannot_be_replaced(self) -> None:
        data = local_fixture()
        data["closure"]["entry_revision"]["withdrawn_entry_proposals"][0]["negative_certificate_reasons"].append(
            {"site": BASE, "reason": "invented"})
        with self.assertRaisesRegex(ValueError, "v11-r3 negative certificate reasons"):
            build(data)
        data = local_fixture()
        with self.assertRaises(ValueError):
            analyze.build_catalogue(data["image"], data["layout"], classifications=[data["classification"]],
                                     protected_functions={BASE + 32: data["previous"]}, closure=data["closure"])

    def test_real_cli_refuses_changed_v12_parent_proof_and_baseline(self) -> None:
        for change in ("parent", "proof", "baseline"):
            with self.subTest(change=change), tempfile.TemporaryDirectory() as temporary:
                root = Path(temporary)
                synthetic_project(root)
                passed = run_catalogue(root)
                self.assertEqual(passed.returncode, 0, passed.stderr)
                revision_path = root / "research/catalogue_v12_evidence.json"
                revision = json.loads(revision_path.read_bytes())
                if change == "parent":
                    revision["parent_evidence"]["sha256"] = sha(b"wrong parent")
                elif change == "proof":
                    revision["object_proof"]["sha256"] = sha(b"wrong proof")
                else:
                    revision["baseline"]["field_sha256"]["functions"] = sha(b"wrong baseline")
                write_json(revision_path, revision)
                refused = run_catalogue(root, "refused.json")
                self.assertEqual(refused.returncode, 1, refused.stderr)
                self.assertFalse((root / "refused.json").exists())


if __name__ == "__main__":
    unittest.main()
