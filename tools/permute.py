"""Search C/C++ source shapes with the repository's strict in-place matching oracle.

Only selected function bodies are rewritten. Outputs are private author hints, not
accepted source; no original instructions or bytes are rendered or recorded.
"""
from __future__ import annotations

import argparse
import copy
import difflib
import hashlib
import json
import multiprocessing
import os
import random
import re
import sys
import tempfile
import time
from collections import Counter
from dataclasses import dataclass
from pathlib import Path

from pycparser import c_ast, c_generator, c_parser, plyparser

sys.path.insert(0, str(Path(__file__).resolve().parent))
import image  # noqa: E402
import inplace  # noqa: E402
import interfaces  # noqa: E402
import match  # noqa: E402
import toolchain  # noqa: E402
import permute_cpp as cpp  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
MODEL = "armlink5-inline-branchnop-v2"
GENERATOR = c_generator.CGenerator()
MUTATIONS = (
    "reorder", "temporary", "inline_temporary", "commute", "comparison",
    "condition", "loop", "increment", "cast", "signedness", "declaration", "qualifier", "pointer",
)
INTEGER_TYPES = {
    "int", "unsigned int", "long", "unsigned long", "long long", "unsigned long long",
}
CPP_SUFFIXES = {".cpp", ".cc", ".cxx"}


class PermuteError(RuntimeError):
    """An input cannot safely enter the search."""


def walk(node: c_ast.Node):
    yield node
    for _, child in node.children():
        yield from walk(child)


def references(node: c_ast.Node, name: str) -> int:
    return sum(isinstance(n, c_ast.ID) and n.name == name for n in walk(node))


def replace_node(root: c_ast.Node, old: c_ast.Node, new: c_ast.Node) -> bool:
    for attr, child in root.children():
        if child is old:
            indexed = re.fullmatch(r"(\w+)\[(\d+)\]", attr)
            if indexed:
                getattr(root, indexed[1])[int(indexed[2])] = new
            else:
                setattr(root, attr, new)
            return True
        if replace_node(child, old, new):
            return True
    return False


def value_ids(node: c_ast.Node):
    """IDs used as values, never member names or modifiable/addressed lvalues."""
    for attr, child in node.children():
        excluded = (isinstance(node, c_ast.StructRef) and attr == "field"
                    or isinstance(node, c_ast.Assignment) and attr == "lvalue"
                    or isinstance(node, c_ast.UnaryOp) and node.op in {"&", "++", "--", "p++", "p--"})
        if isinstance(child, c_ast.ID):
            if not excluded:
                yield child
        else:
            yield from value_ids(child)


def function_spans(text: str) -> dict[str, tuple[int, int, int]]:
    """Name -> (declaration start, body start, body end), without interpreting macros.

    Token boundaries, not brace regexes, handle strings, comments and nested bodies.
    Splice continued lines before recognizing comments or preprocessing directives,
    retaining a map back to the exact input for every returned offset.
    """
    pieces, offsets = [], []
    cursor = 0
    for continuation in re.finditer(r"\\\r?\n", text):
        pieces.append(text[cursor:continuation.start()])
        offsets.extend(range(cursor, continuation.start()))
        cursor = continuation.end()
    pieces.append(text[cursor:])
    offsets.extend(range(cursor, len(text)))
    spliced = "".join(pieces)
    tokens = []
    line_start, directive, previous_end = True, False, 0
    for token in interfaces.TOKEN.finditer(spliced):
        if "\n" in spliced[previous_end:token.start()]:
            line_start, directive = True, False
        previous_end = token.end()
        if token[0].startswith(("/*", "//")):
            continue
        if line_start:
            directive = token[0] == "#"
        line_start = False
        if not directive:
            tokens.append(token)
    spans = {}
    start = 0
    depth = 0
    i = 0
    while i < len(tokens):
        word = tokens[i][0]
        if not depth and re.fullmatch(r"[A-Za-z_]\w*", word) and i + 1 < len(tokens) and tokens[i + 1][0] == "(":
            j, parens = i + 1, 0
            while j < len(tokens):
                parens += (tokens[j][0] == "(") - (tokens[j][0] == ")")
                j += 1
                if not parens:
                    break
            while j + 1 < len(tokens) and tokens[j][0] in {"__attribute__", "__attribute", "__declspec"} and tokens[j + 1][0] == "(":
                j, parens = j + 1, 0
                while j < len(tokens):
                    parens += (tokens[j][0] == "(") - (tokens[j][0] == ")")
                    j += 1
                    if not parens:
                        break
            if j < len(tokens) and tokens[j][0] == "{":
                k, braces = j, 0
                while k < len(tokens):
                    braces += (tokens[k][0] == "{") - (tokens[k][0] == "}")
                    k += 1
                    if not braces:
                        break
                if braces:
                    raise PermuteError("unbalanced function body")
                spans[word] = (offsets[tokens[start].start()], offsets[tokens[j].start()],
                               offsets[tokens[k - 1].end() - 1] + 1)
                i, start = k, k
                continue
        depth += (word == "{") - (word == "}")
        if not depth and word == ";":
            start = i + 1
        i += 1
    return spans


@dataclass
class Program:
    text: str
    ast: c_ast.FileAST
    targets: tuple[str, ...]
    spans: dict[str, tuple[int, int, int]]
    original_functions: dict[str, str]
    suffix: str = ".c"

    @property
    def oracle_targets(self) -> tuple[str, ...]:
        return self.targets

    @classmethod
    def parse(cls, text: str, targets: tuple[str, ...], source: Path,
              original: str | None = None) -> Program | cpp.Program:
        if source.suffix.lower() in CPP_SUFFIXES:
            try:
                return cpp.Program.parse(text, targets, source, original)
            except cpp.CppError as exc:
                raise PermuteError(str(exc)) from exc
        # Let the interface auditor recognize approved SVC/result/CP15 declarations.
        # Their raw spelling is retained; target-body annotations still fail closed.
        try:
            prepared = interfaces.prepare(text, source)
        except interfaces.InterfaceError as exc:
            raise PermuteError(str(exc)) from exc
        try:
            ast = c_parser.CParser().parse(prepared.text, filename=str(source))
        except plyparser.ParseError as exc:
            raise PermuteError("preprocessed input is not supported C (C++ is not supported)") from exc
        functions = {n.decl.name: n for n in ast.ext if isinstance(n, c_ast.FuncDef)}
        parser_spans = function_spans(prepared.text)
        # prepare() expands __int64 to long long, so its offsets cannot slice text.
        # Keep parser coordinates for annotation checks and raw coordinates for edits.
        spans = function_spans(text)
        for name in targets:
            if name not in functions or name not in spans or name not in parser_spans:
                raise PermuteError(f"target function is not defined: {name}")
            _, start, end = parser_spans[name]
            if any(start <= a.start < end for a in prepared.annotations):
                raise PermuteError(f"target body has ABI-relevant ARMCC annotations: {name}")
            if functions[name].param_decls:
                raise PermuteError("old-style function definitions are not supported")
        originals = function_spans(original) if original is not None else spans
        original_text = text if original is None else original
        for name in targets:
            if name not in originals:
                raise PermuteError(f"cannot locate original function text (macro-generated definition?): {name}")
        return cls(text, ast, targets, spans, {
            name: original_text[originals[name][0]:originals[name][2]].strip() + "\n" for name in targets
        })

    @classmethod
    def preprocess(cls, source: Path, targets: tuple[str, ...], profile: toolchain.Profile) -> Program | cpp.Program:
        if source.suffix.lower() not in {".c", *CPP_SUFFIXES}:
            raise PermuteError("the permuter accepts .c, .cpp, .cc or .cxx source files")
        # This is exactly the interface auditor's launcher, flags and include path.
        text = interfaces.preprocess(interfaces.Unit(source, profile))
        return cls.parse(text, targets, source, source.read_text(encoding="utf-8"))

    def functions(self, ast: c_ast.FileAST | None = None) -> dict[str, c_ast.FuncDef]:
        return {n.decl.name: n for n in (ast or self.ast).ext if isinstance(n, c_ast.FuncDef) and n.decl.name in self.targets}

    def render(self, ast: c_ast.FileAST) -> str:
        text = self.text
        functions = self.functions(ast)
        # Keep all headers, declarations, non-target functions and ARMCC extensions verbatim.
        for name in sorted(self.targets, key=lambda n: self.spans[n][1], reverse=True):
            _, start, end = self.spans[name]
            text = text[:start] + GENERATOR.visit(functions[name].body) + text[end:]
        return text

    def hints(self, ast: c_ast.FileAST | None) -> tuple[str, str]:
        text = self.text if ast is None else self.render(ast)
        spans = function_spans(text)
        functions, patches = [], []
        for name in self.targets:
            start, _, end = spans[name]
            function = text[start:end].strip() + "\n"
            functions.append(function)
            patches.extend(difflib.unified_diff(
                self.original_functions[name].splitlines(keepends=True), function.splitlines(keepends=True),
                fromfile=f"original/{name}.c", tofile=f"permuted/{name}.c",
            ))
        return "\n".join(functions), "".join(patches)


def integer_name(words: list[str]) -> str:
    if not set(words) <= {"signed", "unsigned", "char", "short", "int", "long"}:
        return " ".join(words)
    base = "char" if "char" in words else "short" if "short" in words else "long long" if words.count("long") == 2 else "long" if "long" in words else "int"
    return ("unsigned " if "unsigned" in words else "") + base


class Types:
    """Conservative local type/effect facts; unknown aliasing means no rewrite."""

    def __init__(self, ast: c_ast.FileAST, function: c_ast.FuncDef):
        aliases = [n for n in ast.ext if isinstance(n, c_ast.Typedef)]
        aliases += [n for n in walk(function.body) if isinstance(n, c_ast.Typedef)]
        alias_counts = Counter(n.name for n in aliases)
        self.typedefs = {n.name: n.type if alias_counts[n.name] == 1 else None for n in aliases}
        records = [n for n in walk(ast) if isinstance(n, c_ast.Struct) and n.name and n.decls is not None]
        record_counts = Counter(n.name for n in records)
        self.records = {n.name: n for n in records if record_counts[n.name] == 1}
        declarations = [n for n in ast.ext if isinstance(n, c_ast.Decl) and not isinstance(n.type, c_ast.FuncDecl)]
        parameters = function.decl.type.args
        local = ([n for n in parameters.params if isinstance(n, c_ast.Decl)] if parameters else [])
        local += [n for n in walk(function.body) if isinstance(n, c_ast.Decl)]
        self.locals = {n.name for n in local if n.name}
        counts = Counter(n.name for n in declarations + local if n.name)
        self.declarations = {n.name: n for n in declarations + local if n.name and counts[n.name] == 1}
        self.escaped = {n.expr.name for n in walk(function.body) if isinstance(n, c_ast.UnaryOp) and n.op == "&" and isinstance(n.expr, c_ast.ID)}
        self.function = function

    def resolve(self, node: c_ast.Node | None) -> c_ast.Node | None:
        seen = set()
        while isinstance(node, c_ast.TypeDecl):
            base = node.type
            if isinstance(base, c_ast.IdentifierType) and len(base.names) == 1 and base.names[0] in self.typedefs:
                name = base.names[0]
                if name in seen:
                    return None
                seen.add(name)
                node = self.typedefs[name]
            else:
                return base
        return node

    def volatile(self, node: c_ast.Node | None) -> bool:
        if node is None:
            return True
        if "volatile" in getattr(node, "quals", []):
            return True
        if isinstance(node, (c_ast.PtrDecl, c_ast.ArrayDecl, c_ast.TypeDecl)):
            base = node.type
            if isinstance(base, c_ast.IdentifierType) and len(base.names) == 1 and base.names[0] in self.typedefs:
                return self.volatile(self.typedefs[base.names[0]])
            return self.volatile(base)
        return False

    def top_const(self, node: c_ast.Node) -> bool:
        if "const" in getattr(node, "quals", []):
            return True
        if isinstance(node, c_ast.TypeDecl) and isinstance(node.type, c_ast.IdentifierType) and len(node.type.names) == 1:
            alias = self.typedefs.get(node.type.names[0])
            return alias is not None and self.top_const(alias)
        return False

    def words(self, node: c_ast.Node | None) -> str | None:
        base = self.resolve(node)
        return integer_name(base.names) if isinstance(base, c_ast.IdentifierType) else None

    def pointed(self, node: c_ast.Node | None) -> c_ast.Node | None:
        base = self.resolve(node)
        return base.type if isinstance(base, (c_ast.PtrDecl, c_ast.ArrayDecl)) else None

    def expr(self, node: c_ast.Node | None) -> c_ast.Node | None:
        if isinstance(node, c_ast.ID):
            decl = self.declarations.get(node.name)
            return decl.type if decl else None
        if isinstance(node, c_ast.Constant):
            return c_ast.TypeDecl(None, [], None, c_ast.IdentifierType(node.type.split()))
        if isinstance(node, c_ast.Cast):
            return node.to_type.type
        if isinstance(node, c_ast.ArrayRef):
            return self.pointed(self.expr(node.name))
        if isinstance(node, c_ast.StructRef):
            base = self.expr(node.name)
            if node.type == "->":
                base = self.pointed(base)
            record = self.resolve(base)
            if isinstance(record, c_ast.Struct):
                record = record if record.decls is not None else self.records.get(record.name)
                if record:
                    return next((d.type for d in record.decls if d.name == node.field.name), None)
        if isinstance(node, c_ast.UnaryOp):
            if node.op == "*":
                return self.pointed(self.expr(node.expr))
            if node.op == "&":
                return c_ast.PtrDecl([], self.expr(node.expr))
            if node.op == "!":
                return c_ast.TypeDecl(None, [], None, c_ast.IdentifierType(["int"]))
            return self.expr(node.expr)
        if isinstance(node, c_ast.BinaryOp):
            left, right = self.expr(node.left), self.expr(node.right)
            if node.op in {"==", "!=", "<", ">", "<=", ">=", "&&", "||"}:
                return c_ast.TypeDecl(None, [], None, c_ast.IdentifierType(["int"]))
            lw, rw = self.words(left), self.words(right)
            if node.op in {"<<", ">>"} and lw in INTEGER_TYPES:
                return left
            if lw == rw and lw in INTEGER_TYPES:
                return left
            if {lw, rw} <= {"int", "unsigned int"}:
                return c_ast.TypeDecl(None, [], None, c_ast.IdentifierType(["unsigned", "int"]))
            if node.op in {"+", "-"} and self.pointed(left) is not None and rw in INTEGER_TYPES:
                return left
        return None

    def pure(self, node: c_ast.Node | None, local_only: bool = False) -> bool:
        if node is None:
            return True
        if isinstance(node, c_ast.Constant):
            return True
        if isinstance(node, c_ast.ID):
            decl = self.declarations.get(node.name)
            return bool(decl and not self.volatile(decl.type) and node.name not in self.escaped and (not local_only or node.name in self.locals))
        if isinstance(node, c_ast.Cast):
            return not self.volatile(node.to_type.type) and self.pure(node.expr, local_only)
        if isinstance(node, c_ast.BinaryOp):
            return self.pure(node.left, local_only) and self.pure(node.right, local_only)
        if isinstance(node, c_ast.UnaryOp) and node.op in {"+", "-", "!", "~"}:
            return self.pure(node.expr, local_only)
        if not local_only and isinstance(node, c_ast.StructRef):
            return self.location(node) is not None
        if not local_only and isinstance(node, c_ast.ArrayRef):
            return not self.volatile(self.expr(node)) and self.pure(node.name) and self.pure(node.subscript)
        return False

    def location(self, node: c_ast.Node) -> tuple | None:
        if isinstance(node, c_ast.ID):
            decl = self.declarations.get(node.name)
            if not decl or self.volatile(decl.type) or node.name in self.escaped or not self.words(decl.type):
                return None
            return ("local", node.name) if node.name in self.locals else ("memory", node.name)
        fields = []
        cursor = node
        while isinstance(cursor, c_ast.StructRef):
            if self.volatile(self.expr(cursor)):
                return None
            # A second pointer dereference can alias the first object's fields.
            if cursor.type == "->" and isinstance(cursor.name, c_ast.StructRef):
                return None
            fields.insert(0, cursor.field.name)
            cursor = cursor.name
        if not isinstance(cursor, c_ast.ID) or not fields:
            return None
        if cursor.name not in self.declarations or cursor.name in self.escaped:
            return None
        if self.volatile(self.declarations[cursor.name].type):
            return None
        return ("memory", cursor.name, *fields)

    def reads(self, node: c_ast.Node | None) -> set[tuple] | None:
        if node is None or isinstance(node, c_ast.Constant):
            return set()
        if isinstance(node, (c_ast.ID, c_ast.StructRef)):
            location = self.location(node)
            if location is None:
                return None
            reads = {location}
            cursor = node
            while isinstance(cursor, c_ast.StructRef):
                if cursor.type == "->" and isinstance(cursor.name, c_ast.ID):
                    reads.add(("local", cursor.name.name) if cursor.name.name in self.locals else ("memory", cursor.name.name))
                cursor = cursor.name
            return reads
        if isinstance(node, c_ast.Cast):
            return self.reads(node.expr) if not self.volatile(node.to_type.type) else None
        if isinstance(node, c_ast.UnaryOp) and node.op in {"+", "-", "!", "~"}:
            return self.reads(node.expr)
        if isinstance(node, c_ast.BinaryOp):
            left, right = self.reads(node.left), self.reads(node.right)
            return None if left is None or right is None else left | right
        return None

    def effect(self, node: c_ast.Node) -> tuple[set[tuple], set[tuple]] | None:
        if isinstance(node, c_ast.Decl) and node.name in self.locals and not node.storage and self.words(node.type) and not self.volatile(node.type):
            reads = self.reads(node.init)
            return None if reads is None else (reads, {("local", node.name)})
        if isinstance(node, c_ast.Assignment) and node.op == "=":
            writes, reads = self.location(node.lvalue), self.reads(node.rvalue)
            if writes is not None and reads is not None:
                # Lvalue address evaluation reads its pointer, but not its stored value.
                address_reads = (self.reads(node.lvalue) or set()) - {writes}
                return reads | address_reads, {writes}
        return None


def overlaps(left: tuple, right: tuple) -> bool:
    if left[0] != right[0]:
        return False
    if left[0] == "local":
        return left == right
    if left[1] != right[1]:
        return True  # Distinct pointer names may alias.
    return left[:min(len(left), len(right))] == right[:min(len(left), len(right))]


def independent(left: c_ast.Node, right: c_ast.Node, types: Types) -> bool:
    a, b = types.effect(left), types.effect(right)
    return bool(a is not None and b is not None and not any(
        overlaps(x, y) for xs, ys in ((a[1], b[0] | b[1]), (b[1], a[0])) for x in xs for y in ys
    ))


def fresh_name(function: c_ast.FuncDef) -> str:
    used = {n.name for n in walk(function) if isinstance(n, (c_ast.ID, c_ast.Decl, c_ast.Typedef))}
    used.update(word for n in walk(function) if isinstance(n, c_ast.IdentifierType) for word in n.names)
    index = 0
    while f"permute_tmp_{index}" in used:
        index += 1
    return f"permute_tmp_{index}"


def new_decl(name: str, type_node: c_ast.Node, value: c_ast.Node) -> c_ast.Decl:
    type_node = copy.deepcopy(type_node)
    cursor = type_node
    while not isinstance(cursor, c_ast.TypeDecl):
        cursor = cursor.type
    cursor.declname = name
    cursor.quals = []
    return c_ast.Decl(name, [], None, [], [], type_node, value, None)


def assigned(function: c_ast.FuncDef, name: str) -> bool:
    return any((isinstance(n, c_ast.Assignment) and isinstance(n.lvalue, c_ast.ID) and n.lvalue.name == name)
               or (isinstance(n, c_ast.UnaryOp) and n.op in {"++", "--", "p++", "p--"} and isinstance(n.expr, c_ast.ID) and n.expr.name == name)
               for n in walk(function.body))


def mutate_function(ast: c_ast.FileAST, function: c_ast.FuncDef, rng: random.Random, kind: str) -> bool:
    """Apply one eligible AST rewrite; all choices have explicit conservative guards."""
    types = Types(ast, function)
    nodes = list(walk(function.body))
    blocks = [n for n in nodes if isinstance(n, c_ast.Compound)]
    choices = []
    control_labels = any(isinstance(n, (c_ast.Goto, c_ast.Label)) for n in nodes)
    if kind == "reorder" and not control_labels:
        for block in blocks:
            items = block.block_items or []
            choices += [(block, i) for i in range(len(items) - 1) if independent(items[i], items[i + 1], types)]
        if choices:
            block, i = rng.choice(choices)
            block.block_items[i], block.block_items[i + 1] = block.block_items[i + 1], block.block_items[i]
    elif kind == "temporary" and not control_labels:
        for block in blocks:
            for i, stmt in enumerate(block.block_items or []):
                expr = stmt.expr if isinstance(stmt, c_ast.Return) else stmt.rvalue if isinstance(stmt, c_ast.Assignment) and stmt.op == "=" else stmt.init if isinstance(stmt, c_ast.Decl) and not stmt.storage else None
                if isinstance(expr, (c_ast.BinaryOp, c_ast.UnaryOp, c_ast.Cast)) and types.pure(expr, True) and types.words(types.expr(expr)) in INTEGER_TYPES:
                    choices.append((block, i, stmt, expr))
        if choices:
            block, i, stmt, expr = rng.choice(choices)
            name = fresh_name(function)
            decl = new_decl(name, types.expr(expr), expr)
            replace_node(stmt, expr, c_ast.ID(name))
            block.block_items.insert(i, decl)
    elif kind == "inline_temporary" and not control_labels:
        for block in blocks:
            items = block.block_items or []
            for i, decl in enumerate(items[:-1]):
                if not isinstance(decl, c_ast.Decl) or decl.storage or not decl.init or types.words(decl.type) not in INTEGER_TYPES or types.volatile(decl.type):
                    continue
                next_stmt = items[i + 1]
                expr = next_stmt.expr if isinstance(next_stmt, c_ast.Return) else next_stmt.rvalue if isinstance(next_stmt, c_ast.Assignment) and next_stmt.op == "=" else next_stmt.init if isinstance(next_stmt, c_ast.Decl) else None
                if expr is not None and references(function.body, decl.name) == 1 and references(expr, decl.name) == 1 and types.pure(decl.init, True) and types.pure(expr, True) and types.words(types.expr(decl.init)) == types.words(decl.type):
                    choices.append((block, i, expr, decl))
        if choices:
            block, i, expr, decl = rng.choice(choices)
            use = next(n for n in walk(expr) if isinstance(n, c_ast.ID) and n.name == decl.name)
            # Preserve the declared conversion even when integer promotions differ.
            cast = c_ast.Cast(c_ast.Typename(None, [], None, new_decl("", decl.type, decl.init).type), decl.init)
            cast.to_type.type.declname = None
            replace_node(block.block_items[i + 1], use, cast)
            del block.block_items[i]
    elif kind in {"commute", "comparison"}:
        operations = {"+", "*", "&", "|", "^"} if kind == "commute" else {"==", "!=", "<", ">", "<=", ">="}
        choices = [n for n in nodes if isinstance(n, c_ast.BinaryOp) and n.op in operations and types.pure(n.left) and types.pure(n.right)
                   and (kind == "comparison" or types.words(types.expr(n.left)) in INTEGER_TYPES and types.words(types.expr(n.right)) in INTEGER_TYPES)]
        if choices:
            node = rng.choice(choices)
            node.left, node.right = node.right, node.left
            if kind == "comparison":
                node.op = {"<": ">", ">": "<", "<=": ">=", ">=": "<=", "==": "==", "!=": "!="}[node.op]
    elif kind == "condition":
        choices = [n for n in nodes if isinstance(n, c_ast.If)]
        if choices:
            node = rng.choice(choices)
            if node.iffalse is not None:
                node.cond = c_ast.UnaryOp("!", node.cond)
                node.iftrue, node.iffalse = c_ast.Compound([node.iffalse]), c_ast.Compound([node.iftrue])
            elif isinstance(node.cond, c_ast.UnaryOp) and node.cond.op == "!" and isinstance(node.cond.expr, c_ast.UnaryOp) and node.cond.expr.op == "!":
                node.cond = node.cond.expr.expr
            else:
                node.cond = c_ast.UnaryOp("!", c_ast.UnaryOp("!", node.cond))
    elif kind == "loop" and not control_labels:
        choices = [n for n in nodes if isinstance(n, (c_ast.For, c_ast.While, c_ast.DoWhile)) and (isinstance(n, c_ast.While) or not any(isinstance(c, c_ast.Continue) for c in walk(n.stmt)))]
        if choices:
            node = rng.choice(choices)
            if isinstance(node, c_ast.For):
                body = c_ast.Compound([node.stmt] + ([node.next] if node.next else []))
                loop = c_ast.While(node.cond or c_ast.Constant("int", "1"), body)
                init = node.init.decls if isinstance(node.init, c_ast.DeclList) else [node.init] if node.init else []
                replacement = c_ast.Compound(init + [loop])
            elif isinstance(node, c_ast.DoWhile):
                replacement = c_ast.While(c_ast.Constant("int", "1"), c_ast.Compound([node.stmt, c_ast.If(c_ast.UnaryOp("!", node.cond), c_ast.Break(), None)]))
            elif rng.randrange(2):
                replacement = c_ast.For(None, node.cond, None, node.stmt)
            else:
                replacement = c_ast.Compound([c_ast.If(copy.deepcopy(node.cond), c_ast.DoWhile(node.cond, node.stmt), None)])
            replace_node(function.body, node, replacement)
    elif kind == "increment":
        statements = [s for b in blocks for s in b.block_items or []] + [n.next for n in nodes if isinstance(n, c_ast.For) and n.next]
        choices = [n for n in statements if isinstance(n, c_ast.UnaryOp) and n.op in {"++", "--", "p++", "p--"} and isinstance(n.expr, c_ast.ID) and types.pure(n.expr)]
        choices += [n for n in statements if isinstance(n, c_ast.Assignment) and n.op in {"+=", "-="} and isinstance(n.lvalue, c_ast.ID) and types.pure(n.lvalue) and isinstance(n.rvalue, c_ast.Constant) and n.rvalue.value == "1"]
        if choices:
            node = rng.choice(choices)
            if isinstance(node, c_ast.Assignment):
                replacement = c_ast.UnaryOp("p++" if node.op == "+=" else "p--", node.lvalue)
            elif rng.randrange(2):
                replacement = c_ast.UnaryOp(node.op[1:] if node.op.startswith("p") else "p" + node.op, node.expr)
            else:
                replacement = c_ast.Assignment("+=" if "+" in node.op else "-=", node.expr, c_ast.Constant("int", "1"))
            replace_node(function.body, node, replacement)
    elif kind == "cast":
        choices = [n for n in value_ids(function.body) if types.pure(n, True) and types.words(types.expr(n)) in INTEGER_TYPES]
        choices += [n for n in nodes if isinstance(n, c_ast.Cast) and types.words(n.to_type.type) in INTEGER_TYPES and types.words(n.to_type.type) == types.words(types.expr(n.expr)) and types.pure(n.expr, True)]
        if choices:
            node = rng.choice(choices)
            if isinstance(node, c_ast.Cast):
                replacement = node.expr
            else:
                typename = new_decl("", types.expr(node), node).type
                typename.declname = None
                replacement = c_ast.Cast(c_ast.Typename(None, [], None, typename), node)
            replace_node(function.body, node, replacement)
    elif kind == "signedness":
        choices = [n for n in nodes if isinstance(n, c_ast.Decl) and types.declarations.get(n.name) is n and isinstance(n.type, c_ast.TypeDecl) and isinstance(n.type.type, c_ast.IdentifierType) and set(n.type.type.names) <= {"signed", "unsigned", "int", "long"} and types.words(n.type) in INTEGER_TYPES and not n.storage and not types.volatile(n.type)]
        if choices:
            node = rng.choice(choices)
            node.type = copy.deepcopy(node.type)
            words = node.type.type.names
            # Actual sign changes are restricted to immutable small nonnegative indexes.
            uses = [n for n in nodes if isinstance(n, c_ast.ArrayRef) and isinstance(n.subscript, c_ast.ID) and n.subscript.name == node.name]
            literal = node.init.value if isinstance(node.init, c_ast.Constant) and node.init.type == "int" else ""
            if literal.isdecimal() and int(literal) <= 0x7fffffff and not assigned(function, node.name) and node.name not in types.escaped and uses and references(function.body, node.name) == len(uses) and types.words(node.type) in {"int", "unsigned int"}:
                node.type.type.names = ["int"] if "unsigned" in words else ["unsigned", "int"]
            else:
                canonical = types.words(node.type).split()
                alternate = ["signed", *canonical] if "unsigned" not in canonical else [*canonical, "int"] if "long" in canonical else ["unsigned"]
                node.type.type.names = canonical if words == alternate else alternate
    elif kind == "declaration" and not control_labels:
        for block in blocks:
            items = block.block_items or []
            for i, decl in enumerate(items):
                if not isinstance(decl, c_ast.Decl) or decl.storage or types.words(decl.type) not in INTEGER_TYPES or types.volatile(decl.type) or types.top_const(decl.type):
                    continue
                if decl.init is not None:
                    choices.append((block, i, True))
                elif i + 1 < len(items) and isinstance(items[i + 1], c_ast.Assignment) and items[i + 1].op == "=" and isinstance(items[i + 1].lvalue, c_ast.ID) and items[i + 1].lvalue.name == decl.name:
                    choices.append((block, i, False))
        if choices:
            block, i, split = rng.choice(choices)
            decl = block.block_items[i]
            if split:
                assignment = c_ast.Assignment("=", c_ast.ID(decl.name), decl.init)
                decl.init = None
                block.block_items.insert(i + 1, assignment)
            else:
                decl.init = block.block_items.pop(i + 1).rvalue
    elif kind == "qualifier":
        choices = [n for n in nodes if isinstance(n, c_ast.Decl) and types.declarations.get(n.name) is n and n.init is not None and not n.storage and isinstance(n.type, (c_ast.TypeDecl, c_ast.PtrDecl)) and (types.words(n.type) is not None or isinstance(types.resolve(n.type), c_ast.PtrDecl)) and not types.volatile(n.type) and n.name not in types.escaped and not assigned(function, n.name)]
        if choices:
            node = rng.choice(choices)
            node.type = copy.deepcopy(node.type)
            node.type.quals = [q for q in node.type.quals if q != "const"] if "const" in node.type.quals else [*node.type.quals, "const"]
            if isinstance(node.type, c_ast.TypeDecl):
                node.quals = list(node.type.quals)
    elif kind == "pointer":
        choices = [n for n in nodes if isinstance(n, c_ast.ArrayRef) and types.pointed(types.expr(n.name)) is not None and types.pure(n.name) and types.pure(n.subscript)]
        choices += [n for n in nodes if isinstance(n, c_ast.UnaryOp) and n.op == "*" and isinstance(n.expr, c_ast.BinaryOp) and n.expr.op == "+" and types.pointed(types.expr(n.expr.left)) is not None and types.pure(n.expr.left) and types.pure(n.expr.right)]
        choices += [n for n in nodes if isinstance(n, c_ast.StructRef) and n.type == "->"]
        choices += [n for n in nodes if isinstance(n, c_ast.StructRef) and n.type == "." and isinstance(n.name, c_ast.UnaryOp) and n.name.op == "*"]
        if choices:
            node = rng.choice(choices)
            if isinstance(node, c_ast.ArrayRef):
                replacement = c_ast.UnaryOp("*", c_ast.BinaryOp("+", node.name, node.subscript))
            elif isinstance(node, c_ast.UnaryOp):
                replacement = c_ast.ArrayRef(node.expr.left, node.expr.right)
            elif node.type == "->":
                replacement = c_ast.StructRef(c_ast.UnaryOp("*", node.name), ".", node.field)
            else:
                replacement = c_ast.StructRef(node.name.expr, "->", node.field)
            replace_node(function.body, node, replacement)
    return bool(choices)


def c90_scopes(node: c_ast.Node) -> None:
    """Nest a suffix instead of moving an initializer across executable code.

    The frozen game profile is C90. New temporaries and split declarations may
    otherwise create mixed declarations, even when the input is valid C90.
    """
    for _, child in list(node.children()):
        c90_scopes(child)
    if isinstance(node, c_ast.Compound):
        executable = False
        for index, item in enumerate(node.block_items or []):
            if isinstance(item, (c_ast.Decl, c_ast.Typedef)):
                if executable:
                    suffix = c_ast.Compound(node.block_items[index:])
                    node.block_items[index:] = [suffix]
                    c90_scopes(suffix)
                    break
            else:
                executable = True


def mutate(program: Program | cpp.Program, ast: c_ast.FileAST | str, rng: random.Random,
           kind: str | None = None) -> tuple[c_ast.FileAST | str, str] | None:
    if isinstance(program, cpp.Program):
        try:
            return cpp.mutate(program, ast, rng, kind)
        except cpp.CppError as exc:
            raise PermuteError(str(exc)) from exc
    kinds = [kind] if kind else list(MUTATIONS)
    rng.shuffle(kinds)
    names = list(program.targets)
    rng.shuffle(names)
    for mutation in kinds:
        for name in names:
            candidate = copy.deepcopy(ast)
            function = program.functions(candidate)[name]
            if mutate_function(candidate, function, rng, mutation):
                c90_scopes(function.body)
                return candidate, f"{name}:{mutation}"
    return None


@dataclass(frozen=True)
class Evaluation:
    score: int | None
    functions: tuple[dict, ...] = ()
    error: str | None = None


def score_rows(rows: list[dict], targets: tuple[str, ...], functions: dict[int, dict]) -> Evaluation:
    selected = {r["name"]: r for r in rows if r["name"] in targets}
    if set(selected) != set(targets):
        return Evaluation(None, error="target missing from linked object")
    total, summaries = 0, []
    for name in targets:
        row = selected[name]
        original, candidate = row["_original"][:row["size"]], row["_candidate"]
        width = 4 if functions[row["address"]]["isa"] == "arm" else 2
        left = [original[i:i + width] for i in range(0, len(original), width)]
        right = [candidate[i:i + width] for i in range(0, len(candidate), width)]
        changes, aligned_byte_changes = 0, 0
        for tag, a, b, c, d in difflib.SequenceMatcher(None, left, right, autojunk=False).get_opcodes():
            if tag != "equal":
                changes += max(b - a, d - c)
                x, y = b"".join(left[a:b]), b"".join(right[c:d])
                aligned_byte_changes += sum(p != q for p, q in zip(x, y)) + abs(len(x) - len(y))
        delta = len(candidate) - row["size"]
        byte_changes = sum(p != q for p, q in zip(original, candidate)) + abs(delta)
        score = changes * 16 + aligned_byte_changes + abs(delta) * 8
        if row["status"] != "match":
            score = max(1, score)
        elif score:
            raise PermuteError("oracle match disagrees with byte/extent comparison")
        total += score
        summaries.append({
            "name": name, "score": score, "status": row["status"], "address": row["address"],
            "size": row["size"], "candidate_size": len(candidate), "size_delta": delta,
            "instruction_changes": changes, "aligned_changed_bytes": aligned_byte_changes,
            "changed_bytes": byte_changes, "first_diff": row["first_diff"],
            # Rewrite encodings and original instructions must never enter output records.
            "linker_rewrites": dict(Counter(w["rule"] for w in row.get("linker_rewrites", []))),
        })
    return Evaluation(total, tuple(summaries))


class Oracle:
    def __init__(self, profile: toolchain.Profile, catalogue: Path, targets: tuple[str, ...], suffix: str = ".c"):
        self.profile, self.targets, self.suffix = profile, targets, suffix
        self.context = match.Context(catalogue, MODEL)

    def __repr__(self) -> str:
        return f"Oracle(targets={len(self.targets)})"

    def evaluate(self, text: str, worker_dir: Path) -> Evaluation:
        worker_dir.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(prefix="candidate-", dir=worker_dir) as temporary:
            directory = Path(temporary)
            source, obj = directory / f"candidate{self.suffix}", directory / "candidate.o"
            source.write_text(text, encoding="utf-8", newline="\n")
            saved_cache = toolchain.CACHE
            # compile_source(cache=False) still refreshes the object cache. Keep that
            # disposable cache private, rather than filling the shared author cache.
            toolchain.CACHE = directory / "cache"
            try:
                toolchain.compile_source(self.profile, source, obj, cache=False)
                return score_rows(self.context.compare(obj), self.targets, self.context.functions)
            except (toolchain.ToolchainError, inplace.LinkError, image.ImageError, OSError) as exc:
                return Evaluation(None, error=type(exc).__name__)
            finally:
                toolchain.CACHE = saved_cache


_WORKER_ORACLE: Oracle | None = None


def initialize_worker(profile: toolchain.Profile, catalogue: Path, targets: tuple[str, ...], suffix: str = ".c") -> None:
    global _WORKER_ORACLE
    _WORKER_ORACLE = Oracle(profile, catalogue, targets, suffix)


def evaluate_worker(task: tuple[str, Path]) -> Evaluation:
    text, directory = task
    return _WORKER_ORACLE.evaluate(text, directory)


class Climber:
    def __init__(self, program: Program | cpp.Program, seed: int, baseline: Evaluation):
        self.program, self.rng = program, random.Random(seed)
        self.current = copy.deepcopy(program.ast)
        self.score = baseline.score
        self.best = baseline.score
        self.stale = 0
        self.seen = {hashlib.sha256(program.render(self.current).encode()).digest()}

    def propose(self) -> tuple[c_ast.FileAST | str, str] | None:
        for _ in range(48):
            restart = self.stale >= 32 or self.rng.randrange(24) == 0
            base = self.program.ast if restart else self.current
            candidate = copy.deepcopy(base)
            history = []
            for _ in range(self.rng.randint(1, 3) if restart else 1):
                changed = mutate(self.program, candidate, self.rng)
                if changed is None:
                    break
                candidate, mutation = changed
                history.append(mutation)
            if not history:
                return None
            text = self.program.render(candidate)
            digest = hashlib.sha256(text.encode()).digest()
            if digest in self.seen:
                continue
            self.seen.add(digest)
            if restart:
                self.current, self.score, self.stale = copy.deepcopy(self.program.ast), None, 0
            return candidate, ",".join(history)
        return None

    def observe(self, ast: c_ast.FileAST | str, result: Evaluation) -> bool:
        improved = result.score is not None and (self.best is None or result.score < self.best)
        if improved:
            self.best = result.score
        if result.score is not None and (self.score is None or result.score <= self.score):
            self.current, self.score = ast, result.score
        self.stale = 0 if improved else self.stale + 1
        return improved


def save_candidate(out: Path, stem: str, program: Program | cpp.Program, ast: c_ast.FileAST | str | None,
                   result: Evaluation, details: dict) -> None:
    functions, patch = program.hints(ast)
    text = program.text if ast is None else program.render(ast)
    (out / f"{stem}{program.suffix}").write_text(text, encoding="utf-8", newline="\n")
    (out / f"{stem}.functions{program.suffix}").write_text(functions, encoding="utf-8", newline="\n")
    (out / f"{stem}.diff").write_text(patch, encoding="utf-8", newline="\n")
    record = details | {"score": result.score, "functions": list(result.functions)}
    (out / f"{stem}.json").write_text(json.dumps(record, indent=2, sort_keys=True) + "\n", encoding="utf-8", newline="\n")


def search(program: Program | cpp.Program, profile: toolchain.Profile, out: Path, *, seconds: float,
           workers: int, seed: int, catalogue: Path = image.CATALOGUE,
           max_candidates: int | None = None) -> dict:
    if workers < 1 or seconds <= 0 or max_candidates is not None and max_candidates < 1:
        raise PermuteError("workers, seconds and max-candidates must be positive")
    out = out.resolve()
    if not out.is_relative_to((ROOT / "scratch").resolve()) or out == (ROOT / "scratch").resolve():
        raise PermuteError("output must be a new directory underneath this worktree's scratch/")
    out.mkdir(parents=True, exist_ok=False)
    started = time.monotonic()
    oracle = Oracle(profile, catalogue, program.oracle_targets, program.suffix)
    best_ast = None
    metadata = {
        "seed": seed, "workers": workers, "profile": profile.name, "compiler": profile.compiler,
        "flags": list(profile.flags), "targets": list(program.targets), "linker_model": MODEL,
        "catalogue_sha256": match.sha256(catalogue), "image_sha256": oracle.context.image.sha256,
        "input_sha256": hashlib.sha256(program.text.encode()).hexdigest(),
    }
    if isinstance(program, cpp.Program):
        metadata |= {"language": "c++", "target_symbols": dict(zip(program.targets, program.oracle_targets))}
    evaluated, invalid, improvements, match_seconds = 0, 0, 0, None
    with tempfile.TemporaryDirectory(prefix=".workers-", dir=out) as temporary:
        work = Path(temporary)
        # Verify the author's spelling before normalizing target body formatting.
        original = oracle.evaluate(program.text, work / "baseline")
        evaluated += 1
        if original.score is None:
            raise PermuteError(f"input cannot be compiled and linked: {original.error}")
        best = baseline = original
        save_candidate(out, "initial", program, None, original, metadata | {"mutation": "input"})
        normalized = program.render(program.ast)
        if original.score and normalized != program.text and (max_candidates is None or evaluated < max_candidates) and time.monotonic() - started < seconds:
            baseline = oracle.evaluate(normalized, work / "baseline")
            evaluated += 1
            if baseline.score is None:
                raise PermuteError(f"normalized target cannot be compiled and linked: {baseline.error}")
            if baseline.score < best.score:
                best, best_ast = baseline, copy.deepcopy(program.ast)
                improvements += 1
                save_candidate(out, f"improvement-{improvements:06d}", program, best_ast, best,
                               metadata | {"mutation": "normalize", "candidate": evaluated})
        if best.score == 0:
            match_seconds = time.monotonic() - started
        climbers = [Climber(program, seed + i * 0x9e3779b97f4a7c15, baseline) for i in range(workers)]
        stop = "match" if best.score == 0 else "budget"
        if best.score:
            context = multiprocessing.get_context("spawn")
            with context.Pool(workers, initializer=initialize_worker, initargs=(profile, catalogue, program.oracle_targets, program.suffix)) as pool:
                while time.monotonic() - started < seconds and (max_candidates is None or evaluated < max_candidates):
                    proposals = []
                    for index, climber in enumerate(climbers):
                        if max_candidates is not None and evaluated + len(proposals) >= max_candidates:
                            break
                        proposal = climber.propose()
                        if proposal:
                            ast, mutation = proposal
                            proposals.append((index, ast, mutation))
                    if not proposals:
                        stop = "exhausted"
                        break
                    # Ordered rounds make RNG streams, hill-climbing and tie breaking
                    # independent of process completion order. Time limits alone can
                    # of course end after different numbers of deterministic rounds.
                    results = pool.map(evaluate_worker, [(program.render(ast), work / f"worker-{i:03d}") for i, ast, _ in proposals])
                    evaluated += len(results)
                    for offset, ((index, ast, mutation), result) in enumerate(zip(proposals, results), 1):
                        if result.score is None:
                            invalid += 1
                        worker_better = climbers[index].observe(ast, result)
                        if worker_better:
                            improvements += 1
                            save_candidate(out, f"improvement-{improvements:06d}", program, ast, result,
                                           metadata | {"worker": index, "mutation": mutation, "candidate": evaluated - len(results) + offset})
                        if result.score is not None and result.score < best.score:
                            best, best_ast = result, ast
                        if result.score == 0 and match_seconds is None:
                            match_seconds = time.monotonic() - started
                    if best.score == 0:
                        stop = "match"
                        break
                else:
                    stop = "candidate_limit" if max_candidates is not None and evaluated >= max_candidates else "budget"
        save_candidate(out, "best", program, best_ast, best, metadata)
    elapsed = time.monotonic() - started
    summary = metadata | {
        "score": best.score, "initial_score": original.score, "candidates": evaluated,
        "invalid_candidates": invalid, "improvements": improvements, "elapsed_seconds": elapsed,
        "candidates_per_second": evaluated / elapsed, "time_to_match_seconds": match_seconds,
        "stop": stop, "functions": list(best.functions),
    }
    (out / "summary.json").write_text(json.dumps(summary, indent=2, sort_keys=True) + "\n", encoding="utf-8", newline="\n")
    return summary


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path)
    parser.add_argument("functions", nargs="+", help="target function names (only their bodies change)")
    parser.add_argument("--profile", default="game")
    parser.add_argument("--seconds", type=float, default=180, help="search wall-time budget (an in-flight round finishes)")
    parser.add_argument("--workers", type=int, default=os.cpu_count() or 1)
    parser.add_argument("--seed", type=int, default=0)
    parser.add_argument("--max-candidates", type=int, help="deterministic evaluation cap, including baseline compiles")
    parser.add_argument("--out", type=Path, required=True, help="new output directory under scratch/")
    args = parser.parse_args(argv)
    try:
        profiles = toolchain.load_profiles()
        if args.profile not in profiles:
            raise PermuteError(f"unknown profile: {args.profile}")
        profile = profiles[args.profile]
        targets = tuple(dict.fromkeys(args.functions))
        program = Program.preprocess(args.source.resolve(), targets, profile)
        summary = search(program, profile, args.out, seconds=args.seconds, workers=args.workers,
                         seed=args.seed, max_candidates=args.max_candidates)
    except (PermuteError, interfaces.InterfaceError, toolchain.ToolchainError, inplace.LinkError, image.ImageError, OSError) as exc:
        # Never call match.diff_lines: even errors and private output must be free
        # of ROM bytes, original disassembly, packets and compiler secrets.
        print(f"permuter: {exc}", file=sys.stderr)
        return 2
    status = "MATCH" if summary["score"] == 0 else "BEST"
    print(f"{status} score={summary['score']} initial={summary['initial_score']} "
          f"workers={summary['workers']} candidates={summary['candidates']} "
          f"rate={summary['candidates_per_second']:.2f}/s elapsed={summary['elapsed_seconds']:.3f}s")
    if summary["time_to_match_seconds"] is not None:
        print(f"time_to_match={summary['time_to_match_seconds']:.3f}s")
    print(f"Author hints: {args.out / ('best.functions' + program.suffix)} and {args.out / 'best.diff'}")
    return 0 if summary["score"] == 0 else 1


if __name__ == "__main__":
    raise SystemExit(main())
