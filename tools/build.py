"""Build code.bin from the matched source and certify it against the original.

    python tools/build.py --name <unique build name>

For every unit in config/matches.json: compile it under its profile, link it in place, and require
each credited function to match byte for byte. The output image starts as a copy of the original
with every credited function's range erased (0xCC), then the compiled bytes are written in; a range
that compiled code did not fully rewrite is an error. The result must equal orig/code.bin, and
re-compressing it must reproduce the stored ExeFS `.code` exactly. Everything goes into
build/<name>/ (exclusive: an existing name is refused) with receipt.json binding every input hash.

Not credited and not proven here: bytes outside credited functions (still original), linker-made
content (veneers, section order), and data layout. See docs/ACCEPTANCE.md.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import platform
import sys
from datetime import datetime, timezone
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ctrfmt  # noqa: E402
import image as image_mod  # noqa: E402
import inplace  # noqa: E402
import toolchain  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
MATCHES = ROOT / "config/matches.json"
FILL = 0xCC


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path: Path) -> str:
    return sha256_bytes(path.read_bytes())


class BuildError(RuntimeError):
    pass


def build(name: str) -> dict:
    out = ROOT / "build" / name
    try:
        out.mkdir(parents=True, exist_ok=False)
    except FileExistsError:
        raise BuildError(f"build/{name} already exists; build records are immutable") from None
    image = image_mod.Image.load()
    catalogue = image_mod.load_catalogue()
    if catalogue["image_sha256"] != image.sha256:
        raise BuildError("config/catalogue.json was generated from a different code.bin")
    functions = image_mod.functions_by_address(catalogue)
    names = image_mod.load_names()
    resolver = inplace.Resolver(functions, names)
    linker = inplace.InPlaceLinker(image, functions, resolver, image.bss)
    matches = json.loads(MATCHES.read_text())
    profiles = toolchain.load_profiles()

    hybrid = bytearray(image.code)
    credited: dict[int, str] = {}
    units = []
    for unit in matches["units"]:
        src = ROOT / unit["source"]
        profile = profiles.get(unit["profile"])
        if profile is None:
            raise BuildError(f"{unit['source']}: unknown profile {unit['profile']!r}")
        obj = out / "obj" / Path(unit["source"]).with_suffix(".o")
        toolchain.compile_source(profile, src, obj)
        linked = linker.link(inplace.ObjectFile(obj))
        placed = {p.base: data for p, data in linked.values() if p.origin == "catalogue"}
        for fname in unit["functions"]:
            address = resolver.resolve(fname).address
            fn = functions.get(address)
            if fn is None:
                raise BuildError(f"{fname}: not a catalogued function")
            if address in credited:
                raise BuildError(f"{fname}: credited by both {credited[address]} and {unit['source']}")
            data = placed.get(address)
            if data is None or len(data) != fn["size"]:
                raise BuildError(f"{fname}: {unit['source']} does not produce a section of size {fn['size']:#x}")
            start = image.layout.file_offset(address)
            hybrid[start : start + fn["size"]] = bytes([FILL]) * fn["size"]
            credited[address] = unit["source"]
        for address, data in placed.items():
            if address in credited and credited[address] == unit["source"]:
                start = image.layout.file_offset(address)
                hybrid[start : start + len(data)] = data
        units.append({
            "source": unit["source"], "source_sha256": sha256_file(src), "profile": profile.name,
            "compiler": profile.compiler, "flags": list(profile.flags), "object_sha256": sha256_file(obj),
            "functions": sorted(unit["functions"]),
        })

    code = bytes(hybrid)
    mismatched = [
        f"{a:#x}" for a, fn in functions.items() if a in credited
        and code[image.layout.file_offset(a) : image.layout.file_offset(a) + fn["size"]]
        != image.read(a, fn["size"])
    ]
    code_ok = code == image.code
    packed = ctrfmt.blz_compress(code)
    stored = (ROOT / "orig/exefs/code.blz").read_bytes()
    blz_ok = packed == stored
    (out / "code.bin").write_bytes(code)
    (out / "code.blz").write_bytes(packed)

    used = sorted({u["compiler"] for u in units})
    receipt = {
        "schema_version": 1,
        "name": name,
        "created_utc": datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "host": f"{platform.system()}-{platform.machine()}",
        "inputs": {
            "code_bin_sha256": image.sha256,
            "exefs_code_sha256": sha256_bytes(stored),
            "catalogue_sha256": sha256_file(image_mod.CATALOGUE),
            "names_sha256": sha256_file(image_mod.NAMES) if image_mod.NAMES.is_file() else None,
            "matches_sha256": sha256_file(MATCHES),
            "profiles_sha256": sha256_file(toolchain.PROFILES),
            "toolchain_lock_sha256": sha256_file(ROOT / "toolchain.lock.json"),
            "compilers": {c: sha256_file(toolchain.armcc_exe(c)) for c in used},
        },
        "units": units,
        "outputs": {"code_bin_sha256": sha256_bytes(code), "exefs_code_sha256": sha256_bytes(packed)},
        "credited": {
            "functions": len(credited),
            "bytes": sum(functions[a]["size"] for a in credited),
            "catalogue_functions": len(functions),
            "catalogue_bytes": sum(f["size"] for f in functions.values()),
        },
        "checks": {
            "credited_functions_exact": not mismatched,
            "code_bin_identical": code_ok,
            "exefs_code_identical": blz_ok,
        },
    }
    receipt["result"] = "PASS" if all(receipt["checks"].values()) else "FAIL"
    if mismatched:
        receipt["mismatched_functions"] = mismatched
    with (out / "receipt.json").open("x") as f:
        f.write(json.dumps(receipt, indent=1) + "\n")
    return receipt


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--name", required=True, help="unique build name, e.g. 2026-10-15-baseline")
    args = parser.parse_args(argv)
    try:
        receipt = build(args.name)
    except (BuildError, inplace.LinkError, toolchain.ToolchainError, image_mod.ImageError) as e:
        print(f"error: {e}", file=sys.stderr)
        return 2
    c = receipt["credited"]
    print(f"{receipt['result']}: {c['functions']}/{c['catalogue_functions']} functions, "
          f"{c['bytes']}/{c['catalogue_bytes']} bytes credited; checks {receipt['checks']}")
    print(f"receipt: build/{args.name}/receipt.json")
    return 0 if receipt["result"] == "PASS" else 1


if __name__ == "__main__":
    raise SystemExit(main())
