"""Accept matched source as a checkpoint: prepare, independent review, finalize once.

    python tools/accept.py prepare  --checkpoint c001-pilot --candidates config/candidates/pilot.json
    python tools/accept.py review   --checkpoint c001-pilot --reviewer NAME --family FAMILY \
                                    --verdict PASS|PASS_WITH_QUALIFICATIONS|FAIL --notes notes.md
    python tools/accept.py finalize --checkpoint c001-pilot [--min-reviews 1]
    python tools/accept.py verify                                        # CI-safe, no ROM

prepare: the denominator must verify; the candidate units are added to the accepted ledger
(config/matches.json) into accept/<checkpoint>/matches.json; two fresh tools/build.py builds of
that ledger must PASS with identical outputs, and the full test suite must pass. Everything is
recorded in accept/<checkpoint>/prepare.json (exclusive create; a FAIL is kept, never retried
under the same name).

review: records one reviewer's sealed verdict bound to the exact prepare.json and source hashes.
The reviewer must not be the integrator and must re-derive results, not trust prepare.json.

finalize (once): prepare PASS, at least --min-reviews passing reviews of this exact prepare and
no FAIL review, sources/profiles/catalogue unchanged since prepare, and the canonical ledger
unchanged since prepare. Then config/matches.json becomes the prepared ledger and a hash-chained
entry is appended to docs/progress-history.jsonl.

verify: the history chain is intact, config/matches.json is exactly the last finalized ledger,
and every credited source still has its recorded hash. CI runs this, so credit cannot appear in
the progress report without an accepted checkpoint.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import subprocess
import sys
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MATCHES = ROOT / "config/matches.json"
HISTORY = ROOT / "docs/progress-history.jsonl"
ACCEPT = ROOT / "accept"
PROFILES = ROOT / "config/compiler_profiles.json"
CATALOGUE = ROOT / "config/catalogue.json"
NAME = re.compile(r"^[a-z0-9][a-z0-9.-]{2,63}$")
PASSING = ("PASS", "PASS_WITH_QUALIFICATIONS")


class AcceptError(RuntimeError):
    pass


def now() -> str:
    return datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")


def sha256_file(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def canonical(obj) -> bytes:
    return json.dumps(obj, sort_keys=True, separators=(",", ":"), ensure_ascii=False).encode("utf-8")


def entry_hash(entry: dict) -> str:
    return hashlib.sha256(canonical({k: v for k, v in entry.items() if k != "entry_sha256"})).hexdigest()


def write_exclusive(path: Path, obj) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("x", encoding="utf-8", newline="\n") as f:
        f.write(json.dumps(obj, indent=1, ensure_ascii=False) + "\n")


def load_json(path: Path):
    return json.loads(path.read_text(encoding="utf-8"))


def source_hashes(units: list[dict]) -> dict[str, str]:
    out = {}
    for unit in units:
        path = ROOT / unit["source"]
        if not path.is_file():
            raise AcceptError(f"{unit['source']}: missing")
        out[unit["source"]] = sha256_file(path)
    return out


def read_history() -> list[dict]:
    if not HISTORY.is_file():
        return []
    return [json.loads(line) for line in HISTORY.read_text(encoding="utf-8").splitlines() if line.strip()]


def checkpoint_dir(name: str) -> Path:
    if not NAME.match(name):
        raise AcceptError(f"checkpoint name {name!r} must match {NAME.pattern}")
    return ACCEPT / name


# -- prepare ---------------------------------------------------------------------------------------


def run_suite() -> dict:
    argv = [sys.executable, "-m", "unittest", "discover", "-s", "tests", "-t", "."]
    proc = subprocess.run(argv, cwd=ROOT, capture_output=True, text=True, encoding="utf-8", errors="replace", check=False)
    tail = proc.stderr.strip().splitlines()[-3:]
    ran = next((int(m.group(1)) for line in tail if (m := re.match(r"Ran (\d+) tests?", line))), None)
    skipped = re.search(r"skipped=(\d+)", tail[-1]) if tail else None
    return {
        "command": "python -m unittest discover -s tests -t .",
        "returncode": proc.returncode,
        "tests_run": ran,
        "skipped": int(skipped.group(1)) if skipped else 0,
        "summary": tail[-1] if tail else "",
    }


def prepare(name: str, candidates_path: Path) -> dict:
    import build as build_mod  # imported lazily: build needs the private inputs
    import denominator

    out = checkpoint_dir(name)
    try:
        out.mkdir(parents=True, exist_ok=False)
    except FileExistsError:
        raise AcceptError(f"accept/{name} exists; checkpoints are immutable (use a new name)") from None
    try:
        denominator.verify()
    except SystemExit as e:
        raise AcceptError(f"denominator: {e}") from None
    base = load_json(MATCHES)
    candidates = load_json(candidates_path)
    have = {u["source"] for u in base["units"]}
    for unit in candidates["units"]:
        if unit["source"] in have:
            raise AcceptError(f"{unit['source']} is already in the accepted ledger")
    units = base["units"] + candidates["units"]
    ledger = {"schema_version": 1, "note": f"Accepted ledger as of checkpoint {name}.", "units": units}
    write_exclusive(out / "matches.json", ledger)

    builds = []
    for suffix in ("a", "b"):
        try:
            receipt = build_mod.build(f"{name}-{suffix}", out / "matches.json")
        except (build_mod.BuildError, RuntimeError) as e:
            receipt = {"name": f"{name}-{suffix}", "result": "FAIL", "error": str(e)}
        else:
            write_exclusive(out / "receipts" / f"{name}-{suffix}.json", receipt)
        builds.append(receipt)
    suite = run_suite()
    checks = {
        "build_a_pass": builds[0].get("result") == "PASS",
        "build_b_pass": builds[1].get("result") == "PASS",
        "builds_identical": builds[0].get("outputs") == builds[1].get("outputs") and "outputs" in builds[0],
        "suite_pass": suite["returncode"] == 0,
    }
    record = {
        "schema_version": 1,
        "checkpoint": name,
        "prepared_utc": now(),
        "base_matches_sha256": sha256_file(MATCHES),
        "candidates": candidates_path.resolve().relative_to(ROOT).as_posix(),
        "candidates_sha256": sha256_file(candidates_path),
        "ledger_sha256": sha256_file(out / "matches.json"),
        "sources": source_hashes(units),
        "new_units": [u["source"] for u in candidates["units"]],
        "profiles_sha256": sha256_file(PROFILES),
        "catalogue_sha256": sha256_file(CATALOGUE),
        "denominator_version": len(denominator.read_history()),
        "builds": [
            {k: b.get(k) for k in ("name", "result", "outputs", "credited", "checks", "error") if k in b}
            for b in builds
        ],
        "suite": suite,
        "checks": checks,
    }
    record["result"] = "PASS" if all(checks.values()) else "FAIL"
    write_exclusive(out / "prepare.json", record)
    return record


# -- review ------------------------------------------------------------------------------------------


def review(name: str, reviewer: str, family: str, verdict: str, notes: Path) -> dict:
    out = checkpoint_dir(name)
    prepared = out / "prepare.json"
    if not prepared.is_file():
        raise AcceptError(f"accept/{name}: not prepared")
    if verdict not in (*PASSING, "FAIL"):
        raise AcceptError(f"verdict must be one of {', '.join((*PASSING, 'FAIL'))}")
    if not NAME.match(reviewer.lower()):
        raise AcceptError(f"reviewer name {reviewer!r} must match {NAME.pattern}")
    record = load_json(prepared)
    entry = {
        "schema_version": 1,
        "checkpoint": name,
        "reviewer": reviewer,
        "family": family,
        "verdict": verdict,
        "reviewed_utc": now(),
        "prepare_sha256": sha256_file(prepared),
        "sources": source_hashes([{"source": s} for s in record["sources"]]),
        "notes": notes.read_text(encoding="utf-8"),
    }
    write_exclusive(out / "reviews" / f"{reviewer.lower()}.json", entry)
    return entry


# -- finalize ----------------------------------------------------------------------------------------


def finalize(name: str, min_reviews: int) -> dict:
    out = checkpoint_dir(name)
    prepared = out / "prepare.json"
    if not prepared.is_file():
        raise AcceptError(f"accept/{name}: not prepared")
    if (out / "finalize.json").exists():
        raise AcceptError(f"accept/{name}: already finalized")
    record = load_json(prepared)
    if record["result"] != "PASS":
        raise AcceptError(f"accept/{name}: prepare is {record['result']}")
    prepare_sha = sha256_file(prepared)
    reviews = [load_json(p) for p in sorted((out / "reviews").glob("*.json"))] if (out / "reviews").is_dir() else []
    failing = [r["reviewer"] for r in reviews if r["verdict"] not in PASSING]
    if failing:
        raise AcceptError(f"review verdict FAIL from {', '.join(failing)}: fix and prepare a new checkpoint")
    bound = [r for r in reviews if r["prepare_sha256"] == prepare_sha and r["sources"] == record["sources"]]
    if len(bound) < min_reviews or len(bound) != len(reviews):
        raise AcceptError(f"{len(bound)} reviews bound to this prepare (of {len(reviews)}); need {min_reviews}, all bound")
    if source_hashes([{"source": s} for s in record["sources"]]) != record["sources"]:
        raise AcceptError("sources changed since prepare")
    if sha256_file(PROFILES) != record["profiles_sha256"] or sha256_file(CATALOGUE) != record["catalogue_sha256"]:
        raise AcceptError("profiles or catalogue changed since prepare")
    if sha256_file(MATCHES) != record["base_matches_sha256"]:
        raise AcceptError("the accepted ledger changed since prepare (another checkpoint finalized?)")
    verify_history()

    ledger = (out / "matches.json").read_bytes()
    if hashlib.sha256(ledger).hexdigest() != record["ledger_sha256"]:
        raise AcceptError("accept/<checkpoint>/matches.json changed since prepare")
    history = read_history()
    credited = record["builds"][0]["credited"]
    entry = {
        "seq": len(history) + 1,
        "checkpoint": name,
        "recorded_utc": now(),
        "matches_sha256": record["ledger_sha256"],
        "sources": record["sources"],
        "credited_functions": credited["functions"],
        "credited_bytes": credited["bytes"],
        "denominator_functions": credited["catalogue_functions"],
        "denominator_bytes": credited["catalogue_bytes"],
        "catalogue_sha256": record["catalogue_sha256"],
        "prepare_sha256": prepare_sha,
        "outputs": record["builds"][0]["outputs"],
        "reviews": [{"reviewer": r["reviewer"], "family": r["family"], "verdict": r["verdict"]} for r in bound],
        "prev": history[-1]["entry_sha256"] if history else None,
    }
    entry["entry_sha256"] = entry_hash(entry)
    MATCHES.write_bytes(ledger)
    with HISTORY.open("a", encoding="utf-8", newline="\n") as f:
        f.write(json.dumps(entry, ensure_ascii=False, sort_keys=True) + "\n")
    write_exclusive(out / "finalize.json", {"checkpoint": name, "finalized_utc": entry["recorded_utc"], "entry_sha256": entry["entry_sha256"]})
    return entry


# -- verify ------------------------------------------------------------------------------------------


def verify_history() -> list[dict]:
    history = read_history()
    prev = None
    for i, e in enumerate(history, 1):
        if e["seq"] != i or e["prev"] != prev or e["entry_sha256"] != entry_hash(e):
            raise AcceptError(f"progress history broken at seq {i}")
        if not (ACCEPT / e["checkpoint"] / "finalize.json").is_file():
            raise AcceptError(f"seq {i}: accept/{e['checkpoint']}/finalize.json missing")
        prev = e["entry_sha256"]
    ledger = load_json(MATCHES)
    if history:
        if sha256_file(MATCHES) != history[-1]["matches_sha256"]:
            raise AcceptError("config/matches.json is not the last finalized ledger")
        if source_hashes(ledger["units"]) != history[-1]["sources"]:
            raise AcceptError("a credited source differs from its accepted hash")
    elif ledger["units"]:
        raise AcceptError("config/matches.json credits units but no checkpoint was ever finalized")
    return history


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("prepare")
    p.add_argument("--checkpoint", required=True)
    p.add_argument("--candidates", type=Path, required=True)
    r = sub.add_parser("review")
    r.add_argument("--checkpoint", required=True)
    r.add_argument("--reviewer", required=True)
    r.add_argument("--family", required=True)
    r.add_argument("--verdict", required=True)
    r.add_argument("--notes", type=Path, required=True)
    f = sub.add_parser("finalize")
    f.add_argument("--checkpoint", required=True)
    f.add_argument("--min-reviews", type=int, default=1)
    sub.add_parser("verify")
    args = parser.parse_args(argv)
    sys.path.insert(0, str(ROOT / "tools"))
    try:
        if args.cmd == "prepare":
            rec = prepare(args.checkpoint, args.candidates)
            print(f"{rec['result']}: {rec['checks']}; suite {rec['suite']['summary']}")
            return 0 if rec["result"] == "PASS" else 1
        if args.cmd == "review":
            rec = review(args.checkpoint, args.reviewer, args.family, args.verdict, args.notes)
            print(f"recorded {rec['verdict']} from {rec['reviewer']}")
        elif args.cmd == "finalize":
            e = finalize(args.checkpoint, args.min_reviews)
            print(f"seq {e['seq']} {e['checkpoint']}: {e['credited_functions']}/{e['denominator_functions']} functions, "
                  f"{e['credited_bytes']}/{e['denominator_bytes']} bytes")
        else:
            h = verify_history()
            print(f"ok: {len(h)} accepted checkpoints; ledger bound")
    except AcceptError as e:
        print(f"error: {e}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
