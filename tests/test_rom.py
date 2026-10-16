"""tools/rom.py: only pinned dumps are accepted; the extracted tree is re-verified."""
import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import rom  # noqa: E402


class RomIdentityTest(unittest.TestCase):
    def test_unknown_dump_is_rejected(self):
        with tempfile.NamedTemporaryFile(suffix=".3ds") as f:
            f.write(b"NCSD" * 1024)
            f.flush()
            with self.assertRaisesRegex(rom.InputError, "not an accepted input"):
                rom.identify(Path(f.name), rom.load_title())

    def test_encrypted_dump_requires_boot9(self):
        title = rom.load_title()
        encrypted = next(e for e in title["accepted_inputs"] if e["encrypted"])
        with tempfile.NamedTemporaryFile(suffix=".3ds") as f:
            original = rom.identify
            rom.identify = lambda *_: encrypted
            try:
                with self.assertRaisesRegex(rom.InputError, "boot9"):
                    rom.import_rom(Path(f.name), None, False)
            finally:
                rom.identify = original

    @unittest.skipUnless(rom.MANIFEST.is_file(), "private input not imported")
    def test_extracted_tree_matches_manifest_and_pins(self):
        manifest = rom.verify()
        pins = json.loads(rom.TITLE.read_text())["derived"]
        for rel, pin in pins.items():
            self.assertEqual(manifest["files"][rel], pin)


if __name__ == "__main__":
    unittest.main()
