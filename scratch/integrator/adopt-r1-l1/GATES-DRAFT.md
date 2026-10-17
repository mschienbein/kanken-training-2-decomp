# Proposed GATES.md revisions: L1 v1.14 and R1 v1.15

Draft for independent selection review and the integrator, 2026-10-17. **Not applied to
`docs/GATES.md`; no real checkpoint is prepared or finalized by this change.**

Base: frozen v1.13. Adopt v1.14 (L1) before v1.15 (R1), recording each revision before the
first checkpoint using its rule. The model implementations have independent
PASS_WITH_QUALIFICATIONS reviews in `scratch/review/literal-records/REVIEW.md` (§6 accepted
L1 text) and `scratch/review/linker-v3-r3/REVIEW.md` (accepted R1 text, reviewed commit
`d51d84e8badb1d5db90d7047f578f0add4959faf`). The checkpoint selector described below is a
separate change requiring independent review before adoption. Defaults remain v2 / `none`.
Historical prepared/finalized checkpoints keep their original gates, models and evidence.

## v1.14 — L1 plus frozen checkpoint selection

### Replace the heading when adopting v1.14

```markdown
# Gate design — frozen v1.14 (2026-10-17)
```

### Prepend to Revisions

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

### Insert after Author loop

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

### Replace G4's table row

| G4 | Two fresh builds | `prepare` | either `tools/build.py` build (fresh: no object cache) fails, outputs differ, either independent model field (`linker_model`, `literal_model`) differs between the builds, or either differs from the frozen explicit selection. Each build: every credited function an ordinary catalogued function, exact size, equal to the original on its own before being written; every selected L1 pool satisfies L1 and is recorded; no overlapping sections across units; no duplicate global definitions; `code.bin` equal to the original; recompressed `.code` equal to the stored ExeFS `.code`. External data earns no additional credit. Selection and complete receipts remain bound through finalize/history verification |

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

## v1.15 — R1 plus the same frozen selector

### Replace the heading when subsequently adopting v1.15

```markdown
# Gate design — frozen v1.15 (2026-10-17)
```

### Prepend to Revisions

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

### R1 — candidate-only inlined and removed callee

R1 is permitted only for a new checkpoint prepared after this revision and a
passing independent implementation review, under the explicitly recorded model
`armlink5-inline-branchnop-v3`. Both fresh G4 builds, prepare and finalized
history must bind the same model. Existing prepared/finalized checkpoints do
not change gates or model. Author-loop opt-in matches alone do not authorize
acceptance; the acceptance build path must actually select and record v3.

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
   including both conditional arms and branch targets, are checked for reads of
   r1–r3/r12/flags before unconditional definitions up to calls/returns or decoded
   code-range ends. Unknown indirect control flow fails closed; r0 may be a result
   and I1 requires no inputs. These checks do not prove the argument/result
   types, interprocedural liveness, or that the original source contained a call.
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
   Argcheck must not invent an original callee edge for `target: null`; missing
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

## Conservative zero-size destructor-alias boundary (not broadened here)

The integrator relayed w23f's `scratch/authors/w23f/v3-refusals.json`: ARMCC emits a zero-size
D2 base-destructor alias alongside a four-byte D1 body. Current R1 eligibility checks **every**
STT_FUNC alias for offset zero **and full section size**. A zero-size alias therefore leaves
the section ineligible and produces a refusal, consistent with reviewed R1 item 3. This is a
conservative unsupported representation, not evidence that the C++ helper cannot exist.
Changing size-zero aliases into assumed full-section aliases would broaden the reviewed rule
and needs separate implementation/evidence/review; this selector change does not make it.
Do not edit object symbols or add an alias workaround to obtain credit.

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
