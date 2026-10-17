"""Compare declared interfaces with conservative original caller-register evidence.

    python tools/argcheck.py --matches config/matches.json \
        --matches config/candidates/c005.json --json scratch/review/argcheck.json
    python tools/argcheck.py func_00134f04 --json scratch/review/one.json

Exit 1 means review candidates were found, not that their inferred prototypes are
proven wrong. Exit 2 means an input could not be audited. Reports contain address,
register and declaration facts only; original instructions and bytes never leave
this module. A byte match does not establish an unused argument's contract.
"""
from __future__ import annotations

import argparse
from collections import defaultdict, deque
from dataclasses import dataclass
import json
from pathlib import Path
import re
import struct
import sys
import time
from typing import Any

import capstone
from capstone import arm
from pycparser import c_parser, plyparser

sys.path.insert(0, str(Path(__file__).resolve().parent))
import image as image_mod  # noqa: E402
import interfaces  # noqa: E402
import toolchain  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
CORE = frozenset(f"r{i}" for i in range(4))
VFP = frozenset(f"s{i}" for i in range(16))
ARGUMENTS = CORE | VFP
CLOBBERED = CORE | VFP | {"r12", "lr"}
COVERAGE = {
    "arguments": "Explicit writes not subsequently read in the call-site basic block; calls consume/reset pending writes. Core and VFP allocation are independent (AAPCS-VFP).",
    "results": "Explicit r0/s0 reads before overwrite, branch, return or another call in the continuation block; tail-call results are unknown.",
    "callbacks": "Exact symbolic storage-location equality, including literal/data pointers and one-hop registration helpers; unmatched address references remain unresolved, never matched by field offset alone.",
    "rewrites": "Recorded inline_nop/inline_insn sites restore erased calls. Their callee identity is recorded-source evidence, not independently recoverable from the original image.",
    "limits": "[INFERENCE] Register writes can be temporaries; preserved/forwarded arguments, stack arguments, cross-block results, virtual dispatch and general alias analysis are not proven. Excess declared inputs are notes, not failures. C++ is explicitly unchecked, as in interfaces.py.",
}


class ArgcheckError(RuntimeError):
    """An input cannot be audited."""


def address(value: int) -> str:
    return f"0x{value:08x}"


def reg_order(name: str) -> tuple[int, int]:
    return (0 if name.startswith("r") else 1, int(name[1:]))


def canonical(name: str) -> str:
    return {"sb": "r9", "sl": "r10", "fp": "r11", "ip": "r12"}.get(name, name)


def lanes(name: str) -> set[str]:
    if re.fullmatch(r"d[0-7]", name):
        return {f"s{int(name[1:]) * 2}", f"s{int(name[1:]) * 2 + 1}"}
    return {name}


# These private facts intentionally have no instruction text or encoding field.
@dataclass(slots=True)
class Fact:
    address: int
    size: int
    reads: frozenset[str]
    writes: frozenset[str]
    flow: str = "other"
    target: int | None = None
    target_register: str | None = None
    conditional: bool = False
    operation: str = "other"
    operands: tuple = ()
    memory: tuple | None = None
    write_registers: tuple[str, ...] = ()
    pc: int = 0


def decode_function(image: image_mod.Image, function: dict, decoder: capstone.Cs) -> list[Fact]:
    facts = []
    operations = {
        arm.ARM_INS_MOV: "move", arm.ARM_INS_MOVW: "move", arm.ARM_INS_MOVT: "movt",
        arm.ARM_INS_ADD: "add", arm.ARM_INS_SUB: "sub", arm.ARM_INS_ADR: "adr",
        arm.ARM_INS_LDR: "load", arm.ARM_INS_STR: "store",
    }
    for start, stop in function["code_ranges"]:
        decoded = 0
        for insn in decoder.disasm(image.read(start, stop - start), start):
            if not insn.id:
                raise ArgcheckError(f"undecoded original code at {address(insn.address)}")
            read_ids, write_ids = insn.regs_access()
            # Capstone 5 omits VFP register-list accesses for these instructions
            # (the same correction is required by analyze.py's liveness audit).
            fp_list = {op.reg for op in insn.operands if op.type == arm.ARM_OP_REG
                       and re.fullmatch(r"[sd]\d+", insn.reg_name(op.reg))}
            if insn.id in (arm.ARM_INS_VSTMIA, arm.ARM_INS_VSTMDB, arm.ARM_INS_VPUSH):
                read_ids = set(read_ids) | fp_list
            elif insn.id in (arm.ARM_INS_VLDMIA, arm.ARM_INS_VLDMDB, arm.ARM_INS_VPOP):
                write_ids = set(write_ids) | fp_list
            names = lambda ids: frozenset(lane for r in ids for lane in lanes(canonical(insn.reg_name(r))))
            writes = tuple(canonical(insn.reg_name(r)) for r in write_ids)
            operands, memory = [], None
            for op in insn.operands:
                if op.type == arm.ARM_OP_REG:
                    operands.append(("reg", canonical(insn.reg_name(op.reg))))
                elif op.type == arm.ARM_OP_IMM:
                    operands.append(("imm", op.imm))
                else:
                    operands.append(("unknown",))
                if op.type == arm.ARM_OP_MEM and op.shift.type in (0, arm.ARM_SFT_LSL):
                    memory = (canonical(insn.reg_name(op.mem.base)),
                              canonical(insn.reg_name(op.mem.index)) if op.mem.index else None,
                              op.mem.disp, op.mem.scale, op.shift.value)
                # Shifted arithmetic is not ordinary register-copy provenance.
                if op.shift.type and op.type != arm.ARM_OP_MEM:
                    operands[-1] = ("unknown",)
            flow, target, target_register = "other", None, None
            if insn.id in (arm.ARM_INS_BL, arm.ARM_INS_BLX):
                flow = "call"
            elif insn.id in (arm.ARM_INS_B, arm.ARM_INS_CBZ, arm.ARM_INS_CBNZ):
                flow = "branch"
            elif "pc" in names(write_ids) or insn.id == arm.ARM_INS_BX:
                flow = "return"
            if flow in ("call", "branch"):
                destination = insn.operands[-1] if insn.id in (arm.ARM_INS_CBZ, arm.ARM_INS_CBNZ) else insn.operands[0]
                if destination.type == arm.ARM_OP_IMM:
                    target = destination.imm & ~1
                elif destination.type == arm.ARM_OP_REG:
                    target_register = canonical(insn.reg_name(destination.reg))
            conditional = insn.cc not in (arm.ARM_CC_AL, arm.ARM_CC_INVALID) or insn.id in (arm.ARM_INS_CBZ, arm.ARM_INS_CBNZ)
            pc = insn.address + (8 if function["isa"] == "arm" else 4)
            if function["isa"] == "thumb":
                pc &= ~3
            facts.append(Fact(insn.address, insn.size, names(read_ids), names(write_ids), flow,
                              target, target_register, conditional, operations.get(insn.id, "other"),
                              tuple(operands), memory, writes, pc))
            decoded += insn.size
        if decoded != stop - start:
            raise ArgcheckError(f"incomplete original code range at {address(start)}")
    return facts


def blocks(facts: list[Fact], entries: list[int]) -> tuple[list[list[Fact]], list[list[int]]]:
    by_address = {f.address: i for i, f in enumerate(facts)}
    leaders = {0} if facts else set()
    leaders.update(by_address[e] for e in entries if e in by_address)
    for i, fact in enumerate(facts):
        if fact.flow == "branch" and fact.target in by_address:
            leaders.add(by_address[fact.target])
        if i + 1 < len(facts) and (fact.flow in ("branch", "return") or fact.address + fact.size != facts[i + 1].address):
            leaders.add(i + 1)
    starts = sorted(leaders)
    result = [facts[first:last] for first, last in zip(starts, starts[1:] + [len(facts)])]
    owners = {fact.address: i for i, block in enumerate(result) for fact in block}
    successors = []
    for i, block in enumerate(result):
        last = block[-1]
        edges = []
        if last.flow == "branch" and last.target in owners:
            edges.append(owners[last.target])
        falls_through = last.flow not in ("branch", "return") or last.conditional
        if falls_through and i + 1 < len(result) and last.address + last.size == result[i + 1][0].address:
            edges.append(i + 1)
        successors.append(sorted(set(edges)))
    return result, successors


def result_reads(block: list[Fact], index: int, restored_sites: set[int]) -> list[dict]:
    remaining, found = {"r0", "s0"}, []
    for fact in block[index + 1:]:
        if fact.flow == "call" or fact.address in restored_sites:
            break
        for register in sorted(remaining & fact.reads, key=reg_order):
            found.append({"register": register, "read": address(fact.address)})
        remaining -= fact.reads | (fact.writes if not fact.conditional else frozenset())
        if not remaining or fact.flow in ("branch", "return"):
            break
    return found


def call_sites(function: dict, block_list: list[list[Fact]], entries: set[int], rewrites: dict[int, list[dict]]) -> list[dict]:
    sites = []
    for block in block_list:
        pending: dict[str, tuple[int, str]] = {}
        for index, fact in enumerate(block):
            restored = rewrites.get(fact.address, [])
            tail = fact.flow == "branch" and fact.target in entries and not (function["address"] <= fact.target < function["address"] + function["size"])
            if fact.flow == "call" or tail or restored:
                # A BLX operand is a function pointer, not an argument to itself.
                reads = set() if restored else set(fact.reads)
                for register in reads:
                    pending.pop(register, None)
                args = [{"register": r, "write": address(pending[r][0]), "written_as": pending[r][1]}
                        for r in sorted(pending, key=reg_order)]
                targets = restored or [{"target": fact.target, "kind": "tail" if tail else ("direct" if fact.target is not None else "indirect")}]
                for target in targets:
                    site = {"caller": address(function["address"]), "site": address(fact.address),
                            "kind": target["kind"], "target": target["target"], "arguments": args,
                            "results": [] if tail else result_reads(block, index, set(rewrites)),
                            "target_register": fact.target_register}
                    if "records" in target:
                        site["records"] = target["records"]
                        site["qualification"] = "[INFERENCE] erased callee identity comes from recorded linker rewrites"
                    sites.append(site)
                pending.clear()
                continue
            for register in fact.reads:
                pending.pop(register, None)
            for register in fact.writes & ARGUMENTS:
                written_as = next((r for r in fact.write_registers if register in lanes(r)), register)
                # A conditional assignment may not execute at the eventual call.
                if fact.conditional:
                    pending.pop(register, None)
                else:
                    pending[register] = (fact.address, written_as)
    return sites


# Symbolic values describe storage addresses, not arbitrary execution. Unknown is
# absence. Exact root equality is required; equal offsets alone are never enough.
Value = tuple


def plus(value: Value | None, offset: int) -> Value | None:
    if value is None:
        return None
    if not offset:
        return value
    if value[0] == "constant":
        return ("constant", (value[1] + offset) & 0xFFFFFFFF)
    if value[0] == "offset":
        return plus(value[1], value[2] + offset)
    return ("offset", value, offset)


def operand_value(operand: tuple, state: dict[str, Value], fact: Fact) -> Value | None:
    if operand[0] == "imm":
        return ("constant", operand[1] & 0xFFFFFFFF)
    if operand[0] == "reg":
        return ("constant", fact.pc) if operand[1] == "pc" else state.get(operand[1])
    return None


def memory_value(fact: Fact, state: dict[str, Value]) -> Value | None:
    if fact.memory is None:
        return None
    base, index, displacement, scale, shift = fact.memory
    value = ("constant", fact.pc) if base == "pc" else state.get(base)
    if index:
        other = state.get(index)
        if other is None or other[0] != "constant":
            return None
        displacement += other[1] * scale * (1 << shift)
    return plus(value, displacement)


def transfer(fact: Fact, state: dict[str, Value], image: image_mod.Image) -> tuple | None:
    before = state.copy()
    event = None
    if fact.flow == "call":
        event = ("call", fact.target, before, fact.address, before.get(fact.target_register))
        for register in CLOBBERED:
            state.pop(register, None)
        return event
    value = None
    if fact.operation in ("move", "adr") and len(fact.operands) > 1:
        value = operand_value(fact.operands[1], before, fact)
    elif fact.operation == "movt" and len(fact.operands) > 1:
        old = before.get(fact.operands[0][1])
        high = operand_value(fact.operands[1], before, fact)
        if old is not None and high is not None and old[0] == high[0] == "constant":
            value = ("constant", (old[1] & 0xFFFF) | (high[1] << 16))
    elif fact.operation in ("add", "sub") and len(fact.operands) == 3:
        left = operand_value(fact.operands[1], before, fact)
        right = operand_value(fact.operands[2], before, fact)
        if right is not None and right[0] == "constant":
            value = plus(left, right[1] if fact.operation == "add" else -right[1])
    elif fact.operation in ("load", "store"):
        location = memory_value(fact, before)
        if fact.operation == "store":
            source = operand_value(fact.operands[0], before, fact)
            if location is not None and source is not None:
                event = ("store", location, source, fact.address)
        elif location is not None:
            if fact.memory[0] == "pc" and location[0] == "constant":
                try:
                    value = ("constant", struct.unpack("<I", image.read(location[1], 4))[0])
                except (image_mod.ImageError, ValueError, struct.error):
                    value = None
            else:
                value = ("load", location)
    for register in fact.writes:
        state.pop(register, None)
    if value is not None and fact.operands and fact.operands[0][0] == "reg":
        destination = fact.operands[0][1]
        if not fact.conditional or before.get(destination) == value:
            state[destination] = value
    return event


def symbolic_events(function: dict, block_list: list[list[Fact]], successors: list[list[int]], image: image_mod.Image) -> list[tuple]:
    if not block_list:
        return []
    entries = {entry["address"] for entry in function["entries"]}
    incoming: dict[int, dict[str, Value]] = {}
    queue = deque()
    for i, block in enumerate(block_list):
        if block[0].address in entries:
            incoming[i] = {f"r{r}": ("parameter", function["address"], r) for r in range(4)}
            queue.append(i)
    while queue:
        index = queue.popleft()
        state = incoming[index].copy()
        for fact in block_list[index]:
            transfer(fact, state, image)
        for successor in successors[index]:
            previous = incoming.get(successor)
            merged = state.copy() if previous is None else {r: v for r, v in previous.items() if state.get(r) == v}
            if previous != merged:
                incoming[successor] = merged
                queue.append(successor)
    events = []
    for index, initial in sorted(incoming.items()):
        state = initial.copy()
        for fact in block_list[index]:
            event = transfer(fact, state, image)
            if event is not None:
                events.append(event)
    return events


def substitute(value: Value, owner: int, state: dict[str, Value]) -> Value | None:
    if value[0] == "parameter" and value[1] == owner:
        return state.get(f"r{value[2]}")
    if value[0] in ("load", "offset"):
        root = substitute(value[1], owner, state)
        if root is None:
            return None
        return ("load", root) if value[0] == "load" else plus(root, value[2])
    return value


def value_fact(value: Value) -> dict:
    if value[0] == "constant":
        return {"address": address(value[1])}
    if value[0] == "parameter":
        return {"function": address(value[1]), "register": f"r{value[2]}"}
    if value[0] == "load":
        return {"loaded_from": value_fact(value[1])}
    return {"base": value_fact(value[1]), "offset": value[2]}


def pointer_references(image: image_mod.Image, catalogue: dict, targets: set[int]) -> dict[int, list[int]]:
    spans = [(s.address, s.address + s.size) for s in (image.layout.rodata, image.layout.data)]
    spans += [tuple(span) for function in catalogue["functions"] for key in ("pools", "literals") for span in function.get(key, [])]
    references: dict[int, set[int]] = defaultdict(set)
    for start, stop in sorted(set(spans)):
        first = (start + 3) & ~3
        data = image.read(first, (stop - first) // 4 * 4) if stop > first else b""
        for index, (value,) in enumerate(struct.iter_unpack("<I", data)):
            if value & ~1 in targets:
                references[value & ~1].add(first + index * 4)
    return {target: sorted(locations) for target, locations in references.items()}


def callback_sites(events_by_owner: dict[int, list[tuple]], sites: list[dict], references: dict[int, list[int]], targets: set[int]) -> tuple[list[dict], int]:
    stores: dict[Value, dict[int, list[dict]]] = defaultdict(lambda: defaultdict(list))
    summaries = {owner: [event for event in events if event[0] == "store"] for owner, events in events_by_owner.items()}
    for target, locations in references.items():
        for location in locations:
            stores[("constant", location)][target].append({"stored_at": address(location), "kind": "data_pointer"})
    for owner, events in events_by_owner.items():
        for event in events:
            possible = []
            if event[0] == "store":
                possible.append((event[1], event[2], {"registration": address(event[3]), "kind": "store"}))
            elif event[0] == "call" and event[1] in summaries:
                for store in summaries[event[1]]:
                    location = substitute(store[1], event[1], event[2])
                    value = substitute(store[2], event[1], event[2])
                    if location is not None and value is not None:
                        possible.append((location, value, {"registration": address(event[3]), "store": address(store[3]), "kind": "registration_helper"}))
            for location, value, provenance in possible:
                if value[0] == "constant" and value[1] & ~1 in targets:
                    stores[location][value[1] & ~1].append(provenance)
    indirect = {int(site["site"], 16): site for site in sites if site["kind"] == "indirect"}
    found, unresolved = [], 0
    for events in events_by_owner.values():
        for event in events:
            if event[0] != "call" or event[1] is not None or event[3] not in indirect:
                continue
            value = event[4]
            matches = {}
            location = None
            if value is not None and value[0] == "constant" and value[1] & ~1 in targets:
                matches = {value[1] & ~1: [{"kind": "loaded_pointer"}]}
            elif value is not None and value[0] == "load":
                location = value[1]
                matches = stores.get(location, {})
            if not matches:
                unresolved += 1
            for target, provenance in sorted(matches.items()):
                site = dict(indirect[event[3]], kind="callback", target=target,
                            registrations=sorted({json.dumps(p, sort_keys=True) for p in provenance}))
                site["registrations"] = [json.loads(p) for p in site["registrations"]]
                if location is not None:
                    site["storage"] = value_fact(location)
                found.append(site)
    return found, unresolved


def load_rewrites(paths: list[Path], functions: dict[int, dict], names: dict[str, dict], root: Path) -> dict[int, list[dict]]:
    collected: dict[tuple[int, int], set[str]] = defaultdict(set)
    for path in paths:
        data = json.loads(path.read_text(encoding="utf-8"))
        groups = [(name, rows) for unit in data.get("units", []) for name, rows in unit.get("linker_rewrites", {}).items()]
        groups += list(data.get("linker_rewrites", {}).items())
        for name, rows in groups:
            owner = resolve(name, names)
            if owner not in functions:
                continue
            for row in rows:
                if row["rule"] not in ("inline_nop", "inline_insn"):
                    continue
                site = owner + row["offset"]
                if row.get("removed_callee") is not None:
                    callee = row["removed_callee"]
                    if (row["target"] is not None or not callee.get("symbols") or callee.get("size") not in (4, 8)
                            or not any(a <= site < b for a, b in functions[owner]["code_ranges"])):
                        raise ArgcheckError(f"invalid removed-callee rewrite in {path}")
                    # No image address exists to audit as an original call edge. The source-only
                    # helper and its ABI contract require explicit review (docs/LINKER.md, v3).
                    continue
                site, target = owner + row["offset"], row["target"] & ~1
                if target not in functions or not any(a <= site < b for a, b in functions[owner]["code_ranges"]):
                    raise ArgcheckError(f"invalid linker rewrite address in {path}")
                collected[(site, target)].add(interfaces.display_path(path, root))
    result = defaultdict(list)
    for (site, target), records in sorted(collected.items()):
        result[site].append({"target": target, "kind": "linker_rewrite", "records": sorted(records)})
    return dict(result)


def resolve(name: str, names: dict[str, dict]) -> int:
    if name in names:
        return names[name]["address"] & ~1
    match = re.fullmatch(r"func_([0-9a-fA-F]{8})|(?:0x)?([0-9a-fA-F]{6,8})", name)
    if match:
        return int(match[1] or match[2], 16) & ~1
    raise ArgcheckError(f"unknown function {name!r}")


def parse_declarations(text: str, source: Path, root: Path, names: set[str]) -> list[interfaces.Declaration]:
    prepared = interfaces.prepare(text, source)
    tree = c_parser.CParser().parse(prepared.text, filename=interfaces.display_path(source, root))
    analyzer = interfaces.Analyzer(prepared, source, root, names)
    analyzer.bind_annotations(tree)
    analyzer.visit(tree)
    return [d for d in analyzer.declarations if d.type.kind == "function"]


def size_align(t: interfaces.CType) -> tuple[int, int]:
    if any(q.startswith(("packed", "aligned", "align(", "vector_size", "mode")) for q in t.quals):
        raise ArgcheckError("explicit alignment or machine-mode ABI is not covered")
    if t.kind in ("pointer", "function"):
        return 4, 4
    if t.kind == "base":
        name = t.args[0]
        if name == "void":
            return 0, 1
        size = 8 if name in ("double", "double long") or "long long" in name else 1 if "char" in name or name == "_Bool" else 2 if "short" in name else 4
        return size, min(size, 8)
    if t.kind == "array" and t.args[1] is not None:
        size, alignment = size_align(t.args[0])
        return size * t.args[1], alignment
    if t.kind == "record":
        record = t.args[0]
        if record.kind == "enum":
            return 4, 4
        if record.members is None or record.attrs or any(bits is not None for _, _, bits in record.members):
            raise ArgcheckError("incomplete, attributed or bit-field aggregate ABI is not covered")
        size, alignment = 0, 1
        for _, member, _ in record.members:
            width, align = size_align(member)
            alignment = max(alignment, align)
            size = max(size, width) if record.kind == "union" else (size + align - 1) // align * align + width
        return (size + alignment - 1) // alignment * alignment, alignment
    raise ArgcheckError(f"unsupported ABI type {interfaces.describe(t)}")


def homogeneous(t: interfaces.CType) -> tuple[str, int] | None:
    if t.kind == "base" and t.args[0] in ("float", "double", "double long"):
        return ("float" if t.args[0] == "float" else "double"), 1
    if t.kind == "array" and t.args[1] is not None:
        child = homogeneous(t.args[0])
        if child and child[1] * t.args[1] <= 4:
            return child[0], child[1] * t.args[1]
    if t.kind == "record":
        record = t.args[0]
        if record.members and record.kind in ("struct", "union"):
            children = [homogeneous(member) if bits is None else None for _, member, bits in record.members]
            if all(children) and len({child[0] for child in children}) == 1:
                count = max(child[1] for child in children) if record.kind == "union" else sum(child[1] for child in children)
                if count <= 4:
                    return children[0][0], count
    return None


def prototype_layout(t: interfaces.CType) -> dict:
    result, params, variadic = t.args
    if params is None:
        raise ArgcheckError("unspecified () parameter list has no auditable prototype")
    base_pcs = variadic or any("aapcs" in q and "vfp" not in q for q in t.quals)
    value_in_regs = "value_in_regs" in t.quals or "value_in_regs" in result.quals
    hidden_result = result.kind == "record" and not value_in_regs and (base_pcs or not homogeneous(result)) and size_align(result)[0] > 4
    core = int(hidden_result)
    available = set(range(16))
    locations = []
    for param in params:
        if param.kind in ("array", "function"):
            param = interfaces.CType("pointer", (param,))
        size, alignment = size_align(param)
        floating = None if base_pcs else homogeneous(param)
        registers = []
        if floating:
            width = 2 if floating[0] == "double" else 1
            count = width * floating[1]
            first = next((s for s in range(0, 17 - count, width) if set(range(s, s + count)) <= available), None)
            if first is None:
                available.clear()
            else:
                registers = [f"s{s}" for s in range(first, first + count)]
                available.difference_update(range(first, first + count))
        else:
            core = (core + 1) & ~1 if alignment == 8 else core
            registers = [f"r{r}" for r in range(core, min(4, core + (size + 3) // 4))]
            core += (size + 3) // 4
        locations.append(registers)
    return {"parameter_count": len(params), "parameters": locations, "variadic": variadic,
            "return_void": result.kind == "base" and result.args[0] == "void",
            "hidden_result_registers": ["r0"] if hidden_result else [],
            "registers": sorted({r for group in locations for r in group} | ({"r0"} if hidden_result else set()), key=reg_order)}


def compare(declarations: list[interfaces.Declaration], sites: list[dict]) -> tuple[list[dict], list[dict], list[dict]]:
    issues, notes, prototypes = [], [], []
    for declaration in declarations:
        prototype = {"source": declaration.unit, "site": declaration.site,
                     "type": interfaces.describe(declaration.type), "definition": declaration.definition}
        try:
            layout = prototype_layout(declaration.type)
            prototype.update(layout)
        except ArgcheckError as exc:
            prototype["unchecked"] = str(exc)
            prototypes.append(prototype)
            continue
        prototypes.append(prototype)
        expected = set(layout["registers"])
        evidenced = {arg["register"] for site in sites for arg in site["arguments"]}
        for kind, registers in (("missing_core_parameters", (evidenced - expected) & CORE),
                                ("missing_float_parameters", (evidenced - expected) & VFP)):
            if not registers or layout["variadic"]:
                continue
            relevant = [site["site"] for site in sites if registers & {a["register"] for a in site["arguments"]}]
            issues.append({"kind": kind, "registers": sorted(registers, key=reg_order),
                           "declaration": declaration.site, "sites": sorted(set(relevant)),
                           "qualification": "[INFERENCE] caller preparation is evidence, not proof of parameter count/type"})
        consumed = {r["register"] for site in sites for r in site["results"]}
        if layout["return_void"] and consumed:
            issues.append({"kind": "void_result_consumed", "registers": sorted(consumed, key=reg_order),
                           "declaration": declaration.site,
                           "sites": sorted({site["site"] for site in sites if site["results"]}),
                           "qualification": "[INFERENCE] a preserved input or temporary can resemble a result"})
        absent = expected - evidenced
        if sites and absent:
            notes.append({"kind": "declared_inputs_not_observed", "registers": sorted(absent, key=reg_order),
                          "declaration": declaration.site,
                          "qualification": "[INFERENCE] no caller explicitly supplies these in the local unconsumed-write window; forwarding and earlier/consumed setup are not excluded"})
    return issues, notes, prototypes


def select_inputs(ledgers: list[Path], explicit: list[str], overrides: dict[str, Path], root: Path, names: dict[str, dict]) -> tuple[dict[int, str], dict[Path, interfaces.Unit]]:
    profiles = toolchain.load_profiles(root / "config/compiler_profiles.json")
    selected, units = {}, {}
    paths = ledgers or [root / "config/matches.json"]
    wanted = {resolve(name, names) for name in explicit}
    for ledger in paths:
        data = json.loads(ledger.read_text(encoding="utf-8"))
        for row in data["units"]:
            functions = {resolve(name, names): name for name in row["functions"]}
            if wanted:
                functions = {a: n for a, n in functions.items() if a in wanted}
            if not functions:
                continue
            selected.update(functions)
            path = (root / row["source"]).resolve()
            profile = profiles[row["profile"]]
            if path in units and units[path].profile != profile:
                raise ArgcheckError(f"conflicting compiler profiles for {path}")
            units[path] = interfaces.Unit(path, profile)
    for name, path in overrides.items():
        target = resolve(name, names)
        if not wanted or target in wanted:
            selected[target] = name if name.startswith("func_") else f"func_{target:08x}"
            units[path.resolve()] = interfaces.Unit(path.resolve(), profiles["game"])
    missing = wanted - selected.keys()
    if missing:
        # Explicit address-named functions can be defined in a non-ledger source.
        unbound = missing.copy()
        for path in sorted((root / "src").rglob("*")):
            if path.suffix not in (".c", ".cpp"):
                continue
            text = path.read_text(encoding="utf-8")
            found = {a for a in unbound if re.search(rf"\bfunc_{a:08x}\s*\(", text)}
            if found:
                units[path.resolve()] = interfaces.Unit(path.resolve(), profiles["game"])
                selected.update({a: f"func_{a:08x}" for a in found})
                missing -= found
        if missing:
            raise ArgcheckError("no declaration source for " + ", ".join(address(a) for a in sorted(missing)))
    return selected, units


def audit(root: Path, ledgers: list[Path], explicit: list[str], overrides: dict[str, Path], rewrite_paths: list[Path] | None = None) -> dict:
    started = time.monotonic()
    image = image_mod.Image.load(root / "orig")
    catalogue = image_mod.load_catalogue(root / "config/catalogue.json")
    if catalogue["image_sha256"] != image.sha256:
        raise ArgcheckError("catalogue was generated from a different original image")
    functions = image_mod.functions_by_address(catalogue)
    names = image_mod.load_names(root / "config/names.json")
    selected, units = select_inputs(ledgers, explicit, overrides, root, names)
    if not selected:
        raise ArgcheckError("no functions selected")
    if selected.keys() - functions.keys():
        raise ArgcheckError("selected function is not a catalogue owner")
    rewritten = load_rewrites(rewrite_paths if rewrite_paths is not None else sorted((root / "accept").glob("*/receipts/*.json")), functions, names, root)
    report: dict[str, Any] = {"schema_version": 1, "coverage": COVERAGE, "functions": [], "unchecked": [], "summary": {}}
    declarations = defaultdict(list)
    override_addresses = {resolve(name, names): path.resolve() for name, path in overrides.items()}
    for path, unit in sorted(units.items()):
        source = interfaces.display_path(path, root)
        if path.suffix.lower() != ".c":
            report["unchecked"].append({"source": source, "reason": "C++ not supported by interfaces.py/pycparser"})
            continue
        try:
            parsed = parse_declarations(interfaces.preprocess(unit, root), path, root, set(selected.values()))
            for declaration in parsed:
                try:
                    target = resolve(declaration.name, names)
                except ArgcheckError:
                    continue
                if target in selected and (target not in override_addresses or path == override_addresses[target]):
                    declarations[target].append(declaration)
        except (interfaces.InterfaceError, toolchain.ToolchainError, plyparser.ParseError, OSError, ValueError, ZeroDivisionError) as exc:
            # Parser/compiler diagnostics can quote source text; do not echo them.
            report["unchecked"].append({"source": source, "reason": f"preprocessing/parsing failed ({type(exc).__name__})"})
    decoders = {"arm": capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM),
                "thumb": capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)}
    for decoder in decoders.values():
        decoder.detail = True
    sites, events_by_owner = [], {}
    entry_owners = {entry["address"]: f["address"] for f in functions.values() for entry in f["entries"]}
    entries = set(entry_owners)
    for owner, function in sorted(functions.items()):
        facts = decode_function(image, function, decoders[function["isa"]])
        block_list, successors = blocks(facts, [e["address"] for e in function["entries"]])
        sites.extend(call_sites(function, block_list, entries, rewritten))
        events_by_owner[owner] = symbolic_events(function, block_list, successors, image)
    references = pointer_references(image, catalogue, set(selected))
    callbacks, unresolved = callback_sites(events_by_owner, sites, references, set(selected))
    sites.extend(callbacks)
    grouped = defaultdict(list)
    for site in sites:
        entry_address = site["target"]
        target = entry_owners.get(entry_address, entry_address)
        if target in selected:
            public = {k: v for k, v in site.items() if k != "target" and v is not None}
            if entry_address != target:
                public["entry"] = address(entry_address)
            grouped[target].append(public)
    for target, name in sorted(selected.items()):
        call_evidence = sorted(grouped[target], key=lambda s: (s["site"], s["kind"]))
        decls = declarations[target]
        # Repeated shared declarations add no evidence; preserve distinct source sites.
        unique = {(d.site["source"], d.site["line"], interfaces.type_key(d.type), d.definition): d for d in decls}
        issues, notes, prototypes = compare(list(unique.values()), call_evidence)
        entry = {"name": name, "address": address(target), "prototypes": prototypes,
                 "sites": call_evidence, "address_references": [address(a) for a in references.get(target, [])],
                 "issues": issues, "notes": notes}
        if not prototypes:
            entry["unchecked"] = "no parsed C declaration (see unchecked sources)"
        elif any("unchecked" in p for p in prototypes):
            entry["unchecked"] = "one or more prototype ABIs are not covered"
        report["functions"].append(entry)
    report["summary"] = {
        "functions": len(selected), "flagged_functions": sum(bool(f["issues"]) for f in report["functions"]),
        "issues": sum(len(f["issues"]) for f in report["functions"]),
        "notes": sum(len(f["notes"]) for f in report["functions"]),
        "unchecked_functions": sum("unchecked" in f for f in report["functions"]),
        "direct_sites": sum(s["kind"] == "direct" for f in report["functions"] for s in f["sites"]),
        "tail_sites": sum(s["kind"] == "tail" for f in report["functions"] for s in f["sites"]),
        "callback_sites": len(callbacks),
        "rewrite_sites": sum(s["kind"] == "linker_rewrite" for f in report["functions"] for s in f["sites"]),
        "unresolved_indirect_sites": unresolved,
        "elapsed_seconds": round(time.monotonic() - started, 3),
    }
    return report


def print_report(report: dict) -> None:
    for function in report["functions"]:
        for issue in function["issues"]:
            print(f"REVIEW {function['name']} {issue['kind']}: {','.join(issue['registers'])} at {','.join(issue['sites'])}")
        for note in function["notes"]:
            print(f"NOTE   {function['name']} {note['kind']}: {','.join(note['registers'])}")
        if "unchecked" in function:
            print(f"UNCHECKED {function['name']}: {function['unchecked']}")
    print("Summary: " + json.dumps(report["summary"], sort_keys=True))
    print("[INFERENCE] Review candidates are not automatic prototype repairs; absence of local evidence is not proof of an absent argument.")


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("functions", nargs="*", help="bound names or hexadecimal catalogue addresses")
    parser.add_argument("--matches", type=Path, action="append", default=[], help="ledger; repeat to add candidates (default config/matches.json)")
    parser.add_argument("--source", action="append", default=[], metavar="FUNCTION=FILE", help="override a function's prototype source, e.g. a historical source under scratch/")
    parser.add_argument("--rewrites", type=Path, action="append", help="receipt or prepare.json; default all accept/*/receipts/*.json")
    parser.add_argument("--json", type=Path, required=True, help="write address/register/declaration facts (never original bytes/instructions)")
    args = parser.parse_args(argv)
    try:
        overrides = {}
        for item in args.source:
            name, separator, path = item.partition("=")
            if not separator or not name or not path:
                raise ArgcheckError("--source requires FUNCTION=FILE")
            overrides[name] = ROOT / path
        report = audit(ROOT, [ROOT / p for p in args.matches], args.functions, overrides,
                       None if args.rewrites is None else [ROOT / p for p in args.rewrites])
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")
        print_report(report)
        if report["unchecked"] or report["summary"]["unchecked_functions"]:
            return 2
        return int(bool(report["summary"]["flagged_functions"]))
    except (ArgcheckError, image_mod.ImageError, OSError, ValueError, KeyError, TypeError) as exc:
        # Input diagnostics must not quote original image contents or parser input.
        print(f"argcheck: input/audit error ({type(exc).__name__})", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
