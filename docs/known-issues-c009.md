# C009 recovered integration — known issues

## Status

**Preserved integration, not review-ready or accepted.** This branch has not reached the
requested final-match/interface/argcheck/pre-review closure. Do not prepare/finalize it or
interpret the manifest counts as accepted progress. The original base is `11e1992`;
`docs/GATES.md` remains v1.16. Main owns the subsequent serialized build/suite and acceptance.

`config/candidates/c009.json` currently lists 678 units / 678 functions, representing
55,272 target bytes from recovered author records. These are candidate scope numbers,
not a claim that the edited integration currently matches:

| Frozen profile | Units/functions | Target bytes |
| --- | ---: | ---: |
| game | 616 | 49,588 |
| middleware | 37 | 4,000 |
| sdk | 25 | 1,684 |

Previous collect inputs, successful destination receipts, dirty sources and author results
were recovered rather than restarted. L042's recovered `func_001e3af4` is included. Both
w17h SDK alternate MATCH rows (`00190688`, `001dbaac`) are included. v3/literal-record
results are not silently admitted into this default-model checkpoint.

## Blocking evidence

* Last whole-tree `interfaces --allow-unchecked-cpp` run exited **1** on the
  accepted + original C008 + C009 dry ledger (2,206 units): 1,705 checked,
  75 disagreements, 218 inline-declaration policy findings, 501 unchecked.
  This report precedes the final PipelineStorage marker cutover. Of its unchecked
  rows, 498 are C++, two are ARMCC varargs parser failures (`00190d0c`, `00190d68`),
  and one was the since-repaired `001272bc` marker. No new exit-zero report exists.
* The shared-header C++ include probe exited **1**. After repairing the malformed
  archive inline-member scope, it still reports incompatible canonical declarations:
  display-group objects versus NameTable pointer arrays, font vtable typing,
  layout-list/allocator contracts, ResourceEvent versus display-object destruction,
  resource-stream result typing, and scene/global dispatch ownership. C++ waiver
  does not establish these contracts.
* Final all-candidate matching, complete caller-contract argcheck adjudication and
  the complete seven-step pre-review have **not** been executed after the cutovers.
  No full build, suite, prepare or finalize was run by this worker.
* Sixteen additional unique new-scope status-MATCH addresses remain uncollected
  beyond the three approved contract deferrals below; their aliases/canonical
  cutovers are not closed. In particular, `002102b4` and `001eade0` have plausible
  existing-canonical-name cutovers that have not been implemented/matched. These
  are unfinished scope, not newly approved exclusions.

Private exact evidence and source bindings are recorded in the worktree's
`scratch/integrator/c009/HANDOFF.json`, `UNRESOLVED-SCOPE.json`,
`interfaces-canonical-r2.json` and `cpp-includes-r{1,2}.json`.
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

## Pending C008-r2 reconciliation

The current dry ledger still uses original C008, not the pending reviewed revision.
FixC008R2 owns the coherent `001462d4` InputSessionContext/full-word value correction
and same-class InputSessionContext/HandleManager layout sweeps, including the old
selected fields that actually belong to mutex depth. Local predecessor/access
refinements are staged only in this C009 worktree; Main's frozen C008 sources and
review snapshots were not edited. C009 must reconcile onto the completed C008-r2
source/header hashes rather than duplicate or supersede that revision accidentally.

The integration currently revises 37 predecessor source units in addition to the new
manifest. Their complete list and source/header hashes are private in HANDOFF.json.
Every changed predecessor function still needs final-byte and review closure.
