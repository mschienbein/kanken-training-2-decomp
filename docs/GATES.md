# Gate design — frozen v1.18 (2026-10-17)

Revisions:
- **v1.18** (2026-10-17): only the new, explicitly selected immutable linker model
  `armlink5-inline-branchnop-v6` permits proven full-section zero-size ELF `STT_FUNC` entry
  aliases to borrow a sibling's binding or satisfy R1 entry identity. Independently bound
  names retain the frozen placement and complete-extent checks without needing that borrowing
  proof. V6 composes the separately reviewed v4 bounded-switch CFG and fail-closed endian-state
  checks with the alias-identity and legacy-placement requirements below. Frozen strict-v1,
  v2 and v3 retain their exact original semantics; v4 retains its original alias policy.
  V5 is retired, never adopted: reviewed `22cb05b` refuses explicitly bound D1/D2 placements
  that v4 credits (`func_001c6a68`, `func_001ef89c`); it is superseded by v6, not edited in place.
  Every same-COMDAT member copy's binding is resolved before removal; mixed original-bound
  and removable copies, unsupported retained copies and contradictory removed copies refuse.
  Original-bound equality, all R3 roots/CFG/reference/dependency checks, unique emitted
  ownership, strict complete-function extent and R1 provenance qualifications remain mandatory.
  Selection, both fresh receipts, prepare and finalized history bind v6, and a transition
  reopens every affected R1 caller under existing model-review rules. No checkpoint, helper,
  data or denominator credit is granted by this draft or implementation.
- **v1.17** (2026-10-17): the new immutable model `armlink5-inline-branchnop-v4` may resolve
  R1's fully bounded unsigned ARM compiler jump-table forms described in condition 4.
  Frozen `armlink5-inline-branchnop-v3` retains its original CFG policy, caller decoding and
  return classification; this revision never broadens or reinterprets that identifier.
  V4 proves the same-index immediate bound, no guard bypass, complete owned table/default
  successors, and little-endian data interpretation on every path to an address-table load.
  Unknown, unbounded, escaping and other computed transfers remain refusals. The implementation
  accepts only explicit class/encoding effect models, refusing unknown writes/state restoration.
  Existing all-path incoming/outgoing ABI checks apply to the enumerated graph. Helper shape,
  rewriting, retention, bound-callee equality, full source/object closure and QUALIFIED
  provenance obligations are unchanged. V2 is unchanged. Both fresh receipts, prepare and
  history bind v4 explicitly; transitions reopen affected R1 callers. Cause: compiler switches
  in the 2,212-byte `func_001ab680` candidate, then independent F01/F02 and F03/F04 findings
  on the rejected same-name/endian- and effect-unproved predecessors. Prior forced-context
  and private MATCH evidence is not adoption or accepted credit. Adoption must follow c008-r3
  finalization, passing independent implementation/exact-text review, and the recorded revision.
- **v1.16** (2026-10-17): accepted credit follows stable function identities across
  explicitly recorded translation-unit moves. A candidate may declare
  `moves: [{function, from, to}]` and supplies each destination's complete credited
  function set and compilation profile. Prepare revises surviving old units,
  retires empty units, and recompiles the entire proposed ledger twice. Every
  previously accepted identity survives under exactly one owner. Every credited
  identity has exactly one emitted defining unit, its credited owner, including
  weak/COMDAT definitions and names-overlay aliases; byte-identical duplicate
  copies in other units are refused too. Uncredited COMDAT identities retain
  their existing duplicate/section-placement rules. Prepare and the hash-chained
  history seal checkpoint-bound move records and `removed_units`; historical
  ledger/prepare snapshots are not rewritten. Verify validates those snapshots
  and owner transitions without requiring historical source paths to remain
  unchanged or present after later accepted revisions. With an unchanged
  denominator, a pure move changes neither accepted function count nor accepted
  byte total. Candidates without moves remain addition-only. Cause: the native
  NW4C unit needed by `func_00185bb0` and later evidence-backed consolidation
  during readability work.
- **v1.15** (2026-10-17): G4 may use `armlink5-inline-branchnop-v3`
  (`docs/LINKER.md`, rule R1) for source-defined tiny callees that armlink can
  inline and subsequently remove. All v2 bound-callee equality/refusal checks
  remain mandatory, including for rooted/retained candidate definitions. R1
  excludes every retained/root/kept section, including `__tagsym$$used`.
  Removed helpers earn no catalogue credit; their defining sources, frozen
  profiles and objects remain in the prepared ledger/hash closure, including
  helper-only units with `functions: []`. Receipts expose every removed entity
  and per-call rewrite with `target: null`. G6 requires explicit QUALIFIED
  identity provenance, a visible `[INFERENCE]` source note and manual typed
  contract/control-flow review for each affected credited caller. A helper or
  shared-header revision reopens all dependent credited callers, even when
  their source and output bytes are unchanged. Prepare records added, changed
  and removed project headers as explicit G1 revisions, including in non-R1
  checkpoints; R1 dependency expansion is empty when no R1 caller is affected.
  Cause: ARM DUI0474K §4.12 performs inlining before unused-section elimination,
  while §4.3 separately requires retention of rooted/marked sections. R1 models
  a possible removed helper, not recovered historical call identity.
  Independent implementation review: `scratch/review/linker-v3-r3/REVIEW.md`,
  PASS_WITH_QUALIFICATIONS. R1 uses the v1.14 separately reviewed, frozen selector;
  both fresh builds, prepare and history bind the same explicit model. Model
  transitions reopen unchanged affected R1/L1 functions as described above.
- **v1.14** (2026-10-17): define L1, the independent, default-off
  `literal_model=anonymous-string-records-v1`, for qualified placement of complete anonymous
  compiler-marked candidate string sections. L1 proves every candidate record, its original
  referenced address, and every byte of one contiguous witnessed original span; it does not
  prove the original TU, original complete pool extent, sharing with uncompiled units, or
  armlink's ordering algorithm. External literal data earns no additional credit. Models and
  all pool mappings are visible in match/build records; prepare compares both model fields,
  records new/revised-unit mappings and all affected unchanged-source units on a model
  transition, and history retains the literal model. Independent implementation review:
  `scratch/review/literal-records/REVIEW.md`, PASS_WITH_QUALIFICATIONS. A separately reviewed
  checkpoint selector freezes both choices before prepare's gates/builds in `selection.json`,
  passes them explicitly and identically to both fresh builds, and binds the selection and
  complete receipt hashes in prepare/history. The reviewed L1 implementation originally had
  **no** prepare selector and always selected `none`; that review alone was not adoption.
  This selection change, its independent review and this gate revision must be frozen before
  any checkpoint can use L1. Private MATCH/build flags never authorize acceptance.
- **v1.13** (2026-10-17): G3 C++ class/struct/union identity is namespace-, nesting- and
  template-specialization-aware, replacing v1.7's flat-name assumption. Definitions are compared
  by fully qualified name, including enclosing classes and specialization arguments; primary
  templates and explicit/partial specializations are distinct. Unnamed namespaces are
  translation-unit-local, including definitions from included project headers. Different
  definitions of the same qualified class remain ODR failures, and a forward declaration of a
  header-defined qualified class still requires its defining header transitively. Aliases do not
  introduce class identities. C checks and the explicit unchecked-C++ waiver are unchanged. Cause:
  the licensed NW4C integration exposed false collisions between namespaced heaps, nested iterator
  types and `iterator_traits` specializations.
- **v1.12** (2026-10-17): G3 const-write check fix. Indexing through a pointer (`obj->ptr[i] = v`)
  writes the pointee, so the pointer's own constness (a pointer member of a const object) no
  longer makes it a const write; indexing an array member of a const object still is one. Cause: a
  false positive on c006's `func_00183bc8` (`const ArchiveReader *` whose `cache` array is
  writable). Regression tests in `tests/test_interfaces.py`.
- **v1.11** (2026-10-17): reviews add step 7 (`docs/REVIEWING.md`): the caller-contract audit
  `tools/argcheck.py` over the prepared ledger, every reported candidate adjudicated; the
  integrator runs the same audit before each prepare. Cause: c004-r2 and c005 both failed review on
  empty/wrapper/callback prototypes that dropped caller-evidenced arguments. Also: compiler profiles
  `sdk` and `middleware` (ARMCC 4.1 b791, `docs/COMPILER.md`) join `game`; a unit's ledger profile
  is the one its bytes match, and reviewers check that non-`game` units are plausibly SDK/middleware.
- **v1.10** (2026-10-17): G3 (`tools/interfaces.py`) implements the v1.9 source policy and two
  integration safeguards. (a) CP15 named-register variables (`register` storage, `__asm("cp15:...")`)
  are audited as ordinary declarations; `__asm` statements, blocks, assembly functions and
  core-register bindings (`__asm("r0")`) still fail. `__svc` declarations keep their ABI
  annotation. (b) The pinned compiler's standard C headers audit cleanly (`__nothrow`, format
  annotations, anonymous-union pragmas recognised; `__int64` normalised; `__softfp` kept); packing
  and unknown attributes/pragmas still fail closed. (c) A unit source containing an `INTEGRATOR:`
  marker comment (the authors' integration to-do marker) fails the audit. Shared declarations:
  `include/ctr/svc.h` (raw kernel ABI for every SVC that `__svc` can express; r4/r5 calls such as
  ControlMemory deliberately undeclared) and `include/ctr/cp15.h`. Cause: the ARMCC-extensions
  ruling, and c005's units that include <string.h> being marked unchecked.
- **v1.9** (2026-10-17): source policy follows the user ruling
  `docs/rulings/2026-10-17-armcc-extensions.md`: `__svc(N)` declarations, CP15 named-register
  variables and ARMCC intrinsics count as C (declared once in a shared header; each use a
  qualification on the function's review row); `__asm` statements/blocks and embedded-assembler
  functions stay refused. Reviewers apply it under G6 (`docs/REVIEWING.md` step 2). The G3
  interface parser does not yet accept named-register declarations (`__asm` raises an interface
  error), so no unit using them can pass prepare until a later revision adds that support.
  Also recorded: every subagent, both reviewers included, runs one model family
  (`docs/rulings/2026-10-17-reviewer-model-family.md`); the cross-family clause below remains
  "where possible".
- **v1.8** (2026-10-16): the linker model is part of acceptance. Builds use the armlink emulation
  `armlink5-inline-branchnop-v2` (`docs/LINKER.md`; independent review PASS_WITH_QUALIFICATIONS in
  `scratch/review/armlink-inline/REVIEW-v2.md`); `prepare` records `linker_model` and every
  emulated rewrite inside new or revised units (`linker_rewrites`), G4 requires both builds to use
  the same model, the history entry records the model, and reviewers check each rewrite's callee
  choice (`docs/REVIEWING.md` step 6).
- **v1.7** (2026-10-16): class/struct/union names are one project-wide namespace for C++ units.
  G3 compares every definition in a project header with same-named definitions in C++ units (ODR),
  and refuses a C++ forward declaration of a header-defined name in a unit that does not include
  that header (transitively). Cause: `c003-wave2-r2` review F01 — `include/game/screen.h`
  completed `struct Widget` as a 0x94-byte display object while accepted `game_heap.cpp` forward-
  declared a different 0x48-byte `Widget`; the v1.6 check read only `.cpp` text.
- **v1.6** (2026-10-16): (a) finalize is revision-aware — the drift check exempts exactly the
  prepare-declared `revised_units`/`revised_headers` (already pinned to their prepared hashes) and
  still refuses any other change to accepted files; before this, v1.4 revisions could never be
  finalized (`c003-wave2` review F02). (b) G3 refuses a local `extern "C"` declaration in a C++
  unit of an address-named symbol that a C unit or header also declares, or that two C++ units
  declare: shared symbols come from `include/` headers, because C++ declarations are not
  type-checked (`c003-wave2` F01, a C++ caller still using renamed types).
- **v1.5** (2026-10-16): a review binds exactly the prepared files — the current hashes of the
  sources and headers named in `prepare.json` — and lists headers added since prepare
  (`headers_added_since_prepare`) without being unbound by them. Finalize still requires the whole
  header tree to equal the prepared one. Cause: `c002-wave1-r2` — the integrator created two
  unrelated wave-2 headers while the second review ran, so its record hashed a different tree and
  finalize refused it (correctly under v1.4). r2 stays unfinalized; its successor gets fresh
  reviews. Integrator rule: no new files under `include/` while a checkpoint is under review.
- **v1.4** (2026-10-16): revised accepted material is reviewed again. `prepare` records
  `revised_units` (accepted units whose source differs from the last accepted entry) and
  `revised_headers`; G6 requires a verdict row for every function in a revised unit, as for new
  units. Cause: unifying wave 2 needed a correction to an accepted c001 prototype
  (`func_001795bc` takes no argument); without this, such an edit would be rebuilt but unreviewed.
- **v1.3** (2026-10-16): headers bound like sources. `prepare`, each review and the history entry
  record the SHA-256 of every project header (all files under `include/`, plus `.h/.hh/.hpp/.inc`
  under `src/`); G7 refuses a review bound to other header hashes or a header changed since
  prepare; G8 refuses an accepted header that changed or disappeared (new headers are allowed).
  Cause: `c002-wave1` review F02 — the shared `include/game/bss_0049ecf0.h` was unbound, so a
  header edit after review could have changed credited objects unnoticed.
- **v1.2** (2026-10-16): G0 clean tree. `prepare` refuses a working tree with any uncommitted or
  untracked non-ignored file and records the HEAD `commit`; reviews and finalize bind to that
  prepare. Cause: `c001-pilot-r3`'s suite ran against another agent's in-progress
  `tools/analyze.py` edit in the shared checkout and failed. Agents that edit tracked tools work
  in their own git worktree from now on. Test `tests/test_accept.py::test_prepare_refuses_a_dirty_tree`.
- **v1.1** (2026-10-16): `prepare.json` again records the per-gate `checks` map (v1 tooling dropped
  it through an editing error; gate logic and `result` were unaffected). Regression test
  `tests/test_accept.py::test_prepare_records_every_gate_result`. The defective record
  `accept/c001-pilot/` is kept unfinalized (see its DEFECT.md); the candidates were re-prepared as
  `c001-pilot-r2`.

Changing anything below needs a recorded revision here (new version, date, reason) **before** the
next checkpoint is prepared. A batch never runs under a gate design that changed mid-batch.

## Adoption sequencing and accepted text sources

The revision entries and normative blocks below quote the final accepted text from:

- v1.14 L1 and its selector, v1.15 R1 and the same selector, and the mandatory adoption/reviewer
  checklist: `scratch/review/adopt-r1-l1-r2/REVIEW.md`, “Exact GATES text accepted”
  (PASS_WITH_QUALIFICATIONS, `84f0b3a11da06e8ab17504b057ce95027b59b9d2`).
- L1 conditions and witnessed-span qualification: `scratch/review/literal-records/REVIEW.md`,
  §6 (PASS_WITH_QUALIFICATIONS); the later selector review supplies the adopted selection text.
- R1 conditions 1–8, including mandatory F03 reviewer checks:
  `scratch/review/linker-v3-r3/REVIEW.md`, “Exact GATES text accepted”
  (PASS_WITH_QUALIFICATIONS, `d51d84e8badb1d5db90d7047f578f0add4959faf`).
- v1.16 rehoming entry, G2/G4/G6/G7/G8 rows and move-review obligations:
  `scratch/review/ledger-rehome-r2/REVIEW.md`, “Exact acceptable GATES text”
  (PASS_WITH_QUALIFICATIONS, `806b8575149b54469711b122fd491f224429e7bb`). Only that
  review's explicitly permitted revision-number change, v1.14 to v1.16, is made.
- v1.17 bounded-switch entry, R1 introduction and conditions 4 and 7:
  `scratch/review/r1-jumptables-r3/REVIEW.md`, “Exact acceptable GATES text”
  (PASS_WITH_QUALIFICATIONS, reviewed tip `5187259c427b8fc854642520ded03b7743320be6`);
  `SEAL.json` binds approved normative text SHA-256
  `7af3169709c57f170caf55c857d7eb9661bba67d5d87c7b7bf961d5faeb67066`.
  The sole sequencing update, c008-r2 to c008-r3, is explicitly approved by
  `scratch/review/models-v6/GATES-ACCEPTED.md`, Stage A.
- v1.18 v6 entry, adoption sequencing, model authorization, alias/legacy-placement boundary
  and remaining obligations: `scratch/review/models-v6/GATES-ACCEPTED.md`, Stage B
  (PASS_WITH_QUALIFICATIONS, reviewed tip `5bb20a9a3251e83cddc37d6b64a166679ebd8e51`);
  `scratch/review/models-v6/SEAL.json` binds accepted-text SHA-256
  `8527da3980abb976df18dfcaa9934e742c4e339916a2e3af083c3279573a5094`.

The selector re-review requires the following integration order verbatim:

> **Required integration order:** finalize `c007` with pre-selector tooling as legacy sequence **7**, then merge this selector before preparing sequence **8** (`c008`). The fixed creation boundary is part of the safety argument, not an optional naming convention. This review does not authorize merging before that legacy finalization, retroactively selecting a checkpoint, or granting new function/data credit. The substantive R1 and L1 review qualifications remain mandatory.

Sequence 8 and every later checkpoint require the frozen selection binding regardless of
schema/model labels. Gate activation must precede the first relevant prepare and never occur
during a checkpoint's review. Historical prepared/finalized evidence is not rewritten.

### V4/v6 adoption sequencing

**Required integration order:** finalize and close `c008-r3` under its existing frozen
evidence and GATES v1.16; adopt the exact v4 edits below first; then adopt the distinct
v6 revision and boundary below before the first new prepare selecting v6. V5 is never
authorized. Do not edit an in-flight checkpoint or historical evidence. This review
performs no adoption and grants no source/helper/data/denominator credit.

V6 activation is permitted only after the `c008-r3` checkpoint has finalized and its
frozen evidence is closed, after the independently approved v4 implementation and exact gate
text have been adopted, and after this v6 implementation/exact-text proposal receives focused
independent re-review. Adopt v6 under a distinct recorded revision before the first new prepare
selecting it. Do not change gates during a checkpoint, rewrite historical selection/prepare/
receipt/history evidence, merge a stale feature parent, or reinterpret strict-v1/v2/v3/v4 or
retired v5 under another name. A private MATCH and passing checks grant no checkpoint,
function, helper, data or denominator credit.

## Author loop (no gate, private)

`tools/packet.py` → C/C++ → `tools/match.py --profile game` until `MATCH`. Per-function oracle:
compile with the frozen profile, strict in-place link (`tools/inplace.py`), byte compare over the
catalogued extent. Fails closed on: unresolved or unbound symbols, aliased or misaligned sections,
contradictory COMDAT copies, unsupported relocations, veneer needs, multi-function sections.

## L1 — qualified anonymous string-record placement

`--literal-model` is independent of `--linker-model`; its default is `none`. The immutable rule
`anonymous-string-records-v1` may permute a complete candidate `.conststring` or `.constwstring`
section only when all of the following hold:

1. The actual allocated ELF section is readonly, non-executable, non-BSS, relocation-free and
   non-COMDAT, with both SHF_MERGE and SHF_STRINGS. Character entity size is respectively 1 or 2;
   alignment is that entity size or 4. Every definition in the section is a local STT_SECTION
   symbol at offset zero. A familiar name without these flags, named object or exported alias
   is not eligible; ordinary `.constdata` and unmarked `.conststrlit` are not eligible.
2. The complete candidate section is positive-sized and at most 1 MiB, and parses into at most
   4,096 unique, nonempty, completely terminated records. Only zero alignment padding may
   separate records. Duplicate/empty/unterminated records and trailing padding fail closed.
3. Every candidate record has a placed incoming ABS32/TARGET1 relocation against its local
   section symbol with an addend exactly at the whole-record start. Every non-NONE incoming
   reference must satisfy that rule. Unplaced referring sections, unsupported relocations and
   interior/suffix references fail. Repeated references to a record must agree on one original
   address; every candidate record must have a witness.
4. Original record addresses come only from original values at the placed relocation positions.
   No image search, nearest-content lookup, names override, supplied address map or choice
   among equal-content hits is permitted. The addresses must form a bijection over one aligned,
   contiguous span of exactly the candidate section size, outside catalogued functions and BSS.
   Ordinary section-overlap checks apply within and across candidate units without exemption.
5. Candidate records plus their zero padding, reordered to those addresses, must reproduce every
   byte of that witnessed span exactly. No candidate record may be omitted; no original payload
   may be copied into output. Altered contents/padding, gaps, tail sharing, distinct-record
   aliasing or a changed extent fail. L1 proves completeness of the candidate section and this
   witnessed span, NOT completeness or ownership of the original compiler/linker pool: additional
   original records outside the span and sharing with uncompiled units are not established.
6. The complete catalogue extent of every credited function, including relocated pointers and
   all nonrelocated bytes, must still match exactly. L1 introduces no instruction rewriting,
   masking, scheduling change, relaxed function extent or denominator change. A separately
   selected branch model retains only its own already-reviewed rules and qualifications.

Each match/build records independent `linker_model` and `literal_model` fields. Every eligible
pool, including an identity permutation, has a `literal_rebindings` entry naming L1, object/TU
ownership, section/index, base/size, record/reference counts and every candidate/output record
offset and size. Receipts must not contain literal contents. Review the actual compiled metadata,
witness coverage, full-span equality and every record mapping. No literal data is added to
function credit; original pool extent/order/ownership and ordinary standalone all-source linking
remain qualifications. Natural source and interface review remain mandatory: no unused pool
seeding, artificial order-only declarations, section attributes, emitted byte blobs, inline
assembly or address-magic aliases are authorized by L1.

Acceptance remains default-off. Before a checkpoint can select L1, the separately reviewed,
recorded checkpoint-selection change below must be frozen and this gate revision adopted.
Freeze the choice before prepare's gates/builds, pass the identical explicit choice to both
fresh builds, bind it in prepare/history, and preserve all mappings in the full receipts.
Review every affected unit's use, including unchanged-source units when a model selection changes;
prepare's new/revised-unit summary alone is not an exhaustive use list. A private MATCH or build
flag is never acceptance adoption. Existing G3, G4, G5, G6 and G7 obligations remain unchanged.

### Checkpoint selection — normative addition with v1.14

1. `accept.py prepare --checkpoint NAME --candidates PATH --linker-model ID --literal-model ID`
   selects independent checkpoint-global models. Defaults are explicitly materialized as
   `armlink5-inline-branchnop-v2` / `none`, not inherited from the previous checkpoint.
   Unknown/retired identifiers are refused before creating the checkpoint. CLI/API arguments,
   not optional candidate-manifest overrides, are authoritative. This matches existing
   author/build identifiers and makes the opt-in visible at invocation without reinterpreting
   existing manifests. A model requiring an unadopted gate revision is not authorized merely
   because the selector accepts its identifier.
2. Before any prepare gate/build, exclusive `accept/NAME/selection.json` freezes both choices.
   Both calls to `build(..., fresh=True)` receive both exact identifiers explicitly. Failed
   preparations keep their chosen models and name; no retry or later mutation can opt in.
   Prepare compares both model fields across builds **and against the frozen choice**, so two
   identically wrong/substituted receipts still fail even when their output bytes agree.
3. Schema-2 `prepare.json` binds the selection hash, both choices, both complete receipt hashes,
   each build's model inputs, and the ordinary source/header/profile/catalogue/ledger closure.
   Full receipts preserve every mapping, identity permutation, rewrite and removed entity,
   including helper-only units' source/object/profile attribution. No payload is emitted.
4. `model_uses` exposes every current R1 caller, removed-helper unit, L1 owning unit and full
   mapping, not just new/revised sources. `model_changes` compares both models to the previous
   finalized choices (historical absent linker/literal fields mean the old v2 / `none`
   baseline for transition selection only). On either model transition, `model_reviews`
   reopens every current R1 caller and conservatively every credited function in each current
   L1 pool-owning unit, even with unchanged source/output hashes. This is a conservative review
   set, not proof of a recovered per-function string-use graph. R1 helper/COMDAT/header revision
   expansion remains independently mandatory even when the model selection stays unchanged.
5. Independent review covers all new/revised/dependency/model-transition functions. Review and
   finalize recheck selection and full receipt hashes; finalize rejects any missing, changed,
   nonfresh, failed, differently selected or output-divergent receipt. History binds both
   choices and selection hash through the prepare hash. `verify` rechecks that closure without
   private ROM inputs. Existing schema-1 checkpoints retain their frozen verification rules;
   no historical artifact is rewritten or retroactively selected.
6. The selector changes no source-policy, typed-interface, catalogue extent, denominator,
   result-qualification or independent-review obligation. Merely implementing this selector,
   obtaining a private MATCH or passing the suite is not adoption and grants no new credit.

### G4 selection/L1 requirements retained from v1.14

The following accepted v1.14 G4 row remains mandatory in addition to the v1.16 rehoming row
in the checkpoint table; the later row does not weaken selection, receipts or L1 obligations.
Verbatim from `scratch/review/adopt-r1-l1-r2/REVIEW.md`, “Replace G4's table row”:

```markdown
| G4 | Two fresh builds | `prepare` | either `tools/build.py` build (fresh: no object cache) fails, outputs differ, either independent model field (`linker_model`, `literal_model`) differs between the builds, or either differs from the frozen explicit selection. Each build: every credited function an ordinary catalogued function, exact size, equal to the original on its own before being written; every selected L1 pool satisfies L1 and is recorded; no overlapping sections across units; no duplicate global definitions; `code.bin` equal to the original; recompressed `.code` equal to the stored ExeFS `.code`. External data earns no additional credit. Selection and complete receipts remain bound through finalize/history verification |
```

### R1 — candidate-only inlined and removed callee

R1 is permitted only for a new checkpoint prepared after the applicable gate
revision and a passing independent implementation review, under the explicitly
recorded model `armlink5-inline-branchnop-v3` or, after adoption of the bounded-switch
revision below, `armlink5-inline-branchnop-v4`. Both fresh G4 builds, prepare and
finalized history must bind the same explicitly selected model. Existing
prepared/finalized checkpoints do not change gates or model. Author-loop opt-in
matches alone do not authorize acceptance; the acceptance build path must actually
select and record the chosen model.

The bounded-switch CFG proof policy is selected only by
`armlink5-inline-branchnop-v4`. Frozen `armlink5-inline-branchnop-v3` records
retain their original meaning; this revision does not broaden or reinterpret
that identifier. Selection, both fresh receipts, prepare and finalized history
bind the new identifier, and transition review reopens all affected R1 callers
as required by the existing selector. Strict/v2/v3 behavior is not changed.

R1's model-selection introduction and the v4-only bounded-switch authorization also admit the
distinct `armlink5-inline-branchnop-v6` identifier only after the v6 revision is independently
approved and frozen. V6 must satisfy all corrected v4 bounded-CFG, modeled-effect and owner-
state proof conditions, plus the entry-alias/legacy-placement conditions below. This adds a
separately selected composite; it does not change strict/v2/v3/v4 or resurrect retired v5.
The default remains v2. Selection, both fresh receipts, prepare and finalized history must
name v6 explicitly; the existing transition review reopens all affected R1 callers. No old
or in-flight checkpoint is retroactively selected or reinterpreted.

A credited caller may use R1 only when all the following conditions hold:

1. The complete callee section is compiled from supplied source under a frozen
   profile in this candidate build, in the caller object or another supplied
   object. Its defining unit remains in the ledger and every receipt with
   source/object hashes even when `functions: []`. An external declaration,
   guessed library body, ROM bytes or fabricated original address is insufficient.
2. No function, alias or other binding gives the section an original address.
   Reserved address names remain reserved. Every binding overlapping a
   catalogue function, entry or extent takes the retained-callee path, retaining
   all v2 equality/refusal checks. Retention metadata never waives those checks
   or by itself proves calls should remain ordinary BLs. An uncatalogued bound
   address is refused. An R1 section is not entry/root/used/retain/exported,
   explicitly kept, first/last or otherwise required to survive removal.
   Unknown supplied retention metadata fails closed; explicit root selectors
   must resolve. Retention propagates to every copy/member of the COMDAT entity.
   Unexamined linker scripts/options are not evidence of removability.
3. The whole section is executable, word-aligned, wholly ARM code, non-NOBITS,
   exactly 4 or 8 bytes, with every function entry at offset zero spanning the
   complete section. Only nonreferencing `R_ARM_V4BX` markers are allowed. Its
   own compiled body satisfies unchanged I1–I3, including all PC/LR/control-
   transfer/exception/branch-NOP exclusions. The non-return instruction reads
   only r0–r3/flags and writes only r0–r3/r12/flags; SP and callee-saved registers
   are preserved. Aliases and identical COMDAT copies are one entity;
   contradictory copies are refused.
4. Every use is an actual compiler-produced relocated ARM BL/BLcond exactly to
   that entry, at catalogued original ARM code, satisfying unchanged I1–I3
   encodings/conditions. The original-code call-ABI CFG filter must pass. Each
   input is defined within the preceding four straight-line words on every
   incoming path, is an unmodified actual incoming caller argument, or is an
   unmodified actual r0/r1 result of a preceding unconditional call on every
   path. Conditional definitions/calls alone do not establish input provenance.
   Entry forwarding does not waive the outgoing check. All decoded successors,
   including both conditional arms and every resolved branch/table target, are
   checked for reads of r1–r3/r12/flags before unconditional definitions up to
   calls/returns or decoded code-range ends. r0 may be a result and I1 requires
   no inputs. These checks do not prove the argument/result types,
   interprocedural liveness, or that the original source contained a call.

   Only explicitly selected v4 may resolve the following bounded ARM switch
   transfers as otherwise indirect table edges. Let A be the original instruction
   address and rM a non-PC index register. An unconditional immediate CMP rM, #N must guard
   ADDLS pc, pc, rM, LSL #2 (branch table) or
   LDRLS pc, [pc, rM, LSL #2] (absolute-address table).
   The unsigned LO forms are also allowed: LS enumerates exactly indices
   0..N, LO exactly 0..N-1, with at least one entry. The immediate uses ARM's
   decoded rotate encoding. Neither table contents nor apparent table length
   supplies a missing bound. Other predicates, signed comparisons, register
   bounds, scales/bases, writeback, subtractive indexing and unguarded or
   otherwise computed PC writes are not resolved.

   All words between compare and transfer must be contiguous modeled ordinary
   instructions that cannot change the index or NZCV. Conditional writes are
   possible writes. Only explicit instruction-class and validated A32-encoding
   allowlists may establish these effects. Decode architectural core destinations,
   paired/multiple-load destinations, base writeback and NZCV directly; missing
   decoder writes, update_flags or control-flow groups are never safety evidence.
   An unmodeled or invalid subencoding refuses the window. No call or other
   control transfer intervenes. No catalogue entry, direct B/BL/BLX target or
   otherwise-resolvable table target may enter after the compare through the
   transfer. Re-entry at the compare repeats the bound and is allowed.

   Every slot is at A + 8 + 4*i. A branch-table slot is an unconditional
   direct ARM B whose destination is decoded caller code; the CFG visits
   both the entry B and that destination. An address-table slot is a complete
   aligned absolute-address word in this caller's catalogued pool/literal
   data, and its destination is decoded caller code. Instructions and pool
   words are limited to their declared ranges clipped to the owning
   function's complete extent. Pool/data bytes cannot supply instructions,
   and code bytes cannot supply address-table words. Image search, neighboring
   owners, candidate relocations and arbitrary memory reads are not witnesses.
   Thumb, misaligned, data, undecoded and out-of-extent table destinations fail.

   For an LDR address table, little-endian data interpretation at the load must
   be established on every incoming path. A reachable SETEND or other unresolved
   change of data-endianness state is a refusal unless that state is independently
   proved little-endian at the load. A change before the comparison is subject to
   the same requirement; preserving the index and NZCV flags alone is insufficient.
   The target uses the little-endian public ARM ABI at catalogue entries and
   conforming call returns. Before either table form resolves, every decoded
   owner instruction, even unreachable, must pass the explicit effect allowlist:
   ordinary integer data-processing/immediate-shift, validated scalar/double/
   register-list memory with every destination/writeback, ordinary multiply,
   scalar VFP data/memory and explicit VMOV/FPSCR-VMRS/FMSTAT transfers, compiler NOP,
   or recognized branch/call/ordinary-return encodings. Each admitted subencoding
   must have modeled core/NZCV and processor-state effects; class membership
   alone is insufficient. Unsupported or invalid subencodings fail closed.
   Ordinary VFP writes to FP registers/FPSCR do not set core NZCV; FPSCR VMRS
   to APSR (FMSTAT) does. Calls retain the manual LE ABI-return contract.

   Generic coprocessor/exclusive instructions, including MRRC and LDREXD with
   any predicate, are not admitted by an empty write list. All RFE variants and
   other exception/status restores are unmodeled, unknown control flow and a
   refusal of the owner-state proof, even when the decoder omits PC/CPSR writes
   or branch groups. SETEND/MSR/MCR/CPS are likewise outside the allowlist.
   This rule includes instructions before the compare, on one predecessor or
   unreachable, and apparently LE-restoring instructions. Conservative false
   negatives are intentional; no interprocedural processor-state recovery is claimed.

   The not-taken successor at A + 4 is an unconditional direct branch to
   decoded caller code or an ordinary recognized return; it cannot fall into
   the table or leave the caller through an unexamined default branch. Ordinary
   recognized returns are BX lr, plain MOV pc, lr, and
   LDMIA sp!, {..., pc}/POP. Merely reading LR while writing PC, generic decoder
   return groups, exception-return encodings and arbitrary PC loads do not
   establish returns. Predicated recognized returns elsewhere retain their
   conditional fall-through paths. The CFG includes the default and every
   bounded slot; repeated address targets may share a successor but no
   distinct path may be omitted. Every reachable unknown indirect transfer,
   including any rejected table pattern, still fails closed. No function-name,
   address-specific, forced-consistent or other input-specific exception is
   permitted. This is bounded control-flow recovery, not evidence of a
   historical helper identity or an execution of armlink.
5. At least one call to each removed entity is inlined. No reference survives in
   any supplied allocated section, including otherwise unplaced sections. A
   pointer, tail B, dependency, interior/addended target, incompatible condition
   or invalid call context causes refusal. Only the now-unreferenced unrooted
   helper section is removed; there is no general dead-section elimination or
   credited zero-size helper.
6. Each rewrite records `removed_callee` identity, section, size and defining
   source/object with `target: null`. Match/build receipts list removed entities
   by defining unit. Prepared evidence exposes every relevant caller rewrite and
   binds the full helper definition closure. New/revised helper units, any
   identical COMDAT copy, and added/changed/removed shared-header contracts reopen
   all affected credited callers. Prepare's `dependency_reviews` names each
   caller/source and changed dependency; unchanged callers need explicit rows
   like new/revised functions. Review and finalize refuse missing rows or plain
   PASS provenance for R1 callers. Zero credit does not mean zero review.
7. Mandatory reviewer checks cover every incoming path and entry contract,
   setup dominance, outgoing successors and exits, implicit arguments at later
   calls/tail exits, predicate/flag dependencies, helper definitions and every
   declaration/shared-header use, argument types/widths/meanings, mutability and
   const contracts, memory effects, return/result semantics, and preserved
   original bugs. For preceding-call results, reviewers establish the actual
   return type and register width; BL alone does not prove r0/r1 are results.
   For every resolved switch, independently re-derive the same-index immediate
   comparison, decoded unsigned bound and exact entry count, preservation of
   index/flags, absence of alternate entries after the guard, every table slot
   and destination, the default edge and every subsequent path using only the
   original owner's catalogue code/pool ranges. Establish little-endian data
   interpretation on every incoming path to each LDR table, including changes
   before the guard or on only one predecessor, and the entry/call ABI state
   contract; index/NZCV stability alone is insufficient. Check architectural
   register-pair/list and base-writeback effects without trusting missing decoder
   writes, and explicitly refuse exception-return PC/CPSR restoration. Inspect
   incoming setup and outgoing liveness across all cases; no helper-free case,
   repeated target, loop, predication or other table may hide a bypass or read.
   Exercise public consumers for LDREXD/predicated LDREXD/MRRC, modeled pair/
   multiple loads and writeback into the index/base, and every ARM RFE addressing
   form with SP/r4 frames in the window, before the guard, on one predecessor
   and unreachable. Exercise the decoder-ID meta-test: every emitted class is
   explicitly allowlisted with validated effects or refused, including with absent
   access metadata. Retain missing/changed guard, index/flag, entry-bypass,
   endian/state, truncation, escaping-target/default and other computed-flow cases.
   Require frozen v3 refusal and selected v4 positives, all five accepted-function
   regressions under v2/v3/v4, the full suite, CLI/receipt/model-payload bindings
   and unchanged-R1 transition review. No mechanical result waives typed review.
   Argcheck must not invent an original callee edge for target: null; missing
   original-target evidence is a manual-review obligation, not an ABI-audit pass.
8. Every affected caller has a precise `[INFERENCE]` note at the use/helper
   declaration and an explicit QUALIFIED identity-provenance review row. Exact
   bytes establish the caller extent, not that the original source called this
   helper or that its identity/call graph is recovered. A matching original
   store/arithmetic instruction may have arisen without any helper call. Any
   helper selected to influence code generation still needs a defensible typed
   source-level contract and ordinary source-policy compliance; hand-emitted
   calls/instruction encodings and instruction-injection exceptions remain
   forbidden. Census hits, a clean CFG filter and an issue-free argcheck never
   waive these reviewer checks or qualifications.

## Checkpoint gates (`tools/accept.py`)

| # | Gate | Where | Fails when |
|---|---|---|---|
| G0 | Clean tree | `prepare` | any uncommitted or untracked non-ignored file; the HEAD commit is recorded |
| G1 | Denominator | `prepare` | `config/catalogue.json` is not the recorded denominator (`tools/denominator.py verify`) |
| G2 | Ledger composition | `prepare` | a candidate unit is already accepted without participating in an explicit move; a move does not name an accepted identity, its exact current `from` owner, and a different `to` owner whose complete unit lists it in candidates; repeated unit paths, duplicate function claims, duplicate/conflicting move rows, undeclared owner changes, or loss of any accepted identity. Subtract moved identities from old units and retire empty units; retired old sources must already be deleted. Existing destinations retain every previous identity except those explicitly moved elsewhere. Candidates without moves remain addition-only. |
| G3 | Interfaces | `prepare` → `accept/<cp>/interfaces.json` | any declaration/definition or struct-layout disagreement, const-contract violation or policy error across the **whole ledger**; C++ units unchecked unless `--allow-unchecked-cpp` (recorded; reviewers then check those units' interfaces by hand) |
| G4 | Two fresh builds | `prepare` | either fresh build of the complete proposed ledger fails, outputs differ, or linker/literal models differ. Recompile every new, moved and retained credited function, including old residual units, under its proposed unit's profile. Each credited function must be an ordinary catalogued function, exact size, and byte-equal to the original on its own before writing it; `code.bin` and recompressed ExeFS `.code` must equal their originals. Every credited identity must have exactly one emitted defining unit, its credited owner, including weak/COMDAT definitions and names-overlay aliases; identical copies in different units also fail. Inspect individual objects before combination/linking: removing an old credit claim never excuses a remaining old definition. Preserve the existing duplicate-global and section-placement checks for the rest of the ledger; uncredited COMDAT identities retain their existing rules. |
| G5 | Full suite | `prepare` | any module fails (`tools/test_suite.py`, sharded by module) |
| G6 | Review | `review` | the record fails the schema or lacks a destination-bound verdict row for any moved function, or a row for any other function in every new/revised unit. An accepted unit whose ledger entry changes is revised even when its source hash does not. Cover retained functions in old residual units and existing destinations. Retired units are listed in `removed_units`; moved functions are reviewed at their destinations, not twice. Keep unique finding ids, severities, methods and verdict consistency: a blocker or FAIL row requires FAIL; a QUALIFIED row or major finding forbids PASS; PASS_WITH_QUALIFICATIONS requires stated qualifications. |
| G7 | Finalize once | `finalize` | prepare is not PASS; fewer than `--min-reviews` bound passing reviews, any FAIL review, or any review bound to different prepared source/header hashes; prepared sources, headers, profiles, catalogue, proposed ledger or canonical base ledger changed; history is broken; prepared moves/removed units do not exactly describe the transition from the unchanged canonical base to the proposed ledger; or a retired source reappeared. Exempt only prepare-declared revised/retired sources and revised headers from prior accepted-file binding; bind every live proposed source/header to its prepared hash and leave all unrelated accepted files bound. Seal checkpoint-bound move records and removed units in the new history entry. |
| G8 | Ledger binding | `verify` (CI) | the history hash chain, a sealed historical ledger/prepare snapshot, or a ledger transition is invalid; any accepted identity disappears or changes owner without the same checkpoint-bound move in sealed prepare and history; removed-unit records disagree with the snapshots; canonical matches differ from the last finalized ledger; a live credited source differs from its accepted hash; an accepted header changed or disappeared; or credit exists without a finalized checkpoint. Validate historical snapshots and transitions, but compare current source hashes only for the latest ledger's live units: later reviewed revisions/moves may change or delete older source paths. |

For a checkpoint with moves, reviewers independently establish each old/new owner
and the complete destination unit/profile. Re-derive moved and retained byte
matches from both fresh whole-ledger receipts. Inspect individual emitted objects,
including weak/COMDAT symbols and names-overlay aliases, to confirm each credited
identity is defined only by its credited unit; no moved identity may remain
emitted by an old residual or unrelated unit, even with identical bytes. Confirm
retired old sources are deleted. Check linker rewrites involving moved identities,
including callers in unchanged units. Review rows cover exactly the functions in
`new_units` and `revised_units` at their proposed locations. Prepare and history
move records must agree and name this checkpoint. Historical checkpoint files
stay untouched. A pure move under the same denominator preserves the credited
identity set and byte total; only newly matched identities add credit.

The rehoming rows and move-review obligations are additional to the L1/R1 and frozen-selection
requirements above. Their new/revised-unit review locations do not exclude unchanged functions
required by `dependency_reviews` or `model_reviews`; those rows remain mandatory too.

Reviewer independence: the reviewer is never the integrator and never the author; at least one
review per checkpoint is from a model family other than the author's, where possible.

Records: `accept/<cp>/` (exclusive-create JSON; FAIL kept), `docs/progress-history.jsonl`
(hash-chained), `docs/denominator-history.jsonl` (hash-chained).

## Mandatory adoption/reviewer checklist

- Confirm the independently reviewed implementation commits and separately reviewed selector
  match the committed harness being prepared; record v1.14/v1.15 before the first relevant
  prepare. Do not change `docs/GATES.md` or historical artifacts as part of this draft.
- Inspect `selection.json`, `prepare.json`, both full receipts and finalized history: both
  explicit choices, fresh compilation, output equality, selection/receipt/prepare hashes,
  full mapping/rewrite/helper closure and source/object/profile attribution must agree.
  Two matching receipts are insufficient if they select a model other than the frozen choice.
- Review `model_uses`, `model_changes`, `model_reviews` and `dependency_reviews`, including
  unchanged-source units, helper-only units, identical COMDAT copies and shared-header changes.
  All required functions need independent rows. Summary selection is not an excuse to omit
  inspection of every mapping in both complete receipts.
- For L1, independently check all six conditions, actual ELF metadata, complete candidate
  parsing and witness coverage, bijection, every mapping and full witnessed-span equality.
  Explicitly retain unknown original pool extent, ownership, surrounding records, sharing,
  ordering algorithm and ordinary all-source linking as qualifications. No data credit.
- For R1, manually discharge items 7–8 on every affected caller. F03 is **mandatory**, not
  closed by the CFG filter, exact bytes or argcheck. Establish incoming and outgoing typed
  contracts, preceding-call result widths, all declarations and all control-flow paths.
  Require visible `[INFERENCE]` notes and QUALIFIED identity provenance. No recovered
  original helper identity/call graph is claimed.
- Preserve every bound-original equality/refusal and retention safeguard, including compiler
  `__tagsym$$used`, exported roots, KEEP selectors, data references and COMDAT propagation.
  The R3 review closes F01/F02/F08 only for its reviewed implementation; do not weaken it.
- Existing G3–G7, natural source/interface review, original bugs, and complete function extent
  equality remain mandatory. A model selector, private MATCH, suite pass or gate draft grants
  no credit by itself. No real armlink execution or standalone original link is claimed.

### Proven full-section function-entry aliases and independent bindings (v6 only)

1. Only explicit `--linker-model armlink5-inline-branchnop-v6` enables sibling-binding
   inheritance or the zero-size R1 entry representation. When relying on either extension,
   the actual allocated candidate ELF section must be executable, non-NOBITS and completely
   represented by candidate bytes. Every non-internal function symbol has the identical raw
   entry value, exactly zero for ARM or one for Thumb; each size is zero or the complete
   section size; at least one sibling records that complete size. ABI spelling alone is not
   proof. Interior entries, mixed ISA values, absent/partial full-size witnesses, data and
   untyped symbols cannot borrow identity or extent. Objects, symbols and candidate bytes
   are never patched or normalized.

   Independently supplied explicit bindings do not use that extension. If each function
   name has its own binding, ordinary frozen placement still checks catalogued section base,
   same-offset/same-placement consistency, global-definition binding, relocations, alignment,
   overlap and the entire section/catalogue extent. A D1 size excluding a trailing pool and
   a zero-size D2 may follow this legacy path when both are independently bound; no binding
   or extent is borrowed, no pool is discarded and R1 removal is not enabled by this case.
   An absent binding still requires the full-section proof; conflicting bindings or offsets
   still refuse. Every trailing pool byte remains part of equality. A short complete section
   is EXTENT, never MATCH or credit.

2. Either proven function-entry alias may supply the original binding for its own section.
   Every supplied function binding must agree and name a catalogued function start. This
   inference is strictly section-local; no binding is guessed for another COMDAT copy. Before
   selecting R1 removal, resolve every supplied copy's original bindings, including aliases
   and other named symbols. A same-COMDAT member mixing an original-bound copy and an R1-
   removable copy refuses even if identical. Unsupported/ineligible counterparts prevent
   partial removal. Every removed copy independently satisfies R1 and has identical complete
   bytes and ARM mapping. Contradictory copies cannot evade consistency checks by retention.

3. A bound identity never receives an R1 rewrite with `target: null`, directly or through
   another copy. Retained bound definitions keep all v2 original-body, size, eligibility and
   equality refusals. A wrong-sized original remains a refusal when deciding inlining even
   if only the caller is claimed or a root retains the helper. No address is invented for
   an unbound helper. Strict complete-section/catalogue comparison remains mandatory.

4. R3 retention is unchanged: roots through either alias, any same-COMDAT group copy/member,
   `__tagsym$$used`, exported/entry/KEEP/first/last selectors and uninterpreted metadata prevent
   removal. Unmatched explicit selectors fail closed. Retention keeps the section but never
   waives bound equality or proves a call stays BL.

5. V6 inherits the corrected v4 bounded-switch CFG/state proof without weakening it. Every
   guard-window instruction belongs to an explicitly modeled effect class; architectural
   core/NZCV changes are accounted for even if decoder write lists omit them. Empty access
   lists do not prove preservation. Before table recovery, every decoded owner instruction
   passes the approved explicit class/encoding effect model, including core/NZCV and state
   effects. This does not promise all owner NZCV values remain unchanged. Unsupported classes
   or subencodings refuse. Every RFE form is unknown control/state restoration, not ordinary
   fall-through or a recognized return; its presence vetoes owner-state proof even before the
   guard, on one predecessor or in an unreachable suffix. All incoming-path setup/entry/result
   contracts, outgoing-successor/liveness checks and unknown-control-flow refusals remain.
   Every allocated-section reference is censused, including otherwise unplaced sections:
   pointers, tail calls, dependencies, interior/addended targets and PREL31/unwind references
   are not discarded. Only unreferenced unrooted helpers with actual eligible inlined calls
   may be removed; no general dead-section elimination or exidx removal is authorized.

6. G4 unique emitted ownership remains mandatory before credit: each stable credited identity
   has exactly one emitted defining unit, its ledger owner, including bound zero-size aliases
   and weak/COMDAT copies. Identical duplicate credited definitions fail. A hidden unbound
   counterpart cannot bypass this through R1 because mixed bound/removable members fail before
   linking returns. Uncredited helpers earn no credit.

7. Match selects a bound proven alias under v6 and compares the whole emitted section against
   the whole catalogue function; independently bound legacy names retain the same complete-
   extent comparison. Receipts expose every removed copy/source/object and actual rewrite,
   with null targets only for genuinely unbound removed identities. Helper-only units remain
   in the ledger/hash closure. Existing helper/COMDAT/header dependency and model-transition
   reviews remain required, including unchanged credited callers and added/changed/removed
   headers.

8. Frozen R1 conditions 7–8 remain mandatory: manually establish every typed caller/helper
   contract and control-flow path; retain a precise visible `[INFERENCE]` source note and
   explicit QUALIFIED identity-provenance review row for each affected caller. No recovered
   helper identity, historical source call graph, genuine armlink execution or independent
   source-policy waiver is claimed.

The approved v4 qualifications remain mandatory. Explicit effects cover guard and owner-state
classification, not a complete architectural/interprocedural model of inherited helper shape,
input provenance or outgoing-liveness decoder metadata. Little-endian catalogue entries and
conforming call returns are manual ABI obligations; ordinary FP exceptions and arbitrary
external behavior are not newly proved. Conservative unsupported subencodings, including the
one-register scalar-LDR POP alias, remain refusals. Entry aliases do not authorize an instruction-
alias workaround, exidx removal or a broader v4 allowlist.

Except for distinct v6 authorization, full-section entry-alias representation and preservation
of independently bound legacy placements above, retain all approved v4 and frozen R1, selector,
L1, rehoming, source-policy, interface, ownership, extent and denominator obligations. Review
both fresh receipts and every helper-copy dependency in full; summary metadata is insufficient.
The old v5 accepted-text artifact remains historical and is not silently relabeled as v6 approval.

Stage B explicitly extends the adopted R1 introduction and otherwise v4-only authorization
to v6. The full-section zero-size entry representation is a v6-only exception to the
recorded-symbol-size requirement in R1 condition 3; every other R1 shape, root, binding,
reference and qualification requirement remains. Independently bound legacy placement
is not that exception: it neither borrows identity nor makes a helper removable.
No v5 normative text is adopted or mechanically renamed into v6 approval.

## Primary sources and review qualifications

- [ELF gABI section flags](https://gabi.xinuos.com/elf/03-sheader.html#section-flags):
  SHF_MERGE/SHF_STRINGS evidence supports candidate record identity, not original pool extent
  or armlink permutation.
- [ARM Compiler v5.05 armlink User Guide, DUI0474K §§4.3, 4.12–4.14](https://documentation-service.arm.com/static/5ea19ea09931941038de96d2):
  inlining precedes unused-section elimination, while roots are separately retained. The
  manual history identifies issue J with v5.04; no actual armlink run is claimed.
- R3 expressly did not close or reverify inherited F10 source/header drift. This draft does
  not reconcile or rewrite those checkpoints. The selector preserves historical verification;
  any current history verification result must be reported from its actual execution.

## Scale plan for the Windows desktop (Shiren lessons 15–22)

| Lesson | Status here |
|---|---|
| 15 mechanical checks before review | G3 (interfaces). UB lint (signed-shift overflow, out-of-bounds index) not yet automated |
| 16 fixed review schema | G6, frozen in v1 |
| 17 detached long jobs | `tools/jobs.py run --name … -- …` (exit marker in `scratch/jobs/<name>/exit`) |
| 18 parallel build, sharded suite, object cache | `build.py` compiles in parallel (`--jobs`, default CPU count); `tools/test_suite.py` shards by module; exact-input object cache (`.cache/objcache`, keyed by compiler hash, flags, include path, source and every header from `--depend`); acceptance builds are `--fresh` |
| 18a queue time vs runtime | `test_suite.py` and `jobs.py` record wall time per module/job; no semaphore yet (single-host, light jobs) |
| 19 native toolchain | armcc.exe is a 32-bit Windows PE: native on the desktop, wibo + Rosetta on the Mac. Cross-host byte identity to be proven by the desktop re-run of c001 |
| 20–21 compressed evidence, safe pruning | not needed yet (evidence is small JSON); before any pruning tool exists, nothing outside `scratch/` is deleted |
| 22 freeze gates before a batch | this file |

Next speedups when the function count grows: shard `tests/test_analyze.py` (it alone sets the
suite floor, ~45 s, by analysing the real image twice), and cache match results keyed by object
hash + catalogue + image + harness hashes.
