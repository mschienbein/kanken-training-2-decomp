# C010 integration — unaccepted milestone

This is an integrator working record, not acceptance or an independent review.
No full build, full suite, prepare, finalize, history edit or push has run here.
All counts below are candidate scope/evidence, never accepted credit.

## Current milestone 2 — canonical source cutovers, not review-ready

Merged c009 `1d10b1d6ab522bf1ea4abbb2d14cb527303ea21b`, including c008-r3
`642d725079b6a2154bc6f06fa554dcbeb063bcea`, and then c009's newer handoff
`1f66a8a76f7a16ba730946e9403ee2d7cbaf3491`. The inherited c009 dry base is
2,206 units / 2,280 functions; its final matching/G3/probes/argcheck remain
unrun. C009 retains the actual context-bearing slot14 before/after callbacks.
FixC008R3 now owns the higher-priority preserving follow-up for service slots
0/1/2/14 and the notification-list prefix. That follow-up is not merged here.
No whole-ledger G3 success or checkpoint readiness can be inherited from r3.

The current source proposal is **431 units / 438 functions / 48,724 target B**
(311 game / 43 sdk / 77 middleware units), plus three revised accepted units:
`src/sdk2/func_00180768.c` (sdk), `src/wave4/func_00185cc0.c` (game), and
`src/wave6/func_001467e4.c` (game). **No final matching pass of these edits has
run.** The prior collection results below remain historical destination results.

### Scope is preserved through the latest publication seal

`SCOPE-SUPPLEMENT-r2.json` and `SCOPE-SUPPLEMENT-r3.json` extend the initial
272-manifest snapshot to **374 enumerated result manifests**. They add
**19 unique hash-bound default-v2 matches**, not silently replacing historical
attempts. Together with the separately reviewed SVC caller the scope is now
**648 unique functions**:

| Current disposition | Functions |
| --- | ---: |
| Collected proposal, still awaiting final canonical/MATCH checks | 438 |
| Still-uncollected rows, including new publications and the SVC caller | 183 |
| Explicit predecessor/c010 contract-resolution deferrals in this scope | 21 |
| Added to c009, excluded from c010 credit | 6 |

The later-model list remains 46 nondefault witnesses / 21 functions; the
supplementary exact-receipt scan found no additional nondefault MATCH.
C009's additional `0015e5d4`, `00190d0c` and `00190d68` deferrals are also
guarded against recollection, without reclassifying their valid source as non-C.
`scope-dispositions-r2.json` and HANDOFF bind the current row-by-row boundary.

### Actual source and header integration performed

- Removed **319 source-local function prototypes across 106 units** with
  structural AST edits and imported their existing canonical headers.
- Removed **65 source-local global declarations across 59 units**, so real
  owner, pointer-depth, const and vtable contracts are no longer hidden by
  private redeclarations.
- Consolidated **32 token-identical record definitions** across 21 units,
  and **six additional POD records** across four units differing only in
  standard integer/elaborated C++ type spellings. No alias, owner rename,
  member-name change, width narrowing or callback cast was used.
- Added actual defining headers for four C++ forward-owner dependencies;
  removed only seven now-empty extern-C blocks and their completed move markers.
- Retained native preprocessing context: one existing NW_RELEASE definition
  and four existing preinclude.h imports remain ahead of newly imported headers.
- The free-list allocator now has one shared `FreeListPool` state using the
  existing `FreeLink` node. ObjectPool contains that real subobject, its accepted
  initializer uses `.head`, and both allocation wrappers use the actual owner
  instead of a pointer-to-pointer prototype or duplicate BlockPool/SoundPoolLink
  layouts. SoundPool remains an explicitly partial +8 pool-state view.
- RegionHeap exposes the actual count-and-sentinel child list and state-chain
  record. The allocator-mode operation has a void result and the return-only
  hook retains its caller-evidenced owner input; erased identity remains INFERENCE.
- New `game/runtime_type_initializers.h` owns 20 native declarations from
  14 initializer units, retaining canonical RuntimeTypeInfo/VEC3 types.
  One already-collected unit is migrated; 13 exact original-source variants
  await destination collection. Their original receipt/source and variant hashes
  are separately bound in `SOURCE-OVERRIDES-r1.json`. No author file was edited.

Canonical imports are not ABI signoff. The static inventory retained 79
different-definition leads before the newest parent merge; six spelling-only
cases were subsequently discharged. Other owner/layout leads still require
native probes and consumer adjudication. **202 candidate units still contain
INTEGRATOR markers**; they are not represented as ready or silently stripped.

### Names and concrete deferrals

`names-admitted-r1.json` binds **127 non-address-name proposals** added to the
checkpoint overlay, all actual hash-bound same-address emitted self-definitions
(70 compiler-generated static-initializer names). Existing entries are unchanged.
The earlier 717-name analyzer output is a raw metadata inventory that includes
reserved automatic address names; it is not 717 adopted aliases. No undefined
destructor/runtime alias, erased-call guess, __aeabi_atexit or __dso_handle binding
was admitted. Native spelling/class/role and canonical typed contracts remain
qualified and require review before acceptance.

Two new concrete contract deferrals remove **188 B** from the proposal, not
from accepted credit. Original sources/receipts and canonical variants are kept:

- `001db404` (48 B): its private ResourceFont/ResourceFontVTable contradicts
  canonical FontVTable data_00224d30 and the accepted native
  ResFontBase::RemoveResourceBuffer endpoint. No shadow extern-C method or
  guessed destructor alias is used. The accepted 001db130 wrapper is unchanged.
- `00146758` (140 B): its C RegionHeap call to 0018216c cannot be represented
  as a second declaration of the credited native HeapBase::FillFreeMemory.
  The void result/full mode word are corrected, but a real native/C owner
  cutover is still needed. No fake LockHeap/UnlockHeap aliases are invented.

### Newly identified emitted-owner blockers — not yet changed

`EMITTED-OWNERSHIP-LEADS-r1.json` records every extra catalogue definition in
the bound author receipts. Five leads occur in three collected units:

- `ut_LinkList.cpp` also emits accepted 0018d868, 00191154 and 00191178,
  still owned by their old C units. GATES v1.16 requires explicit moves,
  complete four-function destination ownership/profile, and deletion of the
  obsolete old definitions; none of those moves/deletions has been applied.
- The alternate block-query TU also emits 001650f8, duplicating the standalone
  new unit. It narrows the tag to unsigned short while the standalone definition
  retains the full unsigned input. A correct consolidation must preserve the
  full word, remove the duplicate owner and reconcile the real variable-length
  ArchiveFile block tail instead of keeping its shadow BinaryFileHeader cast.
  The proposed archive/header consolidation has **not** been applied.
- `001c7760` also emits 00183458, a classified **runtime** identity. It remains
  uncredited and must not be added to C progress.

These are concrete pre-review/G4 blockers, not reasons to ignore extra emitted
functions or quietly reduce scope. Complete source/evidence is preserved.
The user requested wrap-up before the newly coordinated owner fixes began.

### Exact check boundary and continuation

Main approved a single bounded, nice'd, sequential remaining collection plus
per-unit canonical/MATCH pass, with a clean pause for c008 prepare. Before it
started, FixC008R3 reclaimed the next audit slot for its preserving follow-up.
**No c010 compile/collection/probe pass is currently running.** The driver is
`collect_remaining.py --run collect-r2`; it excludes all predecessor/c010
deferrals, consumes the publication supplements and verified canonical-input
overrides, and preserves new refusals without automatically editing the manifest.
After that collection, canonicalize its SVC source's now-shared 00115b0c
declaration and merge the resulting units into c010's manifest deliberately.

`probe_scope.py` reuses the existing c008-r2 compile helper for native and actual
C-signature checks. `match_scope.py` uses explicit v2/none, --show 0, exact
unit profiles, captured exits and a PAUSE-file boundary between units.
`match-scope-r2.json` includes every current new unit and the three accepted
source revisions. No full build/suite/prepare/finalize/history/push is authorized.

Stable-ledger G3 exit 0, post-cutover native/signature probes, final matching,
complete indirect/callback adjudication, argcheck and the seven-step pre-review
are all **unexercised** on this merged source state. Preserve this milestone;
do not substitute its original author MATCHes for any of those gates.

## Historical milestone 1 record

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
