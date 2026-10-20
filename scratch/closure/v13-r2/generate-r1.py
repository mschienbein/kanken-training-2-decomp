"""One granted generation; release atomically, then enumerate every metadata delta."""
from __future__ import annotations

import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[3]
MAIN = ROOT.parents[2]
OUT = ROOT / "scratch/closure/v13-r2/generation"
PROPOSAL = ROOT / "scratch/analyze/catalogue-v13-r2-r1.json"
BASELINE = MAIN / "scratch/worktrees/catalogue-v13/scratch/analyze/catalogue-v13-r1.json"
BASELINE_SHA = "e333779b6f2272c70cc166f915aa925abbcb06641a11f62f4443368ab847e743"
PYTHON = "/home/michael/code/kanken-training-2-decomp/.venv/bin/python"


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def save(path: Path, value: dict | list) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2, sort_keys=True, ensure_ascii=False) + "\n")


def main() -> int:
    OUT.mkdir(parents=True, exist_ok=True)
    receipt_path = OUT / "generation-receipt-r1.json"
    release_path = OUT / "HEAVY-SLOT-RELEASED.json"
    if PROPOSAL.exists() or receipt_path.exists() or release_path.exists():
        print("Retained attempt exists; require a newly named and granted rN attempt.", file=sys.stderr)
        return 1
    baseline_bytes = BASELINE.read_bytes()
    if digest(baseline_bytes) != BASELINE_SHA:
        print("Sealed v13 baseline digest differs; no generator execution.", file=sys.stderr)
        return 1
    argv = [PYTHON, "-B", "tools/analyze.py", "catalogue", "--out", str(PROPOSAL)]
    source_sha256 = {f"tools/{name}": digest((ROOT / "tools" / name).read_bytes()) for name in (
        "analyze.py", "catalogue_closure.py", "catalogue_alignment.py", "catalogue_frozen.py",
        "catalogue_local_entries.py", "catalogue_runtime_data.py", "catalogue_non_c.py", "catalogue_review.py")}
    started = time.perf_counter()
    result = subprocess.run(argv, cwd=ROOT, capture_output=True)
    elapsed = time.perf_counter() - started
    # Slot release precedes output hashing, catalogue comparisons or adjudication.
    release = {"released": True, "actual_generator_exits": [result.returncode],
               "actual_executions": [{"argv": argv, "cwd": str(ROOT), "actual_exit": result.returncode,
                                      "seconds": elapsed}], "canonical_adoption": False}
    temporary = release_path.with_name(".HEAVY-SLOT-RELEASED-r1.tmp")
    save(temporary, release)
    os.replace(temporary, release_path)
    receipt = {"argv": argv, "cwd": str(ROOT), "actual_exit": result.returncode, "seconds": elapsed,
               "stdout": result.stdout.decode(), "stderr": result.stderr.decode(), "source_sha256": source_sha256,
               "generator_executions": 1, "full_suite": False, "canonical_adoption": False}
    save(receipt_path, receipt)
    if result.returncode:
        print(json.dumps({"actual_generator_exit": result.returncode, "released": True}))
        return result.returncode
    payload = PROPOSAL.read_bytes()
    before, after = json.loads(baseline_bytes), json.loads(payload)
    old_functions = {row["address"]: row for row in before["functions"]}
    new_functions = {row["address"]: row for row in after["functions"]}
    function_changes = []
    for address in sorted(old_functions.keys() | new_functions.keys()):
        old, new = old_functions.get(address), new_functions.get(address)
        if old != new:
            function_changes.append({"address": address, "before": old, "after": new})
    old_ranges = {(row["start"], row["end"]): row for row in before["ranges"]}
    new_ranges = {(row["start"], row["end"]): row for row in after["ranges"]}
    range_changes = []
    for interval in sorted(old_ranges.keys() | new_ranges.keys()):
        old, new = old_ranges.get(interval), new_ranges.get(interval)
        if old != new:
            range_changes.append({"interval": interval, "before": old, "after": new})
    missing = {"missing_record": True}
    metadata_changes = []

    def compare(left, right, path: str) -> None:
        if left == right:
            return
        if left is missing or right is missing:
            metadata_changes.append({"path": path, "before": left, "after": right})
            return
        if isinstance(left, dict) and isinstance(right, dict):
            for key in sorted(left.keys() | right.keys()):
                compare(left.get(key, missing), right.get(key, missing), path + "/" + key)
        elif isinstance(left, list) and isinstance(right, list):
            for index in range(max(len(left), len(right))):
                compare(left[index] if index < len(left) else missing,
                        right[index] if index < len(right) else missing, path + "/" + str(index))
        else:
            metadata_changes.append({"path": path, "before": left, "after": right})

    for key in sorted(before.keys() | after.keys()):
        if key not in ("functions", "ranges"):
            compare(before.get(key, missing), after.get(key, missing), "/" + key)
    comparison = {"baseline": {"path": str(BASELINE), "sha256": BASELINE_SHA},
                  "proposal": {"path": str(PROPOSAL.relative_to(ROOT)), "sha256": digest(payload), "size": len(payload)},
                  "proposal_bytes_equal": payload == baseline_bytes,
                  "function_record_changes": function_changes, "range_record_changes": range_changes,
                  "function_order_changed": [row["address"] for row in before["functions"]] != [row["address"] for row in after["functions"]],
                  "range_order_changed": list(old_ranges) != list(new_ranges),
                  "other_metadata_changes": metadata_changes,
                  "stats": after["stats"], "remaining_unknown_ranges": [row for row in after["ranges"] if row["kind"] == "unknown"],
                  "remaining_anomalies": after["anomalies"], "actual_generator_exit": result.returncode,
                  "released_before_adjudication": True, "canonical_adoption": False, "new_C_credit": 0}
    save(OUT / "comparison-r1.json", comparison)
    print(json.dumps({"actual_generator_exit": 0, "released": True, "proposal_sha256": digest(payload),
                      "function_record_changes": len(function_changes), "range_record_changes": len(range_changes),
                      "other_metadata_changes": len(metadata_changes), "canonical_adoption": False}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
