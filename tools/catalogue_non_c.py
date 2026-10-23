"""Immutable non-C certificates: the v13 stage and the classification-only v14 successor.

    python tools/catalogue_non_c.py propose-v14 --out config/catalogue.json

V14 starts from the adopted v13-r2 Git blob, not a mutable catalogue or a new
boundary discovery. It preserves every complete record except the explicitly
certified kind/evidence transitions. A proposal is never independent review.
"""
from __future__ import annotations

from copy import deepcopy
import argparse
import json
from pathlib import Path
import re
import subprocess
import struct
import sys

from capstone import arm
if __package__:
    from . import catalogue_local_entries as local, catalogue_runtime_data as runtime
else:
    import catalogue_local_entries as local
    import catalogue_runtime_data as runtime

KEY = "non_c_v13"


def load_inputs(root: Path, revision: dict) -> dict[str, str]:
    descriptor = revision["non_c_proof"]
    if descriptor["path"] != "research/catalogue_v13_non_c_proof.json":
        raise ValueError("v13 non-C evidence requires its frozen proof input")
    content = (root / descriptor["path"]).read_bytes()
    if runtime._sha(content) != descriptor["sha256"]:
        raise ValueError("v13 non-C proof digest differs")
    revision["_non_c_proof"] = json.loads(content)
    _proof(revision)
    return {descriptor["path"]: descriptor["sha256"]}


def _proof(revision: dict) -> dict:
    proof = revision["_non_c_proof"]
    if (local.metadata_sha(proof) != revision["non_c_proof"]["sha256"]
            or proof["image_sha256"] != revision["image_sha256"]
            or proof["baseline"] != revision["baseline"]
            or len({row["address"] for row in proof["classifications"]}) != len(proof["classifications"])):
        raise ValueError("v13 non-C certificates differ from the complete reviewed baseline/image")
    return proof


def _read(analyzer, start: int, size: int) -> bytes:
    offset = analyzer.layout.file_offset(start)
    return analyzer.image[offset:offset + size]


def _incoming(catalogue: dict, analyzer, rows: list[dict]) -> None:
    functions = {row["address"]: row for row in catalogue["functions"]}
    if not rows or len({row["site"] for row in rows}) != len(rows):
        raise ValueError("v13 non-C placement lacks distinct retained incoming references")
    for row in rows:
        owner = functions.get(row["owner"])
        instruction = analyzer.decode(row["site"], row["isa"])
        if (owner is None or local.metadata_sha(owner) != row["record_sha256"]
                or runtime._sha(_read(analyzer, owner["address"], owner["size"])) != row["image_sha256"]
                or not any(lo <= row["site"] < hi for lo, hi in owner["code_ranges"])
                or instruction is None or not instruction.call or instruction.conditional != row["conditional"]
                or instruction.target != (row["target"], row["target_isa"])):
            raise ValueError("v13 non-C independent incoming placement reference differs")


def _check(catalogue: dict, analyzer, revision: dict) -> list[dict]:
    proof = _proof(revision)
    functions = {row["address"]: row for row in catalogue["functions"]}
    if runtime._sha(analyzer.image) != proof["image_sha256"]:
        raise ValueError("v13 non-C original image differs")
    groups = {row["id"]: row for row in proof["groups"]}
    if len(groups) != len(proof["groups"]):
        raise ValueError("v13 non-C duplicate complete-section group")
    for group in groups.values():
        _incoming(catalogue, analyzer, group["incoming"])
        bases = {section["index"]: section["base"] for section in group["sections"]}
        if len(bases) != len(group["sections"]):
            raise ValueError("v13 non-C group duplicates a section")
        for section in group["sections"]:
            if (not section["whole_section_equal"] or section["size"] <= 0
                    or section["base"] % section["alignment"]
                    or section["image_sha256"] != section["relocated_sha256"]
                    or runtime._sha(_read(analyzer, section["base"], section["size"])) != section["image_sha256"]):
                raise ValueError("v13 non-C complete section equality/geometry differs")
            for relocation in section["relocations"]:
                if relocation["rule"] == "same_object_definition":
                    if (relocation["symbol_section"] not in bases
                            or relocation["target"] != bases[relocation["symbol_section"]] + relocation["symbol_offset"]):
                        raise ValueError("v13 non-C same-object placement is not the actual declaration")
                elif relocation["rule"] == "existing_inline_arm_call":
                    if runtime._sha(_read(analyzer, relocation["callee_address"], 8)) != relocation["callee_extent_sha256"]:
                        raise ValueError("v13 non-C inline callee comparison witness differs")
                elif relocation["rule"] not in ("observed_external_target_not_API_identity", "unchanged_marker"):
                    raise ValueError("v13 non-C certificate requires an unadmitted linker rewrite")
        for witness in group["placement"]:
            section = next(row for row in group["sections"] if row["index"] == witness["source_section"])
            relocation = next(row for row in section["relocations"] if row["offset"] == witness["offset"])
            if (relocation["rule"] != "same_object_definition" or relocation["symbol_section"] != witness["target_section"]
                    or relocation["target"] - relocation["symbol_offset"] != bases[witness["target_section"]]):
                raise ValueError("v13 non-C complete-object placement relation differs")
    for row in proof["classifications"]:
        function = functions.get(row["address"])
        if (function is None or function["kind"] != "function" or "classification_evidence" in function
                or local.metadata_sha(function) != row["before_sha256"]
                or runtime._sha(_read(analyzer, function["address"], function["size"])) != row["extent_sha256"]):
            raise ValueError("v13 non-C transition lacks its exact unchanged ordinary owner")
        if row["kind"] == "library":
            intervals = []
            for group_id, index, first, end in row["coverage"]:
                group = groups[group_id]
                section = next(section for section in group["sections"] if section["index"] == index)
                if not section["executable"] or not section["base"] <= first < end <= section["base"] + section["size"]:
                    raise ValueError("v13 non-C declaration coverage escapes a complete executable section")
                intervals.append((first, end))
            cursor = function["address"]
            for first, end in sorted(intervals):
                if first != cursor:
                    raise ValueError("v13 non-C section aggregate is partial, overlapping or gapped")
                cursor = end
            if cursor != function["address"] + function["size"]:
                raise ValueError("v13 non-C complete coverage changes the declared catalogue extent")
            if row.get("anchor"):
                anchor = row["anchor"]
                retained = functions[anchor["address"]]
                section = next(section for section in groups[anchor["group"]]["sections"] if section["index"] == anchor["section"])
                if (local.metadata_sha(retained) != anchor["record_sha256"]
                        or retained["classification_evidence"]["identity"] != anchor["symbol"]
                        or retained["address"] - anchor["offset"] != section["base"]
                        or not any(symbol["name"] == anchor["symbol"] and symbol["offset"] == anchor["offset"]
                                   and symbol["size"] == retained["size"] for symbol in section["symbols"])):
                    raise ValueError("v13 errno entry lacks its independent authenticated setter anchor")
        elif row["kind"] == "assembly":
            certificate = row["control_transfer"]
            _incoming(catalogue, analyzer, certificate["incoming"])
            details = [analyzer.detailed(site) for lo, hi in function["code_ranges"] for site in range(lo, hi, 4)]
            loads = [ins for ins in details if ins is not None and ins.id == arm.ARM_INS_LDR
                     and len(ins.operands) == 2 and ins.operands[0].reg == arm.ARM_REG_LR
                     and ins.operands[1].type == arm.ARM_OP_MEM and ins.operands[1].mem.base == arm.ARM_REG_PC]
            if len(loads) != 1 or not details or details[-1] is None:
                raise ValueError("v13 assembly certificate lacks its exact literal LR substitution")
            instruction = loads[0]
            site = instruction.address + 8 + instruction.operands[1].mem.disp
            continuation = int.from_bytes(_read(analyzer, site, 4), "little")
            final = analyzer.decode(details[-1].address, "arm")
            if (site != certificate["literal_site"] or continuation != certificate["continuation"]
                    or continuation not in functions or continuation == function["address"]
                    or final is None or final.call or not final.indirect or final.returns
                    or arm.ARM_REG_PC not in details[-1].regs_access()[1]):
                raise ValueError("v13 assembly certificate lacks its fixed continuation/noncall indirect exit")
        else:
            raise ValueError("v13 non-C transition has an unsupported kind")
    return proof["classifications"]


def _classification(row: dict, revision: dict) -> dict:
    return {"address": row["address"], "size": row["size"], "isa": row["isa"], "kind": row["kind"],
            "basis": "known_library" if row["kind"] == "library" else "explicit_assembly",
            "identity": row["identity"], "image_sha256": revision["image_sha256"],
            "original_extent_sha256": row["extent_sha256"], "evidence": revision["non_c_proof"],
            "qualification": row["qualification"], "coverage": row.get("coverage", []), "new_C_credit": 0}


def _statistics(stats: dict, rows: list[dict], direction: int) -> None:
    stats["creditable_functions"] -= direction * len(rows)
    stats["creditable_bytes"] -= direction * sum(row["size"] for row in rows)
    stats["classified_functions"] += direction * len(rows)
    stats["functions_by_kind"]["function"] -= direction * len(rows)
    for row in rows:
        label = row["kind"]
        stats["functions_by_kind"][label] = stats["functions_by_kind"].get(label, 0) + direction
        if not stats["functions_by_kind"][label]:
            del stats["functions_by_kind"][label]
    stats["functions_by_kind"] = dict(sorted(stats["functions_by_kind"].items()))


def apply(catalogue: dict, analyzer, revision: dict, protected: dict[int, dict]) -> None:
    rows = _check(catalogue, analyzer, revision)
    functions = {row["address"]: row for row in catalogue["functions"]}
    for row in rows:
        if row["address"] in protected:
            raise ValueError("v13 non-C classification would replace a protected complete record")
        function = functions[row["address"]]
        function["kind"] = row["kind"]
        function["classification_evidence"] = _classification(row, revision)
    _statistics(catalogue["stats"], rows, 1)
    catalogue["closure_evidence"][KEY] = {"proof": revision["non_c_proof"], "classifications": rows, "new_C_credit": 0}


def reverse(catalogue: dict, analyzer, revision: dict) -> dict:
    rows = _proof(revision)["classifications"]
    result = deepcopy(catalogue)
    functions = {row["address"]: row for row in result["functions"]}
    if result["closure_evidence"].pop(KEY, None) != {"proof": revision["non_c_proof"], "classifications": rows, "new_C_credit": 0}:
        raise ValueError("v13 non-C receipt differs")
    for row in rows:
        function = functions[row["address"]]
        if function["kind"] != row["kind"] or function.get("classification_evidence") != _classification(row, revision):
            raise ValueError("v13 non-C successor changes its exact kind/evidence transition")
        function["kind"] = "function"
        del function["classification_evidence"]
        if local.metadata_sha(function) != row["before_sha256"]:
            raise ValueError("v13 non-C successor changes entries, extent or complete old metadata")
    _statistics(result["stats"], rows, -1)
    _check(result, analyzer, revision)
    return result


def prove_objects(root: Path, catalogue: dict, analyzer, revision: dict) -> dict:
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
    groups = proof["groups"]
    members = {}
    for archive in sorted({group["witness"]["archive"] for group in groups}):
        requested = {group["witness"]["member_offset"]: group["witness"] for group in groups if group["witness"]["archive"] == archive}
        if any(runtime._sha((root / archive).read_bytes()) != row["archive_sha256"] for row in requested.values()):
            raise ValueError("v13 non-C private archive digest differs")
        found = set()
        for member in fingerprint.archive_members(root / archive):
            row = requested.get(member.offset)
            if row is not None:
                if member.name != row["member"] or runtime._sha(member.data) != row["member_sha256"]:
                    raise ValueError("v13 non-C exact private member differs")
                members[(archive, member.offset)] = inplace.ObjectFile(Path(member.name), data=member.data)
                found.add(member.offset)
            if found == requested.keys():
                break
        if found != requested.keys():
            raise ValueError("v13 non-C exact member is absent")
    checked = []
    for group in groups:
        witness = group["witness"]
        obj = members[(witness["archive"], witness["member_offset"])]
        if sorted(obj.sections) != group["allocated_section_indices"]:
            raise ValueError("v13 non-C complete allocated-section inventory differs")
        bases = {section["index"]: section["base"] for section in group["sections"]}
        for expected in group["sections"]:
            section = obj.sections[expected["index"]]
            symbols = [{"name": symbol.name, "kind": symbol.kind, "bind": symbol.bind, "value": symbol.value,
                        "offset": inplace._symbol_offset(symbol), "size": symbol.size}
                       for symbol in obj.symbols if symbol.shndx == section.index and symbol.name not in ("$a", "$t", "$d")]
            if ((section.name, section.size, section.align, section.executable, section.writable, section.nobits)
                    != (expected["name"], expected["size"], expected["alignment"], expected["executable"], False, False)
                    or symbols != expected["symbols"] or runtime._sha(section.data) != expected["object_sha256"]
                    or [list(row) for row in section.mapping] != expected["mapping"]
                    or len(section.relocs) != len(expected["relocations"])):
                raise ValueError("v13 non-C complete section inventory differs")
            data = bytearray(section.data)
            for rel, retained in zip(section.relocs, expected["relocations"]):
                properties = {"offset": rel.offset, "type": rel.type, "symbol": rel.symbol.name,
                              "symbol_section": rel.symbol.shndx, "symbol_value": rel.symbol.value,
                              "symbol_offset": inplace._symbol_offset(rel.symbol), "symbol_size": rel.symbol.size,
                              "addend": linker._addend(rel, section) if rel.type not in (0, 40) else None}
                if properties != {key: retained[key] for key in properties}:
                    raise ValueError("v13 non-C complete relocation/addend inventory differs")
                rule = retained["rule"]
                if rule == "unchanged_marker":
                    continue
                if rule == "existing_inline_arm_call":
                    callee = original.read(retained["callee_address"], 8)
                    if runtime._sha(callee) != retained["callee_extent_sha256"]:
                        raise ValueError("v13 non-C inline original callee witness differs")
                    decision = inplace.inline_arm_call(struct.unpack_from("<I", section.data, rel.offset)[0], callee)
                    if decision is None or decision[0] != "inline_insn":
                        raise ValueError("v13 non-C existing inline rule refuses this exact original callee")
                    struct.pack_into("<I", data, rel.offset, decision[1])
                else:
                    if rule == "same_object_definition" and retained["target"] != bases[rel.symbol.shndx] + inplace._symbol_offset(rel.symbol):
                        raise ValueError("v13 non-C original same-object target differs")
                    linker._apply(rel, section, data, expected["base"] + rel.offset,
                                  inplace.Target(retained["target"], retained["thumb"]))
            if bytes(data) != original.read(expected["base"], section.size):
                raise ValueError("v13 non-C complete original section reconstruction differs")
            checked.append({"group": group["id"], "section": section.index, "size": section.size,
                            "image_sha256": runtime._sha(bytes(data))})
    return {"sections": checked, "complete_section_bytes": sum(row["size"] for row in checked), "new_C_credit": 0}


V14_INPUT = "research/catalogue_v14_evidence.json"
V14_KEY = "non_c_v14"


def _v14_retained(root: Path, descriptor: dict) -> bytes:
    label = descriptor["path"]
    path = Path(label)
    if (path.is_absolute() or ".." in path.parts or path.as_posix() != label
            or not path.parts or not (root / path).resolve().is_relative_to(root.resolve())):
        raise ValueError("v14 retained input must be a confined normalized relative path")
    content = (root / path).read_bytes()
    if len(content) != descriptor["size"] or runtime._sha(content) != descriptor["sha256"]:
        raise ValueError(f"v14 retained input digest/size differs: {label}")
    return content


def load_v14_inputs(root: Path) -> tuple[dict, dict, dict[str, str]]:
    """Authenticate the immutable adopted parent and all consumed review inputs."""
    content = (root / V14_INPUT).read_bytes()
    revision = json.loads(content)
    if (revision["revision"] != "catalogue-v14"
            or revision["policy"] != "unchanged_extents_evidenced_non_c_only"
            or revision["adoption"] is not False or revision["new_C_credit"] != 0):
        raise ValueError("v14 revision/policy/credit boundary differs")
    descriptor = revision["baseline"]
    blob = descriptor["blob"]
    if re.fullmatch(r"[0-9a-f]{40}", blob) is None:
        raise ValueError("v14 baseline requires an immutable Git blob")
    result = subprocess.run(["git", "show", blob], cwd=root, capture_output=True, check=False)
    if result.returncode or runtime._sha(result.stdout) != descriptor["sha256"]:
        raise ValueError("v14 adopted baseline digest differs")
    baseline = json.loads(result.stdout)
    if (baseline["generator"]["revision"] != descriptor["revision"]
            or baseline["image_sha256"] != revision["image_sha256"]
            or revision["denominator_parent"]["catalogue_sha256"] != descriptor["sha256"]):
        raise ValueError("v14 adopted revision/image/history identity differs")
    inputs = {V14_INPUT: runtime._sha(content), f"git:{descriptor['reference']}@{blob}": descriptor["sha256"]}
    required = {"library", "assembly", "callers", "proof", "ledger", "names", "header",
                "parent_evidence", "parent_review"}
    if set(revision["inputs"]) != required:
        raise ValueError("v14 requires its complete fixed review input set")
    for label, retained in revision["inputs"].items():
        payload = _v14_retained(root, retained)
        revision["_" + label] = payload.decode("utf-8") if label in ("header", "parent_review") else json.loads(payload)
        inputs[retained["path"]] = retained["sha256"]
    return baseline, revision, dict(sorted(inputs.items()))


def v14_protected(baseline: dict, revision: dict) -> dict[int, dict]:
    functions = {row["address"]: row for row in baseline["functions"]}
    names = revision["_names"]["symbols"]
    protected = {}
    for unit in revision["_ledger"]["units"]:
        for name in unit["functions"]:
            address = (names[name]["address"] if name in names else int(name.removeprefix("func_"), 16)) & ~1
            function = functions.get(address)
            if address in protected or function is None or function["kind"] != "function":
                raise ValueError("v14 accepted identity is duplicated, absent or noncreditable")
            protected[address] = function
    summary = {"functions": len(protected), "bytes": sum(row["size"] for row in protected.values()),
               "complete_records_sha256": local.metadata_sha([protected[address] for address in sorted(protected)])}
    if summary != revision["protection"]:
        raise ValueError("v14 accepted complete-record snapshot differs")
    return protected


def _v14_library(candidate: dict, baseline: dict, analyzer, revision: dict) -> None:
    witness = candidate["selected_vendor_witness"]
    strict = witness["strict_comparison"]
    size, address = candidate["size"], candidate["address"]
    if (not candidate["identity_authenticated"] or candidate["body_mask_family_count"] != 1
            or not candidate["admission"]["no_conflicting_body_masks"]
            or not candidate["admission"]["natural_named_or_bound_witness"]
            or candidate["relocation_matching"]["total_relocation_count"] != 0
            or candidate["relocation_matching"]["fields"]
            or witness["object_offset"] != 0 or witness["section_size"] != size
            or witness["section_base"] != address or address % witness["section_alignment"]
            or witness["declared_symbol"]["name"] != candidate["identity"]
            or witness["declared_symbol"]["type"] != 2 or witness["declared_symbol"]["size"] != size
            or witness["section_mapping"] != [[0, "$t" if candidate["isa"] == "thumb" else "$a"]]
            or not all(strict[key] for key in ("whole_relocated_body_equal", "whole_section_equal",
                                              "extent_equal", "isa_equal", "alignment_equal",
                                              "admitted_without_linker_extension"))
            or strict["relocations"] or strict["rewrites"]
            or {witness["object_body_sha256"], witness["image_body_sha256"],
                strict["relocated_body_sha256"], candidate["original_extent_sha256"]} != {runtime._sha(_read(analyzer, address, size))}):
        raise ValueError("v14 library requires an exact complete relocation-free named definition")
    functions = {row["address"]: row for row in baseline["functions"]}
    incoming = []
    for named in candidate["named_identity_witnesses"]:
        if named["symbol"] != candidate["identity"] or named["target"] != address:
            raise ValueError("v14 independent named identity anchor differs")
        if named["kind"] == "authenticated_whole_library_caller_named_relocation":
            parent = revision["_library"]["anchors"]["callback_parent"]
            owner = functions[parent["address"]]
            if (owner["kind"] != "library"
                    or owner["classification_evidence"]["identity"] != parent["current_classification_identity"]
                    or not parent["whole_relocated_body_equal"] or not parent["whole_section_equal"]
                    or parent["relocated_body_sha256"] != runtime._sha(_read(analyzer, owner["address"], owner["size"]))
                    or named not in revision["_library"]["anchors"]["callers"][str(address)]):
                raise ValueError("v14 callback name lacks its independently authenticated complete parent")
        elif named["kind"] == "whole_natural_caller_named_relocation":
            retained = next((row for row in revision["_callers"]["callers"]
                             if row["original_receipt"] == named["receipt"]), None)
            if retained is None or retained["source"] != named["source"] or retained["object"] != named["object"]:
                raise ValueError("v14 named caller lacks its retained source/object/receipt binding")
            receipt = retained["receipt_without_instruction_encodings"]
            matched = next((row for row in receipt["functions"] if row["address"] == named["address"]), None)
            if (runtime._sha(retained["source_text"].encode()) != named["source"]["sha256"]
                    or receipt["source_sha256"] != named["source"]["sha256"]
                    or receipt["object_sha256"] != named["object"]["sha256"]
                    or receipt["image_sha256"] != baseline["image_sha256"]
                    or matched is None or matched["status"] != "match"
                    or matched["size"] != named["size"] or matched["candidate_size"] != named["size"]
                    or any(row["offset"] == named["offset"] for row in matched.get("linker_rewrites", []))):
                raise ValueError("v14 identity caller is partial, rewritten at the name, or unbound")
        else:
            raise ValueError("v14 identity anchor kind is unsupported")
        placed = next((row for row in candidate["caller_witnesses"] if row["site"] == named["site"]
                       and row["kind"] == "direct_call"), None)
        if placed is not None:
            owner = functions[placed["address"]]
            incoming.append({"owner": owner["address"], "site": placed["site"], "isa": placed["isa"],
                             "target": address, "target_isa": candidate["isa"], "conditional": placed["conditional"],
                             "record_sha256": local.metadata_sha(owner), "image_sha256": placed["whole_caller_extent_sha256"]})
    _incoming(baseline, analyzer, incoming)


def check_v14(baseline: dict, analyzer, revision: dict) -> list[dict]:
    """Check certificates before mutation; short-body coincidences alone refuse."""
    proof = revision["_proof"]
    if (proof["revision"] != "catalogue-v14" or proof["adoption"] is not False or proof["new_C_credit"] != 0
            or proof["baseline"] != revision["baseline"] or proof["image_sha256"] != revision["image_sha256"]
            or local.metadata_sha(baseline) != revision["baseline"]["sha256"]
            or runtime._sha(analyzer.image) != revision["image_sha256"]):
        raise ValueError("v14 certificate baseline/image differs")
    functions = {row["address"]: row for row in baseline["functions"]}
    library = revision["_library"]
    assembly = revision["_assembly"]
    if library["image_sha256"] != revision["image_sha256"] or assembly["image_sha256"] != revision["image_sha256"]:
        raise ValueError("v14 retained evidence image differs")
    sources = {}
    for candidate in library["candidates"]:
        _v14_library(candidate, baseline, analyzer, revision)
        sources[candidate["address"]] = (candidate, "library", candidate["identity"], candidate["original_extent_sha256"])
    family = assembly["authors"]["fa03"]["record"]["family"]
    authors = assembly["authors"]
    upstream = authors["fa03"]["record"]["upstream"]
    if (assembly["canonical_header"] != revision["inputs"]["header"]
            or authors["fa03"]["record"]["input_sha256"]["include/nw/gfx/gfx_MemoryUtil.h"]
            != revision["inputs"]["header"]["sha256"]
            or any((source["commit"], source["sha256"]) != (upstream["commit"], upstream["sha256"])
                   for source in (authors["fa04"]["record"]["public_source"], authors["fa05"]["record"]["upstream"]))):
        raise ValueError("v14 assembly source/header provenance differs")
    by_name = {row["function"]: row for row in family}
    for row in authors["fa04"]["record"]["functions"]:
        source = by_name[row["function"]]
        if (row["size"] != source["size"] or row["original_extent_sha256"] != source["original_extent_sha256"]
                or not row["whole_operation_trace_equal"] or not row["complete_code_range"]
                or row["original_operation_sha256"] != row["public_operation_sha256"]):
            raise ValueError("v14 fa04 complete-operation corroboration differs")
    for row in authors["fa05"]["record"]["functions"]:
        source = by_name[row["function"]]
        if (row["size"] != source["size"] or row["original_owner_sha256"] != source["original_extent_sha256"]
                or not row["complete_operation_sequence_equal"] or not row["complete_catalogue_code_range"]
                or row["differing_operation_offsets"]):
            raise ValueError("v14 fa05 complete-operation corroboration differs")
    declarations = [int(value) for value in re.findall(r"^DECL_SUB\((\d+)\)$", revision["_header"], re.MULTILINE)]
    if ("asm u32* FastWordCopyAsm_##size" not in revision["_header"]
            or declarations != [row["word_count"] for row in family]
            or [int(row["address"], 16) for row in family] != sorted({int(row["address"], 16) for row in family})):
        raise ValueError("v14 assembly requires the entire explicitly declared ordered family")
    cursor = assembly["family_start"]
    already = []
    for source in family:
        address = int(source["address"], 16)
        function = functions[address]
        if (address != cursor or source["size"] != function["size"] or source["isa"] != "arm"
                or function["code_ranges"] != [[address, address + source["size"]]]
                or function["pools"] or function["literals"]
                or not source["normalized_instruction_identity"] or not source["full_extent_accounted"]
                or source["non_instruction_span_count"] or source["different_instruction_ordinals"]
                or source["expected_instruction_count"] != source["observed_instruction_count"]
                or source["expected_instruction_count"] * 4 != source["size"]
                or source["expected_instruction_sequence_sha256"] != source["observed_instruction_sequence_sha256"]
                or source["public_symbol"] != f"nw::gfx::internal::FastWordCopyAsm_{source['word_count']}"):
            raise ValueError("v14 assembly lacks complete exact public-macro correspondence")
        cursor += source["size"]
        if function["kind"] != "function":
            if function["kind"] != "assembly":
                raise ValueError("v14 assembly family conflicts with an existing classification")
            already.append(source["function"])
        else:
            sources[address] = (source, "assembly", source["public_symbol"], source["original_extent_sha256"])
    if cursor != assembly["family_end"] or already != assembly["already_classified"]:
        raise ValueError("v14 assembly family extent/already-classified accounting differs")
    rows = proof["classifications"]
    if len(sources) != len(library["candidates"]) + len(family) - len(already):
        raise ValueError("v14 duplicate evidence owner")
    if [row["address"] for row in rows] != sorted(sources):
        raise ValueError("v14 certificates omit, duplicate or add an evidenced owner")
    protected = v14_protected(baseline, revision)
    for row in rows:
        function = functions[row["address"]]
        source, kind, identity, extent_sha = sources[row["address"]]
        if (row["address"] in protected or function["kind"] != "function"
                or "classification_evidence" in function
                or (row["size"], row["isa"]) != (function["size"], function["isa"])
                or row["kind"] != kind or row["identity"] != identity or row["extent_sha256"] != extent_sha
                or row["before_sha256"] != local.metadata_sha(function)
                or row["source_sha256"] != local.metadata_sha(source)
                or extent_sha != runtime._sha(_read(analyzer, row["address"], row["size"]))):
            raise ValueError("v14 classification changes a protected record, identity or exact extent")
    return rows


def propose_v14(baseline: dict, analyzer, revision: dict, inputs: dict[str, str],
                sources: dict[str, str]) -> dict:
    rows = check_v14(baseline, analyzer, revision)
    result = deepcopy(baseline)
    functions = {row["address"]: row for row in result["functions"]}
    certificate = {"image_sha256": revision["image_sha256"], "non_c_proof": revision["inputs"]["proof"]}
    for row in rows:
        functions[row["address"]]["kind"] = row["kind"]
        functions[row["address"]]["classification_evidence"] = _classification(row, certificate)
    _statistics(result["stats"], rows, 1)
    result["closure_evidence"][V14_KEY] = {"proof": revision["inputs"]["proof"], "classifications": rows, "new_C_credit": 0}
    result["generator"] = {"command": "tools/catalogue_non_c.py propose-v14 --out X",
                           "revision": "catalogue-v14", "source_sha256": sources,
                           "input_sha256": inputs, "parent": revision["baseline"],
                           "policy": revision["policy"]}
    protected = v14_protected(baseline, revision)
    result["revision_review"] = {
        "name": "catalogue-v14", "adoption": False, "new_C_credit": 0,
        "baseline": revision["baseline"], "parent_review_sha256": local.metadata_sha(baseline["revision_review"]),
        "classifications": [functions[row["address"]]["classification_evidence"] for row in rows],
        "changed_records": [{"address": row["address"], "changed_fields": ["classification_evidence", "kind"]} for row in rows],
        "protected_summary": revision["protection"],
        "protected_rows": [{"address": address, "size": function["size"], "record_sha256": local.metadata_sha(function)}
                           for address, function in sorted(protected.items())],
        "preservation": {"unchanged_complete_function_records": len(functions) - len(rows),
                         "extent_changes": [], "accepted_record_changes": [], "entry_changes": [],
                         "physical_range_changes": [], "unknown_changes": [], "anomaly_changes": []},
        "unknown_ranges": [row for row in baseline["ranges"] if row["kind"] == "unknown"],
        "anomalies": baseline["anomalies"], "already_classified_assembly_family": revision["_assembly"]["already_classified"],
        "adjudications": revision["_library"]["adjudications"], "input_sha256": {**inputs, **sources},
        "qualification": revision["qualification"],
    }
    if __package__:
        from . import analyze
    else:
        import analyze
    analyze.check_protected_functions(result, protected)
    return result


def validate_v14(candidate: dict, baseline: dict, analyzer, revision: dict,
                 inputs: dict[str, str], sources: dict[str, str]) -> None:
    """Recompute from independently supplied bindings, never the candidate's claims."""
    expected = propose_v14(baseline, analyzer, revision, inputs, sources)
    if candidate != expected:
        raise ValueError("v14 successor differs outside its exact certified kind/evidence transition")


def prove_v14_objects(private_root: Path, baseline: dict, analyzer, revision: dict) -> dict:
    """Reopen five exact licensed definitions and the actual named caller objects."""
    if __package__:
        from . import fingerprint, image, inplace
    else:
        import fingerprint
        import image
        import inplace
    functions = {row["address"]: row for row in baseline["functions"]}
    original = image.Image(analyzer.image, analyzer.layout)
    linker = inplace.InPlaceLinker(original, functions, inplace.Resolver(functions), original.bss)
    checked = []
    witnesses = [row["selected_vendor_witness"] for row in revision["_library"]["candidates"]]
    parent = revision["_library"]["anchors"]["callback_parent"]
    witnesses.append({**parent, "symbol": parent["current_classification_identity"], "object_section": 1,
                      "object_offset": 0, "section_base": parent["address"], "section_size": parent["size"]})
    for witness in witnesses:
        archive = private_root / witness["archive"]
        if runtime._sha(archive.read_bytes()) != witness["archive_sha256"]:
            raise ValueError("v14 private archive digest differs")
        member = next((row for row in fingerprint.archive_members(archive) if row.offset == witness["member_offset"]), None)
        if member is None or member.name != witness["member"] or runtime._sha(member.data) != witness["member_sha256"]:
            raise ValueError("v14 private selected member differs")
        obj = inplace.ObjectFile(Path(member.name), data=member.data)
        section = obj.sections[witness["object_section"]]
        definition = next((symbol for symbol in obj.defined_functions() if symbol.name == witness["symbol"]), None)
        if (definition is None or definition.shndx != section.index or inplace._symbol_offset(definition) != 0
                or not section.executable or section.size != witness["section_size"]):
            raise ValueError("v14 private complete declaration/section differs")
        data = bytearray(section.data)
        if witness is witnesses[-1]:
            retained = parent["relocations"]
            if len(section.relocs) != len(retained):
                raise ValueError("v14 authenticated parent relocation inventory differs")
            for rel, record in zip(section.relocs, retained):
                if (rel.offset, rel.type, rel.symbol.name) != (record["offset"], record["type"], record["symbol"]):
                    raise ValueError("v14 authenticated parent named relocation differs")
                linker._apply(rel, section, data, witness["section_base"] + rel.offset,
                              inplace.Target(record["target"], record["target_thumb"]))
        elif section.relocs or definition.size != section.size:
            raise ValueError("v14 private definition is partial or needs relocations")
        if bytes(data) != original.read(witness["section_base"], section.size):
            raise ValueError("v14 private whole section equality differs")
        checked.append({"symbol": witness["symbol"], "size": section.size, "whole_section_sha256": runtime._sha(data)})
    named_count = 0
    for caller in revision["_callers"]["callers"]:
        path = private_root / caller["object"]["path"]
        if runtime._sha(path.read_bytes()) != caller["object"]["sha256"]:
            raise ValueError("v14 actual named caller object digest differs")
        obj = inplace.ObjectFile(path)
        for candidate in revision["_library"]["candidates"]:
            for named in candidate["named_identity_witnesses"]:
                if named.get("object") != caller["object"]:
                    continue
                section = next((row for row in obj.sections.values() if row.name == named["object_section"]), None)
                rel = next((row for row in section.relocs if row.offset == named["offset"]), None) if section else None
                if (rel is None or (rel.type, rel.symbol.name, rel.symbol.kind)
                        != (named["relocation_type"], named["symbol"], named["symbol_kind"])):
                    raise ValueError("v14 actual named caller relocation differs")
                data = bytearray(section.data)
                linker._apply(rel, section, data, named["site"], inplace.Target(named["target"], named["target_isa"] == "thumb"))
                if bytes(data[rel.offset:rel.offset + 4]) != original.read(named["site"], 4):
                    raise ValueError("v14 actual named caller target reencoding differs")
                named_count += 1
    return {"complete_vendor_sections": checked, "named_caller_relocations": named_count, "new_C_credit": 0}


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    command = sub.add_parser("propose-v14", help="reclassify only the reviewed unchanged v13-r2 owners")
    root = Path(__file__).resolve().parents[1]
    command.add_argument("--out", type=Path, required=True)
    command.add_argument("--image", type=Path, default=root / "orig/code.bin")
    command.add_argument("--exheader", type=Path, default=root / "orig/exheader.bin")
    command.add_argument("--prove-objects", action="store_true", help="also reopen exact private archive/caller objects; no compilation")
    command.add_argument("--private-root", type=Path, default=root)
    args = parser.parse_args(argv)
    try:
        if __package__:
            from . import analyze, ctrfmt
        else:
            import analyze
            import ctrfmt
        baseline, revision, inputs = load_v14_inputs(root)
        image_bytes = args.image.read_bytes()
        exheader = args.exheader.read_bytes()
        analyzer = analyze.Analyzer(image_bytes, ctrfmt.parse_exheader(exheader))
        inputs.update({"image": runtime._sha(image_bytes), "exheader": runtime._sha(exheader)})
        sources = {f"tools/{name}": runtime._sha((root / "tools" / name).read_bytes()) for name in (
            "catalogue_non_c.py", "catalogue_local_entries.py", "catalogue_runtime_data.py", "catalogue_review.py",
            "analyze.py", "ctrfmt.py", "fingerprint.py", "image.py", "inplace.py", "linker_models.py")}
        result = propose_v14(baseline, analyzer, revision, dict(sorted(inputs.items())), sources)
        if args.prove_objects:
            receipt = prove_v14_objects(args.private_root, baseline, analyzer, revision)
            print(f"private proof: {len(receipt['complete_vendor_sections'])} whole vendor sections; "
                  f"{receipt['named_caller_relocations']} retained named caller relocations")
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_bytes(analyze.serialize_catalogue(result))
    except (OSError, ValueError, KeyError, struct.error) as error:
        print(f"catalogue-v14: {error}", file=sys.stderr)
        return 1
    print(f"catalogue-v14: {result['stats']['creditable_functions']} creditable functions; "
          f"{result['stats']['creditable_bytes']} extent bytes; proposal only; {args.out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
