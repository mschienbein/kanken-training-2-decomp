"""Count possible armlink-inlined, eliminated I2 callees; never emit image bytes or disassembly.

    .venv/bin/python research/linker/removed_callee_census.py [--authors scratch/authors]

This is a conservative shape census, not proof that any particular call existed. Require an
I2-eligible, call-ABI instruction, at least one input in r0-r3 materialised in the preceding four
straight-line words, and the linker's as-if-call test after the site. Exclude instructions any
eligible original callee can produce under I3. Author overlap is deliberately literal: mismatch
rows in */RESULT.json whose text contains the case-insensitive substring 'inlin'.
"""
from __future__ import annotations

import argparse
import json
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
import image as image_mod  # noqa: E402
import inplace  # noqa: E402
from capstone import CS_ARCH_ARM, CS_MODE_ARM, Cs  # noqa: E402
from capstone import arm  # noqa: E402

ARGUMENTS = {arm.ARM_REG_R0, arm.ARM_REG_R1, arm.ARM_REG_R2, arm.ARM_REG_R3}
SCRATCH = ARGUMENTS | {arm.ARM_REG_R12, arm.ARM_REG_CPSR}


def mismatch_notes(authors: Path) -> tuple[set[int], int, int]:
    addresses: set[int] = set()
    files = hits = 0

    def visit(value: object) -> None:
        nonlocal hits
        if isinstance(value, dict):
            if (value.get("status") == "mismatch" and "address" in value
                    and "inlin" in json.dumps(value).lower()):
                address = value["address"]
                addresses.add(int(address, 0) if isinstance(address, str) else address)
                hits += 1
            for child in value.values():
                visit(child)
        elif isinstance(value, list):
            for child in value:
                visit(child)

    for path in sorted(authors.glob("*/RESULT.json")):
        files += 1
        visit(json.loads(path.read_text(encoding="utf-8")))
    return addresses, files, hits


def census(catalogue_path: Path, authors: Path) -> dict:
    image = image_mod.Image.load()
    functions = image_mod.functions_by_address(image_mod.load_catalogue(catalogue_path))
    linker = inplace.InPlaceLinker(image, functions, inplace.Resolver(functions), image.bss)
    original_replacements = set()
    for address in functions:
        body = linker.eligible_original_callee(address)
        if body is not None:
            for condition in range(15):
                decision = inplace.inline_arm_call((condition << 28) | 0x0B000000, body)
                if decision is not None:
                    original_replacements.add(decision[1])
    md = Cs(CS_ARCH_ARM, CS_MODE_ARM)
    md.detail = True
    by_function = {}
    for fn in functions.values():
        if fn["isa"] != "arm":
            continue
        spans = (*fn.get("pools", ()), *fn.get("literals", ()))
        instructions = {}
        for lo, hi in fn["code_ranges"]:
            for address in range(lo, hi, 4):
                if any(a <= address < b for a, b in spans):
                    continue
                insn = next(md.disasm(image.read(address, 4), address, 1), None)
                if insn is not None:
                    instructions[address] = insn
        words = inplace.arm_pool_words(image, fn)
        sites = []
        for address, insn in instructions.items():
            word = struct.unpack("<I", insn.bytes)[0]
            if word in original_replacements or not inplace.inlinable_arm_instruction(word):
                continue
            read, written = map(set, insn.regs_access())
            arguments = read & ARGUMENTS
            if not arguments or read - ARGUMENTS - {arm.ARM_REG_CPSR} or written - SCRATCH:
                continue
            if inplace.arm_call_abi_context(instructions, address, arguments, insn.cc, words=words) == "consistent":
                sites.append(address)
        if sites:
            by_function[fn["address"]] = sites
    noted, result_files, note_rows = mismatch_notes(authors)
    overlap = sorted(noted & by_function.keys())
    return {
        "candidate_sites": sum(map(len, by_function.values())),
        "candidate_functions": len(by_function),
        "author_result_files": result_files,
        "author_inlin_mismatch_rows": note_rows,
        "author_inlin_mismatch_functions": len(noted),
        "overlap_sites": sum(len(by_function[address]) for address in overlap),
        "overlap_functions": len(overlap),
        "overlap_function_addresses": [f"0x{address:08x}" for address in overlap],
        "sites_by_function": {f"0x{address:08x}": [f"0x{site:08x}" for site in sites]
                              for address, sites in sorted(by_function.items())},
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--catalogue", type=Path, default=image_mod.CATALOGUE)
    parser.add_argument("--authors", type=Path, default=ROOT / "scratch/authors")
    args = parser.parse_args()
    print(json.dumps(census(args.catalogue, args.authors), indent=1))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
