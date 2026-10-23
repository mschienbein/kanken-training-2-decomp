# c010-r5 candidate — known issues and qualifications

This is an **unaccepted candidate**, integrated onto prepared c009-r5
`e09feab5` (prepared source `c99f2c67`). It does not finalize c009-r5 or authorize
c010 preparation. Main owns acceptance, canonical ledger/history, independent
review registration, finalization and pushing.

## Current scope

- **439 candidate units / 453 claims:** **438 NEW functions / 41,992 B** and
  **15 explicit accepted-identity moves / 348 B**. The two helper-only candidate
  units receive no function/data credit.
- Current prereview deferrals: **69 NEW functions / 5,620 B in 68 units**.
  This is the inherited 66-function set plus NEW `00113e20` (88 B),
  `001da39c` (208 B) and cube cleanup `001c7d7c` (156 B). It is not a
  reclassification of older deferral/re-entry history. See
  `scratch/integrator/c010/r5/NEW-DEFERRALS-r2.json`.
- The proposed complete ledger preserves **all 2,270 prepared c009-r5 identities**
  and contains **2,708 functions / 291,420 B in 2,641 units**, including 24
  helper-only units. These are proposed counts, not new accepted credit.
- `00131540`, `001f4920`, `001efcec` and `001f4994` remain **c009-owned deferrals**;
  c010 neither selects nor re-awards them.

The current evidence entry point is
`scratch/integrator/c010/r5/HANDOFF-r1.json`. It records the actual sequential
matcher outcomes; the source/context and complete scope are separately bound.
No interfaces, argcheck, full build, suite or prepare was run by this author.

## Prereview dispositions

- **F03/F09, resource dispatch and ownership:** preserve c009-r5's native
  `ResTextureMapper`/`ResTexture` value contracts and real `.cpp` providers.
  The three lookup moves now name the exact prepared `.cpp` owners, whose
  superseded definitions are retired. NEW cube cleanup `001c7d7c` and NEW
  pointer-form mapper `001c8af0` remain explicitly deferred; their obsolete
  sources, pointer-facade header and the inferred C++ cube alias are removed.
  Honest canonical-linkage cube forms were exercised but are not byte-exact;
  the last 156-byte form still differs at `+0x3d`. No mismatching provider,
  incompatible conversion or alias is selected. The other selected resource
  providers use the genuine native cutover, not a facade over the old types.
- **F05:** the secondary reader descriptor has four actual
  `u16 (nw::font::CharStrmReader::*)()` providers. UTF8/SJIS Copy bodies are
  native members; UTF16/CP1252 retain explicit accepted-identity moves. There
  is no conversion from unrelated free functions into member pointers.
- **F08:** the exact `void(SoundResourceCache *)` destructor is supplied to an
  explicitly typed, source-only, noninvoking registration helper. Its historical
  erased identity is **[INFERENCE]**. The helper neither stores nor invokes the
  callback, is not an adapter, and receives no credit.
- **F01/F02/F04/F06/F07/F12:** inherited NEW-only deferrals remain explicit.
  They do **not** establish accepted scene, command-member, graphics allocator,
  Pane/Material, sound lifecycle or legacy/retail-font contracts.
- The prepared **opaque `FontVTable`** is preserved. A completion also types
  existing static tables even when their installer is unchanged; incompatible
  height/count receivers or return types cannot be hidden by a new local name.
  The additional glyph/count consumers above are therefore deferred. Serialized
  `GlyphWidths` data does not expose any callable font slots.
- The stale c010 scene-table completion is removed. Prepared `SceneDispatch`
  and all 42 prepared global scene callback bodies are restored; the prepared
  concrete lifecycle table forms are not replaced with a generic receiver.
- The NEW `SoundInstanceVTable.priority_changed` extension is removed rather
  than retaining the receiver-dropping contract behind a deferred caller.
- The prepared **float-to-float converter protocol** is retained for every
  initializer cell. The identity provider returns its phase; the random provider
  keeps its caller-supplied phase even though the body ignores it.

The all-candidate source audit is
`scratch/integrator/c010/r5/CALLABLE-AUDIT-r3.json`: every candidate unit is
hash-bound; changed callable-header families and the original 70-table /
479-slot semantic inventory are adjudicated explicitly. Unknown dynamic target
populations and unrecovered providers stay unresolved, not mechanically proved
compatible. This is an author audit, not an independent acceptance review.

## Continuing source/model qualifications

- **F10:** `__declspec(noinline)` on the archive search is a **[INFERENCE]**
  code-generation qualification preserving an outlined shared search, not a
  recovered original attribute spelling. It applies to all six functions in
  `src/checkpoint10/alternate1/func_001650f8.c`: `001650f8`, `001f1ebc`,
  `001f1ed8`, `001f1f20`, `001f1f34`, `001f1f4c`.
- Explicit v6/L1 selection is required:
  `armlink5-inline-branchnop-v6` and `anonymous-string-records-v1`.
  R1 removed-helper identities remain qualified **[INFERENCE]**; a byte match
  does not recover an erased historical callee. Helpers compile with their
  complete natural caller contexts and earn no separate credit. L1 literal
  placement earns no external data credit or recovered historical TU/pool extent.
- Scheduler callbacks retain `int(void)` and real result providers. C-only
  procedure bodies such as `0013db40`/`00107140`, and the `00190688` mutation
  procedure, retain mandatory discard-result obligations; no invented return,
  C++ fall-through body or incompatible callback conversion is introduced.
- Existing `ORIGINAL BUG:` annotations and preserved guard/runtime qualifications
  remain applicable. Native target-version resource prefixes and descriptive
  class/member names remain qualified at their sources. Matching output is not
  proof of an unknown complete object extent or unknown dynamic slot population.
- The FileSession static-table completion names the actual release/reset
  providers, including reset's real pointer result. Free-list reuse is a
  **named storage union** of the same object shared by C and C++, not an
  unrelated pointer-cast view. The rejected unnamed-union attempt incorrectly
  moved C's handle field to offset zero; named storage preserves the original
  `+4` field and all active/free callers migrate together.

## Remaining preparation blockers

1. c009-r5 must finalize without changing its frozen prepared evidence.
2. Accepted scene/allocator/Pane-Material/sound/legacy-retail-font callable-interface
   prerequisites must close or receive an explicit supported Main adjudication.
   NEW-only deferrals are not a waiver for inherited contracts.
3. Main must run and adjudicate **whole-ledger interfaces and argcheck**, then
   prepare from clean integrated committed inputs, with two fresh complete
   builds and the full suite, followed by two independent bound reviews.

Exact remaining argv and prerequisites are in
`scratch/integrator/c010/r5/REMAINING-HEAVY-COMMANDS-r2.json`. Any integrated
source/header/profile/name/catalogue/composition change requires a fresh affected
closure and unused evidence paths; historical failures and records stay immutable.
