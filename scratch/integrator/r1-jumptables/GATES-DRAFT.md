# R1 bounded ARM jump-table CFG — corrected v4 proposal (r2)

**DRAFT — NOT ADOPTED.** Applies to branch `r1-jumptables`, resumed from `cee2571` with its
existing implementation and regression work preserved. Frozen `docs/GATES.md` remains v1.16.
An independent implementation review and the integrator's recorded revision must precede any
new checkpoint using this extension. Never change gates during a checkpoint or rewrite old
prepared/finalized evidence. No armlink execution or new accepted credit is claimed.

The sealed review of `21c2bd4` is **FAIL** (`scratch/review/r1-jumptables/REVIEW.md`):
F01 requires a new immutable selected model; F02 requires little-endian proof for LDR targets.
The corrected policy is `armlink5-inline-branchnop-v4`. Frozen v3 behavior and all old evidence
are preserved. This draft is not pre-approved by that failed review; a new independent
implementation/text review is required.

The revision number below is proposed only; the integrator assigns the next unused revision
at adoption. The wording of the following normative blocks is the exact proposed text.

## Proposed revision entry

Insert at the top of the revision history and change the document's frozen-version heading
only after independent review and an explicit adoption decision:

```markdown
- **v1.17** (2026-10-17): the new immutable model `armlink5-inline-branchnop-v4` may resolve
  R1's fully bounded unsigned ARM compiler jump-table forms described in condition 4.
  Frozen `armlink5-inline-branchnop-v3` retains its original CFG policy, caller decoding and
  return classification; this revision never broadens or reinterprets that identifier.
  V4 proves the same-index immediate bound, no guard bypass, complete owned table/default
  successors, and little-endian data interpretation on every path to an address-table load.
  Unknown, unbounded, escaping and other computed transfers remain refusals. The implementation
  conservatively rejects owner state changes rather than guessing restored endian state.
  Existing all-path incoming/outgoing ABI checks apply to the enumerated graph. Helper shape,
  rewriting, retention, bound-callee equality, full source/object closure and QUALIFIED
  provenance obligations are unchanged. V2 is unchanged. Both fresh receipts, prepare and
  history bind v4 explicitly; transitions reopen affected R1 callers. Cause: compiler switches
  in the 2,212-byte `func_001ab680` candidate, followed by independent F01/F02 findings on the
  rejected same-name/endian-unproved implementation (`21c2bd4`). Prior forced-context matching
  and private author-loop MATCH results are not adoption or accepted credit. A new independent
  review and this recorded revision must precede any relevant prepare.
```

## Proposed model-selection addition after the R1 introduction

```markdown
The bounded-switch CFG proof policy is selected only by
`armlink5-inline-branchnop-v4`. Frozen `armlink5-inline-branchnop-v3` records
retain their original meaning; this revision does not broaden or reinterpret
that identifier. Selection, both fresh receipts, prepare and finalized history
bind the new identifier, and transition review reopens all affected R1 callers
as required by the existing selector. Strict/v2/v3 behavior is not changed.
```

## Proposed replacement for R1 condition 4

```markdown
4. Every use is an actual compiler-produced relocated ARM BL/BLcond exactly to
   that entry, at catalogued original ARM code, satisfying unchanged I1–I3
   encodings/conditions. The original-code call-ABI CFG filter must pass. Each
   input is defined within the preceding four straight-line words on every
   incoming path, is an unmodified actual incoming caller argument, or is an
   unmodified actual r0/r1 result of a preceding unconditional call on every
   path. Conditional definitions/calls alone do not establish input provenance.
   Entry forwarding does not waive the outgoing check. All decoded successors,
   including both conditional arms and every resolved branch/table target, are
   checked for reads of r1–r3/r12/flags before unconditional definitions up to
   calls/returns or decoded code-range ends. r0 may be a result and I1 requires
   no inputs. These checks do not prove the argument/result types,
   interprocedural liveness, or that the original source contained a call.

   Only explicitly selected v4 may resolve the following bounded ARM switch
   transfers as otherwise indirect table edges. Let A be the original instruction
   address and rM a non-PC index register. An unconditional immediate CMP rM, #N must guard
   ADDLS pc, pc, rM, LSL #2 (branch table) or
   LDRLS pc, [pc, rM, LSL #2] (absolute-address table).
   The unsigned LO forms are also allowed: LS enumerates exactly indices
   0..N, LO exactly 0..N-1, with at least one entry. The immediate uses ARM's
   decoded rotate encoding. Neither table contents nor apparent table length
   supplies a missing bound. Other predicates, signed comparisons, register
   bounds, scales/bases, writeback, subtractive indexing and unguarded or
   otherwise computed PC writes are not resolved.

   All words between the compare and transfer must be contiguous decoded
   ordinary instructions that cannot change the same index or condition
   flags. Conditional writes are possible writes; explicit flag-update
   metadata and system/VFP flag transfers must be checked in addition to
   register-access metadata. No call or other control transfer intervenes.
   No catalogue entry, direct B/BL/BLX target or target of any otherwise
   resolvable table may enter after the compare through the transfer.
   A path may re-enter at the compare because it repeats the bound.

   Every slot is at A + 8 + 4*i. A branch-table slot is an unconditional
   direct ARM B whose destination is decoded caller code; the CFG visits
   both the entry B and that destination. An address-table slot is a complete
   aligned absolute-address word in this caller's catalogued pool/literal
   data, and its destination is decoded caller code. Instructions and pool
   words are limited to their declared ranges clipped to the owning
   function's complete extent. Pool/data bytes cannot supply instructions,
   and code bytes cannot supply address-table words. Image search, neighboring
   owners, candidate relocations and arbitrary memory reads are not witnesses.
   Thumb, misaligned, data, undecoded and out-of-extent table destinations fail.

   For an LDR address table, little-endian data interpretation at the load must
   be established on every incoming path. A reachable SETEND or other unresolved
   change of data-endianness state is a refusal unless that state is independently
   proved little-endian at the load. A change before the comparison is subject to
   the same requirement; preserving the index and NZCV flags alone is insufficient.
   The target uses the little-endian public ARM ABI at catalogue entries and
   conforming call returns. This implementation conservatively resolves no table
   in an owner containing any decoded SETEND, MSR, MCR/MCRR (including suffixed
   variants) or CPS instruction, even when unreachable or apparently restoring LE.
   It also rejects flag-only MSR and non-system MCR rather than infer their state
   effects. These conservative false negatives do not authorize an endian-state
   bypass or claim interprocedural processor-state recovery.

   The not-taken successor at A + 4 is an unconditional direct branch to
   decoded caller code or an ordinary recognized return; it cannot fall into
   the table or leave the caller through an unexamined default branch. Ordinary
   recognized returns are BX lr, plain MOV pc, lr, and
   LDMIA sp!, {..., pc}/POP. Merely reading LR while writing PC, generic decoder
   return groups, exception-return encodings and arbitrary PC loads do not
   establish returns. Predicated recognized returns elsewhere retain their
   conditional fall-through paths. The CFG includes the default and every
   bounded slot; repeated address targets may share a successor but no
   distinct path may be omitted. Every reachable unknown indirect transfer,
   including any rejected table pattern, still fails closed. No function-name,
   address-specific, forced-consistent or other input-specific exception is
   permitted. This is bounded control-flow recovery, not evidence of a
   historical helper identity or an execution of armlink.
```

## Proposed replacement for R1 condition 7

```markdown
7. Mandatory reviewer checks cover every incoming path and entry contract,
   setup dominance, outgoing successors and exits, implicit arguments at later
   calls/tail exits, predicate/flag dependencies, helper definitions and every
   declaration/shared-header use, argument types/widths/meanings, mutability and
   const contracts, memory effects, return/result semantics, and preserved
   original bugs. For preceding-call results, reviewers establish the actual
   return type and register width; BL alone does not prove r0/r1 are results.
   For every resolved switch, independently re-derive the same-index immediate
   comparison, decoded unsigned bound and exact entry count, preservation of
   index/flags, absence of alternate entries after the guard, every table slot
   and destination, the default edge and every subsequent path using only the
   original owner's catalogue code/pool ranges. Establish little-endian data
   interpretation on every incoming path to each LDR table, including changes
   before the guard or on only one predecessor, and the entry/call ABI state
   contract; index/NZCV stability alone is insufficient. Inspect both incoming
   argument setup and outgoing register/flag liveness across all cases; a
   helper-free case, repeated target, loop, predicated instruction or another
   table must not hide a bypass, read or unknown transfer. Independently exercise
   positive and negative public linker consumers, including missing/changed
   guards, index/flag changes, entry/branch/table guard bypasses, endian/status/
   system-state changes, truncated or escaping tables/defaults, and computed
   flow. Require frozen v3's table refusal and selected v4's positive behavior.
   Re-run the five accepted call-bearing function regressions under v2, frozen
   v3 and selected v4, plus the full suite. Check every CLI/receipt/model-payload
   selection and the unchanged-R1 transition review when v3 changes to v4.
   Argcheck must not invent an original callee edge for target: null; missing
   original-target evidence is a manual-review obligation, not an ABI-audit pass.
```

Conditions 1–3, 5–6 and 8 remain **verbatim**. In particular, every rooted/retained helper
remains excluded from removal, every bound candidate retains v2 identity/equality checks,
all surviving references are refusals, helper-only/COMDAT/header source closure stays bound,
and every affected caller still needs the visible `[INFERENCE]` note and explicit QUALIFIED
identity-provenance row. Exact matching and bounded CFG traversal do not prove that the
original source contained a helper call. Model selection/receipt/history binding is unchanged;
this draft does not opt any checkpoint into a new gate or amend existing selection records.

## Independent review inputs and commands

Implementation: `tools/inplace.py`, `tools/linker_models.py`, registry binding in `tools/build.py`;
payload/selector regressions: `tests/test_inplace.py`, `tests/test_match.py`,
`tests/test_build.py`, `tests/test_compid.py`, `tests/test_accept.py`; diagnostic consumer:
`research/linker/removed_callee_census.py`; contracts: `docs/LINKER.md` and
`docs/ACCEPTANCE.md`. Review the complete diff against `cee2571`, including the frozen-policy
restoration and all resumed changes, not merely the r2 delta against the failed `21c2bd4`.
Seal an independent verdict before reading another review. Do not copy original bytes,
disassembly or packets into Git, chat or logs.

Use the main repository interpreter from the assigned worktree:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/test_suite.py
```

Main runs full verification after implementation, serialized with full builds, suites and
checkpoint work. The editing worker performs only the requested focused settled-code smoke,
not this full suite. Relevant new classes are `JumpTableContextTest`, `BoundedSwitchModelTest`
and `SwitchRemovedCalleeTest`; retained `ArmlinkModelTest`, `ArmCallAbiContextTest` and
`RemovedCalleeModelTest` cover frozen v2/v3 safeguards. CLI/model-payload and acceptance-transition
tests additionally cover selected v4. Do not pipe a gate; capture every process exit.

For each row below, execute `tools/match.py SOURCE --profile game --show 0` with **each**
explicit model `armlink5-inline-branchnop-v2`, `armlink5-inline-branchnop-v3`, and
`armlink5-inline-branchnop-v4`. Use separate outputs under
`scratch/integrator/r1-jumptables/r2/accepted/{v2,v3,v4}/FUNCTION`. Require each full extent
to remain MATCH with no rewrites/removals; record all fifteen exits and actual receipts.

| Function | SOURCE | Extent |
|---|---|---:|
| `func_00105084` | `src/pilot/loop_resources.c` | 64 B |
| `func_0010d604` | `src/pilot/game_heap.cpp` | 64 B |
| `func_0011e928` | `src/wave1/func_0011e928.c` | 72 B |
| `func_0018fdd4` | `src/wave1/func_0018fdd4.cpp` | 40 B |
| `func_00195254` | `src/wave1/func_00195254.c` | 224 B |

Corrected real r4052 author-loop command (selected v4, no bypass; capture its actual exit):

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/match.py \
  /home/michael/code/kanken-training-2-decomp/scratch/authors/r4052/src/func_001ab680_removed_setter.c \
  --extra-source /home/michael/code/kanken-training-2-decomp/scratch/authors/r4052/src/removed_content_setter.c \
  --profile game --linker-model armlink5-inline-branchnop-v4 --show 0 \
  --out scratch/out/r4052-v4
```

Historical evidence is retained, not re-labelled: `21c2bd4`'s worker observed a normal-filter
2,212/2,212 B MATCH with two rewrites, but under the incorrectly expanded v3 identifier.
Its source/header/object closure is in `scratch/integrator/r1-jumptables/verification.json`.
Main subsequently reported 652 tests (1 skipped, no failed modules) and ten exact v2/v3
accepted-function regressions for that rejected snapshot. These do not verify corrected v4.
The sealed F01/F02 counterexamples are ground truth and were not rerun on unchanged old code.

Corrected command vectors and focused-smoke/source-closure evidence are kept separately under
`scratch/integrator/r1-jumptables/r2/`, with the real v4 receipt in `scratch/out/r4052-v4`.
No old result, review, selection or checkpoint is overwritten. A MATCH remains a qualified
author candidate, not historical helper identity or accepted credit. New independent
implementation/text review and a normally selected, fresh prepared/finalized checkpoint are
still mandatory. No armlink execution or gate adoption is authorized by this draft.

## Corrected settled-code smoke evidence

The requested focused post-fix smoke exited **0: 53 tests, OK**. Its public linker consumers
require v4 positives and frozen-v3 refusals, reject the reported endian case and before-guard/
single-predecessor/unreachable state changes, and exercise compiled switches, legacy ordinary
helpers, CLI/model payloads, registry receipt binding and v3→v4 transition reviews.

The corrected v4 command above exited **0: MATCH 2,212/2,212 B**, with exactly two
`inline_insn` rewrites at **+0x5a8 / +0x6bc**, both `target: null`, and one 8-byte
`bind_content` removed. Caller/helper sources and objects and their seven/six recorded header
dependencies agree with the exact compiler-cache inputs. The new receipt is
`scratch/out/r4052-v4/func_001ab680_removed_setter.result.json`; command, exit, hashes and closure
are recorded in `scratch/integrator/r1-jumptables/r2/verification.json`.

No full suite, full build, fifteen accepted-function regressions, prepare/finalize or gate
adoption was executed by the editing worker for r2. The exact new Main commands are in
`scratch/integrator/r1-jumptables/r2/regression-commands.json`. The old F01/F02 failures were not
rerun on unchanged code. These focused results are not an independent PASS or adoption.
