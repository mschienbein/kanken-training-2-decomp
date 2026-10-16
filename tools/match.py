"""Compile one candidate translation unit and compare it, in place, with the original executable.

    python tools/match.py src/foo.c --profile <name> [--catalogue config/catalogue.json] [--out DIR]

Every function the unit defines must be named `func_XXXXXXXX` (its catalogue address) or appear in
config/names.json. Each is linked at its original address with tools/inplace.py and compared byte
for byte over its whole catalogued extent (code + literal pool). Exit status: 0 all functions
match, 1 some differ, 2 the candidate could not be compiled or linked. A JSON result is written next
to the object. This is the author's loop; adoption and acceptance happen in the integrator's build.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import image as image_mod  # noqa: E402
import inplace  # noqa: E402
import toolchain  # noqa: E402
from capstone import CS_ARCH_ARM, CS_MODE_ARM, CS_MODE_THUMB, Cs  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
_DISASM = {"arm": Cs(CS_ARCH_ARM, CS_MODE_ARM), "thumb": Cs(CS_ARCH_ARM, CS_MODE_THUMB)}


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def render(data: bytes, base: int, isa: str, data_ranges: list[tuple[int, int]]) -> list[tuple[int, int, str]]:
    """(address, size, text) rows; `data_ranges` are absolute [start, end) literal-pool spans."""
    rows = []
    pos = 0
    step = 4 if isa == "arm" else 2
    while pos < len(data):
        addr = base + pos
        if any(lo <= addr < hi for lo, hi in data_ranges) or len(data) - pos < step:
            word = data[pos : pos + 4]
            rows.append((addr, len(word), ".word 0x" + word[::-1].hex()))
            pos += len(word)
            continue
        insn = next(_DISASM[isa].disasm(data[pos : pos + 4], addr, 1), None)
        if insn is None:
            rows.append((addr, step, ".inst 0x" + data[pos : pos + step][::-1].hex()))
            pos += step
        else:
            rows.append((addr, insn.size, f"{insn.mnemonic} {insn.op_str}".strip()))
            pos += insn.size
    return rows


def diff_lines(original: bytes, candidate: bytes, base: int, fn: dict, limit: int) -> list[str]:
    pools = [tuple(p) for p in (*fn.get("pools", []), *fn.get("literals", []))]  # data inside the section
    left = render(original, base, fn["isa"], pools)
    right = render(candidate, base, fn["isa"], pools)
    out = []
    for i in range(max(len(left), len(right))):
        a = left[i] if i < len(left) else (0, 0, "")
        b = right[i] if i < len(right) else (0, 0, "")
        mark = " " if a[2] == b[2] else "!"
        out.append(f"{mark} {a[0] or b[0]:08x}  {a[2]:<40} {b[2]}")
    shown = [i for i, line in enumerate(out) if line.startswith("!")]
    if not shown:
        return out[:limit]
    first = max(0, shown[0] - 3)
    return out[first : first + limit]


class Context:
    """The original image, catalogue and resolver shared by every comparison."""

    def __init__(self, catalogue_path: Path):
        self.image = image_mod.Image.load()
        self.catalogue_path = catalogue_path
        catalogue = image_mod.load_catalogue(catalogue_path)
        if catalogue["image_sha256"] != self.image.sha256:
            raise inplace.LinkError("catalogue was generated from a different code.bin")
        self.functions = image_mod.functions_by_address(catalogue)
        self.resolver = inplace.Resolver(self.functions, image_mod.load_names())

    def compare(self, obj_path: Path) -> list[dict]:
        """Link the object in place; one result row per catalogued function it defines."""
        obj = inplace.ObjectFile(obj_path)
        linked = inplace.InPlaceLinker(self.image, self.functions, self.resolver, self.image.bss).link(obj)
        results = []
        for placed, data in linked.values():
            if placed.origin != "catalogue":
                continue
            fn = self.functions[placed.base]
            span = max(len(data), fn["size"])
            try:
                original = self.image.read(placed.base, span)
            except image_mod.ImageError:
                original = self.image.read(placed.base, fn["size"])
            same = data == original[: len(data)]
            if same and len(data) == fn["size"]:
                status = "match"
            elif same:
                # The compiled section reproduces the original but ends elsewhere than the
                # catalogue says: a catalogue boundary to revise, not credit.
                status = "match_extent_differs"
            else:
                status = "mismatch"
            common = min(len(data), len(original))
            first = None if same else next((i for i in range(common) if data[i] != original[i]), common)
            name = next(s.name for s in obj.defined_functions() if s.shndx == placed.section.index)
            results.append({
                "name": name, "address": placed.base, "size": fn["size"], "candidate_size": len(data),
                "status": status, "first_diff": first,
                "_original": original, "_candidate": data,
            })
        return sorted(results, key=lambda r: r["address"])


def run(source: Path, profile: toolchain.Profile, catalogue_path: Path, out_dir: Path, show: int) -> int:
    ctx = Context(catalogue_path)
    obj_path = out_dir / (source.stem + ".o")
    toolchain.compile_source(profile, source, obj_path)
    results = ctx.compare(obj_path)
    for r in results:
        label = {"match": "MATCH   ", "match_extent_differs": "EXTENT  ", "mismatch": "MISMATCH"}[r["status"]]
        detail = {
            "match": "",
            "match_extent_differs": " bytes reproduce the original; catalogue extent needs revision",
            "mismatch": f" first difference at +{r['first_diff']:#x}" if r["first_diff"] is not None else "",
        }[r["status"]]
        print(f"{label} {r['name']} @ {r['address']:#x} size {r['candidate_size']:#x}/{r['size']:#x}{detail}")
        if r["status"] == "mismatch" and show:
            for line in diff_lines(r["_original"], r["_candidate"], r["address"], ctx.functions[r["address"]], show):
                print("   " + line)
    record = {
        "source": source.as_posix(), "source_sha256": sha256(source), "profile": profile.name,
        "compiler": profile.compiler, "compiler_sha256": sha256(toolchain.armcc_exe(profile.compiler)),
        "flags": list(profile.flags), "object_sha256": sha256(obj_path), "image_sha256": ctx.image.sha256,
        "catalogue_sha256": sha256(catalogue_path),
        "functions": [{k: v for k, v in r.items() if not k.startswith("_")} for r in results],
    }
    (out_dir / (source.stem + ".result.json")).write_text(json.dumps(record, indent=1) + "\n", encoding="utf-8", newline="\n")
    return 0 if results and all(r["status"] == "match" for r in results) else 1


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("source", type=Path)
    parser.add_argument("--profile", required=True, help="name in config/compiler_profiles.json")
    parser.add_argument("--catalogue", type=Path, default=image_mod.CATALOGUE)
    parser.add_argument("--out", type=Path, help="output directory (default scratch/match/<source stem>/<profile>)")
    parser.add_argument("--show", type=int, default=24, help="diff rows to print per mismatching function")
    args = parser.parse_args(argv)
    profiles = toolchain.load_profiles()
    if args.profile not in profiles:
        print(f"error: unknown profile {args.profile!r}; known: {', '.join(sorted(profiles))}", file=sys.stderr)
        return 2
    out_dir = args.out or ROOT / "scratch/match" / args.source.stem / args.profile
    try:
        return run(args.source.resolve(), profiles[args.profile], args.catalogue, out_dir, args.show)
    except (inplace.LinkError, toolchain.ToolchainError, image_mod.ImageError) as e:
        print(f"error: {e}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
