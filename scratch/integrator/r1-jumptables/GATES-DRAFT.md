# R1 bounded ARM jump-table CFG — effect-allowlisted v4 proposal (r3)

**DRAFT — NOT ADOPTED.** Applies to branch `r1-jumptables`, resumed from `cee2571` with its
existing implementation and regression work preserved. Frozen `docs/GATES.md` remains v1.16.
An independent implementation review and the integrator's recorded revision must precede any
new checkpoint using this extension. Never change gates during a checkpoint or rewrite old
prepared/finalized evidence. No armlink execution or new accepted credit is claimed.

The sealed reviews of `21c2bd4` and `e72fb34` are **FAIL**:
`scratch/review/r1-jumptables/REVIEW.md` (F01/F02) and
`scratch/review/r1-jumptables-r2/REVIEW.md` (F03/F04). R2 fixed immutable selection and SETEND,
but trusted omitted LDREXD/MRRC writes and missed RFE PC/CPSR restoration. R3 uses explicit
instruction-class/encoding effect allowlists for both proofs instead of decoder access lists
or a mnemonic denylist. V4 remains unadopted, so its identifier is unchanged; strict/v2/v3 and
all failed evidence remain frozen. No review pre-approves this successor.

V4 adoption must wait until the in-flight `c008-r2` checkpoint has finalized, then requires
a passing independent implementation/exact-text re-review and a recorded gate revision before
the next relevant prepare. Never change an in-flight checkpoint's interpretation or evidence.

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
  accepts only explicit class/encoding effect models, refusing unknown writes/state restoration.
  Existing all-path incoming/outgoing ABI checks apply to the enumerated graph. Helper shape,
  rewriting, retention, bound-callee equality, full source/object closure and QUALIFIED
  provenance obligations are unchanged. V2 is unchanged. Both fresh receipts, prepare and
  history bind v4 explicitly; transitions reopen affected R1 callers. Cause: compiler switches
  in the 2,212-byte `func_001ab680` candidate, then independent F01/F02 and F03/F04 findings
  on the rejected same-name/endian- and effect-unproved predecessors. Prior forced-context
  and private MATCH evidence is not adoption or accepted credit. Adoption must follow c008-r2
  finalization, passing independent implementation/exact-text review, and the recorded revision.
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

   All words between compare and transfer must be contiguous modeled ordinary
   instructions that cannot change the index or NZCV. Conditional writes are
   possible writes. Only explicit instruction-class and validated A32-encoding
   allowlists may establish these effects. Decode architectural core destinations,
   paired/multiple-load destinations, base writeback and NZCV directly; missing
   decoder writes, update_flags or control-flow groups are never safety evidence.
   An unmodeled or invalid subencoding refuses the window. No call or other
   control transfer intervenes. No catalogue entry, direct B/BL/BLX target or
   otherwise-resolvable table target may enter after the compare through the
   transfer. Re-entry at the compare repeats the bound and is allowed.

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
   conforming call returns. Before either table form resolves, every decoded
   owner instruction, even unreachable, must pass the explicit effect allowlist:
   ordinary integer data-processing/immediate-shift, validated scalar/double/
   register-list memory with every destination/writeback, ordinary multiply,
   scalar VFP data/memory and explicit VMOV/FPSCR-VMRS/FMSTAT transfers, compiler NOP,
   or recognized branch/call/ordinary-return encodings. Each admitted subencoding
   must have modeled core/NZCV and processor-state effects; class membership
   alone is insufficient. Unsupported or invalid subencodings fail closed.
   Ordinary VFP writes to FP registers/FPSCR do not set core NZCV; FPSCR VMRS
   to APSR (FMSTAT) does. Calls retain the manual LE ABI-return contract.

   Generic coprocessor/exclusive instructions, including MRRC and LDREXD with
   any predicate, are not admitted by an empty write list. All RFE variants and
   other exception/status restores are unmodeled, unknown control flow and a
   refusal of the owner-state proof, even when the decoder omits PC/CPSR writes
   or branch groups. SETEND/MSR/MCR/CPS are likewise outside the allowlist.
   This rule includes instructions before the compare, on one predecessor or
   unreachable, and apparently LE-restoring instructions. Conservative false
   negatives are intentional; no interprocedural processor-state recovery is claimed.

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
   contract; index/NZCV stability alone is insufficient. Check architectural
   register-pair/list and base-writeback effects without trusting missing decoder
   writes, and explicitly refuse exception-return PC/CPSR restoration. Inspect
   incoming setup and outgoing liveness across all cases; no helper-free case,
   repeated target, loop, predication or other table may hide a bypass or read.
   Exercise public consumers for LDREXD/predicated LDREXD/MRRC, modeled pair/
   multiple loads and writeback into the index/base, and every ARM RFE addressing
   form with SP/r4 frames in the window, before the guard, on one predecessor
   and unreachable. Exercise the decoder-ID meta-test: every emitted class is
   explicitly allowlisted with validated effects or refused, including with absent
   access metadata. Retain missing/changed guard, index/flag, entry-bypass,
   endian/state, truncation, escaping-target/default and other computed-flow cases.
   Require frozen v3 refusal and selected v4 positives, all five accepted-function
   regressions under v2/v3/v4, the full suite, CLI/receipt/model-payload bindings
   and unchanged-R1 transition review. No mechanical result waives typed review.
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
not this full suite. Relevant classes are `JumpTableContextTest`, `BoundedSwitchEffectsTest`,
`BoundedSwitchModelTest` and `SwitchRemovedCalleeTest`; retained `ArmlinkModelTest`, `ArmCallAbiContextTest` and
`RemovedCalleeModelTest` cover frozen v2/v3 safeguards. CLI/model-payload and acceptance-transition
tests additionally cover selected v4. Do not pipe a gate; capture every process exit.

For each row below, execute `tools/match.py SOURCE --profile game --show 0` with **each**
explicit model `armlink5-inline-branchnop-v2`, `armlink5-inline-branchnop-v3`, and
`armlink5-inline-branchnop-v4`. Use separate outputs under
`scratch/integrator/r1-jumptables/r3/accepted/{v2,v3,v4}/FUNCTION`. Require each full extent
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
  --out scratch/out/r4052-v4-r3
```

Historical evidence is retained, not re-labelled: `21c2bd4`'s worker observed a normal-filter
2,212/2,212 B MATCH with two rewrites, but under the incorrectly expanded v3 identifier.
Its source/header/object closure is in `scratch/integrator/r1-jumptables/verification.json`.
Main subsequently reported 652 tests (1 skipped, no failed modules) and ten exact v2/v3
accepted-function regressions for that rejected snapshot. These do not verify corrected v4.
The sealed F01/F02 counterexamples are ground truth and were not rerun on unchanged old code.

Corrected command vectors and focused-smoke/source-closure evidence are kept separately under
`scratch/integrator/r1-jumptables/r3/`, with the r3 receipt in `scratch/out/r4052-v4-r3`.
No old result, review, selection or checkpoint is overwritten. A MATCH remains a qualified
author candidate, not historical helper identity or accepted credit. New independent
implementation/text review and a normally selected, fresh prepared/finalized checkpoint are
still mandatory. No armlink execution or gate adoption is authorized by this draft.

## Historical failed-r2 smoke evidence (`e72fb34`)

Before F03/F04 were corrected, the r2 focused smoke exited **0: 53 tests, OK**. Its public consumers
require v4 positives and frozen-v3 refusals, reject the reported endian case and before-guard/
single-predecessor/unreachable state changes, and exercise compiled switches, legacy ordinary
helpers, CLI/model payloads, registry receipt binding and v3→v4 transition reviews.

That r2 v4 command exited **0: MATCH 2,212/2,212 B**, with exactly two
`inline_insn` rewrites at **+0x5a8 / +0x6bc**, both `target: null`, and one 8-byte
`bind_content` removed. Caller/helper sources and objects and their seven/six recorded header
dependencies agree with the exact compiler-cache inputs. The new receipt is
`scratch/out/r4052-v4/func_001ab680_removed_setter.result.json`; command, exit, hashes and closure
are recorded in `scratch/integrator/r1-jumptables/r2/verification.json`.

No full suite, full build, fifteen accepted-function regressions, prepare/finalize or gate
adoption was executed by the editing worker for r2. The exact new Main commands are in
`scratch/integrator/r1-jumptables/r2/regression-commands.json`. The old F01/F02 failures were not
rerun on unchanged code. These focused results are not an independent PASS or adoption.

Main's r2 evidence later supplied 661 tests (1 skipped, zero failed modules) and fifteen exact
accepted-function regressions. Its sealed review independently bound those records; they
apply only to `e72fb34`, not to the r3 correction. F03/F04 observations were taken as ground
truth and were not rerun against unchanged old code. All r2 evidence remains immutable.

## R3 focused smoke and source closure

The requested settled-code focused smoke exited **0: 58 tests, OK**. It includes the exhaustive
decoder-ID gate meta-test, independent architectural destination/writeback/NZCV vectors with
access metadata absent, public LDREXD/predicated-LDREXD/MRRC negatives, modeled pair/list/base
writes, every RFE addressing form with SP/r4 frames and all four state-path placements, compiled
switch positives and retained frozen/model-selector tests.

An earlier focused attempt safely refused the legitimate FPSCR-to-NZCV transfer because the
decoder calls it FMSTAT, not VMRS. Its failure/output/input hashes remain in the new evidence;
the explicit FPSCR-only alias effect model fixed the cause. No expected result was narrowed,
and no reported F03/F04 failure was rerun on unchanged old code.

The r3 real-source command above exited **0: MATCH 2,212/2,212 B**, with two null-target
`inline_insn` rewrites at **+0x5a8 / +0x6bc** and one 8-byte helper removed. Both unchanged
sources/objects and their seven/six recorded header dependency hashes agree with the actual
compiler-cache inputs. Evidence is `scratch/integrator/r1-jumptables/r3/verification.json`;
the receipt is `scratch/out/r4052-v4-r3/func_001ab680_removed_setter.result.json`.

The Main command manifest is `scratch/integrator/r1-jumptables/r3/regression-commands.json`.
No full suite/build, fifteen accepted-function regressions, prepare/finalize, armlink execution
or gate adoption was run by this worker for r3. Passing independent implementation/exact-text
re-review and the stated post-c008-r2 adoption sequence remain mandatory.
