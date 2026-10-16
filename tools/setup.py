"""Provision the pinned toolchain from toolchain.lock.json into .cache/toolchain/.

    python tools/setup.py            # ARM compilers (+ wibo on macOS/Linux)
    python tools/setup.py --objdiff  # also the optional objdiff-cli diagnostic tool
    python tools/setup.py --check    # verify what is installed without downloading

Every download is checked against its pinned size/sha256 before use. After extraction each compiler
is run once (`armcc --vsn`) and its banner plus executable hash are recorded in
.cache/toolchain/installed.json. Works on Windows (armcc.exe runs natively), macOS and Linux (via
wibo). Nothing here is committed: the compilers are proprietary and stay private.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import shutil
import subprocess
import sys
import urllib.request
import zipfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from toolchain import INSTALL, LOCK, ROOT, ToolchainError, armcc_exe, host_arch, host_os, wibo_path  # noqa: E402

DOWNLOADS = ROOT / ".cache/downloads"
INSTALLED = INSTALL / "installed.json"


class SetupError(RuntimeError):
    pass


def file_sha256(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        while chunk := f.read(1 << 22):
            h.update(chunk)
    return h.hexdigest()


def fetch(url: str, sha256: str, name: str) -> Path:
    """Download once into .cache/downloads/, verifying the pinned hash (also for cached files)."""
    DOWNLOADS.mkdir(parents=True, exist_ok=True)
    dest = DOWNLOADS / name
    if not dest.is_file() or file_sha256(dest) != sha256:
        tmp = dest.with_suffix(dest.suffix + ".part")
        print(f"downloading {url}")
        with urllib.request.urlopen(url) as response, tmp.open("wb") as out:
            shutil.copyfileobj(response, out)
        got = file_sha256(tmp)
        if got != sha256:
            tmp.unlink()
            raise SetupError(f"{name}: sha256 {got} != pinned {sha256}")
        tmp.replace(dest)
    return dest


def install_armcc() -> None:
    spec = LOCK["armcc"]
    archive = fetch(spec["url"], spec["sha256"], "armcc.zip")
    target = INSTALL / "armcc"
    if all(armcc_exe(cid).is_file() for cid in spec["builds"]):
        return
    if target.exists():
        shutil.rmtree(target)
    with zipfile.ZipFile(archive) as z:
        z.extractall(target)


def install_wibo() -> None:
    if host_os() == "windows":
        return
    spec = LOCK["wibo"]["hosts"][host_os()]
    src = fetch(spec["url"], spec["sha256"], f"wibo-{host_os()}")
    dest = wibo_path()
    dest.parent.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(src, dest)
    dest.chmod(0o755)


def install_objdiff() -> None:
    key = f"{host_os()}-{host_arch()}"
    spec = LOCK["objdiff_cli"]["hosts"].get(key)
    if spec is None:
        raise SetupError(f"no pinned objdiff-cli for {key}")
    src = fetch(spec["url"], spec["sha256"], Path(spec["url"]).name)
    dest = INSTALL / "bin" / ("objdiff-cli.exe" if host_os() == "windows" else "objdiff-cli")
    dest.parent.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(src, dest)
    dest.chmod(0o755)


def probe_compilers() -> dict:
    """Run every compiler once; record its banner and executable hash."""
    launcher = wibo_path()
    record = {"host": f"{host_os()}-{host_arch()}", "compilers": {}}
    for cid in LOCK["armcc"]["builds"]:
        exe = armcc_exe(cid)
        argv = ([str(launcher)] if launcher else []) + [str(exe), "--vsn"]
        proc = subprocess.run(argv, capture_output=True, text=True, encoding="utf-8", errors="replace", timeout=60, env=os.environ | {"LC_ALL": "C"})
        banner = (proc.stdout + proc.stderr).strip().splitlines()
        if proc.returncode != 0 or not banner:
            raise SetupError(f"{cid}: `armcc --vsn` failed ({proc.returncode}): {proc.stderr.strip()}")
        record["compilers"][cid] = {"banner": " | ".join(line.strip() for line in banner), "armcc_exe_sha256": file_sha256(exe)}
    return record


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--objdiff", action="store_true", help="also install objdiff-cli")
    parser.add_argument("--check", action="store_true", help="only verify the installed compilers")
    args = parser.parse_args(argv)
    try:
        if not args.check:
            install_armcc()
            install_wibo()
            if args.objdiff:
                install_objdiff()
        record = probe_compilers()
    except (SetupError, ToolchainError, OSError, subprocess.SubprocessError) as e:
        print(f"error: {e}", file=sys.stderr)
        return 1
    INSTALL.mkdir(parents=True, exist_ok=True)
    INSTALLED.write_text(json.dumps(record, indent=1, sort_keys=True) + "\n", encoding="utf-8", newline="\n")
    for cid, info in record["compilers"].items():
        print(f"{cid:16} {info['banner']}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
