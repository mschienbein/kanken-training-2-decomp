"""Deterministic, metadata-only review attached by ``tools/analyze.py catalogue``.

Reviewed classifications and historical proof notes are immutable tracked inputs;
this module never scans active author worktrees or reruns compiler/emulation probes.
"""
from __future__ import annotations

from bisect import bisect_right
from collections import defaultdict
import hashlib
import json
from pathlib import Path
import struct

from capstone import arm
if __package__:
    from . import analyze, ctrfmt
else:
    import analyze
    import ctrfmt


def load(path: Path):
    return json.loads(path.read_text(encoding="utf-8"))


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def owner(functions: list[dict], starts: list[int], address: int) -> dict | None:
    index = bisect_right(starts, address) - 1
    function = functions[index] if index >= 0 else None
    return function if function and address < function["address"] + function["size"] else None


def pointer_census(candidate: dict, image: bytes, layout: ctrfmt.CodeLayout) -> list[dict]:
    analyzer = analyze.Analyzer(image, layout)
    functions = candidate["functions"]
    starts = [function["address"] for function in functions]
    entries = {(entry["address"], entry["isa"]) for function in functions for entry in function["entries"]}
    references = defaultdict(list)
    code_words = {address & ~3 for function in functions for first, end in function["code_ranges"]
                  for address in range(first, end)}
    pool_owners = {address: function["address"] for function in functions
                   for first, end in function["pools"] for address in range(first, end, 4)}
    literal_owners = {address: function["address"] for function in functions
                      for first, end in function["literals"] for address in range((first + 3) & ~3, end - 3, 4)}
    for address in range(analyzer.base, analyzer.end - 3, 4):
        if address in code_words:
            continue
        value = analyzer.word(address)
        target, isa = value & ~1, "thumb" if value & 1 else "arm"
        if analyzer.inside(target, isa):
            source_owner = pool_owners.get(address, literal_owners.get(address))
            kind = "pool" if address in pool_owners else "literal" if address in literal_owners else "unowned_text"
            references[(target, isa)].append({"address": address, "function": source_owner, "kind": kind})
    for segment in (layout.rodata, layout.data):
        offset = layout.file_offset(segment.address)
        for index in range(0, segment.size - 3, 4):
            value = struct.unpack_from("<I", image, offset + index)[0]
            target, isa = value & ~1, "thumb" if value & 1 else "arm"
            if analyzer.inside(target, isa):
                references[(target, isa)].append({"address": segment.address + index, "function": None, "kind": segment.name})
    uses = defaultdict(list)
    arguments = {arm.ARM_REG_R0, arm.ARM_REG_R1, arm.ARM_REG_R2, arm.ARM_REG_R3}
    for function in functions:
        if function["isa"] != "arm" or function["kind"] == "veneer":
            continue
        blocks = {entry["address"] for entry in function["entries"]}
        for first, end in function["code_ranges"]:
            for address in range(first, end, 4):
                ins = analyzer.decode(address, "arm")
                if ins.branch and ins.target is not None:
                    blocks.add(ins.target[0])
        values, previous = {}, None
        for first, end in function["code_ranges"]:
            for address in range(first, end, 4):
                if previous != address - 4 or address in blocks:
                    values = {}
                decoded, ins = analyzer.detailed(address), analyzer.decode(address, "arm")
                if decoded is None or ins is None:
                    values, previous = {}, address
                    continue
                updates = {}
                if ins.indirect and not ins.returns and decoded.operands and decoded.operands[0].type == arm.ARM_OP_REG:
                    register = decoded.operands[0].reg
                    if register in values:
                        pointer = values[register]
                        target, mode = pointer & ~1, "thumb" if pointer & 1 else "arm"
                        if analyzer.inside(target, mode):
                            uses[(target, mode)].append({"address": address, "function": function["address"], "kind": "indirect_transfer"})
                if ins.call and ins.target is not None:
                    for register, pointer in values.items():
                        target, mode = pointer & ~1, "thumb" if pointer & 1 else "arm"
                        if register in arguments and analyzer.inside(target, mode):
                            uses[(target, mode)].append({"address": address, "function": function["address"],
                                                        "kind": "call_argument", "callee": ins.target[0],
                                                        "register": analyzer.detailed_arm.reg_name(register)})
                if not ins.conditional:
                    if (decoded.id == arm.ARM_INS_LDR and ins.pools and not ins.address_reference
                            and decoded.operands[0].type == arm.ARM_OP_REG
                            and ins.pools[0][0] in pool_owners):
                        updates[decoded.operands[0].reg] = analyzer.word(ins.pools[0][0])
                    elif (decoded.id == arm.ARM_INS_MOV and len(decoded.operands) == 2
                          and decoded.operands[1].type == arm.ARM_OP_REG and decoded.operands[1].reg in values):
                        updates[decoded.operands[0].reg] = values[decoded.operands[1].reg]
                for register in decoded.regs_access()[1]:
                    values.pop(register, None)
                if ins.call:
                    for register in arguments | {arm.ARM_REG_R12}:
                        values.pop(register, None)
                values.update(updates)
                if ins.branch or ins.stop:
                    values = {}
                previous = address
        analyzer.detail_cache.clear()
    rows = []
    for (target, isa), refs in sorted(references.items()):
        function = owner(functions, starts, target)
        if (target, isa) in entries:
            disposition = "callable_entry"
        elif function and any(first <= target < end for first, end in function["code_ranges"]):
            disposition = "interior_code_no_independent_entry_proof"
        elif function and any(first <= target < end for first, end in function["pools"] + function["literals"]):
            disposition = "owned_data_not_entry"
        else:
            disposition = "unowned_no_closed_entry_proof"
        if (target, isa) not in entries and any(use["kind"] == "indirect_transfer" for use in uses[(target, isa)]):
            raise ValueError(f"observed indirect transfer still lacks an entry: {target:#x}/{isa}")
        rows.append({"address": target, "isa": isa, "disposition": disposition, "references": refs,
                     "uses": uses[(target, isa)], "owner": function["address"] if function else None})
    return rows


def accounting(baseline: dict, candidate: dict, image: bytes, layout: ctrfmt.CodeLayout) -> tuple[list[dict], list[dict]]:
    functions = candidate["functions"]
    starts = [function["address"] for function in functions]
    analyzer = analyze.Analyzer(image, layout)
    unknown_rows = []
    for old_range in baseline["ranges"]:
        if old_range["kind"] != "unknown":
            continue
        parts = []
        for new_range in candidate["ranges"]:
            first, end = max(old_range["start"], new_range["start"]), min(old_range["end"], new_range["end"])
            if first >= end:
                continue
            function = owner(functions, starts, first)
            if new_range["kind"] == "unknown":
                conflicts = [conflict for conflict in candidate["literal_conflicts"]
                             if conflict["target"] < end and first < conflict["end"]]
                if conflicts:
                    reason = "PC-relative data reference conflicts with another owner/start or executable bytes; no exclusive data claim"
                elif any(item["end"] == first for item in candidate.get("shared_literals", [])):
                    reason = "Alignment remainder after a proven shared bounded string: no reference, zero-padding proof or data-format witness"
                elif function:
                    reason = "Unreached hole inside the extent: no reachable instruction edge or bounded data-format witness"
                else:
                    reason = "Unowned gap: no evidenced callable entry/closed CFG or bounded data-format witness; plausible decoding alone is insufficient"
            else:
                reason = ("Recovered function instruction/pool/literal ownership" if new_range["kind"] == "function"
                          else "Shared address-taken bounded string data; unowned and not a function extent"
                          if new_range["kind"] == "rodata" else "Proved padding/data")
            parts.append({"start": first, "end": end, "kind": new_range["kind"], "owner": new_range["owner"], "reason": reason})
        if sum(part["end"] - part["start"] for part in parts) != old_range["end"] - old_range["start"]:
            raise ValueError("baseline unknown accounting does not conserve bytes")
        unknown_rows.append({"start": old_range["start"], "end": old_range["end"], "parts": parts})
        offset = layout.file_offset(old_range["start"])
        unknown_rows[-1]["image_interval_sha256"] = hashlib.sha256(
            image[offset:offset + old_range["end"] - old_range["start"]]).hexdigest()
    new_issues = {(row["address"], row["issue"]) for row in candidate["anomalies"]}
    repairs = {row["old_address"]: row["address"] for row in candidate["leading_instruction_repairs"]}
    anomaly_rows = []
    for old_issue in baseline["anomalies"]:
        site, issue = old_issue["address"], old_issue["issue"]
        replacement = repairs.get(site, site)
        remains = (site, issue) in new_issues or (replacement, issue) in new_issues
        function = owner(functions, starts, replacement)
        row = {**old_issue, "disposition": "open" if remains else "resolved", "owner": function["address"] if function else None}
        if not remains:
            row["reason"] = "Image-backed initializer/veneer boundary or bounded literal/shared-data recovery closes this exact old issue"
        elif issue == "unexplained_function_bytes":
            row["reason"] = "Extent contains unowned/unreached holes or alignment not included in code/pools; no byte is silently claimed"
            row["holes"] = [{"start": item["start"], "end": item["end"], "kind": item["kind"]}
                            for item in candidate["ranges"] if function and item["kind"] != "function"
                            and function["address"] <= item["start"] < function["address"] + function["size"]]
        elif issue == "literal_outside_function":
            ins = analyzer.decode(site, function["isa"] if function else "arm")
            row["targets"] = [list(span) for span in ins.pools] if ins else []
            row["reason"] = "Architectural PC-relative target is outside this owner; no conflicting pool ownership or extent is stolen"
        elif issue == "undecodable_reachable_code":
            row["reason"] = "Reachable word after the software-FP exceptional dispatcher does not decode; return-address descriptor/nonreturn semantics remain unproved"
        else:
            row["reason"] = {"shared_address_literal": "Addressed object has multiple independent users; exclusive function ownership is unproved",
                             "address_literal_crosses_start": "Inferred addressed object bound crosses an independently preserved callable start",
                             "address_literal_overlaps_code": "Inferred addressed object bound intersects reachable executable bytes; string-like content alone cannot override code"}[issue]
        anomaly_rows.append(row)
    return unknown_rows, anomaly_rows


def enrich_catalogue(candidate: dict, baseline: dict, image: bytes, layout: ctrfmt.CodeLayout, *,
                     protected: dict[int, dict], protected_summary: dict, evidence: dict,
                     input_sha256: dict[str, str], withheld: list[dict]) -> None:
    """Recompute all image-dependent evidence; retain historical proof provenance."""
    analyze.check_protected_functions(candidate, protected)
    new = {function["address"]: function for function in candidate["functions"]}
    old = {function["address"]: function for function in baseline["functions"]}
    repairs = {row["old_address"]: row["address"] for row in candidate["leading_instruction_repairs"]}
    old_entries = {(entry["address"], entry["isa"]) for function in old.values() for entry in function["entries"]}
    new_entries = {(entry["address"], entry["isa"]) for function in new.values() for entry in function["entries"]}
    starts = sorted(new)
    analyzer = analyze.Analyzer(image, layout)
    boundary_repairs = []
    for address in sorted(old.keys() - new.keys()):
        before = old[address]
        after = owner(candidate["functions"], starts, address)
        if (after is not None and before["evidence"] == ["prologue"]
                and not any(entry["sources"] for entry in before["entries"])
                and "relative_init_array" in after["evidence"]
                and all(any(nlo <= lo < hi <= nhi for nlo, nhi in after[kind])
                        for kind in ("code_ranges", "pools", "literals") for lo, hi in before[kind])):
            boundary_repairs.append({
                "old_address": address, "address": after["address"],
                "old_size": before["size"], "size": after["size"],
                "preserved_entry": any(entry["address"] == address for entry in after["entries"]),
                "entry_evidence": next((entry["evidence"] for entry in after["entries"]
                                        if entry["address"] == address), []),
                "evidence": "section-authenticated initializer entry precedes a weak interior prologue; all old owned spans retained",
            })
    losses = sorted(old_entries - new_entries)
    split_parents = {row["owner"] for row in candidate["linker_tail_extent_splits"]}
    typed_owners = {row["function"] for row in evidence.get("literal_encodings", [])}
    extent_changes, unexpected = [], []
    for address in sorted(old.keys() & new.keys()):
        before, after = old[address], new[address]
        if (before["size"], before["isa"]) == (after["size"], after["isa"]):
            continue
        basis = ("linker_tail_partition" if address in split_parents
                 else "reviewed_utf16_object_bound" if address in typed_owners and before["isa"] == after["isa"]
                 else "canonical_interworking_veneer" if after["kind"] == "veneer" and after["size"] == 8
                 and analyzer.veneer(address, after["isa"]) is not None else None)
        extent_changes.append({"address": address, "old_size": before["size"], "size": after["size"], "basis": basis})
        if basis is None:
            unexpected.append(address)
    structural = analyze.validate_catalogue(candidate, image, layout)
    entry_addresses = {address for address, _ in new_entries}
    missing_initializers = sorted({row["address"] for table in candidate.get("relative_pointer_tables", [])
                                   for row in table["targets"]} - entry_addresses)
    if losses or unexpected or structural or missing_initializers:
        raise ValueError(f"preservation/structural failure: entries={losses}, extents={unexpected}, "
                         f"structural={structural}, initializers={missing_initializers}")
    unknown_rows, anomaly_rows = accounting(baseline, candidate, image, layout)
    classifications = [function["classification_evidence"] for function in candidate["functions"]
                       if "classification_evidence" in function]
    classified = {row["address"] for row in classifications}
    thumb_rows = []
    for row in evidence["thumb_dispositions"]:
        address = int(row["address"], 16)
        current = owner(candidate["functions"], starts, address)
        disposition = ("runtime: admitted extent-bound compiler ABI/emission evidence" if address in classified
                       else "creditable: original-address compiler-emission/library identity missing"
                       if current and current["kind"] == "function"
                       else "topology-excluded; ownership and C eligibility unresolved")
        thumb_rows.append({
            **row, "disposition": disposition,
            "candidate_owner": current["address"] if current else None,
            "candidate_owner_kind": current["kind"] if current else None,
        })
    author_rows = []
    for row in evidence["author_dispositions"]:
        address = int(row["address"], 16)
        current = owner(candidate["functions"], starts, address)
        disposition = row.get("decision", "runtime" if address in classified else "retain_creditable")
        if any(item["old_address"] == address for item in candidate.get("legacy_entry_preservation", [])):
            disposition = "preserved_legacy_entry_in_noncreditable_multi_entry_owner"
        author_rows.append({
            **row, "disposition": disposition,
            "candidate_owner": current["address"] if current else None,
            "candidate_owner_kind": current["kind"] if current else None,
        })
    census = pointer_census(candidate, image, layout)
    for row in unknown_rows:
        row["pointer_witnesses"] = [
            {"address": item["address"], "isa": item["isa"], "disposition": item["disposition"],
             "references": item["references"], "uses": item["uses"]}
            for item in census if row["start"] <= item["address"] < row["end"]]
        row["disposition"] = ("resolved" if all(part["kind"] != "unknown" for part in row["parts"])
                              else "partial" if any(part["kind"] != "unknown" for part in row["parts"]) else "open")
    candidate["revision_review"] = {
        "name": evidence.get("revision", "catalogue-v9"),
        "baseline_sha256": input_sha256["config/catalogue.json"],
        "classifications": classifications, "withheld_classifications": withheld,
        "thumb_dispositions": thumb_rows, "author_dispositions": author_rows,
        "thumb_v9_residue": [
            {**row, "candidate_owner": current["address"] if current else None,
             "candidate_owner_kind": current["kind"] if current else None,
             "ownership_classified": row["address"] in classified}
            for row in evidence.get("thumb_v9_residue", [])
            for current in [owner(candidate["functions"], starts, row["address"])]],
        "kernel_adapter_dispositions": evidence.get("kernel_adapter_dispositions", []),
        "boundary_queue": evidence.get("boundary_queue", []),
        "boundary_repairs": boundary_repairs,
        "extent_changes": extent_changes,
        "legacy_entry_preservation": candidate.get("legacy_entry_preservation", []),
        "provenance": evidence["provenance"],
        "retained_evidence_source_sha256": evidence["source_candidate_sha256"],
        "retained_evidence_input_sha256": evidence["source_input_sha256"],
        "unknown_accounting": unknown_rows, "anomaly_accounting": anomaly_rows,
        "pointer_census": census,
        "protected_summary": protected_summary,
        "protected_rows": [{"address": address, "size": function["size"]}
                           for address, function in sorted(protected.items())],
        "c006_present": any("c006.json" in name for name in protected_summary),
        "c007_present": any("c007.json" in name for name in protected_summary),
        "c008_present": any("c008.json" in name for name in protected_summary),
        "structural_issues": structural,
        "preservation": {
            "unexpected_entry_losses": losses, "protected_changes": [],
            "prior_entries": len(old_entries), "retained_prior_entries": len(old_entries & new_entries),
            "unexpected_extent_changes": unexpected,
            "old_starts_replaced_only_by_listed_repairs": (
                old.keys() - new.keys() <= {row["old_address"] for row in boundary_repairs}
                | {address for address, _ in new_entries} | {address for address in repairs if address in old}),
            "partitioned_owners": sorted(address for address in split_parents & old.keys()
                                         if old[address]["size"] != new[address]["size"]),
        },
        "changed_records": [
            {"address": address,
             "status": "removed" if address not in new else "new" if address not in old else "changed",
             "changed_fields": sorted(key for key in old.get(address, {}).keys() | new.get(address, {}).keys()
                                      if old.get(address, {}).get(key) != new.get(address, {}).get(key))}
            for address in sorted(old.keys() | new.keys()) if old.get(address) != new.get(address)
        ],
        "input_sha256": dict(sorted(input_sha256.items())),
    }
