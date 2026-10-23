# Catalogue v10 bounded closure proposal

## Disposition: partial, not adopted

**The zero-unknown/zero-anomaly target is not achieved.** This proposal reduces the
adopted v9-r2 baseline from **4,408 unknown bytes / 84 intervals / 37 anomalies** to
**342 unknown bytes / 27 intervals / 2 anomalies**: 4,066 unknown bytes and 35
anomalies are resolved. Against the original v9 proposal (4,417 B / 87 intervals),
the reduction is 4,075 B. Every remaining interval and exact missing prerequisite is
listed below. Nothing unresolved is guessed to be padding, library code or data.

This is denominator/evidence work only: **no adoption, accepted-C credit, ledger
change, matching-source authoring, full build, test suite, lint or formatter run**.
Main owns combined checks and independent review. The five new assembly exclusions
are review proposals, not a claim that arbitrary C implementations are impossible.

## C2f326c verification follow-up: discrete synthetic-image fix

Main's clean detached `c2f326c` full-suite run reported three failures in
`tests.test_analyze` (170 tests): two CLI subcases exposed a genuine generator bug;
the neighboring-pool assertion had an incorrect exclusive-ownership expectation.
Main's real-image CLI generation succeeded and reproduced the original proposal.
Those reported failures were not rerun solely for confirmation.

The closure range writer coalesced adjacent equally labelled `segment_padding`
across mapped segment boundaries, even though its witnesses are text-local.
It now preserves every non-text range verbatim, retaining empty/nonempty segment
and page-padding boundaries. **The tiling validator is unchanged**, not relaxed.
A new synthetic regression covers all empty/nonempty rodata/data combinations.

Intermediate trace diagnosis establishes the other fixture's intended behavior:
the neighbor initially has an actual PC-load pool, but an incoming ADR makes that
word shared, and the inherited address-literal pass removes exclusive pool ownership.
Without optional closure the word is therefore unknown/unowned, not the neighbor's
exclusive function data. With v10 closure, its actual four-byte architectural read
proves unowned rodata. The corrected fixture checks both stages and never borrows
the neighbor's pool. Diagnosis receipt:
`scratch/closure/c2f326c-synthetic-diagnosis.json` (exit 0).

The discrete fixup's exact sequential generation commands are:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-v10-fixup.json
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-v10-fixup-rerun.json
```

Both exit 0 and produce identical 6,514,890-byte outputs, SHA-256
`7b5f857fc160083b100440834e4e92557431962b868d03f5b8e2895c0de5e00c`.
**Functions, physical ranges, anomalies and statistics are identical** to the
original proposal. Only two JSON values change:
`generator.source_sha256.tools/catalogue_closure.py` and
`revision_review.input_sha256.tools/catalogue_closure.py`. Keeping the old overall
hash would incorrectly retain stale generator provenance. Comparison and commands:
`scratch/closure/v10-fixup-generation.json`. Original outputs and receipts below
remain preserved. No test, build, lint or formatter was run for this fixup;
Main owns the clean-checkout suite and independent review.

## Additional initializer-owner evidence: eligibility change deferred

Main subsequently supplied the exact retained r41284 `RESULT.json` and
`MEASUREMENTS.json`. No compiler, matcher or saved failure was rerun. The evidence
**positively establishes natural C representability of two complete repaired
owners**; their current topology exclusion must not be read as non-C/library proof.
It does not provide independent matching credit for the old secondary labels.

| Complete owner | Preserved label | Retained whole-owner evidence | V10 decision |
| --- | --- | --- | --- |
| `0x00212674` / 84 B | `0x00212684` | `game`, `middleware`, `sdk`: exit 0, exact 84/84 B, no first difference, no linker rewrites; native array construction/mutex source with `names-array.json` | Keep labels and current topology pending explicit eligibility-model review |
| `0x002136c4` / 56 B | `0x002136d0` | All three frozen profiles: exit 0, exact 56/56 B under approved v2 model; one `inline_nop` rewrite each; natural two-registration source | Same deferral; do not describe these as rewrite-free raw equality |
| `0x00214fe8` / 312 B | `0x00214fec` | Supplied notes retain a natural primary mismatch, 296/312 B with 224 differing bytes | No equivalent whole-owner MATCH evidence in this record |
| `0x001f5980` | `0x001f5990` | Not covered by the two supplied r41284 inputs | No assertion that another author's equivalent evidence does not exist |
| `0x002069ec` | `0x002069f0` | Not covered by the two supplied r41284 inputs | Same qualification |

The first matched source is
`scratch/authors/r41284/src/func_00212674_mixed.cpp`; its per-profile receipt family is
`scratch/authors/r41284/match/func_00212684/03-array-mutex-boundary-<profile>/func_00212674_mixed.result.json`.
The second is `scratch/authors/r41284/src/func_002136c4.c`, with receipt family
`scratch/authors/r41284/match/func_002136d0/01-current-owner-<profile>/func_002136c4.result.json`.
The final game-profile repeats are also preserved. Author notes explicitly retain
canonical shared-header/callback/name-contract review for Main before integration.
The standalone legacy rows remain refused/mismatched; these successes belong to the
complete primary owners, not separately callable old-label functions.

**Decision: do not change v10's catalogue or generator for this new evidence.**
The current validator explicitly rejects ordinary owners with non-primary entries
(`invalid_secondary_entry_kind`); preservation converts retained weak prologue labels
into secondary entries and the associated owner into `multi_entry`. Progress and
credit consumers use the owner kind. Restoring ordinary eligibility while keeping
all labels therefore requires an explicit interior-label versus independently
callable-entry contract and a coherent consumer/generation/preservation cutover,
not an address exception, deleting a label or silently relaxing a validation gate.
That policy change is deferred to a later revision under the frozen-gate contract.
[INFERENCE] These two complete C witnesses support considering that future change;
they are not a universal claim about absence of every possible external interior
caller. Genuine incoming-entry evidence must still veto such a restoration.

If only these two owners are later restored, the denominator change is **+2 owners /
+140 extent B**, topology owners **25 → 23**, without changing physical ownership,
unknown bytes, any entry identity or accepted credit. V10 remains **4,662 ordinary
owners / 1,133,322 B**, 342 unknown B and two anomalies. Its latest generated proposal
remains the `6c5aa76` fixup SHA above; this evaluation changes documentation only.
Exact retained rows, input hashes and the deferral decision are preserved in
`scratch/closure/initializer-eligibility-evaluation.json`.

Supplied evidence identities:

- `scratch/authors/r41284/RESULT.json`: SHA-256 `27205d78ed19537e6b1813ec0da6cf0adb820b9dd6ca5c56b77b58dc0ba26201`.
- `scratch/authors/r41284/MEASUREMENTS.json`: SHA-256 `514e2c5a89f8ad8ca2c5e2840ee793af16ad5fd2873d6e1a81ad1053c57faaf1`.

## Baselines and reproducible output

- Private worktree/branch: `scratch/worktrees/catalogue-v10`, `catalogue-v10`,
  originally created from catalogue-v9 `9b1808944d2fed376aa04f523d15c13bb8bd4dbc`.
- V9-R01/R02 fixes were first isolated as `4891c6d6b923f3e510001a41f5f19ecfa8e45c04`;
  the new-input test-fixture fix was isolated as
  `a930af3c65ff0a74fa4596b80e42038ae3788a26`. Main subsequently adopted v9-r2.
- Main's adopted baseline `959311b` was merged into this private branch before v10
  generation. Its immutable catalogue blob is
  `babbff6901bbcaeb388cf305623aba587db5ad50`, SHA-256
  `d13a8c794676c135b3bcd03676bf1803c5b5215455428ee4177ed911311c3d64`.
  A fixed Git blob, not a live-config hash pin, supplies the historical accounting;
  regeneration after adoption therefore does not require replacing the baseline input.
- Original v9 proposal SHA-256:
  `9b13a770438c91b48ea34d8b2445f41c87ae2543a696618360a0ff550ddfe460`.
  `research/catalogue_v10_v9_proposal.json` preserves every identity, entry and
  full-record digest plus all 87 old intervals and 37 anomalies (metadata only).
- Existing `orig` and `.venv` are linked to Main. Both the private `.cache/toolchain`
  and convenience `toolchain` links point to Main's existing `.cache/toolchain`.
  No installation or private compiler-cache duplication occurred.

From the private worktree, the exact commands were executed sequentially with
`subprocess.run`, captured output and recorded return codes (no piped gates):

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-v10.json
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-v10-rerun.json
```

Both exit **0**; complete outputs are byte-identical, **6,514,890 B**, SHA-256
`d0ab9585b0296662021c9bf38d18e1b7629dc650aafd35ef28cd33c171231c9f`.
Captured wall times are 34.076 s and 34.079 s. Receipts:
`scratch/closure/final-generation.json`. Full old-range/anomaly/record differences,
remaining-prerequisite facts and private evidence hashes:
`scratch/closure/DIFFERENCES-v10.json`. The proposal itself includes the same
machine-readable adopted/original accounting in `revision_review.previous_catalogue`
and `revision_review.superseded_proposal`.

The generator reads only committed, digest-bound evidence plus private original
inputs and immutable Git blobs; it does not scan mutable author-result directories.
The parent `research/catalogue_v9_r2_evidence.json` is preserved unchanged. New
witnesses are in `research/catalogue_v10_evidence.json`. Generator/tool/input hashes
are embedded in the output.

## Population, preservation and denominator accounting

| Owner kind | Adopted v9-r2 | V10 proposal |
| --- | ---: | ---: |
| assembly | 11 | 16 |
| function | 4624 | 4662 |
| multi_entry | 24 | 25 |
| runtime | 22 | 22 |
| veneer | 18 | 18 |

- Owners: **4,699 → 4,743**. Ordinary candidates: **4,624 → 4,662**.
  Ordinary extent denominator: **1,131,454 → 1,133,322 B** (+1,868 B).
- **All 4,730 prior `(entry address, ISA)` pairs survive**; there are 4,776 entries.
  The complete protected union (accepted work and proposed c004-c008 identities,
  including Main's frozen c008 checkpoint) is unchanged. Every protected source's
  recorded change list is empty; no bundled classification was withheld.
- 4,696 of 4,699 prior `(address, size, ISA)` identities are identical. Exactly
  three unprotected identity repairs are explicit: two weak prologue owners become
  preserved secondary labels in one entry-connected owner, and one typed literal
  extends its sole unprotected owner by 4 B. No genuine entry is erased.
- New ordinary leaves: **42 / 2,560 B**. New closed Thumb callbacks: **3 / 36 B**.
  Sole-owner typed extent repair: **+4 B**. Five individually reviewed assembly
  proposals: **−5 / −104 B** from the ordinary denominator.
- **Topology is separate from non-C ownership:** the connected group replaces two
  ordinary weak guesses / 628 extent B with one multi-entry owner, subtracting
  those two ordinary denominator records, not classifying them as runtime/library.
  Thus +2,560 +36 +4 −104 −628 = **+1,868 ordinary extent B**.
  Multi-entry owners increase **24 → 25**; runtime owners remain **22**.
- Physical-map deltas are distinct from extent-denominator deltas: function-owned
  bytes +4,076; rodata +24; padding −34; unknown −4,066; data/segment padding unchanged.
  Complete typed objects absorb previously inferred zero alignment as well as old
  unknown bytes; newly proved halfword alignment is separately unowned padding.
  No padding-pattern guess is used to manufacture closure.
- Structural issues, protected changes, unexpected entry losses, unexpected extent
  changes and new anomalies are all empty in the generated review metadata.

## Newly evidenced closures

### 1. Fixed-copy and fixed-record leaves

`constant_copy_leaf` proves complete straight-line memory effects, not plausible
instruction decoding. It enumerates scalar/multiple-register/single-precision
caller-saved VFP loads/stores; requires balanced stack and preserved LR/callee core
registers; and ends at an ordinary LR return. Calls, predicates, flags-changing
arithmetic, indexed accesses, unknown instructions and preserved VFP aliases fail
closed. Ordinary copies require equal contiguous source/destination effects.
The explicit fixed-record mode additionally permits bounded permutations/subsets,
last-wins stores, immediate fields and incoming r2/r3 fields; holes remain explicit.
It does not admit arithmetic value computation or variable footprints.

- Fixed-copy gaps around `0x00184000–0x00184700`: **15 leaves / 1,108 B**.
- Fixed-record gaps around `0x0018e000–0x0018f000`: **27 leaves / 1,452 B**.
- The remaining **320 B** in the latter original gaps are recovered by the
  entry-connected graph below, not by inventing unrelated tiny leaves.

[INFERENCE] Historical source spelling, callers and library/compiler ownership of
these leaves are not established. They remain ordinary, uncredited candidates.

### 2. Original entry-connected memory graph, retaining every legacy label

The entry at `0x0018eaec` reaches a complete **924-B / 231-instruction** normal ARM
CFG through `0x0018ee88`, without seeding the two interior prologues. It has no pools,
open exits, calls or decode issues and only one known returning ARM tail to
`0x0018e764`. Original span digest and exact legacy owner set are frozen. The prior
weak owners `0x0018ebf8` (104 B) and `0x0018ec7c` (524 B) were unprotected,
prologue-only, single-entry records with no incoming strong-entry evidence.
Every old owned span is retained. Both labels survive as `legacy_prologue`
secondary entries; this conservatively makes the result `multi_entry`.

The retained original-body Unicorn harness was reused, not replaced by a matching
source. With the real known tail included, **1,156 cases** (460 disjoint, 696
potentially overlapping) have exact reference memory effects, return to the sentinel,
restored SP and preserved callee registers, without external writes/emulation errors.
There are **437 executed instruction sites including the original tail**;
**221/231 new-graph sites** are dynamically exercised. Ten new-graph sites are not
exercised; finite behavior is not universal algorithm/ABI proof. The full static CFG
is independently closed. [INFERENCE] A historical memmove-like identity and original
source-level signature are not claimed by that finite experiment.

Evidence: `scratch/closure/memory-graph-unseeded.json`,
`memory_graph_behavior.py`, `memory-graph-behavior-expanded.json`. The earlier narrow
experiment is preserved: 612 cases, with 404 stopping at the legitimate external
original tail because of its then-too-narrow hook window. It is not represented as
an algorithm failure or a passing full witness. The first catalogue attempt also
remains recorded (exit 1: conservative normal-instruction allowlist lacked the
ordinary decrement-after multiple-store class); after that explicit rule change,
subsequent generation exits 0. No exception or unresolved transfer was suppressed.

### 3. Exact typed UTF-16 objects, not encoding guesses or borrowed pools

The preserved natural source/object for `0x00203a68` supplies **23 complete UTF-16
records / 1,234 B** at actual address-reference-derived relative offsets. Each
original interval equals its retained object interval, with no overlapping
relocations; exact bounds/digests/zero alignment are frozen. All **1,012 unknown B**
inside that owner are explained without changing its extent. Source and object:

- `scratch/authors/s203a68/src/func_00203a68.cpp`, SHA-256
  `a51054e168ed05dc4a23594ee1728a698a4bdd893f5d0c507a4a6448601e3829`.
- `scratch/authors/s203a68/match/018-final-game/func_00203a68.o`, SHA-256
  `4500343aac7981469da56b2c17e098b7dbb424cb4fabaa3563b068531732f9c7`.

The whole source remains unaccepted because its external pool ordering is not a
whole-function match; this is bounded data evidence only. No matcher was repeated.
Ten other retained source-typed records / 142 B across eight owners are frozen
with exact reference sites and halfword terminators. Only `0x001b55f8` changes
extent (1,068 → 1,072 B): a complete sole-owner wide record ending at `0x001b5a28`.
Code/pool/entry overlap vetoes remain active.

The inherited shared records at `0x001b54e4`, `0x001b8234`, `0x001bc29c` remain
complete **4-B UTF-16 objects**, shared/unowned, not one-byte narrow empty strings.
The precise r4023 EXTENT witness is retained:
`scratch/authors/r4023/variants/func_001bc220_wc2.c` and
`scratch/authors/r4023/match/func_001bc220/wc2-game/func_001bc220_wc2.result.json`:
`game`, `v2/none`, 128 candidate B versus 124 owner B, `first_diff: null`, no rewrites.
It reopened encoding/bounds/sharing independently; it did **not** authorize extending
exclusive ownership of `0x001bc220`, whose 124-B record remains unchanged.

### 4. Thumb entry and cross-owner read/alignment evidence

Three dominated odd PC-relative pointer values recover callbacks at
`0x001005fa` (16 B), `0x001005c0` (10 B), `0x001009d4` (10 B), each with a complete
closed CFG. Constants flow only through proved definitions and never through joins,
predicates, unknown arithmetic or call-clobbered registers. Unreferenced return-only
halfwords elsewhere remain unknown.

Actual fixed-width reads explain the cross-owner footprints at `0x001005bc` and
`0x0019fd14–0x0019fd20`. They do not extend loading-owner extents or borrow neighboring
pools. ADR alone is not a read. A complete addressed NUL object outside even its sole
owner can become unowned rodata only after whole-span code/pool/entry vetoes.
Exact zero halfword alignment after a terminal before an evidenced aligned
pool/literal/entry resolves the two named owner holes inside `0x0019f5c4` and
`0x0019f63c`, among the nine proved alignment sites. Referenced bytes and nonzero
fill cannot qualify. The exploratory unconditional-callee-prefix machinery supplied
no additional final ownership beyond simpler direct witnesses and was removed.

### 5. Five separately reviewable no-LR-frame kernel proposals

Main requested inclusion of KernelStubProof's proposals from
`scratch/worktrees/kernel-stub-proof/scratch/kernel-stub-proof/classification-proposal.json`.
The frozen v10 evidence preserves each complete original extent digest, positive ABI
constraints and all 18 retained compile/link result cells per function (90 total).
All 90 are byte mismatches at the initial frame instruction, not compiler failures.
No new compiler or matcher was run for this catalogue proposal.

| Entry | B | Syscall contract |
| --- | ---: | --- |
| `0x001109b4` | 24 | CreateAddressArbiter |
| `0x0011ef58` | 8 | SleepThread, signed 64-bit duration |
| `0x0012d0ec` | 24 | ConnectToPort |
| `0x0012d150` | 24 | GetProcessId |
| `0x0018f7d4` | 24 | CreateEvent |

The same function-specific no-LR-save compiler-shape standard as v9's three existing
records is proposed. Four use a 4-B pointer stack slot: they are not frame-free.
Raw r0-r3 syscall interfaces are representable; all retained cells compile/link.
The finite grid does not prove universal C impossibility. [INFERENCE] Historical SDK
or handwritten assembly authorship is not established. Public SDK assembly build
failures remain archived failures, not executable witnesses. Independent review must
adjudicate these five ordinary→assembly denominator exclusions separately from the
new topology exclusion. All accepted/proposed protected owners are unchanged.

## Exact original-v9 and adopted-v9-r2 interval conservation

All intervals are half-open. The following **87 rows** account for every original
v9 unknown byte. The three rows already resolved by adopted v9-r2 are explicitly
marked; the remaining **84 rows are exactly the adopted baseline's unknown table**.
The machine-readable ledgers retain each original interval digest and every exact
replacement part, including distinctions between ordinary, multi-entry, rodata,
padding and still unknown. Physical `function` ownership includes evidenced inline
objects and is not synonymous with instruction bytes or accepted C credit.

| Original v9 interval (half-open) | B | Adopted v9-r2 | V10 disposition and exact replacement parts |
| --- | ---: | --- | --- |
| 0x00100020–0x00100024 | 4 | retained | **open**: 0x00100020–0x00100024 unknown |
| 0x00100076–0x00100078 | 2 | retained | **open**: 0x00100076–0x00100078 unknown |
| 0x0010012e–0x0010015e | 48 | retained | **open**: 0x0010012e–0x0010015e unknown |
| 0x00100396–0x00100398 | 2 | retained | **resolved**: 0x00100396–0x00100398 padding |
| 0x0010051e–0x00100520 | 2 | retained | **open**: 0x0010051e–0x00100520 unknown |
| 0x001005bc–0x001005ca | 14 | retained | **resolved**: 0x001005bc–0x001005c0 rodata; 0x001005c0–0x001005ca function/function@0x001005c0 |
| 0x001005ea–0x001005ec | 2 | retained | **resolved**: 0x001005ea–0x001005ec padding |
| 0x001005fa–0x0010060a | 16 | retained | **resolved**: 0x001005fa–0x0010060a function/function@0x001005fa |
| 0x001007ce–0x001007d0 | 2 | retained | **resolved**: 0x001007ce–0x001007d0 padding |
| 0x001008c6–0x001008c8 | 2 | retained | **resolved**: 0x001008c6–0x001008c8 padding |
| 0x0010090a–0x0010090c | 2 | retained | **open**: 0x0010090a–0x0010090c unknown |
| 0x00100942–0x00100944 | 2 | retained | **open**: 0x00100942–0x00100944 unknown |
| 0x001009d4–0x001009de | 10 | retained | **resolved**: 0x001009d4–0x001009de function/function@0x001009d4 |
| 0x001009fe–0x00100a00 | 2 | retained | **resolved**: 0x001009fe–0x00100a00 padding |
| 0x00100a6a–0x00100a6c | 2 | retained | **open**: 0x00100a6a–0x00100a6c unknown |
| 0x00100ada–0x00100adc | 2 | retained | **resolved**: 0x00100ada–0x00100adc padding |
| 0x00100ae0–0x00100ae4 | 4 | retained | **resolved**: 0x00100ae0–0x00100ae4 rodata |
| 0x00100b26–0x00100b28 | 2 | retained | **open**: 0x00100b26–0x00100b28 unknown |
| 0x00100c7a–0x00100c7c | 2 | retained | **resolved**: 0x00100c7a–0x00100c7c padding |
| 0x00100c92–0x00100c94 | 2 | retained | **open**: 0x00100c92–0x00100c94 unknown |
| 0x00100d70–0x00100d74 | 4 | retained | **open**: 0x00100d70–0x00100d74 unknown |
| 0x00104720–0x00104740 | 32 | retained | **resolved**: 0x00104720–0x00104740 function/function@0x00104458 |
| 0x0010d3a8–0x0010d3b0 | 8 | retained | **open**: 0x0010d3a8–0x0010d3b0 unknown |
| 0x001415b4–0x001415b8 | 4 | retained | **open**: 0x001415b4–0x001415b8 unknown |
| 0x001569b0–0x00156a30 | 128 | retained | **open**: 0x001569b0–0x00156a30 unknown |
| 0x0016067c–0x00160684 | 8 | retained | **resolved**: 0x0016067c–0x00160684 function/function@0x00160454 |
| 0x0016b6a2–0x0016b6a4 | 2 | retained | **open**: 0x0016b6a2–0x0016b6a4 unknown |
| 0x00181eaa–0x00181eac | 2 | retained | **open**: 0x00181eaa–0x00181eac unknown |
| 0x00184204–0x0018423c | 56 | retained | **resolved**: 0x00184204–0x00184220 function/function@0x00184204; 0x00184220–0x0018423c function/function@0x00184220 |
| 0x00184278–0x001842d8 | 96 | retained | **resolved**: 0x00184278–0x001842a8 function/function@0x00184278; 0x001842a8–0x001842d8 function/function@0x001842a8 |
| 0x0018433c–0x001846f8 | 956 | retained | **resolved**: 0x0018433c–0x00184380 function/function@0x0018433c; 0x00184380–0x001843c4 function/function@0x00184380; 0x001843c4–0x00184408 function/function@0x001843c4; 0x00184408–0x00184450 function/function@0x00184408; 0x00184450–0x001844a8 function/function@0x00184450; 0x001844a8–0x00184500 function/function@0x001844a8; 0x00184500–0x00184558 function/function@0x00184500; 0x00184558–0x001845b4 function/function@0x00184558; 0x001845b4–0x00184620 function/function@0x001845b4; 0x00184620–0x0018468c function/function@0x00184620; 0x0018468c–0x001846f8 function/function@0x0018468c |
| 0x0018e0dc–0x0018e14c | 112 | retained | **resolved**: 0x0018e0dc–0x0018e0f8 function/function@0x0018e0dc; 0x0018e0f8–0x0018e120 function/function@0x0018e0f8; 0x0018e120–0x0018e14c function/function@0x0018e120 |
| 0x0018e1c0–0x0018e208 | 72 | retained | **resolved**: 0x0018e1c0–0x0018e208 function/function@0x0018e1c0 |
| 0x0018e270–0x0018e764 | 1268 | retained | **resolved**: 0x0018e270–0x0018e290 function/function@0x0018e270; 0x0018e290–0x0018e2bc function/function@0x0018e290; 0x0018e2bc–0x0018e2ec function/function@0x0018e2bc; 0x0018e2ec–0x0018e328 function/function@0x0018e2ec; 0x0018e328–0x0018e368 function/function@0x0018e328; 0x0018e368–0x0018e3b4 function/function@0x0018e368; 0x0018e3b4–0x0018e404 function/function@0x0018e3b4; 0x0018e404–0x0018e420 function/function@0x0018e404; 0x0018e420–0x0018e440 function/function@0x0018e420; 0x0018e440–0x0018e46c function/function@0x0018e440; 0x0018e46c–0x0018e49c function/function@0x0018e46c; 0x0018e49c–0x0018e4d8 function/function@0x0018e49c; 0x0018e4d8–0x0018e518 function/function@0x0018e4d8; 0x0018e518–0x0018e564 function/function@0x0018e518; 0x0018e564–0x0018e5b4 function/function@0x0018e564; 0x0018e5b4–0x0018e5d0 function/function@0x0018e5b4; 0x0018e5d0–0x0018e5f0 function/function@0x0018e5d0; 0x0018e5f0–0x0018e61c function/function@0x0018e5f0; 0x0018e61c–0x0018e64c function/function@0x0018e61c; 0x0018e64c–0x0018e688 function/function@0x0018e64c; 0x0018e688–0x0018e6c8 function/function@0x0018e688; 0x0018e6c8–0x0018e714 function/function@0x0018e6c8; 0x0018e714–0x0018e764 function/function@0x0018e714 |
| 0x0018eaec–0x0018ebf8 | 268 | retained | **resolved**: 0x0018eaec–0x0018ebf8 function/multi_entry@0x0018eaec |
| 0x0018ec60–0x0018ec7c | 28 | retained | **resolved**: 0x0018ec60–0x0018ec7c function/multi_entry@0x0018eaec |
| 0x0018ed0c–0x0018ed18 | 12 | retained | **resolved**: 0x0018ed0c–0x0018ed18 function/multi_entry@0x0018eaec |
| 0x0018eda8–0x0018edb4 | 12 | retained | **resolved**: 0x0018eda8–0x0018edb4 function/multi_entry@0x0018eaec |
| 0x001911ee–0x001911f0 | 2 | retained | **open**: 0x001911ee–0x001911f0 unknown |
| 0x00191c92–0x00191c94 | 2 | retained | **open**: 0x00191c92–0x00191c94 unknown |
| 0x001944ac–0x001944e8 | 60 | retained | **open**: 0x001944ac–0x001944e8 unknown |
| 0x001964e8–0x00196504 | 28 | retained | **open**: 0x001964e8–0x00196504 unknown |
| 0x0019d5c2–0x0019d5c4 | 2 | retained | **open**: 0x0019d5c2–0x0019d5c4 unknown |
| 0x0019f622–0x0019f624 | 2 | retained | **resolved**: 0x0019f622–0x0019f624 padding |
| 0x0019f696–0x0019f698 | 2 | retained | **resolved**: 0x0019f696–0x0019f698 padding |
| 0x0019f6d6–0x0019f6d8 | 2 | retained | **open**: 0x0019f6d6–0x0019f6d8 unknown |
| 0x0019f910–0x0019f912 | 2 | retained | **open**: 0x0019f910–0x0019f912 unknown |
| 0x0019fd14–0x0019fd20 | 12 | retained | **resolved**: 0x0019fd14–0x0019fd20 rodata |
| 0x0019fd6a–0x0019fd6c | 2 | retained | **open**: 0x0019fd6a–0x0019fd6c unknown |
| 0x0019ffa2–0x0019ffa4 | 2 | retained | **open**: 0x0019ffa2–0x0019ffa4 unknown |
| 0x001a00b4–0x001a00b8 | 4 | retained | **open**: 0x001a00b4–0x001a00b8 unknown |
| 0x001ad598–0x001ad59c | 4 | retained | **resolved**: 0x001ad598–0x001ad59c function/function@0x001ad3c8 |
| 0x001b54e5–0x001b54e8 | 3 | already resolved in v9-r2 | **resolved**: 0x001b54e5–0x001b54e8 rodata |
| 0x001b5a1c–0x001b5a20 | 4 | retained | **resolved**: 0x001b5a1c–0x001b5a20 function/function@0x001b55f8 |
| 0x001b5a24–0x001b5a28 | 4 | retained | **resolved**: 0x001b5a24–0x001b5a28 function/function@0x001b55f8 |
| 0x001b8235–0x001b8238 | 3 | already resolved in v9-r2 | **resolved**: 0x001b8235–0x001b8238 rodata |
| 0x001bc29d–0x001bc2a0 | 3 | already resolved in v9-r2 | **resolved**: 0x001bc29d–0x001bc2a0 rodata |
| 0x001c3c34–0x001c3c48 | 20 | retained | **resolved**: 0x001c3c34–0x001c3c48 function/function@0x001c3ac0 |
| 0x001c7748–0x001c775c | 20 | retained | **open**: 0x001c7748–0x001c775c unknown |
| 0x001cd2e6–0x001cd2e8 | 2 | retained | **open**: 0x001cd2e6–0x001cd2e8 unknown |
| 0x001de4e0–0x001de4e8 | 8 | retained | **resolved**: 0x001de4e0–0x001de4e8 function/function@0x001de2c0 |
| 0x001e84dc–0x001e84e8 | 12 | retained | **resolved**: 0x001e84dc–0x001e84e8 function/function@0x001e81fc |
| 0x001ef550–0x001ef554 | 4 | retained | **resolved**: 0x001ef550–0x001ef554 function/function@0x001edbb0 |
| 0x001ef558–0x001ef55c | 4 | retained | **resolved**: 0x001ef558–0x001ef55c function/function@0x001edbb0 |
| 0x00203ce4–0x00203d10 | 44 | retained | **resolved**: 0x00203ce4–0x00203d10 function/function@0x00203a68 |
| 0x00203d1c–0x00203d4c | 48 | retained | **resolved**: 0x00203d1c–0x00203d4c function/function@0x00203a68 |
| 0x00203d54–0x00203d84 | 48 | retained | **resolved**: 0x00203d54–0x00203d84 function/function@0x00203a68 |
| 0x00203d94–0x00203db4 | 32 | retained | **resolved**: 0x00203d94–0x00203db4 function/function@0x00203a68 |
| 0x00203dc0–0x00203dfc | 60 | retained | **resolved**: 0x00203dc0–0x00203dfc function/function@0x00203a68 |
| 0x00203e04–0x00203e2c | 40 | retained | **resolved**: 0x00203e04–0x00203e2c function/function@0x00203a68 |
| 0x00203e38–0x00203e60 | 40 | retained | **resolved**: 0x00203e38–0x00203e60 function/function@0x00203a68 |
| 0x00203e6c–0x00203e94 | 40 | retained | **resolved**: 0x00203e6c–0x00203e94 function/function@0x00203a68 |
| 0x00203eb8–0x00203ee0 | 40 | retained | **resolved**: 0x00203eb8–0x00203ee0 function/function@0x00203a68 |
| 0x00203eec–0x00203f28 | 60 | retained | **resolved**: 0x00203eec–0x00203f28 function/function@0x00203a68 |
| 0x00203f34–0x00203f64 | 48 | retained | **resolved**: 0x00203f34–0x00203f64 function/function@0x00203a68 |
| 0x00203f6c–0x00203f9c | 48 | retained | **resolved**: 0x00203f6c–0x00203f9c function/function@0x00203a68 |
| 0x00203fb4–0x00203fd8 | 36 | retained | **resolved**: 0x00203fb4–0x00203fd8 function/function@0x00203a68 |
| 0x00203ff0–0x00204024 | 52 | retained | **resolved**: 0x00203ff0–0x00204024 function/function@0x00203a68 |
| 0x00204034–0x00204064 | 48 | retained | **resolved**: 0x00204034–0x00204064 function/function@0x00203a68 |
| 0x00204254–0x0020427c | 40 | retained | **resolved**: 0x00204254–0x0020427c function/function@0x00203a68 |
| 0x00204288–0x002042c0 | 56 | retained | **resolved**: 0x00204288–0x002042c0 function/function@0x00203a68 |
| 0x002042e4–0x002042f8 | 20 | retained | **resolved**: 0x002042e4–0x002042f8 function/function@0x00203a68 |
| 0x00204324–0x0020435c | 56 | retained | **resolved**: 0x00204324–0x0020435c function/function@0x00203a68 |
| 0x00204364–0x0020438c | 40 | retained | **resolved**: 0x00204364–0x0020438c function/function@0x00203a68 |
| 0x00204394–0x002043c8 | 52 | retained | **resolved**: 0x00204394–0x002043c8 function/function@0x00203a68 |
| 0x002043e0–0x002043f0 | 16 | retained | **resolved**: 0x002043e0–0x002043f0 function/function@0x00203a68 |
| 0x00204414–0x00204444 | 48 | retained | **resolved**: 0x00204414–0x00204444 function/function@0x00203a68 |

## Exact original/adopted anomaly accounting

Both baselines have the same 37 issues: **35 resolved, 2 remain**. No new issue is
introduced. `extent_separately_explained...` means independently bounded rodata or
alignment explains the hole; it does not add function/accepted-C ownership. The
entry-connected recovery instead proves the old sparse hole as reachable code.

| Address | Original/adopted issue | V10 | Evidence / remaining prerequisite |
| --- | --- | --- | --- |
| 0x00100360 | unexplained_function_bytes | resolved | extent_separately_explained_by_bounded_rodata_or_alignment_not_function_credit |
| 0x00100372 | address_literal_crosses_start | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x00100372 | address_literal_overlaps_code | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x00100532 | literal_outside_function | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x00100538 | literal_outside_function | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x001005ca | unexplained_function_bytes | resolved | extent_separately_explained_by_bounded_rodata_or_alignment_not_function_credit |
| 0x0010064c | unexplained_function_bytes | resolved | extent_separately_explained_by_bounded_rodata_or_alignment_not_function_credit |
| 0x001008ac | unexplained_function_bytes | resolved | extent_separately_explained_by_bounded_rodata_or_alignment_not_function_credit |
| 0x001009de | unexplained_function_bytes | resolved | extent_separately_explained_by_bounded_rodata_or_alignment_not_function_credit |
| 0x00100a9a | address_literal_crosses_start | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x00100a9a | literal_outside_function | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x00100aac | address_literal_crosses_start | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x00100aac | literal_outside_function | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x00100bb0 | unexplained_function_bytes | resolved | extent_separately_explained_by_bounded_rodata_or_alignment_not_function_credit |
| 0x00104458 | unexplained_function_bytes | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x0010d2b8 | unexplained_function_bytes | open | Sparse process-exit continuation ownership remains unproved (8 B). |
| 0x00145570 | unexplained_function_bytes | resolved | extent_separately_explained_by_bounded_rodata_or_alignment_not_function_credit |
| 0x001456e4 | unexplained_function_bytes | resolved | extent_separately_explained_by_bounded_rodata_or_alignment_not_function_credit |
| 0x00155208 | unexplained_function_bytes | resolved | extent_separately_explained_by_bounded_rodata_or_alignment_not_function_credit |
| 0x00160454 | unexplained_function_bytes | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x0018ec7c | unexplained_function_bytes | resolved | entry_connected_closed_cfg_recovers_sparse_code_with_all_legacy_labels_retained |
| 0x001964e8 | undecodable_reachable_code | open | Exceptional descriptor width/format and original dispatcher nonreturn/continuation semantics remain unproved (28 B). |
| 0x0019f5c4 | unexplained_function_bytes | resolved | extent_separately_explained_by_bounded_rodata_or_alignment_not_function_credit |
| 0x0019f63c | unexplained_function_bytes | resolved | extent_separately_explained_by_bounded_rodata_or_alignment_not_function_credit |
| 0x0019f958 | literal_outside_function | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x0019f95a | literal_outside_function | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x0019f9ce | literal_outside_function | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x0019f9d8 | literal_outside_function | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x001ad3c8 | unexplained_function_bytes | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x001b55f8 | unexplained_function_bytes | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x001c3ac0 | unexplained_function_bytes | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x001de2c0 | unexplained_function_bytes | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x001e81fc | unexplained_function_bytes | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x001edbb0 | unexplained_function_bytes | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |
| 0x001f7d7c | unexplained_function_bytes | resolved | extent_separately_explained_by_bounded_rodata_or_alignment_not_function_credit |
| 0x00202ca8 | unexplained_function_bytes | resolved | extent_separately_explained_by_bounded_rodata_or_alignment_not_function_credit |
| 0x00203a68 | unexplained_function_bytes | resolved | Exact shared read footprint, typed inline object, aligned hole or closed tagged-pointer CFG resolves this issue |

## Remaining 342 B / 27 intervals: precise blockers

These are genuine unresolved prerequisites, not a claim that closure is complete.
Retained evidence includes `residual-census.json`, `thumb-object-alignment.json`,
`indexed-read-bound-census.json`, and `bounded-index-probe.json`, with commands,
exit codes, hashes and structural facts. Both indexed-read investigation receipts
record exit 0; a successful investigation command does not make an unresolved proof pass.
The indexed probe's abstract intervals exceed mapped image bounds, so no indexed-data
closure rule is installed. Six existing Thumb objects do not authenticate the
15 all-ones halfword placement decisions. Compiler payload inventory provides no
matching runtime archive for a library-object extent witness. No installation or
byte-pattern ownership heuristic substitutes for these missing facts.

| Remaining interval (half-open) | B | Exact unresolved prerequisite and evidence tried |
| --- | ---: | --- |
| 0x00100020–0x00100024 | 4 | A bounded closed instruction fragment exists, but no executable-entry/caller/section witness establishes its independent identity. Return-only/tail-only or plausible closed decoding is not used to invent an entry or padding. |
| 0x00100076–0x00100078 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x0010012e–0x0010015e | 48 | A bounded closed instruction fragment exists, but no executable-entry/caller/section witness establishes its independent identity. Return-only/tail-only or plausible closed decoding is not used to invent an entry or padding. |
| 0x0010051e–0x00100520 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x0010090a–0x0010090c | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x00100942–0x00100944 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x00100a6a–0x00100a6c | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x00100b26–0x00100b28 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x00100c92–0x00100c94 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x00100d70–0x00100d74 | 4 | A bounded closed instruction fragment exists, but no executable-entry/caller/section witness establishes its independent identity. Return-only/tail-only or plausible closed decoding is not used to invent an entry or padding. |
| 0x0010d3a8–0x0010d3b0 | 8 | Eight-byte sparse hole in 0x0010d2b8 after process-exit handling: a closed instruction fragment is not an evidenced independent entry, bounded data object or section-owned dead continuation. Original process-exit behavior alone does not establish lexical ownership of unreachable bytes. |
| 0x001415b4–0x001415b8 | 4 | A bounded closed instruction fragment exists, but no executable-entry/caller/section witness establishes its independent identity. Return-only/tail-only or plausible closed decoding is not used to invent an entry or padding. |
| 0x001569b0–0x00156a30 | 128 | A real indexed read uses an address base before this gap, not a directly bounded object reference. The local abstract-value probe cannot bound its index within the image; no preserved matching library object/source or complete format bound is available. Do not borrow the preceding owner or classify this as a lookup table by appearance. |
| 0x0016b6a2–0x0016b6a4 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x00181eaa–0x00181eac | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x001911ee–0x001911f0 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x00191c92–0x00191c94 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x001944ac–0x001944e8 | 60 | A real indexed read uses an address base before this gap, not a directly bounded object reference. The local abstract-value probe cannot bound its index within the image; no preserved matching library object/source or complete format bound is available. The installed compiler inventory has executable/header payloads but no matching runtime archive. Do not borrow the preceding owner or classify this as a lookup table by appearance. |
| 0x001964e8–0x00196504 | 28 | Exceptional-dispatch return-address descriptor: missing exact descriptor width/format and original 0x00145570 nonreturn/continuation contract. The retained 404 normal f2ul ABI cases exclude the exceptional path. A BL-adjacent record and plausible trailing instructions are not a closed entry/data witness. |
| 0x0019d5c2–0x0019d5c4 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x0019f6d6–0x0019f6d8 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x0019f910–0x0019f912 | 2 | A bounded closed instruction fragment exists, but no executable-entry/caller/section witness establishes its independent identity. Return-only/tail-only or plausible closed decoding is not used to invent an entry or padding. |
| 0x0019fd6a–0x0019fd6c | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x0019ffa2–0x0019ffa4 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x001a00b4–0x001a00b8 | 4 | Retained ARM/Thumb CFG probes and reference census supply no complete executable-entry, bounded referenced-data/format or section-alignment witness. The legal owner/boundary remains unproved. |
| 0x001c7748–0x001c775c | 20 | Retained ARM/Thumb CFG probes and reference census supply no complete executable-entry, bounded referenced-data/format or section-alignment witness. The legal owner/boundary remains unproved. |
| 0x001cd2e6–0x001cd2e8 | 2 | Retained ARM/Thumb CFG probes and reference census supply no complete executable-entry, bounded referenced-data/format or section-alignment witness. The legal owner/boundary remains unproved. |

The two retained anomalies are `unexplained_function_bytes` at `0x0010d2b8` (the
8-B continuation hole) and `undecodable_reachable_code` at `0x001964e8` (the
28-B exceptional-dispatch interval). The other unknown intervals have no anomaly
because absence of an owner is accounted separately; absence of a warning is not
proof of ownership. No missing entry/symbol, linker layout or dispatcher contract
has been invented to force the target to zero.

## Regression changes and handoff

`tests/test_analyze.py` now supplies every new bundled input in its synthetic CLI
fixture, authenticates the mocked immutable historical baseline, and updates the
whole-image expectations and adopted-v10 regeneration path. Added positive/negative
regressions cover fixed-copy/record effects, predication/calls/indexing/flags,
callee/stack/VFP clobbers, nonlocal code/pool protection, exact UTF-16 bounds/digests/
zero tails, dominated Thumb pointer definitions, referenced/nonzero alignment vetoes,
protected/strong/data/system-instruction graph-merge vetoes, whole-original-extent
classification digests and independent parent/baseline identities.

**These tests were authored, not executed.** No test/build/lint/formatter was run in
this assignment. Main should run the combined clean-checkout suite (including the
real-input `tests.test_analyze` stability case), then independent catalogue review.
The committed proposal is not authorization to adopt it or take matching credit.
The retained worktree stash used during Main integration remains preserved; prior
v9/v9-r2 reports, evidence, sources, probes and failed receipts are not overwritten.
