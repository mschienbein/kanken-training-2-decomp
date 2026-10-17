"""Real compiler/linker builds against a synthetic image, never private ROM inputs."""
import json
import struct
import sys
import tempfile
import unittest
from contextlib import ExitStack
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import build  # noqa: E402
import ctrfmt  # noqa: E402
import image  # noqa: E402
import toolchain  # noqa: E402

PROFILE = toolchain.Profile(
    "game", "armcc_504_82",
    ("--cpu=MPCore", "--fpmode=fast", "--apcs=/interwork", "--arm", "--split_sections", "-O3", "-Otime"),
)
HAVE_COMPILER = toolchain.armcc_exe(PROFILE.compiler).is_file()


class SyntheticBuildFixture:
    """Shared environment for build, collect and checkpoint integration tests."""

    def setUp(self):
        super().setUp()
        temp = tempfile.TemporaryDirectory()
        self.addCleanup(temp.cleanup)
        self.root = Path(temp.name)
        stack = ExitStack()
        self.addCleanup(stack.close)
        code = bytearray(0x1000)
        functions = []
        for address, value in ((0x100000, 1), (0x100010, 2), (0x100020, 3)):
            # Independently encoded synthetic ARM: mov r0, #value; bx lr.
            struct.pack_into("<II", code, address - 0x100000, 0xE3A00000 | value, 0xE12FFF1E)
            functions.append({"address": address, "size": 8, "isa": "arm", "kind": "function"})
        layout = ctrfmt.CodeLayout(
            "SYNTH", False, ctrfmt.CodeSegment("text", 0x100000, 1, 0x1000),
            ctrfmt.CodeSegment("rodata", 0x101000, 0, 0), ctrfmt.CodeSegment("data", 0x200000, 0, 0),
            0x1000, 0, (),
        )
        self.image = image.Image(bytes(code), layout)
        catalogue = {"image_sha256": self.image.sha256, "functions": functions}
        for rel, text in {
            "config/matches.json": json.dumps({"schema_version": 1, "units": []}),
            "config/catalogue.json": json.dumps(catalogue),
            "config/compiler_profiles.json": "{}",
            "toolchain.lock.json": "{}",
        }.items():
            self.write_source(rel, text)
        (self.root / "docs").mkdir()
        (self.root / "orig/exefs").mkdir(parents=True)
        (self.root / "orig/exefs/code.blz").write_bytes(ctrfmt.blz_compress(self.image.code))
        (self.root / "orig/exheader.bin").write_bytes(b"synthetic exheader hash input")
        for module, attr, value in (
            (build, "ROOT", self.root), (build, "MATCHES", self.root / "config/matches.json"),
            (build, "HARNESS", ()), (image, "CATALOGUE", self.root / "config/catalogue.json"),
            (image, "NAMES", self.root / "config/names.json"),
            (toolchain, "ROOT", self.root), (toolchain, "PROFILES", self.root / "config/compiler_profiles.json"),
            (toolchain, "CACHE", self.root / ".cache/objcache"),
        ):
            stack.enter_context(patch.object(module, attr, value))
        stack.enter_context(patch.object(image.Image, "load", return_value=self.image))
        stack.enter_context(patch.object(image, "load_catalogue", return_value=catalogue))
        stack.enter_context(patch.object(image, "load_names", return_value={}))
        stack.enter_context(patch.object(toolchain, "load_profiles", return_value={"game": PROFILE}))
        stack.enter_context(patch.object(build.denominator, "verify"))
        stack.enter_context(patch.object(build.denominator, "read_history", return_value=[{}]))

    def write_source(self, source, text):
        path = self.root / source
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8")
        return path

    def unit(self, source, *functions):
        return {"source": source, "profile": "game", "functions": list(functions)}

    def build_units(self, name, units):
        ledger = self.write_source(f"config/{name}.json", json.dumps({"units": units}))
        return build.build(name, ledger, fresh=True, jobs=1)


@unittest.skipUnless(HAVE_COMPILER, "pinned compiler not installed (tools/setup.py)")
class BuildTest(SyntheticBuildFixture, unittest.TestCase):
    def test_multi_function_unit_is_verified_function_by_function(self):
        self.write_source("src/native.c", "int func_00100000(void) { return 1; }\n"
                          "int func_00100010(void) { return 2; }\n")
        receipt = self.build_units("native", [self.unit("src/native.c", "func_00100000", "func_00100010")])
        self.assertEqual(receipt["result"], "PASS")
        self.assertEqual(receipt["credited"]["functions"], 2)
        self.assertFalse(receipt["units"][0]["cache_hit"])

    def test_unclaimed_duplicate_definition_is_refused(self):
        self.write_source("src/old.c", "int func_00100000(void) { return 1; }\n"
                          "int func_00100010(void) { return 2; }\n")
        self.write_source("src/new.c", "int func_00100000(void) { return 1; }\n")
        # The old unit no longer claims credit for the moved function, but still defines it.
        with self.assertRaisesRegex(build.BuildError, "defined by both"):
            self.build_units("duplicate", [self.unit("src/old.c", "func_00100010"),
                                           self.unit("src/new.c", "func_00100000")])

    def test_changed_function_cannot_keep_credit(self):
        self.write_source("src/new.c", "int func_00100000(void) { return 4; }\n")
        receipt = self.build_units("mismatch", [self.unit("src/new.c", "func_00100000")])
        self.assertEqual(receipt["result"], "FAIL")
        self.assertFalse(receipt["checks"]["credited_functions_exact"])
        self.assertEqual(receipt["mismatched_functions"], ["0x100000"])


if __name__ == "__main__":
    unittest.main()
