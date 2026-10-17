"""Locate compiled ARM library functions without printing original code or disassembly.

This is a discovery tool, not a matching oracle. Relocated fields are wildcards;
all other bytes, including non-relocated literal pools, must agree. Confirm every
proposed identity with tools/match.py before adopting a symbol binding.
"""
from __future__ import annotations

import argparse
from dataclasses import dataclass, field
import json
from pathlib import Path
import sys
import tempfile

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


def function_fingerprints(obj: inplace.ObjectFile) -> list[FunctionFingerprint]:
    """Include the function's owned section tail, not just ELF st_size.

    armcc's st_size can omit a trailing $d literal pool. With split sections the
    section is the whole matching extent; in a shared section the next distinct
    function start bounds ownership. Aliases intentionally get separate rows.
    """
    functions = obj.defined_functions()
    output = []
    for symbol in functions:
        section = obj.sections[symbol.shndx]
        start = symbol.value & ~1
        following = [s.value & ~1 for s in functions
                     if s.shndx == symbol.shndx and (s.value & ~1) > start]
        end = min(following, default=section.size)
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
                                          len(data) - literal_size, literal_size, count, error))
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


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path, help="library translation unit (or ELF with --object)")
    parser.add_argument("--profile", required=True, help="name in config/compiler_profiles.json")
    parser.add_argument("--object", action="store_true", help="fingerprint an existing ARM ELF object")
    parser.add_argument("--include", "-I", type=Path, action="append", default=[], help="additional include directory")
    parser.add_argument("--define", "-D", action="append", default=[], help="compile-time macro, e.g. NW_RELEASE")
    parser.add_argument("--catalogue", type=Path, default=image_mod.CATALOGUE)
    parser.add_argument("--out", type=Path, help="write the same address-only JSON report to this path")
    args = parser.parse_args(argv)
    try:
        profiles = toolchain.load_profiles()
        if args.profile not in profiles:
            raise FingerprintError(f"unknown profile {args.profile!r}; known: {', '.join(sorted(profiles))}")
        base_profile = profiles[args.profile]
        profile = toolchain.Profile(base_profile.name, base_profile.compiler,
                                    (*base_profile.flags, *(f"-D{value}" for value in args.define)))
        image = image_mod.Image.load()
        catalogue = image_mod.load_catalogue(args.catalogue)
        with tempfile.TemporaryDirectory(prefix="fingerprint-") as tmp:
            obj_path = args.source if args.object else Path(tmp) / "library.o"
            if not args.object:
                toolchain.compile_source(profile, args.source, obj_path, tuple(args.include))
            rows = fingerprint_object(obj_path, image, catalogue)
        report = {"source": args.source.as_posix(), "profile": profile.name,
                  "compiler": profile.compiler, "flags": list(profile.flags),
                  "image_sha256": image.sha256, "functions": rows}
        serialized = json.dumps(report, indent=1) + "\n"
        if args.out:
            args.out.parent.mkdir(parents=True, exist_ok=True)
            args.out.write_text(serialized, encoding="utf-8", newline="\n")
        print(serialized, end="")
        return 0
    except (FingerprintError, inplace.LinkError, image_mod.ImageError, toolchain.ToolchainError, OSError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
