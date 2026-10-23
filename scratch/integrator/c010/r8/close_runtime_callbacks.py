#!/usr/bin/env python3
"""Join the complete original call graph, source corpus, ledger and callback types.

This is a callback provenance/type census, not an interfaces/argcheck gate.
Original bytes and preprocessed library payload are never emitted.
"""
from __future__ import annotations

import argparse
from collections import Counter, defaultdict
import hashlib
import json
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT / "tools"))
import argcheck
import interfaces

BASE = "scratch/integrator/c010/r8/"
ARRAY = {"0x00100c80", "0x001911c4", "0x001911cc", "0x0019f6ac", "0x0019f6b4"}
WITHDRAWN = {"func_00107ce4", "func_0014b54c", "func_0014b85c", "func_001d4078", "func_001e5a20"}


def load(path: str) -> dict:
    return json.loads((ROOT / path).read_text())


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def identity(address: str) -> str:
    return "func_" + address[2:]


def signature(declaration: str) -> dict:
    ds = argcheck.parse_declarations(declaration, ROOT / "runtime_callback_contract.c", ROOT, {"callback"})
    assert len(ds) == 1
    return {"type": interfaces.describe(ds[0].type), "type_key": interfaces.type_key(ds[0].type)}


def scrub(text: str) -> str:
    # Preserve offsets/newlines while dropping comments and string contents.
    return re.sub(r'(?m)^[ \t]*#[^\n]*|//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
                  lambda m: "".join("\n" if c == "\n" else " " for c in m.group()), text)


def arguments_at(text: str, start: int) -> tuple[list[str], int]:
    depth, cursor, item, args = 1, start + 1, start + 1, []
    while cursor < len(text) and depth:
        char = text[cursor]
        if char == "(":
            depth += 1
        elif char == ")":
            depth -= 1
            if not depth:
                args.append(text[item:cursor].strip())
                return args if args != [""] else [], cursor
        elif char == "," and depth == 1:
            args.append(text[item:cursor].strip())
            item = cursor + 1
        cursor += 1
    raise RuntimeError("Unbalanced source call/declaration")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    out = ROOT / args.out
    if out.exists():
        raise RuntimeError(f"Immutable output exists: {out}")
    graph = load(BASE + "original-runtime-graph-r3.json")
    types = load(BASE + "callback-types-r4.json")
    corpus = load(BASE + "SOURCE-FILES.json")
    current = load("config/candidates/c010.json")
    accepted = load("config/matches.json")
    prior_scope = load("scratch/integrator/c010/r7/complete-match-scope-r2.json")
    revised = set(load("accept/c010-r6/prepare.json")["revised_units"])
    old_results = load("scratch/integrator/c010/match-r7-r2/results.json")
    ledger = {f: u for u in accepted["units"] for f in u.get("functions", [])}
    accepted_ids = set(ledger)
    moved_ids = {move["function"] for move in current["moves"]}
    ledger.update({f: u for u in current["units"] for f in u.get("functions", [])})
    prior_by_id = {f: u for u in prior_scope["units"] for f in u.get("functions", [])}
    old_receipts = {row["source"]: row for row in old_results["rows"]}
    declarations: dict[str, list[dict]] = defaultdict(list)
    for row in types["declarations"]:
        declarations[row["name"]].append(row)
    native = {row["identity"]: row for row in types["native_cpp_definitions"]}

    registry: dict[str, dict] = {}
    # Discover every function-pointer formal found by the declaration collector;
    # retain source-only helper names instead of inventing their erased callee.
    for row in types["callback_api_declarations"]:
        name = row["name"]
        key = name
        if name.startswith("func_"):
            key = "0x" + name[5:]
        elif name == "__aeabi_vec_ctor_nocookie_nodtor":
            key = "0x001911cc"
        old = registry.get(key)
        entry = {"names": [name], "category": "project_callback_api", "contract_source": row["site"],
                 "formal_type": row["type"], "callback_parameters": row["callback_parameters"]}
        if old is None or row["definition"]:
            registry[key] = entry
    vec = signature("void *callback(void *);")
    dtor = signature("void callback(void *);")
    for entry in ARRAY:
        registry[entry] = {"names": graph["names_by_address"].get(entry, [identity(entry)]),
                           "category": "array_runtime", "callback_parameters": [{"index": 1, **vec}],
                           "formal_type": "void*(void*,void*(*)(void*),size_t,size_t)" if entry != "0x00100c80" else "void*(void*,void*(*)(void*))",
                           "contract_source": "ARM cppabi32 Library helper functions; authenticated array ABI evidence scratch/integrator/thumb/abi-behavior.json"}
    for entry in ("0x00164ebc", "0x00216140", "0x00100c94", "0x00100c9c"):
        registry[entry] = {"names": graph["names_by_address"].get(entry, [identity(entry)]),
                           "category": "atexit_noninvoking_registration", "callback_parameters": [{"index": 1, **dtor}],
                           "formal_type": "registration(void*,void(*)(void*),image_base_handle)",
                           "contract_source": "include/game/functions.h ObjectDestructor; original return-only registration bodies/veneer; image-base parameter is not an invoked callback"}
    registry["0x0019dd50"] = {"names": ["func_0019dd50"], "category": "thread_start_context_registration",
        "callback_parameters": [{"index": 2, **signature("void callback(void *,void *);")}],
        "formal_type": "thread factory with entry(argument,context)",
        "contract_source": "include/game/input_device.h ThreadStartContext.entry; accepted src/wave13/func_0012d104.c invokes entry(argument,context); original0019de08 forwards0012d104 to kernel CreateThread"}
    registry["0x0011ef08"] = {"names": ["func_0011ef08", "kernel ABI CreateThread"], "category": "kernel_thread_entry",
        "callback_parameters": [{"index": 1, **dtor}], "formal_type": "kernel CreateThread entry(void*) with non-C r4 processor-ID ABI",
        "contract_source": "catalogue authenticated CreateThread identity; include/ctr/svc.h intentionally has no representable C declaration"}
    registry["0x001d995c"] = {"names": ["func_001d995c"], "category": "cleanup_event_registration",
        "callback_parameters": [{"index": 2, **signature("void callback(void *,unsigned,void *);")}],
        "formal_type": "cleanup registration with CleanupCallback", "contract_source": "include/game/cleanup_registry.h CleanupCallback"}
    registry["0x00177b10"] = {"names": ["func_00177b10"], "category": "native_cpp_member_registration",
        "callback_parameters": [{"index": 1, "type": "void(CallbackTarget::*)(void)", "type_key": None}],
        "formal_type": "void(CallbackTarget*,MemberCallback)",
        "contract_source": "include/game/deferred_callback.h; native DeferredMemberCallback storage; dispatcher001b7dac; no hand-decoded C PMF model"}
    unknown = {"0x0014852c": [(1, "frame-timing callback forwarded to0014b17c")],
               "0x0015dedc": [(2, "paired RadarWidget callback"), (3, "paired RadarWidget callback")],
               "0x001d36fc": [(4, "result/context callback stored in owner and invoked at001d387c")],
               "0x0017e3d8": [(2, "float-state callback/member representation requires native callable-family recovery")],
               "0x0019bc7c": [(0, "startup callback invocation at0019bd04"), (1, "startup companion callback") ]}
    for entry, params in unknown.items():
        registry[entry] = {"names": [identity(entry)], "category": "unmodeled_existing_custom_runtime",
            "formal_type": "UNESTABLISHED: no canonical callable family; not specialized to a provider",
            "callback_parameters": [{"index": i, "type": "UNESTABLISHED: " + role, "type_key": None} for i, role in params],
            "contract_source": "complete original graph; source provider declarations; no current NEW/revised provider is permitted unresolved"}
    # All eight typed source-only registration helpers are noninvoking no-ops.
    for name, entry in registry.items():
        if name.startswith("register_"):
            entry["category"] = "source_only_typed_noninvoking_registration"
            entry["callback_parameters"] = [p for p in entry["callback_parameters"] if p["index"] != 2]
            entry["erased_original_identity"] = "UNRECOVERED; do not equate with an atexit symbol"
    for key, entry in registry.items():
        names_for_entry = set(entry["names"])
        if key.startswith("0x"):
            names_for_entry.add(identity(key))
        canonical = {}
        for name in names_for_entry:
            for declaration in declarations[name]:
                token = (name, declaration["type"], declaration["site"]["source"], declaration["site"]["line"])
                canonical[token] = {"name": name, "type": declaration["type"], "site": declaration["site"]}
        entry["source_callable_declarations"] = list(canonical.values())

    def provider(address: str, expected: dict, category: str) -> dict:
        name = identity(address)
        unit = ledger.get(name, prior_by_id.get(name))
        state = "ACCEPTED_REVISED" if name in accepted_ids and (unit["source"] in revised or name in moved_ids) else "ACCEPTED_UNCHANGED" if name in accepted_ids else "NEW" if name in ledger else "DEFERRED_NEW" if name in WITHDRAWN else "UNCLAIMED"
        result = {"address": address, "name": name, "aliases": graph["names_by_address"].get(address, [name]),
                  "ledger_state": state, "prior_prepared_claim": name in prior_by_id and name not in accepted_ids,
                  "runtime_parameter_type": expected["type"]}
        if unit is not None:
            result["source"] = unit["source"]
        defs = [d for d in declarations[name] if d["definition"] and (unit is None or d["source"] == unit["source"])]
        if defs:
            declaration = defs[0]
            result.update(declared_provider_type=declaration["type"], declaration_site=declaration["site"],
                          source_sha256=digest(ROOT / declaration["source"]))
            if declaration.get("type_key") is not None and expected["type_key"] is not None:
                same = json.dumps(declaration["type_key"]) == json.dumps(expected["type_key"])
                result["comparison"] = "EXACT_TYPE" if same else "INCOMPATIBLE_TYPE"
            else:
                result["comparison"] = "UNESTABLISHED_RUNTIME_OR_CPP_NOMINAL_TYPE"
                if expected["type"] in {vec["type"], dtor["type"], "function(pointer to (void), pointer to (void)) -> void"} and re.search(
                    r"\b(?!(?:void|const|unsigned|signed|int|char|short|long|float|double)\b)[A-Za-z_]\w*(?:::\w+)*\s*\*", declaration["type"]
                ):
                    result["comparison"] = "INCOMPATIBLE_CPP_FREE_NOMINAL_TYPE"
        elif name in native:
            row = old_receipts.get(unit["source"])
            emitted = []
            if row:
                receipt = load(row["receipt"])
                emitted = [f["name"] for f in receipt["functions"] if f["address"] == int(address, 16)]
                result["emitted_symbol_receipt"] = row["receipt"]
                result["emitted_symbol_receipt_sha256"] = row["receipt_sha256"]
            result["emitted_symbols"] = emitted
            result["declared_provider_type"] = "Native C++ member definition: " + ",".join(emitted)
            ctor = bool(emitted) and all(re.search(r"C[12]Ev$", s) for s in emitted)
            destructor = bool(emitted) and all(re.search(r"D[12]Ev$", s) for s in emitted)
            result["comparison"] = "NATIVE_CPP_ABI_CONSTRUCTOR" if ctor and category == "array_runtime" else "NATIVE_CPP_ABI_DESTRUCTOR" if destructor and category in {"array_runtime", "atexit_noninvoking_registration"} else "INCOMPATIBLE_NATIVE_MEMBER_ROLE"
            result["abi_rule"] = "Only actual compiler-emitted C1/C2 or D1/D2 symbols receive native ABI treatment. A spare alias or C++ free function never receives this exception."
        else:
            result["declared_provider_type"] = "NO_LEDGER_PROVIDER" if state == "UNCLAIMED" else "MISSING_PROVIDER_DECLARATION"
            result["comparison"] = "UNCLAIMED_PROVIDER" if state == "UNCLAIMED" else "UNRESOLVED_PROVIDER_DECLARATION"
        result["current_NEW_or_revised_closed"] = state not in {"NEW", "ACCEPTED_REVISED"} or result["comparison"] in {"EXACT_TYPE", "NATIVE_CPP_ABI_CONSTRUCTOR", "NATIVE_CPP_ABI_DESTRUCTOR"}
        return result

    calls_by_target: dict[str, list[dict]] = defaultdict(list)
    for row in graph["original_call_graph"]:
        if row["target"] in registry:
            calls_by_target[row["target"]].append(row)

    def argument(row: dict, index: int) -> dict:
        if index < 4:
            value = row["arguments"].get(f"r{index}", {"kind": "unknown"})
            primary = row.get("primary_entry_arguments", {}).get(f"r{index}")
            return primary if value["kind"] == "unknown" and primary is not None else value
        return row["stack_arguments"].get(str((index - 4) * 4), {"kind": "unknown"})

    def origins(value: dict, seen: frozenset[tuple[str, str]] = frozenset()) -> set[str]:
        if value["kind"] == "function_address":
            return {value["owner"]}
        if value["kind"] == "parameter":
            key = value["function"], value["register"]
            if key in seen:
                return set()
            return {a for incoming in calls_by_target.get(value["function"], [])
                    for a in origins(argument(incoming, int(value["register"][1:])), seen | {key})}
        return set()

    original_rows, comparisons, unresolved = [], [], []
    for target, rows in sorted(calls_by_target.items()):
        entry = registry[target]
        for row in rows:
            for parameter in entry["callback_parameters"]:
                value = argument(row, parameter["index"])
                resolved = sorted(origins(value))
                providers = [provider(a, parameter, entry["category"]) for a in resolved]
                comparisons.extend(providers)
                unresolved.extend(p for p in providers if not p["current_NEW_or_revised_closed"])
                original_rows.append({"caller": row["caller"], "site": row["site"], "entrypoint": target,
                    "category": entry["category"], "callback_parameter_index": parameter["index"],
                    "runtime_parameter_type": parameter["type"], "argument_fact": value, "providers": providers,
                    "array_semantics": row.get("array_semantics"),
                    "source_binding": {"source": ledger[identity(row["caller"])]["source"], "kind": "credited source body; final whole-proposal MATCH binds original callsite, including implicit compiler/runtime calls"} if identity(row["caller"]) in ledger else {"kind": "uncredited original caller; intentionally included"}})

    api_names: dict[str, str] = {}
    for key, entry in registry.items():
        for name in entry["names"]:
            if re.fullmatch(r"[A-Za-z_]\w*", name):
                api_names[name] = key
        if key.startswith("0x"):
            api_names[identity(key)] = key
    for name, address in {"__aeabi_vec_ctor_nocookie_nodtor": "0x001911cc", "__aeabi_vec_dtor": "0x0019f6b4", "__aeabi_vec_dtor_cookie": "0x00100c80", "__aeabi_atexit": "0x00164ebc"}.items():
        api_names[name] = address
    pattern = re.compile(r"\b(" + "|".join(sorted(map(re.escape, api_names), key=len, reverse=True)) + r")\s*\(")
    source_rows, source_errors, name_inventory, source_hashes = [], [], [], {}
    family_pattern = re.compile(r"\b(?:__aeabi_vec_\w+|__cxa_vec_\w+|__vec_\w+|qsort|bsearch|atexit|__cxa_atexit|pthread_create|CreateThread)\b")
    for relative in corpus["files"]:
        path = ROOT / relative
        source = path.read_text()
        clean = scrub(source)
        source_hashes[relative] = digest(path)
        for hit in family_pattern.finditer(clean):
            name_inventory.append({"name": hit.group(), "source": relative, "line": clean.count("\n", 0, hit.start()) + 1})
        for hit in pattern.finditer(clean):
            try:
                values, end = arguments_at(clean, hit.end() - 1)
            except RuntimeError:
                source_errors.append({"source": relative, "line": clean.count("\n", 0, hit.start()) + 1})
                continue
            begin = max(clean.rfind(";", 0, hit.start()), clean.rfind("{", 0, hit.start()), clean.rfind("}", 0, hit.start())) + 1
            prefix = clean[begin:hit.start()].strip()
            declaration = bool(re.fullmatch(r"[A-Za-z_][\w\s:*&<>]*", prefix)) and not prefix.startswith(("return", "throw", "co_return"))
            key = api_names[hit.group(1)]
            row = {"source": relative, "line": clean.count("\n", 0, hit.start()) + 1, "name": hit.group(1), "entrypoint": key,
                   "kind": "declaration_or_definition" if declaration else "explicit_source_call", "callbacks": []}
            for parameter in registry[key]["callback_parameters"]:
                index = parameter["index"]
                expression = values[index] if index < len(values) else "<not present>"
                names_found = sorted(set(re.findall(r"\bfunc_[0-9a-f]{8}\b", expression)))
                providers = [] if declaration else [provider("0x" + n[5:], parameter, registry[key]["category"]) for n in names_found]
                comparisons.extend(providers)
                unresolved.extend(p for p in providers if not p["current_NEW_or_revised_closed"])
                row["callbacks"].append({"index": index, "runtime_parameter_type": parameter["type"], "expression": " ".join(expression.split())[:200], "providers": providers})
            source_rows.append(row)
    # Exhaustively account for every passed-code candidate, including unused
    # caller-saved words, stale saved-stack slots and image-base handles.
    classified, unmodeled_candidates = [], []
    for row in graph["passed_function_address_candidates"]:
        key = row["callee"]
        location = row["argument_location"]
        index = int(location[1:]) if location.startswith("r") else 4 + int(location[6:]) // 4
        entry = registry.get(key)
        role = "callback_parameter" if entry and any(p["index"] == index for p in entry["callback_parameters"]) else "noncallback_or_outside_formal_arguments"
        evidence = None
        if key and declarations[identity(key)]:
            evidence = declarations[identity(key)][0]["type"]
        if role != "callback_parameter" and evidence is None:
            role = "unmodeled_existing_passed_code_candidate"
            candidate = provider(row["provider"]["owner"],
                {"type": "UNESTABLISHED: candidate argument to a callee without a canonical callback declaration", "type_key": None},
                "unmodeled_passed_code_candidate")
            unmodeled_candidates.append({**row, "provider_contract": candidate,
                "disposition": "Explicit possible callback candidate, not silently declared noninvoking/noncallback. Main owns unchanged accepted providers."})
            if not candidate["current_NEW_or_revised_closed"]:
                unresolved.append(candidate)
        classified.append({**row, "classification": role, "callee_declared_type": evidence,
                           "reason": "Registered callback parameter" if role == "callback_parameter" else "Canonical source declaration excludes this callback formal (including hoisted registration values and saved stack words)" if evidence is not None else "No canonical callee declaration: retained separately as an unmodeled existing passed-code candidate, not excluded by assumption."})
    native_member_census = []
    for relative in corpus["files"]:
        clean = scrub((ROOT / relative).read_text())
        for match in re.finditer(r"([A-Za-z_]\w*(?:::\w+)*)::\s*\*", clean):
            owner = match.group(1)
            native_member_census.append({"source": relative, "line": clean.count("\n", 0, match.start()) + 1,
                "owner": owner, "role": "noncallable member-data/type-trait" if owner in {"SafeBoolHelper", "C"} else "native member-function pointer",
                "current_selected_source": any(u["source"] == relative for u in ledger.values())})
    native_providers = []
    for address, aliases in graph["names_by_address"].items():
        reader_aliases = [name for name in aliases if "CharStrmReader" in name and "ReadNextChar" in name]
        if not reader_aliases or identity(address) not in ledger:
            continue
        name = identity(address)
        unit = ledger[name]
        receipt_row = old_receipts[unit["source"]]
        receipt = load(receipt_row["receipt"])
        symbols = [f["name"] for f in receipt["functions"] if f["address"] == int(address, 16)]
        source = (ROOT / unit["source"]).read_text()
        exact = unit["source"].endswith(".cpp") and bool(symbols) and all(
            "CharStrmReader" in symbol and re.search(r"ReadNextChar(?:UTF8|UTF16|CP1252|SJIS)(?:Copy)?Ev$", symbol) for symbol in symbols
        ) and "CharStrmReader::ReadNextChar" in source
        state = "ACCEPTED_REVISED" if name in accepted_ids and (name in moved_ids or unit["source"] in revised) else "ACCEPTED_UNCHANGED" if name in accepted_ids else "NEW"
        row = {"address": address, "name": name, "aliases": reader_aliases, "ledger_state": state, "source": unit["source"],
            "source_sha256": digest(ROOT / unit["source"]), "emitted_symbols": symbols,
            "emitted_symbol_receipt": receipt_row["receipt"], "emitted_symbol_receipt_sha256": receipt_row["receipt_sha256"],
            "runtime_parameter_type": "u16(nw::font::CharStrmReader::*)(void)",
            "declared_provider_type": "u16(nw::font::CharStrmReader::*)(void)",
            "contract_source": "include/nw/font/font_CharStrmReader.h ReadNextCharFunc, Next(), constructor and all six declared native member methods; descriptor data_0021c64c",
            "comparison": "EXACT_NATIVE_MEMBER_TYPE" if exact else "UNRESOLVED_NATIVE_MEMBER_PROVIDER",
            "current_NEW_or_revised_closed": exact}
        native_providers.append(row)
        comparisons.append(row)
        if not exact:
            unresolved.append(row)
    native_families = {
        "CallbackTarget": {"parameter_type": "void(CallbackTarget::*)(void)", "registration_entry": "0x00177b10",
            "invoker": "0x001b7dac", "storage": "DeferredMemberCallback.method",
            "source": "include/game/deferred_callback.h", "provider_kind": "native virtual descriptors, not C free-function addresses",
            "descriptors": ["data_00218498", "data_002184b0", "data_00218718", "data_00218860", "data_00218868", "data_002188d0", "data_002188d8", "data_002188e0"]},
        "CharStrmReader": {"parameter_type": "u16(nw::font::CharStrmReader::*)(void)", "storage": "ReadNextCharFunc m_ReadFunc",
            "invoker": "CharStrmReader::Next", "source": "include/nw/font/font_CharStrmReader.h",
            "providers": native_providers},
        "SequenceTrack": {"parameter_types": ["void(SequenceTrack::*)(T)", "void(SequenceTrack::*)(T,U)"],
            "invoker": "SequenceSoundPlayer::SetTrackParam template", "source": "include/nw/snd/snd_SequenceSoundPlayer.h",
            "source_disposition": "Native template declarations/definitions only; no named project source callsite or selected concrete provider in the complete source census"},
        "CommandTarget": {"parameter_types": ["void(CommandTarget::*)(unsigned char)", "void(CommandTarget::*)(signed char)"],
            "source_disposition": "The only three source declarations are existing NEW-only deferred units0015121c/00151458/00151584; no current selected provider"}
    }
    unique = {}
    for row in comparisons:
        unique[(row["name"], row["runtime_parameter_type"], row["comparison"], row["ledger_state"])] = row
    accepted_attention = [p for p in unique.values() if p["ledger_state"].startswith("ACCEPTED") and p["comparison"] not in {"EXACT_TYPE", "NATIVE_CPP_ABI_CONSTRUCTOR", "NATIVE_CPP_ABI_DESTRUCTOR", "EXACT_NATIVE_MEMBER_TYPE"}]
    accepted_mismatches = [p for p in accepted_attention if p["comparison"].startswith("INCOMPATIBLE")]
    source_only = [name for name, entry in registry.items() if entry["category"] == "source_only_typed_noninvoking_registration"]
    array_rows = [r for r in original_rows if r["entrypoint"] in ARRAY]
    result = {"schema_version": 1, "source_commit": load(BASE + "SOURCE-CONTEXT-r4.json")["source_commit"],
        "driver": str(Path(__file__).relative_to(ROOT)), "driver_sha256": digest(Path(__file__)),
        "inputs": {path: digest(ROOT / path) for path in [BASE + "original-runtime-graph-r3.json", BASE + "callback-types-r4.json", BASE + "SOURCE-FILES.json", "config/candidates/c010.json", "config/matches.json"]},
        "enumeration_policy": "All4743 catalogue functions decoded, including uncredited callers. Every direct/tail site to each callback runtime is listed, not only calls from candidate source. Primary initializer flow is retained alongside conservative multi-entry flow. Every src/include file is enumerated without truncated glob buckets. Actual C++ emitted symbols, never spare aliases, distinguish native ABI constructors/destructors. Unknown existing custom families remain explicit; five NEW providers are withdrawn instead of guessed or steered.",
        "array_abi_primary_source": "https://github.com/ARM-software/abi-aa/blob/main/cppabi32/cppabi32.rst#library-helper-functions",
        "counts": {"catalogue_functions": graph["function_count"], "decoded_functions": graph["decoded_function_count"], "decode_errors": len(graph["coverage_errors"]),
                   "original_call_sites_scanned": len(graph["original_call_graph"]), "source_files_scanned": len(corpus["files"]),
                   "callback_runtime_entries": len(registry), "array_entrypoints_including_veneers": len(ARRAY),
                   "array_original_call_sites": len({r["site"] for r in array_rows}), "array_provider_addresses": len({p["address"] for r in array_rows for p in r["providers"]}),
                   "original_callback_parameter_rows": len(original_rows), "source_api_mentions": len(source_rows),
                   "explicit_source_calls": sum(r["kind"] == "explicit_source_call" for r in source_rows),
                   "passed_code_candidates_classified": len(classified), "indirect_parameter_candidates_retained": len(graph["indirect_invocation_parameter_candidates"]),
                   "unique_provider_contract_rows": len(unique), "NEW_provider_addresses": len({p["address"] for p in unique.values() if p["ledger_state"] == "NEW"}),
                   "revised_provider_addresses": len({p["address"] for p in unique.values() if p["ledger_state"] == "ACCEPTED_REVISED"}),
                   "accepted_attention_rows": len(accepted_attention), "source_only_noninvoking_helpers": len(source_only),
                   "native_member_callback_families": len(native_families), "native_member_provider_addresses": len(native_providers),
                   "unmodeled_existing_passed_code_candidates": len(unmodeled_candidates),
                   "source_scan_errors": len(source_errors), "unresolved_NEW_or_revised_rows": len(unresolved)},
        "runtime_entrypoints": registry, "original_callback_calls": original_rows, "source_api_mentions": source_rows,
        "source_runtime_family_name_inventory": name_inventory, "source_corpus_hashes": source_hashes,
        "provider_contract_table": list(unique.values()), "accepted_unchanged_attention_for_Main": accepted_attention,
        "accepted_definite_type_mismatches_for_Main": accepted_mismatches,
        "accepted_unmodeled_or_nominal_qualifications_for_Main": [p for p in accepted_attention if p not in accepted_mismatches],
        "all_passed_code_candidates_classified": classified,
        "native_member_callback_families": native_families, "native_member_pointer_source_census": native_member_census,
        "unmodeled_existing_passed_code_candidates": unmodeled_candidates,
        "all_indirect_parameter_candidates": graph["indirect_invocation_parameter_candidates"],
        "named_family_negative_inventory": {"__cxa_vec_*": not any(r["name"].startswith("__cxa_vec_") for r in name_inventory),
            "vec_new_delete": not any(re.search(r"vec_(new|delete)", r["name"]) for r in name_inventory),
            "sort_search": not any(r["name"] in {"qsort", "bsearch"} for r in name_inventory),
            "basis": "all3228 project source/header files plus all catalogue classification identities/config aliases and passed-function candidate census; negatives are named-family negatives, not claims that arbitrary indirect APIs do not exist"},
        "withdrawn_NEW_providers": sorted(WITHDRAWN), "unresolved_NEW_or_revised": unresolved, "source_scan_errors": source_errors,
        "privacy": "Only project source declarations, semantic address/extent/provenance facts, names, counts and hashes. No original bytes, encodings, disassembly, raw buffers or licensed library payload."}
    out.write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps({"output": str(args.out), **result["counts"]}))
    return int(bool(unresolved or source_errors))


if __name__ == "__main__":
    raise SystemExit(main())
