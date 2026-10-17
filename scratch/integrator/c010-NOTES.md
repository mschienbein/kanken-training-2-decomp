# C010 integration — unaccepted milestone

This is an integrator working record, not acceptance or an independent review.
No full build, full suite, prepare, finalize, history edit or push has run here.
All counts below are candidate scope/evidence, never accepted credit.

## Base and frozen selection

- Worktree `scratch/worktrees/c010`, branch `c010-integrate`, created from
  c009 tip `9d6f0515230d641f51d91dc9ab4b3ffb14011c05`.
- Merged c009 collection milestone `b63317e`; c009 remains under remediation.
  It must be merged again at later milestones and when its owner declares final.
- Gates v1.16; frozen profiles only; explicit intended checkpoint selection is
  `armlink5-inline-branchnop-v2` / `none`. No compiler/profile, denominator,
  gate, accepted receipt or history mutation is proposed.
- `orig`, `.cache/toolchain` and `.venv` resolve to the main checkout's private
  inputs/toolchain/interpreter. Git author is mschienbein.

## Complete source/evidence scope

`c010/SCOPE.json` freezes the escalation ledger hash, all selected author
source/receipt bindings and alternatives, and 272 initially enumerated result
manifests (r40*, r41*, tu*, v9a*, L042 and the published permuter sweep).
The ledger's `author_match_unaccepted` state supplied the starting list;
accepted and c006-c009 manifest ownership were excluded before selection.
Every selected source hash equals its retained MATCH receipt's source hash,
and its receipt selects a frozen profile and default v2/none.

The initial union contains **628 unique functions / 620 source files**.
After the c009 milestone, exact row dispositions are:

| Disposition | Functions |
| --- | ---: |
| Initial destination collection; canonical integration still required | 440 |
| Actual collector refusal still requiring disposition | 163 |
| Explicit predecessor contract-resolution deferral | 19 |
| Added to c009, therefore excluded from c010 | 6 |

The six new c009 owners are 002095c0, 002102b4, 0014d2e4, 001eade0,
0013156c and 001f4cf8. The ten additional c009 contract deferrals are retained
verbatim from `c009/collection-closure-r1.json`; the nine earlier intersecting
c008/c009 deferrals also remain excluded. No source, old receipt or historical
failed result was overwritten. `scope-dispositions-r1.json` records every row.
This is a point-in-time snapshot; subsequently published results must be
reconciled before the final scope is sealed.

`MODEL-QUALIFIED-EXCLUSIONS.json` separately lists **46 nondefault-model
MATCH witnesses / 21 unique functions** found through exact receipt paths:
45 v3 records and one v4 record. No v5 MATCH was observed in that receipt
inventory. These witnesses are for a later model-selected checkpoint, not
permission to select a private model for c010. A function with a separate
qualified default-v2 witness is labelled explicitly rather than losing it.

## Milestone 1 — destination collection

`tools/collect.py` ran twice sequentially on filtered, exact source rows:
611 source files / 619 functions after the original nine contract exclusions.
Both collector commands exited 0, while preserving **178 refused units**;
collector exit 0 is not an all-input success claim. The copied proposal is
**433 units / 440 functions / 48,912 target bytes**:
312 game, 43 sdk and 78 middleware units. The second destination partition
preserves a basename collision without renaming or losing either source.

Exact commands, exits and refusal text are in `collect-r1-part*.json`;
filtered per-source inputs retain original author/record/receipt paths.
The 40 compile refusals contain the collector's first-line diagnostic only;
that is a tool-output limitation, not an asserted compiler-contract diagnosis.
Unresolved names, unbound definitions and compile refusals stay open until
specific source/interface evidence resolves them or justifies a deferral.
Two hundred ten collected units still have INTEGRATOR markers; none is
represented as canonical or review-ready. Collector receipts predate final
parent/canonical edits. The shared SVC revision landed during this initial
collection phase, so no stable whole-phase header closure or final MATCH
sweep is claimed. Final individual matching must cover all included units.

`config/candidates/c010.json` is the unaccepted collected proposal.
`dry-matches-r1.json` extends the c009 milestone's dry ledger without changing
`config/matches.json`. `HANDOFF.json` binds the current live source/header/
configuration hashes and the exact unclosed evidence obligations.

## Names and source policy

`qualify_names.py` reads only hash-bound candidate ELF metadata and exact
receipt/overlay paths, not original bytes. It found 717 mechanically qualified
name proposals, zero address conflicts and 11 unavailable/changed-evidence
issues. A proposal is eligible only through an actual emitted same-address
self-definition or a preserved natural caller relocation, excluding every
rewritten/erased call site. This is **not adoption**: no new names overlay
entry has been added, and canonical owner, input/result and callback contracts
still need adjudication. Descriptive and native spelling remain INFERENCE.

No assembly, register forcing, volatile/dead-code steering, data blobs,
fabricated callees, callback casts or type aliases were introduced to recover
a match. Existing author bytes and all failures remain at their recorded paths.

## Reviewed SVC header revision

The exact reviewed `include/ctr/svc.h` revision from `9ffdbbe` + `dd5fa8c`
has been applied as a **proposed revision of an accepted header**, not an
unrecorded acceptance. Review `scratch/review/svc-hole/REVIEW.md` is
PASS_WITH_QUALIFICATIONS. `applet.h` also gains the correctly typed
`CtrResult func_00115b0c(void)` declaration, coordinated with c009.
Collection and final matching of the reviewed sdk `func_0019ecf8` source
remain outstanding; it is not yet in the candidate manifest.

Mandatory qualifications: the initialized aligned low-word object and signed
high-word split are pinned ARMCC ABI evidence, not a portable scalar ABI;
the shared __svc use needs a QUALIFIED caller row and checkpoint disclosure;
historical SDK spelling/ownership are unrecovered. The seven mismatching
proposal callers receive no credit. Future prepare must bind the revised
accepted header, whole-ledger G3 and both fresh G4 receipts. No old snapshot
or G8 result may be suppressed or rewritten.

## Canonicalization ownership and remaining evidence boundary

FinishC009b still owns active existing game-header/name remediation. It has
released the narrowly coordinated region_heap/001467e4, sound_runtime/00180768,
and font_resource/001db130 families to c010; applet's one declaration addition
was separately approved. C010 must not overwrite c009's input/audio/screen/
startup remediation. The c008-r3 callback fixes arrive through the next c009
merge rather than being duplicated here.

The reusable canonical probes are `c008-r2/probe_contracts.py` and
`probe_signatures.py`; `c010/probe_scope.py` adapts their APIs to the c010
manifest without editing them. FixC008R3's indirect-call auditor is
`scratch/integrator/c008-r3/audit_indirect.py` in the main checkout, with
`DECOMP_ROOT`/`DECOMP_AUDIT_OUT`, `--matches`, `--scope`, and an explicitly
extended `--slot-map`. Whole-ledger/toolchain audits are held until the
predecessor owners release their serialized audit slot.

Stable-ledger G3 exit 0, canonical native/actual-signature probes,
indirect-slot consumer adjudication, final --show 0 MATCH sweep, complete
argcheck adjudication and all seven REVIEWING steps are **not yet exercised**.
The milestone is not ready for a full build/suite or independent checkpoint
review. Preserve the existing sources and continue their integration rather
than restarting completed author searches.
