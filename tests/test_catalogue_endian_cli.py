"""Real synthetic catalogue CLI; hash-bound inputs, no mock wiring or ROM payload."""
from __future__ import annotations

import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest

from tools import analyze
from tests.test_analyze import fixture, library_revision_fixture
from tests.test_catalogue_endian import SEALED_ENDIAN_WORDS

ROOT = Path(__file__).resolve().parents[1]


class CatalogueEndianCliTests(unittest.TestCase):
    def test_real_public_writer_uses_r5_and_keeps_the_sealed_target_unknown(self) -> None:
        image, layout = fixture(SEALED_ENDIAN_WORDS)
        baseline = analyze.build_catalogue(image, layout)
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            for directory in ("tools", "config", "orig", "research"):
                (root / directory).mkdir()
            for name in ("analyze.py", "catalogue_closure.py", "catalogue_review.py", "catalogue_runtime_data.py", "ctrfmt.py"):
                shutil.copyfile(ROOT / "tools" / name, root / "tools" / name)
            subprocess.run(["git", "init", "-q", str(root)], capture_output=True, check=True)
            baseline_payload = analyze.serialize_catalogue(baseline)
            (root / "config/catalogue.json").write_bytes(baseline_payload)
            (root / "config/matches.json").write_text(json.dumps({"units": []}))
            blob = subprocess.run(["git", "hash-object", "-w", "config/catalogue.json"], cwd=root,
                                  capture_output=True, text=True, check=True).stdout.strip()
            evidence = {"revision": "synthetic-v9-r2", "image_sha256": baseline["image_sha256"],
                        "classification_records": [], "thumb_dispositions": [], "author_dispositions": [],
                        "thumb_v9_residue": [], "provenance": {"proof_status": "synthetic"},
                        "source_candidate_sha256": "synthetic", "source_input_sha256": {}}
            parent = root / "research/catalogue_v9_r2_evidence.json"
            parent.write_text(json.dumps(evidence))
            closure = {"image_sha256": baseline["image_sha256"], "literal_encodings": [],
                       "provenance": "synthetic", "closed_graph_owners": [],
                       "parent_evidence": {"path": "research/catalogue_v9_r2_evidence.json",
                                           "sha256": hashlib.sha256(parent.read_bytes()).hexdigest()},
                       "baseline_catalogue": {"blob": blob, "reference": "synthetic",
                                              "revision": "synthetic-adopted",
                                              "sha256": hashlib.sha256(baseline_payload).hexdigest()}}
            closure_path = root / "research/catalogue_v10_evidence.json"
            closure_path.write_text(json.dumps(closure))
            library_revision_fixture(root, closure_path, baseline["image_sha256"])
            (root / "orig/code.bin").write_bytes(image)
            exheader = bytearray(0x200)
            for offset, segment in zip((0x10, 0x20, 0x30), (layout.text, layout.rodata, layout.data)):
                struct.pack_into("<III", exheader, offset, segment.address, 1, segment.size)
            (root / "orig/exheader.bin").write_bytes(exheader)
            output = root / "proposal-v10-r5.json"
            command = [sys.executable, "-B", "tools/analyze.py", "catalogue", "--out", str(output)]
            result = subprocess.run(command, cwd=root, capture_output=True, text=True, check=False)
            self.assertEqual(result.returncode, 0, result.stderr)
            candidate = json.loads(output.read_bytes())
            self.assertEqual(candidate["generator"]["revision"], "catalogue-v11")
            self.assertEqual(candidate["revision_review"]["name"], "catalogue-v11")
            self.assertIn("research/catalogue_v10_r5_evidence.json", candidate["generator"]["input_sha256"])
            self.assertIn("research/catalogue_v11_evidence.json", candidate["generator"]["input_sha256"])
            self.assertEqual(candidate["revision_review"]["data_endian_repair"]["revision"], "catalogue-v10-r5")
            target = layout.text.address + 44
            physical = next(row for row in candidate["ranges"] if row["start"] <= target < row["end"])
            self.assertEqual((physical["kind"], physical["owner"]), ("unknown", None))
            self.assertFalse(any(row["site"] == layout.text.address + 12
                                 for row in candidate["closure_evidence"]["read_footprints"]))
            self.assertEqual(candidate["revision_review"]["structural_issues"], [])
            self.assertEqual(analyze.validate_catalogue(candidate, image, layout), [])
            # An altered r5 parent hash must reach the public nonzero exit, not a
            # suppressed warning or fallback to the sealed r4 producer.
            repair_path = root / "research/catalogue_v10_r5_evidence.json"
            repair = json.loads(repair_path.read_bytes())
            repair["parent_evidence"]["sha256"] = "0" * 64
            repair_path.write_text(json.dumps(repair))
            refused_output = root / "refused-v10-r5.json"
            refused = subprocess.run([*command[:-1], str(refused_output)], cwd=root,
                                     capture_output=True, text=True, check=False)
            self.assertEqual(refused.returncode, 1)
            self.assertIn("v10-r5 evidence differs", refused.stderr)
            self.assertFalse(refused_output.exists())


if __name__ == "__main__":
    unittest.main()
