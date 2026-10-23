"""V18 raw group declaration census and historically scoped receipt requirements."""
from __future__ import annotations

import copy
import io
import struct
from pathlib import Path
from unittest.mock import patch

from tests import test_build_u1_v17 as previous
from tests import test_build_u1_v16 as fixture_module
from tests.elf_fixture import serialize_object

accept = previous.accept
build = previous.build
inplace = previous.inplace
MODEL = "armlink5-inline-branchnop-v18"


class CompleteGroupCensusTest(previous.ClosedCompositionTest):
    def setUp(self):
        selected = patch.object(previous, "MODEL", MODEL)
        selected.start()
        self.addCleanup(selected.stop)
        super().setUp()

    def raw_group_case(self, kind: str, size: int, reverse: bool, *, model: str = MODEL) -> dict:
        f = fixture_module.PerUnitU1BuildTest()
        f.setUp()
        saved = self.fixture
        self.fixture = f
        observed = {}
        try:
            self.grouped_foreign(kind)
            if reverse:
                f.units.reverse()
            original_compile = f.compile_source

            def compiler(profile, source, output, **kwargs):
                original_compile(profile, source, output, **kwargs)
                if source.stem == "other":
                    data = bytearray(output.read_bytes())
                    elf = inplace.ELFFile(io.BytesIO(data))
                    index = elf.get_section_index(".group.D0_group")
                    struct.pack_into("<I", data, elf["e_shoff"] + index * elf["e_shentsize"] + 20, size)
                    output.write_bytes(data)
                    parsed = inplace.ObjectFile(output, input_kind="object")
                    observed.update(declarations=parsed.group_declarations,
                                    declared_count=parsed.input_group_count,
                                    issues=parsed.group_declaration_issues,
                                    legacy_copies=parsed.group_copies)
                return False

            with patch.object(build.toolchain, "compile_source", side_effect=compiler), patch.object(
                    fixture_module, "MODEL", model):
                if model == MODEL:
                    if size < 8:
                        with patch.object(inplace.InPlaceLinker, "prepare_unit_u1") as stage:
                            with self.assertRaisesRegex(inplace.LinkError, "SHT_GROUP"):
                                f.run_build(bounded=True)
                            stage.assert_not_called()
                    else:
                        with self.assertRaisesRegex((inplace.LinkError, build.BuildError), "COMDAT"):
                            f.run_build(bounded=True)
                    self.assertFalse((f.root / "build/fixture/receipt.json").exists())
                else:
                    receipt = f.run_build(bounded=True)
                    self.assertEqual(receipt["result"], "PASS")
                    self.assertEqual(sum(unit["u1_authorization_count"] for unit in receipt["units"]), 1)
            return observed
        finally:
            self.fixture = saved
            f.doCleanups()

    def test_review_six_empty_group_declarations_refuse_before_any_local_u1(self):
        for kind in ("kept-code", "unplaced-data", "zero-member"):
            for reverse in (False, True):
                with self.subTest(kind=kind, reverse=reverse):
                    observed = self.raw_group_case(kind, 0, reverse)
                    self.assertEqual(observed["declared_count"], 1)
                    self.assertEqual(len(observed["declarations"]), 1)
                    declaration = observed["declarations"][0]
                    self.assertEqual(declaration.signature, "D0_group")
                    self.assertIsNone(declaration.flags)
                    self.assertEqual(declaration.members, ())
                    self.assertTrue(declaration.issues)
                    self.assertTrue(observed["issues"])

    def test_review_twelve_nonempty_group_controls_still_refuse(self):
        for size in (4, 8):
            for kind in ("kept-code", "unplaced-data", "zero-member"):
                for reverse in (False, True):
                    with self.subTest(size=size, kind=kind, reverse=reverse):
                        observed = self.raw_group_case(kind, size, reverse)
                        self.assertEqual(observed["declared_count"], 1)
                        self.assertEqual(len(observed["declarations"]), 1)
                        self.assertEqual(len(observed["declarations"][0].members), int(size == 8))

    def test_v17_retains_its_recorded_empty_group_behavior(self):
        observed = self.raw_group_case("kept-code", 0, False, model="armlink5-inline-branchnop-v17")
        self.assertEqual(observed["legacy_copies"], [])
        self.assertEqual(len(observed["declarations"]), 1)
        self.assertFalse(inplace.linker_model("armlink5-inline-branchnop-v17").complete_group_declarations)
        self.assertTrue(inplace.linker_model(MODEL).complete_group_declarations)

    def test_partial_group_words_refuse_before_any_local_u1(self):
        for size in (1, 2, 3, 5, 6, 7):
            with self.subTest(size=size):
                self.raw_group_case("unplaced-data", size, False)

    def test_unknown_foreign_group_semantics_and_unrepresented_members_refuse(self):
        self.grouped_foreign("unplaced-data")
        f = self.fixture
        for flags in (0, 3):
            f.empty.section_groups = [(flags, "D0_group", {1})]
            with self.assertRaisesRegex(inplace.LinkError, "SHT_GROUP"):
                f.run_build(name=f"flags-{flags}", bounded=True)
        f.empty.section_groups = [(1, "D0_group", {1})]
        original_compile = f.compile_source

        def unrepresented(profile, source, output, **kwargs):
            original_compile(profile, source, output, **kwargs)
            if source.stem == "other":
                data = bytearray(output.read_bytes())
                elf = inplace.ELFFile(io.BytesIO(data))
                group = elf.get_section_by_name(".group.D0_group")
                struct.pack_into("<I", data, group["sh_offset"] + 4, 0xFFFF)
                output.write_bytes(data)
            return False

        with patch.object(build.toolchain, "compile_source", side_effect=unrepresented):
            with self.assertRaisesRegex(inplace.LinkError, "SHT_GROUP"):
                f.run_build(name="unrepresented", bounded=True)

    def test_nonallocated_group_members_enter_census_and_affected_copy_proof(self):
        self.grouped_foreign("unplaced-data")
        f = self.fixture
        original_compile = f.compile_source

        def nonallocated(profile, source, output, **kwargs):
            original_compile(profile, source, output, **kwargs)
            if source.stem == "other":
                data = bytearray(output.read_bytes())
                elf = inplace.ELFFile(io.BytesIO(data))
                struct.pack_into("<I", data, elf["e_shoff"] + elf["e_shentsize"] + 8, 0x200)
                output.write_bytes(data)
                parsed = inplace.ObjectFile(output, input_kind="object")
                self.assertFalse(parsed.sections or parsed.dependency_sections)
                self.assertEqual(parsed.group_declarations[0].members, (1,))
                parsed.check_group_declarations()
            return False

        with patch.object(build.toolchain, "compile_source", side_effect=nonallocated):
            for reverse in (False, True):
                if reverse:
                    f.units.reverse()
                with self.assertRaisesRegex(build.BuildError, "COMDAT copy declaration"):
                    f.run_build(name=f"nonallocated-{reverse}", bounded=True)
        # An unrelated, fully accounted nonallocated group is not an affected U1 copy.
        f.empty.sections[1].comdat = "unrelated_group"
        f.empty.section_groups = [(1, "unrelated_group", {1})]
        with patch.object(build.toolchain, "compile_source", side_effect=nonallocated):
            receipt = f.run_build(name="unrelated-nonallocated", bounded=True)
        self.assertEqual(receipt["result"], "PASS")

    def test_orphan_group_flag_and_missing_census_never_default_to_empty(self):
        obj = inplace.ObjectFile(Path("owner.o"), data=serialize_object(self.fixture.owner), input_kind="object")
        obj.check_group_declarations()
        for field in ("group_declarations", "group_declaration_issues", "input_group_count"):
            missing = copy.copy(obj)
            delattr(missing, field)
            with self.subTest(field=field):
                with self.assertRaisesRegex(inplace.LinkError, "SHT_GROUP"):
                    missing.check_group_declarations()
                combined = inplace.ObjectFile.combine([missing])
                with self.assertRaisesRegex(inplace.LinkError, "SHT_GROUP"):
                    combined.check_group_declarations()
        f = self.fixture
        f.empty.sections = {1: inplace.Section(1, ".orphan", bytes(4), 4, False, False, False,
                                             comdat="unrepresented")}
        f.empty.section_groups = []
        with self.assertRaisesRegex(inplace.LinkError, "SHT_GROUP"):
            f.run_build(name="orphan-group-bit", bounded=True)

    def test_combination_preserves_empty_declarations_and_independent_counts(self):
        self.grouped_foreign("zero-member")
        data = bytearray(serialize_object(self.fixture.empty))
        elf = inplace.ELFFile(io.BytesIO(data))
        index = elf.get_section_index(".group.D0_group")
        struct.pack_into("<I", data, elf["e_shoff"] + index * elf["e_shentsize"] + 20, 0)
        obj = inplace.ObjectFile(Path("empty-group.o"), data=bytes(data), input_kind="object")
        combined = inplace.ObjectFile.combine([obj])
        nested = inplace.ObjectFile.combine([combined])
        self.assertEqual(combined.group_declarations, obj.group_declarations)
        self.assertEqual(nested.group_declarations, obj.group_declarations)
        self.assertIs(nested.group_declarations[0].origin, obj.origin)
        for view in (obj, combined, nested):
            self.assertEqual(view.input_group_count, 1)
            with self.assertRaisesRegex(inplace.LinkError, "SHT_GROUP"):
                view.check_group_declarations()
        dropped = copy.copy(obj)
        dropped.group_declarations = []
        with self.assertRaisesRegex(inplace.LinkError, "incomplete"):
            dropped.check_group_declarations()


class FullReceiptV18Test(previous.PositiveFullEvidenceTest):
    def setUp(self):
        selected = patch.object(previous, "MODEL", MODEL)
        selected.start()
        self.addCleanup(selected.stop)
        super().setUp()

    def test_new_full_receipt_still_requires_rewrite_reporting(self):
        receipt = self.fixture.run_build()
        receipt["units"][0].pop("linker_rewrites")
        self.assertFalse(accept.build_receipt_scope_valid(receipt, MODEL))
        self.assertFalse(accept.build_receipt_scope_valid(receipt, MODEL, allow_legacy_full=True))
