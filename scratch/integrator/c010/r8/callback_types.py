#!/usr/bin/env python3
"""Extract project callback declarations; not an interfaces or argcheck gate.

Only declarations/types, callsites, counts and hashes are emitted. ARMCC's
preprocessed library headers remain private process memory and are never saved.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT / "tools"))
import argcheck
import interfaces
import toolchain


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def callback_parameters(typ: interfaces.CType) -> list[dict]:
    if typ.kind != "function" or typ.args[1] is None:
        return []
    result = []
    for index, parameter in enumerate(typ.args[1]):
        if parameter.kind == "pointer" and parameter.args[0].kind == "function":
            result.append({"index": index, "type": interfaces.describe(parameter.args[0]),
                           "type_key": interfaces.type_key(parameter.args[0])})
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--graph", type=Path, required=True)
    parser.add_argument("--prior-scope", type=Path, required=True)
    parser.add_argument("--source-census", type=Path)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    out = ROOT / args.out
    if out.exists():
        raise RuntimeError(f"Immutable output exists: {out}")
    graph = json.loads((ROOT / args.graph).read_text())
    prior = json.loads((ROOT / args.prior_scope).read_text())
    units = list(prior["units"])
    helper_profiles = {path: unit["profile"] for unit in units for path in unit.get("extra_sources", [])}
    units.extend({"source": path, "profile": profile, "functions": []}
                 for path, profile in sorted(helper_profiles.items()))
    names = set(graph["names_by_address"][key][0] for key in graph["names_by_address"])
    names.update(name for values in graph["names_by_address"].values() for name in values)
    candidates = list(graph["passed_function_address_candidates"])
    for row in graph["original_call_graph"]:
        for register, value in row.get("primary_entry_arguments", {}).items():
            if value["kind"] == "function_address":
                candidates.append({"caller": row["caller"], "site": row["site"], "callee": row["target"],
                                   "argument_location": register, "provider": value,
                                   "provenance": "primary_entry"})
    relevant = {"func_" + row["provider"]["owner"][2:] for row in candidates}
    relevant.update("func_" + row["callee"][2:] for row in candidates if row["callee"] is not None)
    if args.source_census:
        census = json.loads((ROOT / args.source_census).read_text())
        relevant.update(provider["name"] for row in census["source_api_mentions"]
                        for callback in row["callbacks"] for provider in callback["providers"])
    profiles = toolchain.load_profiles()
    declarations, native, errors, coverage, callback_apis = [], [], [], [], []
    for unit in units:
        identities = set(unit.get("functions", []))
        if identities and not identities.intersection(relevant):
            continue
        path = ROOT / unit["source"]
        source = path.read_text()
        if not identities:
            names.update(re.findall(r"\b([A-Za-z_][A-Za-z_0-9]*)\s*\(", source))
        if path.suffix == ".cpp":
            # Native member definitions are not C free functions. Do not feed
            # them through a fake C declaration or infer an ABI from an alias.
            definitions = []
            for identity in identities.intersection(relevant):
                match = re.search(r'(?m)^(?:extern\s+"C"\s+)?([^;{}\n]+?)\b' + re.escape(identity)
                                  + r'\s*\(([^;{}]*)\)\s*\{', source)
                if match:
                    signature = match.group(1).strip() + " " + identity + "(" + match.group(2) + ");"
                    try:
                        ds = argcheck.parse_declarations(signature, path, ROOT, {identity})
                        for declaration in ds:
                            declarations.append({"name": declaration.name, "definition": True,
                                                 "source": unit["source"], "site": {"source": unit["source"], "line": source.count("\n", 0, match.start()) + 1},
                                                 "type": interfaces.describe(declaration.type), "type_key": interfaces.type_key(declaration.type),
                                                 "extraction": "C-compatible extern/free C++ signature", "source_sha256": digest(path)})
                        definitions.append(identity)
                    except Exception as exc:
                        declarations.append({"name": identity, "definition": True, "source": unit["source"],
                                             "site": {"source": unit["source"], "line": source.count("\n", 0, match.start()) + 1},
                                             "type": signature, "type_key": None, "extraction": "C++ free function: exact nominal source signature, not a native constructor/destructor",
                                             "source_sha256": digest(path)})
                else:
                    native.append({"identity": identity, "source": unit["source"], "reason": "Native member definition; use emitted symbol and C++ ABI, not a C shadow declaration", "source_sha256": digest(path)})
            coverage.append({"source": unit["source"], "language": "C++", "definitions_extracted": definitions,
                             "source_sha256": digest(path)})
            continue
        try:
            text = interfaces.preprocess(interfaces.Unit(path, profiles[unit["profile"]]), ROOT)
            ds = argcheck.parse_declarations(text, path, ROOT, names)
            del text
            for declaration in ds:
                if declaration.name in relevant or callback_parameters(declaration.type):
                    row = {"name": declaration.name, "definition": declaration.definition,
                           "source": unit["source"], "site": declaration.site,
                           "type": interfaces.describe(declaration.type), "type_key": interfaces.type_key(declaration.type),
                           "source_sha256": digest(path)}
                    declarations.append(row)
                    params = callback_parameters(declaration.type)
                    if params:
                        callback_apis.append({**row, "callback_parameters": params})
            coverage.append({"source": unit["source"], "language": "C", "declaration_count": len(ds), "exit_code": 0,
                             "source_sha256": digest(path)})
        except Exception as exc:
            # Only the exception class is exported: preprocessor diagnostics
            # may contain licensed header fragments and must remain private.
            errors.append({"source": unit["source"], "error_class": type(exc).__name__})
    result = {"schema_version": 1, "driver": str(Path(__file__).relative_to(ROOT)), "driver_sha256": digest(Path(__file__)),
              "graph": str(args.graph), "graph_sha256": digest(ROOT / args.graph), "prior_scope": str(args.prior_scope),
              "prior_scope_sha256": digest(ROOT / args.prior_scope), "candidate_provider_count": len(relevant),
              "coverage": coverage, "declarations": declarations, "callback_api_declarations": callback_apis,
              "native_cpp_definitions": native, "errors": errors,
              "privacy": "No image bytes, instruction encodings, disassembly or preprocessed library payload emitted."}
    if args.source_census:
        result["source_census"] = str(args.source_census)
        result["source_census_sha256"] = digest(ROOT / args.source_census)
    out.write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps({"output": str(args.out), "units": len(coverage), "declarations": len(declarations),
                      "callback_apis": len(callback_apis), "native_cpp": len(native), "errors": len(errors)}))
    return int(bool(errors))


if __name__ == "__main__":
    raise SystemExit(main())
