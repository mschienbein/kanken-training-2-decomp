"""Abstract original/candidate field stores; no instructions or original bytes."""
from __future__ import annotations

import importlib.util
import json
from pathlib import Path
import sys
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[4]
OUT = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location("config_store_flow", OUT / "reader_order_facts.py")
flow = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = flow
spec.loader.exec_module(flow)
from capstone import arm
import image


def effects(source, function):
    decoded = flow.flow.decoded(source, function)
    states = flow.flow.states(source, function, decoded)
    result = []
    for site, instruction in decoded.items():
        if instruction.id not in (arm.ARM_INS_STR, arm.ARM_INS_STRB):
            continue
        memory = next((op for op in instruction.operands if op.type == arm.ARM_OP_MEM), None)
        if memory is None:
            continue
        result.append({"site_offset": site - function["address"], "field_offset": memory.mem.disp,
                       "stored_bits": 8 if instruction.id == arm.ARM_INS_STRB else 32,
                       "source_register": instruction.reg_name(instruction.operands[0].reg)})
    return result


def main():
    original = image.Image.load()
    function = image.functions_by_address(image.load_catalogue(ROOT / "config/catalogue.json"))[0x001719F4]
    receipt = ROOT / "scratch/integrator/c010/config-match-r3/wave13/func_001719f4/func_001719f4.result.json"
    object_path = receipt.with_name(receipt.name.removesuffix(".result.json") + ".o")
    with object_path.open("rb") as stream:
        elf = ELFFile(stream)
        symbol = elf.get_section_by_name(".symtab").get_symbol_by_name("func_001719f4")[0]
        data = elf.get_section(symbol["st_shndx"]).data()
    candidate = flow.CandidateImage(function["address"], data)
    report = {"schema_version": 1, "function": "func_001719f4", "candidate_receipt": str(receipt.relative_to(ROOT)),
              "original_field_effects": effects(original, function),
              "candidate_field_effects": effects(candidate, function),
              "qualification": "Logical field-effect chronology only; source core-register identities expose conservative storage provenance, never an instruction listing. No register forcing or equivalent source steering is authorized by this diagnostic."}
    with (OUT / "config-order-facts-r1.json").open("x") as stream:
        json.dump(report, stream, indent=2)
        stream.write("\n")
    print(json.dumps(report))


if __name__ == "__main__":
    main()
