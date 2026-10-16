# Catalogue v4 — corrected pointer recovery and whole-image target conservation

## Status, scope and identity

**PASS: corrected candidate, not yet re-adopted.** Main reported that the integrate branch adopted the earlier v4 SHA `ccb9168b75df6825bf1ee7b1ff7e86feb8f79c8b9a236b444a30bd8256f0f571`; this report and `catalogue-v4.json` supersede that candidate for re-adoption.
Runtime: **OpenAI / openai-codex/gpt-6.1-sol**. Worktree: `scratch/worktrees/catalogue-v4`, branch `catalogue-v4`, originally created from `armlink-inline` at c11a0ad. Assigned production edits remain only `tools/analyze.py` and `tests/test_analyze.py` in that worktree. The main checkout’s tracked source, canonical catalogue, ledger, history and receipts were not edited by this worker.
Private evidence root **V** = `scratch/worktrees/catalogue-v4/scratch/verification/`. Every repository Python command used `/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python`. Only `tests.test_analyze` was run. The follow-up performed no compilation: existing hash-bound objects were relinked against the corrected catalogue. No original executable bytes or disassembly were printed.

| Input / output | SHA-256 |
| --- | --- |
| Baseline v3 | `d8fc286cd6daf2b201dac18afa20f067fee983333bbb317a4123f7417e104ad7` |
| Previous adopted v4 | `ccb9168b75df6825bf1ee7b1ff7e86feb8f79c8b9a236b444a30bd8256f0f571` |
| Corrected `scratch/analyze/catalogue-v4.json` | `d490e753d55456f6624618c29150ec80dbbf1c3f0aed25fbbe406f58351506da` |
| Original image | `a367b5dcd65e3924c93cf3fbaee5d4d40f9c5205d1287912784a974d5e405361` |
| `tests/test_analyze.py` in the worktree | `3d0c514d32d39b35794c857bad324998ba85fb78aa9f079b60d9d64d6f205c7a` |
| `tools/analyze.py` in the worktree | `e1b61305455c809ebae6acd3a661f3d53537f205d010473e9476e1cc2162c24c` |
| `tools/inplace.py` in the worktree | `dea627025ecaaa2e9f3ff88603efe426b14a31a7bb2fb6a760447bfc786fbc6f` |

## 1. Disposition of all four reported targets

The reported packet/linker refusals are treated as observed failures, not rerun to confirm them. Catalogue inspection establishes the separate facts below. **The first three entries were never lost by v4 and are not members of the NOP groups split by this revision.** Their full records are identical in v3, the adopted ccb9168b candidate, and the corrected candidate.

| Target | v3 | ccb9168b v4 | Corrected v4 | Finding |
| --- | --- | --- | --- | --- |
| `0x00195ec4` | ARM entry of `0x00195ea4` (`multi_entry`) | ARM entry of `0x00195ea4` (`multi_entry`) | ARM entry of `0x00195ea4` (`multi_entry`) | unchanged alias; no catalogue loss |
| `0x00180768` | ARM entry of `0x0018075c` (`multi_entry`) | ARM entry of `0x0018075c` (`multi_entry`) | ARM entry of `0x0018075c` (`multi_entry`) | unchanged alias; no catalogue loss |
| `0x00180884` | ARM entry of `0x00180860` (`multi_entry`) | ARM entry of `0x00180860` (`multi_entry`) | ARM entry of `0x00180860` (`multi_entry`) | unchanged alias; no catalogue loss |
| `0x00149428` | absent | absent | ARM entry of `0x00149428` (`function`) | pre-existing v3 pointer-discovery gap repaired |

The missing callback at **`0x149428`** is now an ordinary **28 B** function. Its exact pointer witness is the pool word at **`0x1474bc`**, owned by **`func_00147478`**. Its six-instruction executable path ends in a direct tail B at `0x14943c` to the previously unseeded `0x14b2b0` leaf. The old pointer probe rejected the callback solely because that leaf was not yet a known seed; following the bounded tail chain provides the missing witness.

**Consumer qualification:** `packet.py` in the inspected branch uses only top-level function keys in `Namer` and `build_packet`; it does not expose an interior alias as a standalone packet target. The armlink-inline `Resolver` explicitly constructs its callable map from both top-level starts and `entries`. Thus the first three reported refusals are not evidence of missing catalogue entries; consumer/version routing still belongs to the integrator. No packet or linker source outside this assignment was changed, and no original failure was dismissed or rerun.

## 2. Whole-image target conservation

Executable check: **`V/check_target_preservation.py`**. It decodes every v3 `code_ranges` instruction in the correct ISA, including ARM instructions in interworking veneers. Every direct B/BL/BLX target that v3 resolves as a top-level start or entry must resolve in corrected v4 with the **same target ISA**. It also checks every v3 entry carrying `pointer` evidence and every rodata/data/pool pointer word that resolves to such a v3 callable address. A separate stronger check requires **all** v3 starts/entries to survive, even unreferenced prologue-only entries.

| Evidence lane | Resolved v3 evidence rows checked |
| --- | ---: |
| `data_pointer` | 67 |
| `direct_branch` | 1,202 |
| `direct_call` | 19,991 |
| `pointer_evidence` | 775 |
| `pool_pointer` | 381 |
| `rodata_pointer` | 722 |
| **Total** | **23,138** |

These rows cover **4,375 distinct callable address/ISA pairs**. All **4,472** v3 callable entries survive, including the entries not referenced by these evidence lanes. Intra-function branch labels that were not callable v3 entries are not misrepresented as required separate functions.

**Complete violation lists:**
- v3 → original ccb9168b v4: **`violations = []`, `all_entry_losses = []`**; exit **0**. The first three author-reported targets were already present.
- v3 → corrected v4: **`violations = []`, `all_entry_losses = []`**; exit **0**.
- Corrected catalogue structural validation: **`validate_catalogue(...) == []`**; no overlapping extents; all **1,327,104 mapped bytes** tile exactly once.

Artifacts: `V/target-preservation-ccb9168b.json`, `V/target-preservation-complete.json`, and their `*-command.json` records. Each audit binds both catalogue hashes and the original-image hash. `V/target-repair-metadata.json` also embeds the independently executed current audit.

## 3. Permanent fix and regression coverage

- Preserve v3 discovery and weak-boundary refinement order. Tail-chain following is **opt-in** in `pointer_plausible`, not enabled during the initial global discovery pass.
- After the v3 boundaries, literal recovery and armlink NOP splits converge, `promote_pointer_entries` examines pointer targets only in **unowned gaps**, excluding the interior of every finalized code/data extent.
- A bounded 32-instruction probe may follow explicit unconditional tail branches into an unseeded leaf. Before the first explicit tail, it may not fall through into the next published owner; this prevents a newly considered prefix from absorbing an existing weak start. Unknown instructions, ISA disagreement and unproved cycles remain rejected.
- Newly evidenced callbacks become seeds; existing `promote_missing_targets` then recovers their direct tail callees. `_pointer_targets` shares the existing rodata/data/non-switch-pool evidence collection with NOP-wrapper discovery. New additive metadata: `pointer_target_repairs`, `stats.repaired_pointer_targets`.
- Added synthetic tests cover an unseeded callback → unseeded leaf chain, conservative cyclic/invalid tail rejection, and rejection of a late pointer probe that would fall into an already published body. The private original-image test now asserts every adopted address/ISA entry survives and explicitly checks all four reported targets.

### Failed intermediate candidates were rejected, not hidden

The first broad tail-following attempt passed the then-current 81-test module but failed the new whole-image audit. It altered initial weak-start discovery order. Its complete lost-entry set and all corresponding direct-target violations are below. That candidate was not delivered for adoption.

| Attempt | Lost target | Violating direct source site(s), if any |
| --- | --- | --- |
| early tail following | `0x001ab680` | `0x001ab678` |
| early tail following | `0x001abf24` | `0x001ab670` |
| early tail following | `0x001bda14` | `0x001bda0c` |
| early tail following | `0x001bdb70` | `0x001bda04` |
| early tail following | `0x001ce684` | no required evidence row; stronger all-entry check caught it |
| late following without initial gap bound | `0x001ce684` | no required evidence row; stronger all-entry check caught it |

The second attempt moved recovery late but still allowed a pointer prefix at `0x1ce67c` to fall into the existing prologue-only start `0x1ce684`; it was likewise rejected. The final gap bound preserves that entry. The failed candidates, their analyzer sources, exit-1 audit outputs and full command records remain in V (`catalogue-v4-tail-follow-early.json`, `analyze-tail-follow-early.py`, `target-preservation-fixed.json`, `catalogue-v4-tail-follow-unbounded-gap.json`, `analyze-tail-follow-unbounded-gap.py`, `target-preservation-repaired.json`). The final stronger all-entry check is empty, not weakened to ignore this failure.

## 4. Counts versus v3 and the adopted v4

| Measure | v3 | Adopted ccb9168b v4 | Corrected v4 | Change from adopted v4 |
| --- | ---: | ---: | ---: | ---: |
| Top-level owners | 4,351 | 4,437 | 4,443 | +6 |
| Callable starts including explicit entries | 4,472 | 4,530 | 4,537 | +7 |
| Creditable ordinary functions | 4,231 | 4,345 | 4,350 | +5 |
| Creditable extent bytes | 1,108,426 | 1,111,866 | 1,111,934 | +68 |
| Multi-entry owners | 114 | 86 | 87 | +1 |
| Veneers | 6 | 6 | 6 | +0 |
| Unknown bytes | 8,836 | 8,836 | 8,704 | -132 |
| Unknown ranges | 194 | 194 | 189 | -5 |
| Semantic anomalies | 82 | 82 | 83 | +1 |
| Function-owned explained bytes | 1,135,300 | 1,135,312 | 1,135,444 | +132 |
| Instruction bytes | 1,054,474 | 1,054,486 | 1,054,598 | +112 |
| Decoded instructions | 264,698 | 264,701 | 264,729 | +28 |
| Pools/tables bytes | 39,968 | 39,968 | 39,988 | +20 |
| Literal bytes | 40,858 | 40,858 | 40,858 | +0 |
| Padding bytes | 172 | 160 | 160 | +0 |
| Sum of extents | 1,136,786 | 1,136,798 | 1,136,930 | +132 |
| Noreturn owners | 5 | 6 | 6 | +0 |

This is a denominator/catalogue correction, **not accepted C progress**. Relative to ccb9168b, six newly recovered records explain **132 previously unknown bytes**. One existing 64 B ordinary function is now honestly classified non-C `multi_entry` because a newly decoded tail B proves an external interior entry; the net creditable denominator increases by **five functions / 68 B**.

All original v4 NOP changes remain: 28 old NOP groups and 55 leading-NOP starts split into ordinary four-byte wrappers and bodies, and pointer-referenced NOP padding at `0x16a994`, `0x186eec`, `0x1900a8` remains callable. The pointer-only empty function at `0x1ef884` remains a four-byte ordinary function, with its rodata witness at `0x223960`. Architectural NOP hints, conditional MOVs, unreferenced padding and unrelated interior/trailing removed-call NOPs are not blanket split points. Other multi-entry evidence remains represented as non-C.

All **19,991** old direct call sites and **26,271** old direct B sites retain the same targets and ISA. There are **three newly decoded B sites**, all in the newly recovered functions; no old transfer or function-owned byte is lost. Non-text ranges and literal recovery remain unchanged. The extra semantic anomaly is **`literal_outside_function` at `0x19f6ac`** in a newly recovered 8 B leaf; it is recorded, not suppressed. The earlier two owner-address relocations of `unexplained_function_bytes` (`0x100078` → `0x10007c`, `0x1c3abc` → `0x1c3ac0`) remain unchanged.

### All seven record changes relative to the adopted ccb9168b catalogue

| Address | Change | Before | After | Details |
| --- | --- | --- | --- | --- |
| `0x00149428` | added | no top-level record | `function`, 28 B, 1 entries | new pointer/tail-evidenced ordinary record |
| `0x0014b2b0` | added | no top-level record | `function`, 56 B, 1 entries | new pointer/tail-evidenced ordinary record |
| `0x00156dcc` | added | no top-level record | `function`, 28 B, 1 entries | new pointer/tail-evidenced ordinary record |
| `0x0019f6ac` | added | no top-level record | `function`, 8 B, 1 entries | new pointer/tail-evidenced ordinary record |
| `0x001c0e44` | added | no top-level record | `function`, 4 B, 1 entries | new pointer/tail-evidenced ordinary record |
| `0x001cfdac` | added | no top-level record | `function`, 8 B, 1 entries | new pointer/tail-evidenced ordinary record |
| `0x001cff50` | modified | `function`, 64 B, 1 entries | `multi_entry`, 64 B, 2 entries | `entries`, `kind`; new ARM entry `0x001cff58` from `0x001cfdb0` |

### All new direct B evidence

| Source | Target | ISA |
| --- | --- | --- |
| `0x0014943c` | `0x0014b2b0` | arm |
| `0x00156ddc` | `0x0019f6ac` | arm |
| `0x001cfdb0` | `0x001cff58` | arm |

## 5. Verification and match preservation

- Final **`tests.test_analyze`: 82 tests, OK, exit 0**, 58.828 s wall time. Full argv/stdout/stderr: `V/tests-target-repair-final.json`. The original-image test builds twice and asserts serialized determinism, valid tiling and all adopted entry/ISA pairs.
- Whole-image conservation: **23,138 evidence rows / 4,375 distinct referenced targets**, zero violations; every one of the 4,472 v3 callable entries also survives. `V/target-preservation-complete.json` and `V/target-repair-metadata.json`.
- Structural validation: **`[]`**; all seven current metadata/conservation gates are true. No catalogue extent overlaps another.
- **83/83 original wrappers MATCH** under `armlink5-inline-branchnop-v1`, with **83 branchnop rewrites**, using the corrected d490e753 catalogue. **55/55 accepted ledger functions MATCH**, with **zero rewrites**, and their entire catalogue records remain identical to v3 and creditable. The accepted denominator numerator stays **55 functions / 2,680 B**.
- These are actual new in-place links of the previously compiled objects, not copied statuses. The repair checker verifies every source and object hash against its previous successful compilation record before comparing it against the new catalogue. New result files are in `V/target-repair-results/`, indexed and hashed by `V/target-repair-relinks.json`. No compilation or shared compiler slot was used in this follow-up.
- The older strict-v1 83-MISMATCH negative control remains in the previous report/artifacts and was exercised on ccb9168b; it is **not claimed as a rerun on this correction**. No new full-image acceptance build, denominator record or checkpoint was run.

### Exact current commands and output locations

Working directory: `/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v4`.

```sh
/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out /Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v4.json
/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python -m unittest tests.test_analyze -v
/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python scratch/verification/check_target_preservation.py --out scratch/verification/target-preservation-complete.json
/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python scratch/verification/summarize_target_repair.py
```

- Catalogue generation: **exit 0**, 41.459 s; `V/catalogue-target-repair-final-command.json`; output `scratch/analyze/catalogue-v4.json` in the main checkout.
- Final tests: **exit 0**, 58.828 s; `V/tests-target-repair-final.json`.
- Whole-image audit: **exit 0**, 2.023 s; `V/target-preservation-complete-command.json`; output `V/target-preservation-complete.json`.
- Metadata and 47 object relinks: **exit 0**, 5.203 s; `V/target-repair-summary-command.json`; outputs `V/target-repair-metadata.json`, `V/target-repair-changed-v3.json`, `V/target-repair-changed-ccb.json`, `V/target-repair-relinks.json`, and all per-unit files in `V/target-repair-results/`.
- Callback pointer inspection: **exit 0**; complete Python argv and stdout in `V/target-pointer-witness-command.json`; extracted metadata `V/target-pointer-witness.json`.

## 6. Complete changed-record inventory versus v3

**85 modified + 92 added + 0 removed = 177 changed top-level records.** The other 4,266 v3 records are identical. Every changed address and field list is below; the exact full before/after JSON for every row is in `V/target-repair-changed-v3.json`. A separate full seven-record delta against the previously adopted v4 is in `V/target-repair-changed-ccb.json`. Both files are hashed below.

| Address | Status | v3 | Corrected v4 | Changed fields |
| --- | --- | --- | --- | --- |
| `0x00100000` | modified | `function`, 32 B, 1 entries | `function`, 32 B, 1 entries | `noreturn_reasons` |
| `0x00100078` | modified | `function`, 16 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `literals`, `size` |
| `0x0010007c` | added | no top-level record | `function`, 12 B, 1 entries | all fields (new record) |
| `0x00100d68` | modified | `function`, 8 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `noreturn_reasons`, `size` |
| `0x00100d6c` | added | no top-level record | `function`, 4 B, 1 entries | all fields (new record) |
| `0x0010c338` | modified | `function`, 68 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `size` |
| `0x0010c33c` | added | no top-level record | `function`, 64 B, 1 entries | all fields (new record) |
| `0x0010c37c` | modified | `function`, 200 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0010c380` | added | no top-level record | `function`, 196 B, 1 entries | all fields (new record) |
| `0x0010e67c` | modified | `function`, 412 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0010e680` | added | no top-level record | `function`, 408 B, 1 entries | all fields (new record) |
| `0x00110aa4` | modified | `function`, 548 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00110aa8` | added | no top-level record | `function`, 544 B, 1 entries | all fields (new record) |
| `0x00114fa4` | modified | `function`, 44 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00114fa8` | added | no top-level record | `function`, 40 B, 1 entries | all fields (new record) |
| `0x0011ef48` | modified | `function`, 12 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `size` |
| `0x0011ef4c` | added | no top-level record | `function`, 8 B, 1 entries | all fields (new record) |
| `0x0011ef54` | modified | `function`, 12 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `size` |
| `0x0011ef58` | added | no top-level record | `function`, 8 B, 1 entries | all fields (new record) |
| `0x00121764` | modified | `function`, 80 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00121768` | added | no top-level record | `function`, 76 B, 1 entries | all fields (new record) |
| `0x001217b4` | modified | `function`, 132 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001217b8` | added | no top-level record | `function`, 128 B, 1 entries | all fields (new record) |
| `0x00121bc4` | modified | `function`, 120 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00121bc8` | added | no top-level record | `function`, 116 B, 1 entries | all fields (new record) |
| `0x00121da0` | modified | `function`, 60 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00121da4` | added | no top-level record | `function`, 56 B, 1 entries | all fields (new record) |
| `0x00121ddc` | modified | `function`, 60 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00121de0` | added | no top-level record | `function`, 56 B, 1 entries | all fields (new record) |
| `0x001224a0` | modified | `function`, 92 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001224a4` | added | no top-level record | `function`, 88 B, 1 entries | all fields (new record) |
| `0x001224fc` | modified | `function`, 16 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00122500` | added | no top-level record | `function`, 12 B, 1 entries | all fields (new record) |
| `0x0012250c` | modified | `function`, 16 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00122510` | added | no top-level record | `function`, 12 B, 1 entries | all fields (new record) |
| `0x00122788` | modified | `function`, 264 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0012278c` | added | no top-level record | `function`, 260 B, 1 entries | all fields (new record) |
| `0x00122890` | modified | `function`, 44 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `size` |
| `0x00122894` | added | no top-level record | `function`, 40 B, 1 entries | all fields (new record) |
| `0x001228bc` | modified | `function`, 20 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001228c0` | added | no top-level record | `function`, 16 B, 1 entries | all fields (new record) |
| `0x001228d0` | modified | `function`, 32 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `size` |
| `0x001228d4` | added | no top-level record | `function`, 28 B, 1 entries | all fields (new record) |
| `0x00122984` | modified | `function`, 52 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `size` |
| `0x00122988` | added | no top-level record | `function`, 48 B, 1 entries | all fields (new record) |
| `0x001229b8` | modified | `function`, 24 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001229bc` | added | no top-level record | `function`, 20 B, 1 entries | all fields (new record) |
| `0x001229d0` | modified | `function`, 76 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001229d4` | added | no top-level record | `function`, 72 B, 1 entries | all fields (new record) |
| `0x00122a1c` | modified | `function`, 20 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00122a20` | added | no top-level record | `function`, 16 B, 1 entries | all fields (new record) |
| `0x00122a68` | modified | `function`, 104 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00122a6c` | added | no top-level record | `function`, 100 B, 1 entries | all fields (new record) |
| `0x00125b50` | modified | `function`, 8 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `size` |
| `0x00125b54` | added | no top-level record | `function`, 4 B, 1 entries | all fields (new record) |
| `0x0012d5c0` | modified | `multi_entry`, 108 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x0012d5c4` | added | entry of `0x0012d5c0`; no own record | `function`, 104 B, 1 entries | all fields (new record) |
| `0x0012d62c` | modified | `function`, 84 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0012d630` | added | no top-level record | `function`, 80 B, 1 entries | all fields (new record) |
| `0x0012f518` | modified | `function`, 88 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0012f51c` | added | no top-level record | `function`, 84 B, 1 entries | all fields (new record) |
| `0x0012fb38` | modified | `function`, 16 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0012fb3c` | added | no top-level record | `function`, 12 B, 1 entries | all fields (new record) |
| `0x0012fb48` | modified | `function`, 16 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0012fb4c` | added | no top-level record | `function`, 12 B, 1 entries | all fields (new record) |
| `0x001363b0` | modified | `function`, 84 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001363b4` | added | no top-level record | `function`, 80 B, 1 entries | all fields (new record) |
| `0x00136404` | modified | `function`, 68 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00136408` | added | no top-level record | `function`, 64 B, 1 entries | all fields (new record) |
| `0x00136858` | modified | `multi_entry`, 32 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x0013685c` | added | entry of `0x00136858`; no own record | `function`, 28 B, 1 entries | all fields (new record) |
| `0x0013706c` | modified | `function`, 64 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00137070` | added | no top-level record | `function`, 60 B, 1 entries | all fields (new record) |
| `0x001370ac` | modified | `function`, 108 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001370b0` | added | no top-level record | `function`, 104 B, 1 entries | all fields (new record) |
| `0x0013714c` | modified | `function`, 8 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `size` |
| `0x00137150` | added | no top-level record | `function`, 4 B, 1 entries | all fields (new record) |
| `0x0013776c` | modified | `function`, 52 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00137770` | added | no top-level record | `function`, 48 B, 1 entries | all fields (new record) |
| `0x001377a0` | modified | `function`, 208 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001377a4` | added | no top-level record | `function`, 204 B, 1 entries | all fields (new record) |
| `0x001378d4` | modified | `multi_entry`, 176 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x001378d8` | added | entry of `0x001378d4`; no own record | `function`, 172 B, 1 entries | all fields (new record) |
| `0x0013e7d0` | modified | `function`, 64 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0013e7d4` | added | no top-level record | `function`, 60 B, 1 entries | all fields (new record) |
| `0x00141c70` | modified | `multi_entry`, 104 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x00141c74` | added | entry of `0x00141c70`; no own record | `function`, 100 B, 1 entries | all fields (new record) |
| `0x00142640` | modified | `function`, 68 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00142644` | added | no top-level record | `function`, 64 B, 1 entries | all fields (new record) |
| `0x00144220` | modified | `multi_entry`, 8 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x00144224` | added | entry of `0x00144220`; no own record | `function`, 4 B, 1 entries | all fields (new record) |
| `0x00147994` | modified | `function`, 116 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00147998` | added | no top-level record | `function`, 112 B, 1 entries | all fields (new record) |
| `0x00149428` | added | no top-level record | `function`, 28 B, 1 entries | all fields (new record) |
| `0x0014a988` | modified | `function`, 80 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0014a98c` | added | no top-level record | `function`, 76 B, 1 entries | all fields (new record) |
| `0x0014a9d8` | modified | `function`, 188 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0014a9dc` | added | no top-level record | `function`, 184 B, 1 entries | all fields (new record) |
| `0x0014b2b0` | added | no top-level record | `function`, 56 B, 1 entries | all fields (new record) |
| `0x0014c338` | modified | `function`, 40 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0014c33c` | added | no top-level record | `function`, 36 B, 1 entries | all fields (new record) |
| `0x00156dcc` | added | no top-level record | `function`, 28 B, 1 entries | all fields (new record) |
| `0x00156de8` | modified | `function`, 284 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00156dec` | added | no top-level record | `function`, 280 B, 1 entries | all fields (new record) |
| `0x0015e2b4` | modified | `function`, 104 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0015e2b8` | added | no top-level record | `function`, 100 B, 1 entries | all fields (new record) |
| `0x0015e31c` | modified | `function`, 152 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0015e320` | added | no top-level record | `function`, 148 B, 1 entries | all fields (new record) |
| `0x00166eac` | modified | `multi_entry`, 112 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x00166eb0` | added | entry of `0x00166eac`; no own record | `function`, 108 B, 1 entries | all fields (new record) |
| `0x00166f1c` | modified | `multi_entry`, 228 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x00166f20` | added | entry of `0x00166f1c`; no own record | `function`, 224 B, 1 entries | all fields (new record) |
| `0x00169874` | modified | `multi_entry`, 128 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x00169878` | added | entry of `0x00169874`; no own record | `function`, 124 B, 1 entries | all fields (new record) |
| `0x0016a994` | added | no top-level record | `function`, 4 B, 1 entries | all fields (new record) |
| `0x0016f39c` | modified | `multi_entry`, 336 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x0016f3a0` | added | entry of `0x0016f39c`; no own record | `function`, 332 B, 1 entries | all fields (new record) |
| `0x0017e358` | modified | `multi_entry`, 128 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x0017e35c` | added | entry of `0x0017e358`; no own record | `function`, 124 B, 1 entries | all fields (new record) |
| `0x00181e40` | modified | `multi_entry`, 68 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x00181e44` | added | entry of `0x00181e40`; no own record | `function`, 64 B, 1 entries | all fields (new record) |
| `0x00182718` | modified | `multi_entry`, 20 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x0018271c` | added | entry of `0x00182718`; no own record | `function`, 16 B, 1 entries | all fields (new record) |
| `0x00183030` | modified | `multi_entry`, 64 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x00183034` | added | entry of `0x00183030`; no own record | `function`, 60 B, 1 entries | all fields (new record) |
| `0x001852f4` | modified | `multi_entry`, 68 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x001852f8` | added | entry of `0x001852f4`; no own record | `function`, 64 B, 1 entries | all fields (new record) |
| `0x00185b9c` | modified | `multi_entry`, 20 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x00185ba0` | added | entry of `0x00185b9c`; no own record | `function`, 16 B, 1 entries | all fields (new record) |
| `0x00186484` | modified | `multi_entry`, 16 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x00186488` | added | entry of `0x00186484`; no own record | `function`, 12 B, 1 entries | all fields (new record) |
| `0x00186eec` | added | no top-level record | `function`, 4 B, 1 entries | all fields (new record) |
| `0x0018f7ec` | modified | `multi_entry`, 8 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x0018f7f0` | added | entry of `0x0018f7ec`; no own record | `function`, 4 B, 1 entries | all fields (new record) |
| `0x001900a8` | added | no top-level record | `function`, 4 B, 1 entries | all fields (new record) |
| `0x0019721c` | modified | `multi_entry`, 460 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x00197220` | added | entry of `0x0019721c`; no own record | `function`, 456 B, 1 entries | all fields (new record) |
| `0x0019dc1c` | modified | `multi_entry`, 228 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x0019dc20` | added | entry of `0x0019dc1c`; no own record | `function`, 224 B, 1 entries | all fields (new record) |
| `0x0019eb48` | modified | `multi_entry`, 24 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x0019eb4c` | added | entry of `0x0019eb48`; no own record | `function`, 20 B, 1 entries | all fields (new record) |
| `0x0019f2e4` | modified | `multi_entry`, 64 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x0019f2e8` | added | entry of `0x0019f2e4`; no own record | `function`, 60 B, 1 entries | all fields (new record) |
| `0x0019f6ac` | added | no top-level record | `function`, 8 B, 1 entries | all fields (new record) |
| `0x0019ffa4` | modified | `multi_entry`, 104 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x0019ffa8` | added | entry of `0x0019ffa4`; no own record | `function`, 100 B, 1 entries | all fields (new record) |
| `0x001b472c` | modified | `function`, 324 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001b4730` | added | no top-level record | `function`, 320 B, 1 entries | all fields (new record) |
| `0x001b54e8` | modified | `function`, 272 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001b54ec` | added | no top-level record | `function`, 268 B, 1 entries | all fields (new record) |
| `0x001c0e44` | added | no top-level record | `function`, 4 B, 1 entries | all fields (new record) |
| `0x001c3abc` | modified | `multi_entry`, 404 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `literals`, `pools`, `size` |
| `0x001c3ac0` | added | entry of `0x001c3abc`; no own record | `function`, 400 B, 1 entries | all fields (new record) |
| `0x001c5b84` | modified | `function`, 8 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `size` |
| `0x001c5b88` | added | no top-level record | `function`, 4 B, 1 entries | all fields (new record) |
| `0x001c6a34` | modified | `function`, 52 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001c6a38` | added | no top-level record | `function`, 48 B, 1 entries | all fields (new record) |
| `0x001c6ad8` | modified | `function`, 8 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `size` |
| `0x001c6adc` | added | no top-level record | `function`, 4 B, 1 entries | all fields (new record) |
| `0x001cfdac` | added | no top-level record | `function`, 8 B, 1 entries | all fields (new record) |
| `0x001cff50` | modified | `function`, 64 B, 1 entries | `multi_entry`, 64 B, 2 entries | `entries`, `kind` |
| `0x001d32e0` | modified | `function`, 144 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `size` |
| `0x001d32e4` | added | no top-level record | `function`, 140 B, 1 entries | all fields (new record) |
| `0x001d54ac` | modified | `multi_entry`, 124 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x001d54b0` | added | entry of `0x001d54ac`; no own record | `function`, 120 B, 1 entries | all fields (new record) |
| `0x001dacbc` | modified | `multi_entry`, 44 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x001dacc0` | added | entry of `0x001dacbc`; no own record | `function`, 40 B, 1 entries | all fields (new record) |
| `0x001dadc0` | modified | `multi_entry`, 44 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x001dadc4` | added | entry of `0x001dadc0`; no own record | `function`, 40 B, 1 entries | all fields (new record) |
| `0x001db130` | modified | `multi_entry`, 8 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x001db134` | added | entry of `0x001db130`; no own record | `function`, 4 B, 1 entries | all fields (new record) |
| `0x001e9818` | modified | `multi_entry`, 300 B, 2 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x001e981c` | added | entry of `0x001e9818`; no own record | `function`, 296 B, 1 entries | all fields (new record) |
| `0x001eafa0` | modified | `function`, 304 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001eafa4` | added | no top-level record | `function`, 300 B, 1 entries | all fields (new record) |
| `0x001ef880` | modified | `function`, 8 B, 1 entries | `function`, 4 B, 1 entries | `code_ranges`, `code_size`, `size` |
| `0x001ef884` | added | no top-level record | `function`, 4 B, 1 entries | all fields (new record) |

## 7. Accepted-function inventory

| Function | Extent B | Entire catalogue record | Corrected-catalogue link |
| --- | ---: | --- | --- |
| `func_0010e664` | 12 | identical / ordinary / creditable | MATCH |
| `func_001149a8` | 24 | identical / ordinary / creditable | MATCH |
| `func_00136930` | 40 | identical / ordinary / creditable | MATCH |
| `func_00140dac` | 40 | identical / ordinary / creditable | MATCH |
| `func_001179b4` | 16 | identical / ordinary / creditable | MATCH |
| `func_001263b8` | 20 | identical / ordinary / creditable | MATCH |
| `func_00122350` | 16 | identical / ordinary / creditable | MATCH |
| `func_00103798` | 40 | identical / ordinary / creditable | MATCH |
| `func_001091d4` | 56 | identical / ordinary / creditable | MATCH |
| `func_001103d4` | 16 | identical / ordinary / creditable | MATCH |
| `func_00103258` | 136 | identical / ordinary / creditable | MATCH |
| `func_00103358` | 116 | identical / ordinary / creditable | MATCH |
| `func_00105084` | 64 | identical / ordinary / creditable | MATCH |
| `func_00105fa8` | 100 | identical / ordinary / creditable | MATCH |
| `func_0010d604` | 64 | identical / ordinary / creditable | MATCH |
| `func_001718ec` | 168 | identical / ordinary / creditable | MATCH |
| `func_00171c7c` | 168 | identical / ordinary / creditable | MATCH |
| `func_001670e4` | 8 | identical / ordinary / creditable | MATCH |
| `func_0011e928` | 72 | identical / ordinary / creditable | MATCH |
| `func_0018fdd4` | 40 | identical / ordinary / creditable | MATCH |
| `func_00195254` | 224 | identical / ordinary / creditable | MATCH |
| `func_00102cb0` | 32 | identical / ordinary / creditable | MATCH |
| `func_00105058` | 44 | identical / ordinary / creditable | MATCH |
| `func_00110358` | 4 | identical / ordinary / creditable | MATCH |
| `func_001107d0` | 4 | identical / ordinary / creditable | MATCH |
| `func_0011362c` | 4 | identical / ordinary / creditable | MATCH |
| `func_0011e970` | 4 | identical / ordinary / creditable | MATCH |
| `func_0011e974` | 4 | identical / ordinary / creditable | MATCH |
| `func_0011e978` | 4 | identical / ordinary / creditable | MATCH |
| `func_0011e998` | 4 | identical / ordinary / creditable | MATCH |
| `func_0019e02c` | 96 | identical / ordinary / creditable | MATCH |
| `func_0012ced8` | 4 | identical / ordinary / creditable | MATCH |
| `func_0012d1c8` | 4 | identical / ordinary / creditable | MATCH |
| `func_0012d4f0` | 4 | identical / ordinary / creditable | MATCH |
| `func_0012f4f0` | 28 | identical / ordinary / creditable | MATCH |
| `func_0012f50c` | 12 | identical / ordinary / creditable | MATCH |
| `func_0018fcb4` | 12 | identical / ordinary / creditable | MATCH |
| `func_00195e84` | 16 | identical / ordinary / creditable | MATCH |
| `func_00195e94` | 16 | identical / ordinary / creditable | MATCH |
| `func_0011f080` | 132 | identical / ordinary / creditable | MATCH |
| `func_00134f04` | 20 | identical / ordinary / creditable | MATCH |
| `func_00135a28` | 48 | identical / ordinary / creditable | MATCH |
| `func_00135a58` | 48 | identical / ordinary / creditable | MATCH |
| `func_00135a88` | 48 | identical / ordinary / creditable | MATCH |
| `func_00135ab8` | 48 | identical / ordinary / creditable | MATCH |
| `func_001360a4` | 56 | identical / ordinary / creditable | MATCH |
| `func_00196cb8` | 96 | identical / ordinary / creditable | MATCH |
| `func_0019f450` | 48 | identical / ordinary / creditable | MATCH |
| `func_0018fcc0` | 96 | identical / ordinary / creditable | MATCH |
| `func_0018fd20` | 72 | identical / ordinary / creditable | MATCH |
| `func_0019d5c4` | 24 | identical / ordinary / creditable | MATCH |
| `func_001c44ec` | 20 | identical / ordinary / creditable | MATCH |
| `func_001c4500` | 56 | identical / ordinary / creditable | MATCH |
| `func_001d2da4` | 52 | identical / ordinary / creditable | MATCH |
| `func_001d3524` | 80 | identical / ordinary / creditable | MATCH |

## 8. Evidence hashes

Paths relative to V. Previous source/candidate/record evidence is retained separately; no failed result was overwritten to claim success.

| Artifact | SHA-256 |
| --- | --- |
| `check_target_preservation.py` | `f25ae930b68725958a84c7c398cdf60d5c0b448b20a44c60795ffe43d6318d83` |
| `summarize_target_repair.py` | `562e6b052f3fb5ef9dc990bf8032f8e640e8f52e9ff5d676b3e084b4197d9f2b` |
| `target-preservation-ccb9168b.json` | `b86954d51684fbe6b70022fabf80e9ff16215e725d6eabbb3713d3e428779c42` |
| `target-preservation-ccb9168b-command.json` | `ae1c0ca1ed2a8e6b9fe8b79f86bc800b0496bc4c8abf30143e4d4a2eb423c453` |
| `catalogue-v4-ccb9168b.json` | `ccb9168b75df6825bf1ee7b1ff7e86feb8f79c8b9a236b444a30bd8256f0f571` |
| `REPORT-v4-ccb9168b.md` | `4a87f0f37e5a52ab6d933692308f994ba339895398f54b4ac172276597130664` |
| `catalogue-v4-tail-follow-early.json` | `222f38965b99ce182c0344c6c22ef1ec123b2ebee2c0c20b3d7c794f292eff7f` |
| `analyze-tail-follow-early.py` | `fba624954f676cbb2f3ee05cc45faa4041edb3a46f8de311002e2f62856fedfe` |
| `target-preservation-fixed.json` | `8eb2ee0277f69f14eb4477c11c3cd3884a7e7a6fbafe788e3e7d8f8cb6203609` |
| `target-preservation-fixed-command.json` | `9b0e351057b416afdb0f761bcef5581fd667f62edc7245618a879c1659b46a71` |
| `catalogue-v4-tail-follow-unbounded-gap.json` | `210261ddc4bcc002b3436baaf7c9d209515906f52d46d923e015552f1e9f054b` |
| `analyze-tail-follow-unbounded-gap.py` | `daa6e6c1ae5ffc048bfe80c8aeef35a17c54dffa323dabae8b8756a4085582bf` |
| `target-preservation-repaired.json` | `f1ce3b1ce5721877d382a4c4d7d5a5f2aaf89f08040d47dd4ece1be84d663324` |
| `target-preservation-repaired-command.json` | `a64323ef39b1761ede6c2eba3fa974195775ac889a457d38777487020efba84f` |
| `tests-target-repair-final.json` | `c6551af213fa0b0a94520113288b7fda6bcbf0ec6dc28bc811676d58b81c2ea4` |
| `catalogue-target-repair-final-command.json` | `b4e515875cd9515d48f9502a1397e76bbb5ac1bfa1491881dce51b46dd6219b1` |
| `target-preservation-complete.json` | `144de95ac58130d71a48c00a7172a6152178a9c8772897e51449f8abe0c5f2d7` |
| `target-preservation-complete-command.json` | `2a1309921ab8107ab526e6e23934289122580e43872661def534c8b989102b6d` |
| `target-repair-metadata.json` | `491210571298159bb212879a130ecf2237a331a629c816314281ddf951f39df9` |
| `target-repair-summary-command.json` | `77f984909853267250cc22b3ab5bf0e78b0cbd66e768cdad14c79ebbc4eb230f` |
| `target-repair-changed-v3.json` | `45e1d9eb26fb8bac69cf8b5c0c61b2a903baadda1da46cda069943cef5257048` |
| `target-repair-changed-ccb.json` | `2882705c868639b9d12b4e2e2f0338902f7b6d4775dedef89ed8586219587e84` |
| `target-repair-relinks.json` | `c4b1a0220898a4dfe70d5aee8f822aa95a3a2bd0c7c1dc603e3bc30014ca3526` |
| `target-pointer-witness.json` | `d425283a3669bf03f9f8acfa07c92fc09a7fa1ef44de973a4acdf3ec5d77a37f` |
| `target-pointer-witness-command.json` | `67c461abba0baa1cfaf530d5243df0cc96157c1d03cb2091d0bca3d200394942` |

### Current match-result index

| Unit / wrapper group | Functions | Status | Rewrites | Result path relative to V | SHA-256 |
| --- | ---: | --- | ---: | --- | --- |
| `leading_nop` | 55 | MATCH | 55 | `target-repair-results/leading_nop.json` | `fc765c68d6e9332802b8804ce6535e947db0e09eb5eb0c00e15e4bf08cfb526c` |
| `nop_entry` | 28 | MATCH | 28 | `target-repair-results/nop_entry.json` | `326d69f6ecac02ef13b2430e29146f42237969e9233c3278f7f565fd2956dd8e` |
| `accepted-00-pilot_a` | 7 | MATCH | 0 | `target-repair-results/accepted-00-pilot_a.json` | `af9e221de7f700277b5afc85dec1df6267c67a2918ad53067b95292f23c97a35` |
| `accepted-01-pilot_b` | 3 | MATCH | 0 | `target-repair-results/accepted-01-pilot_b.json` | `10545d8f5a01fcb2e6f40fe8cd1823f64308a1fcc1828ac99bb40351ffe62ffd` |
| `accepted-02-loop_cleanup` | 2 | MATCH | 0 | `target-repair-results/accepted-02-loop_cleanup.json` | `80d6ed59f2b7196630eea2d77f970e59d10c438ca882e58aa7360705b69a97aa` |
| `accepted-03-loop_resources` | 1 | MATCH | 0 | `target-repair-results/accepted-03-loop_resources.json` | `cc996f3b7aee05b4a5f5823f88116df41eb63ef3acc08a539b350d49131d6dc3` |
| `accepted-04-loop_state` | 1 | MATCH | 0 | `target-repair-results/accepted-04-loop_state.json` | `adde94501f2fc25b99b1c0c9b6463f59cc4022e58007b3b8dc8826d877bc2823` |
| `accepted-05-game_heap` | 1 | MATCH | 0 | `target-repair-results/accepted-05-game_heap.json` | `7208288310c6ade1e1f0bf3470445370c36358bf936237a2d535c20af9a17c9f` |
| `accepted-06-game_enum_switch` | 2 | MATCH | 0 | `target-repair-results/accepted-06-game_enum_switch.json` | `3ae599c98fee7cd04a91f4370c8979b2bf270ce87a81b943533de1871a4f24b9` |
| `accepted-07-func_001670e4` | 1 | MATCH | 0 | `target-repair-results/accepted-07-func_001670e4.json` | `0f76775e6085653cf9f524f5f4d5d5562c62bc1675d4ae6e728e2f975908f6e9` |
| `accepted-08-func_0011e928` | 1 | MATCH | 0 | `target-repair-results/accepted-08-func_0011e928.json` | `d04092c092fdb7a4aae2ddd467b0ead7a9d3a57fce1c9c7a3f037a7635aa06ff` |
| `accepted-09-func_0018fdd4` | 1 | MATCH | 0 | `target-repair-results/accepted-09-func_0018fdd4.json` | `ba83b40e8b40faec243241c7b33db7cae40e25e47911daaeed163b904ebef1e2` |
| `accepted-10-func_00195254` | 1 | MATCH | 0 | `target-repair-results/accepted-10-func_00195254.json` | `7e5f1e6083919560f9d0aa3081c51f41f490593b09bc59ec49efc05e56dfd8c0` |
| `accepted-11-func_00102cb0` | 1 | MATCH | 0 | `target-repair-results/accepted-11-func_00102cb0.json` | `71c791d2c4058676395545766394c17f520bd2c74af7592c276fd77c244f7784` |
| `accepted-12-func_00105058` | 1 | MATCH | 0 | `target-repair-results/accepted-12-func_00105058.json` | `cb2dccf8a3f1b7130eb90ead2ac03eb134312ba3ecd31c03c4f69083251836c1` |
| `accepted-13-func_00110358` | 1 | MATCH | 0 | `target-repair-results/accepted-13-func_00110358.json` | `642d86531f97b9bd11f594a3497fb3614be745077b3619953997756252c27bc4` |
| `accepted-14-func_001107d0` | 1 | MATCH | 0 | `target-repair-results/accepted-14-func_001107d0.json` | `2d4fb68ad9044f2ebbfb1c0d6af9a7386d69e2a9f9ab972a69ae4019300150cb` |
| `accepted-15-func_0011362c` | 1 | MATCH | 0 | `target-repair-results/accepted-15-func_0011362c.json` | `b86cfafff7c0829fe26d0ee570cc0219bf3c5cfb5ef59f4427358431e0e210f1` |
| `accepted-16-func_0011e970` | 1 | MATCH | 0 | `target-repair-results/accepted-16-func_0011e970.json` | `e9a857a94844142a6b73527b8f314c0b75f1f72237611bb7a2584c50bb9d5dff` |
| `accepted-17-func_0011e974` | 1 | MATCH | 0 | `target-repair-results/accepted-17-func_0011e974.json` | `070d8ef0933509128c32d43d4dc91047aef842710820bca2f73da410d6651646` |
| `accepted-18-func_0011e978` | 1 | MATCH | 0 | `target-repair-results/accepted-18-func_0011e978.json` | `c5f2f3d03cb1340b0d406807a287e855978e778f5c52df61490f7d94f8562206` |
| `accepted-19-func_0011e998` | 1 | MATCH | 0 | `target-repair-results/accepted-19-func_0011e998.json` | `94ac11287fbaae27aa6808e969a98040c4fae987f54a77cfc32152792d6ad241` |
| `accepted-20-func_0019e02c_returning` | 1 | MATCH | 0 | `target-repair-results/accepted-20-func_0019e02c_returning.json` | `e051a8b1d5685336236a8e87333696fa9af1cf2f19cb6f37afcd7b392e14e548` |
| `accepted-21-func_0012ced8` | 1 | MATCH | 0 | `target-repair-results/accepted-21-func_0012ced8.json` | `ed8d4482884bb0d789e2544bdbc86c13cb05cb65cbb62c49f29c71fb87067b6c` |
| `accepted-22-func_0012d1c8` | 1 | MATCH | 0 | `target-repair-results/accepted-22-func_0012d1c8.json` | `dadc503fbae88aba61dc59c49107d8eebde0285baf7dde7edbeb7cb27f2b3310` |
| `accepted-23-func_0012d4f0` | 1 | MATCH | 0 | `target-repair-results/accepted-23-func_0012d4f0.json` | `1bc678da4bc7e27e801512103da619a93aa518aa5183777b6a38d48db0ef8d8e` |
| `accepted-24-func_0012f4f0` | 1 | MATCH | 0 | `target-repair-results/accepted-24-func_0012f4f0.json` | `02a56f47af0cce3cb24d758731410ea6c414c6fe05ce5149285e67dfee157cf7` |
| `accepted-25-func_0012f50c` | 1 | MATCH | 0 | `target-repair-results/accepted-25-func_0012f50c.json` | `1ca0aa840c195aade20b3966cc3783e0840af4547085c57efa7edc7d3c0bbaa9` |
| `accepted-26-func_0018fcb4` | 1 | MATCH | 0 | `target-repair-results/accepted-26-func_0018fcb4.json` | `bc5973423a4d5b315b9041a02344ec46d91105625da9d2aece77f492c3b1cdf0` |
| `accepted-27-func_00195e84` | 1 | MATCH | 0 | `target-repair-results/accepted-27-func_00195e84.json` | `5186efae8d5c06f343626551f80f0ce7ca7c0c2b354145ead6973447c8259ce4` |
| `accepted-28-func_00195e94` | 1 | MATCH | 0 | `target-repair-results/accepted-28-func_00195e94.json` | `02f4f33448d120da9b19b7371546a1fcf7aee5b846a58e72d18075af795af571` |
| `accepted-29-func_0011f080` | 1 | MATCH | 0 | `target-repair-results/accepted-29-func_0011f080.json` | `46f12a6e2dce5afd7a37b44870fb5ffc8e73b59391175ebde024ee3db6d567e6` |
| `accepted-30-func_00134f04` | 1 | MATCH | 0 | `target-repair-results/accepted-30-func_00134f04.json` | `fa1025510cb28fe329afff8efe34693ed10e645a9dfd62cc09a8296e8d4fd8fe` |
| `accepted-31-func_00135a28` | 1 | MATCH | 0 | `target-repair-results/accepted-31-func_00135a28.json` | `4e67855e9430683d1e292861a3a921d030c9dadd0624fdbaf75b1c7a3d0c261a` |
| `accepted-32-func_00135a58` | 1 | MATCH | 0 | `target-repair-results/accepted-32-func_00135a58.json` | `78d5af7b8eb7f43b7eaa66e67dc075414181ca39bd6bc8d757438ea3b1dca4d4` |
| `accepted-33-func_00135a88` | 1 | MATCH | 0 | `target-repair-results/accepted-33-func_00135a88.json` | `acfda5582022547627975ccba6e37b94025ce5280d298630eb095b5f727b3e1a` |
| `accepted-34-func_00135ab8` | 1 | MATCH | 0 | `target-repair-results/accepted-34-func_00135ab8.json` | `8ba5ae806d80ff7e3197a6667d71c43cfd44a4d67a1744f9698a9d24cc0270d4` |
| `accepted-35-func_001360a4` | 1 | MATCH | 0 | `target-repair-results/accepted-35-func_001360a4.json` | `9e44dd3357bf48babc53bb242509598d47bb25328d481fb02ce183ab83645c70` |
| `accepted-36-func_00196cb8` | 1 | MATCH | 0 | `target-repair-results/accepted-36-func_00196cb8.json` | `116a12e7ae4feb976c94f0ac6d750216427548ba95296087b813303648b8826d` |
| `accepted-37-func_0019f450` | 1 | MATCH | 0 | `target-repair-results/accepted-37-func_0019f450.json` | `e2e9f8010b6d912e4d8007aae43209fb85532160361271cc39d59a3538d47d31` |
| `accepted-38-func_0018fcc0` | 1 | MATCH | 0 | `target-repair-results/accepted-38-func_0018fcc0.json` | `1b4398a33c62e65a654c99e3e90043195551337963c31f21e75708072a064610` |
| `accepted-39-func_0018fd20` | 1 | MATCH | 0 | `target-repair-results/accepted-39-func_0018fd20.json` | `de94c8727dcc003c94da371206e1078251ae395b25d6797991ecaa6d55caf2c2` |
| `accepted-40-func_0019d5c4` | 1 | MATCH | 0 | `target-repair-results/accepted-40-func_0019d5c4.json` | `0225d08c543ad02f8bc646170a6423fc73a0e43487e98bab362026ba5dfb200b` |
| `accepted-41-func_001c44ec` | 1 | MATCH | 0 | `target-repair-results/accepted-41-func_001c44ec.json` | `8c0ad2032fef80132ecfc5d1f5d1ae0c05f3d0060edf0f4602b5113914d1666d` |
| `accepted-42-func_001c4500` | 1 | MATCH | 0 | `target-repair-results/accepted-42-func_001c4500.json` | `9334505f3aba240371fa258c887665a62f7ec67bd20fff8578f9fe312d7644ac` |
| `accepted-43-func_001d2da4` | 1 | MATCH | 0 | `target-repair-results/accepted-43-func_001d2da4.json` | `7a2e193ef1f10979f53ce0970bc7d63a200ebdb8ecde62e2c6fbe02b493911c0` |
| `accepted-44-func_001d3524` | 1 | MATCH | 0 | `target-repair-results/accepted-44-func_001d3524.json` | `8a575a5d7e34b740484fa6776e6756a8c26b181e3f374652da422e2a6d1e991b` |

## Remaining qualifications / hand-off

- The catalogue-specific follow-up is complete: all four reported addresses are present, the pointer-only tail-chain gap is fixed generically, both synthetic and original-image regressions pass, and the requested whole-image violation list is empty.
- The first three packet/linker consumer refusals were not re-executed and cannot be attributed to an entry removed from these JSON catalogues. Interior packet naming/version routing remains an integrator-side consumer matter, not an unimplemented catalogue fix.
- **83 semantic anomalies / 8,704 unknown bytes** remain honestly recorded. The one newly visible literal-outside-function issue is explicitly listed above; no unrelated layout claim was manufactured.
- This is a corrected denominator candidate, not new accepted C credit. Re-adoption, consumer integration and any full-image acceptance build remain integrator actions. This worker made no follow-up git mutation, commit, deletion or publication.
