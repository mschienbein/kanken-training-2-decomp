"""Emit scoped original address/register/storage facts, never instruction text or bytes."""
from __future__ import annotations

import argparse
from collections import deque
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "tools"))

import argcheck as ac
import capstone


REUSED = ROOT.parents[2] / "scratch/integrator/c008-r3/audit_indirect.py"
os.environ["DECOMP_ROOT"] = str(ROOT)
SPEC = importlib.util.spec_from_file_location("c008_r3_indirect", REUSED)
indirect = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(indirect)


def transfer(insn, fact, state: dict, image):
    """Retain exact constant-root array provenance through scaled ADDs."""
    before = state.copy()
    event = indirect.transfer(insn, fact, state, image)
    if insn.id != capstone.arm.ARM_INS_ADD or len(insn.operands) != 3:
        return event
    def operand(op):
        if op.type == capstone.arm.ARM_OP_IMM:
            return ("constant", op.imm & 0xffffffff)
        if op.type != capstone.arm.ARM_OP_REG:
            return None
        name = ac.canonical(insn.reg_name(op.reg))
        value = indirect.plain(before.get(name), insn.cc if fact.conditional else None)
        if op.shift.type:
            if op.shift.type != capstone.arm.ARM_SFT_LSL or value is None:
                return None
            if value[0] == "constant":
                return ("constant", (value[1] << op.shift.value) & 0xffffffff)
            base, factor = (value[1], value[2]) if value[0] == "scaled" else (value, 1)
            return ("scaled", base, factor << op.shift.value)
        return value
    left, right = (operand(op) for op in insn.operands[1:])
    value = None
    if left is not None and right is not None:
        if right[0] == "constant":
            value = indirect.add(left, right[1])
        elif left[0] == "constant":
            base, factor = (right[1], right[2]) if right[0] == "scaled" else (right, 1)
            value = ("indexed", left, factor)
        else:
            first, a = (left[1], left[2]) if left[0] == "scaled" else (left, 1)
            second, b = (right[1], right[2]) if right[0] == "scaled" else (right, 1)
            if first == second:
                value = ("scaled", first, a + b)
    if value is not None:
        destination = ac.canonical(insn.reg_name(insn.operands[0].reg))
        state[destination] = (("when", insn.cc, before.get(destination), value)
                              if fact.conditional and before.get(destination) != value else value)
    return event


def memory_location(instruction, fact, state: dict):
    """Describe indexed storage using the same roots as the enhanced transfer."""
    operand = next((op for op in instruction.operands
                    if op.type == capstone.arm.ARM_OP_MEM), None)
    if operand is None or operand.shift.type not in (0, capstone.arm.ARM_SFT_LSL):
        return None
    condition = instruction.cc if fact.conditional else None
    get = lambda register: indirect.plain(state.get(
        ac.canonical(instruction.reg_name(register))), condition)
    base = (("constant", fact.pc) if operand.mem.base == capstone.arm.ARM_REG_PC
            else get(operand.mem.base))
    displacement = operand.mem.disp
    if operand.mem.index:
        index = get(operand.mem.index)
        scale = operand.mem.scale * (1 << operand.shift.value)
        if index is not None and index[0] == "constant":
            displacement += index[1] * scale
        elif base is not None:
            base = ("indexed", base, scale)
    return indirect.add(base, displacement)


def enhanced(function: dict, blocks: list, successors: list, decoder, image,
             acyclic: bool, known_entries: set[int]) -> dict:
    """Reuse r3's CFG transfer, including stack and multiple-register storage."""
    instructions = {insn.address: insn for start, stop in function["code_ranges"]
                    for insn in decoder.disasm(image.read(start, stop - start), start)}
    entries = {entry["address"] for entry in function["entries"]}
    incoming = {}
    queue = deque()
    for index, block in enumerate(blocks):
        if block[0].address in entries:
            incoming[index] = {reg: ("parameter", function["address"], reg)
                               for reg in ac.ARGUMENTS}
            incoming[index]["sp"] = ("stack", function["address"], 0)
            queue.append(index)
    while queue:
        index = queue.popleft()
        state = incoming[index].copy()
        for fact in blocks[index]:
            transfer(instructions[fact.address], fact, state, image)
        for successor in successors[index]:
            if acyclic and successor <= index:
                continue
            previous = incoming.get(successor)
            merged = state.copy() if previous is None else {
                key: value for key, value in previous.items() if state.get(key) == value}
            if previous != merged:
                incoming[successor] = merged
                queue.append(successor)
    calls, memory, accesses, returns, signed_shifts = [], {}, [], [], []
    access_types = {
        capstone.arm.ARM_INS_LDR: ("read", 32, False),
        capstone.arm.ARM_INS_LDRB: ("read", 8, False),
        capstone.arm.ARM_INS_LDRSB: ("read", 8, True),
        capstone.arm.ARM_INS_LDRH: ("read", 16, False),
        capstone.arm.ARM_INS_LDRSH: ("read", 16, True),
        capstone.arm.ARM_INS_STR: ("write", 32, False),
        capstone.arm.ARM_INS_STRB: ("write", 8, False),
        capstone.arm.ARM_INS_STRH: ("write", 16, False),
    }
    for index, initial in sorted(incoming.items()):
        state = initial.copy()
        for fact in blocks[index]:
            instruction = instructions[fact.address]
            access = access_types.get(instruction.id)
            if instruction.id in (capstone.arm.ARM_INS_VLDR, capstone.arm.ARM_INS_VSTR):
                register = instruction.reg_name(instruction.operands[0].reg)
                access = ("read" if instruction.id == capstone.arm.ARM_INS_VLDR else "write",
                          64 if register.startswith("d") else 32, False)
            if access is not None:
                accesses.append({"site": ac.address(fact.address), "operation": access[0],
                                 "bits": access[1], "signed_load": access[2],
                                 "location": indirect.describe(memory_location(
                                     instruction, fact, state))})
            elif instruction.id in (capstone.arm.ARM_INS_LDRD, capstone.arm.ARM_INS_STRD):
                location = memory_location(instruction, fact, state)
                operation = "read" if instruction.id == capstone.arm.ARM_INS_LDRD else "write"
                for offset in (0, 4):
                    accesses.append({"site": ac.address(fact.address), "operation": operation,
                                     "bits": 32, "signed_load": False,
                                     "location": indirect.describe(indirect.add(location, offset))})
            elif instruction.id in (capstone.arm.ARM_INS_VSTMIA, capstone.arm.ARM_INS_VSTMDB,
                                     capstone.arm.ARM_INS_VLDMIA, capstone.arm.ARM_INS_VLDMDB):
                operands = instruction.operands
                base = indirect.plain(state.get(ac.canonical(
                    instruction.reg_name(operands[0].reg))),
                    instruction.cc if fact.conditional else None)
                registers = [instruction.reg_name(op.reg) for op in operands[1:]
                             if op.type == capstone.arm.ARM_OP_REG]
                width = sum(8 if register.startswith("d") else 4 for register in registers)
                decrement = instruction.id in (capstone.arm.ARM_INS_VSTMDB,
                                                capstone.arm.ARM_INS_VLDMDB)
                offset = -width if decrement else 0
                operation = "write" if instruction.id in (
                    capstone.arm.ARM_INS_VSTMIA, capstone.arm.ARM_INS_VSTMDB) else "read"
                for register in registers:
                    bits = 64 if register.startswith("d") else 32
                    accesses.append({"site": ac.address(fact.address), "operation": operation,
                                     "bits": bits, "signed_load": False,
                                     "location": indirect.describe(indirect.add(base, offset))})
                    offset += bits // 8
            if instruction.id == capstone.arm.ARM_INS_ASR or any(
                    op.shift.type == capstone.arm.ARM_SFT_ASR for op in instruction.operands):
                signed_shifts.append(ac.address(fact.address))
            if (fact.flow == "branch" and fact.target in known_entries
                    and fact.target not in instructions):
                condition = instruction.cc if fact.conditional else None
                get = lambda register: indirect.plain(state.get(register), condition)
                event = {
                    "site": fact.address, "target": fact.target, "tail": True,
                    "pointer": None,
                    "registers": {register: get(register) for register in
                                  sorted(ac.ARGUMENTS, key=ac.reg_order)},
                    "stack": {offset: state.get(("memory", indirect.add(get("sp"), offset)))
                              for offset in range(0, 32, 4)},
                }
            else:
                event = transfer(instruction, fact, state, image)
            if fact.flow == "return":
                returns.append({"site": ac.address(fact.address),
                                "registers": {reg: indirect.describe(state.get(reg))
                                              for reg in ("r0", "r1", "s0")}})
            if event:
                calls.append({"site": ac.address(event["site"]),
                              "tail": event.get("tail", False),
                              "target": ac.address(event["target"]) if event["target"] else None,
                              "pointer": indirect.describe(event["pointer"]),
                              "registers": {reg: indirect.describe(value)
                                            for reg, value in event["registers"].items()
                                            if value is not None},
                              "stack": {offset: indirect.describe(value)
                                        for offset, value in event["stack"].items()
                                        if value is not None}})
        for key, value in state.items():
            if isinstance(key, tuple) and key[0] == "memory":
                memory[json.dumps(indirect.describe(key[1]), sort_keys=True)] = indirect.describe(value)
    return {"calls": calls, "storage": [{"location": json.loads(key), "value": value}
                                        for key, value in sorted(memory.items())],
            "memory_accesses": accesses, "returns": returns,
            "signed_right_shift_sites": signed_shifts,
            "backedges": [{"from": ac.address(blocks[index][-1].address),
                            "to": ac.address(blocks[target][0].address)}
                           for index, targets in enumerate(successors)
                           for target in targets if target <= index]}

def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("addresses", nargs="+", type=lambda value: int(value, 16))
    parser.add_argument("--out", required=True, type=Path)
    parser.add_argument("--acyclic", action="store_true",
                        help="Exclude backedges to expose first-iteration roots; not an all-path proof")
    args = parser.parse_args()
    image = ac.image_mod.Image.load(ROOT / "orig")
    catalogue = ac.image_mod.load_catalogue(ROOT / "config/catalogue.json")
    functions = ac.image_mod.functions_by_address(catalogue)
    entries = {entry["address"] for function in functions.values() for entry in function["entries"]}
    decoders = {"arm": capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM),
                "thumb": capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)}
    for decoder in decoders.values():
        decoder.detail = True
    rows = []
    for address in args.addresses:
        function = functions[address]
        facts = ac.decode_function(image, function, decoders[function["isa"]])
        blocks, successors = ac.blocks(facts, [entry["address"] for entry in function["entries"]])
        events = ac.symbolic_events(function, blocks, successors, image)
        stores = [{"site": ac.address(event[3]), "location": ac.value_fact(event[1]),
                   "value": ac.value_fact(event[2])} for event in events if event[0] == "store"]
        calls = [{"site": ac.address(event[3]),
                  "target": ac.address(event[1]) if event[1] is not None else None,
                  "target_pointer": ac.value_fact(event[4]) if event[4] else None,
                  "core_inputs": {key: ac.value_fact(value) for key, value in event[2].items()
                                  if key in ("r0", "r1", "r2", "r3")}}
                 for event in events if event[0] == "call"]
        rows.append({"function": ac.address(address), "stores": stores, "calls": calls,
                     "sites": ac.call_sites(function, blocks, entries, {}),
                     "enhanced": enhanced(function, blocks, successors,
                                          decoders[function["isa"]], image, args.acyclic, entries)})
    args.out.write_text(json.dumps({"facts": rows, "coverage": __doc__,
                                   "acyclic_only": args.acyclic,
                                   "reused_helper": str(REUSED),
                                   "reused_helper_sha256": hashlib.sha256(REUSED.read_bytes()).hexdigest()},
                                  indent=2) + "\n")
    print(json.dumps({"functions": len(rows), "report": str(args.out)}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
