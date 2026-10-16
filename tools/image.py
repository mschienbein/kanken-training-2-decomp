"""The original executable (orig/code.bin) addressed by virtual address, plus the function catalogue."""
from __future__ import annotations

import hashlib
import json
from pathlib import Path

import ctrfmt

ROOT = Path(__file__).resolve().parents[1]
CATALOGUE = ROOT / "config/catalogue.json"
NAMES = ROOT / "config/names.json"


class ImageError(RuntimeError):
    pass


class Image:
    def __init__(self, code: bytes, layout: ctrfmt.CodeLayout):
        if len(code) != layout.image_size:
            raise ImageError(f"code.bin is {len(code):#x} bytes, layout expects {layout.image_size:#x}")
        self.code = code
        self.layout = layout
        self.sha256 = hashlib.sha256(code).hexdigest()

    @classmethod
    def load(cls, orig: Path = ROOT / "orig") -> Image:
        code = (orig / "code.bin").read_bytes()
        layout = ctrfmt.parse_exheader((orig / "exheader.bin").read_bytes())
        image = cls(code, layout)
        pinned = json.loads((ROOT / "config/title.json").read_text(encoding="utf-8"))["derived"]["code.bin"]["sha256"]
        if image.sha256 != pinned:
            raise ImageError(f"orig/code.bin sha256 {image.sha256} != pinned {pinned}; run tools/rom.py import")
        return image

    def read(self, address: int, size: int) -> bytes:
        start = self.layout.file_offset(address)
        if size and self.layout.file_offset(address + size - 1) != start + size - 1:
            raise ImageError(f"[{address:#x}, +{size:#x}) crosses a segment boundary")
        return self.code[start : start + size]

    @property
    def bss(self) -> tuple[int, int]:
        data = self.layout.data
        start = data.address + data.size
        return start, start + self.layout.bss_size


def load_catalogue(path: Path = CATALOGUE) -> dict:
    if not path.is_file():
        raise ImageError(f"{path} missing (the function catalogue is produced by tools/analyze.py)")
    return json.loads(path.read_text(encoding="utf-8"))


def functions_by_address(catalogue: dict) -> dict[int, dict]:
    return {f["address"]: f for f in catalogue["functions"]}


def load_names(path: Path = NAMES) -> dict[str, dict]:
    if not path.is_file():
        return {}
    return json.loads(path.read_text(encoding="utf-8"))["symbols"]
