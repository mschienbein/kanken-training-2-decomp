"""Compiler identification: compile probe sources under a grid of compiler builds x flag sets.

    python tools/compid.py probe.c [more.c ...] [--grid grid.json] [--catalogue PATH] [--out DIR]

Each probe defines functions named func_XXXXXXXX whose C was written to reproduce the original.
Every (compiler, flag set) pair is compiled and linked in place (tools/match.py's oracle, under the
selected `--linker-model`, docs/LINKER.md); the table shows which pairs reproduce which functions
byte for byte. A pair that matches every probe is a candidate profile; evidence is written to
<out>/compid.json, with the linker model and every branch it rewrote per function and pair. Probes
must be reviewed C, not tuned per compiler: the point is to discriminate builds, so one source text
is used for all pairs.
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
import match  # noqa: E402
import toolchain  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
BASE = ["--cpu=MPCore", "--fpmode=fast", "--apcs=/interwork", "--arm", "--split_sections", "--signed_chars"]
DEFAULT_GRID = {
    "compilers": ["armcc_40_821", "armcc_41_894", "armcc_41_921", "armcc_41_1049", "armcc_41_1440", "armcc_41_1454", "armcc_504_82"],
    "flagsets": {
        "O3-Otime": BASE + ["-O3", "-Otime"],
        "O3-Ospace": BASE + ["-O3", "-Ospace"],
        "O2-Ospace": BASE + ["-O2", "-Ospace"],
        "O2-Otime": BASE + ["-O2", "-Otime"],
    },
}


def run_grid(ctx, sources: list[Path], grid: dict, out: Path) -> dict:
    """Compile every source under every (compiler, flag set) pair and compare in place.

    Fails closed: a pair qualifies only if every source compiled, linked and produced at least
    one comparable function, and every function of every source matched.
    """
    rows: dict[str, dict[str, str]] = {}
    rewrites: dict[str, dict[str, list[dict]]] = {}  # function -> pair -> linker rewrites (non-empty only)
    failures: dict[str, dict[str, str]] = {}  # pair -> {source: reason}
    pairs = []
    section_rewrites: dict[str, dict[str, list[dict]]] = {}
    special_members: dict[str, dict[str, list[dict]]] = {}
    comparison_inputs: dict[str, dict[str, dict]] = {}
    checked = getattr(ctx.model, "checked_destructor_removal", False)
    for compiler in grid["compilers"]:
        for fname, flags in grid["flagsets"].items():
            pair = f"{compiler}:{fname}"
            pairs.append(pair)
            profile = toolchain.Profile(pair, compiler, tuple(flags))
            for source_index, src in enumerate(sources):
                directory = out / "obj" / compiler / fname
                if checked:
                    directory /= str(source_index)  # same-stem probes must keep distinct bound objects
                obj = directory / (src.stem + ".o")
                try:
                    if checked:
                        toolchain.compile_source(profile, src.resolve(), obj, cache=False)
                    else:
                        toolchain.compile_source(profile, src.resolve(), obj)
                    results = (ctx.compare(obj, compiled_object=True)
                               if getattr(ctx.model, "single_object_u1", False) else ctx.compare(obj))
                    if checked:
                        comparison_inputs.setdefault(src.as_posix(), {})[pair] = ctx.evidence_inputs(profile, src.resolve(), obj)
                    if getattr(ctx, "section_rewrites", []):
                        section_rewrites.setdefault(src.as_posix(), {})[pair] = ctx.section_rewrites
                        special_members.setdefault(src.as_posix(), {})[pair] = ctx.removed_special_members
                except (toolchain.ToolchainError, inplace.LinkError, image_mod.ImageError) as e:
                    failures.setdefault(pair, {})[src.as_posix()] = str(e).splitlines()[0]
                    continue
                if not results:
                    failures.setdefault(pair, {})[src.as_posix()] = "no catalogued function in the object"
                    continue
                for r in results:
                    cell = {"match": "Y", "match_extent_differs": "E"}.get(r["status"]) or f"+{r['first_diff']:x}"
                    rows.setdefault(r["name"], {})[pair] = cell
                    if r["linker_rewrites"]:
                        rewrites.setdefault(r["name"], {})[pair] = r["linker_rewrites"]
    names = sorted(rows)
    qualifying = [
        p for p in pairs
        if names and p not in failures and all(rows[n].get(p) == "Y" for n in names)
    ]
    return {"pairs": pairs, "functions": names, "results": rows, "failures": failures, "matching_all": qualifying,
            "linker_model": ctx.model.name, "linker_rewrites": rewrites,
            **({"linker_section_rewrites": section_rewrites, "removed_special_members": special_members}
               if section_rewrites else {}),
            **({"comparison_inputs": comparison_inputs} if comparison_inputs else {})}


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("sources", type=Path, nargs="+")
    parser.add_argument("--grid", type=Path, help='JSON {"compilers": [...], "flagsets": {name: [flags]}}')
    parser.add_argument("--catalogue", type=Path, default=ROOT / "config/catalogue.json")
    parser.add_argument("--out", type=Path, default=ROOT / "scratch/compid")
    parser.add_argument("--linker-model", default=inplace.DEFAULT_LINKER_MODEL, choices=sorted(inplace.LINKER_MODELS),
                        help="armlink output optimisations reproduced by the in-place link (docs/LINKER.md)")
    args = parser.parse_args(argv)
    grid = json.loads(args.grid.read_text(encoding="utf-8")) if args.grid else DEFAULT_GRID
    ctx = match.Context(args.catalogue, args.linker_model)
    result = run_grid(ctx, args.sources, grid, args.out)

    width = max(len(p) for p in result["pairs"])
    names = result["functions"]
    print(" " * width + "  " + "  ".join(n[-8:] for n in names))
    for pair in result["pairs"]:
        cells = [result["results"][n].get(pair, "-") for n in names]
        failed = result["failures"].get(pair, {})
        note = f"   FAILED: {', '.join(Path(s).name for s in failed)}" if failed else ""
        print(f"{pair:<{width}}  " + "  ".join(f"{c:>8}" for c in cells) + note)
    for pair, failed in result["failures"].items():
        for src, reason in failed.items():
            print(f"{pair} {src}: {reason}", file=sys.stderr)
    full = result["matching_all"]
    print(f"\npairs matching all {len(names)} probe functions from all {len(args.sources)} sources: {', '.join(full) or 'none'}"
          f" (linker model {result['linker_model']})")
    record = {
        "sources": {s.as_posix(): hashlib.sha256(s.read_bytes()).hexdigest() for s in args.sources},
        "grid": grid, "image_sha256": ctx.image.sha256,
        **{k: result[k] for k in ("results", "failures", "matching_all", "linker_model", "linker_rewrites")},
        **{key: result[key] for key in ("linker_section_rewrites", "removed_special_members", "comparison_inputs")
           if key in result},
    }
    args.out.mkdir(parents=True, exist_ok=True)
    (args.out / "compid.json").write_text(json.dumps(record, indent=1, sort_keys=True) + "\n", encoding="utf-8", newline="\n")
    return 0 if full else 1


if __name__ == "__main__":
    raise SystemExit(main())
