# Candidate catalogue v9-r2 — two independent-review corrections

**Proposal only: no adoption, denominator/history update, accepted C credit, gate change, build or checkpoint operation.** This discrete revision is based on `catalogue-v9` commit `9b1808944d2fed376aa04f523d15c13bb8bd4dbc`, in the separate `scratch/worktrees/catalogue-v10` worktree. Subsequent v10 closure work is not part of this revision. The original v9 proposal, evidence and report remain unchanged.

## Review disposition

The sealed independent review is `scratch/review/catalogue-v9/REVIEW.md`, FAIL. Its two findings are addressed without broadening the accepted classification policy:

- **V9-R01 / P2 — shared wide-object bounds.** Explicit encoding/reference witnesses cover `0x001b54e4`, `0x001b8234` and `0x001bc29c`. All are complete four-byte UTF-16 records, including ideographic whitespace, not one-byte empty narrow strings. The UTF-16 parser accepts Unicode whitespace only with an explicit reviewed encoding witness; non-whitespace controls and missing terminators still fail. All three remain **shared unowned rodata**. No function extent or ownership grows.
- **V9-R02 / P1 — post-adoption legacy-label stability.** A previously preserved `legacy_prologue` secondary label is retained when it belongs to the same initializer owner, has no incoming sources, remains an aligned ARM instruction boundary and retains all prior owned code/pool/literal spans. Original single-prologue recovery retains its existing guards. Wrong owners, stronger entry evidence, incoming callers, wrong entry kinds and lost spans are refused; there is no blanket exemption for missing entries.

The exact r4023 witness is `scratch/authors/r4023/variants/func_001bc220_wc2.c`, SHA-256 `6734f1cab911d8595e29d59b3104e4bc3475cb2ac43c1db669268ce2b1ff6a7d`, and retained receipt `scratch/authors/r4023/match/func_001bc220/wc2-game/func_001bc220_wc2.result.json`. The retained object SHA-256 is `2e2735464c80fd6abc3da10d852717255a54188491e99cf8b99aafc2bf13dcae`. It is **game / armlink5-inline-branchnop-v2 / none**, **128 candidate B versus 124 owner B**, `first_diff: null`, **no rewrites**. The independent review relinked that existing object; this author did not repeat the matcher or compiler. Typed text consumers and the exact object bound establish width separately from the independently observed sharing. The complete record and receipt digest are frozen in `research/catalogue_v9_r2_evidence.json`; the old evidence snapshot is untouched.

## Exact generated proposal and delta

- Proposal: `scratch/worktrees/catalogue-v10/scratch/analyze/catalogue-v9-r2.json`.
- SHA-256: **`7bc3fd8bc866ee5827a12eb6e86715ab1464c05830f4adb2d0b0071e39996ac3`**.
- Separate plain-CLI output: `scratch/analyze/catalogue-v9-r2-rerun.json`, complete bytes identical and the same SHA-256.
- All **4,699 complete function records are identical to exact v9**, including every size, kind, entry and classification. All **4,730 prior entry address/ISA pairs** survive. All **1,602 protected complete records / 186,152 B** survive unchanged.
- The **37 anomalies are identical**. There are no new classifications, exclusions, functions, owner changes, topology changes or C-credit changes.

| Measure | v9 | v9-r2 | Delta |
| --- | ---: | ---: | ---: |
| Unknown bytes | 4,417 | **4,408** | −9 |
| Unknown intervals | 87 | **84** | −3 |
| Unowned rodata bytes | 123,157 | **123,166** | +9 |
| Function-owned bytes | 1,139,722 | **1,139,722** | 0 |
| Padding bytes | 76 | **76** | 0 |
| Ordinary denominator functions / extent B | 4,624 / 1,131,454 | **4,624 / 1,131,454** | 0 |

Exactly `[0x001b54e5,0x001b54e8)`, `[0x001b8235,0x001b8238)` and `[0x001bc29d,0x001bc2a0)` move from unknown to the respective four-byte shared wide objects. Every other physical range, old unknown interval and anomaly is unchanged. The full v9 remaining-interval table therefore applies with those three rows removed. This is not complete catalogue closure.

## Reproduction and exercised operations

From `scratch/worktrees/catalogue-v10` at this discrete revision:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out /home/michael/code/kanken-training-2-decomp/scratch/worktrees/catalogue-v10/scratch/analyze/catalogue-v9-r2.json
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out /home/michael/code/kanken-training-2-decomp/scratch/worktrees/catalogue-v10/scratch/analyze/catalogue-v9-r2-rerun.json
/home/michael/code/kanken-training-2-decomp/.venv/bin/python scratch/closure/v9_r2_adopted_baseline.py
```

The first two are the complete plain generation procedure, without postprocessing or optional classification flags. The retained private third driver loads the **exact v9 proposal SHA-256 `9b13a770438c91b48ea34d8b2445f41c87ae2543a696618360a0ff550ddfe460` as the prior catalogue**, resolves its pinned c008 protection, and calls `build_catalogue` with unchanged image/layout, the r2 classification/encoding/table witnesses and `prior_functions=prior["functions"]`. It never replaces `config/catalogue.json`. Its equivalent API call is reproduced by the extended whole-image regression; the complete driver and metadata-only receipts are retained privately.

| Exercised operation | Exit | Observed result |
| --- | ---: | --- |
| First plain CLI generation | **0** | 28.835 s captured wall; proposal identity above |
| Separate plain CLI generation | **0** | 28.799 s; complete byte equality |
| Exact-v9-prior generation after the guard correction | **0** | 24.970 s; all 4,730 entries retained; function records equal both exact v9 and plain r2; ranges equal plain r2; 1,602 protected records |

Receipts are under `scratch/closure/`: `v9-r2-generation.json`, `v9-r2-regeneration.json`, `v9-r2-adopted-baseline-command.json`, `v9-r2-adopted-v9-evidence.json` and `v9-r2-differences.json`. They capture commands, exit codes, complete identities and exact range/stat differences. The reviewer's original exit-1 failure remains immutable evidence; the unchanged failing implementation was not rerun merely to reconfirm it.

**Not run:** new or existing tests, suites, builds, compiler probes, matcher runs, lint/formatters, prepare/finalize, adoption or push. Added regressions cover shared U+3000 bounds, refusal of non-whitespace controls, adopted multi-entry legacy-label regeneration, malformed adopted-label refusals and whole-image regeneration using the freshly proposed function records as the prior baseline. Main must run `tests.test_analyze` and the combined checks, then obtain independent re-review before any adoption. No claim of a passing test is made here.

## Main-observed fixture failure and isolated correction

Main ran the full suite on a clean detached checkout of `4891c6d6b923f3e510001a41f5f19ecfa8e45c04`.
It reported one failed module: `tests.test_analyze` ran 159 tests with two
subtest failures, the protected/unprotected cases of
`test_plain_catalogue_command_includes_review_and_reproduces_all_bytes`.
Its isolated temporary root still wrote the old `catalogue_v9_evidence.json`
name, while the corrected CLI requires `catalogue_v9_r2_evidence.json`.
The other 18 modules were reported OK; 566 tests were reported in total.
These are **main-observed facts**, not author-run checks, and the failure is
not rerun unchanged here.

The discrete fix changes that fixture's sole evidence-input path, supplies its
explicit `catalogue-v9-r2` revision and checks the serialized classification-input
path. A targeted source search found no other temporary-root fixture using the
retired evidence filename; the whole-image fixture already reads the r2 input.
Generator code and proposal contents are unchanged, so the proposal identity
above remains the generation identity. No suite or test was executed by this
author after the fix; Main owns the clean-checkout rerun.
