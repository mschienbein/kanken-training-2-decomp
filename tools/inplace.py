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
callee's instruction (or a NOP), decided from the ORIGINAL function at the call's destination,
proves only that the original callee's body is that instruction, and a B to the next instruction
written as a NOP proves only that the target starts there. Every such rewrite is reported
(`InPlaceLinker.rewrites`) and the model name is recorded by the callers.

An opt-in, non-default model also permits a complete anonymous SHF_MERGE|SHF_STRINGS section
to be permuted by whole-record identity (rule anonymous-string-records-v1). It never searches
for data or changes instructions: every record needs an original relocation witness, the same
record bytes, and a bijective placement inside one equal-sized, completely verified pool.
"""
from __future__ import annotations

import re
import struct
from dataclasses import dataclass, field
from pathlib import Path

from capstone import CS_ARCH_ARM, CS_GRP_BRANCH_RELATIVE, CS_GRP_CALL, CS_GRP_INT, CS_GRP_IRET, CS_GRP_JUMP, CS_GRP_RET, CS_MODE_ARM, Cs
from capstone import arm as cs_arm
from elftools.elf.constants import SH_FLAGS
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

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


@dataclass(frozen=True)
class LinkerModel:
    """The armlink output optimisations an in-place link reproduces.

    The name is the version: a rule change is a new model, never an edited one, so the model name
    recorded in match results and build receipts says exactly which rules credited a function.
    """

    name: str
    inline: bool = False  # --inline: ARM BL to a 4/8-byte ARM function -> its instruction, or a NOP
    branchnop: bool = False  # --branchnop: relocated ARM B to the next instruction -> NOP
    string_records: bool = False  # anonymous-string-records-v1: complete, witnessed literal permutation


LINKER_MODELS = {m.name: m for m in (
    LinkerModel("strict-v1"),
    LinkerModel("armlink5-inline-branchnop-v2", inline=True, branchnop=True),
    LinkerModel("inplace-literal-records-v1", inline=True, branchnop=True, string_records=True),
)}
DEFAULT_LINKER_MODEL = "armlink5-inline-branchnop-v2"
# Names recorded by earlier artifacts whose rules are withdrawn: never run again (docs/LINKER.md).
RETIRED_LINKER_MODELS = {
    "armlink5-inline-branchnop-v1": "judged a callee defined in the candidate object by the candidate's own bytes, "
    "not the original's (review F01); superseded by armlink5-inline-branchnop-v2",
}


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
    target: int  # the branch destination: the inlined callee, or the next instruction
    word: int  # what was written in place of the branch

    def record(self) -> dict:
        return {"offset": self.offset, "rule": self.rule, "target": self.target, "word": f"{self.word:#010x}"}


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
    data: bytes
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


class ObjectFile:
    """The allocatable sections, symbols and REL relocations of one armcc object."""

    def __init__(self, path: Path):
        with path.open("rb") as f:
            elf = ELFFile(f)
            if elf["e_machine"] != "EM_ARM" or elf["e_type"] != "ET_REL":
                raise LinkError(f"{path}: not an ARM relocatable object")
            self.sections: dict[int, Section] = {}
            for i, s in enumerate(elf.iter_sections()):
                if s["sh_flags"] & SH_FLAGS.SHF_ALLOC and s["sh_size"]:
                    nobits = s["sh_type"] == "SHT_NOBITS"
                    self.sections[i] = Section(
                        i, s.name, b"" if nobits else s.data(), s["sh_size"], nobits,
                        bool(s["sh_flags"] & SH_FLAGS.SHF_EXECINSTR), bool(s["sh_flags"] & SH_FLAGS.SHF_WRITE),
                        align=max(1, s["sh_addralign"]),
                        merge_strings=(s["sh_flags"] & (SH_FLAGS.SHF_MERGE | SH_FLAGS.SHF_STRINGS)
                                       == SH_FLAGS.SHF_MERGE | SH_FLAGS.SHF_STRINGS),
                        entsize=s["sh_entsize"],
                    )
            symtab = elf.get_section_by_name(".symtab")
            self.symbols = [
                Symbol(sym.name, sym["st_value"], sym["st_size"], sym["st_info"]["type"], sym["st_info"]["bind"], sym["st_shndx"])
                for sym in symtab.iter_symbols()
            ]
            for s in elf.iter_sections():
                if s["sh_type"] == "SHT_GROUP":
                    words = struct.unpack(f"<{s['sh_size'] // 4}I", s.data())
                    if words and words[0] & 1:  # GRP_COMDAT: one entity, deduplicated by the linker
                        signature = self.symbols[s["sh_info"]].name
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
    literal_data: bytes | None = None  # reordered candidate bytes; never copied from the image


def _is_internal_label(sym: Symbol) -> bool:
    """ARMCC's `__switch$$` marks a jump table's base inside its function's own section: a local,
    zero-size STT_FUNC label that table entries relocate against, not a separate function."""
    return sym.name == "__switch$$" and sym.bind == "STB_LOCAL" and sym.size == 0


def _is_thumb_function(sym: Symbol) -> bool:
    return sym.kind == "STT_FUNC" and bool(sym.value & 1)


def _symbol_offset(sym: Symbol) -> int:
    """Offset inside its section; only function symbols carry the Thumb bit in their value."""
    return sym.value & ~1 if sym.kind == "STT_FUNC" else sym.value


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
                    f"COMDAT {comdat[0]} {comdat[1]} placed at [{first[0]:#x}, {first[1]:#x}) by {first[2]} "
                    f"and at [{lo:#x}, {hi:#x}) by {label}"
                )
        merged.setdefault(("comdat", comdat) if comdat else ("span", i), (lo, hi, label))
    ordered = sorted(merged.values())
    for (lo_a, hi_a, name_a), (lo_b, hi_b, name_b) in zip(ordered, ordered[1:]):
        if lo_b < hi_a:
            raise LinkError(f"{name_a} [{lo_a:#x}, {hi_a:#x}) and {name_b} [{lo_b:#x}, {hi_b:#x}) overlap")


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
                 model: LinkerModel | str = DEFAULT_LINKER_MODEL):
        """image: object with read(address, size) -> bytes; functions: catalogue functions by address;
        bss: [start, end) of zero-initialised memory; model: the armlink output optimisations to
        reproduce (a LinkerModel or a LINKER_MODELS name)."""
        self.image = image
        self.functions = functions
        self.resolver = resolver
        self.bss = bss
        self.model = linker_model(model) if isinstance(model, str) else model
        self.rewrites: list[Rewrite] = []
        self.literal_rebindings: list[dict] = []

    # -- placement -----------------------------------------------------------------------------

    def place(self, obj: ObjectFile, names_to_address: dict[str, int]) -> dict[int, Placement]:
        placements: dict[int, Placement] = {}
        offsets: dict[int, set[int]] = {}
        for fn in obj.defined_functions():
            address = names_to_address.get(fn.name)
            if address is None:
                address = self._address_for(fn.name)
            sec = obj.sections[fn.shndx]
            offsets.setdefault(sec.index, set()).add(fn.value & ~1)
            if len(offsets[sec.index]) > 1:
                # Only aliases (e.g. C++ C1/C2 constructors at one offset) may share a section;
                # several distinct functions in one section need --split_sections.
                raise LinkError(f"{sec.name}: holds several functions at different offsets; compile with --split_sections")
            base = address - (fn.value & ~1)
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
                raise LinkError(f"{sym.name}: bound to {bound.address:#x}, but its section is placed at {prior.base:#x}")
        fixed = set(placements)  # addresses decided by the catalogue or by bound names
        literal_sections = {}
        if self.model.string_records:
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
                            f"({prior.base:#x} vs {base:#x})"
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
            placed = placements.get(sym.shndx)
            bound = self.resolver.lookup(sym.name)
            if bound is None:
                raise LinkError(
                    f"global {sym.name!r} has no address binding: name it func_/data_/bss_XXXXXXXX, "
                    f"add it to config/names.json, or make it static"
                )
            if placed is None:
                continue
            here = placed.base + _symbol_offset(sym)
            if here != bound.address:
                raise LinkError(f"{sym.name}: defined at {here:#x}, but the name is bound to {bound.address:#x}")
        for p in placements.values():
            if p.base % p.section.align:
                raise LinkError(f"{p.section.name}: placed at {p.base:#x}, not aligned to {p.section.align}")
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
        try:
            original = self.image.read(base, sec.size)
        except ValueError:
            raise LinkError(f"{sec.name}: witnessed string pool is outside the image") from None
        if original != reordered:
            raise LinkError(f"{sec.name}: complete witnessed string pool differs from the candidate records")
        self.literal_rebindings.append({
            "rule": "anonymous-string-records-v1", "section": sec.name, "base": base,
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
        if rel.type in (R_ARM_ABS32, R_ARM_TARGET1):
            return struct.unpack("<I", self.image.read(place, 4))[0]
        if rel.type == R_ARM_REL32:
            return (struct.unpack("<I", self.image.read(place, 4))[0] + place) & 0xFFFFFFFF
        if rel.type in (R_ARM_CALL, R_ARM_JUMP24, R_ARM_PC24):
            # branch target = P + 8 + (S + A - P)
            return _arm_branch_target(struct.unpack("<I", self.image.read(place, 4))[0], place) - 8
        if rel.type == R_ARM_THM_CALL:
            # branch target = P + 4 + (S + A - P)
            hi, lo = struct.unpack("<HH", self.image.read(place, 4))
            return _thm_bl_target(hi, lo, place) - 4
        if rel.type == R_ARM_THM_JUMP11:
            insn = struct.unpack("<H", self.image.read(place, 2))[0]
            return place + 4 + (_sx(insn & 0x7FF, 11) << 1) - 4
        if rel.type == R_ARM_THM_JUMP8:
            insn = struct.unpack("<H", self.image.read(place, 2))[0]
            return place + 4 + (_sx(insn & 0xFF, 8) << 1) - 4
        if rel.type == R_ARM_PREL31:
            word = struct.unpack("<I", self.image.read(place, 4))[0]
            return (_sx(word & 0x7FFFFFFF, 31) + place) & 0xFFFFFFFF
        raise LinkError(f"{sec.name}+{rel.offset:#x}: unsupported relocation type {rel.type}")

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
                raise LinkError(f"{sec.name}+{o:#x}: PREL31 to {s:#x} overflows 31 bits")
            word = struct.unpack_from("<I", data, o)[0]
            struct.pack_into("<I", data, o, (word & 0x80000000) | (value & 0x7FFFFFFF))
        elif t in (R_ARM_CALL, R_ARM_JUMP24, R_ARM_PC24):
            insn = struct.unpack_from("<I", data, o)[0]
            off = s + a - place
            # Direct ARM->Thumb interworking (BL -> BLX) is only defined for R_ARM_CALL;
            # JUMP24/PC24 to Thumb needs a linker veneer, which this oracle never invents.
            is_bl = t == R_ARM_CALL and insn >> 24 in (0xEB, 0xFA, 0xFB)
            if not -(1 << 25) <= off < (1 << 25):
                raise LinkError(f"{sec.name}+{o:#x}: branch to {s:#x} out of range (needs a veneer)")
            if target.thumb:
                if not is_bl:
                    raise LinkError(f"{sec.name}+{o:#x}: B/BLcond to Thumb {s:#x} needs an interworking veneer")
                insn = 0xFA000000 | (((off >> 1) & 1) << 24) | ((off >> 2) & 0xFFFFFF)
            else:
                if off & 3:
                    raise LinkError(f"{sec.name}+{o:#x}: misaligned ARM branch target {s:#x}")
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
                raise LinkError(f"{sec.name}+{o:#x}: Thumb BL to {s:#x} out of range")
            imm = (off >> 1) & 0x3FFFFF
            if lo == 0xE800 and imm & 1:
                raise LinkError(f"{sec.name}+{o:#x}: misaligned BLX target {s:#x}")
            struct.pack_into("<HH", data, o, 0xF000 | (imm >> 11), lo | (imm & 0x7FF))
        elif t == R_ARM_THM_JUMP11:
            off = s + a - place
            if not target.thumb or not -(1 << 11) <= off < (1 << 11):
                raise LinkError(f"{sec.name}+{o:#x}: Thumb B to {s:#x} unreachable")
            struct.pack_into("<H", data, o, 0xE000 | ((off >> 1) & 0x7FF))
        elif t == R_ARM_THM_JUMP8:
            off = s + a - place
            insn = struct.unpack_from("<H", data, o)[0]
            if not target.thumb or not -(1 << 8) <= off < (1 << 8):
                raise LinkError(f"{sec.name}+{o:#x}: Thumb Bcond to {s:#x} unreachable")
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
        refused — otherwise a wrong local helper could decide the caller's bytes. When neither
        would be inlined the call is an ordinary BL and nothing is decided.
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
                    f"{sec.name}+{rel.offset:#x}: call to {sym.name or obj.sections[sym.shndx].name} at {dest:#x}: "
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
        return self.image.read(dest, fn["size"])

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

    # -- link + verify -------------------------------------------------------------------------

    def link(self, obj: ObjectFile, names_to_address: dict[str, int] | None = None) -> dict[int, tuple[Placement, bytes]]:
        """Place and relocate every referenced section; returns {section index: (placement, bytes)}.

        Inferred (non-function) sections are verified here: their relocated bytes must equal the
        original image, BSS must lie in the BSS range. Function sections are returned unverified so
        the caller can report per-function differences. Branches the linker model rewrote instead of
        relocating are listed in `self.rewrites` (reset by each call).
        """
        self.literal_rebindings = []
        placements = self.place(obj, names_to_address or {})
        self.rewrites = []
        linked: dict[int, tuple[Placement, bytes]] = {}
        for idx, placed in placements.items():
            sec = placed.section
            if sec.nobits:
                lo, hi = self.bss
                if not (lo <= placed.base and placed.base + sec.size <= hi):
                    raise LinkError(f"{sec.name}: placed at {placed.base:#x}, outside BSS [{lo:#x}, {hi:#x})")
                linked[idx] = (placed, b"")
                continue
            data = bytearray(placed.literal_data if placed.literal_data is not None else sec.data)
            for rel in sec.relocs:
                if rel.type == R_ARM_V4BX:
                    continue  # marker; its symbol may be null and is never resolved
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
            linked[idx] = (placed, bytes(data))
            if placed.origin == "inferred":
                try:
                    original = self.image.read(placed.base, sec.size)
                except ValueError:
                    # The original word where the candidate holds this reference is not an address
                    # inside the image (e.g. an instruction): the candidate cannot match there.
                    raise LinkError(
                        f"{sec.name}: its references imply an address {placed.base:#x} outside the image; "
                        "the original holds no reference there"
                    ) from None
                if original != bytes(data):
                    first = next(i for i in range(sec.size) if original[i] != data[i])
                    raise LinkError(
                        f"{sec.name}: placed at {placed.base:#x} from its references, but its bytes differ from "
                        f"the original at +{first:#x}"
                    )
        return linked
