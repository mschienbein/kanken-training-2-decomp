# Linker model: armlink output optimisations

Status: default model **`armlink5-inline-branchnop-v2`**; frozen
**`armlink5-inline-branchnop-v3`** adds R1 removed-callee handling under adopted gates v1.15.
The opt-in **`armlink5-inline-branchnop-v4`** separately selects bounded-switch CFG recovery
and fail-closed endian-state handling under adopted gates v1.17.
`accept.py prepare --linker-model ... --literal-model ...` freezes both choices before its fresh
builds; see `docs/ACCEPTANCE.md` for selection/receipt/history binding and transition review.
The corrected alias composite is opt-in **`armlink5-inline-branchnop-v6`**, adopted under
gates v1.18; v5 is retired, never adopted. The exact accepted gate text and sealed independent
reviews are `scratch/review/r1-jumptables-r3/REVIEW.md` and
`scratch/review/models-v6/GATES-ACCEPTED.md` / `SEAL.json` (PASS_WITH_QUALIFICATIONS).
Both gate revisions follow finalized `c008-r3` (sequence 8) and precede the code merge.
The opt-in **`armlink5-inline-branchnop-v8`** (corrected v6 plus complete own-unwind closure)
is adopted under gates v1.19; v7 is retired, never adopted. The exact accepted gate text and
sealed independent review are `scratch/review/exidx-v8/GATES-ACCEPTED.md` / `SEAL.json`
(PASS_WITH_QUALIFICATIONS). The v1.19 revision precedes the code merge.
Original model: ArmlinkInline, 2026-10-16; removed-callee extension: LinkerRemovedCallee, 2026-10-17.

The public toolchain has armcc but no armlink, so the original link step cannot be rerun. The
in-place linker (`tools/inplace.py`) relocates each compiled branch itself. Two armlink output
optimisations rewrite branches in the original image; without modelling them, C that the
original linker turned into these bytes can never match. The model reproduces them **only
where the rule follows from the final layout and the original callee's bytes**; every rewritten
branch is reported (`linker_rewrites` in `tools/match.py` results, `tools/build.py` receipts and
`tools/compid.py` records), and the model name is recorded (`linker_model`). `strict-v1` (no
rewrites; the oracle before this change) stays selectable with `--linker-model strict-v1`.

## Placement refusals and matcher exits

`tools/match.py` exits **0** only when every surviving function matches, **1** for a
completed comparison with differing bytes or extents, and **2** when compilation or linking
is refused. A candidate with no surviving catalogued function to compare is also refused.
Compile/link refusals write `<source-stem>.result.json` with `status: "refused"`, a precise
`reason`, the `error_type`, selected profile/models and an empty `functions` list; they never
invent mismatch rows.

Image-address failures during placement are `LinkError`s, including nested references through
common initializer sections, RTTI/typeinfo, ordinary `.constdata`, anonymous string witnesses,
inline-callee reads and removed-call context reads. Diagnostics identify the referring
section/offset or operation, span size and failure category. Unvalidated placement witnesses
and underlying image-error values are redacted, including alignment/conflict/overlap diagnostics.
A function/alias whose section offset implies a noncatalogued section base is refused before
catalogue lookup. These checks
do not infer another placement or change relocation, equality, alias or model decisions.
Only failures of the image-read boundary are translated; unrelated internal `ValueError`s
and `KeyError`s remain visible.


## Model history

A model name is a version: rules are never edited under a recorded name.
The older v3 review fixes below preceded its adoption. V4 never changes v3's frozen policy.

| Model | Status | Rules |
|---|---|---|
| `strict-v1` | selectable | no rewrites |
| `armlink5-inline-branchnop-v1` (commit c11a0ad) | **retired**: `--linker-model` refuses it | as v2, except that a callee defined in the candidate object was judged by the candidate's own section, never by the original. Independent review (ReviewLinker, F01) showed a wrong local helper could make its caller match: a `bx lr` helper at an address whose original is a 12-byte body, the excluded `MOV r0, r0; BX lr`, a veneer or a multi-entry owner turned the caller's BL into the original's NOP, and a caller-only ledger credited the caller (reproduced on the real image with func_001b6ec4 calling a local `func_0013714c`). Only artifacts of this branch's own checks recorded it. |
| `armlink5-inline-branchnop-v2` | default | below |
| `armlink5-inline-branchnop-v3` | frozen opt-in, not the default | all v2 rules plus candidate-only callees in R1 below; original conservative CFG, no resolved jump tables |
| `armlink5-inline-branchnop-v4` | frozen opt-in, adopted gates v1.17; PASS_WITH_QUALIFICATIONS | all v3 helper/retention/equality rules; explicitly selected bounded-switch CFG, strict ordinary returns, owned-range clipping and explicit modeled-effect/state proof |
| `armlink5-inline-branchnop-v5` (reviewed 22cb05b) | **retired**, never adopted | reviewed v4/alias composite incorrectly refuses independently bound D1/D2 sections when the nonzero symbol excludes a trailing pool; historical receipts retain that identity, superseded by v6 |
| `armlink5-inline-branchnop-v6` | frozen opt-in, adopted gates v1.18; PASS_WITH_QUALIFICATIONS | all v4 rules plus proven full-section function-entry aliases and entity-wide removal preflight; independently bound names retain frozen placement/complete-extent checks without borrowing a sibling binding |
| `armlink5-inline-branchnop-v7` (88f5c38) | **retired**, never adopted | its own-exidx exception could partially remove a live COMDAT group and accepted truncated compact extab while rejecting complete zero-terminated entries; sealed review `scratch/review/exidx-v7/REVIEW.md`, FAIL |
| `armlink5-inline-branchnop-v8` | frozen opt-in, adopted gates v1.19; PASS_WITH_QUALIFICATIONS | corrected v6 plus exclusive own-exidx elimination with complete allocated group/copy closure and zero-terminated frame-only compact extab; no kept-function table removal |

## Function-entry alias identity (v6 only)

ARMCC 5.04, under the frozen `game` profile, emits native C1/C2 constructors and both
virtual and non-virtual D1/D2 destructors as same-entry `STT_FUNC` symbols, with a nonzero
size on only one sibling. That size can exclude a trailing literal pool. The ABI names distinguish
[complete/base construction and destruction](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#mangling-special-ctor-dtor);
the names alone **never** prove that two implementations are identical.

Only the explicit **`armlink5-inline-branchnop-v6`** selection enables this identity
extension. V6 composes the independently reviewed v4 baseline with the alias rules
and bound-placement correction below, adopted under gates v1.18. V4's approved
source series is `21c2bd4` + `e72fb34` + `5187259` (sealed review:
`scratch/review/r1-jumptables-r3/REVIEW.md`, PASS_WITH_QUALIFICATIONS).
`strict-v1`, v2, v3 and v4 retain their original alias behavior: no sibling-name binding
inheritance, and no zero-size entry alias in R1.

`tools/inplace.py::function_aliases` proves identity from the ELF instead: the section must
be executable, non-NOBITS and fully represented by its candidate bytes; every function
symbol has the same offset-zero entry **and ISA**, every size is zero or the complete
section size, and at least one function symbol supplies that full size. A proven zero-size
entry has the full sibling's address and extent, without altering its ELF metadata or bytes.
Either ABI name may supply the original address binding, but every supplied binding must
agree and name a catalogued function start. Match results use a bound name for the section.
Interior zero-size symbols, missing/partial full-sized siblings and mixed ARM/Thumb entries
cannot borrow a sibling's binding or satisfy R1's alias proof; data symbols never borrow a function binding.

**Independently bound retained names do not borrow identity.** When every function name has its
own explicit agreeing binding, the existing placement path still checks the section base,
same-offset/same-placement consistency, global-definition binding, relocations, overlap and the
complete candidate/catalogue extent. That frozen path accepts a D1 code-size symbol and a zero-size
D2 symbol even when a trailing literal pool makes neither symbol a full-section witness. V6
preserves it without changing `function_aliases`, inventing a binding, normalizing ELF metadata,
discarding the pool or permitting R1 removal. Missing bindings still require the full-section
proof above; mismatched pool bytes remain MISMATCH and a short section remains EXTENT.

The default v2 model still refuses an entirely unbound destructor. V6 does not invent
an original image address: an entirely unbound helper must satisfy every R1 requirement.
Only v6 evaluates R1's full-section-entry check using the proven identical aliases above;
the original-address exclusion, code/ABI, call-site and all-reference checks remain mandatory.
An allocated `.ARM.exidx` relocation to the helper is still a live reference and prevents
removal. Global data or untyped definitions are not function aliases and prevent v6 removal.
Alias identity never waives those modeled effects: missing decoder write metadata is not
proof that LDREXD/MRRC preserve a bounded index. Unresolved RFE PC/CPSR restoration remains
unknown control/state, including in an otherwise unreachable decoded owner suffix.
V4's effect model applies to guard and owner-state classification, not a complete
architectural/interprocedural proof of inherited helper, input or outgoing decoder metadata.
Little-endian catalogue entries and conforming call returns remain manual ABI obligations;
ordinary FP exceptions and arbitrary external behavior are not newly proved. Every v4
qualification and conservative unsupported-encoding refusal also applies to v6.

Alias binding inheritance is **section-local**, not a guess about another COMDAT copy.
Before R1 removal, every supplied copy's explicit original bindings are resolved.
If one copy of a COMDAT member is original-bound and another would be removed, the
entire link is refused, even when their bytes are identical. A copy with unsupported
entry metadata or otherwise ineligible for removal also prevents partial entity removal.
Every copy of a removed member must pass R1 and have identical complete bytes and code
mapping. No bound identity can acquire an R1 `target: null` through a different copy.
Roots through either entry alias, or a different copy, retain the whole COMDAT group;
retention never waives v2's original-body equality or complete-extent comparison.
The build's pre-link unique emitted-owner checks remain mandatory, including zero-size
bound aliases and byte-identical credited COMDAT copies.

The original proposal failed independent review (F01, sealed in
`scratch/review/d2-alias/REVIEW.md`); the corrected v5 implementation at `22cb05b` received
PASS_WITH_QUALIFICATIONS (`scratch/review/d2-alias-r2/REVIEW.md`). A subsequent real-source
model sweep exposed a different v5 regression: `func_001c6a68` has a 100-byte D1 plus 4-byte
pool, and `func_001ef89c` a 188-byte D1 plus 8-byte pool; both have a zero-size D2, both names
explicitly bound, and full sections matching under v4. V5 incorrectly rejected them before
comparison. Its blanket zero-size refusal is historical, not corrected under the same name:
v5 is retired with that reason and v6 adds the bounded correction described above. No v4 fix
or unsound v4 credit follows from these rows.

The v6 implementation at `5bb20a9` passed focused independent re-review
(`scratch/review/models-v6/REVIEW.md` / `SEAL.json`, PASS_WITH_QUALIFICATIONS).
GATES v1.17 and v1.18 adopt the exact accepted v4 and v6 texts after `c008-r3` finalized,
before the code merge and before any new checkpoint selecting these models.
Selection, both fresh receipts, prepare/history and model-transition review must bind v6.
Existing v5 author scratch receipts and the original sweep remain immutable and grant no credit.
Regression coverage: `CppFunctionAliasTest`, `CompositeAliasSwitchTest`,
`RemovedCalleeModelTest`, `CppAliasCompareTest`, `AliasCompareSafetyTest`, and
`BuildRemovedCalleeModelTest` in `tests/test_inplace.py`, `tests/test_match.py`, and `tests/test_build.py`.

## Removed-helper unwind companions (v8 only)

`armlink5-inline-branchnop-v8` is corrected v6 plus the following single R1 extension.
The default, `strict-v1`, and v2/v3/v4/v6 policies remain unchanged; v5 and v7 remain retired.
V6 still refuses the allocated `.ARM.exidx` reference. Neither alias identity nor unwind
ownership relaxes any helper, original-binding, retention, I1–I3, CFG or typed-contract rule.

1. Only a helper already independently eligible for R1 removal can own eliminated unwind data.
   Its own readonly, allocated, non-executable `.ARM.exidx*` must be `SHT_ARM_EXIDX`, have
   `SHF_LINK_ORDER`, and `sh_link` must name that exact candidate text section. ELF reading
   preserves these fields and combining objects remaps the linked section within its owning
   object; equal section numbers in different objects never establish ownership.
2. The complete index section is exactly one eight-byte entry, aligned at least four, with
   entry size zero or eight. Exactly one `R_ARM_PREL31` at offset zero targets the removed
   helper's section/function entry with zero effective addend and bit 31 clear. A second
   index section for the same text section, a combined index, a kept-function key, an interior
   key or missing/wrong section metadata is refused, not partially discarded.
3. The second word is either `EXIDX_CANTUNWIND` (1), a single-word compact personality-0
   entry, or an offset-four `R_ARM_PREL31` to the beginning of one exclusive `.ARM.extab*`.
   Extab must be complete readonly, non-executable `SHT_PROGBITS` without link-order or
   fixed-entry metadata. Only frame-only compact personalities 0/1/2 with an empty descriptor
   list are modeled. The unwind prefix is four bytes for personality 0 and
   `4 * (1 + extra_word_count)` for personalities 1/2; **one further zero word is mandatory**.
   Thus a complete personality-0 extab is eight bytes and a personality-1/2 extab is
   `4 * (2 + extra_word_count)` bytes. The final zero is validated separately, outside the
   unwind-word count. Missing/partial terminators, nonempty scope descriptors, generic
   personalities, combined/trailing data and interior extab references are refused.
   The one-word personality-0 encoding directly in exidx is unchanged and needs no extab.
4. Beyond those structural PREL31 relocations, only offset-zero `R_ARM_NONE` dependency
   markers to the matching undefined `__aeabi_unwind_cpp_pr0/1/2` are understood. A
   CANTUNWIND entry has no extra dependency. An extab-pointing exidx has exactly its two
   structural relocations; its personality dependence belongs in extab. Every other
   relocation/dependency, including arbitrary NONE markers and V4BX in unwind data, refuses
   removal. V4BX retains its existing nonreferencing meaning in ordinary code.
5. Neither companion may be rooted, independently named/exported, original-address-bound,
   or in a different COMDAT group from its text. Only local offset-zero, zero-size section
   symbols/data mapping markers are permitted. Exidx has no incoming allocated reference;
   extab's only incoming reference is its own exidx's offset-four PREL31. All supplied allocated
   sections, even unplaced sections, are checked. Other link-order dependencies on removed
   code or companions also refuse removal. Shared extab and references from kept entries
   cannot disappear. Every other reference to the helper still passes the unchanged R1
   all-reference check; a pointer, tail call, NONE dependency or inadmissible BL refuses.
   Before any companion-reference exemption, every allocated member of each affected COMDAT
   group and every supplied copy must already be selected v6-eligible R1 code or a validated
   own-exidx/extab companion. A surviving caller/function, bound or unplaced data/BSS,
   unsupported code or unrepresented allocated member prevents whole-group elimination.
   ELF reading retains zero-size allocated group witnesses even though frozen placement does
   not represent their sections; object combination preserves/remaps every witness.
   Non-COMDAT and unknown group semantics are not modeled by this exemption and fail closed.
   No arbitrary unused group member is swept in, and every selected code entity still needs
   its actual eligible inlined call (or an identical used COMDAT copy).
6. A surviving function's own unwind entry is never selected by this rule. Any binding or
   incoming reference that would require a companion in the original output is refused.
   Candidate-only helpers have no original address, so their keys cannot be inserted into
   an original table without inventing an address. Existing original unwind bytes, if any,
   remain untouched by this hybrid linker. V8 is not general exception-table stripping,
   placement, merging, rewriting or proof of a complete standalone armlink image.

Each removed helper and its per-call `target: null` rewrite records `unwind_sections`,
including section name/index, kind, size and defining-object ownership. Whole input-object
hashes already bind the eliminated contents; no private table payload is emitted. The helper
still needs at least one successfully inlined call (or an identical used COMDAT copy), earns
no catalogue credit and retains every R1 identity/provenance qualification.

The sealed v7 review (`scratch/review/exidx-v7/REVIEW.md`, FAIL) found that matching group
signatures alone did not establish group-unit removability (F01), and that its compact-extab
size omitted the empty descriptor-list terminator (F02). V7 and its rejected exact-text
proposal/receipts retain their historical identity; v8 corrects both classes under a new name.
That review approves neither v8 nor any gate text.

### ARM specification and image evidence

- [ARM DUI0474K §4.12](https://documentation-service.arm.com/static/5ea19ea09931941038de96d2)
  runs inlining before unused-section elimination; §4.3 removes whole unreachable sections
  and preserves roots/used sections. §3.7 separately says exception tables are silently
  discarded if no code throws. That last rule is **not** implemented by v8.
- [EHABI32, “The object producer interface — Sections” and “Index table entries”](https://github.com/ARM-software/abi-aa/blob/main/ehabi32/ehabi32.rst)
  specifies one index fragment per code section, `SHT_ARM_EXIDX`, `SHF_LINK_ORDER`,
  `sh_link` to that code, and the two-word PREL31/function plus compact/extab/CANTUNWIND
  format. The cited 2025Q4 text retains the relevant pre-2017 section/table format.
- [EHABI32, “Personality routine exception-handling table entries”](https://github.com/ARM-software/abi-aa/blob/main/ehabi32/ehabi32.rst#personality-routine-exception-handling-table-entries)
  counts only the additional unwind words in personalities 1/2, then separately requires the
  descriptor list's terminating zero word. [LLVM's ARM ELF producer](https://github.com/llvm/llvm-project/blob/main/llvm/lib/Target/ARM/MCTargetDesc/ARMELFStreamer.cpp)
  also emits the zero when no handler data is supplied.
- [ELF gABI, section groups](https://gabi.xinuos.com/elf/03-sheader.html#section-groups)
  requires group sections to be included or omitted as a unit. A kept member of an affected
  COMDAT group/copy therefore prevents the new unwind-elimination exception.
- [ELF gABI, SHF_LINK_ORDER](https://gabi.xinuos.com/elf/03-sheader.html#section-flags)
  explicitly says linked metadata and its referenced section are retained/discarded together,
  and metadata relocations back to that section are not evidence of liveness.
  [INFERENCE] Applying these documented semantics to armlink means an eliminated function's
  exclusive exidx fragment, then exclusively reached extab, dies with it. The supplied public
  toolchain has no armlink; no fresh native-armlink execution is claimed.
- `research/linker/exidx_census.py` reports addresses/counts only, plus optional candidate ELF
  metadata (`--object FILE`). The original is a flat `code.bin`, not an ELF: it has no section
  headers or original exidx bounds. Under this branch's pinned image and catalogue, the RO-data
  segment has one word that could be a PREL31 text key, but its following word is not an EHABI
  second word; no plausible non-code index entry targets a catalogued function/entry start.
  Loose text/data pointer coincidences are reported, not mislabeled as an authenticated table.
  [INFERENCE] No original EHABI unwind-index table is identified; its absence and the original
  exception-link flags cannot be proved solely from this sectionless image. In particular,
  candidate C++ exidx emission is not evidence that the final image contains those entries.
  V8 relies on complete candidate/group ownership and absence of required original placement, not
  on treating this inference as a blanket permission to erase original/kept-function tables.

Synthetic and real-compiler coverage is `RemovedUnwindModelTest` in `tests/test_inplace.py`,
including the sealed counterexamples, all group copies/orderings, unrepresented allocated
members and complete/truncated compact entries. New evidence, copied-source smoke receipts,
frozen-model comparisons and exact exits are under `scratch/exidx/v8/`; v7 evidence is
unchanged. No author source/receipt or `docs/GATES.md` is modified. Independent composite
review and an explicit gate revision after the active frozen checkpoint closes are required
before adoption; a private MATCH does not grant credit.

## Sources

[DUI0474K] *ARM Compiler v5.05 armlink User Guide* (ARM DUI0474K, 2014; its history lists issue J
as the v5.04 release), PDF: <https://documentation-service.arm.com/static/5ea19ea09931941038de96d2>.
Online index of the same chapter: <https://developer.arm.com/documentation/dui0474/k/linker-optimization-features>
(renders client-side; the text below was read from the PDF).

| Section | Rule (quoted or closely paraphrased) |
|---|---|
| 4.12 Function inlining with the linker | "The linker can inline small functions in place of a branch instruction to that function … the function (without the return instruction) must fit in the four bytes of the branch instruction." It "replaces the function call with the instruction from the function that is being called", before unused-section elimination. |
| 4.3 Elimination of unused sections | Retains sections containing entry points, referenced sections, first/last sections and `--keep` sections. A section defining armcc's `__tagsym$$used` marker (`__attribute__((used))`) is not removed. Inlining every reference is therefore necessary but not sufficient for R1. |
| 4.13 Factors that influence function inlining | Never inlines "instructions that read or write to the PC". Table 4-1: ARM caller → ARM callee of **4 to 8 bytes**; ARM → Thumb and Thumb → Thumb 2 to 6 bytes ("not possible to inline from ARM to Thumb"). The last instruction must be `MOV pc, lr` or `BX lr`. "A function that consists only of a return sequence can be inlined as a NOP." A conditional ARM instruction can be inlined only if the BL's condition matches it, or the BL or the instruction is unconditional; an instruction that cannot be conditional is not inlined under a conditional BL. |
| 4.14 About branches that optimize to a NOP; 12.18 `--branchnop` | "By default, the linker replaces any branch with a relocation that resolves to the next instruction with a NOP instruction." Default `--branchnop`. |
| 4.15/4.16 Linker reordering of tail calling sections; 12.153 `--tailreorder` | A section ending in a branch to the start of another section may be placed immediately before it, and the branch then becomes a NOP. One tail-calling section per target, same execution region. Default `--no_tailreorder`. |
| 12.75 `--inline`, 12.76 `--inline_type` | Default `--no_inline` (inlining still applies to the ARM C library); `--inline` = `--inline_type=all`. |

The manual does not say which NOP encoding armlink writes; the image does (below).

## Retained-callee rules (v2, v3, v4 and v6)

Applied to relocated ARM branches (`R_ARM_CALL`, `R_ARM_JUMP24`, `R_ARM_PC24`) in a candidate
object whose destination is ARM code. Nothing else is rewritten.

- **I1 — empty callee.** BL or BL*cond* to a function whose whole section is `BX lr`/`MOV pc, lr`
  is written as **`MOV r0, r0` (0xE1A00000), unconditional even for BL*cond***.
- **I2 — one-instruction callee.** BL to an 8-byte function `insn; BX lr` is written as `insn`,
  unless `insn` reads or writes the PC (manual), is itself armlink's `MOV r0, r0` (see below), or
  — refused because the image cannot tell either way — touches LR, transfers control or
  generates an exception (`SVC`, `BKPT`).
- **I3 — conditions (manual, not observable in the image).** Unconditional BL: `insn` as is.
  BL*cond* with an unconditional `insn`: `insn` with the BL's condition. Equal conditions: as is.
  Different conditions, or an unconditional-only (`cond = 0xF`) `insn` under BL*cond*: kept.
- **Retained-callee identity: decided from the original image only.** The callee is the original function
  at the call destination, eligible only when the catalogue makes it unambiguous: an ordinary
  top-level `kind: function`, ARM, size 4 or 8, all code (no pool or literal), no entry but its
  start. I1–I3 apply to its original bytes (`InPlaceLinker.eligible_original_callee`). A section
  that starts with `insn; BX lr` cannot extend past that extent: only literal data could follow the
  return, and loading it reads the PC, which is never inlined. Interior entries, `multi_entry`
  owners, veneers and Thumb functions are never inlined (the call stays a BL and mismatches if the
  original inlined it).
- **Retained callee defined in the candidate build.** armlink would judge the candidate's own section, so
  it must be the original function: if either its section or the original would be inlined, the
  local section (whole, relocation-free, starting at the destination) must equal the eligible
  original body, otherwise the link is **refused** (`LinkError: armlink inlining is undecidable`).
  Retention metadata (`used`, KEEP, entry and other roots) never waives these checks or proves
  that calls stay BLs: section survival and call inlining are separate decisions. A matching
  rooted helper remains placed at its catalogued address, is compared as that original function,
  and its calls follow I1–I3. `tools/build.py` links the whole object before it reads the ledger's
  function list, so a ledger crediting only the caller is refused on the same identity failures.
  If neither would be inlined the call is an ordinary relocated BL, so a wrong helper stays an
  ordinary mismatch.
- **B1 — branch to the next instruction.** An unconditional relocated `B` whose destination is
  `place + 4` in the final (original) layout is written as `MOV r0, r0`. A relocated BL or
  B*cond* to the next instruction raises `LinkError` (armlink's output for it is not established;
  no instance exists in the image).
- **Not rewritten.** B tail calls are never inlined (manual: calls; image: 42 tail calls to empty
  functions survive). Thumb callers and callees are not modelled: the image has no Thumb function
  of at most 6 bytes ending in a return, so no Thumb inlining can have happened.

Why `MOV r0, r0` is never inlinable: armcc writes NOP as the v6K hint `0xE320F000` (`__nop()`
compiles to it, and every one of the 8 NOP words inside the 146 author-matched functions is
reproduced by armcc), and no author-matched function contains `MOV r0, r0`. So a callee that
begins with `MOV r0, r0` is a section that was a tail-call `B` when inlining ran (inlining happens
before layout; `--branchnop` needs final addresses) — a PC-writing instruction, never inlined. The
image agrees: all 8 surviving BLs to `MOV r0, r0; BX lr` functions (0x125b50, 0x13714c, 0x144220,
0x18f7ec, 0x1c5b84) were kept, while no BL survives to any callee rule I1/I2 inlines.

## Candidate-only removed callees (v3, v4 and v6, rule R1)

The original-only identity rule cannot describe every link. [DUI0474K §4.12](https://documentation-service.arm.com/static/5ea19ea09931941038de96d2)
says inlining runs **before unused-section elimination**: a callee inlined at every reference
need not survive in the executable. V3 admits this bounded, unrooted case, not a replacement for
any catalogued function and not general dead-code removal:

1. The complete 4/8-byte ARM callee section must be defined in the candidate build. A declaration
   of an absent external function is not evidence. `tools/build.py` combines candidate objects
   while preserving local symbols and per-unit ownership; `tools/match.py --extra-source FILE`
   supplies a separate helper translation unit under the same selected compiler profile.
2. The section has **no original address binding**, including aliases. A reserved `func_XXXXXXXX`
   name without a catalogue entry is still refused. Any binding overlapping a catalogued
   function/entry/extent takes the retained-callee path, with every v2 equality/refusal intact;
   an uncatalogued bound address is also refused, rather than treated as absent.
   Under v6, all copies' bindings are inspected before removal. A COMDAT member mixing a
   bound copy and an R1-removable copy is refused, rather than inferring a binding or giving
   the other copy a null-target rewrite. Every supplied copy must be eligible and consistent
   before that member can be removed.
   It must also be unrooted: sections defining `__tagsym$$used` or another uninterpreted tag,
   ELF ARM entry/retain-marked sections, explicitly kept/first/last/entry/exported sections and
   retained COMDAT groups cannot use R1. `ObjectFile` preserves these roots, conservatively
   retaining explicit exported visibility, OS/processor flags and unknown linker directives or
   ELF entry metadata. The linker API's `retention_roots` maps supplied symbol/section globs to
   root reasons; unmatched selectors fail closed. Raw linker scripts/options are not accepted
   as an unexamined promise of removability. An unbound rooted helper is refused, not dropped.
   A bound rooted definition retains all v2 identity/size/equality checks and I1–I3 decisions;
   retention keeps its section, not necessarily the BL.
3. The section is executable, word-aligned, wholly code, non-Thumb and non-NOBITS, with all
   function entries at offset zero spanning the whole section. Frozen v3 and v4
   require each symbol's recorded size to equal that extent. Only v6 permits the proven
   identical zero-size entry aliases above to share a full-sized sibling's extent.
   No relocation is allowed except the nonreferencing
   `R_ARM_V4BX` marker. I1/I2 judge its own generated bytes, with all existing
   exclusions (PC, LR, control transfer, exceptions, armlink's branch NOP) unchanged. In addition,
   an I2 helper may read only r0–r3/flags and write only r0–r3/r12/flags: it cannot change a
   callee-saved register or the stack. Conflicting candidate COMDAT copies are refused.
4. Each reference must be a real relocated ARM `BL`/`BLcond` at a catalogued ARM code site,
   exactly to the section entry (no interior/addended target), eligible under the unchanged
   I1–I3 encodings. Call-site data-flow is checked against **original** catalogued code:
   each input must be defined within the preceding four straight-line words on **every**
   incoming CFG path, be the caller's own incoming argument register unmodified on every
   entry-to-site path, or be an unmodified r0/r1 result of an unconditional preceding call on
   every path. A branch cannot silently bypass setup; calls break entry-argument provenance
   but establish possible result registers. Conditional definitions/calls alone do not prove
   setup. Reviewers must establish the preceding call's actual result type and width.
   After the site, **all CFG successors** (both conditional arms and branch targets) are followed:
   r1–r3/r12 and flags cannot be read before an unconditional definition, until a call/return or
   code-range end. Conditional definitions do not kill values on all paths. Unknown indirect
   successors fail closed; r0 may be a result, and I1 requires no inputs. The post-site clobber
   check remains mandatory for entry-argument forwarding. This bounds ABI consistency in the
   decoded CFG, not historical helper identity, argument meanings or original source provenance.
   V3 keeps its frozen CFG behavior. Only explicitly selected v4 or v6 enables the bounded-switch
   policy described below; v4's range/return refinements are not applied to v3.
5. After inlining, **every** reference to the section in every supplied allocated section must
   be gone, even in an otherwise unplaced section. A function pointer, `B` tail call,
   `R_ARM_NONE` dependency, incompatible I3 condition or non-ABI site raises `LinkError`; the
   linker cannot silently retain an unplaceable callee or erase a live reference. There must be
   at least one inlined call to each removed entity (identical COMDAT copies count together).
   Only then is the helper section dropped; it earns no catalogue credit.

Each rewrite retains `inline_insn`/`inline_nop` and has `target: null`, **not an invented image
address**, plus `removed_callee` (symbols, section, size, defining candidate object/source and,
when applicable, the COMDAT group signature).
Match JSON and build receipt unit rows also list `removed_callees`; the defining source and
object hashes remain bound even for a helper-only ledger unit with an empty `functions` list.
Prepare's `linker_rewrites` includes new/revised callers and **unchanged credited callers whose
removed helper's defining unit or shared-header contract changed**. `dependency_reviews` names
each affected caller/source and its changed helper/header dependencies. These callers require
explicit review rows even if the helper unit has `functions: []`; an empty PASS review is refused.
Every reviewed R1 caller needs QUALIFIED identity provenance (or FAIL), including unchanged ones.
Dependencies include every removed COMDAT copy's defining unit, not just the selected copy.
The review schema and finalize both enforce coverage. `argcheck` does not fabricate an original
edge to a missing address: reviewers must inspect the helper's prototype, arguments and use.
Prepare records added, changed and removed project headers as explicit G1 revisions, including
in non-R1 checkpoints. R1 dependency expansion is empty when no R1 caller is affected; ordinary
no-R1 PASS review rows remain valid. This symmetric header-revision selection is an intentional
behavior change, not a claim of complete no-R1 prepare equivalence (re-review F09).

**Abuse bound and qualification.** An author could invent a source helper to emit an instruction.
This model permits at most one non-PC, non-LR, non-control-transfer/non-exception instruction
(or an empty-call NOP) at a compiler-produced relocated call site with the checked call-ABI
data-flow. The emitted instruction must still match all original caller bytes; the section must
vanish without any live references, and cannot replace any catalogued callee. These restrictions
do **not** prove that the original source had this helper, its identity, or its semantics. R1
credit therefore requires a visible `[INFERENCE]` source note and explicit reviewer qualification,
not a claim of recovered original callee identity.
Reviewers must additionally check every incoming path and the actual forwarded entry contract,
setup dominance, outgoing successors/exits and implicit arguments at later calls or tail exits,
predicate/flag dependencies, helper definitions and every declaration/shared-header use, argument
types/widths/meanings, mutability/const contracts, memory effects, return/result semantics and
preserved original bugs. A preceding BL alone does not establish that r0/r1 hold results: its
actual return type and register width require evidence. The filter does not prove these typed
contracts or interprocedural liveness.

Re-review F03 (`scratch/review/linker-v3-r2/REVIEW.md`) demonstrates why these checks are
mandatory: nine real-compile alternatives (entry-forwarded store, preceding scalar result and
preceding two-word result, each under `game`, `sdk` and `middleware`) exactly matched controlled
reference callers whose source **never called the proposed helper**. Exact bytes, a clean CFG
filter and an argcheck report without issues therefore do not establish historical helper
identity. The `[INFERENCE]` note and explicit QUALIFIED provenance remain required, not waived
by successful linking.

### Removed-callee shape census

`research/linker/removed_callee_census.py` emits counts and addresses only. It enumerates I2
instruction shapes with at least one r0–r3 input, the R1 helper ABI and call-context filter,
excluding every word any eligible original callee can produce under I3. It does not classify
every surviving arithmetic/store instruction as an actual removed call.

Historical, pre-review snapshot on 2026-10-17 (the live catalogue/author inputs were not frozen;
these are **not a current reproducible count**, and the strengthened CFG filter may change them):
**5,443 possible sites in 1,753 functions**. The literal, case-insensitive `inlin` search in
mismatch rows of 151 author `RESULT.json` files finds 131 rows / 130 distinct functions;
**438 census sites in 82 functions** overlap those notes.
The substring deliberately also includes compiler-inline/forceinline discussion, so this is a
triage intersection, not a count of proven linker failures. `func_001ab680` has 22 shape sites,
including both reported setter sites **0x001abc28** and **0x001abd3c**.

Reproduce from a linked worktree (exit 0 for the measurement above):

```
/home/michael/code/kanken-training-2-decomp/.venv/bin/python research/linker/removed_callee_census.py \
  --authors /home/michael/code/kanken-training-2-decomp/scratch/authors
```


## Binary evidence

Reproduce every number: `.venv/bin/python research/linker/nop_census.py` (catalogue v3, counts
and addresses only). The regression test `tests/test_inplace.py::OriginalImageEvidenceTest`
asserts the key invariant.

| Observation | Count | Reading |
|---|---:|---|
| ARM BL/BL*cond* sites in catalogued code | 19,681 | — |
| … targeting a callee I1 inlines (74 return-only functions) | **0** | all such calls were inlined; the 74 are otherwise reached by pointer only (70), B only (2), both (1), or nothing (0x1f4fac) |
| … targeting a callee I2 inlines (36 one-instruction functions) | **0** | same |
| … targeting 8-byte callees the rules keep: `ldr r0, [pc, …]` loaders | 60 (to 12 of 20 such callees) | PC-relative: not inlined (manual) |
| … targeting `MOV r0, r0; BX lr` callees | 8 (to 5 of 8 such callees) | removed tail-call B: not inlined |
| B (tail calls) to the 74 return-only functions | 42 | B is never inlined |
| `MOV<cond> r0, r0` with a condition other than AL | 0 | NOPs from BL*cond* are unconditional |
| B/B*cond* to the next instruction surviving in catalogued code | 10 | all intra-function compiler branches (no relocation: 0x138f84, 0x13947c, 0x1436e8, 0x1455ec, 0x14a48c, 0x153e34, 0x153f08, 0x164650, 0x17e190, 0x1f4868); `--branchnop` only touches relocated branches |

Data-flow test (`as_if_call` in the census): treat the NOP word as a removed BL and scan the
following straight-line code; a read of r1–r3, r12 or the flags before they are written is
impossible after a call (AAPCS caller-saved).

### Where the 776 `MOV r0, r0` words come from

| Position (relative to LR save and catalogue starts) | Words | As-if-call test | Rule |
|---|---:|---|---|
| inside a frame (LR saved): call sites | **505** | 505 consistent, 0 violations; 29 directly follow a compare whose flags nothing reads (a `cmp; blne empty` left behind) | I1 |
| 4-byte NOP entries (v3 `nop_entry`, next word is an entry) | 28 | 12 read pass-through arguments | B1 |
| last word before an entry | 78 | 40 read arguments | B1 |
| first word of a catalogued start, next word not an entry ("leading `mov r0, r0`") | 55 | 14 read arguments | B1 |
| inside a leaf (no LR saved) | 55 | 21 read arguments | B1 |
| after an epilogue (`pop {…, lr}`/`vpop`), incl. 2 before a final `bx lr` | 23 | 10 read arguments | B1 |
| before a prologue (`push {…, lr}`) | 20 | 11 read arguments | B1 |
| outside every catalogued extent, each falling into a start (e.g. 0x186eec; C++ this-adjusting thunks `add/sub r0, r0, #k` + NOP) | 11 | — | B1 |
| unexecuted slot after `add pc, pc, ip, lsl #2` in a jump table (0x1455e0, assembler idiom) | 1 | — | not linker |
| **total** | **776** | | I1 505, B1 270, other 1 |

The tail-call positions read argument registers they never wrote (the arguments pass through to
the tail-called function), which a removed BL would forbid; the 505 call sites never do.
[INFERENCE] The split between "call site" and "tail call" uses a linear scan for the LR save; the
data-flow results are the independent check.

### The 6,753 `NOP` (0xE320F000) words

6,752 in catalogued ARM code, 1 in an uncatalogued gap at 0x110cec. None is explained by a linker
rule (armlink writes `MOV r0, r0`). 3,162 are followed by a read of flags set before them
(`cmp; nop; bne`), so they cannot be removed calls; armcc emits them as scheduling NOPs (authors
reproduce them from plain C, e.g. func_001b6ec4's 15 NOPs). [INFERENCE] The remaining 3,590 are
compiler NOPs too; nothing distinguishes them, and no rule needs them.

### `--tailreorder` or source order?

Both produce the same bytes; the model needs only the final layout. The wrapper and its target
must be separate sections that armcc did not merge: in one translation unit `-O3` inlines the
target into the wrapper (observed for all 8 wrapper/body pairs tried below), so the original pairs
were in different units placed adjacently — consistent with `--tailreorder` [INFERENCE].

## V3 independent-review fixes (branch `linker-v3-r2`, 2026-10-17)

The pre-adoption F01 blocker is fixed: the reviewer's exact game-profile `used` helper is
compiled in a regression and refused, not removed. Explicit KEEP/entry/exported/first/last roots,
ELF entry/retain flags, unknown retention metadata and retained COMDAT copies are covered.
The initial R2 shortcut for bound rooted candidates was rejected by re-review F08 and is removed
by the F08 revision below. Bound rooted definitions now follow every v2 identity/equality rule
and I1–I3 decision while retaining their section. V2 remains the default.

F02 now closes helper-only review dependencies, including identical COMDAT copies and shared
header contracts. Prepare exposes unchanged dependent callers and their rewrite metadata;
review/finalize refuse missing rows or unqualified R1 provenance. Synthetic build tests keep
helper-only source/object hashes and removed metadata without granting helper credit.

F03 follows every decoded CFG successor and checks all incoming paths. It accepts unchanged
entry arguments and unmodified r0/r1 results of preceding unconditional calls, while preserving
the post-site clobber check. The reviewer's branch-successor/skipped-setup probes are negative
tests; entry-forwarding and prior-call-result cases are positive tests. Historical identity,
actual result widths and source-level contracts remain mandatory manual qualifications.

Historical R2 verification before re-review exposed F08 (not evidence for the F08 revision):

```
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -m unittest \
  tests.test_inplace tests.test_match tests.test_compid tests.test_argcheck \
  tests.test_accept tests.test_build
```

**Exit 0; 166 tests; OK; no skips.**

```
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/test_suite.py
```

**Exit 0; `574 tests, 1 skipped, 0 failed modules, 30.3s`.**

The five accepted functions listed in the historical table below were re-matched with the final
v2 and v3 implementations: **all ten commands exit 0**, complete extents MATCH without rewrites.
Invocation pattern (replace source/function/model using that table):

```
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/match.py SOURCE \
  --profile game --linker-model armlink5-inline-branchnop-v3 --show 0 \
  --out scratch/integrator/linker-v3-r2/accepted-final/v3/FUNCTION
```

The same pattern with v2 was run separately. Exact commands/exits are in
`scratch/integrator/linker-v3-r2/final-match-commands.json`.

Additional reported false-negative contexts were exercised without changing author sources:

- w16b `func_00112628`, `func_00112b5c`, `func_001131d4`, each with separate
  `linear_buffer_allocator.c`, `--profile game --linker-model armlink5-inline-branchnop-v3
  --show 0`: each context now permits one `inline_insn` and removes the 8-byte helper.
  **All three still exit 1 / MISMATCH**, 36/36 bytes, first difference **+0x4**.
  Results: `scratch/integrator/linker-v3-r2/w16b/`; commands:
  `scratch/integrator/linker-v3-r2/match-commands.json`.
- w16a `variants/func_0010be58_removed.c` with separate `variants/sdk_result_failure_bits.c`
  and the same options: the preceding-call result is accepted, with one `inline_insn` and
  the 8-byte helper removed. **Exit 1 / MISMATCH**, 60/60 bytes, first difference **+0x2f**.
  Results: `scratch/integrator/linker-v3-r2/w16a/func_0010be58/`; command recorded in
  `final-match-commands.json`.

These four author variants earn **no new matching-C credit**. No checkpoint prepare/finalize
or full-ledger build was run for this revision; the full suite above was explicitly requested.

### F08 rooted-bound safeguard revision

The rooted-section early return in `_inline_decision` is removed. A candidate-defined callee
bound to an original function can no longer force an ordinary BL merely by declaring `used` or
another retention root. It follows the unchanged v2 original-body eligibility and whole-section
equality/refusal rules. A matching rooted helper remains catalogued and compared at its original
address; it is not an R1 removed entity.

`RemovedCalleeModelTest` now includes the independent reviewer's real game-profile reproduction:
a 24-byte caller, an 8-byte `used` candidate setter and a bound 12-byte original setter that also
writes the next field. V2 and v3 must both refuse before caller-only credit could be selected,
with no rewrites/removals. The positive compiled fixture binds the rooted 8-byte setter to its
identical 8-byte original, compares both caller and retained helper, and records an ordinary
address-targeted I2 rewrite rather than `target: null`. The synthetic rooted positive also
requires identical v2/v3 behavior.

F03 remains a mandatory reviewer qualification, not an implementation blocker discharged by
this fix. The proposed GATES text incorporates re-review F09's explicit non-R1 header-revision
behavior. The reviewer's inherited F10 source/header input drift is outside this change; no
passing `accept.py verify` or acceptance adoption is claimed. A passing independent re-review
and the normal fresh G4/prepare/finalize model binding remain prerequisites.

Requested post-F08 verification (all commands used this worktree and the repository venv):

```
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/test_suite.py
```

**Exit 0; `576 tests, 1 skipped, 0 failed modules, 30.9s`.** The inplace module ran
**74 tests, no skips**, including both new real-compile rooted-bound regressions.

All five accepted-function regressions used `--profile game --show 0` and each explicitly
selected linker model. Every result matches its complete extent, with no rewrites or removals:

| Function | Bytes | v2 exit / result | v3 exit / result |
|---|---:|---|---|
| `func_00105084` | 64 | 0 / MATCH | 0 / MATCH |
| `func_0010d604` | 64 | 0 / MATCH | 0 / MATCH |
| `func_0011e928` | 72 | 0 / MATCH | 0 / MATCH |
| `func_0018fdd4` | 40 | 0 / MATCH | 0 / MATCH |
| `func_00195254` | 224 | 0 / MATCH | 0 / MATCH |

Sources are listed in the historical regression table below; current result files are under
`scratch/integrator/linker-v3-f08/accepted/{v2,v3}/FUNCTION/`. Exact commands, individual exits
and the suite summary are recorded in `scratch/integrator/linker-v3-f08/verification.json`.

The requested w16d regression used the unchanged caller
`/home/michael/code/kanken-training-2-decomp/scratch/authors/w16d/v3/func_001456e4.c` and
`--extra-source /home/michael/code/kanken-training-2-decomp/scratch/authors/w16d/v3/display_metadata.c`,
with `--profile game --linker-model armlink5-inline-branchnop-v3 --show 0`. **Exit 0;
MATCH 240/240 bytes**, one `inline_insn` at **+0x7c**, and the 8-byte `w16_set_screen` removed.
The rewrite retains `target: null`. Results are under
`scratch/integrator/linker-v3-f08/w16d/func_001456e4/`; this opt-in match is not accepted credit.

## Jump tables in the call-context CFG (v4 and v6)

The `r1-jumptables` extension resolves two bounded ARM compiler switch idioms inside R1's
**original-code CFG** only under explicitly selected **`armlink5-inline-branchnop-v4`** or **`armlink5-inline-branchnop-v6`**.
Frozen strict/v2/v3 policies retain their exact prior behavior, including v3's original caller
decoding and return classification. It does not add a helper shape, a rewriting rule or relaxed
byte comparison. Every R1 retention, retained-callee equality, reference-elimination,
source/object binding and QUALIFIED provenance requirement remains unchanged.

The first patch (`21c2bd4`) incorrectly broadened the frozen v3 name and did not prove data
endianness for LDR targets; its sealed independent review
`scratch/review/r1-jumptables/REVIEW.md` is **FAIL** (F01/F02). Those reported reproductions are
ground truth, not checks to rerun on unchanged code. The next patch (`e72fb34`) correctly
separated v4 and fixed SETEND, but its sealed
`scratch/review/r1-jumptables-r2/REVIEW.md` is **FAIL**: F03 exposes omitted LDREXD/MRRC writes;
F04 exposes unmodeled RFE PC/CPSR restoration. The r3 correction replaces permissive access
metadata and a state-changing mnemonic denylist with explicit class/encoding effect models.
Those corrections preceded v4 adoption; strict/v2/v3 remain frozen.

The independent implementation/exact-text re-review passed with qualifications
(`scratch/review/r1-jumptables-r3/REVIEW.md` / `SEAL.json`). The later composite review
explicitly updates its adoption boundary to finalized `c008-r3`; gates v1.17 adopt v4,
then v1.18 admit v6 without weakening this proof. Selection, both fresh receipts, prepare
and history bind the chosen identifier; transitions reopen all affected R1 callers.
No old checkpoint or evidence is reinterpreted.

### Exact recognized patterns

Let `A` be the original address of the PC-writing instruction and `rM` a non-PC register:

| Form | Guarded instruction | Each reachable slot at `A + 8 + 4*i` |
|---|---|---|
| Branch table | `ADDLS pc, pc, rM, LSL #2` | an unconditional direct ARM `B` to decoded caller code |
| Address table | `LDRLS pc, [pc, rM, LSL #2]` | a complete aligned absolute-address word in this caller's catalogued pool/literal data |

The unsigned **LO** forms are also recognized. An unconditional immediate `CMP rM, #N` must
guard the same index: **LS enumerates indices 0 through N; LO enumerates 0 through N−1**.
The immediate is decoded with ARM's rotate encoding, not inferred from apparent table length.
There must be at least one entry and every required slot must be present. No unguarded table,
register bound, signed/other predicate, different base, alternate scale, writeback, subtractive
index or arbitrary computed PC write is resolved.

Between the compare and the writer, every word must be contiguous, modeled ordinary caller
code, with no control transfer or possible index/NZCV write. Conditional writes are possible
writes. `_arm_switch_effects` admits only an explicit instruction-ID allowlist **and validated
A32 encoding**; it derives core destinations, load pairs/register lists, base writeback and
NZCV changes from instruction fields, never from `regs_access`, `update_flags` or decoder
control-flow groups. An absent write list is not preservation evidence. Generic coprocessor/
exclusive instructions and all other unmodeled classes are refused rather than presumed safe.
No catalogue entry, direct B/BL/BLX target or target of any otherwise-resolvable table may enter
after the compare through the writer. Re-entry at the compare repeats the bound and is allowed.
Any other reachable unresolved transfer still refuses the complete caller.

Only original catalogue code ranges clipped to the owning function's complete extent supply
instructions; pool/literal bytes never do. Address-table words likewise come only from complete
aligned words of that owner's pool/literal ranges clipped to its extent, never from image
search, candidate relocations, neighboring functions or arbitrary memory reads. Every address
target and every branch-table entry's destination must be a decoded aligned ARM instruction of
this caller. Thumb, out-of-extent, data and undecoded targets are refused.

For LDR tables, data interpretation must be little-endian on **every** incoming path, not just
after a stable compare. The target's public ARM ABI uses LE data at entries and conforming call
returns. Before resolving **either** table form, every decoded owner instruction must pass the
same explicit class/encoding effect model—even unreachable instructions. This is an allowlist,
not a claim that a list of suspicious mnemonic prefixes exhausts state-restoring operations.

Modeled classes are ordinary integer data processing with immediate/immediate-shift operands;
validated scalar, double and register-list loads/stores with all destinations/writeback;
ordinary short/long multiply; scalar VFP data/memory operations, explicitly decoded VMOV core
transfers and FPSCR-only VMRS (including its FMSTAT alias); the actual compiler NOP; and branch/call/return
encodings. Unsupported subencodings, reserved/unpredictable combinations and unknown/new
decoder classes are refused. CPU core/NZCV effects are modeled separately from VFP register/
FPSCR effects. VMRS-to-APSR/FMSTAT sets NZCV; ordinary VFP data operations do not. No modeled ordinary
instruction restores CPSR.E; calls retain the manual LE ABI-return obligation.

LDREXD (including predicated forms), generic MRRC, status/system writes, all RFE variants and
other exception returns are unmodeled: their effects return refusal, and v4 flow is unknown
even when the decoder reports neither writes nor control groups. A known LDRD/LDM or VMOV
pair cannot hide an index or base write: both destinations and writeback are decoded directly.
This owner-wide rule also covers instructions before the guard, on one predecessor or
unreachable. Conservative false negatives are intentional; no interprocedural state recovery
or restored-LE guess is claimed.

The not-taken successor at `A + 4` must be an unconditional direct branch to decoded caller code
or a recognized ordinary return, so it cannot accidentally fall into an address table. Returns
are restricted to `BX lr`, plain `MOV pc, lr`, and `LDMIA sp!, {..., pc}`/`POP`; merely reading LR
while writing PC, a generic decoder return group, exception-return encodings or an arbitrary PC
load is insufficient. Predicated ordinary returns retain both exit/fall-through behavior outside
this unconditional default-slot requirement.

The CFG includes the default successor and every bounded table successor. For branch tables it
visits each entry's B and then its destination; duplicate address targets may share a successor
without dropping a distinct path. The existing incoming argument-provenance and outgoing
register/flag-clobber walks use this complete graph, including case paths that do not contain the
proposed helper. A different case cannot bypass argument setup or hide a later clobbered read.
No source-specific address, function-name exception or forced-consistent context is permitted.

### Evidence and reviewer obligations

`tests/test_inplace.py` adds compiler-free `JumpTableContextTest` cases for both LO/LS table forms,
complete bounds, guard/index/flag stability, direct/entry/table guard bypass, every successor,
pool ownership, unknown computed transfers and escaping defaults/targets. Its compiled
`SwitchRemovedCalleeTest` exercises the public `InPlaceLinker.link` consumer with separate caller
and setter objects: it expects an address table under `game` and a branch table under the
existing `trial-504-82-O3-Ospace` profile. Positive cases require exact full caller
bytes, two `target: null` rewrites and the helper's defining-object provenance; frozen v3 must
still refuse these tables and v2 must refuse the unbound helper. Compiler-free
`BoundedSwitchModelTest` uses the public linker for ADD/LDR LO/LS selection and all reported
LDREXD/predicated-LDREXD/MRRC negatives, modeled load/pair/writeback index changes, and RFE
IA/DA/DB/IB with SP/r4 frames in the window, before the guard, on one predecessor and unreachable.
`BoundedSwitchEffectsTest` enumerates every decoder instruction ID: an unmodeled class fails
closed even without any access metadata; every ID also refuses an invalid encoding. Independent
encoding vectors check modeled destinations/NZCV with access metadata absent. Frozen/new model
payload and legacy classification checks remain. Match/build/compid selectors include v4;
acceptance tests bind both receipts/history and reopen unchanged R1 callers on v3→v4 transition.
New build receipts also bind `tools/linker_models.py` in their harness hashes, so the registry's
selected policy flags are input-bound; historical receipts are not rewritten. The census has
an explicit `--linker-model` selector and enables v4's CFG only for explicitly selected v4/v6.

Independent reviewers must re-derive the compare's decoded immediate, unsigned bound/count,
same-index and flag preservation, entry/dominance proof, every slot and the default edge from
the original catalogue ranges, including each intervening instruction's architectural core
writes, base writeback and NZCV effects even when decoder access sets are incomplete. They must
re-derive the LE entry/call contract and explicit exception-return/state-restoration refusals;
neither a missing write nor a missing flow group proves safety. Run the meta-test and public
positive/negative consumers, plus five accepted-function regressions under v2, frozen v3 and v4;
retain all existing typed-contract, implicit-argument, memory-effect, preserved-bug and historical
helper-identity qualifications. Neither a clean CFG nor exact bytes prove that the original
source contained the helper. Earlier forced-context evidence remains uncredited. This is an
emulated in-place link, **not an execution of armlink**.

### Effect-allowlisted v4 focused evidence (r3)

After the class/encoding models and public/meta regressions settled, the requested focused
smoke exited **0: 58 tests, OK**. It exercises the entire decoder-ID refusal policy, modeled
destinations/writeback/NZCV without decoder access metadata, all reported LDREXD/MRRC and RFE
public-consumer cases, ordinary compiled switches, frozen-model behavior and model/receipt
selectors. The first focused run safely refused the legitimate FPSCR-to-NZCV transfer because
Capstone names it FMSTAT rather than VMRS; that failed run and input hashes are retained.
An explicit FPSCR-only FMSTAT effect model fixed the cause without weakening any test.

The unchanged motivating sources, selected v4 and normal filter, were then exercised with
`tools/match.py ... --profile game --linker-model armlink5-inline-branchnop-v4 --show 0
--out scratch/out/r4052-v4-r3`: **exit 0; MATCH 2,212/2,212 B**, two `inline_insn` rewrites at
**+0x5a8 / +0x6bc**, both `target: null`, and one 8-byte `bind_content` removed. The receipt is
`scratch/out/r4052-v4-r3/func_001ab680_removed_setter.result.json`. Both source/object hashes and
all seven/six recorded transitive header hashes agree with the exact compiler-cache inputs.

Exact commands, both focused attempts, exits, input/receipt hashes and source closure are in
`scratch/integrator/r1-jumptables/r3/verification.json`; fifteen fresh accepted-function command
vectors and the Main full-suite command are in the adjacent `regression-commands.json`.
The worker ran no full suite/build, accepted-function regression, prepare/finalize or adoption.
Prior F03/F04 counterexamples were not rerun on unchanged old code. Independent
implementation/exact-text re-review was still required at that point; this is not armlink execution or credit.

### Historical failed-r2 focused smoke (`e72fb34`)

Before F03/F04 were corrected, its targeted smoke exited **0: 53 tests, OK**.
It includes public v4/frozen-v3 table consumers, endian-state negatives, compiled switch
fixtures, legacy ABI/ordinary-helper behavior, match/build/compid CLI/model payloads and
v3→v4 acceptance transition/receipt/history binding. It is not the full suite or a full build.
The exact command, output and exits are in
`scratch/integrator/r1-jumptables/r2/verification.json`.

The unchanged motivating caller and separate helper were exercised normally with:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/match.py \
  /home/michael/code/kanken-training-2-decomp/scratch/authors/r4052/src/func_001ab680_removed_setter.c \
  --extra-source /home/michael/code/kanken-training-2-decomp/scratch/authors/r4052/src/removed_content_setter.c \
  --profile game --linker-model armlink5-inline-branchnop-v4 --show 0 \
  --out scratch/out/r4052-v4
```

**Exit 0; MATCH 2,212/2,212 B**, two `inline_insn` rewrites at **+0x5a8 / +0x6bc** with
`target: null`, one 8-byte `bind_content` removed. Receipt:
`scratch/out/r4052-v4/func_001ab680_removed_setter.result.json`. The r2 evidence binds both
unchanged sources, both compiled objects and all seven/six recorded header dependency hashes,
which agree with the exact compiler-cache inputs. No override or armlink execution was used.
This is a private qualified candidate, not gate adoption or accepted credit.

Main subsequently supplied **661 tests, 1 skipped, zero failed modules**, plus all fifteen
exact accepted-function regressions for r2, as recorded in its sealed review. Those results
are historical evidence for `e72fb34`, not verification of r3. The r2 command vectors and
records remain unchanged under `scratch/integrator/r1-jumptables/r2/`. New r3 evidence/commands
use `scratch/integrator/r1-jumptables/r3/`; independent re-review and adoption remain required.

### Historical failed-revision r4052 measurement (`21c2bd4`)

Before F01/F02 were corrected, that patch ran this normal-filter command under its incorrectly
expanded v3 name. This is preserved historical evidence, **not current v3/v4 verification**:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/match.py \
  /home/michael/code/kanken-training-2-decomp/scratch/authors/r4052/src/func_001ab680_removed_setter.c \
  --extra-source /home/michael/code/kanken-training-2-decomp/scratch/authors/r4052/src/removed_content_setter.c \
  --profile game --linker-model armlink5-inline-branchnop-v3 --show 0 \
  --out scratch/out/r4052-v3
```

**Exit 0; MATCH 2,212/2,212 B**, two `inline_insn` rewrites at **+0x5a8** and **+0x6bc**, both
`target: null`; one 8-byte `bind_content` section removed. The caller and separate helper sources
were unchanged. Their source/object hashes and all compiler-recorded transitive dependency
hashes (respectively seven and six header dependencies) agree with the exact cache/object
inputs used by this match. The receipt is
`scratch/out/r4052-v3/func_001ab680_removed_setter.result.json`; command, exit, receipt hash,
source/header/object closure and implementation/configuration hashes are recorded in
`scratch/integrator/r1-jumptables/verification.json`. The pre-resume receipt was preserved
separately rather than treated as fresh evidence.

The editing worker did not run suites or accepted-function regressions for that patch. Main
subsequently reported **652 tests, 1 skipped, 0 failed modules**, and ten exact accepted-function
v2/v3 regressions; the sealed review records that attribution. Those checks apply only to the
failed revision, not the corrected v4 implementation. Prior command/evidence files are retained;
new command vectors and focused-smoke evidence live under
`scratch/integrator/r1-jumptables/r2/`. A private match still earns no accepted credit or
historical helper identity, and neither revision executes armlink.




## Historical V3 measurements (branch `linker-removed`, before independent review)

- Regression command:
  `/home/michael/code/kanken-training-2-decomp/.venv/bin/python -m unittest tests.test_inplace tests.test_match tests.test_compid tests.test_argcheck tests.test_accept`
  — **exit 0, 129 tests, no skips**. Includes two-site setter elimination, same-build separate
  objects, pointer/B/dependency/interior refusals, catalogue/alias collisions, PC/LR/SVC/NOP
  exclusions, I3, original call-ABI failures and conflicting removed COMDAT copies. The old F01
  probes still run through `ArmlinkModelTest`.
- Five accepted, call-bearing ledger functions were recompiled through **`tools/match.py`**, under
  both v2 and v3, using `--profile game --show 0` and separate scratch output directories. All ten
  commands **exit 0**, each complete extent remains MATCH, with no rewrites:

  | Function | Source | Bytes | Original call sites |
  |---|---|---:|---:|
  | `func_00105084` | `src/pilot/loop_resources.c` | 64 | 1 |
  | `func_0010d604` | `src/pilot/game_heap.cpp` | 64 | 1 |
  | `func_0011e928` | `src/wave1/func_0011e928.c` | 72 | 1 |
  | `func_0018fdd4` | `src/wave1/func_0018fdd4.cpp` | 40 | 1 |
  | `func_00195254` | `src/wave1/func_00195254.c` | 224 | 4 |

  Results: `scratch/integrator/linker-removed/accepted/{v2,v3}/func_XXXXXXXX/`.
  No `accept.py verify`, checkpoint prepare/finalize, full-suite or full-ledger build was run.
- **s1ab680 source-only setter retest:** `tools/match.py` on the preserved
  `scratch/authors/s1ab680/src/func_001ab680_removed_setter.c`, with
  `--extra-source scratch/authors/s1ab680/src/source_only_setter.c --profile game
  --linker-model armlink5-inline-branchnop-v3 --show 0`, **exits 2**, fail-closed.
  The separate setter is eligible (8 bytes), but this caller variant compiles to **2,232 bytes,
  not 2,212**. Its first call at +0x5a8 / 0x001abc28 has the correct replacement and passes the
  original call-context test. Its second call is at **+0x6d8 / 0x001abd58**, rather than the reported
  original site +0x6bc / 0x001abd3c; it fails argument-materialisation there and would not reproduce
  that original slot. No source was altered or target address fabricated to force a match.
  Objects: `scratch/integrator/linker-removed/s1ab680/v3/`; this is **not new matching-C credit**.

## Measurements (branch `armlink-inline`)

- **Accepted ledger:** `tools/build.py --name armlink-inline-v2-accepted-check` with the default
  model: PASS, 55/55 credited functions exact, `code.bin` and `.code` identical, no rewrites (v1:
  `armlink-inline-accepted-check`, same result).
- **F01 regression, real image:** func_001b6ec4 (below) with its two calls retargeted to a local
  `void *func_0013714c(void *e) { return e; }` (compiles to `bx lr`; the original 0x13714c is
  `MOV r0, r0; BX lr`), in a ledger crediting only func_001b6ec4: the v1 linker (c11a0ad) links it
  with 2 `inline_nop` and the caller equal to the original (the helper differs); v2
  `tools/build.py --matches scratch/armlink/f01-ledger.json` exits 2 with the refusal. The four
  review probes (`scratch/review/armlink-inline/probe_local_callee.py` cases, run against v2) are all
  refused; `tests/test_inplace.py::ArmlinkModelTest` reproduces each from a compiled object.
- **Author mismatches:** every `mismatch` row with a source in `scratch/authors/*/RESULT.json` (67
  rows, `research/linker/measure_authors.py`, sequential, both models; identical under v1 and v2). As written, **none newly matches**:
  only func_001852f4 changes (MISMATCH → EXTENT: the 4-byte wrapper reproduces the NOP; the v3
  extent is the 68-byte group). No other candidate calls a function the model inlines, and none
  ends in a tail call to the next function. 5 rows fail identically under both models: two v3
  `multi_entry` members (not a catalogued start), and three Wave2Author3 sources that no longer
  compile against the c003 `bss_0049ecf0.h`; with that header from 73b9cf8, 0x195ee0 and 0x19cd70
  still mismatch unchanged and 0x19bc7c stops at its uncatalogued callback 0x186eec (V4 item 1).
  (0x11fa34 and 0x11ffdc, recorded as mismatches, now match under both models: catalogue v3 added
  their target entry; unrelated to this model.)
- **func_001b6ec4** (integration case): as written it calls func_001ef880 (`MOV r0, r0; BX lr`),
  which armlink keeps as a BL, so it stays MISMATCH — correctly: the image keeps the BL to
  identically shaped 0x13714c in the same destructor-then-delete pattern (0x11f0ac). With the
  callee renamed to any catalogued return-only function it **MATCHES** (2 `inline_nop`). The
  genuine callee is probably 0x1ef884 (return-only, pointer at 0x223960, not catalogued: a v4 entry).
  An inlined call's target is not recoverable from the bytes.
- **NOP entries as C (catalogue v4 item 2):** with a scratch catalogue that splits each of the 28
  v3 `nop_entry` groups and the 55 leading-NOP starts at `address + 4`
  (`research/linker/nop_wrappers.py`), the C wrapper `void f(void) { g(); }` (g = the next
  function) **MATCHES for all 83** under the model (MISMATCH under `strict-v1`; EXTENT for the 28 under v3).
  For the leading-NOP author candidates (private sources, `scratch/armlink/variants/split/` on this
  worktree), moving the body to `address + 4` and adding the wrapper in a separate unit, against
  that split catalogue: 0x12fb38, 0x12fb48, 0x1228bc, 0x122a1c, 0x1229b8 and 0x13714c match completely
  (wrapper + body); 0x122984 and 0x1228d0 wrappers match, their bodies keep ordinary register
  allocation/scheduling differences.

## What a match under the model proves

For every ordinarily relocated field: the same target address. For a rewritten branch only:
- Retained-callee `inline_nop`/`inline_insn`: the original function at the call's destination is that return-only
  or one-instruction function (a body-equivalence class) — **not which function was called**, nor
  the original call graph; interface and semantic review stay necessary.
- V3 `removed_callee`: the candidate helper's I1/I2 instruction reproduces the caller site and
  all of its candidate references were eliminated; **neither an original address nor the original
  existence/identity of that helper is proved**. Its semantic/interface contract needs source review.
- `branchnop`: a function starts at the next instruction — the target symbol's address agrees with
  the layout, its identity is the C declaration's.
Reviewers see each rewrite in the match result, build receipt or compid record. These
qualifications, and the manual-only basis of I3 for non-empty callees (below), belong in every
checkpoint that credits a function through a rewrite (review F03).

## Catalogue recovery safeguards (candidate v9-r2)

`tools/analyze.py catalogue --out X` is the single candidate generator. For the
reviewed image it includes the immutable, image/extent/ISA-bound classification
records, reviewed UTF-16 object references, the section-authenticated `.init_array`
table and the immutable protected-checkpoint blob list in
`research/catalogue_v9_r2_evidence.json`, and recomputes the revision's
ownership, anomaly and pointer evidence. The output records the command, source
hashes and input hashes; neither output path nor elapsed time changes its bytes.
The former separate `research/catalogue_v8.py` generator has been replaced by
`tools/catalogue_review.py`; no active author-result directory is scanned.

Protection compares complete adopted owners **after classification**, not only
addresses and sizes before it. Accepted matches, existing c004-c008 candidate
files, extra `--protect-manifest` inputs, and the digest-pinned Git blob of the
unmerged `c008-integrate` candidate are included. The blob identity and content
hash are recorded and verified before use. A bundled classification conflicting
with a protected owner is explicitly withheld; an explicit conflicting
`--classifications` input fails. Newly observed callers/start evidence for a
protected owner are recorded in `protected_entry_observations` /
`protected_start_observations` instead of rewriting the accepted record.

Leading-prefix and pointer-prefix recovery allow benign argument-memory loads,
but refuse memory writes, writeback and atomic/exclusive reservation operations.
Explicit instruction-class exclusions cover SWP/STREX/LDREX variants even when
Capstone omits a memory operand's WRITE flag. In particular, `0x001f3e78` is a
nonstoring argument-memory-read prefix, not register-only setup.

The exact executed linker MOV-NOP may also delimit a bounded nonstoring leaf
wrapper from a complete two-instruction scalar field-read body. The read must
return in r0 from a nonindexed, non-writeback r0-based field and end in a plain LR
return or an unconditional tail transfer to an independently known returning ARM
owner. Predication, calls, stack/LR-dependent setup, longer arbitrary suffixes and
unknown/interior/noreturn tail targets are insufficient. Existing control/switch
crossing, literal/data crossing, saved-frame, interior-entry and complete-record
protection vetoes still apply. This recovers `0x001efcd0 | 0x001efcd8` and
`0x00131948 | 0x00131950` under the adopted erased-tail-branch interpretation;
it is not a general rule to split every leaf NOP.

Catalogue v9 adds four image-structural rules. **(1)** A bounded R0-input leaf CFG
(scalar loads, argument-object scalar stores through the same R0-R3/R12 inputs,
loops, intersected definite initialization at every join, plain returns, no calls,
pools, writeback, atomic or exclusive effects, and no exit from the proved suffix)
may follow an exact linker NOP after a nonstoring wrapper prefix
(`nonstoring_wrapper_field_access_graph`). **(2)** A single unconditional tail branch
after an independently proved frame restore may become a four-byte wrapper when its
direct target is a known returning ARM owner (`restored_frame_closed_tail`). The same
control/data/protected-record vetoes still apply. **(3)** A canonical AAPCS
interworking veneer (`add ip, pc, #odd` followed by `bx ip`) is recognized as code
only when its odd PC-relative target is mapped Thumb text; the veneer is not an ADR
string. **(4)** A reviewed record may type an address-taken literal as UTF-16LE
only for the exact owner/reference sites and only when a complete printable or
Unicode-whitespace halfword terminator is present; non-whitespace controls and
unterminated data fail closed. Ordinary untyped NUL-string inference is unchanged.

A complete address-taken NUL string shared by several owners is no longer left
unknown when it does not overlap owned code/pools or branch targets. Its **exact
terminated object** becomes unowned `rodata`; alignment bytes after it are claimed
only when they are all zero. This neither assigns the object to a function nor grows
any function extent.
V9-R01 fixes three typed shared objects at `0x001b54e4`, `0x001b8234` and
`0x001bc29c`: each complete UTF-16 record is four bytes, not one byte of an
empty narrow string. The natural r4023 wide-literal EXTENT witness is 128/124 B,
with no rewrites; its full object does not establish exclusive ownership.


A reviewed `.init_array` table is accepted only as a bounded, aligned, self-relative
32-bit table inside mapped rodata. Every entry must point to plausible code. The
original startup walker's table traversal and the compiler `.init_array` /
`R_ARM_TARGET1` emission are the section-ABI witness; this is not a general scan
for pointer-shaped words. Each target becomes an independent start before weak
interior prologue guesses are consolidated. Every prior entry address/ISA remains
present, and protected complete records remain identical. If a new initializer
start precedes a prior prologue-only owner, the old address is retained as a
`secondary_entry` with `legacy_prologue` evidence only when it is a valid ARM
instruction boundary and every old code/pool/literal span survives in the new
owner. `legacy_entry_preservation` records each exact mapping. The recovered
owner is conservatively `multi_entry`, not credited as C; the retained label is
not evidence of independently proved callability. A later baseline generation
retains that exact `legacy_prologue` secondary-label form only under the same
initializer owner, valid ARM boundary, no incoming sources and complete prior
owned-span preservation. It does not convert genuine called/pointer entries into
legacy guesses. Other entry losses fail closed (V9-R02).

Classification proofs retain their original provenance. Old ABI executions,
natural-caller matches, declaration rejections and the corrected no-frame compiler
grid are cited as retained evidence, not newly executed checks. SDK/archive or
handwritten ownership beyond demonstrated ABI constraints remains [INFERENCE].
Leading-input dependence is a may-read model at calls, not a recovered prototype.

## Residual unknowns

- armlink's output for a relocated BL or B*cond* to the next instruction (refused, none in image).
- I3 for non-empty callees under BL*cond*, and inlining of LR-reading, SVC/BKPT or SP-writing
  instructions: manual-only or unverified (no instance in the image); LR/SVC/BKPT are refused.
- Thumb inlining and Thumb `--branchnop` (`B.W`, `R_ARM_THM_JUMP24` is not supported by the
  in-place linker at all): not modelled; no qualifying Thumb callee exists.
- Whether `--tailreorder` or source order placed each wrapper (does not affect bytes).
- Catalogue v3 boundaries hide B1 sites: tail-call NOPs at 270 positions sit inside one catalogue
  extent with their target, so the C function ending in that tail call cannot be credited until
  catalogue v4 splits there (the 28 NOP entries, 55 leading NOPs, 78 fallthroughs into entries, 98
  other in-extent tail calls, 11 uncatalogued thunks/wrappers such as 0x186eec), plus pointer-only
  entries such as 0x1ef884.

## Experimental literal-record placement (2026-10-17)

`--literal-model anonymous-string-records-v1` is an **opt-in proposal**, independent of the
branch-rewrite `--linker-model`. The literal model defaults to `none` and composes with `strict-v1`,
v2, and the opt-in v3 removed-callee model. No extra branch-model name or alias is introduced.
A compiler-marked anonymous `.conststring` or `.constwstring` pool may be placed by complete
record identity rather than one affine section base. This does **not** claim to reconstruct
armlink's original ordering algorithm. It isolates a separately reviewable data-layout
qualification; no acceptance gate is adopted by implementing the model.

The section must be readonly, non-executable, relocation-free, non-COMDAT, with both
`SHF_MERGE|SHF_STRINGS`, the correct 1/2-byte entity size, and only local section symbols. Records
must be nonempty, unique, terminated, and separated only by zero alignment padding. The bounds
are 1 MiB and 4,096 records. Every record needs a placed ABS32/TARGET1 reference to its start;
all references to one record must agree. Their original pointers must identify a bijection onto
one contiguous, equal-sized witnessed span outside all catalogued functions and BSS. Reordering
**candidate** records and padding must reproduce that entire witnessed span exactly. This does
not establish the original pool's complete extent or ownership; surrounding records and sharing
with uncompiled units remain unknown. No image search, suffix/interior
binding, discarded record, invented data, instruction replacement, or relaxed function comparison
is permitted. Existing intra-/inter-unit section-overlap checks remain in force.

Match JSON and build inputs record `literal_model` independently of `linker_model`.
`literal_rebindings` in match results and per-unit build receipts records the rule, pool address,
size, section index, object ownership, record/reference counts, and candidate/output offsets and
sizes; it contains no literal contents. Combined builds attribute each pool only to its owning
unit. Prepare freezes both explicit choices in `selection.json` before either fresh build,
compares both receipts with that selection, and binds their complete hashes in prepare/history.
Its `model_uses` summary includes unchanged-source uses; a model transition reopens all affected
R1 callers and conservatively all credited functions in L1 pool-owning units. Review, finalize
and history verification check the schema-2 selection/receipt closure; historical checkpoints
retain their recorded rules. Function bytes, including relocated pointer values, must still
match independently. Defaults remain v2 / `none`; enabling the path does not adopt new gates.

The qualification is unknown original pool order/TU ownership, not uncertain literal bytes or
targets. Source/interface review must still reject artificial seeding tables, section attributes,
unused declarations or other compiler steering. `tests/test_literal_pools.py` covers the positive
case, simultaneous v3 callee removal and fail-closed boundaries without private inputs;
`tests/test_build.py` covers combined-build attribution and independent option/receipt fields.
Without `--literal-model`, every branch model retains its existing common-section placement rule.

