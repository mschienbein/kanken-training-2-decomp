"""Census of NOP words and tiny-callee calls in orig/code.bin, for docs/LINKER.md.

    .venv/bin/python research/linker/nop_census.py

Prints counts and addresses only (no original bytes). Every number in docs/LINKER.md's evidence
tables comes from this script against the adopted catalogue (config/catalogue.json).
"""
from __future__ import annotations

import bisect
import collections
import json
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
import image as image_mod  # noqa: E402
import inplace  # noqa: E402
from capstone import CS_ARCH_ARM, CS_MODE_ARM, Cs  # noqa: E402
from capstone import arm as cs_arm  # noqa: E402

NOP_HINT, MOV_R0_R0 = 0xE320F000, 0xE1A00000
CALLER_SAVED = {cs_arm.ARM_REG_R1, cs_arm.ARM_REG_R2, cs_arm.ARM_REG_R3, cs_arm.ARM_REG_R12}

image = image_mod.Image.load()
functions = image_mod.functions_by_address(image_mod.load_catalogue())
starts = sorted(functions)
entries = {e["address"] for f in functions.values() for e in f.get("entries", ())}
md = Cs(CS_ARCH_ARM, CS_MODE_ARM)
md.detail = True


def word(a: int) -> int:
    return struct.unpack("<I", image.read(a, 4))[0]


def insn(a: int):
    return next(md.disasm(image.read(a, 4), a, 1), None)


def data_spans(f: dict) -> list[tuple[int, int]]:
    return [tuple(p) for p in (*f.get("pools", []), *f.get("literals", []))]


def owner(a: int) -> dict | None:
    i = bisect.bisect_right(starts, a) - 1
    f = functions[starts[i]] if i >= 0 else None
    return f if f and a < f["address"] + f["size"] else None


def arm_code_words():
    for f in functions.values():
        if f["isa"] != "arm":
            continue
        spans = data_spans(f)
        for lo, hi in f["code_ranges"]:
            for a in range(lo, hi, 4):
                if not any(x <= a < y for x, y in spans):
                    yield f, a


def as_if_call(a: int, limit: int = 12) -> str:
    """Treat the word at `a` as a removed BL: does the following straight-line code read r1-r3, r12
    or the flags before writing them (impossible after a call)?"""
    written, flags = set(), False
    for p in range(a + 4, a + 4 + 4 * limit, 4):
        f = owner(p)
        if f is None or any(x <= p < y for x, y in data_spans(f)):
            return "consistent"
        i = insn(p)
        if i is None:
            return "consistent"
        read, wrote = i.regs_access()
        if (set(read) & CALLER_SAVED) - written:
            return "reads_args"
        if (i.cc not in (cs_arm.ARM_CC_AL, cs_arm.ARM_CC_INVALID) or cs_arm.ARM_REG_CPSR in read) and not flags:
            return "reads_flags"
        written |= set(wrote) & CALLER_SAVED
        flags |= cs_arm.ARM_REG_CPSR in wrote
        if cs_arm.ARM_REG_PC in wrote or i.mnemonic in ("b", "bl", "bx", "blx"):
            return "consistent"
    return "consistent"


def framed(f: dict, a: int) -> bool:
    """Is LR saved at `a` (linear scan from the start of the owner)?"""
    saved = False
    for p in range(f["address"], a, 4):
        if any(x <= p < y for x, y in data_spans(f)):
            continue
        i = insn(p)
        if i is None or "lr" not in i.op_str:
            continue
        if i.mnemonic in ("push", "stmdb", "stmfd"):
            saved = True
        elif i.mnemonic in ("pop", "ldm", "ldmia", "ldmfd") and "pc" not in i.op_str:
            saved = False
    return saved


def mov_class(f: dict, a: int) -> str:
    prev, nxt = (insn(a - 4) if a > f["address"] else None), insn(a + 4)
    if a in entries and a + 4 in entries:
        return "tail:nop_entry"
    if a + 4 in entries:
        return "tail:falls_into_entry"
    if a in entries:
        return "tail:leading_nop_of_start"
    if prev is not None and (prev.mnemonic == "vpop" or (prev.mnemonic in ("pop", "ldm") and "lr" in prev.op_str and "pc" not in prev.op_str)):
        return "tail:after_epilogue"
    if nxt is not None and nxt.mnemonic in ("push", "stmdb") and "lr" in nxt.op_str:
        return "tail:before_prologue"
    if not framed(f, a):
        return "tail:in_leaf"
    return "call:in_frame"


census: dict = {"nop_hint": collections.Counter(), "mov_r0_r0": collections.Counter(), "as_if_call": {}}
bl_to = collections.defaultdict(list)
b_to = collections.defaultdict(list)
b_to_next = []
for f, a in arm_code_words():
    w = word(a)
    if w == NOP_HINT:
        census["nop_hint"]["in_catalogued_arm_code"] += 1
        census["as_if_call"].setdefault("nop_hint", collections.Counter())[as_if_call(a)] += 1
    elif w & 0x0FFFFFFF == MOV_R0_R0 & 0x0FFFFFFF:
        census["mov_r0_r0"]["cond_AL" if w >> 28 == 0xE else "cond_other"] += 1
        c = mov_class(f, a)
        census["mov_r0_r0"][c] += 1
        census["as_if_call"].setdefault(c, collections.Counter())[as_if_call(a)] += 1
    elif w >> 28 != 0xF and (w >> 25) & 7 == 5:
        t = inplace._arm_branch_target(w, a)
        (bl_to if w & (1 << 24) else b_to)[t].append(a)
        if t == a + 4:
            b_to_next.append(a)
text = image.layout.text
for a in range(text.address, text.address + text.size, 4):
    if word(a) not in (NOP_HINT, MOV_R0_R0):
        continue
    f = owner(a)
    if f is None:
        where = "outside_catalogue"
    elif f["isa"] != "arm" or any(x <= a < y for x, y in data_spans(f)):
        where = "in_thumb_or_data"
    elif not any(lo <= a < hi for lo, hi in f["code_ranges"]):
        where = f"in_owner_gap:{f['address']:#x}"
    else:
        where = None  # counted by the code scan above
    if where:
        census["nop_hint" if word(a) == NOP_HINT else "mov_r0_r0"][where] += 1

linker = inplace.InPlaceLinker(image, functions, inplace.Resolver(functions), image.bss)
small = {}
for a, f in functions.items():
    code = f.get("code_size", f["size"])
    if f["isa"] == "arm" and code in (4, 8) and word(a + code - 4) in (0xE12FFF1E, 0xE1A0F00E):
        body = linker.eligible_original_callee(a)
        rule = inplace.inline_arm_call(0xEB000000, body) if body else None
        first = insn(a).mnemonic + (" [pc]" if "pc" in insn(a).op_str else "") if code == 8 else "return-only"
        small[a] = (rule[0] if rule else "kept", first)
callees = collections.Counter()
for a, (rule, first) in small.items():
    key = f"{rule}:{'empty' if first == 'return-only' else first}"
    callees[key + ":functions"] += 1
    callees[key + ":BL_sites"] += len(bl_to.get(a, ()))
    callees[key + ":B_sites"] += len(b_to.get(a, ()))
callees["kept_callees_with_BL"] = sorted(f"{a:#x}" for a, (rule, _) in small.items() if rule == "kept" and bl_to.get(a))
print(json.dumps({
    "nop_hint": census["nop_hint"], "mov_r0_r0": census["mov_r0_r0"],
    "as_if_call": census["as_if_call"], "tiny_callees": callees,
    "bl_sites": sum(len(v) for v in bl_to.values()),
    "b_to_next_instruction": [f"{a:#x}" for a in b_to_next],
}, indent=1, default=dict))
