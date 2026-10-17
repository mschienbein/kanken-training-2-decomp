"""Catalogue-bounded display, placement refusals and CLI exit semantics in tools/match.py."""
import io
import json
import struct
import sys
import tempfile
import unittest
from contextlib import ExitStack, redirect_stderr, redirect_stdout
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import match  # noqa: E402
from tests.test_inplace import BASE as IMAGE_BASE, placement_failure_fixture  # noqa: E402

BASE = 0x10007C


class ProposedNamesTest(unittest.TestCase):
    NAMES = {"__aeabi_memcpy": {"address": 0x1A000C}}

    def test_proposals_add_names(self):
        merged = match.merge_names(self.NAMES, {"__aeabi_vec_ctor_nocookie_nodtor": {"address": 0x1911CC}})
        self.assertEqual(merged["__aeabi_vec_ctor_nocookie_nodtor"]["address"], 0x1911CC)
        self.assertEqual(merged["__aeabi_memcpy"]["address"], 0x1A000C)

    def test_proposals_never_rebind_a_name(self):
        with self.assertRaisesRegex(match.inplace.LinkError, "__aeabi_memcpy"):
            match.merge_names(self.NAMES, {"__aeabi_memcpy": {"address": 0x1A0010}})

    def test_restating_a_binding_is_allowed(self):
        self.assertEqual(match.merge_names(self.NAMES, dict(self.NAMES)), self.NAMES)


class RenderTest(unittest.TestCase):
    def test_midword_literal_and_padding(self):
        data = struct.pack("<4I", *([0xE12FFF1E] * 4))
        literals = [(BASE + 9, BASE + 12)]
        for code_end in (BASE + 8, BASE + 9):
            with self.subTest(code_end=code_end):
                ranges = [(BASE, code_end), (BASE + 12, BASE + 16)]
                rows = match.render(data, BASE, "arm", ranges, literals)
                self.assertEqual([(a, n) for a, n, _ in rows], [
                    (BASE, 4), (BASE + 4, 4), (BASE + 8, 1), (BASE + 9, 3), (BASE + 12, 4)])
                self.assertEqual(rows[2][2], ".word 0x1e")
                self.assertEqual(rows[3][2], ".word 0xe12fff")
                self.assertEqual(rows[4][2], "bx lr")

    def test_pool_word_and_unlabelled_padding(self):
        data = struct.pack("<4I", 0xE12FFF1E, 0x12345678, 0xE12FFF1E, 0xE12FFF1E)
        rows = match.render(data, BASE, "arm", [(BASE, BASE + 4), (BASE + 12, BASE + 16)],
                            [(BASE + 4, BASE + 8)])
        self.assertEqual([row[2] for row in rows], [
            "bx lr", ".word 0x12345678", ".word 0xe12fff1e", "bx lr"])

    def test_partial_pool_does_not_consume_following_code(self):
        data = b"\x00\x00" + struct.pack("<H", 0x4770)
        rows = match.render(data, BASE, "thumb", [(BASE + 2, BASE + 4)], [(BASE, BASE + 2)])
        self.assertEqual(rows, [(BASE, 2, ".word 0x0000"), (BASE + 2, 2, "bx lr")])

    def test_thumb_wide_instruction_cannot_cross_boundary(self):
        data = bytes.fromhex("00f000f8")  # synthetic Thumb BL
        for stop, expected in (
            (1, [(BASE, 1), (BASE + 1, 3)]),
            (2, [(BASE, 2), (BASE + 2, 2)]),
            (3, [(BASE, 2), (BASE + 2, 1), (BASE + 3, 1)]),
        ):
            with self.subTest(stop=stop):
                rows = match.render(data, BASE, "thumb", [(BASE, BASE + stop)],
                                    [(BASE + stop, BASE + 4)])
                self.assertEqual([(a, n) for a, n, _ in rows], expected)
                self.assertTrue(all(text.startswith((".inst", ".word")) for _, _, text in rows))

    def test_candidate_bytes_outside_catalogue_are_data(self):
        data = struct.pack("<2I", 0xE12FFF1E, 0xE12FFF1E)
        rows = match.render(data, BASE, "arm", [(BASE, BASE + 4)], [])
        self.assertEqual(rows[1], (BASE + 4, 4, ".word 0xe12fff1e"))

    def test_adjacent_literal_and_pool_stay_separate(self):
        data = struct.pack("<5I", *([0xE12FFF1E] * 5))
        rows = match.render(data, BASE, "arm", [(BASE, BASE + 8), (BASE + 16, BASE + 20)],
                            [(BASE + 12, BASE + 16), (BASE + 9, BASE + 12)])
        self.assertEqual(rows[2:5], [
            (BASE + 8, 1, ".word 0x1e"),
            (BASE + 9, 3, ".word 0xe12fff"),
            (BASE + 12, 4, ".word 0xe12fff1e")])
        self.assertEqual(rows[-1], (BASE + 16, 4, "bx lr"))

    def test_unaligned_code_ranges_only_decode_complete_words(self):
        data = struct.pack("<4I", *([0xE12FFF1E] * 4))
        rows = match.render(data, BASE, "arm", [(BASE + 1, BASE + 7), (BASE + 8, BASE + 16)], [])
        self.assertEqual([(a, n) for a, n, _ in rows], [
            (BASE, 1), (BASE + 1, 3), (BASE + 4, 3), (BASE + 7, 1),
            (BASE + 8, 4), (BASE + 12, 4)])
        self.assertTrue(all(text.startswith(".word") for _, _, text in rows[:4]))
        self.assertEqual([text for _, _, text in rows[4:]], ["bx lr", "bx lr"])

    def test_thumb_narrow_and_wide_instructions_fit_code_ranges(self):
        data = bytes.fromhex("704700f000f87047") + struct.pack("<I", 0x12345678) + bytes.fromhex("7047")
        rows = match.render(data, BASE, "thumb", [(BASE, BASE + 8), (BASE + 12, BASE + 14)],
                            [(BASE + 8, BASE + 12)])
        self.assertEqual([(a, n) for a, n, _ in rows], [
            (BASE, 2), (BASE + 2, 4), (BASE + 6, 2), (BASE + 8, 4), (BASE + 12, 2)])
        self.assertEqual([text.split()[0] for _, _, text in rows], ["bx", "bl", "bx", ".word", "bx"])

    def test_thumb_wide_instruction_cannot_cross_adjacent_code_ranges(self):
        rows = match.render(bytes.fromhex("00f000f8"), BASE, "thumb",
                            [(BASE, BASE + 2), (BASE + 2, BASE + 4)], [])
        self.assertEqual(rows, [(BASE, 2, ".inst 0xf000"), (BASE + 2, 2, ".inst 0xf800")])

    def test_data_overrides_overlapping_code_range(self):
        data = struct.pack("<4I", *([0xE12FFF1E] * 4))
        rows = match.render(data, BASE, "arm", [(BASE, BASE + 16)],
                            [(BASE + 4, BASE + 8), (BASE + 9, BASE + 12)])
        self.assertEqual([(a, n) for a, n, _ in rows], [
            (BASE, 4), (BASE + 4, 4), (BASE + 8, 1), (BASE + 9, 3), (BASE + 12, 4)])
        self.assertEqual([text.split()[0] for _, _, text in rows], ["bx", ".word", ".word", ".word", "bx"])

    def test_image_end_clips_partial_literal(self):
        data = struct.pack("<3I", *([0xE12FFF1E] * 3))[:10]
        rows = match.render(data, BASE, "arm", [(BASE, BASE + 8)], [(BASE + 9, BASE + 12)])
        self.assertEqual(rows[-2:], [(BASE + 8, 1, ".word 0x1e"), (BASE + 9, 1, ".word 0xff")])


class DiffLinesTest(unittest.TestCase):
    def test_midword_literal_is_data_on_both_sides(self):
        original = struct.pack("<3I", *([0xE12FFF1E] * 3))
        candidate = struct.pack("<3I", *([0xE1A00000] * 3))
        fn = {"isa": "arm", "code_ranges": [(BASE, BASE + 8)],
              "pools": [], "literals": [(BASE + 9, BASE + 12)]}
        lines = match.diff_lines(original, candidate, BASE, fn, 20)
        self.assertEqual(len(lines), 4)
        for index, offset, left, right in (
            (2, 8, ".word 0x1e", ".word 0x00"),
            (3, 9, ".word 0xe12fff", ".word 0xe1a000"),
        ):
            self.assertEqual(lines[index], f"! {BASE + offset:08x}  {left:<40} {right}")

    def test_thumb_width_changes_keep_data_aligned_on_both_sides(self):
        narrow = bytes.fromhex("70477047") + struct.pack("<I", 0x12345678) + bytes.fromhex("7047")
        wide = bytes.fromhex("00f000f8") + narrow[4:]
        fn = {"isa": "thumb", "kind": "multi_entry",
              "entries": [{"address": BASE, "isa": "thumb"}, {"address": BASE + 8, "isa": "thumb"}],
              "code_ranges": [(BASE, BASE + 4), (BASE + 8, BASE + 10)],
              "pools": [(BASE + 4, BASE + 8)], "literals": []}
        for original, candidate, left, right in (
            (narrow, wide, "bx lr", ""), (wide, narrow, "", "bx lr"),
        ):
            with self.subTest(left=left):
                lines = match.diff_lines(original, candidate, BASE, fn, 20)
                self.assertEqual([int(line[2:10], 16) for line in lines],
                                 [BASE, BASE + 2, BASE + 4, BASE + 8])
                self.assertEqual(lines[1], f"! {BASE + 2:08x}  {left:<40} {right}")
                pool = ".word 0x12345678"
                self.assertEqual(lines[2], f"  {BASE + 4:08x}  {pool:<40} {pool}")
                self.assertEqual(lines[3], f"  {BASE + 8:08x}  {'bx lr':<40} bx lr")

    def test_extent_bytes_are_data_on_either_side(self):
        short = struct.pack("<I", 0xE12FFF1E)
        long = short * 2
        fn = {"isa": "arm", "code_ranges": [(BASE, BASE + 4)], "pools": [], "literals": []}
        for original, candidate, left, right in (
            (short, long, "", ".word 0xe12fff1e"), (long, short, ".word 0xe12fff1e", ""),
        ):
            with self.subTest(left=left):
                lines = match.diff_lines(original, candidate, BASE, fn, 20)
                self.assertEqual(len(lines), 2)
                self.assertEqual(lines[1], f"! {BASE + 4:08x}  {left:<40} {right}")


@unittest.skipUnless(match.toolchain.armcc_exe("armcc_504_82").is_file(), "pinned compiler not installed (tools/setup.py)")
class CppAliasCompareTest(unittest.TestCase):
    def test_result_names_the_bound_abi_alias_and_compares_the_whole_section(self):
        with tempfile.TemporaryDirectory() as tmp:
            source = Path(tmp) / "alias.cpp"
            source.write_text("class Plain { public: ~Plain(); };\nPlain::~Plain() {}\n", encoding="utf-8")
            obj_path = source.with_suffix(".o")
            match.toolchain.compile_source(match.toolchain.load_profiles()["game"], source, obj_path)
            body = struct.pack("<I", 0xE12FFF1E)
            catalogue = {BASE: {"address": BASE, "size": len(body), "isa": "arm"}}

            class Image:
                bss = (0x200000, 0x201000)

                def read(self, address, size):
                    if address != BASE or size != len(body):
                        raise AssertionError("comparison must cover the complete synthetic function")
                    return body

            for model in match.inplace.LINKER_MODELS:
                for name in ("_ZN5PlainD1Ev", "_ZN5PlainD2Ev"):
                    with self.subTest(model=model, name=name):
                        ctx = match.Context.__new__(match.Context)
                        ctx.image, ctx.functions = Image(), catalogue
                        ctx.resolver = match.inplace.Resolver(catalogue, {name: {"address": BASE}})
                        ctx.model = match.inplace.linker_model(model)
                        ctx.literal_model = match.inplace.DEFAULT_LITERAL_MODEL
                        if not ctx.model.function_entry_aliases:
                            with self.assertRaisesRegex(match.inplace.LinkError, "unresolved symbol"):
                                ctx.compare(obj_path)
                            continue
                        rows = ctx.compare(obj_path)
                        self.assertEqual(len(rows), 1)
                        self.assertEqual((rows[0]["name"], rows[0]["candidate_size"], rows[0]["status"]),
                                         (name, len(body), "match"))
                        self.assertEqual(rows[0]["_candidate"], body)
                        self.assertIsInstance(rows[0], match.Comparison)
                        for render in (repr, str):
                            self.assertNotIn("_original", render(rows[0]))
                            self.assertNotIn("_candidate", render(rows[0]))
                            self.assertNotIn(repr(body), render(rows[0]))
                        self.assertEqual(ctx.removed_callees, [])


class AliasCompareSafetyTest(unittest.TestCase):
    def test_extra_comdat_copy_cannot_hide_a_bound_alias_from_comparison(self):
        from tests.test_inplace import RemovedCalleeModelTest

        for contradictory, original_size in ((True, 8), (False, 12)):
            with self.subTest(contradictory=contradictory, original_size=original_size):
                objects, mem, catalogue, names = RemovedCalleeModelTest().comdat_alias_fixture(
                    contradictory, original_size)
                mem.bss = (0x200000, 0x201000)
                ctx = match.Context.__new__(match.Context)
                ctx.image, ctx.functions = mem, catalogue
                ctx.resolver = match.inplace.Resolver(catalogue, names)
                ctx.model = match.inplace.linker_model("armlink5-inline-branchnop-v6")
                ctx.literal_model = match.inplace.DEFAULT_LITERAL_MODEL
                combine = match.inplace.ObjectFile.combine

                def object_file(path):
                    return objects[0 if path.name == "caller.o" else 1]

                object_file.combine = combine
                with patch.object(match.inplace, "ObjectFile", object_file):
                    with self.assertRaisesRegex(match.inplace.LinkError, "mixes original-bound and removable callee copies"):
                        ctx.compare(Path("caller.o"), [Path("bound_copy.o")])

    def test_a_bound_zero_size_alias_does_not_credit_a_short_complete_section(self):
        from tests.test_inplace import BX_LR, RemovedCalleeModelTest

        obj, mem, catalogue = RemovedCalleeModelTest().fixture(BX_LR)
        obj.sections.pop(1)
        obj.symbols.pop(0)
        obj.symbols.append(match.inplace.Symbol("bound_variant", 0, 0, "STT_FUNC", "STB_GLOBAL", 2))
        address = 0x100100
        catalogue[address] = {"address": address, "size": 8, "isa": "arm"}
        mem.write(address, BX_LR + BX_LR)
        mem.bss = (0x200000, 0x201000)
        ctx = match.Context.__new__(match.Context)
        ctx.image, ctx.functions = mem, catalogue
        ctx.resolver = match.inplace.Resolver(catalogue, {"bound_variant": {"address": address}})
        ctx.model = match.inplace.linker_model("armlink5-inline-branchnop-v6")
        ctx.literal_model = match.inplace.DEFAULT_LITERAL_MODEL
        with patch.object(match.inplace, "ObjectFile", return_value=obj):
            rows = ctx.compare(Path("bound_copy.o"))
        self.assertEqual(len(rows), 1)
        self.assertEqual((rows[0]["name"], rows[0]["candidate_size"], rows[0]["size"], rows[0]["status"]),
                         ("bound_variant", 4, 8, "match_extent_differs"))
        self.assertEqual(ctx.removed_callees, [])

    def explicit_bound_pool_fixture(self, code_size=4, pool_size=4):
        from tests.test_inplace import BX_LR, Memory

        body = BX_LR * (code_size // 4) + bytes(range(1, pool_size + 1))
        complete = match.inplace.Symbol("complete_entry", 0, code_size, "STT_FUNC", "STB_GLOBAL", 1)
        zero = match.inplace.Symbol("base_entry", 0, 0, "STT_FUNC", "STB_GLOBAL", 1)
        obj = match.inplace.ObjectFile.__new__(match.inplace.ObjectFile)
        obj.symbols = [complete, zero]
        obj.sections = {1: match.inplace.Section(
            1, "i.complete_entry", body, len(body), False, True, False,
            mapping=[(0, "$a"), (code_size, "$d")], align=4)}
        mem = Memory()
        mem.bss = (0x200000, 0x201000)
        mem.write(BASE, body)
        catalogue = {BASE: {"address": BASE, "size": len(body), "isa": "arm"}}
        names = {sym.name: {"address": BASE} for sym in obj.symbols}
        return obj, mem, catalogue, names

    def test_explicit_bound_aliases_preserve_synthetic_and_reported_pool_extents(self):
        cases = (("synthetic", 4, 4), ("func_001c6a68", 100, 4), ("func_001ef89c", 188, 8))
        for case, code_size, pool_size in cases:
            for model in match.inplace.LINKER_MODELS:
                with self.subTest(case=case, model=model):
                    obj, mem, catalogue, names = self.explicit_bound_pool_fixture(code_size, pool_size)
                    self.assertEqual(match.inplace.function_aliases(obj), {})
                    metadata = [vars(sym).copy() for sym in obj.symbols]
                    ctx = match.Context.__new__(match.Context)
                    ctx.image, ctx.functions = mem, catalogue
                    ctx.resolver = match.inplace.Resolver(catalogue, names)
                    ctx.model = match.inplace.linker_model(model)
                    ctx.literal_model = match.inplace.DEFAULT_LITERAL_MODEL
                    with patch.object(match.inplace, "ObjectFile", return_value=obj):
                        rows = ctx.compare(Path("explicit.o"))
                    self.assertEqual(len(rows), 1)
                    self.assertEqual((rows[0]["status"], rows[0]["size"], rows[0]["candidate_size"]),
                                     ("match", code_size + pool_size, code_size + pool_size))
                    self.assertEqual(rows[0]["_candidate"], mem.read(BASE, code_size + pool_size))
                    self.assertEqual([vars(sym) for sym in obj.symbols], metadata)
                    self.assertEqual((ctx.removed_callees, rows[0]["linker_rewrites"]), ([], []))

    def test_explicit_bound_pool_aliases_do_not_borrow_missing_bindings_or_skip_bytes(self):
        for missing in ("complete_entry", "base_entry"):
            obj, mem, catalogue, names = self.explicit_bound_pool_fixture()
            names.pop(missing)
            linker = match.inplace.InPlaceLinker(
                mem, catalogue, match.inplace.Resolver(catalogue, names), mem.bss,
                "armlink5-inline-branchnop-v6")
            with self.subTest(missing=missing), self.assertRaises(match.inplace.LinkError):
                linker.link(obj)
        for failure in ("pool_mismatch", "extent"):
            obj, mem, catalogue, names = self.explicit_bound_pool_fixture()
            if failure == "pool_mismatch":
                mem.write(BASE + 4, b"\0" * 4)
            else:
                catalogue[BASE]["size"] += 4
            ctx = match.Context.__new__(match.Context)
            ctx.image, ctx.functions = mem, catalogue
            ctx.resolver = match.inplace.Resolver(catalogue, names)
            ctx.model = match.inplace.linker_model("armlink5-inline-branchnop-v6")
            ctx.literal_model = match.inplace.DEFAULT_LITERAL_MODEL
            with self.subTest(failure=failure), patch.object(match.inplace, "ObjectFile", return_value=obj):
                rows = ctx.compare(Path("explicit.o"))
                self.assertEqual(rows[0]["status"],
                                 "mismatch" if failure == "pool_mismatch" else "match_extent_differs")


class ModelOptionsTest(unittest.TestCase):
    def test_v6_is_explicit_and_inherits_v4_without_broadening_frozen_models(self):
        self.assertEqual(match.inplace.DEFAULT_LINKER_MODEL, "armlink5-inline-branchnop-v2")
        for name in ("strict-v1", "armlink5-inline-branchnop-v2", "armlink5-inline-branchnop-v3"):
            policy = match.inplace.linker_model(name)
            self.assertFalse(policy.function_entry_aliases)
            self.assertFalse(policy.bounded_switches)
        v4 = match.inplace.linker_model("armlink5-inline-branchnop-v4")
        v6 = match.inplace.linker_model("armlink5-inline-branchnop-v6")
        self.assertTrue(v4.bounded_switches)
        self.assertFalse(v4.function_entry_aliases)
        self.assertTrue(v6.function_entry_aliases)
        for field, value in vars(v4).items():
            if field not in ("name", "function_entry_aliases"):
                self.assertEqual(getattr(v6, field), value)

    def test_v5_is_retired_instead_of_reinterpreted_as_v6(self):
        self.assertNotIn("armlink5-inline-branchnop-v5", match.inplace.LINKER_MODELS)
        with self.assertRaisesRegex(match.inplace.LinkError, "retired.*superseded by v6, never adopted"):
            match.inplace.linker_model("armlink5-inline-branchnop-v5")

    def test_literal_option_composes_with_frozen_v3_and_selected_successors(self):
        profile = match.toolchain.Profile("game", "synthetic", ())
        for model in ("armlink5-inline-branchnop-v3", "armlink5-inline-branchnop-v4", "armlink5-inline-branchnop-v6"):
            with self.subTest(model=model), \
                    patch.object(match.toolchain, "load_profiles", return_value={"game": profile}), \
                    patch.object(match, "run", return_value=0) as run:
                code = match.main(["candidate.cpp", "--profile", "game", "--linker-model", model,
                                   "--literal-model", "anonymous-string-records-v1"])
                self.assertEqual(code, 0)
                self.assertEqual(run.call_args.args[5], model)
                self.assertEqual(run.call_args.args[-1], "anonymous-string-records-v1")

    def test_literal_option_is_off_by_default(self):
        profile = match.toolchain.Profile("game", "synthetic", ())
        with patch.object(match.toolchain, "load_profiles", return_value={"game": profile}), \
                patch.object(match, "run", return_value=0) as run:
            self.assertEqual(match.main(["candidate.cpp", "--profile", "game"]), 0)
        self.assertEqual(run.call_args.args[5], match.inplace.DEFAULT_LINKER_MODEL)
        self.assertEqual(run.call_args.args[-1], "none")


class PlacementRefusalTest(unittest.TestCase):
    def invoke(self, obj, image, catalogue, model=match.inplace.DEFAULT_LINKER_MODEL):
        if not hasattr(image, "sha256"):
            image.sha256 = "synthetic-image"
        if not hasattr(image, "bss"):
            image.bss = (0x300000, 0x301000)
        with tempfile.TemporaryDirectory() as directory, ExitStack() as stack:
            root = Path(directory)
            source = root / "candidate.c"
            source.write_text("void func_00100000(void) {}\n", encoding="utf-8")
            compiler = root / "compiler"
            compiler.write_bytes(b"synthetic compiler hash input")
            catalogue_path = root / "catalogue.json"
            catalogue_record = {"image_sha256": image.sha256, "functions": list(catalogue.values())}
            catalogue_path.write_text(json.dumps(catalogue_record), encoding="utf-8")
            out_dir = root / "out"
            profile = match.toolchain.Profile("game", "synthetic", ())

            def compile_source(profile, source, output):
                output.parent.mkdir(parents=True, exist_ok=True)
                output.write_bytes(b"synthetic object hash input")

            for module, name, kwargs in (
                (match.toolchain, "load_profiles", {"return_value": {"game": profile}}),
                (match.toolchain, "compile_source", {"side_effect": compile_source}),
                (match.toolchain, "armcc_exe", {"return_value": compiler}),
                (match.image_mod.Image, "load", {"return_value": image}),
                (match.image_mod, "load_catalogue", {"return_value": catalogue_record}),
                (match.image_mod, "load_names", {"return_value": {}}),
                (match.inplace, "ObjectFile", {"return_value": obj}),
            ):
                stack.enter_context(patch.object(module, name, **kwargs))
            stdout, stderr = io.StringIO(), io.StringIO()
            with redirect_stdout(stdout), redirect_stderr(stderr):
                code = match.main([str(source), "--profile", "game", "--catalogue", str(catalogue_path),
                                   "--out", str(out_dir), "--show", "0", "--linker-model", model])
            record = json.loads((out_dir / "candidate.result.json").read_text(encoding="utf-8"))
            return code, record, stderr.getvalue()

    def test_preserved_crash_classes_exit_two_and_record_the_placement_reason(self):
        for base, name, offset in ((0xE1A04000, ".constdata", 0), (0, ".rodata._ZTI6Layout", 4)):
            with self.subTest(section=name):
                obj, image, catalogue = placement_failure_fixture(base, name, offset)
                code, record, stderr = self.invoke(obj, image, catalogue)
                self.assertEqual(code, 2)
                self.assertEqual(record["status"], "refused")
                self.assertEqual(record["error_type"], "LinkError")
                self.assertEqual(record["functions"], [])
                self.assertIn(f"{name}+{offset:#x}", record["reason"])
                self.assertIn("witness address redacted", record["reason"])
                if base:
                    self.assertNotIn(f"{base + offset:#x}", json.dumps(record) + stderr)
                self.assertIn(record["reason"], stderr)
                self.assertNotIn("Traceback", stderr)

    def test_ordinary_constdata_refusal_is_not_exit_one(self):
        code, record, stderr = self.invoke(*placement_failure_fixture(0xE1A04000, nested=False))
        self.assertEqual(code, 2)
        self.assertEqual(record["status"], "refused")
        self.assertIn(".constdata", record["reason"])
        self.assertNotIn("Traceback", stderr)

    def test_refusal_text_and_receipts_redact_unvalidated_witness_words(self):
        for failure in ("outside", "alignment", "conflict"):
            with self.subTest(failure=failure):
                witness = 0xE1A04004 if failure == "alignment" else 0xE1A04000
                witnesses = [witness]
                obj, image, catalogue = placement_failure_fixture(witness, nested=False)
                if failure == "alignment":
                    obj.sections[2].align = 8
                elif failure == "conflict":
                    witnesses.append(0xE1A05000)
                    obj.sections[1].data, obj.sections[1].size = bytes(8), 8
                    obj.sections[1].relocs.append(
                        match.inplace.Reloc(4, match.inplace.R_ARM_ABS32, obj.symbols[1])
                    )
                    image.write(IMAGE_BASE, struct.pack("<2I", *witnesses))
                code, record, stderr = self.invoke(obj, image, catalogue)
                self.assertEqual(code, 2)
                self.assertEqual(record["status"], "refused")
                self.assertIn(".constdata", record["reason"])
                serialized = (json.dumps(record) + stderr).lower()
                for word in witnesses:
                    self.assertNotIn(f"{word:#x}", serialized)
                    self.assertNotIn(str(word), serialized)
                expected_reason = {
                    "outside": "outside the image", "alignment": "not aligned",
                    "conflict": "references disagree",
                }[failure]
                self.assertIn(expected_reason, record["reason"])

    def test_removed_call_context_read_keeps_redacted_refusal_receipts(self):
        from tests.test_inplace import RemovedCalleeModelTest

        for model in ("armlink5-inline-branchnop-v3", "armlink5-inline-branchnop-v4",
                      "armlink5-inline-branchnop-v6"):
            for error in (ValueError, match.image_mod.ImageError):
                with self.subTest(model=model, error=error.__name__):
                    obj, image, catalogue = RemovedCalleeModelTest().fixture()

                    def invalid_read(address, size):
                        raise error("synthetic private witness 0xe1a04000")

                    image.read = invalid_read
                    code, record, stderr = self.invoke(obj, image, catalogue, model)
                    self.assertEqual(code, 2)
                    self.assertEqual(record["status"], "refused")
                    self.assertEqual(record["error_type"], "LinkError")
                    self.assertEqual(record["linker_model"], model)
                    self.assertEqual(record["functions"], [])
                    self.assertIn("i.caller: removed-call context", record["reason"])
                    self.assertIn("witness address redacted", record["reason"])
                    self.assertNotIn("0xe1a04000", json.dumps(record) + stderr)

    def test_alias_offset_refuses_before_a_missing_catalogue_key_is_used(self):
        obj, image, catalogue = placement_failure_fixture(0)
        obj.sections[1].relocs = []
        obj.symbols = [match.inplace.Symbol("func_00100000", 4, 4, "STT_FUNC", "STB_GLOBAL", 1)]
        code, record, _ = self.invoke(obj, image, catalogue)
        self.assertEqual(code, 2)
        self.assertIn("section base 0xffffc", record["reason"])
        self.assertIn("not a catalogued function start", record["reason"])

    def test_byte_and_extent_comparison_exits_are_unchanged(self):
        for size, original, expected_exit, expected_status in (
            (4, 0, 0, "match"), (4, 1, 1, "mismatch"), (8, 0, 1, "match_extent_differs"),
        ):
            with self.subTest(status=expected_status):
                obj, image, catalogue = placement_failure_fixture(original)
                obj.sections[1].relocs = []
                obj.sections[1].data = bytes(size)
                obj.sections[1].size = size
                code, record, stderr = self.invoke(obj, image, catalogue)
                self.assertEqual(code, expected_exit)
                self.assertEqual(record["functions"][0]["status"], expected_status)
                self.assertNotIn("reason", record)
                self.assertEqual(stderr, "")

    def test_candidate_without_functions_is_a_recorded_refusal(self):
        obj, image, catalogue = placement_failure_fixture(0)
        obj.sections, obj.symbols = {}, []
        code, record, _ = self.invoke(obj, image, catalogue)
        self.assertEqual(code, 2)
        self.assertIn("no surviving catalogued functions", record["reason"])

    def test_ordinary_compare_valueerror_at_image_end_uses_catalogue_extent(self):
        ctrfmt = match.image_mod.ctrfmt
        layout = ctrfmt.CodeLayout(
            "SYNTH", False, ctrfmt.CodeSegment("text", IMAGE_BASE, 1, 0x1000),
            ctrfmt.CodeSegment("rodata", IMAGE_BASE + 0x1000, 0, 0),
            ctrfmt.CodeSegment("data", 0x200000, 0, 0), 0x1000, 0, (),
        )
        image = match.image_mod.Image(bytes(0x1000), layout)
        obj, _, _ = placement_failure_fixture(0)
        obj.sections[1].relocs = []
        obj.sections[1].data, obj.sections[1].size = bytes(8), 8
        obj.symbols = [match.inplace.Symbol("func_00100ffc", 0, 8, "STT_FUNC", "STB_GLOBAL", 1)]
        catalogue = {IMAGE_BASE + 0xFFC: {"address": IMAGE_BASE + 0xFFC, "size": 4, "isa": "arm"}}
        code, record, stderr = self.invoke(obj, image, catalogue)
        self.assertEqual(code, 1)
        self.assertEqual(record["functions"][0]["status"], "mismatch")
        self.assertEqual(record["functions"][0]["first_diff"], 4)
        self.assertEqual(stderr, "")

    def test_inferred_section_crossing_segments_is_a_contextual_refusal(self):
        ctrfmt = match.image_mod.ctrfmt
        layout = ctrfmt.CodeLayout(
            "SYNTH", False, ctrfmt.CodeSegment("text", IMAGE_BASE, 1, 0x1000),
            ctrfmt.CodeSegment("rodata", IMAGE_BASE + 0x2000, 1, 0x1000),
            ctrfmt.CodeSegment("data", IMAGE_BASE + 0x4000, 1, 0x1000), 0x1000, 0, (),
        )
        code_bytes = bytearray(0x3000)
        struct.pack_into("<I", code_bytes, 0, IMAGE_BASE + 0xFFC)
        image = match.image_mod.Image(bytes(code_bytes), layout)
        obj, _, catalogue = placement_failure_fixture(IMAGE_BASE + 0xFFC, nested=False)
        obj.sections[2].data, obj.sections[2].size = bytes(0x1008), 0x1008
        code, record, _ = self.invoke(obj, image, catalogue)
        self.assertEqual(code, 2)
        self.assertIn(".constdata", record["reason"])
        self.assertIn("crosses a segment boundary", record["reason"])

    def test_internal_valueerror_and_keyerror_remain_visible(self):
        for error in (ValueError("internal arithmetic"), KeyError("internal invariant")):
            with self.subTest(error=type(error).__name__), \
                    patch.object(match.inplace.InPlaceLinker, "place", side_effect=error):
                with self.assertRaises(type(error)):
                    self.invoke(*placement_failure_fixture(0))


if __name__ == "__main__":
    unittest.main()
