# Proposed v1.18: composite v5 function-entry aliases

Status: **DRAFT, unadopted**. Main alone owns gate adoption. This proposal depends on the
separately corrected/re-reviewed v4 bounded-switch model (proposed v1.17), and on independent
review of this exact v5 composite. Do not use this branch for checkpoint credit before those
reviews and c008 finalization/closure. Frozen `docs/GATES.md` v1.16 is unchanged; no prepared,
accepted or historical artifact is rewritten. The model name, not merely a gate number,
versions behavior. If adoption numbering changes, preserve all normative requirements.

## Exact proposed revision entry

- **v1.18** (2026-10-17): only the new, explicitly selected immutable linker model
  `armlink5-inline-branchnop-v5` permits proven full-section zero-size ELF `STT_FUNC`
  entry aliases. It composes the separately reviewed v4 bounded-switch CFG and fail-closed
  endian-state checks with the alias-identity requirements below. Frozen `strict-v1`,
  `armlink5-inline-branchnop-v2`, `armlink5-inline-branchnop-v3`, and v4 retain their exact
  original semantics: no sibling-name binding inheritance and no zero-size R1 entry alias.
  Every same-COMDAT member copy's binding is resolved before removal; mixed original-bound
  and removable copies, unsupported retained copies, and contradictory removed copies are
  refused. Original-bound equality, all R3 roots/CFG/reference/dependency checks, unique
  emitted ownership, strict complete-function extent, and R1 provenance qualifications
  remain mandatory. The selector, both fresh receipts, prepare and finalized history bind
  v5, and a transition reopens every affected R1 caller under existing model-review rules.
  No checkpoint, helper, data or denominator credit is granted by this implementation or draft.

## Exact replacement for the conservative zero-size destructor-alias boundary

### Proven full-section function-entry aliases (v5 only)

1. Only explicit `--linker-model armlink5-inline-branchnop-v5` enables this representation.
   Its actual allocated candidate ELF section must be executable, non-NOBITS and completely
   represented by candidate bytes. Every non-internal function symbol has the identical
   raw entry value, exactly zero for ARM or one for Thumb; each recorded size is zero or
   the complete section size; at least one sibling records that complete size. A name's ABI
   spelling alone is not proof. Interior entries, mixed ISA values, absent/partial full-size
   witnesses, data symbols and untyped symbols cannot borrow a function's identity or extent.
   Objects, ELF symbols and candidate bytes are never patched or normalized.
2. Either proven function-entry alias may supply the original binding for its own section.
   Every supplied function binding must agree and name a catalogued function start. This
   inference is strictly section-local; no binding is guessed for another COMDAT copy.
   Before R1 selects any removal, resolve every supplied copy's original bindings, including
   aliases and other named symbols. A same-COMDAT member mixing an original-bound copy and
   an R1-removable copy is refused even if its bytes are identical. Any unsupported/ineligible
   counterpart also prevents partial member removal. Every removed copy must independently
   satisfy R1 and have identical complete bytes and ARM code mapping. Contradictory copies
   cannot evade consistency checks by being retained separately.
3. A bound identity never receives an R1 rewrite with `target: null`, directly or through
   another copy. Retained original-bound definitions keep every v2 original-body, size,
   eligibility and equality refusal. A wrong-sized original remains a refusal when deciding
   inlining, even if the caller alone is claimed for credit or a root retains the helper.
   No address is invented for an unbound helper. Strict full-section and full-catalogue
   extent comparison is unchanged; a matching short section is EXTENT, never MATCH/credit.
4. R3 retention is unchanged: roots through either alias, any same-COMDAT group copy/member,
   `__tagsym$$used`, exported/entry/KEEP/first/last selectors and uninterpreted metadata all
   prevent removal. Unmatched explicit selectors fail closed. Retention keeps the section
   but never waives original-bound equality or proves that a call must remain an ordinary BL.
5. V5 inherits v4's complete bounded-switch CFG proof and endian-state refusal without
   weakening them. All existing incoming-path setup/entry/result contracts, outgoing
   successor/liveness checks and unknown-control-flow refusals remain mandatory. Every
   reference in every supplied allocated section is still censused, including otherwise
   unplaced sections; pointers, tail calls, dependencies, interior/addended targets and
   PREL31/unwind references are not silently discarded. Only unreferenced unrooted helpers
   with actual eligible inlined calls may be removed; no general dead-section elimination.
6. G4 unique emitted ownership remains mandatory before credit: each credited stable
   identity has exactly one emitted defining unit, its ledger owner, including zero-size
   bound aliases and weak/COMDAT copies. Identical duplicate credited definitions fail;
   a hidden unbound counterpart cannot bypass this requirement through R1 because mixed
   bound/removable members fail before linking returns. Uncredited helpers earn no credit.
7. Match names a bound proven alias only under v5 and compares its entire section against
   the entire catalogued function. Receipts expose every removed copy/source/object and
   every actual rewrite, with null targets only for genuinely unbound removed identities.
   Helper-only units remain in the ledger/hash closure. All existing helper/COMDAT/header
   dependency reviews and model-transition reviews remain required, including unchanged
   credited callers and added/changed/removed headers.
8. R1 conditions 7-8 remain mandatory, not replaced by metadata, CFG success or exact bytes:
   manually establish all typed caller/helper contracts and control-flow paths; retain a
   precise visible `[INFERENCE]` source note and explicit QUALIFIED identity-provenance
   review row for every affected caller. No recovered original helper identity, historical
   source call graph, genuine armlink execution or independent source-policy waiver is claimed.

## Mandatory re-review and adoption evidence

- Re-derive the exact corrected composite source/registry and selected identifier. Confirm
  v5 has both `bounded_switches` and `function_entry_aliases`, while older models retain
  their frozen flags, decoding, alias rules and bound-name display. Check public match/build
  consumers, not only private proof helpers. Default v2 remains unchanged.
- Exercise contradictory copies and identical copies against a wrong 12-byte bound original,
  caller-only and helper-credit consumers, both object orders, retained roots through either
  alias/different copy, credited duplicate owners, and unsupported retained counterparts.
  Each refusal must precede a successful receipt or null-target escape for a bound identity.
- Preserve the compiler-object constructor/non-virtual/virtual destructor evidence and all
  six pair/binding positives under explicit v5. The old proposal's six whole-object links
  across strict/v2/v3 are historical withdrawn broadening evidence, not current behavior;
  those frozen models now keep their original refusals unless all names are explicitly bound.
- Exercise bounded switches and zero-size helper aliases together, including SETEND refusal,
  not merely each feature in isolation. Retain strict complete-extent and bound-name checks.
- Main runs the combined suite once after all changes land, followed by independent composite
  review; do not overlap full verification with other workers. No full builds/suite, acceptance,
  gate adoption, history or push operations are authorized by this author assignment.

## Retained real-case qualifications

L047 `func_002166cc` and w23f `func_001e722c`, `func_001e7db0`, `func_001ebf18` still have their
independently reported blockers. L047's previous v3 diagnostic refused its call context and had
1,248 emitted versus 1,240 catalogued bytes; w23f's diagnostics refused misaligned `.constdata`,
and allocated unwind references remain another removal obstacle. None was rerun merely to
confirm failure. No motivating real-image MATCH or new acceptance gain is claimed.

The sealed failing `scratch/review/d2-alias/REVIEW.md`, original d2-alias worktree and their
original source/evidence remain unchanged. Pre-versioning corrected safety-smoke evidence is
retained separately as superseded diagnostics, not v5 acceptance evidence. Final settled-code
smoke and exact composite revision are bound in the evidence block below.
