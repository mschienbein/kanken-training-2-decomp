# Near-miss C/C++ source permuter

Run from the repository root, using the project interpreter:

```sh
.venv/bin/python tools/permute.py scratch/candidate.c func_001091d4 --profile game --seconds 180 --seed 17 --out scratch/permuter-run
```

For a C++ candidate, use the same CLI and limits:

```sh
uv sync  # includes the pinned libclang wheel; no system Clang installation needed
.venv/bin/python tools/permute.py scratch/candidate.cpp func_001b54ec --profile game --seconds 180 --workers 2 --seed 17 --max-candidates 100 --out scratch/permuter-cpp-run
```

`.cpp`, `.cc` and `.cxx` select the C++ adapter (case-insensitive). C++ targets may
be unambiguous source names, qualified method names, or emitted mangled symbols.
Overloads require a unique selector; `target_symbols` records the selected emitted
symbols. The strict oracle must still resolve those symbols through existing
address names/bindings. The adapter never invents aliases or callees.

List several function names to search their bodies together. `--workers` defaults to
`os.cpu_count()` (all reported CPUs); reduce it on a shared host. The output directory must
be new and underneath this worktree's `scratch/`. Neither the input nor canonical sources
are edited. Exit status: **0** MATCH, **1** budget/candidate limit/search exhaustion with a
nonzero best score, **2** invalid input, compiler/linker failure on the input, or I/O error.
The time budget stops scheduling rounds; an in-flight compile round is allowed to finish.

## Oracle and reproducibility

Preprocessing calls `interfaces.preprocess` directly: ARMCC `-E`, the pinned profile,
the same launcher, and the same project/compiler include paths as the interface auditor.
`interfaces.prepare` handles parser-facing ARMCC extensions. The existing C path
regenerates only selected bodies with pycparser; its mutation set and scoring are
unchanged. The C++ adapter (`tools/permute_cpp.py`) uses the pinned libclang wheel
for an ARMv6K, signed-char, C++98 syntax/type view of that **same ARMCC-preprocessed
text**, not a host preprocessor or Clang code generation. It applies small
AST-guided text edits instead of regenerating a C++ translation unit. Parser-only
normalization maps UTF-8 byte offsets and expanded `__int64` types back to exact
raw source offsets. Each C++ proposal is reparsed without recovery errors before
entering the ARMCC oracle. Multiprocessing state contains text, not Clang cursors.
Clang receives a parser-only prototype for ARMCC's documented
`__schedule_barrier` intrinsic, keeping it an opaque effectful call. That prototype
never enters candidate text, object generation, or symbol binding.


Other functions, declarations, included headers and ABI annotations remain
verbatim. `__svc`, `__declspec`, `__packed`, `__int64` and `__value_in_regs` use the
interface auditor's extension handling without removing their emitted spelling.
Approved `ctr/svc.h` SVC/register-result declarations and `ctr/cp15.h`
named-coprocessor-register helpers are accepted and preserved outside target bodies.

Original-source hints locate functions after splicing backslash-newline continuations
(LF or CRLF) and ignoring comments and complete preprocessing directives. Multiline
macros before, after or inside an ordinary target do not alter its source boundaries;
all hint slices still use offsets into the original text.

Each evaluation calls `toolchain.compile_source(cache=False)` in a private per-worker
scratch directory, then `match.Context.compare`, using the frozen
`armlink5-inline-branchnop-v2` model. Temporary objects and cache refreshes are private and
removed when finished. No oracle or linker behavior is changed. Unknown symbols, unsupported
relocations and compiler failures invalidate a candidate; they never count as a match.

The score remains `instruction_changes * 16 + aligned_changed_bytes + abs(size_delta) * 8`.
Instruction-token edits align 4-byte ARM words or 2-byte Thumb halfwords;
`aligned_changed_bytes` counts byte edits within those aligned edit hunks. It is a search
heuristic, not a match percentage. Separately, `changed_bytes` compares bytes at the same
offset over the catalogue extent and counts every extra or missing candidate byte once.
Thus moved instructions can give different aligned and positional byte counts without
changing the candidate or its score. Older search records called the aligned component
`changed_bytes`; those immutable records do not contain a positional byte count.
**Zero requires the oracle's exact MATCH status and exact catalogue extent for every
target**; EXTENT is always nonzero. Output summaries contain counts, offsets and linker
rewrite rule counts, never original bytes, instruction text, packets, or rewrite encodings.

Workers hill-climb independently, accept neutral plateaus, and restart randomly or after
stagnation. Ordered multiprocessing rounds make proposals, scoring decisions and tie-breaking
reproducible for fixed input, toolchain, catalogue, seed and worker count. A wall-time budget
can end after different numbers of rounds on different hosts. Add `--max-candidates N`
(including baseline compiles) for an exactly reproducible bounded search; give it sufficient
time to reach that cap. Timings and throughput are intentionally not deterministic.

## Mutations and author hints

The conservative **C** AST mutations include:

- Adjacent independent statements/declarations, including disjoint fields of a known struct.
- Introducing integer temporaries, inlining single-use temporaries with their conversions,
  and splitting/merging local declaration initializers.
- Integral commutative operands and comparison-side swaps.
- Negated conditions and inverted if/else branches, with explicit scopes to avoid dangling else.
- For/while/guarded-do forms; transformations that mishandle `continue` are excluded.
- Unused pre/post increment/decrement and equivalent compound assignments.
- Same-type integer casts and equivalent signed/unsigned type spellings. Actual signedness
  changes require immutable small nonnegative locals used only as array indexes.
- Adding/removing top-level `const` on unescaped, initialized, unwritten scalar/pointer locals.
- Array indexing versus pointer arithmetic/dereference, and arrow versus dereference/member access.

The default game profile is C90: when a mutation introduces a declaration after executable
code, a suffix scope preserves initialization order instead of hoisting it. Unknown aliases,
union fields, volatile accesses, ambiguous shadowed types and relevant goto/label cases are
excluded from transformations that need independence or scope reasoning. No assembly,
named registers, SVC annotations, new volatile qualifiers, arbitrary sign changes or
changed constants are introduced by mutations.

The C++ adapter intentionally has a smaller, independently guarded mutation set:

- Builtin integer commutative operands and comparison-side swaps.
- Same-type casts around builtin integer arithmetic prvalues, and integer
  temporaries in direct scalar-return statements.
- Pure scalar condition negation/branch inversion with explicit branch scopes.
- `while` to `for (; condition ;)`, preserving the original body, `continue`,
  `break`, and lifetime boundaries.
- Unused builtin integer pre/post increments/decrements and compound assignments.
- Builtin array subscripting to pointer arithmetic/dereference, and pointer member
  access to dereference/member access.

Overloaded operators, user-defined conversions, volatile accesses, overloaded
pointer/indexing operations, value-used increments, reference-return temporaries,
and nested local-class method bodies are not candidates for these rewrites.
No C++ declaration reordering, qualifier/signedness changes, floating arithmetic,
or general loop lowering is attempted. Clang's builtin type/effect guards are
conservative hints; ARMCC compilation, exact linking and source review remain required.

`initial.*` preserves the starting point. Every strictly improved worker best is
retained as `improvement-NNNNNN.*`; a C normalized-body improvement is also retained.
`best.c` (C) or `best.cpp` (C++) is a complete preprocessed translation unit.
**`best.functions.c` / `best.functions.cpp` and `best.diff` are the author hints**:
the winning preprocessed functions and a readable unified diff against the original
function text. C++ hints retain constructors, destructor names, namespace/class
qualification, overload identity, and ARMCC declaration annotations. Per-candidate
JSON includes score and numeric diff summaries; `summary.json` records workers,
candidate count/rate, elapsed time, time to match and input/oracle fingerprints.
Port the useful change back to clean source, then use the normal matching and review gates.

## Limits and tests

Parser-unsupported constructs, ABI-relevant annotations inside a target body
(including CP15 named registers), old-style C definitions, macros that generate
whole function definitions, assembly/core-register bindings and pragmas that the
interface parser cannot model are refused rather than silently stripped. C++
requires a unique literal function definition and an emitted symbol; primary
templates without such a symbol and function-try-blocks are not search targets.
C++ syntax in a `.c` file is still refused by the unchanged C parser.
The search is deliberately conservative and incomplete: it cannot repair incorrect semantics,
invent types/interfaces, fix catalogue boundaries, prove arbitrary alias independence, or
guarantee finding an existing matching spelling. A byte match remains an author hint, not
semantic review or checkpoint acceptance. Float arithmetic is not reassociated or split into
new floating temporaries.

```sh
.venv/bin/python -m unittest tests.test_permute tests.test_permute_cpp
```

The modules test reparsing, target-only edits, overlapping function names,
prepared/raw/UTF-8 offset separation, multiline macros and continued comments with
LF/CRLF, approved extension headers, namespaces/templates/classes/methods,
constructors/destructors, overload selection and C++ semantic safety guards.
Coverage also includes deterministic hill-climbing and bounded multiprocessing,
distinct aligned/positional byte counts (including unequal extents), complete C++
candidate/hint emission, synthetic real-compiler mutations/scoring, and optional
native C and C++ execution comparisons. The C regressions remain in `test_permute`.
Compiler tests skip without the pinned compiler/launcher; image tests
also skip without private `orig/code.bin` and `orig/exheader.bin`. All temporary artifacts stay
in `scratch/`; no full build or full test suite is required.
