"""V16 classification-only successor: complete 4.1 array delete and two native anchors."""
from __future__ import annotations

import argparse
from copy import deepcopy
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile

if __package__:
    from . import analyze, catalogue_local_entries as local, catalogue_non_c as non_c
    from . import catalogue_runtime_data as runtime, ctrfmt, fingerprint, image, inplace, toolchain
else:
    import analyze
    import catalogue_local_entries as local
    import catalogue_non_c as non_c
    import catalogue_runtime_data as runtime
    import ctrfmt
    import fingerprint
    import image
    import inplace
    import toolchain

INPUT = "research/catalogue_v16_evidence.json"
POLICY = "unchanged_extent_array_delete_two_native_anchors_v1"
TARGET = 0x19E08C
KEY = "library_v16"
INPUTS = {"proof", "adjudication", "adjudication_report", "parent_review", "parent_evidence", "ledger", "names",
          "c015", "c015_names", "recon", "recon_names", "scalar_source", "array_source"}
SOURCES = ("catalogue_library.py", "catalogue_non_c.py", "catalogue_local_entries.py", "catalogue_runtime_data.py",
           "analyze.py", "ctrfmt.py", "fingerprint.py", "image.py", "inplace.py", "linker_models.py", "toolchain.py")


def load_inputs(root: Path) -> tuple[dict, dict, dict[str, str]]:
    payload = (root / INPUT).read_bytes()
    revision = json.loads(payload)
    if (revision["revision"] != "catalogue-v16" or revision["policy"] != POLICY
            or revision["adoption"] is not False or revision["new_C_credit"] != 0
            or set(revision["inputs"]) != INPUTS):
        raise ValueError("v16 revision/policy/frozen input boundary differs")
    descriptor = revision["baseline"]
    if re.fullmatch(r"[0-9a-f]{40}", descriptor["blob"]) is None:
        raise ValueError("v16 parent must be an immutable Git blob")
    parent = subprocess.run(["git", "show", descriptor["blob"]], cwd=root, capture_output=True, check=False)
    if parent.returncode or runtime._sha(parent.stdout) != descriptor["sha256"]:
        raise ValueError("v16 immutable adopted parent digest differs")
    baseline = json.loads(parent.stdout)
    if (descriptor["revision"] != "catalogue-v15" or baseline["generator"]["revision"] != descriptor["revision"]
            or baseline["image_sha256"] != revision["image_sha256"]
            or revision["denominator_parent"]["catalogue_sha256"] != descriptor["sha256"]):
        raise ValueError("v16 parent revision/image/history differs")
    inputs = {INPUT: runtime._sha(payload), f"git:{descriptor['reference']}@{descriptor['blob']}": descriptor["sha256"]}
    text_labels = {"adjudication_report", "parent_review", "scalar_source", "array_source"}
    for label, retained in revision["inputs"].items():
        content = non_c._v14_retained(root, retained)
        revision["_" + label] = content.decode("utf-8") if label in text_labels else json.loads(content)
        inputs[retained["path"]] = retained["sha256"]
    return baseline, revision, dict(sorted(inputs.items()))


def protected_functions(baseline: dict, revision: dict) -> dict[int, dict]:
    functions = {row["address"]: row for row in baseline["functions"]}
    protected, summaries = {}, {}
    for label, manifest, names in (("accepted", revision["_ledger"], revision["_names"]),
                                    ("c015", revision["_c015"], revision["_c015_names"]),
                                    ("recon-small", revision["_recon"], revision["_recon_names"])):
        group = {}
        for unit in manifest["units"]:
            for name in unit["functions"]:
                address = (names["symbols"][name]["address"] if name in names["symbols"] else int(name.removeprefix("func_"), 16)) & ~1
                if address not in functions or address in group or functions[address]["kind"] != "function":
                    raise ValueError("v16 protected claim is missing, duplicated or noncreditable")
                group[address] = protected[address] = functions[address]
        summaries[label] = {"functions": len(group), "bytes": sum(row["size"] for row in group.values()),
                            "complete_records_sha256": local.metadata_sha([group[address] for address in sorted(group)])}
    pending = revision["pending_recon"]
    if (pending["manifest"] != "config/candidates/c016.json" or pending["status"] != "initial_manifest_frozen"
            or re.fullmatch(r"[0-9a-f]{40}", pending["commit"]) is None
            or pending["adoption_recheck_required"] is not True):
        raise ValueError("v16 recon-small protection requires the frozen initial manifest and adoption recheck")
    if summaries != revision["protection"] or TARGET in protected:
        raise ValueError("v16 target is protected or complete-record protection differs")
    return protected


def section_metadata(obj, section, base: int, targets: dict[str, int], linker, original) -> dict:
    """Reencode every actual relocation; no masks, inferred rewrites or payload output."""
    result = {"index": section.index, "name": section.name, "base": base, "size": section.size, "alignment": section.align,
              "executable": section.executable, "writable": section.writable, "nobits": section.nobits,
              "mapping": [list(row) for row in section.mapping], "object_sha256": runtime._sha(section.data),
              "symbols": [{"name": row.name, "kind": row.kind, "bind": row.bind, "value": row.value,
                           "offset": inplace._symbol_offset(row), "size": row.size}
                          for row in obj.symbols if row.shndx == section.index and row.name not in ("$a", "$t", "$d")],
              "relocations": [], "rewrites": []}
    relocated = bytearray(section.data)
    if {row.symbol.name for row in section.relocs} != targets.keys():
        raise ValueError("v16 replay has an unaccounted actual relocation")
    for relocation in section.relocs:
        name = relocation.symbol.name
        item = {"offset": relocation.offset, "type": relocation.type, "symbol": name,
                "symbol_kind": relocation.symbol.kind, "symbol_binding": relocation.symbol.bind,
                "symbol_section": relocation.symbol.shndx, "symbol_value": relocation.symbol.value,
                "symbol_size": relocation.symbol.size, "addend": linker._addend(relocation, section),
                "target": targets[name], "thumb": False}
        linker._apply(relocation, section, relocated, base + relocation.offset, inplace.Target(targets[name], False))
        item["reencoded_equal"] = bytes(relocated[relocation.offset:relocation.offset + 4]) == original.read(base + relocation.offset, 4)
        if not item["reencoded_equal"]:
            raise ValueError("v16 strict actual relocation does not reencode")
        result["relocations"].append(item)
    result["image_sha256"] = runtime._sha(original.read(base, section.size))
    result["relocated_sha256"] = runtime._sha(bytes(relocated))
    result["whole_section_equal"] = bytes(relocated) == original.read(base, section.size)
    if not result["whole_section_equal"]:
        raise ValueError("v16 complete reencoded section differs")
    return result


def inspect_objects(private_root: Path, source: Path, baseline: dict, analyzer, incoming: dict) -> dict:
    """Bounded independent replay: four members, retained native caller and one fresh compile."""
    original = image.Image(analyzer.image, analyzer.layout)
    functions = {row["address"]: row for row in baseline["functions"]}
    linker = inplace.InPlaceLinker(original, functions, inplace.Resolver(functions), (0, 0x100000000))
    variants, members = [], {}
    for build in (791, 894):
        for variant in ("cpprt_4.l", "cpprt_4f.l"):
            label = f".cache/armlib/4.1-b{build}/lib/cpplib/{variant}"
            path = private_root / label
            member = next(row for row in fingerprint.archive_members(path) if row.name == "array_del.o")
            obj = inplace.ObjectFile(Path(member.name), data=member.data)
            entry = {"archive": label, "archive_sha256": runtime._sha(path.read_bytes()), "build": build,
                     "member": member.name, "member_offset": member.offset, "member_size": len(member.data),
                     "member_sha256": runtime._sha(member.data),
                     "section": section_metadata(obj, obj.sections[1], TARGET, {"_ZdlPv": 0x1852F4}, linker, original)}
            variants.append(entry)
            members[(build, variant)] = member.data
    cross_build = {name: members[(791, name)] == members[(894, name)] for name in ("cpprt_4.l", "cpprt_4f.l")}
    if not all(cross_build.values()):
        raise ValueError("v16 cross-build complete member identity differs")
    object_path = private_root / incoming["object"]["path"]
    if runtime._sha(object_path.read_bytes()) != incoming["object"]["sha256"]:
        raise ValueError("v16 retained native array-caller object differs")
    obj = inplace.ObjectFile(object_path)
    symbol = next(row for row in obj.defined_functions() if row.name == "func_00172f50")
    array = section_metadata(obj, obj.sections[symbol.shndx], 0x172F50, {"_ZdaPv": TARGET}, linker, original)
    profile = toolchain.load_profiles()["game"]
    executable = toolchain.armcc_exe(profile.compiler)
    launcher = toolchain.wibo_path()
    compile_record = {"profile": profile.name, "compiler": profile.compiler, "compiler_sha256": runtime._sha(executable.read_bytes()),
                      "flags": list(profile.flags), "source_sha256": runtime._sha(source.read_bytes()), "cache_used": False}
    with tempfile.TemporaryDirectory(prefix="catalogue-v16-") as temporary:
        output = Path(temporary) / "scalar_delete_caller.o"
        dependency = Path(temporary) / "scalar_delete_caller.d"
        argv = ([str(launcher)] if launcher else []) + [str(executable), "-c", *profile.flags]
        for include in (private_root / "include", toolchain.compiler_root(profile.compiler) / "include"):
            argv.extend(["-I", str(include)])
        argv.extend([f"--depend={dependency}", "--depend_format=unix_escaped", "-o", str(output), str(source)])
        run = subprocess.run(argv, cwd=private_root, capture_output=True, text=True, check=False, env=os.environ | {"LC_ALL": "C"})
        compile_record.update(command=argv, exit_code=run.returncode, stdout=run.stdout, stderr=run.stderr)
        if run.returncode:
            raise ValueError("v16 natural scalar anchor compile failed")
        compile_record["object_sha256"] = runtime._sha(output.read_bytes())
        obj = inplace.ObjectFile(output)
        symbol = next(row for row in obj.defined_functions() if row.name == "func_00102cb0")
        scalar = section_metadata(obj, obj.sections[symbol.shndx], 0x102CB0,
                                  {"_ZdlPv": 0x1852F4, "func_0010c1c8": 0x10C1C8, "bss_00497574": 0x497574}, linker, original)
        dependencies = toolchain._parse_depend(dependency.read_text(), source)
        compile_record["dependencies"] = [{"path": str(path.resolve().relative_to(private_root.resolve())), "sha256": runtime._sha(path.read_bytes())}
                                           for path in sorted(set(dependencies))]
    compile_record["temporary_outputs_removed"] = True
    return {"image_sha256": runtime._sha(analyzer.image), "target_record_sha256": local.metadata_sha(functions[TARGET]),
            "variants": variants, "cross_build_member_identity": cross_build,
            "array_anchor": {**incoming, "record_sha256": local.metadata_sha(functions[0x172F50]), "section": array},
            "scalar_anchor": {"record_sha256": local.metadata_sha(functions[0x102CB0]), "section": scalar, "compile": compile_record},
            "new_C_credit": 0, "adoption": False}


def _read(analyzer, address: int, size: int) -> bytes:
    return non_c._read(analyzer, address, size)


def _section(catalogue: dict, analyzer, section: dict, address: int, size: int, symbol: str) -> None:
    owner = next((row for row in catalogue["functions"] if row["address"] == address), None)
    declarations = [row for row in section["symbols"] if row["kind"] == "STT_FUNC"]
    if (owner is None or owner["size"] != size or owner["isa"] != "arm"
            or section["base"] != address or section["size"] != size or section["alignment"] != 4
            or address % section["alignment"] or not section["executable"] or section["writable"] or section["nobits"]
            or not section["whole_section_equal"] or section["rewrites"]
            or section["relocated_sha256"] != section["image_sha256"]
            or runtime._sha(_read(analyzer, address, size)) != section["image_sha256"]
            or len(declarations) != 1 or declarations[0]["name"] != symbol
            or declarations[0]["bind"] != "STB_GLOBAL" or declarations[0]["offset"] != 0 or declarations[0]["value"] != 0
            or declarations[0]["size"] != (28 if symbol == "func_00102cb0" else size)
            or section["mapping"] != ([[0, "$a"], [28, "$d"]] if symbol == "func_00102cb0" else [[0, "$a"]])):
        raise ValueError("v16 requires the complete named original ARM section, not a generic/prefix/tail match")
    for relocation in section["relocations"]:
        if (not relocation["reencoded_equal"] or relocation["thumb"] or relocation["symbol_section"] != "SHN_UNDEF"
                or relocation["symbol_binding"] != "STB_GLOBAL" or relocation["symbol_size"] != 0
                or relocation["symbol_value"] != 0
                or relocation["symbol_kind"] != ("STT_OBJECT" if relocation["type"] == 2 else "STT_FUNC")):
            raise ValueError("v16 actual named relocation inventory differs")
        if relocation["type"] in (28, 29):
            ins = analyzer.decode(address + relocation["offset"], "arm")
            if ins is None or ins.target != (relocation["target"], "arm"):
                raise ValueError("v16 original native named transfer differs")
            if relocation["type"] == 28 and (not ins.call or ins.conditional):
                raise ValueError("v16 named call was rewritten or is not an unconditional call")
            # Older ARMCC JUMP24 fields also carry conditional calls. Strict
            # reencoding preserves their actual opcode/predicate; do not infer B.
            if relocation["type"] == 29 and not (ins.branch or ins.call):
                raise ValueError("v16 scalar named transfer is not a direct branch/call")


def check_proof(baseline: dict, analyzer, revision: dict) -> None:
    if local.metadata_sha(baseline) != revision["baseline"]["sha256"] or runtime._sha(analyzer.image) != revision["image_sha256"]:
        raise ValueError("v16 exact baseline/private image differs")
    protected = protected_functions(baseline, revision)
    admitted = [row for row in revision["_adjudication"]["candidates"] if row["verdict"] == "PROVEN"]
    if [(row["address"], row["identity"], row["isa"], row["size"]) for row in admitted] != [(TARGET, "_ZdaPv", "arm", 12)]:
        raise ValueError("v16 may promote only the single independently reverified 4.1 identity")
    proof = revision["_proof"]
    if (local.metadata_sha(proof) != revision["inputs"]["proof"]["sha256"] or proof["image_sha256"] != revision["image_sha256"]
            or proof["new_C_credit"] != 0 or proof["adoption"] is not False):
        raise ValueError("v16 requires its complete hash-bound proof")
    functions = {row["address"]: row for row in baseline["functions"]}
    target = functions[TARGET]
    if (target["kind"] != "function" or local.metadata_sha(target) != proof["target_record_sha256"]
            or 0x172F50 + 96 not in {site for entry in target["entries"] for site in entry["sources"]}
            or target["code_ranges"] != [[TARGET, TARGET + target["size"]]] or target["pools"] or target["literals"]):
        raise ValueError("v16 target is not the exact unchanged ordinary owner")
    expected_variants = [(build, f".cache/armlib/4.1-b{build}/lib/cpplib/{name}") for build in (791, 894) for name in ("cpprt_4.l", "cpprt_4f.l")]
    if [(row["build"], row["archive"]) for row in proof["variants"]] != expected_variants:
        raise ValueError("v16 preferred and alternative provenance inventory differs")
    for row in proof["variants"]:
        if row["member"] != "array_del.o" or row["member_offset"] != 10774:
            raise ValueError("v16 requires the selected exact array-delete definition")
        _section(baseline, analyzer, row["section"], TARGET, 12, "_ZdaPv")
        rel = row["section"]["relocations"]
        if len(rel) != 1 or {key: rel[0][key] for key in ("offset", "type", "symbol", "addend", "target")} != {"offset": 4, "type": 28, "symbol": "_ZdlPv", "addend": -8, "target": 0x1852F4}:
            raise ValueError("v16 vendor outgoing call lacks the independent scalar target")
    for name in ("cpprt_4.l", "cpprt_4f.l"):
        members = [row["member_sha256"] for row in proof["variants"] if row["archive"].endswith("/" + name)]
        if len(set(members)) != 1 or proof["cross_build_member_identity"].get(name) is not True:
            raise ValueError("v16 cross-build complete member identity differs")
    array, scalar = proof["array_anchor"], proof["scalar_anchor"]
    _section(baseline, analyzer, array["section"], 0x172F50, 140, "func_00172f50")
    _section(baseline, analyzer, scalar["section"], 0x102CB0, 32, "func_00102cb0")
    if (array["record_sha256"] != local.metadata_sha(functions[0x172F50])
            or scalar["record_sha256"] != local.metadata_sha(functions[0x102CB0]) or 0x102CB0 not in protected
            or array["source_sha256"] != runtime._sha(revision["_array_source"].encode())
            or scalar["compile"]["source_sha256"] != runtime._sha(revision["_scalar_source"].encode())
            or scalar["compile"]["exit_code"] != 0 or scalar["compile"]["cache_used"] is not False
            or scalar["compile"]["temporary_outputs_removed"] is not True):
        raise ValueError("v16 native source/object/accepted scalar anchor closure differs")
    if [(r["offset"], r["type"], r["symbol"], r["addend"], r["target"]) for r in array["section"]["relocations"]] != [(96, 28, "_ZdaPv", -8, TARGET)]:
        raise ValueError("v16 native delete-array caller does not name this original entry")
    if [(r["offset"], r["type"], r["symbol"], r["addend"], r["target"]) for r in scalar["section"]["relocations"]] != [(16, 29, "_ZdlPv", -8, 0x1852F4), (24, 29, "func_0010c1c8", -8, 0x10C1C8), (28, 2, "bss_00497574", 0, 0x497574)]:
        raise ValueError("v16 public-header scalar-delete anchor lacks its complete strict relocation inventory")


def propose(baseline: dict, analyzer, revision: dict, inputs: dict[str, str], sources: dict[str, str]) -> dict:
    check_proof(baseline, analyzer, revision)
    result = deepcopy(baseline)
    target = next(row for row in result["functions"] if row["address"] == TARGET)
    target["kind"] = "library"
    target["classification_evidence"] = {"address": TARGET, "size": target["size"], "isa": "arm", "kind": "library", "basis": "known_library",
                                          "identity": "_ZdaPv", "image_sha256": revision["image_sha256"],
                                          "original_extent_sha256": revision["_proof"]["variants"][0]["section"]["image_sha256"],
                                          "known_library_identity": "Complete Arm Compiler 4.1-b791 cpprt_4.l(array_del.o) definition, strict call reencoding, independent native delete[] and public-header scalar-delete anchors",
                                          "library_witness": revision["_proof"]["variants"][0], "evidence": revision["inputs"]["proof"]["path"],
                                          "qualification": revision["qualification"]}
    non_c._statistics(result["stats"], [{"size": target["size"], "kind": "library"}], 1)
    result["closure_evidence"][KEY] = {"proof": revision["inputs"]["proof"], "address": TARGET, "new_C_credit": 0}
    result["generator"] = {"command": "tools/catalogue_library.py propose-v16 --out X", "revision": "catalogue-v16", "policy": POLICY,
                           "source_sha256": sources, "input_sha256": inputs, "parent": revision["baseline"]}
    protected = protected_functions(baseline, revision)
    result["revision_review"] = {"name": "catalogue-v16", "adoption": False, "new_C_credit": 0, "baseline": revision["baseline"],
                                 "parent_review_sha256": local.metadata_sha(baseline["revision_review"]), "protected_summary": revision["protection"],
                                 "protected_rows": [{"address": address, "size": row["size"], "record_sha256": local.metadata_sha(row)} for address, row in sorted(protected.items())],
                                 "changed_records": [{"address": TARGET, "changed_fields": ["classification_evidence", "kind"]}],
                                 "pending_recon": revision["pending_recon"], "input_sha256": {**inputs, **sources},
                                 "preservation": {"accepted_record_changes": [], "pending_record_changes": [], "physical_range_changes": [], "anomaly_changes": []},
                                 "qualification": revision["qualification"]}
    analyze.check_protected_functions(result, protected)
    return result


def validate(candidate: dict, baseline: dict, analyzer, revision: dict, inputs: dict[str, str], sources: dict[str, str]) -> None:
    if candidate != propose(baseline, analyzer, revision, inputs, sources):
        raise ValueError("v16 successor differs outside its exact kind/evidence transition")


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    command = sub.add_parser("propose-v16")
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
        inputs.update(image=runtime._sha(image_bytes), exheader=runtime._sha(exheader))
        sources = {f"tools/{name}": runtime._sha((root / "tools" / name).read_bytes()) for name in SOURCES}
        result = propose(baseline, analyzer, revision, dict(sorted(inputs.items())), sources)
        if args.prove_objects:
            actual = inspect_objects(args.private_root, root / revision["inputs"]["scalar_source"]["path"], baseline, analyzer,
                                     {key: revision["_proof"]["array_anchor"][key] for key in ("object", "source_sha256")})
            expected = deepcopy(revision["_proof"])
            for proof in (actual, expected):
                for key in ("command", "object_sha256"):
                    proof["scalar_anchor"]["compile"].pop(key)
            if actual != expected:
                raise ValueError("v16 independent complete object/compile replay differs")
            print("private replay: four complete vendor sections, native array caller, fresh scalar caller; temporary outputs removed")
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_bytes(analyze.serialize_catalogue(result))
    except (OSError, ValueError, KeyError, StopIteration) as error:
        print(f"catalogue-v16: {error}", file=sys.stderr)
        return 1
    print(f"catalogue-v16: {result['stats']['creditable_functions']} creditable functions; {result['stats']['creditable_bytes']} extent bytes; proposal only; {args.out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
