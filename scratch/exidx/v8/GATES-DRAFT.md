# Proposed v1.19 — v8 complete own-unwind elimination

**DRAFT, unreviewed, not adopted.** Frozen `docs/GATES.md` remains v1.16. This is a new
exact-text proposal for `armlink5-inline-branchnop-v8`, not an amendment that approves or
relabels the rejected v7 proposal at `scratch/exidx/GATES-DRAFT.md`. Only Main can adopt
an independently approved revision after the active frozen checkpoint finalizes and closes.
A different next-unused revision number/adoption date may be substituted without changing
any normative condition. No active or historical checkpoint is reselected or rewritten.

## Provenance and given base

The given base is corrected v6 at synced `models-integration` commit
`5bb20a9a3251e83cddc37d6b64a166679ebd8e51`. V4 and v6 prerequisites/qualifications remain
mandatory and are not independently approved by this work. V8 = v6 plus the corrected
exclusive own-exidx/extab exception below; it is not a change to a recorded v7 rule set.

The sealed independent v7 review, `scratch/review/exidx-v7/REVIEW.md` and `SEAL.json`, is
**FAIL**, with no acceptable gate text. F01 found partial elimination from a live COMDAT
group; F02 found acceptance of compact extab missing the descriptor-list terminator and
refusal of the complete encoding. V7 is retired by name, never adopted. Its receipts, exact
rejected draft and review remain immutable; none is relabeled as v8 approval or evidence.
The two reviewed smoke MATCHes and frozen-equivalence observations do not waive those defects
or independently approve their correction.

## Exact proposed revision entry

- **v1.19** (2026-10-17): only the new, explicitly selected immutable model
  `armlink5-inline-branchnop-v8` permits an R1-removed helper's exclusive own EHABI index
  and supported frame-only extab to disappear with its complete removable allocated
  section-group/copy closure. V8 is corrected v6 plus this rule only. The active frozen
  strict-v1/v2/v3/v4/v6 rules remain unchanged; v5 and v7 remain retired, never adopted.
  V7's rejected rule permitted partial live-COMDAT-group removal and accepted compact extab
  without the required empty descriptor-list terminator; neither behavior is corrected
  under its recorded name. V8 requires every affected group's allocated member in every
  supplied copy to have an independent R1-code or own-companion removal proof; kept,
  unmodeled or unrepresented members and unsupported group semantics refuse removal.
  Out-of-line compact extab includes the complete unwind prefix plus exactly one final
  zero descriptor-list terminator, checked separately from encoded unwind-word count.
  Shared/combined metadata, kept-function keys, required original placement, other incoming
  references, unsupported relocations, scope descriptors and incomplete/trailing data fail
  closed. Selection, both fresh full receipts, prepare and history explicitly bind v8 and
  every eliminated companion; model transitions reopen every affected R1 caller. No function,
  helper, metadata, data or denominator credit follows from this implementation or draft.

## Exact addition to R1 model selection

After this complete v8 implementation and exact text receive independent approval and this
revision is frozen, R1 also admits the distinct `armlink5-inline-branchnop-v8` identifier.
V8 inherits every approved v4 bounded-CFG/effect/state condition and corrected-v6 alias,
independent-binding, COMDAT, retention and placement condition. Its only exception to v6's
all-reference/unwind boundary is the complete own-companion rule below. V7 remains retired
with its historical failure reason; no old identity is broadened or silently reinterpreted.
Default selection remains v2 / literal `none`. Both fresh G4 builds, immutable selection,
prepare and finalized history must name v8 explicitly. Existing model-transition and
helper/header/COMDAT dependency reviews remain mandatory, including unchanged credited
callers and helper-only ledger units. A private MATCH is neither adoption nor acceptance.

## Exact normative addition: complete own-unwind closure (v8 only)

1. Only explicit v8 selection enables this exception. Select the code helper under every
   unchanged corrected-v6 R1 requirement first. The complete source-defined 4/8-byte ARM
   helper and every supplied identical removable COMDAT copy must remain original-unbound,
   unrooted and independently eligible. At least one actual admissible inlined call per
   entity is required, including the existing identical-used-copy rule. A metadata entry
   or apparently unused group cannot authorize general dead-code/data elimination. Bound
   aliases, mixed original-bound/removable copies, unsupported counterparts, contradictory
   copies and all existing call-ABI/CFG/reference failures remain refusals.
2. Each eliminated text section owns at most one eliminated index. It is complete readonly,
   allocated, non-executable, non-NOBITS `.ARM.exidx*`, type SHT_ARM_EXIDX, SHF_LINK_ORDER,
   with sh_link to that exact defining text section in that object. Combining objects remaps
   the linked index within object ownership. Size is exactly eight, alignment at least four,
   entry size zero/eight. Exactly one offset-zero R_ARM_PREL31 targets the helper's section/
   function entry, zero effective addend and bit 31 clear. Shared/combined/duplicate indexes,
   keys for kept code, interior keys and missing/unsupported metadata are refused; no entry
   is split out of a section or assigned an invented original address.
3. The second index word is CANTUNWIND (1), one-word compact personality 0 directly in exidx,
   or exactly one offset-four R_ARM_PREL31 to the start of one exclusive `.ARM.extab*`.
   Extab is complete readonly, allocated, non-executable, non-NOBITS SHT_PROGBITS, aligned
   at least four, without link-order/fixed-entry metadata. Only compact personalities 0/1/2
   followed by an **empty descriptor list** are modeled. The header/unwind prefix occupies
   four bytes for personality 0 and `4 * (1 + N)` bytes for personalities 1/2, where N is the
   encoded extra-unwind-word count. Exactly one additional zero word must follow that prefix.
   Validate its value independently of N and require the complete section size to be eight
   bytes for personality 0 or `4 * (2 + N)` for personalities 1/2. Missing/partial/nonzero
   terminators, missing/excess unwind words, nonempty scope descriptors, generic formats,
   trailing/combined entries and interior targets refuse removal. The direct one-word
   personality-0 representation inside exidx is distinct and remains unchanged.
4. Only the structural PREL31 references above and offset-zero R_ARM_NONE dependency markers
   to the matching undefined `__aeabi_unwind_cpp_pr0/1/2` are modeled in eliminated metadata.
   CANTUNWIND has no extra dependency. An extab-pointing index has exactly its two structural
   references, with personality dependence in extab. Every other relocation or dependency,
   misplaced/unknown NONE marker, descriptor relocation and V4BX in metadata is refused, not
   ignored on the assumption that it will disappear. Ordinary code's existing nonreferencing
   V4BX treatment is unchanged.
5. Both companions have no root, explicit retention, original binding or independent named/
   exported entity, and their COMDAT identity agrees with the described code. Only local
   zero-size offset-zero section/data-mapping symbols are allowed. Exidx has no incoming
   allocated reference; extab's sole incoming reference is its own index's offset-four
   PREL31. Census every supplied allocated section, including otherwise unplaced sections,
   and all link-order dependencies. Sharing, other incoming references and required original
   placement prevent elimination. Every other helper reference still passes unchanged R1;
   pointers, tail calls, dependencies, interior/addended targets and inadmissible calls cannot
   disappear through this exception.
6. **Complete allocated group/copy closure is mandatory before exempting any metadata
   reference.** Equal signatures alone are insufficient. For each affected COMDAT signature,
   inspect every supplied group's/copy's actual allocated membership and all same-signature
   allocated sections. Every member must already be selected v6-eligible R1 code or a fully
   validated own-exidx/extab companion. Any surviving catalogued caller/function, independently
   bound entity, ordinary placed or unplaced data/BSS, unsupported code or unrepresented
   allocated member prevents whole-group elimination. Preserve allocated membership witnesses
   even for zero-size sections omitted from frozen placement, and preserve/remap them through
   object combination. A missing/contradictory witness or member cannot be ignored. Non-COMDAT
   or unknown group semantics are unsupported by the exception and fail closed. No arbitrary
   unused member is swept into the removal set. Every selected code entity must still pass
   the unchanged actual-inlined-call requirement; closing a group does not waive it.
7. Never select an entry for a surviving function. Such a function's separate unwind index
   remains under the inherited hybrid-link qualification, not a new reconstruction claim.
   A companion binding or incoming reference requiring it in the original output refuses
   elimination. An unbound removed helper has no original address to encode as an original
   key. Do not invent one or alter original unwind bytes. The flat image has no section
   headers/authenticated original exidx bounds; absence of an identified original table stays
   explicitly [INFERENCE], not a stripping premise. V8 is not general exception-table
   placement/merging/stripping or proof of the original exception regime.
8. Each removed helper and per-call target-null rewrite records every eliminated companion's
   name/index, kind, full size and defining-object ownership in `unwind_sections`. Complete
   defining source/object/profile hashes remain bound in both full receipts and the ledger,
   including helper-only units. No helper or metadata earns catalogue/data credit; no payload
   or original disassembly is recorded. Existing selection/full-receipt/history closure and
   all helper/COMDAT/header dependency/model-transition reviews remain mandatory.
9. Exact full-function bytes/extents, every incoming/outgoing ABI/control-flow path, typed
   helper/caller contracts, little-endian/conforming-call obligations, visible [INFERENCE]
   source notes and explicit QUALIFIED identity-provenance review rows remain mandatory.
   Neither group closure, metadata framing nor an exact MATCH proves the helper's original
   existence, identity or historical call graph. No source-policy waiver or complete native
   armlink/all-source unwind/exception proof is claimed.

## Exact G4/G6 reviewer additions

G4's exactness, frozen selection and full-receipt obligations also require the complete v8
rule above in each fresh build. Every eliminated companion must appear in each full receipt.
Byte-identical output cannot waive a live group member, truncated entry, unsupported metadata
or required original placement.

Under G6 inspect actual ELF group membership/flags for all affected copies, including zero-size
allocated witnesses and object-local remapping; do not infer group discard from signatures or
helper-call removal alone. Independently establish every member's removal proof. Inspect
actual type/flags/sh_link, effective PREL31 targets, whole-section extent, encoded unwind-word
count and separate final zero descriptor terminator. Check all incoming references, ownership,
roots and original bindings. Establish that no kept function/group member or required original
entry is erased. Preserve every inherited typed-contract/ABI/QUALIFIED provenance obligation.

## Required independent re-review evidence

Re-derive the sealed F01 class with a kept caller, a different kept function, placed/unplaced
data/BSS and hidden members in another supplied group copy, in both object orders. Exercise
complete removable groups, multiple independently used helpers, uncalled helpers, unsupported
group flags and zero-size/unrepresented allocated membership through real ELF reading and
object combination. Re-derive F02 for personalities 0/1/2: complete 8/12/12-byte examples must
work, truncated 4/8/8-byte examples must refuse, and nonzero/partial terminators, incorrect
unwind counts, trailing/combined data and descriptor relocations must refuse. Preserve the
inline-exidx personality-0 positive separately.

Recheck all previous own-exidx negative boundaries, the re-smoked natural-source receipts,
accepted-regression rows and strict-v1/v2/v3/v4/v6 equivalence. Preserve reported author compile
failures rather than retrying unchanged sources to confirm them. Review the image/table
inference with its existing limitations; it remains no authority to discard live groups.
Main runs the full suite once after corrected integration. No full suite, build, prepare,
finalize, active-gate edit, adoption or push is authorized by this author work.
