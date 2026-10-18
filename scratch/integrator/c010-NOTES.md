# C010 integration — unaccepted milestone

This is an integrator working record, not acceptance or an independent review.
No full build, full suite, prepare, finalize, history edit or push has run here.
All counts below are candidate scope/evidence, never accepted credit.

## Current milestone 4 — remaining collection and audio contracts, not review-ready

Merged finalized main (c008-r3 seq8) and c009 through
`90cc196cb7107f5c79424d721ec4730b74323495`, then main
`85fd18dc7173e0542d8c33db157d03b7910c18c5`. The latter includes the model
checkpoint proposal and GATES v1.19 tooling, not c010 adoption. **C010 still
selects v2/none.** All merges completed without source/header conflict resolution.
The higher-priority heavy slot remains with FinishC009b; no c010 broad audit,
full build, suite or prepare has run.

The source proposal is now **538 units / 553 claimed identities**:
**548 new functions / 56,380 B**, plus **five moved accepted identities**.
The dry parent is 2,205 units / 2,279 functions after c009's additional reader
deferral; applying all five moves gives **2,738 units / 2,827 functions**.
There are **11 directly revised accepted source units** in the final scope.
**253 candidate units remain marked**; moving declarations into headers is not
itself typed-contract or interface signoff.

### Every remaining collection row measured or explicitly deferred

The bounded `collect-r2` processed all **181 source units / 183 functions**
previously uncollected, under the verified v2/none defaults. Collector exit 0
included **109 units / 110 functions collected** and **72 units / 73 functions
refused**. A subsequent explicit-profile, v2/none, `--show 0` sweep of all
109 collected units produced **109 exits 0 / all claimed functions MATCH**.
Original source/receipt and canonical-variant bindings are retained in
`COLLECTION-CLOSURE-r2.json`; no completed author search was restarted.

The reviewed **0019ecf8 SVC caller is now collected and MATCH**. Its redundant
00115b0c declaration was removed in favor of applet.h. The reviewed svc.h ABI
qualifications remain unchanged: pinned ARMCC marshalling, initialized aligned
low carrier/signed high split, unrecovered historical ownership, and an explicit
QUALIFIED review row. The seven reviewed mismatching callers remain uncredited.

All 72 collector refusals received complete individual diagnostics from exact
immutable integration snapshots: **72 exits 2**, not false MATCH or score results.
Thirty require unadmitted constructor/destructor/vtable/runtime identities;
42 expose concrete canonical declaration/receiver/field conflicts. The first-line
collector diagnostics were not treated as sufficient diagnoses. Function-level
`contract-deferrals-r2.json` records all 73 rows with exact reasons, full output
and receipt paths, original source hashes and re-entry requirements. These are
current contract/identity deferrals, not claims that the preserved author
sources never MATCHed or can never be reconciled.

The scope is now **649 functions**: 548 collected, 95 contract-deferred,
five c009-owned and one steering-deferred. The 95 include c009's newly withdrawn
0013156c reader identity. The additional near-miss 0010c138 witness is an exact
132-byte game/v2/none MATCH, but its author explicitly classifies redundant `!!`
normalization as steering; it is recorded, **not adopted**. It has no published
RESULT at the announced sweep roots, so the exact supplied source/verification/
classification paths bind the late publication. `scope-dispositions-r3.json`,
`SCOPE-SUPPLEMENT-r4.json` and `POLICY-DEFERRALS-r1.json` preserve this boundary.
The separate 46 later-model witnesses remain outside c010's selected model.

### Complete emitted ownership of the new collection

The new objects exposed three extra defining identities:

- 00151b04 now lives only with new 00151a74 in `collect-r2/aux_commands.cpp`;
  its superseded new singleton is deleted, not double-counted.
- Accepted 001dc574 and 001dc90c now live with new 001dc4a4 in the complete
  three-function completion TU. Both v1.16 moves are recorded and old accepted
  singleton definitions are deleted, with preserved source/profile/hash evidence.
- The two complete owner units rematch **2/2 exits 0, all five identities MATCH**.
  `owner-cutover/collected-owner-moves-r2.json` records these moves and retired
  sources. Historical receipts predating complete ownership remain preserved.

### Task cancellation is an address-token ID protocol

Original 001d32e4 compares its unchanged r1 word with the task's word at +0x14;
it does not dereference r1. The pinned API distinguishes `CancelTask(Task*)`
from `CancelTaskById(u32)`. The two accepted 24-byte wrappers forward their
entry word unchanged. Shared 001d32e0/001d32e4/001d6b24/001d6678 declarations
now consistently take **unsigned IDs**, not fake RegistryEntry pointers.

001d3230 uses its real +0x2314 member address as a 32-bit task identity, with
the explicit pinned-ARM pointer-to-ID conversion. Opaque member storage makes
no invented Task/RegistryEntry class claim. 001d7e6c's loaded identity is an
unsigned word, and its group pointer now has the actual AudioVoiceGroup owner.
Both control prefixes are shared; the obsolete local views/markers are removed.
**All five task-family units MATCH**, including all three revised accepted
wrappers. Actual initial C++ flexible-tail compile failure and the incorrect
byte-versus-bool lifecycle mismatch remain retained. Existing C-only-tail
convention and the original/native bool lifecycle/accessors resolve them.
`TASK-CANCELLATION-ADJUDICATION-r1.json` records the facts and qualifications;
this is not a completed all-producer/indirect task audit.

### Main-requested accepted audio corrections

`AUDIO-CONTRACT-ADJUDICATION-r1.json` binds these corrections alongside the
earlier 0016a87c priority fix. Both model rows **remain deferred in cmodels**;
neither is re-adopted merely because these prerequisites are corrected.

- **001d3c78** now takes full `int` mode, stores its byte separately and tests
  the unchanged full word. Original facts independently prove the byte store
  and full-word zero comparison. Restoring natural original initialization
  chronology gives **176-byte MATCH**; both earlier ordering mismatches remain.
- **001d9198** now returns the actual canonical AudioVoice and takes
  `(int channels, int priority, typed callback, void *context)`. Its original
  r2/r3 reach 001d96e8, which stores them at +0x1a0/+0x1a4. Original 0018290c
  invokes that stored callback on the same receiver, and 001d5bf4 dereferences
  the context at +0x9c. The factory, accepted callback and new recycler use the
  shared AudioVoice (including its real +0x1b0 link): **3/3 units MATCH**.
  No global SoundTrack rename, common complete-class fiction or callback cast
  is introduced. The lower AudioVoiceGroup callback remains distinct. Pinned
  Channel's generic u32 user token does not override the actual pointer-context
  evidence, and no native mangled identity is inferred from that SDK spelling.
- **001719f4 / 00171a34** use the actual partial ItemPageSet/content relationship:
  kind/index/content are exposed in reserved bytes/word, content is the same
  W11SingleState receiver used by status/reset consumers, and category/enabled
  are full `int` words. The old ConfigState/ConfigTarget shadow views are removed.
  Signed variant/group storage is retained. Original 00171a34 forwards category
  unchanged to 00168670; original 001dc390 returns precisely 0/1. Retaining that
  predicate's unsigned-byte lifetime preserves the observed storage sequence:
  accepted **001719f4 MATCHes all 64 B**. Earlier mismatch/C89-refusal receipts
  remain visible; no narrowed category, volatile or register forcing was used.
- Two further initializer units, 001d38ac/001d9a0c, now reuse the spelling-
  equivalent shared SequenceSoundControl/WaveSoundControl records and three
  shared declarations: **2/2 units MATCH**, completing two more marked blocks.

These are per-touched-unit results, not final dependent-header closure. The
latest parent/model merge introduces additional canonical headers; all affected
accepted/new consumers, canonical signatures, G3, callback/indirect consumers,
argcheck and all seven pre-review steps still need stable-ledger evidence.
`FAILED-ATTEMPTS-r2.json` preserves actual failures and analysis limitations.

### Exact continuation and cross-checkpoint ownership

`HANDOFF.json`, `dry-matches-r4.json` and `match-scope-r4.json` supersede the
milestone-3 live state; its prior handoff remains preserved. No compiler jobs
remain running at this seal. The static inventory still reports substantial
different-definition/forward owner leads; no whole G3 success is inferred from it.
C011 additionally reserves 00155ab4 and the parent lookup/archive units for
forward moves only after c010 finalization. In particular, **0014fe14 now has a
c010 owner** at `collect-r2/cutoff.cpp`, profile middleware, and has MATCHed.
The previously reserved four linked/list/native-pair source identities remain
untouched. C011 must preserve the full-word archive helper contract and all
identity qualifications rather than resurrect its author's narrow formal.


## Historical milestone 3 — owner moves and priority-width correction, not review-ready

Milestone 2 is preserved below as a historical snapshot. The preserving c008-r3
follow-up `b5e8b8abffe6d2e2d3f259920aa9426bd9c1ca0b` is merged, retaining
c009's explicit context-bearing slot14 before/after comments. The latest merged
c009 tip is `b6bfcecb8181c79bf0559e6e7f185156e98b374d`; c009's readiness
checks are currently serialized ahead of c010. Inherited prepare artifacts are
parent provenance, not a c010 prepare or whole-ledger G3 pass.

The manifest now has **430 units / 441 claimed function identities**, comprising
**438 new functions / 48,724 new target B** and **three explicitly moved accepted
identities / 156 B**. Profiles are 311 game, 43 sdk and 76 middleware units.
The new dry ledger is **2,633 units / 2,718 functions**; it removes the three old
singleton owners before adding the complete destination unit. Four directly
revised accepted source units are in the final match scope, including the
priority setter below. **199 candidate units still contain INTEGRATOR markers.**
The 648-row scope, 183 uncollected rows, 21 explicit contract deferrals and
46 later-model witnesses remain as recorded in milestone 2; no old MATCH or
failed author result was overwritten.

### Emitted-owner blockers resolved with measured source

- `ut_LinkList.cpp` is the sole owner of 00185bb0, 0018d868, 00191154 and
  00191178 under middleware. The manifest contains all three required v1.16
  moves, and the three old accepted singleton C definitions are deleted.
  Their original source/profile/hash provenance is preserved under
  `c010/owner-cutover/retired/` and `linklist-moves-r1.json`. No accepted
  history or receipt was rewritten.
- The alternate archive-query unit is the sole owner of 001650f8 and its five
  query endpoints; the duplicate standalone source is deleted and preserved
  under owner-cutover. The helper now accepts a full `unsigned` word.
  Original abstract facts show three unchanged entry-word comparisons against
  stored halfwords; halfword storage did not justify a narrowed parameter.
  The canonical C archive exposes its variable tail directly; the private
  BinaryFileHeader cast is removed. The existing C++ two-reference partial
  prefix remains unchanged, not a claim of a universal archive size.
- Both units were rematched sequentially and nice'd after the preserving
  callback follow-up: **2/2 exits 0, 10/10 claimed functions MATCH** under
  explicit frozen profiles, v2/none and `--show 0`.
  `owner-post-followup-r1/results.json` binds commands, source hashes and
  receipts. Earlier first-pass owner receipts are retained separately.
- Emitted 00183458 remains classified runtime and **uncredited**.
  `EMITTED-OWNERSHIP-DISPOSITIONS-r2.json` closes the two known owner blockers,
  but does not claim a completed whole-object/whole-ledger ownership audit.

### Full priority formals, byte storage, and shared sound prefixes

Main's r41164 caller observation is preserved as ground truth. The new scoped
`owner-cutover/priority-facts-r1.json` corroborates that original 001d0614
supplies word loads to all three 0016a87c calls and the 001d2c84 call.
0016a87c stores r2 as a word at +0x50 and r1 as a byte at +0x9d; 001d2c84
also stores the priority byte. The pinned BasicSound header declares
`SetPriority(int,int)` and `SetPlayerPriority(int)` while storing `u8 m_Priority`.
Both C formals are therefore `int`; **the storage remains unsigned char**.
No failed uchar-caller measurement was rerun or relabelled as an attempt.

The canonical SoundInstance view now names the player pointer at +0x10 and
pool link at +0xf8, preserving its size and +0x108/+0x110 links. Its +0x24
notification slot receives the SoundInstance receiver; it is not void(void).
The common observed pool/control prefixes and declarations are shared in
sound_runtime.h, removing private redefinitions in 001d2c84, 001d3bcc and
001d9c14. 001d3230 now uses the actual named pool link instead of a union
overlay, but **retains its marker**: its task entry pointer still conflicts
with the accepted cancellation wrapper's scalar contract. No cast, shadow
shared declaration or fake callee masks that unresolved issue.

The source/API adjudication and before/after bindings are in
`owner-cutover/priority-contract-r1.json`. Descriptive C owner/control names
are INFERENCE, not recovered native aliases or complete original class sizes.
Native handles already forward int; channel 00155dec already takes/clamps int.
Unrelated scheduler ushort fields are not widened, and 001d2ce0 is an
attachment test, not a numeric priority getter. Body/result identities,
all indirect consumers and final canonical signatures still require audit.
**All five touched sound units now MATCH (5/5 exits 0, 364 target B)** in
c009's explicitly released five-command window, sequential and nice'd.
`priority-match-r1/results.json` binds the current source/profile receipts.
The accepted 0016a87c remains an exact 12-byte match after widening the formal.
The serial slot was returned to c009 immediately; no broader audit was run.

### Delivered standalone header-gap prerequisites

Cherry-picked HeaderGaps `995aa2b3a2f77e8fcd336b5b9a678a8fe2b5c194` as
`c741732`: nn/math.h, nn/snd.h, GLES2/gl2.h and new gl2dmpconstants.h,
plus its THIRD_PARTY/facts/license provenance. PivotDirection/PIVOT_NONE and
the DMP token remain explicitly inferred minimal subsets. The 160-sample
frame length has public libctru provenance; its SDK spelling remains inferred.
No alias, implementation or shim is supplied for the three unresolved smoke
symbols. The companion worktree measured 62/62 affected units MATCH
(92 credited / 93 emitted functions), ten successful smoke compiles followed
by one all-MATCH, six mismatches and three link refusals. Its accepted/c008-r2
interfaces passed; its two c009 reports were byte-identical with the inherited
Heap ODR finding. These are companion-context results, **not fresh c010 gates**.
Exact handoff and receipts remain in `scratch/worktrees/header-gaps/scratch/header-gaps/`.

### Exact unready handoff

`HANDOFF.json` and `dry-matches-r3.json` supersede the milestone-2 live state;
the old handoff is preserved separately. `match-scope-r3.json` has all 430
candidate units and four directly revised accepted units. Final whole-scope matching,
remaining collection including reviewed SVC caller 0019ecf8, 199 marked-unit
contracts, canonical/native signatures, current dry-ledger G3, complete
indirect/callback sweep, argcheck and all seven pre-review steps remain open.
No full build/suite/prepare/finalize/history/push was run here. This is a
committed, exact **unready milestone**, not a checkpoint review request.

C011 has reserved the four unaccepted c010 source identities 001d0348,
001d2234, 001f478c and 001c7760 for cross-checkpoint shared-header/caller
migrations. C010 will not rewrite those sources concurrently. Historical owner
qualifications remain; the native texture pair call requires both same-address
self-definition and actual native return-ABI proof. Runtime 00183458 stays
uncredited. Service/framebuffer/183208 owner forks remain unresolved, not
authorization to replace accepted contracts.


## Historical milestone 2 — canonical source cutovers, not review-ready

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
