"""Sequential canonical native/C-signature probes; no linking or acceptance changes.

Reuses the reviewed c008-r2 compile_probe helper with this worktree's headers.
Each output run is exclusive. Compiler failures and warnings are preserved, never
suppressed; C++-of-C bodies are not substituted for actual C signature extraction.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[3]
OUT = Path(__file__).resolve().parent
REUSED = ROOT.parent / "c008-r2/scratch/integrator/c008-r2/probe_contracts.py"
sys.path.insert(0, str(ROOT / "tools"))

import argcheck
import image
import interfaces
import toolchain
from pycparser import c_ast, c_generator, c_parser, plyparser

SPEC = importlib.util.spec_from_file_location("c008_r2_contract_probe", REUSED)
probe = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(probe)
probe.ROOT = ROOT
SYMBOL = re.compile(r"\b(?:func|data|bss)_[0-9a-fA-F]{8}\b")


def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def save(path: Path, value: object) -> None:
    with path.open("x", encoding="utf-8") as stream:
        json.dump(value, stream, indent=2)
        stream.write("\n")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--mode", choices=("native", "signatures"), required=True)
    parser.add_argument("--run", required=True)
    args = parser.parse_args()
    directory = OUT / "probes" / args.run / args.mode
    directory.mkdir(parents=True, exist_ok=False)
    ledger_path = OUT / "dry-matches.json"
    manifest_path = ROOT / "config/candidates/c009.json"
    handoff_path = OUT / "HANDOFF.json"
    ledger = json.loads(ledger_path.read_text())
    manifest = json.loads(manifest_path.read_text())
    handoff = json.loads(handoff_path.read_text())
    scope_path = ROOT / handoff["current_required_scope"]
    scope = json.loads(scope_path.read_text())
    required = {unit["source"] for unit in scope["units"]}
    project_headers = interfaces.project_headers(ROOT)
    canonical = {name for name in project_headers if name.startswith("game/") or name == "ctr/ipc.h"}
    symbols = {name: set(SYMBOL.findall(text)) for name, text in project_headers.items()}
    profiles = toolchain.load_profiles()
    names = image.load_names(ROOT / "config/names.json")
    generator = c_generator.CGenerator()
    selected = []
    for unit in ledger["units"]:
        source = ROOT / unit["source"]
        text = source.read_text()
        included = interfaces.included_headers(text, project_headers)
        if unit["source"] not in required and not included & canonical:
            continue
        if args.mode == "signatures" and source.suffix != ".c":
            continue
        mentioned = set(SYMBOL.findall(text))
        headers = sorted(name for name, bound in symbols.items() if mentioned & bound)
        selected.append(dict(unit, headers=headers, required=unit["source"] in required,
                             source_sha256=sha(source)))
    save(directory / "selection.json", {
        "mode": args.mode, "required_units": len(required), "selected_units": len(selected),
        "selection": scope["selection"],
        "inputs": {str(path.relative_to(ROOT)): sha(path) for path in
                   (ledger_path, manifest_path, scope_path, ROOT / "config/names.json",
                    ROOT / "config/catalogue.json", ROOT / "config/compiler_profiles.json")},
        "helper": str(REUSED), "helper_sha256": sha(REUSED),
        "headers": {name: sha(ROOT / name if name.startswith("src/") else ROOT / "include" / name)
                    for name in project_headers},
        "units": selected,
        "qualification": "Header-dependent predecessor units are included. Native probes check original-language bodies; signature probes check compiler-preprocessed C definitions under canonical C++ C linkage. Neither establishes original complete owner identity or indirect contracts."})
    rows = []
    for unit in selected:
        source = ROOT / unit["source"]
        subdir = directory / Path(unit["source"]).with_suffix("")
        subdir.mkdir(parents=True)
        suffix = source.suffix if args.mode == "native" else ".cpp"
        path = subdir / ("probe" + suffix)
        body = "".join(f'#include "{name}"\n' for name in unit["headers"])
        try:
            signatures = None
            if args.mode == "native":
                body += f'#include "{source}"\n'
            else:
                cooked = interfaces.prepare(interfaces.preprocess(
                    interfaces.Unit(source, profiles[unit["profile"]]), ROOT), source)
                tree = c_parser.CParser().parse(cooked.text, filename=str(source))
                expected = {argcheck.resolve(name, names) for name in unit["functions"]}
                definitions = []
                found = set()
                for node in tree.ext:
                    if not isinstance(node, c_ast.FuncDef):
                        continue
                    try:
                        address = argcheck.resolve(node.decl.name, names)
                    except argcheck.ArgcheckError:
                        continue
                    if address in expected:
                        definitions.append(node.decl)
                        found.add(address)
                if found != expected:
                    raise ValueError("Not every credited C definition was parsed")
                signatures = [generator.visit(node).replace("_Bool", "bool") for node in definitions]
                original_includes = [match[0] for match in re.finditer(
                    r'^\s*#\s*include\s*[<"][^>"\n]+[>"]', source.read_text(), re.M)]
                local_typedefs = [generator.visit(node).replace("_Bool", "bool") + ";"
                                  for node in tree.ext if isinstance(node, c_ast.Typedef)
                                  and cooked.site(node, ROOT)["source"] == unit["source"]]
                body = '#include <stdbool.h>\n' + "\n".join(original_includes) + "\n" + body
                body += 'extern "C" {\n' + "\n".join(local_typedefs) + "\n"
                body += "\n".join(signature + ";" for signature in signatures) + '\n}\n'
            path.write_text(body)
            row = dict(unit, signatures=signatures,
                       **probe.compile_probe(path, profiles[unit["profile"]], subdir))
        except (interfaces.InterfaceError, toolchain.ToolchainError, OSError, ValueError,
                plyparser.ParseError) as exc:
            row = dict(unit, compiler_exit=None, extraction_error=type(exc).__name__)
        rows.append(row)
        with (directory / "exits.jsonl").open("a", encoding="utf-8") as stream:
            stream.write(json.dumps(row) + "\n")
    failures = [row for row in rows if row.get("compiler_exit") != 0]
    diagnostics = [row for row in rows if row.get("diagnostic_codes")]
    summary = {"mode": args.mode, "selected_units": len(selected), "completed": len(rows),
               "required_units": len(required), "failures": failures,
               "diagnostics": diagnostics, "rows": rows}
    save(directory / "results.json", summary)
    print(json.dumps({key: summary[key] for key in
                      ("mode", "selected_units", "completed", "required_units")} |
                     {"failures": len(failures), "diagnostics": len(diagnostics)}))
    return int(bool(failures or diagnostics))


if __name__ == "__main__":
    raise SystemExit(main())
