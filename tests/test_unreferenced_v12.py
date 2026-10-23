"""V12 regressions for the sealed v11 semantic-section-metadata finding."""
from __future__ import annotations

import io
import json
import struct
import sys
import tempfile
import unittest
import zlib
from contextlib import redirect_stderr, redirect_stdout
from pathlib import Path

from tests import test_inplace as seeds
from tests.elf_fixture import serialize_object

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import collect
import compid
import inplace
import match

MODEL = "armlink5-inline-branchnop-v12"
PREVIOUS = "armlink5-inline-branchnop-v11"


def replace_header(data: bytes, section_name: str, **fields) -> bytes:
    """Change real section-header words in an independently generated ELF fixture."""
    elf = inplace.ELFFile(io.BytesIO(data))
    offset = elf["e_shoff"] + elf.get_section_index(section_name) * elf["e_shentsize"]
    words = {"name": 0, "type": 1, "flags": 2, "address": 3, "size": 5,
             "link": 6, "info": 7, "align": 8, "entsize": 9}
    result = bytearray(data)
    for field, value in fields.items():
        struct.pack_into("<I", result, offset + 4 * words[field], value)
    return bytes(result)


def parse(data: bytes) -> inplace.ObjectFile:
    return inplace.ObjectFile(Path("independent-metadata-fixture.o"), data=data)


def linker(memory, catalogue, names, model=MODEL):
    return inplace.InPlaceLinker(memory, catalogue, inplace.Resolver(catalogue, names),
                                 (0x200000, 0x201000), model)


def fixture(headers=None, *, prefix=False, r1=False):
    if r1:
        obj, memory, catalogue = seeds.RemovedUnwindModelTest().fixture("extab")
        names = {}
        for index in (2, 3, 4):
            obj.sections[index].comdat = "removed_group"
        obj.section_groups = [(1, "removed_group", {2, 3, 4})]
        obj.sections[1].comdat = "caller_group"
        obj.section_groups.append((1, "caller_group", {1}))
    else:
        obj, memory, catalogue, names = seeds.UnreferencedDestructorUnwindModelTest().fixture("extab", True)
        obj.sections[1].comdat = "D1_group"
        obj.section_groups.append((1, "D1_group", {1}))
    if prefix:
        obj.sections = {5: inplace.Section(5, ".zero.prefix", b"", 0, False, False, False), **obj.sections}
    return serialize_object(obj, headers=headers), memory, catalogue, names


def flags(index):
    return {2: 0x206, 3: 0x282, 4: 0x202}[index]


class CompleteSectionMetadataTest(unittest.TestCase):
    def combine(self, blobs):
        return inplace.ObjectFile.combine([parse(blob) for blob in blobs])

    def refuse(self, blobs, memory, catalogue, names, pattern="."):
        oracle = linker(memory, catalogue, names)
        with self.assertRaisesRegex(inplace.LinkError, pattern):
            oracle.link(self.combine(blobs))
        self.assertEqual(oracle.rewrites, [])
        self.assertEqual(oracle.removed_special_members, [])
        self.assertEqual(oracle.removed_callees, [])

    def test_exact_reviewer_tls_counterexample_refuses_through_context_in_both_orders(self):
        first, memory, catalogue, names = fixture()
        second, _, _, _ = fixture({2: {"flags": flags(2) | 0x400}})
        memory.bss = (0x200000, 0x201000)
        for reverse in (False, True):
            blobs = [second, first] if reverse else [first, second]
            with tempfile.TemporaryDirectory() as directory:
                paths = [Path(directory) / f"object-{index}.o" for index in range(2)]
                for path, blob in zip(paths, blobs):
                    path.write_bytes(blob)
                self.assertEqual(sorted(parse(blob).sections[2].metadata.flags for blob in blobs), [518, 1542])
                for model in (PREVIOUS, MODEL):
                    context = match.Context.__new__(match.Context)
                    context.image, context.functions = memory, catalogue
                    context.resolver = inplace.Resolver(catalogue, names)
                    context.model, context.literal_model = inplace.linker_model(model), "none"
                    if model == PREVIOUS:
                        rows = context.compare(paths[0], paths[1:])
                        self.assertEqual([row["status"] for row in rows], ["match", "match"])
                        self.assertEqual(len(context.section_rewrites), 2)
                    else:
                        with self.assertRaisesRegex(inplace.LinkError, "contradictory"):
                            context.compare(paths[0], paths[1:])
                        self.assertEqual((context.section_rewrites, context.removed_special_members), ([], []))

    def test_every_raw_flag_bit_contradiction_refuses_for_d0_exidx_and_extab_in_both_orders(self):
        for target in (2, 3, 4):
            for bit in range(32):
                for reverse in (False, True):
                    with self.subTest(target=target, bit=bit, reverse=reverse):
                        first, memory, catalogue, names = fixture()
                        fields = {"flags": flags(target) ^ (1 << bit)}
                        if bit == 11:  # A real synthetic ELF compression header, not malformed compressed bytes.
                            obj = seeds.UnreferencedDestructorUnwindModelTest().fixture("extab", True)[0]
                            data = obj.sections[target].data
                            packed = struct.pack("<3I", 1, len(data), 4) + zlib.compress(data)
                            fields.update(data=packed, size=len(packed))
                        second, _, _, _ = fixture({target: fields})
                        blobs = [second, first] if reverse else [first, second]
                        self.refuse(blobs, memory, catalogue, names)

    def test_equal_previously_dropped_flags_are_positive_controls_for_every_own_role(self):
        for target in (2, 3, 4):
            for extra in (0, 0x10, 0x20, 0x40, 0x400):
                for reverse in (False, True):
                    with self.subTest(target=target, extra=extra, reverse=reverse):
                        first, memory, catalogue, names = fixture({target: {"flags": flags(target) | extra}})
                        second, _, _, _ = fixture({target: {"flags": flags(target) | extra}}, prefix=True)
                        blobs = [second, first] if reverse else [first, second]
                        oracle = linker(memory, catalogue, names)
                        self.assertEqual(len(oracle.link(self.combine(blobs))), 2)
                        self.assertEqual(len(oracle.removed_special_members), 2)
                        self.assertTrue(all(len(row["unwind_sections"]) == 2 for row in oracle.removed_special_members))

    def test_header_address_alignment_type_size_and_entsize_are_not_lost(self):
        variants = ((2, {"address": 0x100}), (2, {"align": 8}), (2, {"type": 8}),
                    (3, {"entsize": 8}), (4, {"address": 0x100}), (4, {"align": 8}),
                    (4, {"size": 12, "data": bytes(12)}))
        for target, fields in variants:
            for reverse in (False, True):
                with self.subTest(target=target, fields=sorted(fields), reverse=reverse):
                    first, memory, catalogue, names = fixture()
                    second, _, _, _ = fixture({target: fields})
                    self.refuse([second, first] if reverse else [first, second], memory, catalogue, names)

    def test_unmodeled_sh_link_and_scalar_sh_info_refuse_in_both_orders(self):
        for target in (2, 3, 4):
            for field in ("link", "info"):
                if target == 3 and field == "link":
                    continue  # EXIDX's supported LINK_ORDER target is covered separately.
                for reverse in (False, True):
                    with self.subTest(target=target, field=field, reverse=reverse):
                        first, memory, catalogue, names = fixture()
                        second, _, _, _ = fixture({target: {field: 1}})
                        self.refuse([second, first] if reverse else [first, second], memory, catalogue, names,
                                    "sh_link/sh_info")

    def test_link_and_info_section_targets_remap_by_proved_owner_not_raw_index(self):
        first, memory, catalogue, names = fixture({3: {"flags": flags(3) | 0x40, "info": 2}})
        # The leading zero-sized allocated section moves the actual ELF owner index from 2 to 3.
        second, _, _, _ = fixture({3: {"flags": flags(3) | 0x40, "info": 3}}, prefix=True)
        self.assertNotEqual(parse(first).sections[3].metadata.link, parse(second).sections[4].metadata.link)
        for blobs in ([first, second], [second, first]):
            combined = self.combine(blobs)
            for section in combined.sections.values():
                if section.elf_type == "SHT_ARM_EXIDX":
                    self.assertEqual(section.metadata.link, section.linked_section)
                    self.assertEqual(section.metadata.info, section.linked_section)
                    self.assertEqual(combined.section_owners[section.metadata.info], combined.section_owners[section.index])
            oracle = linker(memory, catalogue, names)
            self.assertEqual(len(oracle.link(combined)), 2)
            self.assertEqual(len(oracle.removed_special_members), 2)

    def test_different_info_link_targets_refuse_even_when_flag_words_match(self):
        first, memory, catalogue, names = fixture({3: {"flags": flags(3) | 0x40, "info": 2}})
        second, _, _, _ = fixture({3: {"flags": flags(3) | 0x40, "info": 1}})
        for blobs in ([first, second], [second, first]):
            self.refuse(blobs, memory, catalogue, names, "contradictory")

    def test_missing_duplicate_or_disagreeing_group_membership_refuses(self):
        for case in ("missing", "duplicate", "flag"):
            for reverse in (False, True):
                first, memory, catalogue, names = fixture()
                obj, _, _, _ = seeds.UnreferencedDestructorUnwindModelTest().fixture("extab", True)
                obj.sections[1].comdat = "D1_group"
                obj.section_groups.append((1, "D1_group", {1}))
                headers = None
                if case == "missing":
                    obj.section_groups[0][2].remove(2)
                elif case == "duplicate":
                    obj.section_groups.append((1, "D0_group", {2}))
                else:
                    headers = {4: {"flags": flags(4) & ~0x200}}
                second = serialize_object(obj, headers=headers)
                self.refuse([second, first] if reverse else [first, second], memory, catalogue, names,
                            "group membership|global")

    def test_containing_group_header_metadata_and_link_info_semantics_are_preserved(self):
        variants = ({"flags": 0x400}, {"address": 0x100}, {"align": 8}, {"entsize": 0},
                    {"name": 0}, {"link": 0}, {"info": 0})
        for fields in variants:
            first, memory, catalogue, names = fixture()
            second = replace_header(first, ".group.D0_group", **fields)
            for blobs in ([first, second], [second, first]):
                self.refuse(blobs, memory, catalogue, names)
        first, memory, catalogue, names = fixture()
        identical = replace_header(first, ".group.D0_group", flags=0x400, align=8)
        oracle = linker(memory, catalogue, names)
        self.assertEqual(len(oracle.link(self.combine([identical, identical]))), 2)
        self.assertEqual(len(oracle.removed_special_members), 2)

    def test_r1_code_and_companion_copies_also_compare_complete_metadata(self):
        for target in (2, 3, 4):
            for changed in (False, True):
                for reverse in (False, True):
                    first, memory, catalogue, names = fixture(r1=True)
                    fields = {target: {"flags": flags(target) | 0x400}} if changed else None
                    second, _, _, _ = fixture(fields, r1=True)
                    blobs = [second, first] if reverse else [first, second]
                    if changed:
                        self.refuse(blobs, memory, catalogue, names, "contradictory")
                    else:
                        oracle = linker(memory, catalogue, names)
                        self.assertEqual(len(oracle.link(self.combine(blobs))), 2)
                        self.assertEqual(len(oracle.removed_callees), 2)
                        self.assertEqual(oracle.removed_special_members, [])

    def test_missing_metadata_is_not_reconstructed_from_lossy_booleans(self):
        obj, memory, catalogue, names = seeds.UnreferencedDestructorUnwindModelTest().fixture("extab")
        oracle = linker(memory, catalogue, names)
        with self.assertRaisesRegex(inplace.LinkError, "complete ELF section metadata"):
            oracle.link(obj)
        self.assertEqual((oracle.rewrites, oracle.removed_special_members), ([], []))

    def test_v12_is_distinct_and_every_older_model_keeps_its_metadata_policy(self):
        previous, selected = inplace.linker_model(PREVIOUS), inplace.linker_model(MODEL)
        for key, value in vars(previous).items():
            if key not in ("name", "complete_section_metadata"):
                self.assertEqual(getattr(selected, key), value)
        self.assertTrue(selected.complete_section_metadata)
        self.assertTrue(all(not model.complete_section_metadata for name, model in inplace.LINKER_MODELS.items()
                            if name not in (MODEL, "armlink5-inline-branchnop-v13")))
        self.assertEqual(inplace.DEFAULT_LINKER_MODEL, "armlink5-inline-branchnop-v2")
        self.assertEqual(inplace.DEFAULT_LITERAL_MODEL, "none")


class HeaderDependencyCensusTest(unittest.TestCase):
    def test_zero_sized_info_link_sources_keep_d0_and_both_companions_live(self):
        for target in (2, 3, 4):
            for retained in (False, True):
                obj, memory, catalogue, names = seeds.UnreferencedDestructorUnwindModelTest().fixture("extab")
                obj.sections[5] = inplace.Section(5, ".zero.info.root", b"", 0, False, False, False,
                                                 retention=("GNU retain",) if retained else ())
                blob = serialize_object(obj, headers={5: {"flags": 0x42 | (0x200000 if retained else 0),
                                                         "info": target}})
                parsed = parse(blob)
                self.assertEqual(parsed.dependency_sections[5].metadata.info, target)
                oracle = linker(memory, catalogue, names)
                with self.assertRaisesRegex(inplace.LinkError, "incoming reference|linked section|linked metadata"):
                    oracle.link(parsed)
                self.assertEqual((oracle.rewrites, oracle.removed_special_members), ([], []))

    def test_info_link_to_kept_d1_and_zero_info_are_positive_controls(self):
        for target in (0, 1):
            obj, memory, catalogue, names = seeds.UnreferencedDestructorUnwindModelTest().fixture("extab")
            obj.sections[5] = inplace.Section(5, ".zero.unrelated.root", b"", 0, False, False, False,
                                             retention=("GNU retain",))
            blob = serialize_object(obj, headers={5: {"flags": 0x200042, "info": target}})
            oracle = linker(memory, catalogue, names)
            self.assertEqual(set(oracle.link(parse(blob))), {1})
            self.assertEqual(len(oracle.removed_special_members), 1)

    def test_info_link_sources_prevent_provisional_r1_removal(self):
        obj, memory, catalogue = seeds.RemovedCalleeModelTest().fixture()
        obj.sections[3] = inplace.Section(3, ".zero.info.root", b"", 0, False, False, False,
                                         retention=("GNU retain",))
        blob = serialize_object(obj, headers={3: {"flags": 0x200042, "info": 2}})
        oracle = linker(memory, catalogue, {})
        with self.assertRaisesRegex(inplace.LinkError, "linked section|header dependency"):
            oracle.link(parse(blob))
        self.assertEqual((oracle.rewrites, oracle.removed_callees), ([], []))


class CompleteMetadataReportingTest(unittest.TestCase):
    def test_matcher_grid_and_collector_dispatch_the_v12_selection(self):
        from tests.test_unreferenced_v11 import U1CheckedReportingTest

        setup = U1CheckedReportingTest()
        setup.setUp()
        try:
            source = setup.root / "candidate.cpp"
            with redirect_stdout(io.StringIO()):
                result = match.run(source, setup.profile, setup.root / "config/catalogue.json",
                                   setup.output, 0, MODEL, setup.root / "overlay.json")
            self.assertEqual(result, 0)
            receipt = json.loads((setup.output / "candidate.result.json").read_text())
            self.assertEqual(receipt["linker_model"], MODEL)
            self.assertEqual(len(receipt["linker_rewrites"]), 1)
            self.assertEqual(len(receipt["removed_special_members"][0]["unwind_sections"]), 2)
            self.assertEqual(receipt["harness_sha256"]["tools/inplace.py"], match.sha256(match.ROOT / "tools/inplace.py"))
            grid = compid.run_grid(setup.context(MODEL), [source],
                                  {"compilers": ["synthetic"], "flagsets": {"one": []}}, setup.root / "grid")
            self.assertEqual(grid["matching_all"], ["synthetic:one"])
            self.assertEqual(grid["comparison_inputs"][str(source)]["synthetic:one"]["linker_model"], MODEL)
            self.assertEqual(len(grid["linker_section_rewrites"][str(source)]["synthetic:one"]), 1)
            with redirect_stderr(io.StringIO()):
                result = collect.main(["absent.json", "--dest", "unused", "--candidates", str(setup.root / "unused.json"),
                                       "--linker-model", MODEL])
            self.assertEqual(result, 2)
            self.assertFalse((setup.root / "unused.json").exists())
        finally:
            setup.doCleanups()
