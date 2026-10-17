"""Collect only still-uncollected rows after concrete names/header revisions.

No completed author search or historical source/receipt is restarted. Each run
is exclusive and retains every refusal. This never changes the candidate
manifest automatically: new units still require canonical integration.
"""
from __future__ import annotations

import argparse
import collections
import hashlib
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[3]
OUT = Path(__file__).resolve().parent
PYTHON = ROOT.parents[2] / ".venv/bin/python"


def save(path: Path, value: object) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("x", encoding="utf-8") as stream:
        json.dump(value, stream, indent=2)
        stream.write("\n")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--run", required=True)
    args = parser.parse_args()
    directory = OUT / args.run
    directory.mkdir(parents=True, exist_ok=False)
    scope = json.loads((OUT / "SCOPE.json").read_text())
    selected = {row["function"]: row for row in scope["selected"]}
    for supplement in sorted(OUT.glob("SCOPE-SUPPLEMENT-*.json")):
        for row in json.loads(supplement.read_text())["rows"]:
            if row["receipt_linker_model"] == "armlink5-inline-branchnop-v2" and row["receipt_literal_model"] == "none":
                selected[row["function"]] = row
    source_overrides = {}
    for override_file in sorted(OUT.glob("SOURCE-OVERRIDES-*.json")):
        source_overrides.update(json.loads(override_file.read_text())["functions"])
    excluded = set(scope["excluded_functions"])
    excluded.update({
        "func_0016b6a4", "func_001b7110", "func_001b6df4", "func_001a2fc4",
        "func_0015e60c", "func_001f59b8", "func_001c5a08", "func_00164b14",
        "func_001ba338", "func_001bda14", "func_0010cc94", "func_00203810",
    })
    for manifest in (ROOT / "config/candidates").glob("c00*.json"):
        excluded.update(function for unit in json.loads(manifest.read_text())["units"] for function in unit["functions"])
    excluded.update(function for unit in json.loads((ROOT / "config/candidates/c010.json").read_text())["units"] for function in unit["functions"])
    excluded.update(row["function"] for row in json.loads((OUT / "contract-exclusions-r1.json").read_text()))
    for deferrals in OUT.glob("contract-deferrals-*.json"):
        excluded.update(row["function"] for row in json.loads(deferrals.read_text()))
    closure = json.loads((ROOT / "scratch/integrator/c009/collection-closure-r1.json").read_text())
    excluded.update(row["function"] for row in closure["rows"] if row["classification"].startswith("deferred"))
    parent_interface_deferrals = ROOT / "scratch/integrator/c009/interface-deferrals-r1.json"
    if parent_interface_deferrals.exists():
        excluded.update(row["function"] for row in json.loads(parent_interface_deferrals.read_text())["rows"])
    grouped: dict[str, dict] = {}
    for evidence in selected.values():
        if evidence["function"] in excluded:
            continue
        source = Path(evidence["source"])
        if hashlib.sha256(source.read_bytes()).hexdigest() != evidence["recorded_source_sha256"]:
            raise ValueError(f"Bound original author source changed: {source}")
        original_source = source
        if evidence["function"] in source_overrides:
            override = source_overrides[evidence["function"]]
            if override["original_source"] != str(source) or override["original_source_sha256"] != evidence["recorded_source_sha256"]:
                raise ValueError(f"Integration variant lost original provenance: {source}")
            source = Path(override["source"])
            if hashlib.sha256(source.read_bytes()).hexdigest() != override["source_sha256"]:
                raise ValueError(f"Integration variant changed after declaration cutover: {source}")
        entry = grouped.setdefault(str(source), {"agent": evidence.get("author", evidence.get("agent")), "functions": []})
        entry["functions"].append({"address": f'0x{int(evidence["function"][5:], 16):08x}', "status": "match", "source": str(source),
                                    "profile": evidence["receipt_profile"], "receipt": evidence["receipt"], "origin_record": evidence.get("record"),
                                    "original_source": str(original_source), "integration_variant_requires_fresh_match": source != original_source})
    svc = json.loads((OUT / "svc-input.json").read_text())
    if "func_0019ecf8" not in excluded:
        grouped[svc["functions"][0]["source"]] = svc
    partitions: dict[int, list[dict]] = collections.defaultdict(list)
    counts: collections.Counter[str] = collections.Counter()
    for source, entry in sorted(grouped.items()):
        filename = Path(source).name
        partition = counts[filename]
        counts[filename] += 1
        partitions[partition].append(entry)
    all_units = []
    runs = []
    for part, entries in sorted(partitions.items()):
        inputs = []
        for index, entry in enumerate(entries):
            path = directory / "inputs" / f"part{part}-{index:04d}.json"
            save(path, entry)
            inputs.append(str(path.relative_to(ROOT)))
        output = directory / f"candidates-part{part}.json"
        destination = f"src/checkpoint10/{args.run}" + (f"-alternate{part}" if part else "")
        command = [str(PYTHON), "tools/collect.py", "--dest", destination,
                   "--candidates", str(output.relative_to(ROOT)), *inputs]
        proc = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
        run = {"command": command, "exit_code": proc.returncode, "stdout": proc.stdout, "stderr": proc.stderr}
        save(directory / f"command-part{part}.json", run)
        runs.append(run)
        if output.exists():
            all_units.extend(json.loads(output.read_text())["units"])
    save(directory / "collected.json", {"schema_version": 1, "units": all_units})
    summary = {"input_units": len(grouped), "input_functions": sum(len(entry["functions"]) for entry in grouped.values()),
               "collected_units": len(all_units), "collected_functions": sum(len(unit["functions"]) for unit in all_units),
               "exits": [run["exit_code"] for run in runs],
               "headers_sha256": {str(path.relative_to(ROOT)): hashlib.sha256(path.read_bytes()).hexdigest()
                                  for path in (ROOT / "include").rglob("*") if path.is_file()},
               "names_sha256": hashlib.sha256((ROOT / "config/names.json").read_bytes()).hexdigest()}
    save(directory / "summary.json", summary)
    print(json.dumps({key: value for key, value in summary.items() if not key.endswith("sha256")}))
    return max((run["exit_code"] for run in runs), default=0)


if __name__ == "__main__":
    raise SystemExit(main())
