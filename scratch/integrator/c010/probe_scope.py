"""Canonical native-definition/signature probes using the c008-r2 probe API.

Compiler-only checks; never a full build, acceptance or independent review.
Every failed command is preserved under a new --run name.
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
sys.path.insert(0, str(ROOT / "scratch/integrator/c008-r2"))
from probe_contracts import compile_probe, save
import interfaces
import toolchain
from pycparser import c_ast, c_generator, c_parser

SYMBOL = re.compile(r"\b(?:func|data|bss)_[0-9a-fA-F]{8}\b")


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--run", required=True)
    parser.add_argument("--manifest", type=Path, required=True)
    parser.add_argument("--signatures", action="store_true")
    args = parser.parse_args()
    directory = OUT / "probes" / args.run
    directory.mkdir(parents=True, exist_ok=False)
    manifest = json.loads((ROOT / args.manifest).read_text())
    profiles = toolchain.load_profiles()
    headers = {str(path.relative_to(ROOT)): set(SYMBOL.findall(path.read_text()))
               for path in sorted((ROOT / "include").rglob("*")) if path.is_file()}
    generator = c_generator.CGenerator()
    rows = []
    for unit in manifest["units"]:
        source = ROOT / unit["source"]
        if args.signatures and source.suffix != ".c":
            continue
        text = source.read_text()
        names = set(SYMBOL.findall(text))
        canonical = [header for header, symbols in headers.items() if names & symbols]
        subdir = directory / source.relative_to(ROOT).with_suffix("")
        subdir.mkdir(parents=True)
        suffix = ".cpp" if args.signatures else source.suffix
        path = subdir / ("signatures" + suffix if args.signatures else "probe" + suffix)
        body = "".join('#include "' + header.removeprefix("include/") + '"\n' for header in canonical)
        metadata = dict(unit, headers=canonical, source_sha256=digest(source),
                        language="cxx-c-linkage-signature" if args.signatures else "native")
        try:
            if args.signatures:
                prepared = interfaces.prepare(interfaces.preprocess(interfaces.Unit(source, profiles[unit["profile"]]), ROOT), source)
                tree = c_parser.CParser().parse(prepared.text, filename=str(source))
                definitions = [node.decl for node in tree.ext if isinstance(node, c_ast.FuncDef) and node.decl.name in unit["functions"]]
                if {node.name for node in definitions} != set(unit["functions"]):
                    raise ValueError("Not every credited C definition was parsed")
                metadata["signatures"] = [generator.visit(node).replace("_Bool", "bool") for node in definitions]
                body = '#include <stdbool.h>\n' + body
                body += 'extern "C" {\n' + "\n".join(signature + ";" for signature in metadata["signatures"]) + "\n}\n"
            else:
                body += '#include "' + str(source) + '"\n'
            path.write_text(body)
            metadata.update(compile_probe(path, profiles[unit["profile"]], subdir))
        except (interfaces.InterfaceError, toolchain.ToolchainError, c_parser.ParseError, ValueError) as error:
            metadata.update({"preprocess_error": str(error), "compiler_exit": None})
        rows.append(metadata)
        with (directory / "exits.jsonl").open("a") as stream:
            stream.write(json.dumps(metadata) + "\n")
    failures = [row for row in rows if row.get("compiler_exit") != 0]
    summary = {"manifest": str(args.manifest), "manifest_sha256": digest(ROOT / args.manifest),
               "headers_sha256": {name: digest(ROOT / name) for name in headers},
               "units": len(rows), "failures": len(failures), "rows": rows}
    save(directory / "results.json", summary)
    print(json.dumps({"units": len(rows), "failures": len(failures),
                      "compiler_failures": sum(bool(r.get("compiler_exit")) for r in rows),
                      "preprocess_failures": sum("preprocess_error" in r for r in rows)}))
    return int(bool(failures))


if __name__ == "__main__":
    raise SystemExit(main())
