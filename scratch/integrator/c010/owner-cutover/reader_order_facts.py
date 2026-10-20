"""Compare abstract early field effects, never machine code or disassembly."""
from __future__ import annotations

import importlib.util
import json
from pathlib import Path
import sys
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[4]
OUT = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location("reader_archive_flow", OUT / "archive_tag_facts.py")
flow = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = flow
spec.loader.exec_module(flow)
from capstone import arm
import image


class CandidateImage:
    def __init__(self, address, data):
        self.address, self.data = address, data

    def read(self, address, size):
        offset = address - self.address
        if not 0 <= offset <= offset + size <= len(self.data):
            raise ValueError("candidate read outside bound section")
        return self.data[offset:offset + size]


def effects(source, function):
    decoded = flow.decoded(source, function)
    states = flow.states(source, function, decoded)
    result = []
    for site, instruction in decoded.items():
        if site - function["address"] >= 28 or instruction.id not in (arm.ARM_INS_STR, arm.ARM_INS_STRB):
            continue
        memory = next((op for op in instruction.operands if op.type == arm.ARM_OP_MEM), None)
        if memory is None:
            continue
        value = flow.value(instruction.operands[0], states.get(site, {}))
        result.append({"site_offset": site - function["address"], "field_offset": memory.mem.disp,
                       "stored_bits": 8 if instruction.id == arm.ARM_INS_STRB else 32,
                       "value_is_entry_mode": value == ("entry", arm.ARM_REG_R2),
                       "value_is_zero": value == ("constant", 0)})
    return result


def main():
    original = image.Image.load()
    function = image.functions_by_address(image.load_catalogue(ROOT / "config/catalogue.json"))[0x001D3C78]
    receipt = ROOT / "scratch/integrator/c010/reader-mode-match-r2/retries/func_001d3c78/func_001d3c78.result.json"
    object_path = receipt.with_name(receipt.name.removesuffix(".result.json") + ".o")
    with object_path.open("rb") as stream:
        elf = ELFFile(stream)
        symbol = elf.get_section_by_name(".symtab").get_symbol_by_name("func_001d3c78")[0]
        data = elf.get_section(symbol["st_shndx"]).data()
    candidate = CandidateImage(function["address"], data)
    report = {"schema_version": 1, "function": "func_001d3c78", "candidate_receipt": str(receipt.relative_to(ROOT)),
              "original_early_field_effects": effects(original, function),
              "candidate_early_field_effects": effects(candidate, function),
              "qualification": "Only logical early storage effects are compared. No original/candidate instructions or encodings are serialized; this does not claim an entire-function semantic or register allocation audit."}
    with (OUT / "reader-order-facts-r1.json").open("x") as stream:
        json.dump(report, stream, indent=2)
        stream.write("\n")
    print(json.dumps(report))


if __name__ == "__main__":
    main()
