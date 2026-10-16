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
ARM ``mov r0, r0`` entry prefixes are separate tail-call wrappers under
``armlink5-inline-branchnop-v1`` (docs/LINKER.md), not generic NOP padding.
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
import sys
import time

import capstone
from capstone import arm

if __package__:
    from . import ctrfmt
else:
    import ctrfmt

ROOT = Path(__file__).resolve().parents[1]


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
        self.literal_extent_changes: Counter[str] = Counter()
        self.shared_literal_words: set[int] = set()
        self.entry_groups: list[dict] = []
        self.tail_target_repairs: dict[tuple[int, str], set[int]] = {}
        self.linker_wrappers: set[int] = set()

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
                    if (decoded.id == arm.ARM_INS_LDR and width == 4 and ops[0].type == arm.ARM_OP_REG
                            and literal % 4 == 0 and self.base <= literal <= self.end - 4):
                        ins.register_constant = (ops[0].reg, self.word(literal))
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

    def pointer_plausible(self, address: int, isa: str, *, prologue_sufficient: bool = True) -> bool:
        """A pointer-sized integer alone is not a function-entry witness."""
        if not self.plausible(address, isa):
            return False
        if prologue_sufficient and (self.prologue(address, isa) or self.veneer(address, isa) is not None):
            return True
        cursor = address
        for _ in range(32):
            ins = self.decode(cursor, isa)
            if ins is None:
                return False
            if ins.returns:
                return True
            if ins.branch and not ins.conditional:
                return ins.target is not None and ins.target[0] in self.seeds
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
                result.pools.add(start + 4)
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
        pointers = set(self._external_pointers())
        for trace in traces:
            for address in trace.pools - trace.table_words:
                pointer = self.word(address)
                target, isa = pointer & ~1, "thumb" if pointer & 1 else "arm"
                if self.inside(target, isa):
                    pointers.add((target, isa))
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

    def record_entries(self, traces: list[Trace]) -> None:
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

    def include_address_literals(self, traces: list[Trace]) -> None:
        """Claim exclusive address-taken objects without changing discovered starts.

        NUL-terminated text has a complete bound. Other data retains only the
        addressed word span already evidenced by v2, not a guessed array length.
        References into string suffixes and external pointer tables count as shared.
        """
        self.literal_conflicts = []
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
                            and previous is not None and previous[0] + previous[1].size == address):
                        constant = previous[1].register_constant
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
                content_end = self._string_end(target)
                first = target if content_end is not None else target & ~3
                end = (content_end + 3) & ~3 if content_end is not None else (target + 7) & ~3
                candidates.append({
                    "start": first, "end": end, "target": target, "function": trace.address,
                    "kind": "string" if content_end is not None else "addressed_words",
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
            if shared:
                blocked_words.update(range(first & ~3, (end + 3) & ~3, 4))
            for candidate in cluster:
                trace = by_start[candidate["function"]]
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


def build_catalogue(image: bytes, layout: ctrfmt.CodeLayout, image_name: str = "code.bin") -> dict:
    """Return deterministic schema-v1 metadata, preserving ambiguity as anomalies/unknown."""
    analyzer = Analyzer(image, layout)
    analyzer.discover()
    for _ in range(64):
        traces = analyzer.group_fallthroughs(analyzer.refine())
        analyzer.include_address_literals(traces)
        if analyzer.promote_missing_targets(traces):
            continue
        analyzer.record_entries(traces)
        if not analyzer.promote_linker_entries(traces):
            break
    else:
        raise ValueError("entry-target discovery did not converge after 64 rounds")
    anomalies: set[tuple[int, str]] = set()
    functions = []
    owners: list[int | None] = [None] * layout.text.size
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
            while end < text_end and owners[end - analyzer.base] is None:
                end += 1
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
        "entry_groups": analyzer.entry_groups,
        "tail_target_repairs": [{"address": target, "isa": isa, "sources": sorted(sources)}
                                for (target, isa), sources in sorted(analyzer.tail_target_repairs.items())],
        "linker_tail_splits": [{"address": address, "target": address + 4, "isa": "arm"}
                               for address in sorted(analyzer.linker_wrappers)],
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
    }
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


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)
    command = subparsers.add_parser("catalogue", help="recover conservative ARM/Thumb function boundaries")
    command.add_argument("--image", type=Path, default=ROOT / "orig/code.bin")
    command.add_argument("--exheader", type=Path, default=ROOT / "orig/exheader.bin")
    command.add_argument("--out", type=Path, default=ROOT / "scratch/analyze/catalogue.json")
    args = parser.parse_args(argv)
    started = time.perf_counter()
    try:
        result = build_catalogue(args.image.read_bytes(), ctrfmt.parse_exheader(args.exheader.read_bytes()), args.image.name)
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
