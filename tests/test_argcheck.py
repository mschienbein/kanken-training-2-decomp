"""Synthetic ARM/Thumb register contracts; no original executable fixtures."""
from __future__ import annotations

import contextlib
import io
import json
from pathlib import Path
import struct
import tempfile
import unittest
from unittest.mock import patch

import capstone

from tools import argcheck, ctrfmt

BASE = 0x100000
ROOT = Path(__file__).resolve().parents[1]


def branch(source: int, target: int, *, link: bool = True, condition: int = 14) -> int:
    return (condition << 28) | (0x0B000000 if link else 0x0A000000) | (((target - source - 8) >> 2) & 0xFFFFFF)


def fixture(words: list[int], *, rodata: list[int] | None = None) -> argcheck.image_mod.Image:
    text = struct.pack(f"<{len(words)}I", *words)
    buffers = [text, struct.pack(f"<{len(rodata or [])}I", *(rodata or [])), b""]
    exheader = bytearray(0x200)
    for offset, start, data in zip((0x10, 0x20, 0x30), (BASE, BASE + 0x1000, BASE + 0x2000), buffers):
        struct.pack_into("<III", exheader, offset, start, 1, len(data))
    return argcheck.image_mod.Image(b"".join(data.ljust(0x1000, b"\0") for data in buffers), ctrfmt.parse_exheader(exheader))


def function(start: int, stop: int, *, pools: list | None = None, isa: str = "arm") -> dict:
    return {"address": start, "size": stop - start, "isa": isa,
            "code_ranges": [[start, stop]], "entries": [{"address": start}],
            "pools": pools or [], "literals": []}


def decode(image: argcheck.image_mod.Image, fn: dict) -> tuple[list, list, list]:
    decoder = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM if fn["isa"] == "arm" else capstone.CS_MODE_THUMB)
    decoder.detail = True
    facts = argcheck.decode_function(image, fn, decoder)
    block_list, successors = argcheck.blocks(facts, [fn["address"]])
    return facts, block_list, successors


def declarations(text: str) -> list:
    return argcheck.parse_declarations(text, ROOT / "synthetic.c", ROOT, set())


def registers(site: dict) -> set[str]:
    return {a["register"] for a in site["arguments"]}


class CallEvidenceTests(unittest.TestCase):
    def sites(self, words: list[int], *, rewrites: dict | None = None, entries: set[int] | None = None) -> list[dict]:
        image = fixture(words)
        fn = function(BASE, BASE + 4 * len(words))
        _, block_list, _ = decode(image, fn)
        return argcheck.call_sites(fn, block_list, entries or {BASE + 0x100}, rewrites or {})

    def test_unconsumed_core_writes_and_result_use(self) -> None:
        sites = self.sites([0xE3A00001, 0xE3A01002, branch(BASE + 8, BASE + 0x100), 0xE1A04000, 0xE12FFF1E])
        self.assertEqual(registers(sites[0]), {"r0", "r1"})
        self.assertEqual(sites[0]["arguments"][0]["write"], argcheck.address(BASE))
        self.assertEqual(sites[0]["results"], [{"register": "r0", "read": argcheck.address(BASE + 12)}])

    def test_read_consumes_setup_and_reassignment_restores_it(self) -> None:
        sites = self.sites([0xE3A00001, 0xE3500000, 0xE3A01002, 0xE1A02001,
                            0xE3A00003, branch(BASE + 20, BASE + 0x100), 0xE12FFF1E])
        self.assertEqual(registers(sites[0]), {"r0", "r2"})

    def test_store_consumes_source_and_address(self) -> None:
        sites = self.sites([0xE3A00001, 0xE3A01002, 0xE5801000,
                            branch(BASE + 12, BASE + 0x100), 0xE12FFF1E])
        self.assertEqual(registers(sites[0]), set())

    def test_calls_reset_previous_preparation(self) -> None:
        sites = self.sites([0xE3A00001, branch(BASE + 4, BASE + 0x100),
                            0xE3A01002, branch(BASE + 12, BASE + 0x104), 0xE12FFF1E])
        self.assertEqual(registers(sites[0]), {"r0"})
        self.assertEqual(registers(sites[1]), {"r1"})
        self.assertEqual(sites[0]["results"], [])

    def test_basic_block_join_does_not_claim_predecessor_setup(self) -> None:
        sites = self.sites([0xE3A00001, branch(BASE + 4, BASE + 12, link=False, condition=0),
                            0xE3A01002, branch(BASE + 12, BASE + 0x100), 0xE12FFF1E])
        self.assertEqual(registers(sites[0]), set())

    def test_conditional_write_is_not_guaranteed_argument(self) -> None:
        sites = self.sites([0x03A00001, branch(BASE + 4, BASE + 0x100), 0xE12FFF1E])
        self.assertEqual(registers(sites[0]), set())

    def test_tail_call_and_internal_branch_are_distinct(self) -> None:
        sites = self.sites([0xE3A00001, branch(BASE + 4, BASE + 0x100, link=False)])
        self.assertEqual(sites[0]["kind"], "tail")
        self.assertEqual(registers(sites[0]), {"r0"})
        self.assertEqual(sites[0]["results"], [])
        self.assertEqual(self.sites([branch(BASE, BASE + 4, link=False), 0xE12FFF1E], entries={BASE + 4}), [])

    def test_indirect_target_register_is_not_its_own_argument(self) -> None:
        sites = self.sites([0xE3A00001, 0xE3A01002, 0xE12FFF31, 0xE12FFF1E])
        self.assertEqual(sites[0]["kind"], "indirect")
        self.assertEqual(registers(sites[0]), {"r0"})

    def test_vfp_double_aliases_and_float_result(self) -> None:
        # VLDR D0,[R4]; VLDR S2,[R4]; BL; VSTR S0,[R4]; BX LR.
        sites = self.sites([0xED940B00, 0xED941A00, branch(BASE + 8, BASE + 0x100), 0xED840A00, 0xE12FFF1E])
        self.assertEqual(registers(sites[0]), {"s0", "s1", "s2"})
        self.assertEqual(sites[0]["arguments"][0]["written_as"], "d0")
        self.assertEqual(sites[0]["results"], [{"register": "s0", "read": argcheck.address(BASE + 12)}])

    def test_reading_one_double_lane_consumes_only_that_lane(self) -> None:
        sites = self.sites([0xED940B00, 0xED840A00, branch(BASE + 8, BASE + 0x100), 0xE12FFF1E])
        self.assertEqual(registers(sites[0]), {"s1"})

    def test_vfp_register_lists_have_explicit_access_facts(self) -> None:
        # VLDMIA R4,{S0,S1}; VSTMIA R5,{S0}; BL; BX LR.
        sites = self.sites([0xEC940A02, 0xEC850A01, branch(BASE + 8, BASE + 0x100), 0xE12FFF1E])
        self.assertEqual(registers(sites[0]), {"s1"})

    def test_conditional_overwrite_does_not_hide_possible_result_use(self) -> None:
        sites = self.sites([branch(BASE, BASE + 0x100), 0x03A00000, 0xE1A04000, 0xE12FFF1E])
        self.assertEqual(sites[0]["results"], [{"register": "r0", "read": argcheck.address(BASE + 8)}])

    def test_return_overwrite_and_branch_stop_result_evidence(self) -> None:
        for suffix in ([0xE3A00000, 0xE1A04000, 0xE12FFF1E],
                       [branch(BASE + 4, BASE + 12, link=False), 0xE1A04000, 0xE12FFF1E]):
            with self.subTest(suffix=suffix):
                sites = self.sites([branch(BASE, BASE + 0x100), *suffix])
                self.assertEqual(sites[0]["results"], [])

    def test_rewrite_restores_empty_call_without_counting_replacement_read(self) -> None:
        rewrites = {BASE + 8: [{"target": BASE + 0x100, "kind": "linker_rewrite", "records": ["synthetic.json"]}]}
        sites = self.sites([0xE3A00001, 0xE3A01002, 0xE1A00000, 0xE1A04000, 0xE12FFF1E], rewrites=rewrites)
        self.assertEqual(registers(sites[0]), {"r0", "r1"})
        self.assertEqual(sites[0]["kind"], "linker_rewrite")
        self.assertIn("[INFERENCE]", sites[0]["qualification"])
        issues, _, _ = argcheck.compare(declarations("void func_00100100(void) {}"), sites)
        self.assertEqual({i["kind"] for i in issues}, {"missing_core_parameters", "void_result_consumed"})

    def test_thumb_direct_blx_immediate(self) -> None:
        # ARM BLX immediate to a Thumb catalogue entry.
        sites = self.sites([0xE3A00001, 0xFA000001, 0xE12FFF1E], entries={BASE + 0x10})
        self.assertEqual(sites[0]["target"], BASE + 0x10)
        self.assertEqual(registers(sites[0]), {"r0"})

    def test_thumb_caller(self) -> None:
        # MOVS R0,#1; BLX R3; BX LR (all synthetic Thumb halfwords).
        image = fixture([0x47982001, 0x00004770])
        fn = function(BASE, BASE + 6, isa="thumb")
        _, block_list, _ = decode(image, fn)
        site = argcheck.call_sites(fn, block_list, set(), {})[0]
        self.assertEqual(registers(site), {"r0"})


class PrototypeTests(unittest.TestCase):
    def layout(self, text: str) -> dict:
        return argcheck.prototype_layout(declarations(text)[0].type)

    def test_preprocessor_typedef_and_array_parameter_plumbing(self) -> None:
        parsed = declarations('# 1 "synthetic.c"\ntypedef unsigned Word;\nvoid func_00100100(Word x, const float a[4]) {}')
        self.assertEqual(argcheck.prototype_layout(parsed[0].type)["parameters"], [["r0"], ["r1"]])
        self.assertTrue(parsed[0].definition)

    def test_mixed_core_vfp_allocate_independently(self) -> None:
        layout = self.layout("void func_00100100(void *object, float x, float y, float z) {}")
        self.assertEqual(layout["parameters"], [["r0"], ["s0"], ["s1"], ["s2"]])
        site = {"site": argcheck.address(BASE), "arguments": [{"register": r} for r in ("r0", "r1", "s0", "s1", "s2")], "results": []}
        issues, _, _ = argcheck.compare(declarations("void func_00100100(void *o, float x, float y, float z) {}"), [site])
        self.assertEqual([(i["kind"], i["registers"]) for i in issues], [("missing_core_parameters", ["r1"])])

    def test_double_alignment_and_vfp_backfill(self) -> None:
        layout = self.layout("void func_00100100(float a, double b, float c, unsigned n, unsigned long long wide) {}")
        self.assertEqual(layout["parameters"], [["s0"], ["s2", "s3"], ["s1"], ["r0"], ["r2", "r3"]])
        self.assertEqual(self.layout("void func_00100100(long double x);")["parameters"], [["s0", "s1"]])

    def test_hfa_and_hidden_structure_result(self) -> None:
        layout = self.layout("struct Vec { float x,y,z; }; struct Large { int a,b; }; struct Large func_00100100(struct Vec v, void *p) {}")
        self.assertEqual(layout["parameters"], [["s0", "s1", "s2"], ["r1"]])
        self.assertEqual(layout["hidden_result_registers"], ["r0"])
        self.assertIn("r0", layout["registers"])

    def test_value_in_regs_result_does_not_reserve_hidden_pointer(self) -> None:
        layout = self.layout("struct Pair { int a,b; }; __value_in_regs struct Pair func_00100100(unsigned input);")
        self.assertEqual(layout["parameters"], [["r0"]])
        self.assertEqual(layout["hidden_result_registers"], [])

    def test_base_pcs_floats_use_core_registers(self) -> None:
        layout = self.layout('void __attribute__((pcs("aapcs"))) func_00100100(float a, double b);')
        self.assertEqual(layout["parameters"], [["r0"], ["r2", "r3"]])

    def test_missing_float_and_void_result_are_separate(self) -> None:
        site = {"site": argcheck.address(BASE), "arguments": [{"register": "s0"}], "results": [{"register": "s0", "read": argcheck.address(BASE + 4)}]}
        issues, _, _ = argcheck.compare(declarations("void func_00100100(void) {}"), [site])
        self.assertEqual({i["kind"] for i in issues}, {"missing_float_parameters", "void_result_consumed"})

    def test_excess_declared_parameters_are_only_notes(self) -> None:
        site = {"site": argcheck.address(BASE), "arguments": [{"register": "r0"}], "results": []}
        issues, notes, _ = argcheck.compare(declarations("int func_00100100(int a, int b) {}"), [site])
        self.assertEqual(issues, [])
        self.assertEqual(notes[0]["registers"], ["r1"])

    def test_unspecified_and_variadic_interfaces_not_false_failures(self) -> None:
        site = {"site": argcheck.address(BASE), "arguments": [{"register": "r3"}], "results": []}
        issues, _, prototypes = argcheck.compare(declarations("int func_00100100() {}"), [site])
        self.assertEqual(issues, [])
        self.assertIn("unchecked", prototypes[0])
        issues, _, _ = argcheck.compare(declarations("int func_00100100(int x, ...);"), [site])
        self.assertEqual(issues, [])
        self.assertEqual(self.layout("int func_00100100(float f, ...);")["parameters"], [["r0"]])

    def test_vfp_stack_exhaustion_and_core_stack_arguments(self) -> None:
        params = ",".join(f"float x{i}" for i in range(17)) + ",int a,int b,int c,int d,int e"
        layout = self.layout(f"void func_00100100({params});")
        self.assertEqual(layout["parameters"][16:], [[], ["r0"], ["r1"], ["r2"], ["r3"], []])


class CallbackTests(unittest.TestCase):
    def test_registration_helper_and_exact_global_dispatch(self) -> None:
        # Registrar receives callback in r0 and writes global+0x20; caller passes
        # the callback address through a literal; dispatcher loads that exact slot.
        words = [0xE59F2008, 0xE5820020, 0xE12FFF1E, 0, BASE + 0x2000,
                 0xE59F0008, branch(BASE + 24, BASE), 0xE12FFF1E, 0, BASE + 0x100,
                 0xE59F400C, 0xE5941020, 0xE5940060, 0xE12FFF31, 0xE12FFF1E, BASE + 0x2000]
        image = fixture(words)
        functions = [function(BASE, BASE + 12), function(BASE + 20, BASE + 32), function(BASE + 40, BASE + 60)]
        sites, events = [], {}
        for fn in functions:
            _, block_list, successors = decode(image, fn)
            sites += argcheck.call_sites(fn, block_list, {f["address"] for f in functions}, {})
            events[fn["address"]] = argcheck.symbolic_events(fn, block_list, successors, image)
        callbacks, unresolved = argcheck.callback_sites(events, sites, {}, {BASE + 0x100})
        self.assertEqual(unresolved, 0)
        self.assertEqual(len(callbacks), 1)
        self.assertEqual(callbacks[0]["target"], BASE + 0x100)
        self.assertEqual(registers(callbacks[0]), {"r0"})
        self.assertEqual(callbacks[0]["storage"], {"address": argcheck.address(BASE + 0x2020)})
        self.assertEqual(callbacks[0]["registrations"][0]["kind"], "registration_helper")
        issues, _, _ = argcheck.compare(declarations("void func_00100100(void) {}"), callbacks)
        self.assertEqual(issues[0]["kind"], "missing_core_parameters")

    def test_static_data_pointer_and_different_field_do_not_alias(self) -> None:
        image = fixture([0xE59F400C, 0xE5941000, 0xE3A00001, 0xE12FFF31, 0xE12FFF1E, BASE + 0x1000], rodata=[BASE + 0x100])
        fn = function(BASE, BASE + 20, pools=[[BASE + 20, BASE + 24]])
        _, block_list, successors = decode(image, fn)
        sites = argcheck.call_sites(fn, block_list, set(), {})
        events = {BASE: argcheck.symbolic_events(fn, block_list, successors, image)}
        refs = argcheck.pointer_references(image, {"functions": [fn]}, {BASE + 0x100})
        self.assertEqual(refs, {BASE + 0x100: [BASE + 0x1000]})
        callbacks, _ = argcheck.callback_sites(events, sites, refs, {BASE + 0x100})
        self.assertEqual(len(callbacks), 1)
        callbacks, unresolved = argcheck.callback_sites(events, sites, {BASE + 0x100: [BASE + 0x1004]}, {BASE + 0x100})
        self.assertEqual(callbacks, [])
        self.assertEqual(unresolved, 1)

    def test_symbolic_field_offsets_do_not_match_distinct_roots(self) -> None:
        store = ("store", ("offset", ("parameter", BASE, 0), 8), ("constant", BASE + 0x100), BASE)
        call = ("call", None, {}, BASE + 8, ("load", ("offset", ("parameter", BASE + 4, 0), 8)))
        site = {"site": argcheck.address(BASE + 8), "kind": "indirect", "arguments": [], "results": []}
        callbacks, unresolved = argcheck.callback_sites({BASE: [store], BASE + 4: [call]}, [site], {}, {BASE + 0x100})
        self.assertEqual(callbacks, [])
        self.assertEqual(unresolved, 1)

    def test_preserved_global_pointer_survives_branch_and_calls(self) -> None:
        image = fixture([0xE59F4018, branch(BASE + 4, BASE + 12, link=False, condition=0), 0xE3A00000,
                         branch(BASE + 12, BASE + 0x100), 0xE5941020, 0xE5940060, 0xE12FFF31,
                         0xE12FFF1E, BASE + 0x2000])
        fn = function(BASE, BASE + 32)
        _, block_list, successors = decode(image, fn)
        events = argcheck.symbolic_events(fn, block_list, successors, image)
        indirect = [e for e in events if e[0] == "call" and e[1] is None]
        self.assertEqual(indirect[0][4], ("load", ("constant", BASE + 0x2020)))


class InputAndReportTests(unittest.TestCase):
    def test_rewrite_receipt_deduplication_and_no_encoding_in_output(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "receipt.json"
            row = {"offset": 4, "rule": "inline_nop", "target": BASE + 0x100, "word": "synthetic-private-value"}
            path.write_text(json.dumps({"units": [{"linker_rewrites": {"func_00100000": [row, row]}}]}))
            functions = {BASE: function(BASE, BASE + 8), BASE + 0x100: function(BASE + 0x100, BASE + 0x104)}
            rewrites = argcheck.load_rewrites([path, path], functions, {}, Path(tmp))
            self.assertEqual(len(rewrites[BASE + 4]), 1)
            self.assertNotIn("word", json.dumps(rewrites))
            self.assertNotIn("synthetic-private-value", json.dumps(rewrites))

    def test_removed_callee_has_no_fabricated_original_call_edge(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "receipt.json"
            row = {"offset": 4, "rule": "inline_insn", "target": None,
                   "removed_callee": {"symbols": ["set_field18"], "section": "i.set_field18", "size": 8}}
            functions = {BASE: function(BASE, BASE + 8)}
            path.write_text(json.dumps({"linker_rewrites": {"func_00100000": [row]}}))
            self.assertEqual(argcheck.load_rewrites([path], functions, {}, Path(tmp)), {})
            for patch in ({"target": BASE}, {"offset": 8}, {"removed_callee": {"symbols": [], "size": 8}}):
                path.write_text(json.dumps({"linker_rewrites": {"func_00100000": [{**row, **patch}]}}))
                with self.assertRaises(argcheck.ArgcheckError):
                    argcheck.load_rewrites([path], functions, {}, Path(tmp))

    def test_invalid_rewrite_site_is_refused(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "receipt.json"
            path.write_text(json.dumps({"linker_rewrites": {"func_00100000": [{"offset": 8, "rule": "inline_nop", "target": BASE}]}}))
            with self.assertRaises(argcheck.ArgcheckError):
                argcheck.load_rewrites([path], {BASE: function(BASE, BASE + 4)}, {}, Path(tmp))

    def test_end_to_end_ledgers_override_and_shared_header_deduplication(self) -> None:
        image = fixture([0xE3A00001, 0xE3A01002, branch(BASE + 8, BASE + 16), 0xE12FFF1E, 0xE12FFF1E])
        catalogue = {"image_sha256": image.sha256, "functions": [function(BASE, BASE + 16), function(BASE + 16, BASE + 20)]}
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "config").mkdir()
            (root / "config/compiler_profiles.json").write_text(json.dumps({"profiles": {"game": {"compiler": "synthetic", "flags": []}}}))
            (root / "config/catalogue.json").write_text(json.dumps(catalogue))
            first, second = root / "one.c", root / "two.c"
            first.write_text('# 1 "shared.h"\nextern void func_00100010(unsigned x, unsigned y);\n')
            second.write_text('# 1 "shared.h"\nextern void func_00100010(unsigned x, unsigned y);\n')
            ledger = root / "matches.json"
            ledger.write_text(json.dumps({"units": [{"source": p.name, "profile": "game", "functions": ["func_00100010"]} for p in (first, second)]}))
            with patch.object(argcheck.image_mod.Image, "load", return_value=image), patch.object(argcheck.interfaces, "preprocess", side_effect=lambda unit, root: unit.source.read_text()):
                report = argcheck.audit(root, [ledger], [], {}, [])
                self.assertEqual(len(report["functions"][0]["prototypes"]), 1)
                self.assertEqual(report["summary"]["flagged_functions"], 0)
                historic = root / "historic.c"
                historic.write_text("void func_00100010(void) {}")
                report = argcheck.audit(root, [ledger], ["func_00100010"], {"func_00100010": historic}, [])
            self.assertEqual(report["summary"]["flagged_functions"], 1)
            self.assertEqual(report["functions"][0]["issues"][0]["registers"], ["r0", "r1"])
            serialized = json.dumps(report)
            self.assertNotIn("mnemonic", serialized)
            self.assertNotIn("encoding", serialized)

    def test_cli_writes_json_and_returns_review_status(self) -> None:
        report = {"schema_version": 1, "functions": [], "unchecked": [], "summary": {"flagged_functions": 1, "unchecked_functions": 0}}
        with tempfile.TemporaryDirectory() as tmp, patch.object(argcheck, "audit", return_value=report), contextlib.redirect_stdout(io.StringIO()):
            path = Path(tmp) / "report.json"
            self.assertEqual(argcheck.main(["--json", str(path), "--matches", "one.json", "--matches", "two.json"]), 1)
            self.assertEqual(json.loads(path.read_text()), report)

    def test_cli_input_error_does_not_echo_exception_contents(self) -> None:
        with patch.object(argcheck, "audit", side_effect=argcheck.ArgcheckError("private-input-sentinel")), contextlib.redirect_stderr(io.StringIO()) as output:
            self.assertEqual(argcheck.main(["--json", "unused.json"]), 2)
        self.assertNotIn("private-input-sentinel", output.getvalue())


if __name__ == "__main__":
    unittest.main()
