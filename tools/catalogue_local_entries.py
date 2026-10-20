"""Two explicit object-backed local entries, after the frozen r4 baseline stage.

The generator consumes metadata and original-image hashes, not vendor archives.
A same-section REL32 is a static ELF relocation witness, never an admission of
its runtime pool value or an assumption about the processor's data endianness.
"""
from __future__ import annotations

from copy import deepcopy
import hashlib
import json
from pathlib import Path

if __package__:
    from . import catalogue_runtime_data as runtime
else:
    import catalogue_runtime_data as runtime

BASELINE_FIELDS = ("functions", "ranges", "stats", "anomalies", "pc_relative_pointer_witnesses", "closure_evidence")
ENTRY_EVIDENCE = "exact_bound_local_section_entry"
POLICY = "validate_frozen_r4_before_exact_same_section_local_entries"


def metadata_sha(value: dict | list) -> str:
    return runtime._record_sha(value)


def baseline_hashes(catalogue: dict) -> dict[str, str]:
    """Commit every complete old record/map/statistic and the negative proof stage."""
    return {key: metadata_sha(catalogue[key]) for key in BASELINE_FIELDS}


def _digest(value) -> bool:
    return (isinstance(value, str) and len(value) == 64
            and all(character in "0123456789abcdef" for character in value))


def _records(revision: dict) -> list[dict]:
    proof = revision["_object_proof"]
    if (metadata_sha(proof) != revision["object_proof"]["sha256"]
            or proof["image_sha256"] != revision["image_sha256"]
            or proof["baseline"] != revision["baseline"]
            or proof["exheader_sha256"] != revision["exheader_sha256"]):
        raise ValueError("v12 complete object proof differs from its hash-bound image/baseline")
    records = proof["records"]
    if (len({row["address"] for row in records}) != len(records)
            or len({(row["archive"], row["member_offset"]) for row in records}) != len(records)):
        raise ValueError("v12 object proof duplicates an entry/member")
    transitions = [{"address": row["address"], "size": row["size"], "isa": row["isa"], "symbol": row["symbol"],
                    "from": {"kind": "unknown", "owner": None},
                    "to": {"kind": "library", "physical_kind": "function", "owner": row["address"]}}
                   for row in records]
    if transitions != revision["transitions"]:
        raise ValueError("v12 transitions are not the exact independently proved declarations")
    return records


def load_inputs(root: Path, parent_path: Path, evidence: dict) -> tuple[dict, dict[str, str]]:
    """Load a distinct successor; do not modify any ancestor evidence or policy."""
    path = root / "research/catalogue_v12_evidence.json"
    content = path.read_bytes()
    revision = json.loads(content)
    parent = revision["parent_evidence"]
    if (root / parent["path"] != parent_path
            or hashlib.sha256(parent_path.read_bytes()).hexdigest() != parent["sha256"]
            or revision["revision"] != "catalogue-v12" or revision["policy"] != POLICY
            or revision["image_sha256"] != evidence["image_sha256"]
            or revision["new_C_credit"] != 0 or revision["adoption"] is not False
            or revision["baseline"]["revision"] != "catalogue-v11-r4"
            or revision["baseline"]["adopted"] is not False
            or set(revision["baseline"]["field_sha256"]) != set(BASELINE_FIELDS)
            or any(not _digest(value) for value in revision["baseline"]["field_sha256"].values())):
        raise ValueError("v12 local-entry evidence differs from its immutable r4 parent/image/policy")
    descriptor = revision["object_proof"]
    if descriptor["path"] != "research/catalogue_v12_object_proof.json":
        raise ValueError("v12 object proof must be the explicit frozen metadata input")
    proof_content = (root / descriptor["path"]).read_bytes()
    if hashlib.sha256(proof_content).hexdigest() != descriptor["sha256"]:
        raise ValueError("v12 complete object proof digest differs")
    revision["_object_proof"] = json.loads(proof_content)
    records = _records(revision)
    authentications = {row["address"]: row for row in evidence["library_identification"]["authenticated_owners"]}
    revision["_anchor_authentications"] = {}
    withdrawals = {row["address"]: row for row in evidence["entry_revision"]["withdrawn_entry_proposals"]}
    for row in records:
        previous = withdrawals.get(row["address"])
        authentication = authentications.get(row["anchor"]["address"])
        if (previous is None or row["size"] != previous["prior_function"]["size"]
                or row["isa"] != previous["prior_function"]["isa"]
                or row["fragment_sha256"] != previous["original_extent_sha256"]
                or authentication is None
                or metadata_sha(authentication) != row["anchor"]["authentication_sha256"]):
            raise ValueError("v12 local entry has no unchanged withdrawal/independent source authentication")
        revision["_anchor_authentications"][str(row["anchor"]["address"])] = authentication
    return revision, {str(path.relative_to(root)): hashlib.sha256(content).hexdigest(),
                      descriptor["path"]: descriptor["sha256"]}


def _check_baseline(catalogue: dict, revision: dict) -> None:
    if (catalogue["image_sha256"] != revision["image_sha256"]
            or baseline_hashes(catalogue) != revision["baseline"]["field_sha256"]):
        raise ValueError("v12 requires the independently validated exact r4 baseline before local entries")


def _check_record(catalogue: dict, analyzer, revision: dict, record: dict) -> int:
    """Check the complete section, declaration, source and every relocation."""
    address, size = record["address"], record["size"]
    anchor, section = record["anchor"], record["section"]
    base = anchor["address"] - anchor["symbol_offset"]
    functions = {row["address"]: row for row in catalogue["functions"]}
    bound = functions.get(anchor["address"])
    authentication = revision["_anchor_authentications"].get(str(anchor["address"]))
    if (record["isa"] != "thumb" or size != 10 or address != base
            or anchor["symbol_offset"] != 10 or anchor["declared_size"] != 32
            or (anchor["size"], anchor["isa"]) != (38, "thumb")
            or section["index"] != 1 or section["name"] != ".text"
            or section["size"] != 48 or section["alignment"] != 4 or base % 4
            or not section["allocated"] or not section["executable"] or section["writable"] or section["nobits"]
            or not section["whole_section_reencoded_equal"]
            or section["relocated_sha256"] != section["image_sha256"]
            or record["archive"] != ".cache/armlib/5.04-b82/lib/armlib/c_5.l"
            or record["member_offset"] < 8
            or any(not _digest(value) for value in (record["archive_sha256"], record["member_sha256"],
                                                     section["object_sha256"], section["image_sha256"]))
            or bound is None or metadata_sha(bound) != anchor["record_sha256"]
            or metadata_sha(bound.get("classification_evidence", {})) != anchor["classification_sha256"]
            or authentication is None or metadata_sha(authentication) != anchor["authentication_sha256"]
            or not authentication["authenticated"] or not authentication["whole_body_identical"]
            or authentication["selected_alias_groups"] != [[anchor["symbol"]]]
            or (authentication["address"], authentication["size"], authentication["isa"])
            != (anchor["address"], 38, "thumb")
            or bound["classification_evidence"]["identity"] != anchor["symbol"]
            or (bound["size"], bound["isa"]) != (38, "thumb")
            or bound["code_ranges"] != [[base + 10, base + 42]]
            or bound["pools"] != [[base + 44, base + 48]]):
        raise ValueError("v12 local declaration lacks its exact complete section/independently authenticated source")
    witness = bound["classification_evidence"]["library_witness"]
    if ((witness["archive_member"], witness["member_offset"], witness["member_sha256"], witness["symbol"])
            != (record["member"], record["member_offset"], record["member_sha256"], anchor["symbol"])
            or not (witness["archive"] == record["archive"] or witness["archive"].endswith("/" + record["archive"]))):
        raise ValueError("v12 local declaration belongs to a different authenticated archive/member")
    symbols = [{"name": ".text", "bind": "STB_LOCAL", "kind": "STT_SECTION", "value": 0, "offset": 0, "size": 0},
               {"name": record["symbol"], "bind": "STB_LOCAL", "kind": "STT_FUNC", "value": 1, "offset": 0, "size": 10},
               {"name": anchor["symbol"], "bind": "STB_GLOBAL", "kind": "STT_FUNC", "value": 11, "offset": 10, "size": 32}]
    if section["symbols"] != symbols or section["mapping"] != [[0, "$t"], [42, "$d"]]:
        raise ValueError("v12 needs the complete tagged function declarations and exact code/data mappings")
    composition = section["composition"]
    expected_parts = [(0, 10, "local_declared_function"), (10, 42, "anchor_declared_function"),
                      (42, 44, "mapped_data_halfword_unclassified"), (44, 48, "same_section_relative_relocation")]
    if [(part["offset"], part["end_offset"], part["role"]) for part in composition] != expected_parts:
        raise ValueError("v12 complete section composition has a missing/overlapping/extended part")
    if (runtime._sha(runtime._read(analyzer, base, base + 48)) != section["image_sha256"]
            or runtime._sha(runtime._read(analyzer, base + 10, base + 48)) != anchor["image_sha256"]
            or runtime._sha(runtime._read(analyzer, base, base + 10)) != record["fragment_sha256"]):
        raise ValueError("v12 complete section, source extent or local fragment hash differs")
    for part in composition:
        if (not part["reencoded_equal"] or part["relocated_sha256"] != part["image_sha256"]
                or not _digest(part["object_sha256"])
                or runtime._sha(runtime._read(analyzer, base + part["offset"], base + part["end_offset"]))
                != part["image_sha256"]):
            raise ValueError("v12 complete relocated section partition differs")
    if (composition[0]["object_sha256"] != record["fragment_sha256"]
            or composition[2]["object_sha256"] != composition[2]["image_sha256"]):
        raise ValueError("v12 unrelocated entry/halfword composition differs")
    references = anchor["incoming_references"]
    declared_incoming = {(row["caller"], row["offset"], row["symbol"])
                         for row in authentication["incoming_library_symbol_witnesses"]}
    if (not references or len({row["site"] for row in references}) != len(references)
            or {entry["address"] for entry in bound["entries"]} != {anchor["address"]}
            or {site for entry in bound["entries"] for site in entry["sources"]} != {row["site"] for row in references}
            or {(row["caller"], row["site"] - row["caller"], anchor["symbol"]) for row in references} != declared_incoming):
        raise ValueError("v12 source placement lacks its independent frozen incoming symbol references")
    for reference in references:
        caller = functions.get(reference["caller"])
        instruction = analyzer.decode(reference["site"], reference["isa"])
        if (caller is None or caller["isa"] != reference["isa"]
                or metadata_sha(caller) != reference["caller_record_sha256"]
                or runtime._sha(runtime._read(analyzer, caller["address"], caller["address"] + caller["size"]))
                != reference["caller_image_sha256"]
                or not any(lo <= reference["site"] < hi for lo, hi in caller["code_ranges"])
                or instruction is None or not instruction.call or instruction.conditional
                or instruction.target != (anchor["address"], "thumb")):
            raise ValueError("v12 independent source-placement reference differs")
    relocations = section["relocations"]
    if len(relocations) != 2:
        raise ValueError("v12 complete section must account for every original relocation")
    external, relative = relocations
    expected_external = {"offset": 34, "type": 10, "symbol": "__printf", "symbol_section": "SHN_UNDEF",
                         "symbol_value": 0, "symbol_size": 0, "addend": -4, "target": external["target"],
                         "thumb": True, "agreement": "observed_external_target_not_API_identity", "reencoded_equal": True}
    expected_relative = {"offset": 44, "type": 3, "symbol": record["symbol"], "symbol_section": 1,
                         "symbol_value": 1, "symbol_size": 10, "addend": 16, "target": base,
                         "thumb": True, "agreement": "same_section_definition", "reencoded_equal": True}
    if external != expected_external or relative != expected_relative:
        raise ValueError("v12 complete original relocation inventory/definition/addends differ")
    instruction = analyzer.decode(base + 34, "thumb")
    if (instruction is None or not instruction.call or instruction.conditional
            or instruction.target != (external["target"], "thumb")
            or not any(lo <= base + 34 < base + 38 <= hi for lo, hi in bound["code_ranges"])):
        raise ValueError("v12 observed external relocation target differs; no API identity is inferred")
    # File-endian ELF REL32 arithmetic, not a runtime load/value certificate.
    word = int.from_bytes(runtime._read(analyzer, base + 44, base + 48), "little")
    if word != ((((base + relative["addend"]) | 1) - (base + 44)) & runtime.MASK):
        raise ValueError("v12 static same-section relative relocation does not reencode exactly")
    if record["unclassified_halfword"] != [base + 42, base + 44]:
        raise ValueError("v12 must preserve the separate mapped-data halfword hole")
    instructions = list(analyzer.thumb.disasm(runtime._read(analyzer, base, base + size), base))
    cursor = base
    for instruction in instructions:
        if instruction.address != cursor:
            raise ValueError("v12 local function decoding is incomplete")
        cursor += instruction.size
    if cursor != base + size:
        raise ValueError("v12 local function decoding crosses its declared bound")
    return len(instructions)


def _function(record: dict, revision: dict) -> dict:
    address, size = record["address"], record["size"]
    classification = {
        "address": address, "size": size, "isa": "thumb", "kind": "library", "basis": "known_library",
        "identity": record["symbol"], "image_sha256": revision["image_sha256"],
        "original_extent_sha256": record["fragment_sha256"],
        "known_library_identity": "Exact sized local Thumb definition in an independently authenticated complete 5.04-b82 runtime section",
        "evidence": "research/catalogue_v12_object_proof.json: exact member, complete section and same-section relative relocation",
        "library_witness": {key: record[key] for key in ("archive", "archive_sha256", "member", "member_offset", "member_sha256")},
        "qualification": "No runtime pool-value admission, external API identity, historical archive uniqueness, canonical name or C credit.",
    }
    return {"address": address, "size": size, "code_size": size, "isa": "thumb", "kind": "library",
            "pools": [], "literals": [], "code_ranges": [[address, address + size]], "evidence": [ENTRY_EVIDENCE],
            "entries": [{"address": address, "isa": "thumb", "kind": "entry", "evidence": [ENTRY_EVIDENCE], "sources": []}],
            "noreturn": False, "noreturn_reasons": [], "classification_evidence": classification}


def _statistics(stats: dict, records: list[dict], instruction_count: int, direction: int) -> dict:
    result = deepcopy(stats)
    count, size = len(records), sum(row["size"] for row in records)
    for key, delta in (("functions", count), ("classified_functions", count), ("code_bytes", size),
                       ("decoded_instructions", instruction_count), ("unknown_ranges", -count)):
        result[key] += direction * delta
    for key, label, delta in (("functions_by_isa", "thumb", count), ("functions_by_kind", "library", count),
                               ("starts_by_evidence", ENTRY_EVIDENCE, count),
                               ("bytes_by_range_kind", "unknown", -size), ("bytes_by_range_kind", "function", size)):
        result[key][label] = result[key].get(label, 0) + direction * delta
        if not result[key][label]:
            del result[key][label]
        result[key] = dict(sorted(result[key].items()))
    return result


def _receipt(revision: dict, records: list[dict]) -> dict:
    return {
        "baseline": revision["baseline"], "object_proof": revision["object_proof"],
        "entries": [{"address": row["address"], "size": row["size"], "isa": row["isa"], "symbol": row["symbol"],
                     "anchor": row["anchor"], "section_sha256": row["section"]["image_sha256"],
                     "member_sha256": row["member_sha256"], "new_C_credit": 0} for row in records],
        "preserved_unknown_ranges": revision["preserved_unknown_ranges"], "new_C_credit": 0, "adoption": False,
        "runtime_pool_value_admitted": False,
    }


def close_catalogue(catalogue: dict, analyzer, evidence: dict, protected: dict[int, dict]) -> None:
    """Only unknown -> separate library entries, after the unchanged r3/r4 check."""
    revision = evidence["local_entry_revision"]
    records = _records(revision)
    _check_baseline(catalogue, revision)
    if runtime._sha(analyzer.image) != revision["image_sha256"]:
        raise ValueError("v12 original image digest differs")
    instruction_count = sum(_check_record(catalogue, analyzer, revision, row) for row in records)
    for record in records:
        address, end = record["address"], record["address"] + record["size"]
        if (any(row["address"] < end and address < row["address"] + row["size"]
                for row in [*catalogue["functions"], *protected.values()])
                or {"start": address, "end": end, "kind": "unknown", "owner": None} not in catalogue["ranges"]):
            raise ValueError("v12 local entry would replace an existing/protected owner or a nonexact unknown interval")
    for first, end in revision["preserved_unknown_ranges"]:
        if {"start": first, "end": end, "kind": "unknown", "owner": None} not in catalogue["ranges"]:
            raise ValueError("v12 must preserve every separately unresolved proposal/halfword")
    catalogue["functions"].extend(_function(row, revision) for row in records)
    catalogue["functions"].sort(key=lambda row: row["address"])
    runtime._replace_unknown_ranges(catalogue, [(row["address"], row["address"] + row["size"], "function", row["address"])
                                              for row in records])
    catalogue["stats"] = _statistics(catalogue["stats"], records, instruction_count, 1)
    catalogue["closure_evidence"]["local_runtime_entries"] = _receipt(revision, records)
    check_catalogue(catalogue, analyzer, evidence)


def check_catalogue(catalogue: dict, analyzer, evidence: dict) -> dict:
    """Public consumer: remove only derived entries, then require exact r4 equality.

    No complete positive function record is imported from the historical proposal.
    The inherited r3 checker has already run on the real baseline before closure;
    its complete negative-stage output is also committed by this comparison.
    """
    revision = evidence["local_entry_revision"]
    records = _records(revision)
    if runtime._sha(analyzer.image) != revision["image_sha256"]:
        raise ValueError("v12 original image digest differs")
    addresses = {row["address"] for row in records}
    functions = {row["address"]: row for row in catalogue["functions"]}
    if len(functions) != len(catalogue["functions"]):
        raise ValueError("v12 successor duplicates a complete owner")
    instruction_count = sum(_check_record(catalogue, analyzer, revision, row) for row in records)
    for record in records:
        address = record["address"]
        if functions.get(address) != _function(record, revision):
            raise ValueError("v12 successor entry is not the freshly derived exact local declaration")
        span = {"start": address, "end": address + record["size"], "kind": "function", "owner": address}
        if catalogue["ranges"].count(span) != 1:
            raise ValueError("v12 successor does not have the exact unknown-to-library physical transition")
    baseline = {key: catalogue[key] for key in BASELINE_FIELDS}
    baseline["image_sha256"] = catalogue["image_sha256"]
    baseline["functions"] = [row for row in catalogue["functions"] if row["address"] not in addresses]
    baseline["ranges"] = [{**row, "kind": "unknown", "owner": None} if row["owner"] in addresses else row
                          for row in catalogue["ranges"]]
    baseline["stats"] = _statistics(catalogue["stats"], records, instruction_count, -1)
    baseline["closure_evidence"] = {key: value for key, value in catalogue["closure_evidence"].items()
                                    if key != "local_runtime_entries"}
    _check_baseline(baseline, revision)
    if catalogue["closure_evidence"].get("local_runtime_entries") != _receipt(revision, records):
        raise ValueError("v12 successor certificate receipt differs from the exact independent proof")
    return baseline


def prove_objects(root: Path, catalogue: dict, analyzer, revision: dict) -> dict:
    """Recheck only the two exact private members; never search for a byte body."""
    if __package__:
        from . import fingerprint, image, inplace
    else:
        import fingerprint
        import image
        import inplace
    records = _records(revision)
    original = image.Image(analyzer.image, analyzer.layout)
    functions = {row["address"]: row for row in catalogue["functions"]}
    linker = inplace.InPlaceLinker(original, functions, inplace.Resolver(functions), (0, 0x100000000))
    archives = {row["archive"] for row in records}
    checked = []
    for archive_name in sorted(archives):
        requested = {row["member_offset"]: row for row in records if row["archive"] == archive_name}
        archive = root / archive_name
        digest = runtime._sha(archive.read_bytes())
        if any(row["archive_sha256"] != digest for row in requested.values()):
            raise ValueError("v12 private archive digest differs")
        for member in fingerprint.archive_members(archive):
            record = requested.get(member.offset)
            if record is None:
                continue
            if member.name != record["member"] or runtime._sha(member.data) != record["member_sha256"]:
                raise ValueError("v12 exact private member name/offset/digest differs")
            obj = inplace.ObjectFile(Path(member.name), data=member.data)
            if list(obj.sections) != [1]:
                raise ValueError("v12 complete allocated-section inventory differs")
            actual, expected = obj.sections[1], record["section"]
            properties = (actual.name, actual.size, actual.align, actual.executable, actual.writable, actual.nobits)
            if (properties != (expected["name"], 48, 4, True, False, False)
                    or [list(row) for row in actual.mapping] != expected["mapping"]
                    or runtime._sha(actual.data) != expected["object_sha256"]):
                raise ValueError("v12 original complete section/hash/mappings differ")
            symbols = [{"name": row.name, "kind": row.kind, "bind": row.bind, "value": row.value,
                        "offset": inplace._symbol_offset(row), "size": row.size}
                       for row in obj.symbols if row.shndx == 1 and row.name not in ("$a", "$t", "$d")]
            relocations = [{"offset": row.offset, "type": row.type, "symbol": row.symbol.name,
                            "symbol_section": row.symbol.shndx, "symbol_value": row.symbol.value,
                            "symbol_size": row.symbol.size} for row in actual.relocs]
            retained = [{key: row[key] for key in ("offset", "type", "symbol", "symbol_section",
                                                 "symbol_value", "symbol_size")} for row in expected["relocations"]]
            if symbols != expected["symbols"] or relocations != retained:
                raise ValueError("v12 complete original symbol/relocation inventory differs")
            base = record["anchor"]["address"] - record["anchor"]["symbol_offset"]
            relocated = bytearray(actual.data)
            for relocation, retained in zip(actual.relocs, expected["relocations"]):
                if linker._addend(relocation, actual) != retained["addend"]:
                    raise ValueError("v12 original relocation addend differs")
                target = inplace.Target(retained["target"], retained["thumb"])
                if relocation.symbol.shndx == 1 and (
                        target.address != base + inplace._symbol_offset(relocation.symbol)
                        or target.thumb != bool(relocation.symbol.value & 1)):
                    raise ValueError("v12 relative relocation is not the real same-section tagged definition")
                linker._apply(relocation, actual, relocated, base + relocation.offset, target)
                lo = relocation.offset
                if bytes(relocated[lo:lo + 4]) != original.read(base + lo, 4):
                    raise ValueError("v12 individual original relocation fails exact reencoding")
            if (bytes(relocated) != original.read(base, 48)
                    or runtime._sha(bytes(relocated)) != expected["relocated_sha256"]):
                raise ValueError("v12 complete independently placed section fails exact reencoding")
            for part in expected["composition"]:
                lo, hi = part["offset"], part["end_offset"]
                if (runtime._sha(actual.data[lo:hi]) != part["object_sha256"]
                        or runtime._sha(bytes(relocated[lo:hi])) != part["relocated_sha256"]):
                    raise ValueError("v12 complete object/relocated section composition differs")
            checked.append({"address": base, "member_offset": member.offset, "member_sha256": record["member_sha256"],
                            "section_bytes": 48, "whole_section_sha256": expected["image_sha256"]})
            if len(checked) == len(records):
                break
    if len(checked) != len(records):
        raise ValueError("v12 exact private member offset is absent")
    return {"sections": checked, "complete_section_bytes": 48 * len(checked),
            "runtime_pool_value_admitted": False, "new_C_credit": 0}


def main(argv: list[str] | None = None) -> int:
    """Bounded retained-proposal consumer, optionally rechecking the exact objects."""
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
    source.add_argument("--baseline", type=Path, help="the hash-bound unadopted r4 proposal")
    source.add_argument("--proposal", type=Path, help="a fresh v12 proposal, without regeneration")
    parser.add_argument("--prove-objects", action="store_true", help="inspect only the exact private members")
    args = parser.parse_args(argv)
    try:
        evidence, _, _ = catalogue_closure.load_inputs(args.root, args.root / "research/catalogue_v9_r2_evidence.json")
        revision = evidence["local_entry_revision"]
        image = (args.root / "orig/code.bin").read_bytes()
        exheader = (args.root / "orig/exheader.bin").read_bytes()
        if runtime._sha(image) != revision["image_sha256"] or runtime._sha(exheader) != revision["exheader_sha256"]:
            raise ValueError("v12 original image/exheader digest differs")
        analyzer = analyze.Analyzer(image, ctrfmt.parse_exheader(exheader))
        payload = (args.baseline or args.proposal).read_bytes()
        catalogue = json.loads(payload)
        if args.baseline:
            if runtime._sha(payload) != revision["baseline"]["proposal_sha256"]:
                raise ValueError("v12 retained r4 proposal digest differs")
            baseline = catalogue
            for record in _records(revision):
                _check_record(baseline, analyzer, revision, record)
        else:
            baseline = check_catalogue(catalogue, analyzer, evidence)
        # Run the unchanged historical negative consumer on the complete baseline,
        # not on a projection in which its withdrawn entries were reinterpreted.
        catalogue_closure.check_entry_revision(baseline, analyzer, evidence, {}, evidence["classification_records"])
        _check_baseline(baseline, revision)
        result = {"status": "pass", "mode": "retained_r4_baseline" if args.baseline else "v12_public_consumer",
                  "baseline_proposal_sha256": revision["baseline"]["proposal_sha256"],
                  "exact_entry_transitions": len(revision["transitions"]), "new_C_credit": 0,
                  "adoption": False, "current_image_generation": False}
        if args.prove_objects:
            result["object_proof"] = prove_objects(args.root, baseline, analyzer, revision)
        print(json.dumps(result, sort_keys=True))
    except (OSError, ValueError, KeyError) as error:
        print(f"catalogue local entries: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
