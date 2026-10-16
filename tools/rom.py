"""Import and verify the private input: CCI -> orig/{exheader.bin, exefs/*, code.bin[, romfs/]}.

    python tools/rom.py import --rom <cartridge.3ds> [--boot9 <boot9.bin>] [--romfs]
    python tools/rom.py verify

Accepts only the CCI images pinned in config/title.json. An encrypted dump needs a boot9.bin dumped
from the user's own console (pyctr derives the NCCH keys from it); the decrypted dump needs nothing.
Every extracted byte is checked against the NCCH hash chain (exheader hash, ExeFS superblock hash,
per-file ExeFS hashes, RomFS superblock hash) and against the pinned derived hashes. Key material is
never logged.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import contextlib
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ctrfmt  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
TITLE = ROOT / "config/title.json"
ORIG = ROOT / "orig"
MANIFEST = ORIG / "manifest.json"
EXEFS_NAMES = {".code": "exefs/code.blz", "banner": "exefs/banner.bin", "icon": "exefs/icon.bin", "logo": "exefs/logo.bin"}


class InputError(RuntimeError):
    pass


def file_sha256(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        while chunk := f.read(1 << 22):
            h.update(chunk)
    return h.hexdigest()


def load_title() -> dict:
    return json.loads(TITLE.read_text(encoding="utf-8"))


def identify(rom: Path, title: dict) -> dict:
    digest = file_sha256(rom)
    for entry in title["accepted_inputs"]:
        if entry["sha256"] == digest:
            return entry
    raise InputError(f"{rom} (sha256 {digest}) is not an accepted input for {title['product_code']}")


def _write(dest: Path, data: bytes, manifest: dict) -> None:
    dest.parent.mkdir(parents=True, exist_ok=True)
    dest.write_bytes(data)
    manifest["files"][dest.relative_to(ORIG).as_posix()] = {"size": len(data), "sha256": ctrfmt.sha256(data)}


def import_rom(rom: Path, boot9: Path | None, with_romfs: bool) -> dict:
    title = load_title()
    entry = identify(rom, title)
    if entry["encrypted"] and boot9 is None:
        raise InputError("encrypted dump: pass --boot9 with a boot9.bin dumped from your own console")
    from pyctr.crypto.engine import CryptoEngine
    from pyctr.fileio import SubsectionIO
    from pyctr.type.exefs import decompress_code
    from pyctr.type.ncch import NCCHReader, NCCHSection
    from pyctr.type.romfs import RomFSReader

    with rom.open("rb") as f:
        partitions = ctrfmt.ncsd_partitions(f.read(0x200))
        app = next(p for p in partitions if p.index == 0)
        f.seek(app.offset)
        ncch = ctrfmt.parse_ncch(f.read(0x200))
    if ncch.product_code != title["product_code"] or ncch.program_id != title["program_id"]:
        raise InputError(f"unexpected title {ncch.product_code} {ncch.program_id}")

    manifest = {"schema_version": 1, "input": {"id": entry["id"], "sha256": entry["sha256"]}, "files": {}}
    with rom.open("rb") as raw, contextlib.ExitStack() as stack:
        if entry["encrypted"]:
            reader = stack.enter_context(
                NCCHReader(SubsectionIO(raw, app.offset, app.size), crypto=CryptoEngine(boot9=str(boot9)))
            )

            def section(name: str):
                return reader.open_raw_section(getattr(NCCHSection, name))
        else:
            # The decrypted dump keeps the original crypto flags; read its plaintext regions directly.
            # (pyctr 0.7.6 ignores assume_decrypted for split-keyslot ExeFS and would re-encrypt it.)
            regions = {"ExtendedHeader": (0x200, 0x800), "ExeFS": ncch.exefs, "RomFS": ncch.romfs}

            def section(name: str):
                off, size = regions[name]
                return SubsectionIO(raw, app.offset + off, size)

        with section("ExtendedHeader") as s:
            exheader = s.read(0x800)
        if hashlib.sha256(exheader[:0x400]).digest() != ncch.exheader_hash:
            raise InputError("exheader does not match the NCCH header hash (wrong key or corrupt dump)")
        with section("ExeFS") as s:
            exefs = s.read()
        if hashlib.sha256(exefs[:0x200]).digest() != ncch.exefs_superblock_hash:
            raise InputError("ExeFS header does not match the NCCH superblock hash")
        exefs_files = {}
        for item in ctrfmt.parse_exefs_header(exefs[:0x200]):
            data = exefs[0x200 + item.offset : 0x200 + item.offset + item.size]
            if hashlib.sha256(data).digest() != item.sha256:
                raise InputError(f"ExeFS {item.name} does not match its header hash")
            exefs_files[item.name] = data
        romfs_files: list[tuple[str, bytes]] = []
        if with_romfs:
            romfs_io = stack.enter_context(section("RomFS"))
            if hashlib.sha256(romfs_io.read(ncch.romfs_hash_region_size)).digest() != ncch.romfs_superblock_hash:
                raise InputError("RomFS master hash region does not match the NCCH superblock hash")
            romfs_io.seek(0)
            romfs_files = list(_walk_romfs(stack.enter_context(RomFSReader(romfs_io))))

    layout = ctrfmt.parse_exheader(exheader)
    packed = exefs_files[".code"]
    code = ctrfmt.blz_decompress(packed) if layout.compressed else packed
    if layout.compressed and decompress_code(packed) != code:
        raise InputError("our backward-LZ77 decoder disagrees with pyctr")
    if len(code) != layout.image_size:
        raise InputError(f"code.bin is {len(code):#x} bytes, exheader layout says {layout.image_size:#x}")

    _write(ORIG / "exheader.bin", exheader, manifest)
    for name, data in exefs_files.items():
        _write(ORIG / EXEFS_NAMES.get(name, f"exefs/{name.lstrip('.')}.bin"), data, manifest)
    _write(ORIG / "code.bin", code, manifest)
    for path, data in romfs_files:
        _write(ORIG / "romfs" / path, data, manifest)
    manifest["layout"] = {
        "app_title": layout.app_title,
        "compressed": layout.compressed,
        "segments": [
            {"name": s.name, "address": s.address, "size": s.size, "mapped_size": s.mapped_size} for s in layout.segments
        ],
        "bss_size": layout.bss_size,
    }
    check_pins(manifest, title)
    MANIFEST.write_text(json.dumps(manifest, indent=1, ensure_ascii=False) + "\n", encoding="utf-8", newline="\n")
    return manifest


def _walk_romfs(romfs, path: str = "/"):
    info = romfs.get_info_from_path(path)
    for name in info.contents:
        child = path.rstrip("/") + "/" + name
        sub = romfs.get_info_from_path(child)
        if hasattr(sub, "contents"):
            yield from _walk_romfs(romfs, child)
        else:
            with romfs.open(child) as f:
                yield child.lstrip("/"), f.read()


def check_pins(manifest: dict, title: dict) -> None:
    for rel, pin in title["derived"].items():
        got = manifest["files"].get(rel)
        if got is None:
            raise InputError(f"{rel} missing from extraction")
        if got != pin:
            raise InputError(f"{rel}: extracted {got} != pinned {pin}")


def verify() -> dict:
    """Re-hash every extracted file against orig/manifest.json and the pins in config/title.json."""
    if not MANIFEST.is_file():
        raise InputError("orig/manifest.json missing; run `tools/rom.py import` first")
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    for rel, expected in manifest["files"].items():
        path = ORIG / rel
        if not path.is_file():
            raise InputError(f"orig/{rel} missing")
        got = {"size": path.stat().st_size, "sha256": file_sha256(path)}
        if got != expected:
            raise InputError(f"orig/{rel} changed: {got} != {expected}")
    check_pins(manifest, load_title())
    return manifest


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="cmd", required=True)
    imp = sub.add_parser("import")
    imp.add_argument("--rom", type=Path, required=True)
    imp.add_argument("--boot9", type=Path)
    imp.add_argument("--romfs", action="store_true", help="also extract RomFS files to orig/romfs/")
    sub.add_parser("verify")
    args = parser.parse_args(argv)
    try:
        manifest = import_rom(args.rom, args.boot9, args.romfs) if args.cmd == "import" else verify()
    except InputError as e:
        print(f"error: {e}", file=sys.stderr)
        return 1
    files = manifest["files"]
    print(f"ok: {len(files)} files, code.bin {files['code.bin']['sha256']}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
