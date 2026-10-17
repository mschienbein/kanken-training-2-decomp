# C009 recovered integration — known issues

## Status

**Preserved continuation, not review-ready or accepted.** The user requested wrap-up
before final-match/interface/argcheck/pre-review closure. Do not prepare/finalize
or interpret these scope counts as accepted progress. Continued from `1cc9f60`,
merged adopted main `959311b` and c008-r2 `38b4c01`; `docs/GATES.md` stays v1.16.
Main owns serialized full builds/suite and acceptance.

`config/candidates/c009.json` now lists 676 units / 676 functions / 54,648 target
bytes. Two additional concrete contract blockers were withdrawn below; these
numbers are scope, not an edited-tree final-MATCH claim:

| Frozen profile | Units/functions | Target bytes |
| --- | ---: | ---: |
| game | 614 | 48,964 |
| middleware | 37 | 4,000 |
| sdk | 25 | 1,684 |

Previous collect inputs, successful destination receipts, dirty sources and author results
were recovered rather than restarted. L042's recovered `func_001e3af4` is included. Both
w17h SDK alternate MATCH rows (`00190688`, `001dbaac`) are included. v3/literal-record
results are not silently admitted into this default-model checkpoint.

## Blocking evidence

* The intermediate whole-ledger `interfaces --allow-unchecked-cpp` remediation run
  exited **1**: 2,205 units, 1,705 checked, 48 disagreements, 28 inline-declaration
  findings, 500 unchecked (498 C++ and the two ARMCC varargs parser failures).
  Remediation continued while that audit ran. It is not a current final count,
  and no exit-zero G3 report exists.
* The final **166-game-header C++ co-inclusion probe exited 0** after canonical
  pointer-array, allocator, audio-manager, resource/scene-prefix, stream and raw
  IPC/result repairs. This is compile-only shared-header compatibility, not the
  native-unit, actual-signature or indirect-consumer same-class sweep. Those
  c009-wide probes using c008-r2's reusable helpers remain unexercised.
* The catalogue recheck intersection was `00214580` (MATCH, exit 0) and
  `00203810` (compile exit 2, then deferred on concrete owner-contract evidence).
* Final all-new/revised matching, complete argcheck adjudication and the seven-step
  pre-review have **not** run after these changes. No full build, suite, prepare,
  finalize, history edit or push was performed.
* Sixteen unique additional new-scope status-MATCH addresses are still uncollected.
  Their alias/canonical cutovers in `UNRESOLVED-SCOPE.json` are unfinished scope,
  not newly approved exclusions; no collection claim is made.

Private exact evidence and source bindings are recorded in the worktree's
`scratch/integrator/c009/HANDOFF.json`, `UNRESOLVED-SCOPE.json`,
`interfaces-remediation-r1.json`, `cpp-compat-r5.json` and `contract-deferrals.json`.
The remaining integration state is described in `scratch/integrator/c009-NOTES.md`.

## Approved contract-resolution deferrals

| Function | Reason |
| --- | --- |
| `00164b14` | Native SDK WidgetPlacement by-value record matches, but canonical LayoutDescriptor's flat accepted `0020e460` caller cannot yet migrate byte-identically. Original caller retained; no private duplicate or ABI shim admitted. |
| `001ba338` | L034 exact default MATCH retained privately; AnimationOwner/W6ListController ownership at the shared slot remains unproven. |
| `001bda14` | Same AnimationOwner versus W6ListController relation at +0x4c as accepted `0016c55c`; weakened CallbackTarget redeclaration is not credited. Original source/receipt are under w19a (the approval message called this w19g). |

Main explicitly approved these as `contract-resolution`. Resolve the owner group
in a dedicated reviewed pass; do not invent a common owner merely to combine the rows.
The source/receipt paths and failed natural cutover evidence are in private NOTES/HANDOFF.
L034 `001b8908` remains selected-model/v3 scope, not a default C009 addition.

## Preserved C008 deferrals

None of the prior six has been proven resolved: `001b7110`, `001b6df4`, `001a2fc4`
(AnimationOwner/list owner relation), `0015e60c` (float-output versus unsigned-output
contract), `001f59b8` (canonical FloatPropertyDescriptor caller), and `001c5a08`
(full-word forwarding contract). The previous C008 NOTES/deferred evidence is unchanged.

## Additional evidenced contract deferrals

| Function | Concrete blocker |
| --- | --- |
| `0010cc94` | The proposed extern-C EntryObject constructor contradicts the accepted native `nw::font::ResFontBase` constructor at `00113e78`, and its unsigned-array vtable declaration contradicts canonical FontVTable. The actual derived identity is unresolved; canonical ResFont's constructor is bound elsewhere (`001db3ec`). The shadow source/header were withdrawn, not aliased. |
| `00203810` | Its L045SceneN constructor/destructor declarations disagree with concrete lifecycle definitions, including `001e7470`, `001ec14c` and `001e7fdc`. A shared virtual-dispatch prefix does not prove the full derived owners. The initializer and its 42 shadow prototypes were withdrawn instead of retaining a wrong common-owner erasure. The changed v9-r2 extent remains unclosed. |

Both are recorded under step 3's defer-with-reason option, with private sources
and exact evidence retained in `contract-deferrals.json`. They are not MATCH
or acceptance claims.

## Predecessor boundary and newer pending remediation

c008-r2 `38b4c01` was merged at `9218542`; the dry ledger no longer uses failed
original c008. It has 2,203 units / 2,277 functions: 1,601 predecessor functions
plus 676 c009 functions. Its additional `0016b6a4` deferral remains excluded.

The current source delta against c008-r2 revises **102 predecessor units /
106 functions**, including shared-header inclusion migrations. Every revised
unit is listed in HANDOFF and still needs fresh byte/review closure. Header-only
dependents also require the complete canonical contract sweep.

At wrap-up, FixC008R3 reported new `c008-r3` remediation from current main
`f3ae330`: slot 11 context, slot 15 context/saved arguments/result, and an
indirect-consumer same-class audit. c008-r2 remains unfinalized. Those fixes are
owned by FixC008R3 and were not duplicated or merged here. Reconcile that successor
before claiming a review-ready c009 base; historical c008 records remain untouched.
