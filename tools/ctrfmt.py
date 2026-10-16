"""Nintendo 3DS container formats: NCSD/NCCH headers, exheader code-set layout, ExeFS, backward LZ77.

Pure parsing and hashing. Decryption lives in tools/rom.py (pyctr + the user's boot9).
Offsets follow 3dbrew (NCSD, NCCH, NCCH/Extended_Header, ExeFS).
"""
from __future__ import annotations

import hashlib
import struct
from dataclasses import dataclass

MEDIA_UNIT = 0x200


@dataclass(frozen=True)
class Partition:
    index: int
    offset: int  # bytes from start of CCI
    size: int  # bytes


def ncsd_partitions(header: bytes) -> list[Partition]:
    """Used partitions from the 0x200-byte NCSD header."""
    if header[0x100:0x104] != b"NCSD":
        raise ValueError("not an NCSD image")
    parts = []
    for i in range(8):
        off, size = struct.unpack_from("<II", header, 0x120 + 8 * i)
        if size:
            parts.append(Partition(i, off * MEDIA_UNIT, size * MEDIA_UNIT))
    return parts


@dataclass(frozen=True)
class NcchHeader:
    raw: bytes
    program_id: str
    product_code: str
    version: int
    crypto_method: int
    no_crypto: bool
    fixed_key: bool
    seed_crypto: bool
    exheader_size: int
    exheader_hash: bytes
    plain: tuple[int, int]
    logo: tuple[int, int]
    exefs: tuple[int, int]
    exefs_superblock_hash: bytes
    romfs: tuple[int, int]
    romfs_hash_region_size: int
    romfs_superblock_hash: bytes


def parse_ncch(raw: bytes) -> NcchHeader:
    if raw[0x100:0x104] != b"NCCH":
        raise ValueError("not an NCCH header")
    flags = raw[0x188:0x190]

    def region(off: int) -> tuple[int, int]:
        o, s = struct.unpack_from("<II", raw, off)
        return o * MEDIA_UNIT, s * MEDIA_UNIT

    return NcchHeader(
        raw=raw[:0x200],
        program_id=raw[0x118:0x120][::-1].hex(),
        product_code=raw[0x150:0x160].rstrip(b"\0").decode("ascii"),
        version=struct.unpack_from("<H", raw, 0x112)[0],
        crypto_method=flags[3],
        no_crypto=bool(flags[7] & 0x04),
        fixed_key=bool(flags[7] & 0x01),
        seed_crypto=bool(flags[7] & 0x20),
        exheader_size=struct.unpack_from("<I", raw, 0x180)[0],
        exheader_hash=raw[0x160:0x180],
        plain=region(0x190),
        logo=region(0x198),
        exefs=region(0x1A0),
        exefs_superblock_hash=raw[0x1C0:0x1E0],
        romfs=region(0x1B0),
        romfs_hash_region_size=struct.unpack_from("<I", raw, 0x1B8)[0] * MEDIA_UNIT,
        romfs_superblock_hash=raw[0x1E0:0x200],
    )


@dataclass(frozen=True)
class CodeSegment:
    name: str
    address: int
    pages: int
    size: int

    @property
    def mapped_size(self) -> int:
        return self.pages * 0x1000


@dataclass(frozen=True)
class CodeLayout:
    app_title: str
    compressed: bool
    text: CodeSegment
    rodata: CodeSegment
    data: CodeSegment
    bss_size: int
    stack_size: int
    dependencies: tuple[str, ...]

    @property
    def segments(self) -> tuple[CodeSegment, CodeSegment, CodeSegment]:
        return self.text, self.rodata, self.data

    @property
    def image_size(self) -> int:
        """Size of the decompressed code.bin: every segment padded to whole pages."""
        return sum(s.mapped_size for s in self.segments)

    def file_offset(self, address: int) -> int:
        """Offset in code.bin of a virtual address inside text/rodata/data."""
        offset = 0
        for seg in self.segments:
            if seg.address <= address < seg.address + seg.mapped_size:
                return offset + address - seg.address
            offset += seg.mapped_size
        raise ValueError(f"address {address:#x} is outside code.bin")


def parse_exheader(exh: bytes) -> CodeLayout:
    """System Control Info (first 0x200 bytes) of the extended header."""

    def seg(name: str, off: int) -> CodeSegment:
        address, pages, size = struct.unpack_from("<III", exh, off)
        return CodeSegment(name, address, pages, size)

    deps = tuple(f"{d:016x}" for d in struct.unpack_from("<48Q", exh, 0x40) if d)
    return CodeLayout(
        app_title=exh[0:8].rstrip(b"\0").decode("ascii"),
        compressed=bool(exh[0x0D] & 0x01),
        text=seg("text", 0x10),
        rodata=seg("rodata", 0x20),
        data=seg("data", 0x30),
        bss_size=struct.unpack_from("<I", exh, 0x3C)[0],
        stack_size=struct.unpack_from("<I", exh, 0x1C)[0],
        dependencies=deps,
    )


@dataclass(frozen=True)
class ExefsEntry:
    name: str
    offset: int  # from end of the 0x200-byte ExeFS header
    size: int
    sha256: bytes


def parse_exefs_header(header: bytes) -> list[ExefsEntry]:
    """File table of a decrypted 0x200-byte ExeFS header; hash i is stored at slot 9 - i."""
    entries = []
    for i in range(10):
        name, off, size = struct.unpack_from("<8sII", header, i * 16)
        name = name.rstrip(b"\0").decode("ascii")
        if name:
            digest = header[0xC0 + (9 - i) * 32 : 0xC0 + (10 - i) * 32]
            entries.append(ExefsEntry(name, off, size, digest))
    return entries


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


class BlzError(ValueError):
    pass


def blz_decompress(packed: bytes, max_size: int = 64 << 20) -> bytes:
    """Decompress ExeFS `.code` (backward LZ77, decoded from the end of the buffer towards the start).

    Footer: u32 (top | bottom << 24), u32 extra size. `bottom` bytes of footer/padding sit at the
    end of the compressed stream; `top` is the compressed stream length counted from the end; bytes
    before `len - top` are stored verbatim.
    """
    if len(packed) < 8:
        raise BlzError("too short")
    top_bottom, extra = struct.unpack_from("<II", packed, len(packed) - 8)
    top, bottom = top_bottom & 0xFFFFFF, top_bottom >> 24
    if not (8 <= bottom <= 11 and bottom <= top <= len(packed)):
        raise BlzError(f"bad footer top={top:#x} bottom={bottom:#x}")
    if len(packed) + extra > max_size:
        raise BlzError(f"footer claims {len(packed) + extra:#x} output bytes, more than {max_size:#x}")
    out = bytearray(packed) + bytearray(extra)
    dest = len(out)
    src = len(packed) - bottom
    end = len(packed) - top
    while src > end:
        src -= 1
        flags = out[src]
        for bit in range(8):
            if not (flags << bit) & 0x80:
                if src <= end:
                    raise BlzError("literal past stream start")
                src -= 1
                dest -= 1
                out[dest] = out[src]
            else:
                if src - 2 < end:
                    raise BlzError("reference past stream start")
                hi, lo = out[src - 1], out[src - 2]
                src -= 2
                disp = (((hi & 0x0F) << 8) | lo) + 3
                length = (hi >> 4) + 3
                if dest - length < end or dest + disp > len(out):
                    raise BlzError("reference out of range")
                for _ in range(length):
                    dest -= 1
                    out[dest] = out[dest + disp]
            if src <= end:
                break
    if dest != end:
        raise BlzError(f"stream ended with output gap {dest - end:#x}")
    return bytes(out)


def blz_compress(raw: bytes) -> bytes:
    """Backward-LZ77 encoder with the greedy policy of 3dstool's CBackwardLz77::Compress (MIT,
    dnasdw/3dstool@3771ae96 src/backwardlz77.cpp).

    Policy: walk from the end; at each position take the longest match (3..18 bytes, distance
    3..4098, no overlap with the bytes being encoded); ties go to the smallest distance. Afterwards
    the longest prefix that the decoder could overrun is stored verbatim, the stream is padded to 4
    bytes with 0xFF and the 8-byte footer is appended. Measured: compress(decompress(original
    .code)) reproduces 漢検トレーニング2's stored .code byte for byte.
    """
    n = len(raw)
    window: dict[bytes, list[int]] = {}  # 3 bytes ending at p -> match sources p, in insertion order
    stream = bytearray()  # emitted bytes, highest address first
    src = n

    def slide(count: int) -> None:
        nonlocal src
        for _ in range(count):
            if src >= 3:
                window.setdefault(raw[src - 3 : src], []).append(src)
            src -= 1

    while src > 0:
        flag_at = len(stream)
        stream.append(0)
        for bit in range(8):
            limit = min(18, src, n - src)
            best = distance = 0
            if limit >= 3:
                for p in reversed(window.get(raw[src - 3 : src], ())):
                    d = p - src
                    if d > 4098:
                        break
                    if d < 3:
                        continue
                    cap = min(limit, d)
                    k = 3
                    while k < cap and raw[p - k - 1] == raw[src - k - 1]:
                        k += 1
                    if k > best:
                        best, distance = k, d
                        if best == limit:
                            break
            if best < 3:
                slide(1)
                stream.append(raw[src])
            else:
                stream[flag_at] |= 0x80 >> bit
                slide(best)
                stream.append(((best - 3) << 4) | ((distance - 3) >> 8))
                stream.append((distance - 3) & 0xFF)
            if src <= 0:
                break
    packed = bytes(reversed(stream))

    # Find where the decoder's output would overrun unread input; that prefix is stored raw.
    remaining, cursor, raw_prefix, packed_from = n, len(packed), 0, 0
    done = False
    while remaining > 0 and not done:
        cursor -= 1
        flags = packed[cursor]
        for bit in range(8):
            if not (flags << bit) & 0x80:
                cursor -= 1
                remaining -= 1
            else:
                cursor -= 1
                remaining -= (packed[cursor] >> 4) + 3
                cursor -= 1
                if remaining < cursor:
                    raw_prefix, packed_from, done = remaining, cursor, True
                    break
            if remaining <= 0:
                break
    body = packed[packed_from:]
    pad_at = raw_prefix + len(body)
    footer_at = (pad_at + 3) & ~3
    total = footer_at + 8
    if total >= n or total - raw_prefix > 0xFFFFFF:
        raise BlzError("data does not compress")
    top, bottom = total - raw_prefix, total - pad_at
    return raw[:raw_prefix] + body + b"\xff" * (footer_at - pad_at) + struct.pack("<II", top | bottom << 24, n - total)
