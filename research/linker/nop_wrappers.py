"""Can the catalogue's NOP prefixes be C tail-call wrappers? (docs/LINKER.md, catalogue v4 item 2)

    .venv/bin/python research/linker/nop_wrappers.py [--out scratch/armlink/nop-wrappers]

Writes a scratch catalogue that splits, at `address + 4`, every v3 `nop_entry` group and every
catalogued start whose first word is MOV r0, r0 with no entry after it; then compiles one wrapper
`void func_X(void) { func_X+4(); }` per split point and compares it in place under each linker
model against both catalogues. Prints status counts only.
"""
from __future__ import annotations

import argparse
import collections
import copy
import json
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
import image as image_mod  # noqa: E402
import inplace  # noqa: E402
import match  # noqa: E402
import toolchain  # noqa: E402

MOV_R0_R0 = 0xE1A00000


def split_points(catalogue: dict, image: image_mod.Image) -> dict[str, list[int]]:
    entries = {e["address"] for f in catalogue["functions"] for e in f["entries"]}
    nop_entry = sorted(e["address"] for f in catalogue["functions"] for e in f["entries"] if e["kind"] == "nop_entry")
    leading = sorted(
        f["address"] for f in catalogue["functions"]
        if f["isa"] == "arm" and f["address"] not in nop_entry and f["address"] + 4 not in entries
        and struct.unpack("<I", image.read(f["address"], 4))[0] == MOV_R0_R0
    )
    return {"nop_entry": nop_entry, "leading_nop": leading}


def split(catalogue: dict, points: set[int]) -> dict:
    out = copy.deepcopy(catalogue)
    functions = []
    for f in out["functions"]:
        a = f["address"]
        if a not in points:
            functions.append(f)
            continue
        head = {"address": a, "size": 4, "code_size": 4, "code_ranges": [[a, a + 4]], "isa": "arm",
                "kind": "function", "entries": [dict(e, kind="entry") for e in f["entries"] if e["address"] == a],
                "evidence": f["evidence"], "literals": [], "pools": [], "noreturn": False, "noreturn_reasons": []}
        rest = [e for e in f["entries"] if e["address"] >= a + 4]
        if not any(e["address"] == a + 4 for e in rest):
            rest.insert(0, {"address": a + 4, "isa": "arm", "kind": "entry", "evidence": ["split"], "sources": []})
        ranges = [[max(lo, a + 4), hi] for lo, hi in f["code_ranges"] if hi > a + 4]
        tail = dict(f, address=a + 4, size=f["size"] - 4, code_ranges=ranges,
                    code_size=sum(hi - lo for lo, hi in ranges), entries=rest,
                    kind="function" if len(rest) == 1 else f["kind"])
        functions += [head, tail]
    out["functions"] = functions
    image_mod.functions_by_address(out)  # refuses overlaps
    return out


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--out", type=Path, default=ROOT / "scratch/armlink/nop-wrappers")
    args = parser.parse_args()
    args.out.mkdir(parents=True, exist_ok=True)
    image = image_mod.Image.load()
    v3 = image_mod.load_catalogue()
    points = split_points(v3, image)
    v4 = args.out / "catalogue-split.json"
    v4.write_text(json.dumps(split(v3, {a for group in points.values() for a in group})), encoding="utf-8")
    profile = toolchain.load_profiles()["game"]
    report = {}
    for group, addresses in points.items():
        src = args.out / f"{group}.c"
        src.write_text("".join(f"extern void func_{a + 4:08x}(void);\nvoid func_{a:08x}(void) {{ func_{a + 4:08x}(); }}\n"
                               for a in addresses), encoding="utf-8")
        obj = src.with_suffix(".o")
        toolchain.compile_source(profile, src, obj)
        for catalogue in (image_mod.CATALOGUE, v4):
            for model in sorted(inplace.LINKER_MODELS):
                key = f"{group} ({len(addresses)}) / {catalogue.name} / {model}"
                try:
                    rows = match.Context(catalogue, model).compare(obj)
                    report[key] = dict(collections.Counter(r["status"] for r in rows))
                except inplace.LinkError as e:
                    report[key] = f"LinkError: {e}"
    print(json.dumps(report, indent=1))


if __name__ == "__main__":
    main()
