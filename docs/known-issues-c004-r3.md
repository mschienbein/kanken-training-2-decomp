# Known issues — checkpoint c004-r3

196 new functions — wave 3 (66) and wave 4 (130: 86 armlink-NOP'd tail-call wrappers, 8 empty
functions, 36 bodies) — by OpenAI-family authors (`openai-codex/gpt-6.1-sol`; `author` per unit
in `config/candidates/c004.json`), plus interface revisions to accepted units. Current denominator
v7; gates v1.10; linker model `armlink5-inline-branchnop-v2` (`docs/LINKER.md`). The predecessor
`c004-r2` was prepared against **denominator v6, gates v1.8**, not the v5 printed in its immutable
known-issues record. Main advanced the denominator and gates before this revision's final checks;
none of the c004 or accepted extents changed.

## Review independence

Review is single-family under
[`2026-10-17-reviewer-model-family.md`](rulings/2026-10-17-reviewer-model-family.md): both fresh
reviewers run `openai-codex/gpt-6.1-sol`, independently of the authors and integrator. They must
seal their records before consulting another review, and finalization requires `--min-reviews 2`.
This is not cross-family review.

## Linker model qualifications (`prepare.json` `linker_rewrites`, 89 functions)

- A call the original image holds as `mov r0, r0` was inlined by armlink because its callee is a
  return-only function. The bytes prove only that; **which** return-only function the source calls
  is `[INFERENCE]` unless a pointer or neighbour evidences it (marked in the source).
- A 4-byte wrapper `mov r0, r0` is a tail call to the function immediately after it. Its C
  signature (`void f(void) { g(); }` vs forwarding arguments) compiles to the same branch; the
  sources take it from the callee's known prototype or callers' use, else `void (void)` with a note.
- Empty functions (`bx lr`): parameters and return type cannot be recovered from the bytes; the
  sources use `void (void)` unless callers or callbacks evidence a contract (e.g. identity
  functions `void *f(void *p)` whose result accepted code uses). Pointer tables without a
  recoverable invocation contract remain qualified; an address alone does not recover a prototype.
- The conditional single-instruction inlining rule comes from the armlink manual only; no credited
  function here depends on it unless listed in `linker_rewrites` with a conditional callee.

## Interface revisions in r3

The class-wide audit covers all 157 four-byte c004 functions (67 return-only entries and 90
wrappers, including the four ordinary tail branches), the return-only callees referenced by the
larger c004 units, and their callee declarations: **240 distinct function contracts**. It records
all surviving direct callers, literal/static pointer uses, concrete callback dispatches, argument
registers (including VFP aliases), and r0/s0 result use in the private evidence at
`scratch/integrator/c004-r3-NOTES.md` in the main checkout. Twelve contracts changed; 228 retain
their previous signature with a per-function evidence note. Names, signedness, and opaque object
spellings remain `[INFERENCE]` where the calling code cannot establish them.

- `func_001f4fac`: `void (void)` → `void (unsigned category, unsigned value)`. Its direct Thumb
  caller `func_00100ae4` supplies a fixed r0 and its incoming value in r1 (c004-r2 review F01).
- `func_0010e67c` and its declaration of `func_0010e680`: preserve the unused unsigned r0
  explicitly prepared by `func_00100d74` before its call at `0x00101338`.
- `func_00144220`: preserve the unused second word explicitly supplied by `func_001941f8`;
  its contract is now `int (int value, unsigned flags)`. The adjacent identity
  `func_00144224(int)` remains unchanged: its separate accepted tail caller supplies only the
  identity value, and the wrapper discards its extra word.
- `func_0019ffa4`: `void *(unsigned)` → `void *(unsigned size, const void *tag)`. Allocation callers
  normally supply `data_002232d5` in r1; `func_00193110` supplies a stack address. Neither entry
  writes through r1. The exact odd data address was checked with the corrected packet tool. The
  unused pointer's source-level role is unknown; a nothrow/placement-tag interpretation is `[INFERENCE]`, not a
  recovered overload identity. `func_0019ffa8(unsigned)` stays single-argument: its implementation
  and 42 separate direct callers evidence size only. Both continue returning the allocation.
- `func_001acc18` and `func_001b3340`: empty array-element callbacks now take `void *object`.
  `func_001911cc` invokes the callback supplied by `func_001f5488` with an element address in r0
  and discards the result.
- `func_001c7f1c`, `func_001c8890`, and `func_001c8990`: empty static hooks now take `void *object`,
  from the dispatches in `func_001401fc`, `func_001c74e8`, and `func_001840dc`, respectively.
  Their results are not consumed. The dispatch paths for `func_001c8890` are alternatives, not
  sequential calls whose scratch r0 should be mistaken for a new return contract.
- `func_001c6a34` and its declaration of `func_001c6a38`: the former unsigned word is an opaque
  object pointer, as evidenced by the static hook dispatchers `func_0013888c`/`func_00182ae4`.
  The adjacent implementation only tests that input for null.

These changes affect ten candidate source files. No header or accepted source needs an additional
r3 edit: the changed declarations are local, and the whole composed ledger has zero interface
errors and zero policy items. All 181 c004 units plus the six already-revised accepted units
report MATCH (202 functions), compiled sequentially under `--profile game` on WSL Linux-x86_64.
This author-side evidence is not a checkpoint prepare, full build, or independent review.

## Revisions to accepted material (`revised_units` / `revised_headers`)

Made by the earlier integration pass (agent IntegrateC004; evidence in the commit "Unify c004
interfaces from caller and callback evidence"): shared prototypes moved into
`include/game/runtime.h`, `bss_0049ecf0.h` (command-mutex helpers
`func_0012fb38`/`func_0012fb48`), `render_state.h`; accepted units now include them instead of local
declarations (`func_00195254.c`, `func_0011f080.cpp`, `func_0013db3c.c`, `func_00147260.c`,
`func_00110634.c`, `func_00127294.c`). Evidence-based contract decisions: `func_00122500` returns
`int` (callers test r0); `func_00122510` is `void` (no direct caller reads r0, `[INFERENCE]` for
unresolved indirect callers); `func_0018f7f0`, `func_001ef880`/`func_001ef884`,
`func_0013714c`/`func_00137150` are identity functions `void *(void *)` (accepted callers use the
returned pointer / a typed destroy callback). These inherited revisions are unchanged in r3.

## Preserved original bugs (new in this checkpoint)

- `src/wave4/func_001440ec.c`, `src/wave4/func_0014a2f0.c`, `src/wave4/func_00183c54.c`: when
  the archive lookup finds no block, the original still loads through the null block reference
  (`ORIGINAL BUG:` comments; confirmed against the original control flow by review). Kept;
  matching requires it.

## Not credited

- `func_00100d6c` (SVC 3) remains outside the c004 candidate ledger. The later ARMCC-extension
  ruling (`docs/rulings/2026-10-17-armcc-extensions.md`, gates v1.9) changes source policy, not
  the scope of this checkpoint. `0x17da60` is byte-identical to ARMCC's compiler-generated
  `__ARM_common_memcpy4_6` helper (classification, not C credit).

## Inherited

Partial types, including the manager/lock prefixes in `func_001470c8`; preserved original bugs
(`src/pilot/game_enum_switch.c`, `src/wave1/func_00196cb8.c`); C++ checked mechanically only for
ODR, header class-name collisions and local redeclarations; byte equality of the relinked image;
no runtime test; no Windows reproduction yet. Diagnostic continuations after the terminal
`func_0019f568`/`func_001a0094` break path do not establish a normal returning value; their
`void (void)` contracts remain qualified rather than assigning a result to an abort path.

## Checkpoint history

- `c004`: prepare PASS; `review-c004-openai` PASS_WITH_QUALIFICATIONS (F01 minor: the three
  preserved null dereferences above were not listed); `review-c004-second` FAIL — five wrapper/
  callee interface types inferred too weakly: `func_00156de8` object is written (not `const`),
  `func_00166eac` forwards two float coordinates (s0/s1), `func_001c3abc`'s fourth argument is a
  nullable UTF-16 text pointer, `func_001229d0` registers an event callback (function pointer),
  `func_00121bc4` takes a writable intrusive-list object pointer. Never finalized.
- `c004-r2`: all five signatures fixed and all still MATCH; known issues list the preserved null
  dereferences. Prepare PASS on WSL Linux-x86_64 against denominator v6/gates v1.8.
  `review-c004-second` FAIL (F01 major: `func_001f4fac` drops two evidenced arguments; F02 minor:
  the overview says v5 instead of v6). `review-c004-openai` PASS_WITH_QUALIFICATIONS (202 rows:
  43 PASS, 159 QUALIFIED; F01 minor is the same denominator metadata error). Never finalized.
- `c004-r3`: the class-wide interface revisions and metadata correction above; all c004 and
  inherited revised units MATCH and the composed interface audit is clean. Preparation, the two
  fresh independent reviews, and finalization belong to the integrator.
