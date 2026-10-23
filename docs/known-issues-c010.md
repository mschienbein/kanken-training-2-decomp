# c010 candidates — known issues and qualifications

## c010-r8 accepted (history seq 11)

Finalized 2026-10-23 as history seq 11: **2,682 functions / 288,580 B** credited of denominator
v13-r2 (4,571 functions / 1,121,674 B, **25.73%**). c010 adds **413 new functions / 39,208 B** and
12 accepted moves (192 B, credit unchanged), with explicit linker `armlink5-inline-branchnop-v6`
and literal model `anonymous-string-records-v1`.

- **Reviews:** both PASS_WITH_QUALIFICATIONS with no findings (`accept/c010-r8/reviews/`).
  - codex-a: 111 PASS / 516 QUALIFIED of 627 rows.
  - codex-b: 135 PASS / 492 QUALIFIED.
  - Every QUALIFIED row's reason is in the sealed records under `scratch/review/c010-r8/`.
- **Rounds:**
  - c010-r5 pre-review: FAIL.
  - c010-r6: both reviews FAIL (array-runtime callbacks, envelope object identity, BasicSound slot,
    base-wrapper downcast, C call into a native member, non-adopting SoundVoiceObject provider).
  - c010-r7: codex-b FAIL (three more array-runtime callbacks).
  - c010-r8: closed the class mechanically with an exhaustive runtime-callback table
    (`scratch/integrator/c010/r8/RUNTIME-CALLBACK-CLOSURE.json`: array runtime, atexit, thread
    entries and native-member slots), with zero unresolved new or revised rows.
- **Deferred, uncredited:** 94 new functions / 8,404 B in 93 units, listed in the r8 section below
  and the deferral records. Each failed an exact type or contract fix and was not credited rather
  than bridged with casts.
- **Inherited accepted attention.** The closure found 48 definite type/role mismatch rows,
  7 nominal custom-family rows and 26 unmodeled rows on providers accepted in c001–c009 and
  unchanged here. They are outside c010's scope; both reviewers verified that no new or revised
  c010 unit depends on them through an incompatible type. ctypes1 r2 (`types-compose-r2`) fixes
  32 rows exactly; the rest are held with reasons in `scratch/integrator/ctypes1/r2/`.
- **argcheck** exits 2 (90 issues / 513 notes). All were adjudicated in the r6/r7 records and again
  by both r8 reviewers. The two facts new in r7 (`func_00215ec4` r1/r2 at `0x001b4904`) are
  arguments of the unattributed removed-callee rewrite at `func_001b48c8` +52.

## c010-r8 — exhaustive runtime-callback closure

This **unaccepted successor** remains on branch `c010-r7`, preserving immutable
`819edc6f` and the registered c010-r7 review records on main `8725b134`.
Compiled source is pinned to `102a6d80`; subsequent commits contain evidence and
this documentation only. Historical r6/r7 preparation and review results are
not reused as current acceptance.

### Current scope and honest dispositions

- **414 candidate units / 425 claims:** **413 NEW functions / 39,208 B** and
  **12 accepted-identity moves / 192 B**.
- **94 NEW-only deferrals / 8,404 B in 93 units.** The five r8 additions are
  `0014b54c` (**64 B**), `00107ce4` (**216 B**), `0014b85c` (**124 B**),
  `001d4078` (**48 B**) and `001e5a20` (**120 B**): **572 B** total.
  Their original direct/address consumers are uncredited; no current NEW
  dependent remains. Unselected best attempts are retained, not acceptance
  credit or callable-interface approval.
- All **2,269 accepted predecessor identities / 249,372 B** survive. Proposed
  ledger: **2,618 units / 2,682 functions / 288,580 B**, including **24
  helper-only units**. The accepted opaque `FontVTable`, `float(float)`
  converter protocol and c009-owned deferrals remain unchanged.

**Sealed c010-r7 codex-b F01:** `0014d664` and `001c4b10` now have the actual
`void *(void *)` array-runtime signature. Concrete element storage is recovered
inside each body; there is no callback cast or specialized runtime declaration.
For `0014b54c`, both the honest generic recovery and genuine placement-
construction alternative failed to MATCH (**172/64 B** and **180/64 B**).
The provider is therefore NEW-only deferred, not forced back to a concrete
callback type or a code-generation workaround.

The class sweep additionally repairs sound-loader thread entry `0010a704` to
the actual `void(void *, void *)` `ThreadStartContext.entry` protocol and empty
atexit callbacks `001c5764`, `001c588c`, `001db960` to `void(void *)`.
The honest generic stream-thread repair `00107ce4` was **220/216 B** and is
also NEW-only deferred. Three custom frame-timing, result/context and paired
radar callback families remain unmodeled; their NEW providers are the other
three deferrals above. Their receiver, argument and result types are not guessed.

### Mechanical closure and inherited attention

Authority: `scratch/integrator/c010/r8/RUNTIME-CALLBACK-CLOSURE.json`.
The census decodes **all 4,743 catalogue functions**, including uncredited
callers, scans **20,327 original call/tail sites** and **all 3,228 project
source/header files**, and classifies **308 passed-code candidates** while
retaining **547 indirect-parameter candidates**. Saturated source-glob buckets
are partitioned rather than silently truncated.

The table covers **53 runtime/API entries**, **272 original callback-parameter
rows**, **70 explicit source calls**, **157 provider/contract rows**, all
**five array entrypoints/veneers**, **70 original array callsites** and **45
array-provider addresses**. It inventories atexit registration, scheduled and
thread entrypoints, audio/event callbacks, eight source-only noninvoking typed
registration helpers and four native member-callback families. All six native
character-reader methods are checked as genuine members of their declared
owner; C++ free functions and spare constructor aliases never receive native
constructor/destructor treatment.

**All 17 current NEW and seven revised/moved callback-provider addresses are
closed; zero unresolved NEW/revised rows remain.** Existing accepted providers
are reported separately for Main: **48 definite type/role mismatch rows** and
**seven nominal/custom-family qualifications**, all unchanged accepted
identities. Another **26 unmodeled passed-code candidates** (24 unchanged
accepted, two unclaimed) remain explicit rather than being asserted noninvoking
or excluded by assumption. These are inherited review/gate obligations, not
author permission to modify accepted source or silently waive a contract.

### Complete reproof and integrator boundary

Final `match-r8-r2` **exited 0**: **2,594/2,594 sequential, niced, single-process
matcher commands exited 0; all 2,682 required functions MATCH** under explicit
v6/L1. All 24 helper-only units are supplied in their natural contexts, and the
complete rewrite projection reports `scope_complete=true`.

Earlier focused attempts and the first complete attempt remain immutable,
including every nonzero exit; the latter exposed the unmatched stream-thread
repair instead of narrowing the assertion to a passing subset.

Current handoff: `scratch/integrator/c010/r8/HANDOFF-r3.json`,
`NEW-DEFERRALS-r4.json`, `SOURCE-CONTEXT-r4.json` and
`REMAINING-HEAVY-COMMANDS-r3.json`. The dry ledger is `dry-matches-r4.json`.
No whole-ledger interfaces/argcheck gate, full build, suite, prepare, review
registration, finalize or push was run by the author. Main alone must integrate
without source-context drift, resolve/adjudicate inherited accepted attention,
run the listed heavy gates, prepare `c010-r8` with two fresh full builds and
suite, and obtain two independent bound reviews before finalizing.

## c010-r7 — both reviews folded with real-object and native-owner boundaries

This **unaccepted successor candidate** starts from frozen main `7083a0a3`
(prepared c010-r6 source `3caf7c4d`), over finalized c009-r6 and denominator
v13-r2. Historical c010-r6 prepare/review FAIL records are unchanged.

### Current scope

- **419 candidate units / 430 claims:** **418 NEW functions / 39,780 B**, plus
  **12 accepted-identity moves / 192 B**. Two helper-only candidate units earn
  no function/data credit.
- **89 NEW-only deferrals / 7,832 B in 88 units:** the inherited 83-function
  boundary plus three native sound setters (**276 B**), archive caller
  `00140ecc` (**244 B**), voice-slot provider `001d7e6c` (**112 B**) and
  proactive release-event callback `001d9ca0` (**68 B**).
  No accepted identity is deferred.
- All **2,269 accepted predecessor identities / 249,372 B** survive. Proposed
  ledger: **2,623 units / 2,687 functions / 289,152 B**, including 24 helper-only
  units. These proposal counts are not acceptance credit.

Current authority: `scratch/integrator/c010/r7/HANDOFF-r2.json`,
`NEW-DEFERRALS-r2.json` and `PROACTIVE-SWEEP-r2.json` in that same directory.
The handoff binds the fresh complete sequential matcher result, not the frozen
r6 receipt or a focused subset.

Final combined-review reproof **exited 0**: **2,599/2,599 matcher commands
exited 0; all 2,687 required functions MATCH**, including the natural contexts
for all 24 helper-only definitions. Explicit v6/L1 and frozen per-unit profiles
were used with `--show 0`; the complete rewrite projection reports
`scope_complete=true`.

### Sealed codex-b findings

- **C010R6B-F01 — genuine generic callback repair:** the three C construction
  callbacks `00156dbc`, `001c4490`, `001c5d90` now have the actual
  `void *(void *)` interface. Only a provider that writes concrete element
  fields recovers its actual element pointer inside the body. The
  `001911c4` ARM-to-Thumb veneer is declared once beside the existing generic
  array runtime, with the identical callback, element-size and count types.
  The specialized `StaticServiceEntry` runtime declaration is removed; no
  function-pointer cast, adapter or address-only-registration qualification.
- **C010R6B-F02 — one shared envelope object:** initializer `0021443c` constructs
  actual `const SoundEnvelope data_0024380a` through its shared constructor,
  matching the accepted readers and the sibling `002437f8` precedent. The
  local `AdshrCurve` model and its obsolete initializer-name binding are
  replaced. Original variable/TU spelling remains `[INFERENCE]`; no five-byte
  layout equivalence is used as a type alias.
- **C010R6B-F03 — explicit dependent NEW-only deferrals:** all three native
  `BasicSound` setters that invoke the unclosed playback-handle slot are
  removed from credit/source. Accepted `001d3148` and its existing receiver/
  result model stay unchanged. No invented native provider, name alias,
  receiver/result cast or implicit native-family approval is introduced.

### Additional sealed codex-a findings

The common generic-callback and envelope findings **F01/F02** are the same
genuine repairs described above. The remaining findings are folded as follows:

- **F03 — genuine texture-wrapper object:** `ResAnim::Cleanup` constructs a
  real `ResTextureAnim` value from the tag-selected payload pointer. The
  complete local `ResMemberAnim` wrapper is never downcast to a derived
  reference. This is the existing value-wrapper convention, not an assertion
  that a payload tag changes a C++ wrapper's dynamic type. Exact **188 B MATCH**.
- **F04 — dependent NEW archive caller deferred:** `00140ecc` is removed
  rather than invoking native `SoundInfo::GetSoundType` through a separate
  `SoundRecord` receiver and integer-result facade. The accepted native
  provider is unchanged; no record alias, payload copy or type-erased adapter.
- **F05 — dependent NEW voice provider deferred:** `001d7e6c` is removed
  rather than supplying a free `SoundOperation *` cleanup in the bound native
  `SoundVoiceObject` family. The parallel `SoundOperation`/entry definitions
  and prototype are removed. Accepted native constructor/owner forms survive.

The extended sweep also makes the decoder arena producer explicitly construct
the actual derived `AudioDecodeEvent`, with no base-object downcast; exact
**148 B MATCH**. Two honest real-derived release-event construction variants
remain **68/68 B mismatches**, so NEW callback `001d9ca0` is explicitly deferred,
not retained with a questionable storage projection or code-generation forcing.


### Proactive sweep and continuing qualifications

The sweep covers every NEW/revised unit named by the frozen r6 prepare:
588 input units, of which 582 remain and six are explicitly deferred.
It inventories generic runtime boundaries, reconstructed objects, complete
base-value/derived casts, direct C-to-native relocations and bound native slots.
Both generic providers and the veneer now agree; RTTI roots, handle wrappers,
vector storage and both envelope defaults retain one published object type.
The three isolated initializer records have no competing live declaration
in the exact-symbol source/header search.

Known static native providers are distinguished from unrecovered dynamic
populations. The retained native archive accessor and Pane uniform-command
providers use their real native members. No remaining selected NEW/revised
source invokes `GetBasicSoundPlayerHandle`; complete sound hierarchy recovery
is still a prerequisite to re-entering the three deferred setters. Existing
opaque/dynamic interfaces and inherited qualifications are not declared proved
compatible merely because their callers match.

The current compiled direct-call census covers all 580 scoped primary objects
and 165 native provider identities. Apart from the removed NEW archive edge,
the only C-named-to-native edges are the two pre-existing accepted WaveFile
calls in `001d3c78`; their callee/receiver/result interfaces are unchanged from
the accepted predecessor. This preserves, and does **not** newly approve, that
inherited boundary or claim a complete WaveFile family cutover.

Native-binding prefix scans are not table-extent proofs. Apparent candidates
at ShaderProgram `+0x18` and DisposeCallback `+0x34` lie beyond those classes'
declared virtual prefixes and are not assigned invented owners. The actual
ArcResourceAccessor provider remains native; the proved SoundVoiceObject
provider is deferred. Unrecovered historical extents/populations stay explicit.


All earlier R1 erased-helper identity, L1 placement/extent, C-only scheduler
MUST-discard, ARMCC-extension, noinline, partial-owner and `ORIGINAL BUG:`
qualifications continue to apply. The new callback fixes are actual invoking
contracts, not additional no-op-registration hypotheses.

Exact remaining Main-only interfaces/argcheck/prepare argv are in
`scratch/integrator/c010/r7/REMAINING-HEAVY-COMMANDS-r2.json`, against the fresh
r7 proposal and complete rewrite projection. No heavy gate, full build, suite,
prepare, review registration, finalization or push was run by this author.
Both registered r6 FAIL reviews are retained and folded here. Main must integrate
clean committed inputs and obtain fresh complete acceptance evidence.

## Historical c010-r6 candidate

This is an **unaccepted candidate**, based on c010-r5 `0c62c893` and integrated
onto prepared c009-r6 `8e488abc` (prepared source `f47bb29f`). The c010-r5 tree
and its independently sealed FAIL remain immutable. Main owns acceptance,
canonical ledger/history, review registration, finalization and pushing.

## Current scope

- **425 candidate units / 436 claims:** **424 NEW functions / 40,480 B**, plus
  **12 explicit accepted-identity moves / 192 B**. Two helper-only candidate
  units receive no function/data credit.
- **83 NEW-only deferrals / 7,132 B in 82 units:** 69 inherited c010-r5 deferrals
  plus **14 additional NEW functions / 1,512 B**. This does not reclassify older
  deferred/re-entered histories. Current authority:
  `scratch/integrator/c010/r6/NEW-DEFERRALS-r1.json`.
- All **2,269 prepared c009-r6 identities / 249,372 B** remain. Three list
  identities stay at their exact prepared C owners rather than moving into an
  incomplete native cutover; no accepted identity is deferred or re-awarded.
- Proposed ledger: **2,629 units / 2,693 functions / 289,852 B**, including
  **24 helper-only units**. Proposed numbers are not accepted credit.
- c009-owned deferrals `00131540`, `001f4920`, `001efcec`, `001f4994` and
  DisplayTarget `00117854` remain unselected. The adopted
  `CommandQueueCompletionCallback(int cursor)` contract is preserved.

Current evidence entry point: `scratch/integrator/c010/r6/HANDOFF-r1.json`.
Every primary in the proposed ledger is covered by sequential, niced,
single-process `match.py`, using frozen per-unit profiles, explicit v6/L1,
`--show 0` and natural helper contexts. Final complete reproof **exited 0**:
**2,605/2,605 matcher commands exited 0; all 2,693 required functions MATCH**,
with all 24 helper-only definitions supplied in context. The complete rewrite
projection has `scope_complete=true`. No interfaces, argcheck, full build,
suite or prepare was run by this author.

## c010-r5 pre-review remediation

The independent pre-review reported **7 blockers / 0 major / 0 minor**. Its
findings are ground truth, not rerun unchanged to challenge them. Detailed
per-finding dispositions and source evidence are in
`scratch/integrator/c010/r6/REMEDIATION-r1.json`.

| Finding | Current disposition |
| --- | --- |
| **F01 — scene slots** | Genuine shared owner correction: the concrete prepare slots now take `W23FlowScreen *` and `W6SelectionReset *`, exactly as their revised `001ab65c`/`001bd9f0` providers. Their defining shared headers are included. Neither prepared identity is deferred. |
| **F02 — float PMFs** | NEW `0017e3d8` is explicitly deferred. Distinct free-function providers are not converted into `AnimationTrack` members. |
| **F03 — Layout factories** | NEW `001ce8f4`, `001ce940`, `001ce8b8` are explicitly deferred; their obsolete explicit-this factory declarations and unused `LayoutBindingManager` facade are removed. No `AnimTransform`/manager alias or returned-object cast. |
| **F04 — archive accessor** | Genuine native `ArcResourceAccessor::GetResource` over its actual `ARCHandle`, root and name members; natural narrow-to-wide helper retained. The parallel `ArchiveResourceAccessor` and C-wrapper declarations are removed. |
| **F05 — global identity** | One `ServiceClientState` for `data_00242c18`; one `W22StreamWorker` including the initializer prefix; native const RTTI roots; existing `StaticHandle` used consistently by definitions and every selected client. |
| **F06 — native boundaries** | Exact prepared C owners of `0018d868`/`00191154`/`00191178` restored and their three moves cancelled. NEW range/list/Material/wave dependents are deferred instead of reinterpreting surviving accepted C clients. No full native-family cutover is claimed. |
| **F07 — record overlays** | Four exact owner fixes: notification selection pairs, coordinator DSP volume subobject, `ObjectManager.voice_requests`, and existing kind-zero `Command` payload convention. The two selection-animation helpers and pool release consumer are deferred because their remaining real owner relationships are unproved. |

Additional NEW deferrals are:
`0017e3d8`, `001ce8f4`, `001ce940`, `001ce8b8`, `00185bb0`, `0014d1ec`,
`00138f38`, `001d2c44`, `001cc1d4`, `001312d4`, `0014bc70`, `00171a98`,
`001a78a4`, `00182994`.

The NEW native-list mutation dependency set was derived from actual emitted
references, not only textual C names. Honest C registry-client rewrites were
exercised but still mismatched; those NEW callers are deferred rather than
retaining mismatching bodies. Accepted C interfaces remain intentionally intact.
Source removal does not imply every uncompiled original callee is now modeled.

## Continuing source/model qualifications

- **Native archive vtable:** `[INFERENCE]` the explicit undefined native
  destructor key is a selective-link choice, not recovered original syntax.
  It preserves the already witnessed external `_ZTV` binding; no competing
  vtable/destructor implementation or extra credit is emitted. The installed
  provider uses a genuine member and the real native owner, never an
  explicit-this/free-function substitution.
- **Prior resource dispatch/ownership fixes remain:** native
  `ResTextureMapper`/`ResTexture`/`ResLookupTable` contracts and exact prepared
  lookup move owners survive. NEW mapper `001c8af0` and cube `001c7d7c` remain
  deferred; no obsolete pointer facade or inferred cube alias is restored.
- **Prior reader fix remains:** the secondary descriptor contains real
  `u16 (CharStrmReader::*)()` providers. UTF16/CP1252 accepted identities retain
  explicit moves, with distinct actual UTF8/SJIS Copy members.
- **Typed source-only registration:** the `SoundResourceCache *` destructor
  keeps its exact callback type through an uncredited, noninvoking helper.
  Its erased historical identity remains `[INFERENCE]`, not an invoking adapter.
- **Opaque font interface and converter protocol:** `FontVTable` remains
  incomplete; no same-layout height/count callable facade is reintroduced.
  All converter cells keep `float(float)`, including the identity result and
  the random provider's unused incoming phase.
- **Archive search attribute:** `__declspec(noinline)` is an `[INFERENCE]`
  code-generation qualification, not recovered original syntax. It applies to
  `001650f8`, `001f1ebc`, `001f1ed8`, `001f1f20`, `001f1f34`, `001f1f4c` in
  `src/checkpoint10/alternate1/func_001650f8.c`.
- Explicit selection remains `armlink5-inline-branchnop-v6` and
  `anonymous-string-records-v1`. R1 erased helper identities and L1 historical
  TU/pool extent/order remain qualified. Helpers/external data earn no credit.
- Scheduler callbacks remain `int(void)` with real result providers. Qualified
  C-only procedures retain mandatory discard-result obligations; no invented
  returns, C++ fall-through bodies or incompatible callable conversions.
- Existing `ORIGINAL BUG:` annotations, guard/runtime qualifications, partial
  native target-version prefixes and unknown dynamic populations remain.
  MATCH does not establish an unknown complete object extent or owner family.
- FileSession keeps one named active/free storage union shared by C/C++, its
  actual `+4` handle and genuine release/reset contracts. The previously
  rejected unnamed-union attempt remains a failed record.

## Remaining preparation blockers

1. The applicable c009-r6 predecessor must finalize under its frozen evidence.
2. Inherited accepted scene/allocator/Pane–Material/sound/legacy-font interface
   prerequisites still need supported Main closure/adjudication. NEW-only
   deferrals are not a waiver for inherited defects or unknown populations.
3. Main must integrate clean committed inputs, execute and adjudicate whole-ledger
   interfaces and complete-rewrite argcheck, then prepare with two fresh full
   builds and the suite, followed by two independent bound reviews.

Exact argv: `scratch/integrator/c010/r6/REMAINING-HEAVY-COMMANDS-r1.json`.
Any integrated source/header/profile/name/catalogue/composition change requires
fresh affected-closure evidence under unused names. Prior failures, source
snapshots and reproof records are retained rather than overwritten.
