# c010 candidates — known issues and qualifications

## c010-r7 — generic callbacks, shared envelope and native getter boundary

This **unaccepted successor candidate** starts from frozen main `7083a0a3`
(prepared c010-r6 source `3caf7c4d`), over finalized c009-r6 and denominator
v13-r2. Historical c010-r6 prepare/review FAIL records are unchanged.

### Current scope

- **422 candidate units / 433 claims:** **421 NEW functions / 40,204 B**, plus
  **12 accepted-identity moves / 192 B**. Two helper-only candidate units earn
  no function/data credit.
- **86 NEW-only deferrals / 7,408 B in 85 units:** the inherited 83-function
  boundary plus native sound setters `001913fc`, `001d296c`, `001d2b8c`
  (**276 B**). No accepted identity is deferred.
- All **2,269 accepted predecessor identities / 249,372 B** survive. Proposed
  ledger: **2,626 units / 2,690 functions / 289,576 B**, including 24 helper-only
  units. These proposal counts are not acceptance credit.

Current authority: `scratch/integrator/c010/r7/HANDOFF-r1.json`,
`NEW-DEFERRALS-r1.json` and `PROACTIVE-SWEEP-r1.json` in that same directory.
The handoff binds the fresh complete sequential matcher result, not the frozen
r6 receipt or a focused subset.

Final complete sequential reproof **exited 0**: **2,602/2,602 matcher commands
exited 0; all 2,690 required functions MATCH**, including the natural contexts
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

### Proactive sweep and continuing qualifications

The sweep covers every NEW/revised unit named by the frozen r6 prepare:
588 input units, of which 585 remain and three are explicitly deferred.
It inventories invoking generic runtime boundaries, explicit callable source
expressions, reconstructed storage types and native/virtual consumers.
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

All earlier R1 erased-helper identity, L1 placement/extent, C-only scheduler
MUST-discard, ARMCC-extension, noinline, partial-owner and `ORIGINAL BUG:`
qualifications continue to apply. The new callback fixes are actual invoking
contracts, not additional no-op-registration hypotheses.

Exact remaining Main-only interfaces/argcheck/prepare argv are in
`scratch/integrator/c010/r7/REMAINING-HEAVY-COMMANDS-r1.json`, against the fresh
r7 proposal and complete rewrite projection. No heavy gate, full build, suite,
prepare, review registration, finalization or push was run by this author.
Main must retain failed r6 evidence, fold all independent review findings,
integrate clean committed inputs and obtain fresh complete acceptance evidence.

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
