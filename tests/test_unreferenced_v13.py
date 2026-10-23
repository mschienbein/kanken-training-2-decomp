"""V13: own-unwind exemptions belong to exact structural edges, never whole sources."""
from __future__ import annotations

import struct
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from tests import test_inplace as seeds
from tests.elf_fixture import serialize_object

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import inplace
import match

MODEL = "armlink5-inline-branchnop-v13"
CLASSES = ("Widget", "Other", "Third")


def family(count=2, *, form="extab", grouped=False, prefix=False, reverse=False,
           source_role="exidx", edge_kind=None, edges=(), target_role="d0", zero_info=False,
           personality=None):
    """Independently serialize multiple complete native-destructor witnesses into one real ELF."""
    objects, names, catalogue = [], {}, {}
    memory = seeds.Memory()
    for serial, name in enumerate(CLASSES[:count]):
        obj, _, _, _ = seeds.UnreferencedDestructorUnwindModelTest().fixture(form)
        encoded = f"{len(name)}{name}"
        for symbol in obj.symbols:
            symbol.name = symbol.name.replace("_ZN6Widget", f"_ZN{encoded}")
            if symbol.shndx == 4:
                symbol.name = f".ARM.extab.{name}"
        obj.sections[1].name = f"i._ZN{encoded}D1Ev"
        obj.sections[2].name = f"i._ZN{encoded}D0Ev"
        obj.sections[3].name = f".ARM.exidx.{name}"
        obj.symbols.append(inplace.Symbol(obj.sections[3].name, 0, 0, "STT_SECTION", "STB_LOCAL", 3))
        if 4 in obj.sections:
            obj.sections[4].name = f".ARM.extab.{name}"
        if personality is not None:
            role = 3 if form == "inline" else 4
            if role == 4:
                obj.sections[4].data = struct.pack("<2I", 0x80B0B0B0 if personality == 0 else
                                                   0x8100B0B0 if personality == 1 else 0x8200B0B0, 0)
            marker = inplace.Symbol(f"__aeabi_unwind_cpp_pr{personality}", 0, 0,
                                     "STT_NOTYPE", "STB_GLOBAL", "SHN_UNDEF")
            obj.symbols.append(marker)
            obj.sections[role].relocs.append(inplace.Reloc(0, inplace.R_ARM_NONE, marker))
        address, table = seeds.BASE + serial * 0x100, seeds.TABLE_AT + serial * 0x100
        names[f"_ZN{encoded}D1Ev"] = {"address": address}
        names[f"_ZN{encoded}D2Ev"] = {"address": address}
        names[f"_ZTV{encoded}"] = {"address": table}
        catalogue[address] = {"address": address, "size": 4, "isa": "arm"}
        memory.write(address, obj.sections[1].data)
        memory.write(table, struct.pack("<4I", 0, 0, address, 0))
        objects.append(obj)
    if reverse:
        objects.reverse()
    combined = inplace.ObjectFile.combine(objects)
    roles = []
    for name in CLASSES[:count]:
        encoded = f"{len(name)}{name}"
        wanted = {"d1": f"i._ZN{encoded}D1Ev", "d0": f"i._ZN{encoded}D0Ev",
                  "exidx": f".ARM.exidx.{name}", "extab": f".ARM.extab.{name}"}
        roles.append({role: next(section.index for section in combined.sections.values() if section.name == label)
                      for role, label in wanted.items() if role != "extab" or form == "extab"})
    if grouped:
        combined.section_groups = []
        dead = set()
        for name, own in zip(CLASSES, roles):
            combined.sections[own["d1"]].comdat = f"D1_{name}"
            combined.section_groups.append((1, f"D1_{name}", {own["d1"]}))
            dead.update(index for role, index in own.items() if role != "d1")
        for index in dead:
            combined.sections[index].comdat = "D0_family"
        combined.section_groups.append((1, "D0_family", dead))
    if target_role == "foreign":
        index = max(combined.sections) + 1
        combined.sections[index] = inplace.Section(index, ".unplaced.foreign", b"data", 4, False, False, False)
        combined.symbols.append(inplace.Symbol("foreign", 0, 0, "STT_SECTION", "STB_LOCAL", index))
        for own in roles:
            own["foreign"] = index
    if prefix:
        combined.sections = {99: inplace.Section(99, ".zero.prefix", b"", 0, False, False, False), **combined.sections}
    indices = {index: serial + 1 for serial, index in enumerate(combined.sections)}
    headers = {}

    def flags(section):
        return (2 | int(section.writable) | (4 if section.executable else 0)
                | (0x80 if section.link_order else 0) | (0x200 if section.comdat else 0))

    if zero_info:
        for own in roles:
            for role in ("exidx", "extab"):
                if role in own:
                    index = own[role]
                    headers[index] = {"flags": flags(combined.sections[index]) | 0x40, "info": 0}
    for source, target in edges:
        source_index, target_index = roles[source][source_role], roles[target][target_role]
        section = combined.sections[source_index]
        if edge_kind == "info":
            headers[source_index] = {"flags": flags(section) | 0x40, "info": indices[target_index]}
        elif edge_kind == "link_order":
            section.link_order, section.linked_section = True, target_index
        else:
            relocation = {"none": inplace.R_ARM_NONE, "abs32": inplace.R_ARM_ABS32,
                          "prel31": inplace.R_ARM_PREL31}[edge_kind]
            symbol = next(symbol for symbol in combined.symbols if symbol.shndx == target_index)
            section.relocs.append(inplace.Reloc(0, relocation, symbol))
    memory.bss = (0x200000, 0x201000)
    return serialize_object(combined, headers=headers), memory, catalogue, names


class StructuralCompanionEdgesTest(unittest.TestCase):
    def compare(self, blobs, memory, catalogue, names):
        context = match.Context.__new__(match.Context)
        context.image, context.functions = memory, catalogue
        context.resolver = inplace.Resolver(catalogue, names)
        context.model, context.literal_model = inplace.linker_model(MODEL), "none"
        with tempfile.TemporaryDirectory() as directory:
            paths = [Path(directory) / f"object-{index}.o" for index in range(len(blobs))]
            for path, blob in zip(paths, blobs):
                path.write_bytes(blob)
            return context, context.compare(paths[0], paths[1:])

    def refuse(self, blobs, memory, catalogue, names, pattern="."):
        context = match.Context.__new__(match.Context)
        context.image, context.functions = memory, catalogue
        context.resolver = inplace.Resolver(catalogue, names)
        context.model, context.literal_model = inplace.linker_model(MODEL), "none"
        with tempfile.TemporaryDirectory() as directory:
            paths = [Path(directory) / f"object-{index}.o" for index in range(len(blobs))]
            for path, blob in zip(paths, blobs):
                path.write_bytes(blob)
            with self.assertRaisesRegex(inplace.LinkError, pattern):
                context.compare(paths[0], paths[1:])
        self.assertEqual((context.section_rewrites, context.removed_special_members, context.removed_callees),
                         ([], [], []))

    def test_mutual_three_cycle_and_acyclic_info_dependencies_refuse_in_both_section_orders(self):
        for count, edges in ((2, ((0, 1), (1, 0))), (3, ((0, 1), (1, 2), (2, 0))), (2, ((0, 1),))):
            for source in ("exidx", "extab"):
                for reverse in (False, True):
                    with self.subTest(count=count, edges=edges, source=source, reverse=reverse):
                        blob, memory, catalogue, names = family(count, source_role=source, edge_kind="info",
                                                               edges=edges, reverse=reverse)
                        self.refuse([blob], memory, catalogue, names, "nonstructural or foreign")

    def test_mixed_index_joint_comdat_copies_refuse_peer_cycles_in_both_object_orders(self):
        for count in (2, 3):
            edges = tuple((index, (index + 1) % count) for index in range(count))
            for source in ("exidx", "extab"):
                first, memory, catalogue, names = family(count, grouped=True, source_role=source,
                                                        edge_kind="info", edges=edges)
                second, _, _, _ = family(count, grouped=True, prefix=True, reverse=True,
                                         source_role=source, edge_kind="info", edges=edges)
                for blobs in ([first, second], [second, first]):
                    self.refuse(blobs, memory, catalogue, names, "nonstructural or foreign")

    def test_all_foreign_companion_edge_transports_and_targets_refuse(self):
        for kind in ("info", "link_order", "none", "abs32", "prel31"):
            for source in ("exidx", "extab"):
                for target in ("d0", "d1", "exidx", "extab", "foreign"):
                    for reverse in (False, True):
                        with self.subTest(kind=kind, source=source, target=target, reverse=reverse):
                            blob, memory, catalogue, names = family(source_role=source, edge_kind=kind,
                                                                   edges=((0, 1),), target_role=target, reverse=reverse)
                            self.refuse([blob], memory, catalogue, names)

    def test_same_owner_info_is_not_the_structural_link_order_edge(self):
        for source in ("exidx", "extab"):
            blob, memory, catalogue, names = family(1, source_role=source, edge_kind="info", edges=((0, 0),))
            parsed = inplace.ObjectFile(Path("synthetic.o"), data=blob)
            oracle = inplace.InPlaceLinker(memory, catalogue, inplace.Resolver(catalogue, names), memory.bss, MODEL)
            if source == "exidx":
                section = next(section for section in parsed.sections.values() if section.elf_type == "SHT_ARM_EXIDX")
                owner = section.linked_section
                self.assertEqual(oracle._section_dependency_edges(parsed, section),
                                 (("link_order", owner), ("info_link", owner)))
            self.refuse([blob], memory, catalogue, names, "nonstructural or foreign")

    def test_direct_self_and_peer_u1_cycles_still_refuse(self):
        for count in (1, 2, 3):
            edges = tuple((index, (index + 1) % count) for index in range(count))
            for kind in ("none", "abs32", "prel31"):
                for reverse in (False, True):
                    blob, memory, catalogue, names = family(count, source_role="d0", edge_kind=kind,
                                                           edges=edges, reverse=reverse)
                    self.refuse([blob], memory, catalogue, names, "still referenced")

    def test_structural_only_closures_and_zero_info_markers_match(self):
        for count in (1, 2, 3):
            for form in ("cant", "inline", "extab"):
                for grouped in (False, True):
                    for zero_info in (False, True):
                        with self.subTest(count=count, form=form, grouped=grouped, zero_info=zero_info):
                            first, memory, catalogue, names = family(count, form=form, grouped=grouped, zero_info=zero_info)
                            blobs = [first]
                            if grouped:
                                second, _, _, _ = family(count, form=form, grouped=True, zero_info=zero_info,
                                                         prefix=True, reverse=True)
                                blobs.append(second)
                            context, rows = self.compare(blobs, memory, catalogue, names)
                            self.assertEqual([row["status"] for row in rows], ["match"] * (count * len(blobs)))
                            self.assertEqual(len(context.section_rewrites), count * len(blobs))
                            self.assertEqual(context.removed_callees, [])
                            for member in context.removed_special_members:
                                self.assertEqual([section["kind"] for section in member["unwind_sections"]],
                                                 ["exidx", "extab"] if form == "extab" else ["exidx"])

    def test_exact_undefined_personality_markers_remain_supported(self):
        for form, personalities in (("inline", (0,)), ("extab", (0, 1, 2))):
            for personality in personalities:
                blob, memory, catalogue, names = family(1, form=form, personality=personality)
                context, rows = self.compare([blob], memory, catalogue, names)
                self.assertEqual([row["status"] for row in rows], ["match"])
                self.assertEqual(len(context.section_rewrites), 1)

    def test_r1_only_companions_also_require_exact_structural_edges(self):
        from tests.test_unreferenced_v12 import fixture, flags

        baseline, memory, catalogue, names = fixture(r1=True)
        memory.bss = (0x200000, 0x201000)
        context, rows = self.compare([baseline], memory, catalogue, names)
        self.assertEqual([row["status"] for row in rows], ["match"])
        self.assertEqual(len(context.removed_callees), 1)
        self.assertEqual(context.removed_special_members, [])
        for source in (3, 4):
            for target in (1, 2):
                blob, memory, catalogue, names = fixture(
                    {source: {"flags": flags(source) | 0x40, "info": target}}, r1=True)
                memory.bss = (0x200000, 0x201000)
                self.refuse([blob], memory, catalogue, names, "nonstructural or foreign")

    def test_v13_is_explicit_and_older_models_keep_their_edge_policy(self):
        previous = inplace.linker_model("armlink5-inline-branchnop-v12")
        selected = inplace.linker_model(MODEL)
        for key, value in vars(previous).items():
            if key not in ("name", "structural_companion_edges"):
                self.assertEqual(getattr(selected, key), value)
        self.assertTrue(selected.structural_companion_edges)
        self.assertTrue(all(not model.structural_companion_edges for name, model in inplace.LINKER_MODELS.items()
                            if name not in (MODEL, "armlink5-inline-branchnop-v14", "armlink5-inline-branchnop-v15")))
        self.assertEqual(inplace.DEFAULT_LINKER_MODEL, "armlink5-inline-branchnop-v2")
        self.assertEqual(inplace.DEFAULT_LITERAL_MODEL, "none")

    def test_matcher_grid_and_collector_preserve_v13_reporting(self):
        from tests import test_unreferenced_v12 as previous

        with patch.object(previous, "MODEL", MODEL):
            previous.CompleteMetadataReportingTest().test_matcher_grid_and_collector_dispatch_the_v12_selection()
