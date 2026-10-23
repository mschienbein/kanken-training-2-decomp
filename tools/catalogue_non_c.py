"""Eight v13-only non-C certificates, without changing entries, spans or aliases."""
from __future__ import annotations

from copy import deepcopy
import json
from pathlib import Path
import struct

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
