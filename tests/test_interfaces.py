"""Cross-unit interface regressions; only this module runs in the worker assignment."""
from __future__ import annotations

import contextlib
import hashlib
import io
import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import interfaces  # noqa: E402
import toolchain  # noqa: E402

PROFILE = toolchain.load_profiles()["game"]


class InterfaceTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="interfaces-")
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name).resolve()
        (self.root / "include").mkdir()
        self.units = []

    def source(self, name: str, text: str) -> Path:
        path = self.root / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8")
        return path

    def add(self, name: str, text: str) -> Path:
        path = self.source(name, text)
        self.units.append(interfaces.Unit(path, PROFILE))
        return path

    def audit(self, names=None):
        # Most tests isolate type analysis from compiler availability. The real
        # compiler tests below separately prove macro/include/origin integration.
        with patch.object(interfaces, "preprocess", side_effect=lambda unit, root: unit.source.read_text(encoding="utf-8")):
            return interfaces.audit(self.units, self.root, names or set())

    def assert_checked(self, report):
        self.assertEqual(report["unchecked"], [])
        self.assertTrue(all(unit["checked"] for unit in report["units"]))

    def test_function_parameter_signedness_disagrees(self):
        self.add("a.c", "int func_00100000(int value) { return value; }\n")
        self.add("b.c", "extern int func_00100000(unsigned value);\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertTrue(interfaces.failed(report))
        self.assertEqual([(d["name"], d["kind"]) for d in report["disagreements"]], [("func_00100000", "declaration")])
        self.assertEqual({site["source"] for site in report["disagreements"][0]["sites"]}, {"a.c", "b.c"})

    def test_const_object_disagrees_both_directions(self):
        for definition, declaration in (("const int", "int"), ("int", "const int")):
            with self.subTest(definition=definition):
                self.units.clear()
                self.add("a.c", f"{definition} data_00200000 = 3;\n")
                self.add("b.c", f"extern {declaration} data_00200000;\n")
                report = self.audit()
                self.assert_checked(report)
                self.assertEqual(len(report["disagreements"]), 1)
                self.assertTrue(interfaces.failed(report))

    def test_struct_member_type_disagrees(self):
        self.add("a.c", "struct Shared { int x; unsigned y; }; void func_00100000(struct Shared *s) {}\n")
        self.add("b.c", "struct Shared { int x; int y; }; extern void func_00100000(struct Shared *s);\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual([(d["name"], d["kind"]) for d in report["disagreements"]], [("struct Shared", "layout")])

    def test_struct_member_order_names_bounds_and_bit_widths(self):
        for other in ("int y; int x;", "int x; int z;", "int x; int y[2];", "int x; int y:3;"):
            with self.subTest(other=other):
                self.units.clear()
                self.add("a.c", "struct Shared { int x; int y; }; extern struct Shared data_00200000;\n")
                self.add("b.c", f"struct Shared {{ {other} }}; extern struct Shared data_00200000;\n")
                report = self.audit()
                self.assert_checked(report)
                self.assertEqual(report["disagreements"][0]["kind"], "layout")

    def test_distinct_struct_tags_are_not_equivalent(self):
        self.add("a.c", "struct One { int x; }; void func_00100000(struct One *p) {}\n")
        self.add("b.c", "struct Two { int x; }; void func_00100000(struct Two *p);\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"][0]["kind"], "declaration")

    def test_recursive_struct_typedefs_and_forward_declarations_agree(self):
        self.add("a.c", "typedef struct Node Node; struct Node { int value; Node *next; }; void func_00100000(Node *one) {}\n")
        self.add("b.c", "struct Node; extern void func_00100000(struct Node *two); struct Node { signed int value; struct Node *next; };\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"], [])

    def test_nested_struct_difference_is_reachable(self):
        self.add("a.c", "struct Child { int x; }; struct Parent { struct Child *p; }; extern struct Parent data_00200000;\n")
        self.add("b.c", "struct Child { unsigned x; }; struct Parent { struct Child *p; }; extern struct Parent data_00200000;\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual([(d["name"], d["kind"]) for d in report["disagreements"]], [("struct Child", "layout")])

    def test_parameter_names_typedefs_qualifier_order_ignored(self):
        self.add("a.c", "typedef unsigned int U; int func_00100000(const volatile U *first) { return 0; }\n")
        self.add("b.c", "extern signed int func_00100000(volatile const int unsigned *second);\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"], [])
        self.assertFalse(interfaces.failed(report))

    def test_pointer_depth_return_type_qualifiers_and_variadic_significant(self):
        for definition, declaration in (
            ("int func_00100000(int *p)", "int func_00100000(int **p)"),
            ("int func_00100000(int p)", "unsigned func_00100000(int p)"),
            ("int func_00100000(const int *p)", "int func_00100000(int *p)"),
            ("int func_00100000(int * const p)", "int func_00100000(int *p)"),
            ("int func_00100000(int p)", "int func_00100000(int p, ...)"),
            ("int func_00100000(void)", "int func_00100000()"),
        ):
            with self.subTest(declaration=declaration):
                self.units.clear()
                self.add("a.c", definition + " { return 0; }\n")
                self.add("b.c", declaration + ";\n")
                report = self.audit()
                self.assert_checked(report)
                self.assertEqual(len(report["disagreements"]), 1)

    def test_function_pointer_parameter_names_ignored(self):
        self.add("a.c", "void func_00100000(int (*callback)(unsigned one)) {}\n")
        self.add("b.c", "extern void func_00100000(int (*other)(unsigned two));\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"], [])

    def test_array_constant_expressions_and_enum_bounds(self):
        self.add("a.c", "enum { COUNT = 2 + 2 }; int data_00200000[COUNT];\n")
        self.add("b.c", "extern int data_00200000[0x4U];\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"], [])
        self.source("b.c", "extern int data_00200000[5];\n")
        self.assertTrue(interfaces.failed(self.audit()))

    def test_definition_initializer_completes_array_bound(self):
        self.add("a.c", "int data_00200000[] = { [3] = 1, 2 };\n")
        self.add("b.c", "extern int data_00200000[5];\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"], [])
        self.source("b.c", "extern int data_00200000[4];\n")
        self.assertTrue(interfaces.failed(self.audit()))

    def test_member_alignment_is_part_of_layout(self):
        self.add("a.c", "struct S { char x; _Alignas(8) int y; }; extern struct S data_00200000;\n")
        self.add("b.c", "struct S { char x; int y; }; extern struct S data_00200000;\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"][0]["kind"], "layout")

    def test_unspecified_and_explicit_array_bounds_are_strict(self):
        self.add("a.c", "int data_00200000[4];\n")
        self.add("b.c", "extern int data_00200000[];\n")
        self.assertTrue(interfaces.failed(self.audit()))

    def test_names_overlay_keys_are_bound(self):
        self.add("a.c", "const int real_symbol = 1; int unrelated = 2;\n")
        self.add("b.c", "extern int real_symbol; extern unsigned unrelated;\n")
        report = self.audit({"real_symbol"})
        self.assert_checked(report)
        self.assertEqual([d["name"] for d in report["disagreements"]], ["real_symbol"])

    def test_local_and_static_names_are_not_cross_unit_interfaces(self):
        self.add("a.c", "static int data_00200000; void local(void) { unsigned data_00200000 = 0; }\n")
        self.add("b.c", "static unsigned data_00200000;\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"], [])
        self.assertEqual(report["policy"], [])

    def test_inline_extern_is_policy_warning_not_failure(self):
        self.add("a.c", "int func_00100000(void) { return 1; }\n")
        self.add("b.c", "extern int func_00100000(void);\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertFalse(interfaces.failed(report))
        self.assertEqual(len(report["policy"]), 1)
        self.assertEqual(report["policy"][0]["level"], "policy")
        self.assertEqual(report["policy"][0]["sites"][0]["source"], "b.c")

    def test_block_scope_extern_is_also_policy(self):
        self.add("a.c", "int data_00200000;\n")
        self.add("b.c", "void local(void) { extern int data_00200000; }\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(len(report["policy"]), 1)

    def test_undefined_original_callee_has_no_policy_warning(self):
        self.add("a.c", "extern int func_00100000(void);\n")
        self.add("b.c", "extern int func_00100000(void);\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["policy"], [])
        self.assertFalse(interfaces.failed(report))

    def test_header_origin_passes_and_line_markers_preserved(self):
        self.add("a.c", '#line 7 "include/shared.h"\nint func_00100000(int first);\n#line 1 "a.c"\nint func_00100000(int value) { return value; }\n')
        self.add("b.c", '#line 7 "include/shared.h"\nint func_00100000(int second);\n')
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"], [])
        self.assertEqual(report["policy"], [])
        self.source("b.c", '#line 7 "include/shared.h"\nint func_00100000(unsigned second);\n')
        report = self.audit()
        sites = report["disagreements"][0]["sites"]
        self.assertTrue(any(site["source"] == "include/shared.h" and site["line"] == 7 for site in sites))

    def test_other_header_directory_does_not_satisfy_policy(self):
        self.add("a.c", "int func_00100000(void) { return 0; }\n")
        self.add("b.c", '#line 2 "src/local.h"\nint func_00100000(void);\n')
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["policy"][0]["sites"][0]["source"], "src/local.h")

    def test_const_writes_direct_cast_alias_array_and_member(self):
        self.add("a.c", """const int data_00200000 = 1;
const int data_00200004[2] = { 1, 2 };
struct Shared { int value; }; const struct Shared data_00200010 = { 3 };
void local(void) {
    int *pointer = (int *)&data_00200000;
    *pointer = 2;
    ((int *)data_00200004)[1]++;
    ((struct Shared *)&data_00200010)->value = 4;
    data_00200000 = 3;
}
""")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(len(report["disagreements"]), 4)
        self.assertEqual({d["name"] for d in report["disagreements"]}, {"data_00200000", "data_00200004", "data_00200010"})
        self.assertTrue(all(d["kind"] == "const-write" for d in report["disagreements"]))

    def test_pointer_to_const_typed_write_and_const_member(self):
        self.add("a.c", "struct Shared { const int value; }; void local(const int *p, struct Shared *s) { *p = 2; s->value++; }\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(len(report["disagreements"]), 2)

    def test_const_pointer_is_not_const_pointee(self):
        self.add("a.c", "int value; void local(void) { int * const p = &value; *p = 2; }\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"], [])

    def test_pointer_to_const_can_advance_and_const_pointee_member_can_assign(self):
        self.add("a.c", "struct Font { const void *vtable; }; void local(const int *p, const int text[], struct Font *font) { ++p; ++text; font->vtable = p; }\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"], [])

    def test_indexing_a_pointer_member_of_a_const_object_writes_the_pointee(self):
        # reader->cache is a const pointer member of *reader, but the array it points at is writable.
        self.add("a.c", "struct Reader { const void **cache; }; "
                        "void local(const struct Reader *reader, int i, const void *v) { reader->cache[i] = v; *reader->cache = v; }\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"], [])

    def test_indexing_an_array_member_of_a_const_object_is_a_const_write(self):
        self.add("a.c", "struct Table { int slots[4]; }; void local(const struct Table *t) { t->slots[1] = 2; }\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual([d["kind"] for d in report["disagreements"]], ["const-write"])

    def test_pointee_const_is_distinct_from_pointer_const(self):
        self.add("a.c", "extern const int *data_00200000;\n")
        self.add("b.c", "extern int * const data_00200000;\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(len(report["disagreements"]), 1)

    def test_alias_assignment_and_shadowing(self):
        self.add("a.c", "const int data_00200000 = 1; void local(void) { int *p; p = (int *)&data_00200000; { int value = 0; int *p = &value; *p = 2; } *p = 2; }\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(len(report["disagreements"]), 1)
        self.assertEqual(report["disagreements"][0]["name"], "data_00200000")

    def test_armcc_inline_weak_and_non_type_attributes(self):
        self.add("a.c", "__weak __forceinline int func_00100000(int value) __attribute__((unused, noinline)) { return value; }\n")
        self.add("b.c", "extern __inline int func_00100000(int other);\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"], [])

    def test_standard_header_nothrow_and_nonnull_attributes(self):
        self.add("a.c", '__declspec(__nothrow) __pure int func_00100000(const int *p) __attribute__((__nonnull__(1)));\n')
        self.add("b.c", '__nothrow int func_00100000(const int *renamed);\n')
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"], [])

    def test_softfp_is_retained_as_the_aapcs_function_abi(self):
        self.add("a.c", '__softfp float func_00100000(float value);\n')
        self.add("b.c", 'float func_00100000(float value) __attribute__((__pcs__("aapcs")));\n')
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"], [])
        self.source("b.c", 'float func_00100000(float value);\n')
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(len(report["disagreements"]), 1)

    def test_int64_normalization_preserves_types_annotations_and_source_lines(self):
        self.add("a.c", '__int64 func_00100000(unsigned __int64 value) __attribute__((__pcs__("aapcs")));\n')
        self.add("b.c", 'long long func_00100000(unsigned long long value) __attribute__((__pcs__("aapcs")));\n')
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"], [])
        self.source("b.c", '\nlong long func_00100000(unsigned long value) __attribute__((__pcs__("aapcs")));\n')
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(len(report["disagreements"]), 1)
        self.assertEqual([(s["source"], s["line"]) for s in report["disagreements"][0]["sites"]], [("a.c", 1), ("b.c", 2)])

    def test_standard_header_pragmas_preserve_explicit_layouts_and_lines(self):
        self.add("a.c", '#pragma push\n#pragma anon_unions\n'
                        'struct Shared { union { int value; float real; }; };\n'
                        '#pragma pop /* restore anon_unions */\n'
                        '#pragma __printf_args\n#pragma __scanf_args\n'
                        'extern struct Shared data_00200000;\n')
        self.add("b.c", 'struct Shared { union { int value; double real; }; }; extern struct Shared data_00200000;\n')
        report = self.audit()
        self.assert_checked(report)
        self.assertTrue(interfaces.failed(report))
        self.assertTrue(any(site["source"] == "a.c" and site["line"] == 3 for d in report["disagreements"] for site in d["sites"]))

    def test_pragma_push_does_not_hide_unsupported_layout_changes(self):
        self.add("a.c", '#pragma push\n#pragma pack(1)\nstruct Shared { char c; int i; };\n#pragma pop\n')
        report = self.audit()
        self.assertTrue(interfaces.failed(report))
        self.assertIn("#pragma pack(1)", report["unchecked"][0]["reason"])

    def test_packed_layout_difference(self):
        self.add("a.c", "__packed struct Shared { char c; int i; }; extern struct Shared data_00200000;\n")
        self.add("b.c", "struct Shared { char c; int i; }; extern struct Shared data_00200000;\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"][0]["kind"], "layout")

    def test_attribute_postfix_packed_layout(self):
        self.add("a.c", "struct Shared { char c; int i; } __attribute__((packed)); extern struct Shared data_00200000;\n")
        self.add("b.c", "struct Shared { char c; int i; }; extern struct Shared data_00200000;\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"][0]["kind"], "layout")

    def test_packed_keyword_and_attribute_describe_same_layout(self):
        self.add("a.c", "__packed struct Shared { char c; int i; }; extern struct Shared data_00200000;\n")
        self.add("b.c", "struct Shared { char c; int i; } __attribute__((packed)); extern struct Shared data_00200000;\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"], [])

    def test_value_in_regs_changes_function_contract(self):
        self.add("a.c", "struct R { int x; int y; }; __value_in_regs struct R func_00100000(void);\n")
        self.add("b.c", "struct R { int x; int y; }; struct R func_00100000(void);\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(report["disagreements"][0]["kind"], "declaration")

    def test_svc_intrinsics_are_retained_in_interface_type(self):
        self.add("a.c", "__svc(0x03) void func_00100000(void);\n")
        self.add("b.c", "__svc(0x09) void func_00100000(void);\n")
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual(len(report["disagreements"]), 1)

    def test_cp15_named_register_variables_at_file_and_block_scope(self):
        self.add("a.c", 'register unsigned int tls __asm("cp15:0:c13:c0:3");\n'
                        'unsigned int func_00100000(void) { return tls + 0x5c; }\n')
        self.add("b.c", 'typedef unsigned int Word;\n'
                        'unsigned int local(void) {\n'
                        '  register Word tls /* declaration */ __asm (/* register */ "cp15:0:c13:c0:3");\n'
                        '  return tls;\n}\n')
        report = self.audit()
        self.assert_checked(report)
        self.assertFalse(interfaces.failed(report))
        self.assertEqual(report["policy"], [])

    def test_cp15_named_register_does_not_hide_const_writes(self):
        self.add("a.c", 'void local(void) {\n'
                        '  register const unsigned int tls __asm("cp15:0:c13:c0:3");\n'
                        '  tls = 0;\n}\n')
        report = self.audit()
        self.assert_checked(report)
        self.assertEqual([(d["name"], d["kind"]) for d in report["disagreements"]], [("tls", "const-write")])

    def test_only_cp15_named_register_declarations_are_accepted(self):
        for text in (
            'void local(void) { __asm("cp15:0:c13:c0:3"); }\n',
            'void local(void) { __asm { nop } }\n',
            '__asm void local(void) { bx lr }\n',
            'void local(void) __asm("cp15:0:c13:c0:3");\n',
            'register unsigned int value __asm("r0");\n',
            'register unsigned int value __asm("s0");\n',
            'register unsigned int value __asm("cp14:0:c13:c0:3");\n',
            'register unsigned int value __asm("cp15:0:c13:c0:3" : "=r"(value));\n',
            'register unsigned int value __asm__("cp15:0:c13:c0:3");\n',
            'register unsigned int value asm("cp15:0:c13:c0:3");\n',
            'unsigned int value __asm("cp15:0:c13:c0:3");\n',
            'register unsigned int value __asm("cp15:0:c13:c0:3") = 1;\n',
            'register unsigned int *value __asm("cp15:0:c13:c0:3");\n',
            'register unsigned int value[1] __asm("cp15:0:c13:c0:3");\n',
            'void local(register unsigned int value __asm("cp15:0:c13:c0:3")) {}\n',
        ):
            with self.subTest(text=text):
                self.units.clear()
                self.add("a.c", text)
                report = self.audit()
                self.assertFalse(report["units"][0]["checked"])
                self.assertEqual(len(report["unchecked"]), 1)
                self.assertTrue(interfaces.failed(report, allow_unchecked_cpp=True))

    def test_svc_declarations_and_intrinsic_calls_are_not_bound_interfaces(self):
        for name in ("a.c", "b.c"):
            self.add(name, '__svc(0x28) unsigned long long svcGetSystemTick(void);\n'
                           '__svc(0x23) unsigned int svcCloseHandle(unsigned int handle);\n'
                           'unsigned int local(volatile unsigned int *p) {\n'
                           '  unsigned int value = __ldrex(p);\n'
                           '  __dmb(0xf); __dsb(0xf); __isb(0xf);\n'
                           '  __strex(value, p); __clrex(); __schedule_barrier();\n'
                           '  svcCloseHandle(value);\n'
                           '  return (unsigned int)svcGetSystemTick();\n}\n')
        report = self.audit()
        self.assert_checked(report)
        self.assertFalse(interfaces.failed(report))
        self.assertEqual(report["policy"], [])

    def test_integrator_marker_comments_fail_before_preprocessing(self):
        for suffix in ("c", "cpp"):
            for comment in ("// INTEGRATOR: supply type", "/* INTEGRATOR: finish integration */",
                            "/*\n * INTEGRATOR: finish integration\n */"):
                with self.subTest(suffix=suffix, comment=comment):
                    self.units.clear()
                    self.add(f"a.{suffix}", f"int local(void) {{ return 1; }}\n{comment}\n")
                    with patch.object(interfaces, "preprocess", side_effect=AssertionError("marker must be caught before -E")):
                        report = interfaces.audit(self.units, self.root, set())
                    self.assertEqual(len(report["unchecked"]), 1)
                    self.assertIn("leftover INTEGRATOR: marker comment at line 2", report["unchecked"][0]["reason"])
                    self.assertTrue(interfaces.failed(report, allow_unchecked_cpp=True))

    def test_integrator_marker_in_string_is_not_a_comment(self):
        self.add("a.c", 'const char *message = "INTEGRATOR: not a comment // or /*";\n'
                        '/* Ordinary integration explanation. */\n')
        report = self.audit()
        self.assert_checked(report)
        self.assertFalse(interfaces.failed(report))

    def test_unsupported_syntax_and_assembly_fail_closed(self):
        for text in (
            "__asm void func_00100000(void) { bx lr }\n",
            "int func_00100000(void) __attribute__((mystery_abi));\n",
            "#pragma pack(1)\nstruct S { int x; };\n",
            "this is not C;\n",
        ):
            with self.subTest(text=text):
                self.units.clear()
                self.add("a.c", text)
                report = self.audit()
                self.assertFalse(report["units"][0]["checked"])
                self.assertEqual(len(report["unchecked"]), 1)
                self.assertTrue(interfaces.failed(report, allow_unchecked_cpp=True))

    def test_cpp_is_always_reported_and_only_explicitly_waived(self):
        path = self.add("a.cpp", "class Shared {};\n")
        with patch.object(interfaces, "preprocess", side_effect=AssertionError("C++ must not be silently parsed as C")):
            report = interfaces.audit(self.units, self.root, set())
        self.assertEqual(report["units"], [{"source": "a.cpp", "sha256": hashlib.sha256(path.read_bytes()).hexdigest(), "checked": False}])
        self.assertEqual(report["unchecked"][0]["kind"], "cpp")
        self.assertTrue(interfaces.failed(report))
        self.assertFalse(interfaces.failed(report, allow_unchecked_cpp=True))

    def test_cpp_class_defined_differently_in_two_units_fails_even_when_waived(self):
        """c002-wave1 F01: two units defined `struct Result` differently (an ODR violation)."""
        self.add("a.cpp", "struct Result {\n    unsigned value;\n};\nstruct Same { int x; };\n")
        self.add("b.cpp", "// comment\nstruct Result {\n    unsigned value;\n    unsigned failed() const { return value >> 31; }\n};\n"
                          "struct Same {  int x; /* spacing and comments are not tokens */ };\n")
        report = interfaces.audit(self.units, self.root, set())
        self.assertEqual([(d["name"], d["kind"]) for d in report["disagreements"]], [("class Result", "odr")])
        self.assertEqual([(s["source"], s["line"]) for s in report["disagreements"][0]["sites"]], [("a.cpp", 1), ("b.cpp", 2)])
        self.assertTrue(interfaces.failed(report, allow_unchecked_cpp=True))

    def test_cpp_local_redeclaration_of_a_shared_symbol_fails_even_when_waived(self):
        """c003-wave2 F01: a C++ unit redeclared func_0017a968 with types the C side had renamed."""
        self.add("a.c", "void func_0017a968(int *table) { }\n")
        self.add("b.cpp", 'extern "C" void func_0017a968(long *table);\n'
                          'extern "C" void func_00100000(void);  // local to this unit only: fine\n'
                          'extern "C" int func_00100004(void) { return 0; }\n')
        report = self.audit()
        self.assertEqual([(d["name"], d["kind"]) for d in report["disagreements"]], [("func_0017a968", "cpp-redeclaration")])
        self.assertEqual(sorted(s["source"] for s in report["disagreements"][0]["sites"]), ["a.c", "b.cpp"])
        self.assertTrue(interfaces.failed(report, allow_unchecked_cpp=True))

    def test_flat_cpp_header_identity_and_forward_policy_are_unchanged(self):
        """c003-wave2-r2 F01: game's unqualified Widget stays one project-wide identity."""
        self.source("include/game/screen.h", "struct Widget { unsigned char bytes[0x94]; };\n")
        self.source("include/game/other.h", '#include "game/screen.h"\n')
        self.add("heap.cpp", "struct Widget;\nvoid *make(void);\n")
        self.add("ui.cpp", '#include "game/other.h"\nstruct Widget;\n')
        self.add("odr.cpp", "struct Widget { int other; };\n")
        report = self.audit()
        kinds = sorted((d["name"], d["kind"], tuple(sorted(s["source"] for s in d["sites"]))) for d in report["disagreements"])
        self.assertEqual(kinds, [("class Widget", "odr", ("include/game/screen.h", "odr.cpp")),
                                 ("class Widget", "type-name", ("heap.cpp", "include/game/screen.h"))])

    def test_cpp_namespaces_distinguish_frame_heaps(self):
        self.source("include/ut.h", "namespace nw { namespace ut {\nclass FrameHeap { int allocator; };\n}}\n")
        self.source("include/snd.h", "namespace nw::snd::internal {\nclass FrameHeap { void *buffer; };\n}\n")
        self.add("heap.cpp", '#include "ut.h"\n#include "snd.h"\n')
        report = self.audit()
        self.assertEqual(report["disagreements"], [])
        self.assertFalse(interfaces.failed(report, allow_unchecked_cpp=True))

    def test_cpp_nested_iterators_and_namespace_iterators_are_distinct(self):
        self.source("include/iterators.h", """
namespace std {
template<class T> struct iterator { typedef T value_type; };
}
namespace nw { namespace ut {
namespace internal {
template<class T> struct iterator { T *value; };
class LinkListImpl {
public:
    class iterator { int *node; };
    class const_iterator { const int *node; };
};
}
template<class T> class LinkList {
public:
    class iterator { T *node; };
    class const_iterator { const T *node; };
    typedef iterator Iterator;
    using ConstIterator = const_iterator;
};
}}
""")
        self.add("iterators.cpp", '#include "iterators.h"\n')
        self.assertEqual(self.audit()["disagreements"], [])

    def test_cpp_iterator_traits_primary_and_specializations_are_distinct(self):
        text = """namespace nw { namespace ut { namespace internal {
template<class It> struct iterator_traits { typedef typename It::value_type value_type; };
template<class T> struct iterator_traits<T*> { typedef T value_type; };
template<class T> struct iterator_traits<const T *> { typedef T value_type; typedef const T *pointer; };
template<> struct iterator_traits<int*> { typedef int value_type; };
}}}
"""
        self.source("include/traits.h", text)
        self.add("traits.cpp", '#include "traits.h"\n')
        self.assertEqual(self.audit()["disagreements"], [])
        definitions = interfaces.cpp_class_declarations(text, "include/traits.h")
        self.assertEqual([d.name for d in definitions], [
            "nw::ut::internal::iterator_traits", "nw::ut::internal::iterator_traits<T*>",
            "nw::ut::internal::iterator_traits<const T*>", "nw::ut::internal::iterator_traits<int*>",
        ])
        self.assertTrue(all(d.body is not None for d in definitions))

    def test_cpp_same_qualified_header_and_unit_class_still_disagrees(self):
        self.source("include/heap.h", "namespace nw::ut { class FrameHeap { int state; }; }\n")
        self.add("a.cpp", "namespace nw { namespace ut {\nclass FrameHeap { unsigned state; };\n}}\n")
        self.add("b.cpp", "namespace nw::snd::internal { class FrameHeap { void *state; }; }\n")
        report = self.audit()
        self.assertEqual([(d["name"], d["kind"]) for d in report["disagreements"]], [("class nw::ut::FrameHeap", "odr")])
        self.assertEqual({s["source"] for s in report["disagreements"][0]["sites"]}, {"include/heap.h", "a.cpp"})
        self.assertTrue(interfaces.failed(report, allow_unchecked_cpp=True))

    def test_cpp_same_qualified_class_in_two_units_still_disagrees(self):
        self.add("a.cpp", "namespace nw { namespace ut { struct Shared { int value; }; }}\n")
        self.add("b.cpp", "namespace nw::ut { struct Shared { long value; }; }\n")
        self.assertEqual([(d["name"], d["kind"]) for d in self.audit()["disagreements"]], [("class nw::ut::Shared", "odr")])

    def test_cpp_forward_policy_uses_qualified_identity_and_transitive_headers(self):
        self.source("include/heap.h", "namespace nw::ut { class FrameHeap { int value; }; }\n")
        self.source("include/umbrella.h", "#include <heap.h>\n")
        self.add("missing.cpp", "namespace nw { namespace ut {\nclass FrameHeap;\n}}\n")
        self.add("included.cpp", "#include <umbrella.h>\nnamespace nw::ut { class FrameHeap; }\n")
        self.add("unrelated.cpp", "namespace nw::snd::internal { class FrameHeap; }\n")
        report = self.audit()
        self.assertEqual([(d["name"], d["kind"]) for d in report["disagreements"]], [("class nw::ut::FrameHeap", "type-name")])
        self.assertEqual({s["source"] for s in report["disagreements"][0]["sites"]}, {"include/heap.h", "missing.cpp"})
        self.assertTrue(interfaces.failed(report, allow_unchecked_cpp=True))

    def test_cpp_nested_forward_policy_uses_enclosing_class_identity(self):
        self.source("include/outer.h", "namespace game { class Outer { public: class Inner { int value; }; }; }\n")
        self.add("missing.cpp", "namespace game { class Outer::Inner; }\n")
        self.add("unrelated.cpp", "namespace other { class Outer { class Inner; }; }\n")
        report = self.audit()
        self.assertEqual([(d["name"], d["kind"]) for d in report["disagreements"]], [("class game::Outer::Inner", "type-name")])

    def test_cpp_specialization_odr_and_forward_policy_remain_enforced(self):
        self.source("include/traits.h", "namespace lib {\n"
                    "template<class T> struct traits { int primary; };\n"
                    "template<class T> struct traits<T*> { int pointer; };\n"
                    "template<> struct traits<int> { int integer; };\n}\n")
        self.add("odr.cpp", "namespace lib {\ntemplate<class T> struct traits<T *> { long pointer; };\n}\n")
        self.add("missing.cpp", "namespace lib {\ntemplate<> struct traits<int>;\n}\n")
        self.add("other.cpp", "namespace lib {\ntemplate<> struct traits<float>;\n}\n")
        self.assertEqual([(d["name"], d["kind"]) for d in self.audit()["disagreements"]], [
            ("class lib::traits<T*>", "odr"), ("class lib::traits<int>", "type-name"),
        ])

    def test_cpp_specialized_enclosing_classes_have_distinct_nested_types(self):
        self.add("types.cpp", """namespace lib {
template<class T> class Owner { class iterator { T *value; }; };
template<class T> class Owner<T*> { class iterator { T **value; }; };
template<> class Owner<int> { class iterator { int value; }; };
}
""")
        self.assertEqual(self.audit()["disagreements"], [])

    def test_cpp_unnamed_namespaces_are_unit_local_but_reopen_within_unit(self):
        self.add("a.cpp", "namespace { struct Local { int value; }; }\n")
        self.add("b.cpp", "namespace { struct Local { long value; }; }\n")
        self.assertEqual(self.audit()["disagreements"], [])
        self.add("c.cpp", "namespace { struct Local { int value; }; }\n"
                         "namespace { struct Local { unsigned value; }; }\n")
        report = self.audit()
        self.assertEqual([(d["name"], d["kind"]) for d in report["disagreements"]], [("class (anonymous)::Local", "odr")])
        self.assertEqual({s["source"] for s in report["disagreements"][0]["sites"]}, {"c.cpp"})

    def test_cpp_unnamed_header_classes_belong_to_including_units(self):
        self.source("include/a.h", "namespace { struct Local { int value; }; }\n")
        self.source("include/b.h", "namespace { struct Local { long value; }; }\n")
        self.add("a.cpp", '#include "a.h"\n')
        self.add("b.cpp", '#include "b.h"\n')
        self.add("unrelated.cpp", "namespace { struct Local; }\n")
        self.assertEqual(self.audit()["disagreements"], [])
        self.add("both.cpp", '#include "a.h"\n#include "b.h"\n')
        report = self.audit()
        self.assertEqual([(d["name"], d["kind"]) for d in report["disagreements"]], [("class (anonymous)::Local", "odr")])
        self.assertEqual({s["source"] for s in report["disagreements"][0]["sites"]}, {"include/a.h", "include/b.h"})

    def test_cpp_scope_scanner_ignores_aliases_parameters_enums_and_literal_braces(self):
        declarations = interfaces.cpp_class_declarations("""
#define EXAMPLE namespace fake { class Macro {}; }
namespace real {
template<class Parameter, template<class> class Container> struct Owner {
    using Alias = Parameter;
    typedef Container<Parameter> OtherAlias;
    enum class State { one, two };
    friend class Friend;
    friend bool operator==(Owner, Owner) { return true; }
    struct Inner { const char *brace() { return "} /* not a comment */"; } };
};
namespace alias = real;
using namespace alias;
typedef struct { int value; } AnonymousAlias;
}
""", "types.cpp")
        self.assertEqual([d.name for d in declarations], ["real::Owner", "real::Owner::Inner"])

    def test_cpp_out_of_line_qualified_class_names_and_block_local_classes(self):
        declarations = interfaces.cpp_class_declarations("""
namespace lib { class Outer { class Inner; }; }
struct lib::Outer::Inner { int value; };
namespace other { struct ::lib::Global { int value; }; }
void first() { struct Local { int value; }; }
void second() { struct Local { long value; }; }
""", "types.cpp")
        self.assertEqual([d.name for d in declarations[:4]], [
            "lib::Outer", "lib::Outer::Inner", "lib::Outer::Inner", "lib::Global",
        ])
        self.assertNotEqual(declarations[4].name, declarations[5].name)
        self.assertTrue(all(d.unit_local for d in declarations[4:]))

    def test_preprocess_failure_is_unchecked_not_pass(self):
        self.add("a.c", "int data_00200000;\n")
        with patch.object(interfaces, "preprocess", side_effect=interfaces.InterfaceError("compiler failed (7)")):
            report = interfaces.audit(self.units, self.root, set())
        self.assertFalse(report["units"][0]["checked"])
        self.assertEqual(report["unchecked"][0]["reason"], "compiler failed (7)")
        self.assertTrue(interfaces.failed(report))

    def test_preprocess_uses_launcher_flags_and_build_include_paths(self):
        path = self.add("a.c", "int data_00200000;\n")
        completed = type("Completed", (), {"returncode": 0, "stdout": "int data_00200000;", "stderr": ""})()
        with patch.object(interfaces.subprocess, "run", return_value=completed) as run:
            self.assertEqual(interfaces.preprocess(self.units[0], self.root), completed.stdout)
        argv = run.call_args.args[0]
        launcher = toolchain.wibo_path()
        if launcher:
            self.assertEqual(argv[0], str(launcher))
        self.assertIn(str(toolchain.armcc_exe(PROFILE.compiler)), argv)
        self.assertIn("-E", argv)
        self.assertNotIn("-c", argv)
        self.assertEqual(argv[-1], str(path.resolve()))
        for flag in PROFILE.flags:
            self.assertIn(flag, argv)
        self.assertIn(str((self.root / "include").resolve()), argv)
        self.assertIn(str((toolchain.compiler_root(PROFILE.compiler) / "include").resolve()), argv)

    def test_source_selection_default_ledger_profiles_and_deduplication(self):
        a = self.source("src/nested/a.c", "int data_00200000;\n")
        b = self.source("src/b.cpp", "int data_00200004;\n")
        self.source("src/ignored.h", "")
        units = interfaces.select_units([], None, self.root)
        self.assertEqual({u.source for u in units}, {a, b})
        ledger = self.source("units.json", json.dumps({"units": [{"source": "src/nested/a.c", "profile": "game"}]}))
        self.assertEqual(interfaces.select_units([], ledger, self.root), [interfaces.Unit(a, PROFILE)])
        self.assertEqual(len(interfaces.select_units([str(a), str(a)], None, self.root)), 1)

    def test_cli_json_exit_status_and_human_table(self):
        path = self.add("a.cpp", "class Shared {};\n")
        output = self.root / "out/report.json"
        stdout = io.StringIO()
        with contextlib.redirect_stdout(stdout):
            code = interfaces.main(["check", str(path), "--json", str(output)])
        self.assertEqual(code, 1)
        report = json.loads(output.read_text(encoding="utf-8"))
        self.assertEqual(report["schema_version"], 1)
        self.assertIn("not_covered", report["coverage"])
        self.assertIn("LEVEL", stdout.getvalue())
        self.assertIn("unchecked: 1", stdout.getvalue())
        with contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(interfaces.main(["check", str(path), "--allow-unchecked-cpp"]), 0)


class ArmccIntegrationTest(unittest.TestCase):
    @unittest.skipUnless(toolchain.armcc_exe(PROFILE.compiler).is_file(), "pinned ARMCC not installed")
    def test_real_header_macros_profile_and_source_locations(self):
        with tempfile.TemporaryDirectory(prefix="interfaces-armcc-") as temp:
            root = Path(temp)
            (root / "include").mkdir()
            header = root / "include/shared.h"
            header.write_text("#ifndef SHARED_H\n#define SHARED_H\n#define PARAM unsigned int\nextern int func_00100000(PARAM value);\n#endif\n", encoding="utf-8")
            a, b = root / "a.c", root / "b.c"
            a.write_text('#include "shared.h"\nint func_00100000(PARAM renamed) { return renamed; }\n', encoding="utf-8")
            b.write_text('#include "shared.h"\nint local(void) { return func_00100000(1); }\n', encoding="utf-8")
            units = [interfaces.Unit(path, PROFILE) for path in (a, b)]
            report = interfaces.audit(units, root, set())
            self.assertEqual(report["unchecked"], [])
            self.assertEqual(report["disagreements"], [])
            self.assertEqual(report["policy"], [])
            b.write_text("extern int func_00100000(int other);\n", encoding="utf-8")
            report = interfaces.audit(units, root, set())
            self.assertEqual(report["unchecked"], [])
            self.assertTrue(interfaces.failed(report))
            self.assertEqual(len(report["disagreements"]), 1)
            self.assertEqual(len(report["policy"]), 1)
            sites = report["disagreements"][0]["sites"]
            self.assertTrue(any(site["source"] == "include/shared.h" and site["line"] == 4 for site in sites))

    @unittest.skipUnless(toolchain.armcc_exe(PROFILE.compiler).is_file(), "pinned ARMCC not installed")
    def test_shared_extension_headers_compile_in_c_and_cpp_and_audit_in_c(self):
        with tempfile.TemporaryDirectory(prefix="interfaces-extensions-") as temp:
            root = Path(temp)
            headers = root / "include/ctr"
            headers.mkdir(parents=True)
            for name in ("svc.h", "cp15.h"):
                (headers / name).write_text((interfaces.ROOT / "include/ctr" / name).read_text(encoding="utf-8"), encoding="utf-8")
            text = (
                '#include "ctr/svc.h"\n#include "ctr/cp15.h"\n'
                'register unsigned int file_tls __asm("cp15:0:c13:c0:3");\n'
                'unsigned int func_00100000(CtrHandle handle, volatile unsigned int *word) {\n'
                '  CtrSvcHandleResult event = svcCreateEvent(0, 0);\n'
                '  CtrSvcProcessInfoResult info = svcGetProcessInfo(0, handle, 0);\n'
                '  unsigned int value = __ldrex(word);\n'
                '  __strex(value, word); __clrex();\n'
                '  ctrDataMemoryBarrier(); ctrDataSynchronizationBarrier(); __schedule_barrier();\n'
                '  svcCloseHandle(event.handle);\n'
                '  return (unsigned int)svcGetSystemTick() + info.value_low + file_tls\n'
                '       + (unsigned int)ctrGetThreadLocalStorage();\n}\n'
            )
            for suffix in ("c", "cpp"):
                source = root / f"extensions.{suffix}"
                source.write_text(text, encoding="utf-8")
                toolchain.compile_source(PROFILE, source, root / f"extensions-{suffix}.o", (root / "include",))
                if suffix == "c":
                    report = interfaces.audit([interfaces.Unit(source, PROFILE)], root, set())
                    self.assertEqual(report["unchecked"], [])
                    self.assertEqual(report["disagreements"], [])
                    self.assertEqual(report["policy"], [])

    @unittest.skipUnless(toolchain.armcc_exe(PROFILE.compiler).is_file(), "pinned ARMCC not installed")
    def test_armcc_standard_headers_compile_and_audit(self):
        headers = (
            "assert.h", "ctype.h", "errno.h", "float.h", "inttypes.h", "iso646.h",
            "limits.h", "locale.h", "math.h", "setjmp.h", "signal.h", "stdarg.h",
            "stdbool.h", "stddef.h", "stdint.h", "stdio.h", "stdlib.h", "string.h",
            "time.h", "wchar.h", "wctype.h",
        )
        with tempfile.TemporaryDirectory(prefix="interfaces-standard-") as temp:
            root = Path(temp)
            for header in headers:
                with self.subTest(header=header):
                    source = root / (header.removesuffix(".h") + ".c")
                    source.write_text(f"#include <{header}>\nint local(void) {{ return 0; }}\n", encoding="utf-8")
                    toolchain.compile_source(PROFILE, source, source.with_suffix(".o"))
                    report = interfaces.audit([interfaces.Unit(source, PROFILE)], root, set())
                    self.assertEqual(report["unchecked"], [])
                    self.assertEqual(report["disagreements"], [])
                    self.assertEqual(report["policy"], [])


if __name__ == "__main__":
    unittest.main()
