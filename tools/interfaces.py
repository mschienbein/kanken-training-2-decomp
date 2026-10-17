"""Audit bound C interfaces using the build compiler's preprocessing environment.

    .venv/bin/python tools/interfaces.py check [--matches FILE | SOURCES...]
    .venv/bin/python tools/interfaces.py check --json scratch/interfaces.json

All input units are preprocessed sequentially with their ledger profile (otherwise
`game`), the toolchain launcher, and exactly compile_source's default include paths.
This tool never compiles/replaces a matching object. C++ units get only a token-level
check that every class/struct/union defined in several units is defined identically (the
one-definition rule); their declarations and const writes are unchecked, and
--allow-unchecked-cpp waives only that limitation, not C parsing/preprocess errors or ODR.

Type comparison is intentionally strict: qualifiers and explicit/unspecified array
bounds are retained, including on parameters; typedefs and integer constant bounds
are resolved. Named aggregates retain tag identity, and reachable complete layouts
are compared recursively, including member names/order, bit widths and packing.
Inline declarations of interfaces defined by another selected unit are warnings
(`policy`), not failures. Only declarations originating in include/ satisfy policy.

Const-write detection covers typed lvalues, casts of addresses of const objects,
and simple intraprocedural pointer aliases. It is not whole-program alias analysis:
heap aliases, pointer arithmetic, interprocedural/branch-sensitive propagation and
writes performed by called functions/assembly are NOT covered. Coverage and all
unchecked units are always included in the JSON and human reports. Unsupported
syntax/extensions fail closed instead of being silently stripped into a PASS.
Documented __svc declarations, compiler intrinsic calls and CP15 named-register
declarations are C; other assembly remains refused. Unit-source INTEGRATOR: marker
comments are refused before preprocessing can erase them.
"""
from __future__ import annotations

import argparse
import ast
import hashlib
import json
import os
import re
import subprocess
import sys
from dataclasses import dataclass, field, replace
from pathlib import Path
from typing import Any

from pycparser import c_ast, c_parser, plyparser

sys.path.insert(0, str(Path(__file__).resolve().parent))
import toolchain  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
BOUND = re.compile(r"(?:func|data|bss)_[0-9a-fA-F]{8}\Z")
COVERAGE = {
    "cpp": "class-definition agreement across units and project headers (ODR, token-level), no forward declaration of a header-defined class name without that header, and no local extern \"C\" redeclaration of a symbol shared with another unit; declaration types and const writes unchecked; --allow-unchecked-cpp waives C++ only",
    "const_writes": "typed lvalues, const-address casts and simple local pointer aliases",
    "not_covered": [
        "whole-program/heap alias analysis and pointer arithmetic",
        "interprocedural or branch-sensitive alias propagation",
        "writes inside called functions or assembly",
    ],
}


class InterfaceError(RuntimeError):
    """An input cannot be checked soundly by this audit."""


@dataclass(frozen=True)
class Unit:
    source: Path
    profile: toolchain.Profile


def display_path(path: Path, root: Path) -> str:
    path = path.resolve()
    return path.relative_to(root).as_posix() if path.is_relative_to(root) else path.as_posix()


def origin_path(name: str, root: Path) -> Path:
    # wibo sometimes emits its Z: mapping in #line paths.
    name = name.replace("\\", "/")
    if name[:3].lower() == "z:/":
        name = name[2:]
    return (root / name).resolve()


def preprocess(unit: Unit, root: Path = ROOT) -> str:
    """Use armcc -E, never a host preprocessor or a hand-written macro expander."""
    exe = toolchain.armcc_exe(unit.profile.compiler)
    if not exe.is_file():
        raise InterfaceError(f"{exe} missing; run tools/setup.py")
    launcher = toolchain.wibo_path()
    argv = [str(launcher)] if launcher else []
    argv += [str(exe), "-E", *unit.profile.flags]
    for inc in (root / "include", toolchain.compiler_root(unit.profile.compiler) / "include"):
        argv += ["-I", str(inc.resolve())]
    argv.append(str(unit.source.resolve()))
    try:
        proc = subprocess.run(
            argv, cwd=root, capture_output=True, text=True, encoding="utf-8",
            errors="replace", timeout=300, env=os.environ | {"LC_ALL": "C"},
        )
    except (OSError, subprocess.TimeoutExpired) as exc:
        raise InterfaceError(f"ARMCC preprocessing failed: {exc}") from exc
    if proc.returncode:
        raise InterfaceError(f"ARMCC -E exited {proc.returncode}: {proc.stdout}{proc.stderr}")
    return proc.stdout


# Token positions associate ABI annotations with the owning declaration. Expansion
# of __int64 preserves lines; all subsequent offsets refer to the expanded text.
TOKEN = re.compile(
    r"/\*.*?\*/|//[^\n]*|(?:u8|[LuU])?\"(?:\\.|[^\"\\])*\"|"
    r"(?:[LuU])?'(?:\\.|[^'\\])*'|[A-Za-z_]\w*|(?:0[xX][0-9a-fA-F]+|\d+)\w*|[^\s]",
    re.DOTALL,
)
LINE = re.compile(r'^\s*#\s*(?:line\s+)?(\d+)\s+"((?:\\.|[^"\\])*)"')
HEADER_PRAGMA = re.compile(r"^\s*#\s*pragma\s+(?:push|pop|anon_unions|__printf_args|__scanf_args)\s*$")
IGNORED_WORDS = {
    "__inline", "__inline__", "__forceinline", "__weak", "__pure",
    "__nothrow", "__noreturn",
}
REPLACEMENTS = {
    "__const": "const", "__const__": "const", "__volatile": "volatile",
    "__volatile__": "volatile", "__restrict": "restrict", "__restrict__": "restrict",
    "__signed": "signed", "__signed__": "signed",
}
IGNORED_ATTRIBUTES = {
    "unused", "used", "weak", "noinline", "always_inline", "noreturn", "section",
    "visibility", "deprecated", "pure", "const", "notshared", "nonnull",
    "warn_unused_result", "format", "malloc", "alias", "constructor", "destructor",
    "nothrow",
}
TYPE_ATTRIBUTES = {"packed", "aligned", "pcs", "vector_size", "mode"}


@dataclass
class Annotation:
    start: int
    end: int
    value: str
    declarator: int | None = None


@dataclass
class Prepared:
    text: str
    locations: list[tuple[str, int]]
    offsets: list[int]
    annotations: list[Annotation]

    def offset(self, node: c_ast.Node) -> int:
        return self.offsets[node.coord.line - 1] + (node.coord.column or 1) - 1

    def site(self, node: c_ast.Node, root: Path) -> dict:
        name, line = self.locations[node.coord.line - 1]
        return {"source": display_path(origin_path(name, root), root), "line": line}


def prepare(text: str, source: Path) -> Prepared:
    locations = []
    lines = []
    filename, lineno = str(source.resolve()), 1
    for raw in text.splitlines(keepends=True):
        marker = LINE.match(raw)
        locations.append((filename, lineno))
        if marker:
            filename = marker[2].replace("\\\\", "\\").replace('\\"', '"')
            lineno = int(marker[1])
            lines.append("".join("\n" if c == "\n" else " " for c in raw))
        elif raw.lstrip().startswith("#"):
            # These standard-header pragmas only permit anonymous unions or add
            # format diagnostics. Their layouts are already explicit in the AST.
            directive = re.sub(r"/\*.*?\*/|//[^\n]*", "", raw)
            if not HEADER_PRAGMA.fullmatch(directive):
                # In particular, #pragma pack must still fail closed.
                raise InterfaceError(f"unsupported preprocessing directive: {raw.strip()}")
            lines.append("".join("\n" if c == "\n" else " " for c in raw))
            lineno += 1
        else:
            lines.append(raw)
            lineno += 1
    plain = TOKEN.sub(lambda m: "long long" if m[0] == "__int64" else m[0], "".join(lines))
    chars = list(plain)
    tokens = list(TOKEN.finditer(plain))
    annotations = []

    def blank(start: int, end: int) -> None:
        chars[start:end] = ["\n" if c == "\n" else " " for c in plain[start:end]]

    def closing(index: int) -> int:
        depth = 0
        for j in range(index, len(tokens)):
            value = tokens[j][0]
            if value == "(":
                depth += 1
            elif value == ")":
                depth -= 1
                if not depth:
                    return j
        raise InterfaceError("unbalanced ARMCC annotation")

    i = 0
    while i < len(tokens):
        token = tokens[i]
        word = token[0]
        if word.startswith(("/*", "//")):
            blank(token.start(), token.end())
        elif word in IGNORED_WORDS:
            blank(token.start(), token.end())
        elif word in REPLACEMENTS:
            replacement = REPLACEMENTS[word]
            chars[token.start():token.end()] = list(replacement.ljust(len(word)))
        elif word in ("__packed", "__value_in_regs"):
            annotations.append(Annotation(token.start(), token.end(), word.strip("_")))
            blank(token.start(), token.end())
        elif word == "__softfp":
            annotations.append(Annotation(token.start(), token.end(), 'pcs("aapcs")'))
            blank(token.start(), token.end())
        elif word in ("__svc", "__svc_indirect"):
            if i + 1 >= len(tokens) or tokens[i + 1][0] != "(":
                raise InterfaceError(f"malformed {word}")
            end = closing(i + 1)
            args = "".join(t[0] for t in tokens[i + 1:end + 1])
            annotations.append(Annotation(token.start(), tokens[end].end(), word.strip("_") + args))
            blank(token.start(), tokens[end].end())
            i = end
        elif word in ("__attribute__", "__attribute", "__declspec"):
            if i + 1 >= len(tokens) or tokens[i + 1][0] != "(":
                raise InterfaceError(f"malformed {word}")
            end = closing(i + 1)
            j = i + 2
            while j < end:
                value = tokens[j][0]
                if value in ("(", ")", ","):
                    j += 1
                    continue
                name = value.strip("_")
                arg_end = closing(j + 1) if j + 1 < end and tokens[j + 1][0] == "(" else j
                if name in TYPE_ATTRIBUTES or name in ("value_in_regs", "align"):
                    args = "".join(t[0] for t in tokens[j + 1:arg_end + 1])
                    annotations.append(Annotation(token.start(), tokens[end].end(), name + args))
                elif name not in IGNORED_ATTRIBUTES:
                    raise InterfaceError(f"unsupported ARMCC attribute {value!r}")
                j = arg_end + 1
            blank(token.start(), tokens[end].end())
            i = end
        elif word in ("__asm", "__asm__", "asm"):
            # Only ARMCC's coprocessor named-register *declaration* is C here.
            # Core-register bindings can steer allocation; assembly bodies/labels
            # cannot be erased without concealing interfaces and memory writes.
            if word != "__asm" or i + 1 >= len(tokens) or tokens[i + 1][0] != "(":
                raise InterfaceError("__asm is not covered by the C interface parser")
            end = closing(i + 1)
            args = [t for t in tokens[i + 2:end] if not t[0].startswith(("/*", "//"))]
            previous = i - 1
            while previous >= 0 and tokens[previous][0].startswith(("/*", "//")):
                previous -= 1
            if (
                len(args) != 1 or not re.fullmatch(r'"cp15:[^"\\]*"', args[0][0])
                or previous < 0 or not re.fullmatch(r"[A-Za-z_]\w*", tokens[previous][0])
            ):
                raise InterfaceError("__asm is not a CP15 named-register declaration")
            annotations.append(Annotation(
                token.start(), tokens[end].end(), "named_register(" + args[0][0] + ")",
                tokens[previous].start(),
            ))
            blank(token.start(), tokens[end].end())
            i = end
        i += 1
    offsets, cursor = [], 0
    for raw in plain.splitlines(keepends=True):
        offsets.append(cursor)
        cursor += len(raw)
    return Prepared("".join(chars), locations, offsets, annotations)


@dataclass(frozen=True)
class CType:
    kind: str
    args: tuple[Any, ...] = ()
    quals: tuple[str, ...] = ()

    def qualified(self, quals: list[str] | tuple[str, ...]) -> CType:
        return replace(self, quals=tuple(sorted(set(self.quals) | set(quals))))


@dataclass(eq=False)
class Record:
    kind: str
    name: str | None
    node: c_ast.Node
    members: list[tuple[str | None, CType, int | None]] | None = None
    attrs: tuple[str, ...] = ()


@dataclass
class Scope:
    parent: Scope | None = None
    typedefs: dict[str, CType] = field(default_factory=dict)
    tags: dict[tuple[str, str], Record] = field(default_factory=dict)
    variables: dict[str, CType] = field(default_factory=dict)
    aliases: dict[str, str | None] = field(default_factory=dict)
    constants: dict[str, int] = field(default_factory=dict)
    internal: set[str] = field(default_factory=set)

    def lookup(self, table: str, key: Any) -> Any:
        scope: Scope | None = self
        while scope:
            values = getattr(scope, table)
            if key in values:
                return values[key]
            scope = scope.parent
        return None


@dataclass
class Declaration:
    name: str
    type: CType
    node: c_ast.Node
    definition: bool
    unit: str
    site: dict


def primitive(words: list[str]) -> str:
    if set(words) <= {"signed", "unsigned", "short", "long", "int", "char"}:
        sign = "unsigned " if "unsigned" in words else ""
        if "char" in words:
            return sign + ("signed " if "signed" in words else "") + "char"
        size = "short" if "short" in words else "long long" if words.count("long") == 2 else "long" if "long" in words else "int"
        return sign + size
    return " ".join(sorted(words))


def type_key(t: CType, visiting: frozenset[int] = frozenset()) -> tuple:
    if t.kind == "record":
        record = t.args[0]
        if record.name is not None:
            args = (record.kind, record.name)
        elif id(record) in visiting:
            args = (record.kind, "<recursive anonymous>")
        else:
            args = (record.kind, layout_key(record, visiting | {id(record)}))
    elif t.kind == "function":
        ret, params, variadic = t.args
        args = (type_key(ret, visiting), None if params is None else tuple(type_key(p, visiting) for p in params), variadic)
    else:
        args = tuple(type_key(a, visiting) if isinstance(a, CType) else a for a in t.args)
    return (t.kind, t.quals, args)


def layout_key(record: Record, visiting: frozenset[int] = frozenset()) -> tuple:
    return (record.attrs, None if record.members is None else tuple(
        (name, type_key(t, visiting), bits) for name, t, bits in record.members
    ))


def describe(t: CType) -> str:
    quals = " ".join(t.quals)
    if t.kind == "base":
        body = t.args[0]
    elif t.kind == "record":
        r = t.args[0]
        body = f"{r.kind} {r.name or '<anonymous>'}"
        if r.name is None and r.members is not None:
            body += " { " + "; ".join(f"{describe(mt)} {name or '<anonymous>'}" for name, mt, _ in r.members) + " }"
    elif t.kind == "pointer":
        body = f"pointer to ({describe(t.args[0])})"
    elif t.kind == "array":
        body = f"array[{'' if t.args[1] is None else t.args[1]}] of ({describe(t.args[0])})"
    else:
        ret, params, variadic = t.args
        args = "unspecified" if params is None else ", ".join(describe(p) for p in params) or "void"
        if variadic:
            args += ", ..."
        body = f"function({args}) -> {describe(ret)}"
    return f"{quals} {body}".strip()


class Analyzer(c_ast.NodeVisitor):
    def __init__(self, prepared: Prepared, source: Path, root: Path, names: set[str]):
        self.prepared, self.root = prepared, root
        self.unit = display_path(source, root)
        self.names = names
        self.scope = Scope()
        self.declarations: list[Declaration] = []
        self.writes: list[dict] = []
        self.attributes: dict[int, tuple[str, ...]] = {}
        self.types: dict[int, CType] = {}
        self.records: list[Record] = []

    def bound(self, name: str | None) -> bool:
        return name is not None and (BOUND.fullmatch(name) is not None or name in self.names)

    def bind_annotations(self, tree: c_ast.FileAST) -> None:
        nodes = []
        parents = []
        variable_declarations = set()

        class Index(c_ast.NodeVisitor):
            def generic_visit(_, node):
                if isinstance(node, (c_ast.Decl, c_ast.Typedef, c_ast.Struct, c_ast.Union, c_ast.PtrDecl)) and node.coord:
                    nodes.append(node)
                if isinstance(node, c_ast.Decl) and parents and isinstance(parents[-1], (c_ast.FileAST, c_ast.Compound, c_ast.DeclList)):
                    variable_declarations.add(id(node))
                parents.append(node)
                super().generic_visit(node)
                parents.pop()

        Index().visit(tree)
        for annotation in self.prepared.annotations:
            if annotation.declarator is not None:
                candidates = [
                    node for node in nodes
                    if isinstance(node, c_ast.Decl)
                    and self.prepared.offset(node) == annotation.declarator
                ]
                if len(candidates) != 1 or "register" not in candidates[0].storage:
                    raise InterfaceError("CP15 named-register declaration requires a register variable")
                node = candidates[0]
                if id(node) not in variable_declarations:
                    raise InterfaceError("CP15 named-register variable must have file or block scope")
                if not isinstance(node.type, c_ast.TypeDecl) or node.init is not None or node.bitsize is not None:
                    raise InterfaceError("CP15 named-register declaration requires an uninitialized scalar variable")
                self.attributes[id(node)] = (*self.attributes.get(id(node), ()), annotation.value)
                continue
            candidates = []
            for node in nodes:
                pos = self.prepared.offset(node)
                lo, hi = sorted((pos, annotation.start))
                between = self.prepared.text[lo:hi]
                is_record = (
                    isinstance(node, (c_ast.Struct, c_ast.Union)) and node.decls is not None
                    and annotation.value.split("(")[0] in {"packed", "aligned", "align"}
                )
                # A record annotation may precede its keyword, sit after its tag,
                # or follow its closing brace. Prefer the owning aggregate over
                # member declarations when braces are involved.
                if is_record:
                    tokens = list(TOKEN.finditer(self.prepared.text, pos))
                    depth, end = 0, pos
                    for token in tokens:
                        if token[0] == "{":
                            depth += 1
                        elif token[0] == "}":
                            depth -= 1
                            if depth == 0:
                                end = token.end()
                                break
                    in_prefix = annotation.end <= pos and not any(c in between for c in ";{}")
                    in_tag = pos <= annotation.start and not any(c in between for c in ";{")
                    postfix = end <= annotation.start and not self.prepared.text[end:annotation.start].strip()
                    if in_prefix or in_tag or postfix:
                        candidates.append((0, abs(pos - annotation.start), node))
                elif isinstance(node, (c_ast.Decl, c_ast.Typedef, c_ast.PtrDecl)) and not any(c in between for c in ";{}"):
                    # Prefix attributes belong to the following declarator;
                    # postfix ones to the preceding declarator in this statement.
                    score = 1 if pos >= annotation.end else 2
                    if isinstance(node, c_ast.PtrDecl) and self.prepared.text[pos:annotation.start].strip() == "*":
                        score = 0
                    if (
                        isinstance(node, c_ast.Decl) and isinstance(node.type, c_ast.FuncDecl)
                        and annotation.value.split("(")[0] in {"pcs", "svc", "svc_indirect", "value_in_regs"}
                    ):
                        # An ABI attribute after f(arguments) belongs to f, not
                        # the last parameter declaration inside those parentheses.
                        depth = 0
                        for token in TOKEN.finditer(self.prepared.text, pos):
                            if token[0] == "(":
                                depth += 1
                            elif token[0] == ")":
                                depth -= 1
                                if depth == 0:
                                    if token.end() <= annotation.start and not self.prepared.text[token.end():annotation.start].strip():
                                        score = 0
                                    break
                    candidates.append((score, abs(pos - annotation.start), node))
            if not candidates:
                raise InterfaceError(f"cannot associate ARMCC annotation {annotation.value!r} with a declaration")
            node = min(candidates, key=lambda x: x[:2])[2]
            self.attributes[id(node)] = tuple(sorted((*self.attributes.get(id(node), ()), annotation.value)))

    def constant(self, node: c_ast.Node) -> int:
        if isinstance(node, c_ast.Constant):
            value = node.value
            if node.type == "char":
                literal = ast.literal_eval(re.sub(r"^[LuU]", "", value))
                if not isinstance(literal, str) or len(literal) != 1:
                    raise InterfaceError("multi-character integer literals are compiler-specific")
                return ord(literal)
            value = re.sub(r"[uUlL]+$", "", value)
            if re.fullmatch(r"0[0-7]+", value):
                return int(value, 8)
            return int(value, 0)
        if isinstance(node, c_ast.ID):
            value = self.scope.lookup("constants", node.name)
            if value is not None:
                return value
        elif isinstance(node, c_ast.UnaryOp) and node.op in ("+", "-", "~", "!"):
            value = self.constant(node.expr)
            return {"+": lambda: value, "-": lambda: -value, "~": lambda: ~value, "!": lambda: int(not value)}[node.op]()
        elif isinstance(node, c_ast.BinaryOp):
            left, right = self.constant(node.left), self.constant(node.right)
            # C integer division truncates toward zero, without float rounding.
            def quotient() -> int:
                return (abs(left) // abs(right)) * (-1 if (left < 0) != (right < 0) else 1)

            operations = {
                "+": lambda: left + right, "-": lambda: left - right, "*": lambda: left * right,
                "/": quotient, "%": lambda: left - quotient() * right,
                "<<": lambda: left << right, ">>": lambda: left >> right,
                "&": lambda: left & right, "|": lambda: left | right, "^": lambda: left ^ right,
                "==": lambda: int(left == right), "!=": lambda: int(left != right),
                "<": lambda: int(left < right), ">": lambda: int(left > right),
                "<=": lambda: int(left <= right), ">=": lambda: int(left >= right),
                "&&": lambda: int(bool(left) and bool(right)), "||": lambda: int(bool(left) or bool(right)),
            }
            if node.op in operations:
                return operations[node.op]()
        elif isinstance(node, c_ast.TernaryOp):
            return self.constant(node.iftrue if self.constant(node.cond) else node.iffalse)
        raise InterfaceError(f"non-constant or unsupported array/enum/bit-field expression at {node.coord}")

    def ctype(self, node: c_ast.Node) -> CType:
        if id(node) in self.types:
            return self.types[id(node)]
        quals = getattr(node, "quals", []) or []
        attrs = self.attributes.get(id(node), ())
        if isinstance(node, (c_ast.Decl, c_ast.Typedef, c_ast.Typename)):
            # These nodes duplicate the base qualifiers from TypeDecl. Applying
            # them here would incorrectly turn `const int *p` into a const pointer.
            result = self.ctype(node.type)
        elif isinstance(node, c_ast.TypeDecl):
            result = self.ctype(node.type).qualified(quals)
        elif isinstance(node, c_ast.IdentifierType):
            alias = self.scope.lookup("typedefs", node.names[0]) if len(node.names) == 1 else None
            if alias is None and any(w not in {"void", "char", "short", "int", "long", "float", "double", "signed", "unsigned", "_Bool"} for w in node.names):
                raise InterfaceError(f"unresolved typedef {' '.join(node.names)!r}")
            result = alias if alias is not None else CType("base", (primitive(node.names),))
        elif isinstance(node, c_ast.PtrDecl):
            result = CType("pointer", (self.ctype(node.type),), tuple(sorted(quals)))
        elif isinstance(node, c_ast.ArrayDecl):
            result = CType("array", (self.ctype(node.type), None if node.dim is None else self.constant(node.dim)), tuple(sorted(node.dim_quals or [])))
        elif isinstance(node, c_ast.FuncDecl):
            if node.args is None:
                params, variadic = None, False
            else:
                params = tuple(self.ctype(p) for p in node.args.params if not isinstance(p, c_ast.EllipsisParam))
                variadic = any(isinstance(p, c_ast.EllipsisParam) for p in node.args.params)
                if params == (CType("base", ("void",)),):
                    params = ()
            result = CType("function", (self.ctype(node.type), params, variadic))
        elif isinstance(node, (c_ast.Struct, c_ast.Union, c_ast.Enum)):
            kind = type(node).__name__.lower()
            members = node.values if isinstance(node, c_ast.Enum) else node.decls
            key = (kind, node.name)
            # A definition introduces the current scope's tag; an incomplete use
            # resolves an enclosing tag. Records remain mutable for forward refs.
            record = self.scope.tags.get(key) if members is not None else self.scope.lookup("tags", key)
            if record is None or node.name is None:
                record = Record(kind, node.name, node)
                self.records.append(record)
                if node.name:
                    self.scope.tags[key] = record
            result = CType("record", (record,))
            self.types[id(node)] = result
            if members is not None:
                if record.members is not None:
                    raise InterfaceError(f"duplicate complete {kind} {node.name} in one C scope")
                record.node, record.attrs = node, attrs
                if isinstance(node, c_ast.Enum):
                    value, fields = -1, []
                    for enumerator in members.enumerators:
                        value = self.constant(enumerator.value) if enumerator.value else value + 1
                        self.scope.constants[enumerator.name] = value
                        fields.append((enumerator.name, CType("base", ("int",)), value))
                    record.members = fields
                else:
                    record.members = [(member.name, self.ctype(member), None if member.bitsize is None else self.constant(member.bitsize)) for member in members]
            return result
        else:
            raise InterfaceError(f"unsupported type node {type(node).__name__} at {node.coord}")
        for alignment in getattr(node, "align", None) or []:
            value = alignment.alignment
            marker = (
                f"alignof({type_key(self.ctype(value))})"
                if isinstance(value, c_ast.Typename) else f"aligned({self.constant(value)})"
            )
            result = result.qualified((marker,))
        result = result.qualified(attrs)
        self.types[id(node)] = result
        return result

    def declaration(self, node: c_ast.Decl, definition: bool = False) -> None:
        t = self.ctype(node)
        if t.kind == "array" and t.args[1] is None and node.init is not None:
            if isinstance(node.init, c_ast.Constant) and node.init.type == "string":
                value = ast.literal_eval(re.sub(r"^[LuU]", "", node.init.value))
                bound = len(value) + 1
            elif isinstance(node.init, c_ast.InitList):
                bound, cursor = 0, 0
                for item in node.init.exprs:
                    if isinstance(item, c_ast.NamedInitializer):
                        cursor = self.constant(item.name[0])
                    bound = max(bound, cursor + 1)
                    cursor += 1
            else:
                raise InterfaceError(f"cannot infer array initializer bound at {node.coord}")
            t = replace(t, args=(t.args[0], bound))
            self.types[id(node)] = t
        if node.name:
            if "static" in node.storage:
                self.scope.internal.add(node.name)
            self.scope.variables[node.name] = t
            if t.kind == "pointer":
                self.scope.aliases[node.name] = self.pointer_origin(node.init) if node.init else None
        internal = any(node.name in s.internal for s in self.scopes())
        external = self.scope.parent is None or "extern" in node.storage or t.kind == "function"
        if self.bound(node.name) and external and not internal:
            is_definition = definition or (t.kind != "function" and self.scope.parent is None and ("extern" not in node.storage or node.init is not None))
            self.declarations.append(Declaration(node.name, t, node, is_definition, self.unit, self.prepared.site(node, self.root)))
        if node.init:
            self.visit(node.init)

    def scopes(self):
        scope = self.scope
        while scope:
            yield scope
            scope = scope.parent

    def visit_Decl(self, node):
        self.declaration(node)

    def visit_Typedef(self, node):
        self.scope.typedefs[node.name] = self.ctype(node)

    def visit_FuncDef(self, node):
        if node.param_decls:
            raise InterfaceError("old-style K&R function definition is not covered")
        self.declaration(node.decl, definition=True)
        previous = self.scope
        self.scope = Scope(previous)
        args = node.decl.type.args
        if args:
            for param in args.params:
                if isinstance(param, c_ast.Decl) and param.name:
                    t = self.ctype(param)
                    if t.kind == "array":
                        t = CType("pointer", (t.args[0],), t.quals)
                    elif t.kind == "function":
                        t = CType("pointer", (t,))
                    self.scope.variables[param.name] = t
        self.visit(node.body)
        self.scope = previous

    def visit_Compound(self, node):
        previous = self.scope
        self.scope = Scope(previous)
        for item in node.block_items or []:
            self.visit(item)
        self.scope = previous

    def visit_For(self, node):
        previous = self.scope
        self.scope = Scope(previous)
        self.generic_visit(node)
        self.scope = previous

    def expr_type(self, node: c_ast.Node | None) -> CType | None:
        if isinstance(node, c_ast.ID):
            return self.scope.lookup("variables", node.name)
        if isinstance(node, c_ast.Cast):
            return self.ctype(node.to_type)
        if isinstance(node, c_ast.UnaryOp):
            t = self.expr_type(node.expr)
            if t and node.op == "&":
                return CType("pointer", (t,))
            if t and node.op == "*" and t.kind in ("pointer", "array"):
                return t.args[0]
        if isinstance(node, c_ast.ArrayRef):
            t = self.expr_type(node.name)
            return t.args[0] if t and t.kind in ("array", "pointer") else None
        if isinstance(node, c_ast.StructRef):
            t = self.expr_type(node.name)
            if t and node.type == "->" and t.kind == "pointer":
                t = t.args[0]
            if t and t.kind == "record":
                for name, member, _ in t.args[0].members or []:
                    if name == node.field.name:
                        return member.qualified(t.quals)
        return None

    def is_const(self, t: CType | None) -> bool:
        return bool(t and ("const" in t.quals or t.kind == "array" and self.is_const(t.args[0])))

    def pointer_origin(self, node: c_ast.Node | None) -> str | None:
        if isinstance(node, c_ast.Cast):
            return self.pointer_origin(node.expr)
        if isinstance(node, c_ast.ID):
            t = self.expr_type(node)
            if t and t.kind == "array" and self.is_const(t):
                return node.name
            return self.scope.lookup("aliases", node.name)
        if isinstance(node, c_ast.UnaryOp) and node.op == "&":
            return self.const_origin(node.expr)
        return None

    def const_origin(self, node: c_ast.Node) -> str | None:
        if isinstance(node, c_ast.ID):
            return node.name if self.is_const(self.expr_type(node)) else None
        if isinstance(node, c_ast.UnaryOp) and node.op == "*":
            origin = self.pointer_origin(node.expr)
            return origin or (self.root_name(node.expr) if self.is_const(self.expr_type(node)) else None)
        if isinstance(node, c_ast.ArrayRef):
            return self.pointer_origin(node.name) or self.const_origin(node.name) or (self.root_name(node.name) if self.is_const(self.expr_type(node)) else None)
        if isinstance(node, c_ast.StructRef):
            origin = self.pointer_origin(node.name) if node.type == "->" else self.const_origin(node.name)
            return origin or (self.root_name(node.name) if self.is_const(self.expr_type(node)) else None)
        return None

    def root_name(self, node: c_ast.Node) -> str:
        if isinstance(node, c_ast.ID):
            return node.name
        for _, child in node.children():
            name = self.root_name(child)
            if name != "<const object>":
                return name
        return "<const object>"

    def write(self, node: c_ast.Node) -> None:
        origin = self.const_origin(node)
        if origin:
            t = self.expr_type(node)
            site = self.prepared.site(node, self.root)
            site["type"] = f"write to const object {origin}: {describe(t) if t else 'cast/alias'}"
            self.writes.append({"name": origin, "kind": "const-write", "sites": [site]})

    def visit_Assignment(self, node):
        self.write(node.lvalue)
        self.visit(node.lvalue)
        self.visit(node.rvalue)
        if isinstance(node.lvalue, c_ast.ID) and node.op == "=":
            for scope in self.scopes():
                if node.lvalue.name in scope.variables:
                    scope.aliases[node.lvalue.name] = self.pointer_origin(node.rvalue)
                    break

    def visit_UnaryOp(self, node):
        if node.op in ("p++", "p--", "++", "--"):
            self.write(node.expr)
        self.generic_visit(node)


def reachable_records(t: CType, seen: set[int] | None = None) -> list[Record]:
    seen = seen if seen is not None else set()
    found = []
    if t.kind == "record":
        record = t.args[0]
        if id(record) not in seen:
            seen.add(id(record))
            found.append(record)
            for _, member, _ in record.members or []:
                found.extend(reachable_records(member, seen))
    elif t.kind == "function":
        found.extend(reachable_records(t.args[0], seen))
        for param in t.args[1] or ():
            found.extend(reachable_records(param, seen))
    else:
        for arg in t.args:
            if isinstance(arg, CType):
                found.extend(reachable_records(arg, seen))
    return found


def unique_sites(sites: list[dict]) -> list[dict]:
    return [json.loads(s) for s in sorted({json.dumps(site, sort_keys=True) for site in sites})]


CPP_CLASS = re.compile(r"^[ \t]*(?:struct|class|union)[ \t]+(\w+)[^;{]*\{", re.M)
CPP_COMMENT = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)


def cpp_class_definitions(text: str) -> list[tuple[str, str, int]]:
    """(name, token-normalised definition, line) for each class/struct/union definition in C++
    source text. Comments are blanked (keeping line numbers) and whitespace collapsed, so two
    definitions compare equal exactly when their token sequences agree, as the one-definition rule
    requires of a class defined in several translation units."""
    text = CPP_COMMENT.sub(lambda m: re.sub(r"[^\n]", " ", m.group()), text)
    found = []
    for m in CPP_CLASS.finditer(text):
        depth, i = 0, m.end() - 1
        while i < len(text):
            depth += {"{": 1, "}": -1}.get(text[i], 0)
            i += 1
            if depth == 0:
                break
        body = " ".join(text[m.start():i].split())
        found.append((m.group(1), body, text.count("\n", 0, m.start()) + 1))
    return found


CPP_EXTERN_C = re.compile(r'extern\s+"C"\s+[^;{}]*?\b((?:func|data|bss)_[0-9a-fA-F]{8})\b[^;{}]*;')


def cpp_extern_declarations(text: str) -> list[tuple[str, int]]:
    """(name, line) for each address-named `extern "C"` declaration written in C++ source text
    (definitions, which end in a body, are not declarations). Comments are ignored."""
    text = CPP_COMMENT.sub(lambda m: re.sub(r"[^\n]", " ", m.group()), text)
    return [(m.group(1), text.count("\n", 0, m.start()) + 1) for m in CPP_EXTERN_C.finditer(text)]


CPP_FORWARD = re.compile(r"^[ \t]*(?:struct|class|union)[ \t]+(\w+)[ \t]*;", re.M)
PROJECT_INCLUDE = re.compile(r'^[ \t]*#[ \t]*include[ \t]*"([^"]+)"', re.M)


def project_headers(root: Path) -> dict[str, str]:
    """Text of every project header (include/ plus header files under src/), keyed by the path
    used in `#include "..."` for include/ files (e.g. "game/screen.h") or by its src path."""
    found = {}
    for path in sorted((root / "include").rglob("*")) if (root / "include").is_dir() else []:
        if path.is_file():
            found[path.relative_to(root / "include").as_posix()] = path.read_text(encoding="utf-8")
    for path in sorted((root / "src").rglob("*")) if (root / "src").is_dir() else []:
        if path.is_file() and path.suffix in (".h", ".hh", ".hpp", ".inc"):
            found[path.relative_to(root).as_posix()] = path.read_text(encoding="utf-8")
    return found


def included_headers(text: str, headers: dict[str, str]) -> set[str]:
    """Project headers reached from `text` by quoted includes (transitively)."""
    seen, pending = set(), list(PROJECT_INCLUDE.findall(text))
    while pending:
        name = pending.pop()
        if name in headers and name not in seen:
            seen.add(name)
            pending.extend(PROJECT_INCLUDE.findall(headers[name]))
    return seen


def audit(units: list[Unit], root: Path = ROOT, names: set[str] | None = None) -> dict:
    root = root.resolve()
    if names is None:
        overlay = root / "config/names.json"
        names = set(json.loads(overlay.read_text(encoding="utf-8"))["symbols"]) if overlay.is_file() else set()
    report = {"schema_version": 1, "units": [], "disagreements": [], "policy": [], "unchecked": [], "coverage": COVERAGE}
    declarations: dict[str, list[Declaration]] = {}
    records: dict[tuple[str, str], list[tuple[Record, Analyzer]]] = {}
    cpp_classes: dict[str, list[tuple[str, dict]]] = {}
    cpp_externs: dict[str, list[dict]] = {}
    # Class/struct/union names are one project-wide namespace: every definition in a project header
    # takes part in the ODR comparison, and a C++ unit may not forward-declare a header-defined
    # name without including that header (two different objects sharing a name).
    headers = project_headers(root)
    header_defined: dict[str, list[tuple[str, dict]]] = {}
    for header, text in headers.items():
        display = header if header.startswith("src/") else f"include/{header}"
        for name, body, line in cpp_class_definitions(text):
            site = {"source": display, "line": line, "type": "class definition (header)"}
            header_defined.setdefault(name, []).append((header, site))
            cpp_classes.setdefault(name, []).append((body, site))
    for unit in units:  # Intentionally sequential: armcc runs on a shared laptop.
        source = display_path(unit.source, root)
        entry = {"source": source, "sha256": None, "checked": False}
        report["units"].append(entry)
        try:
            entry["sha256"] = hashlib.sha256(unit.source.read_bytes()).hexdigest()
            text = unit.source.read_text(encoding="utf-8")
            for token in TOKEN.finditer(text):
                if token[0].startswith(("/*", "//")) and re.search(r"\bINTEGRATOR:", token[0]):
                    line = text.count("\n", 0, token.start()) + 1
                    raise InterfaceError(f"leftover INTEGRATOR: marker comment at line {line}")
            if unit.source.suffix.lower() == ".cpp":
                for name, body, line in cpp_class_definitions(text):
                    cpp_classes.setdefault(name, []).append((body, {"source": source, "line": line, "type": "class definition"}))
                reached = included_headers(text, headers)
                stripped = CPP_COMMENT.sub(lambda m: re.sub(r"[^\n]", " ", m.group()), text)
                for m in CPP_FORWARD.finditer(stripped):
                    defining = header_defined.get(m.group(1), [])
                    if defining and not any(header in reached for header, _ in defining):
                        site = {"source": source, "line": stripped.count("\n", 0, m.start()) + 1, "type": "forward declaration without the defining header"}
                        report["disagreements"].append({"name": f"class {m.group(1)}", "kind": "type-name",
                                                        "sites": unique_sites([site] + [s for _, s in defining])})
                for name, line in cpp_extern_declarations(text):
                    cpp_externs.setdefault(name, []).append({"source": source, "line": line, "type": 'local extern "C" declaration'})
                report["unchecked"].append({"source": source, "kind": "cpp", "reason": "C++ parser unavailable; only class-definition (ODR) agreement checked"})
                continue
            if unit.source.suffix.lower() != ".c":
                raise InterfaceError("expected a .c or .cpp translation unit")
            prepared = prepare(preprocess(unit, root), unit.source)
            tree = c_parser.CParser().parse(prepared.text, filename=source)
            analyzer = Analyzer(prepared, unit.source, root, names)
            analyzer.bind_annotations(tree)
            analyzer.visit(tree)
            for decl in analyzer.declarations:
                declarations.setdefault(decl.name, []).append(decl)
                for record in reachable_records(decl.type):
                    if record.name and record.members is not None:
                        records.setdefault((record.kind, record.name), []).append((record, analyzer))
            report["disagreements"].extend(analyzer.writes)
            entry["checked"] = True
        except (InterfaceError, toolchain.ToolchainError, plyparser.ParseError, OSError, ValueError, ZeroDivisionError) as exc:
            report["unchecked"].append({"source": source, "kind": "c", "reason": str(exc)})
    for name, decls in sorted(declarations.items()):
        if len({type_key(d.type) for d in decls}) > 1:
            report["disagreements"].append({"name": name, "kind": "declaration", "sites": unique_sites([dict(d.site, type=describe(d.type)) for d in decls])})
        definitions = {d.unit for d in decls if d.definition}
        bad_sites = []
        for decl in decls:
            origin = origin_path(decl.site["source"], root)
            if not decl.definition and definitions - {decl.unit} and not origin.is_relative_to(root / "include"):
                bad_sites.append(dict(decl.site, type=describe(decl.type)))
        if bad_sites:
            report["policy"].append({"name": name, "kind": "inline-declaration", "level": "policy", "sites": unique_sites(bad_sites)})
    for (kind, name), entries in sorted(records.items()):
        if len({layout_key(record) for record, _ in entries}) > 1:
            sites = []
            for record, analyzer in entries:
                site = analyzer.prepared.site(record.node, root)
                members = "; ".join(f"{describe(t)} {member or '<anonymous>'}" + (f" : {bits}" if bits is not None else "") for member, t, bits in record.members or [])
                site["type"] = f"{' '.join(record.attrs)} {kind} {name} {{ {members} }}".strip()
                sites.append(site)
            report["disagreements"].append({"name": f"{kind} {name}", "kind": "layout", "sites": unique_sites(sites)})
    for name, definitions in sorted(cpp_classes.items()):
        if len({body for body, _ in definitions}) > 1:
            report["disagreements"].append({"name": f"class {name}", "kind": "odr", "sites": unique_sites([site for _, site in definitions])})
    # C++ declarations are not type-checked, so a symbol shared with any other unit must come from
    # a header in include/ (the same text C units are checked against), never a local redeclaration.
    for name, sites in sorted(cpp_externs.items()):
        shared_with_c = [dict(d.site, type=describe(d.type)) for d in declarations.get(name, [])]
        if shared_with_c or len({site["source"] for site in sites}) > 1:
            report["disagreements"].append({"name": name, "kind": "cpp-redeclaration", "sites": unique_sites(sites + shared_with_c)})
    report["disagreements"].sort(key=lambda item: (item["name"], item["kind"]))
    return report


def failed(report: dict, allow_unchecked_cpp: bool = False) -> bool:
    return bool(report["disagreements"] or any(not allow_unchecked_cpp or item["kind"] != "cpp" for item in report["unchecked"]))


def print_report(report: dict) -> None:
    print("LEVEL       NAME / SOURCE                         SITES / TYPE")
    print("----------- ------------------------------------- ----------------------------------------")
    for entry in report["units"]:
        print(f"{'checked' if entry['checked'] else 'unchecked':11} {entry['source']}")
    for category, level in (("disagreements", "error"), ("policy", "policy")):
        for item in report[category]:
            print(f"{level:11} {item['name']} ({item['kind']})")
            for site in item["sites"]:
                print(f"{'':11} {site['source']}:{site['line']}  {site['type']}")
    for item in report["unchecked"]:
        print(f"{'unchecked':11} {item['source']}  {item['reason']}")
    checked = sum(unit["checked"] for unit in report["units"])
    print(f"Units: {len(report['units'])}; checked: {checked}; disagreements: {len(report['disagreements'])}; policy: {len(report['policy'])}; unchecked: {len(report['unchecked'])}")
    print("Const coverage: " + COVERAGE["const_writes"] + ". Not covered: " + "; ".join(COVERAGE["not_covered"]) + ".")
    print("C++: " + COVERAGE["cpp"] + ".")


def select_units(sources: list[str], matches: Path | None, root: Path = ROOT) -> list[Unit]:
    profiles = toolchain.load_profiles()
    if matches is not None:
        data = json.loads((root / matches).read_text(encoding="utf-8"))
        entries = [(root / entry["source"], entry["profile"]) for entry in data["units"]]
    else:
        paths = [root / name for name in sources] if sources else sorted(p for p in (root / "src").rglob("*") if p.suffix.lower() in (".c", ".cpp"))
        entries = [(path, "game") for path in paths]
    selected = {}
    for source, profile in entries:
        if profile not in profiles:
            raise InterfaceError(f"unknown compiler profile {profile!r}")
        key = source.resolve()
        if key in selected and selected[key].profile != profiles[profile]:
            raise InterfaceError(f"{source}: conflicting profiles in input ledger")
        selected[key] = Unit(key, profiles[profile])
    return list(selected.values())


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    commands = parser.add_subparsers(dest="command", required=True)
    check = commands.add_parser("check", help="audit selected cross-unit interfaces")
    check.add_argument("sources", nargs="*")
    check.add_argument("--matches", type=Path, help="audit all units with their recorded profiles")
    check.add_argument("--json", type=Path, help="write a schema-version-1 report")
    check.add_argument("--allow-unchecked-cpp", action="store_true", help="waive unchecked C++ only (still reported)")
    args = parser.parse_args(argv)
    if args.matches and args.sources:
        parser.error("--matches and explicit sources are mutually exclusive")
    try:
        units = select_units(args.sources, args.matches)
        report = audit(units)
        print_report(report)
        if args.json:
            args.json.parent.mkdir(parents=True, exist_ok=True)
            args.json.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")
        return int(failed(report, args.allow_unchecked_cpp))
    except (InterfaceError, OSError, ValueError, KeyError, TypeError) as exc:
        print(f"interfaces: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
