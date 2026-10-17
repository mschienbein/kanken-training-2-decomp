"""Conservative source mutations, strict oracle scoring and reproducible searches.

Compiler tests need the pinned toolchain; image tests additionally need orig/.
All disposable files and compiler caches stay underneath scratch/.
"""
import ctypes
import json
import random
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from pycparser import c_ast, c_parser

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import image  # noqa: E402
import inplace  # noqa: E402
import interfaces  # noqa: E402
import match  # noqa: E402
import permute  # noqa: E402
import toolchain  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
PROFILE = toolchain.load_profiles()["game"]
HAVE_COMPILER = toolchain.armcc_exe(PROFILE.compiler).is_file() and (
    toolchain.wibo_path() is None or toolchain.wibo_path().is_file()
)
HAVE_IMAGE = HAVE_COMPILER and all((ROOT / "orig" / p).is_file() for p in ("code.bin", "exheader.bin"))

SOURCES = (
    """
struct Pair { int a; int b; };
int untouched(int x) { return x * 7; }
int target(int x, int y, int *p, struct Pair *s) {
    int a = x + 3;
    int b = y * 5;
    int t = a + b;
    int index = 1;
    int z;
    int i;
    z = t ^ a;
    s->a = a;
    s->b = b;
    if (x < y) { a += 1; } else { b -= 1; }
    for (i = 0; i < 2; i++) { z += p[i]; }
    while (b < 0) { b++; if (b == -1) break; }
    do { z ^= a; a--; } while (a > 10);
    return z + t + p[index];
}
""",
    """
struct Pair { int a; int b; };
int target(int x, int y, int *p, struct Pair *s) {
    int a = x + 1;
    int b = y + 2;
    s->a = a;
    s->b = b;
    {
        int t = a + b;
        return t;
    }
}
int untouched(int x) { return x - 9; }
""",
    """
int target(int x, int y, int *p) {
    int index = 1;
    unsigned int bits = (unsigned int)x;
    int result = (int)(bits ^ (unsigned int)y);
    for (x = 0; x < 2; ++x) result += p[x];
    if (result > 10) result--; else result++;
    return result + p[index];
}
""",
)


def program(text=SOURCES[0]):
    return permute.Program.parse(text, ("target",), Path("fixture.c"))


class ScratchTest(unittest.TestCase):
    def setUp(self):
        (ROOT / "scratch").mkdir(exist_ok=True)
        self.temporary = tempfile.TemporaryDirectory(prefix="test-permute-", dir=ROOT / "scratch")
        self.directory = Path(self.temporary.name)
        self.addCleanup(self.temporary.cleanup)
        self.cache = toolchain.CACHE
        toolchain.CACHE = self.directory / "cache"
        self.addCleanup(setattr, toolchain, "CACHE", self.cache)


class MutationTest(unittest.TestCase):
    def test_every_mutation_reparses_across_synthetic_functions(self):
        exercised = set()
        for text in SOURCES:
            source = program(text)
            for kind in permute.MUTATIONS:
                for seed in range(4):
                    with self.subTest(kind=kind, seed=seed, fixture=SOURCES.index(text)):
                        changed = permute.mutate(source, source.ast, random.Random(seed), kind)
                        if changed is None:
                            continue
                        ast, description = changed
                        exercised.add(kind)
                        self.assertTrue(description.endswith(":" + kind))
                        rendered = source.render(ast)
                        c_parser.CParser().parse(interfaces.prepare(rendered, Path("mutated.c")).text)
                        self.assertEqual(source.text, text)
        self.assertEqual(exercised, set(permute.MUTATIONS))

    def test_only_target_body_changes(self):
        source = program()
        for kind in permute.MUTATIONS:
            changed = permute.mutate(source, source.ast, random.Random(7), kind)
            if changed is None:
                continue
            rendered = source.render(changed[0])
            start, body, end = source.spans["target"]
            self.assertEqual(rendered[:body], source.text[:body])
            self.assertTrue(rendered.endswith(source.text[end:]))
            self.assertEqual(permute.GENERATOR.visit(source.ast), permute.GENERATOR.visit(program().ast))

    def test_seed_reproduces_mutation_and_hill_climbing(self):
        source = program()
        baseline = permute.Evaluation(100)
        def trace(seed):
            climber = permute.Climber(source, seed, baseline)
            results = []
            for iteration in range(20):
                proposal = climber.propose()
                self.assertIsNotNone(proposal)
                ast, description = proposal
                text = source.render(ast)
                results.append((description, text))
                # A fixed score stream exercises improvements, plateaus and rejection.
                climber.observe(ast, permute.Evaluation(100 - iteration % 4))
            return results
        self.assertEqual(trace(93), trace(93))
        self.assertNotEqual(trace(93), trace(94))

    def test_no_reordering_dependencies_or_ambiguous_aliases(self):
        sources = (
            "int target(int x) { int a=x+1; int b=a+1; return b; }",
            "struct S {int a,b;}; void target(struct S*p,struct S*q){p->a=1;q->b=2;}",
            "union U {int a,b;}; void target(union U*u){u->a=1;u->b=2;}",
            "void target(volatile int *p) { *p=1; *p=2; }",
            "int target(int x) { int a=x; int *p=&a; a=2; *p=3; return a; }",
        )
        for text in sources:
            with self.subTest(source=text):
                source = program(text)
                self.assertIsNone(permute.mutate(source, source.ast, random.Random(0), "reorder"))

    def test_distinct_struct_fields_can_reorder(self):
        source = program("struct S {int a,b;}; void target(struct S*s){s->a=1;s->b=2;}")
        changed = permute.mutate(source, source.ast, random.Random(0), "reorder")
        self.assertIsNotNone(changed)
        body = source.render(changed[0])
        self.assertLess(body.index("s->b = 2"), body.index("s->a = 1"))

    def test_for_and_do_continue_are_not_rewritten(self):
        for loop in ("for(x=0;x<4;x++){if(x)continue;}", "do {if(x)continue;x++;} while(x<4);"):
            source = program("void target(int x){" + loop + "}")
            self.assertIsNone(permute.mutate(source, source.ast, random.Random(1), "loop"))

    def test_goto_functions_keep_structural_order(self):
        source = program("int target(int x){int a=1;int b=2;again:while(x){x--;goto again;}return a+b;}")
        for kind in ("reorder", "loop", "temporary", "inline_temporary", "declaration"):
            self.assertIsNone(permute.mutate(source, source.ast, random.Random(1), kind))

    def test_volatile_typedefs_and_const_initializers_are_preserved(self):
        source = program("typedef volatile int V; int target(int x){V value=x;return value;}")
        for kind in ("signedness", "qualifier", "declaration"):
            self.assertIsNone(permute.mutate(source, source.ast, random.Random(0), kind))
        changed = permute.mutate(source, source.ast, random.Random(0), "cast")
        self.assertIn("return value;", source.render(changed[0]))
        source = program("typedef const int C; int target(void){C value=3;return value;}")
        self.assertIsNone(permute.mutate(source, source.ast, random.Random(0), "declaration"))

    def test_actual_signedness_change_only_for_immutable_small_index(self):
        source = program("int target(int*p){int index=1;return p[index];}")
        changed = permute.mutate(source, source.ast, random.Random(0), "signedness")
        self.assertIn("unsigned int index", source.render(changed[0]))
        source = program("int target(int x){int value=x;return value<0;}")
        changed = permute.mutate(source, source.ast, random.Random(0), "signedness")
        self.assertIn("signed int value", source.render(changed[0]))
        self.assertNotIn("unsigned", source.render(changed[0]))

    def test_inverted_branches_have_no_dangling_else(self):
        source = program("int target(int x,int y){if(x) return 1;else if(y)return 2;return 3;}")
        changed = permute.mutate(source, source.ast, random.Random(1), "condition")
        parsed = program(source.render(changed[0]))
        original_if = source.functions(changed[0])["target"].body.block_items[0]
        reparsed_if = parsed.functions()["target"].body.block_items[0]
        self.assertEqual(permute.GENERATOR.visit(original_if), permute.GENERATOR.visit(reparsed_if))

    def test_shift_type_depends_on_left_operand(self):
        source = program("int target(int x,unsigned int shift){return x>>shift;}")
        function = source.functions()["target"]
        types = permute.Types(source.ast, function)
        self.assertEqual(types.words(types.expr(function.body.block_items[0].expr)), "int")

    def test_functions_and_original_diff_keep_text_not_other_bodies(self):
        source = program()
        changed = permute.mutate(source, source.ast, random.Random(0), "reorder")
        functions, patch = source.hints(changed[0])
        self.assertIn("int target(", functions)
        self.assertNotIn("int untouched(", functions)
        self.assertIn("--- original/target.c", patch)
        self.assertIn("+++ permuted/target.c", patch)
        self.assertIn("-    int a = x + 3;", patch)

    def test_forbidden_extensions_and_missing_targets_are_refused(self):
        for text in ("__svc(1) void target(void);", "register int x __asm(\"r0\"); int target(void){return x;}", "__asm void target(void){}"):
            with self.assertRaises(permute.PermuteError):
                program(text)
        with self.assertRaises(permute.PermuteError):
            permute.Program.parse("int other(void){return 1;}", ("target",), Path("fixture.c"))

    def test_armcc_extensions_outside_body_are_not_deleted(self):
        text = "__weak int target(int x){return x+1;}\n__packed struct S {int x;};\n"
        source = program(text)
        changed = permute.mutate(source, source.ast, random.Random(0), "commute")
        rendered = source.render(changed[0])
        self.assertTrue(rendered.startswith("__weak int target"))
        self.assertTrue(rendered.endswith("__packed struct S {int x;};\n"))

    def test_postfix_armcc_attribute_is_kept_in_original_function_diff(self):
        source = program("int target(int x) __attribute__((noinline)) {return x+1;}")
        changed = permute.mutate(source, source.ast, random.Random(0), "commute")
        self.assertIn("__attribute__((noinline))", source.hints(changed[0])[0])

    def test_shadowed_typedefs_are_unknown_not_nonvolatile(self):
        source = program("typedef int T; int target(int x){typedef volatile int T;T value=x;return value;}")
        function = source.functions()["target"]
        types = permute.Types(source.ast, function)
        self.assertTrue(types.volatile(types.declarations["value"].type))
        self.assertIsNone(permute.mutate(source, source.ast, random.Random(0), "qualifier"))

    def test_aggregate_locals_do_not_get_const(self):
        source = program("struct S{int x;};int target(void){struct S s={1};s.x=2;return s.x;}")
        self.assertIsNone(permute.mutate(source, source.ast, random.Random(0), "qualifier"))

    def test_temporary_names_cannot_shadow_typedefs(self):
        source = program("typedef int permute_tmp_0;int target(int x){int y=x+1;permute_tmp_0 z=y;return z+1;}")
        changed = permute.mutate(source, source.ast, random.Random(0), "temporary")
        text = source.render(changed[0])
        self.assertIn("permute_tmp_1", text)
        c_parser.CParser().parse(text)

    def test_signedness_does_not_modify_shared_sibling_types(self):
        source = program("int target(int*p){int index=1,value=-2;return p[index]+value;}")
        for seed in range(8):
            changed = permute.mutate(source, source.ast, random.Random(seed), "signedness")
            declarations = {n.name: n for n in permute.walk(source.functions(changed[0])["target"].body) if isinstance(n, c_ast.Decl)}
            if "unsigned" in declarations["index"].type.type.names:
                self.assertNotIn("unsigned", declarations["value"].type.type.names)

    def test_preprocessing_reuses_interface_auditor(self):
        source = Path("candidate.c")
        with mock.patch.object(interfaces, "preprocess", return_value="int target(void){return 1;}") as preprocess:
            with mock.patch.object(Path, "read_text", return_value="int target(void){return 1;}"):
                result = permute.Program.preprocess(source, ("target",), PROFILE)
        preprocess.assert_called_once_with(interfaces.Unit(source, PROFILE))
        self.assertEqual(result.targets, ("target",))


class DistanceTest(unittest.TestCase):
    def rows(self, original, candidate, *, size=None, status=None):
        size = len(original) if size is None else size
        return [{"name": "target", "address": 0x100000, "size": size,
                 "_original": original, "_candidate": candidate,
                 "status": status or ("match" if original == candidate and len(candidate) == size else "mismatch"),
                 "first_diff": None if original == candidate else 0,
                 "linker_rewrites": [{"rule": "inline_nop", "word": "do-not-export"}]}]

    def score(self, original, candidate, **kwargs):
        return permute.score_rows(self.rows(original, candidate, **kwargs), ("target",), {0x100000: {"isa": "arm"}})

    def test_exact_zero_and_perturbation_positive(self):
        self.assertEqual(self.score(b"abcdefgh", b"abcdefgh").score, 0)
        self.assertGreater(self.score(b"abcdefgh", b"abcdEfgh").score, 0)

    def test_alignment_and_extent_penalties(self):
        inserted = self.score(b"aaaabbbbcccc", b"aaaaxxxxbbbbcccc")
        self.assertEqual(inserted.functions[0]["instruction_changes"], 1)
        self.assertEqual(inserted.functions[0]["size_delta"], 4)
        extent = self.score(b"aaaabbbb", b"aaaa", status="match_extent_differs")
        self.assertGreater(extent.score, 0)
        self.assertGreater(self.score(b"aaaa", b"aaaa", status="mismatch").score, 0)

    def test_original_data_and_rewrite_words_are_never_exported(self):
        result = self.score(b"abcdefgh", b"abcdEfgh")
        text = json.dumps(result.functions)
        self.assertNotIn("do-not-export", text)
        self.assertNotIn("abcdefgh", text)
        self.assertNotIn("_original", text)
        self.assertEqual(result.functions[0]["linker_rewrites"], {"inline_nop": 1})

    def test_missing_target_is_not_a_zero_score(self):
        self.assertIsNone(permute.score_rows([], ("target",), {}).score)


@unittest.skipUnless(HAVE_COMPILER, "pinned compiler/launcher not installed (tools/setup.py)")
class CompilerMutationTest(ScratchTest):
    def test_every_eligible_mutation_compiles_on_synthetic_functions(self):
        exercised = set()
        for index, text in enumerate(SOURCES):
            source = program(text)
            for kind in permute.MUTATIONS:
                changed = permute.mutate(source, source.ast, random.Random(3), kind)
                if changed is None:
                    continue
                with self.subTest(fixture=index, mutation=kind):
                    rendered = source.render(changed[0])
                    c_parser.CParser().parse(rendered)
                    path = self.directory / f"fixture-{index}-{kind}.c"
                    path.write_text(rendered, encoding="utf-8")
                    toolchain.compile_source(PROFILE, path, path.with_suffix(".o"), cache=False)
                    exercised.add(kind)
        self.assertEqual(exercised, set(permute.MUTATIONS))

    def test_casts_never_replace_lvalues_or_struct_member_names(self):
        source = program("struct S{int x;};int target(int x,struct S*s){int y=x;y=x+1;++y;s->x=y;return s->x;}")
        for seed in range(8):
            changed = permute.mutate(source, source.ast, random.Random(seed), "cast")
            path = self.directory / f"cast-{seed}.c"
            path.write_text(source.render(changed[0]), encoding="utf-8")
            toolchain.compile_source(PROFILE, path, path.with_suffix(".o"), cache=False)

    def test_chained_mutations_compile(self):
        source = program()
        ast, rng = source.ast, random.Random(928)
        for step in range(20):
            changed = permute.mutate(source, ast, rng)
            if changed is None:
                break
            ast, description = changed
            with self.subTest(step=step, mutation=description):
                rendered = source.render(ast)
                c_parser.CParser().parse(rendered)
                path = self.directory / "chain.c"
                path.write_text(rendered, encoding="utf-8")
                toolchain.compile_source(PROFILE, path, path.with_suffix(".o"), cache=False)


@unittest.skipUnless(sys.platform in {"linux", "darwin"} and shutil.which("cc"), "optional native C compiler required")
class NativeSemanticsTest(ScratchTest):
    def test_mutations_preserve_returns_and_memory_on_synthetic_inputs(self):
        class Pair(ctypes.Structure):
            _fields_ = [("a", ctypes.c_int), ("b", ctypes.c_int)]
        for index, text in enumerate(SOURCES):
            source = program(text)
            for kind in permute.MUTATIONS:
                changed = permute.mutate(source, source.ast, random.Random(4), kind)
                if changed is None:
                    continue
                with self.subTest(fixture=index, mutation=kind):
                    original = text.replace("target", "reference").replace("untouched", "reference_untouched").replace("Pair", "ReferencePair")
                    candidate = source.render(changed[0]).replace("target", "candidate").replace("untouched", "candidate_untouched").replace("Pair", "CandidatePair")
                    path = self.directory / f"native-{index}-{kind}.c"
                    library = path.with_suffix(".so")
                    path.write_text(original + "\n" + candidate, encoding="utf-8")
                    compiled = subprocess.run(["cc", "-std=c99", "-O0", "-shared", "-fPIC", str(path), "-o", str(library)], capture_output=True, text=True)
                    self.assertEqual(compiled.returncode, 0, compiled.stderr)
                    module = ctypes.CDLL(str(library))
                    argument_types = [ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_int)]
                    if index != 2:
                        argument_types.append(ctypes.POINTER(Pair))
                    for name in ("reference", "candidate"):
                        function = getattr(module, name)
                        function.argtypes, function.restype = argument_types, ctypes.c_int
                    for x in (-3, 0, 4):
                        for y in (-2, 1, 5):
                            observations = []
                            for name in ("reference", "candidate"):
                                values, pair = (ctypes.c_int * 4)(2, 5, 8, 11), Pair(17, 23)
                                arguments = [x, y, values] + ([ctypes.byref(pair)] if index != 2 else [])
                                value = getattr(module, name)(*arguments)
                                observations.append((value, tuple(values), pair.a, pair.b))
                            self.assertEqual(observations[0], observations[1], (x, y))


@unittest.skipUnless(HAVE_COMPILER, "pinned compiler/launcher not installed (tools/setup.py)")
class SyntheticOracleTest(ScratchTest):
    def setUp(self):
        super().setUp()
        path = self.directory / "reference.c"
        path.write_text("int func_00100000(int x){return x+3;}\n", encoding="utf-8")
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
        self.oracle = permute.Oracle.__new__(permute.Oracle)
        self.oracle.profile, self.oracle.targets, self.oracle.context = PROFILE, ("func_00100000",), context

    def test_compiled_exact_and_perturbed_scores_use_context_compare(self):
        exact = self.oracle.evaluate("int func_00100000(int x){return x+3;}", self.directory / "worker")
        changed = self.oracle.evaluate("int func_00100000(int x){return x+4;}", self.directory / "worker")
        self.assertEqual(exact.score, 0)
        self.assertGreater(changed.score, 0)
        self.assertEqual(exact.functions[0]["status"], "match")
        self.assertEqual(changed.functions[0]["status"], "mismatch")

    def test_unresolved_candidate_is_invalid_not_match(self):
        result = self.oracle.evaluate("extern int unknown(int);int func_00100000(int x){return unknown(x);}", self.directory / "worker")
        self.assertIsNone(result.score)
        self.assertEqual(result.error, "LinkError")

    def test_oracle_uses_uncached_compilation_and_cleans_private_cache(self):
        saved_cache = toolchain.CACHE
        with mock.patch.object(toolchain, "compile_source", wraps=toolchain.compile_source) as compile_source:
            self.oracle.evaluate("int func_00100000(int x){return x+3;}", self.directory / "worker")
        self.assertFalse(compile_source.call_args.kwargs["cache"])
        self.assertEqual(toolchain.CACHE, saved_cache)
        self.assertEqual(list((self.directory / "worker").iterdir()), [])


@unittest.skipUnless(HAVE_IMAGE, "private orig/ image and pinned compiler required")
class OriginalImageTest(ScratchTest):
    def source(self, perturb=False):
        text = (ROOT / "src/pilot/pilot_b.c").read_text(encoding="utf-8")
        if perturb:
            text = text.replace("e->z = 0;", "e->z = 1;")
        path = self.directory / "candidate.c"
        path.write_text(text, encoding="utf-8")
        return permute.Program.preprocess(path, ("func_001091d4",), PROFILE)

    def test_real_preprocessing_and_exact_score(self):
        source = self.source()
        result = permute.Oracle(PROFILE, image.CATALOGUE, source.targets).evaluate(source.text, self.directory / "worker")
        self.assertEqual(result.score, 0)
        self.assertEqual(result.functions[0]["status"], "match")

    def test_exact_search_keeps_preprocessed_input_and_never_edits_author(self):
        source = self.source()
        path = self.directory / "candidate.c"
        before = path.read_bytes()
        result = permute.search(source, PROFILE, self.directory / "exact", seconds=60, workers=2, seed=5, max_candidates=1)
        self.assertEqual(result["score"], 0)
        self.assertEqual(result["candidates"], 1)
        self.assertEqual(path.read_bytes(), before)
        self.assertEqual((self.directory / "exact/best.c").read_text(), source.text)
        functions = (self.directory / "exact/best.functions.c").read_text()
        self.assertIn("void func_001091d4", functions)
        self.assertNotIn("void func_00103798", functions)

    def test_multiprocess_search_reproducible_with_fixed_seed_and_candidate_limit(self):
        source = self.source(perturb=True)
        summaries = []
        for name in ("one", "two"):
            summaries.append(permute.search(source, PROFILE, self.directory / name, seconds=90, workers=2, seed=382, max_candidates=10))
        for key in ("score", "initial_score", "candidates", "invalid_candidates", "improvements", "stop", "functions"):
            self.assertEqual(summaries[0][key], summaries[1][key], key)
        self.assertGreater(summaries[0]["initial_score"], 0)
        self.assertEqual(summaries[0]["candidates"], 10)
        for suffix in ("c", "functions.c", "diff", "json"):
            self.assertEqual((self.directory / "one" / f"best.{suffix}").read_bytes(), (self.directory / "two" / f"best.{suffix}").read_bytes())
        first = sorted(p.name for p in (self.directory / "one").glob("improvement-*"))
        second = sorted(p.name for p in (self.directory / "two").glob("improvement-*"))
        self.assertEqual(first, second)
        for name in first:
            self.assertEqual((self.directory / "one" / name).read_bytes(), (self.directory / "two" / name).read_bytes())


if __name__ == "__main__":
    unittest.main()
