"""Static qualified-type inventory; never preprocesses, compiles or reads ROM.

Only token-identical nonlocal definitions are deletion candidates. Different
same-named owners remain explicit reconciliation obligations, not aliases.
"""
from __future__ import annotations

import argparse
import collections
import hashlib
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[3]
OUT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "tools"))
import interfaces


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out", required=True)
    args = parser.parse_args()
    canonical: dict[str, list[dict]] = collections.defaultdict(list)
    for name, text in interfaces.project_headers(ROOT).items():
        for declaration in interfaces.cpp_class_declarations(text, "include/" + name):
            if not declaration.unit_local and declaration.body is not None:
                canonical[declaration.name].append({"header": "include/" + name, "body": declaration.body, "line": declaration.line})
    rows = []
    manifest = json.loads((ROOT / "config/candidates/c010.json").read_text())
    for unit in manifest["units"]:
        path = ROOT / unit["source"]
        text = path.read_text()
        for declaration in interfaces.cpp_class_declarations(text, unit["source"]):
            if declaration.unit_local or declaration.name not in canonical:
                continue
            definitions = canonical[declaration.name]
            exact = [row for row in definitions if row["body"] == declaration.body]
            rows.append({"source": unit["source"], "type": declaration.name, "line": declaration.line,
                         "source_sha256": hashlib.sha256(path.read_bytes()).hexdigest(),
                         "kind": "forward" if declaration.body is None else "identical-definition" if exact else "different-definition",
                         "body": declaration.body, "canonical": definitions,
                         "matching_headers": [row["header"] for row in exact]})
    result = {"schema_version": 1, "scope_units": len(manifest["units"]),
              "counts": dict(collections.Counter(row["kind"] for row in rows)), "rows": rows,
              "qualification": "Static qualified identity only; no native compilation, semantic owner equivalence or ABI-layout proof is claimed."}
    with (OUT / args.out).open("x") as stream:
        json.dump(result, stream, indent=2)
        stream.write("\n")
    print(json.dumps({key: result[key] for key in ("scope_units", "counts")}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
