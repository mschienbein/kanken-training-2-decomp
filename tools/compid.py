"""Compiler identification: compile probe sources under a grid of compiler builds x flag sets.

    python tools/compid.py probe.c [more.c ...] [--grid grid.json] [--catalogue PATH] [--out DIR]

Each probe defines functions named func_XXXXXXXX whose C was written to reproduce the original.
Every (compiler, flag set) pair is compiled and linked in place (tools/match.py's oracle); the
table shows which pairs reproduce which functions byte for byte. A pair that matches every probe
is a candidate profile; evidence is written to <out>/compid.json. Probes must be reviewed C, not
tuned per compiler: the point is to discriminate builds, so one source text is used for all pairs.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
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


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("sources", type=Path, nargs="+")
    parser.add_argument("--grid", type=Path, help='JSON {"compilers": [...], "flagsets": {name: [flags]}}')
    parser.add_argument("--catalogue", type=Path, default=ROOT / "config/catalogue.json")
    parser.add_argument("--out", type=Path, default=ROOT / "scratch/compid")
    args = parser.parse_args(argv)
    grid = json.loads(args.grid.read_text()) if args.grid else DEFAULT_GRID
    ctx = match.Context(args.catalogue)

    rows: dict[str, dict[str, str]] = {}
    pairs = []
    for compiler in grid["compilers"]:
        for fname, flags in grid["flagsets"].items():
            pair = f"{compiler}:{fname}"
            pairs.append(pair)
            profile = toolchain.Profile(pair, compiler, tuple(flags))
            for src in args.sources:
                obj = args.out / "obj" / compiler / fname / (src.stem + ".o")
                try:
                    toolchain.compile_source(profile, src.resolve(), obj)
                    results = ctx.compare(obj)
                except (toolchain.ToolchainError, inplace.LinkError) as e:
                    print(f"{pair} {src.name}: {e}".splitlines()[0], file=sys.stderr)
                    continue
                for r in results:
                    rows.setdefault(r["name"], {})[pair] = "Y" if r["status"] == "match" else f"+{r['first_diff']:x}"

    width = max(len(p) for p in pairs)
    names = sorted(rows)
    print(" " * width + "  " + "  ".join(n[-8:] for n in names))
    full = []
    for pair in pairs:
        cells = [rows[n].get(pair, "err") for n in names]
        if all(c == "Y" for c in cells):
            full.append(pair)
        print(f"{pair:<{width}}  " + "  ".join(f"{c:>8}" for c in cells))
    print(f"\npairs matching all {len(names)} probe functions: {', '.join(full) or 'none'}")
    record = {
        "sources": {s.as_posix(): hashlib.sha256(s.read_bytes()).hexdigest() for s in args.sources},
        "grid": grid, "image_sha256": ctx.image.sha256, "results": rows, "matching_all": full,
    }
    args.out.mkdir(parents=True, exist_ok=True)
    (args.out / "compid.json").write_text(json.dumps(record, indent=1, sort_keys=True) + "\n")
    return 0 if full else 1


if __name__ == "__main__":
    raise SystemExit(main())
