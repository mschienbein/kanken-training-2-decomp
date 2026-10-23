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
The recorded experimental **v9–v17** implementations remain selectable and immutable;
they and every **v11–v17 gate draft were never adopted**. Sealed FAILs remain under
`scratch/review/`. V18 implementation source `4a071371` and evidence tip `fdc61f7e` received
**PASS, no findings** in `scratch/review/linker-v18/codex/`; that seal explicitly does
not approve exact gate text or adoption.
This branch now proposes the exact **frozen v1.20** text in `docs/GATES.md`, including
the normative **v18-only U1** section, adoption sequence and reviewer checklist. The
v16-era `scratch/integrator/linker-unref/GATES-DRAFT.md` remains immutable historical
evidence, never current v18 authority. V1.20 activates only between closed checkpoints,
after separate exact-text approval and Main's integrated full suite; this author text
does not adopt it. Defaults remain v2/none, and aggregation never authorizes U1.
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
| `armlink5-inline-branchnop-v9` | recorded unadopted history, selectable; review FAIL | v8 plus U1: unbound, witnessed D0 removal without own-unwind exemption; recorded implementation omits zero-sized dependency sources, exact mapping alignment and relocation-target identity |
| `armlink5-inline-branchnop-v10` | recorded unadopted history, selectable; review FAIL | v9 plus v8's own-exidx/extab closure for U1 owners; inherits F01/F02/F05 and incomplete matcher/grid reporting closure (F03/F04) |
| `armlink5-inline-branchnop-v11` | selectable historical implementation/draft; **never adopted**, review FAIL | allocated dependency/root witnesses, mapping checks, normalized copy targets and hash/reporting closure; incomplete raw metadata |
| `armlink5-inline-branchnop-v12` | selectable historical implementation/draft; **never adopted**, review FAIL | unreduced section metadata; incomplete structural companion-edge proof |
| `armlink5-inline-branchnop-v13` | selectable historical implementation/draft; **never adopted**, review FAIL | exact structural companion edges; incomplete supplied-definition/personality namespace |
| `armlink5-inline-branchnop-v14` | selectable historical implementation/draft; **never adopted**, review FAIL | complete namespace resolution; original identity could be borrowed across nested aggregation |
| `armlink5-inline-branchnop-v15` | selectable historical implementation/draft; **never adopted** | narrowed direct-original-object U1, intentionally unavailable through aggregate builder dispatch |
| `armlink5-inline-branchnop-v16` | selectable historical implementation/draft; **never adopted**, review FAIL | direct-unit U1 staging; missing foreign same-signature COMDAT closure and positive full receipt scope |
| `armlink5-inline-branchnop-v17` | selectable historical implementation/draft; **never adopted**, review FAIL | global copy vetoes/full receipt scope; empty declarations disappear and early historical receipts are rejected |
| `armlink5-inline-branchnop-v18` | implementation PASS; v1.20 exact-text proposal, **not adopted** | complete raw group/ownership census, own-original witness and per-unit composition proofs, modern positive full receipts and era-scoped historical verification |

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

The v9–v17 sections below describe historical author proposals and their recorded results,
not adopted gate authority. References to frozen v1.19 or a proposed v1.20 inside those
records describe that historical stage. Every v11–v17 draft was never adopted; the current
v18 exact-text proposal is the normative section and adoption boundary in `docs/GATES.md`.

## Unreferenced deleting destructors (recorded v9; never adopted, rule U1)

V9 is **v8 plus U1**, not a change to any recorded model. Extending v8 preserves its adopted
R1 own-unwind/group closure instead of silently dropping cmodels2's v8 behavior. U1 does not
reuse that exception for a destructor: its own live exidx reference still prevents removal.
The default remains v2; every frozen model still refuses an otherwise unbound, non-R1 D0.

ARMCC emits a distinct D0 deleting-destructor entry when a native virtual destructor is
defined; the observed MenuScene/MenuResetScene objects also have retained D1/D2 entries.
The [Itanium ABI destructor variants](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#mangling-special-ctor-dtor)
identify D0 as deleting destruction, while D1/D2 are complete/base destruction.
[DUI0474K §4.3](https://documentation-service.arm.com/static/5ea19ea09931941038de96d2)
describes unused-section elimination separately from virtual-function elimination.
[INFERENCE] Original primary tables with a bound D1 and null D0 are consistent with virtual
function elimination followed by removal of an unreachable D0. This is not proof of the
original flags, original class spelling, or absence of an unidentified same-body function
elsewhere in a sectionless image. No native armlink execution is claimed.

U1 requires **all** of the following; unsupported/missing/contradictory evidence fails closed:

1. A complete, readonly executable `SHT_PROGBITS` section, not NOBITS/link-ordered/merged,
   named ARMCC's `i.<D0-symbol>`, aligned at least four. Its only function is an offset-zero,
   nonzero-size global/weak `STT_FUNC` with a length-checked `_ZN...D0Ev` name. Accepted class
   encodings contain ordinary length-prefixed identifiers, including namespace/nested-class
   components; substitutions, templates, thunks and suffix lookalikes are not recognized.
   Only local section symbols and valid local code/data mapping markers may accompany it.
   Its function size may exclude a trailing owned literal pool; the whole section is removed.
2. No symbol in that section has any canonical, proposed-overlay, explicit per-link or
   catalogued name/entry binding. A bound D0 retains ordinary placement/comparison, never U1.
   No entry, KEEP/export/used marker, unknown retention directive or rooted group can disappear.
   Every supplied affected COMDAT copy and allocated group member needs an independent U1 or
   already-proved v8 removal proof; live/unrepresented members, unknown group flags and
   contradictory copies refuse the link. No arbitrary data/group member is swept along.
3. The same defining object emits the same class's retained D1 entry, explicitly bound to a
   catalogued ordinary function start. Its `_ZTV` has an explicit original binding outside
   catalogued code and BSS. The primary-table header's offset-to-top is zero, RTTI is null
   or agrees with an explicitly bound `_ZTI`, the slot at `_ZTV + 8` equals the D1 binding
   including the ISA bit, and `_ZTV + 12` is null. Any missing/unreadable/misaligned/overlapping
   witness or disagreeing D1/D2/RTTI binding refuses removal. This supports destructor-first
   primary tables only, not a scan for a convenient null cell or an assumed secondary layout.
4. After existing R1/v8 removals actually pass their checks, **every** remaining supplied
   allocated section is checked, placed or not. No relocation may target the section, one
   of its aliases, or its unresolved external name: code, data, tables, exidx/extab, NONE
   dependencies and unknown relocation types all keep it live. `SHF_LINK_ORDER` is also a
   dependency. V4BX remains nonreferencing in ordinary code only. Self-references and
   references from another U1 candidate are conservatively refused; no dead cycle is inferred.
   In particular, U1 never nulls an emitted vtable slot, drops a live referring table or
   grants an own-exidx exemption. V8's old R1 proofs are unchanged.

**Why not D2/C2?** A zero-size D2/C2 is commonly the same-section alias of a source-defined
D1/C1, not a separately removable implementation. Entry alias identity already has its
v6 proof. Removing the shared section could delete a required original destructor/constructor;
U1 neither discards aliases independently nor guesses unreferenced source-authored bodies.
Only the ABI deleting wrapper D0 is admitted, without claiming metadata proves source provenance.

Each elimination is a `SectionElimination` in `InPlaceLinker.rewrites`, with
`rule: "unused_deleting_destructor"`, `target: null`, and `removed_special_member` identity,
complete size/index, defining object and primary-table/D1 witness metadata; no replacement
instruction or private payload is invented. Match results expose these actions in top-level
`linker_rewrites` and `removed_special_members`. Build units expose `linker_section_rewrites`
and `removed_special_members`, since no surviving function owns a discarded section.
Existing per-function branch rewrite fields are unchanged. Source/object/image/names/overlay,
catalogue, profile and harness hashes bind the action; nothing discarded earns credit.

Prepare exposes every U1 owner (including `functions: []`) in `model_uses` and binds the
complete fresh receipts/actions. A model transition reopens every current R1 caller, existing
L1-owning-unit function, and conservatively every credited function in a U1-owning unit with
review rule `U1`. A v6→v9 transition also selects the separately adopted v8 baseline; a
v8→v9 transition preserves it. c009-r4/c010/c011 and cmodels2 selections are not rewritten.
Independent reviewers must re-derive every U1 action, class/table bindings from constructor
stores and virtual consumers, group/root/reference exclusions, and retained source contracts.
The draft requires adoption after active frozen checkpoints close and before any new prepare;
an author MATCH alone never authorizes the model.

Focused synthetic coverage is `UnreferencedDestructorModelTest`,
`UnreferencedDestructorCompareTest`, the U1 receipt test in `BuildRemovedCalleeModelTest`,
and U1 transition/zero-credit-owner tests in `AcceptGateTest`.
The bounded real fixture is types-menu snapshot `d8aef63a`, attempt008
`src/wave18/func_001acbb4.cpp`. It still has allocated own-exidx metadata referencing D0,
despite emitting no vtable. Attempt006 additionally emits a live table→D0 reference.
V9 must refuse these cases rather than silently implement another elimination rule.

Executed on the recorded v9 proposal: **126 focused regressions, exit 0, OK** (U1, frozen v2/v4/R1/v6/v8,
alias comparison, synthetic receipts and model-transition closure). The bounded attempt008
`tools/match.py` run selected v9/L1 with `--show 0` and the snapshot names overlay:
**exit 2**, `.ARM.exidx: linked metadata keeps a U1 deleting destructor live`.
Only the compiler include root was redirected to the immutable snapshot; frozen `game`
flags/compiler and this branch's matcher/linker were unchanged. Commands, source hash and
exact exits are recorded in `scratch/integrator/linker-unref/evidence.json`; the refusal is
`scratch/integrator/linker-unref/real-menu-reset-v9/func_001acbb4.result.json`.
No full build, whole-ledger audit, adoption or new credit is claimed.

## Deleting-destructor own-unwind closure (recorded v10; never adopted)

V9's real MenuResetScene receipt records a strict own-exidx refusal. It remains a valid
historical identity, not an adopted model or a rule amended under its recorded name.
Only **`armlink5-inline-branchnop-v10`** adds the following U1 companion rule. V10 preserves
all v9 D0 recognition, bindings, original null-vtable/D1 witnesses, root exclusions and
ordinary retained-code comparisons, while retaining the adopted v8 R1 behavior.

An independently U1-eligible D0 may lose **only its exclusive own** unwind closure:

1. Its readonly allocated `SHT_ARM_EXIDX`, with `SHF_LINK_ORDER` and `sh_link` to that
   exact D0 section, is one complete eight-byte entry. Its offset-zero `R_ARM_PREL31`
   targets exactly the D0 entry with zero effective addend; combined/interior/foreign
   keys, duplicate index owners and unsupported section metadata refuse elimination.
2. The second word is CANTUNWIND, an inline compact personality-0 entry, or a structural
   PREL31 to the beginning of one exclusive readonly frame-only compact extab. Out-of-line
   personalities 0/1/2 require the complete encoded unwind prefix **plus exactly one final
   zero descriptor-list terminator**. Nonempty descriptors, truncated/trailing data,
   generic personalities and unsupported relocations/dependencies fail closed. Only the
   matching undefined `__aeabi_unwind_cpp_pr0/1/2` NONE marker is understood.
3. The index has no other incoming reference; extab is referenced only by its own index's
   structural PREL31. Neither companion is rooted, original-bound or independently named,
   and both satisfy v8's exact section/group constraints. Before an exemption, every
   allocated member of every affected group/copy must be selected eligible R1/U1 code or
   a validated own-unwind companion. Kept/unrepresented/unsupported members, unknown group
   flags and contradictory code copies refuse the link. No arbitrary group member is swept in.
4. Every **other** live relocation/link-order dependency still refuses, whether from code,
   data, tables, foreign exidx/extab, aliases or dependency markers, placed or unplaced.
   This includes references to an eliminated companion, not only to D0. Ordinary-code
   V4BX stays nonreferencing; markers in unwind/data do not receive that exemption.
   A bound/retained D1's entry is never selected. No vtable cell is nulled or discarded.

`_removed_unwind_sections` applies its existing v8 structural/group proof to an explicitly
supplied union of independently selected R1 and U1 owners **only under v10**. Older models
pass precisely their old R1 owner set. All-reference checks run after that proof and R1's
actual inlined-call checks; U1 alone never proves an R1 helper removable.

Every U1 action still records `unused_deleting_destructor` with `target: null`; its nested
`unwind_sections` separately identifies each eliminated index/extab's name/index, kind,
complete size and defining object, exactly as v8 reports R1 companions. Match/build/prepare
retain those complete actions in existing U1 receipt/model-use closure. A v9→v10 or
v8/v6→v10 transition reopens current R1/L1 review dependencies and every credited function
of each U1-owning unit. Companion sections earn no credit. The revised draft requires
independent exact-text/implementation review and a recorded gate revision before any
checkpoint selects v10; all current checkpoint selections remain unchanged.

The rationale is the same documented
[ELF `SHF_LINK_ORDER` ownership rule](https://gabi.xinuos.com/elf/03-sheader.html#section-flags)
and [EHABI32 index ownership/format](https://github.com/ARM-software/abi-aa/blob/main/ehabi32/ehabi32.rst)
used by v8. [INFERENCE] armlink discards a proven eliminated section's exclusive own
link-order index and exclusively reached extab together, rather than treating their
structural back-reference as independent liveness. This is not proof of original
linker flags, original unwind-table bounds or a fresh native armlink execution.

Synthetic U1 closure coverage is `UnreferencedDestructorUnwindModelTest`, the v10
matcher/build receipt tests, and the v9→v10 transition test in `AcceptGateTest`.
The recorded v9 tests/receipt remain unchanged evidence; v10 has its own bounded results.

Executed v10 evidence: **138 focused tests, exit 0, OK**, covering own-companion removal,
frame-only/zero-terminator limits, foreign/shared/live references, complete groups/copies,
receipt attribution and v9→v10 transition closure, alongside the frozen-model regressions.
The bounded d8aef63a MenuResetScene attempt008 comparison selected v10/L1: **exit 0**,
`_ZN14MenuResetSceneD1Ev` matched all **100 bytes** at `0x001acbb4`.
Its only rewrite action is `unused_deleting_destructor`, `target: null`, eliminating
`i._ZN14MenuResetSceneD0Ev` (section 1, **108 bytes**) plus its exclusive `.ARM.exidx`
(section 4, **8 bytes**). No extab or per-function branch rewrite was eliminated/applied.
The same immutable snapshot/include-root override as v9 was used; no source flags changed.
Commands and every action are recorded in `scratch/integrator/linker-unref/evidence-v10.json`;
the full result is `scratch/integrator/linker-unref/real-menu-reset-v10/func_001acbb4.result.json`.
This private MATCH does not adopt v10, grant credit, or replace independent gate review.

## Complete U1 dependency and evidence proof (recorded v11; never adopted)

The sealed review of `f9c35805` found one blocker and four major defects; its exact
counterexamples and FAIL remain in `scratch/review/linker-v10/codex/`. Neither v9 nor v10
is corrected under its recorded identity. Only **`armlink5-inline-branchnop-v11`** adds:

- **F01 — complete allocated census:** `ObjectFile.dependency_sections` preserves zero-sized
  allocated sections separately from frozen output placement. Symbols, relocations, roots,
  section links, group membership and defining-object identity survive parsing and combination.
  V11's retention, U1 all-reference, R1 proof and own-unwind incoming/link-order censuses
  include them. A retained or unplaced zero-sized NONE/link-order source can keep D0,
  exidx or extab live; no source is omitted merely because it has no bytes to place.
  Unknown/unproved zero-sized group members veto complete group elimination. Earlier models
  continue to use their original section/reference sets.
  An early U1 census precedes placement witness inference, then the final census follows
  R1's actual call-removal proofs; provisional candidates never publish elimination actions.
- **F02 — exact mapping ranges:** every mapping marker is local zero-size `STT_NOTYPE`,
  exactly represented by the section map. ARM starts and range lengths are multiples of
  four; Thumb starts/ranges multiples of two; data may be byte-aligned. The entry ISA,
  function extent alignment, ordering, unique offsets, complete ranges and power-of-two
  section alignment must agree.
  Odd or incomplete code ranges are unsupported, not evidence for U1.
- **F05 — relocation-target identity:** copies compare decoded section metadata, bytes, mapping
  and each relocation's type/offset plus normalized target identity, binding, kind, value and
  size. A definition's actual combined section identity is mandatory: equal local names,
  equal local data bytes or a matching unproved COMDAT key do not establish one target.
  Shared resolved definitions, agreeing absolute targets and agreeing global extern identities
  are supported. Unproved/local-undefined and contradictory targets refuse in either order.
  Every allocated group copy also has the same proved code/own-companion roles, complete
  metadata and companion contents. Only already-proved own-companion structural targets
  may normalize through those roles; arbitrary local/COMDAT destinations never do.
  Defined-symbol names, binding, kind, value and extent must agree across copies as well.
- **F03 — matcher input closure:** successful v11 results bind canonical names plus overlay,
  canonical and proposed profile inputs and effective profile, catalogue/image/exheader/layout,
  title/toolchain lock/compiler, source/object/dependency manifest and every dependency header,
  and all decision/reporting harness modules. Helper inputs receive the same closure.
  Fresh compilation supplies the required dependency manifest; missing manifests/headers
  refuse rather than publishing an unbound action. The model id alone is not an implementation
  hash. Full-build receipts retain their existing source/object/config/harness closure.
- **F04 — every reporting consumer:** compiler-grid records retain each source/pair's
  `linker_section_rewrites`, `removed_special_members`/companions and v11 `comparison_inputs`.
  V11 gives same-stem source probes distinct object paths so one receipt cannot overwrite
  another comparison's hash-bound object.
  A failed comparison cannot inherit a previous source's actions. `collect.py` explicitly
  refuses every U1 model before writing candidates because its collection schema has no
  per-comparison section-action receipt. Match/build/prepare preserve the existing explicit
  U1 action closure; the permuter remains on its fixed non-U1 model.

The regression helper `tests/elf_fixture.py` adapts the review's independent synthetic ELF
writer; `tests/test_unreferenced_v11.py` reuses its actual parsed-ELF counterexamples instead
of bypassing the reader with only dataclass mutations. It covers zero-sized retained sources
to code/index/extab, both combined-object orders and cross-object extern dependencies,
rooted zero-sized group witnesses, invalid mapping metadata, distinct same-spelled local
relocation targets (including equal-byte locals), exact shared-target controls and reporting
closure/dispatch. V9/v10 receipts, failed review and private historical matches remain evidence
of those implementations only, not approval of this correction.

V11 retains v8's adopted R1 baseline plus the v10-shaped U1 exclusive own-unwind rule; no
general table stripping or dead-code elimination is added. Existing transition review
obligations also apply to v10→v11: every current R1 caller, L1 owning-unit function and
credited function in each U1-owning unit is reopened. The revised draft requires independent
exact-text/implementation review and a recorded gate revision before a checkpoint selects v11.
No checkpoint, historical selection, adopted rule or default is retroactively changed.

Executed v11 measurements for this proposal: **166 focused tests, exit 0, OK**, including
parsed-ELF review counterexamples/positive controls, frozen-model regressions,
matcher/grid/collector dispatch, receipt attribution and v10→v11 transition closure.
All **eight exact sealed F01/F02/F05 ELF fixtures** reused from the reviewer directory
refuse under v11 without elimination actions; six reused valid fixtures still match and
report their actions (probe exit 0). Reviewer files were read only. Unrecorded test counts
in the inherited work-in-progress are not claimed as evidence.

Both real cases use the same immutable `d8aef63a` source/header/name snapshot and frozen
`game` flags/compiler with `--show 0`, explicit v11/L1 and the proposed names overlay:

- **MenuResetScene attempt008: exit 0**, D1 matches all **100 bytes** at `0x001acbb4`.
  Exactly one `unused_deleting_destructor`, `target: null`, removes D0 section **1 / 108 B**
  and its own exidx section **4 / 8 B**. There is no extab or per-function branch rewrite.
  The receipt binds the canonical names, profile inputs/effective profile, nine harness
  modules, fifty dependency headers and the remaining full input closure.
- **MenuScene attempt006: exit 2**, now directly at v11's early all-reference census:
  `.constdata__ZTV9MenuScene+0xc` still refers to D0 through ABS32. No action is published.
  This is not a reclassification of v10's earlier RTTI-placement refusal.

Exact argv/cwd/exits, reused-fixture hashes and every action are recorded in
`scratch/integrator/linker-unref/evidence-v11.json`; full receipts are
`scratch/integrator/linker-unref/real-menu-reset-v11-r2/func_001acbb4.result.json` and
`scratch/integrator/linker-unref/real-menu-scene-v11-r2/func_00174480.result.json`.
No full suite, full build, whole-ledger audit, model adoption or new credit is claimed.

## Unreduced semantic section metadata (recorded v12; never adopted)

The independent review of `5de7845b` held all five earlier root fixes, but sealed **FAIL**
on **V11-F01**: `SHF_TLS`, `SHF_INFO_LINK` and individual `SHF_MERGE`/`SHF_STRINGS`
bits disappeared before the copy comparison. Contradictory D0 copies therefore still
emitted two U1 actions in either object order. V11's rules, failed review, tests and
receipts remain unadopted history; no recorded model is repaired under its old name.

Only **`armlink5-inline-branchnop-v12`** enables the additional complete-metadata proof:

- `SectionMetadata` retains the **entire unmasked flag word**, raw address/alignment and
  link/info fields. Together with the existing resolved section name, type, size and entry
  size, these fields participate in every selected COMDAT code/own-companion comparison.
  The comparison is not a whitelist or a reconstruction from executable/writable/merge
  booleans. Unavailable metadata, decoded/raw disagreement and unmodeled nonzero link/info
  semantics refuse rather than silently disappear.
- `GroupMembership` retains each containing group's complete header flags, name, address,
  size, alignment and entry size, plus its normalized canonical symbol-table/signature
  metadata. Membership must agree with `SHF_GROUP`; missing/duplicate memberships,
  unsupported signatures/tables and unequal complete allocated member roles refuse.
  These checks also apply to R1-only code/companion groups, not just groups containing D0.
- `SHF_LINK_ORDER` and `SHF_INFO_LINK` section-index dependencies survive combination with
  correct defining-object ownership. Only independently proved roles normalize corresponding
  own-unwind targets across copies; arbitrary same-spelled/local targets never normalize.
  Both header dependency forms enter the U1, R1 and own-unwind incoming/link censuses,
  including zero-sized allocated sources.
- ELF file offsets and string-table indices are serialization details, not semantic section
  identities. Canonical symbol-table/signature evidence normalizes group `sh_link`/`sh_info`
  only after validation. Identical metadata remains subject to all independent eligibility,
  root/reference, full-section, own-unwind and source-policy requirements.

`tests/test_unreferenced_v12.py` exercises the actual parsed-ELF TLS reproduction through
`Context.compare`, all **32 individual flag bits** on D0/exidx/extab in **both object
orders** (192 contradictory cases), identical-metadata controls, complete header/group
fields, link/info target identity/remapping, zero-sized INFO_LINK witnesses, pure R1
companion copies, reporting and immutable-model isolation. Its synthetic compressed-section
variant has a real ELF compression header; it does not substitute malformed compressed data.

Executed focused selections: **183 tests, exit 0, OK**, plus **13 checkpoint-selection/
receipt-binding tests, exit 0, OK**. All **18** prior sealed counterexamples and all
**eight** sealed v11 contradictory-flag fixtures refuse without actions under v12; six
valid sealed controls still match and report actions (probe exit 0). The first test-helper
argument-name collision and the first probe's missing `synthetic-elf` path component are
preserved as exit-1 harness records; their corrected invocations use new immutable names.

Bounded `d8aef63a` real cases use the exact v11 commands with only model/output selection
changed, retaining `nice -n 10`, the immutable include-root override, frozen `game` profile,
L1, names overlay and `--show 0`:

- **MenuResetScene attempt008: exit 0**, D1 **MATCH, 100 B at `0x001acbb4`**. Exactly one
  target-null U1 action removes D0 **108 B** and own exidx **8 B**; no extab or branch rewrite.
- **MenuScene attempt006: exit 2**, live vtable ABS32 at `.constdata__ZTV9MenuScene+0xc`
  keeps D0 live; no action is published.

Exact argv/cwd/exits, retained failures, fixture hashes, receipts and actions are recorded in
`scratch/integrator/linker-unref/evidence-v12.json`; fresh real receipts are under
`scratch/integrator/linker-unref/real-menu-{reset,scene}-v12-r3/`.
The v1.20 draft for that proposal named v12 and preserved R1/L1/U1 transition-review obligations.
At that stage V11 remained selectable and never adopted; defaults were v2/none and GATES was frozen v1.19.
No adoption, full suite, full build, checkpoint rewrite or new credit is claimed.

## Edge-specific own-unwind exemption (recorded v13; never adopted)

The independent review of `e0a409b1` confirmed the unreduced-metadata root fix, but sealed
**FAIL** on **V12-F01**: the reference collector saw foreign INFO_LINK edges, then the
blanket companion-source exemption removed them from consideration. Mutual exidx/extab
dependencies to peer D0s returned two MATCH rows and two U1 actions. Two mixed-index
COMDAT copies returned four of each in both object orders. That failed implementation,
its receipt evidence and all older models remain immutable, selectable, unadopted history.

Only **`armlink5-inline-branchnop-v13`** enables `structural_companion_edges`:

- Header dependencies retain both **edge kind and destination**. INFO_LINK does not become
  structural merely because its destination happens to equal a valid LINK_ORDER owner.
- Before a companion can leave the census, its only permitted header edge is an exidx's
  exact LINK_ORDER to its own validated removed owner. Any nonzero INFO_LINK refuses,
  even to that same owner; an INFO_LINK flag with zero info carries no edge.
- Existing per-relocation validation permits only the exact own PREL31 entry key and the
  optional PREL31 tail to that index's own exclusive extab. Extab is reached only by its
  own index. Every other supplied-section edge from a companion refuses: peer D0s,
  other removal candidates, kept/unplaced code/data and foreign metadata are not swept in.
- The exact matching **undefined** EHABI personality NONE dependency remains supported as
  an ABI marker, not an arbitrary section reference. Defined/foreign, extra or unsupported
  relocations still refuse. The rule applies equally to pure R1 own-unwind closures.
- Own-companion header-edge checks precede group-copy normalization, removal reporting and
  the later U1 source exclusions. Neither matching metadata nor corresponding group roles
  establish that a self/peer cycle is dead. No action is published on refusal.

`tests/test_unreferenced_v13.py` uses actual serialized/parsed ELF through `Context.compare`.
It covers mutual pairs, three-cycles, direct self/peer U1 cycles, acyclic foreign edges,
INFO_LINK/LINK_ORDER/NONE/ABS32/PREL31 transports, kept/data/code/metadata targets,
both section/object orders and mixed-index joint COMDAT copies. Positive controls cover
one/two/three independent structural closures, zero-info markers, complete frame-only
extab, exact ABI markers and pure R1 closures. Matcher/grid/collector dispatch and the
v12→v13 selection transition retain the existing action/hash/review obligations.

Executed focused selections: **194 tests, exit 0, OK**, plus **13 checkpoint-binding
tests, exit 0, OK**. Reusing the exact sealed objects under v13, all **26** earlier
counterexamples and all **six** v12 peer/mixed-index comparison scenarios refuse without
actions; six valid controls still match and report actions (probe exit 0). The first
focused run lacked a local section symbol for its synthetic foreign-exidx relocation
fixture and exited 1; that harness record is retained and the corrected run uses a new name.

The same `d8aef63a` real commands, with only model/output selection changed, give:

- **MenuResetScene attempt008: exit 0**, retained D1 **MATCH, 100 B at `0x001acbb4`**.
  Exactly one target-null U1 action removes D0 **108 B** plus its own exidx **8 B**;
  no extab or per-function branch rewrite.
- **MenuScene attempt006: exit 2**, live vtable ABS32 at `.constdata__ZTV9MenuScene+0xc`;
  no elimination action.

Exact argv/cwd/exits, retained failure, fixture hashes and real action/input receipts are
`scratch/integrator/linker-unref/evidence-v13.json`; fresh real receipts are under
`scratch/integrator/linker-unref/real-menu-{reset,scene}-v13-r2/`.
The proposed v1.20 text for that historical proposal named v13 and was never adopted.
Defaults were v2/none and GATES was then frozen v1.19. No adoption or new credit followed.

## Complete supplied-symbol namespace (recorded v14; never adopted)

The independent review of `e164f003` confirmed V12-F01's exact-edge root fix, but sealed
**FAIL** on **V13-F01**: the combination map used `defined_globals()`, a positive-sized
placement view, as an external-symbol namespace. A supplied zero-sized global/weak
personality definition therefore remained labelled SHN_UNDEF and passed the ABI exception.
The nonzero counterpart refused. V13 and every prior record retain their old meaning.

Only **`armlink5-inline-branchnop-v14`** enables the complete namespace cutover:

- `defined_symbols()` inventories **every defined symbol**, regardless of section size,
  allocation, binding or kind, including local, NOBITS, ABS and COMMON definitions.
  Filtered function/global placement views never establish that a name is absent.
- `Reloc.reference` preserves each original pre-combination reference and symbol scopes
  survive nested composition. Before every v14 link—including a single object—
  `resolve_definitions()` creates a non-mutating resolved view. Legal global/weak externs
  bind to actual supplied definitions; locals remain direct scoped identities and never
  satisfy a foreign global extern.
- The personality NONE exception additionally requires a genuinely undefined, default-visible
  global/weak NOTYPE/FUNC with zero value/size and **no supplied same-name definition**.
  This conservative exclusion includes locals, ABS/COMMON, nonallocated and zero-sized
  definitions. Denying the exception does not falsely bind a global to an unrelated local.
- Conflicting/unproved global/weak duplicates refuse in either order; supported COMDAT
  identities retain their complete downstream proof. Equal full ABS identities represent
  the same actual value. Unsupported relocation-reference/resolved-definition kinds,
  illegal undefined-local references, invalid allocated extents and incomplete table/index
  evidence refuse; the unreferenced-definition inventory is not itself a type whitelist.
- COMMON storage allocation is not modeled and explicitly refuses. A supplied NONE
  dependency must have verified output placement; omitted zero/nonallocated storage cannot
  masquerade as a library extern. Actual ABS constants have no output storage and retain
  value-based semantics, never an unrelated names-overlay substitution.
- R1's helper metadata now accounts for every symbol, not just its function view: independent
  data/file/global-section aliases refuse. Only proved full-entry FUNC aliases, permitted
  local section symbols and ARM mapping markers may accompany the removed helper.
  Visibility also participates in v14 target/copy identity. Ordinary byte/extent, original
  callee equality, CFG/ABI, ownership and source-policy gates are unchanged.

The requested systematic author audit is
`scratch/integrator/linker-unref/AUDIT-v14.json`: **20 resolution paths × 33 ELF case
classes = 660 explicit pairs**, each labelled `refuses` or `proven_safe` with its
path-specific identity/dominance proof and mapped regression. It records **11 corrected
unsafe classes**, distinguishing legal namespace visibility from legal extern resolution.
Its coverage includes the reader, both combination passes, scopes/original references,
single/nested dispatch, filtered views, aliases, original bindings, R1/U1/unwind/copy
consumers, placement, NONE/final targets and matcher/build reporting. It is an author audit,
not independent approval; path-entry counts were not instrumented and upstream-refused
cases are explicitly identified as dominated, not claimed to execute unreachable consumers.

`tests/test_unreferenced_v14.py` exercises **120 definition inventories**, **240
both-order resolution instances** and **1,920 supplied-personality scenarios** across
U1/pure-R1, inline/extab personalities 0/1/2, size/allocation/binding/type combinations.
It also covers ABS/COMMON, nonallocated and section symbols, aliases, duplicate weak/global/
COMDAT definitions, single/nested combination, table provenance, kind/extent/visibility
failures and reporting. Executed focused selections: **215 tests, exit 0, OK**, plus
**13 checkpoint-binding tests, exit 0, OK**. The earlier 213-test passing selection is retained.

Sealed-object replay: **82 cases, exit 0**. All **76 negatives** refuse without actions,
including all 44 supplied-personality-definition scenarios and the former local controls;
six valid structural controls still match and report actions. Genuine external marker and
ordinary namespace positive controls remain in the focused suite.

Bounded d8aef63a real commands change only model/output selection:

- **MenuResetScene attempt008: exit 0**, D1 **MATCH, 100 B at `0x001acbb4`**; exactly one
  U1 action removes D0 **108 B** plus own exidx **8 B**, no extab or branch rewrite.
- **MenuScene attempt006: exit 2**, live vtable ABS32 at `.constdata__ZTV9MenuScene+0xc`;
  no elimination action.

Exact argv/cwd/exits, audit binding, fixture hashes and complete real receipts/actions are
`scratch/integrator/linker-unref/evidence-v14.json`; fresh real receipts are under
`scratch/integrator/linker-unref/real-menu-{reset,scene}-v14-r2/`.
The proposed v1.20 text for that historical proposal named v14. No adoption, full suite/build, checkpoint rewrite,
subagent execution or new credit is claimed.

## Narrow single-direct-object U1 scope (recorded v15; never adopted)

The independent review of `5b60ef11` confirmed the namespace fix, but sealed **FAIL**
on **V14-F01**. Original symbol scopes survived nested combination, while U1 compared
collapsed `section_owners`: a D0-only original borrowed D1 from a different original
after nesting, and its action named the outer bundle. Six nested variants reproduced
this; the broad AUDIT-v14 conclusions **P13/C26 and P20/C26 are withdrawn**.

Only **`armlink5-inline-branchnop-v15`** enables the narrower scope:

- U1 and its own-unwind exemption require **exactly one directly supplied, compile-attested
  object**. Every candidate section, symbol and relocation reference read must share its
  real immutable `ObjectOrigin`. Original-image/catalogue/config witnesses remain separate
  hash-bound reference evidence, not additional candidate TUs.
- Every `ObjectFile.combine` result refuses U1, including a single-element wrapper,
  a valid complete unit plus an empty/unrelated unit, flat multi-object and nested inputs.
  Archive members, declared bundles, ambiguous defaults and missing/mixed origins also
  refuse before namespace/witness/placement. Equal filenames, bytes and unit-owner numbers
  never establish original identity.
- Raw readers/default comparisons are ambiguous. The successful compiler dispatch in
  `match.run`/`compid` explicitly attests the object it just compiled. A raw API caller
  must supply genuine direct-input provenance; re-reading a saved aggregate is not an
  attestation. Archive-labelled/member or declared bundled input cannot become direct merely
  by assigning an outer ledger label.
- The in-scope D1 witness consumes original section **and symbol** identity, not
  `section_owners`. Complete input origin maps cover zero/nonallocated headers and symbols
  too; a missing/foreign entry refuses. All namespace, mapping, structural, reference,
  retention and whole-extent rules remain mandatory.

**R1 review:** its linking proof has no same-original-object D1 assumption; it uses actual
section/target identities and retains cross-object helper support. The related attribution
bug did affect R1 (and optional L1) object labels, so v15 preserves original provenance
separately: `object` identifies the original reader input, `unit` identifies the outer
ledger unit, and `input_kind` makes provenance class explicit. Build helper filtering and
acceptance helper-revision expansion consume `unit`; they never mistake an ELF output path
for a ledger source. Known-reader R1 semantics remain unchanged. Missing original identity
does not produce invented provenance.

`tests/test_unreferenced_v15.py` reproduces all six nested witness loans and both flat orders,
then covers single wrappers, extra empty inputs, archives/bundles, unattested/default
readers, equal-path distinct identities, missing/mixed maps and direct positive controls.
Eight R1 flat/nested/both-order/direct/archive variants reproduce their synthetic original
caller bytes and preserve helper.o independently of the outer unit. The revised-helper
dependency test confirms review expansion follows the ledger unit.

`AUDIT-v15.json` records **20 paths × 42 case classes = 840 pairs**, marking out-of-scope
U1 cases **“refused by scope”** with regressions. It rechecks all **100** prior
owner-dependent safe claims and explicitly withdraws the two refuted conclusions instead
of extending their broad proofs. The author audit remains subject to independent review;
build filtering is source-reviewed, not represented as a production build execution.

Executed focused selections: **227 tests, exit 0, OK**, plus **13 checkpoint-binding
tests, exit 0, OK**. The final sealed replay has **91 cases, exit 0**: **58 scope
refusals**, **28 inherited refusals**, and **five direct matches**, including an actual
byte comparison of the review's direct control. Two formerly positive multi-object controls
are intentionally refused. An earlier probe's direct control established linker acceptance
only; the later immutable probe adds the actual comparison rather than claiming the former
as byte-match evidence.

The exact d8aef63a real commands change only model/output selection:

- **MenuResetScene attempt008: exit 0**, D1 **MATCH, 100 B at `0x001acbb4`**; exactly one
  target-null action removes D0 **108 B** plus own exidx **8 B**, with original direct
  provenance, no extab or branch rewrite.
- **MenuScene attempt006: exit 2**, live vtable ABS32 at `.constdata__ZTV9MenuScene+0xc`;
  no elimination action.

Commands, audit binding, hashes and receipts are `scratch/integrator/linker-unref/evidence-v15.json`;
fresh real records are under `real-menu-{reset,scene}-v15-r1/`.
The current builder always supplies an aggregate, so v15 **does not authorize U1 in whole-ledger
builds**, even with one wrapped unit. No per-unit removal workaround or future broader model is
implicit. The v1.20 draft for that historical proposal named v15; no adoption, full suite/build, subagents or new credit.

## Direct-unit U1 in full builds (recorded v16; never adopted, review FAIL)

V15's aggregate refusal remains immutable. Only
**`armlink5-inline-branchnop-v16`** adds a builder stage that authorizes U1 on each ledger
unit's **own direct compiled object before aggregation**:

1. Compile fresh and inspect original objects for unique emitted credited ownership.
2. Run `prepare_unit_u1` on each direct, non-nested unit. It uses the narrow original-object
   witness/reference/metadata rules, stages exact D0/own-companion actions and returns a
   prepared view. Original definitions/reference provenance remain visible; a group with
   an unproved R1/other member refuses rather than borrowing an aggregate proof.
3. Apply **negative composition vetoes** over original inputs. Foreign references to any
   staged removal—including zero-sized/unplaced sources and conservative peer cycles—
   refuse. A supposedly external personality marker with any supplied ledger definition
   also refuses. These checks never add removals or loan a D1 witness.
4. Combine prepared units for ordinary relocation/placement/R1. The aggregate cannot decide
   U1; an escaped unprocessed U1 candidate still fails the narrow scope guard.
5. Verify every retained D1 witness against its complete catalogue extent **after aggregate
   relocation**, even in a zero-credit owning unit, before publishing a successful receipt.

Build units carry complete original-object `linker_section_rewrites`,
`removed_special_members` and nested `unwind_sections`; their section indices remain local
to the original unit. Existing per-function `linker_rewrites` stay separate. Source/object,
fresh dependency manifest/header, compiler/config/model/harness inputs bind the actions.
No removed code or metadata earns function/byte credit.

Prepare records both complete fresh receipts and exposes the actions through `model_uses`
and its U1 summaries. V16 requires `u1_stage: per-unit-direct-before-aggregation`,
one-to-one action/member correspondence and matching retained-witness records. Review,
finalize and history recheck that closure. Existing transitions reopen all R1 callers,
L1 owner functions and credited U1-owner functions; zero-credit owners/actions remain visible.

### Bounded fixture comparison

`build.py --bounded --matches <scratch-ledger>` exercises the same compile/preparation/
composition/relocation/receipt path but writes **metadata only**: no hybrid allocation,
`code.bin`/BLZ output or recompression. It records `covered` counts and span hashes, not
accepted credit or whole-image equality. A proposed `--names` overlay is allowed only in
this mode and is hash-bound with the canonical/merged names. V16 intended to reject
bounded receipts at acceptance; its sealed review demonstrates omission/summary bypasses.

Executed focused selection: **238 tests, exit 0, OK**, plus **13 checkpoint-binding tests,
exit 0, OK**. New synthetic full-path tests cover per-unit action/companion/witness receipts,
zero-credit owners, wrong D1 bytes, combined-input refusal, split-unit D1 loans, foreign
zero-sized references, supplied global/weak/local personality definitions and acceptance
stage/action/witness integrity. The v15→v16 transition binds both receipts and reopens U1 review.

The bounded real check uses a two-unit ledger: exact types-menu d8aef63a MenuResetScene
attempt008 plus an independent empty zero-credit TU. Frozen game compiler/flags, L1,
snapshot include root and proposed names are explicit. **Exit 0, PASS**:

- D1 placed at **`0x001acbb4`**, exact **100 B** covered span.
- Covered/original SHA-256 both
  **`3dcc4769af32c297d855d8f16a49fc153bceefe3730125f0ee42034fcbec4ccf`**.
- Exactly one target-null U1 action removes D0 **108 B** and own exidx **8 B**.
- Complete original object/unit provenance, retained-witness comparison and source/object/
  dependency/harness closure appear in the receipt and derived `model_uses`.
- No image payload files or BLZ recompression.

Bounded receipt: `build/linker-unref-v16-bounded-r1/receipt.json`.
Fixture ledger/helper and exact commands/exits/actions are bound in
`scratch/integrator/linker-unref/evidence-v16.json`.
Direct matcher re-runs preserve MenuResetScene **exit 0 / MATCH 100 B** with the same one
action, and MenuScene **exit 2 / live vtable refusal** without actions.
No whole-ledger production build, full suite, adoption, subagents or new credit is claimed.

## Global composition and positive receipt scope (recorded v17; never adopted, review FAIL)

The sealed v16 review (`scratch/review/linker-v16/codex/SEAL.json`, **FAIL**) reproduces
two independent holes. A foreign same-signature COMDAT group containing kept code,
unplaced data or a zero-sized allocated member did not veto a local D0/own-unwind removal,
in either object order (**V16-F01**). Deleting `mode` from a bounded receipt, or passing it
through the old `build_summary` filter, could misrepresent covered-only evidence as a
successful fresh build (**V16-F02**). V16 and all prior evidence remain historical.

Only **`armlink5-inline-branchnop-v17`** enables `closed_u1_composition`:

- The builder collects affected signatures from the staged local removals, then inspects
  **every original unit**, before combining prepared views. Every positive- or zero-sized
  allocated member of every affected copy needs that unit's own removal proof. Kept code,
  unplaced data, zero-sized members and unproved members veto the entire operation.
- Original group flags, signature/table provenance and raw member indices remain visible;
  unknown semantics or unrepresented/contradictory membership refuse. A metadata-only
  combined view remaps original section identities to compare complete proved copy roles,
  contents, ELF/group metadata and relocation-target identities. It never links that view,
  creates a removal or supplies a cross-unit D1 witness. Existing foreign-reference and
  supplied-personality-definition vetoes still run over the originals.
- Same-stem C/C++ units receive distinct object paths, so staging and object/dependency
  hashes cannot silently describe the other unit's compiler output.

New build receipts explicitly identify `schema_version: 2`, `record_kind: build-receipt`
and `mode: full` or `bounded`. Every per-unit U1 row includes an explicit authorization
count and all three action/member/witness lists, including empty lists. Retained D1
witness records include their full expected extent and equal candidate/original hashes.

Acceptance now requires **positive full-image evidence for every new prepare**, not just
the absence of `mode: bounded`: the full receipt identity/scope, passing fresh status,
matching complete image and BLZ input/output hashes, exact whole-image checks, credited
counts, unit/compiler/source/object/config/harness provenance and the complete selected
U1 schema. Covered counts/spans are forbidden there. `build_summary` now marks itself
`build-summary` and preserves scope metadata; it remains deliberately insufficient as a
receipt. Review, finalize and history re-open the hash-bound complete receipts.
V17 intended an explicit legacy path for already-recorded non-v17 schema-1 full evidence.
Its sealed review shows that a later unconditional `linker_rewrites` requirement breaks
genuine pre-rewrite c001 receipts. Its positive whole-image/BLZ checks and modern scope
requirements remain mandatory; failed legacy postmortems still cannot finalize.

Executed **507 tests, exit 0, OK** across every linker test module plus the build/accept
modules (not the full suite). Exact regressions reproduce all six F01 permutations,
different-signature controls, complete agreeing/contradictory copies, group provenance,
the bounded/missing-mode/relabelled/summary F02 variants, isolated prepare/selection
attacks and positive full-receipt controls. Missing scope, image, unit, action, witness
or provenance fields refuse. Earlier unsuccessful runs and their fixture corrections
are retained in the evidence rather than overwritten.

A fresh two-unit bounded real build writes only metadata under
`build/linker-unref-v17-bounded-r5/`: **exit 0, PASS**, D1 **100 B at `0x001acbb4`**,
with exactly one target-null U1 action for D0 **108 B** plus its own exidx **8 B**.
The independent empty unit remains visible with explicit empty U1 lists/count zero.
Direct matcher controls give MenuResetScene attempt008 **exit 0 / MATCH 100 B**, and
MenuScene attempt006 **exit 2 / live vtable reference**, with no elimination action.
Commands, exits, source/object/header hashes and complete actions are in
`scratch/integrator/linker-unref/evidence-v17.json`; the systematic closure/absence/
consumer sweep is `scratch/integrator/linker-unref/AUDIT-v17.json`.

That v17 proposal changed neither `docs/GATES.md` nor `GATES-DRAFT.md` and was never
adopted. Its historical evidence claims no whole-ledger/full build, full suite or new credit.

## Complete raw group census and historical eras (v18; implementation PASS)

The sealed v17 review (`scratch/review/linker-v17/codex/SEAL.json`, **FAIL**) found two
blockers: empty affected `SHT_GROUP` declarations disappeared before the member census
(**V17-F01**), and finalized full receipts predating rewrite reporting were rejected
(**V17-F02**). V17's six empty-group acceptances and its historical failure remain
immutable evidence, not corrected under that selector.

Only **`armlink5-inline-branchnop-v18`** enables `complete_group_declarations`:

- The reader retains every original declaration's index, signature, optional flag word,
  complete raw member-index tuple, defining-object identity and unsupported-structure
  issues, including empty declarations and nonallocated members. The independent input
  declaration count survives combination and nesting. Group ownership is checked before
  allocation filtering can hide orphan `SHF_GROUP` sections or multiple memberships.
- The builder validates **all original units before the first local U1 decision**.
  Direct matching/preparation also requires the complete census. Missing or partial
  words, missing membership/provenance, orphan/duplicate membership and absent census
  structures refuse, rather than becoming empty safe defaults. Combination preserves
  original issues and marks unavailable original censuses explicitly.
- Every affected declaration must represent at least one allocated member and every such
  member needs that unit's independent removal proof. Thus a same-signature copy made
  entirely of filtered nonallocated members cannot disappear from composition checks.
  Fully accounted unrelated groups remain separate. All existing complete-copy metadata,
  content, relocation-target, foreign-reference and own-object witness proofs still apply.

Historical verification keeps rewrite dictionaries optional **only** for explicitly
allowed schema-1 default-model receipts from before the producer recorded `linker_model`.
The authentic c001–c003 receipts have that shape. Later historical producers and every
new prepare still require rewrite reporting. Historical source/receipt files are never
rewritten, and the modern positive full-image/BLZ/scope/action/witness requirements do
not become optional.

Regression coverage includes the review's six empty declarations and twelve nonempty
controls, malformed words, absent/orphaned censuses, nested preservation, filtered-member
copies, agreeing copies and positive unrelated controls. A pre-rewrite fixture exercises
review, finalize and history without adding a fake empty rewrite dictionary; a modern
receipt missing the same field still refuses. Verification/evidence is recorded in
`scratch/integrator/linker-unref/evidence-v18.json` and `AUDIT-v18.json`.

The integration proof uses fresh detached worktrees and local `--no-ff` merges of source
commit `4a071371`, with no accepted source/header/receipt restoration:

- Last finalized/pushed target **`a443fef1`**, local merge **`52901fa3`**:
  `.venv/bin/python -B tools/accept.py verify` **exit 0**, all **10 accepted checkpoints**
  bound; all linker/accept/build modules **527 tests, exit 0, OK**.
- Pending **`7a975a0d`** (c010-r7 PREPARED), plain versus local merge **`cd294f88`**:
  both verification commands **exit 1** at missing `src/wave12/func_001dc574.c`.
  Complete live source/header/unit/ledger inventories contain the same **273** failures
  and identical accepted-record hashes, with **zero added failures**. These prepared-state
  failures are not relabelled as successful historical verification. The merged target's
  linker/accept/build modules also pass **527 tests, exit 0**.
- A fresh bounded build on the finalized merge writes only metadata to
  `build/linker-unref-v18-bounded-r4/`: **exit 0**, exact D1 **100 B at `0x001acbb4`**,
  one target-null U1 action for **108 B D0 + 8 B own exidx**. The existing d8aef63a author
  fixture is referenced only through scratch paths, never as accepted-source hydration.
  Direct matcher controls give MenuResetScene **exit 0 / MATCH 100 B**, and MenuScene
  **exit 2 / live vtable refusal**, without an elimination action.

Both merge-tree checks and local merges exit 0 without conflicts. The original branch
history/source-drift failure, the first incomplete synthetic-fixture test run and the
first external-manifest bounded setup failure are retained in the evidence. That
implementation/evidence phase amended neither GATES file and performed no production
full build, full suite, adoption, main mutation or push.

### V1.20 normative text and activation boundary

`docs/GATES.md` now contains the separately proposed exact v1.20 text: only explicit
`armlink5-inline-branchnop-v18` may use its witnessed U1 rule after the adoption sequence.
Each positive proof uses one compile-attested direct non-nested original object, its own
retained D1 and primary-table witness, complete raw SHT_GROUP/COMDAT/reference closure
and only exclusive structurally valid own unwind. The builder cannot borrow an aggregate
witness or R1 proof; it applies original-input vetoes and compares every staged D1 after
relocation, including zero-credit owners. The code/metadata section above describes the
reviewed implementation, not an exception to any normative prerequisite.

The first exact-text proposal (`548c0234`, manifest `39bd54e7`) failed independent
gate-text review on three enforcement overstatements; the v18 implementation PASS was
not retracted. The revised text describes the existing boundaries without changing code:

- The D0 guard rejects the string-merge classification with both `SHF_MERGE` and
  `SHF_STRINGS` set, not every standalone raw merge/string flag. Complete metadata,
  entry-size and copy-consistency requirements still apply.
- Emitted D1/D2 address bindings in positive-sized allocated sections agree with the
  retained D1 start, and retained D1 ISA agrees with the catalogue/table. U1 adds no
  independent emitted-D2 ISA check; inherited alias and independently bound placement
  rules remain distinct.
- U1 section actions carry no instruction encodings. Inherited per-function branch
  rewrites retain their modeled replacement `word` metadata; raw original/candidate
  payload buffers and disassembly dumps are not serialized.
- Namespace kind/compatibility checks apply to allocated-source relocation references
  and their resolved symbols, not every unreferenced definition in the inventory.
  Ordinary-code V4BX and unnamed NONE retain their namespace-only exceptions; the
  separate U1 liveness and own-unwind dependency rules still apply.

Both fresh full receipts, frozen selection, prepare, review and history bind every
action/companion and input. Positive schema-2 full-image/BLZ receipt identity and exact
unit/action/witness dispatch are mandatory for new prepares; bounded evidence and
summaries never authorize acceptance. Historical receipts retain their own era's optional
fields. Transitions reopen affected R1 callers and the existing L1/U1 owner/dependency
closure. Earlier algorithms, defaults, records and source-policy qualifications stay
unchanged; v11–v17 drafts are not relabelled into approval.

Main must first close every active checkpoint under its existing gates/evidence, obtain
separate independent approval of the committed exact texts, run the integrated full suite
and record v1.20 on top of v1.19 between checkpoints. The implementation PASS, this text,
private MATCHes and bounded builds grant no credit and perform no adoption. The immutable
text hashes and claim-to-code/test map are in
`scratch/integrator/linker-unref/GATES-v1.20-TEXT-r2.json`.

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

