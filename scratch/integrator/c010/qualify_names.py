"""Qualify only actual emitted definitions or unreplaced caller relocations.

Reads bound author objects and receipt metadata, never original image bytes.
A mechanically qualified binding still requires canonical typed-contract review.
No names overlay or shared configuration is changed by this analysis.
"""
from __future__ import annotations

import hashlib
import json
from pathlib import Path
import sys

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

ROOT = Path(__file__).resolve().parents[3]
MAIN = ROOT.parents[2]
OUT = Path(__file__).resolve().parent


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def original_path(value: str) -> Path:
    path = Path(value)
    return path if path.is_absolute() else MAIN / path


def main() -> int:
    output = OUT / sys.argv[1]
    existing = json.loads((ROOT / "config/names.json").read_text())["symbols"]
    scope = json.loads((OUT / "SCOPE.json").read_text())
    proposed = []
    issues = []
    seen = set()
    for evidence in scope["all_evidence"]:
        receipt = original_path(evidence["receipt"])
        if str(receipt) in seen:
            continue
        seen.add(str(receipt))
        record = json.loads(receipt.read_text())
        source = original_path(evidence["source"])
        if digest(source) != record.get("source_sha256"):
            issues.append({"receipt": str(receipt), "reason": "source changed after author receipt"})
            continue
        object_path = receipt.with_name(receipt.name.removesuffix(".result.json") + ".o")
        if not object_path.exists() or digest(object_path) != record.get("object_sha256"):
            issues.append({"receipt": str(receipt), "reason": "bound author object unavailable or hash changed"})
            continue
        overlay = {}
        binding = record.get("proposed_names")
        if binding:
            overlay_path = original_path(binding["path"])
            if overlay_path.exists() and digest(overlay_path) == binding["sha256"]:
                overlay = json.loads(overlay_path.read_text()).get("symbols", {})
            else:
                issues.append({"receipt": str(receipt), "reason": "proposed names file missing or hash changed"})
        with object_path.open("rb") as stream:
            elf = ELFFile(stream)
            symtab = elf.get_section_by_name(".symtab")
            symbols = {symbol.name: symbol for symbol in symtab.iter_symbols() if symbol.name}
            matched_sections = {}
            for row in record["functions"]:
                symbol = symbols.get(row["name"])
                if row["status"] == "match" and symbol is not None and isinstance(symbol["st_shndx"], int):
                    matched_sections[symbol["st_shndx"]] = row
            for index, row in matched_sections.items():
                for name, symbol in symbols.items():
                    if name in existing or symbol["st_shndx"] != index or symbol["st_info"]["type"] != "STT_FUNC" or symbol["st_value"] != 0:
                        continue
                    address = row["address"]
                    entry = overlay.get(name)
                    if entry and entry["address"] != address:
                        issues.append({"receipt": str(receipt), "name": name, "reason": "same-section definition contradicts overlay"})
                        continue
                    proposed.append({"name": name, "address": address, "proof": "same-address-emitted-self-definition",
                                     "witness_function": f'func_{row["address"]:08x}', "source": str(source),
                                     "receipt": str(receipt), "receipt_sha256": digest(receipt), "object_sha256": record["object_sha256"],
                                     "qualification": "[INFERENCE] Descriptive/native spelling; canonical owner and full typed contracts still require review."})
            for section in elf.iter_sections():
                if not isinstance(section, RelocationSection) or section["sh_info"] not in matched_sections:
                    continue
                row = matched_sections[section["sh_info"]]
                rewritten = {r["offset"] for r in row.get("linker_rewrites", [])}
                for relocation in section.iter_relocations():
                    name = symtab.get_symbol(relocation["r_info_sym"]).name
                    if name in existing or name not in overlay or relocation["r_offset"] in rewritten or relocation["r_info_type"] in {0, 40}:
                        continue
                    symbol = symbols[name]
                    proof = "natural-caller-preserved-relocation" if symbol["st_shndx"] == "SHN_UNDEF" else "same-address-emitted-data-self-definition"
                    proposed.append({"name": name, "address": overlay[name]["address"], "proof": proof,
                                     "witness_function": f'func_{row["address"]:08x}', "offset": relocation["r_offset"],
                                     "relocation_type": relocation["r_info_type"], "source": str(source),
                                     "receipt": str(receipt), "receipt_sha256": digest(receipt), "object_sha256": record["object_sha256"],
                                     "qualification": "[INFERENCE] Descriptive/native spelling; preserved relocation proves target identity, not canonical owner or argument/result types."})
    addresses = {}
    for row in proposed:
        addresses.setdefault(row["name"], set()).add(row["address"])
    conflicts = {name: sorted(values) for name, values in addresses.items() if len(values) != 1}
    result = {"schema_version": 1, "status": "qualified-proposals-only-not-adopted",
              "receipt_objects_examined": len(seen), "qualified_names": len(addresses),
              "conflicts": conflicts, "evidence": proposed, "issues": issues}
    with output.open("x") as stream:
        json.dump(result, stream, indent=2)
        stream.write("\n")
    print(json.dumps({key: result[key] for key in ("receipt_objects_examined", "qualified_names", "conflicts")} | {"issues": len(issues)}))
    return int(bool(conflicts))


if __name__ == "__main__":
    raise SystemExit(main())
