"""Count possible EHABI table structure in the flat original image; never emit payload.

A flat code.bin has no ELF section headers. This reports structural evidence, not a proof
that an unreferenced or nonstandard exception table cannot exist. Candidate ELF metadata
can be inspected separately with --object. No disassembly or ROM words are printed.
"""
from __future__ import annotations

import argparse
from bisect import bisect_right
import collections
import hashlib
import json
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
import image  # noqa: E402
import inplace  # noqa: E402


def prel31(word: int, place: int) -> int:
    return place + ((word & 0x7FFFFFFF) ^ 0x40000000) - 0x40000000


def census(original: image.Image, catalogue: dict) -> dict:
    segments = (original.layout.text, original.layout.rodata, original.layout.data)
    text = original.layout.text
    code = sorted(tuple(span) for fn in catalogue["functions"] for span in fn.get("code_ranges", ()))
    starts = [lo for lo, _ in code]
    entries = {fn["address"] for fn in catalogue["functions"]}
    entries.update(entry["address"] for fn in catalogue["functions"] for entry in fn.get("entries", ()))

    def in_text(target: int) -> bool:
        return text.address <= (target & ~1) < text.address + text.size

    def in_readonly(target: int, size: int = 4) -> bool:
        return any(seg.address <= target <= seg.address + seg.size - size for seg in segments[:2])

    def in_code(address: int) -> bool:
        index = bisect_right(starts, address + 7) - 1
        return index >= 0 and code[index][1] > address

    first_counts = collections.Counter()
    second_counts = collections.Counter()
    candidates = []
    nontext_keys = []
    for seg in segments:
        data = original.read(seg.address, seg.size)
        for offset in range(0, len(data) - 7, 4):
            address = seg.address + offset
            first, second = struct.unpack_from("<2I", data, offset)
            target = prel31(first, address)
            if first & 0x80000000 or not in_text(target):
                continue
            first_counts[seg.name] += 1
            kind = None
            if second == 1:
                kind = "cantunwind"
            elif second >> 24 == 0x80:
                kind = "compact_inline"
            elif not second & 0x80000000:
                extab = prel31(second, address + 4)
                if in_readonly(extab):
                    header = struct.unpack("<I", original.read(extab, 4))[0]
                    if header >> 24 in (0x80, 0x81, 0x82):
                        kind = "compact_extab_possible"
                    elif not header & 0x80000000 and in_text(prel31(header, extab)):
                        kind = "generic_extab_possible"
            if seg.name != "text":
                nontext_keys.append({"address": address, "target": target, "second_word_kind": kind})
            if kind:
                second_counts[seg.name] += 1
                if not in_code(address):
                    candidates.append({"address": address, "target": target, "kind": kind,
                                       "target_is_catalogued_entry": (target & ~1) in entries})
    by_address = {row["address"]: row for row in candidates}
    pairs = [[address, address + 8] for address, row in by_address.items()
             if address + 8 in by_address and row["target"] < by_address[address + 8]["target"]]
    return {
        "image_sha256": original.sha256,
        "catalogue_sha256": hashlib.sha256(json.dumps(catalogue, sort_keys=True).encode()).hexdigest(),
        "prel31_text_key_candidates": {seg.name: first_counts[seg.name] for seg in segments},
        "possible_second_words": {seg.name: second_counts[seg.name] for seg in segments},
        "nontext_key_candidates": nontext_keys,
        "possible_entries_outside_catalogued_code": candidates,
        "outside_code_entries_targeting_catalogued_starts": sum(row["target_is_catalogued_entry"] for row in candidates),
        "sorted_adjacent_entry_pairs_outside_code": pairs,
        "qualification": "[INFERENCE] No identified EHABI table is not proof of absence from a sectionless flat image. "
                         "Loose pointer-shaped candidates are not authenticated unwind entries.",
    }


def object_metadata(path: Path) -> dict:
    obj = inplace.ObjectFile(path)
    return {
        "path": str(path), "sha256": hashlib.sha256(path.read_bytes()).hexdigest(),
        "unwind_sections": [
            {"index": section.index, "name": section.name, "size": section.size,
             "type": section.elf_type, "link_order": section.link_order,
             "linked_section": section.linked_section,
             "linked_section_name": obj.sections[section.linked_section].name if section.linked_section in obj.sections else None,
             "relocations": [{"offset": rel.offset, "type": rel.type, "symbol": rel.symbol.name,
                              "target_section": rel.symbol.shndx} for rel in section.relocs]}
            for section in obj.sections.values() if section.name.startswith((".ARM.exidx", ".ARM.extab"))
        ],
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--catalogue", type=Path, default=image.CATALOGUE)
    parser.add_argument("--object", type=Path, action="append", default=[])
    args = parser.parse_args()
    result = census(image.Image.load(), image.load_catalogue(args.catalogue))
    result["objects"] = [object_metadata(path) for path in args.object]
    print(json.dumps(result, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
