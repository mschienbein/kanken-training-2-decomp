"""Private, catalogue-seeded Ghidra pseudo-C for a function (not a matching-code oracle).

    python tools/decompile.py func_XXXXXXXX

Requires Ghidra 12.1.4 + JDK 21, installed independently; see docs/DECOMPILER.md.
Only prints the private output path. All decompiler output/logs stay under scratch/.
"""
from __future__ import annotations

import argparse
from contextlib import contextmanager
import fcntl
import hashlib
import json
import math
import os
from pathlib import Path
import re
import signal
import subprocess
import sys
import time
from typing import Iterator
import uuid

sys.path.insert(0, str(Path(__file__).resolve().parent))
import image as image_mod  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
SCRIPT = ROOT / "tools/ghidra/DecompileFunction.java"
LANGUAGE = "ARM:LE:32:v6"
SCHEMA = 1
DEFAULT_GHIDRA = Path.home() / ".local/opt/ghidra_12.1.4_PUBLIC"
DEFAULT_JAVA = Path.home() / ".local/opt/jdk-21.0.12.1+1"


class DecompileError(RuntimeError):
    pass


def json_bytes(value: object) -> bytes:
    return json.dumps(value, sort_keys=True, separators=(",", ":")).encode("utf-8")


def safe_scratch(path: Path, root: Path = ROOT) -> Path:
    """Refuse publication outside the shared repo's ignored scratch directory."""
    path = path.resolve()
    scratch = (root / "orig").resolve().parent / "scratch"
    if not path.is_relative_to(scratch.resolve()):
        raise DecompileError("Decompiler artifacts must remain under the shared repository's scratch/")
    return path


def select_function(target: str, catalogue: dict, names: dict) -> dict:
    if target in names:
        address = names[target]["address"]
    elif re.fullmatch(r"func_[0-9a-fA-F]{8}", target):
        address = int(target[5:], 16)
    else:
        raise DecompileError(f"Unknown function name: {target}")
    function = image_mod.functions_by_address(catalogue).get(address)
    if function is None:
        raise DecompileError(f"No catalogued function at {address:#x}")
    if not image_mod.creditable(function):
        raise DecompileError("Only ordinary catalogue functions are decompiled; inspect classified entries with packet.py")
    return function


def subtract_ranges(ranges: list, exclusions: list) -> list[list[int]]:
    """Subtract known literal pools even when an older catalogue range overlaps them."""
    result = []
    for start, end in ranges:
        fragments = [(start, end)]
        for lo, hi in exclusions:
            fragments = [(a, b) for x, y in fragments
                         for a, b in ((x, min(y, lo)), (max(x, hi), y)) if a < b]
        result.extend([a, b] for a, b in fragments)
    return result


def make_manifest(image: image_mod.Image, catalogue: dict, names: dict, binary: Path) -> dict:
    by_address = {entry["address"]: name for name, entry in names.items()}
    functions = []
    for fn in image_mod.functions_by_address(catalogue).values():
        start, stop = fn["address"], fn["address"] + fn["size"]
        ranges = subtract_ranges(fn["code_ranges"], [*fn.get("pools", []), *fn.get("literals", [])])
        if fn["isa"] not in ("arm", "thumb") or any(not start <= lo < hi <= stop for lo, hi in ranges):
            raise DecompileError(f"Invalid catalogue code ranges at {start:#x}")
        functions.append({"address": start, "name": by_address.get(start, f"func_{start:08x}"),
                          "isa": fn["isa"], "code_ranges": ranges})
    segments = [{"name": s.name, "address": s.address, "size": s.size,
                 "offset": image.layout.file_offset(s.address)} for s in image.layout.segments]
    return {"binary": str(binary.resolve()), "segments": segments, "bss_address": image.bss[0],
            "bss_size": image.layout.bss_size, "functions": functions,
            "symbols": [{"name": name, "address": entry["address"]} for name, entry in sorted(names.items())]}


def fingerprint(image: image_mod.Image, manifest: dict, ghidra: Path, script: Path = SCRIPT) -> str:
    properties = ghidra / "Ghidra/application.properties"
    content = {"schema": SCHEMA, "language": LANGUAGE, "image": image.sha256,
               "manifest": manifest, "script": hashlib.sha256(script.read_bytes()).hexdigest(),
               "ghidra": hashlib.sha256(properties.read_bytes()).hexdigest()}
    return hashlib.sha256(json_bytes(content)).hexdigest()


def remaining(deadline: float) -> float:
    seconds = deadline - time.monotonic()
    if seconds <= 0:
        raise DecompileError("Decompiler wall-time budget exhausted (including project-lock wait)")
    return seconds


@contextmanager
def project_lock(path: Path, deadline: float) -> Iterator[None]:
    """One shared project writer across processes and worktrees; cache hits need no lock."""
    with path.open("a") as lock:
        while True:
            remaining(deadline)
            try:
                fcntl.flock(lock.fileno(), fcntl.LOCK_EX | fcntl.LOCK_NB)
                break
            except BlockingIOError:
                time.sleep(min(0.05, remaining(deadline)))
        try:
            yield
        finally:
            fcntl.flock(lock.fileno(), fcntl.LOCK_UN)


def run_headless(command: list[str], env: dict, log: Path, deadline: float) -> None:
    """Kill the entire JVM/decompiler process group on timeout; never echo private logs."""
    timeout = remaining(deadline)
    with log.open("w", encoding="utf-8") as output:
        process = subprocess.Popen(command, stdout=output, stderr=subprocess.STDOUT,
                                   env=env, start_new_session=True, stdin=subprocess.DEVNULL)
        try:
            status = process.wait(timeout=timeout)
        except (subprocess.TimeoutExpired, KeyboardInterrupt):
            os.killpg(process.pid, signal.SIGKILL)
            process.wait()
            raise DecompileError(f"Headless run interrupted or timed out; private log: {log}") from None
    if status:
        raise DecompileError(f"Headless Ghidra exited {status}; private log: {log}")


def cached_result(directory: Path, name: str, key: str) -> Path | None:
    source, receipt = directory / f"{name}.c", directory / f"{name}.json"
    if not source.is_file() or not receipt.is_file():
        return None
    try:
        metadata = json.loads(receipt.read_text(encoding="utf-8"))
    except (json.JSONDecodeError, OSError):
        return None
    if (metadata.get("fingerprint") != key or not metadata.get("completed")
            or metadata.get("source_sha256") != hashlib.sha256(source.read_bytes()).hexdigest()):
        return None
    return source


def decompile(target: str, *, ghidra: Path, java_home: Path, seconds: float = 600,
              function_seconds: int = 60, root: Path = ROOT) -> Path:
    if not math.isfinite(seconds) or seconds <= 0 or function_seconds <= 0:
        raise DecompileError("Runtime budgets must be positive and finite")
    deadline = time.monotonic() + seconds
    image = image_mod.Image.load(root / "orig")
    catalogue = image_mod.load_catalogue(root / "config/catalogue.json")
    names = image_mod.load_names(root / "config/names.json")
    function = select_function(target, catalogue, names)
    name = f"func_{function['address']:08x}"
    ghidra, java_home = ghidra.resolve(), java_home.resolve()
    if not (ghidra / "support/analyzeHeadless").is_file():
        raise DecompileError("Ghidra not installed; set GHIDRA_HOME or pass --ghidra (see docs/DECOMPILER.md)")
    if not (java_home / "bin/java").is_file():
        raise DecompileError("JDK not installed; set JAVA_HOME or pass --java-home (see docs/DECOMPILER.md)")
    manifest = make_manifest(image, catalogue, names, root / "orig/code.bin")
    key = fingerprint(image, manifest, ghidra)
    directory = safe_scratch((root / "orig").resolve().parent / "scratch/decompile" / key, root)
    directory.mkdir(parents=True, exist_ok=True)
    hit = cached_result(directory, name, key)
    if hit:
        return hit
    with project_lock(directory / "project.lock", deadline):
        hit = cached_result(directory, name, key)
        if hit:
            return hit
        manifest_path = directory / "manifest.json"
        manifest_path.write_bytes(json_bytes(manifest))
        project = directory / "project"
        project.mkdir(exist_ok=True)
        initialized = (directory / "ready.json").is_file() and (project / "kanken.gpr").is_file()
        token = uuid.uuid4().hex
        source_tmp = directory / f".{name}-{token}.c"
        metadata_tmp = directory / f".{name}-{token}.json"
        log = directory / f"{name}.headless.log"
        command = [str(ghidra / "support/analyzeHeadless"), str(project), "kanken"]
        if initialized:
            command += ["-process", "code.bin", "-readOnly"]
        else:
            command += ["-import", str((root / "orig/code.bin").resolve()), "-overwrite",
                        "-loader", "BinaryLoader", "-processor", LANGUAGE, "-cspec", "default"]
        command += ["-noanalysis", "-max-cpu", "2", "-scriptPath", str(SCRIPT.parent),
                    "-log", str(directory / "application.log"), "-scriptlog", str(directory / "script.log"),
                    "-postScript", SCRIPT.name, str(manifest_path), "reuse" if initialized else "seed",
                    str(function["address"]), str(function_seconds), str(source_tmp), str(metadata_tmp)]
        env = dict(os.environ, JAVA_HOME=str(java_home), GHIDRA_HEADLESS_MAXMEM="2G",
                   GHIDRA_HEADLESS_JAVA_OPTIONS="-XX:ActiveProcessorCount=2")
        start = time.monotonic()
        try:
            run_headless(command, env, log, deadline)
            # Ghidra can return zero after a script exception: require both explicit script outputs.
            if not source_tmp.is_file() or not metadata_tmp.is_file() or not (project / "kanken.gpr").is_file():
                raise DecompileError(f"Ghidra script did not publish a complete result; private log: {log}")
            metadata = json.loads(metadata_tmp.read_text(encoding="utf-8"))
            if not metadata.get("completed") or metadata.get("address") != function["address"]:
                raise DecompileError(f"Invalid Ghidra result; private log: {log}")
            metadata.update(fingerprint=key, headless_seconds=time.monotonic() - start,
                            initialized_project=not initialized,
                            source_sha256=hashlib.sha256(source_tmp.read_bytes()).hexdigest())
            metadata_tmp.write_bytes(json_bytes(metadata))
            source_tmp.replace(directory / f"{name}.c")
            metadata_tmp.replace(directory / f"{name}.json")
            (directory / "ready.json").write_bytes(json_bytes({"fingerprint": key}))
        finally:
            source_tmp.unlink(missing_ok=True)
            metadata_tmp.unlink(missing_ok=True)
    return directory / f"{name}.c"


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("function", help="func_XXXXXXXX or a names-overlay function")
    parser.add_argument("--ghidra", type=Path, default=Path(os.environ.get("GHIDRA_HOME", DEFAULT_GHIDRA)))
    parser.add_argument("--java-home", type=Path, default=Path(os.environ.get("JAVA_HOME", DEFAULT_JAVA)))
    parser.add_argument("--seconds", type=float, default=600, help="whole call budget, including lock wait (default: 600)")
    parser.add_argument("--function-seconds", type=int, default=60, help="native decompiler budget (default: 60)")
    args = parser.parse_args(argv)
    try:
        result = decompile(args.function, ghidra=args.ghidra, java_home=args.java_home,
                           seconds=args.seconds, function_seconds=args.function_seconds)
    except (DecompileError, image_mod.ImageError, OSError, ValueError) as error:
        print(f"decompile: {error}", file=sys.stderr)
        return 2
    print(result)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
