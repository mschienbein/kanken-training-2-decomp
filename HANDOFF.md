# exidx-removal: final v7 handoff

## Commits, base and non-adoption

- Worktree/branch: `scratch/worktrees/exidx-removal`, `exidx-removal`.
- **Final implementation commit: `b72126ebc55984ed231f8698a87417c284a63271`.**
- **Integration base: `5bb20a9a3251e83cddc37d6b64a166679ebd8e51`**, the synced
  `models-integration` tip = corrected-v6 commit `d525edd6d9e17675d770781b9df4d7d6d8bd17a5`
  plus frozen prepared main `f8adff48098a77057e13a50657b2722655469227`.
- New immutable identity: **`armlink5-inline-branchnop-v7` = corrected v6 + exclusive
  removed-helper EHABI companion elimination only**. V5 stays retired, never adopted.
  Defaults remain v2 / literal `none`.
- Author: `mschienbein <mschienbein@users.noreply.github.com>`.

The assignment initially used `22cb05b` and an unpublished unwind-prototype v6 name. Main
explicitly reassigned that extension to **v7** when corrected alias v6 was introduced. The
branch merges both the corrected base and its prepared-input sync; integrate the final branch
or the final delta against `5bb20a9`, **not prototype commit `2580770` alone**. Historical private
prototype records remain unchanged under `scratch/exidx/`; their old v6 label must never be
reinterpreted as corrected alias v6 or final v7 evidence.

No adoption, prepare/finalize, full suite, full build or push was performed by this worker.
`docs/GATES.md` is untouched by the extension. The requested input-sync merge inherits Main's
prepared c008-r3 artifacts/source/header changes; this worker did not author or rewrite them.
No author directory was edited and no `packets/` was read. No ROM bytes or original disassembly
were printed or committed. `orig` / `.cache/toolchain` are symlinked to main; all Python commands
use main's `.venv`.

Implementation files: `tools/inplace.py`, `tools/linker_models.py`, `tests/test_inplace.py`,
`research/linker/exidx_census.py`, `docs/LINKER.md`. Exact proposed gate text:
**`scratch/exidx/GATES-DRAFT.md`** (proposed v1.19, not adopted). Main must run the full suite
and obtain independent composite/exact-text review before any later adoption, after the active
frozen checkpoint closes. V4 and corrected-v6 prerequisites/qualifications remain mandatory.

## Rule text and refusal boundary

Only v7 permits an R1-removed helper's exclusive unwind companions to disappear with it.
The helper must **first** satisfy every unchanged corrected-v6 condition: complete eligible
4/8-byte ARM code, no original binding through any alias/copy, no roots, full alias/COMDAT
proof where required, I1–I3, all original call-site ABI/CFG checks, every other reference checked,
and at least one actual successfully inlined call per removed entity. Independently bound
retained names keep v6's corrected legacy placement; unwind ownership grants no binding waiver.

1. The helper's own index is readonly, allocated, non-executable, non-NOBITS `.ARM.exidx*`,
   type SHT_ARM_EXIDX, SHF_LINK_ORDER, sh_link to that exact defining text section. ELF reading
   preserves these fields and object combination remaps the link within its owning object.
   The complete section is one eight-byte entry, aligned at least four, entry size zero/eight.
   Exactly one offset-zero PREL31 key targets the helper's section/function entry, zero effective
   addend, bit 31 clear. No combined/shared index, duplicate fragment, interior/kept-function key,
   missing ownership metadata or name-only proof.
2. The second word is CANTUNWIND, a one-word compact personality-0 entry, or exactly one
   offset-four PREL31 to the beginning of an exclusive readonly, non-executable SHT_PROGBITS
   `.ARM.extab*`. Extab has no link-order/fixed-entry metadata. Only frame-only compact
   personality 0 (exactly four bytes) or 1/2 (exactly `4 * (1 + extra_word_count)`) is modeled.
   Generic personalities, scope descriptors, combined/trailing data and interior extab references
   fail closed. Nothing is synthesized or copied from the original.
3. Only those structural PREL31s and offset-zero NONE dependencies on the matching undefined
   `__aeabi_unwind_cpp_pr0/1/2` are understood in eliminated metadata. CANTUNWIND permits no extra
   dependency. An extab-pointing index has exactly its two structural relocations. Any other
   relocation/dependency, including arbitrary NONE markers or V4BX in metadata, refuses removal.
4. Companions cannot be rooted, independently named/exported, original-address-bound or
   differently COMDAT-owned from the text. Only local zero-size offset-zero section/data-mapping
   symbols are permitted. Exidx has no incoming allocated reference. Extab's sole incoming
   reference is its own exidx's offset-four PREL31. Census all allocated sections, even unplaced,
   and link-order dependencies. Shared extab, other incoming references or required original
   placement refuse removal. Every other reference to the helper still passes unchanged R1;
   pointers, tail calls, dependencies and inadmissible calls cannot vanish.
5. A kept function's separate unwind entry is **never selected for elimination**. No original
   unwind bytes are added, removed, rebound or rewritten. An unbound helper cannot receive an
   invented original table key. This is not general exception-table stripping/placement, a
   complete standalone link, or proof of the original unwind/exception regime.

Removed helper records and per-call target-null rewrites expose `unwind_sections`, naming each
companion's section/index/kind/size/object. Existing complete source/object/profile hashes bind
contents. Helpers/metadata earn no catalogue credit. Every typed-contract, manual ABI, visible
[INFERENCE] and explicit QUALIFIED historical-identity obligation remains mandatory.

## Evidence: specifications and original image

Primary references, also linked in `docs/LINKER.md`:

- [ARM DUI0474K §§4.12 and 4.3](https://documentation-service.arm.com/static/5ea19ea09931941038de96d2):
  inlining precedes unused-section elimination; elimination removes whole unreachable sections
  and preserves roots/used sections. §3.7 separately says exception tables are silently discarded
  if no code throws. V7 does **not** implement that general stripping rule.
- [Arm EHABI32: object-producer Sections / Index table entries](https://github.com/ARM-software/abi-aa/blob/main/ehabi32/ehabi32.rst):
  index fragment per code section; SHT_ARM_EXIDX, SHF_LINK_ORDER, sh_link to code; two-word
  PREL31 function key plus compact/extab/CANTUNWIND format.
- [ELF gABI SHF_LINK_ORDER](https://gabi.xinuos.com/elf/03-sheader.html#section-flags): described
  section and metadata are retained/discarded together; metadata back-references do not establish
  liveness of the described section.

[INFERENCE] Applying these documented semantics to armlink drops an eliminated function's
exclusive exidx fragment and then exclusively reached extab. No armlink is available in the
supplied toolchain, so no fresh native-armlink execution is claimed.

`research/linker/exidx_census.py` against the synced branch exited **0**. It emits counts,
addresses and candidate ELF metadata only, not bytes or disassembly. Command/output:
`scratch/exidx/v7/image-census-command.json` / `image-census.json`.

| Original-image structural observation | Count |
|---|---:|
| Word-aligned PREL31-to-text first-word candidates: text / rodata / data | 2,921 / 1 / 0 |
| Those with a loose possible EHABI second word: text / rodata / data | 47 / 0 / 0 |
| Loose non-code candidates targeting a catalogued function/entry start | 0 |
| Loose adjacent increasing-key pairs outside catalogued code | 1 |

The only rodata key candidate (`0x00229b18`) is not followed by an EHABI second word. The loose
text pair at `0x0015c57c` / `0x0015c584` lies in an existing function's non-code extent; both decoded
keys are halfword-offset **ARM** interior addresses, not valid ARM entries. Classification is
recorded in `image-census-pair-classification.json`. **No authenticated original EHABI table was
identified.** [INFERENCE] The image is consistent with discarded tables, but the flat `code.bin`
has no ELF section headers or authenticated exidx bounds: absence and original exception-link
flags cannot be proved from it alone. This inference never authorizes blanket removal.

Candidate relation: the final census records **13** allocated exidx fragments from copied
r4026 hooks (2), r41154 destructors (9) and tu659 destructors (2). All are eight-byte
SHF_LINK_ORDER/SHT_ARM_EXIDX sections linked to their own text, with PREL31 back to that text.
`candidate-entry-forms.json` classifies all **13 as CANTUNWIND**, without payload. Candidate
compiler emission does not establish original image entries. Real compact-extab elimination is
not claimed from these objects; that coverage is synthetic.

## Focused tests and regression coverage

Final synced implementation command, **exit 0, 215 tests, OK**:

```text
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -m unittest \
  tests.test_inplace tests.test_match tests.test_literal_pools
```

Receipt: `scratch/exidx/v7/focused-tests.json`. No full suite was run.

`RemovedUnwindModelTest` covers fail-before/pass-after **v6/v7** behavior for CANTUNWIND,
compact inline/extab entries and zero-size aliases; personalities 0/1/2 and exact dependencies;
real frozen-game D1/D2 ELF metadata in both combined-object orders; malformed type/flags/link/
extent/alignment, shared/combined/duplicate indexes, kept/interior keys, every other incoming
reference even from unplaced sections, unknown outgoing relocations/dependencies, shared/
interior/combined/generic extab, roots, independently named/original-bound metadata and extra
link-order dependencies. A kept caller's separate index is never selected. The new explicit-
binding regression confirms v7 inherits corrected v6/v4 placement without requiring a borrowed
full-section identity. Existing composite switch/effect/alias tests remain active.

A first new explicit-binding fixture mistakenly supplied an unregistered **global** through only
`names_to_address`; v4, v6 and v7 all correctly rejected it at the inherited global-definition
binding guard (215 tests, three subcase errors). `focused-tests-fixture-failure.json` preserves
that exit-1 result. The API-only subcase was corrected to a local symbol, without changing any
model behavior. Corrected pre-sync and final post-sync runs both passed 215 tests. This is not
an unchanged-input rerun-to-hide a model failure.

## Exact smoke input context

Use **`scratch/exidx/v7/`** for final evidence; the outer `scratch/exidx/` contains historical
prototype records that must not be relabeled. `inputs.json` binds 27 complete copied source/
profile cases, covering every named author. No author source was edited. Tu659/r41307/r41103
RESULT.json files were absent at initial scope; their available refusal receipts and compiled
companion filenames supplied the recorded closure. R41307's complete cases include the actual
`parameter_lifetime_nothrow.cpp` companion.

Every final smoke uses **this worktree's matcher**, frozen profiles/executables/flags and main's
current canonical config/includes. Config hashes matched main. The first integration tip had
three stale headers (`aux_effect.h`, `service_callbacks.h`, `service_runtime.h`), so the smoke
runner froze all **471** current-main headers under `v7/context/include` and selected only that
compiler include root. The requested `5bb20a9` sync then brought tracked headers to the same
frozen main inputs, without tool-code changes. `context-manifest.json` and `context-synced.json`
bind the snapshots, initial drift and its resolution. No canonical header edit or source shim
was used to force a result; proposed author names remain unadopted.

## Every natural-source outcome

Exits: 0 = all surviving functions MATCH; 1 = completed comparison differs; 2 = compile/link
refusal. Sizes are candidate/target bytes; `+0x...` is first differing offset. These are private,
unaccepted author-loop results, not checkpoint credit.

| Copied case ID | Profile | V6 exit/outcome | V7 exit/outcome |
|---|---|---|---|
| `tu659-final` | game | 2: exidx live ref | 1: MISMATCH 2444/2444; +0x39d |
| `r41103-game` | game | 2: exidx live ref | 1: MISMATCH 1236/1248; +0x10 |
| `r41154-game` | game | 2: exidx live ref | 1: MISMATCH 12320/12292; +0x24 |
| `r41103-sdk` | sdk | 2: exidx live ref | 1: MISMATCH 1204/1248; +0x4 |
| `r41154-sdk` | sdk | 2: call-ABI refusal | 2: call-ABI refusal |
| `r41103-middleware` | middleware | 2: exidx live ref | 1: MISMATCH 1244/1248; +0x4 |
| `r41154-middleware` | middleware | 2: exidx live ref | 1: MISMATCH 12416/12292; +0x5 |
| `r41128-early` | game | 2: exidx live ref | 1: MISMATCH 5236/5180; +0x29 |
| `r41128-final` | game | 2: call-ABI refusal | 2: call-ABI refusal |
| `r4026-member-hooks` | middleware | 2: exidx live ref | **0: MATCH 164/164** |
| `r4076-middleware` | middleware | 2: exidx live ref | 1: MISMATCH 140/140; +0x14 |
| `r4076-game` | game | 2: exidx live ref | 1: MISMATCH 140/140; +0x5 |
| `r4088-cpp-hook` | game | 2: exidx live ref | 1: MISMATCH 804/808; +0x30 |
| `r41207-cpp-result` | game | 2: exidx live ref | 1: MISMATCH 60/60; +0x2f |
| `r41224-game` | game | 2: compile / header drift | 2: compile / header drift |
| `r41224-middleware` | middleware | 2: compile / header drift | 2: compile / header drift |
| `r41234-game` | game | 2: exidx live ref | 1: MISMATCH 188/192; +0x0 |
| `r41234-sdk` | sdk | 2: exidx live ref | 1: MISMATCH 176/192; +0x0 |
| `r41234-middleware` | middleware | 2: exidx live ref | 1: MISMATCH 188/192; +0x0 |
| `r41262-001bd68c` | game | 2: exidx live ref | **0: MATCH 696/696** |
| `r41262-001c09e8` | game | 2: exidx live ref | 1: MISMATCH 840/840; +0xa8 |
| `r41275-cpp-predicate` | middleware | 2: exidx live ref | 1: MISMATCH 640/636; +0x24 |
| `r3L25-game` | game | 2: call-ABI refusal | 2: call-ABI refusal |
| `r3L25-middleware` | middleware | 2: exidx live ref | 1: MISMATCH 3332/3340; +0xc |
| `r41307-001e722c-complete` | game | 2: exidx live ref | 1: MISMATCH 360/364; +0x18 |
| `r41307-001e7db0-complete` | game | 2: exidx live ref | 1: MISMATCH 344/348; +0x9 |
| `r41307-001e395c-complete` | game | 2: exidx live ref | 1: MISMATCH 440/408; +0x38 |

**54 final natural commands**: v6 = 27 exit-2; v7 = **2 exit-0, 20 exit-1, 5 exit-2**. R41224's
unchanged historical source fails compilation because `PaneMaterialView` is undefined against
current headers; no helper/header workaround was introduced. Those two cases do not demonstrate
v7 linking. Still-refused ABI sites remain r41154/sdk `+0x7f8`, r41128/final `+0x35c` and
r3L25/game `+0x668`, unchanged by v7.

Every command, exit, stdout/stderr and actual receipt is under
`v7/smoke/CASE/{v6,v7}/`. `v7/summary.json` and the committed **`v7/EVIDENCE.json`** list every
receipt path/SHA-256, outcome, complete extent and removal count.

### MATCH receipts (not acceptance)

- r4026 `func_0013f06c`, middleware, 164/164:
  `scratch/exidx/v7/smoke/r4026-member-hooks/v7/func_0013f06c_v3.result.json`, SHA-256
  **`1ee546f54b5107d1215db6c1bddf5449b467eaa83d93269a9820f89357a75278`**.
- r41262 `func_001bd68c`, game, 696/696:
  `scratch/exidx/v7/smoke/r41262-001bd68c/v7/func_001bd68c.result.json`, SHA-256
  **`adff2973069cece6784544286f922826a5b50a92ff6097c3f004ba9cffa399c8`**.

Both identities already had separately reported C-helper/v3 witnesses; these natural C++ helper
results are not two newly accepted functions. [INFERENCE] Historical helper identity/call
provenance remains qualified, and typed/source review remains mandatory.

## Fifteen supplied accepted-regression rows

Input: `scratch/worktrees/r1-jumptables/scratch/integrator/r1-jumptables/r3/regression-commands.json`.
All **15 rows** ran under both v6 and v7 with distinct directories: **30 commands, every exit 0**.
The file contains five unique functions repeated under three historical selectors; execution did
not silently deduplicate the rows. Only the selected linker identity/output path and explicit
main-header context wrapper changed; compiler/profile/flags remained frozen.

| Function | Input rows | Exact bytes | V6 exits | V7 exits |
|---|---|---:|---|---|
| `func_00105084` | 1, 6, 11 | 64 | 0, 0, 0 | 0, 0, 0 |
| `func_0010d604` | 2, 7, 12 | 64 | 0, 0, 0 | 0, 0, 0 |
| `func_0011e928` | 3, 8, 13 | 72 | 0, 0, 0 | 0, 0, 0 |
| `func_0018fdd4` | 4, 9, 14 | 40 | 0, 0, 0 | 0, 0, 0 |
| `func_00195254` | 5, 10, 15 | 224 | 0, 0, 0 | 0, 0, 0 |

Each receipt has exact complete extent, zero rewrites and zero removed helpers. Commands and
MATCH receipts: `scratch/exidx/v7/accepted/rowNN-func_XXXXXXXX/{v6,v7}/`; every receipt hash and
exit is in `v7/EVIDENCE.json`.

## Frozen-model equivalence against the synced base

The snapshot of `5bb20a9` linker/registry blobs is byte-identical to the corrected-v6 `d525edd`
blobs. `fingerprint_frozen.py` loads the base and current implementation in separate processes
and links identical already-compiled objects/image/catalogue/names plus synthetic fixtures.
No compilation is involved.

**40 cases × 5 active frozen models = 200 linker outcomes compare byte-identically as serialized
records**, for **strict-v1 / v2 / v3 / v4 / v6**. Both commands exited **0**. Records include hashes
of every placed byte buffer, placement metadata, full rewrites, removed-helper metadata and
literal rebindings, or exact refusal type/reason. Frozen policy flags agree, and the v5 retirement
refusal is unchanged. Coverage: 25 compiled natural cases, five accepted functions, eight
plain/alias/unwind/pointer/root synthetic cases and two independently-bound partial-symbol cases.
The two current-header compile refusals have no object and are not counted as linker cases.

Evidence: `v7/frozen-synced-equivalence.json`, `frozen-synced-baseline.json`,
`frozen-candidate.json`. Both serialized output SHA-256 values are:
**`f4fa701d4bc700e043263be45e95dfe6310eebe6d50950bd226d4af5fbc788bc`**.
`v7/EVIDENCE.json` binds commands, code/blob hashes and all supporting artifact hashes. This is
finite regression evidence, not a mathematical proof for every malformed object. Older active
models never enter the new companion rule; v5 remains retired and inline v1 remains retired.

## Historical records and delivery artifacts

The original assignment's v5/prototype-v6 smokes and 15-row regressions were also executed and
are retained under the outer `scratch/exidx/`. Initial r41307 runner omissions and the corrected
complete closures are explicitly recorded there, not erased or blamed on authors. Those records
are historical only after Main's model-name/base change. The final corrected-base evidence is
exclusively under **`scratch/exidx/v7/`**.

Committed handoff artifacts are this file, `scratch/exidx/GATES-DRAFT.md` and the compact
payload-free `scratch/exidx/v7/EVIDENCE.json`. Complete receipts, copied sources, objects,
header snapshots and reproduction drivers stay private in the worktree. No ROM payload,
disassembly, packet, compiler or author candidate source is committed.
