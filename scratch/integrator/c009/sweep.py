"""Sequential per-unit checkpoint evidence; never builds or changes acceptance state."""
import hashlib
import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
DEST = ROOT / "scratch/integrator/c009"
PYTHON = ROOT.parents[2] / ".venv/bin/python"
sys.path.insert(0, str(ROOT / "tools"))
from argcheck import resolve


def main():
    phase = sys.argv[1]
    handoff = json.loads((DEST / "HANDOFF.json").read_text())
    scope_path = ROOT / handoff["current_required_scope"]
    scope = json.loads(scope_path.read_text())
    selection = scope["selection"]
    units = {u["source"]: u for u in scope["units"]}
    (DEST / phase).mkdir(parents=True, exist_ok=False)
    names = json.loads((ROOT / "config/names.json").read_text())["symbols"]
    rows = []
    for i, unit in enumerate(units.values()):
        source = unit["source"]
        output = DEST / phase / source.removeprefix("src/").rsplit(".", 1)[0]
        command = [str(PYTHON), "tools/match.py", source, "--profile", unit["profile"],
                   "--linker-model", selection["linker_model"],
                   "--literal-model", selection["literal_model"],
                   "--show", "0", "--out", str(output)]
        for helper in unit.get("extra_sources", []):
            command += ["--extra-source", helper]
        proc = subprocess.run(command, cwd=ROOT, text=True, capture_output=True)
        receipt = output / (Path(source).stem + ".result.json")
        record = json.loads(receipt.read_text()) if receipt.exists() else None
        results = {f["address"]: f["status"] for f in record["functions"]} if record else {}
        expected = [resolve(f, names) for f in unit["functions"]]
        complete = all(results.get(a) == "match" for a in expected)
        rows.append({"source": source, "profile": unit["profile"], "functions": unit["functions"],
                     "extra_sources": unit.get("extra_sources", []), "command": command,
                     "exit_code": proc.returncode, "credited_functions_match": complete,
                     "output": proc.stdout + proc.stderr, "receipt": str(receipt.relative_to(ROOT)) if record else None,
                     "source_sha256": hashlib.sha256((ROOT / source).read_bytes()).hexdigest()})
        report = {"phase": phase, "catalogue_sha256": hashlib.sha256((ROOT / "config/catalogue.json").read_bytes()).hexdigest(),
                  **selection,
                  "scope_sha256": hashlib.sha256(scope_path.read_bytes()).hexdigest(),
                  "total": len(units), "completed": len(rows), "match": sum(r["exit_code"] == 0 and r["credited_functions_match"] for r in rows),
                  "rows": rows}
        (DEST / (phase + ".json")).write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps({k: v for k, v in report.items() if k != "rows"}))
    return 0 if all(r["exit_code"] == 0 and r["credited_functions_match"] for r in rows) else 1


if __name__ == "__main__":
    raise SystemExit(main())
