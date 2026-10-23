"""Reproduce one frozen source scope serially; never run acceptance gates."""
from __future__ import annotations

import argparse
import collections
import hashlib
import json
import logging
import subprocess
import sys
import time
from pathlib import Path
from typing import Any

LOGGER = logging.getLogger(__name__)
ROOT = Path(__file__).resolve().parents[4]


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def read_json(path: Path) -> dict[str, Any]:
    return json.loads(path.read_text(encoding="utf-8"))


def write_new(path: Path, value: dict[str, Any]) -> None:
    with path.open("x", encoding="utf-8", newline="\n") as stream:
        json.dump(value, stream, indent=2)
        stream.write("\n")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--scope", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    scope = read_json(args.scope)
    out = args.out
    out.mkdir(parents=True, exist_ok=False)
    bindings = {**scope["configuration"], **scope["headers"], **scope["source_hashes"]}
    drift = [path for path, digest in bindings.items()
             if not (ROOT / path).is_file() or sha256(ROOT / path) != digest]
    if drift:
        write_new(out / "INPUT-FAILURE.json", {"changed_or_missing_inputs": drift})
        LOGGER.error("Frozen inputs changed; no matcher command ran")
        return 2
    owner = {function: unit["source"] for unit in scope["units"]
             for function in unit["functions"]}
    rows: list[dict[str, Any]] = []
    observed: dict[str, str] = {}
    supplied_helpers: set[str] = set()
    for index, unit in enumerate(scope["units"]):
        if not unit["functions"]:
            continue
        destination = out / f"u{index:04d}"
        command = ["nice", "-n", "10", sys.executable, "tools/match.py",
                   unit["source"], "--profile", unit["profile"],
                   "--linker-model", scope["selection"]["linker_model"],
                   "--literal-model", scope["selection"]["literal_model"],
                   "--show", "0", "--out", str(destination)]
        for extra in unit.get("extra_sources", []):
            command.extend(["--extra-source", extra])
            supplied_helpers.add(extra)
        start = time.monotonic()
        process = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
        receipt = destination / (Path(unit["source"]).stem + ".result.json")
        result = read_json(receipt) if receipt.is_file() else {}
        functions = result.get("functions", [])
        statuses = {f"func_{entry['address']:08x}": entry["status"]
                    for entry in functions}
        duplicate_owners = [{"function": function, "owner": owner[function]}
                            for function in statuses
                            if function in owner and owner[function] != unit["source"]]
        claimed_match = all(statuses.get(function) == "match"
                            for function in unit["functions"])
        full_match = bool(functions) and all(entry["status"] == "match"
                                             for entry in functions)
        for function in unit["functions"]:
            if statuses.get(function) == "match":
                observed[function] = unit["source"]
        row = {"source": unit["source"], "profile": unit["profile"],
               "functions": unit["functions"], "extra_sources": unit.get("extra_sources", []),
               "command": command, "exit_code": process.returncode,
               "seconds": round(time.monotonic() - start, 6),
               "source_sha256": scope["source_hashes"][unit["source"]],
               "extra_source_sha256": {extra: scope["source_hashes"][extra]
                                       for extra in unit.get("extra_sources", [])},
               "credited_functions_match": claimed_match,
               "all_emitted_functions_match": full_match,
               "duplicate_credited_owners": duplicate_owners,
               "receipt": str(receipt.relative_to(ROOT)) if receipt.is_file() else None,
               "receipt_sha256": sha256(receipt) if receipt.is_file() else None,
               "output": process.stdout, "stderr": process.stderr}
        destination.mkdir(parents=True, exist_ok=True)
        write_new(destination / "RUN.json", row)
        rows.append(row)
        if process.returncode or not claimed_match or not full_match or duplicate_owners:
            LOGGER.error("Nonpassing source %s (exit %s)", unit["source"], process.returncode)
    complete = (set(observed) == set(owner)
                and supplied_helpers == set(scope["helper_only_units"]))
    passed = complete and all(row["exit_code"] == 0
                              and row["credited_functions_match"]
                              and row["all_emitted_functions_match"]
                              and not row["duplicate_credited_owners"] for row in rows)
    summary = {"schema_version": 1, "selection": scope["selection"],
               "scope": str(args.scope), "scope_sha256": sha256(args.scope),
               "script_sha256": sha256(Path(__file__)), "expected_units": len(rows),
               "completed_units": len(rows), "required_functions": len(owner),
               "matched_functions": len(observed), "scope_complete": complete,
               "helper_only_units": sorted(supplied_helpers),
               "exit_counts": dict(collections.Counter(str(row["exit_code"]) for row in rows)),
               "result": "PASS" if passed else "FAIL", "rows": rows}
    write_new(out / "results.json", summary)
    LOGGER.info("%s: %s commands, %s/%s required functions, complete=%s",
                summary["result"], len(rows), len(observed), len(owner), complete)
    return 0 if passed else 1


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format="%(levelname)s: %(message)s")
    raise SystemExit(main())
