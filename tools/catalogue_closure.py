"""Bounded catalogue closure witnesses; no executable payload is serialized.

A closed constant-copy leaf is executable evidence, not a library identity. Its
unobserved historical caller and source-level spelling remain [INFERENCE].
"""
from __future__ import annotations

import hashlib
import json
import subprocess
from pathlib import Path

from capstone import CS_AC_WRITE, arm


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
        if (decoded is None or ins is None or ins.conditional or ins.call or ins.branch or ins.pools
                or ins.address_target is not None or decoded.update_flags):
            return None
        instructions += 1
        operands = decoded.operands
        terminal = (decoded.id == arm.ARM_INS_BX and len(operands) == 1 and operands[0].reg == arm.ARM_REG_LR
                    or decoded.id == arm.ARM_INS_MOV and len(operands) == 2
                    and operands[0].reg == arm.ARM_REG_PC and operands[1].reg == arm.ARM_REG_LR
                    and not any(operand.shift.type for operand in operands))
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
        if ins.stop or decoded.writeback and decoded.id not in (
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


_SCALAR_READ_WIDTHS = {
    **{getattr(arm, f"ARM_INS_{name}"): 1 for name in
       ("LDRB", "LDRBT", "LDRSB", "LDRSBT", "LDREXB", "SWPB")},
    **{getattr(arm, f"ARM_INS_{name}"): 2 for name in
       ("LDRH", "LDRHT", "LDRSH", "LDRSHT", "LDREXH")},
    **{getattr(arm, f"ARM_INS_{name}"): 4 for name in ("LDR", "LDRT", "LDREX", "SWP")},
    **{getattr(arm, f"ARM_INS_{name}"): 8 for name in ("LDRD", "LDREXD")},
}
_BLOCK_READ_MODES = {
    arm.ARM_INS_LDM: "ia", arm.ARM_INS_LDMDA: "da", arm.ARM_INS_LDMDB: "db", arm.ARM_INS_LDMIB: "ib",
    arm.ARM_INS_VLDMIA: "ia", arm.ARM_INS_VLDMDB: "db", arm.ARM_INS_POP: "pop", arm.ARM_INS_VPOP: "pop",
}
_MEMORY_STORES_AND_HINTS = {
    getattr(arm, f"ARM_INS_{name}") for name in (
        "STR", "STRB", "STRBT", "STRD", "STRH", "STRHT", "STRT",
        "STREX", "STREXB", "STREXD", "STREXH", "STM", "STMDA", "STMDB", "STMIB",
        "VSTR", "VSTMIA", "VSTMDB", "PUSH", "VPUSH", "STC", "STCL", "STC2", "STC2L",
        "PLD", "PLDW", "PLI")
}
# Also catch implicit-register-list reads and new Capstone load classes whose
# width/address semantics have not been implemented. Operand access flags alone
# are insufficient (notably for atomic instructions).
_READ_CLASSES = {value for name, value in vars(arm).items() if name.startswith((
    "ARM_INS_LD", "ARM_INS_VLD", "ARM_INS_FLD", "ARM_INS_POP", "ARM_INS_VPOP",
    "ARM_INS_SWP", "ARM_INS_RFE"))}


_CALL_CLOBBERS = {
    arm.ARM_REG_R0, arm.ARM_REG_R1, arm.ARM_REG_R2, arm.ARM_REG_R3, arm.ARM_REG_R12, arm.ARM_REG_LR,
}
_RAW_SVC_CLOBBERS = _CALL_CLOBBERS | {
    *(getattr(arm, f"ARM_REG_R{register}") for register in range(4, 8)),
    arm.ARM_REG_CPSR, arm.ARM_REG_APSR, arm.ARM_REG_APSR_NZCV,
}
# Raw consecutive-register results, not the userland pointer-output signatures.
# Kernel_ABI supplies the raw rule and QueryMemory r0-r5; the pinned libctru
# wrappers corroborate the remaining marshalled outputs. All documented services
# still discard r0-r7/IP/LR/flags conservatively, not merely their listed results.
_RAW_SVC_SOURCES = [
    "https://www.3dbrew.org/w/index.php?title=Kernel_ABI&oldid=23691#System_calls",
    "https://www.3dbrew.org/wiki/SVC#System_calls",
    "https://github.com/devkitPro/libctru/blob/cb9d682f652e210824d5ccb3a94468dba207978e/libctru/source/svc.s",
]
_RAW_SVC_CONTRACTS = {
    0x01: ("ControlMemory", (0, 1)),
    0x02: ("QueryMemory", (0, 1, 2, 3, 4, 5)),
    0x03: ("ExitProcess", ()),  # Nonreturning; this does not extend CFG inference.
    0x08: ("CreateThread", (0, 1)),
    0x09: ("ExitThread", ()),
    0x0A: ("SleepThread", ()),
    0x14: ("ReleaseMutex", (0,)),
    0x17: ("CreateEvent", (0, 1)),
    0x18: ("SignalEvent", (0,)),
    0x19: ("ClearEvent", (0,)),
    0x1E: ("CreateMemoryBlock", (0, 1)),
    0x1F: ("MapMemoryBlock", (0,)),
    0x20: ("UnmapMemoryBlock", (0,)),
    0x21: ("CreateAddressArbiter", (0, 1)),
    0x22: ("ArbitrateAddress", (0,)),
    0x23: ("CloseHandle", (0,)),
    0x24: ("WaitSynchronization1", (0,)),
    0x25: ("WaitSynchronizationN", (0, 1)),
    0x27: ("DuplicateHandle", (0, 1)),
    0x28: ("GetSystemTick", (0, 1)),
    0x29: ("GetHandleInfo", (0, 1, 2)),
    0x2A: ("GetSystemInfo", (0, 1, 2)),
    0x2B: ("GetProcessInfo", (0, 1, 2)),
    0x2C: ("GetThreadInfo", (0, 1, 2)),
    0x2D: ("ConnectToPort", (0, 1)),
    0x32: ("SendSyncRequest", (0,)),
    0x35: ("GetProcessId", (0, 1)),
    0x3C: ("Break", None),  # Debugger-dependent continuation; no complete output contract.
}


def _raw_svc_service(instruction) -> int | None:
    operands = instruction.operands
    return (operands[0].imm if len(operands) == 1 and operands[0].type == arm.ARM_OP_IMM
            and 0 <= operands[0].imm <= 0xFF else None)


def _raw_svc_writes(instruction) -> set[int] | None:
    contract = _RAW_SVC_CONTRACTS.get(_raw_svc_service(instruction))
    if contract is None or contract[1] is None:
        # Unknown service/outputs: refuse all surviving address facts, including
        # callee-saved ones. An ordinary-call convention cannot fill this gap.
        return None
    return _RAW_SVC_CLOBBERS | {getattr(arm, f"ARM_REG_R{register}") for register in contract[1]}

_SINGLE_DEST_WRITES = {getattr(arm, f"ARM_INS_{name}") for name in (
    "ADC", "ADD", "AND", "ASR", "BIC", "CLZ", "EOR", "LSL", "LSR", "MLA", "MOV", "MOVT", "MOVW",
    "MUL", "MVN", "ORR", "ROR", "RRX", "RSB", "RSC", "SBC", "SUB", "REV", "REV16", "REVSH",
    "SXTB", "SXTB16", "SXTH", "UXTB", "UXTB16", "UXTH", "PKHBT", "PKHTB", "SSAT", "SSAT16", "USAT", "USAT16")}
_DOUBLE_DEST_WRITES = {getattr(arm, f"ARM_INS_{name}") for name in (
    "UMULL", "SMULL", "UMLAL", "SMLAL", "UMAAL", "SMLALD", "SMLSLD", "LDRD", "LDREXD")}
_COPROCESSOR_CORE_READS = {getattr(arm, f"ARM_INS_{name}") for name in ("MRC", "MRC2", "MRRC", "MRRC2", "MRS")}
_EXCLUSIVE_STATUS_WRITES = {getattr(arm, f"ARM_INS_{name}") for name in ("STREX", "STREXB", "STREXD", "STREXH")}
_NO_CORE_DESTINATION = _MEMORY_STORES_AND_HINTS | {
    arm.ARM_INS_CMP, arm.ARM_INS_CMN, arm.ARM_INS_TST, arm.ARM_INS_TEQ,
    arm.ARM_INS_B, arm.ARM_INS_BX, arm.ARM_INS_NOP,
}


def _register_writes(instruction, *, call: bool = False) -> set[int] | None:
    """Invalidate decoded explicit/implicit writes, with ISA fallbacks for omissions.

    None means an opaque effect with no trustworthy destination inventory: discard
    every fact. An unknown instruction with explicit register operands discards all
    of those operands, never assumes that an omitted WRITE flag means read-only.
    """
    operands = instruction.operands
    registers = [operand.reg for operand in operands if operand.type == arm.ARM_OP_REG]
    writes = set(instruction.regs_write) | set(instruction.regs_access()[1])
    writes.update(operand.reg for operand in operands if operand.type == arm.ARM_OP_REG and operand.access & CS_AC_WRITE)
    if instruction.id == arm.ARM_INS_SVC:
        effects = _raw_svc_writes(instruction)
        if effects is None:
            return None
        writes.update(effects)
    elif call:
        writes.update(_CALL_CLOBBERS)
    elif instruction.id in _BLOCK_READ_MODES:
        minimum = 1 if _BLOCK_READ_MODES[instruction.id] == "pop" else 2
        if len(registers) < minimum or any(operand.type != arm.ARM_OP_REG for operand in operands):
            return None
        writes.update(registers if _BLOCK_READ_MODES[instruction.id] == "pop" else registers[1:])
    elif instruction.id in _DOUBLE_DEST_WRITES:
        if len(operands) < 2 or any(operand.type != arm.ARM_OP_REG for operand in operands[:2]):
            return None
        writes.update(registers[:2])
    elif instruction.id in _COPROCESSOR_CORE_READS:
        # MRC/MRRC destination access flags can incorrectly say READ or zero.
        minimum = 2 if instruction.id in (arm.ARM_INS_MRRC, arm.ARM_INS_MRRC2) else 1
        if len(registers) < minimum:
            return None
        writes.update(registers)
    elif (instruction.id in _SINGLE_DEST_WRITES or instruction.id in _SCALAR_READ_WIDTHS
          or instruction.id == arm.ARM_INS_VLDR or instruction.id in _EXCLUSIVE_STATUS_WRITES):
        if not operands or operands[0].type != arm.ARM_OP_REG:
            return None
        writes.add(operands[0].reg)
    elif instruction.id not in _NO_CORE_DESTINATION:
        if not registers and not writes:
            return None
        writes.update(registers)
    if instruction.id in (arm.ARM_INS_PUSH, arm.ARM_INS_POP, arm.ARM_INS_VPUSH, arm.ARM_INS_VPOP):
        writes.add(arm.ARM_REG_SP)
    if instruction.writeback:
        memory = [operand.mem.base for operand in operands if operand.type == arm.ARM_OP_MEM]
        writes.update(memory or registers[:1])
    return writes


def _instruction_reads(instruction, before: dict, site: int, pc: int) -> tuple[list[tuple], bool]:
    """Return exact static read spans, or an uncertainty veto, never guessed data."""
    operands = instruction.operands
    width = _SCALAR_READ_WIDTHS.get(instruction.id)
    mode = _BLOCK_READ_MODES.get(instruction.id)
    memory_operands = [operand for operand in operands if operand.type == arm.ARM_OP_MEM]
    if instruction.id == arm.ARM_INS_VLDR:
        if not operands or operands[0].type != arm.ARM_OP_REG:
            return [], True
        width = 8 if arm.ARM_REG_D0 <= operands[0].reg <= arm.ARM_REG_D31 else 4
    if width is None and mode is None:
        return [], (instruction.id in _READ_CLASSES
                    or bool(memory_operands) and instruction.id not in _MEMORY_STORES_AND_HINTS)
    if mode is not None:
        if not operands or any(operand.type != arm.ARM_OP_REG for operand in operands):
            return [], True
        base_register = arm.ARM_REG_SP if mode == "pop" else operands[0].reg
        loaded = operands if mode == "pop" else operands[1:]
        if not loaded or base_register == arm.ARM_REG_PC:
            return [], True
        width = sum(8 if arm.ARM_REG_D0 <= operand.reg <= arm.ARM_REG_D31 else 4 for operand in loaded)
        displacement = {"ia": 0, "pop": 0, "ib": 4, "da": 4 - width, "db": -width}[mode]
    else:
        if len(memory_operands) != 1:
            return [], True
        operand = memory_operands[0]
        memory = operand.mem
        base_register, displacement = memory.base, memory.disp
        if memory.index:
            if memory.index not in before:
                # A statically formed base with an unbounded indexed access may
                # reach an alignment candidate. It supplies no ownership bound.
                return [], base_register == arm.ARM_REG_PC or base_register in before
            value = before[memory.index][0] & 0xFFFFFFFF
            shift = operand.shift
            if shift.type == arm.ARM_SFT_LSL:
                value = (value << shift.value) & 0xFFFFFFFF
            elif shift.type == arm.ARM_SFT_LSR:
                value >>= shift.value
            elif shift.type == arm.ARM_SFT_ASR:
                value = ((value if value < 0x80000000 else value - 0x100000000) >> shift.value) & 0xFFFFFFFF
            elif shift.type == arm.ARM_SFT_ROR:
                amount = shift.value % 32
                value = ((value >> amount) | (value << ((32 - amount) % 32))) & 0xFFFFFFFF
            elif shift.type:
                return [], True  # Register shifts/RRX need additional value/flag proofs.
            displacement += -value if operand.subtracted else value
    if base_register == arm.ARM_REG_PC:
        base, origin, basis = pc, site, "architectural_pc_load"
    elif base_register in before:
        base, origin, basis = before[base_register]
    else:
        return [], False  # No statically recovered image address, not an invented reference.
    first = (base + displacement) & 0xFFFFFFFF
    if first + width > 0x100000000:
        return [], True
    return [(first, first + width, origin, basis)], False


_ENDIAN_DP_OPS = {getattr(arm, f"ARM_INS_{name}"): (opcode,) for opcode, name in enumerate(
    ("AND", "EOR", "SUB", "RSB", "ADD", "ADC", "SBC", "RSC", "TST", "TEQ", "CMP", "CMN", "ORR", "MOV", "BIC", "MVN"))}
_ENDIAN_DP_OPS.update({getattr(arm, f"ARM_INS_{name}"): (13,) for name in ("ASR", "LSL", "LSR", "ROR", "RRX")})
_ENDIAN_DP_OPS[arm.ARM_INS_ADR] = (2, 4)
_ENDIAN_SINGLE_MEMORY = {getattr(arm, f"ARM_INS_{name}"): (load, byte) for name, load, byte in (
    ("LDR", 1, 0), ("LDRT", 1, 0), ("LDRB", 1, 1), ("LDRBT", 1, 1),
    ("STR", 0, 0), ("STRT", 0, 0), ("STRB", 0, 1), ("STRBT", 0, 1))}
_ENDIAN_EXTRA_MEMORY = {getattr(arm, f"ARM_INS_{name}"): (load, operation) for name, load, operation in (
    ("LDRH", 1, 1), ("LDRHT", 1, 1), ("STRH", 0, 1), ("STRHT", 0, 1),
    ("LDRSB", 1, 2), ("LDRSBT", 1, 2), ("LDRSH", 1, 3), ("LDRSHT", 1, 3),
    ("LDRD", 0, 2), ("STRD", 0, 3))}
_ENDIAN_MULTI_MEMORY = {getattr(arm, f"ARM_INS_{name}") for name in (
    "PUSH", "POP", "LDM", "LDMDA", "LDMDB", "LDMIB", "STM", "STMDA", "STMDB", "STMIB")}
_ENDIAN_MULTIPLY = {getattr(arm, f"ARM_INS_{name}") for name in ("MUL", "MLA", "UMULL", "UMLAL", "SMULL", "SMLAL")}
_ENDIAN_VFP_DATA = {getattr(arm, f"ARM_INS_{name}") for name in (
    "VABS", "VADD", "VCMP", "VCMPE", "VCVT", "VCVTR", "VDIV", "VMLA", "VMLS", "VMUL", "VNEG",
    "VNMLA", "VNMLS", "VNMUL", "VSQRT", "VSUB")}
_ENDIAN_VFP_MEMORY = {getattr(arm, f"ARM_INS_{name}") for name in (
    "VLDR", "VSTR", "VPUSH", "VPOP", "VLDMIA", "VLDMDB", "VSTMIA", "VSTMDB")}
_ENDIAN_THUMB16_IDS = set(_ENDIAN_DP_OPS) | set(_ENDIAN_SINGLE_MEMORY) | set(_ENDIAN_EXTRA_MEMORY) | (
    _ENDIAN_MULTI_MEMORY | {getattr(arm, f"ARM_INS_{name}") for name in (
        "MUL", "NEG", "REV", "REV16", "REVSH", "SXTB", "SXTH", "UXTB", "UXTH", "B", "BX", "BLX", "CBZ", "CBNZ", "NOP")})


def _data_endian_effect(instruction, isa: str) -> int | None:
    """0 preserves E, 1 establishes LE, 2 establishes BE; None refuses the owner.

    This is deliberately narrower than a processor model. Validate ordinary
    encodings independently of decoder writes/groups, including PC/CPSR restores.
    Unknown classes, status/coprocessor writes, raw SVC and Thumb IT are refusals,
    even when unreachable or apparently followed by an endian restoration.
    """
    raw = instruction.bytes
    if isa == "thumb":
        if len(raw) == 2:
            half = int.from_bytes(raw, "little")
            if instruction.id == arm.ARM_INS_SETEND and half & ~8 == 0xB650:
                return 2 if half & 8 else 1
            if instruction.id not in _ENDIAN_THUMB16_IDS:
                return None
            ordinary = (half < 0xB000 or half & 0xF000 == 0xC000
                        or half & 0xFE00 in (0xB400, 0xBC00)
                        or half & 0xF000 == 0xD000 and (half >> 8) & 15 < 14
                        or half & 0xF800 == 0xE000 or half & 0xFF00 in (0xB000, 0xB200, 0xBA00)
                        or half & 0xF500 == 0xB100 or half == 0xBF00)
            if not ordinary:
                return None
            if (instruction.id in (arm.ARM_INS_ADD, arm.ARM_INS_MOV) and instruction.operands
                    and instruction.operands[0].reg == arm.ARM_REG_PC
                    and not (instruction.id == arm.ARM_INS_MOV and len(instruction.operands) == 2
                             and instruction.operands[1].reg == arm.ARM_REG_LR)):
                return None
            return 0
        if len(raw) == 4 and instruction.id in (arm.ARM_INS_BL, arm.ARM_INS_BLX):
            first, second = int.from_bytes(raw[:2], "little"), int.from_bytes(raw[2:], "little")
            return 0 if first & 0xF800 == 0xF000 and second & 0xC000 == 0xC000 else None
        return None
    if isa != "arm" or len(raw) != 4:
        return None
    word = int.from_bytes(raw, "little")
    condition, rn, rd = word >> 28, (word >> 16) & 15, (word >> 12) & 15
    if instruction.id == arm.ARM_INS_SETEND:
        return (2 if word & 0x200 else 1) if word & ~0x200 == 0xF1010000 else None
    if instruction.id in (arm.ARM_INS_B, arm.ARM_INS_BL, arm.ARM_INS_BLX):
        if word & 0x0E000000 == 0x0A000000:
            valid = ((condition == 15) == (instruction.id == arm.ARM_INS_BLX)
                     and (condition == 15 or bool(word & 0x01000000) == (instruction.id == arm.ARM_INS_BL)))
            return 0 if valid else None
        return 0 if (instruction.id == arm.ARM_INS_BLX and condition != 15
                     and word & 0x0FFFFFF0 == 0x012FFF30 and word & 15 != 15) else None
    if condition == 15:
        return None
    if instruction.id == arm.ARM_INS_BX:
        return 0 if word & 0x0FFFFFFF == 0x012FFF1E else None
    if instruction.id in _ENDIAN_DP_OPS:
        opcode, flags = (word >> 21) & 15, bool(word & 0x00100000)
        if (word & 0x0C000000 or opcode not in _ENDIAN_DP_OPS[instruction.id]
                or not word & 0x02000000 and word & 0x90 == 0x90):
            return None
        if 8 <= opcode <= 11:
            return 0 if flags and rd == 0 else None
        return 0 if rd != 15 or word & 0x0FFFFFFF == 0x01A0F00E else None
    if instruction.id in (arm.ARM_INS_MOVW, arm.ARM_INS_MOVT):
        return 0 if word & 0x0FF00000 == (0x03000000 if instruction.id == arm.ARM_INS_MOVW else 0x03400000) and rd != 15 else None
    if instruction.id == arm.ARM_INS_MRS:
        return 0 if word & 0x0FBF0FFF == 0x010F0000 and rd != 15 else None
    if instruction.id in _ENDIAN_SINGLE_MEMORY or instruction.id in _ENDIAN_EXTRA_MEMORY:
        if instruction.id in _ENDIAN_SINGLE_MEMORY:
            valid = (word & 0x0C000000 == 0x04000000
                     and ((word >> 20) & 1, (word >> 22) & 1) == _ENDIAN_SINGLE_MEMORY[instruction.id]
                     and not (word & 0x02000000 and word & 0x10))
        else:
            valid = (word & 0x0E000090 == 0x00000090
                     and ((word >> 20) & 1, (word >> 5) & 3) == _ENDIAN_EXTRA_MEMORY[instruction.id]
                     and (word & 0x00400000 or not word & 0xF00))
        double = instruction.id in (arm.ARM_INS_LDRD, arm.ARM_INS_STRD)
        if not valid or double and (rd & 1 or rd > 12):
            return None
        if bool(word & 0x00100000) and rd == 15:
            return None  # A computed transfer cannot borrow a lexical E-state proof.
        if (not word & 0x01000000 or word & 0x00200000) and (rn == 15 or rn in ({rd, rd + 1} if double else {rd})):
            return None
        return 0
    if instruction.id in _ENDIAN_MULTI_MEMORY:
        if (word & 0x0E400000 != 0x08000000 or not word & 0xFFFF or rn == 15
                or word & 0x00200000 and word & (1 << rn)):
            return None  # The S bit includes CPSR-restoring LDM, regardless of decoder effects.
        if word & 0x00108000 == 0x00108000 and word & 0x0FFF8000 != 0x08BD8000:
            return None
        return 0
    if instruction.id in _ENDIAN_MULTIPLY:
        if instruction.id in (arm.ARM_INS_MUL, arm.ARM_INS_MLA):
            valid = (word & 0x0FC000F0 == 0x00000090
                     and bool(word & 0x00200000) == (instruction.id == arm.ARM_INS_MLA) and rn not in (13, 15))
        else:
            valid = word & 0x0F8000F0 == 0x00800090 and rn != rd and rn not in (13, 15) and rd not in (13, 15)
        return 0 if valid and word & 15 not in (13, 15) and (word >> 8) & 15 not in (13, 15) else None
    if instruction.id in _ENDIAN_VFP_DATA:
        return 0 if word & 0x0F000E10 == 0x0E000A00 else None
    if instruction.id in _ENDIAN_VFP_MEMORY:
        if word & 0x0E000E00 != 0x0C000A00:
            return None
        pre, up, writeback, load = ((word >> bit) & 1 for bit in (24, 23, 21, 20))
        if instruction.id in (arm.ARM_INS_VLDR, arm.ARM_INS_VSTR):
            return 0 if pre and not writeback and load == (instruction.id == arm.ARM_INS_VLDR) else None
        decrement = instruction.id in (arm.ARM_INS_VPUSH, arm.ARM_INS_VLDMDB, arm.ARM_INS_VSTMDB)
        loading = instruction.id in (arm.ARM_INS_VPOP, arm.ARM_INS_VLDMIA, arm.ARM_INS_VLDMDB)
        if (pre != decrement or up == decrement or load != loading or rn == 15
                or decrement and not writeback
                or instruction.id in (arm.ARM_INS_VPUSH, arm.ARM_INS_VPOP) and (rn != 13 or not writeback)):
            return None
        count, double = word & 0xFF, bool(word & 0x100)
        first = rd + (((word >> 22) & 1) << 4) if double else rd * 2 + ((word >> 22) & 1)
        return 0 if count and not (double and count & 1) and first + (count // 2 if double else count) <= (16 if double else 32) else None
    if instruction.id in (arm.ARM_INS_VMRS, arm.ARM_INS_FMSTAT):
        return 0 if word & 0x0FFF0FFF == 0x0EF10A10 else None  # FPSCR -> core/NZCV, not CPSR.E.
    if instruction.id == arm.ARM_INS_VMOV:
        if word & 0x0F000E10 == 0x0E000A00:
            return 0
        if word & 0x0FE00F7F == 0x0E000A10 and rd not in (13, 15):
            return 0
        if word & 0x0FE00EF0 == 0x0C400A10 and rn != rd and rn not in (13, 15) and rd not in (13, 15):
            return 0
        return None
    if instruction.id in (arm.ARM_INS_NOP, arm.ARM_INS_HINT):
        return 0 if word & 0x0FFFFFFF == 0x0320F000 else None
    return None


def _decoded_functions(catalogue: dict, analyzer) -> list[tuple[dict, dict]]:
    functions = []
    for function in catalogue["functions"]:
        decoded = {}
        for first, end in function["code_ranges"]:
            cursor = first
            while cursor < end:
                instruction = (analyzer.detailed(cursor) if function["isa"] == "arm" else next(
                    analyzer.thumb.disasm(analyzer.text[cursor - analyzer.base:cursor - analyzer.base + 4], cursor, 1), None))
                if instruction is None or cursor + instruction.size > end:
                    break
                decoded[cursor] = instruction
                cursor += instruction.size
        functions.append((function, decoded))
    return functions


def _little_endian_sites(functions: list[tuple[dict, dict]], analyzer) -> set[int]:
    """All-path LE before a reachable instruction; values still do not cross joins.

    Only primary public entries have the target's LE ABI assumption. Secondary
    labels start unknown, and actual direct edges (including back/cross-owner
    edges) join that assumption, never reset it. Ordinary calls preserve LE only
    when it was established at their entry; an unknown/BE call cannot restore it.
    No switch/computed/exception control-flow or opaque processor effect is proved.
    """
    nodes = {site: (function, instruction) for function, decoded in functions for site, instruction in decoded.items()}
    effects, flows, successors, called = {}, {}, {}, {}
    opaque = {function["address"] for function, decoded in functions
              if sum(instruction.size for instruction in decoded.values())
              != sum(end - first for first, end in function["code_ranges"])}
    for site, (function, instruction) in nodes.items():
        effect = _data_endian_effect(instruction, function["isa"])
        flow = analyzer.decode(site, function["isa"])
        effects[site], flows[site] = effect, flow
        successors[site] = []
        if effect is None or flow is None or flow.switch is not None or flow.indirect and not flow.call:
            opaque.add(function["address"])
        if flow is None:
            continue
        if flow.target and (flow.branch or flow.call or flow.stop) and flow.target[0] in nodes:
            target = flow.target[0]
            if nodes[target][0]["isa"] != flow.target[1]:
                opaque.add(function["address"])
            elif flow.call:
                called[site] = target
            else:
                successors[site].append(target)
        if function["kind"] == "veneer" and site == function["address"]:
            target = analyzer.veneer(function["address"], function["isa"])
            if target is not None and target[0] in nodes:
                # Do not lose a possible changed-state ingress merely because
                # the footprint reader skips veneer owners. Opaque state flows
                # to its known destination as a refusal, never a value proof.
                successors[site].append(target[0])
        following = site + instruction.size
        if (flow.call or not (flow.branch or flow.stop) or flow.conditional) and following in nodes:
            successors[site].append(following)
    states, work = {}, []

    def merge(site: int, state: int) -> None:
        if site not in nodes:
            return
        state = 3 if nodes[site][0]["address"] in opaque else state
        combined = states.get(site, 0) | state
        if combined != states.get(site, 0):
            states[site] = combined
            work.append(site)

    for function, _ in functions:
        merge(function["address"], 1)
        for entry in function["entries"]:
            if entry["address"] != function["address"]:
                merge(entry["address"], 3)
    for site, target in called.items():
        if target != nodes[target][0]["address"]:
            merge(target, 3)  # A secondary public ingress is unknown even before entry recording.
    for site, targets in successors.items():
        for target in targets:
            if (target != nodes[target][0]["address"]
                    and nodes[site][0]["address"] != nodes[target][0]["address"]):
                merge(target, 3)
    while work:
        site = work.pop()
        function, instruction = nodes[site]
        before, effect, flow = states[site], effects[site], flows[site]
        after = before if effect == 0 else effect or 3
        if flow is not None and flow.conditional:
            after |= before
        if function["address"] in opaque or flow is not None and flow.call and before != 1:
            after = 3
        for target in successors[site]:
            merge(target, after)
        if site in called:
            merge(called[site], before)
    return {site for site, state in states.items() if state == 1}


def read_footprints(catalogue: dict, analyzer, *, address_witnesses: list[dict] | None = None,
                    include_reads: bool = True, read_vetoes: list[dict] | None = None,
                    raw_svc_sites: list[dict] | None = None) -> list[dict]:
    """Enumerate actual fixed-width reads from locally proved image addresses.

    ADR alone is not a read. Values do not flow through a join, conditional
    definition, unknown arithmetic or a call-clobbered register. Pool values
    require all-path LE at the reachable load, in ARM and Thumb alike; a later
    restoration cannot repair a previously unproved value. Only a later actual
    read contributes the pointee's exact span. Unmodeled reads, unbounded static
    addresses and conditional reads can veto alignment, never create ownership.
    No payload or pointer value is serialized.
    """
    rows = []
    functions = _decoded_functions(catalogue, analyzer)
    little_endian = _little_endian_sites(functions, analyzer)
    incoming_targets = {ins.target[0] for function, decoded in functions for site in decoded
                        for ins in [analyzer.decode(site, function["isa"])] if ins and ins.target}
    for function, decoded in functions:
        if function["kind"] == "veneer":
            continue
        isa = function["isa"]
        joins = {entry["address"] for entry in function["entries"]} | incoming_targets
        known = {}
        previous_end = None
        for site, instruction in decoded.items():
            if site != previous_end or site in joins:
                known.clear()
            previous_end = site + instruction.size
            ins = analyzer.decode(site, isa)
            operands = instruction.operands
            writes = _register_writes(instruction, call=ins.call)
            if raw_svc_sites is not None and instruction.id == arm.ARM_INS_SVC:
                service = _raw_svc_service(instruction)
                contract = _RAW_SVC_CONTRACTS.get(service)
                raw_svc_sites.append({
                    "function": function["address"], "site": site, "isa": isa,
                    "service": service, "conditional": ins.conditional,
                    "name": contract[0] if contract else None,
                    "documented_outputs": list(contract[1]) if contract and contract[1] is not None else None,
                    "invalidation": "all_address_facts" if writes is None else "r0-r7_ip_lr_flags",
                })
            before = dict(known)
            if (read_vetoes is not None and instruction.id == arm.ARM_INS_LDR
                    and ins.pools and site not in little_endian):
                read_vetoes.append({"function": function["address"], "site": site,
                                   "start": analyzer.base, "end": analyzer.end,
                                   "evidence": "unproved_data_endian_pool_value_veto_only"})
            if writes is None:
                known.clear()
            else:
                for register in writes:
                    known.pop(register, None)
            if include_reads:
                pc = site + 8 if isa == "arm" else (site + 4) & ~3
                reads, uncertain = _instruction_reads(instruction, before, site, pc)
                if uncertain and read_vetoes is not None:
                    read_vetoes.append({"function": function["address"], "site": site,
                                       "start": analyzer.base, "end": analyzer.end,
                                       "evidence": "unmodeled_or_unbounded_static_read_veto_only"})
                for first, end, source_site, address_source in reads:
                    if analyzer.base <= first < end <= analyzer.end:
                        row = {"function": function["address"], "site": site, "start": first, "end": end,
                               "address_source_site": source_site, "address_source": address_source,
                               "evidence": "exact_architectural_read_footprint"}
                        if ins.conditional:
                            if read_vetoes is not None:
                                read_vetoes.append({**row, "evidence": "conditional_read_veto_only"})
                        else:
                            rows.append(row)
                    elif read_vetoes is not None and first < analyzer.end and analyzer.base < end:
                        read_vetoes.append({"function": function["address"], "site": site,
                                           "start": max(first, analyzer.base), "end": min(end, analyzer.end),
                                           "evidence": "partially_mapped_read_veto_only"})
            if ins.conditional:
                continue
            if operands and operands[0].type == arm.ARM_OP_REG:
                destination = operands[0].reg
                if ins.address_target is not None:
                    known[destination] = (ins.address_target, site, "pc_relative_address")
                elif ins.pc_add_register is not None and ins.pc_add_register in before:
                    value, origin, _ = before[ins.pc_add_register]
                    known[destination] = ((site + 4 + value) & 0xFFFFFFFF, origin, "constant_plus_thumb_pc")
                elif instruction.id in (arm.ARM_INS_MOV, arm.ARM_INS_MOVW) and ins.register_constant is not None:
                    register, value = ins.register_constant
                    known[register] = (value, site, "literal_or_immediate")
                elif (instruction.id == arm.ARM_INS_LDR and ins.pools and site in little_endian
                      and len(operands) == 2 and operands[1].type == arm.ARM_OP_MEM
                      and operands[1].mem.base == arm.ARM_REG_PC and not operands[1].mem.index
                      and not instruction.writeback):
                    first, end = ins.pools[0]
                    pc = site + 8 if isa == "arm" else (site + 4) & ~3
                    if (end - first == 4 and first == pc + operands[1].mem.disp and first % 4 == 0
                            and analyzer.base <= first <= analyzer.end - 4):
                        basis = "literal_or_immediate" if isa == "thumb" else "loaded_pool_pointer"
                        known[destination] = (analyzer.word(first), site, basis)
                elif (instruction.id == arm.ARM_INS_MOV and len(operands) == 2 and operands[1].type == arm.ARM_OP_REG
                      and not any(operand.shift.type for operand in operands)):
                    if operands[1].reg in before:
                        known[destination] = before[operands[1].reg]
                elif (instruction.id in (arm.ARM_INS_ADD, arm.ARM_INS_SUB) and len(operands) == 3
                      and operands[1].type == arm.ARM_OP_REG and operands[1].reg in before
                      and operands[2].type == arm.ARM_OP_IMM and not any(op.shift.type for op in operands)):
                    value, origin, basis = before[operands[1].reg]
                    known[destination] = (value + operands[2].imm * (-1 if instruction.id == arm.ARM_INS_SUB else 1), origin, basis)
                if address_witnesses is not None and isa == "thumb" and (ins.address_target is not None or ins.pc_add_register is not None):
                    formed = known.get(destination)
                    if formed is not None and formed[0] & 1 and analyzer.inside(formed[0] & ~1, "thumb"):
                        address_witnesses.append({"function": function["address"], "site": site,
                                                  "target": formed[0] & ~1, "isa": "thumb",
                                                  "definition_site": formed[1], "evidence": "dominated_tagged_pc_relative_value"})
            if not ins.call and (ins.branch or ins.stop):
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
    read_vetoes = []
    raw_svc_sites = []
    footprints = read_footprints(catalogue, analyzer, read_vetoes=read_vetoes, raw_svc_sites=raw_svc_sites)
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
                or any(row["start"] < end and first < row["end"] for row in footprints)
                or any(row["start"] < end and first < row["end"] for row in read_vetoes)):
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
                                   "read_vetoes": read_vetoes, "owner_merges": owner_merges,
                                   "alignment": alignment, "extent_explanations": explained,
                                   "raw_svc_sites": raw_svc_sites}
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


def _validate_library_evidence(records: list[dict], library: dict, image_sha256: str) -> None:
    """Check frozen identity/alias contracts, not reinterpret a byte coincidence."""
    owners = {row["address"]: row for row in library["authenticated_owners"]}
    if len(owners) != len(library["authenticated_owners"]):
        raise ValueError("duplicate authenticated library owner")
    if any(not row["authenticated"] or not row["whole_body_identical"] for row in owners.values()):
        raise ValueError("unresolved byte coincidences are not authenticated library owners")
    flags = library["protected_collisions"]
    if any(row["authenticated"] for row in flags):
        raise ValueError("protected library coincidences must remain flag-only")
    flagged = {row["address"] for row in flags}
    if owners.keys() & flagged:
        raise ValueError("protected C coincidences cannot supply authenticated identities or proposed names")
    if len({row["address"] for row in records}) != len(records):
        raise ValueError("duplicate library classification")
    for record in records:
        owner = owners.get(record["address"])
        witness = record.get("library_witness", {})
        digest = record.get("original_extent_sha256")
        if record["address"] in flagged:
            raise ValueError("protected C coincidence cannot become a library classification")
        if (record.get("kind") != "library" or record.get("basis") != "known_library"
                or record.get("image_sha256") != image_sha256 or owner is None
                or not owner["authenticated"] or not owner["whole_body_identical"]
                or (owner["size"], owner["isa"]) != (record["size"], record["isa"])
                or not isinstance(digest, str) or len(digest) != 64
                or witness.get("image_body_sha256") != digest):
            raise ValueError("library proposal lacks a complete image/extent/ISA-bound authenticated identity")
        aliases = {name for group in owner["selected_alias_groups"] for name in group}
        if witness.get("symbol") not in aliases:
            raise ValueError("library witness symbol has no same-object alias evidence")
    for name, binding in library["names_proposal"]["symbols"].items():
        owner = owners.get(binding["address"])
        if (owner is None or not owner["authenticated"] or not owner["whole_body_identical"]
                or name not in {alias for group in owner["selected_alias_groups"] for alias in group}):
            raise ValueError("proposed library name has no authenticated same-object definition")


def _validate_declared_extents(records: list[dict], extension: dict) -> None:
    """Admit only unchanged declared bodies, never their foreign section tails."""
    by_address = {row["address"]: row for row in records}
    adjudications = extension["extent_adjudication"]["adjudications"]
    if extension["extent_adjudication"]["image_sha256"] != extension["image_sha256"]:
        raise ValueError("declared-body adjudication belongs to a different image")
    if ({row["address"] for row in adjudications} != set(extension["pool_qualified_record_addresses"])
            or len({row["address"] for row in adjudications}) != len(adjudications)):
        raise ValueError("pool-qualified declarations require unique exact extent adjudications")
    for row in adjudications:
        record = by_address.get(row["address"])
        if (record is None or any(row[key] != record["size"] for key in (
                "old_catalogue_size", "recommended_catalogue_size", "elf_declared_size"))
                or row["isa"] != record["isa"] or row["image_body_sha256"] != record["original_extent_sha256"]
                or row["symbol"] != record["library_witness"]["symbol"]
                or row["elf_symbol_kind"] != "STT_FUNC"
                or not row["whole_declared_body_masked_equal"] or not row["whole_declared_body_reencoded_equal"]
                or row["rejected_growth_size"] <= record["size"]
                or row["rejected_growth_delta"] != row["rejected_growth_size"] - record["size"]):
            raise ValueError("pool-qualified identity cannot grow beyond its unchanged complete declaration")
        end = row["address"] + record["size"]
        for reader in row["all_foreign_tail_readers"]:
            if reader.get("target_start", reader.get("target", -1)) < end:
                raise ValueError("foreign tail reference does not establish outside-only ownership")
    unresolved = {row["address"] for row in extension["scope_and_dispositions"] if not row["r7_authenticated"]}
    if unresolved & by_address.keys():
        raise ValueError("unresolved runtime identities must retain ordinary C eligibility")


def _metadata_sha(value: dict | list) -> str:
    return hashlib.sha256((json.dumps(value, indent=2, sort_keys=True, ensure_ascii=False) + "\n").encode()).hexdigest()


def load_protection_inputs(root: Path, parent_path: Path, runtime: dict) -> tuple[dict, dict[str, str]]:
    """Bind the successor's exact metadata transitions and the live prepared ledger."""
    path = root / "research/catalogue_v11_r2_evidence.json"
    payload = path.read_bytes()
    revision = json.loads(payload)
    parent = revision["parent_evidence"]
    literal = revision["literal_evidence"]
    if (root / parent["path"] != parent_path
            or hashlib.sha256(parent_path.read_bytes()).hexdigest() != parent["sha256"]
            or revision["image_sha256"] != runtime["image_sha256"]
            or revision["revision"] != "catalogue-v11-r2" or revision["new_C_credit"] != 0
            or revision["prepared_protection"] != runtime.get("prepared_protection")):
        raise ValueError("v11-r2 protection evidence differs from its immutable parent/image")
    inputs = {str(path.relative_to(root)): hashlib.sha256(payload).hexdigest()}
    literal_path = root / literal["path"]
    if (literal_path != root / "research/catalogue_v10_evidence.json"
            or hashlib.sha256(literal_path.read_bytes()).hexdigest() != literal["sha256"]):
        raise ValueError("v11-r2 literal evidence differs from its frozen input")
    inputs[literal["path"]] = literal["sha256"]
    snapshot = runtime.get("_prepared_protection")
    if snapshot is not None:
        identities = snapshot["identities"]
        if (len(identities) != snapshot["identity_count"]
                or len({row["address"] for row in identities}) != len(identities)):
            raise ValueError("v11-r2 prepared identity snapshot has duplicate or missing identities")
        for field in ("adopted_catalogue", "ledger", "names", "prepare", "selection"):
            descriptor = snapshot[field]
            content = (root / descriptor["path"]).read_bytes()
            if len(content) != descriptor["size"] or hashlib.sha256(content).hexdigest() != descriptor["sha256"]:
                raise ValueError(f"v11-r2 prepared {field} differs from its frozen input")
            inputs[descriptor["path"]] = descriptor["sha256"]
        ledger = json.loads((root / snapshot["ledger"]["path"]).read_bytes())
        names = json.loads((root / snapshot["names"]["path"]).read_bytes())["symbols"]
        claims = [(name, (names[name]["address"] if name in names else int(name.removeprefix("func_"), 16)) & ~1,
                   unit["source"], unit["profile"]) for unit in ledger["units"] for name in unit["functions"]]
        if (len(claims) != len(identities) or set(claims) != {
                (row["name"], row["address"], row["source"], row["profile"]) for row in identities}):
            raise ValueError("v11-r2 prepared snapshot does not describe the complete ledger")
        changed = {row["owner"]: row for row in identities if row["previous_metadata_changed"]}
        transitions = revision["protected_record_transitions"]
        if (len(transitions) != len(changed) or {row["address"] for row in transitions} != changed.keys()
                or any((row["before_sha256"], row["after_sha256"]) != (
                    changed[row["address"]]["adopted_record_sha256"],
                    changed[row["address"]]["proposal_record_sha256"]) for row in transitions)):
            raise ValueError("v11-r2 transitions do not bind every exact inherited metadata repair")
    for row in revision["protected_record_transitions"]:
        descriptor = row["source"]
        content = (root / descriptor["path"]).read_bytes()
        if hashlib.sha256(content).hexdigest() != descriptor["sha256"]:
            raise ValueError("v11-r2 metadata repair source differs from its retained compatibility proof")
        inputs[descriptor["path"]] = descriptor["sha256"]
    return revision, inputs


def resolve_protected_records(protected: dict[int, dict], analyzer, evidence: dict,
                              literal_encodings: list[dict]) -> tuple[dict[int, dict], list[dict]]:
    """Derive only approved inline-literal repairs before any complete-record guard.

    Never import proposed records into the recovered catalogue. Old/new complete
    records are comparison witnesses; the expected successor is independently
    derived from the old record and exact original typed-object bounds. Discovery,
    partitioning, classifications and the final check must reproduce it themselves.
    """
    revision = evidence.get("protection_revision")
    if revision is None:
        return dict(protected), []
    if hashlib.sha256(analyzer.image).hexdigest() != revision["image_sha256"]:
        raise ValueError("protected metadata transition belongs to another image")
    resolved, receipts, seen = dict(protected), [], set()
    for row in revision["protected_record_transitions"]:
        address, before, after = row["address"], row["before"], row["after"]
        if (address in seen or before["address"] != address or before["kind"] != "function"
                or _metadata_sha(before) != row["before_sha256"] or _metadata_sha(after) != row["after_sha256"]
                or before == after or {key: value for key, value in before.items() if key != "literals"}
                != {key: value for key, value in after.items() if key != "literals"}):
            raise ValueError("protected metadata transition changes an unapproved complete record")
        seen.add(address)
        compatibility = row["retained_compatibility"]
        if (compatibility["status"] != "match" or compatibility["size"] != before["size"]
                or compatibility["before_outcome_sha256"] != compatibility["after_outcome_sha256"]
                or compatibility["linker_model"] != "armlink5-inline-branchnop-v6"
                or compatibility["literal_model"] != "anonymous-string-records-v1"):
            raise ValueError("protected metadata transition changes its retained source/object contract")
        end = address + before["size"]
        if (not analyzer.base <= address < end <= analyzer.end
                or hashlib.sha256(analyzer.text[address - analyzer.base:end - analyzer.base]).hexdigest()
                != row["original_extent_sha256"]):
            raise ValueError("protected metadata transition lacks its unchanged original extent")
        literals = [item for item in literal_encodings if item["function"] == address]
        if (not literals or _metadata_sha(literals) != row["literal_witnesses_sha256"]
                or len({item["target"] for item in literals}) != len(literals)):
            raise ValueError("protected metadata transition lacks its exact typed literal witnesses")
        spans = [tuple(span) for span in before["literals"]]
        for literal in literals:
            first, stop = literal["target"], literal["end"]
            aligned_end = (stop + 3) & ~3
            if (literal["encoding"] != "utf-16le" or not literal["references"]
                    or not address <= first < stop <= aligned_end <= end
                    or analyzer._utf16_string_end(first) != stop
                    or hashlib.sha256(analyzer.text[first - analyzer.base:stop - analyzer.base]).hexdigest()
                    != literal["image_interval_sha256"]
                    or any(analyzer.text[stop - analyzer.base:aligned_end - analyzer.base])
                    or any(first < hi and lo < aligned_end for lo, hi in before["code_ranges"] + before["pools"])
                    or any(first <= entry["address"] < aligned_end for entry in before["entries"])
                    or any(not any(lo <= site < hi for lo, hi in before["code_ranges"])
                           for site in literal["references"])):
                raise ValueError("protected metadata transition has an unproved inline literal bound")
            spans.append((first, aligned_end))
        merged = []
        for first, stop in sorted(spans):
            if merged and first <= merged[-1][1]:
                merged[-1][1] = max(merged[-1][1], stop)
            else:
                merged.append([first, stop])
        derived = {**before, "literals": merged}
        if derived != after:
            raise ValueError("protected metadata transition does not equal the derived complete successor")
        for identity, expected in protected.items():
            if expected["address"] == address:
                if expected != before and expected != derived:
                    raise ValueError(f"protected function {identity:#x} differs from both approved metadata records")
                resolved[identity] = derived
        receipts.append({key: row[key] for key in (
            "address", "before_sha256", "after_sha256", "original_extent_sha256", "literal_witnesses_sha256")})
    return resolved, receipts


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
    revision_path = root / "research/catalogue_v10_r3_evidence.json"
    revision = json.loads(revision_path.read_bytes())
    descriptor = revision["parent_evidence"]
    if (root / descriptor["path"] != path or descriptor["sha256"] != inputs[str(path.relative_to(root))]
            or revision["image_sha256"] != evidence["image_sha256"]):
        raise ValueError("v10-r3 evidence differs from its frozen parent/image")
    library = revision["library_identification"]
    _validate_library_evidence(revision["classification_records"], library, revision["image_sha256"])
    evidence["classification_records"] = evidence.get("classification_records", []) + revision["classification_records"]
    evidence["library_identification"] = library
    evidence["library_provenance"] = revision["provenance"]
    inputs[str(revision_path.relative_to(root))] = hashlib.sha256(revision_path.read_bytes()).hexdigest()
    extension_path = root / "research/catalogue_v10_r4_evidence.json"
    extension = json.loads(extension_path.read_bytes())
    descriptor = extension["parent_evidence"]
    if (root / descriptor["path"] != revision_path
            or descriptor["sha256"] != inputs[str(revision_path.relative_to(root))]
            or extension["image_sha256"] != evidence["image_sha256"]):
        raise ValueError("v10-r4 evidence differs from its frozen parent/image")
    additions = extension["classification_records"]
    _validate_declared_extents(additions, extension)
    library = {
        **library, "source_revision": extension["source_revision"],
        "authenticated_owners": library["authenticated_owners"] + extension["authenticated_owners"],
        "source_sha256": {**library.get("source_sha256", {}), **extension["source_sha256"]},
        "summary": extension["summary"],
        "r7_extension": {**{key: value for key, value in extension.items()
                           if key not in ("classification_records", "authenticated_owners")},
                         "prior_summary": library.get("summary", {})},
    }
    _validate_library_evidence(revision["classification_records"] + additions, library, evidence["image_sha256"])
    evidence["classification_records"] += additions
    evidence["library_identification"] = library
    evidence["library_extension_provenance"] = extension["qualification"]
    inputs[str(extension_path.relative_to(root))] = hashlib.sha256(extension_path.read_bytes()).hexdigest()
    repair_path = root / "research/catalogue_v10_r5_evidence.json"
    repair = json.loads(repair_path.read_bytes())
    descriptor = repair["parent_evidence"]
    if (root / descriptor["path"] != extension_path
            or descriptor["sha256"] != inputs[str(extension_path.relative_to(root))]
            or repair["image_sha256"] != evidence["image_sha256"]
            or repair["revision"] != "catalogue-v10-r5"):
        raise ValueError("v10-r5 evidence differs from its frozen parent/image/revision")
    evidence["endian_repair"] = repair
    inputs[str(repair_path.relative_to(root))] = hashlib.sha256(repair_path.read_bytes()).hexdigest()
    if __package__:
        from . import catalogue_runtime_data
    else:
        import catalogue_runtime_data
    runtime_data, runtime_inputs = catalogue_runtime_data.load_inputs(root, repair_path)
    protection, protection_inputs = load_protection_inputs(
        root, root / "research/catalogue_v11_evidence.json", runtime_data)
    evidence["protection_revision"] = protection
    evidence["runtime_data"] = runtime_data
    evidence["revision"] = protection["revision"]
    inputs.update(protection_inputs)
    inputs.update(runtime_inputs)
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
    runtime_repairs = {row["address"]: row for row in candidate["closure_evidence"].get("exact_runtime_data", {}).get("extent_repairs", [])}
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
        runtime = runtime_repairs.get(address)
        if runtime is not None:
            if (after is None or (size, isa, after["size"]) != (runtime["old_size"], runtime["isa"], runtime["size"])
                    or not runtime["entries_unchanged"]):
                raise ValueError("runtime extent repair loses its independently witnessed identity")
            extent_changes.append(runtime)
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
    runtime_sections = closure.get("exact_runtime_data", {}).get("sections", [])
    runtime_entries = closure.get("exact_runtime_data", {}).get("entry_fragments", [])
    old_unknown = [row for row in prior["ranges"] if row["kind"] == "unknown"]
    for row in candidate["ranges"]:
        if row["kind"] == "unknown" and sum(max(0, min(row["end"], old["end"]) - max(row["start"], old["start"]))
                                          for old in old_unknown) != row["end"] - row["start"]:
            raise ValueError("closure introduces unknown bytes outside the adopted baseline")
    leaves = {row["start"]: row for row in closure["closed_leaves"]}
    for row in unknown_rows:
        for part in row["parts"]:
            first, end = part["start"], part["end"]
            entry = next((item for item in runtime_entries if item["address"] <= first < end <= item["address"] + item["size"]), None)
            if entry is not None:
                part["reason"] = "Actual global exact-sized Thumb declaration in independently bound complete runtime section; new library entry, no C credit"
                part["runtime_witness"] = entry["id"]
                continue
            runtime = next((item for item in runtime_sections if item["start"] <= first < end <= item["end"]), None)
            if runtime is not None:
                part["reason"] = ("Complete independently placed exact-build section/data partition; unowned data, no function extent growth"
                                  if runtime["kind"] == "rodata" else
                                  "Complete independently bound runtime section plus exact one-word descriptor and every-path original continuation; no C credit")
                part["runtime_witness"] = runtime["id"]
                continue
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
            if any(item.get("descriptor_range", [None])[0] == row["address"] for item in runtime_sections):
                row["reason"] = "Exact dispatcher descriptor width and all-path continuation targets close the exceptional reachable-code anomaly"
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
    library = evidence["library_identification"]
    for row in library["authenticated_owners"]:
        function = current.get(row["address"])
        if function is None or (function["size"], function["isa"]) != (row["size"], row["isa"]):
            raise ValueError("authenticated library evidence no longer matches the candidate extent/ISA")
    prior_authentication = [row for row in library["prior_classification_authentication"] if row["authenticated"]]
    for row in prior_authentication:
        function = current.get(row["address"])
        if row["address"] in runtime_repairs:
            historical = current[row["address"]].get("runtime_extent_evidence", {}).get("historical_classification", {})
            if (historical.get("size"), row["isa"], row["kind"]) != (row["size"], current[row["address"]]["isa"], current[row["address"]]["kind"]):
                raise ValueError("historical runtime authentication is not retained at its original extent")
            continue
        if function is None or (function["size"], function["isa"], function["kind"]) != (row["size"], row["isa"], row["kind"]):
            raise ValueError("supplemental vendor authentication cannot replace prior ownership/provenance")
    proposed = [row for row in evidence["classification_records"] if row["kind"] == "library"]
    applied = [row for row in proposed if current[row["address"]].get("classification_evidence") == row]
    candidate["revision_review"]["library_identification"] = {
        **library, "proposed_classifications": len(proposed), "applied_classifications": len(applied),
        "applied_extent_bytes": sum(row["size"] for row in applied),
        "authenticated_prior_owners_preserved": len(prior_authentication),
        "protected_coincidences_flag_only": len(library["protected_collisions"]),
        "proposed_names": len(library["names_proposal"]["symbols"]), "canonical_names_applied": 0,
        "checkpoint_name_proposals": library["r7_extension"]["names_handoff"]["proposed_names"],
        "new_name_bindings_embedded": library["r7_extension"]["names_handoff"]["new_name_bindings_embedded"],
    }
    if "runtime_data" in evidence:
        candidate["revision_review"]["exact_runtime_data"] = {
            **closure.get("exact_runtime_data", {}), "prior_unadopted_proposal": evidence["runtime_data"]["prior_proposal"],
            "historical_negative_measurements": evidence["runtime_data"]["historical_negative_measurements"],
            "adoption": False, "remaining_unknown_bytes": candidate["stats"]["bytes_by_range_kind"].get("unknown", 0),
            "remaining_unknown_intervals": candidate["stats"]["unknown_ranges"],
        }
    candidate["revision_review"]["raw_svc_contracts"] = {
        "sources": _RAW_SVC_SOURCES,
        "documented_service_policy": "Invalidate r0-r7, IP, LR and flags, unioned with decoded writes and complete documented outputs.",
        "unknown_service_policy": "Unknown number, unlisted service or incomplete output contract discards every address fact; no AAPCS preservation assumption.",
        "flags": "No condition-flag value is propagated or used to prove a predicate; CPSR/APSR/NZCV are explicitly invalidated.",
        "services": [
            {"service": service, "name": name, "documented_outputs": list(outputs) if outputs is not None else None,
             "owned_sites": sum(row["service"] == service for row in closure["raw_svc_sites"])}
            for service, (name, outputs) in sorted(_RAW_SVC_CONTRACTS.items())
        ],
    }
    candidate["revision_review"]["data_endian_repair"] = evidence["endian_repair"]
    candidate["revision_review"]["protected_metadata_transitions"] = candidate["closure_evidence"].get(
        "protected_metadata_transitions", [])
