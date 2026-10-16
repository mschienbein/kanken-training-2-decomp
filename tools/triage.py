#!/usr/bin/env python3
"""Rank unmatched functions without changing the catalogue or accepted ledger.

    python tools/triage.py queue --limit 20 [--max-size 128] [--isa arm]
    python tools/triage.py stratified --per-class 2 --seed 1
    python tools/triage.py batches --size 10 --count 3

All commands accept --exclude FILE (a JSON list of integer or 0x-prefixed string
addresses) and --out DIR (default scratch/triage). They write queue.json,
stratified.json or batches.json, respectively, with input SHA-256 identities.
Only catalogue code_ranges are decoded, with pools subtracted even if a code
range includes them. Data refs count pool words pointing into actual rodata,
data or BSS, not segment padding. Callees/callers count distinct addresses;
loops count local backward branch sites, including self-branches, not calls.

Difficulty is a heuristic, not a match or compiler-profile claim: instruction
count + ceil(bytes/16), plus 8 per uncredited direct callee, 2 per credited
callee, 6 per data ref, 4 per VFP instruction, 20 for a switch, 12 per loop,
30 per indirect call, 20 per SVC, 40 per catalogue anomaly, 4 per undecoded
byte and 8 for Thumb. Value is the catalogue extent in bytes.

Queue order is (unmatched call depth, difficulty, address): callees precede
callers, then easier functions come first. Strongly connected components share
a depth, so recursion cannot make depth undefined. In-flight exclusions remain
in this graph: excluding a dependency must not turn its caller into a leaf.
Feature classes overlap; stratified samples are independent per class, and the
output functions list is deduplicated. Leaf-small means <=32 bytes and
leaf-medium means 33..128 bytes, with neither direct nor indirect calls.
Batches grow from the easiest remaining function through caller/callee
neighbours in queue order; they are disjoint and refuse insufficient capacity.
No timestamps, original bytes or disassembly are emitted.
"""
from __future__ import annotations

import argparse
from bisect import bisect_left
import hashlib
import heapq
import json
from pathlib import Path
import random
import re
import struct
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
import image as image_mod  # noqa: E402
from capstone import CS_ARCH_ARM, CS_MODE_ARM, CS_MODE_THUMB, Cs  # noqa: E402
from capstone.arm import (  # noqa: E402
    ARM_CC_AL, ARM_CC_LO, ARM_CC_LS, ARM_INS_ADD, ARM_INS_B, ARM_INS_BL,
    ARM_INS_BLX, ARM_INS_CBNZ, ARM_INS_CBZ, ARM_INS_LDR, ARM_INS_SVC,
    ARM_OP_IMM, ARM_OP_MEM, ARM_OP_REG, ARM_REG_PC, ARM_SFT_LSL,
)

ROOT = Path(__file__).resolve().parents[1]
AUTO = re.compile(r"^func_([0-9a-fA-F]{8})$")
RESERVED = re.compile(r"^(func|data|bss)_[0-9a-fA-F]{8}$")
FEATURE_CLASSES = (
    "leaf-small", "leaf-medium", "calls", "pool-data", "switch", "loop",
    "vfp", "indirect-call", "thumb", "svc",
)


def credited_addresses(functions: dict[int, dict], matches: dict, names: dict[str, dict]) -> set[int]:
    """Resolve the ledger exactly as ci/progress_report.py does, failing closed."""
    reserved = sorted(name for name in names if RESERVED.fullmatch(name))
    if reserved:
        raise image_mod.ImageError(f"config/names.json may not define reserved address names: {', '.join(reserved[:5])}")
    credited = set()
    for unit in matches["units"]:
        for name in unit["functions"]:
            automatic = AUTO.fullmatch(name)
            address = int(automatic[1], 16) if automatic else names.get(name, {}).get("address")
            if address not in functions:
                raise image_mod.ImageError(f"matches.json credits {name}, which is not a catalogued function")
            if not image_mod.creditable(functions[address]):
                raise image_mod.ImageError(f"matches.json credits {name}, a {functions[address]['kind']!r} that can never earn C credit")
            credited.add(address)
    return credited


def code_ranges(function: dict):
    """Yield code islands minus pools; never decode unclassified extent holes."""
    base = function["address"]
    pools = sorted(function.get("pools", []))
    for start, end in sorted(function.get("code_ranges", [[base, base + function["size"]]])):
        for low, high in pools:
            if high <= start:
                continue
            if low >= end:
                break
            if start < low:
                yield start, low
            start = max(start, high)
            if start >= end:
                break
        if start < end:
            yield start, end


def is_switch(instruction) -> bool:
    """Recognize ARM PC-indexed branch/address tables (LS, LO or unguarded)."""
    if instruction.id not in (ARM_INS_ADD, ARM_INS_LDR) or instruction.cc not in (ARM_CC_LS, ARM_CC_LO, ARM_CC_AL):
        return False
    operands = instruction.operands
    if not operands or operands[0].type != ARM_OP_REG or operands[0].reg != ARM_REG_PC:
        return False
    if instruction.id == ARM_INS_ADD:
        return (len(operands) == 3 and operands[1].type == ARM_OP_REG and operands[1].reg == ARM_REG_PC
                and operands[2].type == ARM_OP_REG and operands[2].shift.type == ARM_SFT_LSL
                and operands[2].shift.value == 2)
    return (len(operands) == 2 and operands[1].type == ARM_OP_MEM and operands[1].mem.base == ARM_REG_PC
            and operands[1].mem.index != 0 and operands[1].mem.disp == 0 and not operands[1].subtracted
            and operands[1].shift.type == ARM_SFT_LSL and operands[1].shift.value == 2)


def call_depths(graph: dict[int, set[int]]) -> dict[int, int]:
    """Longest path to a sink in the SCC graph, using iterative Kosaraju + Kahn."""
    outgoing = {address: sorted(target for target in targets if target in graph) for address, targets in sorted(graph.items())}
    incoming = {address: [] for address in outgoing}
    for address, targets in outgoing.items():
        for target in targets:
            incoming[target].append(address)
    visited, order = set(), []
    for address in outgoing:
        stack = [(address, False)]
        while stack:
            node, finished = stack.pop()
            if finished:
                order.append(node)
            elif node not in visited:
                visited.add(node)
                stack.append((node, True))
                stack.extend((target, False) for target in reversed(outgoing[node]) if target not in visited)
    component = {}
    for node in reversed(order):
        if node in component:
            continue
        group = len(component)
        component[node] = group
        stack = [node]
        while stack:
            for caller in incoming[stack.pop()]:
                if caller not in component:
                    component[caller] = group
                    stack.append(caller)
    children = {group: set() for group in component.values()}
    parents = {group: set() for group in children}
    for address, targets in outgoing.items():
        for target in targets:
            source_group, target_group = component[address], component[target]
            if source_group != target_group:
                children[source_group].add(target_group)
                parents[target_group].add(source_group)
    remaining = {group: len(targets) for group, targets in children.items()}
    depths = dict.fromkeys(children, 0)
    ready = [group for group, count in remaining.items() if count == 0]
    heapq.heapify(ready)
    while ready:
        group = heapq.heappop(ready)
        for parent in sorted(parents[group]):
            depths[parent] = max(depths[parent], depths[group] + 1)
            remaining[parent] -= 1
            if remaining[parent] == 0:
                heapq.heappush(ready, parent)
    return {address: depths[component[address]] for address in outgoing}


def difficulty(features: dict) -> int:
    return (features["instruction_count"] + (features["size"] + 15) // 16
            + 8 * (features["direct_callees"] - features["credited_callees"])
            + 2 * features["credited_callees"] + 6 * features["data_refs"]
            + 4 * features["vfp_instructions"] + 20 * int(features["switch_table"])
            + 12 * features["loops"] + 30 * features["indirect_calls"] + 20 * features["svc"]
            + 40 * len(features["anomalies"]) + 4 * features["undecoded_bytes"]
            + 8 * (features["isa"] == "thumb"))


def feature_classes(function: dict) -> list[str]:
    features = function["features"]
    leaf = features["direct_callees"] == 0 and features["indirect_calls"] == 0
    flags = (
        leaf and function["size"] <= 32,
        leaf and 32 < function["size"] <= 128,
        features["direct_callees"] > 0,
        features["data_refs"] > 0,
        features["switch_table"], features["loops"] > 0,
        features["vfp_instructions"] > 0, features["indirect_calls"] > 0,
        function["isa"] == "thumb", features["svc"] > 0,
    )
    return [name for name, enabled in zip(FEATURE_CLASSES, flags) if enabled]


class Triage:
    def __init__(self, image: image_mod.Image, catalogue: dict, matches: dict,
                 names: dict[str, dict] | None = None, excluded: set[int] | None = None):
        if catalogue["image_sha256"] != image.sha256:
            raise image_mod.ImageError("catalogue image_sha256 does not match orig/code.bin")
        functions = image_mod.functions_by_address(catalogue)
        names = names or {}
        self.credited = credited_addresses(functions, matches, names)
        self.excluded = set(excluded or ())
        display = {}
        for name, entry in sorted(names.items()):
            display.setdefault(entry["address"], name)  # Stable even if an address has aliases.
        decoders = {"arm": Cs(CS_ARCH_ARM, CS_MODE_ARM), "thumb": Cs(CS_ARCH_ARM, CS_MODE_THUMB)}
        for decoder in decoders.values():
            decoder.detail = True
            decoder.skipdata = True
        anomalies = sorted(catalogue.get("anomalies", []), key=lambda item: (item["address"], json.dumps(item, sort_keys=True)))
        anomaly_addresses = [item["address"] for item in anomalies]
        segments = [(segment.address, segment.address + segment.size) for segment in (image.layout.rodata, image.layout.data)]
        segments.append(image.bss)
        self.callees = {}
        self.callers = {address: set() for address in functions}
        self.records = {}
        for address, function in functions.items():
            if function.get("kind") == "veneer":
                continue
            end = address + function["size"]
            features = {
                "size": function["size"], "isa": function["isa"], "kind": function.get("kind", "function"),
                "instruction_count": 0, "direct_callees": 0, "credited_callees": 0, "callers": 0,
                "data_refs": 0, "vfp_instructions": 0, "switch_table": False, "loops": 0,
                "indirect_calls": 0, "svc": 0, "undecoded_bytes": 0,
                "anomalies": anomalies[bisect_left(anomaly_addresses, address):bisect_left(anomaly_addresses, end)],
            }
            targets = set()
            for start, stop in code_ranges(function):
                decoded_bytes = 0
                for instruction in decoders[function["isa"]].disasm(image.read(start, stop - start), start):
                    decoded_bytes += instruction.size
                    if instruction.id == 0:  # Capstone skip-data pseudo-instruction.
                        features["undecoded_bytes"] += instruction.size
                        continue
                    features["instruction_count"] += 1
                    features["vfp_instructions"] += instruction.mnemonic.startswith("v")
                    if instruction.id in (ARM_INS_BL, ARM_INS_BLX):
                        operand = instruction.operands[0]
                        if operand.type == ARM_OP_IMM:
                            targets.add(operand.imm & ~1)
                        else:
                            features["indirect_calls"] += 1
                    elif instruction.id in (ARM_INS_B, ARM_INS_CBZ, ARM_INS_CBNZ):
                        operand = instruction.operands[-1]
                        if operand.type == ARM_OP_IMM and address <= operand.imm <= instruction.address:
                            features["loops"] += 1
                    elif instruction.id == ARM_INS_SVC:
                        features["svc"] += 1
                    if instruction.id in (ARM_INS_ADD, ARM_INS_LDR) and function["isa"] == "arm":
                        features["switch_table"] |= is_switch(instruction)
                features["undecoded_bytes"] += stop - start - decoded_bytes
            for low, high in function.get("pools", []):
                for (word,) in struct.iter_unpack("<I", image.read(low, high - low)):
                    features["data_refs"] += any(start <= word < stop for start, stop in segments)
            features["direct_callees"] = len(targets)
            features["credited_callees"] = len(targets & self.credited)
            self.callees[address] = targets
            for target in targets:
                if target in self.callers:
                    self.callers[target].add(address)
            self.records[address] = {
                "address": address, "name": display.get(address, f"func_{address:08x}"),
                "size": function["size"], "isa": function["isa"], "features": features,
                "difficulty": difficulty(features), "value": function["size"],
            }
        unmatched = {address for address, function in functions.items() if image_mod.creditable(function) and address not in self.credited}
        depths = call_depths({address: self.callees[address] & unmatched for address in sorted(unmatched)})
        for address, record in self.records.items():
            record["features"]["callers"] = len(self.callers[address])
            record["depth"] = depths.get(address, 0)
        self.functions = sorted((self.records[address] for address in unmatched - self.excluded),
                                key=lambda function: (function["depth"], function["difficulty"], function["address"]))

    def queue(self, max_size: int | None = None, isa: str | None = None) -> list[dict]:
        return [function for function in self.functions
                if (max_size is None or function["size"] <= max_size) and (isa is None or function["isa"] == isa)]

    def classes(self) -> dict[str, list[dict]]:
        groups = {name: [] for name in FEATURE_CLASSES}
        for function in self.functions:
            for name in feature_classes(function):
                groups[name].append(function)
        return groups

    def stratified(self, per_class: int, seed: int) -> dict[str, list[dict]]:
        if per_class < 1:
            raise ValueError("per-class must be positive")
        rng = random.Random(seed)
        return {name: sorted(rng.sample(functions, min(per_class, len(functions))), key=lambda function: function["address"])
                for name, functions in self.classes().items()}

    def batches(self, size: int, count: int) -> list[list[dict]]:
        if size < 1 or count < 1:
            raise ValueError("batch size and count must be positive")
        if size * count > len(self.functions):
            raise ValueError(f"requested {count} batches of {size}, but only {len(self.functions)} functions are eligible")
        ranks = {function["address"]: rank for rank, function in enumerate(self.functions)}
        available = set(ranks)
        batches, cursor = [], 0
        for _ in range(count):
            batch, frontier, pending = [], [], set()
            while len(batch) < size:
                if not frontier:
                    while self.functions[cursor]["address"] not in available:
                        cursor += 1
                    heapq.heappush(frontier, (cursor, self.functions[cursor]["address"]))
                _, address = heapq.heappop(frontier)
                if address not in available:
                    continue
                available.remove(address)
                batch.append(self.records[address])
                for neighbour in self.callees[address] | self.callers[address]:
                    if neighbour in available and neighbour not in pending:
                        pending.add(neighbour)
                        heapq.heappush(frontier, (ranks[neighbour], neighbour))
            batches.append(batch)
        return batches


def load_exclusions(path: Path | None) -> set[int]:
    if path is None:
        return set()
    values = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(values, list):
        raise ValueError("exclude file must contain a JSON list of addresses")
    result = set()
    for value in values:
        if isinstance(value, str) and value.lower().startswith("0x"):
            value = int(value, 16)
        if type(value) is not int or not 0 <= value <= 0xFFFFFFFF:
            raise ValueError("excluded addresses must be 32-bit integers or 0x-prefixed strings")
        result.add(value & ~1)
    return result


def load_triage(exclude_path: Path | None = None, root: Path = ROOT) -> tuple[Triage, dict]:
    image = image_mod.Image.load(root / "orig")
    catalogue_path = root / "config/catalogue.json"
    catalogue = image_mod.load_catalogue(catalogue_path)
    matches_bytes = (root / "config/matches.json").read_bytes()
    matches = json.loads(matches_bytes)
    names = image_mod.load_names(root / "config/names.json")
    analysis = Triage(image, catalogue, matches, names, load_exclusions(exclude_path))
    metadata = {
        "schema_version": 1, "image_sha256": image.sha256,
        "catalogue_sha256": hashlib.sha256(catalogue_path.read_bytes()).hexdigest(),
        "generated_from_matches_sha256": hashlib.sha256(matches_bytes).hexdigest(),
    }
    return analysis, metadata


def print_table(functions: list[dict]) -> None:
    print(f"{'Address':10} {'Size':>6} {'ISA':5} {'Score':>6} {'Depth':>5} {'Calls':>5} {'Data':>4}  Name")
    for function in functions:
        features = function["features"]
        print(f"0x{function['address']:08x} {function['size']:6} {function['isa']:5} {function['difficulty']:6} "
              f"{function['depth']:5} {features['direct_callees']:5} {features['data_refs']:4}  {function['name']}")


def positive_int(value: str) -> int:
    number = int(value, 0)
    if number <= 0:
        raise argparse.ArgumentTypeError("must be positive")
    return number


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--exclude", type=Path, help="JSON list of in-flight addresses")
    parser.add_argument("--out", type=Path, default=ROOT / "scratch/triage")
    commands = parser.add_subparsers(dest="command", required=True)
    queue = commands.add_parser("queue", help="easiest-first, dependency-ordered author queue")
    queue.add_argument("--limit", type=positive_int)
    queue.add_argument("--max-size", type=positive_int)
    queue.add_argument("--isa", choices=("arm", "thumb"))
    stratified = commands.add_parser("stratified", help="seeded pilots from overlapping feature classes")
    stratified.add_argument("--per-class", type=positive_int, default=1)
    stratified.add_argument("--seed", type=int, default=0)
    batches = commands.add_parser("batches", help="disjoint, neighbour-aware author assignments")
    batches.add_argument("--size", type=positive_int, default=10)
    batches.add_argument("--count", type=positive_int, default=1)
    for command in (queue, stratified, batches):
        command.add_argument("--exclude", type=Path, default=argparse.SUPPRESS, help="JSON list of in-flight addresses")
        command.add_argument("--out", type=Path, default=argparse.SUPPRESS)
    args = parser.parse_args(argv)
    try:
        analysis, metadata = load_triage(args.exclude)
        document = {**metadata, "excluded_addresses": sorted(analysis.excluded)}
        if args.command == "queue":
            functions = analysis.queue(args.max_size, args.isa)
            document["functions"] = functions[:args.limit]
            print_table(document["functions"])
        elif args.command == "stratified":
            selected = analysis.stratified(args.per_class, args.seed)
            addresses = {function["address"] for functions in selected.values() for function in functions}
            document.update({"seed": args.seed, "per_class": args.per_class,
                             "class_counts": {name: len(functions) for name, functions in analysis.classes().items()},
                             "classes": {name: [function["address"] for function in functions] for name, functions in selected.items()},
                             "functions": [function for function in analysis.functions if function["address"] in addresses]})
            for name, functions in selected.items():
                print(f"\n{name}: {len(functions)} selected / {document['class_counts'][name]} available")
                print_table(functions)
        else:
            groups = analysis.batches(args.size, args.count)
            document.update({"size": args.size, "count": args.count,
                             "batches": [{"id": number, "functions": functions} for number, functions in enumerate(groups, 1)]})
            for number, functions in enumerate(groups, 1):
                print(f"\nBatch {number}")
                print_table(functions)
        args.out.mkdir(parents=True, exist_ok=True)
        output = args.out / f"{args.command}.json"
        output.write_text(json.dumps(document, indent=2, sort_keys=True) + "\n", encoding="utf-8", newline="\n")
        print(f"\nWrote {output}")
    except (OSError, ValueError, image_mod.ImageError) as error:
        parser.exit(1, f"triage: {error}\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
