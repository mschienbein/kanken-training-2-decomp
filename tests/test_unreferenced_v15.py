"""V15 U1 is a single-direct-object proof; ledger owners never establish original identity."""
from __future__ import annotations

import copy
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from tests import test_inplace as seeds
from tests.elf_fixture import serialize_object

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import accept
import inplace
import match

MODEL = "armlink5-inline-branchnop-v15"


def parsed(unit, label="direct.o", kind="object"):
    return inplace.ObjectFile(Path(label), data=serialize_object(unit), input_kind=kind)


def witnesses(form="extab"):
    unit, memory, catalogue, names = seeds.UnreferencedDestructorUnwindModelTest().fixture(form)
    deleting, complete = copy.deepcopy(unit), copy.deepcopy(unit)
    deleting.sections = {index: section for index, section in deleting.sections.items() if index != 1}
    deleting.symbols = [symbol for symbol in deleting.symbols if symbol.shndx != 1]
    complete.sections = {1: complete.sections[1]}
    complete.symbols = [symbol for symbol in complete.symbols if symbol.shndx == 1]
    empty = inplace.ObjectFile.__new__(inplace.ObjectFile)
    empty.sections, empty.symbols, empty.section_groups = {}, [], []
    memory.bss = (0x200000, 0x201000)
    return unit, deleting, complete, empty, memory, catalogue, names


def r1_inputs():
    whole, memory, catalogue = seeds.RemovedUnwindModelTest().fixture("extab")
    caller, helper = copy.deepcopy(whole), copy.deepcopy(whole)
    original = whole.symbols[1]
    external = inplace.Symbol(original.name, 0, 0, "STT_FUNC", "STB_GLOBAL", "SHN_UNDEF")
    caller.sections = {1: caller.sections[1]}
    caller.symbols = [symbol for symbol in caller.symbols if symbol.shndx == 1] + [external]
    for relocation in caller.sections[1].relocs:
        if relocation.symbol.shndx == 2:
            relocation.symbol = external
    helper.sections = {index: section for index, section in helper.sections.items() if index != 1}
    helper.symbols = [symbol for symbol in helper.symbols if symbol.shndx != 1]
    memory.bss = (0x200000, 0x201000)
    return caller, helper, memory, catalogue


class SingleDirectObjectScopeTest(unittest.TestCase):
    def oracle(self, memory, catalogue, names):
        return inplace.InPlaceLinker(memory, catalogue, inplace.Resolver(catalogue, names), memory.bss, MODEL)

    def refuse(self, obj, memory, catalogue, names):
        oracle = self.oracle(memory, catalogue, names)
        with self.assertRaisesRegex(inplace.LinkError, "U1 refused by scope"):
            oracle.link(obj)
        self.assertEqual((oracle.rewrites, oracle.removed_special_members, oracle.removed_callees), ([], [], []))

    def test_review_six_nested_loans_and_both_flat_orders_refuse_by_scope(self):
        unit, deleting, complete, empty, memory, catalogue, names = witnesses()
        for reverse in (False, True):
            for mode in ("flat", "nested-once", "nested-extra", "nested-twice"):
                objects = [parsed(deleting, "D0.o"), parsed(complete, "D1.o")]
                labels = ["D0.cpp", "D1.cpp"]
                if reverse:
                    objects.reverse()
                    labels.reverse()
                combined = inplace.ObjectFile.combine(objects, labels)
                if mode != "flat":
                    combined = inplace.ObjectFile.combine([combined], ["bundle.o"])
                if mode == "nested-extra":
                    combined = inplace.ObjectFile.combine([combined, parsed(empty, "empty.o")], ["bundle.o", "empty.cpp"])
                if mode == "nested-twice":
                    combined = inplace.ObjectFile.combine([combined], ["bundle2.o"])
                with self.subTest(reverse=reverse, mode=mode):
                    self.refuse(combined, memory, catalogue, names)
        direct = parsed(unit, "same-original.o")
        oracle = self.oracle(memory, catalogue, names)
        self.assertEqual(len(oracle.link(direct)), 1)
        self.assertEqual(oracle.removed_special_members[0]["object"], "same-original.o")

    def test_one_element_wrappers_and_unrelated_empty_inputs_do_not_expand_scope(self):
        unit, _, _, empty, memory, catalogue, names = witnesses()
        for mode in ("one-wrapper", "complete-plus-empty", "empty-plus-complete", "nested-complete"):
            direct, other = parsed(unit, "direct.o"), parsed(empty, "empty.o")
            if mode == "one-wrapper":
                combined = inplace.ObjectFile.combine([direct], ["direct.cpp"])
            elif mode == "complete-plus-empty":
                combined = inplace.ObjectFile.combine([direct, other])
            elif mode == "empty-plus-complete":
                combined = inplace.ObjectFile.combine([other, direct])
            else:
                combined = inplace.ObjectFile.combine([inplace.ObjectFile.combine([direct])])
            self.refuse(combined, memory, catalogue, names)

    def test_archive_bundle_and_ambiguous_inputs_refuse_without_owner_label_inference(self):
        unit, _, _, _, memory, catalogue, names = witnesses()
        for kind in ("archive_member", "bundle", "ambiguous"):
            self.refuse(parsed(unit, "normal-looking.o", kind), memory, catalogue, names)
        for label in ("library.a", "library.a(member.o)", "library.a:member.o", "library.lib/member.o", "assets.zip!/member.o"):
            self.refuse(parsed(unit, label), memory, catalogue, names)
        unspecified = inplace.ObjectFile(Path("unknown.o"), data=serialize_object(unit))
        self.refuse(unspecified, memory, catalogue, names)

    def test_missing_and_mixed_real_identities_refuse_even_with_equal_paths_and_ledger_owners(self):
        unit, _, _, _, memory, catalogue, names = witnesses()
        for changed in ("missing-origin", "missing-sections", "missing-symbol", "foreign-section", "foreign-symbol"):
            direct, other = parsed(unit, "same-name.o"), parsed(unit, "same-name.o")
            self.assertIsNot(direct.origin, other.origin)
            direct.section_owners = {index: 0 for index in direct.sections}
            if changed == "missing-origin":
                direct.origin = None
            elif changed == "missing-sections":
                direct.section_origins = {}
            elif changed == "missing-symbol":
                direct.symbol_origins.pop(id(direct.symbols[-1]))
            elif changed == "foreign-section":
                direct.section_origins[2] = other.origin
            else:
                direct.symbol_origins[id(direct.symbols[-1])] = other.origin
            self.refuse(direct, memory, catalogue, names)

    def test_single_direct_forms_and_personality_namespace_remain_positive(self):
        for form in ("cant", "inline", "extab"):
            unit, _, _, _, memory, catalogue, names = witnesses(form)
            direct = parsed(unit)
            oracle = self.oracle(memory, catalogue, names)
            self.assertEqual(len(oracle.link(direct)), 1)
            member, = oracle.removed_special_members
            self.assertEqual(member["object"], "direct.o")
            self.assertEqual(member["input_kind"], "object")
            self.assertTrue(all(row["object"] == "direct.o" for row in member["unwind_sections"]))
            self.assertTrue(all(origin is direct.origin for origin in direct.section_origins.values()))
            self.assertTrue(all(origin is direct.origin for origin in direct.symbol_origins.values()))
        from tests.test_unreferenced_v14 import marker_unit
        for form, personality in (("inline", 0), ("extab", 0), ("extab", 1), ("extab", 2)):
            unit, memory, catalogue, names = marker_unit(form, personality)
            oracle = self.oracle(memory, catalogue, names)
            self.assertEqual(len(oracle.link(parsed(unit))), 1)

    def test_scope_refusal_precedes_witness_namespace_and_placement_consumption(self):
        unit, _, _, empty, memory, catalogue, names = witnesses()
        combined = inplace.ObjectFile.combine([parsed(unit), parsed(empty)])
        oracle = self.oracle(memory, catalogue, names)
        with patch.object(combined, "resolve_definitions", side_effect=AssertionError("namespace must not run")), \
                patch.object(oracle, "_unreferenced_destructors", side_effect=AssertionError("witness must not run")), \
                patch.object(oracle, "place", side_effect=AssertionError("placement must not run")):
            with self.assertRaisesRegex(inplace.LinkError, "U1 refused by scope"):
                oracle.link(combined)

    def test_matcher_requires_compile_attestation_and_refuses_every_extra_object(self):
        unit, _, _, empty, memory, catalogue, names = witnesses()
        with tempfile.TemporaryDirectory() as directory:
            path, extra = Path(directory) / "direct.o", Path(directory) / "empty.o"
            path.write_bytes(serialize_object(unit))
            extra.write_bytes(serialize_object(empty))
            context = match.Context.__new__(match.Context)
            context.image, context.functions, context.resolver = memory, catalogue, inplace.Resolver(catalogue, names)
            context.model, context.literal_model = inplace.linker_model(MODEL), "none"
            with self.assertRaisesRegex(inplace.LinkError, "U1 refused by scope"):
                context.compare(path)
            self.assertEqual([row["status"] for row in context.compare(path, compiled_object=True)], ["match"])
            with self.assertRaisesRegex(inplace.LinkError, "U1 refused by scope"):
                context.compare(path, [extra], compiled_object=True)
            self.assertEqual((context.section_rewrites, context.removed_special_members), ([], []))

    def test_compiler_match_grid_and_collector_dispatch_use_narrow_scope(self):
        from tests import test_unreferenced_v12 as previous
        with patch.object(previous, "MODEL", MODEL):
            previous.CompleteMetadataReportingTest().test_matcher_grid_and_collector_dispatch_the_v12_selection()

    def test_v15_is_distinct_and_only_it_selects_the_narrowed_policy(self):
        previous, selected = inplace.linker_model("armlink5-inline-branchnop-v14"), inplace.linker_model(MODEL)
        for key, value in vars(previous).items():
            if key not in ("name", "single_object_u1"):
                self.assertEqual(getattr(selected, key), value)
        self.assertTrue(selected.single_object_u1)
        self.assertTrue(all(not model.single_object_u1 for name, model in inplace.LINKER_MODELS.items()
                            if name not in (MODEL, "armlink5-inline-branchnop-v16")))
        self.assertEqual(inplace.DEFAULT_LINKER_MODEL, "armlink5-inline-branchnop-v2")


class R1OriginalProvenanceTest(unittest.TestCase):
    def test_r1_flat_nested_and_archive_inputs_keep_linking_proof_and_real_defining_identity(self):
        for reverse in (False, True):
            for nested in (False, True):
                for helper_kind in ("object", "archive_member"):
                    caller, helper, memory, catalogue = r1_inputs()
                    objects = [parsed(caller, "caller.o"), parsed(helper, "helper.o", helper_kind)]
                    labels = ["src/caller.cpp", "src/helper.cpp"]
                    if reverse:
                        objects.reverse()
                        labels.reverse()
                    combined = inplace.ObjectFile.combine(objects, labels)
                    if nested:
                        combined = inplace.ObjectFile.combine([combined], ["src/bundle.cpp"])
                    oracle = inplace.InPlaceLinker(memory, catalogue, inplace.Resolver(catalogue), memory.bss, MODEL)
                    linked = oracle.link(combined)
                    self.assertEqual(len(linked), 1)
                    for placed, data in linked.values():
                        self.assertEqual(data, memory.read(placed.base, len(data)))
                    helper_row, = oracle.removed_callees
                    self.assertEqual(helper_row["object"], "helper.o")
                    self.assertEqual(helper_row["unit"], "src/bundle.cpp" if nested else "src/helper.cpp")
                    self.assertEqual(helper_row["input_kind"], helper_kind)
                    self.assertTrue(all(row["object"] == "helper.o" for row in helper_row["unwind_sections"]))
                    self.assertEqual(oracle.removed_special_members, [])

    def test_revised_helper_review_uses_ledger_unit_not_original_elf_path(self):
        caller, helper, memory, catalogue = r1_inputs()
        combined = inplace.ObjectFile.combine([parsed(caller, "caller.o"), parsed(helper, "helper.o")],
                                             ["src/caller.cpp", "src/helper.cpp"])
        oracle = inplace.InPlaceLinker(memory, catalogue, inplace.Resolver(catalogue), memory.bss, MODEL)
        oracle.link(combined)
        rows = [rewrite.record() for rewrite in oracle.rewrites]
        receipt = {"units": [{"source": "src/caller.cpp", "linker_rewrites": {"func_00100000": rows}},
                             {"source": "src/helper.cpp", "removed_callees": oracle.removed_callees}]}
        _, dependencies = accept.linker_review_summary(receipt, {"src/helper.cpp"}, [])
        self.assertEqual(dependencies["func_00100000"]["revised_helpers"], ["src/helper.cpp"])
        selected = [row for row in oracle.removed_callees if row.get("unit", row["object"]) == "src/helper.cpp"]
        self.assertEqual(len(selected), 1)
