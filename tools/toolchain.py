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
import shutil
import subprocess
import threading
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


def load_profiles(path: Path = PROFILES) -> dict[str, Profile]:
    if not path.is_file():
        return {}
    data = json.loads(path.read_text(encoding="utf-8"))
    return {name: Profile(name, p["compiler"], tuple(p["flags"])) for name, p in data["profiles"].items()}


CACHE = ROOT / ".cache/objcache"


def _cache_key(profile: Profile, source: Path, includes: list[Path]) -> str:
    """Everything that decides the object except the headers, which are checked per entry."""
    h = hashlib.sha256()
    for part in (
        "objcache-v1", profile.compiler, file_sha256(armcc_exe(profile.compiler)), "\0".join(profile.flags),
        "\0".join(str(i) for i in includes), str(source), file_sha256(source),
    ):
        h.update(part.encode("utf-8") + b"\0")
    return h.hexdigest()


def _deps_unchanged(deps: dict[str, str]) -> bool:
    for path, digest in deps.items():
        p = Path(path)
        if not p.is_file() or file_sha256(p) != digest:
            return False
    return True


def _host_dependency(dep: str) -> Path:
    """A --depend entry as a host path. wibo shows armcc the host root as drive Z:, so a header the
    compiler finds through its own install (bin/../include/stddef.h) is reported as Z:/...; on
    Windows every path is already native. Windows names are case-insensitive, so armcc may report
    a header in another case than the host file (algorithm.CC for algorithm.cc): such a path is
    resolved to the existing file, component by component."""
    if wibo_path() is not None and len(dep) > 2 and dep[0] in "Zz" and dep[1] == ":" and dep[2] in "/\\":
        path = Path("/" + dep[3:].replace("\\", "/"))
    else:
        path = Path(dep)
    if wibo_path() is None or path.exists():
        return path
    return _case_insensitive(path) or path


def _case_insensitive(path: Path) -> Path | None:
    current = Path(path.anchor) if path.is_absolute() else Path(".")
    for part in path.parts[1:] if path.is_absolute() else path.parts:
        if part in (".", ".."):
            current = current / part
            continue
        candidate = current / part
        if candidate.exists():
            current = candidate
            continue
        try:
            matches = [p for p in current.iterdir() if p.name.lower() == part.lower()]
        except OSError:
            return None
        if len(matches) != 1:
            return None
        current = matches[0]
    return current


def _parse_depend(text: str, source: Path) -> list[Path]:
    deps = []
    for line in text.splitlines():
        _, _, dep = line.partition(": ")
        dep = dep.strip().replace("\\ ", " ")
        if not dep:
            continue
        path = _host_dependency(dep)
        if path.resolve() != source.resolve():
            deps.append(path)
    return deps


def compile_source(
    profile: Profile, source: Path, output: Path, include_dirs: tuple[Path, ...] = (), cache: bool = True
) -> bool:
    """Compile one translation unit to an ELF object; raise ToolchainError with the diagnostics on failure.

    With `cache`, an object compiled earlier from the same compiler executable, flags, include
    path, source path and bytes, and the same bytes of every header it read (armcc --depend), is
    copied instead of recompiling. The code is identical either way; only the object's embedded
    command line can differ. Returns True on a cache hit.
    """
    exe = armcc_exe(profile.compiler)
    if not exe.is_file():
        raise ToolchainError(f"{exe} missing; run tools/setup.py")
    source = source.resolve()
    includes = [Path(p).resolve() for p in (*include_dirs, ROOT / "include", compiler_root(profile.compiler) / "include")]
    output.parent.mkdir(parents=True, exist_ok=True)
    if output.exists():
        output.unlink()
    key = _cache_key(profile, source, includes)
    entry_path = CACHE / "entries" / f"{key}.json"
    if cache and entry_path.is_file():
        entry = json.loads(entry_path.read_text(encoding="utf-8"))
        blob = CACHE / "objects" / f"{entry['object_sha256']}.o"
        if blob.is_file() and file_sha256(blob) == entry["object_sha256"] and _deps_unchanged(entry["deps"]):
            shutil.copyfile(blob, output)
            return True
    launcher = wibo_path()
    depfile = output.with_suffix(".d")
    argv = [str(launcher)] if launcher else []
    argv += [str(exe), "-c", *profile.flags]
    for inc in includes:
        argv += ["-I", str(inc)]
    argv += [f"--depend={depfile}", "--depend_format=unix_escaped", "-o", str(output), str(source)]
    proc = subprocess.run(argv, capture_output=True, text=True, encoding="utf-8", errors="replace", timeout=300, env=os.environ | {"LC_ALL": "C"})
    if proc.returncode != 0 or not output.is_file():
        raise ToolchainError(f"armcc failed ({proc.returncode}) on {source.name}:\n{proc.stdout}{proc.stderr}")
    # Fresh compiles refresh the cache too; only the lookup above is optional.
    if not depfile.is_file():
        return False
    deps = _parse_depend(depfile.read_text(encoding="utf-8", errors="replace"), source)
    digest = file_sha256(output)
    unique = f"{os.getpid()}-{threading.get_ident()}"
    blob = CACHE / "objects" / f"{digest}.o"
    blob.parent.mkdir(parents=True, exist_ok=True)
    if not blob.is_file():
        tmp = blob.with_suffix(f".{unique}.tmp")
        shutil.copyfile(output, tmp)
        os.replace(tmp, blob)
    entry_path.parent.mkdir(parents=True, exist_ok=True)
    record = {"object_sha256": digest, "deps": {str(d.resolve()): file_sha256(d) for d in deps}}
    tmp = entry_path.with_suffix(f".{unique}.tmp")
    tmp.write_text(json.dumps(record, sort_keys=True) + "\n", encoding="utf-8", newline="\n")
    os.replace(tmp, entry_path)
    return False
