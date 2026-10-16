"""Synthetic ARM/Thumb triage tests; original inputs are optional and never emitted."""
from __future__ import annotations

from contextlib import redirect_stdout
import copy
import hashlib
import io
import json
from pathlib import Path
import struct
import tempfile
import time
import unittest
from unittest.mock import patch

from tools import ctrfmt, triage

BASE = 0x100000
ROOT = Path(__file__).resolve().parents[1]
EMPTY_MATCHES = {"schema_version": 1, "units": []}


def branch(source: int, target: int, *, link: bool = False, condition: int = 14) -> int:
    return (condition << 28) | (0x0B000000 if link else 0x0A000000) | (((target - source - 8) >> 2) & 0xFFFFFF)


def fixture() -> tuple[triage.image_mod.Image, dict]:
    """Hand-encoded fixtures with explicit code islands, pools and one veneer."""
    text = bytearray(0x200)
    functions = []

    def add(offset: int, words: list[int], *, isa: str = "arm", size: int | None = None,
            pools: list[list[int]] | None = None, ranges: list[list[int]] | None = None,
            kind: str = "function") -> None:
        data = struct.pack(f"<{len(words)}I", *words)
        text[offset:offset + len(data)] = data
        address = BASE + offset
        functions.append({"address": address, "size": size or len(data), "isa": isa, "kind": kind,
                          "pools": pools or [], "code_ranges": ranges or [[address, address + len(data)]],
                          "code_size": sum(high - low for low, high in ranges) if ranges else len(data),
                          "evidence": ["synthetic"]})

    add(0x00, [0xE3A00003, 0xE12FFF1E])
    add(0x10, [branch(BASE + 0x10, BASE, link=True), branch(BASE + 0x14, BASE, link=True), 0xE12FFF1E])
    add(0x20, [0xE59F0000, 0xE12FFF1E, BASE + 0x1001, BASE + 0x2000, BASE + 0x2010, BASE],
        pools=[[BASE + 0x28, BASE + 0x38]], ranges=[[BASE + 0x20, BASE + 0x28]])
    add(0x40, [0xE3500001, 0x908FF100, 0xE12FFF1E])  # ADDLS pc,pc,r0,LSL#2.
    add(0x50, [0xE3500001, 0x979FF100, 0xE12FFF1E])  # LDRLS pc,[pc,r0,LSL#2].
    add(0x60, [0xE3500001, branch(BASE + 0x64, BASE + 0x60, condition=1), 0xE12FFF1E])
    add(0x70, [0xEE300A00, 0xE12FFF1E])  # VADD.F32 s0,s0,s0.
    add(0x80, [0xE12FFF33, 0xE12FFF1E])  # BLX r3.
    add(0x90, [0xEF000001, 0xE12FFF1E])  # SVC #1.
    add(0xC0, [0x47702003], isa="thumb")  # MOVS r0,#3; BX lr.
    add(0xD0, [0xFAFFFFFA, 0xE12FFF1E])  # BLX immediate to the Thumb leaf.
    add(0xE0, [0xE12FFF1E], kind="veneer")
    add(0xF0, [branch(BASE + 0xF0, BASE + 0x10, link=True), 0xE12FFF1E])
    add(0x100, [0xE1A00000] * 15 + [0xE12FFF1E])
    add(0x150, [0xE3A00003, branch(BASE + 0x154, BASE, link=True), 0xEE300A00, 0xE12FFF1E],
        ranges=[[BASE + 0x150, BASE + 0x154], [BASE + 0x15C, BASE + 0x160]])
    exheader = bytearray(0x200)
    exheader[:8] = b"SYNTHETC"
    for offset, address, size in ((0x10, BASE, len(text)), (0x20, BASE + 0x1000, 16), (0x30, BASE + 0x2000, 16)):
        struct.pack_into("<III", exheader, offset, address, 1, size)
    struct.pack_into("<I", exheader, 0x3C, 0x100)
    code = bytes(text).ljust(0x1000, b"\0") + bytes(0x2000)
    image = triage.image_mod.Image(code, ctrfmt.parse_exheader(exheader))
    catalogue = {"schema_version": 1, "image_sha256": image.sha256, "functions": functions,
                 "anomalies": [{"address": BASE + 0x28, "issue": "synthetic_pool_anomaly"},
                               {"address": BASE + 0x38, "issue": "outside_function"}]}
    return image, catalogue


class TriageTests(unittest.TestCase):
    def setUp(self) -> None:
        self.image, self.catalogue = fixture()
        self.analysis = triage.Triage(self.image, self.catalogue, EMPTY_MATCHES)

    def test_leaf_calls_and_callers(self) -> None:
        leaf = self.analysis.records[BASE]
        caller = self.analysis.records[BASE + 0x10]
        self.assertEqual(leaf["features"]["instruction_count"], 2)
        self.assertEqual(leaf["features"]["direct_callees"], 0)
        self.assertEqual(leaf["features"]["callers"], 1)  # Two sites, one caller.
        self.assertEqual(caller["features"]["direct_callees"], 1)
        self.assertEqual(caller["features"]["instruction_count"], 3)
        self.assertEqual(caller["features"]["credited_callees"], 0)
        self.assertEqual(self.analysis.callees[BASE + 0x10], {BASE})
        self.assertEqual(leaf["value"], leaf["size"])
        self.assertLess(leaf["difficulty"], caller["difficulty"])

    def test_pool_data_refs_and_anomaly_extent(self) -> None:
        pool = self.analysis.records[BASE + 0x20]["features"]
        self.assertEqual(pool["data_refs"], 3)  # Odd rodata pointer, data and BSS; text is not data.
        self.assertEqual(pool["instruction_count"], 2)
        self.assertEqual(pool["anomalies"], [{"address": BASE + 0x28, "issue": "synthetic_pool_anomaly"}])
        self.assertEqual(self.analysis.records[BASE + 0x40]["features"]["anomalies"], [])

    def test_pools_are_not_decoded_even_inside_code_range(self) -> None:
        catalogue = copy.deepcopy(self.catalogue)
        function = next(function for function in catalogue["functions"] if function["address"] == BASE + 0x20)
        function["code_ranges"] = [[BASE + 0x20, BASE + 0x38]]
        code = bytearray(self.image.code)
        struct.pack_into("<I", code, 0x34, branch(BASE + 0x34, BASE, link=True))
        image = triage.image_mod.Image(bytes(code), self.image.layout)
        catalogue["image_sha256"] = image.sha256
        features = triage.Triage(image, catalogue, EMPTY_MATCHES).records[BASE + 0x20]["features"]
        self.assertEqual(features["instruction_count"], 2)
        self.assertEqual(features["direct_callees"], 0)
        self.assertEqual(features["data_refs"], 3)

    def test_code_islands_do_not_decode_unknown_holes(self) -> None:
        features = self.analysis.records[BASE + 0x150]["features"]
        self.assertEqual(features["instruction_count"], 2)
        self.assertEqual(features["direct_callees"], 0)
        self.assertEqual(features["vfp_instructions"], 0)
        self.assertEqual(features["undecoded_bytes"], 0)

    def test_both_switch_forms(self) -> None:
        for offset in (0x40, 0x50):
            with self.subTest(offset=offset):
                self.assertTrue(self.analysis.records[BASE + offset]["features"]["switch_table"])
        self.assertFalse(self.analysis.records[BASE + 0x20]["features"]["switch_table"])

    def test_loops_vfp_indirect_and_svc(self) -> None:
        for offset, key in ((0x60, "loops"), (0x70, "vfp_instructions"), (0x80, "indirect_calls"), (0x90, "svc")):
            with self.subTest(key=key):
                self.assertEqual(self.analysis.records[BASE + offset]["features"][key], 1)
        self.assertEqual(self.analysis.records[BASE + 0x80]["features"]["direct_callees"], 0)
        self.assertEqual(self.analysis.records[BASE + 0x70]["features"]["kind"], "function")

    def test_thumb_and_direct_blx(self) -> None:
        thumb = self.analysis.records[BASE + 0xC0]
        self.assertEqual(thumb["isa"], "thumb")
        self.assertEqual(thumb["features"]["instruction_count"], 2)
        self.assertEqual(thumb["features"]["callers"], 1)
        self.assertEqual(self.analysis.callees[BASE + 0xD0], {BASE + 0xC0})
        self.assertEqual(self.analysis.records[BASE + 0xD0]["features"]["indirect_calls"], 0)

    def test_conditional_branch_is_not_a_call(self) -> None:
        code = bytearray(self.image.code)
        struct.pack_into("<I", code, 0x64, branch(BASE + 0x64, BASE + 0x60, condition=9))  # BLS, not BL.
        image = triage.image_mod.Image(bytes(code), self.image.layout)
        catalogue = {**self.catalogue, "image_sha256": image.sha256}
        features = triage.Triage(image, catalogue, EMPTY_MATCHES).records[BASE + 0x60]["features"]
        self.assertEqual(features["direct_callees"], 0)
        self.assertEqual(features["loops"], 1)

    def test_credited_names_and_veneers_are_skipped(self) -> None:
        names = {"thumb_leaf": {"address": BASE + 0xC0}, "chosen_caller": {"address": BASE + 0x10}}
        matches = {"units": [{"functions": ["func_00100000", "thumb_leaf"]}]}
        analysis = triage.Triage(self.image, self.catalogue, matches, names)
        addresses = {function["address"] for function in analysis.queue()}
        self.assertNotIn(BASE, addresses)
        self.assertNotIn(BASE + 0xC0, addresses)
        self.assertNotIn(BASE + 0xE0, addresses)
        self.assertEqual(analysis.records[BASE + 0x10]["features"]["credited_callees"], 1)
        self.assertEqual(analysis.records[BASE + 0x10]["name"], "chosen_caller")
        self.assertEqual(analysis.records[BASE + 0xD0]["features"]["credited_callees"], 1)
        self.assertEqual(analysis.records[BASE + 0x10]["depth"], 0)
        self.assertEqual(analysis.records[BASE + 0xF0]["depth"], 1)

    def test_bad_credits_and_reserved_names_fail_closed(self) -> None:
        for name in ("missing", "func_0010FFFF", "func_001000e0"):
            with self.subTest(name=name), self.assertRaises(triage.image_mod.ImageError):
                triage.Triage(self.image, self.catalogue, {"units": [{"functions": [name]}]})
        with self.assertRaisesRegex(triage.image_mod.ImageError, "reserved"):
            triage.Triage(self.image, self.catalogue, EMPTY_MATCHES, {"func_00100000": {"address": BASE + 0x10}})

    def test_wrong_image_and_overlapping_functions_fail_closed(self) -> None:
        catalogue = {**self.catalogue, "image_sha256": "wrong"}
        with self.assertRaisesRegex(triage.image_mod.ImageError, "image_sha256"):
            triage.Triage(self.image, catalogue, EMPTY_MATCHES)
        catalogue = copy.deepcopy(self.catalogue)
        catalogue["functions"][0]["size"] = 0x20
        with self.assertRaisesRegex(triage.image_mod.ImageError, "overlap"):
            triage.Triage(self.image, catalogue, EMPTY_MATCHES)

    def test_excluded_dependencies_still_contribute_depth(self) -> None:
        analysis = triage.Triage(self.image, self.catalogue, EMPTY_MATCHES, excluded={BASE + 0x10})
        self.assertNotIn(BASE + 0x10, {function["address"] for function in analysis.queue()})
        self.assertEqual(analysis.records[BASE + 0xF0]["depth"], 2)
        self.assertEqual(analysis.records[BASE + 0x10]["features"]["callers"], 1)

    def test_queue_filters_and_dependency_order(self) -> None:
        queue = self.analysis.queue()
        ranks = {function["address"]: rank for rank, function in enumerate(queue)}
        self.assertLess(ranks[BASE], ranks[BASE + 0x10])
        self.assertLess(ranks[BASE + 0x10], ranks[BASE + 0xF0])
        self.assertEqual(queue, sorted(queue, key=lambda function: (function["depth"], function["difficulty"], function["address"])))
        self.assertTrue(all(function["size"] <= 8 for function in self.analysis.queue(max_size=8)))
        self.assertEqual([function["address"] for function in self.analysis.queue(max_size=8, isa="thumb")], [BASE + 0xC0])

    def test_call_depth_handles_recursion_and_long_chains(self) -> None:
        graph = {1: {2}, 2: {1, 3}, 3: {3}, 4: {1}, 5: set()}
        self.assertEqual(triage.call_depths(graph), {1: 1, 2: 1, 3: 0, 4: 2, 5: 0})
        chain = {address: {address + 1} for address in range(1500)}
        chain[1500] = set()
        depths = triage.call_depths(chain)
        self.assertEqual((depths[0], depths[1500]), (1500, 0))
        self.assertEqual(triage.call_depths({}), {})

    def test_input_order_and_seed_determinism(self) -> None:
        catalogue = copy.deepcopy(self.catalogue)
        catalogue["functions"].reverse()
        catalogue["anomalies"].reverse()
        names = {"z_alias": {"address": BASE}, "a_alias": {"address": BASE}}
        first = triage.Triage(self.image, catalogue, EMPTY_MATCHES, names)
        second = triage.Triage(self.image, self.catalogue, EMPTY_MATCHES, dict(reversed(list(names.items()))))
        self.assertEqual(first.queue(), second.queue())
        self.assertEqual(first.records[BASE]["name"], "a_alias")
        self.assertEqual(first.stratified(2, 1), second.stratified(2, 1))
        self.assertNotEqual(first.stratified(2, 1)["leaf-small"], first.stratified(2, 2)["leaf-small"])
        self.assertEqual(first.batches(3, 2), second.batches(3, 2))

    def test_stratified_covers_every_feature_class(self) -> None:
        groups = self.analysis.classes()
        self.assertEqual(tuple(groups), triage.FEATURE_CLASSES)
        selected = self.analysis.stratified(2, 1)
        for name, candidates in groups.items():
            with self.subTest(name=name):
                self.assertGreater(len(candidates), 0)
                self.assertEqual(len(selected[name]), min(2, len(candidates)))
                self.assertTrue(all(name in triage.feature_classes(function) for function in selected[name]))
        with self.assertRaisesRegex(ValueError, "positive"):
            self.analysis.stratified(0, 1)

    def test_batches_keep_neighbours_and_are_disjoint(self) -> None:
        batches = self.analysis.batches(3, 2)
        self.assertEqual(len(batches), 2)
        self.assertTrue(all(len(batch) == 3 for batch in batches))
        addresses = [function["address"] for batch in batches for function in batch]
        self.assertEqual(len(addresses), len(set(addresses)))
        self.assertEqual({function["address"] for function in batches[0]}, {BASE, BASE + 0x10, BASE + 0xF0})
        for size, count in ((100, 2), (0, 1), (1, 0)):
            with self.subTest(size=size, count=count), self.assertRaises(ValueError):
                self.analysis.batches(size, count)

    def test_empty_eligible_graph_and_classes(self) -> None:
        matches = {"units": [{"functions": [f"func_{function['address']:08x}" for function in self.catalogue["functions"]
                                            if function["kind"] == "function"]}]}
        analysis = triage.Triage(self.image, self.catalogue, matches)
        self.assertEqual(analysis.queue(), [])
        self.assertTrue(all(not functions for functions in analysis.stratified(2, 1).values()))
        with self.assertRaises(ValueError):
            analysis.batches(1, 1)

    def test_exclusion_file_addresses_and_rejections(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "exclude.json"
            path.write_text(json.dumps([BASE, f"0x{BASE + 0xC1:x}"]), encoding="utf-8", newline="\n")
            self.assertEqual(triage.load_exclusions(path), {BASE, BASE + 0xC0})
            self.assertEqual(triage.load_exclusions(None), set())
            for value in ({"addresses": []}, [True], [-1], ["func_00100000"], [0x100000000]):
                with self.subTest(value=value):
                    path.write_text(json.dumps(value), encoding="utf-8", newline="\n")
                    with self.assertRaises(ValueError):
                        triage.load_exclusions(path)

    def test_loader_uses_image_catalogue_and_exact_file_hashes(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "config").mkdir()
            catalogue_bytes = (json.dumps(self.catalogue, indent=1) + "\n").encode("utf-8")
            matches_bytes = b'{"units":[{"functions":["named_leaf"]}]}\n'
            (root / "config/catalogue.json").write_bytes(catalogue_bytes)
            (root / "config/matches.json").write_bytes(matches_bytes)
            (root / "config/names.json").write_text(json.dumps({"symbols": {"named_leaf": {"address": BASE}}}), encoding="utf-8", newline="\n")
            excluded = root / "in-flight.json"
            excluded.write_text(json.dumps([BASE + 0x10]), encoding="utf-8", newline="\n")
            with patch.object(triage.image_mod.Image, "load", return_value=self.image) as image_load:
                analysis, metadata = triage.load_triage(excluded, root)
            image_load.assert_called_once_with(root / "orig")
            self.assertEqual(metadata, {"schema_version": 1, "image_sha256": self.image.sha256,
                                        "catalogue_sha256": hashlib.sha256(catalogue_bytes).hexdigest(),
                                        "generated_from_matches_sha256": hashlib.sha256(matches_bytes).hexdigest()})
            self.assertEqual(analysis.credited, {BASE})
            self.assertEqual(analysis.excluded, {BASE + 0x10})

    def test_cli_json_schema_filters_and_determinism(self) -> None:
        metadata = {"schema_version": 1, "image_sha256": self.image.sha256,
                    "catalogue_sha256": "c" * 64, "generated_from_matches_sha256": "m" * 64}
        with tempfile.TemporaryDirectory() as directory, patch.object(triage, "load_triage", return_value=(self.analysis, metadata)):
            out = Path(directory)
            for arguments in (["queue", "--limit", "3"], ["queue", "--max-size", "8", "--isa", "thumb"],
                              ["stratified", "--per-class", "2", "--seed", "1"], ["batches", "--size", "3", "--count", "2"]):
                with self.subTest(arguments=arguments):
                    with redirect_stdout(io.StringIO()):
                        self.assertEqual(triage.main([*arguments, "--out", str(out)]), 0)
                    path = out / f"{arguments[0]}.json"
                    first = path.read_bytes()
                    self.assertNotIn(b"\r", first)
                    document = json.loads(first)
                    self.assertEqual({key: document[key] for key in metadata}, metadata)
                    if arguments[0] == "queue":
                        self.assertEqual(len(document["functions"]), 3 if "--limit" in arguments else 1)
                        self.assertTrue(all({"address", "name", "size", "isa", "features", "difficulty", "depth", "value"} <= function.keys()
                                            for function in document["functions"]))
                    elif arguments[0] == "stratified":
                        self.assertEqual(set(document["classes"]), set(triage.FEATURE_CLASSES))
                        self.assertEqual(len(document["functions"]), len({function["address"] for function in document["functions"]}))
                        self.assertEqual(document["class_counts"], {name: len(functions) for name, functions in self.analysis.classes().items()})
                    else:
                        self.assertEqual([len(batch["functions"]) for batch in document["batches"]], [3, 3])
                    with redirect_stdout(io.StringIO()):
                        self.assertEqual(triage.main(["--out", str(out), *arguments]), 0)
                    self.assertEqual(first, path.read_bytes())

    def test_cli_passes_global_and_command_exclusions(self) -> None:
        for arguments in (["--exclude", "in-flight.json", "--avoid", "svc", "queue"],
                          ["queue", "--exclude", "in-flight.json", "--avoid", "svc"]):
            with tempfile.TemporaryDirectory() as directory, patch.object(triage, "load_triage", return_value=(self.analysis, {})) as loader:
                with redirect_stdout(io.StringIO()):
                    triage.main([*arguments, "--out", directory])
                loader.assert_called_once_with(Path("in-flight.json"), avoid=frozenset({"svc"}))

    def test_avoided_functions_leave_the_pool_but_keep_the_graph(self) -> None:
        code = bytearray(self.image.code)
        struct.pack_into("<I", code, 0x00, 0xE1910F9F)  # ldrex r0, [r1] in the leaf at BASE.
        image = triage.image_mod.Image(bytes(code), self.image.layout)
        catalogue = {**self.catalogue, "image_sha256": image.sha256}
        plain = triage.Triage(image, catalogue, EMPTY_MATCHES)
        avoiding = triage.Triage(image, catalogue, EMPTY_MATCHES, avoid=frozenset({"svc", "thumb", "exclusive"}))
        self.assertEqual(plain.records[BASE]["features"]["exclusive"], 1)
        self.assertEqual(avoiding.avoided, [BASE, BASE + 0x90, BASE + 0xC0])
        self.assertTrue({BASE, BASE + 0x90, BASE + 0xC0}.isdisjoint(f["address"] for f in avoiding.queue()))
        self.assertEqual({a: r["depth"] for a, r in plain.records.items()},
                         {a: r["depth"] for a, r in avoiding.records.items()})
        with self.assertRaisesRegex(ValueError, "unknown --avoid class"):
            triage.Triage(image, catalogue, EMPTY_MATCHES, avoid=frozenset({"asm"}))

    @unittest.skipUnless((ROOT / "orig/code.bin").is_file() and (ROOT / "orig/exheader.bin").is_file(), "private inputs absent")
    def test_real_image_smoke(self) -> None:
        started = time.perf_counter()
        analysis, metadata = triage.load_triage()
        elapsed = time.perf_counter() - started
        self.assertLess(elapsed, 30.0)
        self.assertGreater(len(analysis.queue()), 100)
        self.assertEqual(metadata["image_sha256"], triage.image_mod.Image.load().sha256)
        self.assertEqual(len(analysis.queue()[:20]), 20)
        selected = analysis.stratified(2, 1)
        for name, candidates in analysis.classes().items():
            self.assertEqual(len(selected[name]), min(2, len(candidates)))
        self.assertTrue(all(not (function["address"] in analysis.credited or function["features"]["kind"] == "veneer")
                            for function in analysis.queue()))


if __name__ == "__main__":
    unittest.main()
