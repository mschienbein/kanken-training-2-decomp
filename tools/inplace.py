"""Strict in-place link of an armcc ELF object against the original executable.

Function sections are placed at their catalogue addresses. Every other allocatable section the
code references (string pools, const tables, statics, BSS) is placed where the original bytes say
it lives: each relocation's original resolved value fixes the section base, all references must
agree, and the section's own bytes must then equal the original bytes at that base (BSS must fall
inside the BSS range). Relocations are applied exactly as armlink would for ARMv6K, including
ARM<->Thumb BL/BLX interworking; an unsupported relocation type, an unresolved symbol or a
branch that would need a linker veneer is an error, never a silent pass.

Because relocated fields must reproduce the original bytes, a byte match also proves every call
and data reference resolves to the same address as in the original.
"""
from __future__ import annotations

import re
import struct
from dataclasses import dataclass, field
from pathlib import Path

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
            if m.group(1) == "func" and address not in self.functions:
                raise LinkError(f"{name}: no catalogued function at {address:#x}")
        fn = self.functions.get(address)
        return Target(address, bool(fn and fn["isa"] == "thumb"))


@dataclass
class Placement:
    section: Section
    base: int
    origin: str  # "catalogue" or "inferred"


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
    def __init__(self, image, functions: dict[int, dict], resolver: Resolver, bss: tuple[int, int]):
        """image: object with read(address, size) -> bytes; functions: catalogue functions by address;
        bss: [start, end) of zero-initialised memory."""
        self.image = image
        self.functions = functions
        self.resolver = resolver
        self.bss = bss

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
        # Infer data sections from the original values their references resolve to.
        changed = True
        while changed:
            changed = False
            for placed in list(placements.values()):
                for rel in placed.section.relocs:
                    sym = rel.symbol
                    if not isinstance(sym.shndx, int) or sym.shndx not in obj.sections:
                        continue
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
                    elif prior.base != base:
                        raise LinkError(
                            f"{obj.sections[sym.shndx].name}: references disagree on its address "
                            f"({prior.base:#x} vs {base:#x})"
                        )
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

    def _symbol_target(self, sym: Symbol, obj: ObjectFile, placements: dict[int, Placement]) -> Target:
        if isinstance(sym.shndx, int) and sym.shndx in obj.sections:
            placed = placements.get(sym.shndx)
            if placed is None:
                raise LinkError(f"{sym.name or obj.sections[sym.shndx].name}: section was never placed")
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

    # -- link + verify -------------------------------------------------------------------------

    def link(self, obj: ObjectFile, names_to_address: dict[str, int] | None = None) -> dict[int, tuple[Placement, bytes]]:
        """Place and relocate every referenced section; returns {section index: (placement, bytes)}.

        Inferred (non-function) sections are verified here: their relocated bytes must equal the
        original image, BSS must lie in the BSS range. Function sections are returned unverified so
        the caller can report per-function differences.
        """
        placements = self.place(obj, names_to_address or {})
        linked: dict[int, tuple[Placement, bytes]] = {}
        for idx, placed in placements.items():
            sec = placed.section
            if sec.nobits:
                lo, hi = self.bss
                if not (lo <= placed.base and placed.base + sec.size <= hi):
                    raise LinkError(f"{sec.name}: placed at {placed.base:#x}, outside BSS [{lo:#x}, {hi:#x})")
                linked[idx] = (placed, b"")
                continue
            data = bytearray(sec.data)
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
                self._apply(rel, sec, data, placed.base + rel.offset, self._symbol_target(rel.symbol, obj, placements))
            linked[idx] = (placed, bytes(data))
            if placed.origin == "inferred":
                original = self.image.read(placed.base, sec.size)
                if original != bytes(data):
                    first = next(i for i in range(sec.size) if original[i] != data[i])
                    raise LinkError(
                        f"{sec.name}: placed at {placed.base:#x} from its references, but its bytes differ from "
                        f"the original at +{first:#x}"
                    )
        return linked
