"""Conservative source mutations, strict oracle scoring and reproducible searches.

Compiler tests need the pinned toolchain; image tests additionally need orig/.
All disposable files and compiler caches stay underneath scratch/.
"""
import ctypes
import io
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

    def test_approved_svc_results_and_cp15_helpers_are_preserved(self):
        headers = (
            "typedef struct Result {int result; unsigned handle;} Result;\n"
            "__svc(0x17) __value_in_regs Result svcCreateEvent(unsigned reserved, int reset);\n"
            "__svc_indirect(0) void svcIndirect(unsigned number);\n"
            "static __forceinline unsigned char *get_tls(void) {\n"
            '    register unsigned tls __asm("cp15:0:c13:c0:3");\n'
            "    return (unsigned char *)tls;\n"
            "}\n"
        )
        source = program(headers + "int target(int x){return x+1;}\n")
        changed = permute.mutate(source, source.ast, random.Random(0), "commute")
        self.assertIsNotNone(changed)
        rendered = source.render(changed[0])
        self.assertTrue(rendered.startswith(headers))
        functions, _ = source.hints(changed[0])
        self.assertTrue(functions.startswith("int target("))
        self.assertNotIn("__asm", functions)
        self.assertNotIn("__svc", functions)

    def test_cp15_annotations_inside_targets_remain_forbidden(self):
        with self.assertRaisesRegex(permute.PermuteError, "target body has ABI-relevant ARMCC annotations"):
            program('unsigned target(void){register unsigned tls __asm("cp15:0:c13:c0:3");return tls;}')

    def test_non_cp15_assembly_is_refused_even_in_untouched_helpers(self):
        for helper in (
            "__asm void helper(void){}",
            'unsigned helper(void){register unsigned value __asm("r0");return value;}',
            'void helper(void){asm("nop");}',
        ):
            with self.subTest(helper=helper):
                with self.assertRaises(permute.PermuteError):
                    program(helper + "\nint target(int x){return x+1;}")

    def test_postfix_armcc_attribute_is_kept_in_original_function_diff(self):
        source = program("int target(int x) __attribute__((noinline)) {return x+1;}")
        changed = permute.mutate(source, source.ast, random.Random(0), "commute")
        self.assertIn("__attribute__((noinline))", source.hints(changed[0])[0])

    def test_raw_spans_preserve_overlapping_names_and_multiple_target_bodies(self):
        definitions = (
            "__int64 a4(__int64 x) { return x - 4; }",
            "int func_001a78a4_suffix(int x) { return x + 10; }",
            "__weak __int64 func_001a78a4(__int64 x) __attribute__((noinline)) {\n"
            "    __int64 result = x + 1; /* } func_001a78a4_suffix( */\n"
            "    return result;\n}",
            "__int64 prefix_func_001a78a4(__int64 x) { return x + 2; }",
            "__int64 func_001a78a4_long(__int64 x) { return x + 3; }",
        )
        text = '#line 1 "header.h"\ntypedef __int64 Wide;\n'
        text += definitions[0] + '\n# 40 "fixture.c"\n' + "\n".join(definitions[1:]) + "\n"
        names = ("a4", "func_001a78a4_suffix", "func_001a78a4",
                 "prefix_func_001a78a4", "func_001a78a4_long")
        targets = ("func_001a78a4_long", "func_001a78a4")
        source = permute.Program.parse(text, targets, Path("fixture.c"))
        self.assertEqual(set(source.spans), set(names))
        for name, definition in zip(names, definitions):
            with self.subTest(name=name):
                start, body, end = source.spans[name]
                self.assertEqual(text[start:end], definition)
                self.assertEqual(text[body], "{")
                if name in targets:
                    self.assertEqual(source.original_functions[name], definition + "\n")
        rendered = source.render(source.ast)
        reparsed = permute.Program.parse(rendered, targets, Path("normalized.c"))
        self.assertEqual(permute.GENERATOR.visit(reparsed.ast), permute.GENERATOR.visit(source.ast))
        for index in (0, 1, 3):
            self.assertIn(definitions[index], rendered)
        self.assertTrue(rendered.startswith('#line 1 "header.h"\ntypedef __int64 Wide;\n'))
        original_hints, original_patch = source.hints(None)
        self.assertEqual(original_hints, definitions[4] + "\n\n" + definitions[2] + "\n")
        self.assertEqual(original_patch, "")
        functions, patch = source.hints(source.ast)
        self.assertEqual(set(permute.function_spans(functions)), set(targets))
        self.assertIn("__weak __int64 func_001a78a4(", functions)
        self.assertIn("__attribute__((noinline))", functions)
        self.assertNotIn("prefix_func_001a78a4", functions)
        self.assertIn("--- original/func_001a78a4.c", patch)

    def test_multiline_directives_keep_original_function_spans(self):
        before = (
            "#define BEFORE(value) /* continued comment \\\n"
            "    still in the comment */ \\\n"
            "    do { \\\n"
            "        if (value) { ++(value); } \\\n"
            "    } while (0)\n"
        )
        target = (
            "int target(int x) {\n"
            "#define INSIDE(value) do { \\\n"
            "    (value) += 2; \\\n"
            "} while (0)\n"
            "    BEFORE(x);\n"
            "    INSIDE(x);\n"
            "#undef INSIDE\n"
            "    return x;\n"
            "}"
        )
        after = (
            "#define AFTER(value) \\\n"
            "    int macro_generated(int value) { \\\n"
            "        return value; \\\n"
            "    }\n"
        )
        definitions = ("int earlier(void) { return 1; }", target,
                       "int later(void) { return 2; }")
        original = before + "\n".join(definitions[:2]) + "\n" + after + definitions[2] + "\n"
        preprocessed = (
            definitions[0] + "\nint target(int x) { if (x) ++x; x += 2; return x; }\n"
            + definitions[2] + "\n"
        )
        for newline in ("\n", "\r\n"):
            with self.subTest(newline=repr(newline)):
                text = original.replace("\n", newline)
                spans = permute.function_spans(text)
                self.assertEqual(set(spans), {"earlier", "target", "later"})
                for name, definition in zip(("earlier", "target", "later"), definitions):
                    start, body, end = spans[name]
                    self.assertEqual(text[start:end], definition.replace("\n", newline))
                    self.assertEqual(text[body], "{")
                source = permute.Program.parse(preprocessed, ("target",), Path("fixture.c"), text)
                self.assertEqual(source.original_functions["target"], target.replace("\n", newline) + "\n")
                functions, patch = source.hints(source.ast)
                self.assertEqual(set(permute.function_spans(functions)), {"target"})
                self.assertIn("-#define INSIDE(value)", patch)
                self.assertNotIn("#define BEFORE", patch)
                self.assertNotIn("#define AFTER", patch)

    def test_every_trailing_backslash_splices_directive_lines(self):
        target = "int target(void) { return 1; }"
        for newline in ("\n", "\r\n"):
            for backslashes in (1, 2, 3):
                with self.subTest(newline=repr(newline), backslashes=backslashes):
                    directive = "#define UNUSED " + "\\" * backslashes + newline
                    text = directive + "int hidden(void) { return 0; }" + newline + target
                    spans = permute.function_spans(text)
                    self.assertEqual(set(spans), {"target"})
                    start, _, end = spans["target"]
                    self.assertEqual(text[start:end], target)

    def test_continuations_in_comments_cannot_hide_or_invent_functions(self):
        prefix = (
            "/* A directive-looking line is still in this comment.\n"
            "#define HIDDEN \\\n"
            "*/\n"
            "// This entire continued line is a comment. \\\n"
            "int hidden(void) { return 0; } \\\n"
            "#define ALSO_HIDDEN \\\n"
            "int also_hidden(void) { return 1; }\n"
            "/\\\n* Spliced block-comment delimiters. *\\\n/\n"
        )
        target = (
            "int target(int x) {\n"
            "    // A continued comment must not close the target body. \\\n"
            "} \\\n"
            "int hidden_inside(void) { return 2; }\n"
            "    return x + 1;\n"
            "}"
        )
        suffix = (
            "\n// A continued comment must not replace the real target. \\\n"
            "int target(int x) { return x - 1; }\n"
        )
        for newline in ("\n", "\r\n"):
            with self.subTest(newline=repr(newline)):
                text = (prefix + target + suffix).replace("\n", newline)
                spans = permute.function_spans(text)
                self.assertEqual(set(spans), {"target"})
                start, body, end = spans["target"]
                self.assertEqual(text[start:end], target.replace("\n", newline))
                self.assertEqual(text[body], "{")
                source = permute.Program.parse(
                    "int target(int x) { return x + 1; }", ("target",), Path("fixture.c"), text,
                )
                self.assertEqual(source.original_functions["target"], target.replace("\n", newline) + "\n")

    def test_hints_keep_prefix_annotations_after_type_expansion(self):
        definition = "__weak __attribute__((noinline)) int target(int x) {return x+1;}"
        source = program("typedef __int64 Wide;\n" + definition + "\n")
        self.assertEqual(source.original_functions["target"], definition + "\n")
        self.assertEqual(source.hints(None), (definition + "\n", ""))
        self.assertTrue(source.hints(source.ast)[0].startswith(
            "__weak __attribute__((noinline)) int target("))

    def test_body_annotations_use_parser_offsets_not_raw_offsets(self):
        prefix = "".join(f"typedef __int64 Wide{i};\n" for i in range(40))
        source = program(prefix + "__packed struct S {int x;}; int target(struct S *p){return p->x;}")
        self.assertIn("target", source.functions())
        with self.assertRaisesRegex(permute.PermuteError, "target body has ABI-relevant ARMCC annotations"):
            program(prefix + "int target(void){__packed struct S {int x;} s; return s.x;}")

    def test_non_source_extensions_are_refused_before_preprocessing(self):
        for suffix in (".h", ".hpp", ".s", ".txt"):
            with self.subTest(suffix=suffix):
                with mock.patch.object(interfaces, "preprocess") as preprocess:
                    with self.assertRaisesRegex(permute.PermuteError, "accepts .c, .cpp, .cc or .cxx"):
                        permute.Program.preprocess(Path("fixture" + suffix), ("target",), PROFILE)
                preprocess.assert_not_called()

    def test_cpp_syntax_in_c_units_is_refused_cleanly(self):
        for text in (
            'extern "C" int target(int x){return x;}',
            "namespace fixture {int target(int x){return x;}}",
            "class Fixture {public: int target(int x){return x;}};",
            "template<class T> T target(T x){return x;}",
        ):
            with self.subTest(source=text):
                with self.assertRaisesRegex(permute.PermuteError, "C\\+\\+ is not supported"):
                    program(text)

    def test_invalid_extension_cli_returns_input_error_without_search_or_traceback(self):
        with mock.patch.object(interfaces, "preprocess") as preprocess:
            with mock.patch.object(permute, "search") as search:
                with mock.patch.object(sys, "stderr", new_callable=io.StringIO) as stderr:
                    status = permute.main(["fixture.s", "target", "--out", "scratch/invalid-extension"])
        self.assertEqual(status, 2)
        self.assertEqual(stderr.getvalue(), "permuter: the permuter accepts .c, .cpp, .cc or .cxx source files\n")
        preprocess.assert_not_called()
        search.assert_not_called()

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

    def test_positional_byte_count_is_distinct_from_aligned_score_component(self):
        result = self.score(b"aaaabbbbcccc", b"bbbbccccaaaa")
        summary = result.functions[0]
        self.assertEqual(summary["changed_bytes"], 12)
        self.assertEqual(summary["aligned_changed_bytes"], 8)
        self.assertEqual(summary["instruction_changes"], 2)
        self.assertEqual(result.score, 40)

    def test_positional_byte_count_includes_every_unpaired_byte(self):
        for original, candidate, delta in (
            (b"aaaabbbbcccc", b"aaaaxxxxbbbbcccc", 4),
            (b"aaaaxxxxbbbbcccc", b"aaaabbbbcccc", -4),
        ):
            with self.subTest(size_delta=delta):
                result = self.score(original, candidate)
                summary = result.functions[0]
                self.assertEqual(summary["changed_bytes"], 12)
                self.assertEqual(summary["aligned_changed_bytes"], 4)
                self.assertEqual(summary["instruction_changes"], 1)
                self.assertEqual(summary["size_delta"], delta)
                self.assertEqual(result.score, 52)

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

    def test_preprocessed_int64_units_normalize_without_corrupting_declarations(self):
        path = self.directory / "overlapping.c"
        path.write_text(
            "#include <stdint.h>\n"
            "__int64 a4(__int64 x){return x-1;}\n"
            "__int64 func_001a78a4(__int64 x){__int64 value=x+1;return value;}\n"
            "int prefix_func_001a78a4(int x){return x+2;}\n"
            "int func_001a78a4_suffix(int x){return x*3;}\n",
            encoding="utf-8",
        )
        toolchain.compile_source(PROFILE, path, path.with_suffix(".o"), cache=False)
        targets = ("func_001a78a4_suffix", "func_001a78a4")
        source = permute.Program.preprocess(path, targets, PROFILE)
        self.assertNotEqual(len(source.text), len(interfaces.prepare(source.text, path).text))
        normalized = self.directory / "normalized.c"
        normalized.write_text(source.render(source.ast), encoding="utf-8")
        toolchain.compile_source(PROFILE, normalized, normalized.with_suffix(".o"), cache=False)
        functions, _ = source.hints(source.ast)
        self.assertEqual(set(permute.function_spans(functions)), set(targets))

    def test_multiline_macros_preprocess_normalize_and_compile(self):
        prefix = (
            "#define BEFORE(value) do { \\\n"
            "    (value) += 1; \\\n"
            "} while (0)\n"
            "int earlier(void) { return 1; }\n"
        )
        target = (
            "int target(int x) {\n"
            "#define INSIDE(value) do { \\\n"
            "    (value) += 2; \\\n"
            "} while (0)\n"
            "    BEFORE(x);\n"
            "    INSIDE(x);\n"
            "#undef INSIDE\n"
            "    return x;\n"
            "}"
        )
        suffix = (
            "\n#define AFTER(value) do { \\\n"
            "    (value) += 3; \\\n"
            "} while (0)\n"
            "int later(int x) { AFTER(x); return x; }\n"
        )
        for label, newline in (("lf", "\n"), ("crlf", "\r\n")):
            with self.subTest(newline=label):
                path = self.directory / f"macros-{label}.c"
                path.write_text((prefix + target + suffix).replace("\n", newline), encoding="utf-8", newline="")
                toolchain.compile_source(PROFILE, path, path.with_suffix(".o"), cache=False)
                source = permute.Program.preprocess(path, ("target",), PROFILE)
                self.assertEqual(source.original_functions["target"], target + "\n")
                normalized = self.directory / f"normalized-macros-{label}.c"
                normalized.write_text(source.render(source.ast), encoding="utf-8")
                toolchain.compile_source(PROFILE, normalized, normalized.with_suffix(".o"), cache=False)
                functions, patch = source.hints(source.ast)
                self.assertEqual(set(permute.function_spans(functions)), {"target"})
                self.assertIn("-#define INSIDE(value)", patch)

    def test_approved_extension_headers_normalize_and_compile(self):
        path = self.directory / "extensions.c"
        path.write_text(
            '#include "ctr/svc.h"\n#include "ctr/cp15.h"\n'
            "CtrResult target(CtrHandle session) {\n"
            "    unsigned char *tls = ctrGetThreadLocalStorage();\n"
            "    tls[0] = 0;\n"
            "    return svcSendSyncRequest(session);\n"
            "}\n",
            encoding="utf-8",
        )
        toolchain.compile_source(PROFILE, path, path.with_suffix(".o"), cache=False)
        source = permute.Program.preprocess(path, ("target",), PROFILE)
        normalized = self.directory / "normalized-extensions.c"
        rendered = source.render(source.ast)
        start, body, end = source.spans["target"]
        self.assertEqual(rendered[:body], source.text[:body])
        self.assertTrue(rendered.endswith(source.text[end:]))
        self.assertIn("__svc(0x17) __value_in_regs", rendered)
        self.assertIn('__asm("cp15:0:c13:c0:3")', rendered)
        normalized.write_text(rendered, encoding="utf-8")
        toolchain.compile_source(PROFILE, normalized, normalized.with_suffix(".o"), cache=False)


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
        context.literal_model = inplace.DEFAULT_LITERAL_MODEL
        self.oracle = permute.Oracle.__new__(permute.Oracle)
        self.oracle.profile, self.oracle.targets, self.oracle.context = PROFILE, ("func_00100000",), context
        self.oracle.suffix = ".c"

    def test_compiled_exact_and_perturbed_scores_use_context_compare(self):
        exact = self.oracle.evaluate("int func_00100000(int x){return x+3;}", self.directory / "worker")
        changed = self.oracle.evaluate("int func_00100000(int x){return x+4;}", self.directory / "worker")
        self.assertEqual(exact.score, 0)
        self.assertGreater(changed.score, 0)
        self.assertEqual(exact.functions[0]["status"], "match")
        self.assertEqual(changed.functions[0]["status"], "mismatch")

    def test_published_byte_counts_reproduce_from_compiled_candidate(self):
        source = permute.Program.parse(
            "int func_00100000(int x){return x*7+3;}\n",
            self.oracle.targets, self.directory / "input.c",
        )
        result = self.oracle.evaluate(source.text, self.directory / "worker")
        permute.save_candidate(self.directory, "best", source, None, result, {})
        record = json.loads((self.directory / "best.json").read_text(encoding="utf-8"))
        emitted = self.directory / "best.c"
        toolchain.compile_source(PROFILE, emitted, emitted.with_suffix(".o"), cache=False)
        row = self.oracle.context.compare(emitted.with_suffix(".o"))[0]
        original, candidate = row["_original"][:row["size"]], row["_candidate"]
        positional = sum(a != b for a, b in zip(original, candidate)) + abs(len(original) - len(candidate))
        summary = record["functions"][0]
        self.assertEqual(summary["changed_bytes"], positional)
        self.assertNotEqual(summary["aligned_changed_bytes"], positional)
        self.assertEqual(summary["status"], row["status"])
        self.assertEqual(summary["candidate_size"], row["candidate_size"])
        self.assertEqual(summary["first_diff"], row["first_diff"])
        self.assertEqual(
            record["score"],
            summary["instruction_changes"] * 16 + summary["aligned_changed_bytes"]
            + abs(summary["size_delta"]) * 8,
        )

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
