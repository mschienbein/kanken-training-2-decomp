"""tools/toolchain.py object cache: a hit only when compiler, flags, source and every header agree."""
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import toolchain  # noqa: E402

PROFILE = toolchain.Profile("t", "armcc_504_82", ("--cpu=MPCore", "--arm", "--split_sections", "-O3", "-Otime"))


@unittest.skipUnless(toolchain.armcc_exe(PROFILE.compiler).is_file(), "pinned compiler not installed")
class ObjectCacheTest(unittest.TestCase):
    def setUp(self):
        self.saved = toolchain.CACHE
        self.tmp = Path(tempfile.mkdtemp())
        toolchain.CACHE = self.tmp / "cache"
        (self.tmp / "inc").mkdir()
        self.header = self.tmp / "inc/value.h"
        self.header.write_text("#define VALUE 3\n", encoding="utf-8")
        self.source = self.tmp / "unit.c"
        self.source.write_text('#include "value.h"\nint func_00100000(void) { return VALUE; }\n', encoding="utf-8")

    def tearDown(self):
        toolchain.CACHE = self.saved

    def compile(self, profile=PROFILE, cache=True, name="out.o"):
        out = self.tmp / name
        hit = toolchain.compile_source(profile, self.source, out, (self.tmp / "inc",), cache=cache)
        return hit, out.read_bytes()

    def test_hit_after_identical_compile(self):
        self.assertFalse(self.compile()[0])
        self.assertTrue(self.compile(name="again.o")[0])

    def test_header_change_misses(self):
        self.compile()
        self.header.write_text("#define VALUE 4\n", encoding="utf-8")
        hit, _ = self.compile(name="changed.o")
        self.assertFalse(hit)

    def test_flag_change_misses(self):
        self.compile()
        other = toolchain.Profile("t2", PROFILE.compiler, PROFILE.flags[:-1] + ("-Ospace",))
        self.assertFalse(self.compile(profile=other)[0])

    def test_fresh_compile_ignores_but_refreshes_cache(self):
        self.assertFalse(self.compile(cache=False)[0])
        self.assertTrue(self.compile(name="cached.o")[0])

    def test_compiler_system_header_is_tracked(self):
        # armcc finds <stddef.h> through its own install; under wibo --depend reports it as Z:/...
        self.source.write_text(
            "#include <stddef.h>\nstruct pair { int a; int b; };\n"
            "unsigned func_00100000(void) { return offsetof(struct pair, b); }\n", encoding="utf-8")
        self.assertFalse(self.compile()[0])
        self.assertTrue(self.compile(name="again.o")[0])


class DependPathTest(unittest.TestCase):
    @unittest.skipIf(toolchain.wibo_path() is None, "Windows hosts report native paths")
    def test_wibo_drive_paths_map_to_host_paths(self):
        text = "unit.o: Z:/usr/lib/armcc/bin/../include/stddef.h\nunit.o: z:\\opt\\inc\\x.h\nunit.o: inc/value.h\n"
        self.assertEqual(
            toolchain._parse_depend(text, Path("unit.c")),
            [Path("/usr/lib/armcc/bin/../include/stddef.h"), Path("/opt/inc/x.h"), Path("inc/value.h")],
        )

    @unittest.skipIf(toolchain.wibo_path() is None, "Windows hosts resolve case natively")
    def test_dependency_case_mismatch_resolves_to_the_existing_file(self):
        tmp = Path(tempfile.mkdtemp())
        (tmp / "inc" / "rw").mkdir(parents=True)
        (tmp / "inc" / "rw" / "_random.cc").write_text("x", encoding="utf-8")
        text = f"unit.o: Z:{tmp.as_posix()}/inc/RW/_random.CC\n"
        self.assertEqual(toolchain._parse_depend(text, Path("unit.c")), [tmp / "inc" / "rw" / "_random.cc"])


if __name__ == "__main__":
    unittest.main()
