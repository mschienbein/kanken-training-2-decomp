"""V16 direct-unit U1 preparation, aggregate vetoes and checkpoint receipt closure."""
from __future__ import annotations

import copy
import json
import sys
import tempfile
import unittest
from contextlib import ExitStack
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

from tests import test_inplace as seeds
from tests.elf_fixture import serialize_object

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import accept
import build
import inplace
import toolchain

MODEL = "armlink5-inline-branchnop-v16"


class PerUnitU1BuildTest(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.owner, self.memory, self.functions, self.names = seeds.UnreferencedDestructorUnwindModelTest().fixture("extab")
        self.empty = inplace.ObjectFile.__new__(inplace.ObjectFile)
        self.empty.sections, self.empty.symbols, self.empty.section_groups = {}, [], []
        self.objects = {"owner": self.owner, "other": self.empty}
        self.units = [{"source": "src/owner.cpp", "profile": "game", "functions": ["func_00100000"]},
                      {"source": "src/other.cpp", "profile": "game", "functions": []}]
        self.memory.code = self.memory.read(seeds.BASE, 4)
        self.memory.sha256 = build.sha256_bytes(self.memory.code)
        self.memory.bss = (0x200000, 0x201000)
        self.memory.layout = SimpleNamespace(file_offset=lambda address: address - seeds.BASE)
        self.catalogue = {"image_sha256": self.memory.sha256, "functions": list(self.functions.values())}
        self.matches = self.root / "config/fixture.json"
        for name, contents in {
            "src/owner.cpp": "// independent compiler input identity\n",
            "src/other.cpp": "// independent helper input identity\n",
            "header.h": "// independent dependency identity\n",
            "config/catalogue.json": "{}", "config/names.json": "{}", "config/compiler_profiles.json": "{}",
            "toolchain.lock.json": "{}", "orig/exheader.bin": "synthetic layout", "compiler.exe": "synthetic compiler",
            "orig/exefs/code.blz": "synthetic packed image",
        }.items():
            path = self.root / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(contents, encoding="utf-8")
        self.compile_calls = []
        stack = ExitStack()
        self.addCleanup(stack.close)
        for target, name, value in ((build, "ROOT", self.root), (build, "MATCHES", self.matches),
                                    (build, "HARNESS", ("toolchain.lock.json",)),
                                    (build.image_mod, "CATALOGUE", self.root / "config/catalogue.json"),
                                    (build.image_mod, "NAMES", self.root / "config/names.json"),
                                    (build.toolchain, "PROFILES", self.root / "config/compiler_profiles.json")):
            stack.enter_context(patch.object(target, name, value))
        stack.enter_context(patch.object(build.denominator, "verify", return_value=None))
        stack.enter_context(patch.object(build.image_mod.Image, "load", return_value=self.memory))
        stack.enter_context(patch.object(build.image_mod, "load_catalogue", return_value=self.catalogue))
        stack.enter_context(patch.object(build.image_mod, "load_names", side_effect=lambda path=None: self.names if path is None else json.loads(path.read_text())["symbols"]))
        stack.enter_context(patch.object(build.toolchain, "load_profiles", return_value={"game": toolchain.Profile("game", "synthetic", ())}))
        stack.enter_context(patch.object(build.toolchain, "armcc_exe", return_value=self.root / "compiler.exe"))
        stack.enter_context(patch.object(build.toolchain, "compile_source", side_effect=self.compile_source))
        self.compressor = stack.enter_context(patch.object(build.ctrfmt, "blz_compress", return_value=b"synthetic packed image"))

    def compile_source(self, profile, source, output, **kwargs):
        self.compile_calls.append(kwargs)
        output.parent.mkdir(parents=True, exist_ok=True)
        output.write_bytes(serialize_object(self.objects[source.stem]))
        output.with_suffix(".d").write_text(f"{output}: {source}\n{output}: {self.root / 'header.h'}\n")
        return False

    def run_build(self, name="fixture", bounded=False):
        self.matches.write_text(json.dumps({"units": self.units}))
        return build.build(name, self.matches, jobs=1, linker_model=MODEL, bounded=bounded)

    def test_full_build_path_stages_direct_units_then_carries_all_actions(self):
        receipt = self.run_build()
        self.assertEqual(receipt["result"], "PASS")
        self.assertEqual(receipt["inputs"]["linker_model"], MODEL)
        self.assertEqual(receipt["inputs"]["u1_stage"], "per-unit-direct-before-aggregation")
        self.assertTrue(receipt["fresh"])
        owner = receipt["units"][0]
        member, = owner["removed_special_members"]
        action, = owner["linker_section_rewrites"]
        self.assertEqual(action["removed_special_member"], member)
        self.assertEqual((action["rule"], action["target"]), ("unused_deleting_destructor", None))
        self.assertEqual([row["kind"] for row in member["unwind_sections"]], ["exidx", "extab"])
        self.assertEqual(member["unit"], "src/owner.cpp")
        self.assertEqual(member["input_kind"], "object")
        self.assertEqual(owner["u1_witness_comparisons"][0]["status"], "match")
        self.assertEqual((receipt["credited"]["functions"], receipt["credited"]["bytes"]), (1, 4))
        self.assertEqual(owner["headers_sha256"], {str(self.root / "header.h"): build.sha256_file(self.root / "header.h")})
        self.assertTrue(all(call.get("cache") is False for call in self.compile_calls))
        self.assertEqual(accept.model_use_summary(receipt)["src/owner.cpp"]["linker_section_rewrites"], [action])
        self.assertTrue(accept.build_receipt_scope_valid(receipt, MODEL))
        self.assertEqual(build.sha256_file(self.root / "build/fixture/code.bin"), self.memory.sha256)

    def test_bounded_path_compares_spans_without_image_payload_or_recompression(self):
        receipt = self.run_build(bounded=True)
        self.assertEqual((receipt["mode"], receipt["result"]), ("bounded", "PASS"))
        self.assertEqual(receipt["covered"]["bytes"], 4)
        self.assertEqual(receipt["outputs"]["covered_spans"][0]["match"], True)
        self.assertNotIn("code_bin_sha256", receipt["outputs"])
        self.assertFalse((self.root / "build/fixture/code.bin").exists())
        self.assertFalse((self.root / "build/fixture/code.blz").exists())
        self.compressor.assert_not_called()
        self.assertFalse(accept.build_receipt_scope_valid(receipt, MODEL))

    def test_zero_credit_owner_keeps_action_and_independent_witness_check(self):
        self.units[0]["functions"] = []
        receipt = self.run_build(bounded=True)
        self.assertEqual(receipt["covered"]["functions"], 0)
        use = accept.model_use_summary(receipt)["src/owner.cpp"]
        self.assertEqual(use["u1_functions"], [])
        self.assertEqual(len(use["removed_special_members"]), 1)
        self.assertEqual(receipt["units"][0]["u1_witness_comparisons"][0]["status"], "match")

    def test_bad_retained_witness_cannot_publish_success(self):
        self.owner.sections[1].data = b"bad!"
        with self.assertRaisesRegex(build.BuildError, "D1 witness"):
            self.run_build(bounded=True)
        self.assertFalse((self.root / "build/fixture/receipt.json").exists())

    def test_combined_input_cannot_attempt_unit_or_aggregate_u1(self):
        first = inplace.ObjectFile(Path("owner.o"), data=serialize_object(self.owner), input_kind="object")
        second = inplace.ObjectFile(Path("empty.o"), data=serialize_object(self.empty), input_kind="object")
        combined = inplace.ObjectFile.combine([first, second])
        oracle = inplace.InPlaceLinker(self.memory, self.functions, inplace.Resolver(self.functions, self.names), self.memory.bss, MODEL)
        with self.assertRaisesRegex(inplace.LinkError, "refused by scope"):
            oracle.prepare_unit_u1(combined)
        with self.assertRaisesRegex(inplace.LinkError, "refused by scope"):
            oracle.link(combined)
        self.assertEqual(oracle.removed_special_members, [])

    def test_foreign_unplaced_zero_source_keeps_staged_d0_live(self):
        external = inplace.Symbol(seeds.UnreferencedDestructorModelTest.D0, 0, 0, "STT_FUNC", "STB_GLOBAL", "SHN_UNDEF")
        self.empty.sections = {1: inplace.Section(1, ".zero.foreign", b"", 0, False, False, False,
                                                [inplace.Reloc(0, inplace.R_ARM_NONE, external)])}
        self.empty.symbols = [external]
        with self.assertRaisesRegex(build.BuildError, "foreign supplied reference"):
            self.run_build(bounded=True)

    def test_other_unit_personality_definition_vetoes_the_external_exception(self):
        marker = inplace.Symbol("__aeabi_unwind_cpp_pr0", 0, 0, "STT_NOTYPE", "STB_GLOBAL", "SHN_UNDEF")
        self.owner.symbols.append(marker)
        self.owner.sections[4].relocs.append(inplace.Reloc(0, inplace.R_ARM_NONE, marker))
        for bind in ("STB_GLOBAL", "STB_WEAK", "STB_LOCAL"):
            self.empty.sections = {1: inplace.Section(1, ".zero.personality", b"", 0, False, False, False)}
            self.empty.symbols = [inplace.Symbol(marker.name, 0, 0, "STT_OBJECT", bind, 1)]
            with self.assertRaisesRegex(build.BuildError, "supplied ledger definition"):
                self.run_build(name=bind.lower(), bounded=True)

    def test_d1_in_another_unit_cannot_supply_a_per_unit_witness(self):
        complete = copy.deepcopy(self.owner)
        self.owner.sections = {index: section for index, section in self.owner.sections.items() if index != 1}
        self.owner.symbols = [symbol for symbol in self.owner.symbols if symbol.shndx != 1]
        complete.sections = {1: complete.sections[1]}
        complete.symbols = [symbol for symbol in complete.symbols if symbol.shndx == 1]
        self.objects["other"] = complete
        self.units[0]["functions"], self.units[1]["functions"] = [], ["func_00100000"]
        with self.assertRaisesRegex(inplace.LinkError, "same-object"):
            self.run_build(bounded=True)

    def test_acceptance_requires_the_stage_and_complete_action_witness_dispatch(self):
        receipt = self.run_build()
        for mutation in ("stage", "actions", "members", "witness", "unit"):
            changed = copy.deepcopy(receipt)
            owner = changed["units"][0]
            if mutation == "stage":
                changed["inputs"].pop("u1_stage")
            elif mutation == "actions":
                owner["linker_section_rewrites"] = []
            elif mutation == "members":
                owner["removed_special_members"] = []
            elif mutation == "witness":
                owner["u1_witness_comparisons"] = []
            else:
                owner["removed_special_members"][0]["unit"] = "wrong.cpp"
            self.assertFalse(accept.build_receipt_scope_valid(changed, MODEL), mutation)

    def test_v16_is_explicit_and_does_not_reinterpret_v15(self):
        previous, selected = inplace.linker_model("armlink5-inline-branchnop-v15"), inplace.linker_model(MODEL)
        for key, value in vars(previous).items():
            if key not in ("name", "per_unit_u1"):
                self.assertEqual(getattr(selected, key), value)
        self.assertTrue(selected.per_unit_u1)
        self.assertTrue(all(not model.per_unit_u1 for name, model in inplace.LINKER_MODELS.items() if name != MODEL))
