"""Run the granted whole-ledger interface command and atomically release its slot."""
from __future__ import annotations

import hashlib
import json
import os
import subprocess
import sys
import tempfile
from pathlib import Path


def write_new(path: Path, value: dict) -> None:
    with path.open("x", encoding="utf-8") as stream:
        json.dump(value, stream, indent=2)
        stream.write("\n")


def main() -> int:
    evidence = Path(__file__).resolve().parent
    root = evidence.parents[3]
    grant = evidence / "HEAVY-SLOT-GRANTED.json"
    request = json.loads((evidence / "HEAVY-SLOT-REQUEST.json").read_text())
    release = evidence / "HEAVY-SLOT-RELEASED.json"
    run_record = evidence / "INTERFACES-RUN-r1.json"
    report = root / request["output_paths"][0]
    if not grant.is_file():
        raise FileNotFoundError("Main's grant record is required")
    if release.exists() or run_record.exists() or report.exists():
        raise FileExistsError("Every command output must be fresh")
    argv = request["argv"]
    process = subprocess.run(argv, cwd=root, capture_output=True, text=True, check=False)
    released = {"schema_version": 1, "released": True, "argv": argv,
                "cwd": str(root), "actual_exit_codes": [process.returncode],
                "output_paths": request["output_paths"]}
    # Publish the release before any other post-command work.
    with tempfile.NamedTemporaryFile(mode="w", encoding="utf-8", dir=evidence,
                                     prefix=".slot-release-", delete=False) as stream:
        json.dump(released, stream, indent=2)
        stream.write("\n")
        temporary = Path(stream.name)
    os.replace(temporary, release)
    write_new(run_record, {**released, "actual_exit_code": process.returncode,
                           "stdout": process.stdout, "stderr": process.stderr,
                           "report_sha256": hashlib.sha256(report.read_bytes()).hexdigest()
                           if report.is_file() else None})
    print(f"Interfaces actual exit: {process.returncode}; heavy slot released")
    return process.returncode


if __name__ == "__main__":
    raise SystemExit(main())
