# c008 integration issues and qualifications

This is an integration record, **not acceptance or an independent review**. It is exercised on
the `c008-integrate` branch after merging finalized c007 (seq 7) and the v1.14–v1.16 harness from
main. No prepare, review or finalize has run; Main owns those. Every row below uses the default
models (`armlink5-inline-branchnop-v2`, literal model `none`). No R1/L1 (v3 / literal-record) row
is included; such rows belong to a later dedicated checkpoint.

| Area | Units | Functions | Catalogue bytes |
|---|---:|---:|---:|
| wave 12 | 135 | 135 | 6,292 |
| wave 13 | 114 | 114 | 4,956 |
| wave 14 | 121 | 121 | 5,664 |
| numeric retries (r302–r329 and earlier escalations) | 57 | 57 | 5,348 |
| solo initializers / signed-division caller | 8 | 8 | 90,504 |
| **Total new candidates** | **435** | **435** | **112,764** |

Profiles: 332 game / 69 middleware / 34 sdk units. 96 units (96 functions) are C++.
Accepted ledger through c007 is 1,093 units / 1,167 functions / 73,388 B; the dry union is
1,528 units / 1,602 functions / 186,152 B.

## Exercised evidence (worktree `scratch/integrator/c008/`, not committed)

- **Per-unit exactness:** all 435 candidates plus 21 revised accepted sources (456 units / 462
  functions) were recompiled by `tools/match.py` under their profiles: every command exits 0 and
  every credited function reports `MATCH` (`final-match-sweep.json`). Re-run after the main merge.
- **Interfaces (G3):** `tools/interfaces.py check --matches dry-matches.json --allow-unchecked-cpp`
  exits 0: 1,528 units / 1,228 checked C / 300 explicitly unchecked C++, zero disagreements,
  10 inline-declaration policy findings (unchanged shape from accepted checkpoints), zero
  unchecked C (`interfaces-final.json`).
- **Caller audit:** `tools/argcheck.py` exits 2 as designed: 46 raw candidates in 28 functions, each
  adjudicated individually (35 against unchanged predecessor identities, 11 against fresh
  register-only facts) in `argcheck-adjudications.json`; none is a missing input or consumed
  result. `func_00195ec4` is QUALIFIED [INFERENCE]: the extra r1/r2 writes are destructor
  registration operands of an erased call, not mutex inputs. 216 functions have no parsed C
  contract (C++) and 1,048 indirect sites remain unresolved; those limits are explicit.
- **Dry union build** through `tools/jobs.py` (`c008-final-dry-union-3`): exit 0, PASS over 1,528 units.
- **Full suite** on the merged branch: exit 0, 623 tests / 1 skipped / 0 failed modules.

## Source policy

No inline `__asm`, embedded-assembler functions, byte blobs or `INTEGRATOR:` markers appear in any
candidate or revised unit. Documented ARMCC extensions are used through shared headers and are
qualifications on their rows: `__svc` declarations (16 functions), CP15 named-register
bindings/barriers (9 functions) and `__current_pc()` provenance for result-check helpers (6
functions). 15 functions carry `ORIGINAL BUG:` comments for preserved original misbehaviour
(unchecked negative indices, null dereferences after failed lookups, absent-kind one-past-table
reads, uninitialised mode bits). 75 `[INFERENCE]` comments mark descriptive names, callee identity
or role claims that bytes alone cannot prove.

Thirteen definitions in candidates and `func_00112c98` ignore an original input
(`func_001d2d14 registry`, `func_00181384 pool`, `func_0012d858 context`, `func_001462d4 owner`,
`func_001e37e0 interface`, the five `RuntimeTypedObject` identity getters, `func_001f494c state`,
`func_0012e854 second/third`, `SoundStreamInstance::SoundStreamInstance pool`, and
`func_00112c98 handle`). Each keeps the caller-evidenced input; types and roles are [INFERENCE].

## Linker rewrites

Credited candidate and revised functions carry 28 recorded armlink rewrites in 26 functions: 21
`branchnop`, 6 `inline_nop`, 1 `inline_insn` (`func_0016b6a4`). Erased callee identities are
unverifiable from bytes: `func_0016e270` (two sites), `func_001bbc0c`, `func_001cf6dc`,
`LargeEventOwner::~LargeEventOwner()` (`func_001ef818`, with an explicit `[INFERENCE]` comment on
the bound `EventPart28` destructor) and `func_0016b6a4`. Source choices for these callees are
plausible, not proven.

## Names overlay

43 symbols are added to `config/names.json`: 18 native C++ definition aliases (constructors,
destructors and members matched as whole definitions at their bound address, e.g.
`_ZN15LayoutDrawStateC1Ev`), 24 pure call targets that each have a natural whole-function MATCH
at a credited caller and an isolated unresolved-symbol failure without the binding, and the
compiler-defined five-byte `__ARM_common_memset4_5` helper (its body is **not** credited).
17 proposed aliases were unused and are not adopted; the canonical
`FloatPropertyDescriptor` constructor spelling and `SoundPlaybackState::C2` remain unproven and
unadopted. Class names are [INFERENCE].

## Accepted-source revisions (21 sources / 27 functions / 1,104 B)

Eight accepted c001–c005 units (`pilot_a` and the wave2/4/5/6/7 sources), eight wave8/wave9
audio and input consumers, `func_001c5950/70/59fc` and `func_0016d2dc/fc` were revised to the
canonical shared types now used by c008 callers (`Pane::style` is `AudioChannelState *`; the
private duplicate `W10DrawingInput` definitions are removed). Every revised function re-MATCHes.
These are credited functions whose source changed, so they must be reviewed as revised units.

## Shared headers

Shared headers gained canonical definitions for the c008 consumers. Repeated `};;` terminators in
accepted headers were repaired (not preserved). `include/game/graphics_state.h` is now C++-safe: the
trailing `buffers[]` flexible member is a `[1]` storage view for C++ only; the C layout is
unchanged and the view is not an inferred capacity.

## Deferred exact author rows (6; no credit claimed)

- `func_001b7110`, `func_001b6df4` (w12a): scene widgets are passed to both the AnimationOwner
  and W6ListController interfaces; no canonical animation/list-owner relation is established.
  No cast or invented base class is credited.
- `func_0015e60c` (w13g): author unsigned-word outputs conflict with the frozen float-output
  caller at `0015e5f0`; natural float-copy and alias-safe memcpy forms do not reproduce under
  the canonical interface.
- `func_001f59b8` (s1f59b8): native MenuWidget/NameTable aliases match, but the canonical
  `FloatPropertyDescriptor` caller does not reproduce.
- `func_001a2fc4` (s1a2fc4): calls `00179d48` through `W6ListController*` while the frozen
  definition takes `AnimationOwner*`; a real common cutover is required.
- `func_001c5a08` (w13e): the author's uchar formal conflicts with the frozen full-word
  forwarding declaration in `001c59fc`; no narrowing caller ABI or register-binding assembly is
  credited.

Reasons are retained in `scratch/integrator/c008/deferred-matches.json`.

## Qualifications

Descriptive names, class identities, unused inputs, indirect callback ownership and C++
interfaces (checked only for class-definition ODR agreement, not types) remain [INFERENCE].
`include/game` headers and C++ units were re-MATCHed together; no private duplicate shared
types remain in candidates. No original bytes, disassembly, packet contents or keys are part of
this record.
