"""Image-bound v13 alignment evidence; v12 and every earlier rule stay unchanged.

Only complete section certificates authorize geometry. Uniformity is an image-wide
veto, not a substitute for a missing predecessor proof. No payload is serialized.
"""
from __future__ import annotations

from collections import Counter
from copy import deepcopy
import hashlib
import json
from pathlib import Path

if __package__:
    from . import catalogue_local_entries as local
    from . import catalogue_runtime_data as runtime
    from . import catalogue_non_c as non_c
else:
    import catalogue_local_entries as local
    import catalogue_runtime_data as runtime
    import catalogue_non_c as non_c

POLICY = "complete_sections_exact_alignment_uniform_image_fill_v1"
RECEIPT = "section_alignment_v13"
IGNORED_FIELDS = {"generator", "revision_review"}


def baseline_hashes(catalogue: dict) -> dict[str, str]:
    return {key: local.metadata_sha(value) for key, value in catalogue.items() if key not in IGNORED_FIELDS}


def load_inputs(root: Path, parent_path: Path, evidence: dict) -> tuple[dict, dict[str, str]]:
    path = root / "research/catalogue_v13_evidence.json"
    content = path.read_bytes()
    revision = json.loads(content)
    parent = revision["parent_evidence"]
    if (root / parent["path"] != parent_path
            or runtime._sha(parent_path.read_bytes()) != parent["sha256"]
            or revision["revision"] != "catalogue-v13" or revision["policy"] != POLICY
            or revision["image_sha256"] != evidence["image_sha256"]
            or revision["new_C_credit"] != 0 or revision["adoption"] is not False
            or revision["baseline"]["revision"] != "catalogue-v12-r2"
            or revision["baseline"]["adopted"] is not False):
        raise ValueError("v13 alignment evidence differs from its immutable v12 parent/image/policy")
    descriptor = revision["section_proof"]
    if descriptor["path"] != "research/catalogue_v13_section_proof.json":
        raise ValueError("v13 requires its explicit frozen complete-section proof")
    proof_content = (root / descriptor["path"]).read_bytes()
    if runtime._sha(proof_content) != descriptor["sha256"]:
        raise ValueError("v13 complete-section proof digest differs")
    revision["_section_proof"] = json.loads(proof_content)
    _proof(revision)
    inputs = {str(path.relative_to(root)): runtime._sha(content), descriptor["path"]: descriptor["sha256"]}
    if "non_c_proof" in revision:
        inputs.update(non_c.load_inputs(root, revision))
    if "residue" in revision:
        retained = revision["residue"]
        if (retained["path"] != "research/catalogue_v13_residue.json"
                or runtime._sha((root / retained["path"]).read_bytes()) != retained["sha256"]):
            raise ValueError("v13 residue differs from its exact retained evidence")
        inputs[retained["path"]] = retained["sha256"]
    return revision, inputs


def _proof(revision: dict) -> dict:
    proof = revision["_section_proof"]
    if (local.metadata_sha(proof) != revision["section_proof"]["sha256"]
            or proof["image_sha256"] != revision["image_sha256"]
            or proof["baseline"] != revision["baseline"]):
        raise ValueError("v13 section proof is not bound to this complete v12 baseline/image")
    if len({section["id"] for section in proof["sections"]}) != len(proof["sections"]):
        raise ValueError("v13 section proof duplicates a section")
    return proof


def _check_baseline(catalogue: dict, revision: dict) -> None:
    if (catalogue["image_sha256"] != revision["image_sha256"]
            or baseline_hashes(catalogue) != revision["baseline"]["field_sha256"]):
        raise ValueError("v13 requires the exact independently reviewed v12 baseline")


def _overlap(first: int, end: int, lo: int, hi: int) -> bool:
    return first < hi and lo < end


def _section(catalogue: dict, analyzer, section: dict) -> None:
    base, size, alignment = section["base"], section["size"], section["alignment"]
    if (size <= 0 or alignment <= 0 or alignment & (alignment - 1) or base % alignment
            or not section["complete"] or not section["whole_section_reencoded_equal"]
            or section["image_sha256"] != section["relocated_sha256"]
            or not section["allocated"] or not section["executable"]
            or section["writable"] or section["nobits"]
            or runtime._sha(runtime._read(analyzer, base, base + size)) != section["image_sha256"]):
        raise ValueError("v13 requires a complete exactly reencoded section with proven alignment")
    functions = {row["address"]: row for row in catalogue["functions"]}
    if not section["bindings"]:
        raise ValueError("v13 complete section has no retained catalogue placement binding")
    for binding in section["bindings"]:
        function = functions.get(binding["address"])
        if function is None or local.metadata_sha(function) != binding["record_sha256"]:
            raise ValueError("v13 retained complete section placement/owner differs")
    cursor = 0
    for part in section["composition"]:
        lo, hi = part["offset"], part["end_offset"]
        if (lo != cursor or not lo < hi <= size or not part["reencoded_equal"]
                or part["image_sha256"] != part["relocated_sha256"]
                or runtime._sha(runtime._read(analyzer, base + lo, base + hi)) != part["image_sha256"]):
            raise ValueError("v13 complete section partition is partial, overlapping or unequal")
        cursor = hi
    if cursor != size:
        raise ValueError("v13 complete section partition omits its tail")
    for symbol in section["symbols"]:
        if symbol["size"] and symbol["kind"] in ("STT_FUNC", "STT_OBJECT"):
            if not 0 <= symbol["offset"] < symbol["offset"] + symbol["size"] <= size:
                raise ValueError("v13 sized declaration crosses its complete section")
    for relocation in section["relocations"]:
        width = 2 if relocation["type"] in (102, 103) else 4
        if relocation["width"] != width or not 0 <= relocation["offset"] < relocation["offset"] + width <= size:
            raise ValueError("v13 relocation field crosses its complete section")
        if not relocation["reencoded_equal"]:
            raise ValueError("v13 complete section contains a refused relocation")


def _no_reference(catalogue: dict, sections: list[dict], first: int, end: int, *, external: bool) -> None:
    for function in catalogue["functions"]:
        if any(first <= entry["address"] < end for entry in function["entries"]):
            raise ValueError("v13 alignment interval overlaps a catalogue entry")
        if external and _overlap(first, end, function["address"], function["address"] + function["size"]):
            raise ValueError("v13 linker alignment interval overlaps a catalogue owner")
        if any(_overlap(first, end, lo, hi) for key in ("code_ranges", "pools", "literals") for lo, hi in function[key]):
            raise ValueError("v13 alignment interval overlaps retained code or a bounded direct read")
    for section in sections:
        for relocation in section["relocations"]:
            site = section["base"] + relocation["offset"]
            if _overlap(first, end, site, site + relocation["width"]):
                raise ValueError("v13 alignment interval overlaps a relocation field")
            if first <= relocation["target"] < end:
                raise ValueError("v13 alignment interval has an incoming relocation reference")
        for reference in section["bounded_references"]:
            if _overlap(first, end, reference["start"], reference["end"]):
                raise ValueError("v13 alignment interval overlaps a bounded direct read")
    for row in catalogue.get("shared_literals", []) + catalogue.get("literal_conflicts", []):
        lo = row.get("start", row.get("target"))
        if lo is not None and _overlap(first, end, lo, row["end"]):
            raise ValueError("v13 alignment interval overlaps a shared/conflicting bounded reference")
    for row in catalogue.get("closure_evidence", {}).get("read_footprints", []):
        if _overlap(first, end, row["start"], row["end"]):
            raise ValueError("v13 alignment interval overlaps an exact architectural read")
    for row in catalogue.get("pc_relative_pointer_witnesses", []):
        if first <= row["target"] < end:
            raise ValueError("v13 alignment interval has an incoming pointer reference")
    for key in ("protected_entry_observations", "protected_start_observations"):
        for row in catalogue.get(key, []):
            if first <= row.get("address", row.get("target", -1)) < end:
                raise ValueError("v13 alignment interval overlaps a protected observation")


def _transitions(catalogue: dict, analyzer, revision: dict) -> tuple[list[dict], list[dict]]:
    proof = _proof(revision)
    sections = {section["id"]: section for section in proof["sections"]}
    if runtime._sha(analyzer.image) != revision["image_sha256"]:
        raise ValueError("v13 original image digest differs")
    for section in sections.values():
        _section(catalogue, analyzer, section)
    # Every placement in the sealed image-wide census participates, including
    # observations whose predecessor is NOT complete and cannot close.
    census = proof["external_census"]
    unknown = {(row["start"], row["end"]) for row in catalogue["ranges"] if row["kind"] == "unknown" and row["owner"] is None}
    if (len({(row["start"], row["end"]) for row in census}) != len(census)
            or {(row["start"], row["end"]) for row in census} != set(map(tuple, proof["external_census_intervals"]))):
        raise ValueError("v13 uniform fill census is incomplete or duplicates a placement")
    fill = None
    for row in census:
        first, end = row["start"], row["end"]
        alignment = row["successor_alignment"]
        if (alignment < 2 or alignment & (alignment - 1) or not 0 < end - first < alignment
                or end % alignment or end - first != -first % alignment or (first, end) not in unknown):
            raise ValueError("v13 successor is misaligned or gap is not its exact alignment deficit")
        value = runtime._read(analyzer, first, end)
        if runtime._sha(value) != row["image_sha256"]:
            raise ValueError("v13 fill observation hash differs")
        if fill is None:
            fill = value
        elif value != fill:
            raise ValueError("v13 image-wide fill disagreement refuses every linker gap")
    transitions, removed = [], []
    functions = {row["address"]: row for row in catalogue["functions"]}
    for row in census:
        if row["predecessor_section"] is None:
            continue
        section = sections[row["predecessor_section"]]
        first, end = row["start"], row["end"]
        if section["base"] + section["size"] != first:
            raise ValueError("v13 gap does not start at the proven complete predecessor end")
        successor = functions.get(row["successor"])
        if successor is None or successor["address"] != end or local.metadata_sha(successor) != row["successor_record_sha256"]:
            raise ValueError("v13 aligned successor binding differs")
        if row["alignment_authority"] == "architectural_arm_state":
            if successor["isa"] != "arm" or row["successor_alignment"] != 4 or not any(lo == end for lo, _ in successor["code_ranges"]):
                raise ValueError("v13 successor does not prove architectural ARM word alignment")
        elif row["alignment_authority"] == "complete_object_section":
            following = sections[row["successor_section"]]
            if following["base"] != end or following["alignment"] != row["successor_alignment"]:
                raise ValueError("v13 successor object does not prove exact section alignment")
        else:
            raise ValueError("v13 successor alignment has no admitted authority")
        _no_reference(catalogue, list(sections.values()), first, end, external=True)
        transitions.append({"start": first, "end": end, "kind": "padding", "owner": None,
                            "rule": "linker_alignment_fill", "section": section["id"]})
    for row in proof["internal_padding"]:
        section = sections[row["section"]]
        first, end, alignment = row["start"], row["end"], row["field_alignment"]
        lo, hi = first - section["base"], end - section["base"]
        if (alignment <= 1 or alignment & (alignment - 1) or not 0 < end - first < alignment
                or end % alignment or end - first != -first % alignment
                or not 0 <= lo < hi < section["size"] or (first, end) not in unknown
                or any(runtime._read(analyzer, first, end))
                or not any(offset <= lo and kind == "$d" and (index + 1 == len(section["mapping"])
                           or section["mapping"][index + 1][0] >= hi)
                           for index, (offset, kind) in enumerate(section["mapping"]))):
            raise ValueError("v13 internal padding is not exact zero mapped-data alignment before a field")
        if any(symbol["size"] and symbol["kind"] in ("STT_FUNC", "STT_OBJECT")
               and _overlap(lo, hi, symbol["offset"], symbol["offset"] + symbol["size"])
               for symbol in section["symbols"]):
            raise ValueError("v13 internal padding overlaps a sized declaration")
        if not any(symbol["kind"] == "STT_FUNC" and symbol["offset"] + symbol["size"] == lo
                   for symbol in section["symbols"]):
            raise ValueError("v13 internal alignment lacks the exact preceding declaration end")
        owner = functions.get(row["owner"])
        if owner is None or row["owner"] not in {binding["address"] for binding in section["bindings"]}:
            raise ValueError("v13 internal padding has no retained section catalogue owner")
        if row["field_kind"] == "REL32":
            if alignment != 4 or not any(rel["offset"] == hi and rel["type"] == 3 and rel["width"] == 4 for rel in section["relocations"]):
                raise ValueError("v13 internal padding does not end at the aligned REL32 field")
        elif row["field_kind"] == "retained_literal":
            if alignment != 4 or not any(a == end and b == row["field_end"] for a, b in owner["pools"] + owner["literals"]):
                # A multi-declaration input section may have a separate unowned
                # mapped tail, never an extension of the final 10-byte function.
                expected = {"start": end, "end": row["field_end"], "kind": "rodata", "owner": None}
                if alignment != 4 or expected not in catalogue["ranges"]:
                    raise ValueError("v13 internal padding does not end at the exact retained literal field")
        else:
            raise ValueError("v13 internal padding next field lacks an alignment authority")
        _no_reference(catalogue, list(sections.values()), first, end, external=False)
        transitions.append({"start": first, "end": end, "kind": "padding", "owner": row["owner"],
                            "rule": "in_section_zero_alignment", "section": section["id"]})
    for owner in sorted({row["owner"] for row in proof["internal_padding"]}):
        function = functions[owner]
        coverage = function["code_ranges"] + function["pools"] + function["literals"]
        padding = [[row["start"], row["end"]] for row in transitions if row["owner"] == owner
                   and function["address"] <= row["start"] < row["end"] <= function["address"] + function["size"]]
        pieces = sorted(coverage + padding)
        cursor = function["address"]
        for lo, hi in pieces:
            if lo != cursor:
                break
            cursor = hi
        else:
            anomaly = {"address": owner, "issue": "unexplained_function_bytes"}
            if cursor == function["address"] + function["size"] and anomaly in catalogue["anomalies"]:
                removed.append(anomaly)
    transitions.sort(key=lambda row: row["start"])
    if transitions != revision["transitions"] or removed != revision["resolved_anomalies"]:
        raise ValueError("v13 transitions/anomaly explanations differ from the exact complete evidence")
    return transitions, removed


def _stats(catalogue: dict) -> None:
    stats = catalogue["stats"]
    stats["bytes_by_range_kind"] = dict(sorted(Counter({kind: sum(row["end"] - row["start"] for row in catalogue["ranges"] if row["kind"] == kind)
                                                       for kind in {row["kind"] for row in catalogue["ranges"]}}).items()))
    stats["unknown_ranges"] = sum(row["kind"] == "unknown" for row in catalogue["ranges"])
    stats["anomalies"] = len(catalogue["anomalies"])
    stats["anomalies_by_issue"] = dict(sorted(Counter(row["issue"] for row in catalogue["anomalies"]).items()))


def _receipt(revision: dict) -> dict:
    return {key: deepcopy(revision[key]) for key in ("baseline", "section_proof", "transitions", "resolved_anomalies", "new_C_credit", "adoption")}


def close_catalogue(catalogue: dict, analyzer, evidence: dict, protected: dict[int, dict]) -> None:
    revision = evidence["alignment_revision"]
    _check_baseline(catalogue, revision)
    transitions, removed = _transitions(catalogue, analyzer, revision)
    for row in transitions:
        if any(_overlap(row["start"], row["end"], owner["address"], owner["address"] + owner["size"])
               and row["owner"] != owner["address"] for owner in protected.values()):
            raise ValueError("v13 padding would overlap a foreign protected owner")
    replacements = {(row["start"], row["end"]): row for row in transitions}
    catalogue["ranges"] = [{**row, "kind": "padding", "owner": replacements[(row["start"], row["end"])]["owner"]}
                           if (row["start"], row["end"]) in replacements else row for row in catalogue["ranges"]]
    catalogue["anomalies"] = [row for row in catalogue["anomalies"] if row not in removed]
    _stats(catalogue)
    catalogue["closure_evidence"][RECEIPT] = _receipt(revision)
    if "non_c_proof" in revision:
        non_c.apply(catalogue, analyzer, revision, protected)
    check_catalogue(catalogue, analyzer, evidence)


def check_catalogue(catalogue: dict, analyzer, evidence: dict) -> dict:
    """Reverse exact complete physical records only, then prove the whole baseline.

    No projection based on owner identity: unrelated owner injection, split/merged
    spans, extra padding and statistic changes survive projection and are refused.
    """
    revision = evidence["alignment_revision"]
    _proof(revision)
    baseline = (non_c.reverse(catalogue, analyzer, revision) if "non_c_proof" in revision else deepcopy(catalogue))
    for transition in revision["transitions"]:
        span = {key: transition[key] for key in ("start", "end", "kind", "owner")}
        if baseline["ranges"].count(span) != 1:
            raise ValueError("v13 successor lacks an exact complete padding transition")
        index = baseline["ranges"].index(span)
        baseline["ranges"][index] = {**span, "kind": "unknown", "owner": None}
    if catalogue["closure_evidence"].get(RECEIPT) != _receipt(revision):
        raise ValueError("v13 successor alignment receipt differs")
    del baseline["closure_evidence"][RECEIPT]
    if any(row in catalogue["anomalies"] for row in revision["resolved_anomalies"]):
        raise ValueError("v13 explained anomaly was not removed exactly")
    baseline["anomalies"] = sorted(baseline["anomalies"] + deepcopy(revision["resolved_anomalies"]), key=lambda row: (row["address"], row["issue"]))
    # Reverse only the declared accounting deltas; recomputing all statistics
    # would erase an injected statistics error before the baseline commitment.
    stats = baseline["stats"]
    size = sum(row["end"] - row["start"] for row in revision["transitions"])
    if size:
        stats["bytes_by_range_kind"]["unknown"] += size
        stats["bytes_by_range_kind"]["padding"] -= size
        if not stats["bytes_by_range_kind"]["padding"] and "padding" not in revision["baseline"]["range_kinds"]:
            del stats["bytes_by_range_kind"]["padding"]
    stats["unknown_ranges"] += len(revision["transitions"])
    stats["anomalies"] += len(revision["resolved_anomalies"])
    if revision["resolved_anomalies"]:
        stats["anomalies_by_issue"]["unexplained_function_bytes"] = stats["anomalies_by_issue"].get("unexplained_function_bytes", 0) + len(revision["resolved_anomalies"])
    _check_baseline(baseline, revision)
    _transitions(baseline, analyzer, revision)
    return baseline


def prove_objects(root: Path, catalogue: dict, analyzer, revision: dict) -> dict:
    """Reopen only the exact selected members and reencode every complete section."""
    if __package__:
        from . import fingerprint, image, inplace
    else:
        import fingerprint
        import image
        import inplace
    proof = _proof(revision)
    original = image.Image(analyzer.image, analyzer.layout)
    functions = {row["address"]: row for row in catalogue["functions"]}
    linker = inplace.InPlaceLinker(original, functions, inplace.Resolver(functions), (0, 0x100000000))
    checked = []
    for archive in sorted({section["witness"]["archive"] for section in proof["sections"]}):
        requested = [section for section in proof["sections"] if section["witness"]["archive"] == archive]
        path = root / archive
        if any(runtime._sha(path.read_bytes()) != section["witness"]["archive_sha256"] for section in requested):
            raise ValueError("v13 exact private archive digest differs")
        by_offset = {section["witness"]["member_offset"]: section["witness"] for section in requested}
        members = {}
        for member in fingerprint.archive_members(path):
            witness = by_offset.get(member.offset)
            if witness is not None:
                if member.name != witness["member"] or runtime._sha(member.data) != witness["member_sha256"]:
                    raise ValueError("v13 exact private member name/offset/hash differs")
                members[member.offset] = inplace.ObjectFile(Path(member.name), data=member.data)
            if len(members) == len(by_offset):
                break
        if set(members) != set(by_offset):
            raise ValueError("v13 exact private member is absent")
        for expected in requested:
            obj = members[expected["witness"]["member_offset"]]
            actual = obj.sections[expected["index"]]
            if ((actual.name, actual.size, actual.align, actual.executable, actual.writable, actual.nobits)
                    != (expected["name"], expected["size"], expected["alignment"], True, False, False)
                    or runtime._sha(actual.data) != expected["object_sha256"]
                    or [list(row) for row in actual.mapping] != expected["mapping"]):
                raise ValueError("v13 complete object section properties/hash/mapping differ")
            symbols = [{"name": row.name, "kind": row.kind, "bind": row.bind, "value": row.value,
                        "offset": inplace._symbol_offset(row), "size": row.size}
                       for row in obj.symbols if row.shndx == expected["index"] and row.name not in ("$a", "$t", "$d")]
            if symbols != expected["symbols"] or len(actual.relocs) != len(expected["relocations"]):
                raise ValueError("v13 complete object symbol/relocation inventory differs")
            relocated = bytearray(actual.data)
            for relocation, retained in zip(actual.relocs, expected["relocations"]):
                properties = {"offset": relocation.offset, "type": relocation.type, "symbol": relocation.symbol.name,
                              "symbol_section": relocation.symbol.shndx, "symbol_value": relocation.symbol.value,
                              "symbol_size": relocation.symbol.size, "addend": linker._addend(relocation, actual)}
                if properties != {key: retained[key] for key in properties}:
                    raise ValueError("v13 complete relocation inventory/addend differs")
                if relocation.symbol.shndx == expected["index"] and (
                        retained["target"] != expected["base"] + inplace._symbol_offset(relocation.symbol)
                        or retained["thumb"] != bool(relocation.symbol.value & 1)):
                    raise ValueError("v13 same-section relocation is not its actual definition")
                linker._apply(relocation, actual, relocated, expected["base"] + relocation.offset,
                              inplace.Target(retained["target"], retained["thumb"]))
                lo, width = relocation.offset, retained["width"]
                if bytes(relocated[lo:lo + width]) != original.read(expected["base"] + lo, width):
                    raise ValueError("v13 original relocation reencoding differs")
            if bytes(relocated) != original.read(expected["base"], actual.size):
                raise ValueError("v13 complete original section reencoding differs")
            for part in expected["composition"]:
                lo, hi = part["offset"], part["end_offset"]
                if (runtime._sha(actual.data[lo:hi]) != part["object_sha256"]
                        or runtime._sha(bytes(relocated[lo:hi])) != part["relocated_sha256"]):
                    raise ValueError("v13 complete object partition hash differs")
            checked.append({"id": expected["id"], "base": expected["base"], "size": actual.size,
                            "whole_section_sha256": expected["image_sha256"]})
    return {"sections": checked, "complete_section_bytes": sum(row["size"] for row in checked),
            "runtime_pool_value_admitted": False, "new_C_credit": 0}


def main(argv: list[str] | None = None) -> int:
    import argparse
    import sys
    if __package__:
        from . import analyze, catalogue_closure, ctrfmt
    else:
        import analyze
        import catalogue_closure
        import ctrfmt
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    source = parser.add_mutually_exclusive_group(required=True)
    source.add_argument("--baseline", type=Path)
    source.add_argument("--proposal", type=Path)
    parser.add_argument("--prove-objects", action="store_true")
    args = parser.parse_args(argv)
    try:
        evidence, _, _ = catalogue_closure.load_inputs(args.root, args.root / "research/catalogue_v9_r2_evidence.json")
        revision = evidence["alignment_revision"]
        image = (args.root / "orig/code.bin").read_bytes()
        exheader = (args.root / "orig/exheader.bin").read_bytes()
        if runtime._sha(exheader) != revision["exheader_sha256"]:
            raise ValueError("v13 original exheader digest differs")
        analyzer = analyze.Analyzer(image, ctrfmt.parse_exheader(exheader))
        payload = (args.baseline or args.proposal).read_bytes()
        catalogue = json.loads(payload)
        if args.baseline:
            if runtime._sha(payload) != revision["baseline"]["proposal_sha256"]:
                raise ValueError("v13 reviewed v12 proposal digest differs")
            _check_baseline(catalogue, revision)
            _transitions(catalogue, analyzer, revision)
            baseline = catalogue
            if "non_c_proof" in revision:
                non_c._check(baseline, analyzer, revision)
        else:
            baseline = check_catalogue(catalogue, analyzer, evidence)
        r4 = local.check_catalogue(baseline, analyzer, evidence)
        catalogue_closure.check_entry_revision(r4, analyzer, evidence, {}, evidence["classification_records"])
        result = {"status": "pass", "mode": "reviewed_v12_baseline" if args.baseline else "v13_public_consumer",
                  "transitions": revision["transitions"], "resolved_anomalies": revision["resolved_anomalies"],
                  "new_C_credit": 0, "adoption": False, "current_image_generation": False}
        if args.prove_objects:
            result["object_proof"] = prove_objects(args.root, baseline, analyzer, revision)
            if "non_c_proof" in revision:
                result["non_c_object_proof"] = non_c.prove_objects(args.root, baseline, analyzer, revision)
        print(json.dumps(result, sort_keys=True))
    except (OSError, ValueError, KeyError) as error:
        print(f"catalogue alignment: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
