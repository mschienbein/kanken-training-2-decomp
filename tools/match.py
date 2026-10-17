"""Compile one candidate translation unit and compare it, in place, with the original executable.

    python tools/match.py src/foo.c --profile <name> [--catalogue config/catalogue.json] [--out DIR]
                          [--linker-model NAME]

Every function the unit defines must be named `func_XXXXXXXX` (its catalogue address) or appear in
config/names.json. Each is linked at its original address with tools/inplace.py and compared byte
for byte over its whole catalogued extent (code + literal pool). The linker model (default
`inplace.DEFAULT_LINKER_MODEL`, docs/LINKER.md) decides which armlink output optimisations the
link reproduces; every branch it rewrote is listed per function. Exit status: 0 all functions
match, 1 some differ, 2 the candidate could not be compiled or linked. A JSON result is written next
to the object. This is the author's loop; adoption and acceptance happen in the integrator's build.
"""
from __future__ import annotations

import argparse
import collections
import hashlib
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import display  # noqa: E402
import image as image_mod  # noqa: E402
import inplace  # noqa: E402
import toolchain  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def render(data: bytes, base: int, isa: str, code_ranges: list[tuple[int, int]],
           data_ranges: list[tuple[int, int]]) -> list[tuple[int, int, str]]:
    """(address, size, text) rows bounded by absolute catalogue code/data ranges."""
    rows = []
    for kind, addr, chunk, insn in display.spans(data, base, isa, code_ranges, data_ranges):
        if kind == "insn":
            text = f"{insn.mnemonic} {insn.op_str}".strip()
        else:
            directive = ".inst" if kind == "raw" else ".word"
            text = directive + " 0x" + chunk[::-1].hex()
        rows.append((addr, len(chunk), text))
    return rows


def diff_lines(original: bytes, candidate: bytes, base: int, fn: dict, limit: int) -> list[str]:
    pools = [tuple(p) for p in (*fn.get("pools", []), *fn.get("literals", []))]  # data inside the section
    code_ranges = [tuple(p) for p in fn["code_ranges"]]
    left = {addr: text for addr, _, text in render(original, base, fn["isa"], code_ranges, pools)}
    right = {addr: text for addr, _, text in render(candidate, base, fn["isa"], code_ranges, pools)}
    out = []
    # Thumb instruction widths can differ; keep both columns at their actual addresses.
    for addr in sorted(left.keys() | right.keys()):
        a, b = left.get(addr, ""), right.get(addr, "")
        mark = " " if a == b else "!"
        out.append(f"{mark} {addr:08x}  {a:<40} {b}")
    shown = [i for i, line in enumerate(out) if line.startswith("!")]
    if not shown:
        return out[:limit]
    first = max(0, shown[0] - 3)
    return out[first : first + limit]


def merge_names(base: dict[str, dict], proposed: dict[str, dict]) -> dict[str, dict]:
    """config/names.json plus an author's proposed bindings; a proposal may add names, never move one."""
    clash = sorted(n for n, e in proposed.items() if n in base and base[n]["address"] != e["address"])
    if clash:
        raise inplace.LinkError(f"proposed names rebind config/names.json names: {', '.join(clash[:5])}")
    return {**base, **proposed}


class Context:
    """The original image, catalogue and resolver shared by every comparison."""

    def __init__(self, catalogue_path: Path, model: str = inplace.DEFAULT_LINKER_MODEL, names_path: Path | None = None):
        self.image = image_mod.Image.load()
        self.catalogue_path = catalogue_path
        catalogue = image_mod.load_catalogue(catalogue_path)
        if catalogue["image_sha256"] != self.image.sha256:
            raise inplace.LinkError("catalogue was generated from a different code.bin")
        self.functions = image_mod.functions_by_address(catalogue)
        names = image_mod.load_names()
        if names_path is not None:
            names = merge_names(names, image_mod.load_names(names_path))
        self.resolver = inplace.Resolver(self.functions, names)
        self.model = inplace.linker_model(model)

    def compare(self, obj_path: Path) -> list[dict]:
        """Link the object in place; one result row per catalogued function it defines, listing
        the branches the linker model wrote as armlink's optimisations (`linker_rewrites`)."""
        obj = inplace.ObjectFile(obj_path)
        linker = inplace.InPlaceLinker(self.image, self.functions, self.resolver, self.image.bss, self.model)
        linked = linker.link(obj)
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
                "linker_rewrites": [w.record() for w in linker.rewrites if w.section == placed.section.index],
                "_original": original, "_candidate": data,
            })
        return sorted(results, key=lambda r: r["address"])


def run(source: Path, profile: toolchain.Profile, catalogue_path: Path, out_dir: Path, show: int,
        model: str = inplace.DEFAULT_LINKER_MODEL, names_path: Path | None = None) -> int:
    ctx = Context(catalogue_path, model, names_path)
    if names_path is not None:
        print(f"note: proposed names overlay {names_path} (not config/names.json); a MATCH here needs those bindings adopted")
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
        rewrites = collections.Counter(w["rule"] for w in r["linker_rewrites"])
        note = f" [{ctx.model.name}: {', '.join(f'{n} {rule}' for rule, n in sorted(rewrites.items()))}]" if rewrites else ""
        print(f"{label} {r['name']} @ {r['address']:#x} size {r['candidate_size']:#x}/{r['size']:#x}{detail}{note}")
        if r["status"] == "mismatch" and show:
            for line in diff_lines(r["_original"], r["_candidate"], r["address"], ctx.functions[r["address"]], show):
                print("   " + line)
    record = {
        "source": source.as_posix(), "source_sha256": sha256(source), "profile": profile.name,
        "compiler": profile.compiler, "compiler_sha256": sha256(toolchain.armcc_exe(profile.compiler)),
        "flags": list(profile.flags), "object_sha256": sha256(obj_path), "image_sha256": ctx.image.sha256,
        "catalogue_sha256": sha256(catalogue_path), "linker_model": ctx.model.name,
        "proposed_names": None if names_path is None else {"path": names_path.as_posix(), "sha256": sha256(names_path)},
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
    parser.add_argument("--linker-model", default=inplace.DEFAULT_LINKER_MODEL, choices=sorted(inplace.LINKER_MODELS),
                        help="armlink output optimisations reproduced by the in-place link (docs/LINKER.md)")
    parser.add_argument("--names", type=Path,
                        help="proposed extra bindings (config/names.json schema) merged over config/names.json; "
                             "may add names, never rebind one")
    args = parser.parse_args(argv)
    profiles = toolchain.load_profiles()
    if args.profile not in profiles:
        print(f"error: unknown profile {args.profile!r}; known: {', '.join(sorted(profiles))}", file=sys.stderr)
        return 2
    out_dir = args.out or ROOT / "scratch/match" / args.source.stem / args.profile
    try:
        return run(args.source.resolve(), profiles[args.profile], args.catalogue, out_dir, args.show, args.linker_model, args.names)
    except (inplace.LinkError, toolchain.ToolchainError, image_mod.ImageError) as e:
        print(f"error: {e}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
