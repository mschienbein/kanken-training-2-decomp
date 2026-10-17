# Near-miss C source permuter

Run from the repository root, using the project interpreter:

```sh
.venv/bin/python tools/permute.py scratch/candidate.c func_001091d4 --profile game --seconds 180 --seed 17 --out scratch/permuter-run
```

List several function names to search their bodies together. `--workers` defaults to
`os.cpu_count()` (all reported CPUs); reduce it on a shared host. The output directory must
be new and underneath this worktree's `scratch/`. Neither the input nor canonical sources
are edited. Exit status: **0** MATCH, **1** budget/candidate limit/search exhaustion with a
nonzero best score, **2** invalid input, compiler/linker failure on the input, or I/O error.
The time budget stops scheduling rounds; an in-flight compile round is allowed to finish.

## Oracle and reproducibility

Preprocessing calls `interfaces.preprocess` directly: ARMCC `-E`, the pinned profile,
the same launcher, and the same project/compiler include paths as the interface auditor.
`interfaces.prepare` handles parser-facing ARMCC extensions. The unmodified preprocessed
translation unit is retained, and only selected function bodies are regenerated with
pycparser. All other functions, declarations, headers and ABI annotations remain verbatim.

Each evaluation calls `toolchain.compile_source(cache=False)` in a private per-worker
scratch directory, then `match.Context.compare`, using the frozen
`armlink5-inline-branchnop-v2` model. Temporary objects and cache refreshes are private and
removed when finished. No oracle or linker behavior is changed. Unknown symbols, unsupported
relocations and compiler failures invalidate a candidate; they never count as a match.

The score sums aligned instruction-token edits (4-byte ARM words, 2-byte Thumb halfwords),
changed bytes and a size penalty. It is a search heuristic, not a match percentage. **Zero
requires the oracle's exact MATCH status and exact catalogue extent for every target**;
EXTENT is always nonzero. Output summaries contain counts, offsets and linker rewrite rule
counts, never original bytes, instruction text, packets, or rewrite encodings.

Workers hill-climb independently, accept neutral plateaus, and restart randomly or after
stagnation. Ordered multiprocessing rounds make proposals, scoring decisions and tie-breaking
reproducible for fixed input, toolchain, catalogue, seed and worker count. A wall-time budget
can end after different numbers of rounds on different hosts. Add `--max-candidates N`
(including baseline compiles) for an exactly reproducible bounded search; give it sufficient
time to reach that cap. Timings and throughput are intentionally not deterministic.

## Mutations and author hints

The conservative AST mutations include:

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
excluded from transformations that need independence or scope reasoning. No asm, `__svc`,
named registers, new volatile qualifiers, arbitrary sign changes or changed constants are used.

`initial.*` preserves the starting point. Every strictly improved worker best is retained as
`improvement-NNNNNN.*`; a normalized-body improvement is also retained. `best.c` is the complete
preprocessed translation unit. **`best.functions.c` and `best.diff` are the author hints**:
the winning preprocessed functions and a readable unified diff against the original function
text. Per-candidate JSON includes score and numeric diff summaries; `summary.json` records
workers, candidate count/rate, elapsed time, time to match and input/oracle fingerprints.
Port the useful change back to clean source, then use the normal matching and review gates.

## Limits and tests

C only, not C++. Parser-unsupported constructs, ABI-relevant annotations inside a target
body, old-style definitions, macros that generate whole function definitions, assembly and
pragmas that the interface parser cannot model are refused rather than silently stripped.
The search is deliberately conservative and incomplete: it cannot repair incorrect semantics,
invent types/interfaces, fix catalogue boundaries, prove arbitrary alias independence, or
guarantee finding an existing matching spelling. A byte match remains an author hint, not
semantic review or checkpoint acceptance. Float arithmetic is not reassociated or split into
new floating temporaries.

```sh
.venv/bin/python -m unittest tests.test_permute
```

The module tests reparsing, target-only edits, safety guards, deterministic hill-climbing and
bounded multiprocessing, synthetic real-compiler mutations/scoring, and optional native
execution comparisons. Compiler tests skip without the pinned compiler/launcher; image tests
also skip without private `orig/code.bin` and `orig/exheader.bin`. All temporary artifacts stay
in `scratch/`; no full build or full test suite is required.
