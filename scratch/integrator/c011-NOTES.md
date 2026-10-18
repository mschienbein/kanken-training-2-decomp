# c011 — default-model author MATCH integration milestone

**State:** committed source milestone, not accepted progress and not review-ready.
Branch `c011-integrate`, worktree `scratch/worktrees/c011`; main was not edited.
Started from c010 `36924e7`, then merged `9fc2592` and the final handoff tip
`beec234b77c92634aaddea024805ecd83471e852`.

## Closed inventory

`c011/SCOPE.json` and its immutable `SCOPE-SUPPLEMENT-r2.json` bind the publication
snapshot: 906 author RESULT manifests plus the permuter summary snapshots. All
648 c010-owned identities are excluded, including its uncollected rows and
contract deferrals; c008-r3/c009 candidates, the accepted ledger and the exact
c009 handoff deferrals are excluded too. The exclusions contain 2,928 identities.
Later publications are not silently claimed by this milestone.

| Disposition | Units | Functions | Target bytes |
|---|---:|---:|---:|
| Collected, canonical source | 17 | 19 | 2,064 |
| Contract-resolution deferred | — | 26 | 5,716 |
| Total newly inventoried default-model scope | — | 45 | 7,780 |

Every scoped identity has a row in `c011/DISPOSITIONS-r1.json`; no inventoried row
is left without a disposition. Five classified-runtime claims and two trial
Thumb-profile claims are separately excluded. Only the frozen `game`, `sdk` and
`middleware` profiles are used. The candidate has 15 game units, one sdk unit and
one middleware unit. Every admitted receipt uses **v2 / none**; v3, L1, v4, v6 and
v7 evidence is not imported into this default-model checkpoint.

## Collection and canonical contracts

- All destination copies came through `tools/collect.py`, in three sequential,
  nice-10 invocations. The first recorded two prerequisite refusals: the canonical
  index constructor needed its C2 binding, and the sequence updater referenced a
  retired reserved-byte array. Both causes were corrected before collection-r2;
  the original collection record is retained unchanged.
- Reused the existing `PaneIndexList` declaration and its natural matched
  `002095c0` caller instead of introducing a second `ByteSelectionList` identity.
- Shared the independently matching pair/scalar singleton allocations and their
  cleanups through `game/selection_collections.h`. Their registered targets are
  now actual native destructor definitions, not extern-C aliases to shadow
  classes. The separate four-byte/two-cell and eight-byte collection handles use
  the real `ResourceEvent*` cleanup contract; canonical `WidgetSlot` and its
  accepted users are unchanged.
- Removed the two-cell source's uncredited `001acc30` body and c010-owned
  `001ad288` body. The former has its own explicit contract deferral; the latter
  remains solely owned by c010. No accepted function is moved by c011.
- Published the shader cleanup, action, linked-owner, counted-list, resource
  teardown and native pool interfaces in canonical game headers. The sequence
  updater reuses `AnimationTrack.suppress_stop` at +0x82 and the defining
  AudioVoice endpoint contracts.
- With c010's explicit reservation, migrated its **new** units `001d0348`,
  `001d2234`, `001f478c` and `001c7760`. The first three share the qualified C
  records/real head-pointer contract with their new wrappers. The texture caller
  now invokes the actual canonical native `GetReferenceTextureTarget` definition
  and its declared pair result; there is no pair cast or extern-C method shadow.
  These are four revised c010-new units, not four accepted-source revisions.
- Eight new headers and three declaration-only header extensions centralize the
  admitted interfaces. All integration-marked local blocks in the admitted new
  sources are resolved. `SOURCE-CUTOVERS-r1.json` records the changes and proofs.
- Added 13 proven name bindings. `NAMES-PROVENANCE-r1.json` binds emitted
  same-address definitions and retained references in natural whole-function
  MATCH callers; `NAMES-PROVENANCE-r2.json` binds the actual canonical C2/D2
  definitions. No unrelated overlay entries, erased-call guesses, private
  competing constructor names or deferred-row name proposals are adopted.
- Preserved upstream GPL notices and updated `THIRD_PARTY.md` for the three
  adapted native utility/graphics/layout sources. Emitted runtime `00183458`
  remains uncredited wherever native resource wrappers instantiate it.

## Explicit deferrals

The 26 MATCHes are preserved with exact source/receipt hashes, reasons and
accepted-contract evidence; they are not rejected matches. They require true
accepted owner/signature/layout migrations rather than casts or nominal aliases:

- Service session/active-state ownership and framebuffer indirection.
- CharWriter versus the accepted text-writer view; native pane/container,
  Picture/TextBox and font lifecycle identities.
- DisplayObject versus ResourceEvent creation/cleanup/label ownership, and
  singleton/controller allocation views that depend on those accepted contracts.
- Shared filesystem globals and native loader/archive receiver contracts.
- Wave-parameter records currently local to accepted units, native track/timing
  lifetimes, and the SceneController/SoundVoiceObject aggregate/lifecycle family.
- The draw-stream prefix, training-screen/effects/animation owners, and native
  effect-list ownership. Both verified permuter targets retain these genuine
  accepted-contract blockers; no preprocessed shared-header copies are adopted.

c010 explicitly confirmed that its unresolved service, framebuffer and
`00183208` forks are not settled accepted-header cutovers. Identical layout is
not treated as proof of native class identity. Historical class/member spelling
and partial owner extents remain **[INFERENCE]** where recorded.

## Exercised evidence and remaining gates

`POST-CUTOVER-r1.json`: 20 sequential nice-10 per-unit runs, all exit 0 and all
credited targets MATCH — 18 c011-new functions plus the four revised c010-new
functions. The later, unchanged canonical HeapBase unit was independently
re-derived at its destination by collection-r3, bringing the candidate to
19 functions. The constructor-name and native-lifetime prerequisite MATCH
records are also retained. No full build, test suite, whole-ledger audit or
checkpoint gate was run.

**Main-owned readiness remains open:** inherited c009/c010 integration, G3,
argcheck/adjudication, emitted-owner closure, pre-review, full builds/suite and
prepare. These wait for the serialized heavy slot after c010. The proposed
ledger must include c010's explicit moves and exactly one owner per identity.
Freeze v2/none explicitly before prepare; no review/finalize/push was performed.

`c011/HANDOFF.json` is the machine-readable handoff. This milestone completes
inventory, disposition, collection and canonicalization of its admitted scope;
it grants no accepted function or byte credit.
