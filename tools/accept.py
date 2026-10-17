"""Accept matched source as a checkpoint: prepare, independent review, finalize once.

    python tools/accept.py prepare  --checkpoint c001-pilot --candidates config/candidates/c001-pilot.json
    python tools/accept.py review   --checkpoint c001-pilot --record review.json
    python tools/accept.py finalize --checkpoint c001-pilot [--min-reviews 1]
    python tools/accept.py verify                                        # CI-safe, no ROM

prepare: the denominator must verify; the candidate units are added to the accepted ledger
(config/matches.json) into accept/<checkpoint>/matches.json; two fresh tools/build.py builds of
that ledger must PASS with identical outputs, and the full test suite must pass. Everything is
recorded in accept/<checkpoint>/prepare.json (exclusive create; a FAIL is kept, never retried
under the same name).

review: validates one reviewer's structured record (validate_review: a verdict row for every new,
revised or removed-helper-dependent caller, finding ids, severities, verdict consistency) and
seals it bound to the exact prepare.json and source hashes. The reviewer must re-derive
results, not trust prepare.json.

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


HEADER_SUFFIXES = {".h", ".hh", ".hpp", ".inc"}


def header_hashes() -> dict[str, str]:
    """Every project header a unit could include: all files under include/ plus header files
    under src/. Bound like sources (gate G1, docs/GATES.md v1.3); over-binding unused headers is
    deliberate, since one header edit can change any number of objects."""
    paths = [p for p in (ROOT / "include").rglob("*") if p.is_file()] if (ROOT / "include").is_dir() else []
    if (ROOT / "src").is_dir():
        paths += [p for p in (ROOT / "src").rglob("*") if p.is_file() and p.suffix in HEADER_SUFFIXES]
    return {p.relative_to(ROOT).as_posix(): sha256_file(p) for p in sorted(paths)}


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
    import test_suite

    result = test_suite.run()
    return {
        "command": "python tools/test_suite.py",
        "returncode": result["returncode"],
        "tests_run": result["tests_run"],
        "skipped": result["skipped"],
        "failed_modules": result["failed_modules"],
        "seconds": result["seconds"],
        "summary": f"{result['tests_run']} tests, {result['skipped']} skipped, {len(result['failed_modules'])} failed modules",
    }


def clean_commit() -> str:
    """The HEAD commit, refusing a working tree with any uncommitted or untracked (non-ignored) file.

    Gates run on exactly one committed tree, so no in-progress edit (an agent's, or the
    integrator's) can leak into a build or the suite.
    """
    def git(*args: str) -> str:
        proc = subprocess.run(["git", *args], cwd=ROOT, capture_output=True, text=True, check=False)
        if proc.returncode:
            raise AcceptError(f"git {' '.join(args)} failed: {proc.stderr.strip()}")
        return proc.stdout

    dirty = git("status", "--porcelain", "--untracked-files=all").strip()
    if dirty:
        raise AcceptError("working tree is not clean; commit or move these first:\n" + dirty)
    return git("rev-parse", "HEAD").strip()


def linker_review_summary(receipt: dict, changed_units: set[str], revised_headers: list[str]) -> tuple[dict, dict]:
    """A zero-credit removed-helper unit is still a review dependency of its credited callers.

    Headers are deliberately over-bound like header_hashes: a changed shared contract reopens
    every R1 caller, even when the caller source and generated bytes remain unchanged.
    """
    rewrites, dependencies = {}, {}
    comdat_sources: dict[str, set[str]] = {}
    for unit in receipt.get("units", []):
        for helper in unit.get("removed_callees", []):
            if helper.get("comdat"):
                comdat_sources.setdefault(helper["comdat"], set()).add(unit["source"])
    for unit in receipt.get("units", []):
        for function, rows in sorted(unit.get("linker_rewrites", {}).items()):
            if not rows:
                continue
            helpers = {row["removed_callee"]["object"] for row in rows if row.get("removed_callee")}
            for row in rows:
                helpers.update(comdat_sources.get(row.get("removed_callee", {}).get("comdat"), ()))
            changed_helpers = sorted(helpers & changed_units)
            if helpers and (changed_helpers or revised_headers):
                dependencies[function] = {
                    "source": unit["source"], "revised_helpers": changed_helpers,
                    "revised_headers": revised_headers,
                }
            if unit["source"] in changed_units or function in dependencies:
                rewrites[function] = rows
    return rewrites, dependencies


def prepare(name: str, candidates_path: Path, allow_unchecked_cpp: bool = False) -> dict:
    import build as build_mod  # imported lazily: build needs the private inputs
    import denominator
    import interfaces

    out = checkpoint_dir(name)
    commit = clean_commit()
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
    # Accepted material edited since the last accepted entry is reviewed again like new work:
    # revised units' functions need review rows (new_functions); revised headers are listed.
    history = read_history()
    accepted_sources = history[-1]["sources"] if history else {}
    accepted_headers = history[-1].get("headers", {}) if history else {}
    revised_units = [u["source"] for u in base["units"] if accepted_sources.get(u["source"]) != sha256_file(ROOT / u["source"])]
    current_headers = header_hashes()
    revised_headers = sorted(h for h in set(current_headers) | set(accepted_headers)
                             if history and current_headers.get(h) != accepted_headers.get(h))

    # Mechanical interface gate before any review: declaration/definition agreement, struct
    # layouts, const contracts and header policy across the whole ledger.
    iface = interfaces.audit(interfaces.select_units([], out / "matches.json", ROOT), ROOT)
    write_exclusive(out / "interfaces.json", iface)
    iface_ok = not interfaces.failed(iface, allow_unchecked_cpp)

    builds = []
    for suffix in ("a", "b"):
        try:
            # Fresh: acceptance builds never reuse cached objects, so two builds are two compiles.
            receipt = build_mod.build(f"{name}-{suffix}", out / "matches.json", fresh=True)
        except (build_mod.BuildError, RuntimeError) as e:
            receipt = {"name": f"{name}-{suffix}", "result": "FAIL", "error": str(e)}
        else:
            write_exclusive(out / "receipts" / f"{name}-{suffix}.json", receipt)
        builds.append(receipt)
    suite = run_suite()
    candidates_sources = [u["source"] for u in candidates["units"]]
    linker_rewrites, dependency_reviews = linker_review_summary(
        builds[0], set(candidates_sources) | set(revised_units), revised_headers)
    checks = {
        "build_a_pass": builds[0].get("result") == "PASS",
        "build_b_pass": builds[1].get("result") == "PASS",
        "builds_identical": builds[0].get("outputs") == builds[1].get("outputs") and "outputs" in builds[0]
        and builds[0].get("inputs", {}).get("linker_model") == builds[1].get("inputs", {}).get("linker_model")
        and builds[0].get("inputs", {}).get("literal_model", "none")
        == builds[1].get("inputs", {}).get("literal_model", "none"),
        "suite_pass": suite["returncode"] == 0,
        "interfaces_pass": iface_ok,
    }
    record = {
        "schema_version": 1,
        "checkpoint": name,
        "prepared_utc": now(),
        "commit": commit,
        "base_matches_sha256": sha256_file(MATCHES),
        "candidates": candidates_path.resolve().relative_to(ROOT.resolve()).as_posix(),
        "candidates_sha256": sha256_file(candidates_path),
        "ledger_sha256": sha256_file(out / "matches.json"),
        "sources": source_hashes(units),
        "headers": header_hashes(),
        "new_units": [u["source"] for u in candidates["units"]],
        "revised_units": revised_units,
        "revised_headers": revised_headers,
        "profiles_sha256": sha256_file(PROFILES),
        "catalogue_sha256": sha256_file(CATALOGUE),
        "denominator_version": len(denominator.read_history()),
        # Include unchanged credited callers whenever their removed helper or shared contract
        # changes. Their rewrite metadata and required rows cannot hide behind functions: [].
        "linker_model": builds[0].get("inputs", {}).get("linker_model"),
        "literal_model": builds[0].get("inputs", {}).get("literal_model", "none"),
        "literal_rebindings": {
            u["source"]: u["literal_rebindings"]
            for u in builds[0].get("units", [])
            if u.get("source") in set(candidates_sources) | set(revised_units) and u.get("literal_rebindings")
        },
        "linker_rewrites": linker_rewrites,
        "dependency_reviews": dependency_reviews,
        "builds": [
            {k: b.get(k) for k in ("name", "result", "outputs", "credited", "checks", "error") if k in b}
            for b in builds
        ],
        "suite": suite,
        "interfaces": {
            "disagreements": len(iface.get("disagreements", [])),
            "policy": len(iface.get("policy", [])),
            "unchecked": [u["source"] for u in iface.get("units", []) if not u.get("checked")],
            "unchecked_cpp_waived": allow_unchecked_cpp,
        },
        "checks": checks,
    }
    record["result"] = "PASS" if all(checks.values()) else "FAIL"
    write_exclusive(out / "prepare.json", record)
    return record


# -- review ------------------------------------------------------------------------------------------


ROW_VERDICTS = ("PASS", "QUALIFIED", "FAIL")
SEVERITIES = ("blocker", "major", "minor", "note")
FINDING_ID = re.compile(r"^F\d{2,3}$")


def validate_review(record: dict, required: dict[str, str], removed_callers: set[str] | None = None) -> None:
    """Schema v1: `required` includes new/revised functions and unchanged dependent callers.

    {"reviewer", "family", "verdict": PASS|PASS_WITH_QUALIFICATIONS|FAIL,
     "functions": [{"function", "source", "verdict": PASS|QUALIFIED|FAIL, "method", "finding_ids": []}],
     "findings": [{"id": "F01", "severity": blocker|major|minor|note, "subject", "summary"}],
     "qualifications": [str]}
    R1 callers must explicitly qualify removed-helper identity provenance even if unchanged.
    """
    for key in ("reviewer", "family", "verdict", "functions", "findings", "qualifications"):
        if key not in record:
            raise AcceptError(f"review record lacks {key!r}")
    if record["verdict"] not in (*PASSING, "FAIL"):
        raise AcceptError(f"verdict must be one of {', '.join((*PASSING, 'FAIL'))}")
    if not NAME.match(record["reviewer"].lower()):
        raise AcceptError(f"reviewer name {record['reviewer']!r} must match {NAME.pattern}")
    findings = {}
    for f in record["findings"]:
        if not FINDING_ID.match(f.get("id", "")) or f["id"] in findings:
            raise AcceptError(f"finding id {f.get('id')!r} must be unique and look like F01")
        if f.get("severity") not in SEVERITIES or not f.get("subject") or not f.get("summary"):
            raise AcceptError(f"{f['id']}: needs severity ({'/'.join(SEVERITIES)}), subject and summary")
        findings[f["id"]] = f
    rows = {}
    for row in record["functions"]:
        name = row.get("function")
        if name in rows:
            raise AcceptError(f"{name}: reviewed twice")
        if required.get(name) != row.get("source"):
            raise AcceptError(f"{name}: not a review-required function of this checkpoint (or wrong source)")
        if row.get("verdict") not in ROW_VERDICTS or not row.get("method"):
            raise AcceptError(f"{name}: needs verdict ({'/'.join(ROW_VERDICTS)}) and the method used to re-derive it")
        if name in (removed_callers or ()) and row["verdict"] == "PASS":
            raise AcceptError(f"{name}: removed-helper identity provenance requires a QUALIFIED (or FAIL) row")
        unknown = [i for i in row.get("finding_ids", []) if i not in findings]
        if unknown:
            raise AcceptError(f"{name}: unknown finding ids {unknown}")
        rows[name] = row
    missing = sorted(set(required) - set(rows))
    if missing:
        raise AcceptError(f"review does not cover {len(missing)} functions: {', '.join(missing[:5])}")
    blocking = any(r["verdict"] == "FAIL" for r in rows.values()) or any(f["severity"] == "blocker" for f in findings.values())
    qualified = any(r["verdict"] == "QUALIFIED" for r in rows.values()) or any(f["severity"] == "major" for f in findings.values())
    if blocking and record["verdict"] != "FAIL":
        raise AcceptError("a FAIL row or blocker finding requires overall verdict FAIL")
    if qualified and record["verdict"] == "PASS":
        raise AcceptError("QUALIFIED rows or major findings require PASS_WITH_QUALIFICATIONS or FAIL")
    if record["verdict"] == "PASS_WITH_QUALIFICATIONS" and not record["qualifications"]:
        raise AcceptError("PASS_WITH_QUALIFICATIONS needs at least one stated qualification")


def new_functions(out: Path, prepared: dict) -> dict[str, str]:
    """All review-required functions, including unchanged removed-helper-dependent callers."""
    ledger = load_json(out / "matches.json")
    new = set(prepared["new_units"]) | set(prepared.get("revised_units", ()))
    required = {fn: u["source"] for u in ledger["units"] if u["source"] in new for fn in u["functions"]}
    credited = {fn: u["source"] for u in ledger["units"] for fn in u["functions"]}
    for function, dependency in prepared.get("dependency_reviews", {}).items():
        if credited.get(function) != dependency.get("source"):
            raise AcceptError(f"{function}: removed-helper review dependency is not a credited caller")
        required[function] = dependency["source"]
    return required


def removed_review_callers(prepared: dict) -> set[str]:
    return set(prepared.get("dependency_reviews", {})) | {
        function for function, rows in prepared.get("linker_rewrites", {}).items()
        if any(row.get("removed_callee") for row in rows)
    }


def review(name: str, record_path: Path) -> dict:
    out = checkpoint_dir(name)
    prepared = out / "prepare.json"
    if not prepared.is_file():
        raise AcceptError(f"accept/{name}: not prepared")
    prepare_record = load_json(prepared)
    record = load_json(record_path)
    validate_review(record, new_functions(out, prepare_record), removed_review_callers(prepare_record))
    # A review binds the exact files prepare bound: the current hash of each prepared source and
    # header (missing → None). Headers added since prepare are listed but do not unbind the
    # review; finalize still refuses them (a new header could shadow one later on the path).
    current = header_hashes()
    entry = {
        "schema_version": 1,
        "checkpoint": name,
        **record,
        "reviewed_utc": now(),
        "prepare_sha256": sha256_file(prepared),
        "sources": source_hashes([{"source": s} for s in prepare_record["sources"]]),
        "headers": {h: current.get(h) for h in prepare_record.get("headers", {})},
        "headers_added_since_prepare": sorted(set(current) - set(prepare_record.get("headers", {}))),
        "record_sha256": sha256_file(record_path),
    }
    write_exclusive(out / "reviews" / f"{record['reviewer'].lower()}.json", entry)
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
    bound = [r for r in reviews if r["prepare_sha256"] == prepare_sha and r["sources"] == record["sources"]
             and r.get("headers") == record.get("headers")]
    if len(bound) < min_reviews or len(bound) != len(reviews):
        raise AcceptError(f"{len(bound)} reviews bound to this prepare (of {len(reviews)}); need {min_reviews}, all bound")
    required = new_functions(out, record)
    for reviewed in bound:
        validate_review(reviewed, required, removed_review_callers(record))
    if source_hashes([{"source": s} for s in record["sources"]]) != record["sources"]:
        raise AcceptError("sources changed since prepare")
    if header_hashes() != record.get("headers", {}):
        raise AcceptError("headers changed since prepare")
    if sha256_file(PROFILES) != record["profiles_sha256"] or sha256_file(CATALOGUE) != record["catalogue_sha256"]:
        raise AcceptError("profiles or catalogue changed since prepare")
    if sha256_file(MATCHES) != record["base_matches_sha256"]:
        raise AcceptError("the accepted ledger changed since prepare (another checkpoint finalized?)")
    # Revisions prepare declared are pinned to their prepared hashes above ("sources/headers
    # changed since prepare"); every other accepted file must still equal its accepted hash.
    verify_history(revised_units=record.get("revised_units", ()), revised_headers=record.get("revised_headers", ()))

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
        "headers": record.get("headers", {}),
        "credited_functions": credited["functions"],
        "credited_bytes": credited["bytes"],
        "denominator_functions": credited["catalogue_functions"],
        "denominator_bytes": credited["catalogue_bytes"],
        "catalogue_sha256": record["catalogue_sha256"],
        "linker_model": record.get("linker_model"),
        "literal_model": record.get("literal_model", "none"),
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


def verify_history(revised_units=(), revised_headers=()) -> list[dict]:
    """Check the hash chain and that the accepted ledger, credited sources and accepted headers
    are unchanged. `revised_*` (finalize only) exempts exactly the revisions a prepare declared."""
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
        revised = set(revised_units)
        current_sources = source_hashes(ledger["units"])
        drifted = sorted(s for s, sha in history[-1]["sources"].items() if s not in revised and current_sources.get(s) != sha)
        if drifted or set(current_sources) != set(history[-1]["sources"]):
            raise AcceptError(f"a credited source differs from its accepted hash: {', '.join(drifted) or 'unit set'}")
        current = header_hashes()
        changed = sorted(h for h, sha in history[-1].get("headers", {}).items()
                         if h not in set(revised_headers) and current.get(h) != sha)
        if changed:
            raise AcceptError(f"an accepted header changed or disappeared: {', '.join(changed)}")
    elif ledger["units"]:
        raise AcceptError("config/matches.json credits units but no checkpoint was ever finalized")
    return history


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("prepare")
    p.add_argument("--checkpoint", required=True)
    p.add_argument("--candidates", type=Path, required=True)
    p.add_argument("--allow-unchecked-cpp", action="store_true",
                   help="waive the mechanical interface check for C++ units (recorded; reviewers check them by hand)")
    r = sub.add_parser("review")
    r.add_argument("--checkpoint", required=True)
    r.add_argument("--record", type=Path, required=True, help="review record JSON (schema in validate_review)")
    f = sub.add_parser("finalize")
    f.add_argument("--checkpoint", required=True)
    f.add_argument("--min-reviews", type=int, default=1)
    sub.add_parser("verify")
    args = parser.parse_args(argv)
    sys.path.insert(0, str(ROOT / "tools"))
    try:
        if args.cmd == "prepare":
            rec = prepare(args.checkpoint, args.candidates, args.allow_unchecked_cpp)
            print(f"{rec['result']}: {rec['checks']}; suite {rec['suite']['summary']}")
            return 0 if rec["result"] == "PASS" else 1
        if args.cmd == "review":
            rec = review(args.checkpoint, args.record)
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
