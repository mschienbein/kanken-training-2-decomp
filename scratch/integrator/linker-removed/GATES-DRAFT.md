# Proposed GATES.md revision: candidate-only removed callees

Draft for the integrator, 2026-10-17, revised after independent review F01–F03.
This is not an adopted gate change. `docs/GATES.md`, the default v2 linker model and already
prepared checkpoints remain unchanged. Use the next unused revision number on adoption;
v1.14 below is proposed, not reserved. Passing implementation tests does not adopt R1.

## Revision entry to record before a new prepare

- **v1.14 (proposed)** (2026-10-17): G4 may use `armlink5-inline-branchnop-v3`
  (`docs/LINKER.md`, rule R1) for source-only tiny callees that armlink can inline and remove.
  All v2 rules/refusals remain. R1 excludes every unremovable/root/kept section, including a
  section defining `__tagsym$$used`. Removed helpers earn no catalogue credit. Their complete
  defining sources, compiler profiles and objects remain in the prepared ledger/hash closure,
  even for helper-only units with `functions: []`. Receipts expose every removed entity and
  per-call rewrite with `target: null`. G6 requires manual contract/control-flow review, a
  visible `[INFERENCE]` source note and QUALIFIED identity provenance for every credited R1
  caller. Changing a removed helper's defining unit or reviewed shared-header contract reopens
  all affected credited callers, even unchanged caller sources. An empty or unqualified
  helper-only dependency review is refused. Cause: ARM DUI0474K §4.12 performs inlining before
  unused-section elimination, while §4.3 still retains rooted/marked sections. R1 models a
  possible removed helper, not recovery of its historical identity or original call graph.

## R1 — candidate-only inlined and removed callee

R1 is allowed only under the explicitly recorded model `armlink5-inline-branchnop-v3`, for a
new checkpoint prepared after adoption of this revision and a passing independent implementation
review. Both fresh G4 builds, prepare and finalized history must bind the same model. V2 remains
the default; no already-prepared checkpoint changes gates/model mid-review. Opt-in author matches
alone do not authorize acceptance.

A caller may receive R1-qualified catalogue credit only when all of the following hold:

1. The complete callee function section is compiled from source supplied by this candidate
   build, in the caller object or another supplied object. Its defining unit remains in the
   ledger and every receipt with source/object hashes, even when it credits no functions.
   An external declaration, guessed library body, ROM bytes or fabricated address is insufficient.
2. No function, alias or other symbol binding gives the section an original address. Reserved
   address names remain reserved. A binding overlapping any catalogue function/entry/extent
   takes v2's retained-callee path, with its equality/refusal checks; an uncatalogued bound
   address is refused. The section is not an entry/root, `used`/retain-marked, explicitly kept,
   first/last, exported or otherwise required to survive removal. A definition of
   `__tagsym$$used` retains its section. The implementation conservatively retains unknown
   `__tagsym$$` metadata, OS/processor section flags, explicit exported symbol visibility,
   uninterpreted linker directives and ELF entry metadata. Supplied root selectors must resolve
   to candidate symbols/sections; unknown selectors fail closed. Retained COMDAT groups stay
   retained. A rooted unbound helper is refused, never silently dropped; a bound rooted
   candidate's call stays a BL under v3 and therefore mismatches an inlined original slot.
   External KEEP/entry/export/first/last mechanisms must be supplied as explicit retention roots;
   unexamined linker scripts/options are not supported evidence of removability.
3. The whole section is executable, word-aligned, wholly ARM code, non-NOBITS, exactly 4 or 8
   bytes, with every function entry at offset zero spanning the complete section. Only the
   nonreferencing `R_ARM_V4BX` marker is permitted. Its own compiled body satisfies unchanged
   I1–I3, including the PC/LR/control-transfer/exception/branch-NOP exclusions. Its non-return
   instruction reads only r0–r3/flags and writes only r0–r3/r12/flags, preserving SP and all
   callee-saved registers. All aliases/identical COMDAT copies represent the same entity;
   contradictory copies are refused.
4. Every use is an actual compiler-produced relocated ARM BL/BLcond exactly to the entry, at
   original catalogued ARM code, with unchanged I1–I3 encodings/conditions. The original-code
   call-ABI CFG filter must pass. Each r0–r3 input is defined within the preceding four
   straight-line words on every incoming path, or is the caller's own incoming argument
   register unmodified on every CFG path from entry to the site, or is an unmodified r0/r1
   result of an unconditional preceding call on every incoming path. Branches cannot bypass
   required setup; calls break incoming-parameter provenance but establish possible result
   registers. Conditional definitions/calls alone do not establish an input. Reviewers must
   check the actual preceding-call result type/width; BL alone does not prove its contract.
   After the replacement, follow
   all CFG successors, including both conditional arms and unconditional branch targets,
   until calls/returns, code-range ends or unconditional redefinition of the tracked values.
   No r1–r3/r12/flags read may precede an unconditional definition; conditional definitions
   do not kill a value on every path. Unresolved indirect successors fail closed. r0 may be
   a result; I1 requires no inputs. Entry-argument forwarding does not waive the mandatory
   post-site caller-saved/flags check. Ordinary source policy still forbids hand-emitted
   call/instruction encodings.
5. At least one call to each removed entity is inlined and no reference remains in any supplied
   allocated section, including otherwise unplaced sections. A pointer, B tail call, dependency,
   interior/addended target, incompatible condition or invalid ABI context causes refusal.
   Only the now-unreferenced, unrooted helper section is removed. This is not general dead-section
   elimination and does not create a credited zero-size helper.
6. Every rewrite records `removed_callee` (symbols, section, size and defining source/object)
   and `target: null`. Match/build receipts list removed entities by defining unit. Prepared
   evidence binds the full helper definition closure and exposes relevant caller rewrites.
   A new/revised removed helper or reviewed shared-header contract makes all affected credited
   callers review-required, even when caller text and output bytes are unchanged. Prepare's
   `dependency_reviews` lists those callers and changed helpers/headers, and `linker_rewrites`
   includes their per-call metadata. Each unchanged caller needs its own explicit function/source
   row, just like a new/revised function. The review schema and finalize refuse missing rows;
   a zero-credit helper is not a zero-review dependency. R1 caller rows must be QUALIFIED (or FAIL).
   Revisions of any identical removed COMDAT copy's defining unit reopen the same callers.
7. **Mandatory manual ABI/contract review:** reviewers inspect every incoming path and entry
   argument, setup dominance, all outgoing successors, predication and flag dependencies,
   helper definition, every declaration, shared-header use, argument types/meanings,
   mutability/const contracts, memory effects, return/result semantics and preserved original
   bugs. The automatic filter only bounds the decoded CFG; it does not replace these checks.
   Argcheck must not invent an image callee edge from `target: null`. Missing original-target
   evidence is an explicit manual-review obligation, not an ABI-audit pass or evidence of no
   arguments/effects.
8. **Mandatory historical-identity qualification:** every affected caller has a precise
   `[INFERENCE]` note at the use/helper declaration and a QUALIFIED review row identifying
   removed-helper identity provenance. Byte equality proves the emitted caller extent, not
   that the original source contained that helper or that its historical identity/call graph
   has been recovered. A helper chosen to influence code generation still needs a defensible
   source-level contract and ordinary source-policy compliance. R1 is not an instruction-injection
   exception. A census hit, clean CFG filter or argcheck report with no issues never waives this.

## Primary rationale and independent review

[ARM Compiler v5.05 armlink User Guide, DUI0474K §§4.3, 4.12–4.14](https://documentation-service.arm.com/static/5ea19ea09931941038de96d2)
(the document history identifies issue J as v5.04) documents inlining before unused-section
elimination, the tiny-callee rules and retained roots. The pinned game-profile compiler emits
`__tagsym$$used` for the reviewer's exact `__attribute__((used))` reproduction. R1 now refuses that
otherwise eligible section. No real-armlink executable was run, and the model must not claim one.

The independent review is `scratch/review/linker-v3/REVIEW.md`: its verdict was FAIL before these
fixes. A subsequent independent passing review is still required before adoption. F03's identity
qualification is inherent and is not mechanically discharged by the retention/dependency/CFG fixes.

## Historical triage and author measurements

The pre-review census snapshot reported 5,443 possible sites in 1,753 functions, with 438 sites in
82 functions overlapping 131 `inlin` mismatch rows from 151 author RESULT.json files. Those live
inputs were not frozen; these figures are historical, not today's reproducible counts. The reviewer
observed 5,418 sites / 1,740 functions with 174 author files. Neither count means recovered calls,
and the strengthened CFG filter changes what passes. See `docs/LINKER.md` for the historical command.

The earlier s1ab680 preserved caller/separate-setter trial exited 2: 2,232 candidate bytes versus
2,212 original bytes, with a shifted second call site. It was not a match or new credit. Keep v2
for existing/frozen checkpoints; neither that trial nor this draft authorizes a relaxed call context.

## R2 implementation verification (2026-10-17)

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
