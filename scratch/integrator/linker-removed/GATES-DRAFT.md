# Proposed GATES.md revision: candidate-only removed callees

Draft for the integrator, 2026-10-17, revised after independent re-review F08.
This incorporates the re-reviewer's acceptable GATES text; it is not an adopted gate change.
`docs/GATES.md`, the default v2 model and already prepared/finalized checkpoints stay unchanged.
Use v1.14 only if still unused, otherwise the next unused revision without weakening the text.
A passing independent re-review is required before adoption; implementation tests are not adoption.

## Revision entry to record before a new prepare

- **v1.14** (2026-10-17): G4 may use `armlink5-inline-branchnop-v3`
  (`docs/LINKER.md`, rule R1) for source-defined tiny callees that armlink can
  inline and subsequently remove. All v2 bound-callee equality/refusal checks
  remain mandatory, including for rooted/retained candidate definitions. R1
  excludes every retained/root/kept section, including `__tagsym$$used`.
  Removed helpers earn no catalogue credit; their defining sources, frozen
  profiles and objects remain in the prepared ledger/hash closure, including
  helper-only units with `functions: []`. Receipts expose every removed entity
  and per-call rewrite with `target: null`. G6 requires explicit QUALIFIED
  identity provenance, a visible `[INFERENCE]` source note and manual typed
  contract/control-flow review for each affected credited caller. A helper or
  shared-header revision reopens all dependent credited callers, even when
  their source and output bytes are unchanged. Prepare records added, changed
  and removed project headers as explicit G1 revisions, including in non-R1
  checkpoints; R1 dependency expansion is empty when no R1 caller is affected.
  Cause: ARM DUI0474K §4.12 performs inlining before unused-section elimination,
  while §4.3 separately requires retention of rooted/marked sections. R1 models
  a possible removed helper, not recovered historical call identity.

### R1 — candidate-only inlined and removed callee

R1 is permitted only for a new checkpoint prepared after this revision and a
passing independent implementation review, under the explicitly recorded model
`armlink5-inline-branchnop-v3`. Both fresh G4 builds, prepare and finalized
history must bind the same model. Existing prepared/finalized checkpoints do
not change gates or model. Author-loop opt-in matches alone do not authorize
acceptance; the acceptance build path must actually select and record v3.

A credited caller may use R1 only when all the following conditions hold:

1. The complete callee section is compiled from supplied source under a frozen
   profile in this candidate build, in the caller object or another supplied
   object. Its defining unit remains in the ledger and every receipt with
   source/object hashes even when `functions: []`. An external declaration,
   guessed library body, ROM bytes or fabricated original address is insufficient.
2. No function, alias or other binding gives the section an original address.
   Reserved address names remain reserved. Every binding overlapping a
   catalogue function, entry or extent takes the retained-callee path, retaining
   all v2 equality/refusal checks. Retention metadata never waives those checks
   or by itself proves calls should remain ordinary BLs. An uncatalogued bound
   address is refused. An R1 section is not entry/root/used/retain/exported,
   explicitly kept, first/last or otherwise required to survive removal.
   Unknown supplied retention metadata fails closed; explicit root selectors
   must resolve. Retention propagates to every copy/member of the COMDAT entity.
   Unexamined linker scripts/options are not evidence of removability.
3. The whole section is executable, word-aligned, wholly ARM code, non-NOBITS,
   exactly 4 or 8 bytes, with every function entry at offset zero spanning the
   complete section. Only nonreferencing `R_ARM_V4BX` markers are allowed. Its
   own compiled body satisfies unchanged I1–I3, including all PC/LR/control-
   transfer/exception/branch-NOP exclusions. The non-return instruction reads
   only r0–r3/flags and writes only r0–r3/r12/flags; SP and callee-saved registers
   are preserved. Aliases and identical COMDAT copies are one entity;
   contradictory copies are refused.
4. Every use is an actual compiler-produced relocated ARM BL/BLcond exactly to
   that entry, at catalogued original ARM code, satisfying unchanged I1–I3
   encodings/conditions. The original-code call-ABI CFG filter must pass. Each
   input is defined within the preceding four straight-line words on every
   incoming path, is an unmodified actual incoming caller argument, or is an
   unmodified actual r0/r1 result of a preceding unconditional call on every
   path. Conditional definitions/calls alone do not establish input provenance.
   Entry forwarding does not waive the outgoing check. All decoded successors,
   including both conditional arms and branch targets, are checked for reads of
   r1–r3/r12/flags before unconditional definitions up to calls/returns or decoded
   code-range ends. Unknown indirect control flow fails closed; r0 may be a result
   and I1 requires no inputs. These checks do not prove the argument/result
   types, interprocedural liveness, or that the original source contained a call.
5. At least one call to each removed entity is inlined. No reference survives in
   any supplied allocated section, including otherwise unplaced sections. A
   pointer, tail B, dependency, interior/addended target, incompatible condition
   or invalid call context causes refusal. Only the now-unreferenced unrooted
   helper section is removed; there is no general dead-section elimination or
   credited zero-size helper.
6. Each rewrite records `removed_callee` identity, section, size and defining
   source/object with `target: null`. Match/build receipts list removed entities
   by defining unit. Prepared evidence exposes every relevant caller rewrite and
   binds the full helper definition closure. New/revised helper units, any
   identical COMDAT copy, and added/changed/removed shared-header contracts reopen
   all affected credited callers. Prepare's `dependency_reviews` names each
   caller/source and changed dependency; unchanged callers need explicit rows
   like new/revised functions. Review and finalize refuse missing rows or plain
   PASS provenance for R1 callers. Zero credit does not mean zero review.
7. Mandatory reviewer checks cover every incoming path and entry contract,
   setup dominance, outgoing successors and exits, implicit arguments at later
   calls/tail exits, predicate/flag dependencies, helper definitions and every
   declaration/shared-header use, argument types/widths/meanings, mutability and
   const contracts, memory effects, return/result semantics, and preserved
   original bugs. For preceding-call results, reviewers establish the actual
   return type and register width; BL alone does not prove r0/r1 are results.
   Argcheck must not invent an original callee edge for `target: null`; missing
   original-target evidence is a manual-review obligation, not an ABI-audit pass.
8. Every affected caller has a precise `[INFERENCE]` note at the use/helper
   declaration and an explicit QUALIFIED identity-provenance review row. Exact
   bytes establish the caller extent, not that the original source called this
   helper or that its identity/call graph is recovered. A matching original
   store/arithmetic instruction may have arisen without any helper call. Any
   helper selected to influence code generation still needs a defensible typed
   source-level contract and ordinary source-policy compliance; hand-emitted
   calls/instruction encodings and instruction-injection exceptions remain
   forbidden. Census hits, a clean CFG filter and an issue-free argcheck never
   waive these reviewer checks or qualifications.

## Primary rationale and independent review

[ARM Compiler v5.05 armlink User Guide, DUI0474K §§4.3, 4.12–4.14](https://documentation-service.arm.com/static/5ea19ea09931941038de96d2)
(the document history identifies issue J as v5.04) documents inlining before unused-section
elimination, the tiny-callee rules and retained roots. The pinned game-profile compiler emits
`__tagsym$$used` for the reviewer's exact `__attribute__((used))` reproduction. R1 now refuses that
otherwise eligible section. No real-armlink executable was run, and the model must not claim one.

The initial review is `scratch/review/linker-v3/REVIEW.md`; the independent R2 re-review is
`scratch/review/linker-v3-r2/REVIEW.md`. Both verdicts were FAIL before the current F08 fix.
The rooted-bound shortcut is removed: retention no longer bypasses v2 equality/refusal or
forces an ordinary BL. Matching rooted helpers retain their section, original identity and
comparison. A subsequent passing independent re-review remains required before adoption.

F03 is a **mandatory reviewer check**, not something discharged by the implementation fix.
The re-review compiled nine exact source alternatives under all three frozen profiles whose
controlled reference source never called the helper. Entry argument/result type and width,
interprocedural liveness, and historical-call identity therefore still require the explicit
review and QUALIFIED provenance above. F09's added/removed-header G1 behavior is disclosed in
the revision text. The inherited F10 source/header drift is not fixed or re-verified here;
no passing whole-worktree history verification is claimed.

## Historical triage and author measurements

The pre-review census snapshot reported 5,443 possible sites in 1,753 functions, with 438 sites in
82 functions overlapping 131 `inlin` mismatch rows from 151 author RESULT.json files. Those live
inputs were not frozen; these figures are historical, not today's reproducible counts. The reviewer
observed 5,418 sites / 1,740 functions with 174 author files. Neither count means recovered calls,
and the strengthened CFG filter changes what passes. See `docs/LINKER.md` for the historical command.

The earlier s1ab680 preserved caller/separate-setter trial exited 2: 2,232 candidate bytes versus
2,212 original bytes, with a shifted second call site. It was not a match or new credit. Keep v2
for existing/frozen checkpoints; neither that trial nor this draft authorizes a relaxed call context.

## Historical R2 implementation verification (before F08)

The branch merged current main's independent literal-model implementation and retained both
build-test classes before the final runs:

```
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -m unittest \
  tests.test_inplace tests.test_match tests.test_compid tests.test_argcheck \
  tests.test_accept tests.test_build
```

**Exit 0; 166 tests; OK; no skips.** This includes compiler-produced `used`/exported helper
refusals, explicit and ELF roots, unchanged-caller review closure, COMDAT-copy dependencies,
both reviewer CFG counterexamples, entry forwarding, call results and zero-credit build receipts.

```
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/test_suite.py
```

**Exit 0; `574 tests, 1 skipped, 0 failed modules, 30.3s`.**

All five accepted-function regressions (0x00105084, 0x0010d604, 0x0011e928, 0x0018fdd4,
0x00195254) MATCH complete extents under both final v2 and v3: **10 commands, each exit 0**,
without rewrites. Source paths and invocation pattern are in `docs/LINKER.md`; exact commands
and exits are `scratch/integrator/linker-v3-r2/final-match-commands.json`.

Additional author probes no longer fail the call-context filter but are not complete matches:

- w16b's 0x00112628, 0x00112b5c and 0x001131d4, with separate
  `linear_buffer_allocator.c`: one inline rewrite each, helper removed, **exit 1 each**,
  36/36 bytes, first difference **+0x4**.
- w16a's `variants/func_0010be58_removed.c`, with separate
  `variants/sdk_result_failure_bits.c`: one inline rewrite using the preceding call's result,
  helper removed, **exit 1**, 60/60 bytes, first difference **+0x2f**.

Those variants earn no new matching-C credit. Their unchanged sources, command paths and
scratch output locations are documented in `docs/LINKER.md`. No checkpoint prepare/finalize or
full-ledger build was run. The full suite was explicitly requested; no evidence above replaces
the fresh-build/adoption gates or the required new independent review.

## F08 revision verification (2026-10-17)

The rooted-bound early return is removed from `_inline_decision`. A retained candidate definition
must pass the same v2 bound-original eligibility/equality checks; rooting never forces a BL.
The compiler-produced negative regression uses the reviewer's 24-byte caller, wrong 8-byte
`used` setter and bound 12-byte reference setter. Both v2 and v3 refuse before any caller-only
credit, with zero rewrites/removals. The positive compiled regression retains and compares a
legitimate rooted 8-byte helper equal to its original, with the same ordinary address-targeted
I2 rewrite under both models.

Requested full suite:

```
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/test_suite.py
```

**Exit 0; `576 tests, 1 skipped, 0 failed modules, 30.9s`.** The inplace module ran
**74 tests, no skips**, including the negative and positive real-compile F08 cases.

The five accepted functions `func_00105084`, `func_0010d604`, `func_0011e928`, `func_0018fdd4`
and `func_00195254` each MATCH their full extent under both v2 and v3: **10 commands, each exit 0**,
zero rewrites/removals. W16d `func_001456e4`, with its separate `display_metadata.c`, remains
**exit 0 / MATCH 240/240 bytes** under v3: one `inline_insn` at +0x7c and one removed 8-byte
`w16_set_screen`, with `target: null`.

The exact commands/exits and suite summary are
`scratch/integrator/linker-v3-f08/verification.json`; result files are under that directory's
`accepted/{v2,v3}/FUNCTION/` and `w16d/func_001456e4/`. No checkpoint prepare/finalize,
full-ledger build, known-failing history re-verification or gate/default change was performed.
F03 remains the mandatory reviewer check above, and a passing independent re-review is still
required before adoption.

