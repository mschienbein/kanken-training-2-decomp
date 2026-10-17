"""Generate the v8 candidate and exhaustive metadata-only revision report.

Run with the repository's Python from the catalogue-v8 worktree. Private inputs
and existing research evidence are read from --root; only --out is written there.
This is a candidate, not adoption, denominator history, or accepted C credit.
"""
from __future__ import annotations

import argparse
from bisect import bisect_right
from collections import Counter, defaultdict
import hashlib
import json
from pathlib import Path
import struct
import sys
import time

from capstone import arm
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools import analyze, ctrfmt

RUNTIME_REPORT = "scratch/integrator/runtime-helpers/REPORT.md"
RESIDUE_REPORT = "scratch/integrator/residue/REPORT.md"
ABI_REPORT = "scratch/integrator/armcc-ext/REPORT.md"

# Reported whole-body named ARMCC output identities, not vendor-library guesses.
GENERATED = {
    0x152964: "__ARM_common_memclr4_12", 0x1D5560: "__ARM_common_memclr4_5",
    0x130F90: "__ARM_common_memclr4_8", 0x1D481C: "__ARM_common_memclr4_8",
    0x183458: "__ARM_common_memcpy4_5", 0x17DA60: "__ARM_common_memcpy4_6",
    0x131958: "__ARM_common_memcpy4_8", 0x126F2C: "__ARM_common_memset4_5",
}
ABI_HELPERS = {
    0x1070C0: ("__aeabi_lasr", "404 original-ABI cases; probe_lasr emits __aeabi_lasr"),
    0x186454: ("__aeabi_uidivmod", "names.json full natural division caller MATCH; 530 original-ABI cases"),
    0x1864DC: ("__aeabi_memclr / __aeabi_memclr4", "names.json two natural zero-fill caller MATCHes; 50 original-ABI cases"),
    0x18CB4C: ("__aeabi_memset", "names.json natural constructor/fill caller MATCH; 50 original-ABI cases"),
    0x18D204: ("__aeabi_ldivmod", "530 original-ABI cases; probe_idiv64/probe_imod64 emit __aeabi_ldivmod"),
    0x1911CC: ("__aeabi_vec_ctor_nocookie_nodtor", "names.json full natural member-array constructor MATCH; 17 original-ABI cases"),
    0x1941F8: ("__aeabi_uldivmod", "530 original-ABI cases; probe_udiv64/probe_umod64 emit __aeabi_uldivmod"),
    0x196498: ("__aeabi_f2ulz", "404 original-ABI cases; probe_f2ul emits __aeabi_f2ulz"),
    0x196504: ("__aeabi_ul2f", "407 original-ABI cases; probe_ul2f emits __aeabi_ul2f"),
    0x19F6D8: ("__aeabi_idivmod", "530 original-ABI cases; probe_idiv32/probe_imod32 emit __aeabi_idivmod"),
    0x1A000C: ("__aeabi_memcpy / __aeabi_memcpy4", "names.json two full natural copy caller MATCHes; 50 original-ABI cases"),
}
KERNEL_ABIS = {
    0x195CD8: ("ControlMemory", "#1501: required permission input in r4"),
    0x1109CC: ("QueryMemory", "#1505: six raw result words require r0-r5"),
    0x11EF08: ("CreateThread", "#1501: required processor-ID input in r4"),
    0x185108: ("ArbitrateAddress", "#1501: required timeout input in r4/r5"),
    0x12D168: ("WaitSynchronizationN", "#1501: required timeout high input in r4"),
    0x100D6C: ("ExitProcess", "no-frame C __svc shape failed all 16 compiler-grid cells; new 12/4-byte negative probe"),
    0x12D148: ("ExitThread", "no-frame C __svc shape failed all 16 compiler-grid cells"),
    0x134E7C: ("Break", "no-frame C __svc shape failed all 16 compiler-grid cells"),
}
LITERAL_STUBS = {0x100D6C, 0x1109B4, 0x1109CC, 0x11EF08, 0x11EF2C, 0x11EF4C,
                 0x11EF58, 0x12D0EC, 0x12D148, 0x12D150, 0x12D168, 0x134E7C,
                 0x145610, 0x1565B0, 0x185108, 0x18F7D4, 0x195CD8}


def load(path: Path):
    return json.loads(path.read_text(encoding="utf-8"))


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def owner(functions: list[dict], starts: list[int], address: int) -> dict | None:
    index = bisect_right(starts, address) - 1
    function = functions[index] if index >= 0 else None
    return function if function and address < function["address"] + function["size"] else None


def classification_records(baseline: dict) -> list[dict]:
    by_address = {function["address"]: function for function in baseline["functions"]}
    records = []
    for address in sorted(GENERATED.keys() | ABI_HELPERS.keys() | KERNEL_ABIS.keys()):
        function = by_address[address]
        row = {"address": address, "size": function["size"], "isa": function["isa"],
               "image_sha256": baseline["image_sha256"]}
        if address in GENERATED:
            row.update(kind="runtime", basis="compiler_generated", identity=GENERATED[address],
                       evidence=f"{RUNTIME_REPORT}, compiler-generated whole-body identity table",
                       whole_body_identity="Named relocation-free ARMCC-emitted section equals the entire original extent")
        elif address in ABI_HELPERS:
            identity, proof = ABI_HELPERS[address]
            row.update(kind="runtime", basis="compiler_abi", identity=identity,
                       evidence=f"{RUNTIME_REPORT}, binding proofs and 20-owner ABI inventory: {proof}",
                       compiler_emission=proof)
        else:
            identity, proof = KERNEL_ABIS[address]
            row.update(kind="assembly", basis="unrepresentable_abi", identity=f"kernel ABI {identity}",
                       evidence=f"{ABI_REPORT}, raw-ABI compiler limits / literal-stub probes: {proof}",
                       compiler_limit=proof)
        records.append(row)
    return records


def candidate_rows(root: Path) -> list[dict]:
    rows = []
    for path in sorted((root / "scratch/authors").glob("*/RESULT.json")):
        def visit(value):
            if isinstance(value, dict):
                if value.get("status") == "library_candidate":
                    rows.append({**value, "report": str(path.relative_to(root))})
                for child in value.values():
                    visit(child)
            elif isinstance(value, list):
                for child in value:
                    visit(child)
        visit(load(path))
    return rows


def pointer_census(candidate: dict, image: bytes, layout: ctrfmt.CodeLayout) -> list[dict]:
    analyzer = analyze.Analyzer(image, layout)
    functions = candidate["functions"]
    starts = [function["address"] for function in functions]
    entries = {(entry["address"], entry["isa"]) for function in functions for entry in function["entries"]}
    references = defaultdict(list)
    code_words = {address & ~3 for function in functions for first, end in function["code_ranges"]
                  for address in range(first, end)}
    pool_owners = {address: function["address"] for function in functions
                   for first, end in function["pools"] for address in range(first, end, 4)}
    literal_owners = {address: function["address"] for function in functions
                      for first, end in function["literals"] for address in range((first + 3) & ~3, end - 3, 4)}
    for address in range(analyzer.base, analyzer.end - 3, 4):
        if address in code_words:
            continue
        value = analyzer.word(address)
        target, isa = value & ~1, "thumb" if value & 1 else "arm"
        if analyzer.inside(target, isa):
            source_owner = pool_owners.get(address, literal_owners.get(address))
            kind = "pool" if address in pool_owners else "literal" if address in literal_owners else "unowned_text"
            references[(target, isa)].append({"address": address, "function": source_owner, "kind": kind})
    for segment in (layout.rodata, layout.data):
        offset = layout.file_offset(segment.address)
        for index in range(0, segment.size - 3, 4):
            value = struct.unpack_from("<I", image, offset + index)[0]
            target, isa = value & ~1, "thumb" if value & 1 else "arm"
            if analyzer.inside(target, isa):
                references[(target, isa)].append({"address": segment.address + index, "function": None, "kind": segment.name})
    uses = defaultdict(list)
    arguments = {arm.ARM_REG_R0, arm.ARM_REG_R1, arm.ARM_REG_R2, arm.ARM_REG_R3}
    for function in functions:
        if function["isa"] != "arm" or function["kind"] == "veneer":
            continue
        blocks = {entry["address"] for entry in function["entries"]}
        for first, end in function["code_ranges"]:
            for address in range(first, end, 4):
                ins = analyzer.decode(address, "arm")
                if ins.branch and ins.target is not None:
                    blocks.add(ins.target[0])
        values, previous = {}, None
        for first, end in function["code_ranges"]:
            for address in range(first, end, 4):
                if previous != address - 4 or address in blocks:
                    values = {}
                decoded, ins = analyzer.detailed(address), analyzer.decode(address, "arm")
                if decoded is None or ins is None:
                    values, previous = {}, address
                    continue
                updates = {}
                if ins.indirect and not ins.returns and decoded.operands and decoded.operands[0].type == arm.ARM_OP_REG:
                    register = decoded.operands[0].reg
                    if register in values:
                        pointer = values[register]
                        target, mode = pointer & ~1, "thumb" if pointer & 1 else "arm"
                        if analyzer.inside(target, mode):
                            uses[(target, mode)].append({"address": address, "function": function["address"], "kind": "indirect_transfer"})
                if ins.call and ins.target is not None:
                    for register, pointer in values.items():
                        target, mode = pointer & ~1, "thumb" if pointer & 1 else "arm"
                        if register in arguments and analyzer.inside(target, mode):
                            uses[(target, mode)].append({"address": address, "function": function["address"],
                                                        "kind": "call_argument", "callee": ins.target[0],
                                                        "register": analyzer.detailed_arm.reg_name(register)})
                if not ins.conditional:
                    if (decoded.id == arm.ARM_INS_LDR and ins.pools and not ins.address_reference
                            and decoded.operands[0].type == arm.ARM_OP_REG
                            and ins.pools[0][0] in pool_owners):
                        updates[decoded.operands[0].reg] = analyzer.word(ins.pools[0][0])
                    elif (decoded.id == arm.ARM_INS_MOV and len(decoded.operands) == 2
                          and decoded.operands[1].type == arm.ARM_OP_REG and decoded.operands[1].reg in values):
                        updates[decoded.operands[0].reg] = values[decoded.operands[1].reg]
                for register in decoded.regs_access()[1]:
                    values.pop(register, None)
                if ins.call:
                    for register in arguments | {arm.ARM_REG_R12}:
                        values.pop(register, None)
                values.update(updates)
                if ins.branch or ins.stop:
                    values = {}
                previous = address
        analyzer.detail_cache.clear()
    rows = []
    for (target, isa), refs in sorted(references.items()):
        function = owner(functions, starts, target)
        if (target, isa) in entries:
            disposition = "callable_entry"
        elif function and any(first <= target < end for first, end in function["code_ranges"]):
            disposition = "interior_code_no_independent_entry_proof"
        elif function and any(first <= target < end for first, end in function["pools"] + function["literals"]):
            disposition = "owned_data_not_entry"
        else:
            disposition = "unowned_no_closed_entry_proof"
        if (target, isa) not in entries and any(use["kind"] == "indirect_transfer" for use in uses[(target, isa)]):
            raise ValueError(f"observed indirect transfer still lacks an entry: {target:#x}/{isa}")
        rows.append({"address": target, "isa": isa, "disposition": disposition, "references": refs,
                     "uses": uses[(target, isa)], "owner": function["address"] if function else None})
    return rows


def accounting(baseline: dict, candidate: dict, image: bytes, layout: ctrfmt.CodeLayout) -> tuple[list[dict], list[dict]]:
    functions = candidate["functions"]
    starts = [function["address"] for function in functions]
    analyzer = analyze.Analyzer(image, layout)
    unknown_rows = []
    for old_range in baseline["ranges"]:
        if old_range["kind"] != "unknown":
            continue
        parts = []
        for new_range in candidate["ranges"]:
            first, end = max(old_range["start"], new_range["start"]), min(old_range["end"], new_range["end"])
            if first >= end:
                continue
            function = owner(functions, starts, first)
            if new_range["kind"] == "unknown":
                conflicts = [conflict for conflict in candidate["literal_conflicts"]
                             if conflict["target"] < end and first < conflict["end"]]
                if conflicts:
                    reason = "PC-relative data reference conflicts with another owner/start or executable bytes; no exclusive data claim"
                elif function:
                    reason = "Unreached hole inside the extent: no reachable instruction edge or bounded data-format witness"
                else:
                    reason = "Unowned gap: no evidenced callable entry/closed CFG or bounded data-format witness; plausible decoding alone is insufficient"
            else:
                reason = "Recovered function instruction/pool/literal ownership" if new_range["kind"] == "function" else "Proved padding/data"
            parts.append({"start": first, "end": end, "kind": new_range["kind"], "owner": new_range["owner"], "reason": reason})
        if sum(part["end"] - part["start"] for part in parts) != old_range["end"] - old_range["start"]:
            raise ValueError("v7 unknown accounting does not conserve bytes")
        unknown_rows.append({"start": old_range["start"], "end": old_range["end"], "parts": parts})
    new_issues = {(row["address"], row["issue"]) for row in candidate["anomalies"]}
    repairs = {row["old_address"]: row["address"] for row in candidate["leading_instruction_repairs"]}
    anomaly_rows = []
    for old_issue in baseline["anomalies"]:
        site, issue = old_issue["address"], old_issue["issue"]
        replacement = repairs.get(site, site)
        remains = (site, issue) in new_issues or (replacement, issue) in new_issues
        function = owner(functions, starts, replacement)
        row = {**old_issue, "disposition": "open" if remains else "resolved", "owner": function["address"] if function else None}
        if not remains:
            row["reason"] = "Leading-instruction recovery restores the referenced pool and/or closes the function's formerly unknown hole"
        elif issue == "unexplained_function_bytes":
            row["reason"] = "Extent contains unowned/unreached holes or alignment not included in code/pools; no byte is silently claimed"
            row["holes"] = [{"start": item["start"], "end": item["end"], "kind": item["kind"]}
                            for item in candidate["ranges"] if function and item["kind"] != "function"
                            and function["address"] <= item["start"] < function["address"] + function["size"]]
        elif issue == "literal_outside_function":
            ins = analyzer.decode(site, function["isa"] if function else "arm")
            row["targets"] = [list(span) for span in ins.pools] if ins else []
            row["reason"] = "Architectural PC-relative target is outside this owner; no conflicting pool ownership or extent is stolen"
        elif issue == "undecodable_reachable_code":
            row["reason"] = "Reachable word after the software-FP exceptional dispatcher does not decode; return-address descriptor/nonreturn semantics remain unproved"
        else:
            row["reason"] = {"shared_address_literal": "Addressed object has multiple independent users; exclusive function ownership is unproved",
                             "address_literal_crosses_start": "Inferred addressed object bound crosses an independently preserved callable start",
                             "address_literal_overlaps_code": "Inferred addressed object bound intersects reachable executable bytes; string-like content alone cannot override code"}[issue]
        anomaly_rows.append(row)
    return unknown_rows, anomaly_rows


def report(candidate: dict, baseline: dict) -> str:
    review = candidate["revision_review"]
    functions = {function["address"]: function for function in candidate["functions"]}
    old = {function["address"]: function for function in baseline["functions"]}
    lines = ["# Candidate catalogue v8 — entry recovery and evidence-gated classification", "",
             "**Candidate only; not adopted and not a claim that the decompilation is done.** No configuration, denominator history, source, header, compiler profile or accepted ledger was changed. No original bytes/disassembly are emitted.", "",
             "## Evidence standard", "",
             "The integrator explicitly selected the following conservative interpretation of `docs/ACCEPTANCE.md`:", "",
             "- **Compiler runtime (`runtime`)**: both the original routine's ABI behaviour and ARMCC emission of that ABI helper are witnessed, or a names-overlay binding is proved by a whole natural caller MATCH. Named compiler-generated relocation-free whole-body identity is independently sufficient for `__ARM_common_*` sections. ABI-emulation contracts plus emitted-operator symbol evidence are admitted without claiming a whole-caller match. Ordinary string/math/formatting code is not included merely because it belongs to a runtime-shaped graph.",
             "- **Ordinary libc/libm (`library`)**: exact known-library identity is required. Semantic identity, a caller MATCH or an instruction-set/address neighborhood alone does not authenticate the implementation. No supplied ordinary-library candidate meets this threshold.",
             "- **Kernel ABI (`assembly`)**: a required r4/r5 contract rejected by ARMCC #1501/#1505, or retained no-frame/no-C-shape compiler-grid evidence. A failed arbitrary C implementation is not proof of impossibility. Only the specifically investigated literal stubs are admitted; nine shape-only adapters stay creditable.",
             "- **Thumb** is not a provenance class. All 67 residue rows are reviewed separately; only the proven no-cookie array-construction helper meets the supplied original-callsite compiler-emission threshold. The other 66 stay creditable. Generic guard/destructor emission probes do not bind an original address.",
             "- **Entry recovery**: never infer code from a numeric text pointer alone. A closed, bounded CFG with known transfers, no undecodable/open exits and no pool/code overlap proves the long pointer-only bodies. Weak-prologue prefix repair additionally requires an unconditional bounded register-setup prefix, body input dependency, independently delimited unowned gap, no unknown callee-saved inputs, and noncrossing literal ownership. Protected and independently referenced starts cannot move.", "",
             "Runtime proof source: `scratch/integrator/runtime-helpers/REPORT.md` (reported 3,552 ABI cases and retained caller/equality evidence, not rerun here). Non-C proof source: `scratch/integrator/armcc-ext/REPORT.md` and `research/compid/r2/REPORT.md`. SDK/handwritten ownership beyond the proved ABI constraint remains **[INFERENCE]**. The reports' former private worktree artifacts are not all retained in this checkout; this revision cites the retained reports and does not fabricate fresh probe receipts.", "",
             "Public ABI corroboration, not byte-identity evidence: [ARM runtime ABI](https://github.com/ARM-software/abi-aa/blob/main/rtabi32/rtabi32.rst), [3dbrew kernel ABI](https://www.3dbrew.org/wiki/Kernel_ABI), [3dbrew SVC](https://www.3dbrew.org/wiki/SVC), [ARMCC __svc limits](https://support.arm.com/documentation/dui0472/m/Compiler-specific-Features/--svc).", "",
             "## Totals versus v7", "", "| Measure | v7 | v8 | Delta |", "| --- | ---: | ---: | ---: |"]
    measures = [
        ("Top-level owners", len(old), len(functions)),
        ("Callable starts/entries", sum(len(f["entries"]) for f in old.values()), sum(len(f["entries"]) for f in functions.values())),
        ("Creditable ordinary functions", sum(f["kind"] == "function" for f in old.values()), candidate["stats"]["creditable_functions"]),
        ("Creditable extent bytes", sum(f["size"] for f in old.values() if f["kind"] == "function"), candidate["stats"]["creditable_bytes"]),
        ("Unknown bytes", baseline["stats"]["bytes_by_range_kind"]["unknown"], candidate["stats"]["bytes_by_range_kind"].get("unknown", 0)),
        ("Unknown ranges", baseline["stats"]["unknown_ranges"], candidate["stats"]["unknown_ranges"]),
        ("Anomalies", len(baseline["anomalies"]), len(candidate["anomalies"])),
        ("Instruction bytes", baseline["stats"]["code_bytes"], candidate["stats"]["code_bytes"]),
        ("Pool bytes", baseline["stats"]["pool_bytes"], candidate["stats"]["pool_bytes"]),
    ]
    for label, before, after in measures:
        lines.append(f"| {label} | {before:,} | {after:,} | {after-before:+,} |")
    lines += ["", "Classification changes are denominator corrections, never authored-C credit. All sizes include owned pools/literals. Existing multi-entry helpers retain their structural kind and entries while gaining explicit runtime evidence.", "", "## Every boundary correction", "",
              "Only these old starts are replaced. All other v7 starts and secondary entries survive in the same ISA; no accepted/protected address/size changes.", "",
              "| v7 start | v7 bytes | v8 start | v8 bytes | Register setup / pointer witness |", "| --- | ---: | --- | ---: | --- |"]
    for row in candidate["leading_instruction_repairs"]:
        address, previous = row["address"], row["old_address"]
        writes = sorted({register for ins in row["instructions"] for register in ins["writes"]})
        lines.append(f"| `0x{previous:08x}` | {old[previous]['size']} | `0x{address:08x}` | {functions[address]['size']} | {', '.join(writes)}; pointer={row['pointer_witness']} |")
    lines += ["", "The two reported corrections are explicitly included: `0x00113ca8`→`0x00113ca4`, ending at `0x00113da0` with the restored literal at `0x00113d9c`; `0x001f50dc`→`0x001f50d8`, whose prefix loads the static-init object pointer. Exact register/literal-range facts are in candidate `leading_instruction_repairs`; every gap disposition is in `leading_instruction_inventory`.", "", "## Every new start (excluding shifted boundaries)", "", "| Address | Bytes | Evidence |", "| --- | ---: | --- |"]
    shifted = {row["address"] for row in candidate["leading_instruction_repairs"]}
    for address in sorted(functions.keys() - old.keys() - shifted):
        function = functions[address]
        refs = [ref for row in review["pointer_census"] if row["address"] == address for ref in row["references"]]
        text = "; ".join(f"{ref['kind']} pointer at 0x{ref['address']:08x}" for ref in refs)
        proof = next((row for row in candidate["pointer_entry_inventory"] if row["address"] == address), None)
        if proof:
            text += f"; {proof['reason']}"
            if proof["reason"] == "armlink_pointer_prefix":
                text += f"; exact tail NOP 0x{proof['tail_nop']:08x} → existing entry 0x{proof['target']:08x}"
        lines.append(f"| `0x{address:08x}` | {function['size']} | {', '.join(function['evidence'])}; {text} |")
    lines += ["", "`0x00110cc8` is the callback pointer loaded by `func_00107f0c` and passed in r1 to `0x0019f338`; the latter stores the incoming callback in its runtime registration state. The old 32-linear-instruction heuristic rejected its branch-rich body. The new CFG witness has no open exits and one return, and owns 464 instruction bytes plus its literal pool. `0x0014b358` is independently literal-pointer-referenced and has a closed 224-byte graph. No arbitrary interior numeric pointer becomes a start.", "",
              f"The {len(candidate['pointer_prefix_wrappers'])} pointer-prefix wrappers preserve their independently callable successors. Their register-only setup ends at the exact armlink MOV-NOP tail relocation, with no stack/LR modification, calls, pools or uninitialized incoming flags; architectural NOP hints and arbitrary fallthrough are rejected. These logical edges are explicit in `pointer_prefix_wrappers` and their per-pointer proofs, not inferred C matching credit.", "",
              "## Every admitted classification", "", "| Address | Bytes | Old/new kind | Identity | Evidence |", "| --- | ---: | --- | --- | --- |"]
    for row in review["classifications"]:
        address = row["address"]
        lines.append(f"| `0x{address:08x}` | {row['size']} | {old[address]['kind']} → {functions[address]['kind']} | `{row['identity']}` | {row['evidence']} |")
    lines += ["", "Every admitted row is also embedded in its function's `classification_evidence`, bound to image hash, extent, ISA, basis and named proof. Runtime/library ranges continue to have physical `function` ownership: consumers use function `kind` to determine C eligibility, so no bytes disappear from the map.", "", "## All 67 Thumb residue dispositions", "", "| Address | Bytes | Disposition | Individual evidence |", "| --- | ---: | --- | --- |"]
    for row in review["thumb_dispositions"]:
        lines.append(f"| `{row['address']}` | {row['size']} | {row['disposition']} | {row['evidence']} |")
    lines += ["", "## All author library-candidate dispositions", "", "Rows lacking an exact identity or adequate compiler/runtime/library proof remain creditable; CP15/SVC use is not library ownership.", "", "| Address | Disposition | Report / original notes |", "| --- | --- | --- |"]
    for row in review["author_dispositions"]:
        notes = row.get("notes", "").replace("|", "\\|").replace("\n", " ")
        lines.append(f"| `{row['address']}` | {row['disposition']} | `{row['report']}` — {notes} |")
    lines += ["", "## Shape-only literal SVC adapters retained as C-eligible", ""]
    for address in sorted(LITERAL_STUBS - KERNEL_ABIS.keys()):
        lines.append(f"- `0x{address:08x}` / {functions[address]['size']} B: literal ABI/marshaling shape only; no hard ARMCC impossibility proof. SDK ownership [INFERENCE]; retained as ordinary function.")
    lines += ["", "## Protected manifests and complete preservation", "", "| Manifest | Functions | Extent bytes | Address/size changes |", "| --- | ---: | ---: | --- |"]
    for path, summary in review["protected_summary"].items():
        lines.append(f"| `{path}` | {summary['functions']} | {summary['bytes']} | {summary['changes']} |")
    lines += ["", f"c006 presence at generation: **{review['c006_present']}**. If absent, no future c006 contents are claimed protected; the generator automatically includes it when it exists.", "",
              f"Complete preservation results: `{json.dumps(review['preservation'], sort_keys=True)}`. `validate_catalogue` structural issues: `{review['structural_issues']}`. All map ranges still tile the original mapped segments. Every whole-record change is either a listed shift/new start or one admitted evidence/classification addition; the machine-readable `changed_records` list names all changed fields.", "", "## Every v7 unknown range: byte-conserving accounting", "",
              "All 189 original ranges / 8,704 B are listed below, including every unchanged/open byte. Each partition appears in `revision_review.unknown_accounting`; `unknown` means still open, not guessed data or padding. A reason is supplied per partition.", "",
              "| v7 range (exclusive end) | Bytes | v8 partitions and reason |", "| --- | ---: | --- |"]
    for row in review["unknown_accounting"]:
        parts = "; ".join(f"0x{p['start']:08x}–0x{p['end']:08x}: {p['kind']}" + (f" owner 0x{p['owner']:08x}" if p["owner"] is not None else "") + f" — {p['reason']}" for p in row["parts"])
        lines.append(f"| `0x{row['start']:08x}–0x{row['end']:08x}` | {row['end']-row['start']} | {parts} |")
    lines += ["", "## Every v7 anomaly", "", "No issue is suppressed to manufacture closure. Resolved rows follow actual entry/pool recovery; the open rows remain present in `anomalies`.", "", "| Address | Issue | Disposition / reason |", "| --- | --- | --- |"]
    for row in review["anomaly_accounting"]:
        lines.append(f"| `0x{row['address']:08x}` | {row['issue']} | **{row['disposition']}** — {row['reason']} |")
    lines += ["", "## Pointer and leading-gap census", "",
              f"All aligned nonexecuted text words (pools, literals, unknown/padding) and all aligned rodata/data words were scanned. Full target/source metadata (no contents) are in `revision_review.pointer_census`: {len(review['pointer_census']):,} distinct in-text pointer-shaped targets. Dispositions: `{dict(sorted(Counter(row['disposition'] for row in review['pointer_census']).items()))}`. Pointer-shaped data/interior labels are not asserted to be functions or callbacks without a control-flow witness. Text words containing decoded ARM/Thumb instructions are excluded as pointer sources to avoid instruction-bit false positives. Per-block constant propagation records direct indirect-transfer and call-argument uses; values are killed at unknown definitions, calls and control-flow joins. This bounded propagation is not a whole-program indirect-call proof.", "",
              "The leading-gap inventory covers every ARM owner preceded by unowned bytes in the final recovery pass, plus each repaired old start. Independently referenced entries and protected extents are explicit withheld dispositions. Prefix scans are bounded to 256 bytes; longer undecidable gaps remain open rather than being absorbed.", "", "## Reproduction, provenance and open items", "",
              "From `scratch/worktrees/catalogue-v8`:", "", "```sh", "/home/michael/code/kanken-training-2-decomp/.venv/bin/python research/catalogue_v8.py --root /home/michael/code/kanken-training-2-decomp", "/home/michael/code/kanken-training-2-decomp/.venv/bin/python -m unittest tests.test_analyze", "```", "",
              "Generation returns exit 0 only after structural validation, exact protected address/size preservation, callable-entry preservation except the listed boundary corrections, and byte-conserving accounting. Only `tests.test_analyze` is authorized; its exercised result is recorded separately below. No full suite, build, compiler probe, prepare/finalize, adoption, denominator-history write or push is performed.", "",
              f"Open catalogue items: **{candidate['stats']['bytes_by_range_kind'].get('unknown', 0):,} bytes in {candidate['stats']['unknown_ranges']} ranges; {len(candidate['anomalies'])} anomalies**. In addition, 66 Thumb runtime-role candidates, all ordinary-library candidates without known-library identity, and nine shape-only SVC adapters remain C-eligible. This does not satisfy the full decompilation's done gate; it explicitly completes the requested census and accounts for the evidence limits instead of silently reducing the denominator.", "",
              "Input hashes:", ""]
    for path, sha in review["input_sha256"].items():
        lines.append(f"- `{path}`: `{sha}`")
    return "\n".join(lines) + "\n"


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, required=True)
    parser.add_argument("--out", type=Path)
    args = parser.parse_args(argv)
    root = args.root.resolve()
    out = args.out or root / "scratch/analyze"
    started = time.perf_counter()
    baseline_path = root / "scratch/analyze/catalogue-v7.json"
    baseline = load(baseline_path)
    names = load(root / "config/names.json")["symbols"]
    adopted = load(root / "config/catalogue.json")["functions"]
    adopted.sort(key=lambda function: function["address"])
    adopted_starts = [function["address"] for function in adopted]
    manifests = [root / name for name in ("config/matches.json", "config/candidates/c004.json", "config/candidates/c005.json", "config/candidates/c006.json") if (root / name).is_file()]
    protected = {}
    protected_summary = {}
    for path in manifests:
        rows = []
        for unit in load(path)["units"]:
            for name in unit["functions"]:
                address = (names[name]["address"] if name in names else int(name.removeprefix("func_"), 16)) & ~1
                function = owner(adopted, adopted_starts, address)
                if function is None:
                    raise ValueError(f"protected {name} has no adopted owner")
                protected[address] = function
                rows.append((address, function["size"]))
        protected_summary[str(path.relative_to(root))] = {"functions": len(rows), "bytes": sum(size for _, size in rows), "changes": []}
    image = (root / "orig/code.bin").read_bytes()
    layout = ctrfmt.parse_exheader((root / "orig/exheader.bin").read_bytes())
    classifications = classification_records(baseline)
    candidate = analyze.build_catalogue(image, layout, protected_functions=protected, classifications=classifications)
    new = {function["address"]: function for function in candidate["functions"]}
    old = {function["address"]: function for function in baseline["functions"]}
    repairs = {row["old_address"]: row["address"] for row in candidate["leading_instruction_repairs"]}
    old_entries = {(entry["address"], entry["isa"]) for function in old.values() for entry in function["entries"]}
    new_entries = {(entry["address"], entry["isa"]) for function in new.values() for entry in function["entries"]}
    losses = sorted(old_entries - new_entries - {(address, "arm") for address in repairs})
    protected_changes = [address for address, function in protected.items()
                         if address not in new or (new[address]["address"], new[address]["size"]) != (function["address"], function["size"])]
    unexpected = [address for address in old.keys() & new.keys()
                  if (old[address]["size"], old[address]["isa"], old[address]["entries"]) != (new[address]["size"], new[address]["isa"], new[address]["entries"])]
    structural = analyze.validate_catalogue(candidate, image, layout)
    if losses or protected_changes or unexpected or structural:
        raise ValueError(f"preservation/structural failure: entries={losses}, protected={protected_changes}, extents={unexpected}, structural={structural}")
    unknown_rows, anomaly_rows = accounting(baseline, candidate, image, layout)
    classified = {row["address"] for row in classifications}
    thumb_rows = []
    for row in load(root / "scratch/integrator/residue/avoided.json"):
        if row["class"] == "thumb-runtime":
            address = int(row["address"], 16)
            thumb_rows.append({"address": row["address"], "size": row["size"],
                               "disposition": "runtime: bound natural C++ caller proof" if address in classified else "creditable: original-address compiler-emission/library identity missing",
                               "evidence": row["evidence"]})
    author_rows = candidate_rows(root)
    for row in author_rows:
        address = int(row["address"], 16)
        row["disposition"] = "runtime: admitted compiler identity/ABI evidence" if address in classified else "creditable: evidence below ordinary-library/original-callsite runtime threshold"
    inputs = [baseline_path, root / "config/catalogue.json", root / "config/names.json", root / RUNTIME_REPORT,
              root / RESIDUE_REPORT, root / ABI_REPORT, root / "scratch/integrator/residue/avoided.json", *manifests]
    inputs.extend(root / row["report"] for row in author_rows)
    worktree = Path(__file__).resolve().parents[1]
    inputs.extend(worktree / name for name in ("tools/analyze.py", "tests/test_analyze.py", "research/catalogue_v8.py"))
    candidate["revision_review"] = {
        "name": "catalogue-v8", "baseline_sha256": digest(baseline_path),
        "classifications": classifications, "thumb_dispositions": thumb_rows, "author_dispositions": author_rows,
        "unknown_accounting": unknown_rows, "anomaly_accounting": anomaly_rows,
        "pointer_census": pointer_census(candidate, image, layout), "protected_summary": protected_summary,
        "protected_rows": [{"address": address, "size": function["size"]} for address, function in sorted(protected.items())],
        "c006_present": (root / "config/candidates/c006.json").is_file(), "structural_issues": structural,
        "preservation": {"unexpected_entry_losses": losses, "protected_changes": protected_changes, "unexpected_extent_changes": unexpected,
                         "old_starts_replaced_only_by_listed_repairs": old.keys() - new.keys() == repairs.keys()},
        "changed_records": [{"address": address, "status": "removed" if address not in new else "new" if address not in old else "changed",
                             "changed_fields": sorted(key for key in old.get(address, {}).keys() | new.get(address, {}).keys()
                                                      if old.get(address, {}).get(key) != new.get(address, {}).get(key))}
                            for address in sorted(old.keys() | new.keys()) if old.get(address) != new.get(address)],
        "input_sha256": {str(path.relative_to(root)): digest(path) for path in sorted(set(inputs))},
    }
    out.mkdir(parents=True, exist_ok=True)
    (out / "catalogue-v8.json").write_bytes(analyze.serialize_catalogue(candidate))
    text = report(candidate, baseline)
    text += f"\nCandidate SHA-256: `{digest(out / 'catalogue-v8.json')}`. Generation exit: **0**.\n"
    (out / "REPORT-v8.md").write_text(text, encoding="utf-8")
    print(json.dumps({"exit_code": 0, "functions": len(new), "creditable_functions": candidate["stats"]["creditable_functions"],
                      "creditable_bytes": candidate["stats"]["creditable_bytes"], "unknown_bytes": candidate["stats"]["bytes_by_range_kind"].get("unknown", 0),
                      "anomalies": len(candidate["anomalies"]), "seconds": round(time.perf_counter()-started, 3)}, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
