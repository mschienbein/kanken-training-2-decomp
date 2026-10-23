"""Reprove frozen genuine source groups with explicit canonical function owners."""
from __future__ import annotations

import argparse
import collections
import logging
import subprocess
import sys
import time
from pathlib import Path

from reprove_closure import ROOT, read_json, sha256, write_new

LOGGER = logging.getLogger(__name__)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--scope", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    scope = read_json(args.scope)
    out = args.out
    out.mkdir(parents=True, exist_ok=False)
    bindings = {**scope["configuration"], **scope["headers"], **scope["source_hashes"],
                scope["ledger"]: scope["ledger_sha256"]}
    drift = [path for path, digest in bindings.items()
             if not (ROOT / path).is_file() or sha256(ROOT / path) != digest]
    if drift:
        write_new(out / "INPUT-FAILURE.json", {"changed_or_missing_inputs": drift})
        LOGGER.error("Frozen inputs changed; no matcher command ran")
        return 2
    units = {unit["source"]: unit for unit in scope["units"]}
    owner = {function: unit["source"] for unit in scope["units"]
             for function in unit["functions"]}
    helpers = set(scope["helper_only_units"])
    contexts = scope["contexts"]
    counts = collections.Counter(function for context in contexts
                                 for function in context["functions"])
    errors = []
    if set(counts) != set(owner) or any(count != 1 for count in counts.values()):
        errors.append("Every credited function must occur in exactly one context")
    for context in contexts:
        sources = [context["source"], *context.get("extra_sources", [])]
        if len(set(sources)) != len(sources) or any(source not in units for source in sources):
            errors.append(f"Unknown or repeated source in {context['source']}")
            continue
        if any(units[source]["profile"] != context["profile"] for source in sources):
            errors.append(f"Mixed profiles in {context['source']}")
        expected = {function for source in sources for function in units[source]["functions"]}
        if set(context["functions"]) != expected:
            errors.append(f"A compiled credited source lacks its complete claims: {context['source']}")
    if errors:
        write_new(out / "CONTEXT-FAILURE.json", {"errors": errors})
        LOGGER.error("Invalid natural-context ownership; no matcher command ran")
        return 2
    rows = []
    observed = {}
    supplied_helpers = set()
    for index, context in enumerate(contexts):
        destination = out / f"u{index:04d}"
        extras = context.get("extra_sources", [])
        source_set = {context["source"], *extras}
        command = ["nice", "-n", "10", sys.executable, "tools/match.py",
                   context["source"], "--profile", context["profile"],
                   "--linker-model", scope["selection"]["linker_model"],
                   "--literal-model", scope["selection"]["literal_model"],
                   "--show", "0", "--out", str(destination)]
        for extra in extras:
            command.extend(["--extra-source", extra])
            if extra in helpers:
                supplied_helpers.add(extra)
        started = time.monotonic()
        process = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
        receipt = destination / (Path(context["source"]).stem + ".result.json")
        result = read_json(receipt) if receipt.is_file() else {}
        functions = result.get("functions", [])
        statuses = {f"func_{entry['address']:08x}": entry["status"] for entry in functions}
        foreign_owners = [{"function": function, "owner": owner[function]}
                          for function in statuses if function in owner
                          and (owner[function] not in source_set
                               or function not in context["functions"])]
        claimed_match = all(statuses.get(function) == "match" for function in context["functions"])
        full_match = bool(functions) and all(entry["status"] == "match" for entry in functions)
        for function in context["functions"]:
            if statuses.get(function) == "match":
                observed[function] = owner[function]
        row = {"source": context["source"], "profile": context["profile"],
               "functions": context["functions"], "extra_sources": extras,
               "function_owners": {function: owner[function] for function in context["functions"]},
               "command": command, "exit_code": process.returncode,
               "seconds": round(time.monotonic() - started, 6),
               "source_sha256": scope["source_hashes"][context["source"]],
               "extra_source_sha256": {extra: scope["source_hashes"][extra] for extra in extras},
               "credited_functions_match": claimed_match,
               "all_emitted_functions_match": full_match,
               "foreign_credited_owners": foreign_owners,
               "receipt": str(receipt.relative_to(ROOT)) if receipt.is_file() else None,
               "receipt_sha256": sha256(receipt) if receipt.is_file() else None,
               "output": process.stdout, "stderr": process.stderr}
        destination.mkdir(parents=True, exist_ok=True)
        write_new(destination / "RUN.json", row)
        rows.append(row)
        if process.returncode or not claimed_match or not full_match or foreign_owners:
            LOGGER.error("Nonpassing context %s (exit %s)", context["source"], process.returncode)
    complete = observed == owner and supplied_helpers == helpers
    passed = complete and all(row["exit_code"] == 0 and row["credited_functions_match"]
                              and row["all_emitted_functions_match"]
                              and not row["foreign_credited_owners"] for row in rows)
    summary = {"schema_version": 1, "selection": scope["selection"],
               "scope": str(args.scope), "scope_sha256": sha256(args.scope),
               "script_sha256": sha256(Path(__file__)),
               "script_dependencies": {"reprove_closure.py": sha256(Path(__file__).with_name("reprove_closure.py"))},
               "expected_units": len(contexts), "completed_units": len(rows),
               "ledger_units": len(units), "required_functions": len(owner),
               "matched_functions": len(observed), "scope_complete": complete,
               "canonical_function_owners": owner,
               "helper_only_units": sorted(supplied_helpers),
               "exit_counts": dict(collections.Counter(str(row["exit_code"]) for row in rows)),
               "result": "PASS" if passed else "FAIL", "rows": rows}
    write_new(out / "results.json", summary)
    LOGGER.info("%s: %s contexts, %s/%s required functions, complete=%s",
                summary["result"], len(rows), len(observed), len(owner), complete)
    return 0 if passed else 1


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format="%(levelname)s: %(message)s")
    raise SystemExit(main())
