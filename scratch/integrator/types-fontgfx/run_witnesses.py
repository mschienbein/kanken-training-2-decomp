"""Run only the sealed fontgfx per-unit match scope, sequentially."""
from __future__ import annotations

import hashlib
import json
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
BASE = ROOT / "scratch/integrator/types-fontgfx"


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main() -> int:
    round_name = sys.argv[1] if len(sys.argv) > 1 else "r1"
    scope_path = BASE / f"SCOPE-{round_name}.json"
    scope = json.loads(scope_path.read_text())
    runs: list[dict] = []
    started = time.monotonic()
    header_hashes = {p: digest(ROOT / p) for p in scope["headers"]}
    for index, unit in enumerate(scope["units"], 1):
        source = unit["source"]
        if not unit["functions"]:
            continue
        out = BASE / f"final-{round_name}" / f"{index:04d}-{Path(source).stem}"
        argv = [sys.executable, "tools/match.py", source,
                "--profile", unit["profile"], "--show", "0",
                "--linker-model", scope["selection"]["linker_model"],
                "--literal-model", scope["selection"]["literal_model"],
                "--out", str(out.relative_to(ROOT))]
        for extra in unit.get("extra_sources", []):
            argv.extend(["--extra-source", extra])
        before = digest(ROOT / source)
        start = time.monotonic()
        try:
            completed = subprocess.run(argv, cwd=ROOT, text=True,
                                       capture_output=True, timeout=60, check=False)
            exit_code, stdout, stderr = completed.returncode, completed.stdout, completed.stderr
        except subprocess.TimeoutExpired as error:
            exit_code, stdout, stderr = 124, str(error.stdout or ""), str(error.stderr or "")
        receipts = []
        for receipt in sorted(out.glob("*.result.json")):
            receipts.append({"path": str(receipt.relative_to(ROOT)),
                             "sha256": digest(receipt),
                             "data": json.loads(receipt.read_text())})
        row = {"source": source, "profile": unit["profile"],
               "functions": unit["functions"], "argv": argv,
               "extra_sources": unit.get("extra_sources", []),
               "source_sha256": before, "source_sha256_after": digest(ROOT / source),
               "exit": exit_code, "stdout": stdout, "stderr": stderr,
               "seconds": time.monotonic() - start, "receipts": receipts}
        runs.append(row)
        (BASE / f"RUNS-{round_name}.json").write_text(json.dumps({
            "scope_sha256": digest(scope_path), "selection": scope["selection"],
            "header_sha256": header_hashes, "runs": runs,
            "seconds": time.monotonic() - started}, indent=1) + "\n")
    failures = [{"source": r["source"], "exit": r["exit"],
                 "stdout": r["stdout"], "stderr": r["stderr"]}
                for r in runs if r["exit"] or r["source_sha256"] != r["source_sha256_after"]]
    print(json.dumps({"units": len(runs), "functions": sum(len(r["functions"]) for r in runs),
                      "failures": failures, "seconds": time.monotonic() - started}, indent=1))
    return bool(failures)


if __name__ == "__main__":
    raise SystemExit(main())
