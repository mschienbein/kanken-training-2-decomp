"""Build code.bin from the matched source and certify it against the original.

    python tools/build.py --name <unique build name>

Refuses to run unless config/catalogue.json is the recorded denominator (tools/denominator.py).
For every unit in config/matches.json (or --matches): compile it under its profile and link it in
place. Each credited function must be an ordinary catalogued function (never a veneer), must be
produced as a section of exactly its catalogued size, and must equal the original on its own
before it is written into the output image. Its credited unit must be its sole emitted defining
unit, including weak/COMDAT definitions and names-overlay aliases. Sections placed by different
units may never overlap.
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
import match as matcher  # noqa: E402
import toolchain  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
MATCHES = ROOT / "config/matches.json"
HARNESS = ("tools/build.py", "tools/inplace.py", "tools/linker_models.py", "tools/image.py",
           "tools/toolchain.py", "tools/ctrfmt.py", "tools/denominator.py", "tools/match.py")


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path: Path) -> str:
    return sha256_bytes(path.read_bytes())


class BuildError(RuntimeError):
    pass


def unit_object_path(out: Path, source: str, distinct: bool = False) -> Path:
    relative = Path(source + ".o") if distinct else Path(source).with_suffix(".o")
    return out / "obj" / relative


def compile_units(units: list[dict], profiles: dict, out: Path, fresh: bool, jobs: int,
                  distinct_paths: bool = False) -> dict[str, bool]:
    """Compile every unit in parallel (armcc runs as separate processes). Returns source -> cache hit."""
    for unit in units:
        if unit["profile"] not in profiles:
            raise BuildError(f"{unit['source']}: unknown profile {unit['profile']!r}")

    def one(unit: dict) -> tuple[str, bool]:
        obj = unit_object_path(out, unit["source"], distinct_paths)
        hit = toolchain.compile_source(profiles[unit["profile"]], ROOT / unit["source"], obj, cache=not fresh)
        return unit["source"], hit

    with ThreadPoolExecutor(max_workers=max(1, jobs)) as pool:
        return dict(pool.map(one, units))

def check_u1_group_closure(objects: list, units: list[dict], plans: list[dict], image,
                           functions: dict, resolver, model, literal_model: str) -> None:
    """Negative all-original-copy proof; never lend eligibility or decide aggregate U1."""
    affected = {original.sections[index].comdat for original, plan in zip(objects, plans)
                for index in plan["removed"] if index in original.sections} - {None}
    if not affected:
        return
    for original, plan in zip(objects, plans):
        if model.complete_group_declarations:
            allocated = original.sections | original.dependency_sections
            for declaration in original.group_declarations:
                if declaration.signature in affected:
                    members = set(declaration.members) & allocated.keys()
                    if not members or not members <= plan["removed"]:
                        raise BuildError("affected U1 COMDAT copy declaration lacks a complete allocated removal proof")
        if any(signature in affected for signature, _ in getattr(original, "group_issues", [])):
            raise BuildError("affected U1 COMDAT copy has unrepresented group members")
        for signature, flags, canonical, _ in getattr(original, "group_copies", []):
            if signature in affected and (flags != 1 or not canonical):
                raise BuildError("affected U1 COMDAT copy has unsupported group semantics")
        for index, section in (original.sections | getattr(original, "dependency_sections", {})).items():
            memberships = section.metadata.groups if section.metadata is not None else ()
            declared = {group.signature_symbol[0] for group in memberships if group.signature_symbol}
            if section.comdat not in affected and not declared & affected:
                continue
            if (section.comdat not in affected or index not in plan["removed"]
                    or len(memberships) != 1 or memberships[0].flags != 1
                    or memberships[0].signature != section.comdat
                    or not memberships[0].canonical_symbol_table):
                raise BuildError("affected U1 COMDAT copy lacks complete allocated member removal proof")
    original_combined = inplace.ObjectFile.combine(objects, [unit["source"] for unit in units])
    remap = {source: index for index, source in original_combined.section_input_indices.items()}
    checker = inplace.InPlaceLinker(image, functions, resolver, image.bss, model, literal_model)
    for owner, plan in enumerate(plans):
        for member in plan["members"]:
            index = remap[(owner, member["section_index"])]
            checker._unreferenced_sections[index] = member
            for companion in member.get("unwind_sections", []):
                checker._removed_unwind[remap[(owner, companion["section_index"])]] = index
    checker._check_destructor_group_copies(original_combined, affected)


def prepare_unit_objects(objects: list, units: list[dict], image, functions: dict, resolver,
                         model, literal_model: str) -> tuple[list, list[dict]]:
    """V16 decides U1 only on each own direct unit, then applies composition vetoes."""
    if len(objects) != len(units):
        raise BuildError("unit/object count differs")
    if model.complete_group_declarations:
        for original in objects:
            original.check_group_declarations()
    prepared, plans = [], []
    for original, unit in zip(objects, units):
        view = original.__class__.__new__(original.__class__)
        view.__dict__ = original.__dict__.copy()
        view.section_objects = {index: unit["source"] for index in
                                (view.sections | getattr(view, "dependency_sections", {}))}
        local = inplace.InPlaceLinker(image, functions, resolver, image.bss, model, literal_model)
        output, removed = local.prepare_unit_u1(view)
        for member in local.removed_special_members:
            address = member["evidence"]["complete_destructor_address"]
            member["evidence"]["complete_destructor_size"] = functions[address]["size"]
        prepared.append(output)
        plans.append({"source": unit["source"], "removed": removed,
                      "members": local.removed_special_members,
                      "actions": [rewrite.record() for rewrite in local.rewrites]})
    defined_names = {symbol.name for obj in objects for symbol in obj.defined_symbols()}
    for owner, (original, plan) in enumerate(zip(objects, plans)):
        if not plan["removed"]:
            continue
        names = {symbol.name for symbol in original.symbols if symbol.shndx in plan["removed"] and symbol.name}
        for foreign_owner, foreign in enumerate(objects):
            if foreign_owner == owner:
                continue
            for section in (foreign.sections | getattr(foreign, "dependency_sections", {})).values():
                for relocation in section.relocs:
                    if (relocation.type == inplace.R_ARM_V4BX and section.executable
                            and section.elf_type == "SHT_PROGBITS" and not section.link_order):
                        continue
                    reference = relocation.reference if relocation.reference is not None else relocation.symbol
                    if (reference.name in names
                            and (reference.shndx == "SHN_UNDEF" or reference.bind in ("STB_GLOBAL", "STB_WEAK"))):
                        raise BuildError("foreign supplied reference keeps a per-unit U1 removal live")
        companions = {row["section_index"] for member in plan["members"]
                      for row in member.get("unwind_sections", [])}
        for index in companions:
            for relocation in original.sections[index].relocs:
                if (relocation.type == inplace.R_ARM_NONE and relocation.symbol.shndx == "SHN_UNDEF"
                        and relocation.symbol.name in defined_names):
                    raise BuildError("per-unit U1 personality marker has a supplied ledger definition")
    if model.closed_u1_composition:
        check_u1_group_closure(objects, units, plans, image, functions, resolver, model, literal_model)
    return prepared, plans


def u1_witness_comparisons(plan: dict, linked: dict, functions: dict, image) -> list[dict]:
    """Every retained D1 witness is byte/extent checked even when it earns no ledger credit."""
    results = {}
    for member in plan["members"]:
        evidence = member["evidence"]
        address = evidence["complete_destructor_address"]
        candidates = [data for placement, data in linked.values()
                      if placement.origin == "catalogue" and placement.base == address]
        size = functions[address]["size"]
        if len(candidates) != 1 or len(candidates[0]) != size or candidates[0] != image.read(address, size):
            raise BuildError("per-unit U1 retained D1 witness does not match its complete catalogue extent")
        original = image.read(address, size)
        results[address] = {"name": evidence["complete_destructor"], "address": address, "size": size,
                            "status": "match", "sha256": sha256_bytes(candidates[0]),
                            "original_sha256": sha256_bytes(original)}
    return list(results.values())




def build(name: str, matches_path: Path = MATCHES, fresh: bool = False, jobs: int | None = None,
          linker_model: str = inplace.DEFAULT_LINKER_MODEL,
          literal_model: str = inplace.DEFAULT_LITERAL_MODEL, *, bounded: bool = False,
          names_path: Path | None = None) -> dict:
    out = ROOT / "build" / name
    try:
        out.mkdir(parents=True, exist_ok=False)
    except FileExistsError:
        raise BuildError(f"build/{name} already exists; build records are immutable") from None
    if names_path is not None and not bounded:
        raise BuildError("proposed names are allowed only for bounded fixture comparisons")
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
    if names_path is not None:
        names = matcher.merge_names(names, image_mod.load_names(names_path))
    resolver = inplace.Resolver(functions, names)
    linker = inplace.InPlaceLinker(image, functions, resolver, image.bss, linker_model, literal_model)
    matches = json.loads(matches_path.read_text(encoding="utf-8"))
    profiles = toolchain.load_profiles()
    compile_options = {"distinct_paths": True} if linker.model.closed_u1_composition else {}
    hits = compile_units(matches["units"], profiles, out, fresh or linker.model.per_unit_u1,
                         jobs or os.cpu_count() or 1, **compile_options)

    hybrid = None if bounded else bytearray(image.code)
    covered_spans = []
    credited: dict[int, str] = {}
    mismatched: list[str] = []
    occupied: list[tuple[int, int, str, tuple[str, str] | None]] = []  # sections placed by all units
    claims: dict[int, str] = {}
    for unit in matches["units"]:
        for fname in unit["functions"]:
            address = resolver.resolve(fname).address
            if address in claims:
                raise BuildError(f"{fname}: credited by both {claims[address]} and {unit['source']}")
            claims[address] = unit["source"]
    definitions: dict[str, str] = {}  # global (non-COMDAT) definition name -> defining unit
    function_definitions: dict[int, str] = {}  # credited stable identity -> emitted defining unit
    provenance = {"input_kind": "object"} if linker.model.single_object_u1 else {}
    objects = [inplace.ObjectFile(unit_object_path(out, unit["source"], linker.model.closed_u1_composition), **provenance)
               for unit in matches["units"]]
    # Inspect the original objects before combining/linking can coalesce COMDAT copies. Removing
    # an old credit claim must never hide an emitted body, even under another bound symbol name.
    for unit, objfile in zip(matches["units"], objects):
        for sym in objfile.symbols:
            if sym.kind not in ("STT_FUNC", "STT_OBJECT", "STT_NOTYPE") or sym.shndx not in objfile.sections:
                continue
            target = resolver.lookup(sym.name)
            if target is None or target.address not in claims:
                continue
            previous = function_definitions.get(target.address)
            if previous is not None and previous != unit["source"]:
                raise BuildError(f"func_{target.address:08x}: defined by both {previous} and {unit['source']}")
            function_definitions[target.address] = unit["source"]
        for sym in objfile.defined_globals():
            if objfile.sections[sym.shndx].comdat:
                continue  # unrelated COMDAT identities retain the existing overlap rules
            if sym.name in definitions:
                raise BuildError(f"{sym.name}: defined by both {definitions[sym.name]} and {unit['source']}")
            definitions[sym.name] = unit["source"]
    for address, source in function_definitions.items():
        if source != claims[address]:
            raise BuildError(f"func_{address:08x}: defined by {source}, not credited owner {claims[address]}")
    plans = [{"source": unit["source"], "removed": set(), "members": [], "actions": []}
             for unit in matches["units"]]
    if linker.model.per_unit_u1:
        objects, plans = prepare_unit_objects(objects, matches["units"], image, functions, resolver,
                                              linker.model, literal_model)
    combined = inplace.ObjectFile.combine(objects, [unit["source"] for unit in matches["units"]])
    all_linked = linker.link(combined)
    if linker.model.per_unit_u1 and linker.removed_special_members:
        raise BuildError("aggregate linking must not decide U1")
    units = []
    for owner, unit in enumerate(matches["units"]):
        src = ROOT / unit["source"]
        profile = profiles[unit["profile"]]
        obj = unit_object_path(out, unit["source"], linker.model.closed_u1_composition)
        linked = {index: value for index, value in all_linked.items() if combined.section_owners[index] == owner}
        witness_rows = u1_witness_comparisons(plans[owner], linked, functions, image) if linker.model.per_unit_u1 else []
        compile_inputs = {}
        if linker.model.per_unit_u1:
            dependencies = obj.with_suffix(".d")
            if not dependencies.is_file():
                raise BuildError("per-unit U1 build requires the fresh compiler dependency manifest")
            headers = toolchain._parse_depend(dependencies.read_text(encoding="utf-8"), src)
            if any(not header.is_file() for header in headers):
                raise BuildError("per-unit U1 dependency header is unavailable")
            compile_inputs = {"dependency_manifest_sha256": sha256_file(dependencies),
                              "headers_sha256": {header.as_posix(): sha256_file(header) for header in sorted(set(headers))}}
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
            if bounded:
                covered_spans.append({"function": fname, "source": unit["source"], "address": address,
                                      "size": fn["size"], "sha256": sha256_bytes(data),
                                      "original_sha256": sha256_bytes(image.read(address, fn["size"])),
                                      "match": data == image.read(address, fn["size"])})
            else:
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
            **compile_inputs,
            "linker_rewrites": rewrites,
            "literal_rebindings": [row for row in linker.literal_rebindings
                                   if combined.section_owners[row["section_index"]] == owner],
            "removed_callees": [row for row in linker.removed_callees
                                if row.get("unit", row["object"]) == unit["source"]],
            **({"u1_witness_comparisons": witness_rows} if witness_rows else {}),
            **({"removed_special_members": plans[owner]["members"],
                "linker_section_rewrites": plans[owner]["actions"]}
               if plans[owner]["members"] else {}),
            **({"u1_authorization_count": len(plans[owner]["members"]),
                "u1_witness_comparisons": witness_rows,
                "removed_special_members": plans[owner]["members"],
                "linker_section_rewrites": plans[owner]["actions"]}
               if linker.model.per_unit_u1 else {}),
            **({
                "removed_special_members": [row for row in linker.removed_special_members
                                            if combined.section_owners[row["section_index"]] == owner],
                "linker_section_rewrites": [w.record() for w in linker.rewrites
                                            if isinstance(w, inplace.SectionElimination)
                                            and combined.section_owners[w.section] == owner],
            } if any(combined.section_owners[row["section_index"]] == owner
                     for row in linker.removed_special_members) else {}),
        })
    try:
        inplace.reject_overlaps(occupied)
    except inplace.LinkError as e:
        raise BuildError(f"units place overlapping sections: {e}") from None

    if bounded:
        outputs = {"covered_spans": covered_spans, "covered_bytes": sum(row["size"] for row in covered_spans)}
        checks = {"credited_functions_exact": not mismatched,
                  "covered_spans_identical": all(row["match"] for row in covered_spans)}
    else:
        code = bytes(hybrid)
        packed = ctrfmt.blz_compress(code)
        stored = (ROOT / "orig/exefs/code.blz").read_bytes()
        outputs = {"code_bin_sha256": sha256_bytes(code), "exefs_code_sha256": sha256_bytes(packed)}
        checks = {"credited_functions_exact": not mismatched, "code_bin_identical": code == image.code,
                  "exefs_code_identical": packed == stored}
        (out / "code.bin").write_bytes(code)
        (out / "code.blz").write_bytes(packed)

    used = sorted({u["compiler"] for u in units})
    receipt = {
        "schema_version": 2,
        "name": name,
        "created_utc": datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "host": f"{platform.system()}-{platform.machine()}",
        "fresh": fresh or linker.model.per_unit_u1,
        "record_kind": "build-receipt",
        "mode": "bounded" if bounded else "full",
        "inputs": {
            "code_bin_sha256": image.sha256,
            **({"exefs_code_sha256": sha256_bytes(stored)} if not bounded else {}),
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
            **({"u1_stage": "per-unit-direct-before-aggregation"} if linker.model.per_unit_u1 else {}),
            **({"resolver_sha256": sha256_bytes(json.dumps(names, sort_keys=True).encode())}
               if linker.model.per_unit_u1 else {}),
            **({"proposed_names": {"path": names_path.as_posix(), "sha256": sha256_file(names_path)}}
               if names_path is not None else {}),
        },
        "units": units,
        "outputs": outputs,
        "covered" if bounded else "credited": {
            "functions": len(credited),
            "bytes": sum(functions[a]["size"] for a in credited),
            # same C denominator as ci/progress_report.py: classified kinds (veneers) excluded
            "catalogue_functions": sum(1 for f in functions.values() if image_mod.creditable(f)),
            "catalogue_bytes": sum(f["size"] for f in functions.values() if image_mod.creditable(f)),
        },
        "checks": checks,
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
    parser.add_argument("--bounded", action="store_true",
                        help="metadata-only fixture comparison; no hybrid image, recompression or output payload")
    parser.add_argument("--names", type=Path,
                        help="proposed names overlay for --bounded only; canonical names remain acceptance authority")
    parser.add_argument("--linker-model", default=inplace.DEFAULT_LINKER_MODEL, choices=sorted(inplace.LINKER_MODELS),
                        help="armlink output optimisations reproduced by the in-place link (docs/LINKER.md)")
    parser.add_argument("--literal-model", default=inplace.DEFAULT_LITERAL_MODEL, choices=inplace.LITERAL_MODELS,
                        help="independent anonymous string placement rule; default off (docs/LINKER.md)")
    args = parser.parse_args(argv)
    try:
        receipt = build(args.name, args.matches, args.fresh, args.jobs, args.linker_model, args.literal_model,
                        bounded=args.bounded, names_path=args.names)
    except (BuildError, inplace.LinkError, toolchain.ToolchainError, image_mod.ImageError) as e:
        print(f"error: {e}", file=sys.stderr)
        return 2
    c = receipt["covered"] if args.bounded else receipt["credited"]
    print(f"{receipt['result']}: {c['functions']}/{c['catalogue_functions']} functions, "
          f"{c['bytes']}/{c['catalogue_bytes']} bytes {'covered' if args.bounded else 'credited'}; checks {receipt['checks']}")
    print(f"receipt: build/{args.name}/receipt.json")
    return 0 if receipt["result"] == "PASS" else 1


if __name__ == "__main__":
    raise SystemExit(main())
