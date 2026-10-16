#!/usr/bin/env python3
"""Write an objdiff-format progress report (report.json) for ByteMatch (bytematch.dev).

    python3 ci/progress_report.py --version jp --output report.json [--summary $GITHUB_STEP_SUMMARY]

ByteMatch ingests an objdiff `Report` (objdiff-core/protos/report.proto, report version 2; protobuf
JSON mapping, uint64 as strings) uploaded as a GitHub Actions artifact named `<version>_report`.
No ROM is needed:
- the denominator is the frozen function catalogue config/catalogue.json;
- the numerator is every function credited in config/matches.json, which only changes when the
  integrator records an accepted build (tools/build.py receipt PASS).

Units are 64 KiB address blocks of the text segment until functions are grouped into translation
units. Standard library only, so CI needs nothing but Python 3.
"""
from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
REPORT_VERSION = 2
BLOCK = 0x10000
AUTO = re.compile(r"^func_([0-9a-fA-F]{8})$")


def measures(fns: list[dict], matched: set[int], data_total: int = 0) -> dict:
    total = sum(f["size"] for f in fns)
    done = sum(f["size"] for f in fns if f["address"] in matched)
    nf = len(fns)
    nm = sum(1 for f in fns if f["address"] in matched)
    pct = 100.0 * done / total if total else 0.0
    return {
        "fuzzy_match_percent": pct,
        "total_code": str(total),
        "matched_code": str(done),
        "matched_code_percent": pct,
        "total_data": str(data_total),
        "matched_data": "0",
        "matched_data_percent": 0.0,
        "total_functions": nf,
        "matched_functions": nm,
        "matched_functions_percent": 100.0 * nm / nf if nf else 0.0,
        "complete_code": str(done),
        "complete_code_percent": pct,
        "complete_data": "0",
        "complete_data_percent": 0.0,
    }


def load() -> tuple[dict, set[int], dict[int, str]]:
    catalogue = json.loads((ROOT / "config/catalogue.json").read_text(encoding="utf-8"))
    names_path = ROOT / "config/names.json"
    overlay = json.loads(names_path.read_text(encoding="utf-8"))["symbols"] if names_path.is_file() else {}
    by_name = {name: entry["address"] for name, entry in overlay.items()}
    addresses = {f["address"] for f in catalogue["functions"]}
    matched: set[int] = set()
    for unit in json.loads((ROOT / "config/matches.json").read_text(encoding="utf-8"))["units"]:
        for name in unit["functions"]:
            m = AUTO.match(name)
            address = int(m.group(1), 16) if m else by_name.get(name)
            if address not in addresses:
                raise SystemExit(f"matches.json credits {name}, which is not a catalogued function")
            matched.add(address)
    display = {entry["address"]: name for name, entry in overlay.items()}
    return catalogue, matched, display


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--version", default="jp", help="version slug (artifact name is <version>_report)")
    parser.add_argument("--output", type=Path, default=Path("report.json"))
    parser.add_argument("--summary", type=Path, help="also append a Markdown summary (e.g. $GITHUB_STEP_SUMMARY)")
    args = parser.parse_args()
    catalogue, matched, display = load()
    functions = sorted(catalogue["functions"], key=lambda f: f["address"])
    data_total = sum(s["size"] for s in catalogue["segments"] if s["name"] in ("rodata", "data"))

    blocks: dict[int, list[dict]] = {}
    for f in functions:
        blocks.setdefault(f["address"] // BLOCK * BLOCK, []).append(f)
    units = []
    for base, fns in sorted(blocks.items()):
        m = measures(fns, matched)
        m["total_units"] = 1
        m["complete_units"] = int(m["matched_functions"] == m["total_functions"])
        units.append({
            "name": f"text_{base:08x}",
            "measures": m,
            "sections": [{"name": ".text", "size": m["total_code"], "fuzzy_match_percent": m["fuzzy_match_percent"]}],
            "functions": [
                {
                    "name": display.get(f["address"], f"func_{f['address']:08x}"),
                    "size": str(f["size"]),
                    "fuzzy_match_percent": 100.0 if f["address"] in matched else 0.0,
                    "metadata": {"virtual_address": str(f["address"])},
                }
                for f in fns
            ],
            "metadata": {"complete": False, "module_name": "code.bin", "progress_categories": ["game"], "auto_generated": True},
        })
    total = measures(functions, matched, data_total)
    total["total_units"] = len(units)
    total["complete_units"] = sum(u["measures"]["complete_units"] for u in units)
    report = {
        "measures": total,
        "units": units,
        "version": REPORT_VERSION,
        "categories": [{"id": "game", "name": "code.bin", "measures": total}],
    }
    args.output.write_text(json.dumps(report, indent=1) + "\n", encoding="utf-8", newline="\n")
    line = (f"{args.version}: {total['matched_functions']}/{total['total_functions']} functions, "
            f"{total['matched_code']}/{total['total_code']} code bytes ({total['matched_code_percent']:.3f}%)")
    print(line)
    if args.summary:
        with args.summary.open("a") as f:
            f.write(f"### Progress\n\n{line}\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
