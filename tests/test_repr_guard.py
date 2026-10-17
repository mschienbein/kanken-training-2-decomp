"""Implicit displays of public tool objects must never expose private buffers.

Every image and ELF input here is synthetic; no private inputs or compiler are used.
"""
import contextlib
from dataclasses import asdict, replace
import io
import json
from pathlib import Path
import pickle
import shutil
import struct
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import analyze  # noqa: E402
import ctrfmt  # noqa: E402
import image  # noqa: E402
import inplace  # noqa: E402
import match  # noqa: E402
import packet  # noqa: E402
import permute  # noqa: E402
import fingerprint  # noqa: E402
import toolchain  # noqa: E402
from tests import test_build as build_tests  # noqa: E402
from tests import test_fingerprint as fingerprint_tests  # noqa: E402
from tests import test_literal_pools as literal_tests  # noqa: E402

BASE = 0x100000
CANDIDATE = struct.pack("<4I", 0xE59F0004, 0xEB00003D, 0xE12FFF1E, BASE + 0x100)
ORIGINAL = struct.pack("<I", 0xE59F0008) + CANDIDATE[4:]


class ReprGuardTest(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.directory = Path(temporary.name)
        self.object_path = self.directory / "synthetic.o"
        fingerprint_tests.write_elf(self.object_path)
        layout = ctrfmt.CodeLayout(
            "SYNTH", False, ctrfmt.CodeSegment("text", BASE, 1, 16),
            ctrfmt.CodeSegment("rodata", 0x180000, 0, 0),
            ctrfmt.CodeSegment("data", 0x200000, 0, 0), 0x1000, 0x1000, (),
        )
        self.image = image.Image(ORIGINAL.ljust(layout.image_size, b"\0"), layout)
        self.functions = {BASE: {"address": BASE, "size": 16, "isa": "arm",
                                 "code_ranges": [[BASE, BASE + 12]],
                                 "pools": [[BASE + 12, BASE + 16]]}}
        self.names = {"library_function": {"address": BASE}, "external": {"address": BASE + 0x100}}
        self.catalogue_path = self.directory / "catalogue.json"
        self.catalogue_path.write_text(json.dumps({
            "image_sha256": self.image.sha256, "functions": list(self.functions.values()),
        }), encoding="utf-8")

    @contextlib.contextmanager
    def synthetic_inputs(self):
        with patch.object(image.Image, "load", return_value=self.image), \
                patch.object(image, "load_names", return_value=self.names):
            yield

    def assert_buffer_free(self, value, *buffers):
        for render in (repr, str):
            with self.subTest(type=type(value).__name__, render=render.__name__):
                text = render(value)
                self.assertNotRegex(text, r"\bb['\"]|bytearray\(|(?:\\x[0-9a-fA-F]{2}){5,}|0x[0-9a-fA-F]{9,}")
                for data in buffers:
                    self.assertGreater(len(data), 4)
                    self.assertNotIn(bytes(data).hex(), text)
                    self.assertNotIn(repr(bytes(data)), text)

    def assert_receipt_metadata_only(self, value):
        self.assertNotIsInstance(value, (bytes, bytearray, memoryview))
        if isinstance(value, dict):
            self.assertNotIn("_original", value)
            self.assertNotIn("_candidate", value)
            for item in value.values():
                self.assert_receipt_metadata_only(item)
        elif isinstance(value, (tuple, list)):
            for item in value:
                self.assert_receipt_metadata_only(item)

    def compile_fixture(self, profile, source, obj, **kwargs):
        obj.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(self.object_path, obj)

    def test_object_image_analysis_and_fingerprint_displays(self):
        obj = inplace.ObjectFile(self.object_path)
        combined = inplace.ObjectFile.combine([obj])
        analyzer = analyze.Analyzer(self.image.code, self.image.layout)
        namer = packet.Namer(self.image, self.functions, self.names)
        fingerprints = fingerprint.function_fingerprints(obj)
        for value in (self.image, analyzer, namer, obj, combined, obj.sections,
                      *obj.sections.values(), fingerprints, *fingerprints):
            self.assert_buffer_free(value, ORIGINAL, obj.sections[1].data)
        section = obj.sections[1]
        self.assertEqual(asdict(section)["data"], section.data)
        self.assertNotEqual(section, replace(section, data=b"different synthetic bytes"))
        function = fingerprints[0]
        self.assertEqual(asdict(function)["data"], section.data)
        self.assertEqual(asdict(function)["mask"], function.mask)
        self.assertNotEqual(function, replace(function, data=b"different synthetic bytes"))

    def test_bytes_bytearray_and_memoryview_fields_are_hidden(self):
        payload = b"SYNTHETIC-PRIVATE-BUFFER-0123456789"
        for wrap in (bytes, bytearray, memoryview):
            with self.subTest(buffer=wrap.__name__):
                data = wrap(payload)
                section = inplace.Section(1, ".text", data, len(data), False, True, False)
                placed = inplace.Placement(section, BASE, "catalogue", literal_data=data)
                function = fingerprint.FunctionFingerprint("synthetic", "arm", data, data, len(data), 0, 0)
                self.assert_buffer_free(section, payload)
                self.assert_buffer_free(placed, payload)
                self.assert_buffer_free(function, payload)

    def test_container_header_displays_and_field_semantics(self):
        raw = bytearray(b"\xab" * 0x200)
        raw[0x100:0x104] = b"NCCH"
        raw[0x150:0x160] = b"SYNTHETIC-HEADER"
        header = ctrfmt.parse_ncch(bytes(raw))
        exefs = bytearray(0x200)
        struct.pack_into("<8sII", exefs, 0, b".code", 0, 16)
        digest = bytes(range(32))
        exefs[0x1E0:0x200] = digest
        entry = ctrfmt.parse_exefs_header(bytes(exefs))[0]
        self.assert_buffer_free(header, raw, header.exheader_hash)
        self.assert_buffer_free(entry, digest)
        self.assertEqual(asdict(header)["raw"], bytes(raw))
        self.assertEqual(asdict(entry)["sha256"], digest)
        self.assertNotEqual(header, replace(header, raw=b"different synthetic bytes"))
        self.assertNotEqual(entry, replace(entry, sha256=b"different synthetic hash"))

    def test_link_outputs_preserve_tuple_behavior_without_displaying_data(self):
        obj = inplace.ObjectFile(self.object_path)
        linker = inplace.InPlaceLinker(self.image, self.functions,
                                      inplace.Resolver(self.functions, self.names), self.image.bss)
        linked = linker.link(obj)
        section = linked[1]
        placed, data = section
        self.assertIsInstance(section, tuple)
        self.assertEqual(section, (placed, CANDIDATE))
        self.assertEqual(data, CANDIDATE)
        self.assertEqual(pickle.loads(pickle.dumps(section)), section)
        for value in (linker, linker.place(obj, {}), linked, list(linked.values()), section, placed):
            self.assert_buffer_free(value, CANDIDATE, obj.sections[1].data)
        obj, _, literal_linker, *_ = literal_tests.fixture()
        literal_linked = literal_linker.link(obj)
        pool = literal_linked[2]
        self.assertIsNotNone(pool.placement.literal_data)
        self.assert_buffer_free(pool.placement, pool.data, obj.sections[2].data)
        self.assert_buffer_free(literal_linked, pool.data, obj.sections[2].data)

    def test_comparisons_and_permuter_results_preserve_payload_and_equality(self):
        with self.synthetic_inputs():
            context = match.Context(self.catalogue_path)
            oracle = permute.Oracle(toolchain.Profile("synthetic", "synthetic", ()),
                                    self.catalogue_path, ("library_function",))
        rows = context.compare(self.object_path)
        row = rows[0]
        expected = {"name": "library_function", "address": BASE, "size": 16, "candidate_size": 16,
                    "status": "mismatch", "first_diff": 0, "linker_rewrites": [],
                    "_original": ORIGINAL, "_candidate": CANDIDATE}
        self.assertIsInstance(row, dict)
        self.assertEqual(rows, [expected])
        self.assertEqual(pickle.loads(pickle.dumps(row)), expected)
        self.assertEqual(row["_original"], ORIGINAL)
        self.assertEqual(row["_candidate"], CANDIDATE)
        with patch.object(toolchain, "compile_source", side_effect=self.compile_fixture):
            evaluation = oracle.evaluate("void library_function(void) {}", self.directory / "worker")
        self.assertIsNotNone(evaluation.score)
        self.assertGreater(evaluation.score, 0)
        self.assertEqual(evaluation, permute.score_rows(rows, oracle.targets, self.functions))
        for value in (context, rows, row, oracle, evaluation, evaluation.functions):
            self.assert_buffer_free(value, ORIGINAL, CANDIDATE)
        self.assert_receipt_metadata_only(evaluation.functions)

    def test_match_and_build_receipts_still_serialize_metadata_only(self):
        source = self.directory / "candidate.c"
        source.write_text("void library_function(void) {}\n", encoding="utf-8")
        out = self.directory / "match"
        stdout = io.StringIO()
        with self.synthetic_inputs(), contextlib.redirect_stdout(stdout), \
                patch.object(toolchain, "compile_source", side_effect=self.compile_fixture), \
                patch.object(toolchain, "armcc_exe", return_value=self.object_path):
            status = match.run(source, toolchain.Profile("synthetic", "synthetic", ()),
                               self.catalogue_path, out, 0)
        self.assertEqual(status, 1)
        record = json.loads((out / "candidate.result.json").read_text(encoding="utf-8"))
        self.assertEqual(record["functions"], [{
            "name": "library_function", "address": BASE, "size": 16, "candidate_size": 16,
            "status": "mismatch", "first_diff": 0, "linker_rewrites": [],
        }])
        self.assert_receipt_metadata_only(record)
        self.assertNotIn(ORIGINAL.hex(), stdout.getvalue())
        self.assertNotIn(repr(ORIGINAL), stdout.getvalue())
        fixture = build_tests.BuildRemovedCalleeModelTest()
        self.addCleanup(fixture.doCleanups)
        receipt, root = fixture.synthetic_build()
        self.assertEqual(receipt["result"], "PASS")
        self.assert_receipt_metadata_only(receipt)
        saved = root / "build/synthetic-removed/receipt.json"
        self.assertEqual(json.loads(saved.read_text(encoding="utf-8")), receipt)


if __name__ == "__main__":
    unittest.main()
