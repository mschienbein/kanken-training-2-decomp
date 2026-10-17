"""Catalogue-bounded disassembly for display only, never byte matching or linking."""
from __future__ import annotations

from capstone import CS_ARCH_ARM, CS_MODE_ARM, CS_MODE_THUMB, Cs

_CS = {"arm": Cs(CS_ARCH_ARM, CS_MODE_ARM), "thumb": Cs(CS_ARCH_ARM, CS_MODE_THUMB)}


def spans(data: bytes, base: int, isa: str, code_ranges: list[tuple[int, int]],
          data_ranges: list[tuple[int, int]]):
    """Yield (kind, address, bytes, instruction) without crossing catalogue boundaries.

    Only complete, aligned instructions inside code ranges may be decoded. All
    remaining spans retain their exact bytes, including partial instruction words.
    Explicit pool/literal boundaries also split raw spans from adjacent padding.
    """
    end = base + len(data)
    boundaries = sorted({base, end} | {
        address for lo, hi in (*code_ranges, *data_ranges) for address in (lo, hi)
        if base < address < end
    })
    step = 4 if isa == "arm" else 2
    for lo, hi in zip(boundaries, boundaries[1:]):
        code = any(start <= lo < stop for start, stop in code_ranges)
        code = code and not any(start <= lo < stop for start, stop in data_ranges)
        addr = lo
        while addr < hi:
            chunk = data[addr - base : min(addr + 4, hi) - base]
            if code and addr % step == 0 and hi - addr >= step:
                insn = next(_CS[isa].disasm(chunk, addr, 1), None)
                if insn is not None:
                    yield "insn", addr, chunk[:insn.size], insn
                    addr += insn.size
                    continue
                yield "raw", addr, chunk[:step], None
                addr += step
            else:
                # Stop at alignment as well as range boundaries so code can resume.
                size = min(len(chunk), step - addr % step) if code else len(chunk)
                yield "data", addr, chunk[:size], None
                addr += size
