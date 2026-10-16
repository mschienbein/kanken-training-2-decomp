# Candidate catalogue v6 — internal armlink tail-NOP boundaries

## Status and provenance

**PASS: complete candidate, not adopted.** Runtime identity: **OpenAI / openai-codex/gpt-6.1-sol**.
A fresh isolated worktree was used rather than rebasing the previous dirty v4 worktree: `scratch/worktrees/catalogue-v6`, branch `catalogue-v6`, based on **main c33af74**. Assigned tracked edits: `tools/analyze.py` and `tests/test_analyze.py` there. The main checkout received only the requested scratch candidate/report; no canonical catalogue, manifests, accepted sources, history or receipts were changed.
Private evidence root **V** = `scratch/worktrees/catalogue-v6/scratch/verification/`. All repository Python commands used `/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python`; `orig` and `.cache/toolchain` are symlinks to the main checkout. Only `tests.test_analyze` was run. The two source comparisons below compiled sequentially. Deliverable artifacts contain metadata, counts, addresses and authored-source receipts, not original ROM bytes or original disassembly.

| Input / output | SHA-256 |
| --- | --- |
| Adopted v5 `config/catalogue.json` | `d490e753d55456f6624618c29150ec80dbbf1c3f0aed25fbbe406f58351506da` |
| Candidate `scratch/analyze/catalogue-v6.json` | `0526a7574676bc4d5a840c5b891564934b36e6d7d8552be880d3d6e2ad9c8323` |
| Original image | `a367b5dcd65e3924c93cf3fbaee5d4d40f9c5205d1287912784a974d5e405361` |
| `research/linker/nop_census.py` in the v6 worktree | `078abd4ff9c1f2c38f5e75176ec517e31ad6d2d434d6c36fee8456d3f8d92fda` |
| `tests/test_analyze.py` in the v6 worktree | `309ca27863e78bdb3fdc620169fa5e319adf12cd21b1a499792d29be3f0d4236` |
| `tools/analyze.py` in the v6 worktree | `0b150a57d90c74e5476b462280fcf6e3930464a84d48e1d7b46b2d1a3b026330` |
| Protected `config/candidates/c004.json` | `2626c2911fae00d433e162c87b8835cac0415fd0baf6e406610cd96666130af8` |
| Protected `config/matches.json` | `1641e6e3401711ba082de514182045915d888db764ef3697acdfae1e1d54bd1a` |

## Executive results

| Measure | v5 | v6 | Delta |
| --- | ---: | ---: | ---: |
| Top-level owners | 4,443 | 4,572 | +129 |
| Callable starts, including entries | 4,537 | 4,596 | +59 |
| Creditable ordinary functions | 4,350 | 4,549 | +199 |
| Creditable extent bytes | 1,111,934 | 1,129,946 | +18,012 |
| Multi-entry owners | 87 | 17 | -70 |
| Veneers | 6 | 6 | +0 |
| Unknown bytes | 8,704 | 8,704 | +0 |
| Unknown ranges | 189 | 189 | +0 |
| Semantic anomalies | 83 | 83 | +0 |
| Function-owned bytes | 1,135,444 | 1,135,444 | +0 |
| Instruction bytes | 1,054,598 | 1,054,598 | +0 |
| Decoded instructions | 264,729 | 264,729 | +0 |
| Pool/table bytes | 39,988 | 39,988 | +0 |
| Literal bytes | 40,858 | 40,858 | +0 |
| Sum of extents | 1,136,930 | 1,136,930 | +0 |
| Fallthrough groups | 71 | 13 | -58 |
| Secondary entries | 94 | 24 | -70 |
| Removed weak-start records | 59 | 58 | -1 |
| Noreturn owners | 6 | 7 | +1 |

**129 new boundaries**, covering 70 separable two-entry owners and 59 previously implicit successors. All resulting parts are ordinary `kind: function`. There are **129 modified old records, 129 added records and zero removed records**. Every one of the **4,537 v5 callable starts/entries** survives in the same ISA.
The **+199 creditable functions / +18,012 B** is a denominator correction, **not accepted C progress**: 70 formerly non-C groups become ordinary functions. No new executable byte is decoded, no unknown byte is claimed, and the sum of extents is conserved. The old `tail_calls` statistic increases by 129 logical branchnop edges; the physical direct B/BL graph is unchanged.

## 1. Complete classification of the 270 census tail NOPs

The frozen v5 census (`research/linker/nop_census.py`) supplies the cohort, not a claim that every classified position is safely separable. The implementation reproduces exactly the **270 (site, census-class) pairs**. Its separate stack-LR state and control/data checks decide which may become section boundaries. The other 505 in-frame MOV-NOP census sites and the unexecuted switch slot are not split as tail boundaries.

| Final disposition | Positions | Meaning |
| --- | ---: | --- |
| Split | 129 | Independent successor evidence; no control/data crossing; ordinary parts; protected records preserved |
| Already separate | 86 | v5 already has the next instruction as a separate top-level function |
| Control flow crosses | 31 | Direct branch or address-switch edge crosses the proposed cut |
| No independent successor witness | 15 | A census tail label alone is not enough to establish a new callable section |
| Unowned tail, no head start | 8 | NOP is outside every v5 extent; no independently established head to split |
| Literal ownership crosses | 1 | Prefix PC-relative literals lie on the other side of the proposed cut |
| **Total** | **270** | Every position is individually listed below and in the candidate metadata |

The **129 splits** comprise 70 `tail:falls_into_entry`, 20 `tail:before_prologue`, 19 `tail:after_epilogue`, and 20 `tail:in_leaf` positions. The inventory is additive metadata in `linker_tail_inventory`; actual accepted boundaries are `linker_tail_extent_splits`. Both arrays are also preserved separately in V.

### Census LR-base qualification

The published census tests whether the text of a push/ldm operand contains `lr`. An LR **base** operand can therefore clear its linear saved-LR flag without restoring the LR saved on the stack. **21 census-tail positions** show this disagreement. The implementation retains their census identity so none disappears from the audit, but tracks `census_lr_saved` and actual stack-register-list `lr_saved` separately. Every one is withheld by the crossing-control-flow gate; the stronger LR guard is also regression-tested with both an immediate and a separated LR-base instruction. The census label is explicitly heuristic, not independent proof of a C section boundary.

## 2. Boundary rules and preservation design

1. Run existing v5 discovery, weak-boundary refinement, missing-target recovery, pointer-tail recovery, literal ownership and leading-NOP wrapper splitting unchanged. Apply the internal-tail partition only after this has converged; do not restart global discovery/refinement after cutting sections.
2. A candidate must be an executed unconditional ARM `mov r0,r0` in the census tail cohort, with owned ARM code at its next instruction. Architectural NOP hints, predicated MOVs, in-frame removed-call sites, data words and the unexecuted switch slot are not substitute cut points.
3. Require successor evidence: an existing entry, a prologue, an independently referenced pointer, an unconditional return-only successor (`bx lr` / `mov pc,lr`), or an AAPCS live-argument violation under the census’s as-if-call probe. The last case distinguishes an LR-clear tail from an erased ordinary call without assuming that every leaf suffix starts a new function. A flags-only dependency is not used as this ABI witness.
4. Reject every direct B/Bcond crossing in either direction and every crossing address-switch case. Calls between sections are not confused with intraprocedural branch connectivity. Reject PC-relative pool/address ownership crossings, unresolved literal conflicts, a still-saved stack LR, and any resulting part with an additional interior entry. This is why the 31 control-crossing, one literal-crossing and 15 unwitnessed positions stay intact.
5. Partition code ranges, pool/literal spans and ownership ranges only; never add code, move bytes, borrow data across a cut, or erase old entry addresses. New functions inherit the existing closed-noreturn proof where applicable; existing unrelated metadata is not globally re-inferred. Merge/group metadata is updated only for entries restored by these boundaries.
6. Protection binds **full adopted catalogue records**, not just a set of names. The CLI always protects `config/matches.json`; repeatable `--protect-manifest` arguments add candidate manifests. This run supplied `config/candidates/c004.json`. A proposal that changes a protected adopted record is retained as `protected_owner` evidence instead of applied; inability to retain the adopted record fails closed.
7. Protection is adoption-safe: once a split is adopted and either part becomes accepted, regeneration must reproduce that split rather than reverting to the pre-split extent. A dedicated regression protects the generated head and body records, rebuilds the same image, and asserts identical serialized output. The pure `build_catalogue` API accepts the same protected address → adopted-record mapping without reading repository manifests itself.

## 3. Accepted and candidate functions: unchanged

| Manifest | Functions | Extent bytes | Whole-record equality | Would-change list |
| --- | ---: | ---: | --- | --- |
| `config/matches.json` | 103 | 5,056 | **103/103 identical and creditable** | `[]` |
| `config/candidates/c004.json` | 196 | 2,564 | **196/196 identical and creditable** | `[]` |
| **Total protected** | **299** | **7,620** | **299/299 identical** | **`[]`** |

There are **zero currently eligible protected-owner conflicts** in this cohort. The guard is still exercised by synthetic protected-head and protected-secondary-entry tests. Individual manifest/source/function/size/equality records are in `V/protected-functions-v6.json`; input manifest hashes are above. Candidate bytes are not added to accepted progress.

All physical **19,991 call sites** and **26,274 B sites** retain exactly their old `(site, transfer kind, target, ISA)` tuples. In addition, old and new v2 inlining decisions were compared at every original ARM call site: **zero changes**. Thus the changed catalogue does not newly inline a previously kept original ARM call. This is a static oracle-impact check, not a claim of a new accepted full-image build or a fresh compile of all protected units.

## 4. Reported function: end-to-end source check

`func_00112c80` changes from **72 B** to **24 B**, ending with the tail-NOP at **`0x112c94`**. A separate ordinary function starts at **`0x112c98`**, has **48 B**, and owns the original pool `[0x112cc4,0x112cc8)`. No instruction or pool value was changed.
The author’s already-preserved C sources were compiled individually through the ordinary matching CLI, in sequence, against the v6 candidate and **armlink5-inline-branchnop-v2**:

| Source | Candidate / catalogue bytes | Result | Rewrites |
| --- | --- | --- | --- |
| `scratch/authors/Wave5Author2/func_00112c80.c` | 24 / 24 | **MATCH**, exit 0 | 1 branchnop |
| `scratch/authors/Wave5Author2/func_00112c98.c` | 48 / 48 | **MATCH**, exit 0 | 0 |

Head source SHA-256: `8d0b1710e1f38c51b6bd1f099cf5825c67b96da5a54edba9dae9a3749c04a8f1`. Body source SHA-256: `c827c199208b26c28ede394ebf4d35da7879b9b6d055b2cf21e1874a1efd1b59`. Both result records bind source, object, compiler, flags, original image, catalogue and model hashes. No author source was edited. The body’s existing `ORIGINAL BUG` comment about the uninitialized stack copy remains; successful matching is not independent semantic/interface review or accepted credit.

## 5. Whole-image verification

**All 12 preservation gates pass.** `validate_catalogue` returns **`[]`**. The map still tiles all **1,327,104 mapped bytes** exactly once, with no extent overlap. Instruction coverage, pools, literals, total function-owned coverage, unowned ranges and non-text ranges are byte-identical as interval sets; only function ownership and classification change.
All **4,537 v5 callable entries** are retained in the same ISA. The existing target-preservation audit was extended across **23,158 resolved direct/pointer evidence rows**, covering **4,387 distinct referenced address/ISA pairs**. Complete `entry_losses = []`, `target_violations = []`, `inline_changes = []`.

| Resolved v5 evidence lane | Rows |
| --- | ---: |
| `branch` | 1,205 |
| `call` | 19,991 |
| `data_pointer` | 67 |
| `pointer_evidence` | 783 |
| `pool_pointer` | 387 |
| `rodata_pointer` | 725 |

Anomalies remain **83**, with the same issue counts. The sole address-set difference is the owner marker `unexplained_function_bytes`: **`0x10d27c` → `0x10d2b8`**, because the unknown hole belongs to the newly separate suffix. No anomaly is suppressed. Noreturn owners increase from six to seven because that same closed-noreturn section now has two separate callable parts. The 17 literal-conflict records remain unchanged.

## 6. Tests, commands and retained failed gates

Final command: **96 tests, OK, exit 0**, 91.397 s wall time. Only `tests.test_analyze` was run. Coverage includes the reported split, separable multi-entry tails, live-argument evidence, unsupported tails, return-only successors, forward/backward/switch crossings, literal crossings, extra interior entries, protected owners and aliases, adoption-safe protection, LR-base census ambiguity, page-padding boundaries, original-image determinism and all protected records.

Failed intermediate runs are retained rather than overwritten:
- `V/tests-v6-first.json`: exit **1**, 93 tests / 102 subtest failures. One map-construction bug coalesced segment-padding ranges across page boundaries; the corrected pass retains untouched ranges and a dedicated empty-segment test. That run also exposed a census-cohort mismatch (249 versus 270); preserving the census’s LR-textual cohort while independently guarding actual stack LR restores all 270 positions.
- `V/tests-v6-final.json`: exit **1**, 95 tests / one assertion failure in the new LR-base fixture: the immediate preceding LR-base LDM receives the census’s `after_epilogue` label, not `in_leaf`. The regression was strengthened to exercise both the immediate and separated forms, with both required to retain the saved stack LR and refuse splitting.
- `V/tests-v6-passed.json`: earlier **95-test PASS**. The subsequent adopted-record protection regression was added to prevent future accepted split parts from reverting on regeneration; final **96-test PASS** is `V/tests-v6-adoption-safe.json`. No production or test source changed after that final pass.

Working directory for all commands below: `/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v6`.

```sh
/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python -m unittest tests.test_analyze -v
/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --protect-manifest config/candidates/c004.json --out /Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v6.json
/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python scratch/verification/check_catalogue_v6.py
/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python tools/match.py /Users/mooki/Code/kanken-training-2-decomp/scratch/authors/Wave5Author2/func_00112c80.c --profile game --catalogue /Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v6.json --out scratch/verification/example/func_00112c80 --show 0 --linker-model armlink5-inline-branchnop-v2
/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python tools/match.py /Users/mooki/Code/kanken-training-2-decomp/scratch/authors/Wave5Author2/func_00112c98.c --profile game --catalogue /Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v6.json --out scratch/verification/example/func_00112c98 --show 0 --linker-model armlink5-inline-branchnop-v2
```

- Final tests: exit **0**, 91.397 s; `V/tests-v6-adoption-safe.json`.
- Protected catalogue generation: exit **0**, 44.527 s; `V/catalogue-v6-adoption-safe-command.json`; candidate in the main checkout’s `scratch/analyze/catalogue-v6.json`.
- Whole-image/census/protection/oracle checks: exit **0**, 5.159 s; `V/verification-adoption-safe-command.json`, `V/verification-v6.json`.
- Reported head/body comparisons: each exit **0**; full argv/stdout/stderr/timings in `V/example-match-commands.json`; result records under `V/example/`.
- Census inventory command: exit **0**, `V/inventory-command.json`; source and complete frozen cohort in `V/inventory_tail_nops.py` and `V/tail-nop-inventory-v5.json`.
- No full suite, full-image acceptance build, formatter, denominator recording, checkpoint mutation, commit, deletion or publication was run by this worker. Worktree creation and private-input symlinks each exited 0; git reported only the documented clonefile-to-plain-checkout fallback.

## 7. Full list of all 129 applied splits

Every row produces two ordinary functions; all 129 source owners are distinct. A successor already declared as a v5 entry is promoted to a top-level owner rather than counted as a newly discovered address. **70 existing entries are promoted, and 59 new callable starts are added.** The exact complete before/after JSON for all **258 changed records** is `V/changed-records-v6.json`; all other **4,314 v5 records** are identical.

| Old owner | Old B | Tail NOP | New head B | Successor | Successor B | Existing v5 entry? | Successor evidence |
| --- | ---: | --- | ---: | --- | ---: | --- | --- |
| `0x00100ca8` | 192 | `0x00100cd4` | 48 | `0x00100cd8` | 144 | no | aapcs_live_arguments |
| `0x001071cc` | 188 | `0x001071e8` | 32 | `0x001071ec` | 156 | no | prologue |
| `0x0010d27c` | 412 | `0x0010d2b4` | 60 | `0x0010d2b8` | 352 | no | prologue, aapcs_live_arguments |
| `0x001110f0` | 300 | `0x00111110` | 36 | `0x00111114` | 264 | no | prologue, aapcs_live_arguments |
| `0x00112c80` | 72 | `0x00112c94` | 24 | `0x00112c98` | 48 | no | prologue |
| `0x001139dc` | 80 | `0x001139e0` | 8 | `0x001139e4` | 72 | no | aapcs_live_arguments |
| `0x00113a2c` | 132 | `0x00113a30` | 8 | `0x00113a34` | 124 | no | aapcs_live_arguments |
| `0x001176e0` | 192 | `0x00117748` | 108 | `0x0011774c` | 84 | yes | existing_entry |
| `0x00123a64` | 212 | `0x00123a6c` | 12 | `0x00123a70` | 200 | no | aapcs_live_arguments |
| `0x00124874` | 428 | `0x001248fc` | 140 | `0x00124900` | 288 | no | prologue, aapcs_live_arguments |
| `0x00127398` | 1052 | `0x0012739c` | 8 | `0x001273a0` | 1044 | yes | existing_entry, prologue |
| `0x001277b4` | 384 | `0x001277b8` | 8 | `0x001277bc` | 376 | no | aapcs_live_arguments |
| `0x00127934` | 1192 | `0x0012793c` | 12 | `0x00127940` | 1180 | no | prologue, aapcs_live_arguments |
| `0x0012d0a4` | 32 | `0x0012d0b4` | 20 | `0x0012d0b8` | 12 | yes | existing_entry |
| `0x0012d104` | 76 | `0x0012d144` | 68 | `0x0012d148` | 8 | no | pointer |
| `0x0012d1cc` | 240 | `0x0012d1d4` | 12 | `0x0012d1d8` | 228 | yes | existing_entry, prologue |
| `0x0012f9fc` | 112 | `0x0012fa20` | 40 | `0x0012fa24` | 72 | no | prologue, aapcs_live_arguments |
| `0x00131920` | 40 | `0x00131924` | 8 | `0x00131928` | 32 | no | prologue |
| `0x00136104` | 72 | `0x00136130` | 48 | `0x00136134` | 24 | no | aapcs_live_arguments |
| `0x00136630` | 228 | `0x00136654` | 40 | `0x00136658` | 188 | no | prologue |
| `0x0013672c` | 172 | `0x00136750` | 40 | `0x00136754` | 132 | no | prologue, aapcs_live_arguments |
| `0x00137118` | 20 | `0x00137124` | 16 | `0x00137128` | 4 | no | return_only_successor |
| `0x00140a08` | 404 | `0x00140a0c` | 8 | `0x00140a10` | 396 | no | aapcs_live_arguments |
| `0x00140ec4` | 252 | `0x00140ec8` | 8 | `0x00140ecc` | 244 | no | prologue, aapcs_live_arguments |
| `0x00141554` | 64 | `0x0014155c` | 12 | `0x00141560` | 52 | yes | existing_entry, prologue |
| `0x00142684` | 112 | `0x001426a4` | 36 | `0x001426a8` | 76 | no | prologue, aapcs_live_arguments |
| `0x001426f4` | 112 | `0x00142714` | 36 | `0x00142718` | 76 | no | prologue, aapcs_live_arguments |
| `0x00143328` | 264 | `0x0014334c` | 40 | `0x00143350` | 224 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x00146758` | 144 | `0x001467e0` | 140 | `0x001467e4` | 4 | no | return_only_successor |
| `0x00148210` | 244 | `0x0014821c` | 16 | `0x00148220` | 228 | yes | existing_entry, prologue |
| `0x0014837c` | 432 | `0x001483bc` | 68 | `0x001483c0` | 364 | no | prologue |
| `0x0014a35c` | 28 | `0x0014a360` | 8 | `0x0014a364` | 20 | no | prologue |
| `0x0014a378` | 28 | `0x0014a37c` | 8 | `0x0014a380` | 20 | no | prologue |
| `0x0014b20c` | 40 | `0x0014b220` | 24 | `0x0014b224` | 16 | no | aapcs_live_arguments |
| `0x0014b85c` | 480 | `0x0014b8d4` | 124 | `0x0014b8d8` | 356 | no | prologue |
| `0x0014cad4` | 40 | `0x0014caec` | 28 | `0x0014caf0` | 12 | no | aapcs_live_arguments |
| `0x0014d000` | 88 | `0x0014d028` | 44 | `0x0014d02c` | 44 | no | prologue |
| `0x0014ded8` | 156 | `0x0014df08` | 52 | `0x0014df0c` | 104 | no | aapcs_live_arguments |
| `0x0014df74` | 260 | `0x0014dfa4` | 52 | `0x0014dfa8` | 208 | no | aapcs_live_arguments |
| `0x0014e20c` | 704 | `0x0014e214` | 12 | `0x0014e218` | 692 | yes | existing_entry, prologue |
| `0x0014e504` | 344 | `0x0014e508` | 8 | `0x0014e50c` | 336 | no | prologue, aapcs_live_arguments |
| `0x0014fa30` | 412 | `0x0014fa44` | 24 | `0x0014fa48` | 388 | no | prologue |
| `0x001525f0` | 52 | `0x0015260c` | 32 | `0x00152610` | 20 | yes | existing_entry, aapcs_live_arguments |
| `0x0015263c` | 80 | `0x00152660` | 40 | `0x00152664` | 40 | no | aapcs_live_arguments |
| `0x001526b4` | 152 | `0x001526dc` | 44 | `0x001526e0` | 108 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x001538f4` | 108 | `0x0015391c` | 44 | `0x00153920` | 64 | no | aapcs_live_arguments |
| `0x001545ec` | 28 | `0x001545f0` | 8 | `0x001545f4` | 20 | no | aapcs_live_arguments |
| `0x00154608` | 28 | `0x0015460c` | 8 | `0x00154610` | 20 | no | aapcs_live_arguments |
| `0x00154f78` | 360 | `0x00154f90` | 28 | `0x00154f94` | 332 | no | prologue, aapcs_live_arguments |
| `0x001550e0` | 44 | `0x001550e4` | 8 | `0x001550e8` | 36 | no | prologue |
| `0x00156398` | 164 | `0x0015639c` | 8 | `0x001563a0` | 156 | no | prologue, aapcs_live_arguments |
| `0x0015e5f0` | 52 | `0x0015e608` | 28 | `0x0015e60c` | 24 | yes | existing_entry, aapcs_live_arguments |
| `0x0015e780` | 164 | `0x0015e788` | 12 | `0x0015e78c` | 152 | no | aapcs_live_arguments |
| `0x0015e824` | 160 | `0x0015e82c` | 12 | `0x0015e830` | 148 | no | aapcs_live_arguments |
| `0x0015e948` | 36 | `0x0015e950` | 12 | `0x0015e954` | 24 | no | aapcs_live_arguments |
| `0x001678dc` | 856 | `0x00167914` | 60 | `0x00167918` | 796 | yes | existing_entry, prologue, pointer, aapcs_live_arguments |
| `0x00169920` | 84 | `0x00169928` | 12 | `0x0016992c` | 72 | yes | existing_entry, aapcs_live_arguments |
| `0x0016b248` | 52 | `0x0016b25c` | 24 | `0x0016b260` | 28 | yes | existing_entry |
| `0x0016b58c` | 208 | `0x0016b5f8` | 112 | `0x0016b5fc` | 96 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x0016bda4` | 476 | `0x0016bddc` | 60 | `0x0016bde0` | 416 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x0016dae8` | 360 | `0x0016dbc0` | 220 | `0x0016dbc4` | 140 | yes | existing_entry, prologue |
| `0x0016e1c8` | 168 | `0x0016e1cc` | 8 | `0x0016e1d0` | 160 | yes | existing_entry, aapcs_live_arguments |
| `0x0016e61c` | 380 | `0x0016e644` | 44 | `0x0016e648` | 336 | yes | existing_entry, aapcs_live_arguments |
| `0x0016e798` | 220 | `0x0016e7dc` | 72 | `0x0016e7e0` | 148 | yes | existing_entry, aapcs_live_arguments |
| `0x00170298` | 240 | `0x001702f8` | 100 | `0x001702fc` | 140 | yes | existing_entry, aapcs_live_arguments |
| `0x00170db4` | 132 | `0x00170dbc` | 12 | `0x00170dc0` | 120 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x00170e50` | 208 | `0x00170e54` | 8 | `0x00170e58` | 200 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x001719f4` | 148 | `0x00171a30` | 64 | `0x00171a34` | 84 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x00178154` | 3400 | `0x00178e94` | 3396 | `0x00178e98` | 4 | yes | existing_entry |
| `0x0017a518` | 152 | `0x0017a554` | 64 | `0x0017a558` | 88 | yes | existing_entry |
| `0x0017a6f4` | 240 | `0x0017a718` | 40 | `0x0017a71c` | 200 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x0017a9ac` | 228 | `0x0017a9b0` | 8 | `0x0017a9b4` | 220 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x0017df50` | 140 | `0x0017df80` | 52 | `0x0017df84` | 88 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x0017fe60` | 980 | `0x0017fe94` | 56 | `0x0017fe98` | 924 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x00180358` | 120 | `0x00180384` | 48 | `0x00180388` | 72 | yes | existing_entry, aapcs_live_arguments |
| `0x0018075c` | 100 | `0x00180764` | 12 | `0x00180768` | 88 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x00180860` | 228 | `0x00180880` | 36 | `0x00180884` | 192 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x001810e4` | 212 | `0x001810fc` | 28 | `0x00181100` | 184 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x001812f8` | 60 | `0x00181324` | 48 | `0x00181328` | 12 | yes | existing_entry |
| `0x00181420` | 200 | `0x00181444` | 40 | `0x00181448` | 160 | yes | existing_entry, prologue |
| `0x00181724` | 36 | `0x00181730` | 16 | `0x00181734` | 20 | yes | existing_entry |
| `0x0018221c` | 184 | `0x0018227c` | 100 | `0x00182280` | 84 | yes | existing_entry, prologue |
| `0x0018290c` | 196 | `0x00182990` | 136 | `0x00182994` | 60 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x0018371c` | 60 | `0x00183720` | 8 | `0x00183724` | 52 | yes | existing_entry, aapcs_live_arguments |
| `0x001838ec` | 224 | `0x001838fc` | 20 | `0x00183900` | 204 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x00183f08` | 28 | `0x00183f14` | 16 | `0x00183f18` | 12 | yes | existing_entry |
| `0x00186874` | 148 | `0x00186880` | 16 | `0x00186884` | 132 | yes | existing_entry |
| `0x00186fbc` | 552 | `0x00186fc4` | 12 | `0x00186fc8` | 540 | yes | existing_entry, prologue |
| `0x0018d860` | 52 | `0x0018d864` | 8 | `0x0018d868` | 44 | yes | existing_entry, aapcs_live_arguments |
| `0x0018d894` | 64 | `0x0018d8c0` | 48 | `0x0018d8c4` | 16 | yes | existing_entry |
| `0x0018eef0` | 132 | `0x0018ef00` | 20 | `0x0018ef04` | 112 | yes | existing_entry, prologue |
| `0x0018f634` | 148 | `0x0018f6c0` | 144 | `0x0018f6c4` | 4 | yes | existing_entry |
| `0x00190634` | 68 | `0x00190638` | 8 | `0x0019063c` | 60 | yes | existing_entry |
| `0x00191144` | 52 | `0x00191150` | 16 | `0x00191154` | 36 | yes | existing_entry, aapcs_live_arguments |
| `0x001911f0` | 192 | `0x00191248` | 92 | `0x0019124c` | 100 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x001913fc` | 128 | `0x00191454` | 92 | `0x00191458` | 36 | yes | existing_entry, aapcs_live_arguments |
| `0x00192410` | 72 | `0x00192428` | 28 | `0x0019242c` | 44 | yes | existing_entry |
| `0x001944e8` | 28 | `0x001944ec` | 8 | `0x001944f0` | 20 | yes | existing_entry, aapcs_live_arguments |
| `0x00195168` | 236 | `0x001951d8` | 116 | `0x001951dc` | 120 | yes | existing_entry, prologue |
| `0x00195a54` | 280 | `0x00195acc` | 124 | `0x00195ad0` | 156 | yes | existing_entry |
| `0x00195e5c` | 40 | `0x00195e60` | 8 | `0x00195e64` | 32 | yes | existing_entry |
| `0x00195ea4` | 60 | `0x00195ec0` | 32 | `0x00195ec4` | 28 | yes | existing_entry, prologue |
| `0x001963e8` | 176 | `0x001963f4` | 16 | `0x001963f8` | 160 | yes | existing_entry, prologue |
| `0x0019c9b0` | 396 | `0x0019c9d0` | 36 | `0x0019c9d4` | 360 | yes | existing_entry, prologue |
| `0x0019d788` | 316 | `0x0019d794` | 16 | `0x0019d798` | 300 | yes | existing_entry, prologue |
| `0x0019ea64` | 192 | `0x0019ea68` | 8 | `0x0019ea6c` | 184 | yes | existing_entry, prologue |
| `0x0019ec64` | 148 | `0x0019ec84` | 36 | `0x0019ec88` | 112 | yes | existing_entry, prologue |
| `0x0019edbc` | 308 | `0x0019edc8` | 16 | `0x0019edcc` | 292 | yes | existing_entry, prologue |
| `0x0019f518` | 164 | `0x0019f564` | 80 | `0x0019f568` | 84 | yes | existing_entry |
| `0x001adff8` | 328 | `0x001ae008` | 20 | `0x001ae00c` | 308 | no | prologue |
| `0x001b3ac4` | 324 | `0x001b3b00` | 64 | `0x001b3b04` | 260 | no | aapcs_live_arguments |
| `0x001c228c` | 1824 | `0x001c2298` | 16 | `0x001c229c` | 1808 | no | prologue, aapcs_live_arguments |
| `0x001c5918` | 56 | `0x001c592c` | 24 | `0x001c5930` | 32 | no | aapcs_live_arguments |
| `0x001c5950` | 32 | `0x001c5954` | 8 | `0x001c5958` | 24 | no | aapcs_live_arguments |
| `0x001c5970` | 36 | `0x001c5974` | 8 | `0x001c5978` | 28 | no | aapcs_live_arguments |
| `0x001c5994` | 104 | `0x001c5998` | 8 | `0x001c599c` | 96 | no | prologue, aapcs_live_arguments |
| `0x001c59fc` | 32 | `0x001c5a04` | 12 | `0x001c5a08` | 20 | no | aapcs_live_arguments |
| `0x001cf418` | 236 | `0x001cf444` | 48 | `0x001cf448` | 188 | yes | existing_entry, prologue, aapcs_live_arguments |
| `0x001cff50` | 64 | `0x001cff54` | 8 | `0x001cff58` | 56 | yes | existing_entry |
| `0x001d4124` | 88 | `0x001d4170` | 80 | `0x001d4174` | 8 | yes | existing_entry, aapcs_live_arguments |
| `0x001dac54` | 104 | `0x001dac80` | 48 | `0x001dac84` | 56 | yes | existing_entry, prologue, pointer, aapcs_live_arguments |
| `0x001dacf8` | 200 | `0x001dad44` | 80 | `0x001dad48` | 120 | yes | existing_entry, prologue, pointer, aapcs_live_arguments |
| `0x001dd4f8` | 292 | `0x001dd500` | 12 | `0x001dd504` | 280 | no | aapcs_live_arguments |
| `0x001debb4` | 1140 | `0x001decbc` | 268 | `0x001decc0` | 872 | no | prologue |
| `0x001e9944` | 436 | `0x001e994c` | 12 | `0x001e9950` | 424 | no | prologue |
| `0x001efeac` | 340 | `0x001efee0` | 56 | `0x001efee4` | 284 | no | prologue, aapcs_live_arguments |
| `0x001f2404` | 144 | `0x001f2408` | 8 | `0x001f240c` | 136 | no | prologue, aapcs_live_arguments |
| `0x001f2b90` | 28 | `0x001f2b94` | 8 | `0x001f2b98` | 20 | no | prologue |
| `0x001f4b58` | 340 | `0x001f4b8c` | 56 | `0x001f4b90` | 284 | no | prologue, aapcs_live_arguments |

## 8. Full classification of every census tail position

`saved LR` distinguishes the independent stack-LR guard from the census’s textual heuristic. Crossing counts include switch-table cases. The 15 unwitnessed and eight unowned positions remain explicit evidence limits, not silently guessed function boundaries.

| Tail NOP | v5 owner | Census class | Final decision | Successor | Start evidence | Crossing control / data | Saved stack LR |
| --- | --- | --- | --- | --- | --- | --- | --- |
| `0x00100078` | `0x00100078` | tail:nop_entry | **already_separate** | `0x0010007c` | — | 0 / 0 | no |
| `0x00100cd4` | `0x00100ca8` | tail:in_leaf | **split** | `0x00100cd8` | aapcs_live_arguments | 0 / 0 | no |
| `0x00100d68` | `0x00100d68` | tail:nop_entry | **already_separate** | `0x00100d6c` | — | 0 / 0 | no |
| `0x001071e8` | `0x001071cc` | tail:after_epilogue | **split** | `0x001071ec` | prologue | 0 / 0 | no |
| `0x00109ed8` | `0x00109a48` | tail:in_leaf | **control_flow_crosses** | `0x00109edc` | — | 11 / 10 | yes |
| `0x00109f48` | `0x00109a48` | tail:in_leaf | **control_flow_crosses** | `0x00109f4c` | — | 11 / 11 | yes |
| `0x0010a368` | `0x00109a48` | tail:in_leaf | **control_flow_crosses** | `0x0010a36c` | — | 15 / 19 | yes |
| `0x0010a4b4` | `0x00109a48` | tail:in_leaf | **control_flow_crosses** | `0x0010a4b8` | — | 17 / 23 | yes |
| `0x0010a530` | `0x00109a48` | tail:in_leaf | **control_flow_crosses** | `0x0010a534` | — | 14 / 24 | yes |
| `0x0010bef0` | `0x0010bed4` | tail:after_epilogue | **no_independent_successor_witness** | `0x0010bef4` | — | 0 / 0 | no |
| `0x0010c338` | `0x0010c338` | tail:nop_entry | **already_separate** | `0x0010c33c` | — | 0 / 0 | no |
| `0x0010c37c` | `0x0010c37c` | tail:nop_entry | **already_separate** | `0x0010c380` | — | 0 / 0 | no |
| `0x0010d2b4` | `0x0010d27c` | tail:after_epilogue | **split** | `0x0010d2b8` | prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x0010e67c` | `0x0010e67c` | tail:nop_entry | **already_separate** | `0x0010e680` | — | 0 / 0 | no |
| `0x00110aa4` | `0x00110aa4` | tail:nop_entry | **already_separate** | `0x00110aa8` | — | 0 / 0 | no |
| `0x00111110` | `0x001110f0` | tail:before_prologue | **split** | `0x00111114` | prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00111d2c` | `0x00111af4` | tail:in_leaf | **control_flow_crosses** | `0x00111d30` | — | 6 / 8 | yes |
| `0x00111e94` | `0x00111af4` | tail:in_leaf | **control_flow_crosses** | `0x00111e98` | — | 5 / 9 | yes |
| `0x00111ef0` | `0x00111af4` | tail:in_leaf | **control_flow_crosses** | `0x00111ef4` | — | 2 / 11 | yes |
| `0x00112c94` | `0x00112c80` | tail:after_epilogue | **split** | `0x00112c98` | prologue | 0 / 0 | no |
| `0x001139e0` | `0x001139dc` | tail:in_leaf | **split** | `0x001139e4` | aapcs_live_arguments | 0 / 0 | no |
| `0x00113a30` | `0x00113a2c` | tail:in_leaf | **split** | `0x00113a34` | aapcs_live_arguments | 0 / 0 | no |
| `0x00114fa4` | `0x00114fa4` | tail:nop_entry | **already_separate** | `0x00114fa8` | — | 0 / 0 | no |
| `0x00117748` | `0x001176e0` | tail:falls_into_entry | **split** | `0x0011774c` | existing_entry | 0 / 0 | no |
| `0x0011eb4c` | `0x0011e99c` | tail:in_leaf | **control_flow_crosses** | `0x0011eb50` | — | 6 / 3 | yes |
| `0x0011eb60` | `0x0011e99c` | tail:in_leaf | **control_flow_crosses** | `0x0011eb64` | — | 5 / 3 | yes |
| `0x0011ec5c` | `0x0011e99c` | tail:in_leaf | **control_flow_crosses** | `0x0011ec60` | — | 9 / 3 | yes |
| `0x0011ecf8` | `0x0011e99c` | tail:in_leaf | **control_flow_crosses** | `0x0011ecfc` | — | 7 / 0 | yes |
| `0x0011ef48` | `0x0011ef48` | tail:nop_entry | **already_separate** | `0x0011ef4c` | — | 0 / 0 | no |
| `0x0011ef54` | `0x0011ef54` | tail:nop_entry | **already_separate** | `0x0011ef58` | — | 0 / 0 | no |
| `0x00121764` | `0x00121764` | tail:nop_entry | **already_separate** | `0x00121768` | — | 0 / 0 | no |
| `0x001217b4` | `0x001217b4` | tail:nop_entry | **already_separate** | `0x001217b8` | — | 0 / 0 | no |
| `0x00121bc4` | `0x00121bc4` | tail:nop_entry | **already_separate** | `0x00121bc8` | — | 0 / 0 | no |
| `0x00121da0` | `0x00121da0` | tail:nop_entry | **already_separate** | `0x00121da4` | — | 0 / 0 | no |
| `0x00121ddc` | `0x00121ddc` | tail:nop_entry | **already_separate** | `0x00121de0` | — | 0 / 0 | no |
| `0x001224a0` | `0x001224a0` | tail:nop_entry | **already_separate** | `0x001224a4` | — | 0 / 0 | no |
| `0x001224fc` | `0x001224fc` | tail:nop_entry | **already_separate** | `0x00122500` | — | 0 / 0 | no |
| `0x0012250c` | `0x0012250c` | tail:nop_entry | **already_separate** | `0x00122510` | — | 0 / 0 | no |
| `0x00122788` | `0x00122788` | tail:nop_entry | **already_separate** | `0x0012278c` | — | 0 / 0 | no |
| `0x00122890` | `0x00122890` | tail:nop_entry | **already_separate** | `0x00122894` | — | 0 / 0 | no |
| `0x001228bc` | `0x001228bc` | tail:nop_entry | **already_separate** | `0x001228c0` | — | 0 / 0 | no |
| `0x001228d0` | `0x001228d0` | tail:nop_entry | **already_separate** | `0x001228d4` | — | 0 / 0 | no |
| `0x00122984` | `0x00122984` | tail:nop_entry | **already_separate** | `0x00122988` | — | 0 / 0 | no |
| `0x001229b8` | `0x001229b8` | tail:nop_entry | **already_separate** | `0x001229bc` | — | 0 / 0 | no |
| `0x001229d0` | `0x001229d0` | tail:nop_entry | **already_separate** | `0x001229d4` | — | 0 / 0 | no |
| `0x00122a1c` | `0x00122a1c` | tail:nop_entry | **already_separate** | `0x00122a20` | — | 0 / 0 | no |
| `0x00122a68` | `0x00122a68` | tail:nop_entry | **already_separate** | `0x00122a6c` | — | 0 / 0 | no |
| `0x00123a6c` | `0x00123a64` | tail:in_leaf | **split** | `0x00123a70` | aapcs_live_arguments | 0 / 0 | no |
| `0x001248fc` | `0x00124874` | tail:before_prologue | **split** | `0x00124900` | prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00125b50` | `0x00125b50` | tail:nop_entry | **already_separate** | `0x00125b54` | — | 0 / 0 | no |
| `0x0012739c` | `0x00127398` | tail:falls_into_entry | **split** | `0x001273a0` | existing_entry, prologue | 0 / 0 | no |
| `0x001277b8` | `0x001277b4` | tail:in_leaf | **split** | `0x001277bc` | aapcs_live_arguments | 0 / 0 | no |
| `0x0012793c` | `0x00127934` | tail:before_prologue | **split** | `0x00127940` | prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x0012d0b4` | `0x0012d0a4` | tail:falls_into_entry | **split** | `0x0012d0b8` | existing_entry | 0 / 0 | no |
| `0x0012d144` | `0x0012d104` | tail:after_epilogue | **split** | `0x0012d148` | pointer | 0 / 0 | no |
| `0x0012d1d4` | `0x0012d1cc` | tail:falls_into_entry | **split** | `0x0012d1d8` | existing_entry, prologue | 0 / 0 | no |
| `0x0012d5c0` | `0x0012d5c0` | tail:nop_entry | **already_separate** | `0x0012d5c4` | — | 0 / 0 | no |
| `0x0012d62c` | `0x0012d62c` | tail:nop_entry | **already_separate** | `0x0012d630` | — | 0 / 0 | no |
| `0x0012f518` | `0x0012f518` | tail:nop_entry | **already_separate** | `0x0012f51c` | — | 0 / 0 | no |
| `0x0012fa20` | `0x0012f9fc` | tail:after_epilogue | **split** | `0x0012fa24` | prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x0012fb38` | `0x0012fb38` | tail:nop_entry | **already_separate** | `0x0012fb3c` | — | 0 / 0 | no |
| `0x0012fb48` | `0x0012fb48` | tail:nop_entry | **already_separate** | `0x0012fb4c` | — | 0 / 0 | no |
| `0x00131924` | `0x00131920` | tail:before_prologue | **split** | `0x00131928` | prologue | 0 / 0 | no |
| `0x0013194c` | `0x00131948` | tail:in_leaf | **no_independent_successor_witness** | `0x00131950` | — | 0 / 0 | no |
| `0x00136130` | `0x00136104` | tail:in_leaf | **split** | `0x00136134` | aapcs_live_arguments | 0 / 0 | no |
| `0x001363b0` | `0x001363b0` | tail:nop_entry | **already_separate** | `0x001363b4` | — | 0 / 0 | no |
| `0x00136404` | `0x00136404` | tail:nop_entry | **already_separate** | `0x00136408` | — | 0 / 0 | no |
| `0x00136654` | `0x00136630` | tail:before_prologue | **split** | `0x00136658` | prologue | 0 / 0 | no |
| `0x00136750` | `0x0013672c` | tail:before_prologue | **split** | `0x00136754` | prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00136858` | `0x00136858` | tail:nop_entry | **already_separate** | `0x0013685c` | — | 0 / 0 | no |
| `0x0013706c` | `0x0013706c` | tail:nop_entry | **already_separate** | `0x00137070` | — | 0 / 0 | no |
| `0x001370ac` | `0x001370ac` | tail:nop_entry | **already_separate** | `0x001370b0` | — | 0 / 0 | no |
| `0x00137124` | `0x00137118` | tail:after_epilogue | **split** | `0x00137128` | return_only_successor | 0 / 0 | no |
| `0x0013714c` | `0x0013714c` | tail:nop_entry | **already_separate** | `0x00137150` | — | 0 / 0 | no |
| `0x0013776c` | `0x0013776c` | tail:nop_entry | **already_separate** | `0x00137770` | — | 0 / 0 | no |
| `0x001377a0` | `0x001377a0` | tail:nop_entry | **already_separate** | `0x001377a4` | — | 0 / 0 | no |
| `0x001378d4` | `0x001378d4` | tail:nop_entry | **already_separate** | `0x001378d8` | — | 0 / 0 | no |
| `0x0013e7d0` | `0x0013e7d0` | tail:nop_entry | **already_separate** | `0x0013e7d4` | — | 0 / 0 | no |
| `0x00140a0c` | `0x00140a08` | tail:in_leaf | **split** | `0x00140a10` | aapcs_live_arguments | 0 / 0 | no |
| `0x00140ec8` | `0x00140ec4` | tail:before_prologue | **split** | `0x00140ecc` | prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x0014155c` | `0x00141554` | tail:falls_into_entry | **split** | `0x00141560` | existing_entry, prologue | 0 / 0 | no |
| `0x00141c70` | `0x00141c70` | tail:nop_entry | **already_separate** | `0x00141c74` | — | 0 / 0 | no |
| `0x00142640` | `0x00142640` | tail:nop_entry | **already_separate** | `0x00142644` | — | 0 / 0 | no |
| `0x001426a4` | `0x00142684` | tail:after_epilogue | **split** | `0x001426a8` | prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00142714` | `0x001426f4` | tail:after_epilogue | **split** | `0x00142718` | prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x0014334c` | `0x00143328` | tail:falls_into_entry | **split** | `0x00143350` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00144220` | `0x00144220` | tail:nop_entry | **already_separate** | `0x00144224` | — | 0 / 0 | no |
| `0x001467e0` | `0x00146758` | tail:after_epilogue | **split** | `0x001467e4` | return_only_successor | 0 / 0 | no |
| `0x00147994` | `0x00147994` | tail:nop_entry | **already_separate** | `0x00147998` | — | 0 / 0 | no |
| `0x0014821c` | `0x00148210` | tail:falls_into_entry | **split** | `0x00148220` | existing_entry, prologue | 0 / 0 | no |
| `0x001483bc` | `0x0014837c` | tail:after_epilogue | **split** | `0x001483c0` | prologue | 0 / 0 | no |
| `0x0014a360` | `0x0014a35c` | tail:before_prologue | **split** | `0x0014a364` | prologue | 0 / 0 | no |
| `0x0014a37c` | `0x0014a378` | tail:before_prologue | **split** | `0x0014a380` | prologue | 0 / 0 | no |
| `0x0014a988` | `0x0014a988` | tail:nop_entry | **already_separate** | `0x0014a98c` | — | 0 / 0 | no |
| `0x0014a9d8` | `0x0014a9d8` | tail:nop_entry | **already_separate** | `0x0014a9dc` | — | 0 / 0 | no |
| `0x0014b220` | `0x0014b20c` | tail:after_epilogue | **split** | `0x0014b224` | aapcs_live_arguments | 0 / 0 | no |
| `0x0014b8d4` | `0x0014b85c` | tail:after_epilogue | **split** | `0x0014b8d8` | prologue | 0 / 0 | no |
| `0x0014c338` | `0x0014c338` | tail:nop_entry | **already_separate** | `0x0014c33c` | — | 0 / 0 | no |
| `0x0014caec` | `0x0014cad4` | tail:after_epilogue | **split** | `0x0014caf0` | aapcs_live_arguments | 0 / 0 | no |
| `0x0014d028` | `0x0014d000` | tail:before_prologue | **split** | `0x0014d02c` | prologue | 0 / 0 | no |
| `0x0014df08` | `0x0014ded8` | tail:after_epilogue | **split** | `0x0014df0c` | aapcs_live_arguments | 0 / 0 | no |
| `0x0014dfa4` | `0x0014df74` | tail:after_epilogue | **split** | `0x0014dfa8` | aapcs_live_arguments | 0 / 0 | no |
| `0x0014e214` | `0x0014e20c` | tail:falls_into_entry | **split** | `0x0014e218` | existing_entry, prologue | 0 / 0 | no |
| `0x0014e508` | `0x0014e504` | tail:before_prologue | **split** | `0x0014e50c` | prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x0014fa44` | `0x0014fa30` | tail:after_epilogue | **split** | `0x0014fa48` | prologue | 0 / 0 | no |
| `0x0015260c` | `0x001525f0` | tail:falls_into_entry | **split** | `0x00152610` | existing_entry, aapcs_live_arguments | 0 / 0 | no |
| `0x00152660` | `0x0015263c` | tail:in_leaf | **split** | `0x00152664` | aapcs_live_arguments | 0 / 0 | no |
| `0x001526dc` | `0x001526b4` | tail:falls_into_entry | **split** | `0x001526e0` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00152754` | `0x0015274c` | tail:in_leaf | **no_independent_successor_witness** | `0x00152758` | — | 0 / 0 | no |
| `0x0015391c` | `0x001538f4` | tail:in_leaf | **split** | `0x00153920` | aapcs_live_arguments | 0 / 0 | no |
| `0x001545f0` | `0x001545ec` | tail:in_leaf | **split** | `0x001545f4` | aapcs_live_arguments | 0 / 0 | no |
| `0x0015460c` | `0x00154608` | tail:in_leaf | **split** | `0x00154610` | aapcs_live_arguments | 0 / 0 | no |
| `0x00154f90` | `0x00154f78` | tail:before_prologue | **split** | `0x00154f94` | prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x001550e4` | `0x001550e0` | tail:before_prologue | **split** | `0x001550e8` | prologue | 0 / 0 | no |
| `0x0015639c` | `0x00156398` | tail:before_prologue | **split** | `0x001563a0` | prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00156de8` | `0x00156de8` | tail:nop_entry | **already_separate** | `0x00156dec` | — | 0 / 0 | no |
| `0x0015e2b4` | `0x0015e2b4` | tail:nop_entry | **already_separate** | `0x0015e2b8` | — | 0 / 0 | no |
| `0x0015e31c` | `0x0015e31c` | tail:nop_entry | **already_separate** | `0x0015e320` | — | 0 / 0 | no |
| `0x0015e47c` | `0x0015e474` | tail:in_leaf | **no_independent_successor_witness** | `0x0015e480` | — | 0 / 0 | no |
| `0x0015e4a4` | `0x0015e49c` | tail:in_leaf | **no_independent_successor_witness** | `0x0015e4a8` | — | 0 / 0 | no |
| `0x0015e5d0` | `0x0015e5c4` | tail:in_leaf | **no_independent_successor_witness** | `0x0015e5d4` | — | 0 / 0 | no |
| `0x0015e608` | `0x0015e5f0` | tail:falls_into_entry | **split** | `0x0015e60c` | existing_entry, aapcs_live_arguments | 0 / 0 | no |
| `0x0015e630` | `0x0015e624` | tail:in_leaf | **no_independent_successor_witness** | `0x0015e634` | — | 0 / 0 | no |
| `0x0015e694` | `0x0015e684` | tail:in_leaf | **no_independent_successor_witness** | `0x0015e698` | — | 0 / 0 | no |
| `0x0015e788` | `0x0015e780` | tail:in_leaf | **split** | `0x0015e78c` | aapcs_live_arguments | 0 / 0 | no |
| `0x0015e82c` | `0x0015e824` | tail:in_leaf | **split** | `0x0015e830` | aapcs_live_arguments | 0 / 0 | no |
| `0x0015e950` | `0x0015e948` | tail:in_leaf | **split** | `0x0015e954` | aapcs_live_arguments | 0 / 0 | no |
| `0x00162ec4` | `0x00161ad0` | tail:in_leaf | **control_flow_crosses** | `0x00162ec8` | — | 3 / 0 | yes |
| `0x00164900` | `0x0016471c` | tail:in_leaf | **control_flow_crosses** | `0x00164904` | — | 8 / 2 | yes |
| `0x00164918` | `0x0016471c` | tail:in_leaf | **control_flow_crosses** | `0x0016491c` | — | 6 / 2 | yes |
| `0x00166eac` | `0x00166eac` | tail:nop_entry | **already_separate** | `0x00166eb0` | — | 0 / 0 | no |
| `0x00166f1c` | `0x00166f1c` | tail:nop_entry | **already_separate** | `0x00166f20` | — | 0 / 0 | no |
| `0x00167914` | `0x001678dc` | tail:falls_into_entry | **split** | `0x00167918` | existing_entry, prologue, pointer, aapcs_live_arguments | 0 / 0 | no |
| `0x00169874` | `0x00169874` | tail:nop_entry | **already_separate** | `0x00169878` | — | 0 / 0 | no |
| `0x00169928` | `0x00169920` | tail:falls_into_entry | **split** | `0x0016992c` | existing_entry, aapcs_live_arguments | 0 / 0 | no |
| `0x0016a994` | `0x0016a994` | tail:nop_entry | **already_separate** | `0x0016a998` | — | 0 / 0 | no |
| `0x0016b25c` | `0x0016b248` | tail:falls_into_entry | **split** | `0x0016b260` | existing_entry | 0 / 0 | no |
| `0x0016b5f8` | `0x0016b58c` | tail:falls_into_entry | **split** | `0x0016b5fc` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x0016bddc` | `0x0016bda4` | tail:falls_into_entry | **split** | `0x0016bde0` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x0016dbc0` | `0x0016dae8` | tail:falls_into_entry | **split** | `0x0016dbc4` | existing_entry, prologue | 0 / 0 | no |
| `0x0016e1cc` | `0x0016e1c8` | tail:falls_into_entry | **split** | `0x0016e1d0` | existing_entry, aapcs_live_arguments | 0 / 0 | no |
| `0x0016e644` | `0x0016e61c` | tail:falls_into_entry | **split** | `0x0016e648` | existing_entry, aapcs_live_arguments | 0 / 0 | no |
| `0x0016e7dc` | `0x0016e798` | tail:falls_into_entry | **split** | `0x0016e7e0` | existing_entry, aapcs_live_arguments | 0 / 0 | no |
| `0x0016f204` | `0x0016f174` | tail:falls_into_entry | **control_flow_crosses** | `0x0016f208` | existing_entry, aapcs_live_arguments | 1 / 0 | no |
| `0x0016f39c` | `0x0016f39c` | tail:nop_entry | **already_separate** | `0x0016f3a0` | — | 0 / 0 | no |
| `0x001702f8` | `0x00170298` | tail:falls_into_entry | **split** | `0x001702fc` | existing_entry, aapcs_live_arguments | 0 / 0 | no |
| `0x00170dbc` | `0x00170db4` | tail:falls_into_entry | **split** | `0x00170dc0` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00170e54` | `0x00170e50` | tail:falls_into_entry | **split** | `0x00170e58` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00171a30` | `0x001719f4` | tail:falls_into_entry | **split** | `0x00171a34` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00173d88` | `0x001737c0` | tail:falls_into_entry | **control_flow_crosses** | `0x00173d8c` | existing_entry | 3 / 0 | no |
| `0x001780c0` | `0x00177c58` | tail:falls_into_entry | **control_flow_crosses** | `0x001780c4` | existing_entry, aapcs_live_arguments | 8 / 0 | no |
| `0x00178e94` | `0x00178154` | tail:falls_into_entry | **split** | `0x00178e98` | existing_entry | 0 / 0 | no |
| `0x00179a44` | `0x001799b4` | tail:falls_into_entry | **control_flow_crosses** | `0x00179a48` | existing_entry, prologue, aapcs_live_arguments | 1 / 0 | no |
| `0x0017a554` | `0x0017a518` | tail:falls_into_entry | **split** | `0x0017a558` | existing_entry | 0 / 0 | no |
| `0x0017a718` | `0x0017a6f4` | tail:falls_into_entry | **split** | `0x0017a71c` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x0017a9b0` | `0x0017a9ac` | tail:falls_into_entry | **split** | `0x0017a9b4` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x0017df80` | `0x0017df50` | tail:falls_into_entry | **split** | `0x0017df84` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x0017e358` | `0x0017e358` | tail:nop_entry | **already_separate** | `0x0017e35c` | — | 0 / 0 | no |
| `0x0017fe94` | `0x0017fe60` | tail:falls_into_entry | **split** | `0x0017fe98` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00180384` | `0x00180358` | tail:falls_into_entry | **split** | `0x00180388` | existing_entry, aapcs_live_arguments | 0 / 0 | no |
| `0x00180574` | `—` | tail:outside_catalogue | **unowned_tail_no_head_start** | `0x00180578` | — | 0 / 0 | — |
| `0x00180764` | `0x0018075c` | tail:falls_into_entry | **split** | `0x00180768` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00180880` | `0x00180860` | tail:falls_into_entry | **split** | `0x00180884` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00180adc` | `0x00180a4c` | tail:falls_into_entry | **control_flow_crosses** | `0x00180ae0` | existing_entry, aapcs_live_arguments | 1 / 0 | no |
| `0x001810fc` | `0x001810e4` | tail:falls_into_entry | **split** | `0x00181100` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00181324` | `0x001812f8` | tail:falls_into_entry | **split** | `0x00181328` | existing_entry | 0 / 0 | no |
| `0x00181444` | `0x00181420` | tail:falls_into_entry | **split** | `0x00181448` | existing_entry, prologue | 0 / 0 | no |
| `0x00181730` | `0x00181724` | tail:falls_into_entry | **split** | `0x00181734` | existing_entry | 0 / 0 | no |
| `0x00181e40` | `0x00181e40` | tail:nop_entry | **already_separate** | `0x00181e44` | — | 0 / 0 | no |
| `0x0018227c` | `0x0018221c` | tail:falls_into_entry | **split** | `0x00182280` | existing_entry, prologue | 0 / 0 | no |
| `0x00182718` | `0x00182718` | tail:nop_entry | **already_separate** | `0x0018271c` | — | 0 / 0 | no |
| `0x00182990` | `0x0018290c` | tail:falls_into_entry | **split** | `0x00182994` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00183030` | `0x00183030` | tail:nop_entry | **already_separate** | `0x00183034` | — | 0 / 0 | no |
| `0x00183720` | `0x0018371c` | tail:falls_into_entry | **split** | `0x00183724` | existing_entry, aapcs_live_arguments | 0 / 0 | no |
| `0x001838fc` | `0x001838ec` | tail:falls_into_entry | **split** | `0x00183900` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00183f14` | `0x00183f08` | tail:falls_into_entry | **split** | `0x00183f18` | existing_entry | 0 / 0 | no |
| `0x001852f4` | `0x001852f4` | tail:nop_entry | **already_separate** | `0x001852f8` | — | 0 / 0 | no |
| `0x001856d4` | `0x001855fc` | tail:falls_into_entry | **control_flow_crosses** | `0x001856d8` | existing_entry, prologue | 1 / 0 | no |
| `0x00185b9c` | `0x00185b9c` | tail:nop_entry | **already_separate** | `0x00185ba0` | — | 0 / 0 | no |
| `0x00186484` | `0x00186484` | tail:nop_entry | **already_separate** | `0x00186488` | — | 0 / 0 | no |
| `0x00186880` | `0x00186874` | tail:falls_into_entry | **split** | `0x00186884` | existing_entry | 0 / 0 | no |
| `0x00186cec` | `0x00186afc` | tail:in_leaf | **control_flow_crosses** | `0x00186cf0` | — | 6 / 2 | yes |
| `0x00186d00` | `0x00186afc` | tail:in_leaf | **control_flow_crosses** | `0x00186d04` | — | 5 / 2 | yes |
| `0x00186eec` | `0x00186eec` | tail:nop_entry | **already_separate** | `0x00186ef0` | — | 0 / 0 | no |
| `0x00186fc4` | `0x00186fbc` | tail:falls_into_entry | **split** | `0x00186fc8` | existing_entry, prologue | 0 / 0 | no |
| `0x0018d864` | `0x0018d860` | tail:falls_into_entry | **split** | `0x0018d868` | existing_entry, aapcs_live_arguments | 0 / 0 | no |
| `0x0018d8c0` | `0x0018d894` | tail:falls_into_entry | **split** | `0x0018d8c4` | existing_entry | 0 / 0 | no |
| `0x0018ef00` | `0x0018eef0` | tail:falls_into_entry | **split** | `0x0018ef04` | existing_entry, prologue | 0 / 0 | no |
| `0x0018f6c0` | `0x0018f634` | tail:falls_into_entry | **split** | `0x0018f6c4` | existing_entry | 0 / 0 | no |
| `0x0018f7ec` | `0x0018f7ec` | tail:nop_entry | **already_separate** | `0x0018f7f0` | — | 0 / 0 | no |
| `0x001900a8` | `0x001900a8` | tail:nop_entry | **already_separate** | `0x001900ac` | — | 0 / 0 | no |
| `0x00190638` | `0x00190634` | tail:falls_into_entry | **split** | `0x0019063c` | existing_entry | 0 / 0 | no |
| `0x00191150` | `0x00191144` | tail:falls_into_entry | **split** | `0x00191154` | existing_entry, aapcs_live_arguments | 0 / 0 | no |
| `0x00191248` | `0x001911f0` | tail:falls_into_entry | **split** | `0x0019124c` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00191454` | `0x001913fc` | tail:falls_into_entry | **split** | `0x00191458` | existing_entry, aapcs_live_arguments | 0 / 0 | no |
| `0x00191ae8` | `0x0019198c` | tail:falls_into_entry | **control_flow_crosses** | `0x00191aec` | existing_entry | 1 / 0 | no |
| `0x00192428` | `0x00192410` | tail:falls_into_entry | **split** | `0x0019242c` | existing_entry | 0 / 0 | no |
| `0x001944ec` | `0x001944e8` | tail:falls_into_entry | **split** | `0x001944f0` | existing_entry, aapcs_live_arguments | 0 / 0 | no |
| `0x00194a30` | `0x001949f8` | tail:falls_into_entry | **control_flow_crosses** | `0x00194a34` | existing_entry, prologue, aapcs_live_arguments | 1 / 0 | no |
| `0x001951d8` | `0x00195168` | tail:falls_into_entry | **split** | `0x001951dc` | existing_entry, prologue | 0 / 0 | no |
| `0x00195acc` | `0x00195a54` | tail:falls_into_entry | **split** | `0x00195ad0` | existing_entry | 0 / 0 | no |
| `0x00195d70` | `0x00195cfc` | tail:falls_into_entry | **control_flow_crosses** | `0x00195d74` | existing_entry | 1 / 0 | no |
| `0x00195e60` | `0x00195e5c` | tail:falls_into_entry | **split** | `0x00195e64` | existing_entry | 0 / 0 | no |
| `0x00195ec0` | `0x00195ea4` | tail:falls_into_entry | **split** | `0x00195ec4` | existing_entry, prologue | 0 / 0 | no |
| `0x001963f4` | `0x001963e8` | tail:falls_into_entry | **split** | `0x001963f8` | existing_entry, prologue | 0 / 0 | no |
| `0x0019721c` | `0x0019721c` | tail:nop_entry | **already_separate** | `0x00197220` | — | 0 / 0 | no |
| `0x0019c9d0` | `0x0019c9b0` | tail:falls_into_entry | **split** | `0x0019c9d4` | existing_entry, prologue | 0 / 0 | no |
| `0x0019d5e0` | `—` | tail:outside_catalogue | **unowned_tail_no_head_start** | `0x0019d5e4` | — | 0 / 0 | — |
| `0x0019d794` | `0x0019d788` | tail:falls_into_entry | **split** | `0x0019d798` | existing_entry, prologue | 0 / 0 | no |
| `0x0019dc1c` | `0x0019dc1c` | tail:nop_entry | **already_separate** | `0x0019dc20` | — | 0 / 0 | no |
| `0x0019ea68` | `0x0019ea64` | tail:falls_into_entry | **split** | `0x0019ea6c` | existing_entry, prologue | 0 / 0 | no |
| `0x0019eb48` | `0x0019eb48` | tail:nop_entry | **already_separate** | `0x0019eb4c` | — | 0 / 0 | no |
| `0x0019ec84` | `0x0019ec64` | tail:falls_into_entry | **split** | `0x0019ec88` | existing_entry, prologue | 0 / 0 | no |
| `0x0019edc8` | `0x0019edbc` | tail:falls_into_entry | **split** | `0x0019edcc` | existing_entry, prologue | 0 / 0 | no |
| `0x0019f2e4` | `0x0019f2e4` | tail:nop_entry | **already_separate** | `0x0019f2e8` | — | 0 / 0 | no |
| `0x0019f564` | `0x0019f518` | tail:falls_into_entry | **split** | `0x0019f568` | existing_entry | 0 / 0 | no |
| `0x0019ffa4` | `0x0019ffa4` | tail:nop_entry | **already_separate** | `0x0019ffa8` | — | 0 / 0 | no |
| `0x001a0070` | `0x001a000c` | tail:in_leaf | **control_flow_crosses** | `0x001a0074` | aapcs_live_arguments | 1 / 0 | no |
| `0x001ae008` | `0x001adff8` | tail:before_prologue | **split** | `0x001ae00c` | prologue | 0 / 0 | no |
| `0x001b3134` | `0x001b1f20` | tail:in_leaf | **literal_ownership_crosses** | `0x001b3138` | — | 0 / 5 | no |
| `0x001b3b00` | `0x001b3ac4` | tail:in_leaf | **split** | `0x001b3b04` | aapcs_live_arguments | 0 / 0 | no |
| `0x001b472c` | `0x001b472c` | tail:nop_entry | **already_separate** | `0x001b4730` | — | 0 / 0 | no |
| `0x001b54e8` | `0x001b54e8` | tail:nop_entry | **already_separate** | `0x001b54ec` | — | 0 / 0 | no |
| `0x001c2298` | `0x001c228c` | tail:before_prologue | **split** | `0x001c229c` | prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x001c3abc` | `0x001c3abc` | tail:nop_entry | **already_separate** | `0x001c3ac0` | — | 0 / 0 | no |
| `0x001c563c` | `0x001c5630` | tail:after_epilogue | **no_independent_successor_witness** | `0x001c5640` | — | 0 / 0 | no |
| `0x001c592c` | `0x001c5918` | tail:in_leaf | **split** | `0x001c5930` | aapcs_live_arguments | 0 / 0 | no |
| `0x001c5954` | `0x001c5950` | tail:in_leaf | **split** | `0x001c5958` | aapcs_live_arguments | 0 / 0 | no |
| `0x001c5974` | `0x001c5970` | tail:in_leaf | **split** | `0x001c5978` | aapcs_live_arguments | 0 / 0 | no |
| `0x001c5998` | `0x001c5994` | tail:before_prologue | **split** | `0x001c599c` | prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x001c5a04` | `0x001c59fc` | tail:in_leaf | **split** | `0x001c5a08` | aapcs_live_arguments | 0 / 0 | no |
| `0x001c5b84` | `0x001c5b84` | tail:nop_entry | **already_separate** | `0x001c5b88` | — | 0 / 0 | no |
| `0x001c6a34` | `0x001c6a34` | tail:nop_entry | **already_separate** | `0x001c6a38` | — | 0 / 0 | no |
| `0x001c6ad8` | `0x001c6ad8` | tail:nop_entry | **already_separate** | `0x001c6adc` | — | 0 / 0 | no |
| `0x001c775c` | `—` | tail:outside_catalogue | **unowned_tail_no_head_start** | `0x001c7760` | — | 0 / 0 | — |
| `0x001cf444` | `0x001cf418` | tail:falls_into_entry | **split** | `0x001cf448` | existing_entry, prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x001cff54` | `0x001cff50` | tail:falls_into_entry | **split** | `0x001cff58` | existing_entry | 0 / 0 | no |
| `0x001d0344` | `0x001d033c` | tail:in_leaf | **no_independent_successor_witness** | `0x001d0348` | — | 0 / 0 | no |
| `0x001d2230` | `0x001d222c` | tail:in_leaf | **no_independent_successor_witness** | `0x001d2234` | — | 0 / 0 | no |
| `0x001d2c74` | `0x001d2c44` | tail:after_epilogue | **no_independent_successor_witness** | `0x001d2c78` | — | 0 / 0 | no |
| `0x001d2d10` | `0x001d2cf0` | tail:after_epilogue | **no_independent_successor_witness** | `0x001d2d14` | — | 0 / 0 | no |
| `0x001d32e0` | `0x001d32e0` | tail:nop_entry | **already_separate** | `0x001d32e4` | — | 0 / 0 | no |
| `0x001d4170` | `0x001d4124` | tail:falls_into_entry | **split** | `0x001d4174` | existing_entry, aapcs_live_arguments | 0 / 0 | no |
| `0x001d54ac` | `0x001d54ac` | tail:nop_entry | **already_separate** | `0x001d54b0` | — | 0 / 0 | no |
| `0x001d5c9c` | `—` | tail:outside_catalogue | **unowned_tail_no_head_start** | `0x001d5ca0` | — | 0 / 0 | — |
| `0x001d7868` | `—` | tail:outside_catalogue | **unowned_tail_no_head_start** | `0x001d786c` | — | 0 / 0 | — |
| `0x001d80ac` | `—` | tail:outside_catalogue | **unowned_tail_no_head_start** | `0x001d80b0` | — | 0 / 0 | — |
| `0x001d89e0` | `—` | tail:outside_catalogue | **unowned_tail_no_head_start** | `0x001d89e4` | — | 0 / 0 | — |
| `0x001dac80` | `0x001dac54` | tail:falls_into_entry | **split** | `0x001dac84` | existing_entry, prologue, pointer, aapcs_live_arguments | 0 / 0 | no |
| `0x001dacbc` | `0x001dacbc` | tail:nop_entry | **already_separate** | `0x001dacc0` | — | 0 / 0 | no |
| `0x001dad44` | `0x001dacf8` | tail:falls_into_entry | **split** | `0x001dad48` | existing_entry, prologue, pointer, aapcs_live_arguments | 0 / 0 | no |
| `0x001dadc0` | `0x001dadc0` | tail:nop_entry | **already_separate** | `0x001dadc4` | — | 0 / 0 | no |
| `0x001db130` | `0x001db130` | tail:nop_entry | **already_separate** | `0x001db134` | — | 0 / 0 | no |
| `0x001dd500` | `0x001dd4f8` | tail:in_leaf | **split** | `0x001dd504` | aapcs_live_arguments | 0 / 0 | no |
| `0x001decbc` | `0x001debb4` | tail:after_epilogue | **split** | `0x001decc0` | prologue | 0 / 0 | no |
| `0x001e9818` | `0x001e9818` | tail:nop_entry | **already_separate** | `0x001e981c` | — | 0 / 0 | no |
| `0x001e994c` | `0x001e9944` | tail:before_prologue | **split** | `0x001e9950` | prologue | 0 / 0 | no |
| `0x001eafa0` | `0x001eafa0` | tail:nop_entry | **already_separate** | `0x001eafa4` | — | 0 / 0 | no |
| `0x001ef880` | `0x001ef880` | tail:nop_entry | **already_separate** | `0x001ef884` | — | 0 / 0 | no |
| `0x001efcd4` | `0x001efcd0` | tail:in_leaf | **no_independent_successor_witness** | `0x001efcd8` | — | 0 / 0 | no |
| `0x001efee0` | `0x001efeac` | tail:after_epilogue | **split** | `0x001efee4` | prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x001f2408` | `0x001f2404` | tail:before_prologue | **split** | `0x001f240c` | prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x001f2b94` | `0x001f2b90` | tail:before_prologue | **split** | `0x001f2b98` | prologue | 0 / 0 | no |
| `0x001f3e88` | `—` | tail:outside_catalogue | **unowned_tail_no_head_start** | `0x001f3e8c` | — | 0 / 0 | — |
| `0x001f4788` | `0x001f4784` | tail:in_leaf | **no_independent_successor_witness** | `0x001f478c` | — | 0 / 0 | no |
| `0x001f4b8c` | `0x001f4b58` | tail:after_epilogue | **split** | `0x001f4b90` | prologue, aapcs_live_arguments | 0 / 0 | no |
| `0x00216874` | `0x002166cc` | tail:in_leaf | **control_flow_crosses** | `0x00216878` | — | 6 / 3 | yes |
| `0x00216888` | `0x002166cc` | tail:in_leaf | **control_flow_crosses** | `0x0021688c` | — | 5 / 3 | yes |
| `0x00216984` | `0x002166cc` | tail:in_leaf | **control_flow_crosses** | `0x00216988` | — | 9 / 3 | yes |
| `0x00216a14` | `0x002166cc` | tail:in_leaf | **control_flow_crosses** | `0x00216a18` | — | 7 / 3 | yes |

## 9. Evidence artifact hashes

Paths relative to V.

| Artifact | SHA-256 |
| --- | --- |
| `inventory_tail_nops.py` | `62b75ed79fcf02db39f10ac21d88164e50b797ff638756ad7ced4ef510488b5f` |
| `tail-nop-inventory-v5.json` | `2dcaf0ff7909e0478dbc3b47cab4caaf19ec32a31bfb79a8c2e849116dc43f28` |
| `inventory-command.json` | `257cdd291a2648d4f301bc411efdf9e38fb4633fdca3bbd7052ebd9978dd7c29` |
| `check_catalogue_v6.py` | `d608fdbd046c1f48af6b269316c678d3d47dfd897386853f092d718dfcaf027e` |
| `verification-v6.json` | `95560307f8ce16190f883e1df1b44eed48bc8f2f4c5e28d04688680646b31f61` |
| `verification-adoption-safe-command.json` | `20d10edc85ef2b1609976b9ae9416237e058e3b61d8139e3b9f043906ca333c5` |
| `protected-functions-v6.json` | `98c6261fdcea76fa20969b9d032be8913fc8b56ee81c9da48756a9f97691e2fb` |
| `changed-records-v6.json` | `be30377d078843127575a1c46a94f8402cf187c3cea899fd64869443c8a0aaf4` |
| `tail-splits-v6.json` | `5a513ef9bae0dce92554df0d050c0d1175c3610b3e28b6aa17618f4d6fd8b16b` |
| `tail-classification-v6.json` | `ed79af68a3d5c30ef141b21d5febafe4878b3d961c548908390b9351ee9d8474` |
| `tests-v6-first.json` | `15ff773f42bd757c664425f96945cbc808968f42a97c30d9cc2d56b6dd587e2d` |
| `tests-v6-final.json` | `3fb7332f5f2357b8593c4ea425271f4af4f40848c1ed17a0d2a0c7aded9bc6d2` |
| `tests-v6-passed.json` | `767f359331a5ed844665ddadec57e4329443520d2d70d8f1ee4eed94de65d71f` |
| `tests-v6-adoption-safe.json` | `54d2a1abbbfed069f67ced7085e29d32afc742889b0c456ffa45d6ff52065113` |
| `catalogue-v6-adoption-safe-command.json` | `5d1f9c04eb727679bcb1e7fd2fe0fc8e808ac4c8dea5f963e23650e069b3b384` |
| `example-match-commands.json` | `39d13f190da5c3bb1a9e347a92e3135dd31bcea91cfb2439483abae2df22fa2e` |
| `example/func_00112c80/func_00112c80.result.json` | `62545bfa519e333ff001120013d2ca937a4cbf125d0d2d1871f50a10eac194c1` |
| `example/func_00112c98/func_00112c98.result.json` | `ebb7c1e83ca91a2644417739d2b9c4f8c6f41de31c0622334f0cf455eed8ea22` |

## Hand-off / remaining qualifications

- Complete within scope: every one of the 270 tail-census positions is classified, every independently justified non-crossing boundary is split, all v5 entries survive, every accepted/c004 record is unchanged, the named head/body both MATCH, and the touched module passes.
- The held positions are intentionally not ordinary-function guesses: 31 have crossing control, one has crossing literals, 15 lack independent successor evidence and eight lack an independently known head start. The full per-position evidence is preserved for any future investigation; no placeholder implementation remains.
- This is a candidate denominator revision. Re-adoption, independent review, final protected-source builds and checkpoint acceptance remain integrator actions. No accepted C progress was claimed from classification alone.
