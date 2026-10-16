"""Re-run tools/match.py on every author `mismatch` row that has a source, under each linker model.

    .venv/bin/python research/linker/measure_authors.py [--authors DIR] [--sources-root DIR] [--out FILE]

Sequential (one compile at a time). `--authors` holds `<agent>/RESULT.json`; row `source` paths
are relative to `--sources-root` (default: this checkout). Writes per-row status and the linker
rewrites per model to `--out` and prints one line per row (docs/LINKER.md, Measurements).
"""
from __future__ import annotations

import argparse
import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
import inplace  # noqa: E402

MODELS = sorted(inplace.LINKER_MODELS)


def address(row: dict) -> int:
    a = row["address"]
    return int(a, 16) if isinstance(a, str) else a


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--authors", type=Path, default=ROOT / "scratch/authors")
    parser.add_argument("--sources-root", type=Path, default=ROOT)
    parser.add_argument("--out", type=Path, default=ROOT / "scratch/armlink/measure.json")
    args = parser.parse_args()
    rows = [
        (result.parent.name, row)
        for result in sorted(args.authors.glob("*/RESULT.json"))
        for row in json.loads(result.read_text(encoding="utf-8"))["functions"]
        if row.get("status") == "mismatch" and row.get("source")
    ]
    out = []
    for agent, row in rows:
        src = args.sources_root / row["source"]
        entry = {"agent": agent, "address": f"{address(row):#x}", "source": row["source"], "profile": row["profile"]}
        for model in MODELS:
            odir = args.out.parent / "measure" / agent / src.stem / model
            proc = subprocess.run(
                [sys.executable, str(ROOT / "tools/match.py"), str(src), "--profile", row["profile"],
                 "--out", str(odir), "--show", "0", "--linker-model", model],
                capture_output=True, text=True, cwd=ROOT,
            )
            result = odir / f"{src.stem}.result.json"
            if proc.returncode in (0, 1) and result.is_file():
                hit = [f for f in json.loads(result.read_text(encoding="utf-8"))["functions"] if f["address"] == address(row)]
                entry[model] = ({"status": hit[0]["status"], "first_diff": hit[0]["first_diff"], "rewrites": hit[0]["linker_rewrites"]}
                                if hit else {"status": "not_defined"})
            else:
                entry[model] = {"status": "error", "detail": (proc.stderr or proc.stdout).strip().splitlines()[-1:]}
        out.append(entry)
        print(agent, entry["address"], *(f"{m}={entry[m]['status']}" for m in MODELS), flush=True)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(out, indent=1) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
