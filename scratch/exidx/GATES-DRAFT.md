# Proposed v1.19: exclusive removed-helper EHABI companions

Status: **DRAFT, unadopted**. `docs/GATES.md` remains frozen v1.16. This proposal grants no
credit and changes no active, prepared, accepted or historical checkpoint. Main alone may
adopt a separately reviewed revision after the current frozen checkpoint has finalized and
closed. Revision numbering may be changed only without changing the normative requirements.

The unwind extension was first developed against `d2-alias-r2` tip `22cb05b`. Its final
v7 identity composes with the corrected v6 baseline on `models-integration`: v5 is retired
and never adopted because it regressed independently bound D1/D2 placements that v4 credits.
V4/v6 qualifications remain mandatory. The whole v7 composite and this exact text require
independent review; predecessor reviews do not approve v7.

The exact synced base is `5bb20a9a3251e83cddc37d6b64a166679ebd8e51` (corrected-v6
`d525edd6d9e17675d770781b9df4d7d6d8bd17a5` plus frozen main inputs). The final v7
implementation is `b72126ebc55984ed231f8698a87417c284a63271`; handoff/evidence-only commits
do not change its linker rules. No predecessor review or private match authorizes adoption.

## Exact proposed revision entry

- **v1.19** (2026-10-17): only the new immutable, explicitly selected linker model
  `armlink5-inline-branchnop-v7` adds elimination of an R1-removed helper's exclusive
  EHABI `.ARM.exidx` and supported compact `.ARM.extab` companions. It is corrected v6 plus
  this rule only. `strict-v1` and v2/v3/v4/v6 retain byte-identical behavior, including their
  existing unwind-reference refusals. Retired v5 remains retired. The code helper must first
  satisfy every v6 R1 eligibility, original-binding, root, alias/COMDAT, ABI/CFG and
  all-reference condition.
  Its single-entry index must be authenticated by ELF type, SHF_LINK_ORDER, object-local
  sh_link and an entry-only PREL31 key. Shared/combined metadata, kept-function entries,
  required original placement, other incoming references and unsupported relocations or
  table formats fail closed. Receipts bind and expose the eliminated companions without
  payload. Both fresh builds, selection, prepare and history explicitly bind v7; model
  transitions reopen every affected R1 caller. All source, typed-contract, manual ABI,
  exact-extent and QUALIFIED identity-provenance requirements remain mandatory. Cause:
  ARM DUI0474K inlining-before-unused-elimination, EHABI per-code-section index metadata,
  and ELF SHF_LINK_ORDER coupled retention/discard semantics. No general exception-table
  stripping, original unwind-table reconstruction, recovered helper identity, denominator
  change or acceptance credit follows from this revision.

## Exact addition to R1 model selection

After this revision and its independent composite implementation/exact-text review are
approved and frozen, R1 also admits the distinct identifier
`armlink5-inline-branchnop-v7`. V7 inherits every reviewed v4 bounded-CFG/effect/state
condition and corrected v6 full-section-entry alias/COMDAT condition. Its only exception to v6's
all-reference/unwind boundary is the exclusive companion rule below. Neither a gate-only
revision nor a private tool invocation changes the meaning of any older model. Default
selection remains `armlink5-inline-branchnop-v2` with literal model `none`. Both fresh G4
builds, the immutable selection, prepare and finalized history must name v7 explicitly.
The existing model-transition and helper/header/COMDAT dependency reviews remain mandatory,
including unchanged credited callers. Old or in-flight checkpoints are never reselected.

## Exact normative addition: R1 exclusive EHABI companions (v7 only)

1. Only explicit v7 selection enables this rule. Select the code helper under the unchanged
   v6 R1 conditions first. The whole source-defined 4/8-byte ARM helper and every identical
   removable COMDAT copy remain unbound to the original, unrooted and independently
   eligible. At least one actual admissible inlined call per entity is still required;
   metadata alone cannot authorize dead-code elimination. Bound aliases, mixed bound and
   removable copies, retained/unsupported counterparts and contradictory copies fail.
2. Each eliminated code section has at most one eliminated index section. It is a complete,
   readonly, allocated, non-executable, non-NOBITS `.ARM.exidx*`, type `SHT_ARM_EXIDX`, with
   SHF_LINK_ORDER and sh_link to that exact text section in that object. Object combination
   remaps sh_link without crossing object ownership. It is exactly eight bytes, aligned
   at least four, with entry size zero or eight. Exactly one offset-zero R_ARM_PREL31
   against its code section/function entry has zero effective addend and bit 31 clear.
   Combined/shared indexes, duplicate fragments for one text section, keys for kept code,
   interior keys and unsupported/missing ELF metadata are refused; no entry is split out.
3. The second word is CANTUNWIND (1), a single-word compact personality-0 entry, or exactly
   one offset-four R_ARM_PREL31 to the start of one exclusive `.ARM.extab*` section. Extab
   is complete readonly, allocated, non-executable, non-NOBITS SHT_PROGBITS, aligned at
   least four, without link-order/fixed-entry metadata. Only a frame-only compact entry
   is modeled: personality 0 occupies exactly four bytes; personality 1 or 2 occupies
   exactly `4 * (1 + extra_word_count)` bytes. Generic personalities, scope descriptors,
   trailing/combined data and interior extab targets are conservatively refused.
4. Only the preceding structural PREL31 references and offset-zero R_ARM_NONE dependency
   markers to the matching undefined `__aeabi_unwind_cpp_pr0`, `pr1` or `pr2` are modeled
   in eliminated metadata. CANTUNWIND has no extra dependency. An extab-pointing exidx has
   exactly its two structural relocations and no separate dependency. Any other relocation
   kind, unknown NONE dependency, misplaced marker or V4BX in unwind data is refused, not
   ignored merely because the metadata might die. Ordinary code's nonreferencing V4BX
   handling is unchanged.
5. Both companions have no root, explicit retention, original address binding or independently
   named/exported entity. Their COMDAT identity is exactly their code section's identity.
   Only local zero-size offset-zero section/data-mapping symbols are permitted. The index
   has no incoming allocated reference. Extab's sole incoming reference is its own index's
   offset-four PREL31. Census every supplied allocated section, including unplaced ones,
   and all link-order dependencies. Other incoming references, shared extab, dependencies
   from kept code/metadata or required original placement prevent elimination. Every other
   reference to the helper remains subject to unchanged R1: pointers, tail calls, dependency
   markers, unsupported references and inadmissible calls cannot disappear.
6. Never eliminate an entry for a surviving function. A surviving function's separate
   index remains outside this exception, under the existing hybrid-link qualification.
   Any companion binding or incoming reference that would require it in the original
   output is a refusal. An unbound removed helper has no original address to encode in
   an original index. Do not invent one, change any original unwind bytes or claim that
   candidate ELF unwind emission establishes an original table. The flat image lacks
   section headers and authenticated original exidx bounds; absence of an identified
   table is explicitly [INFERENCE], not a premise authorizing general table stripping.
7. Each removed helper and its per-call target-null rewrite records every eliminated
   companion's section name/index, kind, full size and defining-object ownership in
   `unwind_sections`. The whole defining object/source/profile hashes remain in both full
   receipts and the ledger, including helper-only units. Companions and helpers earn no
   function/data credit. Selection/receipt/history closure and every existing dependency
   review remain unchanged. No private payload or disassembly is recorded.
8. Exact full-function bytes/extents, all incoming/outgoing ABI/control-flow paths, typed
   helper/caller contracts, little-endian/conforming-call obligations, visible [INFERENCE]
   source notes and explicit QUALIFIED identity-provenance review rows remain mandatory.
   Neither exclusive metadata nor an exact match proves the helper's original existence,
   identity or call graph. V7 does not prove complete original unwind-table placement,
   exception behavior or a standalone all-source armlink image, and does not waive source
   policy or any inherited v4/v6 qualification.

## Exact G4/G6 reviewer additions

G4's existing exactness/selection/receipt obligations also require that any selected v7
companion satisfy the complete rule above in each fresh build and appear in that build's
full receipt. A byte-identical output does not waive an unsupported or still-required entry.

Under G6, inspect the actual candidate ELF type/flags/sh_link and effective PREL31 targets,
whole-section extents and symbol/retention metadata, all incoming references and object
ownership. Establish that no kept-function index or required original entry is erased.
Review inherited R1 caller/helper contracts and QUALIFIED provenance independently; do not
infer historical helper identity or original exception settings from an empty destructor,
a successful CFG filter, an absent identified original table or a private MATCH.

## Required independent evidence before adoption

Re-derive fail-before/pass-after synthetic and real-compiler D1/D2 examples; every listed
negative rule; object combination in both orders; the original-image structural census
and its explicit limitations; all named natural-source v6/v7 smoke exits and any claimed
MATCH receipts; the 15 supplied accepted-regression rows under each model; and frozen-model
fingerprints against the corrected v6 integration base. Main runs the full suite once after integration, not in parallel
with worker verification. No full suite, prepare/finalize, gate adoption or push is authorized
by this author work.
