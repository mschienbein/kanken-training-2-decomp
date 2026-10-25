"""V15: one complete named callback definition, never runtime pool-value admission.

This successor preserves every adopted complete function record. Its new entry
requires a whole separately declared vendor section and a named static ELF
reference in a complete parent whose placement is independently anchored.
"""
from __future__ import annotations

import argparse
from copy import deepcopy
import json
from pathlib import Path
import re
import subprocess
import sys

if __package__:
    from . import analyze, catalogue_local_entries as local, catalogue_non_c as non_c
    from . import catalogue_runtime_data as runtime, ctrfmt
else:
    import analyze
    import catalogue_local_entries as local
    import catalogue_non_c as non_c
    import catalogue_runtime_data as runtime
    import ctrfmt

INPUT = "research/catalogue_v15_evidence.json"
POLICY = "complete_named_callback_sections_v1"
ENTRY_EVIDENCE = "exact_bound_library_callback_entry"
RECEIPT = "named_callback_v15"
REQUIRED_INPUTS = {"proof", "observations", "ledger", "names", "c012", "c012_names", "c015", "c015_names",
                   "parent_evidence", "parent_review", "padding_proof", "padding_review"}
SOURCE_FILES = ("catalogue_unknowns.py", "catalogue_non_c.py", "catalogue_local_entries.py", "catalogue_runtime_data.py",
                "analyze.py", "ctrfmt.py", "fingerprint.py", "image.py", "inplace.py", "linker_models.py")


def load_inputs(root: Path) -> tuple[dict, dict, dict[str, str]]:
    content = (root / INPUT).read_bytes()
    revision = json.loads(content)
    if (revision["revision"] != "catalogue-v15" or revision["policy"] != POLICY
            or revision["adoption"] is not False or revision["new_C_credit"] != 0
            or set(revision["inputs"]) != REQUIRED_INPUTS):
        raise ValueError("v15 revision requires the complete frozen input and no-credit boundary")
    parent = revision["baseline"]
    if re.fullmatch(r"[0-9a-f]{40}", parent["blob"]) is None:
        raise ValueError("v15 baseline requires an immutable Git blob")
    result = subprocess.run(["git", "show", parent["blob"]], cwd=root, capture_output=True, check=False)
    if result.returncode or runtime._sha(result.stdout) != parent["sha256"]:
        raise ValueError("v15 adopted parent digest differs")
    baseline = json.loads(result.stdout)
    if (baseline["generator"]["revision"] != parent["revision"]
            or parent["revision"] != "catalogue-v14" or baseline["image_sha256"] != revision["image_sha256"]
            or revision["denominator_parent"]["catalogue_sha256"] != parent["sha256"]):
        raise ValueError("v15 parent revision, image or history binding differs")
    inputs = {INPUT: runtime._sha(content), f"git:{parent['reference']}@{parent['blob']}": parent["sha256"]}
    for label, descriptor in revision["inputs"].items():
        body = non_c._v14_retained(root, descriptor)
        revision["_" + label] = body.decode("utf-8") if label.endswith("review") else json.loads(body)
        inputs[descriptor["path"]] = descriptor["sha256"]
    return baseline, revision, dict(sorted(inputs.items()))


def protected_functions(baseline: dict, revision: dict) -> dict[int, dict]:
    functions = {row["address"]: row for row in baseline["functions"]}
    protected = {}
    groups = {}
    for label, manifest, names in (("accepted", revision["_ledger"], revision["_names"]),
                                    ("c012-r6", revision["_c012"], revision["_c012_names"]),
                                    ("c015", revision["_c015"], revision["_c015_names"])):
        claims = set()
        for unit in manifest["units"]:
            for name in unit["functions"]:
                address = (names["symbols"][name]["address"] if name in names["symbols"] else int(name.removeprefix("func_"), 16)) & ~1
                if address not in functions or address in claims or functions[address]["kind"] != "function":
                    raise ValueError("v15 protected claim is missing, duplicated or noncreditable")
                claims.add(address)
                protected[address] = functions[address]
        groups[label] = {"functions": len(claims), "bytes": sum(functions[address]["size"] for address in claims),
                         "complete_records_sha256": local.metadata_sha([functions[address] for address in sorted(claims)])}
    if groups != revision["protection"]:
        raise ValueError("v15 accepted/candidate complete-record protection differs")
    return protected


def _read(analyzer, address: int, size: int) -> bytes:
    return non_c._read(analyzer, address, size)


def check_proof(baseline: dict, analyzer, revision: dict) -> tuple[dict, int]:
    if (local.metadata_sha(baseline) != revision["baseline"]["sha256"]
            or runtime._sha(analyzer.image) != revision["image_sha256"]):
        raise ValueError("v15 exact baseline or private image differs")
    proof = revision["_proof"]
    if (local.metadata_sha(proof) != revision["inputs"]["proof"]["sha256"]
            or proof["image_sha256"] != revision["image_sha256"]
            or proof["runtime_pool_value_admitted"] is not False or proof["new_C_credit"] != 0
            or len(proof["sections"]) != 2):
        raise ValueError("v15 callback proof is not the exact retained static-linkage certificate")
    parent_section, callback = proof["sections"]
    functions = {row["address"]: row for row in baseline["functions"]}
    parent = proof["parent"]
    bound = functions.get(parent["address"])
    if (bound is None or local.metadata_sha(bound) != parent["record_sha256"]
            or parent_section["base"] != bound["address"] or bound["size"] != 60
            or bound["isa"] != "thumb" or bound["code_ranges"] != [[bound["address"], bound["address"] + 54]]
            or bound["pools"] != [[bound["address"] + 56, bound["address"] + 60]] or bound["literals"]
            or (parent_section["member"], parent_section["size"], parent_section["alignment"], parent_section["mapping"])
            != ("__0snprintf.o", 60, 4, [[0, "$t"], [54, "$d"]])):
        raise ValueError("v15 complete parent placement/declaration/tail differs")
    expected_symbols = [{"name": ".text", "kind": "STT_SECTION", "bind": "STB_LOCAL", "value": 0, "offset": 0, "size": 0},
                        {"name": "__0snprintf", "kind": "STT_FUNC", "bind": "STB_GLOBAL", "value": 1, "offset": 0, "size": 54}]
    if parent_section["symbols"] != expected_symbols:
        raise ValueError("v15 parent lacks its complete exact named declaration")
    incoming = parent["incoming"]
    caller = functions.get(incoming["address"])
    instruction = analyzer.decode(incoming["site"], "arm")
    if (caller is None or caller["isa"] != "arm" or local.metadata_sha(caller) != incoming["record_sha256"]
            or caller["size"] != incoming["size"]
            or runtime._sha(_read(analyzer, caller["address"], caller["size"])) != incoming["image_sha256"]
            or not any(lo <= incoming["site"] < hi for lo, hi in caller["code_ranges"])
            or instruction is None or not instruction.call or instruction.conditional
            or instruction.target != (bound["address"], "thumb")
            or incoming["site"] not in {site for entry in bound["entries"] for site in entry["sources"]}):
        raise ValueError("v15 parent has no independently retained incoming code-entry placement")
    for section in proof["sections"]:
        if (section["index"] != 1 or section["name"] != ".text" or not section["allocated"]
                or not section["executable"] or section["writable"] or section["nobits"]
                or section["base"] % section["alignment"] or not section["whole_section_reencoded_equal"]
                or section["image_sha256"] != section["relocated_sha256"]
                or runtime._sha(_read(analyzer, section["base"], section["size"])) != section["image_sha256"]
                or section["archive"] != ".cache/toolchain/armcc/5.06/b960/lib/armlib/c_5.l"):
            raise ValueError("v15 requires complete equal allocated executable sections, not a prefix or masked body")
    address, size = callback["base"], callback["size"]
    if ((address, size, callback["member"], callback["alignment"], callback["mapping"])
            != (0x1005FA, 16, "_snputc.o", 2, [[0, "$t"]]) or callback["relocations"]
            or callback["object_sha256"] != callback["image_sha256"]
            or callback["symbols"] != [expected_symbols[0], {"name": "_snputc", "kind": "STT_FUNC", "bind": "STB_GLOBAL", "value": 1, "offset": 0, "size": size}]):
        raise ValueError("v15 needs the whole relocation-free sized _snputc definition")
    relocations = parent_section["relocations"]
    if [(row["offset"], row["type"], row["symbol"], row["addend"]) for row in relocations] != [(26, 10, "_printf_char_common", -4), (40, 10, "_sputc", -4), (56, 3, "_snputc", 34)]:
        raise ValueError("v15 complete named parent relocation inventory/addends differ")
    for relocation in relocations:
        if (relocation["symbol_section"] != "SHN_UNDEF" or relocation["symbol_value"] != 0
                or relocation["symbol_size"] != 0 or relocation["width"] != 4
                or not relocation["thumb"] or not relocation["reencoded_equal"]):
            raise ValueError("v15 named imports require exact complete relocation fields")
        site = bound["address"] + relocation["offset"]
        if relocation["type"] == 10:
            anchor = relocation["anchor"]
            owner = functions.get(anchor["address"])
            ins = analyzer.decode(site, "thumb")
            if (owner is None or owner["kind"] != "library" or owner["isa"] != "thumb"
                    or owner["classification_evidence"]["identity"] != relocation["symbol"]
                    or (anchor["identity"], anchor["isa"], anchor["size"], relocation["target"])
                    != (relocation["symbol"], owner["isa"], owner["size"], owner["address"])
                    or local.metadata_sha(owner) != anchor["record_sha256"]
                    or runtime._sha(_read(analyzer, owner["address"], owner["size"])) != anchor["image_sha256"]
                    or ins is None or not ins.call or ins.conditional or ins.target != (owner["address"], "thumb")):
                raise ValueError("v15 parent named imports lack independently classified exact original callees")
        elif (relocation["target"] != address
              or int.from_bytes(_read(analyzer, site, 4), "little") != ((((address + relocation["addend"]) | 1) - site) & runtime.MASK)):
            raise ValueError("v15 named static callback REL32 does not reencode with its actual addend/tag")
    protected = protected_functions(baseline, revision)
    if (any(row["address"] < address + size and address < row["address"] + row["size"]
            for row in [*functions.values(), *protected.values()])
            or {"start": address, "end": address + size, "kind": "unknown", "owner": None} not in baseline["ranges"]):
        raise ValueError("v15 callback would replace a protected/existing owner or a nonexact unknown range")
    remaining = [row for row in baseline["ranges"] if row["kind"] == "unknown" and row["start"] != address]
    if remaining != revision["remaining_unknown_ranges"] or baseline["anomalies"] != revision["remaining_anomalies"]:
        raise ValueError("v15 must retain every separately unresolved range and anomaly")
    count, site = 0, address
    while site < address + size:
        ins = analyzer.decode(site, "thumb")
        if ins is None or site + ins.size > address + size:
            raise ValueError("v15 whole declared callback code does not decode completely")
        site += ins.size
        count += 1
    return callback, count


def _function(section: dict, revision: dict) -> dict:
    address, size = section["base"], section["size"]
    classification = {"address": address, "size": size, "isa": "thumb", "kind": "library", "basis": "known_library",
                      "identity": "_snputc", "image_sha256": revision["image_sha256"],
                      "original_extent_sha256": section["image_sha256"],
                      "known_library_identity": "Complete named Arm 5.06-b960 definition plus whole independently placed __0snprintf parent and named static REL32 linkage",
                      "evidence": revision["inputs"]["proof"]["path"],
                      "library_witness": {key: section[key] for key in ("archive", "archive_sha256", "member", "member_offset", "member_sha256")},
                      "qualification": "Static ELF linkage only: no runtime pool/endian/computed-transfer admission, historical-build uniqueness, parent reclassification, canonical name or C credit. Older 5.04 mismatch remains a negative."}
    return {"address": address, "size": size, "code_size": size, "isa": "thumb", "kind": "library",
            "code_ranges": [[address, address + size]], "pools": [], "literals": [], "evidence": [ENTRY_EVIDENCE],
            "entries": [{"address": address, "isa": "thumb", "kind": "entry", "evidence": [ENTRY_EVIDENCE], "sources": []}],
            "noreturn": False, "noreturn_reasons": [], "classification_evidence": classification}


def propose(baseline: dict, analyzer, revision: dict, inputs: dict[str, str], sources: dict[str, str]) -> dict:
    section, instructions = check_proof(baseline, analyzer, revision)
    result = deepcopy(baseline)
    function = _function(section, revision)
    result["functions"].append(function)
    result["functions"].sort(key=lambda row: row["address"])
    address, size = section["base"], section["size"]
    result["ranges"] = [{**row, "kind": "function", "owner": address} if row["start"] == address and row["end"] == address + size else row for row in result["ranges"]]
    stats = result["stats"]
    for key, delta in (("functions", 1), ("classified_functions", 1), ("code_bytes", size), ("decoded_instructions", instructions), ("unknown_ranges", -1)):
        stats[key] += delta
    for key, label, delta in (("functions_by_isa", "thumb", 1), ("functions_by_kind", "library", 1),
                              ("starts_by_evidence", ENTRY_EVIDENCE, 1), ("bytes_by_range_kind", "unknown", -size),
                              ("bytes_by_range_kind", "function", size)):
        stats[key][label] = stats[key].get(label, 0) + delta
        stats[key] = dict(sorted(stats[key].items()))
    receipt = {"proof": revision["inputs"]["proof"], "entry": {"address": address, "size": size, "isa": "thumb", "kind": "library"},
               "runtime_pool_value_admitted": False, "new_C_credit": 0, "adoption": False}
    result["closure_evidence"][RECEIPT] = receipt
    result["generator"] = {"command": "tools/catalogue_unknowns.py propose-v15 --out X", "revision": "catalogue-v15",
                           "source_sha256": sources, "input_sha256": inputs, "parent": revision["baseline"], "policy": POLICY}
    protected = protected_functions(baseline, revision)
    result["revision_review"] = {"name": "catalogue-v15", "adoption": False, "new_C_credit": 0, "baseline": revision["baseline"],
                                 "parent_review_sha256": local.metadata_sha(baseline["revision_review"]), "transition": receipt,
                                 "protected_summary": revision["protection"],
                                 "protected_rows": [{"address": address, "size": row["size"], "record_sha256": local.metadata_sha(row)} for address, row in sorted(protected.items())],
                                 "unknown_ranges": revision["remaining_unknown_ranges"], "anomalies": revision["remaining_anomalies"],
                                 "preservation": {"changed_old_function_records": [], "accepted_record_changes": [], "candidate_record_changes": []},
                                 "observations": revision["inputs"]["observations"], "input_sha256": {**inputs, **sources}, "qualification": revision["qualification"]}
    analyze.check_protected_functions(result, protected)
    return result


def validate(candidate: dict, baseline: dict, analyzer, revision: dict, inputs: dict[str, str], sources: dict[str, str]) -> None:
    if candidate != propose(baseline, analyzer, revision, inputs, sources):
        raise ValueError("v15 successor differs outside its exact certified added-entry transition")


def prove_objects(root: Path, baseline: dict, analyzer, revision: dict) -> dict:
    """Reopen only two exact licensed members; no extraction, compilation or payload output."""
    if __package__:
        from . import fingerprint, image, inplace
    else:
        import fingerprint
        import image
        import inplace
    check_proof(baseline, analyzer, revision)
    expected = revision["_proof"]["sections"]
    archive = root / expected[0]["archive"]
    if any(section["archive"] != expected[0]["archive"] or runtime._sha(archive.read_bytes()) != section["archive_sha256"] for section in expected):
        raise ValueError("v15 private archive digest differs")
    wanted = {section["member_offset"]: section for section in expected}
    objects = {}
    for member in fingerprint.archive_members(archive):
        section = wanted.get(member.offset)
        if section is not None:
            if (member.name, len(member.data), runtime._sha(member.data)) != (section["member"], section["member_size"], section["member_sha256"]):
                raise ValueError("v15 exact private member differs")
            objects[member.offset] = inplace.ObjectFile(Path(member.name), data=member.data)
        if len(objects) == len(wanted):
            break
    if objects.keys() != wanted.keys():
        raise ValueError("v15 exact private member is absent")
    original = image.Image(analyzer.image, analyzer.layout)
    functions = {row["address"]: row for row in baseline["functions"]}
    linker = inplace.InPlaceLinker(original, functions, inplace.Resolver(functions), (0, 0x100000000))
    checked = []
    for section in expected:
        obj = objects[section["member_offset"]]
        actual = obj.sections[section["index"]]
        symbols = [{"name": symbol.name, "kind": symbol.kind, "bind": symbol.bind, "value": symbol.value,
                    "offset": inplace._symbol_offset(symbol), "size": symbol.size}
                   for symbol in obj.symbols if symbol.shndx == section["index"] and symbol.name not in ("$a", "$t", "$d")]
        if ((actual.name, actual.size, actual.align, actual.executable, actual.writable, actual.nobits)
                != (section["name"], section["size"], section["alignment"], True, False, False)
                or [list(row) for row in actual.mapping] != section["mapping"] or symbols != section["symbols"]
                or runtime._sha(actual.data) != section["object_sha256"] or len(actual.relocs) != len(section["relocations"])):
            raise ValueError("v15 complete private section/declaration inventory differs")
        relocated = bytearray(actual.data)
        for relocation, retained in zip(actual.relocs, section["relocations"]):
            metadata = {"offset": relocation.offset, "type": relocation.type, "symbol": relocation.symbol.name,
                        "symbol_section": relocation.symbol.shndx, "symbol_value": relocation.symbol.value,
                        "symbol_size": relocation.symbol.size, "addend": linker._addend(relocation, actual)}
            if metadata != {key: retained[key] for key in metadata}:
                raise ValueError("v15 exact private relocation/addend inventory differs")
            linker._apply(relocation, actual, relocated, section["base"] + relocation.offset, inplace.Target(retained["target"], retained["thumb"]))
        if bytes(relocated) != original.read(section["base"], actual.size):
            raise ValueError("v15 whole private section does not reencode exactly")
        checked.append({"member": section["member"], "base": section["base"], "size": actual.size, "sha256": runtime._sha(bytes(relocated))})
    return {"sections": checked, "runtime_pool_value_admitted": False, "new_C_credit": 0}


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    command = sub.add_parser("propose-v15")
    root = Path(__file__).resolve().parents[1]
    command.add_argument("--out", type=Path, required=True)
    command.add_argument("--image", type=Path, default=root / "orig/code.bin")
    command.add_argument("--exheader", type=Path, default=root / "orig/exheader.bin")
    command.add_argument("--prove-objects", action="store_true")
    command.add_argument("--private-root", type=Path, default=root)
    args = parser.parse_args(argv)
    try:
        baseline, revision, inputs = load_inputs(root)
        image_bytes, exheader = args.image.read_bytes(), args.exheader.read_bytes()
        analyzer = analyze.Analyzer(image_bytes, ctrfmt.parse_exheader(exheader))
        inputs.update({"image": runtime._sha(image_bytes), "exheader": runtime._sha(exheader)})
        sources = {f"tools/{name}": runtime._sha((root / "tools" / name).read_bytes()) for name in SOURCE_FILES}
        result = propose(baseline, analyzer, revision, dict(sorted(inputs.items())), sources)
        if args.prove_objects:
            receipt = prove_objects(args.private_root, baseline, analyzer, revision)
            print(f"private proof: {len(receipt['sections'])} complete sections; static linkage only")
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_bytes(analyze.serialize_catalogue(result))
    except (OSError, ValueError, KeyError) as error:
        print(f"catalogue-v15: {error}", file=sys.stderr)
        return 1
    print(f"catalogue-v15: {result['stats']['creditable_functions']} creditable functions; {result['stats']['creditable_bytes']} extent bytes; proposal only; {args.out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
