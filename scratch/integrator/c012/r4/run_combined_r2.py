"""Run the frozen combined source proof and retain its actual driver exit code."""
from __future__ import annotations

import json
import subprocess
import sys
import time
from pathlib import Path


def main() -> int:
    root = Path(__file__).resolve().parents[4]
    evidence = Path(__file__).resolve().parent
    destination = evidence / "match-combined-r2"
    exit_path = evidence / "DRIVER-EXIT-COMBINED-r2.json"
    if destination.exists() or exit_path.exists():
        raise FileExistsError("Combined proof evidence paths must be fresh")
    command = [sys.executable, str(evidence / "reprove_grouped_closure.py"),
               "--scope", str(evidence / "COMPLETE-SCOPE-r3.json"),
               "--out", str(destination)]
    started = time.monotonic()
    process = subprocess.run(command, cwd=root, check=False)
    with exit_path.open("x", encoding="utf-8", newline="\n") as stream:
        json.dump({"schema_version": 1, "argv": command, "cwd": str(root),
                   "actual_exit_code": process.returncode,
                   "seconds": round(time.monotonic() - started, 6),
                   "output": str(destination)}, stream, indent=2)
        stream.write("\n")
    return process.returncode


if __name__ == "__main__":
    raise SystemExit(main())
