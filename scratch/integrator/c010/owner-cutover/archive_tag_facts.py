"""Abstract original caller/callee facts for func_001650f8, without code output.

This single-target analysis never serializes instruction text, encodings, raw
buffers, packets or literal contents. It does not claim the historical formal
parameter spelling; conservative full-word source preserves observed semantics.
"""
from __future__ import annotations

from collections import deque
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[4]
OUT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "tools"))
import argcheck
import capstone
from capstone import arm
import image

TARGET = 0x001650F8


def decoder_for(function: dict):
    decoder = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB if function["isa"] == "thumb" else capstone.CS_MODE_ARM)
    decoder.detail = True
    return decoder


def decoded(original, function):
    decoder = decoder_for(function)
    return {instruction.address: instruction for start, stop in function["code_ranges"]
            for instruction in decoder.disasm(original.read(start, stop - start), start)}


def value(operand, state):
    if operand.type == arm.ARM_OP_IMM:
        return ("constant", operand.imm & 0xFFFFFFFF)
    if operand.type == arm.ARM_OP_REG and not operand.shift.type:
        return state.get(operand.reg)
    return None


def transfer(instruction, state):
    before = state.copy()
    _, written = instruction.regs_access()
    result = None
    operands = instruction.operands
    if instruction.id in (arm.ARM_INS_BL, arm.ARM_INS_BLX):
        for register in (arm.ARM_REG_R0, arm.ARM_REG_R1, arm.ARM_REG_R2, arm.ARM_REG_R3, arm.ARM_REG_R12):
            state.pop(register, None)
        return
    if instruction.id in (arm.ARM_INS_MOV, arm.ARM_INS_MOVW) and len(operands) == 2:
        result = value(operands[1], before)
    elif instruction.id == arm.ARM_INS_MOVT and len(operands) == 2:
        low = before.get(operands[0].reg)
        high = value(operands[1], before)
        if low and high and low[0] == high[0] == "constant":
            result = ("constant", (low[1] & 0xFFFF) | (high[1] << 16))
    elif instruction.id == arm.ARM_INS_LDRH:
        result = ("unsigned-halfword",)
    elif instruction.id == arm.ARM_INS_LDRB:
        result = ("unsigned-byte",)
    elif instruction.id == arm.ARM_INS_UXTH:
        result = ("unsigned-halfword",)
    elif instruction.id == arm.ARM_INS_UXTB:
        result = ("unsigned-byte",)
    for register in written:
        state.pop(register, None)
    if result is not None and operands and operands[0].type == arm.ARM_OP_REG:
        destination = operands[0].reg
        unconditional = instruction.cc in (arm.ARM_CC_AL, arm.ARM_CC_INVALID)
        if unconditional or before.get(destination) == result:
            state[destination] = result


def states(original, function, instructions):
    decoder = decoder_for(function)
    facts = argcheck.decode_function(original, function, decoder)
    blocks, successors = argcheck.blocks(facts, [entry["address"] for entry in function["entries"]])
    incoming = {}
    pending = deque()
    entries = {entry["address"] for entry in function["entries"]}
    for index, block in enumerate(blocks):
        if block[0].address in entries:
            incoming[index] = {register: ("entry", register) for register in (arm.ARM_REG_R0, arm.ARM_REG_R1, arm.ARM_REG_R2, arm.ARM_REG_R3)}
            pending.append(index)
    while pending:
        index = pending.popleft()
        state = incoming[index].copy()
        for fact in blocks[index]:
            transfer(instructions[fact.address], state)
        for successor in successors[index]:
            previous = incoming.get(successor)
            merged = state.copy() if previous is None else {register: item for register, item in previous.items() if state.get(register) == item}
            if previous != merged:
                incoming[successor] = merged
                pending.append(successor)
    before = {}
    for index, initial in incoming.items():
        state = initial.copy()
        for fact in blocks[index]:
            before[fact.address] = state.copy()
            transfer(instructions[fact.address], state)
    return before


def category(item):
    if item is None:
        return {"origin": "unknown", "proven_unsigned_16_bit": False}
    if item[0] == "constant":
        return {"origin": "constant", "proven_unsigned_16_bit": item[1] <= 0xFFFF,
                "high_half_proven_nonzero": item[1] > 0xFFFF}
    return {"origin": item[0], "proven_unsigned_16_bit": item[0] in ("unsigned-halfword", "unsigned-byte")}


def main():
    original = image.Image.load()
    catalogue = image.load_catalogue(ROOT / "config/catalogue.json")
    functions = image.functions_by_address(catalogue)
    function = functions[TARGET]
    instructions = decoded(original, function)
    before = states(original, function, instructions)
    comparisons = []
    tag_writes = []
    for address, instruction in instructions.items():
        if address not in before:
            continue
        if arm.ARM_REG_R1 in instruction.regs_access()[1]:
            tag_writes.append(address - TARGET)
        if instruction.id == arm.ARM_INS_CMP and len(instruction.operands) == 2:
            operands = [value(operand, before[address]) for operand in instruction.operands]
            if ("entry", arm.ARM_REG_R1) in operands:
                other = operands[1] if operands[0] == ("entry", arm.ARM_REG_R1) else operands[0]
                comparisons.append({"offset": address - TARGET, "compares_unchanged_entry_core_word": True,
                                    "other_value": category(other)})
    callers = []
    for owner, caller in sorted(functions.items()):
        if not caller.get("code_ranges"):
            continue
        caller_instructions = decoded(original, caller)
        sites = [instruction for instruction in caller_instructions.values()
                 if instruction.id in (arm.ARM_INS_BL, arm.ARM_INS_BLX, arm.ARM_INS_B)
                 and instruction.operands and instruction.operands[0].type == arm.ARM_OP_IMM
                 and instruction.operands[0].imm & ~1 == TARGET]
        if not sites:
            continue
        caller_before = states(original, caller, caller_instructions)
        for site in sites:
            callers.append({"function": f"func_{owner:08x}", "offset": site.address - owner,
                            "kind": "direct-tail" if site.id == arm.ARM_INS_B else "direct-call",
                            "tag_argument": category(caller_before.get(site.address, {}).get(arm.ARM_REG_R1))})
    report = {"schema_version": 1, "function": "func_001650f8", "image_sha256": original.sha256,
              "callee": {"entry_tag_register_writes": tag_writes, "full_word_comparisons": comparisons},
              "callers": callers, "caller_count": len(callers),
              "decision": "Retain unsigned full-word tag in the canonical C endpoint; the stored unsigned-short field does not narrow the parameter.",
              "qualification": "[INFERENCE] Historical formal spelling is not recovered. An unchanged entry core word is compared without an observed truncation; a narrow C formal would introduce an unsupported caller-side input restriction. All discovered direct/tail callers are listed, not indirect aliases or uncatalogued code. The analysis does not execute the original program."}
    with (OUT / "archive-tag-facts-r1.json").open("x") as stream:
        json.dump(report, stream, indent=2)
        stream.write("\n")
    print(json.dumps({"function": report["function"], "full_word_compare_sites": len(comparisons),
                      "entry_tag_register_write_sites": len(tag_writes), "caller_sites": len(callers),
                      "callers_proven_u16_at_site": sum(row["tag_argument"]["proven_unsigned_16_bit"] for row in callers)}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
