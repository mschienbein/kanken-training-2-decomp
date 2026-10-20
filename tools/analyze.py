"""Conservative ARM11 function and literal-pool catalogue (no ROM contents emitted).

Run ``.venv/bin/python tools/analyze.py catalogue``. The output is a candidate,
not a frozen matching denominator. Discovery combines executable call edges,
external pointers, linker veneers and prologues in otherwise unexplained gaps.
``code_ranges`` makes sparse recursive-descent coverage explicit; the enclosing
``size`` includes trailing pools, but unexplained holes remain unknown ranges.
Boundary refinement uses ARMCC split-section literal ownership, without discarding
observed call targets or callback-table entries. ``boundary_merges`` records every
removed weak start; ``noreturn`` means the recovered normal-flow graph is closed,
not merely that a return opcode was absent from an incomplete trace.
PC-relative address-taken strings/data are finalized only after discovery: their
byte ranges live in ``literals``, and shared or boundary-crossing objects remain
explicit conflicts rather than being stolen from another function.
Non-C fallthrough chains and externally callable interior labels are represented
as ``multi_entry`` routines with explicit, ISA-checked ``entries``.
ARM ``mov r0, r0`` entry wrappers and proved internal tail boundaries use the
``armlink5-inline-branchnop-v2`` layout rule (docs/LINKER.md), not generic NOP padding.
Leading-instruction recovery requires a register dependency, a bounded straight-line
prefix and an independently delimited gap; pointer recovery also checks closed CFGs.
Reviewed runtime/ABI classifications are explicit, image/extent-bound inputs, never
inferred from an instruction-set choice or an address neighborhood.
"""
from __future__ import annotations

import argparse
from bisect import bisect_right
from collections import Counter
from dataclasses import dataclass, field
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import sys
import time

import capstone
from capstone import arm

if __package__:
    from . import ctrfmt
else:
    import ctrfmt

ROOT = Path(__file__).resolve().parents[1]

# Capstone does not reliably mark the memory operand WRITE access on SWP/STREX.
# Exclusive loads also change reservation state and are not benign setup loads.
MEMORY_SIDE_EFFECTS = frozenset(
    getattr(arm, f"ARM_INS_{name}") for name in (
        "CLREX", "LDAEX", "LDAEXB", "LDAEXD", "LDAEXH",
        "LDREX", "LDREXB", "LDREXD", "LDREXH", "PUSH",
        "SRSDA", "SRSDB", "SRSIA", "SRSIB", "STC", "STC2", "STC2L", "STCL",
        "STL", "STLB", "STLEX", "STLEXB", "STLEXD", "STLEXH", "STLH",
        "STM", "STMDA", "STMDB", "STMIB", "STR", "STRB", "STRBT", "STRD",
        "STREX", "STREXB", "STREXD", "STREXH", "STRH", "STRHT", "STRT",
        "SWP", "SWPB", "VPUSH", "VST1", "VST2", "VST3", "VST4",
        "VSTMDB", "VSTMIA", "VSTR",
    )
)


def _memory_side_effect(instruction) -> bool:
    return (instruction.id in MEMORY_SIDE_EFFECTS
            or any(op.type == arm.ARM_OP_MEM and op.access & capstone.CS_AC_WRITE
                   for op in instruction.operands))


def _signed(value: int, bits: int) -> int:
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def _immediate(word: int) -> int:
    value, shift = word & 255, ((word >> 8) & 15) * 2
    return ((value >> shift) | (value << (32 - shift))) & 0xFFFFFFFF if shift else value


def _merge(intervals: list[tuple[int, int]]) -> list[list[int]]:
    merged: list[list[int]] = []
    for start, end in sorted(intervals):
        if merged and start <= merged[-1][1]:
            merged[-1][1] = max(end, merged[-1][1])
        elif start < end:
            merged.append([start, end])
    return merged


@dataclass(slots=True)
class Instruction:
    size: int
    conditional: bool = False
    call: bool = False
    branch: bool = False
    stop: bool = False
    returns: bool = False
    indirect: bool = False
    target: tuple[int, str] | None = None
    pools: tuple[tuple[int, int], ...] = ()
    address_reference: bool = False
    switch: tuple[str, int] | None = None
    terminal: str | None = None
    address_target: int | None = None
    pc_add_register: int | None = None
    register_constant: tuple[int, int] | None = None
    register_pool: tuple[int, int] | None = None


@dataclass(slots=True)
class Seed:
    isa: str
    evidence: set[str] = field(default_factory=set)


@dataclass(slots=True)
class Trace:
    address: int
    isa: str
    kind: str = "function"
    code: dict[int, int] = field(default_factory=dict)
    pools: set[int] = field(default_factory=set)
    table_words: set[int] = field(default_factory=set)
    calls: set[tuple[int, str, int]] = field(default_factory=set)
    tails: set[tuple[int, str, int]] = field(default_factory=set)
    issues: set[tuple[int, str]] = field(default_factory=set)
    indirect_exits: int = 0
    switches: int = 0
    return_sites: set[int] = field(default_factory=set)
    open_exits: set[int] = field(default_factory=set)
    address_words: set[int] = field(default_factory=set)
    address_refs: dict[int, int] = field(default_factory=dict)
    literals: list[list[int]] = field(default_factory=list)
    literal_kinds: Counter[str] = field(default_factory=Counter)
    members: set[int] = field(default_factory=set)
    entries: dict[int, dict] = field(default_factory=dict)

    @property
    def end(self) -> int:
        return max([self.address] + [a + n for a, n in self.code.items()]
                   + [a + 4 for a in self.pools] + [end for _, end in self.literals])


class Analyzer:
    """One-image analysis; cached decoding avoids repeated Capstone allocations."""

    def __init__(self, image: bytes, layout: ctrfmt.CodeLayout):
        if len(image) != layout.image_size:
            raise ValueError(f"image size {len(image)} != exheader size {layout.image_size}")
        ordered = sorted(layout.segments, key=lambda segment: segment.address)
        for segment in ordered:
            if segment.address % 4 or not 0 <= segment.size <= segment.mapped_size:
                raise ValueError(f"invalid segment layout: {segment.name}")
        for left, right in zip(ordered, ordered[1:]):
            if left.address + left.mapped_size > right.address:
                raise ValueError("exheader segments overlap")
        self.image = image
        self.layout = layout
        self.base = layout.text.address
        self.end = self.base + layout.text.size
        offset = layout.file_offset(self.base)
        self.text = image[offset:offset + layout.text.size]
        self.arm = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)
        self.thumb = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
        self.thumb.detail = True
        self.cache: dict[tuple[int, str], Instruction | None] = {}
        self.seeds: dict[int, Seed] = {}
        self.raw_calls: dict[tuple[int, str], set[int]] = {}
        self.discovery_rounds = 0
        self.rejected_call_seeds = 0
        self.internal_starts: set[int] = set()
        self.boundary_merges: list[dict] = []
        self.noreturn: dict[int, list[str]] = {}
        self.protected_pointer_starts: set[int] = set()
        self.refining = False
        self.literal_conflicts: list[dict] = []
        self.shared_literals: list[dict] = []
        self.literal_encodings: dict[tuple[int, int], dict] = {}
        self.relative_table_pointers: set[tuple[int, str]] = set()
        self.relative_pointer_tables: list[dict] = []
        self.protected_entry_observations: set[tuple[int, int, int]] = set()
        self.protected_start_observations: set[tuple[int, str]] = set()
        self.literal_extent_changes: Counter[str] = Counter()
        self.shared_literal_words: set[int] = set()
        self.entry_groups: list[dict] = []
        self.tail_target_repairs: dict[tuple[int, str], set[int]] = {}
        self.linker_wrappers: set[int] = set()
        self.pointer_target_repairs: set[tuple[int, str]] = set()
        self.leading_instruction_repairs: list[dict] = []
        self.leading_instruction_inventory: dict[int, dict] = {}
        self.pointer_entry_inventory: dict[tuple[int, str], dict] = {}
        self.formed_code_pointers: list[dict] = []
        self.pointer_prefix_wrappers: dict[int, int] = {}
        self.detailed_arm = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)
        self.detailed_arm.detail = True
        self.detail_cache: dict[int, object] = {}

    def __repr__(self) -> str:
        return f"Analyzer(size={len(self.image)}, text=({self.base:#x}, {self.end:#x}))"

    def inside(self, address: int, isa: str = "arm") -> bool:
        return self.base <= address < self.end and address % (4 if isa == "arm" else 2) == 0

    def word(self, address: int) -> int:
        return struct.unpack_from("<I", self.text, address - self.base)[0]

    def decode(self, address: int, isa: str) -> Instruction | None:
        key = address, isa
        if key in self.cache:
            return self.cache[key]
        result = self._decode_arm(address) if isa == "arm" else self._decode_thumb(address)
        self.cache[key] = result
        return result

    def _decode_arm(self, address: int) -> Instruction | None:
        if not self.inside(address) or address + 4 > self.end:
            return None
        word = self.word(address)
        if not word:
            return None  # Zero alignment is not evidence for executable ANDEQ instructions.
        offset = address - self.base
        decoded = next(self.arm.disasm_lite(self.text[offset:offset + 4], address, count=1), None)
        if decoded is None or decoded[2] in {"udf", "udf.w"}:
            return None
        ins = Instruction(4, conditional=word >> 28 not in (14, 15))
        if word & 0xFE000000 == 0xFA000000:
            ins.call = True
            ins.target = (address + 8 + (_signed(word & 0xFFFFFF, 24) << 2) + ((word >> 23) & 2), "thumb")
        elif word >> 28 == 15:
            # Unconditional-space PLD/PLI encodings resemble LDRB pc,[pc,...],
            # but prefetching instruction bytes neither exits nor owns a pool.
            return ins
        elif word & 0x0FFFFFFF == 0x0F000003:
            ins.terminal = "svc_exit_process"
        elif word & 0x0E000000 == 0x0A000000:
            ins.target = (address + 8 + (_signed(word & 0xFFFFFF, 24) << 2), "arm")
            ins.call = bool(word & 0x01000000)
            ins.branch = not ins.call
        elif word & 0x0FFFFFF0 in (0x012FFF10, 0x012FFF30):
            ins.call = word & 0x0FFFFFF0 == 0x012FFF30
            ins.stop = not ins.call
            ins.returns = ins.stop and word & 15 == 14
            ins.indirect = not ins.returns
        elif word & 0x0FFFFFF0 == 0x008FF100:
            ins.switch = ("branches", word & 15)
            ins.stop = True
        elif word & 0x0FFFFFF0 == 0x079FF100:
            ins.switch = ("addresses", word & 15)
            ins.stop = True
        else:
            # ARM/VFP literal addressing uses the architectural PC (+8), not the symbol end.
            if word & 0x0E1F0000 == 0x041F0000 and word & 0x01000000:
                literal = address + 8 + ((word & 0xFFF) if word & 0x00800000 else -(word & 0xFFF))
                width = 1 if word & 0x00400000 else 4
                ins.pools = ((literal & ~3, (literal + width + 3) & ~3),)
                if (word >> 12) & 15 == 15:
                    ins.stop = ins.indirect = True
                    if literal % 4 == 0 and self.base <= literal <= self.end - 4:
                        pointer = self.word(literal)
                        ins.target = (pointer & ~1, "thumb" if pointer & 1 else "arm")
            elif word & 0x0F3F0E00 == 0x0D1F0A00:
                literal = address + 8 + ((word & 255) * 4 if word & 0x00800000 else -(word & 255) * 4)
                width = 8 if word & 0x100 else 4
                ins.pools = ((literal, literal + width),)
            elif word & 0x0E1F0000 == 0x020F0000 and (word >> 21) & 15 in (2, 4):
                literal = address + 8 + (_immediate(word) if (word >> 21) & 15 == 4 else -_immediate(word))
                ins.pools = ((literal & ~3, (literal + 7) & ~3),)
                ins.address_reference = True
                if (word >> 12) & 15 != 15:
                    ins.address_target = literal
            if word & 0x0E100000 == 0x08100000 and word & 0x8000:
                ins.stop = True
                ins.returns = (word >> 16) & 15 == 13
                ins.indirect = not ins.returns
            elif word & 0x0FEFFFFF == 0x01A0F00E:
                ins.stop = ins.returns = True
            elif word & 0x0C10F000 == 0x0410F000:
                ins.stop = True
                ins.returns = (word >> 16) & 15 == 13
                ins.indirect = not ins.returns
            elif word & 0x0C00F000 == 0x0000F000 and (word >> 21) & 15 not in (8, 9, 10, 11):
                # Other data-processing writes to PC are computed exits, not fallthroughs.
                ins.stop = ins.indirect = True
        return ins

    def _decode_thumb(self, address: int) -> Instruction | None:
        if not self.inside(address, "thumb"):
            return None
        offset = address - self.base
        decoded = next(self.thumb.disasm(self.text[offset:offset + 4], address, count=1), None)
        if decoded is None or decoded.mnemonic.startswith("udf"):
            return None
        ins = Instruction(decoded.size, conditional=decoded.cc not in (arm.ARM_CC_AL, arm.ARM_CC_INVALID))
        ops = decoded.operands
        if decoded.id in (arm.ARM_INS_BL, arm.ARM_INS_BLX):
            ins.call = True
            if ops and ops[0].type == arm.ARM_OP_IMM:
                ins.target = (ops[0].imm & ~1, "arm" if decoded.id == arm.ARM_INS_BLX else "thumb")
        elif decoded.id == arm.ARM_INS_B:
            ins.branch = True
            ins.target = (ops[0].imm, "thumb")
        elif decoded.id in (arm.ARM_INS_CBZ, arm.ARM_INS_CBNZ):
            ins.branch = ins.conditional = True
            ins.target = (ops[-1].imm, "thumb")
        elif decoded.id == arm.ARM_INS_BX:
            ins.stop = True
            ins.returns = ops[0].reg == arm.ARM_REG_LR
            ins.indirect = not ins.returns
        elif decoded.id == arm.ARM_INS_POP and any(op.reg == arm.ARM_REG_PC for op in ops):
            ins.stop = ins.returns = True
        elif decoded.id == arm.ARM_INS_MOV and len(ops) == 2 and ops[0].reg == arm.ARM_REG_PC:
            ins.stop = True
            ins.returns = ops[1].reg == arm.ARM_REG_LR
            ins.indirect = not ins.returns
        elif decoded.id == arm.ARM_INS_SVC and ops and ops[0].imm == 3:
            ins.terminal = "svc_exit_process"
        elif decoded.id in (arm.ARM_INS_LDR, arm.ARM_INS_VLDR):
            for op in ops:
                if op.type == arm.ARM_OP_MEM and op.mem.base == arm.ARM_REG_PC and not op.mem.index:
                    literal = ((address + 4) & ~3) + op.mem.disp
                    width = 8 if ops[0].type == arm.ARM_OP_REG and arm.ARM_REG_D0 <= ops[0].reg <= arm.ARM_REG_D31 else 4
                    ins.pools = ((literal, literal + width),)
                    if decoded.id == arm.ARM_INS_LDR and width == 4 and ops[0].type == arm.ARM_OP_REG:
                        ins.register_pool = (ops[0].reg, literal)
            if ops and ops[0].reg == arm.ARM_REG_PC:
                ins.stop = ins.indirect = True
        elif decoded.id == arm.ARM_INS_ADR and len(ops) == 2:
            literal = ((address + 4) & ~3) + ops[1].imm
            ins.pools = ((literal & ~3, (literal + 7) & ~3),)
            ins.address_reference = True
            ins.address_target = literal
        elif decoded.id in (arm.ARM_INS_ADD, arm.ARM_INS_ADDW, arm.ARM_INS_SUB, arm.ARM_INS_SUBW):
            if (len(ops) == 3 and ops[1].type == arm.ARM_OP_REG and ops[1].reg == arm.ARM_REG_PC
                    and ops[2].type == arm.ARM_OP_IMM):
                sign = -1 if decoded.id in (arm.ARM_INS_SUB, arm.ARM_INS_SUBW) else 1
                ins.address_target = ((address + 4) & ~3) + sign * ops[2].imm
                ins.address_reference = True
            elif (decoded.id == arm.ARM_INS_ADD and len(ops) == 2
                  and ops[1].type == arm.ARM_OP_REG and ops[1].reg == arm.ARM_REG_PC):
                ins.pc_add_register = ops[0].reg
        if (decoded.id in (arm.ARM_INS_MOV, arm.ARM_INS_MOVW) and len(ops) == 2
                and ops[0].type == arm.ARM_OP_REG and ops[1].type == arm.ARM_OP_IMM):
            ins.register_constant = (ops[0].reg, ops[1].imm)
        return ins

    def plausible(self, address: int, isa: str) -> bool:
        if not self.inside(address, isa):
            return False
        ins = self.decode(address, isa)
        if ins is None:
            return False
        if isa == "arm":
            word = self.word(address)
            # Pointer guesses must not start on predicated data or ARMv7-only encodings.
            if word >> 28 != 14 and not (ins.call or ins.branch):
                return False
        return True

    def pointer_plausible(self, address: int, isa: str, *, prologue_sufficient: bool = True,
                          follow_tail_branches: bool = False, fallthrough_limit: int | None = None) -> bool:
        """Bound pointer witnesses; tail-chain recovery is opt-in after v3 refinement."""
        if not self.plausible(address, isa):
            return False
        if prologue_sufficient and (self.prologue(address, isa) or self.veneer(address, isa) is not None):
            return True
        cursor = address
        for _ in range(32):
            ins = self.decode(cursor, isa)
            if ins is None or fallthrough_limit is not None and cursor + ins.size > fallthrough_limit:
                return False
            if ins.returns:
                return True
            if ins.branch and not ins.conditional:
                if ins.target is None:
                    return False
                target, target_isa = ins.target
                seed = self.seeds.get(target)
                if seed is not None:
                    return seed.isa == target_isa
                if not follow_tail_branches or target_isa != isa or not self.plausible(target, isa):
                    return False
                # A pointer-only thunk may tail-call another undiscovered leaf.
                # Both paths share this 32-instruction bound, including cycles.
                cursor = target
                fallthrough_limit = None  # The explicit tail B, not fallthrough, leaves the gap.
                continue
            if ins.stop and not ins.conditional:
                return ins.indirect
            cursor += ins.size
        return False

    def prologue(self, address: int, isa: str = "arm") -> bool:
        if isa == "thumb":
            if address + 2 > self.end:
                return False
            half = struct.unpack_from("<H", self.text, address - self.base)[0]
            return half & 0xFF00 == 0xB500
        if address + 4 > self.end:
            return False
        word = self.word(address)
        return (
            word & 0xFFFF0000 == 0xE92D0000 and bool(word & 0x4000) and not word & 0x8000
            or word == 0xE52DE004
            or word & 0xFFFFF000 == 0xE24DD000
            or word & 0xFFFF0E00 == 0xED2D0A00
        )

    def add_seed(self, address: int, isa: str, evidence: str, *, force: bool = False) -> bool:
        if not self.inside(address, isa) or (not force and not self.plausible(address, isa)):
            return False
        seed = self.seeds.get(address)
        if seed is not None:
            if seed.isa == isa:
                seed.evidence.add(evidence)
            return False
        self.seeds[address] = Seed(isa, {evidence})
        return True

    def veneer(self, address: int, isa: str) -> tuple[int, str] | None:
        if address + 8 > self.end:
            return None
        if isa == "arm" and self.word(address) == 0xE51FF004:
            target = self.word(address + 4)
            return target & ~1, "thumb" if target & 1 else "arm"
        if isa == "arm":
            setup, transfer = self.word(address), self.word(address + 4)
            # AAPCS interworking veneer: a PC-relative odd address in IP,
            # immediately exchanged through IP. This is code, not an ADR string.
            if setup & 0xFFFFF000 == 0xE28FC000 and transfer == 0xE12FFF1C:
                target = (address + 8 + _immediate(setup)) & 0xFFFFFFFF
                if target & 1 and self.inside(target & ~1, "thumb"):
                    return target & ~1, "thumb"
        if isa == "thumb" and self.word(address) in (0x46C04778, 0xBF004778):
            branch = self.word(address + 4)
            if branch & 0xFF000000 == 0xEA000000:
                return address + 12 + (_signed(branch & 0xFFFFFF, 24) << 2), "arm"
        return None

    def _switch_bound(self, address: int, register: int) -> int | None:
        for previous in range(address - 4, max(self.base - 1, address - 65), -4):
            word = self.word(previous)
            if word & 0x0FF00000 == 0x03500000 and (word >> 16) & 15 == register:
                # LO indexes [0,n); LS indexes [0,n]. ARMCC emits both.
                count = _immediate(word) + (self.word(address) >> 28 != 3)
                return count if 0 < count <= 4096 else None
        return None

    def trace(self, start: int, limit: int, *, internal_starts: set[int] | None = None) -> Trace:
        isa = self.seeds[start].isa
        result = Trace(start, isa)
        if start in self.linker_wrappers and limit == start + 4:
            # armlink --branchnop erased a relocated tail B to the next section.
            # Keep its logical tail edge, without inventing a BL/branch source.
            result.code[start] = 4
            result.tails.add((limit, "arm", start))
            return result
        veneer = self.veneer(start, isa)
        if veneer is not None and start + 8 <= limit:
            result.kind = "veneer"
            if isa == "arm":
                result.code[start] = 4
                if self.word(start) == 0xE51FF004:
                    result.pools.add(start + 4)
                else:
                    result.code[start + 4] = 4
            else:
                result.code.update({start: 2, start + 2: 2, start + 4: 4})
            result.tails.add((*veneer, start))
            return result
        internal = self.internal_starts if internal_starts is None else internal_starts
        work = [start] + sorted(address for address in internal if start < address < limit)
        address_words: set[int] = set()
        while work:
            address = work.pop()
            while start <= address < limit:
                if address in result.code:
                    break
                if address & ~3 in result.pools:
                    result.open_exits.add(address)
                    break
                ins = self.decode(address, isa)
                if ins is None or address + ins.size > limit:
                    result.issues.add((address, "undecodable_reachable_code"))
                    result.open_exits.add(address)
                    break
                result.code[address] = ins.size
                if self.pointer_prefix_wrappers.get(start) == limit and address + ins.size == limit:
                    result.tails.add((limit, "arm", address))
                    break
                if ins.returns:
                    result.return_sites.add(address)
                for first, end in ins.pools:
                    if first % 4 or end % 4:
                        result.issues.add((address, "unaligned_literal_reference"))
                    elif start <= first < end <= limit:
                        (address_words if ins.address_reference else result.pools).update(range(first, end, 4))
                    elif self.base <= first < self.end:
                        result.issues.add((address, "literal_outside_function"))
                if ins.call:
                    if ins.target is not None:
                        result.calls.add((*ins.target, address))
                        if ins.target[0] in self.noreturn and not ins.conditional:
                            break
                elif ins.switch is not None:
                    style, register = ins.switch
                    count = self._switch_bound(address, register)
                    table = address + 8
                    if count is None and style == "branches":
                        count = 0
                        while table + count * 4 + 4 <= limit and count < 4096:
                            if self.word(table + count * 4) & 0xFF000000 != 0xEA000000:
                                break
                            count += 1
                        if not count:
                            count = None
                    if count is None or table + count * 4 > limit:
                        result.issues.add((address, "unresolved_switch"))
                        result.open_exits.add(address)
                    else:
                        result.switches += 1
                        if style == "branches":
                            for entry in range(table, table + count * 4, 4):
                                if self.word(entry) & 0xFF000000 != 0xEA000000:
                                    result.issues.add((entry, "invalid_switch_branch"))
                                    result.open_exits.add(entry)
                                else:
                                    work.append(entry)
                        else:
                            result.pools.update(range(table, table + count * 4, 4))
                            result.table_words.update(range(table, table + count * 4, 4))
                            for entry in range(table, table + count * 4, 4):
                                pointer = self.word(entry)
                                target = pointer & ~1
                                target_isa = "thumb" if pointer & 1 else "arm"
                                if start <= target < limit and target_isa == isa:
                                    work.append(target)
                                else:
                                    result.tails.add((target, target_isa, address))
                    if not ins.conditional:
                        break
                elif ins.branch and ins.target is not None:
                    target, target_isa = ins.target
                    previous = self.word(address - 4) if isa == "arm" and address >= start + 4 else 0
                    restored_lr = (
                        previous & 0x0FFF0000 == 0x08BD0000 and bool(previous & 0x4000)
                        or previous & 0x0FFFFFFF == 0x049DE004
                    )
                    padding_gap = (
                        isa == "arm" and address + 4 < target < min(limit, address + 68)
                        and all(a not in result.pools and self.word(a) in (0, 0xE1A00000, 0xE320F000)
                                for a in range(address + 4, target, 4))
                    )
                    tail = not start <= target < limit or (
                        target not in internal and not ins.conditional
                        and (address == start or restored_lr or padding_gap)
                    )
                    if tail:
                        result.tails.add((target, target_isa, address))
                    else:
                        work.append(target)
                    if not ins.conditional:
                        break
                elif ins.stop or self.refining and ins.terminal is not None:
                    if ins.target is not None:
                        result.tails.add((*ins.target, address))
                    result.indirect_exits += ins.indirect
                    if ins.indirect and ins.target is None:
                        result.open_exits.add(address)
                    if not ins.conditional:
                        break
                address += ins.size
                if address == limit:
                    result.open_exits.add(address)
                    if limit < self.end:
                        result.issues.add((address, "fallthrough_into_start"))
        occupied = {address & ~3 for address in result.code}
        result.address_words.update(address_words - occupied - result.pools)
        result.pools.update(result.address_words)
        return result

    def traces(self) -> list[Trace]:
        starts = sorted(self.seeds)
        return [self.trace(start, starts[index + 1] if index + 1 < len(starts) else self.end)
                for index, start in enumerate(starts)]

    def _external_pointers(self) -> list[tuple[int, str]]:
        targets = []
        for segment in (self.layout.rodata, self.layout.data):
            if not segment.size:
                continue
            offset = self.layout.file_offset(segment.address)
            for index in range(0, segment.size - 3, 4):
                pointer = struct.unpack_from("<I", self.image, offset + index)[0]
                target, isa = pointer & ~1, "thumb" if pointer & 1 else "arm"
                if self.inside(target, isa):
                    targets.append((target, isa))
        return targets

    def include_relative_pointer_tables(self, tables: list[dict]) -> None:
        """Import reviewed section-ABI tables, not arbitrary pointer-shaped words."""
        for row in tables:
            first, end = row["start"], row["end"]
            if (row.get("encoding") != "self_relative_32" or row.get("section") != ".init_array"
                    or not row.get("evidence") or first % 4 or end % 4 or not first < end
                    or not self.layout.rodata.address <= first < end <= self.layout.rodata.address + self.layout.rodata.size):
                raise ValueError("relative initializer table lacks a bounded section-ABI witness")
            for key, expected in (("start", first), ("end", end)):
                pool, base = row.get("bounds", {}).get(key, (None, None))
                if (pool is None or pool % 4 or not self.base <= pool <= self.end - 4
                        or (base + self.word(pool)) & 0xFFFFFFFF != expected):
                    raise ValueError(f"relative initializer table {key} is not derived by its walker pool word")
            targets = []
            for site in range(first, end, 4):
                offset = self.layout.file_offset(site)
                pointer = (site + struct.unpack_from("<I", self.image, offset)[0]) & 0xFFFFFFFF
                target, isa = pointer & ~1, "thumb" if pointer & 1 else "arm"
                if not self.plausible(target, isa):
                    raise ValueError(f"initializer table {site:#x} does not target plausible code")
                targets.append({"site": site, "address": target, "isa": isa})
                if target not in self.seeds:
                    self.add_seed(target, isa, "relative_init_array")
                    self.relative_table_pointers.add((target, isa))
            self.relative_pointer_tables.append({**row, "targets": targets})

    def discover(self) -> list[Trace]:
        self.add_seed(self.base, "arm", "entry", force=True)
        for address in range(self.base, self.end - 3, 4):
            word = self.word(address)
            if word & 0x0F000000 == 0x0B000000 and word >> 28 != 15:
                target = address + 8 + (_signed(word & 0xFFFFFF, 24) << 2)
                if self.add_seed(target, "arm", "call_scan") or target in self.seeds:
                    self.raw_calls.setdefault((target, "arm"), set()).add(address)
            elif word & 0xFE000000 == 0xFA000000:
                target = address + 8 + (_signed(word & 0xFFFFFF, 24) << 2) + ((word >> 23) & 2)
                if self.add_seed(target, "thumb", "call_scan") or target in self.seeds:
                    self.raw_calls.setdefault((target, "thumb"), set()).add(address)
            if self.veneer(address, "arm") is not None:
                self.add_seed(address, "arm", "veneer_pattern", force=True)
            if self.veneer(address, "thumb") is not None:
                self.add_seed(address, "thumb", "veneer_pattern", force=True)
        external = self._external_pointers()
        for _ in range(64):
            self.discovery_rounds += 1
            traces = self.traces()
            code = {address + offset for trace in traces for address, size in trace.code.items() for offset in range(0, size, 2)}
            pools = {address for trace in traces for address in trace.pools}
            tables = {address for trace in traces for address in trace.table_words}
            changed = False
            observed_calls = {(target, isa) for trace in traces for target, isa, _ in trace.calls}
            for address, seed in list(self.seeds.items()):
                if "call_scan" in seed.evidence:
                    sources = self.raw_calls.get((address, seed.isa), set())
                    if (address, seed.isa) in observed_calls:
                        seed.evidence.discard("call_scan")
                    elif sources and sources <= pools:
                        seed.evidence.discard("call_scan")
                        if not seed.evidence:
                            del self.seeds[address]
                            self.rejected_call_seeds += 1
                            changed = True
            for trace in traces:
                for target, isa, _ in sorted(trace.calls):
                    changed |= self.add_seed(target, isa, "blx_target" if isa != trace.isa else "bl_target", force=True)
                for target, isa, _ in sorted(trace.tails):
                    if target & ~3 not in pools and (target not in code or target in self.seeds):
                        changed |= self.add_seed(target, isa, "veneer_target" if trace.kind == "veneer" else "tail_call")
            pointer_targets = list(external)
            for address in sorted(pools - tables):
                pointer = self.word(address)
                target, isa = pointer & ~1, "thumb" if pointer & 1 else "arm"
                if self.inside(target, isa):
                    pointer_targets.append((target, isa))
            trace_starts = [trace.address for trace in traces]
            for target, isa in sorted(set(pointer_targets)):
                index = bisect_right(trace_starts, target) - 1
                interior = index >= 0 and traces[index].address < target < traces[index].end
                if target in self.seeds or (not interior and target & ~3 not in pools and self.pointer_plausible(target, isa)):
                    changed |= self.add_seed(target, isa, "pointer")
            if not changed:
                # Consume one gap function before looking for another prologue;
                # SUB sp in its interior must not become a second weak start.
                known_starts = sorted(self.seeds)
                for address in range(self.base, self.end - 3, 4):
                    if address not in code and address & ~3 not in pools and self.prologue(address):
                        if self.add_seed(address, "arm", "prologue"):
                            changed = True
                            index = bisect_right(known_starts, address)
                            limit = known_starts[index] if index < len(known_starts) else self.end
                            probe = self.trace(address, limit)
                            code.update(a + offset for a, size in probe.code.items() for offset in range(0, size, 2))
                            pools.update(probe.pools)
                for address in range(self.base, self.end - 1, 2):
                    if (address not in code and address & ~3 not in pools and self.prologue(address, "thumb")
                            and self.pointer_plausible(address, "thumb", prologue_sufficient=False)):
                        if self.add_seed(address, "thumb", "prologue"):
                            changed = True
                            index = bisect_right(known_starts, address)
                            limit = known_starts[index] if index < len(known_starts) else self.end
                            probe = self.trace(address, limit)
                            code.update(a + offset for a, size in probe.code.items() for offset in range(0, size, 2))
                            pools.update(probe.pools)
                if not changed:
                    return traces
        raise ValueError("function-start discovery did not converge after 64 rounds")

    def _pointer_table_starts(self, traces: list[Trace]) -> set[int]:
        """Protect runs of distinct entry pointers, allowing one null callback slot.

        Switch-address tables name local labels, not separately callable functions.
        A lone pointer remains weak evidence; adjacent pointers to different starts
        are conservatively treated as a vtable/callback array, even without types.
        """
        protected: set[int] = set()
        words: dict[int, int] = {}
        for segment in (self.layout.rodata, self.layout.data):
            if segment.size:
                offset = self.layout.file_offset(segment.address)
                words.update((segment.address + index, struct.unpack_from("<I", self.image, offset + index)[0])
                             for index in range(0, segment.size - 3, 4))
        for trace in traces:
            words.update((address, self.word(address)) for address in trace.pools - trace.table_words)
        run: set[int] = set()
        previous = -8
        null_slot = False
        for address, pointer in sorted(words.items()):
            target, isa = pointer & ~1, "thumb" if pointer & 1 else "arm"
            seed = self.seeds.get(target)
            entry = seed is not None and seed.isa == isa
            if address != previous + 4 or not (entry or pointer == 0 and run and not null_slot):
                if len(run) >= 2:
                    protected.update(run)
                run = set()
            if entry:
                run.add(target)
            null_slot = pointer == 0
            previous = address
        if len(run) >= 2:
            protected.update(run)
        return protected

    def _resolve_noreturn(self) -> list[Trace]:
        """Find closed no-return graphs, then propagate terminating direct calls.

        Unknown indirect exits, incomplete decoding, fallthroughs and unresolved
        switches disqualify a graph. Tail-call cycles are solved together; a mere
        lack of recognized returns is not sufficient evidence.
        """
        self.noreturn = {}
        # Finish start discovery before pruning paths at newly recognized exits.
        # Otherwise starts evidenced by pools on those paths disappear unrecorded.
        self.refining = True
        while True:
            traces = self.traces()
            by_start = {trace.address: trace for trace in traces}
            candidates = {trace.address for trace in traces
                          if trace.code and not trace.return_sites and not trace.open_exits}
            while True:
                rejected = {start for start in candidates if any(
                    target not in candidates or by_start[target].isa != isa
                    for target, isa, _ in by_start[start].tails)}
                if not rejected:
                    break
                candidates.difference_update(rejected)
            additions = candidates - self.noreturn.keys()
            if not additions:
                return traces
            for start in sorted(additions):
                trace = by_start[start]
                reasons = {"no_return_sites"}
                for address in trace.code if trace.kind != "veneer" else ():
                    ins = self.decode(address, trace.isa)
                    if ins.terminal is not None:
                        reasons.add(ins.terminal)
                    if ins.call and ins.target is not None and ins.target[0] in self.noreturn:
                        reasons.add("noreturn_call")
                if trace.tails:
                    reasons.add("noreturn_tail")
                if reasons == {"no_return_sites"} or any(target == start for target, _, _ in trace.tails):
                    reasons.add("closed_loop")
                self.noreturn[start] = sorted(reasons)

    def _connections(self, traces: list[Trace]) -> list[tuple[int, int, str, int, int]]:
        """Section constraints, expressed as inclusive runs of candidate indices."""
        starts = [trace.address for trace in traces]
        connections: set[tuple[int, int, str, int, int]] = set()
        for index, trace in enumerate(traces):
            if trace.kind == "veneer":
                continue
            for address in sorted(trace.code):
                ins = self.decode(address, trace.isa)
                for first, end in ins.pools if not ins.address_reference else ():
                    if self.base <= first < end <= self.end:
                        for word in range(first, end, 4):
                            other = bisect_right(starts, word) - 1
                            if other != index:
                                connections.add((min(index, other), max(index, other),
                                                 "section_literal", address, word))
                if ins.branch and ins.target is not None:
                    target, isa = ins.target
                    if self.inside(target, isa):
                        other = bisect_right(starts, target) - 1
                        if (other != index and traces[other].isa == isa
                                and target > traces[other].address and target in traces[other].code):
                            connections.add((min(index, other), max(index, other),
                                             "branch_into_body", address, target))
            if (index + 1 < len(traces) and not trace.return_sites
                    and (starts[index + 1], "fallthrough_into_start") in trace.issues):
                connections.add((index, index + 1, "fallthrough_without_return",
                                 max(trace.code), starts[index + 1]))
        return sorted(connections, key=lambda item: (item[1] - item[0], item))

    def refine(self) -> list[Trace]:
        """Merge only connected weak entries; preserve every observed BL/BLX start."""
        self.protected_pointer_starts.update(self._pointer_table_starts(self.traces()))
        traces = self._resolve_noreturn()
        while True:
            self.protected_pointer_starts.update(self._pointer_table_starts(traces))
            incoming: dict[int, set[int]] = {}
            for trace in traces:
                for target, _, _ in trace.calls | trace.tails:
                    incoming.setdefault(target, set()).add(trace.address)
            connections = self._connections(traces)
            changed: set[int] = set()
            groups = sorted({connection[:2] for connection in connections},
                            key=lambda interval: (interval[1] - interval[0], interval))
            for first, last in groups:
                group = traces[first:last + 1]
                left = group[0]
                removed = {trace.address for trace in group[1:]}
                members = removed | {left.address}
                if members & changed:
                    continue
                # call_scan is unproved raw-byte evidence, unlike bl[x]_target.
                weak = {"tail_call", "prologue", "pointer", "call_scan"}
                if any(trace.kind != "function" or trace.isa != left.isa for trace in group):
                    continue
                if any(not self.seeds[address].evidence <= weak
                       or address in self.protected_pointer_starts
                       or incoming.get(address, set()) - members for address in removed):
                    continue
                limit = traces[last + 1].address if last + 1 < len(traces) else self.end
                internal = self.internal_starts | removed
                probe = self.trace(left.address, limit, internal_starts=internal)
                old_code = {address for trace in group for address in trace.code}
                if not old_code <= probe.code.keys():
                    continue
                if any(address & ~3 in probe.pools for address in probe.code):
                    continue
                if any(target in removed or target not in self.seeds or self.seeds[target].isa != isa
                       for target, isa, _ in probe.calls):
                    continue
                for boundary, trace in enumerate(group[1:], first + 1):
                    address = trace.address
                    witnesses = [{"reason": reason, "address": site, "target": target}
                                 for lo, hi, reason, site, target in connections
                                 if first <= lo < boundary <= hi <= last]
                    reasons = sorted({witness["reason"] for witness in witnesses})
                    self.boundary_merges.append({
                        "address": address, "retained_address": left.address,
                        "evidence": sorted(self.seeds[address].evidence),
                        "reasons": reasons, "witnesses": witnesses,
                    })
                    del self.seeds[address]
                self.internal_starts.update(removed)
                changed.update(members)
            if not changed:
                break
            traces = self._resolve_noreturn()
        owners = {merge["address"]: merge["retained_address"] for merge in self.boundary_merges}
        for merge in self.boundary_merges:
            owner = merge["retained_address"]
            while owner in owners:
                owner = owners[owner]
            merge["retained_address"] = owner
        self.boundary_merges.sort(key=lambda item: item["address"])
        return traces

    def group_fallthroughs(self, traces: list[Trace]) -> list[Trace]:
        """Keep every entry while grouping code that falls across section guesses.

        Reachable fallthroughs cannot connect separate ARMCC split sections,
        except modelled armlink tail-call NOPs (handled by trace()).
        Other groups are never offered as ordinary creditable C.
        """
        groups: list[Trace] = []
        self.entry_groups = []
        for trace in traces:
            trace.members = {trace.address}
            trace.entries = {trace.address: {
                "address": trace.address, "isa": trace.isa, "kind": "entry",
                "evidence": set(self.seeds[trace.address].evidence), "sources": set(),
            }}
            if groups and (trace.address, "fallthrough_into_start") in groups[-1].issues:
                left = groups[-1]
                if left.isa != trace.isa or trace.kind == "veneer" or left.kind == "veneer":
                    left.issues.add((trace.address, "incompatible_fallthrough_entries"))
                    groups.append(trace)
                    continue
                left.kind = "multi_entry"
                left.code.update(trace.code)
                for name in ("pools", "table_words", "calls", "tails", "issues", "return_sites",
                             "open_exits", "address_words", "members"):
                    getattr(left, name).update(getattr(trace, name))
                left.entries.update(trace.entries)
                left.indirect_exits += trace.indirect_exits
                left.switches += trace.switches
                left.issues.discard((trace.address, "fallthrough_into_start"))
                left.open_exits.discard(trace.address)
            else:
                groups.append(trace)
        for trace in groups:
            trace.tails = {(target, isa, site) for target, isa, site in trace.tails
                           if target not in trace.code or isa != trace.isa}
            if len(trace.members) > 1:
                self.entry_groups.append({"address": trace.address, "members": sorted(trace.members),
                                          "reason": "fallthrough"})
        return groups

    def promote_missing_targets(self, traces: list[Trace]) -> bool:
        """Repair direct tail/call targets outside every final extent, not data holes."""
        starts = [trace.address for trace in traces]
        changed = False
        for trace in traces:
            for target, isa, site in sorted(trace.calls | trace.tails):
                if target in self.seeds:
                    continue
                source_isa = "arm" if trace.kind == "veneer" and trace.isa == "thumb" and site == trace.address + 4 else trace.isa
                ins = self.decode(site, source_isa)
                if not (ins.call or ins.branch) or not self.inside(target, isa):
                    continue
                index = bisect_right(starts, target) - 1
                if index >= 0 and target < traces[index].end:
                    continue
                if self.decode(target, isa) is None:
                    trace.issues.add((site, "undecodable_tail_target"))
                    continue
                evidence = "blx_target" if ins.call and isa != source_isa else "bl_target" if ins.call else "tail_call"
                if self.add_seed(target, isa, evidence, force=True):
                    self.tail_target_repairs.setdefault((target, isa), set()).add(site)
                    changed = True
        return changed

    def _pointer_targets(self, traces: list[Trace]) -> set[tuple[int, str]]:
        targets = set(self._external_pointers()) | self.relative_table_pointers
        for trace in traces:
            for address in trace.pools - trace.table_words:
                pointer = self.word(address)
                target, isa = pointer & ~1, "thumb" if pointer & 1 else "arm"
                if self.inside(target, isa):
                    targets.add((target, isa))
            # A PC-relative value with the architecture's Thumb tag is an
            # address witness too. Admission below requires a complete CFG;
            # an odd byte-string address is not sufficient.
            targets.update((pointer & ~1, "thumb") for pointer in trace.address_refs.values()
                           if pointer & 1 and self.inside(pointer & ~1, "thumb"))
        if __package__:
            from . import catalogue_closure
        else:
            import catalogue_closure
        self.formed_code_pointers = []
        catalogue_closure.read_footprints({"functions": [
            {"address": trace.address, "isa": trace.isa, "kind": trace.kind,
             "code_ranges": _merge([(site, site + size) for site, size in trace.code.items()]),
             "entries": list(trace.entries.values())}
            for trace in traces]}, self,
            address_witnesses=self.formed_code_pointers, include_reads=False)
        targets.update((row["target"], row["isa"]) for row in self.formed_code_pointers)
        return targets

    def detailed(self, address: int):
        if address not in self.detail_cache:
            offset = address - self.base
            self.detail_cache[address] = (next(self.detailed_arm.disasm(self.text[offset:offset + 4], address, 1), None)
                                          if self.inside(address) else None)
        return self.detail_cache[address]

    def _live_core_inputs(self, trace: Trace) -> set[int]:
        """Find read-before-write witnesses, excluding saves of incoming nonarguments.

        Calls consume argument registers and destroy caller-saved values. This is
        deliberately a may-read analysis: it is a boundary witness, not a prototype.
        Conditional definitions cannot kill a dependency on another incoming path.
        """
        core = set(range(arm.ARM_REG_R0, arm.ARM_REG_R12 + 1))
        arguments = {arm.ARM_REG_R0, arm.ARM_REG_R1, arm.ARM_REG_R2, arm.ARM_REG_R3}
        live: set[int] = set()
        work = [(trace.address, frozenset())]
        seen: set[tuple[int, frozenset]] = set()
        while work:
            address, written = work.pop()
            if (address, written) in seen or len(seen) >= 4096:
                continue
            seen.add((address, written))
            if address not in trace.code:
                continue
            decoded, ins = self.detailed(address), self.decode(address, "arm")
            if decoded is None or ins is None:
                continue
            read, wrote = (set(items) for items in decoded.regs_access())
            stack_save = (decoded.id == arm.ARM_INS_PUSH or
                          decoded.id == arm.ARM_INS_STMDB and decoded.operands
                          and decoded.operands[0].type == arm.ARM_OP_REG
                          and decoded.operands[0].reg == arm.ARM_REG_SP)
            if stack_save:
                read.difference_update(core)
            if ins.call:
                read.update(arguments)
                wrote.update(arguments | {arm.ARM_REG_R12})
            live.update((read & core) - written)
            after = written | frozenset(wrote & core) if not ins.conditional else written
            if ins.branch and ins.target is not None and ins.target[1] == "arm":
                work.append((ins.target[0], after))
            if not (ins.stop or ins.branch) or ins.conditional:
                work.append((address + ins.size, after))
        return live

    def promote_leading_instructions(self, traces: list[Trace], protected: dict[int, dict]) -> bool:
        """Repair only weak prologue starts; a real incoming entry is never moved.

        Scan every gap, not selected addresses. A prefix must be unconditional,
        nonstoring register/constant setup (benign memory reads are allowed), fit
        entirely in the gap, supply a body input, and have no unexplained
        callee-saved input. Literal ownership, executable/data overlap, known entries
        and protected extents are guards.
        """
        pointers = self._pointer_targets(traces)
        occupied = {address + offset for trace in traces for address, size in trace.code.items()
                    for offset in range(size)}
        data = {address + offset for trace in traces for address in trace.pools for offset in range(4)}
        data.update(address for trace in traces for first, end in trace.literals for address in range(first, end))
        core = set(range(arm.ARM_REG_R0, arm.ARM_REG_R12 + 1))
        allowed_inputs = {arm.ARM_REG_R0, arm.ARM_REG_R1, arm.ARM_REG_R2, arm.ARM_REG_R3,
                          arm.ARM_REG_SP, arm.ARM_REG_LR, arm.ARM_REG_PC}
        changed = False
        previous_end = self.base
        for index, trace in enumerate(traces):
            start, gap = trace.address, previous_end
            previous_end = trace.end
            if trace.isa != "arm" or gap >= start:
                continue
            row = {"address": start, "gap_start": gap, "gap_end": start, "decision": "no_register_prefix"}
            self.leading_instruction_inventory.setdefault(start, row)
            seed = self.seeds[start]
            if seed.evidence != {"prologue"} or trace.kind not in ("function", "multi_entry"):
                row["decision"] = "independently_evidenced_entry"
                self.leading_instruction_inventory[start] = row
                continue
            protected_owners = [record for record in protected.values()
                                if record["address"] <= start < record["address"] + record["size"]]
            if any(record["address"] == start for record in protected_owners):
                row["decision"] = "protected_extent"
                self.leading_instruction_inventory[start] = row
                continue
            live = self._live_core_inputs(trace)
            needed = set(live)
            prefix = []
            prefix_inputs: set[int] = set()
            candidates = []
            limit = traces[index + 1].address if index + 1 < len(traces) else self.end
            for address in range(start - 4, max(gap - 1, start - 256 - 1), -4):
                decoded, ins = self.detailed(address), self.decode(address, "arm")
                if (decoded is None or ins is None or ins.conditional or ins.call or ins.branch or ins.stop
                        or ins.address_reference or self.word(address) in (0, 0xE1A00000, 0xE320F000)):
                    break
                read, wrote = (set(items) for items in decoded.regs_access())
                if (not wrote or not wrote <= core or not wrote & needed
                        or _memory_side_effect(decoded) or decoded.writeback):
                    break
                if any(not start <= first < end <= limit or any(p in occupied for p in range(first, end))
                       for first, end in ins.pools):
                    row["decision"] = "literal_crosses_boundary_or_code"
                    break
                if any(p in occupied or p in data for p in range(address, address + 4)):
                    break
                needed.difference_update(wrote)
                needed.update(read)
                prefix_inputs.difference_update(wrote)
                prefix_inputs.update(read)
                prefix.append({"address": address,
                               "writes": sorted(self.detailed_arm.reg_name(register) for register in wrote),
                               "literal_ranges": [list(span) for span in ins.pools]})
                before = self.decode(address - 4, "arm") if address - 4 >= gap else None
                anchored = (address == gap or (address, "arm") in pointers
                            or before is not None and (before.returns and not before.conditional
                                                       or self.word(address - 4) in (0, 0xE1A00000, 0xE320F000)))
                if anchored and prefix_inputs <= allowed_inputs:
                    candidates.append((address, list(reversed(prefix))))
            if not candidates:
                self.leading_instruction_inventory[start] = row
                continue
            address, instructions = candidates[-1]
            if any(record["address"] != address for record in protected_owners):
                row["decision"] = "protected_extent"
                self.leading_instruction_inventory[start] = row
                continue
            del self.seeds[start]
            self.seeds[address] = Seed("arm", {"leading_instructions"} |
                                      ({"pointer"} if (address, "arm") in pointers else set()))
            self.internal_starts.discard(start)
            self.boundary_merges = [merge for merge in self.boundary_merges if merge["address"] != start]
            repair = {"old_address": start, "address": address, "old_end": trace.end,
                      "reason": "gap_prefix_supplies_body_input", "instructions": instructions,
                      "body_live_inputs": sorted(self.detailed_arm.reg_name(register) for register in live),
                      "pointer_witness": (address, "arm") in pointers}
            self.leading_instruction_repairs.append(repair)
            row.update(decision="recovered", recovered_address=address)
            self.leading_instruction_inventory[start] = row
            changed = True
        return changed

    def _closed_pointer_body(self, address: int, isa: str, limit: int) -> tuple[bool, dict]:
        """A bounded complete CFG is stronger than the old 32-linear-instruction probe."""
        if not self.plausible(address, isa) or limit - address > 0x10000:
            return False, {"reason": "invalid_or_oversized_pointer_probe"}
        self.seeds[address] = Seed(isa)
        try:
            probe = self.trace(address, limit)
        finally:
            del self.seeds[address]
        bad_calls = [(target, target_isa) for target, target_isa, _ in probe.calls | probe.tails
                     if target not in self.seeds or self.seeds[target].isa != target_isa]
        code = {site + offset for site, size in probe.code.items() for offset in range(size)}
        overlaps = bool(code & {word + offset for word in probe.pools for offset in range(4)})
        accepted = (bool(probe.code) and not probe.issues and not probe.open_exits and not bad_calls
                    and not overlaps and bool(probe.return_sites or probe.tails))
        return accepted, {"reason": "closed_pointer_cfg" if accepted else "pointer_cfg_not_closed",
                          "code_bytes": sum(probe.code.values()), "end": probe.end,
                          "issues": [{"address": site, "issue": issue} for site, issue in sorted(probe.issues)],
                          "open_exits": sorted(probe.open_exits), "return_sites": sorted(probe.return_sites),
                          "unresolved_transfers": [{"address": target, "isa": mode} for target, mode in bad_calls],
                          "pool_overlaps_code": overlaps}

    def _pointer_prefix_wrapper(self, address: int, isa: str, limit: int) -> bool:
        """Prove pointer-referenced nonstoring setup before an independent entry."""
        return (isa == "arm" and limit in self.seeds and self.seeds[limit].isa == "arm"
                and self._nonstoring_prefix(address, limit))

    def _nonstoring_prefix(self, address: int, limit: int) -> bool:
        """Bound setup ending in armlink's exact erased tail B; loads are allowed.

        No SP/LR change, call, memory side effect, pool, architectural NOP hint or
        incoming flags may stand in for the linker witness. Conditional setup
        needs locally set flags.
        """
        if not 4 < limit - address <= 64 or self.word(limit - 4) != 0xE1A00000:
            return False
        core = set(range(arm.ARM_REG_R0, arm.ARM_REG_R3 + 1)) | {arm.ARM_REG_R12}
        written: set[int] = set()
        for site in range(address, limit - 4, 4):
            decoded, ins = self.detailed(site), self.decode(site, "arm")
            if (decoded is None or ins is None or ins.call or ins.branch or ins.stop
                    or ins.pools or ins.address_reference or decoded.writeback
                    or _memory_side_effect(decoded)):
                return False
            read, wrote = (set(items) for items in decoded.regs_access())
            if (not wrote or not wrote <= core | {arm.ARM_REG_CPSR}
                    or not read <= core | {arm.ARM_REG_CPSR}
                    or arm.ARM_REG_CPSR in read - written
                    or ins.conditional and arm.ARM_REG_CPSR not in written):
                return False
            if not ins.conditional:
                written.update(wrote)
        return True

    def promote_pointer_entries(self, traces: list[Trace]) -> bool:
        """Recover unowned pointer-only tail chains without rerunning discovery.

        Early tail following changes v3's weak-start refinement order. Wait for
        its final extents, then promote only gaps, never code/data inside an owner.
        Existing missing-target recovery discovers the new thunk's tail callee.
        """
        starts = [trace.address for trace in traces]
        changed = False
        addressed = {(pointer & ~1, "thumb") for trace in traces for pointer in trace.address_refs.values()
                     if pointer & 1 and self.inside(pointer & ~1, "thumb")}
        targets = self._pointer_targets(traces)
        addressed.update((row["target"], row["isa"]) for row in self.formed_code_pointers)
        for target, isa in sorted(targets):
            if target in self.seeds:
                continue
            index = bisect_right(starts, target) - 1
            if index >= 0 and target < traces[index].end:
                continue
            limit = starts[index + 1] if index + 1 < len(starts) else self.end
            if (target, isa) in addressed:
                accepted, proof = self._closed_pointer_body(target, isa, limit)
                self.pointer_entry_inventory[(target, isa)] = {
                    "address": target, "isa": isa, "pointer_source": "tagged_pc_relative_address", **proof}
            else:
                accepted = self.pointer_plausible(target, isa, follow_tail_branches=True, fallthrough_limit=limit)
            if not accepted and self._pointer_prefix_wrapper(target, isa, limit):
                self.pointer_prefix_wrappers[target] = limit
                self.pointer_entry_inventory[(target, isa)] = {
                    "address": target, "isa": isa, "reason": "armlink_pointer_prefix",
                    "tail_nop": limit - 4, "target": limit, "code_bytes": limit - target,
                }
                accepted = True
            if not accepted:
                accepted, proof = self._closed_pointer_body(target, isa, limit)
                self.pointer_entry_inventory[(target, isa)] = {"address": target, "isa": isa, **proof}
            if accepted:
                if self.add_seed(target, isa, "pointer"):
                    self.pointer_target_repairs.add((target, isa))
                    changed = True
        return changed

    def promote_linker_entries(self, traces: list[Trace]) -> bool:
        """Split evidenced MOV-r0-r0 entries, after v3 boundary/data recovery.

        armcc emits the architectural NOP hint, not MOV r0,r0. At an entry
        the latter is armlink's relocated tail B to the next section. Only
        existing entries and pointer-referenced padding establish wrappers;
        interior call-site NOPs and unreferenced padding remain untouched.
        """
        entries = {address: entry for trace in traces for address, entry in trace.entries.items()}
        code = {address: trace.isa for trace in traces for address in trace.code}
        pools = {address for trace in traces for address in trace.pools}
        pointers = self._pointer_targets(traces)
        # A secondary body label alone is not an independent section-start witness.
        candidates = {address for address, entry in entries.items()
                      if address in self.seeds and entry["isa"] == "arm"
                      and self.word(address) == 0xE1A00000 and code.get(address + 4) == "arm"}
        changed = False
        for target, isa in sorted(pointers):
            # A long callee can exceed pointer_plausible's bounded probe;
            # its independently evidenced entry still proves this one-word tail.
            if (isa == "arm" and target not in code and target not in pools
                    and target + 4 in entries and entries[target + 4]["isa"] == "arm"
                    and self.word(target) == 0xE1A00000):
                changed |= self.add_seed(target, isa, "pointer")
                candidates.add(target)
        restored = set()
        for address in sorted(candidates):
            target = address + 4
            if target not in self.seeds:
                if not self.add_seed(target, "arm", "linker_branchnop"):
                    continue
                if target in entries:
                    self.seeds[target].evidence.update(entries[target]["evidence"])
                restored.add(target)
                changed = True
            if (target, "arm") in pointers:
                self.seeds[target].evidence.add("pointer")
            if address not in self.linker_wrappers:
                self.linker_wrappers.add(address)
                changed = True
        # A former weak body start is now a section boundary, not a removed label.
        self.internal_starts.difference_update(restored)
        self.boundary_merges = [merge for merge in self.boundary_merges if merge["address"] not in restored]
        return changed

    def record_entries(self, traces: list[Trace], protected_entries: dict[int, dict] | None = None) -> None:
        """Record external B targets and every BL/BLX target on instruction boundaries."""
        starts = [trace.address for trace in traces]
        for source in traces:
            for target, isa, site in sorted(source.calls | source.tails):
                index = bisect_right(starts, target) - 1
                if index < 0 or target >= traces[index].end:
                    continue
                owner = traces[index]
                source_isa = "arm" if source.kind == "veneer" and source.isa == "thumb" and site == source.address + 4 else source.isa
                ins = self.decode(site, source_isa)
                if not (ins.call or ins.branch) or source is owner and not ins.call:
                    continue
                target_isa = "arm" if owner.kind == "veneer" and owner.isa == "thumb" and target == owner.address + 4 else owner.isa
                if target not in owner.code or isa != target_isa:
                    source.issues.add((site, "entry_target_not_code"))
                    continue
                expected = (protected_entries or {}).get(target)
                if expected is not None and site not in expected["sources"]:
                    # New caller observations are retained separately rather than
                    # silently changing an accepted record's metadata.
                    self.protected_entry_observations.add((source.address, site, target))
                    continue
                entry = owner.entries.setdefault(target, {
                    "address": target, "isa": isa, "kind": "secondary_entry",
                    "evidence": set(), "sources": set(),
                })
                entry["sources"].add(site)
                entry["evidence"].add("blx_target" if ins.call and isa != source_isa
                                      else "bl_target" if ins.call else "tail_call")
        owner_by_member = {member: trace.address for trace in traces for member in trace.members}
        for merge in self.boundary_merges:
            merge["retained_address"] = owner_by_member.get(merge["retained_address"], merge["retained_address"])
            index = bisect_right(starts, merge["address"]) - 1
            if index >= 0 and merge["address"] in traces[index].code:
                merge["retained_address"] = traces[index].address
        for trace in traces:
            entry_addresses = sorted(trace.entries)
            for address, next_address in zip(entry_addresses, entry_addresses[1:]):
                if (trace.entries[address]["isa"] == "arm" and next_address == address + 4
                        and self.word(address) in (0xE1A00000, 0xE320F000)):
                    trace.entries[address]["kind"] = "nop_entry"
            if len(trace.entries) > 1 and trace.kind != "veneer":
                trace.kind = "multi_entry"

    def _string_end(self, target: int) -> int | None:
        offset = target - self.base
        stop = self.text.find(b"\0", offset)
        if stop < 0:
            return None
        raw = self.text[offset:stop]
        for encoding in ("utf-8", "cp932"):
            try:
                value = raw.decode(encoding)
            except UnicodeDecodeError:
                continue
            if all(character.isprintable() or character in "\t\r\n" for character in value):
                return self.base + stop + 1
        return None

    def _utf16_string_end(self, target: int) -> int | None:
        """Bound a reviewed halfword-string use, never infer width from glyphs."""
        if target % 2:
            return None
        cursor = target
        while cursor + 2 <= self.end:
            if struct.unpack_from("<H", self.text, cursor - self.base)[0] == 0:
                try:
                    value = self.text[target - self.base:cursor - self.base].decode("utf-16le")
                except UnicodeDecodeError:
                    return None
                return cursor + 2 if all(c.isprintable() or c.isspace() for c in value) else None
            cursor += 2
        return None

    def include_address_literals(self, traces: list[Trace]) -> None:
        """Claim exclusive address-taken objects without changing discovered starts.

        NUL-terminated text has a complete bound. Other data retains only the
        addressed word span already evidenced by v2, not a guessed array length.
        References into string suffixes and external pointer tables count as shared.
        """
        if __package__:
            from . import catalogue_closure
        else:
            import catalogue_closure
        functions = [{"address": trace.address, "isa": trace.isa, "kind": trace.kind,
                      "code_ranges": _merge([(site, site + size) for site, size in trace.code.items()]),
                      "entries": list(trace.entries.values())} for trace in traces]
        little_endian = catalogue_closure._little_endian_sites(
            catalogue_closure._decoded_functions({"functions": functions}, self), self)
        self.literal_conflicts = []
        self.shared_literals = []
        self.literal_extent_changes.clear()
        previous_ends = {trace.address: trace.end for trace in traces}
        by_start = {trace.address: trace for trace in traces}
        references: dict[int, set[int | None]] = {}
        branch_targets = set(self.seeds) | self.internal_starts
        for trace in traces:
            trace.pools.difference_update(trace.address_words)
            for target, _, _ in trace.calls | trace.tails:
                branch_targets.add(target)
            branch_targets.update(self.word(word) & ~1 for word in trace.table_words)
            if trace.kind != "veneer":
                for address in trace.code:
                    ins = self.decode(address, trace.isa)
                    if ins.branch and ins.target is not None:
                        branch_targets.add(ins.target[0])
        occupied = sorted((first, end, trace.address, kind) for trace in traces
                          for kind, spans in (
                              ("code", _merge([(a, a + size) for a, size in trace.code.items()])),
                              ("pool", _merge([(a, a + 4) for a in trace.pools])))
                          for first, end in spans)
        occupied_starts = [first for first, _, _, _ in occupied]

        def overlaps(first: int, end: int) -> list[tuple[int, int, int, str]]:
            index = max(0, bisect_right(occupied_starts, first) - 1)
            found = []
            while index < len(occupied) and occupied[index][0] < end:
                if first < occupied[index][1]:
                    found.append(occupied[index])
                index += 1
            return found

        def reference(target: int, owner: int | None) -> None:
            if self.base <= target < self.end:
                references.setdefault(target, set()).add(owner)

        for trace in traces:
            previous: tuple[int, Instruction] | None = None
            if trace.kind != "veneer":
                for address in sorted(trace.code):
                    ins = self.decode(address, trace.isa)
                    target = ins.address_target
                    if (ins.pc_add_register is not None and address not in branch_targets
                            and previous is not None and not previous[1].conditional and not ins.conditional
                            and previous[0] + previous[1].size == address):
                        constant = previous[1].register_constant
                        pool_constant = previous[1].register_pool
                        if pool_constant is not None and previous[0] in little_endian:
                            register, pool = pool_constant
                            if pool % 4 == 0 and self.base <= pool <= self.end - 4:
                                constant = (register, self.word(pool))
                        if constant is not None and constant[0] == ins.pc_add_register:
                            target = (address + 4 + constant[1]) & 0xFFFFFFFF
                    if target is not None:
                        trace.address_refs[address] = target
                        reference(target, trace.address)
                    if ins.target is not None:
                        reference(ins.target[0], trace.address)
                    previous = (address, ins)
            for word in trace.pools:
                reference(word, trace.address)
                reference(self.word(word), trace.address)
        for segment in (self.layout.rodata, self.layout.data):
            if segment.size:
                offset = self.layout.file_offset(segment.address)
                for index in range(0, segment.size - 3, 4):
                    reference(struct.unpack_from("<I", self.image, offset + index)[0], None)
        reference_starts = sorted(references)
        candidates = []
        for index, trace in enumerate(traces):
            targets: dict[int, list[int]] = {}
            for site, target in sorted(trace.address_refs.items()):
                if self.base <= target < self.end:
                    targets.setdefault(target, []).append(site)
            for target, sites in sorted(targets.items()):
                if any(kind == "code" for _, _, _, kind in overlaps(target, target + 1)):
                    continue  # Taking an instruction's address is not a data object.
                encoding = self.literal_encodings.get((trace.address, target))
                if encoding is not None and encoding["references"] != sorted(sites):
                    raise ValueError(f"reviewed literal {target:#x} has different address references")
                content_end = self._utf16_string_end(target) if encoding else self._string_end(target)
                if encoding is not None and content_end is None:
                    raise ValueError(f"reviewed UTF-16 literal {target:#x} has no complete printable bound")
                if encoding is not None and "end" in encoding:
                    actual = self.text[target - self.base:content_end - self.base]
                    if (content_end != encoding["end"]
                            or hashlib.sha256(actual).hexdigest() != encoding.get("image_interval_sha256")
                            or any(self.text[content_end - self.base:((content_end + 3) & ~3) - self.base])):
                        raise ValueError(f"reviewed UTF-16 literal {target:#x} changed its exact bound, contents or zero alignment")
                first = target if content_end is not None else target & ~3
                end = (content_end + 3) & ~3 if content_end is not None else (target + 7) & ~3
                candidates.append({
                    "start": first, "end": end, "target": target, "function": trace.address,
                    "kind": "utf16_string" if encoding else "string" if content_end is not None else "addressed_words",
                    "content_end": content_end,
                    "references": sites,
                    "limit": traces[index + 1].address if index + 1 < len(traces) else self.end,
                })
        candidates.sort(key=lambda item: (item["start"], item["end"], item["function"]))
        clusters: list[list[dict]] = []
        cluster_end = self.base
        for candidate in candidates:
            if not clusters or candidate["start"] >= cluster_end:
                clusters.append([])
                cluster_end = candidate["end"]
            clusters[-1].append(candidate)
            cluster_end = max(cluster_end, candidate["end"])
        blocked_words: set[int] = set()
        for cluster in clusters:
            first = cluster[0]["start"]
            end = max(candidate["end"] for candidate in cluster)
            owners = {candidate["function"] for candidate in cluster}
            incoming = set()
            index = bisect_right(reference_starts, first - 1)
            while index < len(reference_starts) and reference_starts[index] < end:
                incoming.update(references[reference_starts[index]])
                index += 1
            incoming.update(owner for _, _, owner, kind in overlaps(first, end) if kind == "pool")
            shared = len(owners) > 1 or bool(incoming - owners)
            nonlocal_object = any(candidate["start"] < candidate["function"]
                                  or candidate["end"] > candidate["limit"] for candidate in cluster)
            if shared:
                blocked_words.update(range(first & ~3, (end + 3) & ~3, 4))
            padding_start = max((candidate["content_end"] or 0 for candidate in cluster), default=0)
            shared_data = ((shared or nonlocal_object)
                           and all(candidate["kind"] in ("string", "utf16_string") for candidate in cluster)
                           and end <= self.end and padding_start > first
                           and not overlaps(first, end)
                           and not any(first <= address < end for address in branch_targets))
            # The exact terminated object is data. Claim alignment bytes only when they are zero.
            shared_end = end if shared_data and not any(
                self.text[address - self.base] for address in range(padding_start, end)) else padding_start
            if shared_data:
                blocked_words.update(range(first & ~3, (end + 3) & ~3, 4))
                self.shared_literals.append({
                    "start": first, "end": shared_end, "kind": "rodata",
                    "owners": sorted(owners),
                    "references": sorted(site for candidate in cluster for site in candidate["references"]),
                    "evidence": "shared_bounded_addressed_string" if shared else "nonlocal_bounded_addressed_string",
                })
            for candidate in cluster:
                trace = by_start[candidate["function"]]
                if shared_data:
                    for site in candidate["references"]:
                        trace.issues.discard((site, "literal_outside_function"))
                    continue
                reasons = set()
                if shared:
                    reasons.add("shared_address_literal")
                if candidate["start"] < trace.address:
                    reasons.add("address_literal_before_function")
                if candidate["end"] > candidate["limit"]:
                    reasons.add("address_literal_crosses_start")
                spans = overlaps(candidate["start"], candidate["end"])
                if any(kind == "code" for _, _, _, kind in spans):
                    reasons.add("address_literal_overlaps_code")
                if reasons:
                    self.literal_conflicts.append({
                        "function": trace.address, "target": candidate["target"], "end": candidate["end"],
                        "next_start": candidate["limit"], "references": candidate["references"],
                        "reasons": sorted(reasons),
                    })
                    trace.issues.update((site, reason) for site in candidate["references"] for reason in reasons)
                    continue
                cursor = candidate["start"]
                for pool_start, pool_end, _, _ in spans:
                    if cursor < pool_start:
                        trace.literals.append([cursor, pool_start])
                    cursor = max(cursor, pool_end)
                if cursor < candidate["end"]:
                    trace.literals.append([cursor, candidate["end"]])
                trace.literal_kinds[candidate["kind"]] += 1
                for site in candidate["references"]:
                    trace.issues.discard((site, "literal_outside_function"))
        self.shared_literal_words = blocked_words
        for trace in traces:
            trace.pools.difference_update(blocked_words)
            trace.literals = _merge([tuple(interval) for interval in trace.literals])
            change = trace.end - previous_ends[trace.address]
            if change:
                direction = "grown" if change > 0 else "shrunk"
                self.literal_extent_changes[f"functions_{direction}"] += 1
                self.literal_extent_changes[f"extent_bytes_{direction}"] += abs(change)
        self.literal_conflicts.sort(key=lambda item: (item["function"], item["target"]))


def _append_range(ranges: list[dict], start: int, end: int, kind: str, owner: int | None) -> None:
    if start >= end:
        return
    if ranges and ranges[-1]["end"] == start and ranges[-1]["kind"] == kind and ranges[-1]["owner"] == owner:
        ranges[-1]["end"] = end
    else:
        ranges.append({"start": start, "end": end, "kind": kind, "owner": owner})


def _split_linker_tail_extents(catalogue: dict, analyzer: Analyzer, protected: dict[int, dict]) -> dict:
    """Partition proved branchnop sections after v5 recovery, without rediscovering starts.

    Preserve the census's labels, but also consider independently proved stack-LR
    restores that its textual heuristic misses. Neither population is sufficient:
    require successor evidence and reject every control/data crossing.
    Keeping this pass last preserves earlier entries, literals and protected C extents.
    Protection compares adopted records, so already-adopted split parts remain
    reproducible when those parts are later added to an accepted manifest.
    """
    functions = catalogue["functions"]
    by_address = {function["address"]: function for function in functions}
    starts = sorted(by_address)
    entries = {entry["address"]: entry for function in functions for entry in function["entries"]}
    detailed = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)
    detailed.detail = True
    decoded = {}

    def instruction(address: int):
        if address not in decoded:
            offset = address - analyzer.base
            decoded[address] = (next(detailed.disasm(analyzer.text[offset:offset + 4], address, 1), None)
                                if analyzer.inside(address) and address + 4 <= analyzer.end else None)
        return decoded[address]

    def owner(address: int) -> dict | None:
        index = bisect_right(starts, address) - 1
        function = by_address[starts[index]] if index >= 0 else None
        return function if function and address < function["address"] + function["size"] else None

    def is_data(function: dict, address: int) -> bool:
        return any(first <= address < end for first, end in function["pools"] + function["literals"])

    core_arguments = {arm.ARM_REG_R0, arm.ARM_REG_R1, arm.ARM_REG_R2, arm.ARM_REG_R3, arm.ARM_REG_R12}
    fp_arguments = set(range(arm.ARM_REG_S0, arm.ARM_REG_S15 + 1))
    caller_saved = core_arguments | fp_arguments
    # A D register overlaps two S lanes: a partial S write must not initialize D.
    register_units = {arm.ARM_REG_D0 + index: {arm.ARM_REG_S0 + 2 * index, arm.ARM_REG_S0 + 2 * index + 1}
                      for index in range(16)}
    result_classes = ({arm.ARM_REG_R0}, set(range(arm.ARM_REG_S0, arm.ARM_REG_S7 + 1)))

    def registers(ins) -> tuple[set[int], set[int]]:
        read, wrote = (set(items) for items in ins.regs_access())
        # Capstone omits the VFP register lists of VSTM/VLDM from regs_access
        # and marks those operands with access=0. Scalar VSTR/VLDR are complete.
        fp_list = {op.reg for op in ins.operands if op.type == arm.ARM_OP_REG
                   and (arm.ARM_REG_S0 <= op.reg <= arm.ARM_REG_S31
                        or arm.ARM_REG_D0 <= op.reg <= arm.ARM_REG_D31)}
        if ins.id in (arm.ARM_INS_VSTMIA, arm.ARM_INS_VSTMDB, arm.ARM_INS_VPUSH):
            read.update(fp_list)
        elif ins.id in (arm.ARM_INS_VLDMIA, arm.ARM_INS_VLDMDB, arm.ARM_INS_VPOP):
            wrote.update(fp_list)
        return ({unit for register in read for unit in register_units.get(register, {register})},
                {unit for register in wrote for unit in register_units.get(register, {register})})

    def as_if_call(site: int) -> dict:
        written, live, flags = set(), set(), False
        for address in range(site + 4, site + 52, 4):
            function = owner(address)
            if function is None or function["isa"] != "arm" or is_data(function, address):
                break
            ins = instruction(address)
            if ins is None:
                break
            read, wrote = registers(ins)
            live.update((read & caller_saved) - written)
            # Keep both possible result classes, not a fictitious R0+VFP result.
            # HFA returns can occupy D0-D3 (S0-S7); R0 alone is a core result.
            # Thus S0 alone is not proof, but an untouched object pointer plus
            # pass-through S0 is, as are argument lanes beyond the result block.
            if not any(live <= result for result in result_classes):
                return {"as_if_call": "reads_args",
                        "live_argument_registers": sorted(detailed.reg_name(register) for register in live)}
            if (ins.cc not in (arm.ARM_CC_AL, arm.ARM_CC_INVALID) or arm.ARM_REG_CPSR in read) and not flags:
                return {"as_if_call": "reads_flags"}
            if ins.cc in (arm.ARM_CC_AL, arm.ARM_CC_INVALID):
                written.update(wrote & caller_saved)
                flags |= arm.ARM_REG_CPSR in wrote
            if arm.ARM_REG_PC in wrote or ins.mnemonic in ("b", "bl", "bx", "blx"):
                break
        return {"as_if_call": "consistent"}

    def leaf_setter(target: int, function: dict) -> bool:
        """Prove a bounded, stack-independent field setter, not just a plausible opcode."""
        written, stores = set(), False
        end = function["address"] + function["size"]
        for address in range(target, min(target + 48, end), 4):
            if is_data(function, address) or not any(first <= address < last for first, last in function["code_ranges"]):
                return False
            ins = instruction(address)
            if ins is None or ins.cc not in (arm.ARM_CC_AL, arm.ARM_CC_INVALID):
                return False
            read, wrote = registers(ins)
            decoded_ins = analyzer.decode(address, "arm")
            if analyzer.word(address) in (0xE12FFF1E, 0xE1A0F00E):
                return stores and address + 4 == end
            if (decoded_ins.call or decoded_ins.branch or decoded_ins.stop or decoded_ins.terminal
                    or decoded_ins.pools or arm.ARM_REG_SP in read | wrote or arm.ARM_REG_LR in read | wrote
                    or read - core_arguments or wrote - core_arguments
                    or (read & core_arguments) - written - {arm.ARM_REG_R0}
                    or arm.ARM_REG_R0 in wrote or ins.writeback
                    or any(op.type == arm.ARM_OP_MEM and (op.mem.base != arm.ARM_REG_R0 or op.mem.index)
                           for op in ins.operands)):
                return False
            stores |= ins.id in (arm.ARM_INS_STR, arm.ARM_INS_STRB, arm.ARM_INS_STRH)
            written.update(wrote)
        return False

    def field_read_successor(target: int, function: dict) -> bool:
        """A nonstoring leaf wrapper followed by a complete scalar field getter.

        The exact linker NOP is still required. Unlike an arbitrary R0-only
        arithmetic suffix, a field load has an object input and a closed return
        contract, even though the may-read ABI analysis also permits R0 results.
        A final direct tail transfer must reach an independently known returning
        ARM owner; calls and unknown/interior targets cannot establish this proof.
        """
        end = function["address"] + function["size"]
        if (end != target + 8 or not analyzer._nonstoring_prefix(function["address"], target)
                or any(is_data(function, address) or address not in code_by_owner[function["address"]]
                       for address in (target, target + 4))):
            return False
        load, terminal = instruction(target), analyzer.decode(target + 4, "arm")
        if (load is None or terminal is None or load.cc != arm.ARM_CC_AL or load.writeback
                or load.id not in (arm.ARM_INS_LDR, arm.ARM_INS_LDRB, arm.ARM_INS_LDRH,
                                   arm.ARM_INS_LDRSB, arm.ARM_INS_LDRSH)
                or len(load.operands) != 2 or load.operands[0].type != arm.ARM_OP_REG
                or load.operands[0].reg != arm.ARM_REG_R0 or load.operands[1].type != arm.ARM_OP_MEM
                or load.operands[1].mem.base != arm.ARM_REG_R0 or load.operands[1].mem.index
                or terminal.conditional or terminal.call):
            return False
        if terminal.returns:
            return analyzer.word(target + 4) in (0xE12FFF1E, 0xE1A0F00E)
        tail = by_address.get(terminal.target[0]) if terminal.branch and terminal.target else None
        return (tail is not None and terminal.target[1] == tail["isa"] == "arm"
                and tail["address"] != function["address"] and not tail["noreturn"])

    def field_access_graph_successor(target: int, function: dict) -> bool:
        """Prove a complete scalar-memory R0-input leaf CFG, including bounded loops.

        Definite register initialization is intersected at joins. The erased-tail
        interpretation still requires an exact NOP, a nonstoring wrapper prefix,
        and the outer control/data/frame/protection vetoes.
        """
        end = function["address"] + function["size"]
        if (not 8 < end - target <= 64 or not analyzer._nonstoring_prefix(function["address"], target)
                or any(is_data(function, site) or site not in code_by_owner[function["address"]]
                       for site in range(target, end, 4))):
            return False
        first = instruction(target)
        scalar_loads = (arm.ARM_INS_LDR, arm.ARM_INS_LDRB, arm.ARM_INS_LDRH,
                        arm.ARM_INS_LDRSB, arm.ARM_INS_LDRSH)
        scalar_stores = (arm.ARM_INS_STR, arm.ARM_INS_STRB, arm.ARM_INS_STRH)
        first_load = (first is not None and first.id in scalar_loads and len(first.operands) == 2
                      and first.operands[1].type == arm.ARM_OP_MEM
                      and first.operands[1].mem.base == arm.ARM_REG_R0 and not first.operands[1].mem.index)
        first_copy = (first is not None and first.id == arm.ARM_INS_MOV and len(first.operands) == 2
                      and first.operands[1].type == arm.ARM_OP_REG and first.operands[1].reg == arm.ARM_REG_R0)
        if first is None or first.cc != arm.ARM_CC_AL or first.writeback or not (first_load or first_copy):
            return False
        incoming, work, returns = {target: {arm.ARM_REG_R0}}, [target], set()
        allowed = core_arguments | {arm.ARM_REG_CPSR}
        loads, branches = set(), set()
        while work:
            site = work.pop()
            ins, flow = instruction(site), analyzer.decode(site, "arm")
            if ins is None or flow is None:
                return False
            initialized = incoming[site]
            if flow.returns:
                if flow.conditional or analyzer.word(site) not in (0xE12FFF1E, 0xE1A0F00E):
                    return False
                returns.add(site)
                continue
            read, wrote = registers(ins)
            read.discard(arm.ARM_REG_PC)  # Direct branches read architectural PC.
            if flow.branch:
                wrote.discard(arm.ARM_REG_PC)
            if flow.conditional:
                read.add(arm.ARM_REG_CPSR)
            memory = [operand.mem for operand in ins.operands if operand.type == arm.ARM_OP_MEM]
            scalar_store = ins.id in scalar_stores and memory and all(
                operand.base in core_arguments and not operand.index for operand in memory)
            if ins.id in scalar_loads:
                loads.add(site)
            if (flow.call or flow.stop or flow.terminal or flow.pools or ins.writeback
                    or _memory_side_effect(ins) and not scalar_store or read - allowed or wrote - allowed
                    or read - initialized):
                return False
            outgoing = initialized | wrote if not flow.conditional else initialized
            successors = []
            if flow.branch:
                branches.add(site)
                if flow.target is None or flow.target[1] != "arm":
                    return False
                successors.append(flow.target[0])
            if not flow.branch or flow.conditional:
                successors.append(site + 4)
            for successor in successors:
                if not target <= successor < end:
                    return False
                merged = incoming[successor] & outgoing if successor in incoming else outgoing
                if successor not in incoming or merged != incoming[successor]:
                    incoming[successor] = merged
                    work.append(successor)
        return bool(returns and loads and branches) and incoming.keys() == set(range(target, end, 4))

    def closed_tail_successor(target: int, function: dict, restored: int | None) -> bool:
        if restored is None or target + 4 != function["address"] + function["size"]:
            return False
        terminal = analyzer.decode(target, "arm")
        tail = by_address.get(terminal.target[0]) if terminal and terminal.branch and terminal.target else None
        return (tail is not None and not terminal.conditional and terminal.target[1] == tail["isa"] == "arm"
                and tail["address"] != function["address"] and not tail["noreturn"])

    pointers = set(analyzer._external_pointers())
    for function in functions:
        for first, end in function["pools"]:
            for address in range(first, end, 4):
                value = analyzer.word(address)
                pointers.add((value & ~1, "thumb" if value & 1 else "arm"))
    protected_owners = {}
    for address in protected:
        if (function := owner(address)) is not None:
            protected_owners.setdefault(function["address"], []).append(address)
    conflicted = {conflict["function"] for conflict in catalogue["literal_conflicts"]}
    inventory, nop_inventory = [], []
    classes: Counter[str] = Counter()
    code_by_owner = {}
    for function in functions:
        if function["isa"] != "arm":
            continue
        code = {address for first, end in function["code_ranges"] for address in range(first, end, 4)}
        sites = {address for address in code if analyzer.word(address) == 0xE1A00000}
        if not sites:
            continue
        code_by_owner[function["address"]] = code
        saved = frame_saved = frame_restored = False
        restored_at = None
        stack_offset, saved_lr_slot = 0, None
        for address in range(function["address"], max(sites) + 4, 4):
            if is_data(function, address):
                continue
            if address in sites:
                previous = instruction(address - 4) if address > function["address"] else None
                following = instruction(address + 4)
                if address in entries and address + 4 in entries:
                    category = "tail:nop_entry"
                elif address + 4 in entries:
                    category = "tail:falls_into_entry"
                elif address in entries:
                    category = "tail:leading_nop_of_start"
                elif previous is not None and (previous.mnemonic == "vpop" or (
                        previous.mnemonic in ("pop", "ldm") and "lr" in previous.op_str and "pc" not in previous.op_str)):
                    category = "tail:after_epilogue"
                elif following is not None and following.mnemonic in ("push", "stmdb") and "lr" in following.op_str:
                    category = "tail:before_prologue"
                else:
                    category = "call:in_frame" if saved else "tail:in_leaf"
                classes[category] += 1
                row = {"owner": function["address"], "site": address, "target": address + 4,
                       "census_class": category, "census_lr_saved": saved, "lr_saved": frame_saved,
                       "restored_frame_at": restored_at if frame_restored and stack_offset == 0 else None,
                       **as_if_call(address)}
                nop_inventory.append(row)
                if category.startswith("tail:") or frame_restored and stack_offset == 0:
                    inventory.append(row)
                else:
                    row["decision"] = "in_frame_not_tail"
            word = analyzer.word(address)
            ins = instruction(address)
            if ins is None:
                continue
            block = word & 0x0E000000 == 0x08000000
            stack_lr = block and (word >> 16) & 15 == 13 and bool(word & 0x4000)
            stack_memory = any(op.type == arm.ARM_OP_MEM and op.mem.base == arm.ARM_REG_SP for op in ins.operands)
            single_lr = (word & 0x0E0F0000 == 0x040D0000 and (word >> 12) & 15 == 14)
            stack_save = stack_lr and not word & 0x00100000 or (
                single_lr and ins.id == arm.ARM_INS_STR and stack_memory)
            stack_restore = stack_lr and bool(word & 0x00100000) or (
                single_lr and ins.id == arm.ARM_INS_LDR and stack_memory)
            unconditional = ins.cc in (arm.ARM_CC_AL, arm.ARM_CC_INVALID)
            slot, adjustment = None, 0
            if block and (word >> 16) & 15 == 13:
                count = (word & 0xFFFF).bit_count()
                upward, preindexed = bool(word & 0x00800000), bool(word & 0x01000000)
                first = (4 if preindexed else 0) if upward else -4 * count + (0 if preindexed else 4)
                if stack_lr and stack_offset is not None:
                    slot = stack_offset + first + 4 * (word & 0x3FFF).bit_count()
                if word & 0x00200000:
                    adjustment = 4 * count * (1 if upward else -1)
            elif word & 0x0E0F0000 == 0x040D0000:
                displacement = (word & 0xFFF) * (1 if word & 0x00800000 else -1)
                if single_lr and stack_offset is not None:
                    slot = stack_offset + (displacement if word & 0x01000000 else 0)
                if word & 0x00200000 or not word & 0x01000000:
                    adjustment = displacement
            elif (ins.id in (arm.ARM_INS_ADD, arm.ARM_INS_SUB) and len(ins.operands) == 3
                  and ins.operands[0].type == ins.operands[1].type == arm.ARM_OP_REG
                  and ins.operands[0].reg == ins.operands[1].reg == arm.ARM_REG_SP
                  and ins.operands[2].type == arm.ARM_OP_IMM):
                adjustment = ins.operands[2].imm * (1 if ins.id == arm.ARM_INS_ADD else -1)
            elif arm.ARM_REG_SP in ins.regs_access()[1]:
                stack_offset = None
            if stack_save:
                saved_lr_slot = slot if unconditional and not frame_saved else None
                frame_saved, frame_restored = True, False
            elif stack_restore and unconditional:
                matched_slot = slot is not None and slot == saved_lr_slot
                frame_restored = frame_saved and matched_slot
                # Ordinary stack-register-list restores retain v6 semantics;
                # newly recognized single loads need a matching saved slot.
                if stack_lr or matched_slot:
                    frame_saved = False
                restored_at = address if frame_restored else None
                saved_lr_slot = None
            elif arm.ARM_REG_LR in ins.regs_access()[1]:
                frame_restored = False
            if adjustment:
                stack_offset = stack_offset + adjustment if unconditional and stack_offset is not None else None
            if (block or word in (0xE52DE004, 0xE49DE004)) and "lr" in ins.op_str:
                # Preserve the published census's textual LR heuristic, but
                # independently recognize actual stack saves/restores above.
                if ins.mnemonic in ("push", "stmdb", "stmfd"):
                    saved = True
                elif ins.mnemonic in ("pop", "ldm", "ldmia", "ldmfd") and "pc" not in ins.op_str:
                    saved = False
    for address in range(analyzer.base, analyzer.end - 3, 4):
        if analyzer.word(address) != 0xE1A00000:
            continue
        function = owner(address)
        if function is None:
            classes["tail:outside_catalogue"] += 1
            row = {"owner": None, "site": address, "target": address + 4,
                   "census_class": "tail:outside_catalogue", **as_if_call(address)}
            inventory.append(row)
            nop_inventory.append(row)
        elif address not in code_by_owner.get(function["address"], set()):
            nop_inventory.append({"owner": function["address"], "site": address, "target": address + 4,
                                  "census_class": "not_executed", "decision": "not_executed_arm_code"})

    selected = {}
    for row in inventory:
        parent, target = row["owner"], row["target"]
        if parent is None:
            row["decision"] = "unowned_tail_no_head_start"
            continue
        function = by_address[parent]
        if target in by_address:
            row["decision"] = "already_separate"
            continue
        if target not in code_by_owner[parent]:
            row["decision"] = "successor_not_owned_code"
            continue
        evidence = []
        if target in entries and entries[target]["isa"] == "arm":
            evidence.append("existing_entry")
        if analyzer.prologue(target):
            evidence.append("prologue")
        if (target, "arm") in pointers:
            evidence.append("pointer")
        if analyzer.word(target) in (0xE12FFF1E, 0xE1A0F00E):
            evidence.append("return_only_successor")
        if row["as_if_call"] == "reads_args":
            evidence.append("aapcs_live_arguments")
        if row["restored_frame_at"] is not None and leaf_setter(target, function):
            evidence.append("restored_frame_leaf_setter")
        if field_read_successor(target, function):
            evidence.append("nonstoring_wrapper_field_read")
        if field_access_graph_successor(target, function):
            evidence.append("nonstoring_wrapper_field_access_graph")
        if closed_tail_successor(target, function, row["restored_frame_at"]):
            evidence.append("restored_frame_closed_tail")
        row["start_evidence"] = evidence
        crossings, data_crossings = [], []
        for address in sorted(code_by_owner[parent]):
            ins = analyzer.decode(address, "arm")
            if ins.branch and ins.target is not None:
                destination, isa = ins.target
                if parent <= destination < parent + function["size"] and (address < target) != (destination < target):
                    crossings.append({"site": address, "target": destination, "isa": isa})
            for first, end in ins.pools:
                if parent <= first < parent + function["size"] and (address < target) != (first < target):
                    data_crossings.append({"site": address, "target": first, "end": end})
            if ins.switch is not None:
                style, register = ins.switch
                count = analyzer._switch_bound(address, register)
                if style == "addresses" and count is not None:
                    for word in range(address + 8, address + 8 + 4 * count, 4):
                        destination = analyzer.word(word) & ~1
                        if parent <= destination < parent + function["size"] and (address < target) != (destination < target):
                            crossings.append({"site": address, "target": destination, "table_word": word})
        row["crossing_branches"] = crossings
        row["crossing_data"] = data_crossings
        if function["kind"] not in ("function", "multi_entry"):
            row["decision"] = "classified_owner"
        elif crossings:
            row["decision"] = "control_flow_crosses"
        elif data_crossings or parent in conflicted:
            row["decision"] = "literal_ownership_crosses"
        elif row["lr_saved"]:
            row["decision"] = "frame_not_restored"
        elif not evidence:
            row["decision"] = "no_independent_successor_witness"
        else:
            row["decision"] = "split"
            selected.setdefault(parent, []).append(row)

    partitions = {}
    for parent, proposals in list(selected.items()):
        function = by_address[parent]
        bounds = [parent] + sorted(row["target"] for row in proposals) + [parent + function["size"]]
        if any(sum(first <= entry["address"] < end and entry["address"] != first for entry in function["entries"])
               for first, end in zip(bounds, bounds[1:])):
            for row in proposals:
                row["decision"] = "additional_interior_entries"
            del selected[parent]
            continue
        partitions[parent] = bounds

    new_functions, split_records = [], []
    for function in functions:
        parent = function["address"]
        if parent not in partitions:
            new_functions.append(function)
            continue
        bounds = partitions[parent]
        proposals = {row["target"]: row for row in selected[parent]}
        parts, part_splits = [], []
        for index, (first, end) in enumerate(zip(bounds, bounds[1:])):
            code = [[max(lo, first), min(hi, end)] for lo, hi in function["code_ranges"] if lo < end and first < hi]
            pools = [[lo, hi] for lo, hi in function["pools"] if first <= lo < hi <= end]
            literals = [[lo, hi] for lo, hi in function["literals"] if first <= lo < hi <= end]
            own = next((entry for entry in function["entries"] if entry["address"] == first), None)
            evidence = set(own["evidence"] if own is not None else ())
            if index:
                evidence.add("linker_branchnop")
                evidence.update(item for item in proposals[first]["start_evidence"] if item in ("prologue", "pointer"))
            else:
                evidence.update(function["evidence"])
            own = {"address": first, "isa": "arm", "kind": "entry", "evidence": sorted(evidence),
                   "sources": list(own["sources"]) if own is not None else []}
            reasons = set()
            if function["noreturn"]:
                reasons.add("no_return_sites")
                if index + 1 < len(bounds) - 1:
                    reasons.add("noreturn_tail")
                for lo, hi in code:
                    for address in range(lo, hi, 4):
                        ins = analyzer.decode(address, "arm")
                        if ins.terminal is not None:
                            reasons.add(ins.terminal)
                        if ins.call and ins.target is not None and ins.target[0] in analyzer.noreturn:
                            reasons.add("noreturn_call")
                        if ins.branch and ins.target is not None and not first <= ins.target[0] < end:
                            reasons.add("noreturn_tail")
                if reasons == {"no_return_sites"}:
                    reasons.add("closed_loop")
            parts.append({"address": first, "size": end - first, "code_size": max(hi for _, hi in code) - first,
                          "isa": "arm", "kind": "function", "pools": pools, "literals": literals,
                          "code_ranges": code, "evidence": sorted(evidence), "entries": [own],
                          "noreturn": function["noreturn"], "noreturn_reasons": sorted(reasons)})
            if index:
                row = proposals[first]
                row.update(head_address=bounds[index - 1], head_size=first - bounds[index - 1], tail_size=end - first)
                part_splits.append(dict(row))
        changed = []
        for address in protected_owners.get(parent, []):
            proposed = next(part for part in parts if part["address"] <= address < part["address"] + part["size"])
            if proposed != protected[address]:
                changed.append({"address": address, "expected_owner": protected[address]["address"],
                                "expected_size": protected[address]["size"], "proposed_owner": proposed["address"],
                                "proposed_size": proposed["size"]})
        if changed:
            for row in selected[parent]:
                row.update(decision="protected_owner", protected_changes=changed)
            del partitions[parent]
            new_functions.append(function)
        else:
            new_functions.extend(parts)
            split_records.extend(part_splits)
    new_functions.sort(key=lambda function: function["address"])
    new_starts = [function["address"] for function in new_functions]

    def new_owner(address: int) -> int:
        return new_starts[bisect_right(new_starts, address) - 1]

    new_by_address = {function["address"]: function for function in new_functions}
    for address, expected in protected.items():
        actual = new_by_address.get(new_owner(address))
        if actual != expected:
            raise ValueError(f"protected function {address:#x} cannot retain its adopted catalogue record")

    ranges = []
    for item in catalogue["ranges"]:
        if item["owner"] not in partitions:
            ranges.append(item)  # Segment-padding ranges must retain their page boundaries.
            continue
        bounds = partitions[item["owner"]]
        for first, end in zip(bounds, bounds[1:]):
            _append_range(ranges, max(first, item["start"]), min(end, item["end"]), item["kind"], first)
    catalogue["functions"], catalogue["ranges"] = new_functions, ranges
    catalogue["boundary_merges"] = [dict(merge, retained_address=new_owner(merge["address"]))
                                     for merge in catalogue["boundary_merges"] if merge["address"] not in new_starts]
    catalogue["entry_groups"] = [group for group in catalogue["entry_groups"] if group["address"] not in partitions]
    issues = {(item["address"], item["issue"]) for item in catalogue["anomalies"]
              if not (item["address"] in partitions and item["issue"] == "unexplained_function_bytes")}
    for function in new_functions:
        if function["address"] in partitions or function["address"] not in by_address:
            explained = sum(end - first for first, end in function["code_ranges"] + function["pools"] + function["literals"])
            if explained != function["size"]:
                issues.add((function["address"], "unexplained_function_bytes"))
    issues.update((item["address"], item["issue"]) for item in validate_catalogue(catalogue, analyzer.image, analyzer.layout))
    catalogue["anomalies"] = [{"address": address, "issue": issue} for address, issue in sorted(issues)]
    catalogue["linker_tail_inventory"] = sorted(inventory, key=lambda row: row["site"])
    catalogue["linker_nop_inventory"] = sorted(nop_inventory, key=lambda row: row["site"])
    catalogue["linker_tail_extent_splits"] = sorted(split_records, key=lambda row: row["site"])
    stats = catalogue["stats"]
    stats.update(functions=len(new_functions),
                 functions_by_isa=dict(sorted(Counter(function["isa"] for function in new_functions).items())),
                 functions_by_kind=dict(sorted(Counter(function["kind"] for function in new_functions).items())),
                 starts_by_evidence=dict(sorted(Counter(item for function in new_functions for item in function["evidence"]).items())),
                 anomalies=len(issues), anomalies_by_issue=dict(sorted(Counter(issue for _, issue in issues).items())),
                 merged_starts=len(catalogue["boundary_merges"]),
                 merges_by_reason=dict(sorted(Counter(reason for merge in catalogue["boundary_merges"] for reason in merge["reasons"]).items())),
                 fallthrough_groups=len(catalogue["entry_groups"]),
                 absorbed_function_starts=sum(len(group["members"]) - 1 for group in catalogue["entry_groups"]),
                 secondary_entries=sum(len(function["entries"]) - 1 for function in new_functions),
                 nop_entries=sum(entry["kind"] == "nop_entry" for function in new_functions for entry in function["entries"]),
                 noreturn_functions=sum(function["noreturn"] for function in new_functions),
                 functions_with_literals=sum(bool(function["literals"]) for function in new_functions),
                 prologue_only_starts=sum(function["evidence"] == ["prologue"] for function in new_functions),
                 linker_tail_census=dict(sorted(classes.items())),
                 linker_nop_words=len(nop_inventory),
                 linker_tail_extent_splits=len(split_records))
    stats["tail_calls"] += len(split_records)
    return catalogue


def check_protected_functions(catalogue: dict, protected: dict[int, dict]) -> None:
    """Compare complete adopted owners, including classifications and evidence."""
    functions = catalogue["functions"]
    starts = [function["address"] for function in functions]
    for address, expected in protected.items():
        index = bisect_right(starts, address) - 1
        actual = functions[index] if index >= 0 else None
        if actual != expected or not actual["address"] <= address < actual["address"] + actual["size"]:
            raise ValueError(f"protected function {address:#x} cannot retain its adopted catalogue record")


def preserve_prior_entries(catalogue: dict, prior_functions: list[dict]) -> None:
    """Retain weak legacy labels without claiming independently proved callability.

    An authenticated initializer can precede a former prologue-only owner. Keep
    that old address as a conservative secondary entry only when every old owned
    span survives in the recovered owner and the address is an ARM instruction
    boundary. A subsequently adopted legacy label is carried forward under the
    same owner and span checks. Other entry losses fail closed.
    """
    functions = catalogue["functions"]
    starts = [function["address"] for function in functions]
    entries = {(entry["address"], entry["isa"])
               for function in functions for entry in function["entries"]}
    retained = []
    for before in prior_functions:
        for entry in before["entries"]:
            address, isa = entry["address"], entry["isa"]
            if (address, isa) in entries:
                continue
            index = bisect_right(starts, address) - 1
            after = functions[index] if index >= 0 else None
            original_label = (before["evidence"] == ["prologue"] and before["kind"] == "function"
                              and len(before["entries"]) == 1 and entry["evidence"] == ["prologue"])
            adopted_label = (after is not None and before["kind"] == "multi_entry"
                             and before["address"] == after["address"]
                             and "relative_init_array" in before["evidence"]
                             and entry["kind"] == "secondary_entry"
                             and entry["evidence"] == ["legacy_prologue"])
            if (after is None or not (original_label or adopted_label) or entry["sources"]
                    or isa != after["isa"] or isa != "arm" or address % 4
                    or not after["address"] < address < after["address"] + after["size"]
                    or "relative_init_array" not in after["evidence"]
                    or after["kind"] not in ("function", "multi_entry")
                    or not any(first <= address < end for first, end in after["code_ranges"])
                    or not all(any(nlo <= lo < hi <= nhi for nlo, nhi in after[kind])
                               for kind in ("code_ranges", "pools", "literals") for lo, hi in before[kind])):
                raise ValueError(f"prior entry {address:#x}/{isa} cannot be preserved")
            after["entries"].append({
                "address": address, "isa": isa, "kind": "secondary_entry",
                "evidence": ["legacy_prologue"], "sources": [],
            })
            after["entries"].sort(key=lambda item: item["address"])
            after["kind"] = "multi_entry"
            entries.add((address, isa))
            retained.append({
                "old_address": address, "address": after["address"],
                "old_size": before["size"], "size": after["size"], "isa": isa,
                "old_entry_evidence": list(entry["evidence"]),
                "entry_evidence": ["legacy_prologue"],
                "reason": "preserved prior prologue-derived label; no independent callability witness or C credit",
            })
    catalogue["legacy_entry_preservation"] = sorted(retained, key=lambda row: row["old_address"])
    catalogue["stats"]["secondary_entries"] = sum(len(function["entries"]) - 1 for function in functions)


def apply_classifications(catalogue: dict, records: list[dict]) -> None:
    """Apply reviewed evidence, not semantic guesses or a library-address heuristic.

    Compiler ABI contracts need a compiler-emission witness; compiler-outlined
    helpers need named whole-body equality. Ordinary library code needs a known
    library identity. Raw ABI exclusions need an actual compiler-limit/shape proof.
    Check all records before mutation, binding identity to image, ISA and extent.
    Existing multi-entry topology remains explicit and independently noncreditable.
    """
    functions = {function["address"]: function for function in catalogue["functions"]}
    standards = {"compiler_abi": ("runtime", "compiler_emission"),
                 "compiler_generated": ("runtime", "whole_body_identity"),
                 "known_library": ("library", "known_library_identity"),
                 "unrepresentable_abi": ("assembly", "compiler_limit")}
    seen = set()
    for row in records:
        address = row["address"]
        function = functions.get(address)
        expected = standards.get(row.get("basis"))
        if (address in seen or function is None or expected is None or row.get("kind") != expected[0]
                or not row.get("identity") or not row.get("evidence") or not row.get(expected[1])
                or row.get("image_sha256") != catalogue["image_sha256"]
                or (row.get("size"), row.get("isa")) != (function["size"], function["isa"])):
            raise ValueError(f"classification {address:#x} lacks extent-bound identity/evidence")
        seen.add(address)
    for row in records:
        function = functions[row["address"]]
        if function["kind"] == "function":
            function["kind"] = row["kind"]
        function["classification_evidence"] = dict(row)
    stats = catalogue["stats"]
    stats["functions_by_kind"] = dict(sorted(Counter(function["kind"] for function in functions.values()).items()))
    stats["classified_functions"] = len(records)
    stats["creditable_functions"] = sum(function["kind"] == "function" for function in functions.values())
    stats["creditable_bytes"] = sum(function["size"] for function in functions.values() if function["kind"] == "function")


def build_catalogue(image: bytes, layout: ctrfmt.CodeLayout, image_name: str = "code.bin", *,
                    protected_functions: dict[int, dict] | None = None,
                    classifications: list[dict] | None = None,
                    literal_encodings: list[dict] | None = None,
                    relative_pointer_tables: list[dict] | None = None,
                    prior_functions: list[dict] | None = None,
                    closure: dict | None = None) -> dict:
    """Return deterministic schema-v1 metadata, preserving ambiguity as anomalies/unknown."""
    analyzer = Analyzer(image, layout)
    protected_functions = protected_functions or {}
    metadata_transitions = []
    if closure is not None:
        if __package__:
            from . import catalogue_closure
        else:
            import catalogue_closure
        protected_functions, metadata_transitions = catalogue_closure.resolve_protected_records(
            protected_functions, analyzer, closure, literal_encodings or [])
    for record in classifications or []:
        if "original_extent_sha256" in record:
            first = layout.file_offset(record["address"])
            if hashlib.sha256(image[first:first + record["size"]]).hexdigest() != record["original_extent_sha256"]:
                raise ValueError("classification whole-extent witness differs from the image")
    for row in literal_encodings or []:
        key = row["function"], row["target"]
        if key in analyzer.literal_encodings or row.get("encoding") != "utf-16le" or not row.get("references"):
            raise ValueError("reviewed literal needs unique owner/target, encoding and reference sites")
        analyzer.literal_encodings[key] = row
    analyzer.discover()
    analyzer.include_relative_pointer_tables(relative_pointer_tables or [])
    protected_entries = {entry["address"]: entry for function in (protected_functions or {}).values()
                         for entry in function["entries"]}
    for _ in range(64):
        for address, expected in (protected_functions or {}).items():
            seed = analyzer.seeds.get(address)
            if seed is not None and expected["address"] == address:
                additions = (seed.evidence - set(expected["evidence"])) & {"pointer", "relative_init_array", "veneer_target"}
                analyzer.protected_start_observations.update((address, item) for item in additions)
                seed.evidence.difference_update(additions)
        traces = analyzer.group_fallthroughs(analyzer.refine())
        analyzer.include_address_literals(traces)
        if analyzer.promote_leading_instructions(traces, protected_functions or {}):
            continue
        if analyzer.promote_missing_targets(traces):
            continue
        analyzer.record_entries(traces, protected_entries)
        if analyzer.promote_linker_entries(traces):
            continue
        if not analyzer.promote_pointer_entries(traces):
            break
    else:
        raise ValueError("entry-target discovery did not converge after 64 rounds")
    for (owner, target), row in analyzer.literal_encodings.items():
        if not any(trace.address == owner and all(trace.address_refs.get(site) == target
                                                for site in row["references"]) for trace in traces):
            raise ValueError(f"reviewed literal {target:#x} has no retained owner/reference")
    anomalies: set[tuple[int, str]] = set()
    functions = []
    owners: list[int | None] = [None] * layout.text.size
    shared_data = {address for row in analyzer.shared_literals for address in range(row["start"], row["end"])}
    for trace in traces:
        if not trace.code:
            anomalies.add((trace.address, "empty_function_start"))
            continue
        code_ranges = _merge([(address, address + size) for address, size in trace.code.items()])
        pools = _merge([(address, address + 4) for address in trace.pools])
        functions.append({
            "address": trace.address, "size": trace.end - trace.address,
            "code_size": max(address + size for address, size in trace.code.items()) - trace.address,
            "isa": trace.isa, "kind": trace.kind, "pools": pools, "literals": trace.literals,
            "code_ranges": code_ranges, "evidence": sorted(analyzer.seeds[trace.address].evidence),
            "entries": [{**entry, "evidence": sorted(entry["evidence"]), "sources": sorted(entry["sources"])}
                        for _, entry in sorted(trace.entries.items())],
            "noreturn": trace.address in analyzer.noreturn,
            "noreturn_reasons": analyzer.noreturn.get(trace.address, []),
        })
        anomalies.update(trace.issues)
        for first, end in code_ranges + pools + trace.literals:
            for address in range(first, end):
                offset = address - analyzer.base
                if owners[offset] is not None and owners[offset] != trace.address:
                    anomalies.add((address, "overlapping_function_ownership"))
                owners[offset] = trace.address
        for first, end in code_ranges:
            if any(first < pool_end and pool_start < end for pool_start, pool_end in pools):
                anomalies.add((first, "pool_overlaps_code"))
        explained = _merge([tuple(interval) for interval in code_ranges + pools + trace.literals])
        if sum(end - first for first, end in explained) != trace.end - trace.address:
            anomalies.add((trace.address, "unexplained_function_bytes"))
    ranges: list[dict] = []
    text_end = analyzer.end
    cursor = analyzer.base
    while cursor < text_end:
        offset = cursor - analyzer.base
        owner = owners[offset]
        if owner is not None:
            end = cursor + 1
            while end < text_end and owners[end - analyzer.base] == owner:
                end += 1
            _append_range(ranges, cursor, end, "function", owner)
            cursor = end
        else:
            end = cursor + 1
            is_shared = cursor in shared_data
            while end < text_end and owners[end - analyzer.base] is None and (end in shared_data) == is_shared:
                end += 1
            if is_shared:
                _append_range(ranges, cursor, end, "rodata", None)
                cursor = end
                continue
            # Only zero words / architectural NOPs adjacent to an explained boundary are padding.
            address = cursor
            while address < end:
                is_padding = (address % 4 == 0 and address + 4 <= end
                              and address not in analyzer.shared_literal_words
                              and analyzer.word(address) in (0, 0xE1A00000, 0xE320F000))
                stop = address + (4 if is_padding else 1)
                _append_range(ranges, address, stop, "padding" if is_padding else "unknown", None)
                address = stop
            cursor = end
    for segment in sorted(layout.segments, key=lambda item: item.address):
        if segment.name != "text":
            _append_range(ranges, segment.address, segment.address + segment.size, segment.name, None)
        if segment.size < segment.mapped_size:
            ranges.append({"start": segment.address + segment.size,
                           "end": segment.address + segment.mapped_size,
                           "kind": "segment_padding", "owner": None})
    ranges.sort(key=lambda item: item["start"])
    result = {
        "schema_version": 1, "image": Path(image_name).name,
        "image_sha256": hashlib.sha256(image).hexdigest(),
        "generator": {"tool": "tools/analyze.py", "capstone": capstone.__version__},
        "segments": [{"name": segment.name, "address": segment.address, "size": segment.size,
                      "mapped_size": segment.mapped_size} for segment in sorted(layout.segments, key=lambda item: item.address)],
        "functions": sorted(functions, key=lambda item: item["address"]), "ranges": ranges,
        "boundary_merges": analyzer.boundary_merges,
        "literal_conflicts": analyzer.literal_conflicts,
        "shared_literals": analyzer.shared_literals,
        "relative_pointer_tables": analyzer.relative_pointer_tables,
        "pc_relative_pointer_witnesses": [row for row in analyzer.formed_code_pointers
                                         if (row["target"], row["isa"]) in analyzer.pointer_target_repairs],
        "protected_entry_observations": [
            {"function": function, "site": site, "target": target, "reason": "protected_adopted_entry_metadata"}
            for function, site, target in sorted(analyzer.protected_entry_observations)],
        "protected_start_observations": [
            {"address": address, "evidence": evidence, "reason": "protected_adopted_start_metadata"}
            for address, evidence in sorted(analyzer.protected_start_observations)],
        "entry_groups": analyzer.entry_groups,
        "tail_target_repairs": [{"address": target, "isa": isa, "sources": sorted(sources)}
                                for (target, isa), sources in sorted(analyzer.tail_target_repairs.items())],
        "linker_tail_splits": [{"address": address, "target": address + 4, "isa": "arm"}
                               for address in sorted(analyzer.linker_wrappers)],
        "pointer_target_repairs": [{"address": address, "isa": isa}
                                   for address, isa in sorted(analyzer.pointer_target_repairs)],
        "leading_instruction_repairs": sorted(analyzer.leading_instruction_repairs, key=lambda row: row["old_address"]),
        "leading_instruction_inventory": sorted(analyzer.leading_instruction_inventory.values(), key=lambda row: row["address"]),
        "pointer_entry_inventory": [row for _, row in sorted(analyzer.pointer_entry_inventory.items())],
        "pointer_prefix_wrappers": [{"address": address, "target": target, "tail_nop": target - 4}
                                    for address, target in sorted(analyzer.pointer_prefix_wrappers.items())],
        "stats": {}, "anomalies": [],
    }
    anomalies.update((issue["address"], issue["issue"]) for issue in validate_catalogue(result, image, layout))
    result["anomalies"] = [{"address": address, "issue": issue} for address, issue in sorted(anomalies)]
    range_bytes: Counter[str] = Counter()
    for item in ranges:
        range_bytes[item["kind"]] += item["end"] - item["start"]
    result["stats"] = {
        "functions": len(functions),
        "functions_by_isa": dict(sorted(Counter(function["isa"] for function in functions).items())),
        "functions_by_kind": dict(sorted(Counter(function["kind"] for function in functions).items())),
        "starts_by_evidence": dict(sorted(Counter(evidence for function in functions for evidence in function["evidence"]).items())),
        "bytes_by_range_kind": dict(sorted(range_bytes.items())),
        "unknown_ranges": sum(item["kind"] == "unknown" for item in ranges),
        "anomalies": len(anomalies), "anomalies_by_issue": dict(sorted(Counter(issue for _, issue in anomalies).items())),
        "direct_calls": sum(len(trace.calls) for trace in traces),
        "tail_calls": sum(len(trace.tails) for trace in traces),
        "switch_tables": sum(trace.switches for trace in traces),
        "indirect_exits": sum(trace.indirect_exits for trace in traces),
        "decoded_instructions": sum(len(trace.code) for trace in traces),
        "code_bytes": sum(sum(trace.code.values()) for trace in traces),
        "pool_bytes": sum(len(trace.pools) * 4 for trace in traces),
        "prologue_only_starts": sum(function["evidence"] == ["prologue"] for function in functions),
        "discovery_rounds": analyzer.discovery_rounds, "rejected_raw_call_seeds": analyzer.rejected_call_seeds,
        "merged_starts": len(analyzer.boundary_merges),
        "merges_by_reason": dict(sorted(Counter(
            reason for merge in analyzer.boundary_merges for reason in merge["reasons"]).items())),
        "protected_pointer_table_starts": len(analyzer.protected_pointer_starts),
        "noreturn_functions": sum(function["noreturn"] for function in functions),
        "literal_bytes": sum(end - first for trace in traces for first, end in trace.literals),
        "functions_with_literals": sum(bool(trace.literals) for trace in traces),
        "literal_objects_by_kind": dict(sorted(sum((trace.literal_kinds for trace in traces), Counter()).items())),
        "literal_extent_changes": dict(sorted(analyzer.literal_extent_changes.items())),
        "literal_conflicts": len(analyzer.literal_conflicts),
        "fallthrough_groups": len(analyzer.entry_groups),
        "absorbed_function_starts": sum(len(trace.members) - 1 for trace in traces),
        "secondary_entries": sum(len(trace.entries) - 1 for trace in traces),
        "nop_entries": sum(entry["kind"] == "nop_entry" for trace in traces for entry in trace.entries.values()),
        "repaired_tail_targets": len(analyzer.tail_target_repairs),
        "linker_tail_wrappers": len(analyzer.linker_wrappers),
        "repaired_pointer_targets": len(analyzer.pointer_target_repairs),
    }
    result = _split_linker_tail_extents(result, analyzer, protected_functions or {})
    if closure is not None:
        catalogue_closure.close_catalogue(result, analyzer, closure, protected_functions or {})
        result["closure_evidence"]["protected_metadata_transitions"] = metadata_transitions
    deferred_entries = ({record["address"] for record in closure["runtime_data"].get("entry_fragments", [])}
                        if closure is not None and "runtime_data" in closure else set())
    preserve_prior_entries(result, [function for function in prior_functions or [] if function["address"] not in deferred_entries])
    apply_classifications(result, classifications or [])
    if closure is not None and "runtime_data" in closure:
        if __package__:
            from . import catalogue_runtime_data
        else:
            import catalogue_runtime_data
        catalogue_runtime_data.close_catalogue(result, analyzer, closure["runtime_data"], protected_functions or {})
        catalogue_runtime_data.close_entry_fragments(result, analyzer, closure["runtime_data"], protected_functions or {})
        apply_classifications(result, catalogue_runtime_data.classification_records(
            classifications or [], closure["runtime_data"]))
        catalogue_runtime_data.check_prepared_protection(result, closure["runtime_data"])
        prior_entries = {(entry["address"], entry["isa"]) for function in prior_functions or [] for entry in function["entries"]}
        current_entries = {(entry["address"], entry["isa"]) for function in result["functions"] for entry in function["entries"]}
        if prior_entries - current_entries:
            raise ValueError("runtime closure loses a deferred prior entry")
    if closure is not None:
        catalogue_closure.check_entry_revision(
            result, analyzer, closure, protected_functions, classifications or [])
    check_protected_functions(result, protected_functions or {})
    return result


def validate_catalogue(catalogue: dict, image: bytes, layout: ctrfmt.CodeLayout) -> list[dict]:
    """Recheck map, function, pool, decoding, call-target and page-padding invariants.

    Returns deterministic anomaly records rather than suppressing uncertain recovery.
    Structural layout errors (e.g. a wrong-sized input) raise ValueError.
    """
    analyzer = Analyzer(image, layout)
    issues: set[tuple[int, str]] = set()
    functions = catalogue["functions"]
    function_starts = [function["address"] for function in functions]
    entry_model = any("entries" in function for function in functions)
    entry_owners = {function["address"]: (function["address"], function["isa"]) for function in functions}
    for function in functions:
        previous_entry = function["address"] - 1
        for entry in function.get("entries", []):
            address, isa = entry["address"], entry["isa"]
            if address <= previous_entry:
                issues.add((address, "invalid_entry_order"))
            previous_entry = address
            if address in entry_owners and entry_owners[address][0] != function["address"]:
                issues.add((address, "overlapping_entry_ownership"))
            entry_owners[address] = (function["address"], isa)
            if address != function["address"] and function["kind"] not in ("multi_entry", "veneer"):
                issues.add((address, "invalid_secondary_entry_kind"))
    previous_end = layout.text.address
    for function in functions:
        start, end = function["address"], function["address"] + function["size"]
        if start < previous_end:
            issues.add((start, "overlapping_functions"))
        previous_end = max(previous_end, end)
        if not analyzer.inside(start, function["isa"]) or end > analyzer.end or function["size"] <= 0:
            issues.add((start, "invalid_function_extent"))
        if not 0 < function["code_size"] <= function["size"]:
            issues.add((start, "invalid_code_size"))
        pools = function["pools"]
        pool_end = start
        for first, stop in pools:
            if first % 4 or stop % 4 or not pool_end <= first < stop <= end:
                issues.add((first, "invalid_pool_range"))
            pool_end = stop
        literals = function.get("literals", [])
        literal_end = start
        for first, stop in literals:
            if stop % 4 or not literal_end <= first < stop <= end:
                issues.add((first, "invalid_literal_range"))
            if any(first < pool_stop and pool_start < stop for pool_start, pool_stop in pools):
                issues.add((first, "literal_overlaps_pool"))
            literal_end = stop
        code_end = start
        decoded_entries: dict[int, str] = {}
        for first, stop in function.get("code_ranges", [[start, start + function["code_size"]]]):
            if not code_end <= first < stop <= start + function["code_size"]:
                issues.add((first, "invalid_code_range"))
            code_end = stop
            if any(first < pool_stop and pool_start < stop for pool_start, pool_stop in pools):
                issues.add((first, "pool_overlaps_code"))
            if any(first < literal_stop and literal_start < stop for literal_start, literal_stop in literals):
                issues.add((first, "literal_overlaps_code"))
            address = first
            while address < stop:
                isa = function["isa"]
                if function["kind"] == "veneer" and isa == "thumb" and address == start + 4:
                    isa = "arm"  # The linker-defined interworking veneer has one ARM instruction.
                ins = analyzer.decode(address, isa)
                if ins is None or address + ins.size > stop:
                    issues.add((address, "code_does_not_decode"))
                    break
                decoded_entries[address] = isa
                if ins.call and ins.target is not None:
                    target, target_isa = ins.target
                    if target not in entry_owners or entry_owners[target][1] != target_isa:
                        issues.add((address, "call_target_not_function_start"))
                if ins.branch and ins.target is not None:
                    target, target_isa = ins.target
                    if analyzer.inside(target, target_isa):
                        index = bisect_right(function_starts, target) - 1
                        if index < 0 or target >= functions[index]["address"] + functions[index]["size"]:
                            issues.add((address, "branch_target_outside_function_extents"))
                        elif entry_model and functions[index]["address"] != start and (
                                target not in entry_owners or entry_owners[target][1] != target_isa):
                            issues.add((address, "branch_target_not_entry"))
                address += ins.size
        if code_end != start + function["code_size"]:
            issues.add((start, "invalid_code_size"))
        if max(code_end, pool_end, literal_end) != end:
            issues.add((start, "invalid_function_extent"))
        for entry in function.get("entries", []):
            address, isa = entry["address"], entry["isa"]
            if (not start <= address < end or not analyzer.inside(address, isa)
                    or decoded_entries.get(address) != isa):
                issues.add((address, "invalid_entry_point"))
    ranges = catalogue["ranges"]
    expected = [(segment.address, segment.address + segment.mapped_size) for segment in sorted(layout.segments, key=lambda item: item.address) if segment.mapped_size]
    index = 0
    for first, end in expected:
        cursor = first
        while index < len(ranges) and ranges[index]["start"] < end:
            item = ranges[index]
            if item["start"] != cursor or not cursor < item["end"] <= end:
                issues.add((cursor, "ranges_do_not_tile"))
            cursor = item["end"]
            index += 1
        if cursor != end:
            issues.add((cursor, "ranges_do_not_tile"))
    if index != len(ranges):
        issues.add((ranges[index]["start"], "range_outside_segments"))
    for segment in layout.segments:
        if segment.mapped_size:
            offset = layout.file_offset(segment.address)
            if any(image[offset + segment.size:offset + segment.mapped_size]):
                issues.add((segment.address + segment.size, "nonzero_segment_padding"))
    return [{"address": address, "issue": issue} for address, issue in sorted(issues)]


def serialize_catalogue(catalogue: dict) -> bytes:
    return (json.dumps(catalogue, indent=2, sort_keys=True, ensure_ascii=False) + "\n").encode("utf-8")


def load_protected_functions(root: Path, baseline: dict, manifests: list[Path], *,
                             git_blobs: list[dict] | None = None) -> tuple[dict, dict, dict]:
    """Resolve whole adopted records; remote checkpoint inputs are immutable blobs."""
    adopted = sorted(baseline["functions"], key=lambda function: function["address"])
    starts = [function["address"] for function in adopted]
    names_path = root / "config/names.json"
    names = json.loads(names_path.read_text(encoding="utf-8"))["symbols"] if names_path.is_file() else {}
    inputs = {"config/names.json": hashlib.sha256(names_path.read_bytes()).hexdigest()} if names_path.is_file() else {}
    sources = {}
    paths = [root / "config/matches.json"]
    paths.extend(path for name in ("c004", "c005", "c006", "c007", "c008")
                 if (path := root / f"config/candidates/{name}.json").is_file())
    paths.extend(manifests)
    for path in paths:
        label = str(path.relative_to(root)) if path.is_relative_to(root) else str(path)
        sources[label] = path.read_bytes()
    for row in git_blobs or []:
        blob = row["blob"]
        if len(blob) != 40 or any(character not in "0123456789abcdef" for character in blob):
            raise ValueError("protected checkpoint needs an immutable Git blob")
        content = subprocess.run(["git", "show", blob], cwd=root, capture_output=True, check=False)
        if content.returncode:
            raise ValueError(f"cannot read protection blob (git exit {content.returncode})")
        if hashlib.sha256(content.stdout).hexdigest() != row["sha256"]:
            raise ValueError("protected checkpoint blob digest does not match reviewed evidence")
        sources[f"git:{row['reference']}@{blob}"] = content.stdout
    protected, summaries = {}, {}
    for label, content in sources.items():
        rows = []
        for unit in json.loads(content)["units"]:
            for name in unit["functions"]:
                address = (names[name]["address"] if name in names else int(name.removeprefix("func_"), 16)) & ~1
                index = bisect_right(starts, address) - 1
                if index < 0 or address >= adopted[index]["address"] + adopted[index]["size"]:
                    raise ValueError(f"protected function {name} has no adopted catalogue record")
                protected[address] = adopted[index]
                rows.append((address, adopted[index]["size"]))
        inputs[label] = hashlib.sha256(content).hexdigest()
        summaries[label] = {"functions": len(rows), "bytes": sum(size for _, size in rows), "changes": []}
    return protected, summaries, inputs


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)
    command = subparsers.add_parser("catalogue", help="recover conservative ARM/Thumb function boundaries")
    command.add_argument("--image", type=Path, default=ROOT / "orig/code.bin")
    command.add_argument("--exheader", type=Path, default=ROOT / "orig/exheader.bin")
    command.add_argument("--out", type=Path, default=ROOT / "scratch/analyze/catalogue.json")
    command.add_argument("--protect-manifest", type=Path, action="append", default=[],
                         help="also protect this manifest; accepted/candidate checkpoints are automatic")
    command.add_argument("--classifications", type=Path,
                         help="override bundled image-bound reviewed classification evidence")
    args = parser.parse_args(argv)
    started = time.perf_counter()
    try:
        if __package__:
            from . import catalogue_review
        else:
            import catalogue_review
        baseline_path = ROOT / "config/catalogue.json"
        baseline = catalogue_review.load(baseline_path)
        image, exheader = args.image.read_bytes(), args.exheader.read_bytes()
        layout = ctrfmt.parse_exheader(exheader)
        evidence_path = ROOT / "research/catalogue_v9_r2_evidence.json"
        evidence = catalogue_review.load(evidence_path)
        bundled = args.classifications is None and hashlib.sha256(image).hexdigest() == evidence["image_sha256"]
        records = (catalogue_review.load(args.classifications) if args.classifications
                   else evidence["classification_records"] if bundled else [])
        withheld = []
        inputs = {}
        manifests = list(args.protect_manifest)
        if bundled:
            if __package__:
                from . import catalogue_closure
            else:
                import catalogue_closure
            closure_evidence, prior_proposal, closure_inputs = catalogue_closure.load_inputs(ROOT, evidence_path)
            if closure_evidence["image_sha256"] != evidence["image_sha256"]:
                raise ValueError("closure evidence image differs from its parent")
            inputs.update(closure_inputs)
            snapshot = closure_evidence["runtime_data"].get("_prepared_protection")
            if snapshot is not None:
                manifests.append(ROOT / snapshot["ledger"]["path"])
            records = records + closure_evidence.get("classification_records", [])
            evidence = {**evidence, "revision": closure_evidence["revision"],
                        "literal_encodings": evidence.get("literal_encodings", []) + closure_evidence["literal_encodings"],
                        "provenance": {**evidence["provenance"], "catalogue_v10": closure_evidence["provenance"],
                                       "catalogue_v10_r3": closure_evidence["library_provenance"],
                                       "catalogue_v10_r4": closure_evidence["library_extension_provenance"],
                                       "catalogue_v10_r5": closure_evidence["endian_repair"]["provenance"],
                                       "catalogue_v11": closure_evidence["runtime_data"]["qualification"]}}
        protected, protected_summary, protected_inputs = load_protected_functions(
            ROOT, baseline, manifests, git_blobs=evidence.get("protected_git_blobs", []))
        inputs.update(protected_inputs)
        if bundled:
            protected, _ = catalogue_closure.resolve_protected_records(
                protected, Analyzer(image, layout), closure_evidence, evidence["literal_encodings"])
            # A reviewed reclassification cannot silently outrank an accepted or
            # in-flight complete record. Explicit classification inputs instead
            # reach build_catalogue's final invariant and fail on any conflict.
            protected_owners = {function["address"] for function in protected.values()}
            withheld = [{**row, "reason": "protected_adopted_record"}
                        for row in records if row["address"] in protected_owners]
            records = [row for row in records if row["address"] not in protected_owners]
        result = build_catalogue(
            image, layout, args.image.name, protected_functions=protected, classifications=records,
            literal_encodings=evidence.get("literal_encodings", []) if bundled else [],
            relative_pointer_tables=evidence.get("relative_pointer_tables", []) if bundled else [],
            prior_functions=baseline["functions"] if bundled else [], closure=closure_evidence if bundled else None)
        inputs.update({
            "config/catalogue.json": catalogue_review.digest(baseline_path),
            "exheader": hashlib.sha256(exheader).hexdigest(),
            "image": result["image_sha256"],
        })
        classification_input = args.classifications or evidence_path
        classification_label = (str(classification_input.relative_to(ROOT))
                                if classification_input.is_relative_to(ROOT) else str(classification_input))
        inputs[classification_label] = catalogue_review.digest(classification_input)
        source_root = Path(__file__).resolve().parent
        sources = {f"tools/{name}": catalogue_review.digest(source_root / name)
                   for name in ("analyze.py", "catalogue_review.py", "catalogue_closure.py", "catalogue_runtime_data.py")}
        result["generator"] = {
            "command": "tools/analyze.py catalogue --out X",
            "revision": "catalogue-v11-r3",
            "source_sha256": sources,
            "classification_input": classification_label,
            "bundled_review_applied": bundled,
            "input_sha256": dict(sorted(inputs.items())),
        }
        if bundled:
            catalogue_review.enrich_catalogue(
                result, baseline, image, layout, protected=protected,
                protected_summary=protected_summary, evidence=evidence,
                input_sha256={**inputs, **sources}, withheld=withheld)
            catalogue_closure.enrich_review(result, prior_proposal, image, layout, closure_evidence)
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_bytes(serialize_catalogue(result))
    except (OSError, ValueError, struct.error) as error:
        print(f"catalogue: {error}", file=sys.stderr)
        return 1
    stats = result["stats"]
    print(f"{stats['functions']} functions; {stats['bytes_by_range_kind'].get('unknown', 0)} unknown bytes; "
          f"{stats['anomalies']} anomalies; {time.perf_counter() - started:.3f}s; {args.out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
