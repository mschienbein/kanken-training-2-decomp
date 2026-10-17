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


def _arm_call_flow(insn) -> str:
    """Classify only control flow whose successors the ARM-only R1 filter can prove."""
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


def arm_call_abi_context(instructions: dict, site: int, arguments: set[int], condition: int,
                         entries: set[int] | None = None, call_sites: set[int] | None = None) -> str:
    """Conservative whole-caller CFG filter, not evidence of a historical helper's identity.

    An input is defined in the preceding four straight-line words on every incoming path,
    is an unmodified incoming argument, or is an unmodified r0/r1 result of an unconditional
    preceding call on that path. Conditional definitions/calls alone do not establish inputs.
    Both conditional successors are checked; unresolved indirect transfers fail closed.
    Afterwards, follow every successor until a call/return or unconditional definitions of all
    caller-saved registers/flags. r0 may be a result. Candidate removed calls are call barriers.
    """
    if site not in instructions:
        return "arguments_not_materialised"
    entries = set(entries) if entries is not None else {min(instructions)}
    call_sites = call_sites or set()
    flows = {address: _arm_call_flow(insn) for address, insn in instructions.items()}
    successors = {address: _arm_successors(instructions, insn, flows[address])
                  for address, insn in instructions.items()}
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


class ObjectFile:
    """The allocatable sections, symbols and REL relocations of one armcc object."""

    def __init__(self, path: Path):
        self.path = path
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
            for index, section in obj.sections.items():
                new_index = indices[index]
                merged.sections[new_index] = replace(section, index=new_index, relocs=[])
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

    def __repr__(self) -> str:
        return f"InPlaceLinker(functions={len(self.functions)}, bss=({self.bss[0]:#x}, {self.bss[1]:#x}))"

    # -- placement -----------------------------------------------------------------------------

    def place(self, obj: ObjectFile, names_to_address: dict[str, int]) -> dict[int, Placement]:
        placements: dict[int, Placement] = {}
        offsets: dict[int, set[int]] = {}
        for fn in obj.defined_functions():
            if fn.shndx in self._removed_sections:
                continue
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
        removed = {}
        for index, aliases in by_section.items():
            section = obj.sections[index]
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
                    or any(sym.value != 0 or sym.size != section.size for sym in aliases)
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
        comdats = {}
        for index in removed:
            section = obj.sections[index]
            key = comdat_key(section)
            if key is not None:
                prior = comdats.setdefault(key, section)
                if section.data != prior.data or section.mapping != prior.mapping:
                    raise LinkError(f"COMDAT {key[0]} {key[1]} has contradictory removed-callee copies")
        return removed

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
                            original = self._read_original(lo, hi - lo, f"{section.name}: removed-call context")
                            for insn in _ARM.disasm(original, lo):
                                if not any(a <= insn.address < b for a, b in (*fn.get("pools", ()), *fn.get("literals", ()))):
                                    instructions[insn.address] = insn
                        call_sites = {
                            placed.base + relocation.offset for relocation in section.relocs
                            if relocation.symbol.shndx in self._removed_sections
                            and relocation.type in (R_ARM_CALL, R_ARM_JUMP24, R_ARM_PC24)
                        }
                        contexts[placed.base] = instructions, call_sites
                    instructions, call_sites = contexts[placed.base]
                    replacement = next(_ARM.disasm(struct.pack("<I", decision[1]), site, 1), None)
                    arguments = set(replacement.regs_access()[0]) & _ARM_ARGUMENTS if obj.sections[index].size == 8 else set()
                    if (fn["isa"] != "arm" or site not in instructions
                            or arm_call_abi_context(instructions, site, arguments, replacement.cc,
                                                    {placed.base, *(entry["address"] for entry in fn.get("entries", ()))},
                                                    call_sites) != "consistent"
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
        self._retained_sections = self._retention_sections(obj) if self.model.removed_inline else {}
        self._removed_sections = self._candidate_only_callees(obj, names_to_address or {})
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
