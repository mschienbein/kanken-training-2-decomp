"""Parsed-ELF regressions for sealed v9/v10 review F01–F05 and v11 positive controls."""
from __future__ import annotations

import io
import json
import struct
import sys
import tempfile
import unittest
from contextlib import ExitStack, redirect_stderr, redirect_stdout
from pathlib import Path
from unittest.mock import patch

from tests import test_inplace as seeds
from tests.elf_fixture import serialize_object

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import collect
import compid
import inplace
import match
import toolchain

MODEL = "armlink5-inline-branchnop-v11"


def parse_object(obj: inplace.ObjectFile) -> inplace.ObjectFile:
    return inplace.ObjectFile(Path("independent-synthetic.o"), data=serialize_object(obj))


def checked_linker(memory, catalogue, names, **kwargs):
    return inplace.InPlaceLinker(memory, catalogue, inplace.Resolver(catalogue, names),
                                 (0x200000, 0x201000), MODEL, **kwargs)


class U1CheckedElfTest(unittest.TestCase):
    def fixture(self, form="extab", group=False):
        return seeds.UnreferencedDestructorUnwindModelTest().fixture(form, group)

    def refuse(self, obj, memory, catalogue, names, pattern="."):
        linker = checked_linker(memory, catalogue, names)
        with self.assertRaisesRegex(inplace.LinkError, pattern):
            linker.link(obj)
        self.assertEqual(linker.removed_special_members, [])
        self.assertEqual(linker.rewrites, [])

    def test_f01_all_zero_sized_allocated_dependency_sources_participate_in_census(self):
        for target in (2, 3, 4):
            for retained in (False, True):
                for kind in ("data", "code", "nobits", "link_order"):
                    with self.subTest(target=target, retained=retained, kind=kind):
                        obj, memory, catalogue, names = self.fixture()
                        symbol = inplace.Symbol("target", 0, 0, "STT_SECTION", "STB_LOCAL", target)
                        obj.symbols.append(symbol)
                        obj.sections[5] = inplace.Section(
                            5, ".zero.dependency", b"", 0, kind == "nobits", kind == "code", False,
                            [] if kind == "link_order" else [inplace.Reloc(0, inplace.R_ARM_NONE, symbol)],
                            retention=("GNU retain",) if retained else (), link_order=kind == "link_order",
                            linked_section=target if kind == "link_order" else 0)
                        parsed = parse_object(obj)
                        self.assertNotIn(5, parsed.sections)
                        self.assertIn(5, parsed.dependency_sections)
                        if retained:
                            self.assertTrue(parsed.dependency_sections[5].retention)
                        self.refuse(parsed, memory, catalogue, names)

    def test_f01_zero_sized_dependency_without_own_unwind_also_vetoes_d0(self):
        obj, memory, catalogue, names = seeds.UnreferencedDestructorModelTest().fixture()
        obj.sections[3] = inplace.Section(
            3, ".zero.root", b"", 0, False, False, False,
            [inplace.Reloc(0, inplace.R_ARM_NONE, obj.symbols[2])], retention=("GNU retain",))
        self.refuse(parse_object(obj), memory, catalogue, names, "still referenced")

    def test_f01_zero_source_relocations_and_owners_survive_both_combination_orders(self):
        for target in (2, 3, 4):
            for reverse in (False, True):
                for source_kind in ("relocation", "link_order"):
                    obj, memory, catalogue, names = self.fixture()
                    symbol = inplace.Symbol("target", 0, 0, "STT_SECTION", "STB_LOCAL", target)
                    obj.symbols.append(symbol)
                    obj.sections[5] = inplace.Section(
                        5, ".zero.dependency", b"", 0, False, False, False,
                        [inplace.Reloc(0, inplace.R_ARM_NONE, symbol)] if source_kind == "relocation" else [],
                        retention=("GNU retain",), link_order=source_kind == "link_order",
                        linked_section=target if source_kind == "link_order" else 0)
                    other = inplace.ObjectFile.__new__(inplace.ObjectFile)
                    other.sections = {1: inplace.Section(1, ".unplaced", b"data", 4, False, False, False)}
                    other.symbols, other.section_groups = [], []
                    objects, labels = [parse_object(obj), parse_object(other)], ["destructor.cpp", "other.cpp"]
                    if reverse:
                        objects.reverse()
                        labels.reverse()
                    combined = inplace.ObjectFile.combine(objects, labels)
                    witness, = combined.dependency_sections.values()
                    self.assertEqual(combined.section_objects[witness.index], "destructor.cpp")
                    destination = (witness.relocs[0].symbol.shndx if witness.relocs else witness.linked_section)
                    self.assertEqual(combined.section_objects[destination], "destructor.cpp")
                    self.refuse(combined, memory, catalogue, names)

    def test_f01_separate_zero_source_external_dependency_resolves_to_supplied_d0(self):
        for reverse in (False, True):
            obj, memory, catalogue, names = self.fixture()
            symbol = inplace.Symbol(seeds.UnreferencedDestructorModelTest.D0, 0, 0,
                                    "STT_NOTYPE", "STB_GLOBAL", "SHN_UNDEF")
            other = inplace.ObjectFile.__new__(inplace.ObjectFile)
            other.sections = {1: inplace.Section(1, ".zero.external", b"", 0, False, False, False,
                                                [inplace.Reloc(0, inplace.R_ARM_NONE, symbol)], retention=("KEEP",))}
            other.symbols, other.section_groups = [symbol], []
            objects = [parse_object(obj), parse_object(other)]
            if reverse:
                objects.reverse()
            self.refuse(inplace.ObjectFile.combine(objects), memory, catalogue, names, "still referenced")

    def test_f01_root_and_group_membership_of_zero_sized_witnesses_is_preserved(self):
        for root_kind in ("flag", "tagsym", "explicit", "unrooted"):
            obj, memory, catalogue, names = self.fixture(group=True)
            obj.sections[5] = inplace.Section(5, ".zero.member", b"", 0, False, False, False, comdat="D0_group",
                                             retention=("GNU retain",) if root_kind == "flag" else ())
            obj.section_groups[0][2].add(5)
            if root_kind == "tagsym":
                obj.symbols.append(inplace.Symbol("__tagsym$$used", 0, 0, "STT_NOTYPE", "STB_LOCAL", 5))
            parsed = parse_object(obj)
            roots = {".zero.member": "KEEP"} if root_kind == "explicit" else {}
            linker = checked_linker(memory, catalogue, names, retention_roots=roots)
            with self.assertRaisesRegex(inplace.LinkError, "retained|group"):
                linker.link(parsed)
        obj, memory, catalogue, names = self.fixture()
        obj.sections[5] = inplace.Section(5, ".zero.unrelated.root", b"", 0, False, False, False,
                                         retention=("GNU retain",))
        parsed = parse_object(obj)
        self.assertEqual(set(checked_linker(memory, catalogue, names).link(parsed)), {1})

    def test_f01_zero_source_prevents_r1_proof_before_u1_can_rely_on_that_removal(self):
        obj, memory, catalogue, names = self.fixture()
        caller, original, functions = seeds.RemovedCalleeModelTest().fixture()
        caller.symbols[0].name = "func_00100100"
        memory.write(seeds.BASE + 0x100, original.read(seeds.BASE, 40))
        catalogue[seeds.BASE + 0x100] = dict(functions[seeds.BASE], address=seeds.BASE + 0x100)
        caller.sections[5] = inplace.Section(
            5, ".zero.r1.dependency", b"", 0, False, False, False,
            [inplace.Reloc(0, inplace.R_ARM_NONE, caller.symbols[1])], retention=("KEEP",))
        combined = inplace.ObjectFile.combine([parse_object(obj), parse_object(caller)])
        self.refuse(combined, memory, catalogue, names, "still referenced")

    def test_f01_live_reference_preflight_precedes_placement_witness_consumption(self):
        obj, memory, catalogue, names = self.fixture()
        symbol = inplace.Symbol("target", 0, 0, "STT_SECTION", "STB_LOCAL", 2)
        obj.symbols.append(symbol)
        obj.sections[5] = inplace.Section(5, ".live.table", bytes(4), 4, False, False, False,
                                         [inplace.Reloc(0, inplace.R_ARM_ABS32, symbol)])
        linker = checked_linker(memory, catalogue, names)
        with patch.object(linker, "place", side_effect=AssertionError("placement must not run")):
            with self.assertRaisesRegex(inplace.LinkError, "still referenced"):
                linker.link(parse_object(obj))

    def test_f02_unaligned_arm_and_thumb_mapping_markers_and_incomplete_ranges_refuse(self):
        for marker, offset in (("$a", 1), ("$a", 2), ("$t", 1), ("$t", 3), ("$d", 5)):
            obj, memory, catalogue, names = self.fixture()
            obj.sections[2].mapping.append((offset, marker))
            parsed = parse_object(obj)
            self.refuse(parsed, memory, catalogue, names, "mapping")
        obj, memory, catalogue, names = self.fixture()
        obj.sections[2].data += b"x"
        obj.sections[2].size += 1
        obj.sections[2].mapping = [(0, "$a")]
        obj.symbols = [symbol for symbol in obj.symbols if not (symbol.shndx == 2 and symbol.name == "$d")]
        self.refuse(parse_object(obj), memory, catalogue, names, "mapping")

    def test_f02_marker_symbol_metadata_and_power_of_two_section_alignment_refuse(self):
        for case in ("global", "sized", "wrong_type", "duplicate", "bad_alignment", "function_size"):
            obj, memory, catalogue, names = self.fixture()
            marker = next(symbol for symbol in obj.symbols if symbol.shndx == 2 and symbol.name == "$a")
            if case == "global":
                marker.bind = "STB_GLOBAL"
            elif case == "sized":
                marker.size = 4
            elif case == "wrong_type":
                marker.kind = "STT_OBJECT"
            elif case == "duplicate":
                obj.symbols.append(inplace.replace(marker))
            elif case == "function_size":
                obj.symbols[2].size = 7
            else:
                obj.sections[2].align = 6
            self.refuse(parse_object(obj), memory, catalogue, names, "mapping|section metadata")

    def test_f02_valid_aligned_arm_and_thumb_ranges_and_byte_aligned_data_are_supported(self):
        for thumb in (False, True):
            obj, memory, catalogue, names = self.fixture()
            if thumb:
                obj.symbols[2].value = 1
                obj.symbols[4].name = "$t"
                obj.sections[2].mapping = [(0, "$t"), (8, "$d")]
            parsed = parse_object(obj)
            linker = checked_linker(memory, catalogue, names)
            self.assertEqual(set(linker.link(parsed)), {1})
            self.assertEqual(len(linker.removed_special_members[0]["unwind_sections"]), 2)
        obj, memory, catalogue, names = self.fixture()
        obj.symbols[2].value, obj.symbols[2].size = 1, 2
        obj.symbols[4].name, obj.symbols[5].value = "$t", 2
        obj.sections[2].mapping = [(0, "$t"), (2, "$d")]
        self.assertEqual(set(checked_linker(memory, catalogue, names).link(parse_object(obj))), {1})

    def copies(self, target_kind="local", same_bytes=False):
        first, memory, catalogue, names = self.fixture(group=True)
        second, _, _, _ = self.fixture(group=True)
        for serial, obj in enumerate((first, second)):
            obj.sections[1].comdat = "D1_group"
            obj.section_groups.append((1, "D1_group", {1}))
            symbol = inplace.Symbol("payload", 0, 4, "STT_OBJECT", "STB_LOCAL", 5)
            if target_kind in ("external", "shared_definition"):
                symbol = inplace.Symbol("payload", 0, 0, "STT_OBJECT", "STB_GLOBAL", "SHN_UNDEF")
            elif target_kind == "absolute":
                symbol.shndx, symbol.value = "SHN_ABS", seeds.TABLE_AT + 0x100
            obj.symbols.append(symbol)
            if target_kind == "local":
                obj.sections[5] = inplace.Section(
                    5, ".data.local", struct.pack("<I", 1 if same_bytes else serial + 1), 4, False, False, True)
            obj.sections[2].data = obj.sections[2].data[:-4] + bytes(4)
            obj.sections[2].relocs.append(inplace.Reloc(8, inplace.R_ARM_ABS32, symbol))
        if target_kind == "shared_definition":
            symbol = first.symbols[-1]
            symbol.shndx, symbol.size = 5, 4
            first.sections[5] = inplace.Section(5, ".shared", b"data", 4, False, False, True)
            names["payload"] = {"address": seeds.TABLE_AT + 0x100}
            memory.write(seeds.TABLE_AT + 0x100, b"data")
        return [parse_object(first), parse_object(second)], memory, catalogue, names

    def test_f05_different_local_relocation_targets_refuse_in_both_orders_even_with_equal_bytes(self):
        for same_bytes in (False, True):
            for reverse in (False, True):
                objects, memory, catalogue, names = self.copies(same_bytes=same_bytes)
                if reverse:
                    objects.reverse()
                self.refuse(inplace.ObjectFile.combine(objects), memory, catalogue, names, "relocation targets")

    def test_f05_external_absolute_and_actually_shared_definition_targets_are_provable(self):
        for target_kind in ("external", "absolute", "shared_definition"):
            for reverse in (False, True):
                objects, memory, catalogue, names = self.copies(target_kind)
                if reverse:
                    objects.reverse()
                linker = checked_linker(memory, catalogue, names)
                linker.link(inplace.ObjectFile.combine(objects))
                self.assertEqual(len(linker.removed_special_members), 2)

    def test_f05_changed_target_binding_kind_size_and_section_metadata_refuse(self):
        for case in ("binding", "kind", "size", "absolute_value", "alignment"):
            objects, memory, catalogue, names = self.copies("absolute")
            symbol = next(symbol for symbol in objects[1].symbols if symbol.name == "payload")
            if case == "binding":
                symbol.bind = "STB_GLOBAL"
            elif case == "kind":
                symbol.kind = "STT_NOTYPE"
            elif case == "size":
                symbol.size += 4
            elif case == "absolute_value":
                symbol.value += 4
            else:
                objects[1].sections[2].align = 8
            self.refuse(inplace.ObjectFile.combine(objects), memory, catalogue, names, "copies|targets")

    def test_f05_contradictory_d0_symbol_metadata_refuses_in_both_orders(self):
        for case in ("binding", "function_extent", "section_symbol_extent"):
            for reverse in (False, True):
                objects, memory, catalogue, names = self.copies("absolute")
                symbol = next(symbol for symbol in objects[1].symbols
                              if symbol.kind == ("STT_SECTION" if case == "section_symbol_extent" else "STT_FUNC")
                              and symbol.shndx == 2)
                if case == "binding":
                    symbol.bind = "STB_WEAK"
                else:
                    symbol.size = 4 if case == "function_extent" else objects[1].sections[2].size
                if reverse:
                    objects.reverse()
                self.refuse(inplace.ObjectFile.combine(objects), memory, catalogue, names, "copies")

    def test_f05_every_group_copy_has_agreeing_companion_bytes_metadata_and_closure(self):
        for case in ("agree", "different_extab", "different_inline", "missing_companions"):
            for reverse in (False, True):
                first, memory, catalogue, names = self.fixture(group=True)
                second, _, _, _ = self.fixture(group=True)
                if case == "different_extab":
                    second.sections[4].data = struct.pack("<2I", 0x80B0B0B1, 0)
                elif case == "different_inline":
                    first, memory, catalogue, names = self.fixture("cant", group=True)
                    second, _, _, _ = self.fixture("inline", group=True)
                elif case == "missing_companions":
                    second.sections.pop(3)
                    second.sections.pop(4)
                    second.symbols.pop()
                    second.section_groups[0][2].difference_update({3, 4})
                for obj in (first, second):
                    obj.sections[1].comdat = "D1_group"
                    obj.section_groups.append((1, "D1_group", {1}))
                objects = [parse_object(first), parse_object(second)]
                if reverse:
                    objects.reverse()
                combined = inplace.ObjectFile.combine(objects)
                if case == "agree":
                    linker = checked_linker(memory, catalogue, names)
                    linker.link(combined)
                    self.assertEqual(len(linker.removed_special_members), 2)
                else:
                    self.refuse(combined, memory, catalogue, names, "copies|copy")

    def test_positive_controls_and_old_models_do_not_acquire_v11_rules(self):
        for model in ("armlink5-inline-branchnop-v6", "armlink5-inline-branchnop-v8",
                      "armlink5-inline-branchnop-v9", "armlink5-inline-branchnop-v10"):
            self.assertFalse(inplace.linker_model(model).checked_destructor_removal)
        previous, selected = inplace.linker_model("armlink5-inline-branchnop-v10"), inplace.linker_model(MODEL)
        for key, value in vars(previous).items():
            if key not in ("name", "checked_destructor_removal"):
                self.assertEqual(getattr(selected, key), value)
        for form in ("cant", "inline", "extab"):
            obj, memory, catalogue, names = self.fixture(form)
            self.assertEqual(set(checked_linker(memory, catalogue, names).link(parse_object(obj))), {1})


class U1CheckedReportingTest(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.obj, self.memory, self.catalogue, self.names = seeds.UnreferencedDestructorUnwindModelTest().fixture("extab")
        self.memory.bss = (0x200000, 0x201000)
        self.memory.sha256 = "independent-synthetic-image"
        self.profile = toolchain.Profile("game", "synthetic", ())
        for path, text in {
            "candidate.cpp": "// independent synthetic compiler input\n",
            "extra.cpp": "// independent synthetic helper input\n",
            "header.h": "// independent synthetic header input\n",
            "config/names.json": json.dumps({"symbols": self.names}),
            "overlay.json": json.dumps({"symbols": {"extra_binding": {"address": seeds.TABLE_AT + 0x100}}}),
            "config/compiler_profiles.json": json.dumps({"profiles": {"game": {"compiler": "synthetic", "flags": []}}}),
            "profiles.json": json.dumps({"profiles": {"trial": {"compiler": "synthetic", "flags": ["-O2"]}}}),
            "config/catalogue.json": json.dumps({"image_sha256": self.memory.sha256,
                                                "functions": list(self.catalogue.values())}),
            "orig/exheader.bin": "synthetic layout hash input",
            "config/title.json": "synthetic title hash input",
            "compiler.exe": "synthetic compiler hash input",
        }.items():
            dest = self.root / path
            dest.parent.mkdir(parents=True, exist_ok=True)
            dest.write_text(text, encoding="utf-8")
        stack = ExitStack()
        self.addCleanup(stack.close)
        for owner, name, value in (
                (match.image_mod, "ROOT", self.root),
                (match.image_mod, "NAMES", self.root / "config/names.json"),
                (toolchain, "PROFILES", self.root / "config/compiler_profiles.json")):
            stack.enter_context(patch.object(owner, name, value))
        stack.enter_context(patch.object(match.image_mod.Image, "load", return_value=self.memory))
        stack.enter_context(patch.object(toolchain, "armcc_exe", return_value=self.root / "compiler.exe"))
        self.compiler = stack.enter_context(patch.object(toolchain, "compile_source", side_effect=self.compile_source))
        self.output = self.root / "out"

    def compile_source(self, profile, source, output, **kwargs):
        output.parent.mkdir(parents=True, exist_ok=True)
        obj = self.obj
        if source.name == "extra.cpp":
            obj = inplace.ObjectFile.__new__(inplace.ObjectFile)
            obj.sections, obj.symbols, obj.section_groups = {}, [], []
        output.write_bytes(serialize_object(obj))
        output.with_suffix(".d").write_text(f"{output}: {source}\n{output}: {self.root / 'header.h'}\n", encoding="utf-8")
        return False

    def context(self, model=MODEL):
        return match.Context(self.root / "config/catalogue.json", model, self.root / "overlay.json")

    def test_f03_matcher_binds_canonical_overlay_profile_header_object_and_harness_inputs(self):
        with redirect_stdout(io.StringIO()):
            code = match.run(self.root / "candidate.cpp", self.profile, self.root / "config/catalogue.json",
                             self.output, 0, MODEL, self.root / "overlay.json", [self.root / "extra.cpp"],
                             profiles_path=self.root / "profiles.json")
        self.assertEqual(code, 0)
        record = json.loads((self.output / "candidate.result.json").read_text())
        for key, path in (("names_sha256", "config/names.json"), ("profiles_sha256", "config/compiler_profiles.json"),
                          ("exheader_sha256", "orig/exheader.bin"), ("title_sha256", "config/title.json")):
            self.assertEqual(record[key], match.sha256(self.root / path))
        self.assertEqual(record["proposed_names"]["sha256"], match.sha256(self.root / "overlay.json"))
        self.assertEqual(record["proposed_profiles"]["sha256"], match.sha256(self.root / "profiles.json"))
        self.assertEqual(record["headers_sha256"], {str(self.root / "header.h"): match.sha256(self.root / "header.h")})
        self.assertEqual(set(record["harness_sha256"]), set(match.HARNESS))
        self.assertEqual(record["harness_sha256"]["tools/inplace.py"], match.sha256(match.ROOT / "tools/inplace.py"))
        self.assertEqual(record["effective_profile"], {"name": "game", "compiler": "synthetic", "flags": []})
        self.assertEqual(record["object_sha256"], match.sha256(self.output / "candidate.o"))
        self.assertEqual(record["extra_inputs"][0]["source_sha256"], match.sha256(self.root / "extra.cpp"))
        self.assertEqual(record["extra_inputs"][0]["object_sha256"], match.sha256(self.output / "extra-0-extra.o"))
        self.assertTrue(all(call.kwargs.get("cache") is False for call in self.compiler.call_args_list))
        self.assertEqual(len(record["linker_rewrites"]), 1)
        self.assertEqual(len(record["removed_special_members"][0]["unwind_sections"]), 2)

    def test_f03_canonical_bindings_actually_used_and_their_hash_changes(self):
        first = self.context()
        names_path = self.root / "config/names.json"
        names_path.write_text(json.dumps({"symbols": {**self.names, seeds.UnreferencedDestructorModelTest.D0:
                                                    {"address": seeds.TABLE_AT + 0x100}}}))
        second = self.context()
        self.assertNotEqual(first.decision_inputs["names_sha256"], second.decision_inputs["names_sha256"])
        self.assertNotEqual(first.decision_inputs["resolver_sha256"], second.decision_inputs["resolver_sha256"])
        self.compile_source(self.profile, self.root / "candidate.cpp", self.output / "candidate.o")
        with self.assertRaises(inplace.LinkError):
            second.compare(self.output / "candidate.o")
        self.assertEqual(second.removed_special_members, [])

    def test_f03_missing_dependency_manifest_or_header_refuses_instead_of_publishing_unbound_action(self):
        context = self.context()
        source, obj = self.root / "candidate.cpp", self.output / "candidate.o"
        self.compile_source(self.profile, source, obj)
        obj.with_suffix(".d").unlink()
        with self.assertRaisesRegex(inplace.LinkError, "dependency manifest"):
            context.evidence_inputs(self.profile, source, obj)
        obj.with_suffix(".d").write_text(f"{obj}: {self.root / 'missing.h'}\n")
        with self.assertRaisesRegex(inplace.LinkError, "dependency header"):
            context.evidence_inputs(self.profile, source, obj)

    def test_f03_unbound_inputs_refuse_before_any_success_is_reported(self):
        def missing_dependencies(profile, source, output, **kwargs):
            self.compile_source(profile, source, output, **kwargs)
            output.with_suffix(".d").unlink()

        with patch.object(toolchain, "compile_source", side_effect=missing_dependencies), \
                redirect_stdout(io.StringIO()) as output:
            with self.assertRaisesRegex(inplace.LinkError, "dependency manifest"):
                match.run(self.root / "candidate.cpp", self.profile, self.root / "config/catalogue.json",
                          self.output, 0, MODEL)
        self.assertNotIn("MATCH", output.getvalue())
        self.assertNotIn("REMOVED", output.getvalue())
        self.assertFalse((self.output / "candidate.result.json").exists())

    def test_f04_compid_keeps_each_pair_source_action_companions_and_hash_closure(self):
        context = self.context()
        source = self.root / "candidate.cpp"
        grid = {"compilers": ["synthetic"], "flagsets": {"one": [], "two": ["-O2"]}}
        result = compid.run_grid(context, [source], grid, self.output)
        self.assertEqual(result["matching_all"], ["synthetic:one", "synthetic:two"])
        for pair in result["matching_all"]:
            actions = result["linker_section_rewrites"][str(source)][pair]
            member, = result["removed_special_members"][str(source)][pair]
            self.assertEqual(actions[0]["removed_special_member"], member)
            self.assertEqual(len(member["unwind_sections"]), 2)
            inputs = result["comparison_inputs"][str(source)][pair]
            self.assertEqual(inputs["names_sha256"], match.sha256(self.root / "config/names.json"))
            self.assertEqual(inputs["object_sha256"], match.sha256(Path(inputs["object"])))
        self.assertEqual(result["linker_rewrites"], {})
        with patch.object(compid.match, "Context", return_value=context), patch.object(compid, "run_grid", return_value=result), \
                redirect_stdout(io.StringIO()):
            code = compid.main([str(source), "--linker-model", MODEL, "--out", str(self.output / "main")])
        self.assertEqual(code, 0)
        record = json.loads((self.output / "main/compid.json").read_text())
        for key in ("linker_section_rewrites", "removed_special_members", "comparison_inputs"):
            self.assertEqual(record[key], result[key])

    def test_f04_historical_u1_models_also_report_section_actions(self):
        for model in ("armlink5-inline-branchnop-v9", "armlink5-inline-branchnop-v10"):
            if model.endswith("-v9"):
                self.obj = seeds.UnreferencedDestructorModelTest().fixture()[0]
            else:
                self.obj = seeds.UnreferencedDestructorUnwindModelTest().fixture("extab")[0]
            result = compid.run_grid(self.context(model), [self.root / "candidate.cpp"],
                                     {"compilers": ["synthetic"], "flagsets": {"one": []}},
                                     self.root / model)
            self.assertEqual(result["matching_all"], ["synthetic:one"])
            member, = result["removed_special_members"][str(self.root / "candidate.cpp")]["synthetic:one"]
            self.assertEqual(result["linker_section_rewrites"][str(self.root / "candidate.cpp")]["synthetic:one"][0]
                             ["removed_special_member"], member)

    def test_f04_same_stem_sources_keep_distinct_objects_in_comparison_closure(self):
        sources = [self.root / directory / "candidate.cpp" for directory in ("a", "b")]
        for source in sources:
            source.parent.mkdir()
            source.write_text("// independent probe\n", encoding="utf-8")
        result = compid.run_grid(self.context(), sources,
                                 {"compilers": ["synthetic"], "flagsets": {"one": []}}, self.output)
        objects = [result["comparison_inputs"][str(source)]["synthetic:one"]["object"] for source in sources]
        self.assertEqual(len(set(objects)), 2)
        for source, obj in zip(sources, objects):
            inputs = result["comparison_inputs"][str(source)]["synthetic:one"]
            self.assertEqual(inputs["object_sha256"], match.sha256(Path(obj)))
            self.assertEqual(inputs["source_sha256"], match.sha256(source))

    def test_f04_collector_refuses_u1_before_creating_unreported_candidates(self):
        for model in ("armlink5-inline-branchnop-v9", "armlink5-inline-branchnop-v10", MODEL):
            candidates = self.root / "unreported.json"
            with redirect_stderr(io.StringIO()) as error:
                code = collect.main(["missing-result.json", "--dest", "unused", "--candidates", str(candidates),
                                     "--linker-model", model])
            self.assertEqual(code, 2)
            self.assertIn("section-action receipt", error.getvalue())
            self.assertFalse(candidates.exists())

    def test_f04_comparison_failure_clears_previous_context_actions(self):
        context = self.context()
        path = self.output / "candidate.o"
        self.compile_source(self.profile, self.root / "candidate.cpp", path)
        context.compare(path)
        self.assertEqual(len(context.section_rewrites), 1)
        symbol = inplace.Symbol("target", 0, 0, "STT_SECTION", "STB_LOCAL", 2)
        self.obj.symbols.append(symbol)
        self.obj.sections[5] = inplace.Section(5, ".zero.live", b"", 0, False, False, False,
                                             [inplace.Reloc(0, inplace.R_ARM_NONE, symbol)], retention=("KEEP",))
        self.compile_source(self.profile, self.root / "candidate.cpp", path)
        with self.assertRaises(inplace.LinkError):
            context.compare(path)
        self.assertEqual((context.section_rewrites, context.removed_special_members), ([], []))

    def test_v11_synthetic_build_keeps_full_action_ownership_and_zero_extra_credit(self):
        from tests.test_build import BuildRemovedCalleeModelTest

        receipt, root = BuildRemovedCalleeModelTest.synthetic_build(self, model=MODEL, unreferenced="unwind")
        self.assertEqual(receipt["result"], "PASS")
        self.assertEqual(receipt["inputs"]["linker_model"], MODEL)
        self.assertEqual((receipt["credited"]["functions"], receipt["credited"]["bytes"]), (1, 4))
        owner = next(unit for unit in receipt["units"] if unit["source"] == "src/caller.c")
        member, = owner["removed_special_members"]
        self.assertEqual(owner["linker_section_rewrites"][0]["removed_special_member"], member)
        self.assertEqual({row["object"] for row in member["unwind_sections"]}, {"src/caller.c"})
        self.assertEqual([row["kind"] for row in member["unwind_sections"]], ["exidx", "extab"])
        self.assertEqual(owner["object_sha256"], match.sha256(root / "build/synthetic-removed/obj/src/caller.o"))
