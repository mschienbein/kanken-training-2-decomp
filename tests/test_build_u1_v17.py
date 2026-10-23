"""V17 all-copy U1 composition and positive full-build receipt boundaries."""
from __future__ import annotations

import copy
import io
import json
import struct
import sys
import types
import unittest
from pathlib import Path
from unittest.mock import patch

from tests import test_build_u1_v16 as fixture_module
from tests import test_inplace as seeds
from tests.elf_fixture import serialize_object
from tests import test_accept as accept_tests

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import accept
import build
import inplace

MODEL = "armlink5-inline-branchnop-v17"


class ClosedCompositionTest(unittest.TestCase):
    def setUp(self):
        self.model_patch = patch.object(fixture_module, "MODEL", MODEL)
        self.model_patch.start()
        self.addCleanup(self.model_patch.stop)
        self.fixture = fixture_module.PerUnitU1BuildTest()
        self.fixture.setUp()
        self.addCleanup(self.fixture.doCleanups)

    def grouped_foreign(self, kind, different=False):
        f = self.fixture
        for index in (2, 3, 4):
            f.owner.sections[index].comdat = "D0_group"
        f.owner.section_groups = [(1, "D0_group", {2, 3, 4})]
        signature = "unrelated_group" if different else "D0_group"
        if kind == "kept-code":
            address = seeds.BASE + 0x100
            f.functions[address] = {"address": address, "size": 4, "isa": "arm"}
            f.catalogue["functions"].append(f.functions[address])
            f.memory.write(address, seeds.BX_LR)
            f.empty.sections = {1: inplace.Section(1, "i.func_00100100", seeds.BX_LR, 4, False, True, False,
                                                  mapping=[(0, "$a")], align=4, comdat=signature)}
            f.empty.symbols = [inplace.Symbol("func_00100100", 0, 4, "STT_FUNC", "STB_GLOBAL", 1)]
            f.units[1]["functions"] = ["func_00100100"]
        else:
            size = 0 if kind == "zero-member" else 4
            f.empty.sections = {1: inplace.Section(1, ".foreign.group.member", bytes(size), size,
                                                  False, False, False, comdat=signature)}
            f.empty.symbols = [inplace.Symbol("foreign", 0, 0, "STT_SECTION", "STB_LOCAL", 1)]
        f.empty.section_groups = [(1, signature, {1})]

    def test_review_six_same_signature_kept_data_and_zero_members_refuse(self):
        for kind in ("kept-code", "unplaced-data", "zero-member"):
            for reverse in (False, True):
                f = fixture_module.PerUnitU1BuildTest()
                f.setUp()
                old = self.fixture
                self.fixture = f
                try:
                    self.grouped_foreign(kind)
                    if reverse:
                        f.units.reverse()
                    with self.assertRaisesRegex(build.BuildError, "COMDAT copy"):
                        f.run_build(bounded=True)
                    self.assertFalse((f.root / "build/fixture/receipt.json").exists())
                finally:
                    self.fixture = old
                    f.doCleanups()

    def test_different_signatures_remain_positive_in_both_orders(self):
        self.grouped_foreign("unplaced-data", different=True)
        for reverse in (False, True):
            if reverse:
                self.fixture.units.reverse()
            receipt = self.fixture.run_build(name=f"unrelated-{reverse}", bounded=True)
            self.assertEqual(receipt["result"], "PASS")
            self.assertEqual(sum(len(unit["removed_special_members"]) for unit in receipt["units"]), 1)

    def test_unknown_foreign_group_semantics_and_unrepresented_members_refuse(self):
        self.grouped_foreign("unplaced-data")
        for flags in (0, 3):
            self.fixture.empty.section_groups = [(flags, "D0_group", {1})]
            with self.assertRaisesRegex(build.BuildError, "COMDAT copy"):
                self.fixture.run_build(name=f"flags-{flags}", bounded=True)
        self.fixture.empty.section_groups = [(1, "D0_group", {1})]
        original_compile = self.fixture.compile_source

        def bad_group(profile, source, output, **kwargs):
            original_compile(profile, source, output, **kwargs)
            if source.stem == "other":
                data = bytearray(output.read_bytes())
                elf = inplace.ELFFile(io.BytesIO(data))
                group = elf.get_section_by_name(".group.D0_group")
                struct.pack_into("<I", data, group["sh_offset"] + 4, 0xFFFF)
                output.write_bytes(data)
            return False

        with patch.object(build.toolchain, "compile_source", side_effect=bad_group):
            with self.assertRaisesRegex(build.BuildError, "unrepresented group"):
                self.fixture.run_build(name="unrepresented", bounded=True)

    def test_all_closed_copies_require_equal_complete_code_and_companion_metadata(self):
        f = self.fixture
        for delta in (None, "code", "extab"):
            first, memory, functions, names = seeds.UnreferencedDestructorUnwindModelTest().fixture("extab", True)
            second = copy.deepcopy(first)
            if delta == "code":
                second.sections[2].data = second.sections[2].data[:-4] + b"diff"
            elif delta == "extab":
                second.sections[4].data = struct.pack("<2I", 0x80B0B0B1, 0)
            for obj in (first, second):
                obj.sections[1].comdat = "D1_group"
                obj.section_groups.append((1, "D1_group", {1}))
            memory.bss = (0x200000, 0x201000)
            for reverse in (False, True):
                objects = [inplace.ObjectFile(Path(label), data=serialize_object(obj), input_kind="object")
                           for label, obj in (("first.o", first), ("second.o", second))]
                units = [{"source": "first.cpp"}, {"source": "second.cpp"}]
                if reverse:
                    objects.reverse(); units.reverse()
                args = (objects, units, memory, functions, inplace.Resolver(functions, names), inplace.linker_model(MODEL), "none")
                if delta:
                    with self.assertRaisesRegex(inplace.LinkError, "contradictory"):
                        build.prepare_unit_objects(*args)
                else:
                    _, plans = build.prepare_unit_objects(*args)
                    self.assertEqual([len(plan["members"]) for plan in plans], [1, 1])

    def test_same_stem_c_and_cpp_units_keep_distinct_compiled_inputs(self):
        f = self.fixture
        (f.root / "src/owner.c").write_text("// independent empty C fixture\n")
        f.units[1]["source"] = "src/owner.c"
        original_compile = f.compile_source

        def separate(profile, source, output, **kwargs):
            saved = f.objects["owner"]
            if source.suffix == ".c":
                f.objects["owner"] = f.empty
            try:
                return original_compile(profile, source, output, **kwargs)
            finally:
                f.objects["owner"] = saved

        with patch.object(build.toolchain, "compile_source", side_effect=separate):
            receipt = f.run_build(bounded=True)
        self.assertEqual(receipt["result"], "PASS")
        self.assertTrue((f.root / "build/fixture/obj/src/owner.cpp.o").exists())
        self.assertTrue((f.root / "build/fixture/obj/src/owner.c.o").exists())
        self.assertNotEqual(receipt["units"][0]["object_sha256"], receipt["units"][1]["object_sha256"])


class PositiveFullEvidenceTest(unittest.TestCase):
    def setUp(self):
        self.model_patch = patch.object(fixture_module, "MODEL", MODEL)
        self.model_patch.start()
        self.addCleanup(self.model_patch.stop)
        self.fixture = fixture_module.PerUnitU1BuildTest()
        self.fixture.setUp()
        self.addCleanup(self.fixture.doCleanups)

    def test_bounded_omission_relabel_and_builtin_summary_never_become_full(self):
        bounded = self.fixture.run_build(bounded=True)
        variants = {"bounded": bounded, "summary": accept.build_summary(bounded)}
        no_mode = copy.deepcopy(bounded); no_mode.pop("mode")
        variants["no-mode"] = no_mode
        full_mode = copy.deepcopy(bounded); full_mode["mode"] = "full"
        variants["full-label-only"] = full_mode
        summary_no_mode = accept.build_summary(bounded); summary_no_mode.pop("mode")
        variants["summary-no-mode"] = summary_no_mode
        for label, value in variants.items():
            self.assertFalse(accept.build_receipt_scope_valid(value, MODEL), label)
            self.assertFalse(accept.build_receipt_scope_valid(value, MODEL, allow_legacy_full=True), label)
        self.assertEqual(variants["summary"]["mode"], "bounded")
        self.assertEqual(variants["summary"]["record_kind"], "build-summary")

    def test_full_receipt_requires_each_positive_image_scope_and_unit_field(self):
        full = self.fixture.run_build()
        self.assertTrue(accept.build_receipt_scope_valid(full, MODEL))
        self.assertFalse(accept.build_receipt_scope_valid(accept.build_summary(full), MODEL))
        for field in ("mode", "record_kind", "schema_version", "units", "credited", "checks", "outputs", "inputs"):
            changed = copy.deepcopy(full); changed.pop(field)
            self.assertFalse(accept.build_receipt_scope_valid(changed, MODEL), field)
        for container, field in (("outputs", "exefs_code_sha256"), ("outputs", "code_bin_sha256"),
                                 ("inputs", "exefs_code_sha256"), ("checks", "exefs_code_identical"),
                                 ("checks", "code_bin_identical"), ("checks", "credited_functions_exact")):
            changed = copy.deepcopy(full); changed[container].pop(field)
            self.assertFalse(accept.build_receipt_scope_valid(changed, MODEL), container + ":" + field)
        for field in ("linker_model", "literal_model", "catalogue_sha256", "profiles_sha256",
                      "toolchain_lock_sha256", "exheader_sha256", "matches_sha256", "harness_sha256", "compilers"):
            changed = copy.deepcopy(full); changed["inputs"].pop(field)
            self.assertFalse(accept.build_receipt_scope_valid(changed, MODEL), field)
        for field in ("source_sha256", "object_sha256", "profile", "compiler", "flags",
                      "removed_special_members", "linker_section_rewrites", "u1_witness_comparisons",
                      "u1_authorization_count", "dependency_manifest_sha256", "headers_sha256"):
            changed = copy.deepcopy(full); changed["units"][0].pop(field)
            self.assertFalse(accept.build_receipt_scope_valid(changed, MODEL), field)
        changed = copy.deepcopy(full); changed["units"] = []
        self.assertFalse(accept.build_receipt_scope_valid(changed, MODEL))
        changed = copy.deepcopy(full); changed["inputs"]["compilers"] = {}
        self.assertFalse(accept.build_receipt_scope_valid(changed, MODEL))
        changed = copy.deepcopy(full); changed["units"].append(copy.deepcopy(changed["units"][0]))
        self.assertFalse(accept.build_receipt_scope_valid(changed, MODEL))
        changed = copy.deepcopy(full); changed["units"][0]["u1_witness_comparisons"][0]["size"] = 1
        self.assertFalse(accept.build_receipt_scope_valid(changed, MODEL))

    def test_isolated_prepare_rejects_the_reviewers_bounded_transformations(self):
        bounded = self.fixture.run_build(bounded=True)
        for mutation in ("bounded", "no-mode", "summary"):
            fixture = accept_tests.AcceptGateTest(); fixture.setUp()
            try:
                value = copy.deepcopy(bounded)
                if mutation == "no-mode": value.pop("mode")
                elif mutation == "summary": value = accept.build_summary(value)
                candidate = fixture.root / "config/candidate-scope.json"
                candidate.write_text(json.dumps({"units": [{"source": "src/a.c", "profile": "game", "functions": ["func_00100000"]}]}))

                def fake_build(name, matches, **kwargs):
                    result = copy.deepcopy(value); result["name"] = name; result["fresh"] = True
                    return result

                stubs = {
                    "build": types.SimpleNamespace(build=fake_build, BuildError=RuntimeError),
                    "denominator": types.SimpleNamespace(verify=lambda: None, read_history=lambda: [{}]),
                    "interfaces": types.SimpleNamespace(audit=lambda *args, **kwargs: {"units": [], "disagreements": [], "policy": []},
                                                        select_units=lambda *args: [], failed=lambda *args: False),
                    "test_suite": types.SimpleNamespace(run=lambda: {"returncode": 0, "tests_run": 0, "skipped": 0,
                                                                     "failed_modules": [], "seconds": 0.0})}
                with patch.dict(sys.modules, stubs), patch.object(accept, "clean_commit", return_value="0" * 40):
                    record = accept.prepare("scope-attack", candidate, linker_model=MODEL)
                self.assertEqual(record["result"], "FAIL", mutation)
                self.assertFalse(record["checks"]["build_scope_valid"], mutation)
            finally:
                fixture.tearDown()

    def test_selected_verification_rejects_missing_mode_and_summary_receipts(self):
        bounded = self.fixture.run_build(bounded=True)
        for mutation in ("no-mode", "summary"):
            value = copy.deepcopy(bounded)
            if mutation == "no-mode": value.pop("mode")
            else: value = accept.build_summary(value)
            out = self.fixture.root / ("selected-" + mutation)
            selection = {"linker_model": MODEL, "literal_model": "none"}
            accept.write_exclusive(out / "selection.json", selection)
            summaries = []
            for suffix in ("a", "b"):
                receipt = copy.deepcopy(value); receipt["name"] = out.name + "-" + suffix
                path = out / "receipts" / (receipt["name"] + ".json")
                accept.write_exclusive(path, receipt)
                summaries.append({**accept.build_summary(receipt), "receipt_sha256": accept.sha256_file(path)})
            prepared = {"schema_version": 2, "build_evidence_version": 2, **selection,
                        "selection_sha256": accept.sha256_file(out / "selection.json"), "builds": summaries}
            with self.assertRaisesRegex(accept.AcceptError, "full per-unit U1 receipt"):
                accept.verify_selection(out, prepared, required=True)

    def test_legacy_full_evidence_is_explicitly_scoped_and_partial_evidence_never_is(self):
        full = self.fixture.run_build()
        legacy = copy.deepcopy(full); legacy["schema_version"] = 1
        legacy.pop("mode"); legacy.pop("record_kind")
        self.assertFalse(accept.build_receipt_scope_valid(legacy, MODEL, allow_legacy_full=True))
        self.assertFalse(accept.build_receipt_scope_valid(legacy, "armlink5-inline-branchnop-v16"))
        self.assertTrue(accept.build_receipt_scope_valid(legacy, "armlink5-inline-branchnop-v16", allow_legacy_full=True))
