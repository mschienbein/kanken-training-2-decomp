"""Abstract reader mode width facts; never emits original code or literals."""
from __future__ import annotations

import importlib.util
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[4]
OUT = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location("archive_word_provenance", OUT / "archive_tag_facts.py")
flow = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = flow
spec.loader.exec_module(flow)
from capstone import arm
import image


def main() -> int:
    original = image.Image.load()
    functions = image.functions_by_address(image.load_catalogue(ROOT / "config/catalogue.json"))
    address = 0x001D3C78
    function = functions[address]
    instructions = flow.decoded(original, function)
    states = flow.states(original, function, instructions)
    incoming = ("entry", arm.ARM_REG_R2)
    stores, comparisons = [], []
    for site, instruction in instructions.items():
        state = states.get(site)
        if state is None:
            continue
        ops = instruction.operands
        if instruction.id == arm.ARM_INS_STRB and flow.value(ops[0], state) == incoming:
            memory = next((op for op in ops if op.type == arm.ARM_OP_MEM), None)
            stores.append({"site_offset": site - address, "stored_bits": 8,
                           "field_offset": memory.mem.disp if memory else None})
        if instruction.id == arm.ARM_INS_CMP and len(ops) == 2:
            values = [flow.value(op, state) for op in ops]
            if incoming in values:
                other = values[1] if values[0] == incoming else values[0]
                comparisons.append({"site_offset": site - address,
                                    "compares_unchanged_entry_word": True,
                                    "other_is_zero": other == ("constant", 0)})
    report = {"schema_version": 1, "function": "func_001d3c78", "image_sha256": original.sha256,
              "entry_mode_byte_stores": stores, "entry_mode_word_comparisons": comparisons,
              "decision": "Keep byte storage, widen the formal to int, and test the untruncated formal separately from its byte assignment.",
              "qualification": "[INFERENCE] Exact historical mode spelling is unrecovered. This callee-only scoped CFG evidence proves storage width separately from the preserved word test, not a general caller or native identity audit. The reported signed-byte-to-int caller is preserved as ground truth."}
    flow.OUT = OUT
    with (OUT / "reader-mode-facts-r1.json").open("x", encoding="utf-8") as stream:
        json.dump(report, stream, indent=2)
        stream.write("\n")
    print(json.dumps({"mode_byte_stores": len(stores), "unchanged_word_compares": len(comparisons)}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
