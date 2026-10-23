"""Scoped, metadata-only original priority argument/storage adjudication.

Never writes original code, encodings, literal payloads or instruction text.
The shared spill-aware transfer is extended with explicit load/store widths;
unknown return values are never presented as proven meaningful results.
"""
from __future__ import annotations

from collections import deque
import importlib.util
import json
import os
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[4]
OUT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "tools"))
os.environ["DECOMP_ROOT"] = str(ROOT)
HELPER = ROOT.parents[2] / "scratch/integrator/c008-r3/audit_indirect.py"
spec = importlib.util.spec_from_file_location("priority_spill_transfer", HELPER)
audit = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = audit
spec.loader.exec_module(audit)
from capstone import arm
import argcheck
import capstone
import image

TARGETS = {0x0016A87C, 0x001D2C84}


def category(value):
    if value is None:
        return {"origin": "unknown"}
    kind = value[0]
    if kind == "parameter":
        return {"origin": "entry-register", "register": value[2]}
    if kind == "word-load":
        return {"origin": "word-memory-load"}
    if kind == "narrow":
        return {"origin": "explicit-narrow", "bits": value[1], "input": category(value[2])}
    if kind == "when":
        return {"origin": "conditional", "otherwise": category(value[2]), "when_taken": category(value[3])}
    if kind == "result":
        return {"origin": "prior-call-register-unspecified", "meaningful_return_proven": False}
    if kind == "constant":
        return {"origin": "constant", "fits_unsigned_byte": 0 <= value[1] <= 255}
    return {"origin": kind}


def transfer(insn, fact, state, original):
    before = state.copy()
    event = audit.transfer(insn, fact, state, original)
    ops = insn.operands
    reg = lambda op: argcheck.canonical(insn.reg_name(op.reg))
    narrow_load = {arm.ARM_INS_LDRB: 8, arm.ARM_INS_LDRSB: 8,
                   arm.ARM_INS_LDRH: 16, arm.ARM_INS_LDRSH: 16}
    if ops and ops[0].type == arm.ARM_OP_REG:
        name = reg(ops[0])
        if insn.id in narrow_load:
            old = state.get(name)
            state[name] = ("narrow", narrow_load[insn.id], old)
        elif insn.id == arm.ARM_INS_LDR and state.get(name, (None,))[0] == "load":
            state[name] = ("word-load", state[name][1])
        elif insn.id in (arm.ARM_INS_UXTB, arm.ARM_INS_UXTH, arm.ARM_INS_SXTB, arm.ARM_INS_SXTH) and len(ops) > 1:
            bits = 8 if insn.id in (arm.ARM_INS_UXTB, arm.ARM_INS_SXTB) else 16
            state[name] = ("narrow", bits, before.get(reg(ops[1])))
    if insn.id in (arm.ARM_INS_STRB, arm.ARM_INS_STRH):
        bits = 8 if insn.id == arm.ARM_INS_STRB else 16
        for name, value in list(state.items()):
            if isinstance(name, tuple) and name[0] == "memory" and value != before.get(name):
                state[name] = ("narrow", bits, value)
    return event


def inspect(original, function):
    decoder = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB if function["isa"] == "thumb" else capstone.CS_MODE_ARM)
    decoder.detail = True
    instructions = {insn.address: insn for start, stop in function["code_ranges"]
                    for insn in decoder.disasm(original.read(start, stop - start), start)}
    facts = argcheck.decode_function(original, function, decoder)
    entries = {entry["address"] for entry in function["entries"]}
    blocks, successors = argcheck.blocks(facts, sorted(entries))
    owner = function["address"]
    initial = {f"r{i}": ("parameter", owner, f"r{i}") for i in range(4)}
    initial["sp"] = ("stack", owner, 0)
    incoming = {index: initial.copy() for index, block in enumerate(blocks) if block[0].address in entries}
    pending = deque(incoming)
    while pending:
        index = pending.popleft()
        state = incoming[index].copy()
        for fact in blocks[index]:
            transfer(instructions[fact.address], fact, state, original)
        for successor in successors[index]:
            previous = incoming.get(successor)
            merged = state.copy() if previous is None else {key: item for key, item in previous.items() if state.get(key) == item}
            if previous != merged:
                incoming[successor] = merged
                pending.append(successor)
    stores, calls, indirect = [], [], []
    for index, state in incoming.items():
        state = state.copy()
        for fact in blocks[index]:
            insn = instructions[fact.address]
            before = state.copy()
            if insn.id in (arm.ARM_INS_STR, arm.ARM_INS_STRB, arm.ARM_INS_STRH):
                source = argcheck.canonical(insn.reg_name(insn.operands[0].reg))
                if before.get(source) in (("parameter", owner, "r1"), ("parameter", owner, "r2")):
                    memory = next((op for op in insn.operands if op.type == arm.ARM_OP_MEM), None)
                    base = argcheck.canonical(insn.reg_name(memory.mem.base)) if memory else None
                    stores.append({"offset": fact.address - owner,
                                   "input_register": before[source][2],
                                   "stored_bits": 8 if insn.id == arm.ARM_INS_STRB else 16 if insn.id == arm.ARM_INS_STRH else 32,
                                   "object_offset": memory.mem.disp if memory and before.get(base) == ("parameter", owner, "r0") else None})
            event = transfer(insn, fact, state, original)
            if event and event["target"] in TARGETS:
                calls.append({"offset": fact.address - owner, "target": f"func_{event['target']:08x}",
                              "priority_argument": category(event["registers"].get("r1"))})
            if event and event["target"] is None:
                indirect.append({"offset": fact.address - owner,
                                 "arg0_is_sound_entry": event["registers"].get("r0") == ("parameter", owner, "r0"),
                                 "extra_argument_arity_proven": False})
    return {"function": f"func_{owner:08x}", "entry_argument_stores": stores,
            "priority_calls": calls, "indirect_calls": indirect}


def main() -> int:
    original = image.Image.load()
    functions = image.functions_by_address(image.load_catalogue(ROOT / "config/catalogue.json"))
    facts = [inspect(original, functions[address]) for address in sorted(TARGETS | {0x001D0614})]
    report = {"schema_version": 1, "image_sha256": original.sha256, "functions": facts,
              "native_contract": {"header": "include/nw/snd/snd_BasicSound.h",
                                  "SetPriority": "void(int,int)", "SetPlayerPriority": "void(int)",
                                  "priority_storage": "u8", "OnUpdatePlayerPriority": "virtual void()"},
              "author_caller_observation": "scratch/authors/r41164/RESULT.json: original allocation priority reaches three func_0016a87c calls as a full signed word; unsigned-char declaration inserted three caller-side narrowings.",
              "decision": "Use int priority formals for both setters and retain the unsigned-byte storage conversion. Do not widen unrelated scheduler ushort fields or claim the handle getter is a numeric priority getter.",
              "qualification": "[INFERENCE] Native API corroboration does not recover historical address-to-mangled-name identities. Spill-aware original CFG facts are conservative; unsupported operations produce unknown values. Any prior-call r0 is unspecified unless a separate return contract proves it. Indirect live registers do not prove extra arguments or a complete slot-consumer sweep. No original execution or full audit is claimed."}
    audit.save(OUT / "priority-facts-r1.json", report)
    print(json.dumps({"functions": len(facts), "setter_entry_store_sites": sum(len(row["entry_argument_stores"]) for row in facts),
                      "observed_priority_call_sites": sum(len(row["priority_calls"]) for row in facts)}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
