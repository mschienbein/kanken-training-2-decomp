"""Probe c008 interfaces without changing frozen checkpoint evidence.

Run from this worktree with the main worktree's Python interpreter:
  .../.venv/bin/python scratch/integrator/c008-r2/probe_contracts.py --run NAME

Every c008 new/revised unit is covered. Additional existing units are selected
when they mention a symbol declared by a c008 revised header. Relevant canonical
headers are included before each definition; C units also get a C++ C-linkage
probe. A failed C++-of-C probe can be a language difference, never an automatic
repair. Original instructions/bytes are never printed or serialized.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]
OUT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "tools"))
import toolchain

SYMBOL = re.compile(r"\b(?:func|data|bss)_[0-9a-fA-F]{8}\b")
DEFERRED = {"src/wave12/func_0016b6a4.c": "F01: erased cleanup identity is not established; canonical FileInputStream is not U8Directory"}


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def save(path: Path, value: object) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("x", encoding="utf-8") as stream:
        json.dump(value, stream, indent=2)
        stream.write("\n")


def compile_probe(source: Path, profile: toolchain.Profile, directory: Path) -> dict:
    launcher = toolchain.wibo_path()
    command = ([str(launcher)] if launcher else []) + [str(toolchain.armcc_exe(profile.compiler)), "-c", *profile.flags]
    for include in (ROOT / "include", toolchain.compiler_root(profile.compiler) / "include"):
        command += ["-I", str(include)]
    command += ["-o", str(directory / "probe.o"), str(source)]
    proc = subprocess.run(command, cwd=ROOT, text=True, capture_output=True, timeout=300, env=os.environ | {"LC_ALL": "C"})
    (directory / "diagnostic.txt").write_text(proc.stdout + proc.stderr, encoding="utf-8")
    return {"compiler_exit": proc.returncode, "command": command, "diagnostic": str((directory / "diagnostic.txt").relative_to(ROOT)), "diagnostic_codes": sorted(set(re.findall(r"(?:error|warning):?\s+#(\d+)", proc.stdout + proc.stderr, re.IGNORECASE)))}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--run", required=True, help="new output subdirectory; existing runs are never overwritten")
    args = parser.parse_args()
    directory = OUT / "probes" / args.run
    directory.mkdir(parents=True, exist_ok=False)
    prepared = json.loads((ROOT / "accept/c008/prepare.json").read_text())
    ledger = json.loads((ROOT / "accept/c008/matches.json").read_text())
    profiles = toolchain.load_profiles()
    required = set(prepared["new_units"]) | set(prepared["revised_units"])
    headers = {str(path.relative_to(ROOT)): set(SYMBOL.findall(path.read_text())) for path in sorted((ROOT / "include").rglob("*")) if path.is_file()}
    revised_symbols = set().union(*(headers[path] for path in prepared["revised_headers"]))
    selected = []
    rows = []
    for unit in ledger["units"]:
        source = unit["source"]
        if source in DEFERRED:
            rows.append({"source": source, "profile": unit["profile"], "required": True, "disposition": "deferred", "reason": DEFERRED[source]})
            continue
        text = (ROOT / source).read_text()
        names = set(SYMBOL.findall(text))
        if source not in required and not names & revised_symbols:
            continue
        extra = [header for header, symbols in headers.items() if names & symbols]
        selected.append({"source": source, "profile": unit["profile"], "required": source in required, "functions": unit["functions"], "headers": extra, "source_sha256": digest(ROOT / source)})
    save(directory / "selection.json", {"prepared_sha256": digest(ROOT / "accept/c008/prepare.json"), "selection": {"linker_model": "armlink5-inline-branchnop-v2", "literal_model": "none"}, "required_units": len(required), "header_sha256": {path: digest(ROOT / path) for path in headers}, "units": selected, "deferred": rows})
    for unit in selected:
        suffix = Path(unit["source"]).suffix
        for language in (["native", "cxx-c-linkage"] if suffix == ".c" else ["native"]):
            subdir = directory / Path(unit["source"]).with_suffix("") / language
            subdir.mkdir(parents=True)
            path = subdir / ("probe.cpp" if language == "cxx-c-linkage" or suffix == ".cpp" else "probe.c")
            body = "".join('#include "' + header.removeprefix("include/") + '"\n' for header in unit["headers"])
            if language == "cxx-c-linkage":
                body += 'extern "C" {\n'
            body += '#include "' + str(ROOT / unit["source"]) + '"\n'
            if language == "cxx-c-linkage":
                body += '}\n'
            path.write_text(body, encoding="utf-8")
            result = dict(unit, language=language, **compile_probe(path, profiles[unit["profile"]], subdir))
            rows.append(result)
            with (directory / "exits.jsonl").open("a", encoding="utf-8") as stream:
                stream.write(json.dumps(result) + "\n")
    failures = [row for row in rows if row.get("compiler_exit", 0)]
    summary = {"required_units": len(required), "required_compiled_units": len({r["source"] for r in rows if r.get("required") and "compiler_exit" in r}), "deferred_units": len(DEFERRED), "additional_units": sum(not u["required"] for u in selected), "compile_commands": sum("compiler_exit" in r for r in rows), "failures": failures, "rows": rows}
    save(directory / "results.json", summary)
    print(json.dumps({key: summary[key] for key in ("required_units", "required_compiled_units", "deferred_units", "additional_units", "compile_commands")} | {"failures": [{"source": r["source"], "language": r["language"], "codes": r["diagnostic_codes"]} for r in failures]}))
    return int(bool(failures))


if __name__ == "__main__":
    raise SystemExit(main())
