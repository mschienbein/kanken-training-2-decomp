"""Scoped original allocation/callback field and argument facts, without code."""
from __future__ import annotations

import argparse
from collections import deque
import hashlib
import importlib.util
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[4]
OUT = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location("task_field_transfer", OUT / "task_facts.py")
flow = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = flow
spec.loader.exec_module(flow)
from capstone import arm
import argcheck
import capstone
import image


def describe(value):
    if value is None:
        return {"kind": "unknown"}
    if value[0] == "parameter":
        return {"kind": "entry", "function": f"func_{value[1]:08x}", "register": value[2]}
    if value[0] == "result":
        return {"kind": "prior-call-register", "target": f"func_{value[1]:08x}" if value[1] is not None else None,
                "meaningful_result_requires_use_evidence": True}
    if value[0] == "offset":
        return {"kind": "offset", "base": describe(value[1]), "offset": value[2]}
    if value[0] in ("load", "word-load"):
        return {"kind": "word-read", "from": describe(value[1])}
    if value[0] == "constant":
        return {"kind": "constant", "is_zero": value[1] == 0}
    return flow.abstract(value)


def inspect(original, function):
    owner = function["address"]
    decoder = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB if function["isa"] == "thumb" else capstone.CS_MODE_ARM)
    decoder.detail = True
    decoded = {insn.address: insn for start, stop in function["code_ranges"]
               for insn in decoder.disasm(original.read(start, stop - start), start)}
    facts = argcheck.decode_function(original, function, decoder)
    entries = {entry["address"] for entry in function["entries"]}
    blocks, successors = argcheck.blocks(facts, sorted(entries))
    initial = {f"r{i}": ("parameter", owner, f"r{i}") for i in range(4)}
    initial["sp"] = ("stack", owner, 0)
    incoming = {i: initial.copy() for i, block in enumerate(blocks) if block[0].address in entries}
    queue = deque(incoming)
    while queue:
        index = queue.popleft()
        state = incoming[index].copy()
        for fact in blocks[index]:
            flow.transfer(decoded[fact.address], fact, state, original, owner, function["code_ranges"])
        for successor in successors[index]:
            previous = incoming.get(successor)
            merged = state.copy() if previous is None else {key: item for key, item in previous.items() if state.get(key) == item}
            if previous != merged:
                incoming[successor] = merged
                queue.append(successor)
    accesses, calls = [], []
    for index, initial in incoming.items():
        state = initial.copy()
        for fact in blocks[index]:
            insn = decoded[fact.address]
            before = state.copy()
            reg = lambda op: argcheck.canonical(insn.reg_name(op.reg))
            mem = next((op for op in insn.operands if op.type == arm.ARM_OP_MEM), None)
            if mem is not None:
                base = before.get(argcheck.canonical(insn.reg_name(mem.mem.base)))
                if base and base[0] in ("parameter", "result", "word-load", "load"):
                    store = insn.id in (arm.ARM_INS_STR, arm.ARM_INS_STRB, arm.ARM_INS_STRH)
                    value = before.get(reg(insn.operands[0])) if insn.operands[0].type == arm.ARM_OP_REG else None
                    accesses.append({"site_offset": fact.address - owner, "base": describe(base),
                                     "field_offset": mem.mem.disp, "writes_memory": store,
                                     "bits": 8 if insn.id in (arm.ARM_INS_STRB, arm.ARM_INS_LDRB, arm.ARM_INS_LDRSB) else 16 if insn.id in (arm.ARM_INS_STRH, arm.ARM_INS_LDRH, arm.ARM_INS_LDRSH) else 32,
                                     "stored_value": describe(value) if store else None})
                    if insn.id == arm.ARM_INS_STRD:
                        accesses.pop()
                        for index, operand in enumerate(insn.operands[:2]):
                            accesses.append({"site_offset": fact.address - owner,
                                             "base": describe(base),
                                             "field_offset": mem.mem.disp + index * 4,
                                             "writes_memory": True, "bits": 32,
                                             "stored_value": describe(before.get(reg(operand)))})
            event = flow.transfer(insn, fact, state, original, owner, function["code_ranges"])
            for key, value in state.items():
                if not isinstance(key, tuple) or key[0] != "memory" or before.get(key) == value:
                    continue
                location = key[1]
                if location and location[0] == "offset" and location[1] == ("parameter", owner, "r0") and value and value[0] == "parameter":
                    accesses.append({"site_offset": fact.address - owner,
                                     "base": describe(location[1]),
                                     "field_offset": location[2],
                                     "writes_memory": True, "bits": 32,
                                     "stored_value": describe(value),
                                     "evidence": "abstract multi-register field store"})
            if event:
                calls.append({"site_offset": fact.address - owner,
                              "target": f"func_{event['target']:08x}" if event["target"] is not None else "indirect",
                              "arguments": {f"r{i}": describe(event["registers"].get(f"r{i}")) for i in range(4)},
                              "live_registers_do_not_prove_arity": True})
                if event["target"] == 0x0019CECC:
                    size = event["registers"].get("r1")
                    if size and size[0] == "constant":
                        calls[-1]["cleared_byte_count"] = size[1]
    return {"function": f"func_{owner:08x}", "memory_facts": accesses, "calls": calls}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--run", required=True)
    parser.add_argument("--function", action="append", type=lambda value: int(value, 0))
    args = parser.parse_args()
    original = image.Image.load()
    functions = image.functions_by_address(image.load_catalogue(ROOT / "config/catalogue.json"))
    targets = args.function or [0x001D9198, 0x001D96E8, 0x0018290C, 0x001D5BF4, 0x001D5528]
    report = {"schema_version": 1, "image_sha256": original.sha256,
              "analysis_script_sha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
              "functions": [inspect(original, functions[address]) for address in targets],
              "qualification": "Original scoped CFG facts only. A memory read through a returned value supports that value's pointer use, but does not recover original class or native mangled identity. No common complete object type is inferred solely from offsets, SDK spelling or live incoming register presence. Indirect slots and all other consumers still need final audit."}
    flow.flow.audit.save(OUT / f"factory-facts-{args.run}.json", report)
    print(json.dumps({"functions": len(targets), "memory_facts": sum(len(row["memory_facts"]) for row in report["functions"]), "calls": sum(len(row["calls"]) for row in report["functions"])}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
