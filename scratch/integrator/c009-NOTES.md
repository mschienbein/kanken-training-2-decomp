# C009 recovered integration handoff

## Bound state

Worktree: `/home/michael/code/kanken-training-2-decomp/scratch/worktrees/c009`.
Branch `c009-integrate`, original base `11e1992`. This is preserved incomplete work,
not a ready checkpoint. No reset, provider switch, replacement author search, full
build, test suite, prepare or finalize was performed. Existing `inputs/`,
`initial-*.json`, `filter-skipped.json` and `filter.py` remain in `c009/`.

`c009/HANDOFF.json` binds 2,374 current source/header/configuration hashes and contains
all 37 revised predecessor units. It records 678 new units/functions and 55,272
recovered target bytes: game 616/49,588 B; middleware 37/4,000 B; sdk 25/1,684 B.
These are unaccepted scope totals, not an edited-tree final-match claim.
The dry ledger is accepted c007 + original c008 + c009 = 2,206 units. It is NOT yet
based on FixC008R2's pending revision. Do not repeat completed destination collection;
continue from the current canonical sources and the saved inputs/receipts.

## Continuation on adopted v9-r2

Merged current `main` (959311b) into `c009-integrate` without restarting the
preserved source integration. The catalogue recheck intersection is exactly
`00203810` and `00214580`; `match.py --show 0` reports `00214580` MATCH, while
`00203810` is blocked by existing incompatible SceneLifecycle declarations
(not a byte comparison failure). Exact diagnostics are retained in
`c009/catalogue-rechecks.json`; this row must be rerun after the header repair.
FixC008R2 has been asked for its successor commit and reusable linkage probe.

## Completed integration refinements (not full verification)

* Refined existing ServiceState, PendingState, signed ItemPageSet +0x10, SelectionState
  +0x4e/+0x50, GraphicsState, CommandSystem/queues/resources and DrawingSystem layouts.
* Canonicalized audio channels, voice allocations, sequence tracks versus unrelated
  WidgetAnimationTrack, service callbacks, GPU active texture/framebuffers, graphics
  resources and renderer shutdown; migrated the affected older source accesses.
* Refined NotificationFields history/course/channel views, timing flags, input/touch
  mode views, tagged buffers, AllocationRecordTable, resource/event pools, scheduler
  fields and mapped memory arenas using matching consumer/interface evidence.
* Moved shared C++ declarations from candidate prefixes into game headers; canonicalized
  native heap/controller/resource/font/archive/playback types. These moves exposed
  unresolved cross-header contracts; they have NOT passed complete C++ compatibility.
* Local ServiceOutput/InputSessionContext callback/workspace refinements distinguish
  the output/backend objects; HandleManager mutex ownership is staged consistently
  with the separately owned r2 correction. Raw logical names remain [INFERENCE].
* Kept original LayoutDescriptor flat caller after the approved record-contract defer.
  ResourceDescriptor's native constructor and ResourceObject's self-definition aliases
  have focused MATCH evidence; they are not excuses to alias unrelated caller classes.
* Last edit resolved the remaining PipelineStorage INTEGRATOR marker naturally:
  current word +0x808 and C flexible snapshot tail +0x810, without inferring an array
  bound from old padding. `001272bc` destination MATCH game 28/28, exit 0, receipt
  `c009/pipeline-final/func_001272bc.result.json`.

Two insertion-placement defects in shared headers were repaired: the extra/missing
ServiceState/AppletStartupState delimiter and the misplaced SoundArchiveAddon inline
member delimiter. Earlier edit gap-placement anomalies were reported through tool QA;
config/names.json was rebuilt as structured JSON from the original symbol dictionary
plus admitted proposals, not by salvaging malformed nested entries.

## Observed exits and exact private evidence

* Retained initial collect receipts are in `c009/initial-*.json`; do not redo that work.
* `c009/interfaces-integration.json`: initial whole-tree failure retained.
* First canonical interface run: exit 1, pycparser AssertionError caused by the now
  repaired ServiceState delimiter (no usable complete report).
* `c009/interfaces-canonical-r2.json`: exit 1, 2,206 units, 1,705 checked,
  75 disagreements, 218 inline-declaration policy rows, 501 unchecked. This predates
  the PipelineStorage marker repair. 498 unchecked are C++; the two remaining C
  parser failures are `00190d0c`/`00190d68` ARMCC varargs. The obsolete PipelineStorage
  marker accounts for the third unchecked C row in this saved report.
* `c009/cpp-includes-r1.json`: exit 1, malformed archive member scope plus contracts.
* `c009/cpp-includes-r2.json`: exit 1 after repairing that scope, 30 reported errors
  (compiler reporting cap; not a claim that only 30 errors exist). Current concrete
  contracts include W17DisplayGroup versus NameTable pointer arrays; FontVTable data;
  layout animation list and allocator types; ResourceEvent destruction versus
  DisplayObject/ScreenAsset; resource-stream Result; SceneObject/GlobalDispatchObject.
* `c009/contract-audit-r2.json`: old ushort forwarding diagnostic mismatched;
  full-word `001821f4` caller and revised `001462d4` callee MATCH, exits 0.
* `c009/native-cutover-r2.json`: focused native/contract results including
  ResourceObject ctor MATCH, HandleManager depth getter MATCH (game), mutex release
  MATCH (sdk), and failed record/flat caller alternatives.
* `c009/native-cutover-r3.json`: ResourceDescriptor native ctor MATCH, restored
  original `0020e460` MATCH, unsuccessful flat-versus-record ctor alternative.
* `c009/native-pod.json` and `native-pod-r2.json`: unsuccessful natural record caller
  forms retained. No private mixed record or ABI wrapper was adopted to force a match.
* `c009/pipeline-final/`: final changed pipeline unit MATCH, exit 0.

The 75/218 counts are the last observed whole-tree report, not a new audit of the
last pipeline edit. No final all-unit sweep or complete argcheck adjudication exists.
No full-build/suite success is claimed. Current source hashes are frozen only as
handoff evidence, not acceptance artifacts.

## Approved contract-resolution rows: exact paths

### func_00164b14

Original source: `scratch/authors/w18i/src/func_00164b14.cpp` (main repo).
Frozen MATCH: `scratch/authors/w18i/match/func_00164b14/final/func_00164b14.result.json`.
Integration variant: worktree `scratch/integrator/c009/deferred-contract/func_00164b14.cpp`.
Evidence: `c009/native-cutover-r2.json`, `native-cutover-r3.json`,
`native-pod.json`, `native-pod-r2.json`. The by-value native record matches SDK 32 B;
canonical flat ctor/caller migration does not. Original accepted caller `0020e460`
retained. Main explicitly approved `contract-resolution`; no duplicate class or ABI shim.

### func_001ba338

Original source: `scratch/authors/L034/src/func_001ba338.cpp`.
Frozen exact default MATCH: `scratch/authors/L034/match/func_001ba338/result/func_001ba338.result.json`;
source hash in receipt `fa4e6dd0c07111686db1ac1f1d650c969e48ff330a8e1832175c7b8e011f7c15`.
Integration variant: worktree `scratch/integrator/c009/deferred-contract/func_001ba338.cpp`.
Same AnimationOwner/W6ListController relation as the prior three c008 owner deferrals.
Main explicitly approved `contract-resolution`. Do not await another L034 match run.
Its other `001b8908` selected-model/v3 result stays deferred.

### func_001bda14

Actual original source: `scratch/authors/w19a/src/func_001bda14.cpp`.
Frozen MATCH: `scratch/authors/w19a/match/func_001bda14/final/func_001bda14.result.json`;
source hash `362157e85237adece16149db1b884da3f09927e76833a58d5287e869ea055ec3`.
Integration variant: worktree `scratch/integrator/c009/deferred-contract/func_001bda14.cpp`.
The Main approval called the row w19g, but the filesystem and immutable receipt
establish w19a provenance. Same +0x4c AnimationOwner/W6ListController relation and
accepted `0016c55c` consumer; weakened CallbackTarget pointer not credited.
Main explicitly approved `contract-resolution`, not a common owner cutover.

## Prior C008 rows remain deferred

Main's `scratch/integrator/c008-NOTES.md` and the prior deferred-matches evidence were
not changed. Preserve all six: `001b7110`, `001b6df4`, `001a2fc4` owner identity;
`0015e60c` float/unsigned output; `001f59b8` FloatPropertyDescriptor caller;
`001c5a08` full-word forwarding. None has been proven resolved here.

## Unclosed collection and predecessor revision boundary

`c009/UNRESOLVED-SCOPE.json` preserves every still-uncollected status-MATCH row,
including duplicate author deliveries and the two existing C008 deferred functions.
There are sixteen unique new-scope addresses beyond the three approved deferrals.
They are unfinished collection/canonicalization, NOT approved scope reductions.
In particular L046/w18b `002102b4` can plausibly use canonical ResourceDescriptor
constructors and the existing allocation tag; `001eade0` can plausibly reuse the
canonical allocation tag and MotionPair self-definition. Neither migration is yet
implemented or matched. Remaining rows need provenance rather than unproven pure
callee/destructor method aliases; function-local static/runtime rows need the same
scrutiny. Existing filter-skipped rows that are accepted/C008/noncreditable remain excluded.

FixC008R2 owns c008-r2: `001462d4` InputSessionContext* + int value, InputSessionContext
mutex +0x34/old selected +0x3c as depth; HandleManager pending0/queued4/completed8,
identifier+0x0c, active+0x0e, mutex+0x10, channels+0x18dc, old selected+0x18 as depth;
associated predecessor `0013712c`, `0012f1b4`, c008 `00154224`/`00154f78` and same-class
layout sweeps. C009 must reconcile with that committed/reviewed revision before its
final dry ledger/source binding. Current local predecessor changes are not an r2 merge.

## Seven-step pre-review state

1. Exactness: focused successes above; all-candidate + revised-unit final sweep absent.
2. Source policy: canonicalization performed, complete signoff absent.
3. Plausibility: unresolved contracts explicitly retained as blockers.
4. Interfaces: mechanical exit 1; C++ manual compatibility not closed.
5. Credit scope: 678 manifest functions, uncollected scope recorded, no accepted increment.
6. Linker rewrites: default model only; R1/inlined callee inference needs final receipt review.
7. Caller contracts: complete argcheck run/adjudication absent; especially retain VFP/core
   inputs and return-only/empty/callback contracts. No waiver from matching bodies.

This state is NOT ready for serialized dry build/suite. Main should resume the existing
integration and close these evidence gaps before exercising its full acceptance gates.
