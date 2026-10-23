"""Join existing metadata into a private, reproducible decompilation residue table.

Run from the isolated tools worktree, pointing --root at the repository containing
private scratch records::

    .venv/bin/python scratch/worktrees/residue-table/tools/residue.py --root .

Only scratch/integrator/residue/RESIDUE-r1.{json,md} is written. No compiler,
matcher, ROM, packet, diff, source, or disassembly input is opened. The JSON binds
all consumed inputs by SHA-256 and pins branch manifests to immutable commits.
The same input bytes and branch tips produce identical output bytes; filesystem
mtimes, wall-clock time, absolute checkout paths and directory order are ignored.

Accepted ledger ownership precedes pending checkpoint manifests, which precede
residue. A batch assignment is NOT an attempted/completed ladder rung: a published
target result or an explicit census completion witness is required. Context-only
rows can improve a reported residual but cannot complete an assigned target rung.
All six author rungs (1, 2, 3, 5, 5b, 5c) are required for unmatched exhaustion.
Step 4 is either a completed bounded search or an evidenced adapter/operational
boundary, never a claim that natural C is impossible. An improved <=32-byte seed
reopens an older above-bound refusal. Unknown residuals do not waive step 4.

Free-form author/hold text is used ONLY to assign a small diagnosis/blocker
vocabulary. It is never copied into outputs. Evidence paths/pointers, hashes,
identities, sizes, residual counts, batch IDs and states are the output allowlist.
These are reported metadata, not re-derived source/receipt acceptance. Exact
unheld direct author MATCHes under frozen profiles are pending_intake; Main must
collect/review them. They share the actionable list with missing ladder rungs.
"""
from __future__ import annotations

import argparse
from collections import Counter
from dataclasses import dataclass, field
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
from typing import Any, Iterator


AUTHOR_STEPS = ("1", "2", "3", "5", "5b", "5c")
STEPS = (*AUTHOR_STEPS, "4")
CLASSES = ("held_contract", "header_blocked", "near_miss_exhausted",
           "never_matched_exhausted", "awaits_user_ruling", "ladder_incomplete",
           "pending_intake")
FROZEN_PROFILES = frozenset(("game", "sdk", "middleware"))
FUNCTION_RE = re.compile(r"func_([0-9a-fA-F]{8})\Z")
BATCH_RE = re.compile(r"[A-Za-z][A-Za-z0-9_-]*\Z")
HOLD_NAME_RE = re.compile(r"defer|handoff|dispositions", re.I)
HOLD_RE = re.compile(r"defer|held|hold|withheld|blocked|unresolved", re.I)
CLOSED_RE = re.compile(r"closed|resolved_deferrals|collected|accepted|repaired", re.I)
NEEDS_KEYS = ("integration_needs", "header_issues", "header_conflicts", "header_needs",
              "needs_headers", "needs_shared_header", "header_blocked",
              "canonical_header_blocked", "current_header_compile_blocked",
              "integration_hold", "blocked_contract", "integration_blocked")
TEXT_KEYS = ("notes", "reason", "diagnosis", "family", "category", "subclass",
             "classification", "status", "decision", "blocker", "blockers",
             "adoption_flags", "hard_dispatch_holds", *NEEDS_KEYS)
FAMILY_PATTERNS = {
    "register_role": r"register|allocat|spill|live.range",
    "instruction_scheduling": r"schedul|load.order|store.order|code.motion",
    "floating_point": r"\bvfp\b|floating.point|rounding|fpmode",
    "literal_pool": r"literal.pool|pool.order",
    "native_owner_type": r"native|owner|receiver|constructor|destructor|vtable|nominal|hierarchy",
    "callback_contract": r"callback|function.pointer|parameter|prototype|return.contract",
    "atomic_exclusive": r"exclusive|\bldrex\b|\bstrex\b|atomic",
    "control_flow": r"\bloop\b|\bbranch\b|control.flow",
    "permuter_adapter": r"parser|adapter|oracle.refus",
    "source_policy": r"steering|punning|inline.asm|byte.blob",
    "header_contract": r"header|canonical.decl|redecl",
}
BLOCKER_PATTERNS = {
    "native_owner_identity": FAMILY_PATTERNS["native_owner_type"],
    "callback_signature": FAMILY_PATTERNS["callback_contract"],
    "canonical_header": r"header|canonical.compilation|incompatible.declaration|redecl",
    "symbol_identity": r"unresolved.symbol|name.admission|unadmitted|unresolved.identity|names.admission",
    "source_policy": FAMILY_PATTERNS["source_policy"],
    "linker_model": r"linker.model|zero.size.alias|unused.delet|\bu1\b|model.qualified",
}


def canonical(value: Any) -> bytes:
    return (json.dumps(value, sort_keys=True, indent=2, ensure_ascii=True) + "\n").encode()


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def integer(value: Any) -> int | None:
    if isinstance(value, bool):
        return None
    if isinstance(value, int):
        return value
    if isinstance(value, str):
        try:
            return int(value, 16 if value.lower().startswith("0x") or
                       re.fullmatch(r"[0-9a-fA-F]{8}", value) else 10)
        except ValueError:
            pass
    return None


def identity(value: Any) -> str | None:
    """Normalize a name/function/address without mining identities from prose."""
    if isinstance(value, dict):
        for key in ("function", "name", "address"):
            name = identity(value.get(key))
            if name:
                return name
        return None
    if isinstance(value, str) and (match := FUNCTION_RE.fullmatch(value)):
        return f"func_{int(match[1], 16):08x}"
    address = integer(value)
    return f"func_{address:08x}" if address is not None and 0x100000 <= address <= 0xffffffff else None


def batch_step(batch: str) -> str | None:
    if batch.startswith("r4"):
        return "2"
    for prefix, step in (("tu", "3"), ("nf", "5"), ("ns", "5"),
                         ("np", "5b"), ("nm", "5c")):
        if batch.startswith(prefix):
            return step
    if re.match(r"r[123]", batch):
        return "1"
    return None


def residual(row: dict) -> tuple[int | None, str | None, int | None]:
    """Read the selected outcome, never baseline/before/diagnostic alternatives."""
    for key in ("residual_after", "residual", "current_residual", "residual_evidence",
                "diff", "best_diff", "measured_diff", "residual_metrics", "final_diff"):
        value = row.get(key)
        if isinstance(value, dict):
            for metric in ("differing_bytes_including_size_delta", "differing_bytes"):
                count = value.get(metric)
                if isinstance(count, int) and not isinstance(count, bool) and count >= 0:
                    extent = integer(value.get("target_size", value.get("measured_target_size")))
                    return count, f"{key}.{metric}", extent
        elif isinstance(value, int) and not isinstance(value, bool) and value >= 0:
            return value, key, None
    for key in ("differing_bytes_including_size_delta", "differing_bytes",
                "residual_differing_bytes", "residual_after_differing_bytes",
                "residual_differing_bytes_after", "differing_bytes_after",
                "residual_after_bytes", "differing_byte_count", "different_bytes"):
        value = row.get(key)
        if isinstance(value, int) and not isinstance(value, bool) and value >= 0:
            return value, key, integer(row.get("target_size"))
    return None, None, None


def text(value: Any) -> str:
    if isinstance(value, str):
        return value
    if isinstance(value, dict):
        return " ".join(f"{key} {text(val)}" for key, val in value.items())
    if isinstance(value, list):
        return " ".join(text(val) for val in value)
    return ""


def relevant_text(row: dict) -> str:
    return " ".join(text(row.get(key)) for key in TEXT_KEYS)


def meaningful_need(value: Any) -> bool:
    if not value:
        return False
    if value is True:
        return True
    if isinstance(value, (list, dict)):
        vals = value.values() if isinstance(value, dict) else value
        return any(meaningful_need(item) for item in vals)
    if not isinstance(value, str):
        return False
    stripped = value.strip().lower().rstrip(".")
    if stripped in ("none", "no", "false", "n/a", "not applicable", "[]", "{}"):
        return False
    if re.fullmatch(r"no (?:new |additional |shared |canonical )?(?:header|names?|integration|contract)(?:[ /a-z-]*)(?:needed|required|changes|issues)", stripped):
        return False
    return True


def safe_path(value: Any, root: Path) -> str | None:
    """Retain path-shaped metadata only, never messages or inline diagnostics."""
    if not isinstance(value, str) or len(value) > 1000 or any(c in value for c in "\n\r\t"):
        return None
    if value.startswith(str(root) + "/"):
        value = value[len(str(root)) + 1:]
    elif "/kanken-training-2-decomp/" in value:
        value = value.split("/kanken-training-2-decomp/", 1)[1]
    if re.fullmatch(r"[A-Za-z0-9_./+@:-]+\.(?:json|jsonl|md|c|cpp|cc|h|hpp)", value):
        return value
    return None


def walk_rows(value: Any, pointer: str = "") -> Iterator[tuple[dict, str]]:
    """Walk structured records, retaining JSON pointers for every direct identity."""
    if isinstance(value, dict):
        if identity(value):
            yield value, pointer
        for key, val in value.items():
            escaped = str(key).replace("~", "~0").replace("/", "~1")
            child = f"{pointer}/{escaped}"
            if isinstance(val, dict) and identity(key):
                yield {"function": key, **val}, child
            if isinstance(val, (dict, list)):
                yield from walk_rows(val, child)
    elif isinstance(value, list):
        for index, val in enumerate(value):
            if isinstance(val, (dict, list)):
                yield from walk_rows(val, f"{pointer}/{index}")


class Inputs:
    def __init__(self, root: Path):
        self.root = root
        self.hashes: dict[str, str] = {}
        self.cache: dict[str, Any] = {}
        self.absent: set[str] = set()
        self.hashes["generator:tools/residue.py"] = digest(Path(__file__).read_bytes())

    def load(self, path: str, required: bool = False) -> Any:
        if path in self.cache:
            return self.cache[path]
        candidate = self.root / path
        if not candidate.is_file():
            if required:
                raise ValueError(f"required metadata input missing: {path}")
            self.absent.add(path)
            return None
        raw = candidate.read_bytes()
        return self.consume(path, raw)

    def consume(self, label: str, raw: bytes) -> Any:
        self.hashes[label] = digest(raw)
        if label.lower().endswith(".json"):
            try:
                value = json.loads(raw)
            except (ValueError, UnicodeError) as exc:
                raise ValueError(f"invalid JSON metadata: {label}") from exc
        else:
            value = raw.decode("utf-8")
        self.cache[label] = value
        return value

    def git(self, *args: str) -> bytes:
        process = subprocess.run(["git", "-C", str(self.root), *args],
                                 stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=False)
        if process.returncode:
            raise ValueError(f"git metadata query failed: {' '.join(args)}")
        return process.stdout

    def branch_commit(self, branch: str) -> str | None:
        try:
            return self.git("rev-parse", "--verify", f"refs/heads/{branch}").decode().strip()
        except ValueError:
            self.absent.add(f"branch:{branch}")
            return None

    def branch_paths(self, commit: str, directory: str) -> list[str]:
        raw = self.git("ls-tree", "-r", "--name-only", "-z", commit, "--", directory)
        return sorted(p.decode() for p in raw.split(b"\0") if p)

    def branch_json(self, commit: str, path: str) -> tuple[str, Any]:
        label = f"git:{commit}:{path}"
        if label not in self.cache:
            self.consume(label, self.git("show", f"{commit}:{path}"))
        return label, self.cache[label]


@dataclass
class Function:
    size: int
    evidence: set[str] = field(default_factory=set)
    attempts: dict[str, set[str]] = field(default_factory=lambda: {s: set() for s in STEPS})
    assignments: dict[str, set[str]] = field(default_factory=lambda: {s: set() for s in STEPS})
    step_evidence: dict[str, set[str]] = field(default_factory=lambda: {s: set() for s in STEPS})
    completed: set[str] = field(default_factory=set)
    observations: list[dict] = field(default_factory=list)
    holds: list[dict] = field(default_factory=list)
    families: set[str] = field(default_factory=set)
    permuter: list[dict] = field(default_factory=list)
    harvest: list[dict] = field(default_factory=list)
    stale_evidence: set[str] = field(default_factory=set)


class Joiner:
    def __init__(self, root: Path):
        self.inputs = Inputs(root)
        self.root = root
        self.functions: dict[str, Function] = {}
        self.batch_targets: dict[str, set[str]] = {}
        self.batch_steps: dict[str, str | None] = {}
        self.pending_sources: list[dict] = []
        self.pending: dict[str, list[str]] = {}
        self.rulings: dict[str, str] = {}

    def observe(self, name: str, row: dict, evidence: str, batch: str | None,
                origin: str, default_status: str | None = None) -> None:
        target = self.functions[name]
        target.evidence.add(evidence)
        if batch is None:
            for key in ("source", "prev_source", "receipt", "prev_receipt", "prev_author"):
                value = row.get(key)
                if isinstance(value, str):
                    if match := re.search(r"scratch/authors/([^/]+)/", value):
                        batch = match[1]
                        break
                    if key == "prev_author" and BATCH_RE.fullmatch(value):
                        batch = value
        size = integer(row.get("target_bytes", row.get("size", row.get("target_size"))))
        count, metric, measured_size = residual(row)
        if (size is not None and size != target.size) or (measured_size is not None and measured_size != target.size):
            target.stale_evidence.add(evidence)
            return
        status = str(row.get("status", default_status or "unknown")).lower()
        if status in ("match", "context_match") and count is None:
            count, metric = 0, "reported_status"
        prose = relevant_text(row)
        target.families.update(k for k, pattern in FAMILY_PATTERNS.items() if re.search(pattern, prose, re.I))
        if count is not None:
            observation = {"residual_bytes": count, "metric": metric, "batch": batch,
                           "status": status if status in ("match", "mismatch", "context_match", "extent", "library_candidate") else "reported",
                           "evidence": evidence, "origin": origin}
            for key in ("source", "receipt", "profile"):
                val = row.get(key, row.get(f"prev_{key}"))
                if key == "profile":
                    if val in FROZEN_PROFILES:
                        observation[key] = val
                elif path := safe_path(val, self.root):
                    observation[key] = path
            target.observations.append(observation)
        needs = {key: row[key] for key in NEEDS_KEYS if meaningful_need(row.get(key))}
        if needs:
            need_text = " ".join(f"{key} {text(val)}" for key, val in needs.items())
            if re.search(r"header|contract|owner|receiver|type|callback|native|symbol|name", need_text, re.I):
                blocked = any(meaningful_need(row.get(key)) for key in (
                    "header_blocked", "canonical_header_blocked", "current_header_compile_blocked"))
                self.add_hold(name, need_text, evidence, "author_integration_need", blocked)

    def add_hold(self, name: str, prose: str, evidence: str, basis: str,
                 blocks_authoring: bool = False) -> None:
        classes = sorted(k for k, pattern in BLOCKER_PATTERNS.items() if re.search(pattern, prose, re.I))
        if not classes:
            classes = ["contract_type"]
        awaiting = bool(re.search(r"await(?:s|ing)? (?:a |the )?(?:user )?(?:ruling|decision)|user (?:ruling|decision) (?:required|needed)", prose, re.I))
        resolved_by = []
        if awaiting and re.search(r"__svc|cp15|named.register|armcc.extension|compiler.intrinsic", prose, re.I):
            resolved_by = [p for p in self.rulings if "armcc-extensions" in p]
            awaiting = not resolved_by
        hold = {"blocker_classes": classes, "evidence": evidence, "basis": basis,
                "awaits_user_ruling": awaiting, "blocks_authoring": blocks_authoring}
        if resolved_by:
            hold["resolved_policy_question_by"] = sorted(resolved_by)
        self.functions[name].holds.append(hold)
        self.functions[name].evidence.add(evidence)
        self.functions[name].families.update(k for k, pattern in FAMILY_PATTERNS.items() if re.search(pattern, prose, re.I))

    def select_pending(self, specs: list[tuple[tuple[str, ...], str]]) -> None:
        for branches, path in specs:
            selected = None
            tried = []
            for branch in branches:
                commit = self.inputs.branch_commit(branch)
                tried.append(branch)
                if not commit:
                    continue
                if path not in self.inputs.branch_paths(commit, "config/candidates"):
                    self.inputs.absent.add(f"git:{commit}:{path}")
                    continue
                label, doc = self.inputs.branch_json(commit, path)
                selected = {"branch": branch, "commit": commit, "path": path,
                            "input": label, "fallback_from": tried[:-1]}
                for unit in doc.get("units", []):
                    for value in unit.get("functions", []):
                        name = identity(value)
                        if name in self.functions:
                            self.pending.setdefault(name, []).append(label)
                break
            if selected is None:
                raise ValueError(f"no committed pending manifest: {','.join(branches)}:{path}")
            self.pending_sources.append(selected)

    def load_batches(self) -> None:
        for path in sorted((self.root / "scratch/integrator/batches").glob("*.json")):
            label = path.relative_to(self.root).as_posix()
            doc = self.inputs.load(label)
            batch = path.stem
            step = batch_step(batch)
            if step is None and isinstance(doc, dict) and str(doc.get("step")) in STEPS:
                step = str(doc["step"])
            self.batch_steps[batch] = step
            rows = doc.get("functions", doc.get("targets", [])) if isinstance(doc, dict) else doc
            targets = {name for value in rows if (name := identity(value)) in self.functions}
            self.batch_targets[batch] = targets
            if step:
                for name in targets:
                    target = self.functions[name]
                    target.assignments[step].add(batch)
                    target.step_evidence[step].add(label)

    def load_census(self) -> None:
        label = "scratch/integrator/census-20261021-r3/LADDER.json"
        doc = self.inputs.load(label, required=True)
        self.inputs.load("scratch/integrator/census-20261021-r3/REPORT.md", required=True)
        for index, row in enumerate(doc["rows"]):
            name = identity(row)
            if name not in self.functions:
                continue
            evidence = f"{label}#/rows/{index}"
            target = self.functions[name]
            target.evidence.add(evidence)
            if integer(row.get("target_bytes")) != target.size:
                target.stale_evidence.add(evidence)
                continue
            for step, entries in row.get("completion_evidence", {}).items():
                if step not in STEPS:
                    continue
                if step in row.get("completed_steps", []):
                    target.completed.add(step)
                for entry in entries:
                    batch = entry.get("batch")
                    if isinstance(batch, str) and BATCH_RE.fullmatch(batch):
                        target.attempts[step].add(batch)
                    target.step_evidence[step].add(evidence)
            for key in ("best_bound_mismatch", "best_reported_mismatch"):
                best = row.get(key)
                if isinstance(best, dict):
                    record = best.get("author_result", "")
                    batch = Path(record).parent.name if record else None
                    self.observe(name, {**best, "status": best.get("reported_status")},
                                 f"{evidence}/{key}", batch, "census")
            for hold in row.get("current_checkpoint_deferrals", []):
                self.add_hold(name, relevant_text(hold), evidence, "census_contract_deferral")
        for label in ("scratch/integrator/escalation/RESIDUE_READY.json",
                      "scratch/integrator/escalation/QUEUES.json"):
            doc = self.inputs.load(label, required=True)
            for row, pointer in walk_rows(doc):
                name = identity(row)
                if name not in self.functions:
                    continue
                evidence = f"{label}#{pointer}"
                self.observe(name, row, evidence, None, "escalation")
                if (size := integer(row.get("size"))) is not None and size != self.functions[name].size:
                    continue
                for rung, info in row.get("ladder", {}).items():
                    match = re.match(r"step(5b|5c|[1-5])_", rung)
                    if not match or not isinstance(info, dict):
                        continue
                    step = match[1]
                    if info.get("completed") and step != "4":
                        self.functions[name].completed.add(step)
                        self.functions[name].step_evidence[step].add(evidence)
                    for entry in info.get("evidence", []):
                        if not entry.get("completed") or entry.get("context_only"):
                            continue
                        batch = entry.get("batch")
                        if not batch and entry.get("record"):
                            batch = Path(entry["record"]).parent.name
                        if isinstance(batch, str) and BATCH_RE.fullmatch(batch):
                            self.functions[name].attempts[step].add(batch)
                        self.functions[name].step_evidence[step].add(evidence)
                if row.get("hard_dispatch_holds"):
                    self.add_hold(name, relevant_text(row), evidence, "escalation_dispatch_hold")

    def load_authors(self) -> None:
        for path in sorted((self.root / "scratch/authors").glob("*/RESULT.json")):
            label = path.relative_to(self.root).as_posix()
            doc = self.inputs.load(label)
            batch = path.parent.name
            step = self.batch_steps.get(batch, batch_step(batch))
            targets = self.batch_targets.get(batch, set())
            for index, row in enumerate(doc.get("functions", [])):
                name = identity(row)
                if name not in self.functions:
                    continue
                evidence = f"{label}#/functions/{index}"
                scoped = doc.get("integration_needs", {})
                needs = []
                if isinstance(scoped, dict):
                    if meaningful_need(scoped.get(name)):
                        needs.append(scoped[name])
                    for group in scoped.values():
                        if isinstance(group, dict) and meaningful_need(group.get(name)):
                            needs.append(group[name])
                if needs:
                    row = {**row, "integration_needs": [row.get("integration_needs"), *needs]}
                self.observe(name, row, evidence, batch, "author")
                if step and name in targets and row.get("status") in ("match", "mismatch", "library_candidate", "extent"):
                    size = integer(row.get("size"))
                    if size is None or size == self.functions[name].size:
                        self.functions[name].completed.add(step)
                        self.functions[name].attempts[step].add(batch)
                        self.functions[name].step_evidence[step].add(evidence)

    def load_permuter(self) -> None:
        label = "scratch/permute/sweep-10/summary.json"
        doc = self.inputs.load(label, required=True)
        self.permuter_bound = integer(doc.get("policy", {}).get("operational_differing_byte_cap")) or 32
        for path in sorted((self.root / "scratch/permute/sweep-10").glob("HEAVY-SLOT-RELEASED-*.json")):
            release_label = path.relative_to(self.root).as_posix()
            release = self.inputs.load(release_label)
            for index, execution in enumerate(release.get("actual_row_exits", [])):
                name = identity(execution)
                if name in self.functions:
                    evidence = f"{release_label}#/actual_row_exits/{index}"
                    self.functions[name].step_evidence["4"].add(evidence)
                    self.functions[name].evidence.add(evidence)
        for key, row in sorted(doc.get("rows", {}).items()):
            name = identity(row) or identity(key)
            if name not in self.functions:
                continue
            evidence = f"{label}#/rows/{key}"
            state = row.get("state")
            if state not in ("adapter_refused", "plateau", "exhausted", "match",
                             "not_applicable_operational_boundary", "skipped_claimed", "pending"):
                state = "unclassified"
            target = self.functions[name]
            target.permuter.append({"state": state, "evidence": evidence,
                                   "above_bound": row.get("stop_reason") == "source_bound_residual_above_32_differing_bytes"})
            target.evidence.add(evidence)
            if state in ("plateau", "exhausted", "match"):
                target.completed.add("4")
                target.attempts["4"].add("sweep-10")
                target.step_evidence["4"].add(evidence)
            elif state == "adapter_refused":
                target.attempts["4"].add("sweep-10")
                target.step_evidence["4"].add(evidence)
            for index, final in enumerate(row.get("final_functions") or []):
                if identity(final) == name:
                    measured = {**final, "differing_bytes": final.get("changed_bytes")}
                    self.observe(name, measured, f"{evidence}/final_functions/{index}", "sweep-10", "permuter")
            applicability = row.get("applicability", {})
            if isinstance(applicability.get("residual_evidence"), dict):
                self.observe(name, {"residual": applicability["residual_evidence"],
                                    "source": row.get("source"), "receipt": row.get("origin_receipt"),
                                    "status": "mismatch"}, f"{evidence}/applicability", "sweep-10", "permuter_seed")
        for path in sorted((self.root / "scratch/permute/step4-nm").rglob("*.json")):
            label = path.relative_to(self.root).as_posix()
            doc = self.inputs.load(label)
            for row, pointer in walk_rows(doc):
                name = identity(row)
                if name in self.functions and re.search(r"refus|adapter", relevant_text(row), re.I):
                    evidence = f"{label}#{pointer}"
                    self.functions[name].permuter.append({"state": "adapter_refused", "evidence": evidence, "above_bound": False})
                    self.functions[name].attempts["4"].add("step4-nm")
                    self.functions[name].step_evidence["4"].add(evidence)
                    self.functions[name].evidence.add(evidence)

    def holds_from(self, label: str, doc: Any) -> None:
        def visit(value: Any, pointer: str, inherited: str = "") -> None:
            if isinstance(value, dict):
                prose = relevant_text(value)
                contextual = f"{inherited} {prose}"
                names = [identity(value)]
                for key, values in value.items():
                    if (key in ("functions", "names", "targets") or
                            (HOLD_RE.search(key) and not CLOSED_RE.search(key))):
                        if isinstance(values, list):
                            names.extend(identity(v) for v in values if not isinstance(v, dict))
                is_hold = bool(HOLD_RE.search(contextual + " " + label))
                if is_hold and not CLOSED_RE.search(pointer) and not re.search(r"^(?:collected|closed|resolved|accepted)", str(value.get("status", "")), re.I):
                    for name in set(names):
                        if name in self.functions:
                            self.add_hold(name, contextual, f"{label}#{pointer}", "checkpoint_deferral")
                for key, val in value.items():
                    if isinstance(val, (dict, list)) and key not in ("evidence", "verification", "model_uses"):
                        child = f"{pointer}/{key}"
                        # Do not propagate whole-handoff status into candidate rows.
                        context = contextual if identity(value) or key in ("functions", "names", "targets") else key
                        visit(val, child, context)
            elif isinstance(value, list):
                for index, val in enumerate(value):
                    if isinstance(val, (dict, list)):
                        visit(val, f"{pointer}/{index}", inherited)
        visit(doc, "")

    def load_holds(self) -> None:
        for directory in ("c009", "c010", "c011", "c012"):
            base = self.root / "scratch/integrator" / directory
            for current, directories, filenames in os.walk(base):
                directories[:] = sorted(d for d in directories if not re.search(r"match|derive|collect|attempt|inputs|^src$|^include|snapshots", d, re.I))
                for filename in sorted(filenames):
                    if filename.endswith(".json") and HOLD_NAME_RE.search(filename):
                        label = (Path(current) / filename).relative_to(self.root).as_posix()
                        self.holds_from(label, self.inputs.load(label))
        # Branch-local evidence is read from the same immutable commit as its manifest.
        for selected in self.pending_sources:
            if selected["path"] not in ("config/candidates/c011.json", "config/candidates/c012.json"):
                continue
            directory = "scratch/integrator/" + Path(selected["path"]).stem
            for path in self.inputs.branch_paths(selected["commit"], directory):
                if path.endswith(".json") and HOLD_NAME_RE.search(Path(path).name):
                    label, doc = self.inputs.branch_json(selected["commit"], path)
                    self.holds_from(label, doc)

    def load_harvest(self) -> None:
        label = "scratch/integrator/HARVEST-LIST.json"
        doc = self.inputs.load(label)
        if doc is None:
            return
        for index, entry in enumerate(doc):
            if not isinstance(entry, str):
                continue
            match = re.fullmatch(r"([A-Za-z0-9_-]+):(func_[0-9a-fA-F]{8})\((.*)\)", entry)
            if not match:
                raise ValueError(f"unrecognized harvest entry: {label}#/{index}")
            batch, raw_name, detail = match.groups()
            name = identity(raw_name)
            if name not in self.functions:
                continue
            evidence = f"{label}#/{index}"
            state = detail.split(";", 1)[0].strip()
            conflict = bool(re.search(r"conflict|contract|reconcil|\bvs\b|blocked|conditional|held|ownership|punning|reinterpret", detail, re.I))
            self.functions[name].harvest.append({"batch": batch, "state": state if state in ("held", "pending", "conditional", "c013") else "other",
                                                "noted_contract_conflict": conflict, "evidence": evidence})
            self.functions[name].evidence.add(evidence)
            if conflict:
                self.add_hold(name, detail, evidence, "harvest_contract_conflict")

    def rows(self, accepted: set[str]) -> tuple[list[dict], list[dict]]:
        rows = []
        actionable = []
        stage_rank = {step: i for i, step in enumerate(STEPS)}
        for name, target in sorted(self.functions.items()):
            if name in accepted or name in self.pending:
                continue
            observations = sorted(target.observations, key=lambda v: (
                v["residual_bytes"], v["status"] != "match", v["origin"] != "author",
                -stage_rank.get(self.batch_steps.get(v["batch"], batch_step(v["batch"] or "")), -1),
                -int((re.search(r"\d+", v["batch"] or "") or ["0"])[0]), v["evidence"]))
            best = observations[0] if observations else None
            direct_matches = [v for v in observations if v["origin"] == "author" and
                              v["status"] == "match" and v["residual_bytes"] == 0 and
                              v.get("profile") in FROZEN_PROFILES]
            matched = bool(direct_matches)
            body_exact = bool(best is not None and best["residual_bytes"] == 0 and
                              best["status"] in ("match", "context_match"))
            current = best["residual_bytes"] if best else None
            permuter = sorted(target.permuter, key=lambda p: (
                {"match": 0, "exhausted": 1, "plateau": 2, "adapter_refused": 3,
                 "not_applicable_operational_boundary": 4}.get(p["state"], 5), p["evidence"]))
            state = permuter[0]["state"] if permuter else "not_recorded"
            bounded_complete = "4" in target.completed
            boundary = state == "adapter_refused"
            boundary_basis = "adapter_refusal" if boundary else None
            if not bounded_complete and not boundary and current is not None and current > self.permuter_bound:
                boundary, boundary_basis = True, "current_residual_above_bound"
                state = "operational_boundary"
            elif state == "not_applicable_operational_boundary":
                state = "reopened_inside_bound" if current is not None and current <= self.permuter_bound else "unknown_residual_boundary"
            missing = [step for step in AUTHOR_STEPS if step not in target.completed]
            if not bounded_complete and not boundary:
                missing.append("4")
            if body_exact:
                missing = []  # Exact bodies need integration/contract work, not more search.
            active_holds = [h for h in target.holds if h["basis"] != "author_integration_need" or
                            (best is not None and h["evidence"] == best["evidence"])]
            blocker_classes = sorted({v for hold in active_holds for v in hold["blocker_classes"]})
            awaiting = any(h["awaits_user_ruling"] for h in active_holds)
            if awaiting:
                status = "awaits_user_ruling"
            elif body_exact and active_holds:
                status = "held_contract"
            elif matched:
                status = "pending_intake"
            elif any(h["blocks_authoring"] and "canonical_header" in h["blocker_classes"] for h in active_holds):
                status = "header_blocked"
            elif missing or body_exact:
                status = "ladder_incomplete"
            elif current is not None and current <= 64:
                status = "near_miss_exhausted"
            else:
                status = "never_matched_exhausted"
            actions = list(missing)
            if status == "pending_intake":
                actions.append("checkpoint_intake")
            elif body_exact and status == "ladder_incomplete":
                actions.append("context_match_contract_review")
            row = {"function": name, "address": f"0x{name[5:]}", "size": target.size,
                   "best": best, "status_class": status, "blocker_classes": blocker_classes,
                   "diagnosis_families": sorted(target.families),
                   "diagnosis_basis": "INFERENCE: keyword families; see evidence, not an independent code diagnosis",
                   "ladder": {s: {"completed": s in target.completed,
                                  "attempted_batches": sorted(target.attempts[s]),
                                  "assigned_batches": sorted(target.assignments[s]),
                                  "evidence": sorted(target.step_evidence[s])} for s in STEPS},
                   "missing_steps": missing, "actionable_items": actions,
                   "permuter": {"state": state, "search_completed": bounded_complete,
                                "boundary_not_search": boundary, "boundary_basis": boundary_basis,
                                "bound_bytes": self.permuter_bound, "evidence": permuter},
                   "holds": sorted(active_holds, key=lambda h: (h["evidence"], h["basis"])),
                   "historical_integration_needs": sorted(
                       (h for h in target.holds if h not in active_holds),
                       key=lambda h: (h["evidence"], h["basis"])),
                   "harvest": sorted(target.harvest, key=lambda h: h["evidence"]),
                   "stale_extent_evidence": sorted(target.stale_evidence),
                   "evidence": sorted(target.evidence)}
            rows.append(row)
            if actions:
                actionable.append({key: row[key] for key in ("function", "address", "size", "status_class", "missing_steps", "actionable_items")})
        return rows, actionable


def default_pending() -> list[tuple[tuple[str, ...], str]]:
    return [(("c011-r8",), "config/candidates/c011.json"),
            (("c012-r4", "c012"), "config/candidates/c012.json"),
            (("types-compose-r3",), "config/candidates/ctypes1.json"),
            (("cmodels2-r5",), "config/candidates/cmodels2.json")]


def generate(root: Path, pending: list[tuple[tuple[str, ...], str]] | None = None) -> dict:
    join = Joiner(root.resolve())
    catalogue = join.inputs.load("config/catalogue.json", required=True)
    ledger = join.inputs.load("config/matches.json", required=True)
    join.inputs.load("config/compiler_profiles.json", required=True)
    join.inputs.load("tools/denominator.py")
    for row in catalogue["functions"]:
        if row.get("kind") != "function":
            continue
        name = identity(row)
        if not name or name in join.functions or not isinstance(row.get("size"), int):
            raise ValueError("catalogue has an invalid/duplicate creditable identity or size")
        join.functions[name] = Function(row["size"])
    accepted = {name for unit in ledger["units"] for value in unit.get("functions", [])
                if (name := identity(value)) in join.functions}
    for path in sorted((join.root / "docs/rulings").glob("*.md")):
        label = path.relative_to(join.root).as_posix()
        join.rulings[label] = join.inputs.load(label)
    join.select_pending(default_pending() if pending is None else pending)
    join.load_batches()
    join.load_census()
    join.load_authors()
    join.load_permuter()
    join.load_holds()
    join.load_harvest()
    rows, actionable = join.rows(accepted)
    pending_names = set(join.pending) - accepted
    summary = {state: {"functions": sum(r["status_class"] == state for r in rows),
                       "bytes": sum(r["size"] for r in rows if r["status_class"] == state)} for state in CLASSES}
    missing_counts = Counter(step for r in actionable for step in r["missing_steps"])
    action_counts = Counter(step for r in actionable for step in r["actionable_items"])
    hashes = dict(sorted(join.inputs.hashes.items()))
    def total(names: set[str]) -> dict:
        return {"functions": len(names), "bytes": sum(join.functions[n].size for n in names)}
    return {"schema_version": 1, "evidence_scope": "metadata join only; no new matches, classification, acceptance or impossibility proof",
            "policy": {"creditable_kind": "function", "author_steps_required": list(AUTHOR_STEPS),
                       "permuter_bound_bytes": join.permuter_bound,
                       "boundary_is_not_completed_search": True,
                       "assignment_is_not_attempt": True,
                       "context_result_does_not_complete_target_rung": True,
                       "missing_step5c_is_not_waived_above_64_bytes": True,
                       "pending_intake_requires": "direct author status match, zero residual, frozen profile, no recorded hold",
                       "incomplete_list_includes": "all missing search rungs plus pending intake/context-only contract review"},
            "denominator": total(set(join.functions)), "accepted": total(accepted),
            "pending_checkpoint": {**total(pending_names), "sources": join.pending_sources,
                "rows": [{"function": n, "size": join.functions[n].size,
                               "status": "pending checkpoint", "evidence": sorted(join.pending[n])} for n in sorted(pending_names)]},
            "residue": {"functions": len(rows), "bytes": sum(r["size"] for r in rows)},
            "summary_by_class": summary, "incomplete_ladder_count": len(actionable),
            "incomplete_by_missing_step": {s: missing_counts[s] for s in STEPS},
            "actionable_by_item": dict(sorted(action_counts.items())),
            "incomplete_ladder": actionable, "rows": rows,
            "user_rulings": sorted(join.rulings), "input_hashes": hashes,
            "input_manifest_sha256": digest(canonical(hashes)),
            "absent_optional_inputs": sorted(join.inputs.absent)}


def markdown(report: dict) -> str:
    lines = ["# Residue draft r1", "", "Metadata join only: not checkpoint acceptance, non-C classification, or proof that natural C is impossible.", "",
             "## Accounting", "", "| Bucket | Functions | Bytes |", "|---|---:|---:|"]
    for key, label in (("denominator", "Current creditable denominator"), ("accepted", "Accepted"),
                       ("pending_checkpoint", "Pending checkpoint (not residue)"), ("residue", "Residue")):
        value = report[key]
        lines.append(f"| {label} | {value['functions']:,} | {value['bytes']:,} |")
    lines += ["", "## Residue by class", "", "| Class | Functions | Bytes |", "|---|---:|---:|"]
    for state, value in report["summary_by_class"].items():
        lines.append(f"| {state} | {value['functions']:,} | {value['bytes']:,} |")
    lines += ["", "## Pending manifest snapshots", "", "| Branch | Commit | Manifest | Fallback from |", "|---|---|---|---|"]
    for row in report["pending_checkpoint"]["sources"]:
        lines.append(f"| {row['branch']} | `{row['commit']}` | `{row['path']}` | {', '.join(row['fallback_from']) or '—'} |")
    lines += ["", "## Interpretation", "",
              "- All six author rungs are required for unmatched exhaustion. An assignment without a delivered target RESULT does not complete a rung; context-only rows do not complete target rungs.",
              "- Step 4 separates actual completed search from adapter refusal and above-bound operational exclusion. A current improved seed inside the bound reopens an older above-bound exclusion. Unknown residuals never waive step 4.",
              "- Missing step 5c is reported even above the current 64-byte dispatch pool: there is no blanket waiver for those functions.",
              "- `held_contract` means an exact reported body plus a recorded contract/type/policy hold, not accepted C. `header_blocked` requires an explicit authoring/compile block, not an ordinary adoption-time header request; it may still have missing ladder rungs. Only the selected best author's integration needs remain active, while older needs stay in historical evidence.",
              "- `pending_intake` is a direct unheld author MATCH under a frozen profile, outside all pending manifests. It is actionable collection work, not an exhausted search or accepted credit.",
              "- Free-form source/author/hold text is not copied. Diagnosis/blocker families are [INFERENCE] keyword labels; the evidence pointers are the authority. Residuals are selected reported outcomes, not remeasured. Old-extent observations are excluded.",
              "- The actionable list includes every remaining required search rung, pending intake, and exact context-only contract review. A held exact body needs contract work rather than another author search.",
              "", f"## Actionable incomplete-ladder / intake list ({report['incomplete_ladder_count']:,} functions)", "",
              "Counts overlap when a function is missing multiple steps.", "", "| Missing step / action | Functions |", "|---|---:|"]
    for item, count in report["actionable_by_item"].items():
        lines.append(f"| {item} | {count:,} |")
    lines += ["", "| Function | Bytes | Class | Missing steps / action |", "|---|---:|---|---|"]
    for row in report["incomplete_ladder"]:
        lines.append(f"| `{row['function']}` | {row['size']} | {row['status_class']} | {', '.join(row['actionable_items'])} |")
    lines += ["", "## Per-function residue", "",
              "Complete evidence/assigned-batch/hold/hash detail is in the corresponding `RESIDUE-r1.json` row. Each evidence input is SHA-256-bound by that file's `input_hashes` map.", "",
              "| Function / address | Bytes | Best residual / batch | Attempted ladder steps and batches | Permuter state | Class / missing steps | Diagnosis families | Primary evidence |",
              "|---|---:|---|---|---|---|---|---|"]
    for index, row in enumerate(report["rows"]):
        best = row["best"]
        metric = f"{best['residual_bytes']} / {best['batch'] or 'recorded'}" if best else "unknown"
        steps = "; ".join(f"{s}: {','.join(v['attempted_batches']) or 'recorded'}" for s, v in row["ladder"].items() if v["completed"] or v["attempted_batches"])
        state = row["status_class"] + (" / " + ",".join(row["missing_steps"]) if row["missing_steps"] else "")
        evidence = best["evidence"] if best else (row["evidence"][0] if row["evidence"] else "none")
        lines.append(f"| `{row['function']}` / `{row['address']}` | {row['size']} | {metric} | {steps} | {row['permuter']['state']} | {state} | {', '.join(row['diagnosis_families']) or '—'} | `{evidence}`; [all evidence](RESIDUE-r1.json#/rows/{index}) |")
    lines += ["", "## Reproduction and input hashes", "", "```sh",
              ".venv/bin/python scratch/worktrees/residue-table/tools/residue.py --root .", "```", "",
              f"Input manifest SHA-256: `{report['input_manifest_sha256']}` ({len(report['input_hashes']):,} consumed inputs).",
              "All exact input hashes and absent optional branch/file probes are recorded in `RESIDUE-r1.json`. No timestamps or absolute checkout paths enter the report.", "",
              "| Input | SHA-256 |", "|---|---|"]
    for path in ("generator:tools/residue.py", "config/catalogue.json", "config/matches.json", "config/compiler_profiles.json"):
        lines.append(f"| `{path}` | `{report['input_hashes'][path]}` |")
    lines.append("")
    return "\n".join(lines)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--output", type=Path, help="output directory, restricted to ROOT/scratch/integrator/residue")
    args = parser.parse_args(argv)
    root = args.root.resolve()
    output = (args.output or root / "scratch/integrator/residue").resolve()
    if not output.is_relative_to(root / "scratch/integrator/residue"):
        parser.error("draft output must stay under scratch/integrator/residue/")
    try:
        report = generate(root)
    except (ValueError, OSError, KeyError, TypeError) as exc:
        parser.exit(1, f"residue metadata join failed: {exc}\n")
    output.mkdir(parents=True, exist_ok=True)
    (output / "RESIDUE-r1.json").write_bytes(canonical(report))
    (output / "RESIDUE-r1.md").write_text(markdown(report), encoding="utf-8", newline="\n")
    print(json.dumps({"summary_by_class": report["summary_by_class"],
                      "incomplete_ladder_count": report["incomplete_ladder_count"],
                      "incomplete_by_missing_step": report["incomplete_by_missing_step"],
                      "actionable_by_item": report["actionable_by_item"]}, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
