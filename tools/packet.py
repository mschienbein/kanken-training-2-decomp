"""Author packet for one function: symbolized disassembly, pool words, callers/callees, data refs.

    python tools/packet.py func_00103798 [--catalogue config/catalogue.json] [--out scratch/packets]

Writes scratch/packets/<name>.md (private: it contains disassembly of the original) plus a C
skeleton scratch/packets/<name>.c with extern declarations for every referenced symbol, ready for
`tools/match.py <name>.c --profile <profile>`. Names come from config/names.json, otherwise the
func_/data_/bss_XXXXXXXX convention the in-place linker resolves.
"""
from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import display  # noqa: E402
import image as image_mod  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]


class Namer:
    def __init__(self, image: image_mod.Image, functions: dict[int, dict], names: dict[str, dict]):
        self.image = image
        self.functions = functions
        self.by_address = {e["address"]: n for n, e in names.items()}
        # Catalogue v3+ `entries` (secondary entry points of multi-entry routines) are call
        # targets named func_XXXXXXXX, as tools/inplace.Resolver resolves them.
        self.entries = {e["address"] for f in functions.values() for e in f.get("entries", ())}

    def __repr__(self) -> str:
        return f"Namer(functions={len(self.functions)}, names={len(self.by_address)})"

    def kind(self, address: int) -> str | None:
        lay = self.image.layout
        if address in self.functions or address in self.entries:
            return "func"
        for seg, prefix in ((lay.text, "text"), (lay.rodata, "data"), (lay.data, "data")):
            if seg.address <= address < seg.address + seg.size:
                return prefix
        lo, hi = self.image.bss
        return "bss" if lo <= address < hi else None

    def name(self, address: int) -> str | None:
        if address in self.by_address:
            return self.by_address[address]
        kind = self.kind(address)
        if kind in ("func", "data", "bss"):
            return f"{kind}_{address:08x}"
        return None

    def code_target(self, word: int) -> int | None:
        """The function or entry a literal word points at, clearing bit 0 (the Thumb bit) only
        when that lands on one. Any other word, odd data addresses included, is not code."""
        for address in ((word & ~1, word) if word & 1 else (word,)):
            if self.kind(address) == "func":
                return address
        return None

    def literal(self, word: int) -> str | None:
        """Name a pointer-sized literal: a code address by its function, anything else exactly."""
        target = self.code_target(word)
        return self.name(word if target is None else target)


def branch_target(insn) -> int | None:
    m = insn.mnemonic
    if m.startswith("b") and not m.startswith(("bic", "bkpt")) and insn.op_str.startswith("#"):
        return int(insn.op_str[1:], 16)
    return None


def disassemble(image, fn: dict):
    base, size, isa = fn["address"], fn["size"], fn["isa"]
    data = image.read(base, size)
    pools = [tuple(p) for p in (*fn.get("pools", []), *fn.get("literals", []))]  # data inside the section
    code_ranges = [tuple(p) for p in fn["code_ranges"]]
    rows = []
    for kind, addr, chunk, insn in display.spans(data, base, isa, code_ranges, pools):
        if kind == "insn":
            rows.append(("insn", addr, insn, None))
        elif kind == "data" and len(chunk) == 4:
            rows.append(("word", addr, struct.unpack("<I", chunk)[0], None))
        else:
            rows.append(("raw", addr, chunk.hex(), None))
    return rows


def pc_literal(insn, isa: str) -> int | None:
    """Address loaded by `ldr rX, [pc, #imm]` / `vldr`, or produced by `adr`.

    The displacement is parsed with its sign (`#-0x4` is -4) and added once.
    """
    op = insn.op_str
    if "[pc" in op and insn.mnemonic.startswith(("ldr", "vldr")):
        imm = int(op.split("#")[1].rstrip("]!").strip(), 0) if "#" in op else 0
        pc = insn.address + (8 if isa == "arm" else 4)
        if isa == "thumb":
            pc &= ~3
        return pc + imm
    if insn.mnemonic == "adr" and "#" in op:
        return int(op.split("#")[1], 0)
    return None


def build_packet(target: str, catalogue_path: Path) -> tuple[str, str, Path]:
    image = image_mod.Image.load()
    catalogue = image_mod.load_catalogue(catalogue_path)
    functions = image_mod.functions_by_address(catalogue)
    names = image_mod.load_names()
    namer = Namer(image, functions, names)
    address = names[target]["address"] if target in names else int(target.split("_")[-1], 16)
    fn = functions.get(address)
    if fn is None:
        raise SystemExit(f"{target}: no catalogued function at {address:#x}")
    fname = namer.name(address)

    callees, data_refs, lines = set(), set(), []
    for kind, addr, item, _ in disassemble(image, fn):
        if kind == "word":
            ref = namer.literal(item) if item is not None else None
            if ref and ref != fname:
                (callees if ref.startswith("func_") or namer.code_target(item) is not None else data_refs).add(ref)
            lines.append(f"{addr:08x}:  .word 0x{item:08x}" + (f"    ; {ref}" if ref else ""))
        elif kind == "raw":
            lines.append(f"{addr:08x}:  .inst {item}")
        else:
            text = f"{item.mnemonic} {item.op_str}".strip()
            note = ""
            tgt = branch_target(item)
            if tgt is not None:
                if (base := functions.get(tgt)) or (tgt in namer.entries and not fn["address"] <= tgt < fn["address"] + fn["size"]):
                    ref = namer.name(base["address"] if base else tgt)
                    if tgt != address:
                        callees.add(ref)
                    note = ref
                elif fn["address"] <= tgt < fn["address"] + fn["size"]:
                    note = f".L_{tgt:08x}"
            lit = pc_literal(item, fn["isa"])
            if lit is not None and fn["address"] <= lit < fn["address"] + fn["size"]:
                word = struct.unpack("<I", image.read(lit, 4))[0]
                ref = namer.literal(word)
                note = f"={ref}" if ref else f"=0x{word:08x}"
            lines.append(f"{item.address:08x}:  {text:<40}" + (f"; {note}" if note else ""))

    callers = sorted(
        namer.name(f["address"]) for f in functions.values()
        if f["address"] != address and _calls(image, f, address)
    )
    md = [
        f"# {fname} @ {address:#x}", "",
        f"- size {fn['size']:#x} (code {fn.get('code_size', fn['size']):#x}), isa {fn['isa']}, kind {fn.get('kind', 'function')}",
        f"- evidence: {', '.join(fn.get('evidence', [])) or 'n/a'}",
        f"- pools: {', '.join(f'{lo:#x}-{hi:#x}' for lo, hi in fn.get('pools', [])) or 'none'}",
        f"- callers ({len(callers)}): {', '.join(callers[:30])}{' …' if len(callers) > 30 else ''}",
        f"- callees: {', '.join(sorted(callees)) or 'none'}",
        f"- data: {', '.join(sorted(data_refs)) or 'none'}", "",
        "```", *lines, "```", "",
        f"Match: `.venv/bin/python tools/match.py scratch/packets/{fname}.c --profile <profile>`",
    ]
    c = ["/* Candidate for " + fname + "; types and names are placeholders. */", ""]
    c += [f"extern void {n}(void);" for n in sorted(callees)]
    c += [f"extern char {n}[];" for n in sorted(data_refs)]
    c += ["", f"void {fname}(void) {{", "}", ""]
    return "\n".join(md) + "\n", "\n".join(c), Path(fname)


def _calls(image, fn: dict, target: int) -> bool:
    """Raw scan of ARM B/BL/BLX(imm) and Thumb BL/BLX pairs, skipping literal pools."""
    if fn.get("kind") == "veneer":
        return False
    data = image.read(fn["address"], fn["size"])
    pools = [(lo - fn["address"], hi - fn["address"]) for lo, hi in (*fn.get("pools", []), *fn.get("literals", []))]
    if fn["isa"] == "arm":
        for pos in range(0, len(data) - 3, 4):
            if any(lo <= pos < hi for lo, hi in pools):
                continue
            w = struct.unpack_from("<I", data, pos)[0]
            if (w >> 25) & 7 == 5:  # B/BL (cond) or BLX(imm) when cond == 0xF
                off = ((w & 0xFFFFFF) ^ 0x800000) - 0x800000
                dest = fn["address"] + pos + 8 + (off << 2) + ((((w >> 24) & 1) << 1) if w >> 28 == 0xF else 0)
                if dest == target:
                    return True
        return False
    for pos in range(0, len(data) - 3, 2):
        hi, lo = struct.unpack_from("<HH", data, pos)
        if hi >> 11 == 0x1E and lo >> 11 in (0x1F, 0x1D):
            off = ((((hi & 0x7FF) << 11) | (lo & 0x7FF)) ^ 0x200000) - 0x200000
            pc = fn["address"] + pos + 4
            dest = ((pc & ~3) if lo >> 11 == 0x1D else pc) + (off << 1)
            if dest == target:
                return True
    return False


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("function", help="func_XXXXXXXX or a names-overlay name")
    parser.add_argument("--catalogue", type=Path, default=image_mod.CATALOGUE)
    parser.add_argument("--out", type=Path, default=ROOT / "scratch/packets")
    args = parser.parse_args(argv)
    md, c, stem = build_packet(args.function, args.catalogue)
    args.out.mkdir(parents=True, exist_ok=True)
    (args.out / f"{stem}.md").write_text(md, encoding="utf-8", newline="\n")
    skeleton = args.out / f"{stem}.c"
    if not skeleton.exists():
        skeleton.write_text(c, encoding="utf-8", newline="\n")
    print(args.out / f"{stem}.md")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
