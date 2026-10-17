"""Build code.bin from the matched source and certify it against the original.

    python tools/build.py --name <unique build name>

Refuses to run unless config/catalogue.json is the recorded denominator (tools/denominator.py).
For every unit in config/matches.json (or --matches): compile it under its profile and link it in
place. Each credited function must be an ordinary catalogued function (never a veneer), must be
produced as a section of exactly its catalogued size, and must equal the original on its own
before it is written into the output image; sections placed by different units may never overlap.
The output must equal orig/code.bin, and re-compressing it must reproduce the stored ExeFS
`.code` exactly. Everything goes into build/<name>/ (exclusive: an existing name is refused) with
receipt.json binding every input hash, including the harness sources, and the linker model
(`--linker-model`, default `inplace.DEFAULT_LINKER_MODEL`, docs/LINKER.md) with every branch it
wrote as an armlink optimisation inside a credited function.

Not credited and not proven here: bytes outside credited functions (still original), linker-made
content (veneers, section order), and data layout. See docs/ACCEPTANCE.md.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import platform
import sys
from concurrent.futures import ThreadPoolExecutor
from datetime import datetime, timezone
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ctrfmt  # noqa: E402
import denominator  # noqa: E402
import image as image_mod  # noqa: E402
import inplace  # noqa: E402
import toolchain  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
MATCHES = ROOT / "config/matches.json"
HARNESS = ("tools/build.py", "tools/inplace.py", "tools/image.py", "tools/toolchain.py", "tools/ctrfmt.py", "tools/denominator.py")


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path: Path) -> str:
    return sha256_bytes(path.read_bytes())


class BuildError(RuntimeError):
    pass


def compile_units(units: list[dict], profiles: dict, out: Path, fresh: bool, jobs: int) -> dict[str, bool]:
    """Compile every unit in parallel (armcc runs as separate processes). Returns source -> cache hit."""
    for unit in units:
        if unit["profile"] not in profiles:
            raise BuildError(f"{unit['source']}: unknown profile {unit['profile']!r}")

    def one(unit: dict) -> tuple[str, bool]:
        obj = out / "obj" / Path(unit["source"]).with_suffix(".o")
        hit = toolchain.compile_source(profiles[unit["profile"]], ROOT / unit["source"], obj, cache=not fresh)
        return unit["source"], hit

    with ThreadPoolExecutor(max_workers=max(1, jobs)) as pool:
        return dict(pool.map(one, units))


def build(name: str, matches_path: Path = MATCHES, fresh: bool = False, jobs: int | None = None,
          linker_model: str = inplace.DEFAULT_LINKER_MODEL,
          literal_model: str = inplace.DEFAULT_LITERAL_MODEL) -> dict:
    out = ROOT / "build" / name
    try:
        out.mkdir(parents=True, exist_ok=False)
    except FileExistsError:
        raise BuildError(f"build/{name} already exists; build records are immutable") from None
    try:
        denominator.verify()
    except SystemExit as e:
        raise BuildError(f"denominator: {e}") from None
    image = image_mod.Image.load()
    catalogue = image_mod.load_catalogue()
    if catalogue["image_sha256"] != image.sha256:
        raise BuildError("config/catalogue.json was generated from a different code.bin")
    functions = image_mod.functions_by_address(catalogue)
    names = image_mod.load_names()
    resolver = inplace.Resolver(functions, names)
    linker = inplace.InPlaceLinker(image, functions, resolver, image.bss, linker_model, literal_model)
    matches = json.loads(matches_path.read_text(encoding="utf-8"))
    profiles = toolchain.load_profiles()
    hits = compile_units(matches["units"], profiles, out, fresh, jobs or os.cpu_count() or 1)

    hybrid = bytearray(image.code)
    credited: dict[int, str] = {}
    mismatched: list[str] = []
    occupied: list[tuple[int, int, str, tuple[str, str] | None]] = []  # sections placed by all units
    definitions: dict[str, str] = {}  # global (non-COMDAT) definition name -> defining unit
    objects = [inplace.ObjectFile(out / "obj" / Path(unit["source"]).with_suffix(".o")) for unit in matches["units"]]
    combined = inplace.ObjectFile.combine(objects, [unit["source"] for unit in matches["units"]])
    all_linked = linker.link(combined)
    units = []
    for owner, unit in enumerate(matches["units"]):
        src = ROOT / unit["source"]
        profile = profiles[unit["profile"]]
        obj = out / "obj" / Path(unit["source"]).with_suffix(".o")
        objfile = objects[owner]
        linked = {index: value for index, value in all_linked.items() if combined.section_owners[index] == owner}
        for sym in objfile.defined_globals():
            if objfile.sections[sym.shndx].comdat:
                continue  # one entity; its single address is enforced by reject_overlaps
            if sym.name in definitions:
                raise BuildError(f"{sym.name}: defined by both {definitions[sym.name]} and {unit['source']}")
            definitions[sym.name] = unit["source"]
        occupied += [
            (p.base, p.base + p.section.size, f"{unit['source']}:{p.section.name}", inplace.comdat_key(p.section))
            for p, _ in linked.values() if p.section.size
        ]
        placed = {p.base: data for p, data in linked.values() if p.origin == "catalogue"}
        section_of = {p.base: p.section.index for p, _ in linked.values() if p.origin == "catalogue"}
        rewrites = {}
        for fname in unit["functions"]:
            address = resolver.resolve(fname).address
            fn = functions.get(address)
            if fn is None:
                raise BuildError(f"{fname}: not a catalogued function")
            if not image_mod.creditable(fn):
                raise BuildError(f"{fname}: catalogue kind {fn.get('kind')!r} can never earn C credit")
            if address in credited:
                raise BuildError(f"{fname}: credited by both {credited[address]} and {unit['source']}")
            data = placed.get(address)
            if data is None or len(data) != fn["size"]:
                raise BuildError(f"{fname}: {unit['source']} does not produce a section of size {fn['size']:#x}")
            # Check each credited function on its own before anything is written, so a later
            # write can never conceal a wrong one.
            if data != image.read(address, fn["size"]):
                mismatched.append(f"{address:#x}")
            start = image.layout.file_offset(address)
            hybrid[start : start + fn["size"]] = data
            credited[address] = unit["source"]
            done = [w.record() for w in linker.rewrites if w.section == section_of[address]]
            if done:
                rewrites[fname] = done
        units.append({
            "source": unit["source"], "source_sha256": sha256_file(src), "profile": profile.name,
            "compiler": profile.compiler, "flags": list(profile.flags), "object_sha256": sha256_file(obj),
            "functions": sorted(unit["functions"]), "cache_hit": hits[unit["source"]],
            "linker_rewrites": rewrites,
            "literal_rebindings": [row for row in linker.literal_rebindings
                                   if combined.section_owners[row["section_index"]] == owner],
            "removed_callees": [row for row in linker.removed_callees if row["object"] == unit["source"]],
        })
    try:
        inplace.reject_overlaps(occupied)
    except inplace.LinkError as e:
        raise BuildError(f"units place overlapping sections: {e}") from None

    code = bytes(hybrid)
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
        "fresh": fresh,
        "inputs": {
            "code_bin_sha256": image.sha256,
            "exefs_code_sha256": sha256_bytes(stored),
            "catalogue_sha256": sha256_file(image_mod.CATALOGUE),
            "names_sha256": sha256_file(image_mod.NAMES) if image_mod.NAMES.is_file() else None,
            "matches": matches_path.resolve().relative_to(ROOT).as_posix(),
            "matches_canonical": matches_path.resolve() == MATCHES.resolve(),
            "matches_sha256": sha256_file(matches_path),
            "profiles_sha256": sha256_file(toolchain.PROFILES),
            "toolchain_lock_sha256": sha256_file(ROOT / "toolchain.lock.json"),
            "exheader_sha256": sha256_file(ROOT / "orig/exheader.bin"),
            "harness_sha256": {p: sha256_file(ROOT / p) for p in HARNESS},
            "linker_model": linker.model.name,
            "literal_model": linker.literal_model,
            "compilers": {c: sha256_file(toolchain.armcc_exe(c)) for c in used},
        },
        "units": units,
        "outputs": {"code_bin_sha256": sha256_bytes(code), "exefs_code_sha256": sha256_bytes(packed)},
        "credited": {
            "functions": len(credited),
            "bytes": sum(functions[a]["size"] for a in credited),
            # same C denominator as ci/progress_report.py: classified kinds (veneers) excluded
            "catalogue_functions": sum(1 for f in functions.values() if image_mod.creditable(f)),
            "catalogue_bytes": sum(f["size"] for f in functions.values() if image_mod.creditable(f)),
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
    with (out / "receipt.json").open("x", encoding="utf-8", newline="\n") as f:
        f.write(json.dumps(receipt, indent=1) + "\n")
    return receipt


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--name", required=True, help="unique build name, e.g. 2026-10-15-baseline")
    parser.add_argument("--matches", type=Path, default=MATCHES,
                        help="units to credit (default config/matches.json; a scratch file makes a candidate build)")
    parser.add_argument("--fresh", action="store_true", help="compile everything, ignoring the object cache")
    parser.add_argument("--jobs", type=int, help="parallel compiles (default: CPU count)")
    parser.add_argument("--linker-model", default=inplace.DEFAULT_LINKER_MODEL, choices=sorted(inplace.LINKER_MODELS),
                        help="armlink output optimisations reproduced by the in-place link (docs/LINKER.md)")
    parser.add_argument("--literal-model", default=inplace.DEFAULT_LITERAL_MODEL, choices=inplace.LITERAL_MODELS,
                        help="independent anonymous string placement rule; default off (docs/LINKER.md)")
    args = parser.parse_args(argv)
    try:
        receipt = build(args.name, args.matches, args.fresh, args.jobs, args.linker_model, args.literal_model)
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
