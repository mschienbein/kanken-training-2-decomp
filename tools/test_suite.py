"""Run the unittest suite sharded by module, in parallel processes.

    python tools/test_suite.py [--jobs N] [--json result.json]

Each tests/test_*.py module runs in its own `python -m unittest` process; results are summed. Exit
status 0 only if every module passed. Skipped tests are counted and reported, never as passes.
"""
from __future__ import annotations

import argparse
import json
import os
import re
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def run_module(module: str) -> dict:
    started = time.perf_counter()
    proc = subprocess.run(
        [sys.executable, "-m", "unittest", module], cwd=ROOT, capture_output=True, text=True,
        encoding="utf-8", errors="replace", check=False,
    )
    lines = proc.stderr.strip().splitlines()
    ran = next((int(m.group(1)) for line in lines if (m := re.match(r"Ran (\d+) tests?", line))), 0)
    last = lines[-1] if lines else ""
    skipped = int(m.group(1)) if (m := re.search(r"skipped=(\d+)", last)) else 0
    return {
        "module": module, "returncode": proc.returncode, "tests_run": ran, "skipped": skipped,
        "summary": last, "seconds": round(time.perf_counter() - started, 3),
        "output": proc.stderr if proc.returncode else "",
    }


def run(jobs: int | None = None) -> dict:
    modules = sorted(f"tests.{p.stem}" for p in (ROOT / "tests").glob("test_*.py"))
    started = time.perf_counter()
    with ThreadPoolExecutor(max_workers=jobs or os.cpu_count() or 1) as pool:
        results = list(pool.map(run_module, modules))
    return {
        "modules": results,
        "tests_run": sum(r["tests_run"] for r in results),
        "skipped": sum(r["skipped"] for r in results),
        "failed_modules": [r["module"] for r in results if r["returncode"]],
        "returncode": 1 if any(r["returncode"] for r in results) else 0,
        "seconds": round(time.perf_counter() - started, 3),
    }


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--jobs", type=int)
    parser.add_argument("--json", type=Path)
    args = parser.parse_args(argv)
    result = run(args.jobs)
    for r in result["modules"]:
        print(f"{'ok  ' if not r['returncode'] else 'FAIL'} {r['module']:<28} {r['tests_run']:>4} tests "
              f"({r['skipped']} skipped) {r['seconds']:>7.2f}s")
        if r["returncode"]:
            print(r["output"])
    print(f"{result['tests_run']} tests, {result['skipped']} skipped, {len(result['failed_modules'])} failed modules, "
          f"{result['seconds']:.1f}s")
    if args.json:
        args.json.write_text(json.dumps(result, indent=1) + "\n", encoding="utf-8", newline="\n")
    return result["returncode"]


if __name__ == "__main__":
    raise SystemExit(main())
