"""Public hand-encoded fixtures; no original executable or licensed object bytes."""
from __future__ import annotations

from copy import deepcopy
import hashlib
import json
from pathlib import Path
import tempfile
import unittest

from tools import analyze, catalogue_runtime_data as runtime
from tests.test_analyze import BASE, branch, fixture


class RuntimeDescriptorTests(unittest.TestCase):
    def analyzer(self, words: list[int]):
        image, layout = fixture(words)
        return analyze.Analyzer(image, layout)

    def descriptor_fixture(self, offset: int = 0):
        # A new synthetic descriptor consumer: skip the descriptor, add a bounded
        # continuation offset, restore the two saved words and transfer to it.
        descriptor = BASE + 16
        words = [0xE49E4004, 0xE28EC000 | offset, 0xE8BD4010, 0xE12FFF1C, 0x13579BDF]
        return self.analyzer(words), descriptor

    def test_exact_post_descriptor_return_and_stack_restore(self) -> None:
        for offset in (0, 4, 8, 12):
            analyzer, descriptor = self.descriptor_fixture(offset)
            proof = runtime.descriptor_paths(analyzer, BASE, 16, descriptor)
            self.assertEqual(proof["descriptor_width"], 4)
            self.assertEqual(proof["return_address_delta"], 4)
            self.assertEqual(proof["continuation_targets"], [descriptor + 4 + offset])
            self.assertEqual(proof["descriptor_read_sites"], [BASE])
            self.assertEqual(proof["saved_pair_restore_sites"], [BASE + 8])
            self.assertTrue(proof["all_incoming_core_register_values"])
            self.assertEqual(proof["entry_data_endian"], "little")
            self.assertFalse(proof["global_noreturn_inferred"])

    def test_unknown_predicate_cannot_hide_an_invalid_continuation(self) -> None:
        analyzer = self.analyzer([
            0xE49E4004, branch(BASE + 4, BASE + 16, condition=0),
            0xE28EC000, branch(BASE + 12, BASE + 20), 0xE3A0C000,
            0xE8BD4010, 0xE12FFF1C, 0x2468ACE0])
        with self.assertRaisesRegex(ValueError, "escapes"):
            runtime.descriptor_paths(analyzer, BASE, 28, BASE + 28)

    def test_wrong_width_double_read_and_missing_restore_fail_closed(self) -> None:
        variants = [
            [0xE49E4008, 0xE28EC000, 0xE8BD4010, 0xE12FFF1C],
            [0xE49E4004, 0xE49E4004, 0xE8BD4010, 0xE12FFF1C],
            [0xE49E4004, 0xE28EC000, 0xE3A00000, 0xE12FFF1C],
            [0xE49E4004, 0xE28EC000, 0xE8BD4010, 0xE12FFF1E],
        ]
        for words in variants:
            with self.subTest(words=words), self.assertRaises(ValueError):
                runtime.descriptor_paths(self.analyzer(words + [0]), BASE, 16, BASE + 16)

    def test_unsupported_internal_dispatch_loop_and_escape_fail_closed(self) -> None:
        for middle in (0xE08FF10C, branch(BASE + 4, BASE + 4), branch(BASE + 4, BASE + 28)):
            with self.subTest(middle=middle), self.assertRaises(ValueError):
                runtime.descriptor_paths(self.analyzer([0xE49E4004, middle, 0xE8BD4010, 0xE12FFF1C, 0]),
                                         BASE, 16, BASE + 16)

    def test_register_shift_uses_architectural_low_byte(self) -> None:
        registers = {analyze.arm.ARM_REG_R0: runtime.Value.finite((0x100,))}
        operand = type("Operand", (), {
            "type": analyze.arm.ARM_OP_IMM, "imm": 1,
            "shift": type("Shift", (), {"type": analyze.arm.ARM_SFT_LSL_REG,
                                           "value": analyze.arm.ARM_REG_R0})(),
        })()
        self.assertEqual(runtime.operand_value(operand, registers).values, frozenset((1,)))

    def test_continuation_must_preserve_restored_lr_and_cover_every_edge(self) -> None:
        analyzer = self.analyzer([0xE3A00000, 0xE12FFF1E, 0xE3A01000, 0xE12FFF1E])
        first = runtime._continuation_cfg(analyzer, BASE, [[BASE, BASE + 16]], BASE)
        self.assertEqual(first["return_sites"], [BASE + 4])
        self.assertEqual(first["instruction_sites"], [BASE, BASE + 4])
        for bad in (0xE3A0E000, 0xE3A0D000, 0xE3A04000, 0xE12FFF10):
            with self.subTest(bad=bad), self.assertRaises(ValueError):
                runtime._continuation_cfg(self.analyzer([bad, 0xE12FFF1E]), BASE, [[BASE, BASE + 8]], BASE)

    def test_caller_prefix_proves_saved_pair_dominates_exceptional_call(self) -> None:
        descriptor = {"save_site": BASE + 4, "call_site": BASE + 8, "dispatcher": BASE + 20}
        words = [0xE3A00000, 0xE92D4010, branch(BASE + 8, BASE + 20, link=True),
                 0xE12FFF1E, 0, 0xE12FFF1E]
        proof = runtime._caller_prefix(self.analyzer(words), BASE, [[BASE, BASE + 12]], descriptor)
        self.assertTrue(proof["saved_pair_is_original_r4_lr"])
        words[0] = branch(BASE, BASE + 8)
        with self.assertRaisesRegex(ValueError, "saved LR"):
            runtime._caller_prefix(self.analyzer(words), BASE, [[BASE, BASE + 12]], descriptor)


class RuntimeObjectWitnessTests(unittest.TestCase):
    def metadata(self, root: Path) -> tuple[dict, Path]:
        parent = root / "research/parent.json"
        parent.parent.mkdir()
        parent.write_text(json.dumps({"image_sha256": "a" * 64}))
        binding = {"address": BASE, "isa": "arm", "symbol": "synthetic", "symbol_offset": 0,
                   "incoming_sources": [BASE + 16], "prior_record_sha256": "b" * 64}
        section = {"allocated": True, "executable": True, "writable": False,
                   "size": 8, "alignment": 4, "image_sha256": "c" * 64, "object_sha256": "d" * 64,
                   "mapping": [[0, "$a"], [4, "$d"]],
                   "partitions": [{"offset": 0, "end_offset": 4, "kind": "code"},
                                  {"offset": 4, "end_offset": 8, "kind": "data"}],
                   "symbols": [{"name": "synthetic", "kind": "STT_FUNC", "offset": 0, "size": 4},
                               {"name": ".text", "kind": "STT_SECTION", "offset": 0, "size": 8}]}
        witness = {"exact_build": "5.04-b82", "whole_section_reencoded_equal": True,
                   "image_section_sha256": "c" * 64, "archive_member": "synthetic.o",
                   "member_offset": 8, "member_sha256": "e" * 64, "archive_sha256": "f" * 64,
                   "source_match_index": 0}
        evidence = {
            "revision": "catalogue-v11", "image_sha256": "a" * 64,
            "parent_evidence": {"path": "research/parent.json", "sha256": hashlib.sha256(parent.read_bytes()).hexdigest()},
            "prior_proposal": {"sha256": "9" * 64, "commit": "1" * 40,
                               "unknown_bytes": 4, "unknown_intervals": 1, "adopted": False},
            "new_C_credit": 0,
            "sections": [{"id": "synthetic-section", "binding": binding, "section": section,
                          "object_witness": witness, "closure_kind": "unowned_data"}],
        }
        section.update(index=1, relocations=[])
        evidence["retained_source_rows"] = {"0": {
            "match": {"name": "synthetic", "size": 8, "object_body_sha256": "d" * 64,
                      "hit": {"image_body_sha256": "c" * 64, "whole_body_masked_equal": True},
                      "object_section": 1, "object_offset": 0, "member": "synthetic.o",
                      "member_sha256": "e" * 64, "relocation_fields": []},
            "relocation_check": {"whole_relocated_body_equal": True, "size": 8, "address": BASE,
                                 "symbol": "synthetic", "relocated_body_sha256": "c" * 64, "relocations": []},
        }}
        return evidence, parent

    def test_complete_object_mapping_and_parent_image_are_required(self) -> None:
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            evidence, parent = self.metadata(root)
            path = root / "research/catalogue_v11_evidence.json"
            path.write_text(json.dumps(evidence))
            loaded, inputs = runtime.load_inputs(root, parent)
            self.assertEqual(loaded, evidence)
            self.assertIn("research/catalogue_v11_evidence.json", inputs)
            changes = (
                ("image", lambda value: value.update(image_sha256="0" * 64)),
                ("mapping", lambda value: value["sections"][0]["section"].update(mapping=[[0, "$a"]])),
                ("bounds", lambda value: value["sections"][0]["section"]["partitions"][-1].update(end_offset=12)),
                ("member", lambda value: value["sections"][0]["object_witness"].update(member_sha256="not-a-hash")),
                ("placement", lambda value: value["sections"][0]["binding"].update(incoming_sources=[])),
                ("build", lambda value: value["sections"][0]["object_witness"].update(exact_build="5.06")),
                ("equality", lambda value: value["sections"][0]["object_witness"].update(whole_section_reencoded_equal=False)),
                ("writable", lambda value: value["sections"][0]["section"].update(writable=True)),
                ("function", lambda value: value["sections"][0]["section"]["symbols"][0].update(kind="STT_OBJECT")),
                ("section", lambda value: value["sections"][0]["section"]["symbols"][1].update(size=4)),
                ("whole object", lambda value: value["retained_source_rows"]["0"]["relocation_check"].update(whole_relocated_body_equal=False)),
                ("relocation closure", lambda value: value["sections"][0]["section"]["relocations"].append({"offset": 0, "type": 2, "symbol": "foreign"})),
                ("adoption", lambda value: value["prior_proposal"].update(adopted=True)),
                ("credit", lambda value: value.update(new_C_credit=4)),
                ("duplicate", lambda value: value["sections"].append(deepcopy(value["sections"][0]))),
            )
            for label, change in changes:
                value = deepcopy(evidence)
                change(value)
                path.write_text(json.dumps(value))
                with self.subTest(change=label), self.assertRaises(ValueError):
                    runtime.load_inputs(root, parent)

    def test_unknown_only_range_cutover_preserves_all_existing_boundaries(self) -> None:
        catalogue = {"ranges": [
            {"start": BASE, "end": BASE + 4, "kind": "function", "owner": BASE},
            {"start": BASE + 4, "end": BASE + 8, "kind": "unknown", "owner": None},
            {"start": BASE + 8, "end": BASE + 16, "kind": "segment_padding", "owner": None},
            {"start": BASE + 16, "end": BASE + 20, "kind": "segment_padding", "owner": None},
        ]}
        before = deepcopy(catalogue["ranges"])
        runtime._replace_unknown_ranges(catalogue, [(BASE + 4, BASE + 8, "rodata", None)])
        self.assertEqual(catalogue["ranges"][0], before[0])
        self.assertEqual(catalogue["ranges"][2:], before[2:])
        self.assertEqual(catalogue["ranges"][1]["kind"], "rodata")
        with self.assertRaisesRegex(ValueError, "existing physical"):
            runtime._replace_unknown_ranges(catalogue, [(BASE, BASE + 4, "rodata", None)])

    def test_full_section_digest_prevents_partial_data_coincidence(self) -> None:
        image, layout = fixture([0xE12FFF1E, 0x10203040])
        analyzer = analyze.Analyzer(image, layout)
        section = {"size": 8, "image_sha256": hashlib.sha256(analyzer.text[:8]).hexdigest(),
                   "partitions": [{"offset": 0, "end_offset": 4, "image_sha256": hashlib.sha256(analyzer.text[:4]).hexdigest()},
                                  {"offset": 4, "end_offset": 8, "image_sha256": hashlib.sha256(analyzer.text[4:8]).hexdigest()}],
                   "relocations": []}
        record = {"binding": {"address": BASE}, "section": section}
        runtime._check_section(analyzer, record)
        record["section"]["image_sha256"] = "0" * 64
        with self.assertRaisesRegex(ValueError, "complete runtime section"):
            runtime._check_section(analyzer, record)

    def test_historical_shorter_classification_is_never_reinterpreted(self) -> None:
        historical = {"address": BASE, "size": 4, "isa": "arm", "identity": "synthetic", "kind": "runtime"}
        replacement = {**historical, "size": 8, "historical_classification_evidence": historical}
        evidence = {"sections": [{"closure_kind": "descriptor_continuation", "binding": {"address": BASE},
                                   "prior_function": {"classification_evidence": historical},
                                   "classification_record": replacement}]}
        original = deepcopy(historical)
        self.assertEqual(runtime.classification_records([historical], evidence), [replacement])
        self.assertEqual(historical, original)
        self.assertEqual(runtime.classification_records([replacement], evidence), [replacement])
        with self.assertRaisesRegex(ValueError, "historical identity"):
            runtime.classification_records([{**historical, "identity": "another"}], evidence)
        with self.assertRaisesRegex(ValueError, "lost its prior"):
            runtime.classification_records([], evidence)


class RuntimeEntryProtectionTests(unittest.TestCase):
    def test_actual_global_entry_declaration_and_complete_section_are_required(self) -> None:
        image, layout = fixture([0x47704770, 0xE12FFF1E])
        analyzer = analyze.Analyzer(image, layout)
        anchor = {"address": BASE + 4, "size": 4, "isa": "arm", "kind": "library",
                  "code_ranges": [[BASE + 4, BASE + 8]], "entries": [],
                  "classification_evidence": {"identity": "synthetic_anchor"}}
        catalogue = {"image_sha256": hashlib.sha256(image).hexdigest(), "functions": [anchor],
                     "ranges": [{"start": BASE, "end": BASE + 4, "kind": "unknown", "owner": None}],
                     "stats": {}, "closure_evidence": {"exact_runtime_data": {}}}
        section = {"section_base": BASE, "section_size": 8, "alignment": 4,
                   "whole_section_reencoded_equal": True, "image_section_sha256": hashlib.sha256(analyzer.text).hexdigest(),
                   "symbols": [{"name": "synthetic_selector", "kind": "STT_FUNC", "bind": "STB_LOCAL", "offset": 0, "size": 2}]}
        record = {"address": BASE, "size": 2, "isa": "thumb", "symbol": "synthetic_selector", "symbol_offset": 0,
                  "section": section, "anchor": {"address": BASE + 4, "size": 4, "isa": "arm", "symbol": "synthetic_anchor",
                                                "symbol_offset": 4, "record_sha256": runtime._record_sha(anchor),
                                                "image_sha256": hashlib.sha256(analyzer.text[4:8]).hexdigest()}}
        with self.assertRaisesRegex(ValueError, "global function declaration"):
            runtime.close_entry_fragments(catalogue, analyzer, {"entry_fragments": [record]}, {})
        section["whole_section_reencoded_equal"] = False
        with self.assertRaisesRegex(ValueError, "complete independently bound"):
            runtime.close_entry_fragments(catalogue, analyzer, {"entry_fragments": [record]}, {})

    def test_prepared_protection_checks_complete_metadata_not_only_extent(self) -> None:
        function = {"address": BASE, "size": 4, "isa": "arm", "entries": [], "literals": []}
        evidence = {"_prepared_protection": {"identity_count": 1, "identities": [
            {"name": "synthetic", "owner": BASE, "size": 4, "isa": "arm", "entries": [],
             "proposal_record_sha256": runtime._record_sha(function)}]}}
        catalogue = {"functions": [function]}
        runtime.check_prepared_protection(catalogue, evidence)
        function["literals"] = [[BASE, BASE + 4]]
        with self.assertRaisesRegex(ValueError, "prepared identity"):
            runtime.check_prepared_protection(catalogue, evidence)



if __name__ == "__main__":
    unittest.main()
