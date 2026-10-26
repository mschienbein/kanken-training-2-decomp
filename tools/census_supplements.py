#!/usr/bin/env python3
"""Write the next immutable metadata-only census supplement.

Objective rungs 1-3 require a Main-recorded batch of that explicit step, a named
creditable target, and a delivered target RESULT row with current extents. The
census and previous supplements prevent duplicate rung completion. Today's
rv01-rv12, fx01-fx05 and dt01-dt04 batches are supplemental revisit witnesses,
not objective rungs; fs10 is an ordinary explicit step-2 batch. Every witness
binds its batch and RESULT bytes by SHA-256. tools/residue.py re-verifies them.

Usage: .venv/bin/python tools/census_supplements.py --root .
The private scratch/integrator/census-supplements/build.py is a thin entry point.
"""
from __future__ import annotations

import argparse
import json
from pathlib import Path

from residue import (AUTHOR_STEPS, REVISIT_BATCHES, WITNESS_STATUSES, canonical,
                     census_step, digest, identity, reported_extents)

CENSUS = "scratch/integrator/census-20261021-r3/LADDER.json"
SUPPLEMENTS = "scratch/integrator/census-supplements"


def build(root: Path) -> tuple[Path | None, dict]:
    root = root.resolve()
    out_dir = root / SUPPLEMENTS
    catalogue = json.loads((root / "config/catalogue.json").read_bytes())
    sizes = {identity(row): row["size"] for row in catalogue["functions"]
             if row.get("kind") == "function"}
    census = json.loads((root / CENSUS).read_bytes())
    held = {(identity(row), step) for row in census["rows"]
            if sizes.get(identity(row)) == row.get("target_bytes")
            for label in row.get("completed_steps", [])
            if (step := census_step(label)) in AUTHOR_STEPS}
    previous = sorted(out_dir.glob("S*.json"))
    seen_revisits = set()
    for path in previous:
        doc = json.loads(path.read_bytes())
        for row in doc.get("rows", []):
            if sizes.get(identity(row)) == row.get("target_bytes"):
                held.update((identity(row), step) for step in row.get("completion_evidence", {}))
        for row in doc.get("revisit_witnesses", []):
            if sizes.get(identity(row)) == row.get("target_bytes"):
                seen_revisits.update((entry["batch"], entry["result_row"])
                                     for entry in row.get("evidence", []))

    inputs, rows, revisits = {}, {}, {}
    for batch_path in sorted((root / "scratch/integrator/batches").glob("*.json")):
        batch_raw = batch_path.read_bytes()
        batch_doc = json.loads(batch_raw)
        if not isinstance(batch_doc, dict) or batch_doc.get("created_by") != "Main":
            continue
        batch = batch_path.stem
        step = str(batch_doc.get("step"))
        revisit = batch in REVISIT_BATCHES and "revisit" in step.lower()
        if step not in AUTHOR_STEPS and not revisit:
            continue
        result_path = root / f"scratch/authors/{batch}/RESULT.json"
        if not result_path.is_file():
            continue
        targets = {identity(row) for row in batch_doc.get("functions", batch_doc.get("targets", []))}
        result_raw = result_path.read_bytes()
        delivered = json.loads(result_raw).get("functions", [])
        for index, row in enumerate(delivered):
            name = identity(row)
            if name not in targets or name not in sizes or row.get("status") not in WITNESS_STATUSES:
                continue
            if any(size != sizes[name] for size in reported_extents(row)):
                continue
            entry = {"batch": batch, "result_row": index}
            if revisit:
                if (batch, index) in seen_revisits:
                    continue
                record = revisits.setdefault(name, {"function": name,
                                                    "target_bytes": sizes[name], "evidence": []})
                record["evidence"].append(entry)
            else:
                if (name, step) in held:
                    continue
                record = rows.setdefault(name, {"function": name, "target_bytes": sizes[name],
                                                "completion_evidence": {}})
                record["completion_evidence"].setdefault(step, []).append(entry)
            for path, raw in ((batch_path, batch_raw), (result_path, result_raw)):
                inputs[path.relative_to(root).as_posix()] = digest(raw)

    summary = {"functions": len(rows), "revisit_functions": len(revisits),
               "revisit_witnesses": sum(len(row["evidence"]) for row in revisits.values()),
               "by_step": {step: sum(step in row["completion_evidence"] for row in rows.values())
                           for step in AUTHOR_STEPS}}
    if not rows and not revisits:
        return None, summary
    number = 1
    while (out_dir / f"S{number:02d}.json").exists():
        number += 1
    output = out_dir / f"S{number:02d}.json"
    doc = {"schema_version": 2, "label": f"S{number:02d}", "created_by": "Main",
           "census": CENSUS, "previous_supplements": [path.name for path in previous],
           "rule": "Only explicit Main rungs 1-3 complete steps; supplemental revisits add provenance only.",
           "input_sha256": dict(sorted(inputs.items())),
           "rows": [rows[name] for name in sorted(rows)],
           "revisit_witnesses": [revisits[name] for name in sorted(revisits)]}
    out_dir.mkdir(parents=True, exist_ok=True)
    with output.open("xb") as handle:
        handle.write(canonical(doc))
    return output, summary


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--root", type=Path, default=Path("."))
    root = parser.parse_args(argv).root.resolve()
    output, summary = build(root)
    print(json.dumps({"written": output.relative_to(root).as_posix() if output else None,
                      **summary}, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
