"""Sequential bounded v8/L1 witnesses with immutable attempts and safe projections.

Only supplied source contexts are compiled. Raw compiler/linker artifacts remain
private; published metadata omits generated instruction words and image contents.
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
SELECTION = {
    "linker_model": "armlink5-inline-branchnop-v8",
    "literal_model": "anonymous-string-records-v1",
}


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def write_new(path: Path, value: dict) -> None:
    with path.open("x", encoding="utf-8") as stream:
        json.dump(value, stream, indent=2)
        stream.write("\n")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--scope", type=Path, required=True)
    parser.add_argument("--run", required=True)
    args = parser.parse_args()
    scope_path = ROOT / args.scope
    scope = json.loads(scope_path.read_text())
    if scope["selection"] != SELECTION:
        raise SystemExit("Scope must explicitly select the frozen v8/L1 models")
    directory = OUT / "runs" / args.run
    directory.mkdir(parents=True, exist_ok=False)
    rows, projected = [], []
    for unit in scope["units"]:
        source = unit["source"]
        output = directory / source.removeprefix("src/").rsplit(".", 1)[0]
        command = [sys.executable, "tools/match.py", source, "--profile", unit["profile"],
                   "--linker-model", SELECTION["linker_model"],
                   "--literal-model", SELECTION["literal_model"], "--show", "0",
                   "--out", str(output.relative_to(ROOT))]
        for extra in unit.get("extra_sources", []):
            command.extend(["--extra-source", extra])
        proc = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
        output.mkdir(parents=True, exist_ok=True)
        log_path = output / "command.log"
        with log_path.open("x", encoding="utf-8") as stream:
            stream.write(proc.stdout + proc.stderr)
        receipt = output / (Path(source).stem + ".result.json")
        record = json.loads(receipt.read_text()) if receipt.is_file() else {}
        observed = [{key: function.get(key) for key in
                     ("name", "address", "size", "candidate_size", "status", "first_diff")}
                    for function in record.get("functions", [])]
        actual = {f'func_{function["address"]:08x}': function["status"]
                  for function in record.get("functions", [])}
        complete = bool(record) and all(actual.get(fn) == "match" for fn in unit["functions"])
        row = {**unit, "command": command, "exit_code": proc.returncode,
               "all_credited_targets_match": complete, "observed": observed,
               "source_sha256": sha256(ROOT / source),
               "extra_source_sha256": {extra: sha256(ROOT / extra)
                                       for extra in unit.get("extra_sources", [])},
               "receipt": str(receipt.relative_to(ROOT)) if receipt.is_file() else None,
               "receipt_sha256": sha256(receipt) if receipt.is_file() else None,
               "log": str(log_path.relative_to(ROOT)), "log_sha256": sha256(log_path),
               "object_sha256": record.get("object_sha256"),
               "compiled_extra_sources": record.get("extra_sources", [])}
        rows.append(row)
        with (directory / "exits.jsonl").open("a", encoding="utf-8") as stream:
            stream.write(json.dumps(row) + "\n")
        if proc.returncode == 0 and complete:
            projected.append({
                "source": source, "profile": unit["profile"], "functions": unit["functions"],
                "source_sha256": row["source_sha256"],
                "extra_source_sha256": row["extra_source_sha256"],
                "receipt": row["receipt"], "receipt_sha256": row["receipt_sha256"],
                "linker_rewrites": {
                    f'func_{function["address"]:08x}': [
                        {key: rewrite[key] for key in ("offset", "rule", "target", "removed_callee")
                         if key in rewrite}
                        for rewrite in function.get("linker_rewrites", [])]
                    for function in record.get("functions", [])
                    if function.get("linker_rewrites")},
                "literal_rebindings": record.get("literal_rebindings", []),
                "removed_callees": record.get("removed_callees", []),
            })
    summary = {
        "schema_version": 1, "selection": SELECTION,
        "scope": str(args.scope), "scope_sha256": sha256(scope_path),
        "runner_sha256": sha256(Path(__file__)), "expected_units": len(scope["units"]),
        "completed_units": len(rows),
        "exit_counts": {str(code): sum(row["exit_code"] == code for row in rows)
                        for code in sorted({row["exit_code"] for row in rows})},
        "all_credited_targets_match": all(row["all_credited_targets_match"] for row in rows),
        "rows": rows,
    }
    write_new(directory / "results.json", summary)
    write_new(directory / "linker-rewrites.json", {
        "schema_version": 1, "selection": SELECTION, "scope": str(args.scope),
        "scope_sha256": summary["scope_sha256"], "scope_complete": len(projected) == len(rows),
        "basis": "Only successful fresh receipts from this run; supported units[].linker_rewrites. Not a sweep index. Generated instruction-word fields omitted.",
        "units": projected,
    })
    print(json.dumps({key: value for key, value in summary.items() if key != "rows"}))
    return int(any(row["exit_code"] != 0 or not row["all_credited_targets_match"] for row in rows))


if __name__ == "__main__":
    raise SystemExit(main())
