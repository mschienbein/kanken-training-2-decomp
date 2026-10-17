"""C++ parse/edit/emit guards and the unchanged strict compilation/scoring path."""
import ctypes
import io
import json
import random
import shutil
import subprocess
import sys
import unittest
from pathlib import Path
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import inplace  # noqa: E402
import interfaces  # noqa: E402
import match  # noqa: E402
import permute  # noqa: E402
import permute_cpp as cpp  # noqa: E402
import toolchain  # noqa: E402
from tests.test_permute import HAVE_COMPILER, PROFILE, ScratchTest  # noqa: E402


SOURCE = """
template<class T> struct Pair { T a, b; };
int untouched(int x) { return x * 17; }
extern "C" int target(int x, int y, int *p, Pair<int> *s) {
    int value = x + y;
    s->a = value;
    if (x < y) value += 2; else value -= 3;
    while (y < 0) { ++y; if (y == -2) continue; }
    ++value;
    value += p[1];
    return value + x;
}
"""


def program(text=SOURCE, targets=("target",), original=None):
    return permute.Program.parse(text, targets, Path("fixture.cpp"), original)


class CppMutationTest(unittest.TestCase):
    def test_every_cpp_mutation_reparses_and_only_changes_target_body(self):
        source = program()
        self.assertIsInstance(source, cpp.Program)
        self.assertEqual(source.oracle_targets, ("target",))
        for kind in cpp.MUTATIONS:
            for seed in (0, 4):
                with self.subTest(kind=kind, seed=seed):
                    changed = permute.mutate(source, source.ast, random.Random(seed), kind)
                    self.assertIsNotNone(changed)
                    ast, description = changed
                    self.assertEqual(description, "target:" + kind)
                    rendered = source.render(ast)
                    self.assertNotEqual(rendered, SOURCE)
                    _, body, end = source.spans["target"]
                    self.assertEqual(rendered[:body], SOURCE[:body])
                    self.assertTrue(rendered.endswith(SOURCE[end:]))
                    reparsed = program(rendered)
                    self.assertEqual(reparsed.oracle_targets, source.oracle_targets)
                    self.assertEqual(source.ast, SOURCE)

    def test_preprocessing_is_the_interface_auditor_for_every_cpp_suffix(self):
        for suffix in (".cpp", ".cc", ".cxx", ".CPP"):
            source = Path("fixture" + suffix)
            with self.subTest(suffix=suffix):
                with mock.patch.object(interfaces, "preprocess", return_value=SOURCE) as preprocess:
                    with mock.patch.object(Path, "read_text", return_value=SOURCE):
                        result = permute.Program.preprocess(source, ("target",), PROFILE)
                preprocess.assert_called_once_with(interfaces.Unit(source, PROFILE))
                self.assertIsInstance(result, cpp.Program)
                self.assertEqual(result.render(result.ast), SOURCE)

    def test_cpp_cli_uses_search_and_cpp_author_hint_names(self):
        source = program()
        summary = {"score": 10, "initial_score": 12, "workers": 1, "candidates": 2,
                   "candidates_per_second": 1, "elapsed_seconds": 2, "time_to_match_seconds": None}
        with mock.patch.object(permute.Program, "preprocess", return_value=source):
            with mock.patch.object(permute, "search", return_value=summary) as search:
                with mock.patch.object(sys, "stdout", new_callable=io.StringIO) as stdout:
                    status = permute.main(["fixture.cpp", "target", "--workers", "1", "--seed", "7", "--max-candidates", "2", "--out", "scratch/cpp-run"])
        self.assertEqual(status, 1)
        self.assertEqual(search.call_args.args[0], source)
        self.assertEqual(search.call_args.kwargs["seed"], 7)
        self.assertEqual(search.call_args.kwargs["max_candidates"], 2)
        self.assertIn("best.functions.cpp", stdout.getvalue())

    def test_namespaces_inline_and_out_of_line_methods_and_templates(self):
        text = """
namespace N {
template<class T> struct Box { T value; };
class Thing {
public:
    Thing(int x) : value(x) {}
    ~Thing() {}
    int other(int x) const { return x - 2; }
    int target(int x) const;
    int value;
};
int Thing::target(int x) const { return value + x; }
}
"""
        source = program(text, ("N::Thing::target",), text)
        self.assertEqual(source.oracle_targets, ("_ZNK1N5Thing6targetEi",))
        changed = permute.mutate(source, source.ast, random.Random(1), "commute")
        self.assertIsNotNone(changed)
        rendered = source.render(changed[0])
        self.assertIn("Thing(int x) : value(x) {}", rendered)
        self.assertIn("~Thing() {}", rendered)
        self.assertIn("int other(int x) const { return x - 2; }", rendered)
        hints, patch = source.hints(changed[0])
        self.assertTrue(hints.startswith("int Thing::target"))
        self.assertIn("original/N::Thing::target.cpp", patch)
        # Both constructor/destructor boundaries remain distinct in author text.
        program(text, ("N::Thing::Thing", "N::Thing::~Thing"), text)

    def test_overloads_need_unique_selector_and_mangled_names_resolve(self):
        text = "int target(int x){return x+1;} int target(unsigned x){return x+2;}\n"
        with self.assertRaisesRegex(permute.PermuteError, "ambiguous"):
            program(text)
        source = program(text, ("_Z6targeti",), text)
        changed = permute.mutate(source, source.ast, random.Random(0), "commute")
        self.assertIsNotNone(changed)
        self.assertIn("int target(unsigned x){return x+2;}", source.render(changed[0]))
        with self.assertRaisesRegex(permute.PermuteError, "same C\\+\\+ body"):
            program('namespace N { int target(int x){return x+1;} }', ("N::target", "_ZN1N6targetEi"))

    def test_missing_and_invalid_cpp_are_refused_without_recovery_ast(self):
        for text in ("int other(int x){return x;}", "int target(int x){return missing + x;}",
                     "int target(int x){ return x + ; }"):
            with self.subTest(text=text):
                with self.assertRaises(permute.PermuteError):
                    program(text)

    def test_utf8_and_int64_offsets_preserve_armcc_extensions_and_other_bodies(self):
        prefix = (
            '/* 日本語: __int64 and braces { } are comments. */\n'
            'typedef __int64 Wide;\n'
            '__packed struct Record { int x; };\n'
            'struct Result { int result; unsigned handle; };\n'
            '__svc(0x17) __value_in_regs Result svcCreateEvent(unsigned, int);\n'
            'static __forceinline unsigned char *tls() {\n'
            ' register unsigned value __asm("cp15:0:c13:c0:3");\n'
            ' return (unsigned char *)value;\n}\n'
        )
        target = '__declspec(noinline) __int64 target(__int64 x) { return x + 1; }\n'
        suffix = '__weak int target_suffix(int x) { return x - 1; }\n'
        source = program(prefix + target + suffix)
        changed = permute.mutate(source, source.ast, random.Random(1), "commute")
        rendered = source.render(changed[0])
        self.assertTrue(rendered.startswith(prefix + '__declspec(noinline) __int64 target(__int64 x) '))
        self.assertTrue(rendered.endswith(suffix))
        self.assertEqual(source.original_functions["target"], target)
        self.assertIn('__int64 target(__int64 x)', source.hints(changed[0])[0])

    def test_annotations_inside_body_and_non_cp15_assembly_fail_closed(self):
        prefix = '/* 日本語 */\n' + 'typedef __int64 Wide;\n' * 20
        with self.assertRaisesRegex(permute.PermuteError, "target body has ABI-relevant"):
            program(prefix + 'int target(){__packed struct Local {int x;} a; return a.x;}')
        for text in ('register int reg __asm("r0"); int target(){return reg;}',
                     '__asm void helper() {} int target(){return 1;}',
                     '#pragma pack(1)\nstruct S{int x;}; int target(){return 1;}'):
            with self.subTest(text=text):
                with self.assertRaises(permute.PermuteError):
                    program(text)

    def test_original_hints_respect_macros_continued_comments_and_line_markers(self):
        prefix = '#define BEFORE(x) \\\n  ((x) + 2)\n// continued comment \\\nint hidden() { return 0; }\n'
        target = 'extern "C" int target(int x) { return BEFORE(x); }'
        suffix = '\n#define AFTER(x) \\\n  int hidden_after() { return x; }\n'
        for newline in ("\n", "\r\n"):
            original = (prefix + target + suffix).replace("\n", newline)
            text = '#line 5 "' + str(Path("fixture.cpp").resolve()) + '"\nextern "C" int target(int x) { return ((x) + 2); }\n'
            source = program(text, original=original)
            self.assertEqual(source.original_functions["target"], target + "\n")
            changed = permute.mutate(source, source.ast, random.Random(3), "commute")
            hints, patch = source.hints(changed[0])
            self.assertNotIn("hidden", hints)
            self.assertIn("-" + target, patch)
        with self.assertRaisesRegex(permute.PermuteError, "macro-generated"):
            program('extern "C" int target(int x){return x+1;}', original='#define DEFINE(name) int name(int x){return x+1;}\nDEFINE(target)')

    def test_user_defined_operators_conversions_and_volatile_are_not_rewritten(self):
        overloads = (
            ('struct S { S operator+(const S&) const; }; S target(S a,S b){return a+b;}', "commute"),
            ('struct S { bool operator<(const S&) const; }; bool target(S a,S b){return a<b;}', "comparison"),
            ('struct S { S& operator++(); }; void target(S &s){++s;}', "increment"),
            ('struct S { int &operator[](int); }; int target(S &s){return s[1];}', "pointer"),
            ('struct S { operator bool() const; bool operator!() const; }; int target(S s){if(s)return 1;else return 2;}', "condition"),
            ('int target(volatile int x, int y){return x+y;}', "commute"),
            ('int target(volatile int *p){++*p;return 0;}', "increment"),
            ('double target(double x,double y){return x+y;}', "commute"),
            ('int target(int x){if(int y=x)return y;return 0;}', "condition"),
        )
        for text, kind in overloads:
            with self.subTest(kind=kind, text=text):
                source = program(text)
                self.assertIsNone(permute.mutate(source, source.ast, random.Random(0), kind))

    def test_enum_condition_does_not_introduce_an_overloaded_negation(self):
        source = program('enum Flag {Off, On}; bool operator!(Flag f){return true;} int target(Flag f){if(f)return 1;else return 2;}')
        changed = permute.mutate(source, source.ast, random.Random(1), "condition")
        self.assertIsNotNone(changed)
        self.assertIn("(bool)", source.render(changed[0]))
        _, tree = cpp.parse_tree(source.render(changed[0]), source.source)
        _, body = cpp.resolve(cpp.definitions(tree), source.targets)["target"]
        self.assertFalse(any(n.kind.name == "CALL_EXPR" for n in cpp.walk(body)))

    def test_nested_class_methods_are_not_treated_as_the_outer_function(self):
        source = program('int target(int x){struct Local { int nested(int y){return y+1;} };return x;}')
        self.assertIsNone(permute.mutate(source, source.ast, random.Random(0), "temporary"))
        self.assertIsNone(permute.mutate(source, source.ast, random.Random(0), "commute"))

    def test_value_used_increment_is_not_mutated(self):
        source = program("int target(int x){return x++;}")
        self.assertIsNone(permute.mutate(source, source.ast, random.Random(0), "increment"))

    def test_seed_reproduces_chained_mutation_and_hill_climbing(self):
        def trace(seed):
            source = program()
            climber = permute.Climber(source, seed, permute.Evaluation(100))
            result = []
            for index in range(10):
                proposal = climber.propose()
                self.assertIsNotNone(proposal)
                ast, description = proposal
                result.append((description, source.render(ast)))
                climber.observe(ast, permute.Evaluation(100 - index % 3))
            return result
        self.assertEqual(trace(82), trace(82))
        self.assertNotEqual(trace(82), trace(83))


@unittest.skipUnless(HAVE_COMPILER, "pinned compiler/launcher not installed (tools/setup.py)")
class CppCompilerTest(ScratchTest):
    def test_every_cpp_mutation_and_chains_compile_with_armcc(self):
        source = program()
        for kind in cpp.MUTATIONS:
            with self.subTest(kind=kind):
                changed = permute.mutate(source, source.ast, random.Random(4), kind)
                self.assertIsNotNone(changed)
                path = self.directory / f"{kind}.cpp"
                path.write_text(source.render(changed[0]), encoding="utf-8")
                toolchain.compile_source(PROFILE, path, path.with_suffix(".o"), cache=False)
        ast, rng = source.ast, random.Random(1)
        for step in range(8):
            ast, _ = permute.mutate(source, ast, rng)
        path = self.directory / "chain.cpp"
        path.write_text(source.render(ast), encoding="utf-8")
        toolchain.compile_source(PROFILE, path, path.with_suffix(".o"), cache=False)

    def test_armcc_preprocessing_preserves_approved_extension_headers(self):
        path = self.directory / "extensions.cpp"
        path.write_text(
            '#include "ctr/svc.h"\n#include "ctr/cp15.h"\n#include <stdint.h>\n'
            'extern "C" __int64 target(__int64 x) { return x + 1; }\n', encoding="utf-8",
        )
        source = permute.Program.preprocess(path, ("target",), PROFILE)
        changed = permute.mutate(source, source.ast, random.Random(1), "commute")
        self.assertIsNotNone(changed)
        rendered = source.render(changed[0])
        self.assertIn("__svc(0x17) __value_in_regs", rendered)
        self.assertIn('__asm("cp15:0:c13:c0:3")', rendered)
        self.assertIn("__schedule_barrier();", rendered)
        self.assertNotIn(cpp.PARSER_INTRINSICS, rendered)
        path = self.directory / "emitted.cpp"
        path.write_text(rendered, encoding="utf-8")
        toolchain.compile_source(PROFILE, path, path.with_suffix(".o"), cache=False)


@unittest.skipUnless(HAVE_COMPILER, "pinned compiler/launcher not installed (tools/setup.py)")
class CppOracleTest(ScratchTest):
    def setUp(self):
        super().setUp()
        path = self.directory / "reference.cpp"
        path.write_text('extern "C" int func_00100000(int x){return x+3;}\n', encoding="utf-8")
        toolchain.compile_source(PROFILE, path, path.with_suffix(".o"), cache=False)
        obj = inplace.ObjectFile(path.with_suffix(".o"))
        function = obj.defined_functions()[0]
        data = obj.sections[function.shndx].data

        class Memory:
            bss = (0x200000, 0x201000)

            def read(self, address, size):
                offset = address - 0x100000
                return (data + b"\0" * 1024)[offset:offset + size]

        context = match.Context.__new__(match.Context)
        context.image = Memory()
        context.functions = {0x100000: {"address": 0x100000, "size": len(data), "isa": "arm"}}
        context.resolver = inplace.Resolver(context.functions)
        context.model = inplace.linker_model(permute.MODEL)
        context.literal_model = inplace.DEFAULT_LITERAL_MODEL
        self.oracle = permute.Oracle.__new__(permute.Oracle)
        self.oracle.profile, self.oracle.targets, self.oracle.context = PROFILE, ("func_00100000",), context
        self.oracle.suffix = ".cpp"

    def test_cpp_oracle_and_saved_candidates_keep_cpp_extension_and_numeric_scores(self):
        text = 'template<class T> struct Unused {T value;};\nextern "C" int func_00100000(int x){return x*7+3;}\n'
        source = permute.Program.parse(text, self.oracle.targets, self.directory / "input.cpp")
        result = self.oracle.evaluate(source.text, self.directory / "cpp-worker")
        self.assertGreater(result.score, 0)
        permute.save_candidate(self.directory, "cpp-best", source, None, result, {})
        emitted = self.directory / "cpp-best.cpp"
        self.assertTrue(emitted.is_file())
        self.assertFalse((self.directory / "cpp-best.c").exists())
        self.assertTrue((self.directory / "cpp-best.functions.cpp").is_file())
        toolchain.compile_source(PROFILE, emitted, emitted.with_suffix(".o"), cache=False)
        row = self.oracle.context.compare(emitted.with_suffix(".o"))[0]
        original, candidate = row["_original"][:row["size"]], row["_candidate"]
        record = json.loads((self.directory / "cpp-best.json").read_text())
        numbers = record["functions"][0]
        self.assertEqual(numbers["changed_bytes"], sum(a != b for a, b in zip(original, candidate)) + abs(len(original) - len(candidate)))
        self.assertIn("aligned_changed_bytes", numbers)
        self.assertNotIn("_original", json.dumps(record))
        self.assertEqual(record["score"], numbers["instruction_changes"] * 16 + numbers["aligned_changed_bytes"] + abs(numbers["size_delta"]) * 8)
        exact = self.oracle.evaluate('extern "C" int func_00100000(int x){return x+3;}', self.directory / "cpp-worker")
        self.assertEqual(exact.score, 0)

    def test_bounded_multiprocessing_cpp_search_is_deterministic(self):
        # Spawned workers use the ordinary real-image strict oracle. The scalar
        # fixture deliberately differs from the pilot, so a bounded search runs.
        required = [permute.ROOT / "orig" / n for n in ("code.bin", "exheader.bin")]
        if not all(p.is_file() for p in required):
            self.skipTest("private image required for spawned oracle workers")
        text = 'template<class T> struct Box {T value;};\nextern "C" int func_001091d4(int x){if(x<3)x++;return x*7+5;}\n'
        source = permute.Program.parse(text, ("func_001091d4",), self.directory / "search.cpp")
        summaries = []
        for name in ("cpp-one", "cpp-two"):
            summaries.append(permute.search(source, PROFILE, self.directory / name, seconds=90, workers=2, seed=38, max_candidates=6))
        for key in ("score", "initial_score", "candidates", "invalid_candidates", "improvements", "stop", "functions", "target_symbols"):
            self.assertEqual(summaries[0][key], summaries[1][key], key)
        self.assertEqual(summaries[0]["candidates"], 6)
        for suffix in ("cpp", "functions.cpp", "json", "diff"):
            self.assertEqual((self.directory / "cpp-one" / f"best.{suffix}").read_bytes(), (self.directory / "cpp-two" / f"best.{suffix}").read_bytes())


@unittest.skipUnless(sys.platform in {"linux", "darwin"} and shutil.which("c++"), "optional native C++ compiler required")
class CppNativeSemanticsTest(ScratchTest):
    def test_mutations_preserve_scalar_returns_and_memory(self):
        class Pair(ctypes.Structure):
            _fields_ = [("a", ctypes.c_int), ("b", ctypes.c_int)]
        source = program()
        for kind in cpp.MUTATIONS:
            with self.subTest(kind=kind):
                ast, _ = permute.mutate(source, source.ast, random.Random(4), kind)
                original = SOURCE.replace("target", "reference")
                candidate = source.render(ast).replace("target", "candidate")
                path = self.directory / f"native-{kind}.cpp"
                path.write_text("namespace Ref {" + original + "}\nnamespace Candidate {" + candidate + "}\n", encoding="utf-8")
                library = path.with_suffix(".so")
                compiled = subprocess.run(["c++", "-std=c++98", "-O0", "-shared", "-fPIC", str(path), "-o", str(library)], capture_output=True, text=True)
                self.assertEqual(compiled.returncode, 0, compiled.stderr)
                module = ctypes.CDLL(str(library))
                for name in ("reference", "candidate"):
                    function = getattr(module, name)
                    function.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_int), ctypes.POINTER(Pair)]
                    function.restype = ctypes.c_int
                for x in (-3, 0, 5):
                    for y in (-4, 0, 2):
                        observations = []
                        for name in ("reference", "candidate"):
                            array, pair = (ctypes.c_int * 3)(3, 7, 11), Pair(2, 4)
                            value = getattr(module, name)(x, y, array, ctypes.byref(pair))
                            observations.append((value, tuple(array), pair.a, pair.b))
                        self.assertEqual(observations[0], observations[1])


if __name__ == "__main__":
    unittest.main()
