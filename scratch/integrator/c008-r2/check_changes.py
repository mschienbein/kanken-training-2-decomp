"""Run only the assigned serial unit/interface/caller checks, never acceptance.

Each invocation gets a new evidence directory. Match output always uses --show 0.
The dry ledger is derived from the immutable c008 ledger, never config/matches.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]
OUT = Path(__file__).resolve().parent
MAIN_PYTHON = Path("/home/michael/code/kanken-training-2-decomp/.venv/bin/python")
MODELS = ["--linker-model", "armlink5-inline-branchnop-v2", "--literal-model", "none"]


def save(path: Path, value: object) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("x", encoding="utf-8") as stream:
        json.dump(value, stream, indent=2)
        stream.write("\n")


def execute(command: list[str], log: Path) -> dict:
    with log.open("x", encoding="utf-8") as stream:
        proc = subprocess.run(command, cwd=ROOT, text=True, stdout=stream, stderr=subprocess.STDOUT, timeout=300)
    return {"command": command, "exit": proc.returncode, "log": str(log.relative_to(ROOT))}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--run", required=True)
    parser.add_argument("--scope", choices=("direct", "c008"), default="direct")
    parser.add_argument("--step", choices=("matches", "interfaces", "argcheck", "witnesses"), required=True)
    args = parser.parse_args()
    directory = OUT / "checks" / args.run
    directory.mkdir(parents=True, exist_ok=False)
    ledger = json.loads((OUT / "dry-matches.json").read_text())
    prepared = json.loads((ROOT / "accept/c008/prepare.json").read_text())
    changed = subprocess.run(["git", "diff", "--name-only", "8bf3fe821c2172b94522fb334e7a8c5aa5512173", "--", "src", "include"], cwd=ROOT, text=True, capture_output=True, check=True).stdout.splitlines()
    required = {path for path in changed if path.startswith("src/") and (ROOT / path).exists()}
    if args.scope == "c008":
        required |= (set(prepared["new_units"]) | set(prepared["revised_units"])) - {"src/wave12/func_0016b6a4.c"}
    units = [unit for unit in ledger["units"] if unit["source"] in required]
    rows = []
    if args.step == "matches":
        for unit in units:
            subdir = directory / Path(unit["source"]).with_suffix("")
            subdir.mkdir(parents=True)
            command = [str(MAIN_PYTHON), "tools/match.py", unit["source"], "--profile", unit["profile"], "--out", str((subdir / "match").relative_to(ROOT)), "--show", "0", *MODELS]
            row = dict(unit, source_sha256=hashlib.sha256((ROOT / unit["source"]).read_bytes()).hexdigest(), **execute(command, subdir / "command.log"))
            result_path = subdir / "match" / (Path(unit["source"]).stem + ".result.json")
            if result_path.is_file():
                row["receipt"] = str(result_path.relative_to(ROOT))
                row["receipt_sha256"] = hashlib.sha256(result_path.read_bytes()).hexdigest()
            rows.append(row)
            with (directory / "exits.jsonl").open("a", encoding="utf-8") as stream:
                stream.write(json.dumps(row) + "\n")
    elif args.step == "witnesses":
        sys.path.insert(0, str(OUT))
        from probe_contracts import compile_probe, toolchain
        compiled = compile_probe(OUT / "bridges.cpp", toolchain.load_profiles()["game"], directory)
        compiled["exit"] = compiled.pop("compiler_exit")
        compiled["log"] = compiled["diagnostic"]
        rows.append(compiled)
        command = [str(MAIN_PYTHON), "tools/match.py", str((OUT / "witnesses/func_001821f4.c").relative_to(ROOT)), "--profile", "game", "--out", str((directory / "caller-match").relative_to(ROOT)), "--show", "0", *MODELS]
        rows.append(execute(command, directory / "caller-command.log"))
    elif args.step == "interfaces":
        command = [str(MAIN_PYTHON), "tools/interfaces.py", "check", "--matches", str((OUT / "dry-matches.json").relative_to(ROOT)), "--allow-unchecked-cpp", "--json", str((directory / "interfaces.json").relative_to(ROOT))]
        rows.append(execute(command, directory / "command.log"))
    else:
        functions = [name for unit in units for name in unit["functions"]]
        command = [str(MAIN_PYTHON), "tools/argcheck.py", *functions, "--matches", str((OUT / "dry-matches.json").relative_to(ROOT)), "--rewrites", str((OUT / "dry-rewrites.json").relative_to(ROOT)), "--json", str((directory / "argcheck.json").relative_to(ROOT))]
        rows.append(execute(command, directory / "command.log"))
    save(directory / "results.json", {"step": args.step, "scope": args.scope, "changed_files": changed, "rows": rows})
    print(json.dumps({"step": args.step, "commands": len(rows), "failed": [{"source": row.get("source"), "exit": row["exit"], "log": row["log"]} for row in rows if row["exit"]]}))
    return int(any(row["exit"] for row in rows))


if __name__ == "__main__":
    raise SystemExit(main())
