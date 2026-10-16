"""Container parsing and the backward-LZ77 codec (synthetic data only)."""
import hashlib
import random
import struct
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import ctrfmt  # noqa: E402

ORIG = Path(__file__).resolve().parents[1] / "orig"


def compressible(n: int, seed: int) -> bytes:
    rng = random.Random(seed)
    words = [rng.randrange(1 << 32) for _ in range(64)]
    out = bytearray()
    while len(out) < n:
        if rng.random() < 0.6:
            out += struct.pack("<I", rng.choice(words))
        elif rng.random() < 0.5 and len(out) > 64:
            start = rng.randrange(len(out) - 32)
            out += out[start : start + rng.randrange(3, 30)]
        else:
            out += bytes([rng.randrange(256)])
    return bytes(out[:n])


class BlzTest(unittest.TestCase):
    def test_roundtrip_synthetic(self):
        for seed in range(5):
            raw = compressible(20000 + seed * 977, seed)
            packed = ctrfmt.blz_compress(raw)
            self.assertLess(len(packed), len(raw))
            self.assertEqual(ctrfmt.blz_decompress(packed), raw)

    def test_incompressible_is_refused(self):
        rng = random.Random(9)
        with self.assertRaises(ctrfmt.BlzError):
            ctrfmt.blz_compress(bytes(rng.randrange(256) for _ in range(4096)))

    def test_bad_footer_is_rejected(self):
        packed = bytearray(ctrfmt.blz_compress(compressible(8000, 1)))
        packed[-5] = 0x40  # header length 0x40 > 11
        with self.assertRaises(ctrfmt.BlzError):
            ctrfmt.blz_decompress(bytes(packed))

    def test_truncated_stream_is_rejected(self):
        packed = ctrfmt.blz_compress(compressible(8000, 2))
        extra = struct.unpack_from("<I", packed, len(packed) - 4)[0]
        # claim more decompressed bytes than the stream produces
        tampered = packed[:-4] + struct.pack("<I", extra + 64)
        with self.assertRaises(ctrfmt.BlzError):
            ctrfmt.blz_decompress(tampered)

    @unittest.skipUnless((ORIG / "code.bin").is_file(), "private input not imported")
    def test_original_code_recompresses_exactly(self):
        code = (ORIG / "code.bin").read_bytes()
        stored = (ORIG / "exefs/code.blz").read_bytes()
        self.assertEqual(ctrfmt.blz_compress(code), stored)


class HeaderTest(unittest.TestCase):
    def test_exheader_layout(self):
        exh = bytearray(0x400)
        exh[0:8] = b"TESTAPP\0"
        exh[0x0D] = 1
        struct.pack_into("<III", exh, 0x10, 0x100000, 3, 0x2100)
        struct.pack_into("<I", exh, 0x1C, 0x4000)
        struct.pack_into("<III", exh, 0x20, 0x103000, 1, 0x10)
        struct.pack_into("<III", exh, 0x30, 0x104000, 2, 0x1004)
        struct.pack_into("<I", exh, 0x3C, 0x800)
        struct.pack_into("<Q", exh, 0x40, 0x0004013000002402)
        layout = ctrfmt.parse_exheader(bytes(exh))
        self.assertEqual(layout.app_title, "TESTAPP")
        self.assertTrue(layout.compressed)
        self.assertEqual(layout.image_size, (3 + 1 + 2) * 0x1000)
        self.assertEqual(layout.file_offset(0x100000), 0)
        self.assertEqual(layout.file_offset(0x103008), 0x3008)
        self.assertEqual(layout.file_offset(0x104ffc), 0x4ffc)
        self.assertEqual(layout.dependencies, ("0004013000002402",))
        with self.assertRaises(ValueError):
            layout.file_offset(0x106000)  # past the data segment's pages

    def test_exefs_hashes_are_reverse_ordered(self):
        header = bytearray(0x200)
        names = [(b".code", 0, 0x10), (b"icon", 0x200, 0x20)]
        for i, (name, off, size) in enumerate(names):
            struct.pack_into("<8sII", header, i * 16, name, off, size)
            header[0xC0 + (9 - i) * 32 : 0xC0 + (10 - i) * 32] = hashlib.sha256(name).digest()
        entries = ctrfmt.parse_exefs_header(bytes(header))
        self.assertEqual([e.name for e in entries], [".code", "icon"])
        self.assertEqual(entries[1].sha256, hashlib.sha256(b"icon").digest())

    def test_ncsd_partitions(self):
        header = bytearray(0x200)
        header[0x100:0x104] = b"NCSD"
        struct.pack_into("<II", header, 0x120, 0x20, 0x100)
        struct.pack_into("<II", header, 0x120 + 8 * 6, 0x200, 0x10)
        parts = ctrfmt.ncsd_partitions(bytes(header))
        self.assertEqual([(p.index, p.offset, p.size) for p in parts], [(0, 0x4000, 0x20000), (6, 0x40000, 0x2000)])


if __name__ == "__main__":
    unittest.main()
