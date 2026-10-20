"""Plan conservative c010 header consumers without compiling or asserting G3.

Uses the repository's existing textual include closure. Symbol and named-record
references also retain consumers that shadow a declaration instead of including
its canonical provider. The result is a proposal: rebuild after header cutovers.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[3]
OUT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "tools"))
import interfaces

ADDRESS = re.compile(r"\b(?:func|data|bss)_[0-9a-fA-F]{8}\b")
RECORD = re.compile(r"\b(?:struct|class|union|enum)\s+([A-Za-z_]\w*)")
TOKEN = re.compile(r"\b[A-Za-z_]\w*\b")


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--run", required=True)
    args = parser.parse_args()
    report_path = OUT / ("affected-header-plan-" + args.run + ".json")
    scope_path = OUT / ("affected-header-match-scope-" + args.run + ".json")
    if report_path.exists() or scope_path.exists():
        raise SystemExit("Refusing to replace historical scope evidence")
    handoff_path = OUT / "HANDOFF.json"
    handoff = json.loads(handoff_path.read_text())
    dry_path = ROOT / handoff["dry_ledger"]["path"]
    dry = json.loads(dry_path.read_text())
    direct_path = ROOT / handoff["match_scope"]
    direct = json.loads(direct_path.read_text())
    required = {row["source"] for row in direct["units"]}
    header_paths = sorted(set(handoff["new_headers"] + handoff["revised_headers"]))
    texts = interfaces.project_headers(ROOT)
    changed = {}
    for path in header_paths:
        text = (ROOT / path).read_text()
        changed[path] = {
            "key": path.removeprefix("include/"),
            "address_symbols": set(ADDRESS.findall(text)),
            "record_tags": {name for name in RECORD.findall(text) if len(name) > 2},
        }
    selected, consumers, exclusions = [], [], []
    for row in dry["units"]:
        source = ROOT / row["source"]
        text = source.read_text()
        included = interfaces.included_headers(text, texts)
        symbols, tokens = set(ADDRESS.findall(text)), set(TOKEN.findall(text))
        reasons = []
        for path, metadata in changed.items():
            address_hits = sorted(symbols & metadata["address_symbols"])
            record_hits = sorted(tokens & metadata["record_tags"])
            include_hit = metadata["key"] in included
            if include_hit or address_hits or record_hits:
                reasons.append({"header": path, "transitive_include": include_hit,
                                "address_reference_hits": address_hits,
                                "named_record_reference_hits": record_hits})
        if row["source"] in required or reasons:
            selected.append(dict(row))
            consumers.append(dict(row, source_sha256=sha256(source),
                                  directly_required=row["source"] in required,
                                  header_reasons=reasons))
        else:
            exclusions.append({"source": row["source"], "source_sha256": sha256(source)})
    report = {
        "schema_version": 1,
        "status": "static-plan-not-a-gate-result-recompute-after-header-cutovers",
        "scope_record": scope_path.name,
        "selection": handoff["selection"],
        "input_sha256": {str(path.relative_to(ROOT)): sha256(path)
                         for path in (handoff_path, dry_path, direct_path)},
        "header_sha256": {path: sha256(ROOT / path) for path in header_paths},
        "dry_units": len(dry["units"]),
        "direct_units": len(required),
        "selected_units": len(selected),
        "selected_functions": sum(len(row["functions"]) for row in selected),
        "additional_header_consumers": sum(not row["directly_required"] for row in consumers),
        "header_consumer_counts": {path: sum(any(reason["header"] == path
                                                for reason in row["header_reasons"])
                                             for row in consumers)
                                   for path in header_paths},
        "consumers": consumers,
        "units_without_current_header_evidence": exclusions,
        "method": [
            "Repository interfaces.project_headers/included_headers, with all preprocessor branches retained.",
            "Conservative address-name and named-record token intersections retain local declaration/record shadows.",
            "All directly required candidate/revised predecessor units are retained even without a header reference."
        ],
        "limitations": [
            "No preprocessing, compiler, MATCH, G3, argcheck or original-image decoding was run.",
            "Named native methods/operators and typedef-only spellings are not a complete nominal-ABI index; canonical probes/G3 and the full original consumer sweep remain required.",
            "This is a conservative intermediate plan bound to the current handoff and headers, not permission for a broad job or a claim of a final frozen scope."
        ],
    }
    scope = {"schema_version": 1,
             "note": "Conservative static scope plan only; sequential final checks need Main's released heavy slot and stable headers.",
             "units": selected}
    report_path.write_text(json.dumps(report, indent=2) + "\n")
    scope_path.write_text(json.dumps(scope, indent=2) + "\n")
    print(json.dumps({key: report[key] for key in (
        "dry_units", "direct_units", "selected_units", "selected_functions",
        "additional_header_consumers", "header_consumer_counts")}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
