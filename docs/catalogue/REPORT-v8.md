# Candidate catalogue v8-r2 — independent-review corrections

**Candidate only: not adopted, not accepted C progress, and not a claim that the decompilation is complete.** The original v8 candidate and REPORT remain untouched. No main-checkout tracked file, accepted ledger, compiler profile, source/header, catalogue configuration or denominator history was changed.

- Branch/worktree: `catalogue-v8`, `scratch/worktrees/catalogue-v8`.
- Revised commit: **`b52f08b1b6f17be29c03634c17c9cbd078ecd65c`** (parent `4cf77e7939ea7021a2d99a0bed9fe94543d9e144`).
- Candidate: `scratch/analyze/catalogue-v8-r2.json`.
- Candidate SHA-256: **`e33aad973846b60b5286be6becb5080c9626188fa9a64a6c0b48197b16e8adad`**.
- Immutable v8 input: `scratch/analyze/catalogue-v8.json`, SHA-256 `2fe7c9767b57a60a1b5a43b4fdc2c2ac0651a30d580341f75f183230e1dc3d8c`.
- v7 baseline SHA-256: `b303210af5d4f8395044bb8cd31508d7ee801b4640704c1253a433e68e1c88d3`.
- Review addressed in full: `scratch/review/catalogue-v8/REVIEW.md`.

## Per-finding disposition

| Finding | Status | Correction / exercised evidence |
| --- | --- | --- |
| **V8-R01 — major** | **Fixed** | The plain `tools/analyze.py catalogue --out X` entry point now loads the immutable reviewed evidence snapshot, applies classifications, and recomputes the complete image-dependent revision review. `generator` records the command, revision, source hashes, classification input and all current input hashes. Two separate CLI processes, with different output paths, produced identical complete bytes and the SHA-256 above. |
| **V8-R02** | **Fixed** | `build_catalogue` compares every complete protected owner again after `apply_classifications`; evidence-only or kind-only changes fail, even at unchanged address/size. Default protection now includes matches, c005, existing candidate files and the pinned-content c006 branch blob. Synthetic regressions cover conflicting well-formed classifications, evidence-only mutation, automatic c005/c006 loading, and explicit withholding of bundled conflicts. Final generation preserves all 870 protected records exactly. |
| **V8-R03** | **Fixed** | The exact tail site `0x001efcd4` now partitions the 16-byte owner at `0x001efcd0` into an 8-byte wrapper and an 8-byte returning field-read body at `0x001efcd8`. The generalized guarded rule is covered by scalar-width, return-form, memory-side-effect, protection and crossing regressions. |
| **V8-R04** | **Fixed** | The exact tail site `0x0013194c` now partitions `0x00131948` into an 8-byte wrapper and an 8-byte field-read body at `0x00131950`; the body closes via a direct tail transfer to an independently known ARM owner not classified noreturn. The rule also rejects calls, unknown/interior/noreturn targets, conditional or stack-dependent returns, indexed/writeback reads and arbitrary longer suffixes. |
| **V8-R05 — note** | **Retained / documented** | Historical ABI/compiler executions are not represented as fresh work. The immutable evidence snapshot and candidate retain report identities, the inherited proof status, absent-worktree qualification, corrected-grid provenance, and the distinction between ABI constraints and inferred SDK/handwritten ownership. No compiler/emulation probe was rerun. |
| **V8-R06** | **Fixed** | Both leading-instruction and pointer-prefix recovery use the same explicit memory-side-effect predicate. It refuses store classes, SWP/SWPB, STREX variants, LDREX variants and reservation operations even when Capstone omits MEM WRITE flags; writeback remains forbidden. The review-equivalent synthetic SWP and STREX tests explicitly assert the missing WRITE flag and refusal by both entry rules. Ten atomic/exclusive forms are covered. Benign argument-memory reads remain accepted. `0x001f3e78` is correctly described below as nonstoring, not register-only. |

## Exact reproduction and exercised checks

Run from `scratch/worktrees/catalogue-v8`, with the recorded repository inputs and c006 branch blob:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out /home/michael/code/kanken-training-2-decomp/scratch/analyze/catalogue-v8-r2.json
```

That command alone generated the supplied candidate, exit **0**. No second classification script, postprocessing step, caller-provided classifications option or manual JSON modification is required. The separate reproduction actually exercised was:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out /home/michael/code/kanken-training-2-decomp/scratch/worktrees/catalogue-v8/scratch/verification/catalogue-v8-r2-rerun.json
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -m unittest tests.test_analyze -v
```

Both complete candidate byte strings compare equal, and both SHA-256 values are **`e33aad973846b60b5286be6becb5080c9626188fa9a64a6c0b48197b16e8adad`**. Elapsed time and output pathname are printed only to stdout, never serialized. Source hashes and protected-input hashes are serialized intentionally: changing an input means generating a different candidate, not silently reproducing this one.

| Exercised operation | Exit | Result |
| --- | ---: | --- |
| Authorized `tests.test_analyze` | **0** | **140 tests, OK**, no skips; 50.771 s unittest / 50.867 s captured wall time |
| Final plain-CLI generation | **0** | 4,592 owners; 7,724 unknown bytes; 70 anomalies; 28.493 s wall time |
| Independent plain-CLI regeneration | **0** | Complete byte equality; identical SHA-256; 30.610 s wall time |
| Final requested c006 `git show` read | **0** | 381 candidate functions; `func_00152964` absent |
| Generation-time complete protection / structural / entry-preservation / byte-accounting gates | **0** | No protected record changes; no unexpected entry loss or extent changes; structural issue list `[]`; all v7 unknown bytes conserved |

Receipts and logs are under `scratch/worktrees/catalogue-v8/scratch/verification/`: `tests-v8-r2.json`, `tests-v8-r2.log`, `generation-v8-r2.json`, `regeneration-v8-r2.json`, `c006-v8-r2.json`, `commit-v8-r2.json`, `differences-v8-r2.json`, and `differences-v8-r2-detail.json`. Test and generation outputs were captured without output-filtering a check before judging its exit code. No full suite, build, compiler probe, formatter, prepare/finalize, adoption, push or denominator-history command was run.

## Totals versus v7 and immutable v8

| Measure | v7 | v8 | v8-r2 | Delta vs v7 | Delta vs v8 |
| --- | ---: | ---: | ---: | ---: | ---: |
| Top-level owners | 4,580 | 4,590 | 4,592 | +12 | +2 |
| Callable starts/entries | 4,604 | 4,614 | 4,616 | +12 | +2 |
| Creditable ordinary functions | 4,557 | 4,542 | 4,544 | -13 | +2 |
| Creditable extent bytes | 1,129,946 | 1,129,400 | 1,129,400 | -546 | +0 |
| All-kind extent bytes | 1,136,930 | 1,137,870 | 1,137,870 | +940 | +0 |
| Physically function-owned bytes | 1,135,444 | 1,136,460 | 1,136,460 | +1,016 | +0 |
| Instruction bytes | 1,054,598 | 1,055,506 | 1,055,506 | +908 | +0 |
| Pool bytes | 39,988 | 40,096 | 40,096 | +108 | +0 |
| Addressed-literal bytes | 40,858 | 40,858 | 40,858 | +0 | +0 |
| Unknown bytes | 8,704 | 7,724 | 7,724 | -980 | +0 |
| Unknown ranges | 189 | 140 | 140 | -49 | +0 |
| Anomalies | 83 | 70 | 70 | -13 | +0 |

Kinds: **4,544 ordinary function + 17 runtime + 8 assembly + 17 multi_entry + 6 veneer = 4,592 owners**. There are 4,523 ARM owners and 69 Thumb owners. The same **27** classification-evidence records remain: 19 runtime-evidence owners (17 newly noncreditable ordinary functions plus two already noncreditable multi-entry owners) and eight kernel-ABI assembly owners. No library kind was introduced.

Count balance: **4,557 + 10 newly recovered pointer/leaf owners + 2 newly split bodies − 17 runtime − 8 assembly = 4,544 creditable functions**. Byte balance is unchanged from v8: **1,129,946 + 152 leading-repair extent growth + 788 new extents − 1,286 runtime − 200 assembly = 1,129,400 creditable extent bytes**. Splitting already-owned extents adds functions/entries, not bytes or accepted C credit.

Relative to v7, recovery still claims exactly **980 formerly unknown bytes + 36 formerly padding bytes = 1,016 newly owned bytes**, decomposed as **908 instruction + 108 pool bytes**. The 22 leading repairs, ten v8 newly recovered functions, all literal ownership, all 49 closed unknown intervals and all 13 resolved anomalies are unchanged by r2. The same 140 unknown ranges / 7,724 B and 70 open anomalies remain honest open work. Extents still contain 1,410 B not explained as owned code/pools/literals; extent totals must not be mistaken for full coverage.

## Every change since v8

### Function records and physical ranges

**Exactly four records differ:** two old owners shrink to 8 bytes and two new 8-byte ordinary ARM owners are added. No old start or entry is removed. The other **4,588 v8 function records are identical**, including all classification records. No instruction, pool, literal, unknown, padding, data, rodata or mapped-segment byte changes its physical range kind.

| v8 owner / extent | r2 wrapper | r2 field-read body | Exact tail site |
| --- | --- | --- | --- |
| `0x00131948` / 16 B | `0x00131948` / 8 B | `0x00131950` / 8 B | `0x0013194c` |
| `0x001efcd0` / 16 B | `0x001efcd0` / 8 B | `0x001efcd8` / 8 B | `0x001efcd4` |

The only changed fields on each existing wrapper record are `size`, `code_size` and `code_ranges`. Each new body has ordinary `function` kind, a single top-level ARM entry and `linker_branchnop` evidence, no pool/literal, and a complete 8-byte code range. This is a rule-based recovery, not an address allowlist.

The only `ranges` changes are the following owner partitions (exclusive end); their covered bytes are identical:

- `[0x00131948, 0x00131958)` becomes `[0x00131948, 0x00131950)` owned by `0x00131948` plus `[0x00131950, 0x00131958)` owned by `0x00131950`.
- `[0x001efcd0, 0x001efce0)` becomes `[0x001efcd0, 0x001efcd8)` owned by `0x001efcd0` plus `[0x001efcd8, 0x001efce0)` owned by `0x001efcd8`.

### Linker inventories and all statistics changes

Only the two tail-site rows above change in `linker_tail_inventory` and `linker_nop_inventory`: disposition `no_independent_successor_witness` becomes `split`, start evidence gains `nonstoring_wrapper_field_read`, and the 8+8 partition fields are recorded. Exactly two rows are added to `linker_tail_extent_splits`; **all 137 prior split records are identical**. The complete 776-word disposition is now 139 split, 93 already separate, 505 in-frame, 31 control-flow crossing, one literal crossing, five lacking independent successor proof, one unowned tail, and one unexecuted switch slot. No crossing population is admitted.

These are **all seven changed top-level stats fields**; every other stats field, including all byte totals, classifications, anomalies and unknown ranges, is identical to v8:

| Stats field | v8 | v8-r2 |
| --- | --- | --- |
| `functions` | 4,590 | 4,592 |
| `creditable_functions` | 4,542 | 4,544 |
| `functions_by_isa.arm` | 4,521 | 4,523; Thumb remains 69 |
| `functions_by_kind.function` | 4,542 | 4,544; other kinds unchanged |
| `linker_tail_extent_splits` | 137 | 139 |
| `starts_by_evidence.linker_branchnop` | 199 | 201; other evidence counts unchanged |
| `tail_calls` | 1,414 | 1,416 |

### Leading-gap inventory changes from expanded protection

Exactly six `leading_instruction_inventory` rows change only their `decision` from `no_register_prefix` to `protected_extent`, because c006 is now protected. Their gap bounds and function records are unchanged:

- `0x001c526c`.
- `0x0020e3b0`.
- `0x00212598`.
- `0x002126f0`.
- `0x00214468`.
- `0x002145f4`.

All 22 actual leading repairs and all seven pointer-prefix wrappers are unchanged. Six pointer prefixes use register-only setup; **`0x001f3e78` performs one benign argument-memory read plus register/constant setup**. All seven are nonstoring. The new guards therefore change no supplied-image prefix decision; their effect is the missing synthetic safety contract.

### Generator, review metadata, and unchanged fields

The new `generator` object records `tools/analyze.py catalogue --out X`, revision `catalogue-v8-r2`, the two generator-source hashes, the reviewed classification snapshot, whether the bundled review applied, and the complete current input hashes. It has no clock, output path or active-author-directory dependence.

These are **all 12 changed `revision_review` fields**:

- `name`: `catalogue-v8` → `catalogue-v8-r2`.
- `c006_present`: false → true.
- `protected_rows`: adds the 381 unique c006 addresses (870 total).
- `protected_summary`: adds the exact c006 branch blob and its 381-function / 30,724-byte summary; prior summaries are unchanged.
- `preservation`: adds `partitioned_owners` naming only `0x00131948` and `0x001efcd0`; no unexpected loss/change is reported.
- `changed_records`: accounts for the two changed wrappers and two new bodies in addition to the original v7→v8 changes.
- `input_sha256`: records the current, complete deterministic generator/protection inputs instead of scanning ongoing private author work.
- `provenance`: explicitly retains old-proof / absent-worktree / may-read qualifications.
- `retained_evidence_source_sha256`: binds the immutable v8 candidate used to retain the reviewed classifications/dispositions.
- `retained_evidence_input_sha256`: preserves the original v8 evidence-source hash ledger as historical provenance, not as fresh reads.
- `withheld_classifications`: new explicit empty list; there were no conflicts in this final c006 snapshot.

`baseline_sha256`, `classifications`, `thumb_dispositions`, `author_dispositions`, `pointer_census`, `unknown_accounting`, `anomaly_accounting` and `structural_issues` are **exactly unchanged**. No old proof was silently promoted to a fresh execution.

The complete changed top-level field list is: `functions`, `generator`, `leading_instruction_inventory`, `linker_nop_inventory`, `linker_tail_extent_splits`, `linker_tail_inventory`, `ranges`, `revision_review`, `stats`. **Every other top-level field is identical.** The two machine-readable differences receipts enumerate every changed function field, inventory row, physical range, stat and review-metadata field.

### Tracked implementation/documentation changes

- `tools/analyze.py`: integrated deterministic evidence/review invocation, automatic complete-record manifest protection including the c006 branch blob, post-classification invariant, shared explicit memory-side-effect guards, generalized guarded field-read boundary witness, and generator metadata.
- `tools/catalogue_review.py`: retained and moved the image-dependent pointer census/accounting logic out of the obsolete separate generator; recomputes preservation/revision metadata through the analyzer entry point.
- `research/catalogue_v8_evidence.json`: immutable metadata-only snapshot of the 27 reviewed classification records, 67 Thumb dispositions, 33 author dispositions and inherited proof provenance. No original executable bytes or disassembly are included.
- `research/catalogue_v8.py`: removed the superseded separate generator; there is no second candidate-generation convention or compatibility shim.
- `tests/test_analyze.py`: 14 additional regression tests (126 → 140) plus updated whole-image expectations/protection, covering the review defects and guarded field-read split cases.
- `docs/LINKER.md`: documented the single command, provenance, whole-record protection, nonstoring/read qualification, exact guarded split rule and residual inference limits.

## Final protected records, including c006

The required final `git show c006-integrate:config/candidates/c006.json` was re-read immediately before generation, exit **0**. It contains **381** functions and does **not** contain `func_00152964`, following the integrator’s decision to remove it from c006. Therefore `0x00152964` retains its normal v8 runtime classification; no classification was withheld.

- c006 Git blob: `a4ef5151bdf965bfc7de9456be856be0f7263204`.
- c006 file SHA-256: `b760840461a9a00f239e55b277799222c2203a7b4ff972e051aaf217690c5781`.

| Protected manifest | Functions | Extent bytes | Complete-record changes |
| --- | ---: | ---: | ---: |
| `config/candidates/c004.json` | 196 | 2,564 | **0** |
| `config/candidates/c005.json` | 190 | 11,668 | **0** |
| `config/matches.json` | 299 | 7,620 | **0** |
| `git:c006-integrate:config/candidates/c006.json@a4ef5151bdf965bfc7de9456be856be0f7263204` | 381 | 30,724 | **0** |

**870 unique protected records / 50,012 extent bytes are identical to their complete adopted records after classification.** c004 is already contained in matches; it is shown as a provenance check, not counted a second time. c006 contributes 381 additional unique records. Every protected address remains an ordinary top-level function. The candidate’s final `preservation.protected_changes`, `unexpected_entry_losses`, `unexpected_extent_changes` and `structural_issues` are empty.

## Provenance and limits retained (V8-R05)

The source runtime-helper and ARMCC-extension private worktrees are absent. The retained reports, named whole-original-extent digest identities, natural-caller bindings, original ABI behavior and corrected no-frame-grid/source identities remain the evidence standard selected for v8. The independent v8 review checked eight common-helper digests and the corrected retained grid/source identities. **This r2 task did not rerun the 3,552 ABI executions, natural-caller compiler matches, five raw-ABI declaration rejections, or the three-function 16-cell compiler grid.** Those are inherited proof claims, not new receipts.

Public contract references, not byte-identity proof: [Arm runtime ABI](https://github.com/ARM-software/abi-aa/blob/main/rtabi32/rtabi32.rst), [3dbrew Kernel ABI](https://www.3dbrew.org/wiki/Kernel_ABI), [libctru public SVC wrappers](https://github.com/devkitPro/libctru/blob/master/libctru/source/svc.s), and [Arm SWP semantics](https://developer.arm.com/documentation/ddi0406/c/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/SWP--SWPB). No newly fetched compiler-documentation shell is treated as proof.

**[INFERENCE]** SDK/archive/handwritten ownership beyond the demonstrated ABI/shape constraint is not authenticated. The two additional boundaries use the adopted erased-tail-branch interpretation and explicit field-read/closed-transfer shape guards, not recovered source section symbols. Leading-entry dependence remains a call-interface **may-read** model; the r1 setup at `0x002102b4` retains that specific qualification. Ordinary libc/libm without known-library identity, 66 unbound Thumb residue candidates and nine shape-only SVC adapters remain creditable.

## Deterministic current input identities

- `config/candidates/c004.json`: `2626c2911fae00d433e162c87b8835cac0415fd0baf6e406610cd96666130af8`
- `config/candidates/c005.json`: `514701fedc425d536f37fed07472173d99e5fd0015d837265ca3fcd04dff7df5`
- `config/catalogue.json`: `b303210af5d4f8395044bb8cd31508d7ee801b4640704c1253a433e68e1c88d3`
- `config/matches.json`: `fd31d2cd50424520fe6e7444fe70b553dc76e1b2a2abb377eb61e8ef85cd11e4`
- `config/names.json`: `90834c90c362fcd648d3e777b253bfa7aa35d8496cc03b2649459f175ab61378`
- `exheader`: `0c43847e321bb81809c7f01489f6f321695646f6ec8e7131936b2eea9b75c6d8`
- `git:c006-integrate:config/candidates/c006.json@a4ef5151bdf965bfc7de9456be856be0f7263204`: `b760840461a9a00f239e55b277799222c2203a7b4ff972e051aaf217690c5781`
- `image`: `a367b5dcd65e3924c93cf3fbaee5d4d40f9c5205d1287912784a974d5e405361`
- `research/catalogue_v8_evidence.json`: `a7c09fd1a0bfa3876e30ff93af2e9cf9d869954f0c4ed758edea98cc857bddf8`
- `tools/analyze.py`: `f7b2b35bfa9e68bdd7de5ec7c286817142e630804db1a3647b661c4537bbd70e`
- `tools/catalogue_review.py`: `341c3a1d8695e80f423cd4f952f7943be1a93129a8490c52e09fd7b49e8a432d`

Adoption, accepted-credit changes, config/catalogue updates, full-suite/build review and prepare/finalize remain integrator-owned. This handoff closes the requested v8 review corrections and candidate-generation acceptance, not those separate gates.
