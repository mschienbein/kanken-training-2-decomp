"""V14 namespace audit regressions: all supplied definitions precede R1/U1 decisions."""
from __future__ import annotations

import io
import struct
import sys
import unittest
from pathlib import Path
from unittest.mock import patch

from tests import test_inplace as seeds
from tests.elf_fixture import serialize_object
from tests.test_unreferenced_v12 import replace_header

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import inplace
import match

MODEL = "armlink5-inline-branchnop-v14"
STORAGES = ("progbits", "nobits", "nonalloc", "absolute", "common")
BINDINGS = ("STB_GLOBAL", "STB_WEAK", "STB_LOCAL")
KINDS = ("STT_FUNC", "STT_OBJECT", "STT_NOTYPE", "STT_SECTION")


def parse(blob):
    return inplace.ObjectFile(Path("synthetic-namespace.o"), data=blob)


def definition(storage, bind="STB_GLOBAL", kind="STT_OBJECT", size=0, name="target", symbol_size=None):
    obj = inplace.ObjectFile.__new__(inplace.ObjectFile)
    obj.sections, obj.section_groups = {}, []
    if storage in ("absolute", "common"):
        shndx = "SHN_ABS" if storage == "absolute" else "SHN_COMMON"
        value = 0x110000 if storage == "absolute" else 4
    else:
        shndx, value = 1, 0
        obj.sections[1] = inplace.Section(1, ".supplied.definition", b"" if storage == "nobits" else bytes(size),
                                         size, storage == "nobits", False, storage == "nobits",
                                         elf_type="SHT_NOBITS" if storage == "nobits" else "SHT_PROGBITS")
    obj.symbols = [inplace.Symbol(name, value, size if symbol_size is None else symbol_size, kind, bind, shndx)]
    return serialize_object(obj, headers={1: {"flags": 0}} if storage == "nonalloc" else None)


def reference(name="target", bind="STB_GLOBAL", kind="STT_NOTYPE"):
    obj = inplace.ObjectFile.__new__(inplace.ObjectFile)
    symbol = inplace.Symbol(name, 0, 0, kind, bind, "SHN_UNDEF")
    obj.sections = {1: inplace.Section(1, ".zero.reference", b"", 0, False, False, False,
                                      [inplace.Reloc(0, inplace.R_ARM_NONE, symbol)])}
    obj.symbols, obj.section_groups = [symbol], []
    return serialize_object(obj)


def marker_unit(form="extab", personality=0, r1=False, **changes):
    if r1:
        obj, memory, catalogue = seeds.RemovedUnwindModelTest().fixture(form)
        names = {}
    else:
        obj, memory, catalogue, names = seeds.UnreferencedDestructorUnwindModelTest().fixture(form)
    if form == "extab":
        obj.sections[4].data = struct.pack("<2I", (0x80 + personality) << 24, 0)
    marker = inplace.Symbol(f"__aeabi_unwind_cpp_pr{personality}", 0, 0, "STT_NOTYPE", "STB_GLOBAL", "SHN_UNDEF")
    for key, value in changes.items():
        setattr(marker, key, value)
    obj.symbols.append(marker)
    obj.sections[3 if form == "inline" else 4].relocs.append(inplace.Reloc(0, inplace.R_ARM_NONE, marker))
    memory.bss = (0x200000, 0x201000)
    return obj, memory, catalogue, names


def all_relocations(obj):
    return [relocation for section in (obj.sections | obj.dependency_sections).values() for relocation in section.relocs]


class CompleteNamespaceTest(unittest.TestCase):
    def test_inventory_and_resolution_cover_every_storage_binding_kind_size_and_order(self):
        for storage in STORAGES:
            for bind in BINDINGS:
                for kind in KINDS:
                    for size in (0, 4):
                        blob = definition(storage, bind, kind, size)
                        parsed = parse(blob)
                        defined, = [symbol for symbol in parsed.defined_symbols() if symbol.name == "target"]
                        self.assertEqual((defined.kind, defined.bind, defined.size), (kind, bind, size))
                        self.assertNotEqual(defined.shndx, "SHN_UNDEF")
                        for reverse in (False, True):
                            with self.subTest(storage=storage, bind=bind, kind=kind, size=size, reverse=reverse):
                                objects = [parse(reference()), parsed]
                                if reverse:
                                    objects.reverse()
                                combined = inplace.ObjectFile.combine(objects)
                                if storage == "common":
                                    with self.assertRaisesRegex(inplace.LinkError, "COMMON"):
                                        combined.resolve_definitions()
                                    continue
                                resolved = combined.resolve_definitions()
                                self.assertEqual(len(resolved.definition_namespace["target"]), 1)
                                edge, = all_relocations(resolved)
                                if bind == "STB_LOCAL":
                                    self.assertEqual(edge.symbol.shndx, "SHN_UNDEF")
                                else:
                                    self.assertIs(edge.symbol, resolved.definition_namespace["target"][0])
                                    self.assertNotEqual(edge.symbol.shndx, "SHN_UNDEF")
                                self.assertEqual(edge.reference.shndx, "SHN_UNDEF")

    def test_symbol_size_zero_is_distinct_from_a_zero_sized_section(self):
        for storage in ("progbits", "nobits", "nonalloc"):
            parsed = parse(definition(storage, size=4, symbol_size=0))
            symbol, = [symbol for symbol in parsed.defined_symbols() if symbol.name == "target"]
            self.assertEqual(symbol.size, 0)
            if storage != "nonalloc":
                self.assertEqual(parsed.sections[symbol.shndx].size, 4)
            resolved = inplace.ObjectFile.combine([parse(reference()), parsed]).resolve_definitions()
            self.assertNotEqual(all_relocations(resolved)[0].symbol.shndx, "SHN_UNDEF")

    def test_local_definitions_remain_scoped_and_global_resolution_is_order_independent(self):
        for reverse in (False, True):
            objects = [parse(definition("progbits", "STB_LOCAL", size=0)),
                       parse(definition("nobits", "STB_GLOBAL", size=4)), parse(reference())]
            if reverse:
                objects.reverse()
            combined = inplace.ObjectFile.combine(objects)
            resolved = combined.resolve_definitions()
            self.assertEqual(len(resolved.definition_namespace["target"]), 2)
            edge, = all_relocations(resolved)
            self.assertEqual(edge.symbol.bind, "STB_GLOBAL")
            self.assertNotEqual(resolved.symbol_owners[id(edge.symbol)], resolved.symbol_owners[id(edge.reference)])
        two_locals = inplace.ObjectFile.combine([parse(definition("progbits", "STB_LOCAL", size=4)),
                                                parse(definition("progbits", "STB_LOCAL", size=4)), parse(reference())])
        resolved = two_locals.resolve_definitions()
        self.assertEqual(all_relocations(resolved)[0].symbol.shndx, "SHN_UNDEF")
        self.assertEqual(len({resolved.symbol_owners[id(symbol)] for symbol in resolved.definition_namespace["target"]}), 2)

    def test_undefined_local_cannot_borrow_a_global_or_hide_behind_legacy_combination(self):
        combined = inplace.ObjectFile.combine([parse(reference(bind="STB_LOCAL")),
                                               parse(definition("progbits", size=4))])
        edge, = all_relocations(combined)
        self.assertEqual(edge.reference.bind, "STB_LOCAL")
        self.assertEqual(edge.reference.shndx, "SHN_UNDEF")
        with self.assertRaisesRegex(inplace.LinkError, "undefined local"):
            combined.resolve_definitions()

    def test_nested_combination_preserves_reference_provenance_and_local_scopes(self):
        first = inplace.ObjectFile.combine([parse(reference()), parse(definition("nobits", size=0))])
        nested = inplace.ObjectFile.combine([first, parse(definition("progbits", "STB_LOCAL", name="private"))])
        original_edge, = all_relocations(nested)
        resolved = nested.resolve_definitions()
        edge, = all_relocations(resolved)
        self.assertNotEqual(edge.symbol.shndx, "SHN_UNDEF")
        self.assertEqual(edge.reference.shndx, "SHN_UNDEF")
        self.assertIs(all_relocations(nested)[0], original_edge)
        self.assertEqual(len(set(resolved.symbol_owners.values())), 3)

    def test_global_weak_duplicates_refuse_without_an_order_selected_winner(self):
        for first_bind, second_bind in (("STB_GLOBAL", "STB_GLOBAL"), ("STB_GLOBAL", "STB_WEAK"),
                                        ("STB_WEAK", "STB_GLOBAL"), ("STB_WEAK", "STB_WEAK")):
            for storage in ("progbits", "nobits", "nonalloc", "common"):
                for reverse in (False, True):
                    objects = [parse(definition(storage, first_bind)), parse(definition(storage, second_bind))]
                    if reverse:
                        objects.reverse()
                    with self.assertRaises(inplace.LinkError):
                        inplace.ObjectFile.combine(objects).resolve_definitions()
        same = parse(definition("absolute"))
        self.assertIn("target", inplace.ObjectFile.combine([same, same]).resolve_definitions().definition_namespace)

    def test_abs_target_uses_its_value_not_a_names_overlay_and_common_refuses(self):
        parsed = parse(definition("absolute", kind="STT_FUNC"))
        symbol, = [symbol for symbol in parsed.symbols if symbol.name == "target"]
        oracle = inplace.InPlaceLinker(None, {}, inplace.Resolver({}, {"target": {"address": 0x120000}}),
                                       (0, 0), MODEL)
        self.assertEqual(oracle._symbol_target(symbol, parsed, {}).address, 0x110000)
        with self.assertRaisesRegex(inplace.LinkError, "COMMON"):
            parse(definition("common")).resolve_definitions()

    def test_defined_extents_must_fit_even_when_a_catalogue_body_would_match(self):
        for storage in ("progbits", "nobits"):
            with self.assertRaisesRegex(inplace.LinkError, "extent"):
                parse(definition(storage, size=0, symbol_size=4)).resolve_definitions()
        unit, memory, catalogue, names = seeds.UnreferencedDestructorModelTest().fixture()
        unit.symbols[0].size = unit.sections[1].size + 4
        oracle = inplace.InPlaceLinker(memory, catalogue, inplace.Resolver(catalogue, names),
                                       (0x200000, 0x201000), MODEL)
        with self.assertRaisesRegex(inplace.LinkError, "extent"):
            oracle.link(parse(serialize_object(unit)))
        self.assertEqual((oracle.rewrites, oracle.removed_special_members), ([], []))

    def test_reference_kind_conflicts_refuse_and_notype_resolution_is_explicit(self):
        for reference_kind, defined_kind in (("STT_FUNC", "STT_OBJECT"), ("STT_SECTION", "STT_FUNC")):
            combined = inplace.ObjectFile.combine([parse(reference(kind=reference_kind)),
                                                   parse(definition("progbits", kind=defined_kind, size=4))])
            with self.assertRaisesRegex(inplace.LinkError, "kind disagrees"):
                combined.resolve_definitions()
        resolved = inplace.ObjectFile.combine([parse(reference()), parse(definition("progbits", kind="STT_FUNC", size=4))]).resolve_definitions()
        self.assertEqual(all_relocations(resolved)[0].symbol.kind, "STT_FUNC")

    def test_unsupported_supplied_symbol_kinds_refuse_after_global_binding(self):
        for kind in ("STT_FILE", "STT_TLS", "STT_COMMON"):
            with self.assertRaisesRegex(inplace.LinkError, "symbol kind"):
                inplace.ObjectFile.combine([parse(reference()), parse(definition("absolute", kind=kind))]).resolve_definitions()


class PersonalityNamespaceTest(unittest.TestCase):
    def reject(self, objects, memory, catalogue, names, pattern="."):
        oracle = inplace.InPlaceLinker(memory, catalogue, inplace.Resolver(catalogue, names), memory.bss, MODEL)
        with self.assertRaisesRegex(inplace.LinkError, pattern):
            oracle.link(inplace.ObjectFile.combine(objects) if len(objects) > 1 else objects[0])
        self.assertEqual((oracle.rewrites, oracle.removed_special_members, oracle.removed_callees), ([], [], []))

    def test_every_supplied_personality_definition_denies_the_exception(self):
        for r1 in (False, True):
            for form, personality in (("inline", 0), ("extab", 0), ("extab", 1), ("extab", 2)):
                unit, memory, catalogue, names = marker_unit(form, personality, r1)
                original = serialize_object(unit)
                name = f"__aeabi_unwind_cpp_pr{personality}"
                for storage in STORAGES:
                    for bind in BINDINGS:
                        for kind in KINDS:
                            for size in (0, 4):
                                supplied = definition(storage, bind, kind, size, name)
                                for reverse in (False, True):
                                    with self.subTest(r1=r1, form=form, personality=personality, storage=storage,
                                                      bind=bind, kind=kind, size=size, reverse=reverse):
                                        objects = [parse(original), parse(supplied)]
                                        self.reject(list(reversed(objects)) if reverse else objects,
                                                    memory, catalogue, names)

    def test_single_object_definitions_are_checked_without_extra_source_dispatch(self):
        for r1 in (False, True):
            for bind in BINDINGS:
                unit, memory, catalogue, names = marker_unit(r1=r1)
                name = "__aeabi_unwind_cpp_pr0"
                unit.sections[5] = inplace.Section(5, ".zero.supplied", b"", 0, False, False, False)
                unit.symbols.append(inplace.Symbol(name, 0, 0, "STT_NOTYPE", bind, 5))
                self.reject([parse(serialize_object(unit))], memory, catalogue, names)

    def test_external_marker_requires_supported_binding_kind_value_size_visibility(self):
        bad = ({"bind": "STB_LOCAL"}, {"kind": "STT_OBJECT"}, {"kind": "STT_SECTION"},
               {"value": 4}, {"size": 4}, {"visibility": "STV_HIDDEN"}, {"visibility": "STV_INTERNAL"},
               {"visibility": "STV_PROTECTED"})
        for r1 in (False, True):
            for change in bad:
                unit, memory, catalogue, names = marker_unit(r1=r1, **change)
                self.reject([parse(serialize_object(unit))], memory, catalogue, names)
        for r1 in (False, True):
            for bind in ("STB_GLOBAL", "STB_WEAK"):
                for kind in ("STT_NOTYPE", "STT_FUNC"):
                    unit, memory, catalogue, names = marker_unit(r1=r1, bind=bind, kind=kind)
                    oracle = inplace.InPlaceLinker(memory, catalogue, inplace.Resolver(catalogue, names), memory.bss, MODEL)
                    linked = oracle.link(parse(serialize_object(unit)))
                    self.assertEqual(len(linked), 1)
                    self.assertEqual(len(oracle.removed_callees if r1 else oracle.removed_special_members), 1)

    def test_noncanonical_relocation_symbol_table_refuses_before_whitelist(self):
        unit, memory, catalogue, names = marker_unit()
        blob = serialize_object(unit)
        changed = replace_header(blob, ".rel.ARM.extab.D0", link=0)
        self.reject([parse(changed)], memory, catalogue, names, "symbol table")

    def test_additional_symbol_tables_refuse_as_an_incomplete_namespace(self):
        for table_type in (2, 11):
            unit, memory, catalogue, names = marker_unit()
            unit.sections[5] = inplace.Section(5, ".extra.symbols", bytes(32), 32, False, False, False)
            blob = serialize_object(unit, headers={5: {"flags": 0}})
            elf = inplace.ELFFile(io.BytesIO(blob))
            strings = elf.get_section_by_name(".strtab").data()
            name = strings.index(b"__aeabi_unwind_cpp_pr0\0")
            changed = replace_header(blob, ".extra.symbols", type=table_type, entsize=16,
                                     link=elf.get_section_index(".strtab"), info=1)
            start = elf.get_section_by_name(".extra.symbols")["sh_offset"] + 16
            data = bytearray(changed)
            struct.pack_into("<IIIBBH", data, start, name, 0, 0, 0x11, 0, 0xFFF1)
            self.reject([parse(bytes(data))], memory, catalogue, names, "symbol table")


class ResolutionConsumerTest(unittest.TestCase):
    def test_none_dependency_requires_verified_supplied_storage(self):
        for storage in ("progbits", "nobits", "nonalloc", "common"):
            unit, memory, catalogue, names = seeds.UnreferencedDestructorModelTest().fixture()
            memory.bss = (0x200000, 0x201000)
            external = inplace.Symbol("target", 0, 0, "STT_NOTYPE", "STB_GLOBAL", "SHN_UNDEF")
            unit.symbols.append(external)
            unit.sections[1].relocs.append(inplace.Reloc(0, inplace.R_ARM_NONE, external))
            for reverse in (False, True):
                objects = [parse(serialize_object(unit)), parse(definition(storage))]
                if reverse:
                    objects.reverse()
                oracle = inplace.InPlaceLinker(memory, catalogue, inplace.Resolver(catalogue, names), memory.bss, MODEL)
                with self.assertRaisesRegex(inplace.LinkError, "verified output placement|COMMON"):
                    oracle.link(inplace.ObjectFile.combine(objects))
                self.assertEqual(oracle.removed_special_members, [])
        for supplied in (False, True):
            unit, memory, catalogue, names = seeds.UnreferencedDestructorModelTest().fixture()
            memory.bss = (0x200000, 0x201000)
            external = inplace.Symbol("external_constant", 0, 0, "STT_NOTYPE", "STB_GLOBAL", "SHN_UNDEF")
            unit.symbols.append(external)
            unit.sections[1].relocs.append(inplace.Reloc(0, inplace.R_ARM_NONE, external))
            objects = [parse(serialize_object(unit))]
            if supplied:
                objects.append(parse(definition("absolute", name=external.name)))
            oracle = inplace.InPlaceLinker(memory, catalogue, inplace.Resolver(catalogue, names), memory.bss, MODEL)
            self.assertEqual(len(oracle.link(inplace.ObjectFile.combine(objects))), 1)

    def test_r1_cannot_drop_independent_data_file_or_global_section_aliases(self):
        for kind, bind in (("STT_OBJECT", "STB_LOCAL"), ("STT_NOTYPE", "STB_LOCAL"),
                           ("STT_SECTION", "STB_GLOBAL"), ("STT_FILE", "STB_GLOBAL")):
            unit, memory, catalogue = seeds.RemovedCalleeModelTest().fixture()
            unit.symbols.append(inplace.Symbol("independent_alias", 0, 0, kind, bind, 2))
            oracle = inplace.InPlaceLinker(memory, catalogue, inplace.Resolver(catalogue),
                                           (0x200000, 0x201000), MODEL)
            with self.assertRaises(inplace.LinkError):
                oracle.link(parse(serialize_object(unit)))
            self.assertEqual((oracle.rewrites, oracle.removed_callees), ([], []))
        unit, memory, catalogue = seeds.RemovedUnwindModelTest().fixture("extab", alias=True)
        oracle = inplace.InPlaceLinker(memory, catalogue, inplace.Resolver(catalogue), (0x200000, 0x201000), MODEL)
        self.assertEqual(len(oracle.link(parse(serialize_object(unit)))), 1)
        self.assertEqual(len(oracle.removed_callees), 1)

    def test_comdat_order_and_local_target_identity_keep_full_proof(self):
        from tests.test_unreferenced_v12 import fixture
        from tests.test_unreferenced_v11 import U1CheckedElfTest

        for reverse in (False, True):
            first, memory, catalogue, names = fixture()
            second, _, _, _ = fixture(prefix=True)
            objects = [parse(first), parse(second)]
            if reverse:
                objects.reverse()
            oracle = inplace.InPlaceLinker(memory, catalogue, inplace.Resolver(catalogue, names),
                                           (0x200000, 0x201000), MODEL)
            self.assertEqual(len(oracle.link(inplace.ObjectFile.combine(objects))), 2)
            self.assertEqual(len(oracle.removed_special_members), 2)
            objects, memory, catalogue, names = U1CheckedElfTest().copies(same_bytes=True)
            if reverse:
                objects.reverse()
            oracle = inplace.InPlaceLinker(memory, catalogue, inplace.Resolver(catalogue, names),
                                           (0x200000, 0x201000), MODEL)
            with self.assertRaisesRegex(inplace.LinkError, "relocation targets"):
                oracle.link(inplace.ObjectFile.combine(objects))
            self.assertEqual(oracle.removed_special_members, [])

    def test_section_and_named_entry_refs_share_proof_but_undefined_weak_does_not_guess_zero(self):
        for section_reference in (False, True):
            unit, memory, catalogue = seeds.RemovedUnwindModelTest().fixture("extab", alias=True)
            if section_reference:
                key = next(symbol for symbol in unit.symbols if symbol.kind == "STT_SECTION" and symbol.shndx == 2)
                unit.sections[1].relocs[-1].symbol = key
            oracle = inplace.InPlaceLinker(memory, catalogue, inplace.Resolver(catalogue), (0x200000, 0x201000), MODEL)
            self.assertEqual(len(oracle.link(parse(serialize_object(unit)))), 1)
            self.assertEqual(len(oracle.removed_callees), 1)
        oracle = inplace.InPlaceLinker(None, {}, inplace.Resolver({}), (0, 0), MODEL)
        weak = inplace.Symbol("missing_weak", 0, 0, "STT_FUNC", "STB_WEAK", "SHN_UNDEF")
        with self.assertRaisesRegex(inplace.LinkError, "unresolved symbol"):
            oracle._symbol_target(weak, parse(reference()), {})

    def test_reporting_and_explicit_immutable_selection(self):
        from tests import test_unreferenced_v12 as previous

        with patch.object(previous, "MODEL", MODEL):
            previous.CompleteMetadataReportingTest().test_matcher_grid_and_collector_dispatch_the_v12_selection()
        old, selected = inplace.linker_model("armlink5-inline-branchnop-v13"), inplace.linker_model(MODEL)
        for key, value in vars(old).items():
            if key not in ("name", "complete_symbol_resolution"):
                self.assertEqual(getattr(selected, key), value)
        self.assertTrue(selected.complete_symbol_resolution)
        self.assertTrue(all(not model.complete_symbol_resolution for name, model in inplace.LINKER_MODELS.items()
                            if name not in (MODEL, "armlink5-inline-branchnop-v15", "armlink5-inline-branchnop-v16",
                                            "armlink5-inline-branchnop-v17")))
        self.assertEqual(inplace.DEFAULT_LINKER_MODEL, "armlink5-inline-branchnop-v2")
