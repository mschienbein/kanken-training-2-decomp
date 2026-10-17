"""Catalogue-bounded display of synthetic instructions and data in tools/match.py."""
import struct
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import match  # noqa: E402

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
                ctx.model = match.inplace.linker_model("armlink5-inline-branchnop-v5")
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
        ctx.model = match.inplace.linker_model("armlink5-inline-branchnop-v5")
        ctx.literal_model = match.inplace.DEFAULT_LITERAL_MODEL
        with patch.object(match.inplace, "ObjectFile", return_value=obj):
            rows = ctx.compare(Path("bound_copy.o"))
        self.assertEqual(len(rows), 1)
        self.assertEqual((rows[0]["name"], rows[0]["candidate_size"], rows[0]["size"], rows[0]["status"]),
                         ("bound_variant", 4, 8, "match_extent_differs"))
        self.assertEqual(ctx.removed_callees, [])


class ModelOptionsTest(unittest.TestCase):
    def test_v5_is_explicit_and_inherits_v4_without_broadening_frozen_models(self):
        self.assertEqual(match.inplace.DEFAULT_LINKER_MODEL, "armlink5-inline-branchnop-v2")
        for name in ("strict-v1", "armlink5-inline-branchnop-v2", "armlink5-inline-branchnop-v3"):
            policy = match.inplace.linker_model(name)
            self.assertFalse(policy.function_entry_aliases)
            self.assertFalse(policy.bounded_switches)
        v4 = match.inplace.linker_model("armlink5-inline-branchnop-v4")
        v5 = match.inplace.linker_model("armlink5-inline-branchnop-v5")
        self.assertTrue(v4.bounded_switches)
        self.assertFalse(v4.function_entry_aliases)
        self.assertTrue(v5.function_entry_aliases)
        for field in ("inline", "branchnop", "removed_inline", "bounded_switches"):
            self.assertEqual(getattr(v5, field), getattr(v4, field))

    def test_literal_option_composes_with_frozen_v3_and_selected_successors(self):
        profile = match.toolchain.Profile("game", "synthetic", ())
        for model in ("armlink5-inline-branchnop-v3", "armlink5-inline-branchnop-v4", "armlink5-inline-branchnop-v5"):
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


if __name__ == "__main__":
    unittest.main()
