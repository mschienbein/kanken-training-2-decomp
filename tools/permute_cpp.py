"""Conservative C++ AST-guided source edits for the private permuter.

ARMCC still preprocesses and compiles every candidate. Clang supplies syntax and
builtin type/effect guards only; its code generator and ABI are never the oracle.
Only text state crosses multiprocessing boundaries, not libclang cursors.
"""
from __future__ import annotations

import bisect
import difflib
import random
import re
from dataclasses import dataclass
from pathlib import Path

import interfaces
import toolchain


class CppError(RuntimeError):
    """A C++ input cannot safely enter the search."""


MUTATIONS = ("temporary", "commute", "comparison", "condition", "loop", "increment", "cast", "pointer")
FUNCTION_KINDS = {"FUNCTION_DECL", "CXX_METHOD", "CONSTRUCTOR", "DESTRUCTOR", "CONVERSION_FUNCTION", "FUNCTION_TEMPLATE"}
INTEGER_KINDS = {"CHAR_U", "UCHAR", "CHAR16", "CHAR32", "USHORT", "UINT", "ULONG", "ULONGLONG", "CHAR_S", "SCHAR", "WCHAR", "SHORT", "INT", "LONG", "LONGLONG"}
SCALAR_KINDS = INTEGER_KINDS | {"BOOL", "ENUM", "POINTER"}
BINARY = {"+", "-", "*", "/", "%", "<<", ">>", "&", "|", "^", "==", "!=", "<", ">", "<=", ">=", "&&", "||"}
# ARMCC recognizes this documented compiler intrinsic without a declaration
# (ctr/cp15.h). Clang does not. A parser-only prototype keeps it an opaque,
# effectful call; no replacement, declaration or alias enters emitted candidates.
PARSER_INTRINSICS = 'extern "C" void __schedule_barrier(void);\n'
# Longest operators first. Unlike the interface auditor's C tokenizer, keep C++
# scope/operators and raw string literals whole when locating author hints.
TOKEN = re.compile(
    r'/\*.*?\*/|//[^\n]*|(?:u8|[LuU])?R"([^\s()\\]{0,16})\(.*?\)\1"|'
    r'(?:u8|[LuU])?"(?:\\.|[^"\\])*"|(?:[LuU])?\'(?:\\.|[^\'\\])*\'|'
    r'[A-Za-z_]\w*|(?:0[xX][0-9a-fA-F]+|\d+)\w*|'
    r'>>=|<<=|->\*|\.\.\.|::|->|\+\+|--|&&|\|\||==|!=|<=|>=|<<|>>|[+*/%&|^!-]=|[^\s]',
    re.DOTALL,
)


@dataclass(frozen=True)
class Token:
    word: str
    start: int
    end: int
    line: int


def tokens(text: str) -> list[Token]:
    """Splice before lexing, ignore comments/directives, retain raw offsets."""
    pieces, offsets, cursor = [], [], 0
    for continuation in re.finditer(r"\\\r?\n", text):
        pieces.append(text[cursor:continuation.start()])
        offsets.extend(range(cursor, continuation.start()))
        cursor = continuation.end()
    pieces.append(text[cursor:])
    offsets.extend(range(cursor, len(text)))
    spliced = "".join(pieces)
    result, previous, line_start, directive = [], 0, True, False
    newlines = [m.start() for m in re.finditer("\n", text)]
    for match in TOKEN.finditer(spliced):
        if "\n" in spliced[previous:match.start()]:
            line_start, directive = True, False
        previous = match.end()
        word = match[0]
        if word.startswith(("/*", "//")):
            continue
        if line_start:
            directive = word == "#"
        line_start = False
        if not directive:
            start, end = offsets[match.start()], offsets[match.end() - 1] + 1
            result.append(Token(word, start, end, bisect.bisect_left(newlines, start) + 1))
    return result


def source_spans(text: str, spelling: str) -> list[tuple[int, tuple[int, int, int]]]:
    """Locate literal ordinary C++ definitions, including methods/ctor initializers.

    The AST chooses the identity; this lexer only recovers original spelling for
    hints. Ambiguous/macro-generated definitions fail closed in Program.parse.
    """
    ts = tokens(text)
    wanted = [t.word for t in tokens(spelling)]
    pairs, stack = {}, []
    for i, token in enumerate(ts):
        if token.word in {"(", "[", "{"}:
            stack.append(i)
        elif token.word in {")", "]", "}"}:
            if not stack or ts[stack[-1]].word != {")": "(", "]": "[", "}": "{"}[token.word]:
                raise CppError("unbalanced C++ source delimiters")
            pairs[stack.pop()] = i
    if stack:
        raise CppError("unbalanced C++ source delimiters")
    found = []
    for i, token in enumerate(ts):
        if [t.word for t in ts[i:i + len(wanted)]] != wanted:
            continue
        if i and ts[i - 1].word == "~" and not spelling.startswith("~"):
            continue
        j = i + len(wanted)
        if j >= len(ts) or ts[j].word != "(":
            continue
        j = pairs[j] + 1
        while j < len(ts) and ts[j].word in {"const", "volatile", "override", "final", "&", "&&", "throw", "noexcept", "__attribute__", "__attribute", "__declspec"}:
            j += 1
            if j < len(ts) and ts[j].word == "(":
                j = pairs[j] + 1
        if j < len(ts) and ts[j].word == ":":
            # Constructor mem-initializers each have a (...) or {...} argument.
            j += 1
            while j < len(ts):
                while j < len(ts) and ts[j].word not in {"(", "{", ";", "}"}:
                    j += 1
                if j >= len(ts) or ts[j].word not in {"(", "{"}:
                    break
                j = pairs[j] + 1
                if j < len(ts) and ts[j].word == ",":
                    j += 1
                    continue
                break
        if j >= len(ts) or ts[j].word != "{":
            continue
        first = i
        while first and ts[first - 1].word not in {";", "{", "}"}:
            # Do not include a class access label in an inline method's hint.
            if ts[first - 1].word == ":" and first > 1 and ts[first - 2].word in {"public", "protected", "private"}:
                break
            first -= 1
        found.append((token.line, (ts[first].start, ts[j].start, ts[pairs[j]].end)))
    return found


class View:
    """Parser-only normalization with an exact UTF-8-byte -> raw-char map."""

    def __init__(self, text: str, source: Path, profile: toolchain.Profile | None = None):
        # Expand ourselves before prepare(), so every length-changing replacement
        # has a known map. Other prepare() substitutions preserve character count.
        parts, raw_map, cursor = [], [], 0
        for token in interfaces.TOKEN.finditer(text):
            if token[0] != "__int64":
                continue
            parts.append(text[cursor:token.start()])
            raw_map.extend(range(cursor, token.start()))
            parts.append("long long")
            raw_map.extend([token.start()] * len("long long"))
            cursor = token.end()
        parts.append(text[cursor:])
        raw_map.extend(range(cursor, len(text)))
        expanded = "".join(parts)
        try:
            self.prepared = interfaces.prepare(expanded, source, profile=profile)
        except interfaces.InterfaceError as exc:
            raise CppError(str(exc)) from exc
        self.offsets = []
        for char, offset in zip(self.prepared.text, raw_map):
            self.offsets.extend([offset] * len(char.encode("utf-8")))
        self.offsets.append(len(text))
        self.text = text
        self.source = source

    def span(self, cursor) -> tuple[int, int]:
        return self.offsets[cursor.extent.start.offset], self.offsets[cursor.extent.end.offset]

    def get(self, cursor) -> str:
        start, end = self.span(cursor)
        return self.text[start:end]


def clang_api():
    try:
        from clang import cindex
        return cindex
    except ImportError as exc:
        raise CppError("C++ support requires the pinned libclang wheel; run uv sync") from exc


def parse_tree(text: str, source: Path, profile: toolchain.Profile | None = None):
    clang = clang_api()
    view = View(text, source, profile)
    intrinsic_header = str(Path(__file__).resolve().with_name(".permute-cpp-intrinsics.h"))
    try:
        tree = clang.Index.create().parse(
            str(source.resolve()),
            args=["-x", "c++", "-std=c++98", "-target", "armv6k-none-eabi", "-fsigned-char", "-fms-extensions", "-Wno-everything", "-include", intrinsic_header],
            unsaved_files=[(str(source.resolve()), view.prepared.text), (intrinsic_header, PARSER_INTRINSICS)],
        )
    except (clang.LibclangError, clang.TranslationUnitLoadError) as exc:
        raise CppError("cannot load the pinned libclang C++ parser") from exc
    # Never trust recovery ASTs, or emit diagnostics containing source snippets.
    failures = [d for d in tree.diagnostics if d.severity >= clang.Diagnostic.Error]
    if failures:
        first = failures[0]
        raise CppError(f"unsupported C++ input at preprocessed line {first.location.line} (clang diagnostic {first.category_number})")
    return view, tree


def walk(cursor):
    yield cursor
    for child in cursor.get_children():
        yield from walk(child)


def body_walk(cursor):
    yield cursor
    for child in cursor.get_children():
        if child.kind.name not in FUNCTION_KINDS:
            yield from body_walk(child)


def qualified(cursor) -> str:
    parts = [cursor.spelling]
    parent = cursor.semantic_parent
    while parent and parent.kind.name != "TRANSLATION_UNIT":
        if parent.spelling:
            parts.append(parent.spelling)
        parent = parent.semantic_parent
    return "::".join(reversed(parts))


def definitions(tree) -> list:
    # Template declarations can expose the same method via more than one cursor.
    found = {}
    for cursor in walk(tree.cursor):
        if cursor.kind.name in FUNCTION_KINDS and cursor.is_definition():
            bodies = [c for c in cursor.get_children() if c.kind.name == "COMPOUND_STMT"]
            if bodies:
                found[(cursor.extent.start.offset, cursor.extent.end.offset)] = (cursor, bodies[-1])
    return list(found.values())


def resolve(functions: list, targets: tuple[str, ...]) -> dict:
    result = {}
    for target in targets:
        choices = [(f, b) for f, b in functions if target in {f.spelling, qualified(f), f.mangled_name}]
        if len(choices) != 1:
            raise CppError(f"target function is {'ambiguous' if choices else 'not defined'}: {target}")
        result[target] = choices[0]
    identities = [(f.extent.start.offset, f.extent.end.offset) for f, _ in result.values()]
    if len(set(identities)) != len(identities):
        raise CppError("multiple target selectors name the same C++ body")
    return result


@dataclass
class Program:
    text: str
    ast: str
    targets: tuple[str, ...]
    source: Path
    spans: dict[str, tuple[int, int, int]]
    original_functions: dict[str, str]
    oracle_targets: tuple[str, ...]
    suffix: str = ".cpp"
    profile: toolchain.Profile | None = None

    @classmethod
    def parse(cls, text: str, targets: tuple[str, ...], source: Path, original: str | None = None,
              *, profile: toolchain.Profile | None = None) -> Program:
        view, tree = parse_tree(text, source, profile)
        functions = definitions(tree)
        selected = resolve(functions, targets)
        spans, originals, symbols = {}, {}, []
        original_text = text if original is None else original
        for target, (function, body) in selected.items():
            body_start, body_end = view.span(body)
            start_char = len(view.prepared.text.encode("utf-8")[:body.extent.start.offset].decode("utf-8"))
            end_char = len(view.prepared.text.encode("utf-8")[:body.extent.end.offset].decode("utf-8"))
            if any(start_char <= a.start < end_char for a in view.prepared.annotations):
                raise CppError(f"target body has ABI-relevant ARMCC annotations: {target}")
            name_offset = view.offsets[function.location.offset]
            candidates = source_spans(text, function.spelling)
            matching = [s for _, s in candidates if s[0] <= name_offset < s[1] and s[1:] == (body_start, body_end)]
            if len(matching) != 1:
                raise CppError(f"cannot locate literal C++ function text: {target}")
            spans[target] = matching[0]
            if original is None:
                original_span = matching[0]
            else:
                filename, line = view.prepared.locations[function.location.line - 1]
                if interfaces.origin_path(filename, interfaces.ROOT) != source.resolve():
                    raise CppError(f"target definition is not in the input source: {target}")
                peers = [f for f, _ in functions if f.spelling == function.spelling and view.prepared.locations[f.location.line - 1] == (filename, line)]
                peers.sort(key=lambda f: f.location.offset)
                raw = [s for ln, s in source_spans(original, function.spelling) if ln == line]
                if len(raw) != len(peers):
                    raise CppError(f"cannot locate original function text (macro-generated definition?): {target}")
                original_span = raw[next(i for i, f in enumerate(peers) if f == function)]
            originals[target] = original_text[original_span[0]:original_span[2]].strip() + "\n"
            symbol = function.mangled_name
            if not symbol:
                raise CppError(f"target has no unique emitted C++ symbol: {target}")
            symbols.append(symbol)
        return cls(text, text, targets, source, spans, originals, tuple(symbols), profile=profile)

    def render(self, ast: str) -> str:
        return ast

    def hints(self, ast: str | None) -> tuple[str, str]:
        text = self.text if ast is None else ast
        current = self if text == self.text else self.parse(text, self.targets, self.source, profile=self.profile)
        functions, patch = [], []
        for target in self.targets:
            start, _, end = current.spans[target]
            function = text[start:end].strip() + "\n"
            functions.append(function)
            patch.extend(difflib.unified_diff(
                self.original_functions[target].splitlines(keepends=True), function.splitlines(keepends=True),
                fromfile=f"original/{target}.cpp", tofile=f"permuted/{target}.cpp",
            ))
        return "\n".join(functions), "".join(patch)


def canonical(cursor):
    return cursor.type.get_canonical()


def integer(cursor) -> bool:
    return canonical(cursor).kind.name in INTEGER_KINDS


def pure(cursor) -> bool:
    """Builtin, nonvolatile reads only; calls and overloaded operators fail closed."""
    if canonical(cursor).is_volatile_qualified():
        return False
    kind = cursor.kind.name
    children = list(cursor.get_children())
    if kind in {"INTEGER_LITERAL", "CHARACTER_LITERAL", "CXX_BOOL_LITERAL_EXPR", "CXX_NULL_PTR_LITERAL_EXPR"}:
        return True
    if kind == "DECL_REF_EXPR":
        return canonical(cursor).kind.name in SCALAR_KINDS and cursor.referenced.kind.name in {"VAR_DECL", "PARM_DECL", "ENUM_CONSTANT_DECL"}
    if kind in {"UNEXPOSED_EXPR", "PAREN_EXPR"}:
        return len(children) == 1 and pure(children[0])
    if kind == "CXX_THIS_EXPR":
        return True
    if kind == "MEMBER_REF_EXPR":
        return canonical(cursor).kind.name in SCALAR_KINDS and all(pure(c) for c in children)
    if kind in {"CSTYLE_CAST_EXPR", "CXX_STATIC_CAST_EXPR"}:
        return canonical(cursor).kind.name in SCALAR_KINDS and len(children) == 1 and pure(children[0])
    # Do not read arbitrary pointer/array memory, or hide updates in a unary node.
    if kind == "BINARY_OPERATOR":
        ts = list(cursor.get_tokens())
        return len(children) == 2 and binary_operator(cursor, ts) in BINARY and all(pure(c) for c in children)
    if kind == "UNARY_OPERATOR":
        ts = list(cursor.get_tokens())
        return bool(ts) and ts[0].spelling in {"+", "-", "!", "~"} and len(children) == 1 and pure(children[0])
    return False


def binary_operator(cursor, ts=None) -> str | None:
    children = list(cursor.get_children())
    if len(children) != 2:
        return None
    left, right = children
    between = [t.spelling for t in (ts if ts is not None else cursor.get_tokens()) if left.extent.end.offset <= t.extent.start.offset < right.extent.start.offset]
    return between[0] if len(between) == 1 else None


def apply(text: str, edits: list[tuple[int, int, str]], bounds: tuple[int, int]) -> str:
    previous = bounds[1]
    for start, end, replacement in sorted(edits, reverse=True):
        if not bounds[0] <= start <= end <= previous:
            raise CppError("C++ mutation overlaps or leaves the target body")
        text = text[:start] + replacement + text[end:]
        previous = start
    return text


def choices_for(view: View, function, body, kind: str) -> list[list[tuple[int, int, str]]]:
    choices = []
    nodes = list(body_walk(body))
    parents = {c.hash: p for p in nodes for c in p.get_children()}
    used = {t.word for t in tokens(view.text)}
    index = 0
    while f"permute_tmp_{index}" in used:
        index += 1
    fresh = f"permute_tmp_{index}"
    for node in nodes:
        name = node.kind.name
        children = list(node.get_children())
        start, end = view.span(node)
        text = view.get(node)
        if kind in {"commute", "comparison", "cast"} and name == "BINARY_OPERATOR" and len(children) == 2:
            left, right = children
            op = binary_operator(node)
            if not integer(left) or not integer(right) or not pure(left) or not pure(right):
                continue
            if kind == "cast" and op in BINARY and integer(node):
                choices.append([(start, end, f"(({canonical(node).spelling})({text}))")])
            elif op in ({"+", "*", "&", "|", "^"} if kind == "commute" else {"==", "!=", "<", ">", "<=", ">="}):
                reverse = {"<": ">", ">": "<", "<=": ">=", ">=": "<="}.get(op, op)
                choices.append([(start, end, f"(({view.get(right)}) {reverse} ({view.get(left)}))")])
        elif kind == "condition" and name == "IF_STMT" and len(children) in {2, 3}:
            cond, yes, *no = children
            if canonical(cond).kind.name not in SCALAR_KINDS or cond.kind.name == "VAR_DECL" or not pure(cond):
                continue
            # Condition declarations have an extra child or an '=' in the header.
            ts = list(node.get_tokens())
            if not ts or ts[0].spelling != "if" or ts[1].spelling != "(":
                continue
            # Even an enum can overload operator!. Materialize the original
            # builtin boolean conversion before adding a negation.
            condition = f"((bool)({view.get(cond)}))"
            if no:
                # libclang expression-statement extents omit their semicolon.
                yes_end = statement_end(view, yes)
                no_end = statement_end(view, no[0])
                replacement = f"if (!{condition}) {{ {view.text[view.span(no[0])[0]:no_end]} }} else {{ {view.text[view.span(yes)[0]:yes_end]} }}"
                choices.append([(start, no_end, replacement)])
            else:
                a, b = view.span(cond)
                choices.append([(a, b, f"!!{condition}")])
        elif kind == "loop" and name == "WHILE_STMT" and len(children) == 2:
            cond, stmt = children
            if cond.kind.name != "VAR_DECL" and canonical(cond).kind.name in SCALAR_KINDS:
                a, b = view.span(cond)
                # Change just the header; continue, break and body lifetimes stay identical.
                choices.append([(start, a, "for (; "), (b, view.span(stmt)[0], ";) ")])
        elif kind == "increment" and name == "UNARY_OPERATOR" and len(children) == 1:
            expr = children[0]
            parent = parents.get(node.hash)
            if not parent or parent.kind.name != "COMPOUND_STMT" or not integer(expr) or not pure(expr):
                continue
            ts = list(node.get_tokens())
            if ts and (ts[0].spelling in {"++", "--"} or ts[-1].spelling in {"++", "--"}):
                op = ts[0].spelling if ts[0].spelling in {"++", "--"} else ts[-1].spelling
                choices.append([(start, end, f"({view.get(expr)}) {'+=' if op == '++' else '-='} 1")])
                choices.append([(start, end, f"({view.get(expr)}){op}" if ts[0].spelling == op else f"{op}({view.get(expr)})")])
        elif kind == "pointer" and name == "ARRAY_SUBSCRIPT_EXPR" and len(children) == 2:
            base, subscript = children
            if canonical(base).kind.name == "POINTER" and integer(subscript) and pure(base) and pure(subscript) and not canonical(node).is_volatile_qualified():
                choices.append([(start, end, f"*((({view.get(base)})) + ({view.get(subscript)}))")])
        elif kind == "pointer" and name == "MEMBER_REF_EXPR" and len(children) == 1:
            base = children[0]
            if canonical(base).kind.name == "POINTER" and pure(base) and not canonical(node).is_volatile_qualified():
                _, b = view.span(base)
                tail = view.text[b:end]
                if tail.lstrip().startswith("->"):
                    choices.append([(start, end, f"(*({view.get(base)})).{tail.lstrip()[2:]}")])
        elif kind == "temporary" and name == "RETURN_STMT" and len(children) == 1:
            expr = children[0]
            parent = parents.get(node.hash)
            if parent and parent.kind.name == "COMPOUND_STMT" and function.result_type.get_canonical().kind.name in INTEGER_KINDS and expr.kind.name == "BINARY_OPERATOR" and integer(expr) and pure(expr):
                # A builtin binary expression is already a prvalue. Only scalar
                # returns are eligible, never overload-sensitive call arguments.
                choices.append([(start, start, f"{canonical(expr).spelling} {fresh} = {view.get(expr)};\n"), (*view.span(expr), fresh)])
    return choices


def statement_end(view: View, node) -> int:
    start, end = view.span(node)
    # A compound's closing brace belongs to its extent; expression/return/break
    # statement semicolons do not. Do not consume an unrelated following ';'.
    if view.text[start:end].rstrip().endswith("}"):
        return end
    match = re.match(r"\s*;", view.text[end:])
    return end + match.end() if match else end


def mutate(program: Program, ast: str, rng: random.Random, kind: str | None = None) -> tuple[str, str] | None:
    view, tree = parse_tree(ast, program.source, program.profile)
    selected = resolve(definitions(tree), program.targets)
    kinds = [kind] if kind else list(MUTATIONS)
    rng.shuffle(kinds)
    targets = list(program.targets)
    rng.shuffle(targets)
    for mutation in kinds:
        for target in targets:
            function, body = selected[target]
            choices = choices_for(view, function, body, mutation)
            rng.shuffle(choices)
            for edits in choices:
                # Clang's statement extents can exclude a pragma between an if
                # header and its body. Never consume one while replacing text.
                if any(re.search(r"(?m)^[ \t]*#[ \t]*pragma\b", ast[a:b]) for a, b, _ in edits):
                    continue
                candidate = apply(ast, edits, view.span(body))
                if candidate == ast:
                    continue
                try:
                    # C++ has overload resolution, value categories and lifetime
                    # rules absent from C. Never retain a recovery/invalid AST.
                    Program.parse(candidate, program.targets, program.source, profile=program.profile)
                except CppError:
                    continue
                return candidate, f"{target}:{mutation}"
    return None
