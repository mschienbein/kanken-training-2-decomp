"""Strict in-place link of an armcc ELF object against the original executable.

Function sections are placed at their catalogue addresses. Every other allocatable section the
code references (string pools, const tables, statics, BSS) is placed where the original bytes say
it lives: each relocation's original resolved value fixes the section base, all references must
agree, and the section's own bytes must then equal the original bytes at that base (BSS must fall
inside the BSS range). Relocations are applied exactly as armlink would for ARMv6K, including
ARM<->Thumb BL/BLX interworking; an unsupported relocation type, an unresolved symbol or a
branch that would need a linker veneer is an error, never a silent pass.

Because relocated fields must reproduce the original bytes, a byte match also proves every call
and data reference resolves to the same address as in the original — except where the selected
`LinkerModel` reproduces an armlink output optimisation (docs/LINKER.md): a BL inlined as its
callee's instruction (or a NOP) proves body equivalence, not callee identity. The opt-in v3 model
also permits a candidate-defined tiny callee with no catalogue counterpart, only when every
reference is an ABI-consistent inlined call and its section can be removed. A B rewritten as a
NOP proves only that the target starts there. Every rewrite and removed callee is reported.

The independent, opt-in literal model permits a complete anonymous SHF_MERGE|SHF_STRINGS section
to be permuted by whole-record identity (rule anonymous-string-records-v1). It never searches
for data or changes instructions: every record needs an original relocation witness, the same
record bytes, and a bijective placement inside one equal-sized, completely verified pool.
"""
from __future__ import annotations

import re
import struct
from dataclasses import dataclass, field, replace
from fnmatch import fnmatchcase
from io import BytesIO
from pathlib import Path
from typing import NamedTuple

from capstone import CS_ARCH_ARM, CS_GRP_BRANCH_RELATIVE, CS_GRP_CALL, CS_GRP_INT, CS_GRP_IRET, CS_GRP_JUMP, CS_GRP_RET, CS_MODE_ARM, Cs
from capstone import arm as cs_arm
from elftools.elf.constants import SH_FLAGS
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
from image import ImageError

from linker_models import (DEFAULT_LINKER_MODEL, DEFAULT_LITERAL_MODEL, LINKER_MODELS,  # noqa: F401  (re-exported)
                           LITERAL_MODELS, RETIRED_LINKER_MODELS, LinkerModel)

# ELF relocation types (AAELF32) handled here.
R_ARM_NONE = 0
R_ARM_PC24 = 1
R_ARM_ABS32 = 2
R_ARM_REL32 = 3
R_ARM_THM_CALL = 10
R_ARM_CALL = 28
R_ARM_JUMP24 = 29
R_ARM_TARGET1 = 38
R_ARM_V4BX = 40
R_ARM_PREL31 = 42
R_ARM_THM_JUMP11 = 102
R_ARM_THM_JUMP8 = 103

AUTO_NAME = re.compile(r"^(func|data|bss)_([0-9a-fA-F]{8})$")


class LinkError(RuntimeError):
    pass


# -- armlink output optimisations (docs/LINKER.md) ---------------------------------------------

ARM_LINKER_NOP = 0xE1A00000  # MOV r0, r0: what armlink writes for a removed ARM branch or empty call
_ARM_RETURNS = frozenset((0xE12FFF1E, 0xE1A0F00E))  # BX lr, MOV pc, lr (unconditional)
_COND_AL = 0xE
_ARM = Cs(CS_ARCH_ARM, CS_MODE_ARM)
_ARM.detail = True
_FLOW_GROUPS = frozenset((CS_GRP_JUMP, CS_GRP_CALL, CS_GRP_RET, CS_GRP_INT, CS_GRP_IRET, CS_GRP_BRANCH_RELATIVE))


def linker_model(name: str) -> LinkerModel:
    if name in RETIRED_LINKER_MODELS:
        raise LinkError(f"linker model {name!r} is retired: {RETIRED_LINKER_MODELS[name]}")
    model = LINKER_MODELS.get(name)
    if model is None:
        raise LinkError(f"unknown linker model {name!r}; known: {', '.join(sorted(LINKER_MODELS))}")
    return model


@dataclass(frozen=True)
class Rewrite:
    """A branch written as armlink's output optimisation instead of being relocated."""

    section: int  # index of the section holding the branch
    offset: int  # of the branch inside that section
    rule: str  # "inline_nop", "inline_insn" or "branchnop"
    target: int | None  # None only for a removed callee, which has no original image address
    word: int  # what was written in place of the branch
    removed_callee: dict | None = None  # candidate-only section's identity, never its bytes

    def record(self) -> dict:
        row = {"offset": self.offset, "rule": self.rule, "target": self.target, "word": f"{self.word:#010x}"}
        if self.removed_callee is not None:
            row["removed_callee"] = self.removed_callee
        return row


def inlinable_arm_instruction(word: int) -> bool:
    """May armlink copy this callee instruction into the caller? (docs/LINKER.md, rule I2)

    Not when it reads or writes the PC (the manual's rule); also refused, unverified either way:
    LR access, control transfer, exception generation. Never armlink's own NOP: a callee starting
    with MOV r0, r0 is a tail-call B that --branchnop removed after inlining ran, so armlink saw a
    branch there (the original keeps the BL to every such callee)."""
    if word == ARM_LINKER_NOP:
        return False
    insn = next(_ARM.disasm(struct.pack("<I", word), 0, 1), None)
    if insn is None or _FLOW_GROUPS & set(insn.groups):
        return False
    read, written = insn.regs_access()
    return not {cs_arm.ARM_REG_PC, cs_arm.ARM_REG_LR} & (set(read) | set(written))


def inline_arm_call(call: int, body: bytes) -> tuple[str, int] | None:
    """armlink's replacement for the ARM BL[cond] `call` to an ARM function whose whole section is
    `body`: (rule, word), or None when the call stays a BL (docs/LINKER.md, rules I1-I3)."""
    if len(body) not in (4, 8):
        return None
    words = struct.unpack(f"<{len(body) // 4}I", body)
    if words[-1] not in _ARM_RETURNS:
        return None
    if len(words) == 1:
        # A return-only function becomes a NOP, unconditional even for BLcond: in the original,
        # conditional calls of empty functions left plain MOV r0, r0 after their compares.
        return "inline_nop", ARM_LINKER_NOP
    insn = words[0]
    if not inlinable_arm_instruction(insn):
        return None
    call_cond, insn_cond = call >> 28, insn >> 28
    if call_cond == _COND_AL:
        return "inline_insn", insn
    if insn_cond == 0xF:
        return None  # an unconditional-only instruction cannot take the BL's condition
    if insn_cond == _COND_AL:
        return "inline_insn", (insn & 0x0FFFFFFF) | (call_cond << 28)
    if insn_cond == call_cond:
        return "inline_insn", insn
    return None


_ARM_ARGUMENTS = frozenset((cs_arm.ARM_REG_R0, cs_arm.ARM_REG_R1, cs_arm.ARM_REG_R2, cs_arm.ARM_REG_R3))
_ARM_CALLER_SAVED = frozenset((cs_arm.ARM_REG_R1, cs_arm.ARM_REG_R2, cs_arm.ARM_REG_R3, cs_arm.ARM_REG_R12))
_ARM_RESULTS = frozenset((cs_arm.ARM_REG_R0, cs_arm.ARM_REG_R1))


def _arm_call_flow(insn, *, bounded_switches: bool = False) -> str:
    """R1 flow classification; v4 models effects without reinterpreting frozen v3."""
    if bounded_switches:
        effects = _arm_switch_effects(insn)
        if effects is None:
            return "unknown"
        if insn.id in (cs_arm.ARM_INS_BL, cs_arm.ARM_INS_BLX):
            return "call"
        if insn.id == cs_arm.ARM_INS_B:
            return "branch"
        word = _arm_word(insn)
        if (word & 0x0FFFFFFF in (0x012FFF1E, 0x01A0F00E)
                or word & 0x0FFF8000 == 0x08BD8000):
            return "return"
        return "unknown" if cs_arm.ARM_REG_PC in effects[0] else "other"
    if insn.id in (cs_arm.ARM_INS_BL, cs_arm.ARM_INS_BLX):
        return "call"
    if insn.id == cs_arm.ARM_INS_B and insn.operands[0].type == cs_arm.ARM_OP_IMM:
        return "branch"
    read, written = map(set, insn.regs_access())
    if (CS_GRP_RET in insn.groups
            or insn.id == cs_arm.ARM_INS_BX and insn.operands[0].reg == cs_arm.ARM_REG_LR
            or cs_arm.ARM_REG_PC in written and (
                cs_arm.ARM_REG_LR in read or insn.id == cs_arm.ARM_INS_POP
                or insn.id in (cs_arm.ARM_INS_LDM, cs_arm.ARM_INS_LDMDA, cs_arm.ARM_INS_LDMDB, cs_arm.ARM_INS_LDMIB)
                and cs_arm.ARM_REG_SP in read)):
        return "return"
    if _FLOW_GROUPS & set(insn.groups) or cs_arm.ARM_REG_PC in written:
        return "unknown"
    return "other"


def _arm_successors(instructions: dict, insn, flow: str) -> list[int]:
    conditional = insn.cc not in (cs_arm.ARM_CC_AL, cs_arm.ARM_CC_INVALID)
    successors = []
    if flow == "branch":
        target = insn.operands[0].imm
        if target in instructions:
            successors.append(target)
    if flow in ("other", "call") or conditional and flow in ("branch", "return"):
        following = insn.address + insn.size
        if following in instructions:
            successors.append(following)
    return successors


# -- ARMCC bounded jump tables (docs/LINKER.md, "Jump tables in the call-context CFG") ----------

_ARM_NUMBERED = (*(getattr(cs_arm, f"ARM_REG_R{n}") for n in range(13)),
                 cs_arm.ARM_REG_SP, cs_arm.ARM_REG_LR, cs_arm.ARM_REG_PC)
# The two encodings armcc 5.04 emits for a dense switch, both with the index scaled LSL #2:
#   LDR<cc> pc, [pc, rM, lsl #2]  (word table: absolute target addresses, in the caller's pool)
#   ADD<cc> pc, pc, rM, lsl #2    (branch table: one unconditional B per entry, in the code)
# Each is taken only when its condition is LO (rM <  N) or LS (rM <= N) after `cmp rM, #N`.
_SWITCH_ENCODINGS = {0x079FF100: "words", 0x008FF100: "branches"}
_SWITCH_EXTRA_ENTRIES = {0x3: 0, 0x9: 1}  # condition field -> entries beyond N
_SWITCH_BASE = 8  # the PC reads as the writer + 8: entry 0 is two words after the writer

# Explicit ARM instruction classes whose core-register/NZCV/processor-state effects are modeled.
# Membership is necessary, not sufficient: each encoding below is also validated. Decoder
# regs_access/update_flags/groups are deliberately NOT evidence for either switch proof.
_ARM_SWITCH_DP_OPS = {
    getattr(cs_arm, "ARM_INS_" + name): (opcode,)
    for opcode, name in enumerate(("AND", "EOR", "SUB", "RSB", "ADD", "ADC", "SBC", "RSC",
                                  "TST", "TEQ", "CMP", "CMN", "ORR", "MOV", "BIC", "MVN"))
}
_ARM_SWITCH_DP_OPS.update({getattr(cs_arm, "ARM_INS_" + name): (13,)
                           for name in ("ASR", "LSL", "LSR", "ROR", "RRX")})
_ARM_SWITCH_DP_OPS[cs_arm.ARM_INS_ADR] = (2, 4)
_ARM_SWITCH_SINGLE_MEMORY = {
    cs_arm.ARM_INS_LDR: (1, 0), cs_arm.ARM_INS_STR: (0, 0),
    cs_arm.ARM_INS_LDRB: (1, 1), cs_arm.ARM_INS_STRB: (0, 1),
}
_ARM_SWITCH_EXTRA_MEMORY = {
    cs_arm.ARM_INS_LDRH: (1, 1), cs_arm.ARM_INS_STRH: (0, 1),
    cs_arm.ARM_INS_LDRSB: (1, 2), cs_arm.ARM_INS_LDRSH: (1, 3),
    cs_arm.ARM_INS_LDRD: (0, 2), cs_arm.ARM_INS_STRD: (0, 3),
}
_ARM_SWITCH_MULTI_MEMORY = frozenset(getattr(cs_arm, "ARM_INS_" + name) for name in (
    "PUSH", "POP", "LDM", "LDMDA", "LDMDB", "LDMIB", "STM", "STMDA", "STMDB", "STMIB"))
_ARM_SWITCH_VFP_DATA = frozenset(getattr(cs_arm, "ARM_INS_" + name) for name in (
    "VABS", "VADD", "VCMP", "VCMPE", "VCVT", "VCVTR", "VDIV", "VMLA", "VMLS",
    "VMUL", "VNEG", "VNMLA", "VNMLS", "VNMUL", "VSQRT", "VSUB"))
_ARM_SWITCH_VFP_MEMORY = frozenset(getattr(cs_arm, "ARM_INS_" + name) for name in (
    "VLDR", "VSTR", "VPUSH", "VPOP", "VLDMIA", "VLDMDB", "VSTMIA", "VSTMDB"))
_ARM_SWITCH_MULTIPLY = frozenset(getattr(cs_arm, "ARM_INS_" + name) for name in (
    "MUL", "MLA", "UMULL", "UMLAL", "SMULL", "SMLAL"))
_ARM_SWITCH_CONTROL = frozenset((cs_arm.ARM_INS_B, cs_arm.ARM_INS_BL, cs_arm.ARM_INS_BLX, cs_arm.ARM_INS_BX))
_ARM_SWITCH_HINTS = frozenset((cs_arm.ARM_INS_NOP, cs_arm.ARM_INS_HINT))
_ARM_SWITCH_MODELED_IDS = frozenset(_ARM_SWITCH_DP_OPS) | frozenset(_ARM_SWITCH_SINGLE_MEMORY) | frozenset(
    _ARM_SWITCH_EXTRA_MEMORY) | _ARM_SWITCH_MULTI_MEMORY | _ARM_SWITCH_VFP_DATA | _ARM_SWITCH_VFP_MEMORY | (
    _ARM_SWITCH_MULTIPLY | _ARM_SWITCH_CONTROL | _ARM_SWITCH_HINTS
    | {cs_arm.ARM_INS_VMOV, cs_arm.ARM_INS_VMRS, cs_arm.ARM_INS_FMSTAT})


def _arm_switch_effects(insn) -> tuple[frozenset[int], bool] | None:
    """Complete possible core writes and NZCV writes for allowed A32 classes; None is refusal.

    Only validated, ordinary encodings with known processor-state effects are admitted.
    In particular, no exception return, status restore, coprocessor/exclusive instruction
    or unknown/new decoder class is inferred harmless from an empty decoder access list.
    VFP data/FP-status writes do not change core registers, CPSR.NZCV or CPSR.E; VMOV/VMRS
    core transfers and scalar/multiple load/writeback destinations are decoded explicitly.
    Calls retain the existing LE entry/ABI-conforming-return contract, not a callee-state proof.
    """
    if insn.id not in _ARM_SWITCH_MODELED_IDS:
        return None
    word = _arm_word(insn)
    condition, rn, rd = word >> 28, (word >> 16) & 15, (word >> 12) & 15
    written = set()
    flags = False
    if insn.id in _ARM_SWITCH_CONTROL:
        if (word >> 25) & 7 == 5:
            if (condition == 15) != (insn.id == cs_arm.ARM_INS_BLX):
                return None
            if condition != 15 and bool(word & (1 << 24)) != (insn.id == cs_arm.ARM_INS_BL):
                return None
        elif (condition == 15 or insn.id not in (cs_arm.ARM_INS_BX, cs_arm.ARM_INS_BLX)
              or word & 0x0FFFFFF0 != (0x012FFF30 if insn.id == cs_arm.ARM_INS_BLX else 0x012FFF10)
              or word & 15 == 15):
            return None
        written.add(cs_arm.ARM_REG_PC)
        if insn.id in (cs_arm.ARM_INS_BL, cs_arm.ARM_INS_BLX):
            written.add(cs_arm.ARM_REG_LR)
    elif condition == 15:
        return None  # unconditional system/exception/NEON encodings are not ordinary A32
    elif insn.id in _ARM_SWITCH_HINTS:
        if word & 0x0FFFFFFF != 0x0320F000:
            return None  # only the compiler's actual NOP, not arbitrary hints
    elif insn.id in _ARM_SWITCH_DP_OPS:
        opcode = (word >> 21) & 15
        if (word & 0x0C000000 or opcode not in _ARM_SWITCH_DP_OPS[insn.id]
                or not word & (1 << 25) and word & (1 << 4)):
            return None
        flags = bool(word & (1 << 20))
        if 8 <= opcode <= 11:
            if not flags or rd:
                return None
        else:
            if rd == 15 and (flags or not (
                    word & 0x0FFFFFFF == 0x01A0F00E
                    or word & 0x0FFFFFF0 == 0x008FF100 and condition in _SWITCH_EXTRA_ENTRIES)):
                return None
            written.add(_ARM_NUMBERED[rd])
    elif insn.id in _ARM_SWITCH_SINGLE_MEMORY or insn.id in _ARM_SWITCH_EXTRA_MEMORY:
        if insn.id in _ARM_SWITCH_SINGLE_MEMORY:
            if (word & 0x0C000000 != 0x04000000
                    or word & (1 << 25) and (word & (1 << 4) or word & 15 == 15)
                    or ((word >> 20) & 1, (word >> 22) & 1) != _ARM_SWITCH_SINGLE_MEMORY[insn.id]):
                return None
        elif (word & 0x0E000090 != 0x00000090
              or ((word >> 20) & 1, (word >> 5) & 3) != _ARM_SWITCH_EXTRA_MEMORY[insn.id]
              or not word & (1 << 22) and (word & 0xF00 or word & 15 == 15)):
            return None
        double = insn.id in (cs_arm.ARM_INS_LDRD, cs_arm.ARM_INS_STRD)
        if double and (rd & 1 or rd > 12):
            return None
        load = bool(word & (1 << 20)) or insn.id == cs_arm.ARM_INS_LDRD
        destinations = {rd, rd + 1} if double else {rd}
        if not load and rd == 15 and insn.id != cs_arm.ARM_INS_STR:
            return None
        if load:
            if rd == 15 and not (
                    insn.id == cs_arm.ARM_INS_LDR and word & 0x0FFFFFF0 == 0x079FF100
                    and condition in _SWITCH_EXTRA_ENTRIES):
                return None
            written.update(_ARM_NUMBERED[register] for register in destinations)
        if not word & (1 << 24) or word & (1 << 21):
            if rn == 15 or rn in destinations:
                return None  # reserved/unpredictable writeback is not a preservation proof
            written.add(_ARM_NUMBERED[rn])
    elif insn.id in _ARM_SWITCH_MULTI_MEMORY:
        registers = word & 0xFFFF
        if (word & 0x0E000000 != 0x08000000 or word & (1 << 22) or not registers or rn == 15
                or word & (1 << 21) and registers & (1 << rn)):
            return None
        if word & (1 << 20):
            if registers & (1 << 15) and word & 0x0FFF8000 != 0x08BD8000:
                return None  # only the ordinary SP-based POP return, never CPSR-restoring LDM
            written.update(_ARM_NUMBERED[register] for register in range(16) if registers & (1 << register))
        if word & (1 << 21):
            written.add(_ARM_NUMBERED[rn])
    elif insn.id in _ARM_SWITCH_MULTIPLY:
        if insn.id in (cs_arm.ARM_INS_MUL, cs_arm.ARM_INS_MLA):
            if word & 0x0FC000F0 != 0x00000090 or bool(word & (1 << 21)) != (insn.id == cs_arm.ARM_INS_MLA):
                return None
            if (insn.id == cs_arm.ARM_INS_MUL and rd
                    or insn.id == cs_arm.ARM_INS_MLA and rd in (13, 15)):
                return None
            destinations = {rn}
        else:
            if word & 0x0F8000F0 != 0x00800090 or rn == rd:
                return None
            destinations = {rn, rd}
        if destinations & {13, 15} or (word & 15) in (13, 15) or ((word >> 8) & 15) in (13, 15):
            return None
        written.update(_ARM_NUMBERED[register] for register in destinations)
        flags = bool(word & (1 << 20))
    elif insn.id in _ARM_SWITCH_VFP_MEMORY:
        if word & 0x0E000E00 != 0x0C000A00:
            return None
        pre, up, writeback, load = ((word >> bit) & 1 for bit in (24, 23, 21, 20))
        if insn.id in (cs_arm.ARM_INS_VLDR, cs_arm.ARM_INS_VSTR):
            if not pre or writeback or load != (insn.id == cs_arm.ARM_INS_VLDR):
                return None
        else:
            decrement = insn.id in (cs_arm.ARM_INS_VPUSH, cs_arm.ARM_INS_VLDMDB, cs_arm.ARM_INS_VSTMDB)
            loading = insn.id in (cs_arm.ARM_INS_VPOP, cs_arm.ARM_INS_VLDMIA, cs_arm.ARM_INS_VLDMDB)
            if (pre != decrement or up == decrement or load != loading or rn == 15
                    or decrement and not writeback
                    or insn.id in (cs_arm.ARM_INS_VPUSH, cs_arm.ARM_INS_VPOP) and (rn != 13 or not writeback)):
                return None
            count = word & 0xFF
            double = bool(word & (1 << 8))
            first = rd + (((word >> 22) & 1) << 4) if double else rd * 2 + ((word >> 22) & 1)
            if (not count or double and count & 1
                    or first + (count // 2 if double else count) > (16 if double else 32)):
                return None
        if word & (1 << 21):
            if rn == 15:
                return None
            written.add(_ARM_NUMBERED[rn])
    elif insn.id in (cs_arm.ARM_INS_VMRS, cs_arm.ARM_INS_FMSTAT):
        if word & 0x0FFF0FFF != 0x0EF10A10 or insn.id == cs_arm.ARM_INS_FMSTAT and rd != 15:
            return None  # only FPSCR, not an arbitrary processor/system-register transfer
        if rd == 15:
            flags = True
        else:
            written.add(_ARM_NUMBERED[rd])
    elif insn.id == cs_arm.ARM_INS_VMOV:
        if word & 0x0F000E10 == 0x0E000A00:
            pass  # VFP register/immediate data move: no core destination or CPSR update
        elif word & 0x0FE00F7F == 0x0E000A10:
            if rd in (13, 15):
                return None
            if word & (1 << 20):
                written.add(_ARM_NUMBERED[rd])
        elif word & 0x0FE00EF0 == 0x0C400A10:
            if rn in (13, 15) or rd in (13, 15) or rn == rd:
                return None
            if word & (1 << 20):
                written.update((_ARM_NUMBERED[rn], _ARM_NUMBERED[rd]))
        else:
            return None
    elif insn.id in _ARM_SWITCH_VFP_DATA:
        if word & 0x0F000E10 != 0x0E000A00:
            return None
    else:
        return None
    return frozenset(written), flags


def _arm_word(insn) -> int:
    return struct.unpack("<I", insn.bytes)[0]


def _arm_compare_bound(insn) -> tuple[int, int] | None:
    """(register number, value) of an unconditional `cmp rN, #immediate`, else None."""
    word = _arm_word(insn)
    if word >> 28 != _COND_AL or word & 0x0FF0F000 != 0x03500000:
        return None
    rotate, value = (word >> 7) & 0x1E, word & 0xFF
    if rotate:
        value = (value >> rotate | value << (32 - rotate)) & 0xFFFFFFFF
    return (word >> 16) & 0xF, value


def _arm_may_change(insn, register: int) -> bool:
    """An unmodeled instruction is never evidence that the index or NZCV is preserved."""
    effects = _arm_switch_effects(insn)
    return effects is None or register in effects[0] or effects[1]


def _arm_switch_guard(instructions: dict, flows: dict, writer: int, index: int) -> tuple[int, int] | None:
    """(address, value) of the `cmp rIndex, #value` that bounds the index at `writer`: the nearest
    earlier word in straight-line code, with nothing between that can change the index or the
    flags (every word between must be a plain, non-transferring instruction)."""
    register = _ARM_NUMBERED[index]
    address = writer
    while True:
        address -= 4
        insn = instructions.get(address)
        if insn is None or flows[address] != "other":
            return None
        bound = _arm_compare_bound(insn)
        if bound is not None and bound[0] == index:
            return address, bound[1]
        if _arm_may_change(insn, register):
            return None




def _arm_jump_tables(instructions: dict, flows: dict, entries: set[int], words: dict[int, int]) -> dict[int, list[int]]:
    """Successors of every ARMCC jump-table writer whose bound and every target are proven.

    The bound comes from the guarding `cmp rM, #N` (see `_arm_switch_guard`) and the writer's own
    LO/LS condition, so exactly N (LO) or N + 1 (LS) entries are reachable: no inference from the
    table's contents or length. Entry i is the word at writer + 8 + 4i: an absolute address in
    `words` (the caller's own pool, never an instruction) for the word-table form, or an
    unconditional direct B for the branch-table form. Every target must be a decoded instruction
    of this caller (never outside its catalogue extent, never Thumb), and the conditional-false
    fall-through must be an unconditional transfer that cannot run into the table. The writer may
    be reached only by falling through from the guard: no branch, call, entry or other table
    target may land in (compare, writer]. Anything else stays unknown and the caller fails closed.
    V4 resolves no table unless every decoded owner instruction has a modeled ordinary
    processor-state effect, even when unreachable. Incomplete/unknown classes and exception
    returns cannot establish preserved state; the target's LE entry/call ABI remains required.
    """
    if any(_arm_switch_effects(insn) is None for insn in instructions.values()):
        return {}
    candidates = {}
    for address, insn in instructions.items():
        if flows[address] != "unknown":
            continue
        word = _arm_word(insn)
        form = _SWITCH_ENCODINGS.get(word & 0x0FFFFFF0)
        extra = _SWITCH_EXTRA_ENTRIES.get(word >> 28)
        if form is None or extra is None or word & 0xF == 15:
            continue
        guard = _arm_switch_guard(instructions, flows, address, word & 0xF)
        fall = instructions.get(address + 4)
        if (guard is None or guard[1] + extra < 1 or fall is None
                or fall.cc not in (cs_arm.ARM_CC_AL, cs_arm.ARM_CC_INVALID)
                or flows[address + 4] not in ("branch", "return")
                or flows[address + 4] == "branch" and fall.operands[0].imm not in instructions):
            continue
        count = guard[1] + extra
        if count > len(words if form == "words" else instructions):
            continue  # the complete bounded table must fit in the supplied caller ranges
        targets = []
        for slot in range(count):
            entry = address + _SWITCH_BASE + 4 * slot
            if form == "words":
                target = words.get(entry) if entry not in instructions else None
            else:
                branch = instructions.get(entry)
                unconditional = (branch is not None and flows[entry] == "branch"
                                 and branch.cc in (cs_arm.ARM_CC_AL, cs_arm.ARM_CC_INVALID))
                target = entry if unconditional and branch.operands[0].imm in instructions else None
            if target not in instructions:
                break
            targets.append(target)
        else:
            candidates[address] = guard[0], [address + 4, *dict.fromkeys(targets)]
    # Every address entered other than by falling through, tentatively including all table targets.
    entered = set(entries)
    for insn in instructions.values():
        if insn.id in (cs_arm.ARM_INS_B, cs_arm.ARM_INS_BL, cs_arm.ARM_INS_BLX) and insn.operands[0].type == cs_arm.ARM_OP_IMM:
            entered.add(insn.operands[0].imm)
    for _, successors in candidates.values():
        entered.update(successors[1:])
    return {address: successors for address, (guard, successors) in candidates.items()
            if not any(window in entered for window in range(guard + 4, address + 4, 4))}


def arm_call_abi_context(instructions: dict, site: int, arguments: set[int], condition: int,
                         entries: set[int] | None = None, call_sites: set[int] | None = None,
                         words: dict[int, int] | None = None, *, bounded_switches: bool = False) -> str:
    """Conservative whole-caller CFG filter, not evidence of a historical helper's identity.

    An input is defined in the preceding four straight-line words on every incoming path,
    is an unmodified incoming argument, or is an unmodified r0/r1 result of an unconditional
    preceding call on that path. Conditional definitions/calls alone do not establish inputs.
    Both conditional successors are checked; unresolved indirect transfers fail closed.
    Only the explicit `bounded_switches` policy (v4/v6) enables `_arm_jump_tables`, with
    `words` limited to this caller's own original pools. The default retains frozen v3 behavior.
    Afterwards, follow every successor until a call/return or unconditional definitions of all
    caller-saved registers/flags. r0 may be a result. Candidate removed calls are call barriers.
    """
    if site not in instructions:
        return "arguments_not_materialised"
    entries = set(entries) if entries is not None else {min(instructions)}
    call_sites = call_sites or set()
    flows = {address: _arm_call_flow(insn, bounded_switches=bounded_switches)
             for address, insn in instructions.items()}
    tables = _arm_jump_tables(instructions, flows, entries, words or {}) if bounded_switches else {}
    successors = {address: tables[address] if address in tables else _arm_successors(instructions, insn, flows[address])
                  for address, insn in instructions.items()}
    flows.update(dict.fromkeys(tables, "switch"))
    reachable, pending = set(), list(entries)
    while pending:
        address = pending.pop()
        if address in reachable:
            continue
        if address not in instructions or flows[address] == "unknown":
            return "unresolved_control_flow"
        reachable.add(address)
        pending.extend(successors[address])
    if site not in reachable:
        return "arguments_not_materialised"
    predecessors: dict[int, list[int]] = {address: [] for address in reachable}
    for address in reachable:
        for following in successors[address]:
            predecessors[following].append(address)
    for argument in arguments:
        # remaining counts the bounded straight-line setup window; clean tracks whether the
        # register could still be the caller's own incoming parameter after that window ends.
        pending, seen = [(site, 4, True)], set()
        while pending:
            address, remaining, clean = pending.pop()
            state = address, remaining, clean
            if state in seen:
                continue
            seen.add(state)
            if address in entries and not clean:
                return "arguments_not_materialised"
            if not predecessors[address] and address not in entries:
                return "arguments_not_materialised"
            for previous in predecessors[address]:
                insn = instructions[previous]
                flow = "call" if previous in call_sites else flows[previous]
                if flow == "call":
                    if (clean and argument in _ARM_RESULTS
                            and insn.cc in (cs_arm.ARM_CC_AL, cs_arm.ARM_CC_INVALID)):
                        continue  # an unmodified r0/r1 result on this incoming CFG path
                    return "arguments_not_materialised"
                window = remaining if flow == "other" and previous + insn.size == address else 0
                wrote = set(insn.regs_access()[1])
                unchanged = clean
                if argument in wrote:
                    if window and insn.cc in (cs_arm.ARM_CC_AL, cs_arm.ARM_CC_INVALID):
                        continue  # this definition dominates this incoming path within the window
                    unchanged = False
                    if not window:
                        return "arguments_not_materialised"
                pending.append((previous, max(0, window - 1), unchanged))
    pending = [(following, _ARM_CALLER_SAVED, False) for following in successors[site]]
    seen = set()
    while pending:
        address, live, flags_defined = pending.pop()
        state = address, live, flags_defined
        if state in seen:
            continue
        seen.add(state)
        insn = instructions[address]
        read, wrote = map(set, insn.regs_access())
        if read & live:
            return "reads_arguments"
        conditional = insn.cc not in (cs_arm.ARM_CC_AL, cs_arm.ARM_CC_INVALID)
        if (conditional or cs_arm.ARM_REG_CPSR in read) and not flags_defined:
            return "reads_flags"
        if not conditional:
            live = live - wrote
            flags_defined |= cs_arm.ARM_REG_CPSR in wrote
        flow = "call" if address in call_sites else flows[address]
        if flow == "unknown":
            return "unresolved_control_flow"
        if flow == "call" or flow == "return" and not conditional or not live and flags_defined:
            continue
        pending.extend((following, live, flags_defined) for following in successors[address])
    return "consistent"


def arm_pool_words(image, fn: dict) -> dict[int, int]:
    """The original 32-bit words of one catalogued function's pools/literals, inside its own extent:
    the only data `arm_call_abi_context` lets a jump table name targets from."""
    words = {}
    for lo, hi in (*fn.get("pools", ()), *fn.get("literals", ())):
        lo, hi = (max(lo, fn["address"]) + 3) & ~3, min(hi, fn["address"] + fn["size"])
        data = image.read(lo, hi - lo) if lo < hi else b""
        for offset in range(0, len(data) - 3, 4):
            words[lo + offset] = struct.unpack_from("<I", data, offset)[0]
    return words


@dataclass
class Symbol:
    name: str
    value: int
    size: int
    kind: str  # STT_* type
    bind: str
    shndx: int | str


@dataclass
class Reloc:
    offset: int
    type: int
    symbol: Symbol


@dataclass
class Section:
    index: int
    name: str
    data: bytes = field(repr=False)
    size: int
    nobits: bool
    executable: bool
    writable: bool
    relocs: list[Reloc] = field(default_factory=list)
    mapping: list[tuple[int, str]] = field(default_factory=list)  # ($a|$t|$d offset, kind)
    align: int = 1  # sh_addralign: every placement must respect it
    comdat: str | None = None  # signature of the COMDAT group this section belongs to
    merge_strings: bool = False  # both SHF_MERGE and SHF_STRINGS, not merely a familiar section name
    entsize: int = 0
    retention: tuple[str, ...] = ()  # explicit roots and uninterpreted retention metadata: never R1
    elf_type: str = "SHT_PROGBITS"
    link_order: bool = False
    linked_section: int | str = 0  # sh_link for SHF_LINK_ORDER; remapped with candidate ownership


class ObjectFile:
    """The allocatable sections, symbols and REL relocations of one armcc object."""

    def __init__(self, path: Path, *, data: bytes | None = None):
        self.path = path
        with path.open("rb") if data is None else BytesIO(data) as f:
            elf = ELFFile(f)
            if elf["e_machine"] != "EM_ARM" or elf["e_type"] != "ET_REL":
                raise LinkError(f"{path}: not an ARM relocatable object")
            self.sections: dict[int, Section] = {}
            # Keep even zero-sized allocated group members as closure witnesses. They do not
            # enter the frozen placement model, but v8 must not silently discard their group.
            allocated_indices: set[int] = set()
            self.section_groups: list[tuple[int, str, set[int | str]]] = []
            for i, s in enumerate(elf.iter_sections()):
                if s["sh_flags"] & SH_FLAGS.SHF_ALLOC:
                    allocated_indices.add(i)
                if s["sh_flags"] & SH_FLAGS.SHF_ALLOC and s["sh_size"]:
                    nobits = s["sh_type"] == "SHT_NOBITS"
                    self.sections[i] = Section(
                        i, s.name, b"" if nobits else s.data(), s["sh_size"], nobits,
                        bool(s["sh_flags"] & SH_FLAGS.SHF_EXECINSTR), bool(s["sh_flags"] & SH_FLAGS.SHF_WRITE),
                        align=max(1, s["sh_addralign"]),
                        merge_strings=(s["sh_flags"] & (SH_FLAGS.SHF_MERGE | SH_FLAGS.SHF_STRINGS)
                                       == SH_FLAGS.SHF_MERGE | SH_FLAGS.SHF_STRINGS),
                        entsize=s["sh_entsize"],
                        elf_type=s["sh_type"], link_order=bool(s["sh_flags"] & SH_FLAGS.SHF_LINK_ORDER),
                        linked_section=s["sh_link"] if s["sh_flags"] & SH_FLAGS.SHF_LINK_ORDER else 0,
                        retention=tuple(reason for flag, reason in (
                            (0x10000000, "ARM entry section"), (0x200000, "GNU retain"),
                            (SH_FLAGS.SHF_MASKOS | SH_FLAGS.SHF_MASKPROC, "OS/processor section flags"),
                        ) if s["sh_flags"] & flag),
                    )
            symtab = elf.get_section_by_name(".symtab")
            self.symbols = [
                Symbol(sym.name, sym["st_value"], sym["st_size"], sym["st_info"]["type"], sym["st_info"]["bind"], sym["st_shndx"])
                for sym in symtab.iter_symbols()
            ]
            for sym in symtab.iter_symbols():
                section = self.sections.get(sym["st_shndx"])
                if section is None:
                    continue
                if sym.name.startswith("__tagsym$$"):
                    section.retention += (sym.name,)  # unknown tag semantics also fail closed
                if sym["st_other"]["visibility"] not in ("STV_DEFAULT", "STV_HIDDEN", "STV_INTERNAL"):
                    section.retention += (f"exported visibility: {sym.name}",)
            # These input mechanisms can carry roots outside ordinary allocated relocations.
            # Do not guess a script/directive/entry's meaning or silently discard its retention.
            metadata = [s.name for s in elf.iter_sections()
                        if (s["sh_size"] and s.name in (".directive", ".drectve", ".linker-options")
                            or s["sh_type"] == "SHT_LLVM_LINKER_OPTIONS")]
            if elf["e_entry"]:
                metadata.append("ELF entry point")
            for section in self.sections.values():
                section.retention += tuple(f"uninterpreted {name}" for name in metadata)
            for s in elf.iter_sections():
                if s["sh_type"] == "SHT_GROUP":
                    words = struct.unpack(f"<{s['sh_size'] // 4}I", s.data())
                    if words:
                        flags = words[0]
                        signature = self.symbols[s["sh_info"]].name if flags & 1 else ""
                        self.section_groups.append((
                            flags, signature, {member for member in words[1:] if member in allocated_indices}))
                        if flags & 1:  # Frozen COMDAT identity; other group semantics are not modeled.
                            for member in words[1:]:
                                if member in self.sections:
                                    self.sections[member].comdat = signature
            for s in elf.iter_sections():
                if not isinstance(s, RelocationSection):
                    continue
                if s.is_RELA():
                    raise LinkError(f"{path}: RELA section {s.name} unsupported")
                target = self.sections.get(s["sh_info"])
                if target is None:
                    continue  # relocations of non-allocated sections (debug_frame)
                for r in s.iter_relocations():
                    target.relocs.append(Reloc(r["r_offset"], r["r_info_type"], self.symbols[r["r_info_sym"]]))
        for sym in self.symbols:
            if sym.name in ("$a", "$t", "$d") and isinstance(sym.shndx, int) and sym.shndx in self.sections:
                self.sections[sym.shndx].mapping.append((sym.value, sym.name))
        for sec in self.sections.values():
            sec.mapping.sort()

    def __repr__(self) -> str:
        return f"ObjectFile(sections={len(self.sections)}, symbols={len(self.symbols)})"

    @classmethod
    def combine(cls, objects: list[ObjectFile], labels: list[str] | None = None) -> ObjectFile:
        """One candidate build's sections, with local identities kept and externs bound only to
        definitions actually supplied by that build. No libraries or synthetic bodies are loaded.
        Section ownership lets receipts keep their per-translation-unit attribution.
        """
        merged = cls.__new__(cls)
        merged.sections, merged.symbols = {}, []
        merged.section_owners, merged.section_objects = {}, {}
        merged.section_groups = []
        symbols_by_object, indices_by_object = [], []
        definitions = {}
        for owner, obj in enumerate(objects):
            indices = {index: len(merged.sections) + i + 1 for i, index in enumerate(obj.sections)}
            indices_by_object.append(indices)
            symbols = {}
            for sym in obj.symbols:
                shndx = sym.shndx
                if isinstance(shndx, int):
                    shndx = indices.get(shndx, f"UNALLOCATED_{owner}_{shndx}")
                symbols[id(sym)] = replace(sym, shndx=shndx)
            symbols_by_object.append(symbols)
            merged.symbols.extend(symbols.values())
            for flags, signature, members in getattr(obj, "section_groups", ()):
                merged.section_groups.append((
                    flags, signature, {indices.get(index, f"UNALLOCATED_{owner}_{index}") for index in members}))
            for index, section in obj.sections.items():
                new_index = indices[index]
                linked = (indices.get(section.linked_section, f"UNALLOCATED_{owner}_{section.linked_section}")
                          if section.link_order else 0)
                merged.sections[new_index] = replace(section, index=new_index, relocs=[], linked_section=linked)
                merged.section_owners[new_index] = owner
                merged.section_objects[new_index] = labels[owner] if labels is not None else str(getattr(obj, "path", f"object[{owner}]"))
            for sym in obj.defined_globals():
                new_sym = symbols[id(sym)]
                prior = definitions.get(sym.name)
                if prior is not None:
                    first, second = merged.sections[prior.shndx], merged.sections[new_sym.shndx]
                    if not first.comdat or comdat_key(first) != comdat_key(second) or prior.value != new_sym.value:
                        raise LinkError(f"global {sym.name!r} is defined by several candidate objects")
                else:
                    definitions[sym.name] = new_sym
        for owner, obj in enumerate(objects):
            symbols = symbols_by_object[owner]
            for index, section in obj.sections.items():
                new_index = indices_by_object[owner][index]
                for rel in section.relocs:
                    sym = symbols[id(rel.symbol)]
                    if sym.shndx == "SHN_UNDEF" and sym.name in definitions:
                        sym = definitions[sym.name]
                    merged.sections[new_index].relocs.append(replace(rel, symbol=sym))
        return merged

    def defined_functions(self) -> list[Symbol]:
        return [
            s for s in self.symbols
            if s.kind == "STT_FUNC" and isinstance(s.shndx, int) and s.shndx in self.sections
            and not _is_internal_label(s)
        ]

    def defined_globals(self) -> list[Symbol]:
        """Externally visible definitions (functions and data) in allocated sections."""
        return [
            s for s in self.symbols
            if s.bind in ("STB_GLOBAL", "STB_WEAK") and s.kind in ("STT_FUNC", "STT_OBJECT", "STT_NOTYPE")
            and s.name and s.name not in ("$a", "$t", "$d")
            and isinstance(s.shndx, int) and s.shndx in self.sections
        ]


@dataclass(frozen=True)
class Target:
    """Where a symbol lives in the original image."""

    address: int
    thumb: bool = False


class Resolver:
    """Maps symbol names to original addresses: the names overlay, or func_/data_/bss_XXXXXXXX.

    The overlay may not define names in the reserved func_/data_/bss_ form, so every consumer
    (linker, build, progress report) resolves a name to the same address.
    """

    def __init__(self, functions: dict[int, dict], names: dict[str, dict] | None = None):
        self.functions = functions  # catalogue functions keyed by address
        self.names = names or {}
        reserved = sorted(n for n in self.names if AUTO_NAME.match(n))
        if reserved:
            raise LinkError(f"config/names.json may not define reserved address names: {', '.join(reserved[:5])}")
        # Call targets: every top-level start plus every recorded secondary entry (catalogue v3
        # `entries`, e.g. interior B/BL targets of multi-entry routines), with its ISA.
        self.call_targets: dict[int, str] = {}
        for fn in functions.values():
            for entry in fn.get("entries", ()):
                self.call_targets[entry["address"]] = entry["isa"]
            self.call_targets[fn["address"]] = fn["isa"]

    def resolve(self, name: str) -> Target:
        target = self.lookup(name)
        if target is None:
            raise LinkError(f"unresolved symbol {name!r} (not in config/names.json, not func_/data_/bss_XXXXXXXX)")
        return target

    def lookup(self, name: str) -> Target | None:
        """The bound address of `name`, or None when the name has no binding."""
        entry = self.names.get(name)
        if entry is not None:
            address = entry["address"]
        else:
            m = AUTO_NAME.match(name)
            if not m:
                return None
            address = int(m.group(2), 16)
            if m.group(1) == "func" and address not in self.call_targets:
                raise LinkError(f"{name}: no catalogued function or entry at {address:#x}")
        return Target(address, self.call_targets.get(address) == "thumb")


@dataclass
class Placement:
    section: Section
    base: int
    origin: str  # "catalogue" or "inferred"
    literal_offsets: dict[int, int] | None = None  # candidate record start -> output record start
    literal_data: bytes | None = field(default=None, repr=False)  # reordered candidate bytes; never copied from the image


class LinkedSection(NamedTuple):
    """A tuple-compatible link result whose display never renders its candidate buffer."""

    placement: Placement
    data: bytes

    def __repr__(self) -> str:
        return (f"LinkedSection(section={self.placement.section.index}, "
                f"address={self.placement.base:#x}, size={len(self.data)})")

    __str__ = __repr__



def _is_internal_label(sym: Symbol) -> bool:
    """ARMCC's `__switch$$` marks a jump table's base inside its function's own section: a local,
    zero-size STT_FUNC label that table entries relocate against, not a separate function."""
    return sym.name == "__switch$$" and sym.bind == "STB_LOCAL" and sym.size == 0


def _is_thumb_function(sym: Symbol) -> bool:
    return sym.kind == "STT_FUNC" and bool(sym.value & 1)


def _symbol_offset(sym: Symbol) -> int:
    """Offset inside its section; only function symbols carry the Thumb bit in their value."""
    return sym.value & ~1 if sym.kind == "STT_FUNC" else sym.value


def function_aliases(obj: ObjectFile) -> dict[int, list[Symbol]]:
    """Proven full-section entry aliases, without changing the ELF symbols.

    ARMCC gives C1/C2 and D1/D2 aliases the same section and value, but may give only
    one the function's size. A zero size alone proves nothing: require an executable
    section, a full-sized sibling and identical entry/ISA for every function symbol.
    Data symbols and interior labels never acquire a function's address or extent.
    """
    by_section: dict[int, list[Symbol]] = {}
    for sym in obj.defined_functions():
        by_section.setdefault(sym.shndx, []).append(sym)
    return {
        index: symbols for index, symbols in by_section.items()
        if obj.sections[index].executable and not obj.sections[index].nobits
        and obj.sections[index].size == len(obj.sections[index].data)
        and symbols[0].value in (0, 1)
        and all(sym.value == symbols[0].value and sym.size in (0, obj.sections[index].size)
                for sym in symbols)
        and any(sym.size == obj.sections[index].size for sym in symbols)
    }


def reject_overlaps(spans: list[tuple[int, int, str, tuple[str, str] | None]]) -> None:
    """Distinct sections can never share storage, and one COMDAT entity has one address.

    spans: (start, end, label, comdat key or None), where the key is (group signature, member
    section name). The linker keeps a single copy of a COMDAT group, so every placement of the
    same member must be the same span (merged into one), and a different span is a contradiction
    even when nothing overlaps. Every other span stays distinct.
    """
    merged = {}
    canonical: dict[tuple[str, str], tuple[int, int, str]] = {}
    for i, (lo, hi, label, comdat) in enumerate(spans):
        if comdat:
            first = canonical.setdefault(comdat, (lo, hi, label))
            if first[:2] != (lo, hi):
                raise LinkError(
                    f"COMDAT {comdat[0]} {comdat[1]} has contradictory placement spans "
                    f"in {first[2]} and {label} (witness addresses redacted)"
                )
        merged.setdefault(("comdat", comdat) if comdat else ("span", i), (lo, hi, label))
    ordered = sorted(merged.values())
    for (lo_a, hi_a, name_a), (lo_b, hi_b, name_b) in zip(ordered, ordered[1:]):
        if lo_b < hi_a:
            raise LinkError(f"{name_a} ({hi_a - lo_a:#x} bytes) and {name_b} ({hi_b - lo_b:#x} bytes) overlap")


def comdat_key(section: Section) -> tuple[str, str] | None:
    return (section.comdat, section.name) if section.comdat else None


def _sx(value: int, bits: int) -> int:
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def _arm_branch_target(insn: int, place: int) -> int:
    off = _sx(insn & 0xFFFFFF, 24) << 2
    if insn >> 28 == 0xF:  # BLX imm: H bit selects the halfword
        off |= ((insn >> 24) & 1) << 1
        return place + 8 + off  # Thumb destination (caller adds no T bit here)
    return place + 8 + off


def _thm_bl_target(hi: int, lo: int, place: int) -> int:
    off = _sx(((hi & 0x7FF) << 11) | (lo & 0x7FF), 22) << 1
    if (lo >> 11) & 0x1F == 0x1D:  # BLX: target is word aligned relative to Align(PC, 4)
        return ((place + 4) & ~3) + off
    return place + 4 + off


class InPlaceLinker:
    def __init__(self, image, functions: dict[int, dict], resolver: Resolver, bss: tuple[int, int],
                 model: LinkerModel | str = DEFAULT_LINKER_MODEL,
                 literal_model: str = DEFAULT_LITERAL_MODEL,
                 retention_roots: dict[str, str] | None = None):
        """image: object with read(address, size) -> bytes; functions: catalogue functions by address;
        bss: [start, end) of zero-initialised memory; model: branch output optimisations to reproduce;
        literal_model: an independent, default-off anonymous literal placement rule.
        retention_roots maps symbol/section globs to retention reasons (KEEP, entry, exported,
        first/last). Unknown reasons still retain; a selector that cannot be resolved is refused.
        Raw linker scripts/directives are not interpreted by this linker.
        """
        self.image = image
        self.functions = functions
        self.resolver = resolver
        self.bss = bss
        self.model = linker_model(model) if isinstance(model, str) else model
        if literal_model not in LITERAL_MODELS:
            raise LinkError(f"unknown literal model {literal_model!r}; known: {', '.join(LITERAL_MODELS)}")
        self.literal_model = literal_model
        self.retention_roots = retention_roots or {}
        self._retained_sections: dict[int, tuple[str, ...]] = {}
        self.rewrites: list[Rewrite] = []
        self.literal_rebindings: list[dict] = []
        self.removed_callees: list[dict] = []
        self._removed_sections: dict[int, dict] = {}
        self._removed_unwind: dict[int, int] = {}  # companion section -> its removed code section

    def __repr__(self) -> str:
        return f"InPlaceLinker(functions={len(self.functions)}, bss=({self.bss[0]:#x}, {self.bss[1]:#x}))"

    # -- placement -----------------------------------------------------------------------------

    def place(self, obj: ObjectFile, names_to_address: dict[str, int]) -> dict[int, Placement]:
        placements: dict[int, Placement] = {}
        offsets: dict[int, set[int]] = {}
        aliases = function_aliases(obj) if self.model.function_entry_aliases else {}
        alias_addresses: dict[int, int] = {}
        for index, symbols in aliases.items():
            if index in self._removed_sections:
                continue
            for sym in symbols:
                address = names_to_address.get(sym.name)
                if address is None and self.resolver.lookup(sym.name) is not None:
                    address = self._address_for(sym.name)
                if address is None:
                    continue
                prior = alias_addresses.setdefault(index, address)
                if prior != address:
                    raise LinkError(f"{obj.sections[index].name}: aliases resolve to different addresses")
        for fn in obj.defined_functions():
            if fn.shndx in self._removed_sections:
                continue
            # Full-section proof is required to borrow a sibling's binding, not to
            # place independently bound names through the frozen extent checks.
            if (self.model.function_entry_aliases and fn.size == 0 and fn.shndx not in aliases
                    and fn.name not in names_to_address and self.resolver.lookup(fn.name) is None):
                raise LinkError(f"{fn.name}: zero-size function is not a full-section entry alias")
            address = names_to_address.get(fn.name, alias_addresses.get(fn.shndx))
            if address is None:
                address = self._address_for(fn.name)
            sec = obj.sections[fn.shndx]
            offsets.setdefault(sec.index, set()).add(fn.value & ~1)
            if len(offsets[sec.index]) > 1:
                # Only aliases (e.g. C++ C1/C2 constructors at one offset) may share a section;
                # several distinct functions in one section need --split_sections.
                raise LinkError(f"{sec.name}: holds several functions at different offsets; compile with --split_sections")
            base = address - (fn.value & ~1)
            if base not in self.functions:
                raise LinkError(
                    f"{sec.name}: {fn.name} at section offset {fn.value & ~1:#x} implies "
                    f"section base {base:#x}, which is not a catalogued function start"
                )
            prior = placements.get(sec.index)
            if prior and prior.base != base:
                raise LinkError(f"{sec.name}: aliases {fn.name} and another name resolve to different addresses")
            placements[sec.index] = Placement(sec, base, "catalogue")
        # A data definition whose name is bound (names overlay or data_/bss_XXXXXXXX) lives at
        # that address, whether or not any code references it; its bytes are then verified.
        for sym in obj.defined_globals():
            if sym.kind == "STT_FUNC":
                continue
            bound = self.resolver.lookup(sym.name)
            if bound is None:
                continue
            base = bound.address - sym.value
            prior = placements.get(sym.shndx)
            if prior is None:
                placements[sym.shndx] = Placement(obj.sections[sym.shndx], base, "inferred")
            elif prior.base != base:
                raise LinkError(f"{sym.name}: its section placement disagrees with its bound address")
        fixed = set(placements)  # addresses decided by the catalogue or by bound names
        literal_sections = {}
        if self.literal_model == "anonymous-string-records-v1":
            for sec in obj.sections.values():
                if sec.name in (".conststring", ".constwstring") and sec.merge_strings:
                    literal_sections[sec.index] = self._literal_records(obj, sec)
        # Infer data sections from the original values their references resolve to.
        changed = True
        while changed:
            changed = False
            for placed in list(placements.values()):
                for rel in placed.section.relocs:
                    sym = rel.symbol
                    if not isinstance(sym.shndx, int) or sym.shndx not in obj.sections:
                        continue
                    if sym.shndx in literal_sections:
                        continue  # a mergeable pool has record addresses, not one affine section base
                    if sym.shndx in self._removed_sections:
                        continue  # no original address exists; only a removable call may name it
                    implied = self._original_value(rel, placed.base + rel.offset, placed.section)
                    if implied is None:
                        continue
                    if _is_thumb_function(sym):
                        implied &= ~1  # the T bit is not part of the address
                    base = implied - self._addend(rel, placed.section) - _symbol_offset(sym)
                    prior = placements.get(sym.shndx)
                    if prior is None:
                        placements[sym.shndx] = Placement(obj.sections[sym.shndx], base, "inferred")
                        changed = True
                    elif prior.base != base and sym.shndx not in fixed:
                        raise LinkError(
                            f"{obj.sections[sym.shndx].name}: references disagree on its address "
                            "(witness addresses redacted)"
                        )
                    # A reference into a section whose address is fixed (catalogue function or bound
                    # name) that disagrees with the original is a byte mismatch in the referencing
                    # section, reported by the comparison; it says nothing about placement.
        for idx, records in literal_sections.items():
            placements[idx] = self._place_literal_records(obj, obj.sections[idx], records, placements)
        # Every global definition must be bound and sit exactly at its binding, so a definition
        # reached through a section symbol and an extern reference by name always resolve to one
        # address. (Static definitions are local to the unit and need no binding.)
        for sym in obj.defined_globals():
            if sym.shndx in self._removed_sections:
                continue  # checked globally after every reference has been inlined
            placed = placements.get(sym.shndx)
            bound = self.resolver.lookup(sym.name)
            if bound is None and sym.kind == "STT_FUNC" and sym.shndx in alias_addresses:
                bound = Target(alias_addresses[sym.shndx], _is_thumb_function(sym))
            if bound is None:
                raise LinkError(
                    f"global {sym.name!r} has no address binding: name it func_/data_/bss_XXXXXXXX, "
                    f"add it to config/names.json, or make it static"
                )
            if placed is None:
                continue
            here = placed.base + _symbol_offset(sym)
            if here != bound.address:
                raise LinkError(f"{sym.name}: its defined location disagrees with its bound address")
        for p in placements.values():
            if p.base % p.section.align:
                raise LinkError(
                    f"{p.section.name}: placement is not aligned to {p.section.align} "
                    "(witness address redacted)"
                )
        reject_overlaps(
            [(p.base, p.base + p.section.size, p.section.name, comdat_key(p.section))
             for p in placements.values() if p.section.size]
        )
        return placements

    @staticmethod
    def _literal_records(obj: ObjectFile, sec: Section) -> dict[int, bytes]:
        """Parse only compiler-marked anonymous pools, including their zero alignment padding."""
        unit = 2 if sec.name == ".constwstring" else 1
        if (sec.nobits or sec.executable or sec.writable or sec.relocs or sec.comdat
                or sec.entsize != unit or sec.align not in (unit, 4)
                or sec.size != len(sec.data) or not 0 < sec.size <= 1 << 20):
            raise LinkError(f"{sec.name}: not an eligible anonymous string pool")
        symbols = [s for s in obj.symbols if s.shndx == sec.index]
        if not symbols or any(s.kind != "STT_SECTION" or s.bind != "STB_LOCAL" or s.value
                              for s in symbols):
            raise LinkError(f"{sec.name}: string pool has a named or non-section definition")
        records: dict[int, bytes] = {}
        seen: set[bytes] = set()
        offset = 0
        while offset < sec.size:
            end = offset
            while end + unit <= sec.size and any(sec.data[end:end + unit]):
                end += unit
            if end == offset or end + unit > sec.size:
                raise LinkError(f"{sec.name}+{offset:#x}: empty or unterminated string record")
            end += unit
            record = sec.data[offset:end]
            if record in seen or len(records) >= 4096:
                raise LinkError(f"{sec.name}: duplicate records or too many string records")
            records[offset] = record
            seen.add(record)
            aligned = (end + sec.align - 1) // sec.align * sec.align
            if end == sec.size:
                break
            if aligned >= sec.size or any(sec.data[end:aligned]):
                raise LinkError(f"{sec.name}+{end:#x}: invalid string alignment padding")
            offset = aligned
        return records

    def _place_literal_records(self, obj: ObjectFile, sec: Section, records: dict[int, bytes],
                               placements: dict[int, Placement]) -> Placement:
        """L1: use each incoming original pointer as a witness; no image scan or suffix merging."""
        targets: dict[int, int] = {}
        references = 0
        for source in obj.sections.values():
            for rel in source.relocs:
                if rel.symbol.shndx != sec.index or rel.type == R_ARM_NONE:
                    continue
                placed = placements.get(source.index)
                if (placed is None or rel.type not in (R_ARM_ABS32, R_ARM_TARGET1)
                        or rel.symbol.kind != "STT_SECTION" or rel.symbol.bind != "STB_LOCAL"
                        or rel.symbol.value):
                    raise LinkError(f"{sec.name}: unsupported or unplaced incoming string reference")
                offset = self._addend(rel, source)
                if offset not in records:
                    raise LinkError(f"{sec.name}+{offset:#x}: reference is not a whole-record start")
                target = self._original_value(rel, placed.base + rel.offset, source)
                if target is None or target % sec.align:
                    raise LinkError(f"{sec.name}: misaligned original string reference")
                prior = targets.setdefault(offset, target)
                if prior != target:
                    raise LinkError(f"{sec.name}+{offset:#x}: repeated string references disagree")
                references += 1
        if targets.keys() != records.keys():
            raise LinkError(f"{sec.name}: every string record needs a placed relocation witness")
        if len(set(targets.values())) != len(records):
            raise LinkError(f"{sec.name}: string record placement is not bijective")
        base = min(targets.values())
        end = base + sec.size
        if (base < self.bss[1] and self.bss[0] < end
                or any(base < address + fn["size"] and address < end
                       for address, fn in self.functions.items())):
            raise LinkError(f"{sec.name}: string pool overlaps a catalogued function or BSS")
        reordered = bytearray()
        offsets: dict[int, int] = {}
        ordered = sorted(targets, key=targets.__getitem__)
        for index, offset in enumerate(ordered):
            if targets[offset] != base + len(reordered):
                raise LinkError(f"{sec.name}: string witnesses do not cover one contiguous pool")
            offsets[offset] = len(reordered)
            reordered.extend(records[offset])
            if index + 1 < len(ordered):
                reordered.extend(bytes((-len(reordered)) % sec.align))
        if len(reordered) != sec.size:
            raise LinkError(f"{sec.name}: reordered string pool changes the section size")
        original = self._read_original(base, sec.size, f"{sec.name}: witnessed string pool")
        if original != reordered:
            raise LinkError(f"{sec.name}: complete witnessed string pool differs from the candidate records")
        self.literal_rebindings.append({
            "rule": "anonymous-string-records-v1", "section": sec.name, "base": base,
            "section_index": sec.index,
            "object": getattr(obj, "section_objects", {}).get(sec.index, str(getattr(obj, "path", ""))),
            "size": sec.size, "record_count": len(records), "reference_count": references,
            "records": [{"candidate_offset": offset, "output_offset": offsets[offset],
                         "size": len(records[offset])} for offset in records],
        })
        return Placement(sec, base, "inferred", offsets, bytes(reordered))

    def _address_for(self, name: str) -> int:
        target = self.resolver.resolve(name)
        if target.address not in self.functions:
            raise LinkError(f"{name} -> {target.address:#x} is not a catalogued function start")
        return target.address

    # -- relocation arithmetic -------------------------------------------------------------------

    def _read_original(self, address: int, size: int, context: str) -> bytes:
        """Translate only image-address failures, not exceptions from linker arithmetic."""
        try:
            return self.image.read(address, size)
        except ValueError:
            raise LinkError(
                f"{context}: original {size:#x}-byte span is outside the image "
                "(witness address redacted)"
            ) from None
        except ImageError:
            raise LinkError(
                f"{context}: original {size:#x}-byte span crosses a segment boundary "
                "(witness address redacted)"
            ) from None

    @staticmethod
    def _addend(rel: Reloc, sec: Section) -> int:
        data = sec.data
        if rel.type in (R_ARM_ABS32, R_ARM_TARGET1, R_ARM_REL32):
            return _sx(struct.unpack_from("<I", data, rel.offset)[0], 32)
        if rel.type == R_ARM_PREL31:
            return _sx(struct.unpack_from("<I", data, rel.offset)[0] & 0x7FFFFFFF, 31)
        if rel.type in (R_ARM_CALL, R_ARM_JUMP24, R_ARM_PC24):
            return _sx(struct.unpack_from("<I", data, rel.offset)[0] & 0xFFFFFF, 24) << 2
        if rel.type == R_ARM_THM_CALL:
            hi, lo = struct.unpack_from("<HH", data, rel.offset)
            return _sx(((hi & 0x7FF) << 11) | (lo & 0x7FF), 22) << 1
        if rel.type == R_ARM_THM_JUMP11:
            return _sx(struct.unpack_from("<H", data, rel.offset)[0] & 0x7FF, 11) << 1
        if rel.type == R_ARM_THM_JUMP8:
            return _sx(struct.unpack_from("<H", data, rel.offset)[0] & 0xFF, 8) << 1
        return 0

    def _original_value(self, rel: Reloc, place: int, sec: Section) -> int | None:
        """(S + A) | T as implied by the original bytes at `place`, or None for marker relocations."""
        if rel.type in (R_ARM_NONE, R_ARM_V4BX):
            return None
        if rel.type not in (R_ARM_ABS32, R_ARM_TARGET1, R_ARM_REL32, R_ARM_PREL31,
                            R_ARM_CALL, R_ARM_JUMP24, R_ARM_PC24, R_ARM_THM_CALL,
                            R_ARM_THM_JUMP11, R_ARM_THM_JUMP8):
            raise LinkError(f"{sec.name}+{rel.offset:#x}: unsupported relocation type {rel.type}")
        size = 2 if rel.type in (R_ARM_THM_JUMP11, R_ARM_THM_JUMP8) else 4
        original = self._read_original(
            place, size, f"{sec.name}+{rel.offset:#x}: placement reference to {rel.symbol.name!r}"
        )
        if rel.type in (R_ARM_ABS32, R_ARM_TARGET1):
            return struct.unpack("<I", original)[0]
        if rel.type == R_ARM_REL32:
            return (struct.unpack("<I", original)[0] + place) & 0xFFFFFFFF
        if rel.type in (R_ARM_CALL, R_ARM_JUMP24, R_ARM_PC24):
            # branch target = P + 8 + (S + A - P)
            return _arm_branch_target(struct.unpack("<I", original)[0], place) - 8
        if rel.type == R_ARM_THM_CALL:
            # branch target = P + 4 + (S + A - P)
            hi, lo = struct.unpack("<HH", original)
            return _thm_bl_target(hi, lo, place) - 4
        if rel.type == R_ARM_THM_JUMP11:
            insn = struct.unpack("<H", original)[0]
            return place + 4 + (_sx(insn & 0x7FF, 11) << 1) - 4
        if rel.type == R_ARM_THM_JUMP8:
            insn = struct.unpack("<H", original)[0]
            return place + 4 + (_sx(insn & 0xFF, 8) << 1) - 4
        if rel.type == R_ARM_PREL31:
            word = struct.unpack("<I", original)[0]
            return (_sx(word & 0x7FFFFFFF, 31) + place) & 0xFFFFFFFF

    def _symbol_target(self, sym: Symbol, obj: ObjectFile, placements: dict[int, Placement],
                       addend: int = 0) -> Target:
        if isinstance(sym.shndx, int) and sym.shndx in obj.sections:
            placed = placements.get(sym.shndx)
            if placed is None:
                raise LinkError(f"{sym.name or obj.sections[sym.shndx].name}: section was never placed")
            if placed.literal_offsets is not None:
                offset = _symbol_offset(sym) + addend
                if offset not in placed.literal_offsets:
                    raise LinkError(f"{placed.section.name}+{offset:#x}: not a string record start")
                return Target(placed.base + placed.literal_offsets[offset] - addend)
            return Target(placed.base + _symbol_offset(sym), _is_thumb_function(sym))
        if sym.shndx == "SHN_UNDEF":
            return self.resolver.resolve(sym.name)
        if sym.shndx == "SHN_ABS":
            return Target(sym.value & ~1 if sym.kind == "STT_FUNC" else sym.value, _is_thumb_function(sym))
        raise LinkError(f"{sym.name}: unsupported symbol section {sym.shndx}")

    def _apply(self, rel: Reloc, sec: Section, data: bytearray, place: int, target: Target) -> None:
        a = self._addend(rel, sec)
        s = target.address
        o = rel.offset
        t = rel.type
        if t in (R_ARM_NONE, R_ARM_V4BX):
            return
        if t in (R_ARM_ABS32, R_ARM_TARGET1):
            struct.pack_into("<I", data, o, ((s + a) | target.thumb) & 0xFFFFFFFF)
        elif t == R_ARM_REL32:
            struct.pack_into("<I", data, o, (((s + a) | target.thumb) - place) & 0xFFFFFFFF)
        elif t == R_ARM_PREL31:
            value = ((s + a) | target.thumb) - place
            if not -(1 << 30) <= value < (1 << 30):
                raise LinkError(f"{sec.name}+{o:#x}: PREL31 to {rel.symbol.name!r} overflows 31 bits")
            word = struct.unpack_from("<I", data, o)[0]
            struct.pack_into("<I", data, o, (word & 0x80000000) | (value & 0x7FFFFFFF))
        elif t in (R_ARM_CALL, R_ARM_JUMP24, R_ARM_PC24):
            insn = struct.unpack_from("<I", data, o)[0]
            off = s + a - place
            # Direct ARM->Thumb interworking (BL -> BLX) is only defined for R_ARM_CALL;
            # JUMP24/PC24 to Thumb needs a linker veneer, which this oracle never invents.
            is_bl = t == R_ARM_CALL and insn >> 24 in (0xEB, 0xFA, 0xFB)
            if not -(1 << 25) <= off < (1 << 25):
                raise LinkError(f"{sec.name}+{o:#x}: branch to {rel.symbol.name!r} out of range (needs a veneer)")
            if target.thumb:
                if not is_bl:
                    raise LinkError(f"{sec.name}+{o:#x}: B/BLcond to Thumb {rel.symbol.name!r} needs an interworking veneer")
                insn = 0xFA000000 | (((off >> 1) & 1) << 24) | ((off >> 2) & 0xFFFFFF)
            else:
                if off & 3:
                    raise LinkError(f"{sec.name}+{o:#x}: misaligned ARM branch target {rel.symbol.name!r}")
                if insn >> 28 == 0xF:  # BLX(imm) to an ARM target becomes BL
                    insn = 0xEB000000
                insn = (insn & 0xFF000000) | ((off >> 2) & 0xFFFFFF)
            struct.pack_into("<I", data, o, insn)
        elif t == R_ARM_THM_CALL:
            hi, lo = struct.unpack_from("<HH", data, o)
            if target.thumb:
                off = s + a - place
                lo = 0xF800
            else:  # BLX: target = Align(P + 4, 4) + offset, so the offset is taken from the aligned PC
                off = (s + a + 4) - ((place + 4) & ~3)
                lo = 0xE800
            if not -(1 << 22) <= off < (1 << 22):
                raise LinkError(f"{sec.name}+{o:#x}: Thumb BL to {rel.symbol.name!r} out of range")
            imm = (off >> 1) & 0x3FFFFF
            if lo == 0xE800 and imm & 1:
                raise LinkError(f"{sec.name}+{o:#x}: misaligned BLX target {rel.symbol.name!r}")
            struct.pack_into("<HH", data, o, 0xF000 | (imm >> 11), lo | (imm & 0x7FF))
        elif t == R_ARM_THM_JUMP11:
            off = s + a - place
            if not target.thumb or not -(1 << 11) <= off < (1 << 11):
                raise LinkError(f"{sec.name}+{o:#x}: Thumb B to {rel.symbol.name!r} unreachable")
            struct.pack_into("<H", data, o, 0xE000 | ((off >> 1) & 0x7FF))
        elif t == R_ARM_THM_JUMP8:
            off = s + a - place
            insn = struct.unpack_from("<H", data, o)[0]
            if not target.thumb or not -(1 << 8) <= off < (1 << 8):
                raise LinkError(f"{sec.name}+{o:#x}: Thumb Bcond to {rel.symbol.name!r} unreachable")
            struct.pack_into("<H", data, o, (insn & 0xFF00) | ((off >> 1) & 0xFF))
        else:
            raise LinkError(f"{sec.name}+{o:#x}: unsupported relocation type {t}")

    # -- armlink output optimisations ------------------------------------------------------------

    def _optimised_branch(self, rel: Reloc, sec: Section, place: int, target: Target, obj: ObjectFile,
                          placements: dict[int, Placement]) -> Rewrite | None:
        """What armlink writes instead of this relocated ARM branch under the model, or None."""
        if rel.type not in (R_ARM_CALL, R_ARM_JUMP24, R_ARM_PC24) or target.thumb or not sec.executable:
            return None  # Thumb destinations: relocated (and interworked or refused) as usual
        insn = struct.unpack_from("<I", sec.data, rel.offset)[0]
        cond = insn >> 28
        if cond == 0xF or (insn >> 25) & 7 != 0b101:
            return None  # BLX(imm), or a data word under a branch relocation
        is_call = bool(insn >> 24 & 1)
        dest = target.address + self._addend(rel, sec) + 8
        if dest == place + 4:
            if not self.model.branchnop:
                return None
            if is_call or cond != _COND_AL:
                raise LinkError(
                    f"{sec.name}+{rel.offset:#x}: {'BL' if is_call else 'conditional B'} to the next instruction: "
                    f"armlink's --branchnop output for it is not established (docs/LINKER.md)"
                )
            return Rewrite(sec.index, rel.offset, "branchnop", dest, ARM_LINKER_NOP)
        if not (is_call and self.model.inline):
            return None
        inlined = self._inline_decision(insn, rel, sec, dest, obj, placements)
        return Rewrite(sec.index, rel.offset, inlined[0], dest, inlined[1]) if inlined else None

    def _inline_decision(self, insn: int, rel: Reloc, sec: Section, dest: int, obj: ObjectFile,
                         placements: dict[int, Placement]) -> tuple[str, int] | None:
        """Rules I1-I3 for the BL `insn` to `dest`, decided from the ORIGINAL image only.

        The callee is the original function at `dest` when the catalogue makes it an unambiguous
        candidate (`eligible_original_callee`). A callee the candidate object defines itself must be
        that same function: when either its own section or the original would be inlined, its
        whole, relocation-free section must equal the eligible original body, or the link is
        refused — otherwise a wrong local helper could decide the caller's bytes. Retention
        keeps the callee's section, but never waives these checks or prevents I1-I3 inlining.
        When neither would be inlined the call is an ordinary BL and nothing is decided.
        """
        original = self.eligible_original_callee(dest)
        decided = inline_arm_call(insn, original) if original is not None else None
        sym = rel.symbol
        if isinstance(sym.shndx, int) and sym.shndx in obj.sections:
            local = self._local_callee_section(sym, dest, obj, placements)
            candidate = inline_arm_call(insn, local) if local is not None else None
            if (decided or candidate) and (original is None or local != original):
                why = ("is not an ordinary 4/8-byte ARM function with a single entry in the catalogue"
                       if original is None else
                       "is defined in this object with other bytes than the original"
                       if local is not None else "is defined in this object by a section the model cannot judge")
                raise LinkError(
                    f"{sec.name}+{rel.offset:#x}: call to {sym.name or obj.sections[sym.shndx].name}: "
                    f"armlink inlining is undecidable, the callee {why} (docs/LINKER.md)"
                )
        return decided

    def eligible_original_callee(self, dest: int) -> bytes | None:
        """The original bytes of the function at `dest` when the catalogue makes it an unambiguous
        inlining candidate: an ordinary top-level ARM `function` of 4 or 8 bytes, all code, no
        literal pool, no entry but its start. Its section cannot be longer than that extent: only
        literal data could follow its return, and a callee that loads it reads the PC and is never
        inlined. None for anything else (interior entries, multi-entry owners, veneers, Thumb):
        such a call stays a BL."""
        fn = self.functions.get(dest)
        if fn is None or fn.get("isa") != "arm" or fn.get("kind", "function") != "function":
            return None
        if fn["size"] not in (4, 8) or fn.get("code_size", fn["size"]) != fn["size"]:
            return None
        if fn.get("pools") or fn.get("literals") or any(e["address"] != dest for e in fn.get("entries", ())):
            return None
        return self._read_original(dest, fn["size"], f"function {dest:#x}: inline-callee body")

    @staticmethod
    def _local_callee_section(sym: Symbol, dest: int, obj: ObjectFile, placements: dict[int, Placement]) -> bytes | None:
        """The whole section of a callee defined in this object, when it starts at `dest` and holds
        only executable, relocation-free bytes; None when the model cannot judge it."""
        callee = obj.sections[sym.shndx]
        placed = placements.get(sym.shndx)
        if placed is None or dest != placed.base or not callee.executable or callee.nobits:
            return None
        if any(r.type not in (R_ARM_NONE, R_ARM_V4BX) for r in callee.relocs):
            return None
        return callee.data

    def _retention_sections(self, obj: ObjectFile) -> dict[int, tuple[str, ...]]:
        """DUI0474K §4.3 roots survive independently of whether their calls inline."""
        retained = {index: section.retention for index, section in obj.sections.items() if section.retention}
        for sym in obj.symbols:
            if sym.shndx in obj.sections and sym.name.startswith("__tagsym$$"):
                retained[sym.shndx] = (*retained.get(sym.shndx, ()), sym.name)
        for selector, reason in self.retention_roots.items():
            matches = {index for index, section in obj.sections.items() if fnmatchcase(section.name, selector)}
            matches.update(sym.shndx for sym in obj.symbols
                           if sym.shndx in obj.sections and fnmatchcase(sym.name, selector))
            if not matches:
                raise LinkError(f"uninterpretable retention root {selector!r}: no supplied symbol/section")
            for index in matches:
                retained[index] = (*retained.get(index, ()), reason)
        groups = {obj.sections[index].comdat for index in retained} - {None}
        for index, section in obj.sections.items():
            if section.comdat in groups and index not in retained:
                retained[index] = ("retained COMDAT group",)
        return retained

    def _candidate_only_callees(self, obj: ObjectFile, names_to_address: dict[str, int]) -> dict[int, dict]:
        """The v3 exception never competes with an original function/entry, including aliases."""
        if not self.model.removed_inline:
            return {}
        by_section: dict[int, list[Symbol]] = {}
        for sym in obj.defined_functions():
            by_section.setdefault(sym.shndx, []).append(sym)
        entry_aliases = function_aliases(obj) if self.model.function_entry_aliases else {}
        non_function_globals = ({sym.shndx for sym in obj.defined_globals() if sym.kind != "STT_FUNC"}
                                if self.model.function_entry_aliases else set())
        # V6 resolves every copy before selecting any removal. A section-local alias binding
        # cannot turn another copy into an unbound R1 entity; older models keep their scan below.
        bindings: dict[int, list[int]] = {}
        if self.model.function_entry_aliases:
            for sym in obj.symbols:
                if sym.shndx not in obj.sections or not sym.name or sym.name.startswith("$"):
                    continue
                target = self.resolver.lookup(sym.name)
                address = names_to_address.get(sym.name, target.address if target else None)
                if address is not None:
                    bindings.setdefault(sym.shndx, []).append(address - _symbol_offset(sym))
        removed = {}
        for index, aliases in by_section.items():
            section = obj.sections[index]
            bound = bindings.get(index, ())
            if not self.model.function_entry_aliases:
                bound = []
                for sym in obj.symbols:
                    if sym.shndx != index or not sym.name or sym.name.startswith("$"):
                        continue
                    target = self.resolver.lookup(sym.name)
                    address = names_to_address.get(sym.name, target.address if target else None)
                    if address is not None:
                        bound.append(address - _symbol_offset(sym))
            # A binding is a claim about an extant function, not licence to erase it. Include
            # interior entries/extents, so an alias cannot disguise a catalogued counterpart.
            if any(lo < address + section.size and address < lo + fn["size"]
                   for address in bound for lo, fn in self.functions.items()):
                continue
            if bound:
                raise LinkError(f"{section.name}: removed callee must have no original address binding")
            if (not section.executable or section.nobits or section.size != len(section.data)
                    or section.size not in (4, 8) or section.align < 4
                    or (index not in entry_aliases or any(sym.value != 0 for sym in aliases)
                        if self.model.function_entry_aliases
                        else any(sym.value != 0 or sym.size != section.size for sym in aliases))
                    or index in non_function_globals
                    or any(kind != "$a" or offset != 0 for offset, kind in section.mapping)
                    or any(rel.type != R_ARM_V4BX for rel in section.relocs)
                    or inline_arm_call(0xEB000000, section.data) is None):
                continue
            if section.size == 8:
                insn = next(_ARM.disasm(section.data[:4], 0, 1))
                read, written = map(set, insn.regs_access())
                if (read - _ARM_ARGUMENTS - {cs_arm.ARM_REG_CPSR}
                        or written - _ARM_ARGUMENTS - _ARM_CALLER_SAVED - {cs_arm.ARM_REG_CPSR}):
                    continue  # a tiny source callee must itself respect the call ABI
            if index in self._retained_sections:
                reasons = ", ".join(self._retained_sections[index])
                raise LinkError(f"{section.name}: retained callee ({reasons}); its call must remain a BL")
            removed[index] = {
                "symbols": sorted(sym.name for sym in aliases), "section": section.name, "size": section.size,
                "object": getattr(obj, "section_objects", {}).get(index, str(getattr(obj, "path", "candidate"))),
            }
            if section.comdat:
                removed[index]["comdat"] = section.comdat
        removed_comdats = {comdat_key(obj.sections[index]) for index in removed} - {None}
        comdats = {}
        copies = (obj.sections.items() if self.model.function_entry_aliases
                  else ((index, obj.sections[index]) for index in removed))
        for index, section in copies:
            key = comdat_key(section)
            if key is not None:
                if key not in removed_comdats:
                    continue
                if bindings.get(index):
                    raise LinkError(f"COMDAT {key[0]} {key[1]} mixes original-bound and removable callee copies")
                if index not in removed:
                    raise LinkError(f"COMDAT {key[0]} {key[1]} has an unsupported retained callee copy")
                prior = comdats.setdefault(key, section)
                if section.data != prior.data or section.mapping != prior.mapping:
                    raise LinkError(f"COMDAT {key[0]} {key[1]} has contradictory removed-callee copies")
        return removed

    @staticmethod
    def _compact_unwind_prefix_size(word: int) -> int | None:
        """Header plus compact unwind words only; an out-of-line descriptor terminator follows."""
        personality = word >> 24
        if personality == 0x80:
            return 4
        if personality in (0x81, 0x82):
            return 4 * (1 + ((word >> 16) & 0xFF))
        return None

    def _removed_unwind_sections(self, obj: ObjectFile, names: dict[str, int]) -> dict[int, int]:
        """V8: an exclusive SHF_LINK_ORDER companion dies only with its complete R1/group closure.

        This is not general unwind-table placement or exception-table stripping. In particular,
        kept code's entries, shared extab data, roots and original-bound metadata never disappear.
        """
        if not self.model.removed_unwind or not self._removed_sections:
            return {}
        companions: dict[int, int] = {}
        exidx_by_owner: dict[int, int] = {}
        incoming: dict[int, list[tuple[Section, Reloc]]] = {}
        for section in obj.sections.values():
            for rel in section.relocs:
                if rel.type != R_ARM_V4BX:
                    incoming.setdefault(rel.symbol.shndx, []).append((section, rel))

        def private_data(section: Section, owner: int, kind: str) -> None:
            if (not section.name.startswith(f".ARM.{kind}") or section.nobits or section.executable
                    or section.writable or section.size != len(section.data) or section.align < 4
                    or section.size % 4 or section.merge_strings
                    or section.comdat != obj.sections[owner].comdat
                    or section.index in self._retained_sections):
                raise LinkError(f"{section.name}: not an exclusive unrooted {kind} companion")
            for sym in obj.symbols:
                if sym.shndx != section.index:
                    continue
                if sym.name and (sym.name in names or self.resolver.lookup(sym.name) is not None):
                    raise LinkError(f"{section.name}: unwind companion has an original address binding")
                if not (sym.bind == "STB_LOCAL" and sym.value == 0 and sym.size == 0
                        and (sym.kind == "STT_SECTION" or sym.kind == "STT_NOTYPE" and sym.name == "$d")):
                    raise LinkError(f"{section.name}: unwind companion has an independently named entity")

        def dependencies(section: Section, personality: int, structural: list[Reloc]) -> None:
            for rel in section.relocs:
                if any(rel is known for known in structural):
                    continue
                if (rel.type != R_ARM_NONE or rel.offset != 0
                        or rel.symbol.shndx != "SHN_UNDEF"
                        or rel.symbol.name != f"__aeabi_unwind_cpp_pr{personality}"):
                    raise LinkError(f"{section.name}: unsupported unwind relocation or dependency")

        for section in obj.sections.values():
            references = [rel for rel in section.relocs
                          if rel.type != R_ARM_V4BX and rel.symbol.shndx in self._removed_sections]
            linked_owner = section.linked_section if section.link_order else None
            if not references and linked_owner not in self._removed_sections:
                continue
            # Ordinary call/data references still go through the unchanged R1 all-reference test.
            if not (section.name.startswith(".ARM.exidx") or section.elf_type == "SHT_ARM_EXIDX"
                    or linked_owner in self._removed_sections):
                continue
            owner = linked_owner
            if (owner not in self._removed_sections or section.elf_type != "SHT_ARM_EXIDX"
                    or section.size != 8 or section.entsize not in (0, 8)
                    or owner in exidx_by_owner):
                raise LinkError(f"{section.name}: shared, combined or unassociated unwind index")
            private_data(section, owner, "exidx")
            first, second = struct.unpack("<2I", section.data)
            keys = [rel for rel in section.relocs if rel.offset == 0 and rel.type == R_ARM_PREL31]
            if (len(keys) != 1 or keys[0].symbol.shndx != owner or first & 0x80000000
                    or keys[0].symbol.kind not in ("STT_SECTION", "STT_FUNC")
                    or _symbol_offset(keys[0].symbol) + self._addend(keys[0], section) != 0):
                raise LinkError(f"{section.name}: unwind key is not the removed helper's exact entry")
            if incoming.get(section.index):
                raise LinkError(f"{section.name}: unwind index has another incoming reference")
            structural = [keys[0]]
            tails = [rel for rel in section.relocs if rel.offset == 4 and rel.type == R_ARM_PREL31]
            if tails:
                tail = tails[0]
                extab = obj.sections.get(tail.symbol.shndx)
                if (len(tails) != 1 or second & 0x80000000 or extab is None
                        or _symbol_offset(tail.symbol) + self._addend(tail, section) != 0
                        or extab.index in companions):
                    raise LinkError(f"{section.name}: shared or interior exception table")
                private_data(extab, owner, "extab")
                if extab.elf_type != "SHT_PROGBITS" or extab.link_order or extab.size < 8 or extab.entsize:
                    raise LinkError(f"{extab.name}: unsupported exception-table section metadata")
                header = struct.unpack_from("<I", extab.data)[0]
                prefix_size = self._compact_unwind_prefix_size(header)
                if (prefix_size is None or extab.size != prefix_size + 4
                        or struct.unpack_from("<I", extab.data, prefix_size)[0] != 0):
                    raise LinkError(f"{extab.name}: truncated, combined or nonempty-descriptor exception-table entry")
                if incoming.get(extab.index) != [(section, tail)]:
                    raise LinkError(f"{extab.name}: exception table has another incoming reference")
                dependencies(extab, (header >> 24) & 0xF, [])
                companions[extab.index] = owner
                structural.append(tail)
                # Personality dependence lives in extab, never an unrelated exidx dependency.
                if len(section.relocs) != 2:
                    raise LinkError(f"{section.name}: unsupported unwind relocation or dependency")
            else:
                if second != 1 and second >> 24 != 0x80:
                    raise LinkError(f"{section.name}: unsupported inline unwind entry")
                dependencies(section, 0, structural)
                if second == 1 and len(section.relocs) != 1:
                    raise LinkError(f"{section.name}: CANTUNWIND entry has an unexpected dependency")
            companions[section.index] = owner
            exidx_by_owner[owner] = section.index

        # Section links are dependencies too, even without an allocated relocation.
        for section in obj.sections.values():
            if (section.link_order and section.linked_section in (self._removed_sections | companions)
                    and section.index not in companions):
                raise LinkError(f"{section.name}: another linked section keeps removed unwind/code live")
        # A group is indivisible, including all supplied copies and allocated members that
        # placement would otherwise ignore. Only already-selected R1 code and validated own
        # unwind sections have a removal proof; do not infer general unused-data elimination.
        groups = {obj.sections[owner].comdat for owner in exidx_by_owner} - {None}
        group_members = {
            signature: {index for index, section in obj.sections.items() if section.comdat == signature}
            for signature in groups
        }
        exempt = companions.keys() | exidx_by_owner.keys()
        for flags, signature, members in getattr(obj, "section_groups", ()):
            if (flags & 1 and signature in groups) or members & exempt:
                if flags != 1:
                    raise LinkError("unwind companion belongs to an unsupported section group")
                group_members.setdefault(signature, set()).update(members)
        for signature, members in sorted(group_members.items()):
            for index in sorted(members, key=str):
                member = obj.sections.get(index)
                if (member is None or member.comdat != signature
                        or index not in self._removed_sections and index not in companions):
                    name = member.name if member is not None else "unrepresented allocated member"
                    raise LinkError(f"COMDAT {signature}: {name} has no complete group-removal proof")
        for owner in exidx_by_owner:
            self._removed_sections[owner]["unwind_sections"] = [
                {"section": obj.sections[index].name, "section_index": index,
                 "size": obj.sections[index].size, "kind": "exidx" if index == exidx_by_owner[owner] else "extab",
                 "object": getattr(obj, "section_objects", {}).get(index, str(getattr(obj, "path", "candidate")))}
                for index in sorted(companions) if companions[index] == owner
            ]
        return companions

    def _removed_call_rewrites(self, obj: ObjectFile, placements: dict[int, Placement]) -> dict[tuple[int, int], Rewrite]:
        """Inline every reference to candidate-only callees, or refuse the entire link.

        Inspect all allocated sections, not only ones placement happened to reach: pointers,
        dependencies, B tail calls, interior/addended calls and incompatible I3 conditions must
        not leave a removed function live. No general unused-section elimination is attempted.
        """
        if not self._removed_sections:
            return {}
        rewrites = {}
        used = set()
        contexts = {}
        for section in obj.sections.values():
            if section.index in self._removed_unwind:
                continue
            for rel in section.relocs:
                index = rel.symbol.shndx
                if index not in self._removed_sections or rel.type == R_ARM_V4BX:
                    continue
                placed = placements.get(section.index)
                decision = None
                if (placed is not None and placed.origin == "catalogue" and section.executable
                        and rel.type in (R_ARM_CALL, R_ARM_JUMP24, R_ARM_PC24)
                        and rel.offset % 4 == 0 and rel.offset + 4 <= len(section.data)
                        and _symbol_offset(rel.symbol) + self._addend(rel, section) + 8 == 0):
                    word = struct.unpack_from("<I", section.data, rel.offset)[0]
                    if word >> 28 != 0xF and (word >> 24) & 0xF == 0xB:
                        decision = inline_arm_call(word, obj.sections[index].data)
                if decision is not None:
                    fn = self.functions[placed.base]
                    site = placed.base + rel.offset
                    if placed.base not in contexts:
                        instructions = {}
                        for lo, hi in fn.get("code_ranges", [(placed.base, placed.base + fn["size"])]):
                            if self.model.bounded_switches:
                                lo, hi = max(lo, fn["address"]), min(hi, fn["address"] + fn["size"])
                                if lo >= hi:
                                    continue
                            original = self._read_original(lo, hi - lo, f"{section.name}: removed-call context")
                            for insn in _ARM.disasm(original, lo):
                                if self.model.bounded_switches:
                                    if (insn.address % 4 or insn.size != 4 or insn.address + 4 > hi
                                            or any(a < insn.address + 4 and insn.address < b
                                                   for a, b in (*fn.get("pools", ()), *fn.get("literals", ())))):
                                        continue
                                elif any(a <= insn.address < b
                                         for a, b in (*fn.get("pools", ()), *fn.get("literals", ()))):
                                    continue
                                instructions[insn.address] = insn
                        words = arm_pool_words(self.image, fn) if self.model.bounded_switches else None
                        call_sites = {
                            placed.base + relocation.offset for relocation in section.relocs
                            if relocation.symbol.shndx in self._removed_sections
                            and relocation.type in (R_ARM_CALL, R_ARM_JUMP24, R_ARM_PC24)
                        }
                        contexts[placed.base] = instructions, call_sites, words
                    instructions, call_sites, words = contexts[placed.base]
                    replacement = next(_ARM.disasm(struct.pack("<I", decision[1]), site, 1), None)
                    arguments = set(replacement.regs_access()[0]) & _ARM_ARGUMENTS if obj.sections[index].size == 8 else set()
                    if (fn["isa"] != "arm" or site not in instructions
                            or arm_call_abi_context(instructions, site, arguments, replacement.cc,
                                                    {placed.base, *(entry["address"] for entry in fn.get("entries", ()))},
                                                    call_sites, words, bounded_switches=self.model.bounded_switches) != "consistent"
                            or next((kind for offset, kind in reversed(section.mapping) if offset <= rel.offset), "$a") != "$a"):
                        decision = None
                if decision is None:
                    raise LinkError(
                        f"{section.name}+{rel.offset:#x}: removed callee {obj.sections[index].name} is still referenced "
                        "(requires an entry-only ARM BL, I1-I3 eligibility and call-ABI data-flow)"
                    )
                rewrites[(section.index, rel.offset)] = Rewrite(
                    section.index, rel.offset, decision[0], None, decision[1], self._removed_sections[index],
                )
                used.add(index)
        used_comdats = {comdat_key(obj.sections[index]) for index in used} - {None}
        used.update(index for index in self._removed_sections if comdat_key(obj.sections[index]) in used_comdats)
        for index in self._removed_sections:
            if index not in used:
                raise LinkError(f"{obj.sections[index].name}: candidate-only callee has no inlined call; cannot remove it")
        self.removed_callees = [self._removed_sections[index] for index in sorted(used)]
        return rewrites

    # -- link + verify -------------------------------------------------------------------------

    def link(self, obj: ObjectFile, names_to_address: dict[str, int] | None = None) -> dict[int, LinkedSection]:
        """Place and relocate every referenced section; returns {section index: (placement, bytes)}.

        Inferred (non-function) sections are verified here: their relocated bytes must equal the
        original image, BSS must lie in the BSS range. Function sections are returned unverified so
        the caller can report per-function differences. Branches the linker model rewrote instead of
        relocating are listed in `self.rewrites` (reset by each call).
        """
        self.literal_rebindings = []
        self.rewrites, self.removed_callees = [], []
        self._removed_unwind = {}
        self._retained_sections = self._retention_sections(obj) if self.model.removed_inline else {}
        self._removed_sections = self._candidate_only_callees(obj, names_to_address or {})
        self._removed_unwind = self._removed_unwind_sections(obj, names_to_address or {})
        placements = self.place(obj, names_to_address or {})
        removed_rewrites = self._removed_call_rewrites(obj, placements)
        linked: dict[int, LinkedSection] = {}
        for idx, placed in placements.items():
            sec = placed.section
            if sec.nobits:
                lo, hi = self.bss
                if not (lo <= placed.base and placed.base + sec.size <= hi):
                    raise LinkError(
                        f"{sec.name}: placement is outside BSS [{lo:#x}, {hi:#x}) "
                        "(witness address redacted)"
                    )
                linked[idx] = LinkedSection(placed, b"")
                continue
            data = bytearray(placed.literal_data if placed.literal_data is not None else sec.data)
            for rel in sec.relocs:
                if rel.type == R_ARM_V4BX:
                    continue  # marker; its symbol may be null and is never resolved
                removed = removed_rewrites.get((idx, rel.offset))
                if removed is not None:
                    struct.pack_into("<I", data, rel.offset, removed.word)
                    self.rewrites.append(removed)
                    continue
                if rel.type == R_ARM_NONE:
                    # A dependency: a section of this object it names must be part of the output,
                    # which here means placed (and so verified); library symbols are external.
                    sym = rel.symbol
                    if isinstance(sym.shndx, int) and sym.shndx in obj.sections and sym.shndx not in placements:
                        raise LinkError(f"{sec.name} depends on {obj.sections[sym.shndx].name}, which no reference places")
                    continue
                place = placed.base + rel.offset
                target = self._symbol_target(rel.symbol, obj, placements, self._addend(rel, sec))
                rewrite = self._optimised_branch(rel, sec, place, target, obj, placements)
                if rewrite is not None:
                    struct.pack_into("<I", data, rel.offset, rewrite.word)
                    self.rewrites.append(rewrite)
                    continue
                self._apply(rel, sec, data, place, target)
            linked[idx] = LinkedSection(placed, bytes(data))
            if placed.origin == "inferred":
                original = self._read_original(
                    placed.base, sec.size, f"{sec.name}: section placed from its references"
                )
                if original != bytes(data):
                    first = next(i for i in range(sec.size) if original[i] != data[i])
                    raise LinkError(
                        f"{sec.name}: placed from its references, but its bytes differ from "
                        f"the original at +{first:#x} (witness address redacted)"
                    )
        return linked
