"""Plan exact canonical-definition removal with the interface scanner's tokens.

ast_edit rejected complete struct definitions plus their terminating semicolon
as multiple nodes. This preserves that failure and uses token-exact complete
statement spans, not regex substitutions or renaming/shadow-type aliases.
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
    parser.add_argument("--inventory", default="type-inventory-r1.json")
    parser.add_argument("--out", default="type-cutover-plan-r1.json")
    args = parser.parse_args()
    inventory = json.loads((OUT / args.inventory).read_text())
    selected: dict[str, list[dict]] = collections.defaultdict(list)
    for row in inventory["rows"]:
        if row["kind"] == "identical-definition":
            selected[row["source"]].append(row)
    plans = []
    skipped = []
    for source, rows in selected.items():
        text = (ROOT / source).read_text()
        cooked = interfaces.CPP_DIRECTIVE.sub(lambda match: "".join("\n" if char == "\n" else " " for char in match[0]), text)
        tokens = [match for match in interfaces.CPP_TOKEN.finditer(cooked) if not match[0].startswith(("//", "/*"))]
        spans = []
        for row in rows:
            matches = []
            for index, token in enumerate(tokens):
                if token[0] not in ("struct", "class", "union"):
                    continue
                words = []
                for stop in range(index, len(tokens)):
                    words.append(tokens[stop][0])
                    candidate = " ".join(words)
                    if candidate == row["body"]:
                        if stop + 1 >= len(tokens) or tokens[stop + 1][0] != ";":
                            raise ValueError("Definition lacks its complete terminating semicolon")
                        matches.append((token.start(), tokens[stop + 1].end()))
                        break
                    if not row["body"].startswith(candidate):
                        break
            if len(matches) != 1:
                raise ValueError(f"Expected exactly one token-exact {row['type']} in {source}, found {len(matches)}")
            start, stop = matches[0]
            if "ORIGINAL BUG" in text[start:stop]:
                skipped.append({"source": source, "type": row["type"], "reason": "Preserve a unique original-bug annotation for manual consolidation"})
                continue
            spans.append({"start": start, "stop": stop, "type": row["type"], "headers": row["matching_headers"]})
        line_spans = []
        for span in sorted(spans, key=lambda item: item["start"]):
            first = text.count("\n", 0, span["start"]) + 1
            last = text.count("\n", 0, span["stop"] - 1) + 1
            if line_spans and first <= line_spans[-1]["last"]:
                line_spans[-1]["last"] = max(last, line_spans[-1]["last"])
                line_spans[-1]["spans"].append(span)
            else:
                line_spans.append({"first": first, "last": last, "spans": [span]})
        lines = text.splitlines(keepends=True)
        edits = []
        for group in line_spans:
            first, last = group["first"], group["last"]
            offset = sum(len(line) for line in lines[:first - 1])
            body = "".join(lines[first - 1:last])
            for span in sorted(group["spans"], key=lambda item: item["start"], reverse=True):
                body = body[:span["start"] - offset] + body[span["stop"] - offset:]
            edits.append({"first": first, "last": last, "replacement": body if body.strip() else "", "types": [span["type"] for span in group["spans"]]})
        plans.append({"source": source, "source_sha256": hashlib.sha256(text.encode()).hexdigest(),
                      "headers": sorted({header for span in spans for header in span["headers"]}), "edits": edits})
    result = {"schema_version": 1, "status": "token-exact-plan-only", "units": plans, "skipped": skipped,
              "source_inventory": args.inventory, "definitions": sum(len(edit["types"]) for row in plans for edit in row["edits"])}
    with (OUT / args.out).open("x") as stream:
        json.dump(result, stream, indent=2)
        stream.write("\n")
    print(json.dumps({"units": len(plans), "definitions": result["definitions"], "skipped": len(skipped)}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
