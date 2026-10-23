"""Original task argument facts only; no code/encoding/literal output."""
from __future__ import annotations

import argparse
import hashlib
from collections import deque
import importlib.util
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[4]
OUT = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location("priority_width_transfer", OUT / "priority_facts.py")
flow = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = flow
spec.loader.exec_module(flow)
from capstone import arm
import argcheck
import capstone
import image


def abstract(value):
    if value is None:
        return {"kind": "unknown"}
    if value[0] == "parameter":
        return {"kind": "entry", "register": value[2]}
    if value[0] in ("word-load", "load"):
        return {"kind": "word-memory-read", "from": abstract(value[1])}
    if value[0] == "word-read-site":
        return {"kind": "word-memory-read-with-unresolved-base",
                "site_offset": value[2] - value[1],
                "field_offset": value[3], "bits": value[4]}
    if value[0] == "offset":
        return {"kind": "offset", "base": abstract(value[1]), "offset": value[2]}
    if value[0] == "narrow":
        return {"kind": "narrow", "bits": value[1], "value": abstract(value[2])}
    if value[0] == "result":
        return {"kind": "prior-call-register-unspecified", "meaningful_result_proven": False}
    return {"kind": value[0]}


def transfer(insn, fact, state, original, owner, ranges):
    before = state.copy()
    event = flow.transfer(insn, fact, state, original)
    mem = next((op for op in insn.operands if op.type == arm.ARM_OP_MEM), None)
    if mem is not None and insn.id == arm.ARM_INS_LDR and insn.operands[0].type == arm.ARM_OP_REG:
        destination = argcheck.canonical(insn.reg_name(insn.operands[0].reg))
        if destination not in state and not fact.conditional:
            state[destination] = ("word-read-site", owner, fact.address, mem.mem.disp, 32)
    if fact.flow == "branch" and fact.target is not None and not any(start <= fact.target < stop for start, stop in ranges):
        event = {"target": fact.target, "tail": True,
                 "registers": {f"r{i}": before.get(f"r{i}") for i in range(4)}}
    return event


def inspect(original, function):
    owner = function["address"]
    decoder = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB if function["isa"] == "thumb" else capstone.CS_MODE_ARM)
    decoder.detail = True
    instructions = {insn.address: insn for start, stop in function["code_ranges"]
                    for insn in decoder.disasm(original.read(start, stop - start), start)}
    facts = argcheck.decode_function(original, function, decoder)
    entries = {entry["address"] for entry in function["entries"]}
    blocks, successors = argcheck.blocks(facts, sorted(entries))
    initial = {f"r{i}": ("parameter", owner, f"r{i}") for i in range(4)}
    initial["sp"] = ("stack", owner, 0)
    incoming = {i: initial.copy() for i, block in enumerate(blocks) if block[0].address in entries}
    pending = deque(incoming)
    while pending:
        index = pending.popleft()
        state = incoming[index].copy()
        for fact in blocks[index]:
            transfer(instructions[fact.address], fact, state, original, owner, function["code_ranges"])
        for successor in successors[index]:
            previous = incoming.get(successor)
            merged = state.copy() if previous is None else {key: item for key, item in previous.items() if state.get(key) == item}
            if previous != merged:
                incoming[successor] = merged
                pending.append(successor)
    accesses, comparisons, calls, manager_forwards = [], [], [], set()
    for index, state in incoming.items():
        state = state.copy()
        for fact in blocks[index]:
            insn = instructions[fact.address]
            before = state.copy()
            reg = lambda op: argcheck.canonical(insn.reg_name(op.reg))
            operand = lambda op: before.get(reg(op)) if op.type == arm.ARM_OP_REG else ("constant", op.imm) if op.type == arm.ARM_OP_IMM else None
            mem = next((op for op in insn.operands if op.type == arm.ARM_OP_MEM), None)
            if mem is not None:
                base = before.get(argcheck.canonical(insn.reg_name(mem.mem.base)))
                if base and base[0] == "parameter":
                    accesses.append({"site_offset": fact.address - owner, "base_register": base[2],
                                     "field_offset": mem.mem.disp, "writes_memory": insn.id in (arm.ARM_INS_STR, arm.ARM_INS_STRB, arm.ARM_INS_STRH)})
            if insn.id == arm.ARM_INS_CMP and len(insn.operands) == 2:
                values = [operand(op) for op in insn.operands]
                if ("parameter", owner, "r1") in values:
                    other = values[1] if values[0] == ("parameter", owner, "r1") else values[0]
                    comparisons.append({"site_offset": fact.address - owner, "input_register": "r1", "other": abstract(other)})
            event = transfer(insn, fact, state, original, owner, function["code_ranges"])
            if event and event["target"] is not None:
                forwarded = event["registers"].get("r1") == ("parameter", owner, "r1")
                same_manager = event["registers"].get("r0") == ("parameter", owner, "r0")
                calls.append({"site_offset": fact.address - owner, "target": f"func_{event['target']:08x}",
                              "same_entry_receiver": same_manager, "entry_r1_present": forwarded,
                              "arg1_is_entry_r0": event["registers"].get("r1") == ("parameter", owner, "r0"),
                              "tail": event.get("tail", False),
                              "arg1": abstract(event["registers"].get("r1"))})
                if forwarded and same_manager and event["target"] != 0x00185DE4:
                    manager_forwards.add(event["target"])
    return {"function": f"func_{owner:08x}", "entry_based_memory_accesses": accesses,
            "entry_r1_comparisons": comparisons, "calls": calls}, manager_forwards


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--run", required=True)
    args = parser.parse_args()
    original = image.Image.load()
    functions = image.functions_by_address(image.load_catalogue(ROOT / "config/catalogue.json"))
    queue = deque([(0x001D32E0, 0), (0x001D32E4, 0), (0x001D6B24, 0), (0x001D6678, 0)])
    seen, results = set(), []
    while queue:
        address, depth = queue.popleft()
        if address in seen:
            continue
        seen.add(address)
        result, forwards = inspect(original, functions[address])
        results.append(result)
        if depth < 2:
            queue.extend((target, depth + 1) for target in sorted(forwards)
                         if target in functions and functions[target]["kind"] == "function" and functions[target]["size"] <= 512)
    report = {"schema_version": 2, "image_sha256": original.sha256, "functions": results,
              "analysis_script_sha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
              "native_contracts": {"TaskManager::CancelTask": "void(Task*)", "TaskManager::CancelTaskById": "void(u32)"},
              "qualification": "Abstract scoped CFG facts only, with conservative merge and explicit load/store-width tags. Presence of an entry register at a call does not establish actual argument arity. Unknown prior-call r0 is not evidence of a meaningful result. Public API names alone do not bind addresses or invent owner identities."}
    flow.audit.save(OUT / f"task-facts-{args.run}.json", report)
    print(json.dumps({"functions": len(results), "root_r1_memory_accesses": sum(row["base_register"] == "r1" for result in results for row in result["entry_based_memory_accesses"]), "root_comparisons": sum(len(result["entry_r1_comparisons"]) for result in results)}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
