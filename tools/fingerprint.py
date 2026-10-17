"""Locate compiled ARM library functions without printing original code or disassembly.

This is a discovery tool, not a matching oracle. Relocated fields are wildcards;
all other bytes, including non-relocated literal pools, must agree. Confirm every
proposed identity with tools/match.py before adopting a symbol binding.
"""
from __future__ import annotations

import argparse
from dataclasses import dataclass, field
import hashlib
import json
from pathlib import Path
import sys
import tempfile
from collections.abc import Iterator

from elftools.common.exceptions import ELFError

sys.path.insert(0, str(Path(__file__).resolve().parent))
import image as image_mod  # noqa: E402
import inplace  # noqa: E402
import toolchain  # noqa: E402


class FingerprintError(RuntimeError):
    pass


# (field width, invariant bits), little endian. Preserve opcode/register bits:
# wildcarding entire instructions would turn short wrappers into spurious hits.
# R_ARM_CALL also permits ARM/Thumb BL/BLX interworking.
RELOCATION_MASKS = {
    1: (4, 0xFF000000),       # R_ARM_PC24
    2: (4, 0),                # R_ARM_ABS32
    3: (4, 0),                # R_ARM_REL32
    5: (2, 0),                # R_ARM_ABS16
    8: (1, 0),                # R_ARM_ABS8
    10: (4, 0xC000F800),      # R_ARM_THM_CALL (including BLX)
    28: (4, 0x0E000000),      # R_ARM_CALL
    29: (4, 0xFF000000),      # R_ARM_JUMP24
    30: (4, 0xD000F800),      # R_ARM_THM_JUMP24
    38: (4, 0),               # R_ARM_TARGET1
    41: (4, 0),               # R_ARM_TARGET2
    42: (4, 0x80000000),      # R_ARM_PREL31
    43: (4, 0xFFF0F000),      # R_ARM_MOVW_ABS_NC
    44: (4, 0xFFF0F000),      # R_ARM_MOVT_ABS
    45: (4, 0xFFF0F000),      # R_ARM_MOVW_PREL_NC
    46: (4, 0xFFF0F000),      # R_ARM_MOVT_PREL
    102: (2, 0xF800),         # R_ARM_THM_JUMP11
    103: (2, 0xFF00),         # R_ARM_THM_JUMP8
}
UNCHANGED_RELOCATIONS = {0, 40}  # R_ARM_NONE, R_ARM_V4BX (ARMv6K supports BX)


@dataclass(frozen=True)
class FunctionFingerprint:
    name: str
    isa: str
    data: bytes = field(repr=False)
    mask: bytes = field(repr=False)
    code_size: int
    literal_size: int
    relocation_count: int
    error: str | None = None
    section_index: int = 0
    section_offset: int = 0
    relocation_fields: tuple[inplace.Reloc, ...] = ()
    overlapping_entries: tuple[int, ...] = ()

    @property
    def size(self) -> int:
        return len(self.data)

    @property
    def invariant_bits(self) -> int:
        return sum(byte.bit_count() for byte in self.mask)

    def agrees(self, candidate: bytes) -> bool:
        return len(candidate) == self.size and all(
            (left ^ right) & mask == 0
            for left, right, mask in zip(self.data, candidate, self.mask)
        )


def function_fingerprints(obj: inplace.ObjectFile, *, symbol_extents: bool = False) -> list[FunctionFingerprint]:
    """Include the function's owned section tail, not just ELF st_size.

    armcc's st_size can omit a trailing $d literal pool. With split sections the
    section is the whole matching extent; in a shared section the next distinct
    function start bounds ownership. Aliases intentionally get separate rows.
    Archive assembly can export secondary entries inside a sized outer routine.
    With symbol_extents, the largest declaration at one start also bounds its
    zero-sized aliases; inner entries remain visible instead of truncating it.
    """
    functions = obj.defined_functions()
    output = []
    for symbol in functions:
        section = obj.sections[symbol.shndx]
        start = symbol.value & ~1
        if symbol_extents and symbol.size == 0 and start == section.size:
            continue  # exported end labels have no body to fingerprint
        following = [s.value & ~1 for s in functions
                     if s.shndx == symbol.shndx and (s.value & ~1) > start]
        end = min(following, default=section.size)
        if symbol_extents:
            declared_end = max(start + s.size for s in functions
                               if s.shndx == symbol.shndx and (s.value & ~1) == start)
            if declared_end > section.size:
                raise FingerprintError(f"{symbol.name}: declared function exceeds section")
            end = min((offset for offset in following if offset >= declared_end), default=section.size)
        if not section.executable or not 0 <= start < end <= len(section.data):
            raise FingerprintError(f"{symbol.name}: invalid executable function extent")
        if start + symbol.size > end:
            raise FingerprintError(f"{symbol.name}: overlapping function extents")
        data = section.data[start:end]
        mask = bytearray(b"\xff" * len(data))
        error = None
        count = 0
        for reloc in section.relocs:
            if reloc.type in UNCHANGED_RELOCATIONS:
                continue
            encoding = RELOCATION_MASKS.get(reloc.type)
            if encoding is None:
                if start <= reloc.offset < end:
                    error = f"unsupported relocation type {reloc.type}"
                continue
            width, invariant = encoding
            if reloc.offset < end and reloc.offset + width > start:
                count += 1
                if not start <= reloc.offset <= reloc.offset + width <= end:
                    error = "relocation crosses function boundary"
                    continue
                offset = reloc.offset - start
                for index, byte in enumerate(invariant.to_bytes(width, "little")):
                    mask[offset + index] &= byte
        mapping = [(offset, kind) for offset, kind in section.mapping if offset < end]
        initial = "$t" if symbol.value & 1 else "$a"
        kind = next((kind for offset, kind in reversed(mapping) if offset <= start), initial)
        isa = "thumb" if kind == "$t" else "arm"
        cursor = start
        literal_size = 0
        for offset, next_kind in [*[(a, k) for a, k in mapping if a > start], (end, "")]:
            if kind == "$d":
                literal_size += offset - cursor
            cursor, kind = offset, next_kind
        output.append(FunctionFingerprint(symbol.name, isa, data, bytes(mask),
                                          len(data) - literal_size, literal_size, count, error,
                                          symbol.shndx, start,
                                          tuple(reloc for reloc in section.relocs
                                                if start <= reloc.offset < end),
                                          tuple(sorted({offset - start for offset in following if offset < end}))))
    return output


def _anchor(fingerprint: FunctionFingerprint) -> tuple[int, bytes]:
    """Longest fully invariant byte run; find() filters the whole-text scan in C."""
    best_start = best_size = run_start = run_size = 0
    for index, mask in enumerate(fingerprint.mask):
        if mask == 0xFF:
            if run_size == 0:
                run_start = index
            run_size += 1
            if run_size > best_size:
                best_start, best_size = run_start, run_size
        else:
            run_size = 0
    return best_start, fingerprint.data[best_start:best_start + best_size]


def locate_function(fingerprint: FunctionFingerprint, image: image_mod.Image,
                    functions: dict[int, dict]) -> dict:
    """Prefer equal-size catalogue starts; only fall back when that pass has no hits."""
    text = image.layout.text
    text_end = text.address + text.size
    alignment = 2 if fingerprint.isa == "thumb" else 4
    row = {"name": fingerprint.name, "size": fingerprint.size, "isa": fingerprint.isa,
           "code_size": fingerprint.code_size, "literal_size": fingerprint.literal_size,
           "relocations": fingerprint.relocation_count, "invariant_bits": fingerprint.invariant_bits,
           "status": "no_hits", "confidence": "none", "scope": "catalogue_size", "hits": []}
    if fingerprint.error:
        row["reason"] = fingerprint.error
        return row

    def agrees(address: int) -> bool:
        return (text.address <= address <= text_end - fingerprint.size
                and address % alignment == 0
                and fingerprint.agrees(image.read(address, fingerprint.size)))

    hits = [address for address, fn in sorted(functions.items())
            if fn["size"] == fingerprint.size and fn["isa"] == fingerprint.isa and agrees(address)]
    if not hits:
        row["scope"] = "text"
        code = image.read(text.address, text.size)
        anchor_offset, anchor = _anchor(fingerprint)
        if anchor:
            cursor = 0
            while (found := code.find(anchor, cursor)) >= 0:
                address = text.address + found - anchor_offset
                if agrees(address):
                    hits.append(address)
                cursor = found + 1
        else:
            start = (text.address + alignment - 1) // alignment * alignment
            hits = [address for address in range(start, text_end - fingerprint.size + 1, alignment)
                    if agrees(address)]
    row["hits"] = [{"address": address, "size": fingerprint.size,
                    "catalogue_size": functions.get(address, {}).get("size")}
                   for address in hits]
    row["status"] = "unique" if len(hits) == 1 else "ambiguous" if hits else "no_hits"
    # Evidence ranks, not statistical probabilities; uniqueness in the first
    # pass does not exclude an identical interior/unequal-size occurrence.
    if hits:
        bits = fingerprint.invariant_bits
        fraction = bits / (8 * fingerprint.size)
        row["confidence"] = ("high" if len(hits) == 1 and row["scope"] == "catalogue_size"
                             and bits >= 128 and fraction >= 0.5 else
                             "medium" if len(hits) == 1 and bits >= 64 and fraction >= 0.25 else "low")
    return row


def fingerprint_object(obj_path: Path, image: image_mod.Image, catalogue: dict) -> list[dict]:
    if catalogue["image_sha256"] != image.sha256:
        raise FingerprintError("catalogue was generated from a different code.bin")
    functions = image_mod.functions_by_address(catalogue)
    return [locate_function(fn, image, functions)
            for fn in function_fingerprints(inplace.ObjectFile(obj_path))]


@dataclass(frozen=True)
class ArchiveMember:
    name: str
    offset: int
    data: bytes = field(repr=False)


def archive_members(path: Path) -> Iterator[ArchiveMember]:
    """Read regular SysV/GNU/BSD ar members in memory, never extracting input files."""
    data = path.read_bytes()
    if data.startswith(b"!<thin>\n"):
        raise FingerprintError(f"{path}: thin archives are not self-contained")
    if not data.startswith(b"!<arch>\n"):
        raise FingerprintError(f"{path}: not a regular ar archive")
    cursor = 8
    names = b""
    while cursor < len(data):
        header = data[cursor:cursor + 60]
        if len(header) != 60 or header[58:60] != b"`\n":
            raise FingerprintError(f"{path}: invalid ar header at offset {cursor}")
        try:
            size = int(header[48:58].strip())
            name = header[:16].decode("utf-8").rstrip()
        except (ValueError, UnicodeDecodeError):
            raise FingerprintError(f"{path}: invalid ar metadata at offset {cursor}") from None
        first, end = cursor + 60, cursor + 60 + size
        if size < 0 or end > len(data) or end + (size & 1) > len(data):
            raise FingerprintError(f"{path}: truncated ar member at offset {cursor}")
        payload = data[first:end]
        offset = cursor
        cursor = end + (size & 1)
        if name == "//":
            names = payload
            continue
        if name in ("/", "/SYM64/"):
            continue
        if name.startswith("#1/"):
            try:
                length = int(name[3:])
            except ValueError:
                raise FingerprintError(f"{path}: invalid BSD member name at offset {offset}") from None
            if not 0 < length <= size:
                raise FingerprintError(f"{path}: invalid BSD name length at offset {offset}")
            try:
                name = payload[:length].rstrip(b"\0").decode("utf-8")
            except UnicodeDecodeError:
                raise FingerprintError(f"{path}: invalid BSD member name at offset {offset}") from None
            payload = payload[length:]
        elif name.startswith("/") and name[1:].isdigit():
            index = int(name[1:])
            if not 0 <= index < len(names) or index and names[index - 1] not in (0, 10):
                raise FingerprintError(f"{path}: invalid GNU name offset at member {offset}")
            stop = names.find(b"\n", index)
            if stop < 0:
                raise FingerprintError(f"{path}: unterminated GNU member name at offset {offset}")
            try:
                name = names[index:stop].rstrip(b"/\0").decode("utf-8")
            except UnicodeDecodeError:
                raise FingerprintError(f"{path}: invalid GNU member name at offset {offset}") from None
        else:
            name = name.removesuffix("/")
        if name.startswith("__.SYMDEF"):
            continue
        if not name:
            raise FingerprintError(f"{path}: empty member name at offset {offset}")
        yield ArchiveMember(name, offset, payload)


def _masked(data: bytes, mask: bytes) -> bytes:
    return bytes(value & bits for value, bits in zip(data, mask))


def archive_function_report(fingerprint: FunctionFingerprint, image: image_mod.Image,
                            functions: dict[int, dict], search_cache: dict | None = None) -> dict:
    """Add safe digest/relocation metadata; masked equality is not target agreement."""
    normalized = _masked(fingerprint.data, fingerprint.mask)
    key = (fingerprint.isa, normalized, fingerprint.mask, fingerprint.error)
    if search_cache is not None and key in search_cache:
        located = search_cache[key]
    else:
        located = locate_function(fingerprint, image, functions)
        if search_cache is not None:
            search_cache[key] = located
    row = dict(located, name=fingerprint.name, code_size=fingerprint.code_size,
               literal_size=fingerprint.literal_size, relocations=fingerprint.relocation_count,
               object_section=fingerprint.section_index, object_offset=fingerprint.section_offset,
               object_body_sha256=hashlib.sha256(fingerprint.data).hexdigest(),
               masked_body_sha256=hashlib.sha256(normalized).hexdigest(),
               mask_sha256=hashlib.sha256(fingerprint.mask).hexdigest())
    row["overlapping_entries"] = list(fingerprint.overlapping_entries)
    row["relocation_fields"] = [
        {"offset": reloc.offset - fingerprint.section_offset, "type": reloc.type,
         "symbol": reloc.symbol.name, "symbol_section": reloc.symbol.shndx,
         "symbol_value": reloc.symbol.value, "symbol_kind": reloc.symbol.kind,
         "symbol_binding": reloc.symbol.bind}
        for reloc in fingerprint.relocation_fields
    ]
    row["hits"] = []
    for hit in located["hits"]:
        original = image.read(hit["address"], fingerprint.size)
        row["hits"].append(dict(
            hit, image_body_sha256=hashlib.sha256(original).hexdigest(),
            masked_body_sha256=hashlib.sha256(_masked(original, fingerprint.mask)).hexdigest(),
            whole_body_masked_equal=True, whole_body_exact_equal=original == fingerprint.data,
            relocation_agreement="unverified" if fingerprint.relocation_count else "not_applicable"))
    return row


def fingerprint_archive(path: Path, image: image_mod.Image, catalogue: dict,
                        search_cache: dict | None = None) -> dict:
    if catalogue["image_sha256"] != image.sha256:
        raise FingerprintError("catalogue was generated from a different code.bin")
    functions = image_mod.functions_by_address(catalogue)
    report = {"archive": path.as_posix(), "archive_sha256": toolchain.file_sha256(path),
              "image_sha256": image.sha256, "members": [], "functions": []}
    cache = search_cache if search_cache is not None else {}
    for member in archive_members(path):
        metadata = {"member": member.name, "member_offset": member.offset, "size": len(member.data),
                    "sha256": hashlib.sha256(member.data).hexdigest()}
        if not member.data.startswith(b"\x7fELF"):
            metadata["status"] = "linker_selection_metadata" if member.name == "SELECTION_SCRIPT" else "not_elf"
            report["members"].append(metadata)
            continue
        try:
            obj = inplace.ObjectFile(Path(f"{path}({member.name})"), data=member.data)
            fingerprints = function_fingerprints(obj, symbol_extents=True)
            metadata.update(status="ok", functions=len(fingerprints))
            metadata["empty_end_labels"] = [
                {"symbol": sym.name, "section": sym.shndx, "offset": sym.value & ~1}
                for sym in obj.defined_functions()
                if sym.size == 0 and (sym.value & ~1) == obj.sections[sym.shndx].size
            ]
            for fn in fingerprints:
                row = archive_function_report(fn, image, functions, cache)
                row.update(archive=path.as_posix(), member=member.name, member_offset=member.offset,
                           member_sha256=metadata["sha256"])
                report["functions"].append(row)
        except (FingerprintError, inplace.LinkError, ELFError, ValueError, KeyError) as exc:
            metadata.update(status="error", reason=f"{type(exc).__name__}: {exc}")
        report["members"].append(metadata)
    report["member_errors"] = sum(member["status"] not in ("ok", "linker_selection_metadata")
                                  for member in report["members"])
    return report

def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", nargs="?", type=Path, help="library translation unit (or ELF with --object)")
    parser.add_argument("--profile", help="name in config/compiler_profiles.json (required for source)")
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--object", action="store_true", help="fingerprint an existing ARM ELF object")
    mode.add_argument("--archive", type=Path, help="fingerprint every ELF member of a regular ar archive in memory")
    parser.add_argument("--include", "-I", type=Path, action="append", default=[], help="additional include directory")
    parser.add_argument("--define", "-D", action="append", default=[], help="compile-time macro, e.g. NW_RELEASE")
    parser.add_argument("--catalogue", type=Path, default=image_mod.CATALOGUE)
    parser.add_argument("--out", type=Path, help="write the same address-only JSON report to this path")
    args = parser.parse_args(argv)
    if args.archive and args.source or not args.archive and args.source is None:
        parser.error("supply either SOURCE or --archive LIBRARY")
    if not args.archive and not args.object and not args.profile:
        parser.error("--profile is required when compiling source")
    if (args.object or args.archive) and (args.include or args.define):
        parser.error("--include/--define apply only when compiling source")
    try:
        profile = None
        if args.profile:
            profiles = toolchain.load_profiles()
            if args.profile not in profiles:
                raise FingerprintError(f"unknown profile {args.profile!r}; known: {', '.join(sorted(profiles))}")
            base_profile = profiles[args.profile]
            profile = toolchain.Profile(base_profile.name, base_profile.compiler,
                                        (*base_profile.flags, *(f"-D{value}" for value in args.define)))
        image = image_mod.Image.load()
        catalogue = image_mod.load_catalogue(args.catalogue)
        if args.archive:
            report = fingerprint_archive(args.archive, image, catalogue)
        else:
            with tempfile.TemporaryDirectory(prefix="fingerprint-") as tmp:
                obj_path = args.source if args.object else Path(tmp) / "library.o"
                if not args.object:
                    toolchain.compile_source(profile, args.source, obj_path, tuple(args.include))
                rows = fingerprint_object(obj_path, image, catalogue)
            report = {"source": args.source.as_posix(), "image_sha256": image.sha256, "functions": rows}
            if profile:
                report.update(profile=profile.name, compiler=profile.compiler, flags=list(profile.flags))
        serialized = json.dumps(report, indent=1) + "\n"
        if args.out:
            args.out.parent.mkdir(parents=True, exist_ok=True)
            args.out.write_text(serialized, encoding="utf-8", newline="\n")
        print(serialized, end="")
        return 2 if report.get("member_errors") else 0
    except (FingerprintError, inplace.LinkError, image_mod.ImageError, toolchain.ToolchainError, ELFError, OSError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
