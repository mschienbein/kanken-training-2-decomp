# R1 bounded ARM jump-table CFG — proposed GATES revision

**DRAFT — NOT ADOPTED.** Applies to branch `r1-jumptables`, resumed from `cee2571` with its
existing implementation and regression work preserved. Frozen `docs/GATES.md` remains v1.16.
An independent implementation review and the integrator's recorded revision must precede any
new checkpoint using this extension. Never change gates during a checkpoint or rewrite old
prepared/finalized evidence. No armlink execution or new accepted credit is claimed.

The revision number below is proposed only; the integrator assigns the next unused revision
at adoption. The wording of the following normative blocks is the exact proposed text.

## Proposed revision entry

Insert at the top of the revision history and change the document's frozen-version heading
only after independent review and an explicit adoption decision:

```markdown
- **v1.17** (2026-10-17): R1's original-code CFG may resolve the fully bounded unsigned ARM
  compiler jump-table forms described in condition 4 below. The guard compares the unchanged
  index with an immediate, no entry bypasses the guard, and every table/default successor is
  proven inside the caller's catalogued code. Unknown, unbounded, escaping and other computed
  transfers remain refusals. The existing incoming argument-provenance and outgoing
  register/flag checks apply to all enumerated paths. No helper instruction shape, rewrite,
  retention exception, equality relaxation, source/provenance waiver or additional credit is
  introduced. V2 is unchanged. Cause: the 2,212-byte `func_001ab680` candidate was refused on a
  compiler switch CFG even though a separate, explicitly uncredited forced-context experiment
  reproduced its bytes. That experiment is not acceptance evidence. Independent reviewers
  must re-derive the bounded CFG and all existing R1 qualifications before this revision is
  adopted; implementing the decoder or obtaining a private MATCH alone is not adoption.
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

   Only the following bounded ARM switch transfers may resolve an otherwise
   indirect table edge. Let A be the original instruction address and rM a
   non-PC index register. An unconditional immediate CMP rM, #N must guard
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
   original owner's catalogue code/pool ranges. Inspect both incoming argument
   setup and outgoing register/flag liveness across all cases; a helper-free
   case, repeated target, loop, predicated instruction or another table must
   not hide a bypass, read or unknown transfer. Independently exercise the
   positive and negative public linker consumers, including missing/changed
   guards, index/flag changes, entry/branch/table guard bypasses, truncated or
   escaping tables/defaults, and other computed flow. Re-run the five accepted
   call-bearing function regressions under both v2 and v3 and the full suite.
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

Implementation: `tools/inplace.py`; tests: `tests/test_inplace.py`; consumer synchronization:
`research/linker/removed_callee_census.py`; exact explanatory contract: `docs/LINKER.md`,
“Jump tables in the call-context CFG”. Review the complete diff against `cee2571`, including
resumed changes, rather than only the latest session's edits. Seal an independent verdict
before reading another review. Do not copy original bytes, disassembly or packets into Git,
chat or logs.

Use the main repository interpreter from the assigned worktree:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/test_suite.py
```

The main agent runs verification after implementation, serialized with other full builds,
suites and checkpoint work. The editing worker does not run this suite. The relevant new
classes are `JumpTableContextTest` and `SwitchRemovedCalleeTest`; the existing
`ArmlinkModelTest`, `ArmCallAbiContextTest` and `RemovedCalleeModelTest` retain the v2/R1
refusal, equality, rooting, ABI and provenance regressions. Do not pipe a gate; capture each
process exit status.

For each row below, independently execute `tools/match.py SOURCE --profile game --show 0`
with **each** explicit `--linker-model armlink5-inline-branchnop-v2` and
`--linker-model armlink5-inline-branchnop-v3`, using separate outputs under
`scratch/integrator/r1-jumptables/accepted/{v2,v3}/FUNCTION`. Require each full extent to
remain MATCH with no rewrites/removals; record all ten exits and actual receipts.

| Function | SOURCE | Extent |
|---|---|---:|
| `func_00105084` | `src/pilot/loop_resources.c` | 64 B |
| `func_0010d604` | `src/pilot/game_heap.cpp` | 64 B |
| `func_0011e928` | `src/wave1/func_0011e928.c` | 72 B |
| `func_0018fdd4` | `src/wave1/func_0018fdd4.cpp` | 40 B |
| `func_00195254` | `src/wave1/func_00195254.c` | 224 B |

Real r4052 author-loop command (no bypass; capture its actual exit):

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/match.py \
  /home/michael/code/kanken-training-2-decomp/scratch/authors/r4052/src/func_001ab680_removed_setter.c \
  --extra-source /home/michael/code/kanken-training-2-decomp/scratch/authors/r4052/src/removed_content_setter.c \
  --profile game --linker-model armlink5-inline-branchnop-v3 --show 0 \
  --out scratch/out/r4052-v3
```

Observed editing-worker execution after implementation settled: **exit 0; MATCH 2,212/2,212 B**,
two `inline_insn` rewrites at **+0x5a8/+0x6bc**, both `target: null`, and one 8-byte
`bind_content` section removed. No forced-context override was used. The unchanged caller and
helper source/object hashes, and their seven/six compiler-recorded transitive dependencies,
agree with the exact compiler-cache inputs used by this command.

The receipt and private `scratch/integrator/r1-jumptables/verification.json` record the
worker's settled-code execution, result, rewrite/removal count and source/header/object
closure. The pre-existing output was preserved separately; it and the earlier forced-context
experiment are not fresh verification of this implementation. No tests, suite, full build,
accepted-function regressions, prepare/finalize or gate-adoption command was executed by the
editing worker. Exact command vectors for the main agent's ten accepted-function matches and
full suite are in `scratch/integrator/r1-jumptables/regression-commands.json`.

The observed MATCH remains a qualified author candidate, not accepted source credit.
Independent typed-contract/source review and a normally selected, fresh prepared/finalized
checkpoint remain mandatory.
