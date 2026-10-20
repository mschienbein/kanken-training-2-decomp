# C009 recovered integration handoff

## Current resumed boundary: cmodels seq9 merged, explicit v6 + L1

Finalized main **261fdc9** is merged at **b8f15ed**, preserving the saved receiver
patch from **a3122a6** rather than reapplying older work. Current selection is
**armlink5-inline-branchnop-v6 + anonymous-string-records-v1**. All older v2/none
readiness instructions below are superseded, not a reinterpretation of their receipts.
Frozen GATES v1.19 and catalogue v9-r2 remain unchanged.

Canonical `MessageLayout` supplies the cast-free `00177b84`/`00170db4`/`00170dc0`
receiver; `001ad6d0/ec` use `MessageEntry` at `entries[2]`. `MessageEntry` is not
`DisplayHandle`, and the obsolete WidgetContainer/WidgetActivity shadows are removed.
`00177598` now updates layout x/y under the same receiver. The cmodels height getter
`0017a424` keeps its exact accepted const-layout API and source.

The incoming cmodels consumers exposed conflicting reset/number owner declarations.
Original-safe `00175d18` first-iteration facts identify the +0x7ec visual-node vector;
clock/number fields and render entry now use canonical `W11jVisualOwner`. Reset
callers use the existing coherent Screen family without SelectionList casts/unions.
Scoped **001b8908**, **001e0afc**, **001ed874** all MATCH with the supplied original
helper closure under v6+L1 (`widget-receiver-match-r3.json`). The earlier failed
merged-header witness is kept in `widget-receiver-match-r2.json`.

Current candidate scope **676 new functions / 57,052 B**; **142 revised accepted
units / 147 functions**, plus remaining header-reopened R1 functions, give **832
required units / 838 functions**. Dry ledger **2,238 units / 2,297 functions** retains
all **1,621 accepted identities**. `readiness-scope-r10.json` is current authority.
`00113c0c`/`0018ee88` stay evidenced new-function deferrals; accepted `001c4494`
Result/int-output and migrated `0019d5dc` remain intact. Plain original `001c20e8`
retains its `[INFERENCE]` note and mandatory **QUALIFIED** pre-review/review row.

**Ready source boundary 1851bf3.** Final matching **832/832 units MATCH**;
G3 **zero errors/policy items/unchecked C**, 510 explicitly waived C++ units;
1,500 actual C signatures and 174-header co-inclusion both clean. Native 1,940
units have zero compiler failures and ten nonfatal diagnostic units, all qualified.
Final argcheck exit2 is retained: 78 issues / 412 notes all individually adjudicated,
389 mechanical ABI gaps explicitly supplemented by native/source/indirect evidence.
All seven integrator pre-review steps have 838 exact destination-bound rows;
`PRE-REVIEW-resume-r1.json` binds manifests, source/header hashes and exercised exits.
No full build, suite, prepare, finalize, accepted-record write or push was run here.
Main alone owns full gates and independent reviews. Heavy slot is released on handoff.

## Bounded mismatch closure after 66ae993

Main authorized a bounded pass over the three remaining mismatches, then withdrawal
of unmatched **new** definitions rather than delaying the other 676 functions.
The accepted reader **`001c4494` is resolved, not withdrawn**: restoring its exact
accepted **`Result(ResourceStream *, int *, void *, unsigned)`** provider contract
and source gives **MATCH44/44 B, exit0**. New forwarder **`0019d5dc`** now uses the
same canonical Result/int-output API and **MATCHes8/8 B, exit0**. There is no cast,
fake provider, alternate callee identity or accepted credit removal. The accepted
reader no longer differs from its source baseline; it drops naturally from revisions.

New definitions **`00113c0c`** and **`0018ee88`** are withdrawn with exact source,
author receipt, failed variants, real-contract conflicts and measured exits preserved
in **`interface-deferrals-r4.json`**. Their author MATCH forms respectively declare
raw-status providers as Result-returning, and shadow the complete callback-state
global as a scalar byte while widening a canonical byte-mode parameter. Correct
canonical consumers still fail byte matching; no shadow type or codegen steering
is introduced to recover those bytes. The Result wrapper declaration remains
because retained `0010b8b8` really calls that uncredited original function.

Current scope: **676 new units/functions / 57,052 B**, **122 revised predecessor
units / 126 functions**, required union **798 units / 802 functions**, dry ledger
**2,203 units / 2,277 functions**. Exact current inputs are in
**`readiness-scope-r6.json`**, **`HANDOFF.json`** and **`candidates.json`**.
The four earlier current interface deferrals remain, making six later deferrals;
historical `interface-deferrals-r3.json` is still superseded for the counted loop.
`001c20e8` stays MATCH with the mandatory **QUALIFIED** review row.

This closes the known mismatch slice, **not the final readiness gates**. G3 r5 is
historical to its prior scope/header hashes, and no current whole-scope all-MATCH,
native/signature/co-inclusion/indirect/forwarding/argcheck/pre-review PASS is claimed.
Main instructed us to wait for its explicit **cmodels-finalized hash** before
merging newer main, then reconcile accepted `0017a424`, recompute scopes and run
the final merged-tree evidence. Independent canonical probes can proceed now.
All following milestone sections are retained history; their old counts/blockers
are superseded by this section. No full build/suite/prepare/history/push was run.

## Handoff boundary after 600f734

Current corrected G3 r5 is **exit0** on the f24fa0b-based c009 tree, with
**26 of the 29** originally failed units now scoped MATCH; **three** byte
mismatches remain, detailed below. This is a committed continuation, **not
review-ready**: the final passing full sweep/canonical probes/argcheck/pre-review
are still required. No background measurements remain.

At the boundary, Main advanced to **`85fd18dc7173e0542d8c33db157d03b7910c18c5`**
with cmodels code/shared headers. That newer main is **not merged** into the
audited tree here. The explicitly requested
`git merge-tree --write-tree main c009-integrate` nevertheless **exits0** for
`600f734be5d1327edfd9b2cd6994f905af472f00`; exact evidence is
`merge-feasibility-r1.json`. Coordinate Main's cmodels preparation, merge/reconcile
its later inputs, then redo affected final evidence. Do not misattribute r5's
current gate PASS or earlier scoped matches to unmerged cmodels headers.


## Audited Main f24fa0b milestone: three mismatches remain

Merged current Main **`f24fa0b`** cleanly at
**`90cc196cb7107f5c79424d721ec4730b74323495`**. Gates are now frozen **v1.19**.
This brings reviewed tools/model availability, not a c009 model transition:
c009 stays explicitly **armlink5-inline-branchnop-v2 / none**.

Evidence-backed semantic repairs close **nine** of the thirteen original byte
mismatches (`semantic-repair-r1.json` and `semantic-repair-r2.json`); the ruled
counter-loop restoration below closes the tenth. All sixteen prior compilation
failures are also repaired. The actual canonical CommandQueue
now contains its 0x44-byte tail (patch/captured size/complete/flush); five
ExtendedCommandQueue/W19aWritableQueue/CommandQueueDetail shadows are removed.
The query unit also MATCHes after that migration. Actual signed ring-producer
modulo, drawing scale +0x8bc, finished byte +0x8b9 and counter-enabled +8 fields
are restored. The display setup keeps the original unchecked-null bug and
proven first-member DisplayObject prefix.

**Three** last-observed mismatches remain, all exact extents:
`00113c0c` 104/104 B, `0018ee88` 56/56 B, `001c4494` 44/44 B.
These are result-check/default-mode/register-scheduling differences, not an
excuse to change canonical provider contracts, hard-code an instruction address,
or add volatile/register/dead steering. Nine bounded natural source forms
failed; their sources, actual exits and receipts are preserved under
`natural-forms-r1/`. One initial C90 declaration-placement compile failure and
its changed-source correction are both retained. Do not restart those forms.

Stable G3 r4 on the corrected main1.19 tree **exits 0**:
2,204 units, 1,704 checked, zero disagreements/policy findings, 500 unchecked C++
and zero unchecked C. Its exact ledger is `dry-matches-g3-r4.json`; it excludes
the temporarily withdrawn counter loop described below. **G3 r5 also exits 0**
on the restored **2,205-unit** scope: **1,704 checked, zero disagreements,
zero policy findings, 501 unchecked C++ and zero unchecked C**. This is the
current gate verdict; the final all-MATCH/sweep/argcheck/pre-review work below
is still incomplete. No background measurement remains running.

### Main ruling: retain the original counted loop, qualified

Main explicitly ruled that `001c20e8`'s observed counted backedge is faithful
original control flow, not an absent construct added for codegen. Its plain
loop is restored without volatile, asm, artificial uses or register forcing,
and **MATCHes 20/20 B, exit0** (`counter-loop-ruled-r1.json`). The visible
`[INFERENCE]` note preserves its unknown purpose (delay or leftover loop).
The eventual pre-review and independent review row **MUST be QUALIFIED**;
`pre-review-required-qualifications-r1.json` records that mandatory disposition.
`interface-deferrals-r3.json` remains the superseded policy concern/history of
the loop and 8-byte constant form, not a current deferral.

Current scope remains **678 new units/functions / 57,212 B**, **123 revised
predecessor units / 127 functions**, dry **2,205 units / 2,279 functions**.
`readiness-scope-r5.json` binds the required **801-unit / 805-function** union.
Main's manifest is `scratch/integrator/c009/candidates.json`; it is **not ready
for prepare**. After the three fixes: stable whole-scope MATCH, native/signature/
co-inclusion/indirect/forwarding consumers, complete argcheck adjudication and
seven-step pre-review still remain. No full build/suite/prepare/history/push ran.

Main may land cmodels first. The sole overlapping revised accepted source is
`src/wave10/func_0017a424.c`; c009 does not revise `00172c04` or `00173f9c`.
Shared-header overlap is broader: merge/reconcile cmodels if it lands, then
recompute scopes/dry ledger and final input-bound evidence.


## Historical full MATCH coverage and compilation repair slice

At `c43d995`, finalized-predecessor **G3 r3 exits 0**: 2,205 units,
1,704 checked, zero disagreements/policy findings, 501 unchecked C++ and
zero unchecked C. The sequential final match sweep then measured **all
801 required units / 805 functions**, explicitly v2/none and `--show 0`:
**772 units MATCH, 16 compiler failures, 13 byte mismatches**. The shell
stopped there, so native/signature probes did **not** run. Exact per-unit
commands, exits, source hashes and failure output remain in `final-match-r1.json`.

All **16 compiler failures** now have scoped MATCH evidence:
`compile-repair-r1.json` records 14 MATCH plus two remaining mismatches;
`compile-repair-r2.json` records both corrected units MATCH. Fixes reuse actual
canonical members/prototypes: service flag bytes, nested heap/material/buffer
owners, SoundTrack child/task, and the real WorkerThread mutex/stop fields.
No DispatcherWorker or SceneChild shadow was restored. Playback original facts
prove transition +0x34, count +0x9c, handle +0x98, and tail slot byte offset
0x20 (`callback8`), not slot2. The signed-byte IPC wrapper retains signed storage
through a standard unsigned-char object-representation view of its raw-byte output.
Its original uninitialized-error-path bug stays explicit.

These repairs change four headers, so **neither the earlier 772 MATCH rows nor
G3 r3 is a final verdict on the later changed-header tree**. Thirteen original
byte mismatches remain: `00113c0c`, `0018ee88`, `001c1d4c`, `001865ec`,
`0011db78`, `0012ccd8`, `00141514`, `001c20e8`, `001eb1cc`, `001105c0`,
`001eb190`, `001c4494`, `0019eb24`. HANDOFF retains every exact row/output.
The four ExtendedCommandQueue/W19aWritableQueue shadows incorrectly append a
prefix onto the now-complete 0x44-byte CommandQueue and need canonical migration.
The other mismatches require real width/offset/ABI evidence; no dead/register/
volatile steering or guessed aliases are permitted.

Scope remains **678 new / 57,212 B**, **123 revised units / 127 functions**,
dry **2,205 / 2,279**, required union **801 / 805**. The requested
`scratch/integrator/c009/candidates.json` exists but is **not review-ready**.
After the remaining repairs: fresh stable G3 and complete MATCH, native/signature/
co-inclusion/indirect/forwarding sweeps, full argcheck adjudication and seven-step
pre-review are still required. `names-review-r1.json` inventories all 30 c009
name additions, zero modified bindings and two removed obsolete descriptor aliases.
Main currently holds the isolated heavy slot; wait for its explicit
**"slot returned"** before another full audit/build/suite. Static work is allowed.


## Historical finalized predecessor merge at d4f52d9

Merged Main **`bb60cd7`** at **`d4f52d963cae908de5207bb320b2e7cee3a6299a`**.
C008-r3 is finalized, sequence 8, with **1,527 units / 1,601 accepted functions**.
The merge is clean and changes only accepted records, config/matches.json and docs;
no source, header, tool or linker-model adoption arrived. The dry ledger now uses
that finalized canonical predecessor. `readiness-scope-r3.json` binds the
**801-unit / 805-function** final new/revised union with canonical credit names.

The requested `candidates.json` proposal is present; it remains unaccepted and
not yet review-ready. Next is stable G3 r3, then the final MATCH and canonical
checks, complete argcheck adjudication and seven-step pre-review, all nice'd
and sequential. Source-level G3 r2's PASS and all earlier failed evidence remain
preserved under their actual input snapshots. C009 remains explicit v2/none.


## Historical milestone: stable G3 r2 exits zero

`interfaces-stable-r2.json` exits **0** with `--allow-unchecked-cpp`:
**2,205 units, 1,704 checked, zero disagreements, zero policy findings,
501 unchecked C++ units and zero unchecked C**. The exact input ledger is
preserved as `dry-matches-g3-r2.json`. All prior failures remain intact.
The native member's ledger credit identifier was then normalized back to
catalogue `func_001f4cf8`; the genuine own-definition alias stays in names.json.
This label normalization does not change any source, profile or address.

Main subsequently finalized c008-r3 (sequence 8, 1,601 accepted functions) at
`bb60cd7`. Merge that exact main commit next; incoming changes are only accepted
records, config/matches.json and documentation. Then repeat stable G3 on the
finalized successor ledger before the final MATCH/canonical/argcheck/pre-review
checks. C009 stays explicitly **armlink5-inline-branchnop-v2 / none**; do not
merge the parallel adopt-models branch or select v4/v6.

Main's requested manifest is `scratch/integrator/c009/candidates.json`, with the
same schema and canonical credit identifiers as the tracked candidate manifest.
Scope remains **678 new / 57,212 B**, **123 revised units / 127 functions**;
the required final union is **801 units / 805 functions**. This is a completed
G3 remediation milestone, not yet a review-ready checkpoint.


## Earlier stable G3 remediation and native reader contract

Continued merge `b6bfcec` without restarting any completed author search.
Stable `interfaces-stable-r1.json` exited **1**: 2,206 units, 1,704 checked,
one Heap ODR disagreement, one duplicated-local-prototype policy finding,
and 502 unchecked C++ units (**zero unchecked C**). The exact failed report
is retained. `pilot/game_heap.cpp` now includes the canonical Heap definition;
the two redundant `func_0016fca0` declarations were removed because
`game/functions.h` already supplies the shared declaration.

Manual C++ inspection found that both mutable CodeUnitReader reconstructions
duplicated the native character-reader layout without preserving its full API.
`001f4cf8` now defines the frozen
`nw::font::Font::GetCharStrmReader(wchar_t) const` interface: a by-value const
CharStrmReader result, const receiver, and the unused dummy all remain explicit.
The genuine private constructor initializes its const method-pointer member.
This complete native definition is **MATCH 64/64 B, exit 0** in
`native-reader-r2/func_001f4cf8.result.json`; its own same-address mangled name
is adopted. The initial missing-NW_RELEASE compile failure is preserved in
`native-reader-attempts-r1.json`; only the existing upstream release-header
configuration was added, with no frozen header/profile change.

The separate middleware `0013156c` identity/API is not established by that
physical resemblance. Its distinct table/version/namespace cannot be replaced
by the already-bound native member name, a guessed overload, or a shadow class.
It is withdrawn with source and original collection receipt preserved in
`interface-deferrals-r2.json`. Both mutable reader shadows are removed.
The original sixteen-address boundary now retains **five** current destinations
and **eleven** exact deferrals; the earlier six MATCH measurements stay historical.

Current scope: **678 new units/functions / 57,212 B** (game 616/51,528 B,
middleware 37/4,000 B, sdk 25/1,684 B), **123 revised predecessor units /
127 functions**, dry **2,205 units / 2,279 functions**. The final required union
is **801 units / 805 functions**, bound in `readiness-scope-r2.json`.
Stable G3 r2, the final MATCH sweep, canonical probes, full argcheck adjudication
and seven-step pre-review remain required; no review-ready claim is made here.
Checks remain nice'd and sequential. C010's agreed five-unit window has finished
and no peer compiler/audit process remains in the slot.


## Historical initial readiness resume from 1f66a8a on main f8adff4

Main explicitly resumed c009 readiness. Merged prepared main `f8adff4`,
including complete c008-r3 successor `b5e8b8abffe6d2e2d3f259920aa9426bd9c1ca0b`.
The sole merge conflict was the equivalent slot13/14 explanatory comment;
c009's precise context/tail note and before/after names were retained.
Slots 0/1/2 now use the corrected pending/byte-mode, ushort-command/handle-reply
and const-data/handle contracts; ServiceCallbackState's real notification-node
prefix is adopted. No earlier source search or collection was restarted.

The prepared predecessor ledger preserves every prior identity/profile/owner.
Scope is still **679 candidate units/functions / 57,276 B**, **122 revised
predecessor units / 126 functions**, dry **2,206 units / 2,280 functions**.
Authoritative predecessor evidence is main-root
`scratch/integrator/c008-r3/complete/`; older root/final reports are historical.
C010 confirms no compiler/audit job is running. C009 now owns the finite,
nice'd sequential readiness slot. Stable G3, final MATCH, canonical sweeps,
complete argcheck adjudication and seven-step pre-review are next; none is
claimed complete by this merge milestone. No full suite/build/prepare is run here.

## Historical merged wrap-up at 1f66a8a

The user requested wrap-up at this merge milestone. This is a committed,
resumable continuation, **not review-ready**. New static remediation after
`b63317e` remains unexercised: no merged-tree G3, final MATCH sweep, canonical
probe/same-class sweep, complete argcheck adjudication or seven-step PASS exists.

Merged predecessor `642d725079b6a2154bc6f06fa554dcbeb063bcea` after preserving
source-remediation milestone `d031cce`. Its 1,527-unit / 1,601-function ledger
has exactly the same predecessor identities/profiles/owners as the prior dry base.
Current c009 scope stays **679 units / 57,276 B**, dry **2,206 units /
2,280 functions**, and source delta against r3 **122 units / 126 functions**.

Merge choices preserve c009's evidenced `ServiceOutput.channels[2]` while
adopting the corrected shared AuxCallback channels/scalar-count/context type.
Service slots 11/13/15 retain r3's actual context/message contracts and explicit
return-width qualification. The original `00115000` additionally proves slot14:
tail `00115034` loads callback `00242ab0` and context `00242af0`. C009 retains its
existing `before`/`after` names, now both typed context notifications; evidence
is `service-notify-r1.json`. FixC008R3 acknowledged its missing disposition and
is preparing a preserving follow-up/audit fix without taking c009's heavy slot.
Merge that follow-up when supplied; do not silently downgrade slot14 to reserved.

Post-merge follow-up notice from FixC008R3: slot14 was decoded but left
unadjudicated because its consumer was outside selected source rows. Complete
canonical-contract adjudication must not use that scope filter. FixC008R3 also
reports new complete `00122b30` author evidence requiring slot0 int pending +
byte mode, slot1 ushort command + CtrHandle reply pointer, and slot2 const buffer
+ CtrHandle rather than signed result. These additional corrections are pending
its preserving follow-up commit, not already applied or verified in c009.
The serialized audit slot is released to FixC008R3 **before c010** for rebinding
the affected dependencies/G3/probes.

No merged-tree G3/final match/probe/argcheck result is yet claimed. The next command
is whole-ledger interfaces on this stable merged dry ledger; no full build/suite
or acceptance/history/push operation is authorized here.

## Pre-merge d031cce snapshot — preserved unexercised remediation

Continued milestone `b63317e` without restarting prior searches. Current scope is
**679 units/functions / 57,276 target B**: game 616/51,528 B, middleware
38/4,064 B, sdk 25/1,684 B. Dry ledger: **2,206 units / 2,280 functions**.
The source delta against c008-r2 is **122 predecessor units / 126 functions**.
All current units and exact revisions are bound in HANDOFF.

The remaining saved C interface families were migrated to canonical input-update/
session/channel types, ResourceEvent pools, resource records, numeric visual owners,
typed drawing/position prefixes and shared GPU/notification/startup declarations.
These edits have **not yet been exercised** by final matching or stable-ledger G3.

Scoped address/register/storage facts establish that `00195b6c`'s fifth stack
argument is stored at `0049c880` and later called indirectly by `00105444`.
The old scalar declaration was corrected to a callback, and all four StartupState
users now share the callback-bearing header. Scheduler binding stores are rooted
at `00495c8c` with 24-byte stride; the first-iteration consumer trace observes its
callback at +8. `scheduler-bindings-acyclic-r1.json` is expressly **not** an
all-path proof; the complete indirect sweep and manual adjudication remain required.

Three further rows are deferred with exact preserved source/receipt evidence in
`interface-deferrals-r1.json`: `0015e5d4` has incompatible DisplayPaneLink versus
DisplaySurface/SurfaceTextState consumers; `00190d0c` and `00190d68` use valid
standard-C varargs that the frozen G3 C parser does not support. No source-language
waiver conversion, ABI hand-coding or frozen tool change was used to hide those
parser failures. The two independent surface text wrappers remain in scope using
one shared partial field view; complete native identity/allocation remains explicit
`[INFERENCE]`, not a claim that this is DisplayPane.

FixC008R3 has now handed off **`642d725079b6a2154bc6f06fa554dcbeb063bcea`**,
closing its serialized toolchain slot. Merge it immediately, then run c009 G3 and
final matching/probes/argcheck/pre-review on that merged tree. Its five tracked
files correct service slots 11/13/15 and AuxCallback's channels/count/context chain.
Reusable scripts/evidence are in **main-root**
`scratch/integrator/c008-r3/`, not inside the c008-r3 worktree:
`audit_indirect.py`, `final/source-slot-map.json`,
`final/indirect/consumers/results.json`. C009 now owns the next finite audit slot;
c010 has been asked to continue static work only.

`probe_interfaces.py` reuses c008-r2's compile helper for canonical native and
compiler-derived C-signature probes. It is staged but unexercised. No new names
were adopted after collection milestone 1. No full suite/build, prepare/finalize,
history edit or push was performed.

## Milestone 1 at b63317e — preserved collection evidence

Continued `9d6f051` in the existing `c009-integrate` worktree; no completed
author search or destination collection was restarted.

* All **16 unique** originally uncollected new-scope addresses now have exact
  dispositions in `c009/collection-closure-r1.json` and `UNRESOLVED-SCOPE.json`.
  **Six** were collected and measured MATCH at their canonical destinations:
  `002095c0`, `002102b4`, `0014d2e4`, `001eade0`, `0013156c`, `001f4cf8`.
  The duplicate w18b/L046 delivery of `002102b4` earns one scope row.
* **Ten** remain expressly deferred for recorded canonical-contract reasons,
  not as MATCH credit: `00214d9c`, `0010e1ac`, `001671d4`, `001e3380`,
  `001dce40`, `001dca5c`, `0015eae0`, `001acc88`, `001aea80`, `001cf974`.
  Their original sources, immutable MATCH receipts and receipt/source hashes
  are retained. All original receipt/source hashes agree.
* Current scope: **682 units/functions / 57,484 target B**:
  game 619/51,736 B; middleware 38/4,064 B; sdk 25/1,684 B.
  Dry ledger: **2,209 units / 2,283 functions**. Revised predecessor scope:
  **104 units / 108 functions**, including new `0014de8c` and `00179160` revisions.
* Canonical migrations reuse ResourceDescriptor, tagged allocation, shared
  MotionPair/display-handle types and the ARM EABI constructor callback result.
  The concrete StyleOwner contains the shared display prefix plus 32 handles;
  other display sets retain their evidenced variable-length prefix.
  Both reader method-pointer tables stay external original data: no method
  aliases or data credit were added.
* Adopted names: `_ZN13PaneIndexListC1Eiz` has the natural non-inlined
  `002095c0` caller witness; RuntimeMemberOwner C1/C2 are same-address own
  definitions; RuntimeMember C1 is the compiler-emitted element callback in
  matching `0014d2e4`. Names and semantic roles remain `[INFERENCE]`.
* Evidence: `collection-r1.json` (nine MATCH results plus the initial style-owner
  mismatch); `collection-r2.json` (same 244/248 mismatch after separate color
  assignments); `collection-r3.json` (concrete shared-prefix containment MATCH,
  248/248). All commands used `--show 0` and captured exits. Failures are kept.
* FixC008R3 is still running its assigned serial matching/audit checks and has
  requested that c009 hold whole-ledger/heavy checks until its committed handoff.
  Merge its committed branch first, then run stable-ledger G3 remediation,
  all-new/revised matching, canonical native/signature/indirect-consumer sweeps,
  complete argcheck adjudication and all seven pre-review steps.

`HANDOFF-from-9d6f051.json` preserves the incoming handoff. The prior 166-header
probe is historical evidence, not a probe of these newer header changes.
No full suite/build, prepare/finalize, history edit or push was performed.

## Previous handoff at 9d6f051 — preserved historical state

The user requested wrap-up during remediation. Preserve this branch; do not
prepare/finalize or treat any scope count as accepted/MATCH credit.

* Resumed `1cc9f60`; merged adopted main `959311b` and c008-r2 `38b4c01`
  (merge `9218542`). The current dry ledger is successor-based, not failed c008.
* Current candidate scope: **676 units/functions / 54,648 target B**:
  game 614/48,964 B; middleware 37/4,000 B; sdk 25/1,684 B.
  Dry ledger: **2,203 units / 2,277 functions**. Source delta against c008-r2:
  **102 predecessor units / 106 functions**, all listed in HANDOFF.
* The final compile-only co-inclusion of **166 game headers exits 0**
  (`c009/cpp-compat-r5.json`, input `cpp-compat-r3.cpp`). Native-unit, actual C
  signature, indirect-consumer and matching sweeps have not discharged those
  contracts; this is not a whole-ledger G3 or matching pass.
* The intermediate G3 remediation run exits 1 (48 disagreements / 28 policy /
  500 unchecked). It ran while edits continued and before the two new deferrals,
  so those are historical observed counts, not a current final audit.
* Additional evidenced deferrals: `0010cc94` (accepted native ResFontBase
  constructor versus EntryObject shadow) and `00203810` (concrete scene owner/
  initialization/destruction contracts; a common dispatch view is insufficient).
  Their sources/contradictions are in `contract-deferrals.json`; the shared
  shadow declarations were removed, not aliased. Preserve the original three
  approved c009 deferrals and six c008 deferrals, plus c008-r2's `0016b6a4`.
* All sixteen original uncollected new-scope MATCH addresses remain open in
  `UNRESOLVED-SCOPE.json`. None was silently dropped or newly collected here.
* New information at wrap-up: FixC008R3 owns `c008-r3` from current main
  `f3ae330`, correcting slot 11 context and slot 15 context/saved arguments/
  result with an indirect-consumer audit. c008-r2 remains unfinalized.
  Reconcile that successor before claiming a review-ready predecessor base;
  these fixes were not duplicated in c009.

Canonicalization includes shared NameTable arrays, address-rooted global fields,
canonical Heap and AudioTaskManager clients, ResourceEvent's display prefix,
the shared ScreenResourceManager view, and raw IPC scalar/result/handle contracts.
The source changes still need final byte matches. Partial views do not establish
every original complete class or allocation size. The array-constructor callback
now carries the shared ARM EABI pointer-result contract and still needs its
revised accepted-row byte/review closure. No new names-overlay alias was adopted
during this continuation; inherited proposals remain subject to final review.

The c008-r2 reusable scripts are at `../c008-r2/scratch/integrator/c008-r2/`:
`probe_contracts.py` exposes `compile_probe(source, profile, directory)` and
`probe_signatures.py` derives C signatures from compiler-preprocessed ASTs.
The c009-wide reuse/sweep has **not run**. `c009/declaration-plan.json` records
the 242 shared-prototype removals; `c009/sweep.py` is the sequential match driver.
HANDOFF now binds **2,371 live input hashes** and preserves the initial record
separately as `HANDOFF-from-1cc9f60.json`.

Final MATCH sweep, stable-ledger G3 exit 0, complete argcheck adjudication and all
seven pre-review steps remain unexercised. No full build/suite, prepare/finalize,
history edit or push was performed.

## Original preserved state at 1cc9f60

Worktree: `/home/michael/code/kanken-training-2-decomp/scratch/worktrees/c009`.
Branch `c009-integrate`, original base `11e1992`. This is preserved incomplete work,
not a ready checkpoint. No reset, provider switch, replacement author search, full
build, test suite, prepare or finalize was performed. Existing `inputs/`,
`initial-*.json`, `filter-skipped.json` and `filter.py` remain in `c009/`.

The original `HANDOFF-from-1cc9f60.json` binds 2,374 source/header/configuration hashes and contains
all 37 revised predecessor units. It records 678 new units/functions and 55,272
recovered target bytes: game 616/49,588 B; middleware 37/4,000 B; sdk 25/1,684 B.
These are unaccepted scope totals, not an edited-tree final-match claim.
The dry ledger is accepted c007 + original c008 + c009 = 2,206 units. It is NOT yet
based on FixC008R2's pending revision. Do not repeat completed destination collection;
continue from the current canonical sources and the saved inputs/receipts.

## Continuation on adopted v9-r2

Merged current `main` (959311b) into `c009-integrate` without restarting the
preserved source integration. The catalogue recheck intersection is exactly
`00203810` and `00214580`; `match.py --show 0` reports `00214580` MATCH.
`00203810` exited 2 on incompatible scene declarations, not a byte comparison
failure. It was subsequently withdrawn on concrete owner-contract evidence;
reinstatement needs real per-owner initialization contracts and the changed
catalogue extent match. Diagnostics remain in `c009/catalogue-rechecks.json`.
Merged `c008-r2` commit `38b4c01cdabd94141f5d00c74c49ac8e1c5d9eac` as
`9218542`. The dry ledger is now based on that successor's 1,601 functions;
`0016b6a4` stays deferred. Conflicts retained r2's ManagedExpHeap containment,
DisplayPane material-bearing view, mutex/channel ownership, and InputSessionContext
resetters; C009's extra callback/gain fields and ItemPageSet metric fields were
retained without reintroducing the old owner/value declarations.

Declaration migration has removed 242 source-local shared prototypes from 116
files and added their compiler-derived interfaces to existing game headers.
The intermediate remediation report (`interfaces-remediation-r1.json`) has 48
disagreements / 28 policy findings / 500 unchecked, versus the inherited 75/218/501.
It is not a final gate result: remediation continued while it ran. The native
font wrapper `0010cc94` is now explicitly deferred with exact constructor/owner
contradiction evidence in `contract-deferrals.json`; no EntryObject shadow or
false extern-C declaration of the accepted NW4C constructor is retained.


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
