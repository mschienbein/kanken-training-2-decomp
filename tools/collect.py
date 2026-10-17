"""Collect author results into checkpoint candidates (integrator).

    python tools/collect.py --dest src/wave1 --candidates config/candidates/c002-wave1.json \
        scratch/authors/*/RESULT.json

For every RESULT.json row with status "match": copy the recorded source into --dest, recompile
the copy there (so includes resolve as in the build) under its recorded profile, and compare in
place (tools/match.py's oracle). Only functions that re-derive as MATCH are credited; a source
whose credited functions don't all re-derive is removed again and reported, as are name clashes,
non-creditable catalogue kinds, and functions already accepted or claimed twice. Writes the
candidates file and prints what was taken and what was refused.
"""
from __future__ import annotations

import argparse
import json
import shutil
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import inplace  # noqa: E402
import match  # noqa: E402
import toolchain  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]


def accepted_functions() -> set[str]:
    ledger = json.loads((ROOT / "config/matches.json").read_text(encoding="utf-8"))
    return {fn for unit in ledger["units"] for fn in unit["functions"]}


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("results", type=Path, nargs="+")
    parser.add_argument("--dest", type=Path, required=True)
    parser.add_argument("--candidates", type=Path, required=True)
    args = parser.parse_args(argv)
    if args.candidates.exists():
        print(f"error: {args.candidates} exists", file=sys.stderr)
        return 1
    profiles = toolchain.load_profiles()
    ctx = match.Context(ROOT / "config/catalogue.json")
    taken = accepted_functions()
    by_source: dict[Path, dict] = {}
    for result in args.results:
        data = json.loads(result.read_text(encoding="utf-8"))
        for row in data["functions"]:
            if row.get("status") != "match":
                continue
            src = (ROOT / row["source"]).resolve()
            entry = by_source.setdefault(src, {"profile": row.get("profile", "game"), "claimed": set(), "agent": data.get("agent")})
            if entry["profile"] != row.get("profile", "game"):
                print(f"refused {src}: rows disagree on the profile", file=sys.stderr)
                entry["claimed"] = None
            elif entry["claimed"] is not None:
                entry["claimed"].add(f"func_{int(row['address'], 16):08x}")

    units, refused = [], []
    dest = ROOT / args.dest
    with tempfile.TemporaryDirectory() as tmp:
        for src, entry in sorted(by_source.items()):
            if not entry["claimed"]:
                continue
            profile = profiles.get(entry["profile"])
            if profile is None:
                refused.append((src, f"unknown profile {entry['profile']}"))
                continue
            target = dest / src.name
            classified = sorted(
                n for n in entry["claimed"]
                if ctx.functions.get(int(n[5:], 16), {}).get("kind", "function") != "function"
            )
            clash = sorted(entry["claimed"] & taken)
            if classified:
                refused.append((src, f"catalogue kind is not creditable C: {', '.join(classified)}"))
                continue
            if clash:
                refused.append((src, f"already accepted or claimed: {', '.join(clash)}"))
                continue
            if target.exists():
                refused.append((src, f"{target.relative_to(ROOT)} exists"))
                continue
            # Re-derive from the copy at its destination, so the include context is the one the
            # build will use (a header next to the author's file is not there).
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(src, target)
            obj = Path(tmp) / (src.stem + ".o")
            try:
                toolchain.compile_source(profile, target, obj, cache=False)
                # Keyed by address: a function defined under a names-overlay name (a C++ mangled
                # constructor, say) is still claimed by its func_XXXXXXXX identity.
                rows = {f"func_{r['address']:08x}": r["status"] for r in ctx.compare(obj)}
            except (toolchain.ToolchainError, inplace.LinkError) as e:
                target.unlink()
                refused.append((src, str(e).splitlines()[0]))
                continue
            bad = sorted(n for n in entry["claimed"] if rows.get(n) != "match")
            if bad:
                target.unlink()
                refused.append((src, f"does not re-derive MATCH at {target.relative_to(ROOT)}: {', '.join(bad)}"))
                continue
            taken |= entry["claimed"]
            units.append({"source": target.relative_to(ROOT).as_posix(), "profile": entry["profile"],
                          "functions": sorted(entry["claimed"]), "author": entry["agent"]})
    record = {"schema_version": 1, "note": f"Collected from {len(args.results)} author results.", "units": units}
    args.candidates.parent.mkdir(parents=True, exist_ok=True)
    args.candidates.write_text(json.dumps(record, indent=1, ensure_ascii=False) + "\n", encoding="utf-8", newline="\n")
    print(f"taken: {len(units)} units, {sum(len(u['functions']) for u in units)} functions -> {args.candidates}")
    for src, why in refused:
        print(f"refused: {src.relative_to(ROOT) if src.is_relative_to(ROOT) else src}: {why}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
