"""Bounded original display contract facts; never emits instructions or payload."""
from __future__ import annotations

import hashlib
import importlib.util
import json
import os
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[4]
MAIN = ROOT.parents[2]
OUT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "tools"))
os.environ["DECOMP_ROOT"] = str(ROOT)
HELPER = MAIN / "scratch/integrator/c008-r3/audit_indirect.py"
spec = importlib.util.spec_from_file_location("display_original_transfer", HELPER)
audit = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = audit
spec.loader.exec_module(audit)
from capstone import arm
import argcheck
import capstone
import image


NARROW_LOADS = {arm.ARM_INS_LDRB: 8, arm.ARM_INS_LDRSB: 8,
                arm.ARM_INS_LDRH: 16, arm.ARM_INS_LDRSH: 16}
STORES = {arm.ARM_INS_STR: 32, arm.ARM_INS_STRB: 8, arm.ARM_INS_STRH: 16}


def describe(value: tuple | None) -> dict:
    if value is None:
        return {"kind": "unknown"}
    kind = value[0]
    if kind == "constant":
        number = value[1]
        return {"kind": "constant", "fits_unsigned_byte": 0 <= number <= 255,
                "fits_signed_halfword": number <= 32767 or number >= 0xffff8000,
                "catalogued_function": f"func_{number:08x}" if number in FUNCTIONS else None,
                "image_pointer": 0x100000 <= number < 0x100000 + len(ORIGINAL.code)}
    if kind == "parameter":
        return {"kind": "entry", "register": value[2]}
    if kind in ("load", "word-load"):
        return {"kind": kind, "location": describe(value[1])}
    if kind == "offset":
        return {"kind": kind, "base": describe(value[1]), "offset": value[2]}
    if kind == "stack":
        return {"kind": "frame", "offset": value[2]}
    if kind == "indexed":
        return {"kind": kind, "base": describe(value[1]), "stride": value[2]}
    if kind == "narrow":
        return {"kind": kind, "bits": value[1], "source": describe(value[2])}
    if kind == "result":
        return {"kind": "prior_call_register", "meaningful_result_proven": False,
                "callee": f"func_{value[1]:08x}" if value[1] is not None else None,
                "site": f"0x{value[2]:08x}", "register": value[3]}
    if kind == "when":
        return {"kind": "conditional", "otherwise": describe(value[2]), "taken": describe(value[3])}
    return {"kind": kind}


def transfer(insn, fact, state: dict) -> dict | None:
    before = state.copy()
    event = audit.transfer(insn, fact, state, ORIGINAL)
    if event:
        return event
    ops = insn.operands
    canonical = lambda rid: argcheck.canonical(insn.reg_name(rid))
    get = lambda rid: before.get(canonical(rid))
    mem = next((op for op in ops if op.type == arm.ARM_OP_MEM), None)
    # The reused helper deliberately does not model single-memory writeback.
    # Account for that actual frame adjustment, not an inferred argument.
    if mem is not None and insn.writeback:
        base = get(mem.mem.base)
        displacement = mem.mem.disp
        if mem.mem.index:
            index = get(mem.mem.index)
            if index and index[0] == "constant":
                displacement += index[1] * mem.mem.scale * (1 << mem.shift.value)
            else:
                base = None
        updated = audit.add(base, displacement)
        if updated is not None:
            state[canonical(mem.mem.base)] = updated
        # A pre-indexed store is addressed using the old base plus displacement;
        # retain it even though the helper invalidated the writeback base first.
        if insn.id in STORES and base is not None and ops[0].type == arm.ARM_OP_REG:
            location = audit.add(base, displacement)
            value = get(ops[0].reg)
            if location is not None and value is not None:
                state[("memory", location)] = value
    if ops and ops[0].type == arm.ARM_OP_REG:
        name = canonical(ops[0].reg)
        if insn.id in NARROW_LOADS:
            state[name] = ("narrow", NARROW_LOADS[insn.id], state.get(name))
        elif insn.id in (arm.ARM_INS_UXTB, arm.ARM_INS_UXTH, arm.ARM_INS_SXTB, arm.ARM_INS_SXTH) and len(ops) > 1:
            bits = 8 if insn.id in (arm.ARM_INS_UXTB, arm.ARM_INS_SXTB) else 16
            state[name] = ("narrow", bits, get(ops[1].reg))
        elif insn.id == arm.ARM_INS_ADD and len(ops) == 3 and ops[2].type == arm.ARM_OP_REG:
            base, index = get(ops[1].reg), get(ops[2].reg)
            scale = 1 << ops[2].shift.value if ops[2].shift.type == arm.ARM_SFT_LSL else 1
            if base is not None:
                state[name] = audit.add(base, index[1] * scale) if index and index[0] == "constant" else ("indexed", base, scale)
    return None


def inspect(address: int) -> dict:
    function = FUNCTIONS[address]
    decoder = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)
    decoder.detail = True
    instructions = {insn.address: insn for start, stop in function["code_ranges"]
                    for insn in decoder.disasm(ORIGINAL.read(start, stop - start), start)}
    decoded = argcheck.decode_function(ORIGINAL, function, decoder)
    # These facts are path-local and explicitly not an alias/loop fixed-point proof.
    state = {f"r{i}": ("parameter", address, f"r{i}") for i in range(4)}
    state["sp"] = ("stack", address, 0)
    calls, stores = [], []
    for fact in decoded:
        insn = instructions[fact.address]
        before = state.copy()
        event = transfer(insn, fact, state)
        if event:
            calls.append({"site_offset": fact.address - address,
                          "target": f"func_{event['target']:08x}" if event["target"] else None,
                          "arguments": {key: describe(value) for key, value in event["registers"].items() if key in ("r0", "r1", "r2", "r3")},
                          "stack": {str(key): describe(value) for key, value in event["stack"].items() if key < 12}})
        if insn.id in STORES or insn.id == arm.ARM_INS_STRD:
            for key, value in state.items():
                if isinstance(key, tuple) and key[0] == "memory" and value != before.get(key):
                    stores.append({"site_offset": fact.address - address,
                                   "location": describe(key[1]), "bits": STORES.get(insn.id, 32),
                                   "value": describe(value)})
    return {"function": f"func_{address:08x}", "size": function["size"],
            "calls": calls, "stores": stores,
            "byte_memory_read_count": sum(insn.id in (arm.ARM_INS_LDRB, arm.ARM_INS_LDRSB) for insn in instructions.values()),
            "byte_store_count": sum(insn.id == arm.ARM_INS_STRB for insn in instructions.values()),
            "explicit_byte_extension_count": sum(insn.id in (arm.ARM_INS_UXTB, arm.ARM_INS_SXTB) for insn in instructions.values())}


def direct_sites(targets: set[int]) -> list[dict]:
    result = []
    decoder = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)
    decoder.detail = True
    for address, function in FUNCTIONS.items():
        if function["isa"] != "arm":
            continue
        for start, stop in function["code_ranges"]:
            for insn in decoder.disasm(ORIGINAL.read(start, stop - start), start):
                if insn.id in (arm.ARM_INS_BL, arm.ARM_INS_BLX, arm.ARM_INS_B) and insn.operands and insn.operands[0].type == arm.ARM_OP_IMM:
                    target = insn.operands[0].imm
                    if target in targets and not address <= target < address + function["size"]:
                        result.append({"caller": f"func_{address:08x}", "site_offset": insn.address - address,
                                       "target": f"func_{target:08x}"})
    return result


if __name__ == "__main__":
    ORIGINAL = image.Image.load(ROOT / "orig")
    FUNCTIONS = image.functions_by_address(image.load_catalogue(ROOT / "config/catalogue.json"))
    targets = {0x00168cec, 0x0016e300, 0x00175ab8, 0x00170030, 0x00170088,
               0x00170014, 0x0016fff8, 0x00167e08, 0x00174d9c, 0x0017f7f8,
               0x001ae9e4, 0x001af00c, 0x001c2f10}
    owners = targets | {0x001ae908, 0x001ae2e4, 0x001ad148, 0x0017a818, 0x001de638,
                        0x001c2dd0, 0x001c3068, 0x0016f608, 0x00181700, 0x0015e684,
                        0x0015e5c4, 0x0016f534, 0x00183168, 0x00183208, 0x001cf1f4}
    report = {"schema_version": 1, "image_sha256": ORIGINAL.sha256,
              "script_sha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
              "reused_helper": str(HELPER), "reused_helper_sha256": hashlib.sha256(HELPER.read_bytes()).hexdigest(),
              "scope": "Bounded display owners and direct caller census only, not a whole-ledger audit.",
              "qualification": "Abstract path-local facts only; branch merging, loops, virtual arity, complete aliasing and meaningful return contracts are not proved. Constants are range-classified, not reproduced. A prior-call register value is not by itself a proved return. No instructions, image payload, encodings or packets are emitted.",
              "direct_sites": direct_sites(targets), "owners": [inspect(address) for address in sorted(owners)]}
    destination = OUT / "original-facts-r1.json"
    with destination.open("x", encoding="utf-8") as stream:
        json.dump(report, stream, indent=2)
        stream.write("\n")
    print(json.dumps({"owners": len(owners), "direct_sites": len(report["direct_sites"]), "report": str(destination.relative_to(ROOT))}))
