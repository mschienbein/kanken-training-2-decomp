"""Conservative canonical-owner, width and const propagation over original CFGs.

Uses the existing argcheck decoder and exact symbolic merge, augmented with typed
call results. Emits only types, addresses and abstract provenance, never original
instructions or bytes. Every proposed c008 function gets a coverage row. Candidate
mismatches require manual adjudication; absence is not a proof of full behavior.
"""
from __future__ import annotations

import argparse
from collections import deque
import json
from pathlib import Path
import sys

import capstone
from pycparser import c_ast, c_parser

ROOT = Path(__file__).resolve().parents[3]
OUT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "tools"))
import argcheck
import image
import interfaces
import toolchain


def save(path: Path, value: object) -> None:
    with path.open("x", encoding="utf-8") as stream:
        json.dump(value, stream, indent=2)
        stream.write("\n")


def pointer(t: interfaces.CType) -> interfaces.CType:
    return interfaces.CType("pointer", (t,))


def element_at(t: interfaces.CType, offset: int) -> interfaces.CType | None:
    if offset == 0:
        return t
    if t.kind == "array":
        width, _ = argcheck.size_align(t.args[0])
        if width and offset >= 0 and (t.args[1] is None or offset // width < t.args[1]):
            return element_at(t.args[0], offset % width)
    if t.kind == "record":
        record = t.args[0]
        position = 0
        for _, member, bits in record.members or []:
            if bits is not None:
                return None
            try:
                width, alignment = argcheck.size_align(member)
            except argcheck.ArgcheckError:
                return None
            start = 0 if record.kind == "union" else (position + alignment - 1) // alignment * alignment
            if start <= offset < start + width:
                return element_at(member, offset - start)
            position = start + width
    return None


def compatible(expected: interfaces.CType, actual: interfaces.CType) -> str | None:
    if expected.kind == actual.kind == "pointer":
        want, have = expected.args[0], actual.args[0]
        if "const" in have.quals and "const" not in want.quals:
            return "const_removed_from_forwarded_object"
        if want.kind == have.kind == "record":
            if (want.args[0].kind, want.args[0].name) != (have.args[0].kind, have.args[0].name):
                return "nominal_owner_mismatch"
        elif want.kind != "base" or want.args[0] != "void":
            if have.kind != "base" or have.args[0] != "void":
                if interfaces.type_key(want.qualified(["const"])) != interfaces.type_key(have.qualified(["const"])):
                    return "pointee_value_type_mismatch"
    elif expected.kind == actual.kind == "base":
        try:
            want_width, _ = argcheck.size_align(expected)
            have_width, _ = argcheck.size_align(actual)
        except argcheck.ArgcheckError:
            return None
        if want_width != have_width:
            return "forwarded_scalar_width_change"
    elif expected.kind == "pointer" or actual.kind == "pointer":
        return "pointer_value_role_mismatch"
    return None


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--run", required=True)
    args = parser.parse_args()
    directory = OUT / "forwarding" / args.run
    directory.mkdir(parents=True, exist_ok=False)
    ledger = json.loads((OUT / "dry-matches.json").read_text())
    matching = json.loads((OUT / "match-index.json").read_text())
    required_sources = {unit["source"] for unit in matching}
    selected = {int(name[5:], 16): {"source": unit["source"], "function": name} for unit in ledger["units"] if unit["source"] in required_sources for name in unit["functions"]}
    probes = json.loads((OUT / "compiler-probes.json").read_text())["runs"][-1]
    header_names = {header for row in probes["rows"] if row.get("language") == "native" and row["source"].endswith(".c") and row.get("compiler_exit") == 0 for header in row["headers"]}
    headers = [ROOT / header for header in sorted(header_names)]
    umbrella = directory / "canonical.c"
    umbrella.write_text("".join('#include "' + str(path) + '"\n' for path in headers))
    profile = toolchain.load_profiles()["game"]
    prepared_c = interfaces.prepare(interfaces.preprocess(interfaces.Unit(umbrella, profile), ROOT), umbrella)
    tree = c_parser.CParser().parse(prepared_c.text, filename=str(umbrella))
    analyzer = interfaces.Analyzer(prepared_c, umbrella, ROOT, set())
    analyzer.bind_annotations(tree)
    analyzer.visit(tree)
    types = {declaration.name: declaration.type for declaration in analyzer.declarations}
    functions = {int(name[5:], 16): value for name, value in types.items() if name.startswith("func_") and value.kind == "function"}
    globals_by_address = {int(name.split("_")[1], 16): (name, value) for name, value in types.items() if name.startswith(("data_", "bss_"))}
    layout = {}
    for address, value in functions.items():
        try:
            layout[address] = argcheck.prototype_layout(value)
        except argcheck.ArgcheckError:
            continue
    original = image.Image.load(ROOT / "orig")
    catalogue = image.functions_by_address(image.load_catalogue(ROOT / "config/catalogue.json"))
    decoders = {mode: capstone.Cs(capstone.CS_ARCH_ARM, flag) for mode, flag in (("arm", capstone.CS_MODE_ARM), ("thumb", capstone.CS_MODE_THUMB))}
    for decoder in decoders.values():
        decoder.detail = True

    def value_type(value: tuple | None) -> tuple[interfaces.CType | None, dict]:
        if value is None:
            return None, {"kind": "unknown"}
        kind = value[0]
        if kind == "constant":
            bound = globals_by_address.get(value[1])
            if bound:
                return pointer(bound[1]), {"global": bound[0], "form": "address"}
            return None, {"kind": "constant_or_unbound_address"}
        if kind == "result":
            t = functions.get(value[1])
            return (t.args[0] if t else None), {"return_of": f"func_{value[1]:08x}", "site": hex(value[2])}
        if kind == "parameter":
            t = functions.get(value[1])
            shape = layout.get(value[1])
            if t and shape:
                for index, registers in enumerate(shape["parameters"]):
                    if registers == [f"r{value[2]}"]:
                        return t.args[1][index], {"parameter_of": f"func_{value[1]:08x}", "index": index}
            return None, {"kind": "untyped_input"}
        if kind == "load":
            t, provenance = value_type(value[1])
            if t and t.kind == "pointer":
                child = t.args[0]
                while child.kind in ("record", "array"):
                    if child.kind == "array":
                        child = child.args[0]
                    elif child.args[0].members:
                        child = child.args[0].members[0][1]
                    else:
                        return None, {"kind": "incomplete_load"}
                if child.kind == "base" and child.args[0] == "void":
                    return None, {"kind": "untyped_load"}
                return child, {"loaded_from": provenance}
            return None, {"kind": "untyped_load"}
        if kind == "offset":
            t, provenance = value_type(value[1])
            if t and t.kind == "pointer":
                member = element_at(t.args[0], value[2])
                if member:
                    return pointer(member), {"base": provenance, "offset": value[2]}
            return None, {"kind": "untyped_subobject", "offset": value[2]}
        return None, {"kind": "unknown"}

    def transfer(fact: argcheck.Fact, state: dict, owner: int) -> tuple | None:
        before = state.copy()
        event = argcheck.transfer(fact, state, original)
        if fact.flow == "call":
            if fact.target is not None and not fact.conditional:
                result = functions.get(fact.target)
                if result and result.args[0].kind != "record" and not (result.args[0].kind == "base" and result.args[0].args[0] == "void"):
                    state["r0"] = ("result", fact.target, fact.address)
        elif fact.flow == "branch" and fact.target in functions and fact.target != owner:
            event = ("call", fact.target, before, fact.address, None)
        return event

    rows = {address: dict(row, canonical_type=interfaces.describe(functions[address]) if address in functions else None, incoming_typed_edges=[], outgoing_typed_edges=[]) for address, row in selected.items()}
    candidates = []
    for owner, function in sorted(catalogue.items()):
        facts = argcheck.decode_function(original, function, decoders[function["isa"]])
        blocks, successors = argcheck.blocks(facts, [entry["address"] for entry in function["entries"]])
        incoming = {}
        queue = deque()
        for index, block in enumerate(blocks):
            if block[0].address in {entry["address"] for entry in function["entries"]}:
                incoming[index] = {f"r{r}": ("parameter", owner, r) for r in range(4)}
                queue.append(index)
        while queue:
            index = queue.popleft()
            state = incoming[index].copy()
            for fact in blocks[index]:
                transfer(fact, state, owner)
            for successor in successors[index]:
                previous = incoming.get(successor)
                merged = state.copy() if previous is None else {r: value for r, value in previous.items() if state.get(r) == value}
                if previous != merged:
                    incoming[successor] = merged
                    queue.append(successor)
        for index, initial in sorted(incoming.items()):
            state = initial.copy()
            for fact in blocks[index]:
                event = transfer(fact, state, owner)
                if not event or event[0] != "call" or event[1] not in functions or (owner not in selected and event[1] not in selected):
                    continue
                target, registers, site = event[1:4]
                if target not in layout:
                    continue
                parameters = functions[target].args[1]
                for param_index, locations in enumerate(layout[target]["parameters"]):
                    if len(locations) != 1:
                        continue
                    actual, provenance = value_type(registers.get(locations[0]))
                    if actual is None:
                        continue
                    expected = parameters[param_index]
                    edge = {"caller": f"func_{owner:08x}", "callee": f"func_{target:08x}", "site": hex(site), "parameter": param_index, "actual": interfaces.describe(actual), "expected": interfaces.describe(expected), "provenance": provenance}
                    problem = compatible(expected, actual)
                    if problem:
                        edge["candidate"] = problem
                        candidates.append(edge)
                    if target in rows:
                        rows[target]["incoming_typed_edges"].append(edge)
                    if owner in rows:
                        rows[owner]["outgoing_typed_edges"].append(edge)
    report = {"scope": "Every c008 new/revised function plus r2 additionally revised units; all catalogue callers examined", "coverage": {"selected_functions": len(rows), "canonical_functions": len(functions), "canonical_globals": len(globals_by_address), "catalogue_functions": len(catalogue)}, "limitations": ["C++ methods/native names are not parsed by this C type model; canonical compile probes and retained independent manual reviews cover them", "Scalar register dataflow does not track stack spills, arbitrary arithmetic, narrow memory loads, or virtual calls", "A candidate is not a confirmed defect: validated first-member subobjects and explicit numeric conversions can differ nominally", "No candidate and no observed typed edge do not establish missing caller contracts", "Const writes in C are separately checked by tools/interfaces.py; indirect/C++ mutation remains manual"], "candidates": candidates, "rows": list(rows.values())}
    save(directory / "results.json", report)
    print(json.dumps(report["coverage"] | {"candidate_count": len(candidates), "candidate_functions": sorted({row["callee"] for row in candidates})}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
