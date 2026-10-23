# c010-r6 candidate — known issues and qualifications

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
