"""Bounded catalogue closure witnesses; no executable payload is serialized.

A closed constant-copy leaf is executable evidence, not a library identity. Its
unobserved historical caller and source-level spelling remain [INFERENCE].
"""
from __future__ import annotations

import hashlib
import json
import subprocess
from pathlib import Path

from capstone import arm


def constant_copy_leaf(analyzer, start: int, limit: int, *, allow_record_layout: bool = False) -> dict | None:
    """Prove a complete straight-line two-pointer copy, not plausible decoding.

    All memory effects are enumerated. Only incoming r1 bytes may be written to
    incoming r0, exactly once, over one equal contiguous positive span. The only
    other memory is a balanced save/restore of incoming registers on the stack.
    Calls, predicates, unknown instructions, indexed memory and register shifts
    fail closed. SP, LR and every callee-saved core register must be restored.
    The optional record contract permits a subset/permutation of loaded source
    words plus immediate constants or incoming r2/r3 words, over an exact bounded
    footprint. Last writes win; holes remain explicit, never implied writes.
    Only caller-saved single-precision VFP registers are permitted, never aliases
    of preserved d8-d15, arithmetic or a variable extent.
    """
    if start % 4 or not start < limit <= analyzer.end or limit - start > 4096:
        return None
    registers = {register: ("input", register) for register in range(arm.ARM_REG_R0, arm.ARM_REG_R12 + 1)}
    registers.update({arm.ARM_REG_SP: ("pointer", "stack", 0), arm.ARM_REG_LR: ("input", arm.ARM_REG_LR)})
    registers[arm.ARM_REG_R0] = ("pointer", "destination", 0)
    registers[arm.ARM_REG_R1] = ("pointer", "source", 0)
    original = dict(registers)
    stack = {}
    loads, stores, fields = [], [], []
    saved = set()

    def pointer(register: int):
        value = registers.get(register)
        return value if value and value[0] == "pointer" else None

    def load(register: int, base, offset: int, width: int) -> bool:
        kind, origin, displacement = base
        position = displacement + offset
        if origin == "source" and position >= 0:
            if (arm.ARM_REG_D0 <= register <= arm.ARM_REG_D31
                    or arm.ARM_REG_S16 <= register <= arm.ARM_REG_S31):
                return False
            registers[register] = ("source", position, width)
            loads.append((position, position + width))
            return True
        if origin == "stack" and width == 4 and position in stack:
            registers[register] = stack.pop(position)
            return True
        return False

    def store(register: int, base, offset: int, width: int) -> bool:
        _, origin, displacement = base
        position = displacement + offset
        value = registers.get(register)
        copied = value == ("source", position, width)
        record_field = (allow_record_layout and width == 4 and position % 4 == 0 and value
                        and (value[0] == "source" and value[2] == 4 or value[0] == "constant"
                             or value[0] == "input" and value[1] in (arm.ARM_REG_R2, arm.ARM_REG_R3)))
        if origin == "destination" and position >= 0 and (copied or record_field):
            stores.append((position, position + width))
            fields.append({"destination_offset": position, "width": width, "kind": value[0],
                           **({"source_offset": value[1]} if value[0] == "source" else {})})
            return True
        if origin == "stack" and width == 4 and position < 0 and position not in stack and value == original.get(register):
            stack[position] = value
            saved.add(register)
            return True
        return False

    def transfer(registers_list, base_register: int, first: int, step: int, loading: bool, writeback: bool) -> bool:
        base = pointer(base_register)
        if base is None or base_register in registers_list:
            return False
        cursor = first
        for register in registers_list:
            width = 8 if arm.ARM_REG_D0 <= register <= arm.ARM_REG_D31 else 4
            if not (load(register, base, cursor, width) if loading else store(register, base, cursor, width)):
                return False
            cursor += width
        if writeback:
            registers[base_register] = ("pointer", base[1], base[2] + step)
        return True

    cursor, instructions = start, 0
    while cursor + 4 <= limit:
        decoded = analyzer.detailed(cursor)
        ins = analyzer.decode(cursor, "arm")
        if decoded is None or ins is None or ins.conditional or ins.call or ins.branch or ins.pools or ins.address_target is not None:
            return None
        instructions += 1
        operands = decoded.operands
        terminal = (decoded.id == arm.ARM_INS_BX and len(operands) == 1 and operands[0].reg == arm.ARM_REG_LR
                    or decoded.id == arm.ARM_INS_MOV and len(operands) == 2
                    and operands[0].reg == arm.ARM_REG_PC and operands[1].reg == arm.ARM_REG_LR)
        if terminal:
            end = cursor + 4
            if registers.get(arm.ARM_REG_SP) != original[arm.ARM_REG_SP] or stack:
                return None
            if any(registers.get(register) != original[register]
                   for register in [arm.ARM_REG_LR, *range(arm.ARM_REG_R4, arm.ARM_REG_R11 + 1)]):
                return None
            ordered = sorted(stores)
            if not ordered or ordered[0][0] != 0 or ordered[-1][1] > 4096:
                return None
            if not allow_record_layout and any(left[1] != right[0] for left, right in zip(ordered, ordered[1:])):
                return None
            final_fields = list({row["destination_offset"]: row for row in fields}.values())
            source = sorted(set(loads))
            if allow_record_layout:
                if (not source or source[0][0] != 0
                        or any(left[1] != right[0] for left, right in zip(source, source[1:]))
                        or not any(row["kind"] == "source" for row in final_fields)
                        or not {row["source_offset"] for row in final_fields if row["kind"] == "source"}
                        <= {first for first, _ in source}):
                    return None
            elif sorted(loads) != ordered:
                return None
            return {"start": start, "end": end, "copy_bytes": ordered[-1][1],
                    "source_bytes": source[-1][1], "fields": sorted(final_fields, key=lambda row: row["destination_offset"]),
                    "write_spans": [list(span) for span in sorted(set(stores))], "store_count": len(stores),
                    "instruction_count": instructions, "saved_register_count": len(saved),
                    "contract": "closed_fixed_record_leaf" if allow_record_layout else "closed_constant_copy_leaf",
                    "ownership_classified": False,
                    "qualification": "[INFERENCE] Historical section/caller identity is not recovered; exact closed record effects and balanced ABI are proved."}
        if ins.stop or decoded.update_flags or decoded.writeback and decoded.id not in (
                arm.ARM_INS_PUSH, arm.ARM_INS_POP, arm.ARM_INS_LDM, arm.ARM_INS_STM,
                arm.ARM_INS_STMDB, arm.ARM_INS_LDR, arm.ARM_INS_STR, arm.ARM_INS_LDRB,
                arm.ARM_INS_STRB, arm.ARM_INS_LDRH, arm.ARM_INS_STRH,
                arm.ARM_INS_VLDMIA, arm.ARM_INS_VSTMIA):
            return None
        if decoded.id == arm.ARM_INS_PLD:
            if any(op.type == arm.ARM_OP_MEM and (op.mem.index or pointer(op.mem.base) is None) for op in operands):
                return None
        elif decoded.id in (arm.ARM_INS_PUSH, arm.ARM_INS_POP):
            regs = [op.reg for op in operands if op.type == arm.ARM_OP_REG]
            if not regs or len(regs) != len(operands) or arm.ARM_REG_PC in regs or arm.ARM_REG_SP in regs:
                return None
            loading = decoded.id == arm.ARM_INS_POP
            size = 4 * len(regs)
            if not transfer(regs, arm.ARM_REG_SP, 0 if loading else -size, size if loading else -size, loading, True):
                return None
        elif decoded.id in (arm.ARM_INS_LDM, arm.ARM_INS_STM, arm.ARM_INS_STMDB, arm.ARM_INS_VLDMIA, arm.ARM_INS_VSTMIA):
            if len(operands) < 2 or any(op.type != arm.ARM_OP_REG for op in operands):
                return None
            base_register, regs = operands[0].reg, [op.reg for op in operands[1:]]
            loading = decoded.id in (arm.ARM_INS_LDM, arm.ARM_INS_VLDMIA)
            size = sum(8 if arm.ARM_REG_D0 <= reg <= arm.ARM_REG_D31 else 4 for reg in regs)
            decrement = decoded.id == arm.ARM_INS_STMDB
            if arm.ARM_REG_PC in regs or not transfer(regs, base_register, -size if decrement else 0,
                    -size if decrement else size, loading, decoded.writeback):
                return None
        elif decoded.id in (arm.ARM_INS_LDR, arm.ARM_INS_STR, arm.ARM_INS_LDRB, arm.ARM_INS_STRB,
                            arm.ARM_INS_LDRH, arm.ARM_INS_STRH, arm.ARM_INS_VLDR, arm.ARM_INS_VSTR):
            if (len(operands) not in (2, 3) or operands[0].type != arm.ARM_OP_REG
                    or operands[1].type != arm.ARM_OP_MEM or operands[1].mem.index
                    or any(op.shift.type for op in operands)):
                return None
            base_register = operands[1].mem.base
            base = pointer(base_register)
            if base is None or operands[0].reg == base_register:
                return None
            width = (1 if decoded.id in (arm.ARM_INS_LDRB, arm.ARM_INS_STRB) else
                     2 if decoded.id in (arm.ARM_INS_LDRH, arm.ARM_INS_STRH) else
                     8 if arm.ARM_REG_D0 <= operands[0].reg <= arm.ARM_REG_D31 else 4)
            loading = decoded.id in (arm.ARM_INS_LDR, arm.ARM_INS_LDRB, arm.ARM_INS_LDRH, arm.ARM_INS_VLDR)
            effect = load if loading else store
            if not effect(operands[0].reg, base, operands[1].mem.disp, width):
                return None
            if decoded.writeback:
                delta = operands[1].mem.disp
                if len(operands) == 3:
                    if operands[2].type != arm.ARM_OP_IMM:
                        return None
                    delta = operands[2].imm
                registers[base_register] = ("pointer", base[1], base[2] + delta)
            elif len(operands) == 3:
                return None
        elif decoded.id in (arm.ARM_INS_ADD, arm.ARM_INS_SUB):
            if (len(operands) != 3 or any(op.shift.type for op in operands)
                    or operands[0].type != arm.ARM_OP_REG or operands[1].type != arm.ARM_OP_REG
                    or operands[2].type != arm.ARM_OP_IMM or operands[0].reg in (arm.ARM_REG_SP, arm.ARM_REG_LR, arm.ARM_REG_PC)):
                return None
            base = pointer(operands[1].reg)
            if base is None:
                return None
            delta = operands[2].imm * (-1 if decoded.id == arm.ARM_INS_SUB else 1)
            registers[operands[0].reg] = ("pointer", base[1], base[2] + delta)
        elif (allow_record_layout and decoded.id == arm.ARM_INS_MOV and len(operands) == 2
              and operands[0].type == arm.ARM_OP_REG and operands[1].type == arm.ARM_OP_IMM
              and operands[0].reg in (arm.ARM_REG_R2, arm.ARM_REG_R3, arm.ARM_REG_R12)):
            registers[operands[0].reg] = ("constant", operands[1].imm)
        elif decoded.id == arm.ARM_INS_MOV and len(operands) == 2 and all(op.type == arm.ARM_OP_REG and not op.shift.type for op in operands):
            if operands[0].reg in (arm.ARM_REG_SP, arm.ARM_REG_LR, arm.ARM_REG_PC):
                return None
            registers[operands[0].reg] = registers.get(operands[1].reg)
        else:
            return None
        cursor += 4
    return None


def read_footprints(catalogue: dict, analyzer, *, address_witnesses: list[dict] | None = None,
                    include_reads: bool = True) -> list[dict]:
    """Enumerate actual fixed-width reads from locally proved image addresses.

    ADR alone is not a read. Values do not flow through a join, conditional
    definition, unknown arithmetic or a call-clobbered register. Reading a pool
    pointer does not grant ownership of its pointee; only a later actual read
    contributes that exact span. No payload or pointer value is serialized.
    """
    rows = []
    for function in catalogue["functions"]:
        if function["kind"] == "veneer":
            continue
        isa = function["isa"]
        decoded = {}
        for first, end in function["code_ranges"]:
            cursor = first
            while cursor < end:
                instruction = (analyzer.detailed(cursor) if isa == "arm" else next(
                    analyzer.thumb.disasm(analyzer.text[cursor - analyzer.base:cursor - analyzer.base + 4], cursor, 1), None))
                if instruction is None:
                    break
                decoded[cursor] = instruction
                cursor += instruction.size
        joins = {entry["address"] for entry in function["entries"]}
        joins.update(ins.target[0] for site in decoded
                     for ins in [analyzer.decode(site, isa)] if ins.branch and ins.target)
        known = {}
        previous_end = None
        for site, instruction in decoded.items():
            if site != previous_end or site in joins:
                known.clear()
            previous_end = site + instruction.size
            ins = analyzer.decode(site, isa)
            operands = instruction.operands
            writes = set(instruction.regs_access()[1])
            before = dict(known)
            for register in writes:
                known.pop(register, None)
            if ins.conditional:
                continue
            source_site = site
            address_source = "architectural_pc_load"
            reads = []
            load_ids = (arm.ARM_INS_LDR, arm.ARM_INS_LDRB, arm.ARM_INS_LDRH, arm.ARM_INS_VLDR)
            if (not ins.address_reference and instruction.id in load_ids and len(operands) >= 2
                    and operands[1].type == arm.ARM_OP_MEM and operands[1].mem.base == arm.ARM_REG_PC
                    and not operands[1].mem.index):
                width = (1 if instruction.id == arm.ARM_INS_LDRB else 2 if instruction.id == arm.ARM_INS_LDRH
                         else 8 if arm.ARM_REG_D0 <= operands[0].reg <= arm.ARM_REG_D31 else 4)
                pc = site + 8 if isa == "arm" else (site + 4) & ~3
                first = pc + operands[1].mem.disp
                reads.append((first, first + width))
            if instruction.id in load_ids and len(operands) >= 2 and operands[1].type == arm.ARM_OP_MEM:
                memory = operands[1].mem
                if memory.base in before and not memory.index and not instruction.writeback:
                    base, source_site, address_source = before[memory.base]
                    width = (1 if instruction.id == arm.ARM_INS_LDRB else 2 if instruction.id == arm.ARM_INS_LDRH
                             else 8 if arm.ARM_REG_D0 <= operands[0].reg <= arm.ARM_REG_D31 else 4)
                    reads.append((base + memory.disp, base + memory.disp + width))
            elif instruction.id in (arm.ARM_INS_LDM, arm.ARM_INS_VLDMIA) and operands and operands[0].reg in before:
                base, source_site, address_source = before[operands[0].reg]
                width = sum(8 if arm.ARM_REG_D0 <= operand.reg <= arm.ARM_REG_D31 else 4 for operand in operands[1:])
                reads.append((base, base + width))
            for first, end in reads if include_reads else []:
                if analyzer.base <= first < end <= analyzer.end:
                    rows.append({"function": function["address"], "site": site, "start": first, "end": end,
                                 "address_source_site": source_site, "address_source": address_source,
                                 "evidence": "exact_architectural_read_footprint"})
            if operands and operands[0].type == arm.ARM_OP_REG:
                destination = operands[0].reg
                if ins.address_target is not None:
                    known[destination] = (ins.address_target, site, "pc_relative_address")
                elif ins.pc_add_register is not None and ins.pc_add_register in before:
                    value, origin, _ = before[ins.pc_add_register]
                    known[destination] = ((site + 4 + value) & 0xFFFFFFFF, origin, "constant_plus_thumb_pc")
                elif ins.register_constant is not None:
                    register, value = ins.register_constant
                    known[register] = (value, site, "literal_or_immediate")
                elif instruction.id == arm.ARM_INS_LDR and ins.pools:
                    first, end = ins.pools[0]
                    if end - first == 4 and analyzer.base <= first <= analyzer.end - 4:
                        known[destination] = (analyzer.word(first), site, "loaded_pool_pointer")
                elif instruction.id == arm.ARM_INS_MOV and len(operands) == 2 and operands[1].type == arm.ARM_OP_REG:
                    if operands[1].reg in before:
                        known[destination] = before[operands[1].reg]
                elif (instruction.id in (arm.ARM_INS_ADD, arm.ARM_INS_SUB) and len(operands) == 3
                      and operands[1].type == arm.ARM_OP_REG and operands[1].reg in before
                      and operands[2].type == arm.ARM_OP_IMM and not any(op.shift.type for op in operands)):
                    value, origin, basis = before[operands[1].reg]
                    known[destination] = (value + operands[2].imm * (-1 if instruction.id == arm.ARM_INS_SUB else 1), origin, basis)
                if address_witnesses is not None and (ins.address_target is not None or ins.pc_add_register is not None):
                    formed = known.get(destination)
                    if formed is not None and formed[0] & 1 and analyzer.inside(formed[0] & ~1, "thumb"):
                        address_witnesses.append({"function": function["address"], "site": site,
                                                  "target": formed[0] & ~1, "isa": "thumb",
                                                  "definition_site": formed[1], "evidence": "dominated_tagged_pc_relative_value"})
            if ins.call:
                for register in (arm.ARM_REG_R0, arm.ARM_REG_R1, arm.ARM_REG_R2, arm.ARM_REG_R3, arm.ARM_REG_R12, arm.ARM_REG_LR):
                    known.pop(register, None)
            elif ins.branch or ins.stop:
                known.clear()
    return sorted(rows, key=lambda row: (row["start"], row["end"], row["function"], row["site"]))


def merge_closed_graphs(catalogue: dict, analyzer, records: list[dict], protected: dict[int, dict]) -> list[dict]:
    """Join a witnessed entry-connected CFG across only weak prologue guesses.

    Every old label survives, so the result is conservatively multi-entry, not a
    library/runtime exclusion or C credit. A genuine incoming entry, protected
    record, data/pool crossing, incomplete graph or unbound tail vetoes the merge.
    """
    allowed = {getattr(arm, f"ARM_INS_{name}") for name in (
        "ADC", "ADD", "AND", "ASR", "B", "BIC", "BX", "CMP", "EOR", "LDM",
        "LDMDB", "LDMIB", "LSL", "LSR", "MOV", "MVN", "ORR", "PLD", "POP",
        "PUSH", "RSB", "SBC", "STM", "STMDA", "STMDB", "STMIB", "STR", "STRB", "STRH",
        "SUB", "TST", "LDR", "LDRB", "LDRH")}
    functions = catalogue["functions"]
    merged = []
    for record in records:
        first, end = record["start"], record["end"]
        if (first % 4 or end % 4 or not analyzer.base <= first < end <= analyzer.end or end - first > 0x10000
                or not record.get("behavior_witness") or record.get("behavior_cases", 0) <= 0
                or record.get("behavior_copy_and_abi_passes") != record["behavior_cases"]
                or hashlib.sha256(analyzer.text[first - analyzer.base:end - analyzer.base]).hexdigest()
                != record["image_interval_sha256"]):
            raise ValueError("closed graph lacks a bounded original entry/ABI witness")
        if any(function["address"] < end and first < function["address"] + function["size"]
               for function in protected.values()):
            raise ValueError("closed graph would alter a protected record")
        members = [function for function in functions if function["address"] < end
                   and first < function["address"] + function["size"]]
        if not members or [function["address"] for function in members] != record["legacy_owners"]:
            raise ValueError("closed graph members differ from the witnessed weak owners")
        for function in members:
            if (not first < function["address"] < function["address"] + function["size"] <= end
                    or function["isa"] != "arm" or function["kind"] != "function"
                    or function["evidence"] != ["prologue"] or len(function["entries"]) != 1
                    or function["entries"][0]["evidence"] != ["prologue"] or function["entries"][0]["sources"]
                    or function["pools"] or function["literals"] or not analyzer.prologue(function["address"])):
                raise ValueError("closed graph cannot erase a genuine entry or data owner")
        member_addresses = {function["address"] for function in members}
        if any(row["start"] < end and first < row["end"]
               and not (row["kind"] == "unknown" or row["kind"] == "function" and row["owner"] in member_addresses)
               for row in catalogue["ranges"]):
            raise ValueError("closed graph crosses an independently classified range")
        if first in analyzer.seeds or not analyzer.add_seed(first, "arm", "closed_entry_cfg"):
            raise ValueError("closed graph start is not a new evidenced ARM entry")
        try:
            probe = analyzer.trace(first, end, internal_starts=set())
        finally:
            del analyzer.seeds[first]
        entry_owners = {entry["address"]: function for function in functions for entry in function["entries"]}
        if (probe.issues or probe.open_exits or probe.pools or probe.calls or not probe.return_sites
                or sum(probe.code.values()) != end - first
                or any(address not in probe.code for address in member_addresses)
                or any(analyzer.detailed(site).id not in allowed for site in probe.code)
                or any(target not in entry_owners or isa != "arm" or entry_owners[target]["isa"] != isa
                       or entry_owners[target]["noreturn"] for target, isa, _ in probe.tails)):
            raise ValueError("closed graph does not retain one complete connected normal ARM CFG")
        starts = sorted(analyzer.seeds)
        old_traces = [analyzer.trace(function["address"],
                                    next((address for address in starts if address > function["address"]), analyzer.end))
                      for function in members]
        entries = [{"address": first, "isa": "arm", "kind": "entry", "evidence": ["closed_entry_cfg"], "sources": []}]
        entries.extend({"address": function["address"], "isa": "arm", "kind": "secondary_entry",
                        "evidence": ["legacy_prologue"], "sources": []} for function in members)
        replacement = {"address": first, "size": end - first, "code_size": end - first, "isa": "arm",
                       "kind": "multi_entry", "evidence": ["closed_entry_cfg"], "entries": entries,
                       "code_ranges": [[first, end]], "pools": [], "literals": [],
                       "noreturn": False, "noreturn_reasons": []}
        functions[:] = [function for function in functions if function["address"] not in member_addresses] + [replacement]
        functions.sort(key=lambda function: function["address"])
        merged.append({**record, "legacy_sizes": {str(function["address"]): function["size"] for function in members},
                       "former_ordinary_extent_bytes": sum(function["size"] for function in members),
                       "code_bytes_added": end - first - sum(sum(hi - lo for lo, hi in function["code_ranges"]) for function in members),
                       "instruction_delta": len(probe.code) - sum(len(trace.code) for trace in old_traces),
                       "tail_call_delta": len(probe.tails) - sum(len(trace.tails) for trace in old_traces),
                       "ownership_classified": False,
                       "qualification": "[INFERENCE] Original historical section identity is not recovered; prior weak labels remain, conservatively topology-excluding this owner, not proving non-C ownership."})
    return merged


def close_catalogue(catalogue: dict, analyzer, evidence: dict, protected: dict[int, dict]) -> None:
    """Apply proved leaves, exact shared reads and zero halfword alignment.

    Protected records are immutable. New leaves are ordinary uncredited C-denominator
    candidates; entry-connected weak-owner groups retain every legacy label.
    Neither topology exclusions nor global rodata/alignment grant library identity.
    """
    from collections import Counter

    functions = catalogue["functions"]
    owner_merges = merge_closed_graphs(catalogue, analyzer, evidence.get("closed_graph_owners", []), protected)
    merged_owners = {address: row["start"] for row in owner_merges for address in row["legacy_owners"]}
    leaves = []
    for gap in catalogue["ranges"]:
        if gap["kind"] != "unknown" or gap["start"] % 4:
            continue
        # Sparse holes within existing owners are not new callable entries.
        if any(function["address"] <= gap["start"] < function["address"] + function["size"] for function in functions):
            continue
        cursor = gap["start"]
        while cursor < gap["end"]:
            proof = constant_copy_leaf(analyzer, cursor, gap["end"])
            if proof is None:
                proof = constant_copy_leaf(analyzer, cursor, gap["end"], allow_record_layout=True)
            if proof is None:
                break
            end = proof["end"]
            function = {"address": cursor, "size": end - cursor, "code_size": end - cursor, "isa": "arm",
                        "kind": "function", "pools": [], "literals": [], "code_ranges": [[cursor, end]],
                        "evidence": [proof["contract"]], "entries": [
                            {"address": cursor, "isa": "arm", "kind": "entry", "evidence": [proof["contract"]], "sources": []}],
                        "noreturn": False, "noreturn_reasons": []}
            functions.append(function)
            leaves.append(proof)
            cursor = end
    functions.sort(key=lambda function: function["address"])
    code = [(first, end) for function in functions for first, end in function["code_ranges"]]
    entries = {entry["address"] for function in functions for entry in function["entries"]}
    footprints = read_footprints(catalogue, analyzer)
    needed_sites = {row["address"] for row in catalogue["anomalies"] if row["issue"] == "literal_outside_function"}
    unknown = [row for row in catalogue["ranges"] if row["kind"] == "unknown"]
    accepted_reads = [row for row in footprints if (
        row["site"] in needed_sites or any(gap["start"] < row["end"] and row["start"] < gap["end"] for gap in unknown))
        and not any(first < row["end"] and row["start"] < end for first, end in code)
        and not any(row["start"] <= entry < row["end"] for entry in entries)]
    replacements = [(row["start"], row["end"], "function", row["start"]) for row in leaves]
    replacements.extend((row["start"], row["end"], "function", row["start"]) for row in owner_merges)
    replacements.extend((row["start"], row["end"], "rodata", None) for row in accepted_reads)
    alignment = []
    for gap in catalogue["ranges"]:
        first, end = gap["start"], gap["end"]
        if gap["kind"] != "unknown" or first % 4 != 2 or end != first + 2 or any(analyzer.text[first - analyzer.base:end - analyzer.base]):
            continue
        preceding = None
        for function in functions:
            if function["isa"] != "thumb":
                continue
            for lo, hi in function["code_ranges"]:
                cursor = lo
                while cursor < hi:
                    ins = analyzer.decode(cursor, "thumb")
                    if cursor + ins.size == first:
                        preceding = (function, cursor, ins)
                    cursor += ins.size
        boundary = (end in entries or any(end == lo for function in functions
                                          for lo, _ in function["pools"] + function["literals"])
                    or any(end == row["start"] for row in catalogue["shared_literals"]))
        if (preceding is None or preceding[2].conditional or not boundary
                or not (preceding[2].returns or preceding[2].branch or preceding[2].stop)
                or any(row["start"] < end and first < row["end"] for row in footprints)):
            continue
        alignment.append({"start": first, "end": end, "function": preceding[0]["address"],
                          "terminal": preceding[1], "evidence": "zero_halfword_after_thumb_terminal_before_word_boundary"})
        replacements.append((first, end, "padding", None))
    old_ranges = catalogue["ranges"]
    new_ranges = []
    for old in old_ranges:
        # Every witness is text-local. Keep other mapped segments' boundaries:
        # adjacent, equally labelled page padding must not become one range.
        if old["end"] <= analyzer.base or old["start"] >= analyzer.end:
            new_ranges.append(dict(old))
            continue
        cuts = {old["start"], old["end"]}
        applicable = [row for row in replacements if row[0] < old["end"] and old["start"] < row[1]]
        for first, end, _, _ in applicable:
            cuts.update((max(first, old["start"]), min(end, old["end"])))
        ordered = sorted(cuts)
        for first, end in zip(ordered, ordered[1:]):
            kind, owner = old["kind"], old["owner"]
            if kind == "unknown" or kind == "function" and owner in merged_owners:
                covering = [row for row in applicable if row[0] <= first < end <= row[1]]
                if covering:
                    chosen = next((row for row in covering if row[2] == "function"), covering[0])
                    kind, owner = chosen[2], chosen[3]
            if new_ranges and (new_ranges[-1]["kind"], new_ranges[-1]["owner"], new_ranges[-1]["end"]) == (kind, owner, first):
                new_ranges[-1]["end"] = end
            else:
                new_ranges.append({"start": first, "end": end, "kind": kind, "owner": owner})
    catalogue["ranges"] = new_ranges
    explained = []
    retained = []
    for issue in catalogue["anomalies"]:
        site = issue["address"]
        if issue["issue"] == "literal_outside_function":
            if any(row["site"] == site for row in accepted_reads):
                continue
        if issue["issue"] == "unexplained_function_bytes":
            if site in merged_owners:
                merge = next(row for row in owner_merges if row["start"] == merged_owners[site])
                end = site + merge["legacy_sizes"][str(site)]
                explained.append({"function": site, "replacement_owner": merge["start"],
                                  "holes": [{"start": row["start"], "end": row["end"], "kind": row["kind"]}
                                            for row in old_ranges if site <= row["start"] < end and row["kind"] != "function"],
                                  "evidence": "entry_connected_closed_cfg_recovers_sparse_code_with_all_legacy_labels_retained"})
                continue
            function = next(function for function in functions if function["address"] == site)
            end = site + function["size"]
            holes = [{"start": max(site, row["start"]), "end": min(end, row["end"]), "kind": row["kind"]}
                     for row in new_ranges if row["start"] < end and site < row["end"] and row["owner"] != site]
            if holes and all(row["kind"] in ("rodata", "padding") for row in holes):
                explained.append({"function": site, "holes": holes,
                                  "evidence": "extent_separately_explained_by_bounded_rodata_or_alignment_not_function_credit"})
                continue
        retained.append(issue)
    catalogue["anomalies"] = retained
    catalogue["closure_evidence"] = {"closed_leaves": leaves, "read_footprints": accepted_reads,
                                   "owner_merges": owner_merges, "alignment": alignment, "extent_explanations": explained}
    stats = catalogue["stats"]
    stats.update({"functions": len(functions),
                  "functions_by_isa": dict(sorted(Counter(function["isa"] for function in functions).items())),
                  "functions_by_kind": dict(sorted(Counter(function["kind"] for function in functions).items())),
                  "starts_by_evidence": dict(sorted(Counter(item for function in functions for item in function["evidence"]).items())),
                  "bytes_by_range_kind": dict(sorted(Counter({kind: sum(row["end"] - row["start"] for row in new_ranges if row["kind"] == kind)
                                                            for kind in {row["kind"] for row in new_ranges}}).items())),
                  "unknown_ranges": sum(row["kind"] == "unknown" for row in new_ranges),
                  "anomalies": len(retained), "anomalies_by_issue": dict(sorted(Counter(row["issue"] for row in retained).items())),
                  "code_bytes": sum(end - first for function in functions for first, end in function["code_ranges"]),
                  "creditable_functions": sum(function["kind"] == "function" for function in functions),
                  "creditable_bytes": sum(function["size"] for function in functions if function["kind"] == "function")})
    stats["decoded_instructions"] += sum(row["instruction_count"] for row in leaves) + sum(row["instruction_delta"] for row in owner_merges)
    stats["tail_calls"] += sum(row["tail_call_delta"] for row in owner_merges)
    stats["prologue_only_starts"] = sum(function["evidence"] == ["prologue"] for function in functions)
    stats["closed_graph_owner_merges"] = len(owner_merges)
    stats["closed_graph_absorbed_owners"] = sum(len(row["legacy_owners"]) for row in owner_merges)


def load_inputs(root: Path, parent_path: Path) -> tuple[dict, dict, dict]:
    """Load immutable metadata witnesses, never changing author directories."""
    path = root / "research/catalogue_v10_evidence.json"
    evidence = json.loads(path.read_bytes())
    descriptor = evidence["parent_evidence"]
    if root / descriptor["path"] != parent_path or hashlib.sha256(parent_path.read_bytes()).hexdigest() != descriptor["sha256"]:
        raise ValueError("closure parent evidence differs from its frozen identity")
    descriptor = evidence["baseline_catalogue"]
    blob = descriptor["blob"]
    if len(blob) != 40 or any(character not in "0123456789abcdef" for character in blob):
        raise ValueError("closure baseline requires an immutable Git blob")
    result = subprocess.run(["git", "show", blob], cwd=root, capture_output=True, check=False)
    if result.returncode or hashlib.sha256(result.stdout).hexdigest() != descriptor["sha256"]:
        raise ValueError("closure adopted baseline differs from its frozen identity")
    prior = json.loads(result.stdout)
    prior["source_catalogue_sha256"] = descriptor["sha256"]
    prior["revision"] = descriptor["revision"]
    if prior["image_sha256"] != evidence["image_sha256"]:
        raise ValueError("closure baseline/image identity is inconsistent")
    inputs = {str(path.relative_to(root)): hashlib.sha256(path.read_bytes()).hexdigest(),
              f"git:{descriptor['reference']}@{blob}": descriptor["sha256"]}
    if "earlier_proposal" in evidence:
        descriptor = evidence["earlier_proposal"]
        earlier_path = root / descriptor["path"]
        payload = earlier_path.read_bytes()
        if hashlib.sha256(payload).hexdigest() != descriptor["sha256"]:
            raise ValueError("closure earlier proposal differs from its frozen metadata")
        evidence["_earlier_catalogue"] = json.loads(payload)
        inputs[str(earlier_path.relative_to(root))] = descriptor["sha256"]
    return evidence, prior, inputs


def enrich_review(candidate: dict, prior: dict, image: bytes, layout, evidence: dict) -> None:
    """Conserve every old unknown byte/anomaly and preserve all proposed entries."""
    if __package__:
        from . import catalogue_review
    else:
        import catalogue_review
    old_entries = {(entry["address"], entry["isa"]) for function in prior["functions"] for entry in function["entries"]}
    new_entries = {(entry["address"], entry["isa"]) for function in candidate["functions"] for entry in function["entries"]}
    old_identities = {(function["address"], function["size"], function["isa"]) for function in prior["functions"]}
    new_identities = {(function["address"], function["size"], function["isa"]) for function in candidate["functions"]}
    extent_changes = []
    current = {function["address"]: function for function in candidate["functions"]}
    for address, size, isa in sorted(old_identities - new_identities):
        after = current.get(address)
        merge = next((row for row in candidate["closure_evidence"]["owner_merges"] if address in row["legacy_owners"]), None)
        if after is None and merge is not None:
            replacement = current[merge["start"]]
            before = next(function for function in prior["functions"] if function["address"] == address)
            if (isa != replacement["isa"] or not all(any(nlo <= lo < hi <= nhi for nlo, nhi in replacement[kind])
                                                   for kind in ("code_ranges", "pools", "literals") for lo, hi in before[kind])):
                raise ValueError("closed graph loses a prior owned span")
            extent_changes.append({"address": address, "old_size": size, "replacement_owner": merge["start"],
                                   "size": replacement["size"], "isa": isa, "entry_preserved": True,
                                   "basis": "entry_connected_closed_cfg_with_retained_legacy_prologue"})
            continue
        bounds = [((row["end"] + 3) & ~3) for row in evidence["literal_encodings"] if row["function"] == address]
        if (after is None or after["isa"] != isa or not bounds
                or address + after["size"] != max(address + size, *bounds)):
            raise ValueError("closure loses a prior proposed identity without an exact typed extent repair")
        extent_changes.append({"address": address, "old_size": size, "size": after["size"], "isa": isa,
                               "basis": "exact_source_witnessed_utf16_object_bound"})
    if old_entries - new_entries:
        raise ValueError("closure loses a prior proposed entry")
    unknown_rows, anomaly_rows = catalogue_review.accounting(prior, candidate, image, layout)
    closure = candidate["closure_evidence"]
    old_unknown = [row for row in prior["ranges"] if row["kind"] == "unknown"]
    for row in candidate["ranges"]:
        if row["kind"] == "unknown" and sum(max(0, min(row["end"], old["end"]) - max(row["start"], old["start"]))
                                          for old in old_unknown) != row["end"] - row["start"]:
            raise ValueError("closure introduces unknown bytes outside the adopted baseline")
    leaves = {row["start"]: row for row in closure["closed_leaves"]}
    for row in unknown_rows:
        for part in row["parts"]:
            first, end = part["start"], part["end"]
            if part["owner"] in leaves:
                part["reason"] = "Complete closed symbolic leaf with exact fixed memory effects, preserved callee registers and stack; ordinary denominator candidate, not library ownership"
            elif part["owner"] in {row["start"] for row in closure["owner_merges"]}:
                part["reason"] = "Original entry ABI witness and complete connected CFG; prior weak labels retained in a multi-entry owner. Topology exclusion is not non-C ownership."
            elif part["kind"] == "function":
                objects = [item for item in evidence["literal_encodings"]
                           if item["target"] < end and first < ((item["end"] + 3) & ~3)]
                part["reason"] = ("Exact source/object-witnessed UTF-16 record and zero alignment" if objects
                                  else "Tagged PC-relative callable pointer and complete CFG recover instruction ownership")
            elif part["kind"] == "rodata":
                reads = [item for item in closure["read_footprints"] if item["start"] < end and first < item["end"]]
                part["reason"] = ("Exact architectural fixed-width reads; unowned rodata, no loading-owner extent growth" if reads
                                  else "Explicit typed, complete shared UTF-16 object; no exclusive ownership")
                part["read_sites"] = sorted({item["site"] for item in reads})
            elif part["kind"] == "padding":
                part["reason"] = "Exact zero halfword after a Thumb terminal before an evidenced aligned pool/entry; no incoming reference"
        row["disposition"] = ("resolved" if all(part["kind"] != "unknown" for part in row["parts"])
                              else "partial" if any(part["kind"] != "unknown" for part in row["parts"]) else "open")
    for row in anomaly_rows:
        if row["disposition"] == "resolved":
            proof = next((item for item in closure["extent_explanations"] if item["function"] == row["address"]), None)
            row["reason"] = (proof["evidence"] if proof else
                             "Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue")
    old_records = {function["address"]: function for function in prior["functions"]}
    changed_records = []
    for function in candidate["functions"]:
        before = old_records.get(function["address"])
        digest = hashlib.sha256((json.dumps(function, indent=2, sort_keys=True, ensure_ascii=False) + "\n").encode()).hexdigest()
        before_digest = (before.get("record_sha256") or hashlib.sha256(
            (json.dumps(before, indent=2, sort_keys=True, ensure_ascii=False) + "\n").encode()).hexdigest()) if before else None
        if before is None or before_digest != digest:
            changed_records.append({"address": function["address"], "status": "new" if before is None else "changed",
                                    "before_sha256": before_digest, "after_sha256": digest})
    candidate["revision_review"]["previous_catalogue"] = {
        "revision": prior["revision"], "catalogue_sha256": prior["source_catalogue_sha256"],
        "prior_identities": len(old_identities), "retained_prior_identities": len(old_identities & new_identities),
        "reviewed_extent_changes": extent_changes,
        "prior_entries": len(old_entries), "retained_prior_entries": len(old_entries & new_entries),
        "unknown_accounting": unknown_rows, "anomaly_accounting": anomaly_rows, "changed_records": changed_records,
        "new_anomalies": [row for row in candidate["anomalies"] if row not in prior["anomalies"]],
        "ownership_exclusions_added": sum(function["address"] in old_records
                                          and old_records[function["address"]]["kind"] == "function"
                                          and function["kind"] in ("assembly", "runtime", "library")
                                          for function in candidate["functions"]),
        "topology_exclusions": sum(function["kind"] == "multi_entry" for function in candidate["functions"]),
    }
    if "_earlier_catalogue" in evidence:
        earlier = evidence["_earlier_catalogue"]
        unknown, anomalies = catalogue_review.accounting(earlier, candidate, image, layout)
        for row in unknown:
            row["disposition"] = ("resolved" if all(part["kind"] != "unknown" for part in row["parts"])
                                  else "partial" if any(part["kind"] != "unknown" for part in row["parts"]) else "open")
        candidate["revision_review"]["superseded_proposal"] = {
            "revision": earlier["revision"], "catalogue_sha256": earlier["source_catalogue_sha256"],
            "unknown_accounting": unknown, "anomaly_accounting": anomalies,
        }
