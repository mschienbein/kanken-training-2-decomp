"""Retain the actual exit of the complete serial c012-r5 source proof."""
from __future__ import annotations
import json
import subprocess
import sys
import time
from pathlib import Path

def main() -> int:
    evidence = Path(__file__).resolve().parent
    root = evidence.parents[3]
    destination = evidence / "match-r1"
    record = evidence / "DRIVER-EXIT-r1.json"
    if destination.exists() or record.exists():
        raise FileExistsError("Evidence paths must be unused")
    argv = [sys.executable, str(evidence / "reprove_grouped_closure.py"),
            "--scope", str(evidence / "COMPLETE-SCOPE-r1.json"),
            "--out", str(destination)]
    started = time.monotonic()
    process = subprocess.run(argv, cwd=root, check=False)
    with record.open("x", encoding="utf-8") as stream:
        json.dump({"argv": argv, "cwd": str(root), "actual_exit_code": process.returncode,
                   "seconds": round(time.monotonic() - started, 6)}, stream, indent=2)
        stream.write("\n")
    return process.returncode

if __name__ == "__main__":
    raise SystemExit(main())
