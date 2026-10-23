"""Private original-image dataflow; publish callback identities and semantic facts only.

This is an evidence inventory, not an interfaces/argcheck gate or a build. All
instructions and image buffers stay in memory. Original graph coverage is whole
catalogue; expensive symbolic work is limited to calls and passed code pointers.
"""
from __future__ import annotations

import argparse
from collections import defaultdict, deque
import hashlib
import json
from pathlib import Path
import re
import struct
import sys
from typing import Any

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT / "tools"))
import argcheck
import image as image_mod
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM, CS_MODE_THUMB, arm

Value = tuple[Any, ...]


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def address(value: int) -> str:
    return f"0x{value:08x}"


def public_value(value: Value | None, entries: dict[int, int], aliases: dict[int, list[str]]) -> dict:
    if value is None:
        return {"kind": "unknown"}
    if value[0] == "constant":
        raw = value[1]
        normalized = raw & ~1
        if normalized in entries:
            owner = entries[normalized]
            return {"kind": "function_address", "address": address(normalized),
                    "owner": address(owner), "names": aliases.get(normalized, [f"func_{normalized:08x}"])}
        if raw == 0:
            return {"kind": "null"}
        # Scalar values are not exported without a known ABI role. A digest is
        # enough to retain equality while avoiding arbitrary original payload.
        return {"kind": "nonfunction_constant", "value_sha256": hashlib.sha256(struct.pack("<I", raw & 0xffffffff)).hexdigest()}
    if value[0] == "parameter":
        return {"kind": "parameter", "function": address(value[1]), "register": f"r{value[2]}"}
    if value[0] == "stack_parameter":
        return {"kind": "stack_parameter", "function": address(value[1]), "entry_stack_offset": value[2]}
    if value[0] == "stack":
        return {"kind": "stack_address", "function": address(value[1]), "entry_stack_offset": value[2]}
    if value[0] == "load":
        return {"kind": "load", "storage": public_value(value[1], entries, aliases)}
    if value[0] == "offset":
        return {"kind": "offset", "base": public_value(value[1], entries, aliases), "offset": value[2]}
    return {"kind": "unmodeled_symbolic_value"}


def memory_key(value: Value | None) -> str | None:
    if value is not None and value[0] == "stack":
        return f"memory:{value[1]}:{value[2]}"
    return None


def plus(value: Value | None, amount: int) -> Value | None:
    if value is not None and value[0] == "stack":
        return ("stack", value[1], value[2] + amount)
    return argcheck.plus(value, amount)


def location(fact: argcheck.Fact, state: dict[str, Value]) -> Value | None:
    if fact.memory is None:
        return None
    base, index, displacement, scale, shift = fact.memory
    value = ("constant", fact.pc) if base == "pc" else state.get(base)
    if index:
        idx = state.get(index)
        if idx is None or idx[0] != "constant":
            return None
        displacement += idx[1] * scale * (1 << shift)
    return plus(value, displacement)


def read_word(loc: Value | None, state: dict[str, Value], image: image_mod.Image) -> Value | None:
    key = memory_key(loc)
    if key and key in state:
        return state[key]
    if loc is None:
        return None
    if loc[0] == "stack" and loc[2] >= 0:
        return ("stack_parameter", loc[1], loc[2])
    if loc[0] == "constant":
        try:
            return ("constant", struct.unpack("<I", image.read(loc[1], 4))[0])
        except (ValueError, struct.error, image_mod.ImageError):
            pass
    return ("load", loc)


def apply(fact: argcheck.Fact, insn: Any, state: dict[str, Value], image: image_mod.Image) -> None:
    before = state.copy()
    register_list = [argcheck.canonical(insn.reg_name(op.reg)) for op in insn.operands if op.type == arm.ARM_OP_REG]
    is_push = insn.id in (arm.ARM_INS_PUSH, arm.ARM_INS_VPUSH)
    is_pop = insn.id in (arm.ARM_INS_POP, arm.ARM_INS_VPOP)
    if is_push or is_pop:
        if fact.conditional:
            for reg in fact.writes:
                state.pop(reg, None)
            return
        widths = [8 if r.startswith("d") else 4 for r in register_list]
        total = sum(widths)
        sp = before.get("sp")
        cursor = plus(sp, -total) if is_push else sp
        for reg, width in zip(register_list, widths):
            key = memory_key(cursor)
            if is_push and key and reg in before:
                state[key] = before[reg]
            elif is_pop:
                if key and key in before:
                    state[reg] = before[key]
                else:
                    state.pop(reg, None)
            cursor = plus(cursor, width)
        state["sp"] = plus(sp, -total if is_push else total) or ("unknown_stack",)
        return
    multiple_load_ids = {getattr(arm, n) for n in ("ARM_INS_LDM", "ARM_INS_LDMIB", "ARM_INS_LDMDA", "ARM_INS_LDMDB") if hasattr(arm, n)}
    if insn.id in multiple_load_ids and register_list:
        base_reg, loaded = register_list[0], register_list[1:]
        base_value = before.get(base_reg)
        first = 4 if insn.id == getattr(arm, "ARM_INS_LDMIB", -1) else (
            -4 * len(loaded) if insn.id == getattr(arm, "ARM_INS_LDMDB", -1) else (
                -4 * (len(loaded) - 1) if insn.id == getattr(arm, "ARM_INS_LDMDA", -1) else 0))
        for i, reg in enumerate(loaded):
            value = read_word(plus(base_value, first + i * 4), before, image)
            if value is not None and (not fact.conditional or before.get(reg) == value):
                state[reg] = value
            else:
                state.pop(reg, None)
        if insn.writeback:
            state[base_reg] = plus(base_value, (4 if first >= 0 else -4) * len(loaded)) or ("unknown_stack",)
        return
    if insn.id == arm.ARM_INS_LDRD and len(register_list) >= 2:
        loc = location(fact, before)
        for i, reg in enumerate(register_list[:2]):
            value = read_word(plus(loc, i * 4), before, image)
            if value is not None and (not fact.conditional or before.get(reg) == value):
                state[reg] = value
            else:
                state.pop(reg, None)
        return
    if fact.flow == "call":
        for reg in argcheck.CLOBBERED:
            state.pop(reg, None)
        return
    value = None
    if fact.operation in ("move", "adr") and len(fact.operands) > 1:
        value = argcheck.operand_value(fact.operands[1], before, fact)
    elif fact.operation == "movt" and len(fact.operands) > 1:
        old = before.get(fact.operands[0][1])
        high = argcheck.operand_value(fact.operands[1], before, fact)
        if old and high and old[0] == high[0] == "constant":
            value = ("constant", (old[1] & 0xffff) | (high[1] << 16))
    elif fact.operation in ("add", "sub") and len(fact.operands) == 3:
        left = argcheck.operand_value(fact.operands[1], before, fact)
        right = argcheck.operand_value(fact.operands[2], before, fact)
        if right and right[0] == "constant":
            value = plus(left, right[1] if fact.operation == "add" else -right[1])
    elif fact.operation in ("load", "store"):
        loc = location(fact, before)
        key = memory_key(loc)
        if fact.operation == "store":
            val = argcheck.operand_value(fact.operands[0], before, fact)
            if key and val is not None and not fact.conditional:
                state[key] = val
        elif key and key in before:
            value = before[key]
        elif loc is not None:
            if loc[0] == "stack" and loc[2] >= 0:
                value = ("stack_parameter", loc[1], loc[2])
            elif loc[0] == "constant":
                try:
                    value = ("constant", struct.unpack("<I", image.read(loc[1], 4))[0])
                except (ValueError, struct.error, image_mod.ImageError):
                    value = ("load", loc)
            else:
                value = ("load", loc)
    for reg in fact.writes:
        state.pop(reg, None)
    if value is not None and fact.operands and fact.operands[0][0] == "reg":
        reg = fact.operands[0][1]
        if not fact.conditional or before.get(reg) == value:
            state[reg] = value


def symbolic(function: dict, facts: list[argcheck.Fact], decoded: dict[int, Any], image: image_mod.Image,
             entry_owners: dict[int, int], *, primary_only: bool = False) -> list[dict]:
    blocks, successors = argcheck.blocks(facts, [e["address"] for e in function["entries"]])
    incoming: dict[int, dict[str, Value]] = {}
    queue: deque[int] = deque()
    starts = {function["address"]} if primary_only else {e["address"] for e in function["entries"]}
    for i, block in enumerate(blocks):
        if block[0].address in starts:
            incoming[i] = {f"r{r}": ("parameter", function["address"], r) for r in range(4)}
            incoming[i]["sp"] = ("stack", function["address"], 0)
            queue.append(i)
    while queue:
        i = queue.popleft()
        state = incoming[i].copy()
        for fact in blocks[i]:
            apply(fact, decoded[fact.address], state, image)
        for target in successors[i]:
            old = incoming.get(target)
            merged = state.copy() if old is None else {r: v for r, v in old.items() if state.get(r) == v}
            if merged != old:
                incoming[target] = merged
                queue.append(target)
    rows = []
    for i, initial in sorted(incoming.items()):
        state = initial.copy()
        for fact in blocks[i]:
            tail = fact.flow == "branch" and fact.target in entry_owners and not (
                function["address"] <= fact.target < function["address"] + function["size"])
            indirect_return_target = None
            if function["kind"] == "veneer" and fact.flow == "return" and fact.operands and fact.operands[0][0] == "reg":
                indirect_return_target = state.get(fact.operands[0][1])
            if fact.flow == "call" or tail or indirect_return_target is not None:
                target = fact.target
                target_value = state.get(fact.target_register) if fact.target_register else indirect_return_target
                if target is None and target_value and target_value[0] == "constant":
                    target = target_value[1] & ~1
                args = {f"r{n}": state.get(f"r{n}") for n in range(4)}
                sp = state.get("sp")
                stack = {}
                for off in range(0, 32, 4):
                    key = memory_key(plus(sp, off))
                    if key and key in state:
                        stack[str(off)] = state[key]
                rows.append({"caller": function["address"], "site": fact.address,
                             "kind": "veneer_tail" if indirect_return_target else ("tail" if tail else "call"),
                             "target": target, "target_value": target_value,
                             "arguments": args, "stack_arguments": stack})
            apply(fact, decoded[fact.address], state, image)
    return rows


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    out = ROOT / args.out
    if out.exists():
        raise RuntimeError(f"Immutable output already exists: {out}")
    catalogue = image_mod.load_catalogue(ROOT / "config/catalogue.json")
    image = image_mod.Image.load()
    names = image_mod.load_names()
    entries: dict[int, int] = {}
    aliases: dict[int, list[str]] = defaultdict(list)
    for f in catalogue["functions"]:
        entries[f["address"]] = f["address"]
        aliases[f["address"]].append(f"func_{f['address']:08x}")
        for entry in f["entries"]:
            entries[entry["address"]] = f["address"]
    for name, value in names.items():
        aliases[value["address"] & ~1].append(name)
    calls = []
    coverage = []
    errors = []
    runtime_entries = []
    rx = re.compile(r"__aeabi_vec_|__cxa_vec_|__vec_|vec_(?:ctor|dtor|new|delete)|atexit|qsort|bsearch")
    for function in catalogue["functions"]:
        owner = function["address"]
        classification = function.get("classification_evidence", {})
        identity = classification.get("identity", "") if isinstance(classification, dict) else ""
        if identity and identity not in aliases[owner]:
            aliases[owner].append(identity)
        if rx.search(identity) or any(rx.search(n) for n in aliases[owner]):
            runtime_entries.append({"address": address(owner), "names": sorted(set(aliases[owner])),
                                    "kind": function["kind"], "size": function["size"], "isa": function["isa"],
                                    "classification_evidence": classification})
        decoder = Cs(CS_ARCH_ARM, CS_MODE_THUMB if function["isa"] == "thumb" else CS_MODE_ARM)
        decoder.detail = True
        try:
            facts = argcheck.decode_function(image, function, decoder)
            decoded = {ins.address: ins for start, stop in function["code_ranges"]
                       for ins in decoder.disasm(image.read(start, stop-start), start)}
            rows = symbolic(function, facts, decoded, image, entries)
            if len(function["entries"]) > 1:
                primary_rows = {row["site"]: row for row in symbolic(
                    function, facts, decoded, image, entries, primary_only=True)}
                for row in rows:
                    if row["site"] in primary_rows:
                        row["primary_entry_arguments"] = primary_rows[row["site"]]["arguments"]
            calls.extend(rows)
            coverage.append({"function": address(owner), "size": function["size"], "kind": function["kind"],
                             "decoded_instruction_count": len(facts), "call_or_tail_count": len(rows)})
        except (argcheck.ArgcheckError, ValueError, KeyError) as exc:
            errors.append({"function": address(owner), "error": str(exc)})
    public_calls = []
    passed = []
    invocation_parameters = []
    for row in calls:
        public = {"caller": address(row["caller"]), "site": address(row["site"]), "kind": row["kind"],
                  "target": address(row["target"]) if row["target"] is not None else None,
                  "target_owner": address(entries[row["target"]]) if row["target"] in entries else None,
                  "target_names": sorted(set(aliases.get(row["target"], []))),
                  "target_value": public_value(row["target_value"], entries, aliases),
                  "arguments": {r: public_value(v, entries, aliases) for r, v in row["arguments"].items()},
                  "stack_arguments": {r: public_value(v, entries, aliases) for r, v in row["stack_arguments"].items()}}
        if "primary_entry_arguments" in row:
            public["primary_entry_arguments"] = {
                r: public_value(v, entries, aliases) for r, v in row["primary_entry_arguments"].items()}
        if row["target"] in {0x1911c4, 0x1911cc, 0x19f6ac, 0x19f6b4}:
            public["array_semantics"] = {}
            for role, register in (("element_size", "r2"), ("element_count", "r3")):
                value = row["arguments"].get(register)
                public["array_semantics"][role] = value[1] if value and value[0] == "constant" else public_value(value, entries, aliases)
                primary = row.get("primary_entry_arguments", {}).get(register)
                if primary and primary[0] == "constant":
                    public["array_semantics"][f"primary_entry_{role}"] = primary[1]
            storage = row["arguments"].get("r0")
            if storage and storage[0] == "constant":
                public["array_semantics"]["array_storage_address"] = address(storage[1])
        public_calls.append(public)
        for reg, value in {**row["arguments"], **{f"stack+{k}": v for k, v in row["stack_arguments"].items()}}.items():
            fact = public_value(value, entries, aliases)
            if fact["kind"] == "function_address":
                passed.append({"caller": public["caller"], "site": public["site"], "callee": public["target"],
                               "argument_location": reg, "provider": fact})
        if row["target"] is None and row["target_value"] is not None:
            invocation_parameters.append({"owner": public["caller"], "site": public["site"],
                                          "invoked_value": public["target_value"], "arguments": public["arguments"]})
    result = {"schema_version": 1, "tool": "runtime_graph.py", "tool_sha256": digest(Path(__file__)),
              "catalogue_sha256": digest(ROOT / "config/catalogue.json"), "image_sha256": image.sha256,
              "names_sha256": digest(ROOT / "config/names.json"), "function_count": len(catalogue["functions"]),
              "decoded_function_count": len(coverage), "coverage_errors": errors,
              "runtime_named_entries": runtime_entries, "function_coverage": coverage,
              "original_call_graph": public_calls, "passed_function_address_candidates": passed,
              "indirect_invocation_parameter_candidates": invocation_parameters,
              "names_by_address": {address(a): sorted(set(ns)) for a, ns in aliases.items()},
              "entry_owner": {address(a): address(o) for a, o in entries.items()},
              "privacy": "No instruction text/encoding, original buffers or arbitrary literal words are serialized."
              }
    out.parent.mkdir(parents=True, exist_ok=True)
    with out.open("x") as stream:
        json.dump(result, stream, indent=2)
        stream.write("\n")
    print(json.dumps({"output": str(args.out), "function_count": result["function_count"],
                      "decoded_function_count": result["decoded_function_count"], "coverage_errors": len(errors),
                      "original_call_sites": len(public_calls), "passed_code_argument_candidates": len(passed),
                      "indirect_parameter_candidates": len(invocation_parameters), "named_runtime_entries": len(runtime_entries)}))
    return int(bool(errors))


if __name__ == "__main__":
    raise SystemExit(main())
