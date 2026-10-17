# Linker model: armlink output optimisations

Status: **implemented on branch `armlink-inline`, default model `armlink5-inline-branchnop-v2`**
(`tools/inplace.py`, `LINKER_MODELS`). Not yet adopted by a checkpoint: adopting it changes what the
per-function oracle credits, so the integrator records a gate revision (docs/GATES.md) first.
Author: ArmlinkInline (Anthropic, claude-opus-5-5, xhigh), 2026-10-16.

The public toolchain has armcc but no armlink, so the original link step cannot be rerun. The
in-place linker (`tools/inplace.py`) relocates each compiled branch itself. Two armlink output
optimisations rewrite branches in the original image; without modelling them, C that the
original linker turned into these bytes can never match. The model reproduces them **only
where the rule follows from the final layout and the original callee's bytes**; every rewritten
branch is reported (`linker_rewrites` in `tools/match.py` results, `tools/build.py` receipts and
`tools/compid.py` records), and the model name is recorded (`linker_model`). `strict-v1` (no
rewrites; the oracle before this change) stays selectable with `--linker-model strict-v1`.

## Model history

A model name is a version: rules are never edited under a recorded name.

| Model | Status | Rules |
|---|---|---|
| `strict-v1` | selectable | no rewrites |
| `armlink5-inline-branchnop-v1` (commit c11a0ad) | **retired**: `--linker-model` refuses it | as v2, except that a callee defined in the candidate object was judged by the candidate's own section, never by the original. Independent review (ReviewLinker, F01) showed a wrong local helper could make its caller match: a `bx lr` helper at an address whose original is a 12-byte body, the excluded `MOV r0, r0; BX lr`, a veneer or a multi-entry owner turned the caller's BL into the original's NOP, and a caller-only ledger credited the caller (reproduced on the real image with func_001b6ec4 calling a local `func_0013714c`). Only artifacts of this branch's own checks recorded it. |
| `armlink5-inline-branchnop-v2` | default | below |

## Sources

[DUI0474K] *ARM Compiler v5.05 armlink User Guide* (ARM DUI0474K, 2014; its history lists issue J
as the v5.04 release), PDF: <https://documentation-service.arm.com/static/5ea19ea09931941038de96d2>.
Online index of the same chapter: <https://developer.arm.com/documentation/dui0474/k/linker-optimization-features>
(renders client-side; the text below was read from the PDF).

| Section | Rule (quoted or closely paraphrased) |
|---|---|
| 4.12 Function inlining with the linker | "The linker can inline small functions in place of a branch instruction to that function … the function (without the return instruction) must fit in the four bytes of the branch instruction." It "replaces the function call with the instruction from the function that is being called", before unused-section elimination. |
| 4.13 Factors that influence function inlining | Never inlines "instructions that read or write to the PC". Table 4-1: ARM caller → ARM callee of **4 to 8 bytes**; ARM → Thumb and Thumb → Thumb 2 to 6 bytes ("not possible to inline from ARM to Thumb"). The last instruction must be `MOV pc, lr` or `BX lr`. "A function that consists only of a return sequence can be inlined as a NOP." A conditional ARM instruction can be inlined only if the BL's condition matches it, or the BL or the instruction is unconditional; an instruction that cannot be conditional is not inlined under a conditional BL. |
| 4.14 About branches that optimize to a NOP; 12.18 `--branchnop` | "By default, the linker replaces any branch with a relocation that resolves to the next instruction with a NOP instruction." Default `--branchnop`. |
| 4.15/4.16 Linker reordering of tail calling sections; 12.153 `--tailreorder` | A section ending in a branch to the start of another section may be placed immediately before it, and the branch then becomes a NOP. One tail-calling section per target, same execution region. Default `--no_tailreorder`. |
| 12.75 `--inline`, 12.76 `--inline_type` | Default `--no_inline` (inlining still applies to the ARM C library); `--inline` = `--inline_type=all`. |

The manual does not say which NOP encoding armlink writes; the image does (below).

## Rules adopted (`armlink5-inline-branchnop-v2`)

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
- **Callee identity: decided from the original image only.** The callee is the original function
  at the call destination, eligible only when the catalogue makes it unambiguous: an ordinary
  top-level `kind: function`, ARM, size 4 or 8, all code (no pool or literal), no entry but its
  start. I1–I3 apply to its original bytes (`InPlaceLinker.eligible_original_callee`). A section
  that starts with `insn; BX lr` cannot extend past that extent: only literal data could follow the
  return, and loading it reads the PC, which is never inlined. Interior entries, `multi_entry`
  owners, veneers and Thumb functions are never inlined (the call stays a BL and mismatches if the
  original inlined it).
- **Callee defined in the candidate object.** armlink would judge the candidate's own section, so
  it must be the original function: if either its section or the original would be inlined, the
  local section (whole, relocation-free, starting at the destination) must equal the eligible
  original body, otherwise the link is **refused** (`LinkError: armlink inlining is undecidable`).
  `tools/build.py` links the whole object before it reads the ledger's function list, so a ledger
  crediting only the caller is refused too. If neither would be inlined the call is an ordinary
  relocated BL, so a wrong helper stays an ordinary mismatch.
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

As before, for every relocated field: the same target address. For a rewritten branch only:
- `inline_nop`/`inline_insn`: the original function at the call's destination is that return-only
  or one-instruction function (a body-equivalence class) — **not which function was called**, nor
  the original call graph; interface and semantic review stay necessary.
- `branchnop`: a function starts at the next instruction — the target symbol's address agrees with
  the layout, its identity is the C declaration's.
Reviewers see each rewrite in the match result, build receipt or compid record. These two
qualifications, and the manual-only basis of I3 for non-empty callees (below), belong in every
checkpoint that credits a function through a rewrite (review F03).

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

`--linker-model inplace-literal-records-v1` is an **opt-in proposal**, not the frozen default.
It retains the default's branch rules and adds `anonymous-string-records-v1`: a compiler-marked,
anonymous `.conststring` or `.constwstring` pool may be placed by complete record identity rather
than one affine section base. This does **not** claim to reconstruct armlink's original ordering
algorithm. It isolates a separately reviewable data-layout qualification; no acceptance gate is
changed by selecting or implementing the model.

The section must be readonly, non-executable, relocation-free, non-COMDAT, with both
`SHF_MERGE|SHF_STRINGS`, the correct 1/2-byte entity size, and only local section symbols. Records
must be nonempty, unique, terminated, and separated only by zero alignment padding. The bounds
are 1 MiB and 4,096 records. Every record needs a placed ABS32/TARGET1 reference to its start;
all references to one record must agree. Their original pointers must identify a bijection onto
one contiguous, equal-sized pool outside all catalogued functions and BSS. Reordering **candidate**
records and padding must reproduce that entire pool exactly. No image search, suffix/interior
binding, discarded record, invented data, instruction replacement, or relaxed function comparison
is permitted. Existing intra-/inter-unit section-overlap checks remain in force.

`literal_rebindings` in match results and per-unit build receipts records the rule, pool address,
size, record/reference counts, and candidate/output offsets and sizes; it contains no literal
contents. Function bytes, including relocated pointer values, must still match independently.
The qualification is unknown original pool order/TU ownership, not uncertain literal bytes or
targets. Source/interface review must still reject artificial seeding tables, section attributes,
unused declarations or other compiler steering. `tests/test_literal_pools.py` covers the positive
case and fail-closed boundaries without private inputs. The frozen default and `strict-v1` retain
their existing common-section placement rule.

