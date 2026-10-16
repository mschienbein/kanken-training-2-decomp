"""Conservative ARM11 function and literal-pool catalogue (no ROM contents emitted).

Run ``.venv/bin/python tools/analyze.py catalogue``. The output is a candidate,
not a frozen matching denominator. Discovery combines executable call edges,
external pointers, linker veneers and prologues in otherwise unexplained gaps.
``code_ranges`` makes sparse recursive-descent coverage explicit; the enclosing
``size`` includes trailing pools, but unexplained holes remain unknown ranges.
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

    @property
    def end(self) -> int:
        return max([self.address] + [a + n for a, n in self.code.items()] + [a + 4 for a in self.pools])


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
        elif decoded.id in (arm.ARM_INS_LDR, arm.ARM_INS_VLDR):
            for op in ops:
                if op.type == arm.ARM_OP_MEM and op.mem.base == arm.ARM_REG_PC and not op.mem.index:
                    literal = ((address + 4) & ~3) + op.mem.disp
                    width = 8 if ops[0].type == arm.ARM_OP_REG and arm.ARM_REG_D0 <= ops[0].reg <= arm.ARM_REG_D31 else 4
                    ins.pools = ((literal, literal + width),)
            if ops and ops[0].reg == arm.ARM_REG_PC:
                ins.stop = ins.indirect = True
        elif decoded.id == arm.ARM_INS_ADR and len(ops) == 2:
            literal = ((address + 4) & ~3) + ops[1].imm
            ins.pools = ((literal & ~3, (literal + 7) & ~3),)
            ins.address_reference = True
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

    def trace(self, start: int, limit: int) -> Trace:
        isa = self.seeds[start].isa
        result = Trace(start, isa)
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
        work = [start]
        address_words: set[int] = set()
        while work:
            address = work.pop()
            while start <= address < limit:
                if address in result.code or address & ~3 in result.pools:
                    break
                ins = self.decode(address, isa)
                if ins is None or address + ins.size > limit:
                    result.issues.add((address, "undecodable_reachable_code"))
                    break
                result.code[address] = ins.size
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
                    else:
                        result.switches += 1
                        if style == "branches":
                            for entry in range(table, table + count * 4, 4):
                                if self.word(entry) & 0xFF000000 != 0xEA000000:
                                    result.issues.add((entry, "invalid_switch_branch"))
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
                        not ins.conditional and (address == start or restored_lr or padding_gap)
                    )
                    if tail:
                        result.tails.add((target, target_isa, address))
                    else:
                        work.append(target)
                    if not ins.conditional:
                        break
                elif ins.stop:
                    if ins.target is not None:
                        result.tails.add((*ins.target, address))
                    result.indirect_exits += ins.indirect
                    if not ins.conditional:
                        break
                address += ins.size
                if address == limit and limit < self.end:
                    result.issues.add((address, "fallthrough_into_start"))
        occupied = {address & ~3 for address in result.code}
        result.pools.update(address_words - occupied)
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
    traces = analyzer.discover()
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
            "isa": trace.isa, "kind": trace.kind, "pools": pools,
            "code_ranges": code_ranges, "evidence": sorted(analyzer.seeds[trace.address].evidence),
        })
        anomalies.update(trace.issues)
        for first, end in code_ranges + pools:
            for address in range(first, end):
                offset = address - analyzer.base
                if owners[offset] is not None and owners[offset] != trace.address:
                    anomalies.add((address, "overlapping_function_ownership"))
                owners[offset] = trace.address
        for first, end in code_ranges:
            if any(first < pool_end and pool_start < end for pool_start, pool_end in pools):
                anomalies.add((first, "pool_overlaps_code"))
        explained = _merge([tuple(interval) for interval in code_ranges + pools])
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
                is_padding = address % 4 == 0 and address + 4 <= end and analyzer.word(address) in (0, 0xE1A00000, 0xE320F000)
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
    starts = {function["address"]: function for function in functions}
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
        code_end = start
        for first, stop in function.get("code_ranges", [[start, start + function["code_size"]]]):
            if not code_end <= first < stop <= start + function["code_size"]:
                issues.add((first, "invalid_code_range"))
            code_end = stop
            if any(first < pool_stop and pool_start < stop for pool_start, pool_stop in pools):
                issues.add((first, "pool_overlaps_code"))
            address = first
            while address < stop:
                isa = function["isa"]
                if function["kind"] == "veneer" and isa == "thumb" and address == start + 4:
                    isa = "arm"  # The linker-defined interworking veneer has one ARM instruction.
                ins = analyzer.decode(address, isa)
                if ins is None or address + ins.size > stop:
                    issues.add((address, "code_does_not_decode"))
                    break
                if ins.call and ins.target is not None:
                    target, target_isa = ins.target
                    if target not in starts or starts[target]["isa"] != target_isa:
                        issues.add((address, "call_target_not_function_start"))
                address += ins.size
        if code_end != start + function["code_size"]:
            issues.add((start, "invalid_code_size"))
        if max(code_end, pool_end) != end:
            issues.add((start, "invalid_function_extent"))
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
