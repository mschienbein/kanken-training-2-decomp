"""ROM-free tests for the optional, private Ghidra author aid."""
from concurrent.futures import ThreadPoolExecutor
import hashlib
import json
import os
from pathlib import Path
import struct
import sys
import tempfile
import time
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import decompile  # noqa: E402
import ctrfmt  # noqa: E402
import image  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]


class DecompilerFixture(unittest.TestCase):
    def setUp(self):
        scratch = ROOT / "scratch"
        scratch.mkdir(exist_ok=True)
        self.temporary = tempfile.TemporaryDirectory(prefix="test-decompile-", dir=scratch)
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        (self.root / "orig").mkdir()
        (self.root / "config").mkdir()
        # Entirely synthetic ARM and Thumb: return the first argument plus one.
        code = bytearray(3 * 0x1000)
        code[:8] = struct.pack("<II", 0xE2800001, 0xE12FFF1E)
        code[8:12] = struct.pack("<HH", 0x3001, 0x4770)
        layout = ctrfmt.CodeLayout("test", False,
            ctrfmt.CodeSegment("text", 0x100000, 1, 12),
            ctrfmt.CodeSegment("rodata", 0x102000, 1, 4),
            ctrfmt.CodeSegment("data", 0x104000, 1, 4), 16, 0, ())
        self.image = image.Image(bytes(code), layout)
        (self.root / "orig/code.bin").write_bytes(code)
        self.functions = [
            {"address": 0x100000, "size": 8, "isa": "arm", "code_ranges": [[0x100000, 0x100008]]},
            {"address": 0x100008, "size": 4, "isa": "thumb", "code_ranges": [[0x100008, 0x10000c]]},
        ]
        self.catalogue = {"functions": self.functions}
        (self.root / "config/catalogue.json").write_text(json.dumps(self.catalogue))
        (self.root / "config/names.json").write_text(json.dumps({"symbols": {}}))
        self.ghidra = self.root / "fake-ghidra"
        (self.ghidra / "support").mkdir(parents=True)
        (self.ghidra / "support/analyzeHeadless").touch()
        (self.ghidra / "Ghidra").mkdir()
        (self.ghidra / "Ghidra/application.properties").write_text("application.version=test")
        self.java = self.root / "fake-java"
        (self.java / "bin").mkdir(parents=True)
        (self.java / "bin/java").touch()
        self.calls = []
        self.loader = patch.object(image.Image, "load", return_value=self.image)
        self.loader.start()
        self.addCleanup(self.loader.stop)

    def invoke(self, target="func_00100000", **kwargs):
        return decompile.decompile(target, root=self.root, ghidra=self.ghidra, java_home=self.java, **kwargs)

    def fake_headless(self, command, env, log, deadline):
        self.calls.append(command)
        marker = command.index("-postScript")
        source, metadata = Path(command[marker + 6]), Path(command[marker + 7])
        address = int(command[marker + 4])
        source.write_text(f"int func_{address:08x}(int value) {{ return value + 1; }}\n")
        metadata.write_text(json.dumps({"completed": True, "address": address, "basic_blocks": 1}))
        (Path(command[1]) / "kanken.gpr").touch()
        log.write_text("synthetic headless log")


class ManifestTests(DecompilerFixture):
    def test_function_name_and_overlay(self):
        selected = decompile.select_function("alias", self.catalogue, {"alias": {"address": 0x100008}})
        self.assertEqual(selected["isa"], "thumb")
        self.assertEqual(decompile.select_function("func_00100000", self.catalogue, {}), self.functions[0])

    def test_invalid_and_unmapped_names(self):
        for name in ("../escape", "func_00100000.c", "func_00100001", "func_00000000", "0x100000"):
            with self.subTest(name=name), self.assertRaises(decompile.DecompileError):
                decompile.select_function(name, self.catalogue, {})

    def test_classified_entry_refused(self):
        self.functions[0]["kind"] = "veneer"
        with self.assertRaises(decompile.DecompileError):
            decompile.select_function("func_00100000", self.catalogue, {})

    def test_subtract_literals(self):
        self.assertEqual(decompile.subtract_ranges([[10, 30]], [[12, 16], [20, 24]]),
                         [[10, 12], [16, 20], [24, 30]])
        self.assertEqual(decompile.subtract_ranges([[10, 30]], [[0, 40]]), [])
        self.assertEqual(decompile.subtract_ranges([[10, 30]], [[0, 5], [40, 50]]), [[10, 30]])

    def test_segment_offsets_do_not_assume_contiguous_virtual_addresses(self):
        manifest = decompile.make_manifest(self.image, self.catalogue, {}, self.root / "orig/code.bin")
        self.assertEqual([s["address"] for s in manifest["segments"]], [0x100000, 0x102000, 0x104000])
        self.assertEqual([s["offset"] for s in manifest["segments"]], [0, 0x1000, 0x2000])
        self.assertEqual(manifest["bss_address"], 0x104004)
        self.assertEqual(manifest["functions"][1]["isa"], "thumb")

    def test_literal_pool_removed_from_code(self):
        self.functions[0]["pools"] = [[0x100004, 0x100008]]
        manifest = decompile.make_manifest(self.image, self.catalogue, {}, self.root / "orig/code.bin")
        self.assertEqual(manifest["functions"][0]["code_ranges"], [[0x100000, 0x100004]])

    def test_invalid_ranges_rejected(self):
        self.functions[0]["code_ranges"] = [[0x100000, 0x100010]]
        with self.assertRaises(decompile.DecompileError):
            decompile.make_manifest(self.image, self.catalogue, {}, self.root / "orig/code.bin")

    def test_fingerprint_changes_for_names_modes_and_tool_version(self):
        manifest = decompile.make_manifest(self.image, self.catalogue, {}, self.root / "orig/code.bin")
        original = decompile.fingerprint(self.image, manifest, self.ghidra)
        self.assertEqual(original, decompile.fingerprint(self.image, manifest, self.ghidra))
        manifest["functions"][0]["name"] = "renamed"
        self.assertNotEqual(original, decompile.fingerprint(self.image, manifest, self.ghidra))
        manifest["functions"][0]["name"] = "func_00100000"
        manifest["functions"][0]["isa"] = "thumb"
        self.assertNotEqual(original, decompile.fingerprint(self.image, manifest, self.ghidra))
        manifest["functions"][0]["isa"] = "arm"
        (self.ghidra / "Ghidra/application.properties").write_text("application.version=changed")
        self.assertNotEqual(original, decompile.fingerprint(self.image, manifest, self.ghidra))

    def test_shared_cache_location_and_escape_refusal(self):
        good = self.root / "scratch/decompile/test"
        self.assertEqual(decompile.safe_scratch(good, self.root), good)
        with self.assertRaises(decompile.DecompileError):
            decompile.safe_scratch(self.root / "published.c", self.root)
        (self.root / "scratch").mkdir(exist_ok=True)
        (self.root / "scratch/escape").symlink_to(self.root / "config", target_is_directory=True)
        with self.assertRaises(decompile.DecompileError):
            decompile.safe_scratch(self.root / "scratch/escape/output.c", self.root)


class WrapperTests(DecompilerFixture):
    def test_initialization_followed_by_lock_free_cache_hit(self):
        with patch.object(decompile, "run_headless", side_effect=self.fake_headless):
            result = self.invoke()
        with patch.object(decompile, "project_lock", side_effect=AssertionError("cache took project lock")):
            self.assertEqual(self.invoke(), result)
        self.assertEqual(len(self.calls), 1)
        self.assertIn("-import", self.calls[0])
        receipt = json.loads(result.with_suffix(".json").read_text())
        self.assertTrue(receipt["initialized_project"])
        self.assertEqual(receipt["source_sha256"], hashlib.sha256(result.read_bytes()).hexdigest())

    def test_second_function_reuses_read_only_project(self):
        with patch.object(decompile, "run_headless", side_effect=self.fake_headless):
            self.invoke()
            result = self.invoke("func_00100008")
        self.assertEqual(len(self.calls), 2)
        self.assertIn("-process", self.calls[1])
        self.assertIn("-readOnly", self.calls[1])
        self.assertEqual(self.calls[0][1], self.calls[1][1])
        self.assertFalse(json.loads(result.with_suffix(".json").read_text())["initialized_project"])

    def test_tampered_cache_is_not_reused(self):
        with patch.object(decompile, "run_headless", side_effect=self.fake_headless):
            result = self.invoke()
            result.write_text("changed")
            self.invoke()
        self.assertEqual(len(self.calls), 2)

    def test_parallel_requests_share_one_import(self):
        with patch.object(decompile, "run_headless", side_effect=self.fake_headless):
            with ThreadPoolExecutor(max_workers=3) as pool:
                outputs = list(pool.map(lambda _: self.invoke(), range(3)))
        self.assertEqual(len(set(outputs)), 1)
        self.assertEqual(len(self.calls), 1)

    def test_zero_exit_without_script_outputs_fails_closed(self):
        with patch.object(decompile, "run_headless", return_value=None):
            with self.assertRaisesRegex(decompile.DecompileError, "complete result"):
                self.invoke()
        self.assertFalse(list((self.root / "scratch").rglob("ready.json")))

    def test_wrong_address_is_not_published(self):
        def wrong_address(*args):
            self.fake_headless(*args)
            command = args[0]
            marker = command.index("-postScript")
            Path(command[marker + 7]).write_text('{"completed": true, "address": 1}')
        with patch.object(decompile, "run_headless", side_effect=wrong_address):
            with self.assertRaisesRegex(decompile.DecompileError, "Invalid Ghidra result"):
                self.invoke()
        self.assertFalse(list((self.root / "scratch").rglob("func_00100000.c")))
        self.assertFalse(list((self.root / "scratch").rglob(".*.c")))

    def test_missing_runtimes_give_install_instructions(self):
        (self.ghidra / "support/analyzeHeadless").unlink()
        with self.assertRaisesRegex(decompile.DecompileError, "Ghidra not installed"):
            self.invoke()

    def test_missing_java_gives_install_instructions(self):
        (self.java / "bin/java").unlink()
        with self.assertRaisesRegex(decompile.DecompileError, "JDK not installed"):
            self.invoke()

    def test_runtime_budgets_rejected(self):
        for seconds in (0, -1, float("nan"), float("inf")):
            with self.subTest(seconds=seconds), self.assertRaises(decompile.DecompileError):
                self.invoke(seconds=seconds)
        with self.assertRaises(decompile.DecompileError):
            self.invoke(function_seconds=0)

    def test_contended_lock_has_bounded_wait(self):
        lock = self.root / "lock"
        with decompile.project_lock(lock, time.monotonic() + 1):
            with self.assertRaisesRegex(decompile.DecompileError, "wall-time"):
                with decompile.project_lock(lock, time.monotonic() + 0.02):
                    self.fail("contended lock acquired")

    def test_failed_process_never_echoes_private_log(self):
        log = self.root / "private.log"
        with self.assertRaises(decompile.DecompileError) as failure:
            decompile.run_headless([sys.executable, "-c", "print('PRIVATE'); raise SystemExit(7)"],
                                   dict(os.environ), log, time.monotonic() + 5)
        self.assertIn("exited 7", str(failure.exception))
        self.assertNotIn("PRIVATE", str(failure.exception))
        self.assertIn("PRIVATE", log.read_text())

    def test_process_timeout_kills_its_process_group(self):
        log = self.root / "private.log"
        start = time.monotonic()
        with self.assertRaisesRegex(decompile.DecompileError, "timed out"):
            decompile.run_headless([sys.executable, "-c", "import time; time.sleep(30)"],
                                   dict(os.environ), log, start + 0.05)
        self.assertLess(time.monotonic() - start, 3)


@unittest.skipUnless(os.environ.get("DECOMPILE_INTEGRATION") == "1", "set DECOMPILE_INTEGRATION=1 for local Ghidra")
class GhidraIntegrationTests(DecompilerFixture):
    def test_real_arm_and_thumb_share_catalogue_seeded_project(self):
        self.ghidra = Path(os.environ.get("GHIDRA_HOME", decompile.DEFAULT_GHIDRA))
        self.java = Path(os.environ.get("JAVA_HOME", decompile.DEFAULT_JAVA))
        arm = self.invoke()
        thumb = self.invoke("func_00100008")
        for path in (arm, thumb):
            text = path.read_text()
            self.assertIn("+ 1", text)
            self.assertNotIn("halt_baddata", text)
            self.assertTrue(json.loads(path.with_suffix(".json").read_text())["completed"])
        self.assertEqual(arm.parent, thumb.parent)
        self.assertEqual(self.invoke(), arm)
        self.assertFalse(json.loads(thumb.with_suffix(".json").read_text())["initialized_project"])


if __name__ == "__main__":
    unittest.main()
