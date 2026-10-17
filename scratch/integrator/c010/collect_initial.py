"""Collect the frozen c010 author-source snapshot without changing acceptance state.

All inputs remain at their original paths; the filtered manifests preserve source
and receipt provenance in SCOPE.json. Refusals are historical results, not success.
"""
from __future__ import annotations

import collections
import hashlib
import json
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]
OUT = Path(__file__).resolve().parent
PYTHON = ROOT.parents[2] / ".venv/bin/python"
CONTRACT_DEFERRALS = {
    "func_0016b6a4", "func_001b7110", "func_001b6df4", "func_001a2fc4",
    "func_0015e60c", "func_001f59b8", "func_001c5a08", "func_00164b14",
    "func_001ba338", "func_001bda14", "func_0010cc94", "func_00203810",
}


def save(path: Path, value: object) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("x", encoding="utf-8") as stream:
        json.dump(value, stream, indent=2)
        stream.write("\n")


def main() -> int:
    snapshot = json.loads((OUT / "SCOPE.json").read_text())
    groups: dict[str, dict] = {}
    deferred = []
    for evidence in snapshot["selected"]:
        function = evidence["function"]
        if function in CONTRACT_DEFERRALS:
            deferred.append({"function": function, "reason": "Preserve predecessor contract-resolution deferral; no replacement identity proof is claimed.",
                             "source": evidence["source"], "receipt": evidence["receipt"]})
            continue
        source = Path(evidence["source"])
        if hashlib.sha256(source.read_bytes()).hexdigest() != evidence["recorded_source_sha256"]:
            raise ValueError(f"Source changed after exact receipt binding: {source}")
        entry = groups.setdefault(str(source), {"source": str(source), "rows": [],
                                  "agent": evidence.get("author", evidence.get("agent"))})
        entry["rows"].append({"address": f"0x{int(function[5:], 16):08x}", "status": "match",
                              "source": str(source), "profile": evidence["receipt_profile"],
                              "receipt": evidence["receipt"], "origin_record": evidence.get("record")})
    counts: collections.Counter[str] = collections.Counter()
    parts: dict[int, list[dict]] = collections.defaultdict(list)
    for source, entry in sorted(groups.items()):
        name = Path(source).name
        part = counts[name]
        counts[name] += 1
        parts[part].append(entry)
    save(OUT / "contract-exclusions-r1.json", deferred)
    summaries = []
    all_units = []
    for part, entries in sorted(parts.items()):
        inputs = []
        for index, entry in enumerate(entries):
            path = OUT / "inputs-r1" / f"part{part}-{index:04d}.json"
            save(path, {"agent": entry["agent"], "functions": entry["rows"]})
            inputs.append(str(path.relative_to(ROOT)))
        candidate = OUT / f"collected-r1-part{part}.json"
        destination = "src/checkpoint10" if part == 0 else f"src/checkpoint10/alternate{part}"
        command = [str(PYTHON), "tools/collect.py", "--dest", destination,
                   "--candidates", str(candidate.relative_to(ROOT)), *inputs]
        proc = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
        summary = {"part": part, "command": command, "exit_code": proc.returncode,
                   "stdout": proc.stdout, "stderr": proc.stderr,
                   "input_units": len(entries), "input_functions": sum(len(e["rows"]) for e in entries)}
        if candidate.exists():
            collected = json.loads(candidate.read_text())
            all_units.extend(collected["units"])
            summary["collected_units"] = len(collected["units"])
            summary["collected_functions"] = sum(len(u["functions"]) for u in collected["units"])
        save(OUT / f"collect-r1-part{part}.json", summary)
        summaries.append(summary)
    save(OUT / "collected-r1.json", {"schema_version": 1, "note": "Unaccepted c010 destination collection; canonical integration and pre-review remain required.", "units": all_units})
    print(json.dumps({"parts": len(parts), "input_units": len(groups), "input_functions": sum(len(e["rows"]) for e in groups.values()),
                      "contract_exclusions": len(deferred), "collected_units": len(all_units), "collected_functions": sum(len(u["functions"]) for u in all_units),
                      "collect_exits": [s["exit_code"] for s in summaries]}))
    return max((s["exit_code"] for s in summaries), default=0)


if __name__ == "__main__":
    raise SystemExit(main())
