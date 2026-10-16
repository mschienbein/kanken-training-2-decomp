"""Hash-chained history of the function catalogue (the progress denominator).

    python tools/denominator.py record --note "why this revision"   # integrator only
    python tools/denominator.py verify                              # CI-safe, no ROM

config/catalogue.json may change only together with a new entry here. Each entry records the
catalogue's sha256 and headline counts and links to the previous entry's hash, so silent edits
to the denominator are detectable.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import sys
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CATALOGUE = ROOT / "config/catalogue.json"
HISTORY = ROOT / "docs/denominator-history.jsonl"


def canonical(entry: dict) -> bytes:
    return json.dumps(entry, sort_keys=True, separators=(",", ":"), ensure_ascii=False).encode("utf-8")


def entry_hash(entry: dict) -> str:
    return hashlib.sha256(canonical({k: v for k, v in entry.items() if k != "entry_sha256"})).hexdigest()


def read_history() -> list[dict]:
    if not HISTORY.is_file():
        return []
    return [json.loads(line) for line in HISTORY.read_text(encoding="utf-8").splitlines() if line.strip()]


def verify() -> list[dict]:
    entries = read_history()
    if not entries:
        raise SystemExit("no denominator history")
    prev = None
    for i, e in enumerate(entries, 1):
        if e["version"] != i or e["prev"] != prev or e["entry_sha256"] != entry_hash(e):
            raise SystemExit(f"denominator history broken at version {i}")
        prev = e["entry_sha256"]
    current = hashlib.sha256(CATALOGUE.read_bytes()).hexdigest()
    if current != entries[-1]["catalogue_sha256"]:
        raise SystemExit(f"config/catalogue.json ({current}) is not the recorded denominator v{len(entries)}")
    return entries


def record(note: str) -> dict:
    entries = read_history()
    catalogue = json.loads(CATALOGUE.read_text(encoding="utf-8"))
    stats = catalogue.get("stats", {})
    by_kind = stats.get("bytes_by_range_kind", {})
    entry = {
        "version": len(entries) + 1,
        "recorded_utc": datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "catalogue_sha256": hashlib.sha256(CATALOGUE.read_bytes()).hexdigest(),
        "image_sha256": catalogue["image_sha256"],
        "functions": len(catalogue["functions"]),
        "function_bytes": sum(f["size"] for f in catalogue["functions"]),
        "unknown_bytes": by_kind.get("unknown"),
        "anomalies": len(catalogue.get("anomalies", [])),
        "generator": catalogue.get("generator"),
        "analyze_sha256": hashlib.sha256((ROOT / "tools/analyze.py").read_bytes()).hexdigest(),
        "note": note,
        "prev": entries[-1]["entry_sha256"] if entries else None,
    }
    if entries and entries[-1]["catalogue_sha256"] == entry["catalogue_sha256"]:
        raise SystemExit("catalogue unchanged since the last recorded version")
    entry["entry_sha256"] = entry_hash(entry)
    with HISTORY.open("a", encoding="utf-8", newline="\n") as f:
        f.write(json.dumps(entry, ensure_ascii=False, sort_keys=True) + "\n")
    return entry


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="cmd", required=True)
    rec = sub.add_parser("record")
    rec.add_argument("--note", required=True)
    sub.add_parser("verify")
    args = parser.parse_args(argv)
    if args.cmd == "record":
        e = record(args.note)
        print(f"denominator v{e['version']}: {e['functions']} functions, {e['function_bytes']} bytes ({e['entry_sha256'][:12]})")
    else:
        entries = verify()
        print(f"ok: denominator v{len(entries)} matches config/catalogue.json")
    return 0


if __name__ == "__main__":
    sys.exit(main())
