# Catalogue v14 — evidenced library and handwritten-assembly owners

## Proposal boundary and frozen parent

**Unadopted proposal, not an independent review or C credit.** This work is on the
new `catalogue-v14` branch and `scratch/worktrees/catalogue-v14` worktree, created
from Main's `5820254f213d8b28130f18c59afde974733112b8` after confirming that neither
name existed. Main's worktree, accepted sources, names, ledger, gate text and old
catalogue evidence were not edited. Independent review and Main's adoption,
between checkpoints, remain required under ACCEPTANCE H4 and GATES v1.20.

The adopted parent is **v13-r2**, SHA-256
`394e550406cc227550245bbbed378e7bba21f1de858fdd9310d9a737aba27b8a`, retained as
immutable Git blob `1326f1675735048b26fa435f11605e613cac89f2`. The passing
`REVIEW-v13-r2.md` and v13-r2 evidence are separately hash-bound inputs. All older
failed proposals, refusals, qualifications, reviews and history records remain
unchanged. Nothing here retroactively upgrades them.

The proposal's `config/catalogue.json` is **6,706,167 B**, SHA-256
`add2952983f72625f2d990605864b4389889da11dd413f09bd040ccbaa012eff`.

## Denominator and complete-record accounting

| Measure | Adopted v13-r2 | Proposed v14 | Delta |
|---|---:|---:|---:|
| Creditable ordinary owners | 4,571 | **4,546** | **−25** |
| Creditable owner-extent bytes | 1,121,674 | **1,120,094** | **−1,580 B** |
| Library owners | 90 | **95** | **+5 / 104 B** |
| Assembly owners | 17 | **37** | **+20 / 1,476 B** |
| All owners | 4,743 | 4,743 | 0 |
| All entries | 4,776 | 4,776 | 0 |
| Unknown bytes / intervals | 114 / 11 | 114 / 11 | 0 / 0 |
| Anomalies | 1 | 1 | 0 |
| Accepted functions / bytes | 2,682 / 288,580 | 2,682 / 288,580 | 0 / 0 |

The other owner kinds remain runtime **22**, multi-entry **25**, and veneer **18**.
Every one of the 25 transitioned records changes **only** `kind` and the added
`classification_evidence`. Its complete address, size, ISA, entries, code ranges,
pools, literals and every other old field are identical. All **4,718 other complete
function records** are identical, as are the ordered physical ranges and every
other top-level image/ownership field. Inherited statistics change only in the
classification/creditable totals. The previous generator/review are authenticated
by the parent blob; the new generator and review describe this successor rather
than copying the previous revision's historical change list as a v14 claim.

Against the exact frozen **current** `config/matches.json` and `config/names.json`,
all **2,682 accepted functions** retain their **entire** catalogue records, not just
sizes: **zero accepted extent, ISA, entry, kind or status changes**. The live branch
ledger and names files equal their frozen input copies. Their accepted bytes stay
288,580; no numerator credit is granted. Every accepted address, extent, ISA and
complete-record digest is retained in `research/catalogue_v14_generation.json`.

The exact eleven unknown intervals are unchanged:
`00100020/4`, `0010012e/48`, `001005fa/16`, `00100942/2`, `00100b26/2`,
`00100d70/4`, `0010d3a8/8`, `001415b4/4`, `001a00b4/4`, `001c7748/20`,
`001cd2e6/2`. The unchanged anomaly is
`0010d2b8: unexplained_function_bytes`. This proposal does not claim zero residue.

## Five vendor runtime-library identities

| Unchanged owner | B | ISA | Identity | Independent named original-address anchor |
|---|---:|---|---|---|
| `func_00100a04` | 16 | Thumb | `_snputwc` | Authenticated complete `__c89vswprintf` at `func_0010083c` |
| `func_0018272c` | 24 | Thumb | `strcat` | Natural `func_00195900`, retained call at `00195924` |
| `func_0018651c` | 28 | Thumb | `wcsncpy` | Qualified natural `func_00193168`, retained call at `0019321c` |
| `func_00191c80` | 18 | Thumb | `__cxa_guard_acquire` | Natural C++ static-local `func_00180234`, retained calls at `00180250` and `0018027c` |
| `func_0019d5b0` | 18 | Thumb | `wcslen` | Public-C-header replay of `func_001cf418`, retained call at `001cf42c` |

These are the five exact sweep candidates, not the sweep's unadmitted whole-body
coincidences, section-prefix leads, unsupported relocation leads or unequal
extents. Fa02, fa06, fa12 and fa13 evidence is retained with original artifact
hashes. Every selected definition is a complete named `STT_FUNC`, the entire
allocated executable section has the same extent and ISA as the unchanged owner,
and there are **zero relocations in all five target definitions**. No whole-field
mask, prefix, neighboring tail, borrowed pool, new linker rewrite or extent growth
is used. All five previously had kind `function`; none was already classified.

A fresh bounded project-reader replay reopened the exact licensed archive/member
hashes and compared all five whole target sections in memory. It also reconstructed
the complete **72-byte** previously authenticated `__c89vswprintf` parent, including
its three actual relocations. Its declared function symbol remains 68 bytes; the
parent's existing whole-section/tail qualification is not reinterpreted. These are
**six checked sections**, not six new library owners. The four retained natural
caller objects were reopened, and their **five actual named call relocations**
were reencoded and checked at the original sites. No compiler or matcher was run.
The original whole-caller MATCH receipts remain retained evidence, not newly
claimed compilations or independent source reviews.

### `_snputwc`: distinct later-build evidence

The admitted complete 16-byte definition is from **Arm 5.06 b960**
`lib/armlib/c_5.l(_snputwc.o)`, with the equivalent variant metadata retained.
The older **5.04 b82 setup mismatch stays a genuine negative**. The new positive is
not a retry of that failed body or a relaxed comparison. The independently
classified `__c89vswprintf` supplies the actual named callback reference, including
the original direct call; callback-like shape alone would still be insufficient.
Whole-body identity does not determine which historical archive/compiler variant
the game actually used.

### `wcsncpy`: admit identity, not nominal C++ type equivalence

The existing admission standard requires complete named vendor-body identity plus
an independent natural original-address caller/ABI anchor for a short generic
body. The 204-byte natural bounded-copy replay preserves the accepted caller's
unsigned-short interfaces and exactly reproduces every caller byte, with **zero
linker rewrites**. Its named C-linkage declaration is representation-equivalent to
the installed public **C** `wchar.h` ABI and retains the original `wcsncpy` call.
That is sufficient for the bounded library-identity claim under the existing
standard; it is **not** approval that native C++ `wchar_t` and `unsigned short` are
nominally the same type.

The public-header C++ refusal and the C minimum-selection mismatches in fa06's
`MATCH-RUNS.json` remain failures and are retained. No caller, header, prototype,
name or type adoption follows; no cast or replacement accepted source is proposed.
The classification qualification states this limit at the claim.

### `wcslen`: preserve the old refusal; add the missing anchor

The v13/thumb-profile **generic-body provenance refusal was correct on its old
body-only evidence** and remains unchanged. Fa13's frozen-profile diagnostics
also remain mismatches, not C/profile evidence. Merely finding the complete
relocation-free 18-byte `wcslen.o` section would still be insufficient.

The sweep adds the missing independent evidence: a natural replay of the already
accepted **48-byte C caller**, using the installed public `wchar.h`, reproduces the
whole caller and retains the actual named `wcslen` relocation at offset **20** to
`0019d5b0`. Its one inherited v2 `branchnop` is at the separate final tail offset
**44**, not the library call. The full vendor definition **plus this new original-
address named anchor** meets the unchanged standard. No historical provenance
uniqueness, nominal C++ type claim, source credit or Thumb-profile adoption follows.

The guard-acquire evidence likewise does not use the caller's separately marked
inference about guard-release. Its two acquire relocations remain ordinary calls;
the unrelated inherited rewrites are recorded separately.

## Whole handwritten `FastWordCopyAsm` family

The canonical
[`include/nw/gfx/gfx_MemoryUtil.h`](../../include/nw/gfx/gfx_MemoryUtil.h)
explicitly declares these functions `asm`. The pinned public implementation is
[`gfx_MemoryUtil.cpp` at `15ddb5632db5a6f0d2f46e3827fc78df468684bd`](https://github.com/ctrdecomp/NintendoWare4Ctr/blob/15ddb5632db5a6f0d2f46e3827fc78df468684bd/sources/libraries/gfx/gfx_MemoryUtil.cpp),
SHA-256 `b5b1cba6154e1663c9dad92ec601b975c12f2e4815859dd06642912096498e31`.

Fa03's retained full-family proof covers **all 20** public macro expansions,
not only its six assigned targets. Fa04 and fa05 independently corroborate their
six and three targets with complete normalized operation traces. The source,
header, complete original-extent hashes, operation-count/hash equality and empty
difference lists are retained and bound by the generator. Normalization is limited
to spelling/whitespace, architectural register aliases and ranges, multiple-
transfer spelling and numeric notation; predicates, register order, writeback,
immediates, prefetch positions and returns remain compared. No private trace or
instruction encoding is stored. The retained public-macro comparisons were not
rerun or represented as new compilations in this proposal.

| Owner | Specialization | Exact B | Previous classification |
|---|---:|---:|---|
| `00184204` | 2 | 28 | ordinary `function` |
| `00184220` | 4 | 28 | ordinary `function` |
| `0018423c` | **6** | 28 | ordinary `function` |
| `00184258` | **8** | 32 | ordinary `function` |
| `00184278` | 10 | 48 | ordinary `function` |
| `001842a8` | 12 | 48 | ordinary `function` |
| `001842d8` | **14** | 48 | ordinary `function` |
| `00184308` | **16** | 52 | ordinary `function` |
| `0018433c` | 18 | 68 | ordinary `function` |
| `00184380` | 20 | 68 | ordinary `function` |
| `001843c4` | 22 | 68 | ordinary `function` |
| `00184408` | 24 | 72 | ordinary `function` |
| `00184450` | 26 | 88 | ordinary `function` |
| `001844a8` | 28 | 88 | ordinary `function` |
| `00184500` | 30 | 88 | ordinary `function` |
| `00184558` | 32 | 92 | ordinary `function` |
| `001845b4` | 34 | 108 | ordinary `function` |
| `00184620` | 36 | 108 | ordinary `function` |
| `0018468c` | 38 | 108 | ordinary `function` |
| `001846f8` | **78** | 208 | ordinary `function` |

The family is exactly contiguous over **`[00184204,001847c8)` / 1,476 B**. Bold
specializations are the **five additional owners / 368 B** beyond the 15 assigned
author targets. The endpoint is **78 words**, not an invented 40-word entry.
**Already classified within this family: none.** The inherited 17 assembly owners
are outside this family. Every member is unaccepted, entirely ARM code, and has
no pools/literals. All twenty receive kind `assembly`, never library or C credit.
The adjacent `001847c8` runtime helper stays unchanged and outside the family.

**[INFERENCE]** The public namespace/symbol-to-image mapping follows complete
normalized operation correspondence, the exact fixed word counts and explicit
canonical assembly declarations. This is evidence for handwritten assembly, not
a claim of a uniquely authenticated historical NW4C archive/compiler build. No
assembler implementation or new canonical name binding is imported.

## Generation, input closure and regression checks

V14 is deliberately a **classification-only successor** of the frozen adopted
v13-r2 map, using the existing non-C tool. It does not rediscover boundaries or
reinterpret older generator revisions. `tools/analyze.py catalogue` retains its
historical v13-r2 semantics; this revision has the explicit public command below.
The loader authenticates the immutable Git parent, retained sources/certificates,
current accepted ledger/names snapshots, header and parent review. All certificates
are checked before mutation; a protected owner, missing identity anchor, partial
section, conflicting body, rewritten named call, missing family declaration or
changed extent refuses. Public validation recomputes the exact successor from
independently supplied input/source bindings, rejecting unrelated record, physical
map, statistic, receipt, review or generator changes.

Actual generator argv, executed once in the new worktree:

```text
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -B tools/catalogue_non_c.py propose-v14 --image /home/michael/code/kanken-training-2-decomp/orig/code.bin --exheader /home/michael/code/kanken-training-2-decomp/orig/exheader.bin --prove-objects --private-root /home/michael/code/kanken-training-2-decomp --out config/catalogue.json
```

**Exit 0; 1.367818 s captured wall time.** A normal checkout with private inputs at
its own root may omit `--image`, `--exheader` and `--private-root`. Omit
`--prove-objects` for certificate/image-only generation; the output is identical.
The optional replay uses hash-bound private archive/caller objects, never compiler
invocations or mutable scratch discovery. Original licensed/ROM buffers remain
in memory and are not serialized.

Focused checks actually exercised:

```text
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -B -m unittest tests.test_catalogue_v14 tests.test_catalogue_alignment tests.test_catalogue_v13_r2 tests.test_analyze.CatalogueTests.test_reviewed_classification_retains_extent_and_physical_ownership tests.test_analyze.CatalogueTests.test_classification_requires_identity_proofs_and_is_transactional tests.test_analyze.CatalogueTests.test_classification_cannot_mutate_a_complete_protected_record tests.test_analyze.CatalogueClosureTests.test_classification_checks_the_entire_original_extent_identity tests.test_analyze.CatalogueClosureTests.test_frozen_baseline_and_parent_are_independently_authenticated
```

**Exit 0; 37 tests; 26.294 s unittest time / 26.646687 s captured wall.** The v14
regressions cover all 2,682 frozen accepted records, exact permitted transitions,
whole-image metadata/statistic preservation, generic-body refusal without a named
anchor, conflicting/partial definitions, the qualified wide-character cases,
rewritten-name refusal versus the legitimate separate tail, callback-parent
identity, all twenty declarations including 78, complete macro-trace checks,
protected-target collisions, duplicate/missing/forged certificates, generator
binding forgery, retained-input tampering and escaping paths. Old v13 apply/reverse,
alignment, absence and bounded-read safeguards also pass. Metadata-only v14
fixtures contain authored nonexecutable text, not ROM or encoded instructions.

The branch adds **denominator-history sequence 11** (catalogue revision v14, not
history version 14), explicitly marked **UNADOPTED**. The predecessor remains
sequence 10 / adopted v13-r2. The requested command actually ran on this branch:

```text
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -B tools/denominator.py verify
ok: denominator v11 matches config/catalogue.json
```

**Exit 0.** No full suite, build, compiler, matcher, acceptance prepare/finalize,
canonical adoption or push was performed. Main retains independent review and the
adoption-merge checks required by the existing process.

## Retained evidence map

- `research/catalogue_v14_evidence.json`: revision/policy, frozen parent, input
  descriptors, denominator parent and current accepted-record commitment.
- `research/catalogue_v14_non_c_proof.json`: exact 25 owner certificates, complete
  before-record hashes, extents, classifications and qualifications.
- `research/catalogue_v14_library_evidence.json`: exact five sweep candidates,
  caller/parent anchors, fa02/fa06/fa12/fa13 evidence, preserved negative trials
  and old generic-body refusal, plus explicit adjudications.
- `research/catalogue_v14_callers.json`: four original natural caller sources and
  hash-bound receipt projections; instruction-encoding fields are omitted, while
  the original receipt/object/source digests are preserved.
- `research/catalogue_v14_assembly_evidence.json`: all twenty fa03 records,
  fa04/fa05 corroboration, upstream/header hashes and already-classified census.
- `research/catalogue_v14_frozen/config/{matches,names}.json`: exact current
  protected inputs; no future live-file mutation can silently alter this proposal.
- `research/catalogue_v14_generation.json`: exact generator command/result,
  exhaustive 25-record delta, all 2,682 accepted identities/record hashes,
  unchanged physical/unknown/anomaly records and per-class accounting.
- `research/catalogue_v14_checks.json`: actual commands, exits and captured
  outputs/timings, source/test/proposal/history hashes. This is author evidence,
  never a passing independent review seal.

## r2 — behavioural-test repair and finalized Main merge

The sealed r1 **FAIL/F01** is accepted: aggregate totals and explanation prose
were not adequate regression assertions. The tests now derive exact transition,
extent/ISA/entry, creditable and classification sets from the parent and evidence;
they check complete records against the **live** accepted ledger and candidate
manifests. Source/object/whole-receipt binding refusals replace the prose-only
test. Every retained input is tested for missing/tampered content and missing/extra
roles; an uncertified non-family neighbour must remain unchanged and cannot gain
a classification through an added certificate alone. Existing mutation refusals
remain. This is an author repair, not an independent closure seal for F01.

Main `69854e8ef7989eb1431f56ffed1d41dc930f4def` was merged as
`88ab1ecf18bb52a81a38c30c08b02e592063866c`; `5b8f88dd` was not amended.
All **2,770** live accepted identities and all **16** tracked candidate manifests
were reconciled to that exact Main commit: none claims an evidenced v14 owner,
and every claimed complete function record equals the parent. The original
frozen ledger/names, generator inputs, catalogue and denominator history remain
unchanged; the earlier 2,682-function snapshot above is historical r1 evidence,
not the live protection population after this merge.

The focused unittest command above, with the rewritten v14 module, exited **0**:
**42 tests passed**, 53.921 s unittest / 54.230725 s captured wall time.
`tools/denominator.py verify` exited **0**, still binding history sequence 11.
Exact commands, exits, hashes and Main-claim reconciliation are retained in
`research/catalogue_v14_r2_checks.json`. No full suite, regeneration, compilation
or private replay was run for r2. The reviewer-only r1 harness assertion is not a
patch finding and was not rerun; fresh independent replay remains the r2 reviewer's
work. Main retains the full-suite gate on this r2 head and adoption.
