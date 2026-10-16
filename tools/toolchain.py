"""Locate and run the pinned ARM compilers (see toolchain.lock.json, installed by tools/setup.py).

Windows runs armcc.exe directly; macOS/Linux run it through wibo. Compiler ids are the archive's
directory labels (`armcc_41_1049` -> 4.1/b1049); profiles in config/compiler_profiles.json bind an
id to an ordered flag list.
"""
from __future__ import annotations

import hashlib
import json
import os
import platform
import subprocess
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
LOCK = json.loads((ROOT / "toolchain.lock.json").read_text(encoding="utf-8"))
INSTALL = ROOT / LOCK["install_directory"]
PROFILES = ROOT / "config/compiler_profiles.json"


class ToolchainError(RuntimeError):
    pass


def host_os() -> str:
    return {"Darwin": "darwin", "Linux": "linux", "Windows": "windows"}[platform.system()]


def host_arch() -> str:
    return "arm64" if platform.machine().lower() in ("arm64", "aarch64") else "x86_64"


def wibo_path() -> Path | None:
    """The wibo launcher on macOS/Linux; None on Windows."""
    return None if host_os() == "windows" else INSTALL / "bin/wibo"


def compiler_root(compiler_id: str) -> Path:
    try:
        return INSTALL / "armcc" / LOCK["armcc"]["builds"][compiler_id]
    except KeyError:
        raise ToolchainError(f"unknown compiler id {compiler_id!r}") from None


def armcc_exe(compiler_id: str) -> Path:
    return compiler_root(compiler_id) / "bin/armcc.exe"


def file_sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


@dataclass(frozen=True)
class Profile:
    name: str
    compiler: str
    flags: tuple[str, ...]


def load_profiles() -> dict[str, Profile]:
    if not PROFILES.is_file():
        return {}
    data = json.loads(PROFILES.read_text(encoding="utf-8"))
    return {name: Profile(name, p["compiler"], tuple(p["flags"])) for name, p in data["profiles"].items()}


def compile_source(profile: Profile, source: Path, output: Path, include_dirs: tuple[Path, ...] = ()) -> None:
    """Compile one translation unit to an ELF object; raise ToolchainError with the diagnostics on failure."""
    exe = armcc_exe(profile.compiler)
    if not exe.is_file():
        raise ToolchainError(f"{exe} missing; run tools/setup.py")
    launcher = wibo_path()
    argv = [str(launcher)] if launcher else []
    argv += [str(exe), "-c", *profile.flags]
    for inc in (*include_dirs, ROOT / "include", compiler_root(profile.compiler) / "include"):
        argv += ["-I", str(inc)]
    argv += ["-o", str(output), str(source)]
    output.parent.mkdir(parents=True, exist_ok=True)
    if output.exists():
        output.unlink()
    proc = subprocess.run(argv, capture_output=True, text=True, encoding="utf-8", errors="replace", timeout=300, env=os.environ | {"LC_ALL": "C"})
    if proc.returncode != 0 or not output.is_file():
        raise ToolchainError(f"armcc failed ({proc.returncode}) on {source.name}:\n{proc.stdout}{proc.stderr}")
