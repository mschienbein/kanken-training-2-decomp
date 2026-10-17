"""Check actual C definition signatures against canonical C++ C-linkage headers.

Native bodies are checked elsewhere. This avoids treating C flexible members,
implicit void* conversions and C-only guard initialization as C++ body defects.
Signatures come from the compiler-preprocessed C AST, never guessed prototypes.
"""
from __future__ import annotations

import argparse
import json
from pathlib import Path
import re
import sys

from pycparser import c_ast, c_generator, c_parser

ROOT = Path(__file__).resolve().parents[3]
OUT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(OUT))
import interfaces
import toolchain
from probe_contracts import compile_probe, save


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--run", required=True)
    args = parser.parse_args()
    directory = OUT / "signatures" / args.run
    directory.mkdir(parents=True, exist_ok=False)
    matched = json.loads((OUT / "match-index.json").read_text())
    compiled = json.loads((OUT / "compiler-probes.json").read_text())["runs"][-1]
    headers = {row["source"]: row["headers"] for row in compiled["rows"] if row.get("language") == "native"}
    profiles = toolchain.load_profiles()
    generator = c_generator.CGenerator()
    rows = []
    for unit in matched:
        source = ROOT / unit["source"]
        if source.suffix != ".c":
            continue
        prepared = interfaces.prepare(interfaces.preprocess(interfaces.Unit(source, profiles[unit["profile"]]), ROOT), source)
        tree = c_parser.CParser().parse(prepared.text, filename=str(source))
        definitions = [node.decl for node in tree.ext if isinstance(node, c_ast.FuncDef) and node.decl.name in unit["functions"]]
        if {node.name for node in definitions} != set(unit["functions"]):
            raise ValueError(f"not every credited definition was parsed in {unit['source']}")
        subdir = directory / source.relative_to(ROOT).with_suffix("")
        subdir.mkdir(parents=True)
        path = subdir / "signatures.cpp"
        canonical = headers.get(unit["source"], ["include/" + name for name in re.findall(r'^\s*#include\s+"([^"]+)"', source.read_text(), re.MULTILINE)])
        body = '#include <stdbool.h>\n' + "".join('#include "' + header.removeprefix("include/") + '"\n' for header in canonical)
        body += 'extern "C" {\n' + "\n".join(generator.visit(node).replace("_Bool", "bool") + ";" for node in definitions) + '\n}\n'
        path.write_text(body, encoding="utf-8")
        rows.append({"source": unit["source"], "functions": unit["functions"], "profile": unit["profile"], "source_sha256": unit["source_sha256"], "signatures": [generator.visit(node) for node in definitions], **compile_probe(path, profiles[unit["profile"]], subdir)})
    failures = [row for row in rows if row["compiler_exit"]]
    save(directory / "results.json", {"units": len(rows), "failures": failures, "rows": rows})
    print(json.dumps({"units": len(rows), "failures": [{"source": row["source"], "codes": row["diagnostic_codes"]} for row in failures]}))
    return int(bool(failures))


if __name__ == "__main__":
    raise SystemExit(main())
