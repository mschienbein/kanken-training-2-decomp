"""Sequential nice-able per-unit MATCH evidence, with a clean pause boundary.

No whole image is linked. Every invocation has --show 0 and explicit frozen
v2/none choices; all exits and metadata-only receipts are retained.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[3]
OUT = Path(__file__).resolve().parent
PYTHON = ROOT.parents[2] / ".venv/bin/python"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--run", required=True)
    parser.add_argument("--scope", type=Path, required=True)
    args = parser.parse_args()
    directory = OUT / args.run
    directory.mkdir(parents=True, exist_ok=False)
    units = json.loads((ROOT / args.scope).read_text())["units"]
    rows = []
    paused = False
    for unit in units:
        if (OUT / "PAUSE").exists():
            paused = True
            break
        source = unit["source"]
        output = directory / source.removeprefix("src/").rsplit(".", 1)[0]
        command = [str(PYTHON), "tools/match.py", source, "--profile", unit["profile"],
                   "--linker-model", "armlink5-inline-branchnop-v2", "--literal-model", "none",
                   "--show", "0", "--out", str(output)]
        proc = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
        receipt = output / (Path(source).stem + ".result.json")
        record = json.loads(receipt.read_text()) if receipt.exists() else None
        actual = {f'func_{row["address"]:08x}': row["status"] for row in record["functions"]} if record else {}
        complete = bool(record) and all(actual.get(function) == "match" for function in unit["functions"])
        row = dict(unit, command=command, exit_code=proc.returncode, credited_functions_match=complete,
                   output=proc.stdout + proc.stderr,
                   receipt=str(receipt.relative_to(ROOT)) if receipt.exists() else None,
                   source_sha256=hashlib.sha256((ROOT / source).read_bytes()).hexdigest())
        rows.append(row)
        with (directory / "exits.jsonl").open("a") as stream:
            stream.write(json.dumps(row) + "\n")
    summary = {"schema_version": 1, "scope": str(args.scope), "scope_sha256": hashlib.sha256((ROOT / args.scope).read_bytes()).hexdigest(),
               "selection": {"linker_model": "armlink5-inline-branchnop-v2", "literal_model": "none"},
               "expected_units": len(units), "completed_units": len(rows), "paused": paused,
               "match_units": sum(row["exit_code"] == 0 and row["credited_functions_match"] for row in rows),
               "exit_counts": {str(code): sum(row["exit_code"] == code for row in rows) for code in sorted({row["exit_code"] for row in rows})},
               "rows": rows}
    with (directory / "results.json").open("x") as stream:
        json.dump(summary, stream, indent=2)
        stream.write("\n")
    print(json.dumps({key: value for key, value in summary.items() if key != "rows"}))
    if paused:
        return 75
    return int(any(row["exit_code"] != 0 or not row["credited_functions_match"] for row in rows))


if __name__ == "__main__":
    raise SystemExit(main())
