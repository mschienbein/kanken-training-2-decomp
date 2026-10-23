"""Collect author results into checkpoint candidates (integrator).

    python tools/collect.py --dest src/wave1 --candidates config/candidates/c002-wave1.json \
        scratch/authors/*/RESULT.json

For every RESULT.json row with status "match": copy the recorded source into --dest, recompile
the copy there (so includes resolve as in the build) under its recorded profile, and compare in
place (tools/match.py's oracle). Only functions that re-derive as MATCH are credited; a source
whose credited functions don't all re-derive is removed again and reported, as are name clashes,
non-creditable catalogue kinds, and functions already accepted or claimed twice. An accepted
function may be explicitly selected with --rehome func_XXXXXXXX; the candidates then record its
old/new unit (the integrator must remove its old definition before prepare). Writes the candidates
file and prints what was taken and what was refused.

Explicit --linker-model/--literal-model choices use the matcher's oracle; prepare must select
them again to freeze the checkpoint. Repeatable --extra-source helpers are copied and compiled
at their destination under the callers' single shared profile, then retained as zero-credit units.
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


def accepted_sources() -> dict[str, str]:
    ledger = json.loads((ROOT / "config/matches.json").read_text(encoding="utf-8"))
    return {fn: unit["source"] for unit in ledger["units"] for fn in unit["functions"]}


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("results", type=Path, nargs="+")
    parser.add_argument("--dest", type=Path, required=True)
    parser.add_argument("--candidates", type=Path, required=True)
    parser.add_argument("--rehome", action="append", default=[], metavar="FUNCTION",
                        help="allow this accepted func_XXXXXXXX to move into a collected unit (repeatable)")
    parser.add_argument("--linker-model", default=inplace.DEFAULT_LINKER_MODEL, choices=sorted(inplace.LINKER_MODELS),
                        help="armlink model used to re-derive candidates (prepare must select it explicitly)")
    parser.add_argument("--literal-model", default=inplace.DEFAULT_LITERAL_MODEL, choices=inplace.LITERAL_MODELS,
                        help="independent anonymous string placement rule; default off")
    parser.add_argument("--extra-source", type=Path, action="append", default=[],
                        help="same-build helper copied with all callers, which must share one profile (repeatable)")
    args = parser.parse_args(argv)
    if inplace.linker_model(args.linker_model).unreferenced_destructors:
        print("error: collection refuses U1 linker models without a per-comparison section-action receipt; "
              "use match.py/build.py for explicit U1 evidence", file=sys.stderr)
        return 2
    if args.candidates.exists():
        print(f"error: {args.candidates} exists", file=sys.stderr)
        return 1
    profiles = toolchain.load_profiles()
    ctx = match.Context(ROOT / "config/catalogue.json", model=args.linker_model, literal_model=args.literal_model)
    accepted = accepted_sources()
    requested_moves = set(args.rehome)
    unknown = sorted(requested_moves - accepted.keys())
    if unknown:
        print(f"error: --rehome functions are not accepted: {', '.join(unknown)}", file=sys.stderr)
        return 1
    taken = accepted.keys() - requested_moves
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

    units, moves, refused = [], [], []
    dest = ROOT / args.dest
    extras = list(dict.fromkeys((ROOT / path).resolve() for path in args.extra_source))
    extra_targets = [dest / path.name for path in extras]
    helper_profile = None
    if extras:
        profile_names = {entry["profile"] for entry in by_source.values() if entry["claimed"]}
        if len(profile_names) != 1 or not profile_names <= profiles.keys():
            print("error: --extra-source callers must share one known profile", file=sys.stderr)
            return 1
        helper_profile = profiles[next(iter(profile_names))]
        names = [path.name for path in extras]
        if len(names) != len(set(names)) or set(names) & {path.name for path in by_source}:
            print("error: --extra-source destination name clashes with another input", file=sys.stderr)
            return 1
        for source, target in zip(extras, extra_targets):
            if not source.is_file() or target.exists():
                print(f"error: helper source missing or destination exists: {source} -> {target}", file=sys.stderr)
                return 1
    with tempfile.TemporaryDirectory() as tmp:
        extra_objects = []
        try:
            for index, (source, target) in enumerate(zip(extras, extra_targets)):
                target.parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(source, target)
                obj = Path(tmp) / f"extra-{index}-{source.stem}.o"
                toolchain.compile_source(helper_profile, target, obj, cache=False)
                extra_objects.append(obj)
        except toolchain.ToolchainError as e:
            for target in extra_targets:
                target.unlink(missing_ok=True)
            print(f"error: helper compilation refused: {str(e).splitlines()[0]}", file=sys.stderr)
            return 1
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
                results = ctx.compare(obj, extra_objects) if extra_objects else ctx.compare(obj)
                rows = {f"func_{r['address']:08x}": r["status"] for r in results}
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
            moves.extend({"function": fn, "from": accepted[fn], "to": target.relative_to(ROOT).as_posix()}
                         for fn in sorted(entry["claimed"] & requested_moves))
    if units:
        units.extend({"source": target.relative_to(ROOT).as_posix(), "profile": helper_profile.name,
                      "functions": []} for target in extra_targets)
    else:
        for target in extra_targets:
            target.unlink()
    record = {"schema_version": 1, "note": f"Collected from {len(args.results)} author results.", "units": units}
    if moves:
        record["moves"] = sorted(moves, key=lambda m: m["function"])
    args.candidates.parent.mkdir(parents=True, exist_ok=True)
    args.candidates.write_text(json.dumps(record, indent=1, ensure_ascii=False) + "\n", encoding="utf-8", newline="\n")
    print(f"taken: {len(units)} units, {sum(len(u['functions']) for u in units)} functions -> {args.candidates}")
    for src, why in refused:
        print(f"refused: {src.relative_to(ROOT) if src.is_relative_to(ROOT) else src}: {why}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
