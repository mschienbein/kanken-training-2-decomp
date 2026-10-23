# Catalogue-v14 independent verdict

**FAIL**. Finding **F01 / major / P2**. Confidence **0.97**.

Frozen source `5b8f88dddd987cff94dc03bca6291b5c9ce7b585`, parent `5820254f`.
Proposal SHA-256 `add2952983f72625f2d990605864b4389889da11dd413f09bd040ccbaa012eff`.

## Finding

**F01 — Replace pinned totals and prose assertions with behavioural invariants** (`tests/test_catalogue_v14.py:90-97`; related
102-112, 177-178 and 201-202). The new regression module pins aggregate revision totals (2682/288580 and 25/1580 here, plus successor totals at lines 102-112), while the wcsncpy test checks explanation prose at lines 177-178. These assertions do not derive the protected identity set or certificate-transition relation and do not exercise the C/C++ boundary. This violates mandatory review-contract check 6: derive exact expected identity/extent sets from the frozen ledger and proof, and replace or remove prose-only checks in favor of the relevant input-binding/refusal behaviour. Existing substantive mutation/refusal tests should remain.

The 12 tests actually executed all pass. The finding is the explicit check-6 test-design
shortfall, not a reported unittest failure.

## Required-check results and run limits

- **Accounting PASS:** exact owner/entry/ISA/extent sets unchanged; exactly 25 complete
  records differ only in kind/evidence. Denominator **4571/1121674 B -> 4546/1120094 B**.
  Physical ranges, all eleven unknown intervals/114 B and the one anomaly are identical.
- **Protection PASS:** all 2682 accepted records, 98 c011-r10 new-unit identities and every
  named ctypes1/cmodels2/c012/harvest claim retain their complete records.
- **Consumers/input closure PASS:** every direct tools/tests/ci live-key consumer traced;
  no dropped consumed key or silent skip found. Parent blob/history/docs remain reachable;
  missing/extra retained-input roles refuse.
- **Library replay LIMITED:** the single granted checker reached its failure after the
  five complete vendor-definition and four whole-caller assertion loops, but no per-row
  receipt was emitted; callback-parent reconstruction was not run. See replay-frontier.json.
- **Assembly and generation INCOMPLETE:** reviewer-only LF-specific public-source assertion
  at private_evidence_audit.py:116 failed before original-trace comparisons and before the
  public generator command. One granted wrapper execution, exit **1**; atomic release
  published immediately. **Zero public regenerations**, no byte-identical regeneration claim.
  Main requested pause/no further passes; no retry or extra grant was requested.
- **Tests:** reviewer ran **12 tests, all pass**, plus two input-roster refusals. Main ran
  **1104 tests, 1 skipped, zero failed modules, exit 0**, result hash independently checked.
  That suite was not rerun and does not replace missing review coverage.

The reviewer-only harness failure is not a patch finding. It is an additional reason this
review cannot approve adoption. REPORT.md and REVIEW.json distinguish observed execution,
frontier-based inference, retained author claims and missing checks.

## Qualifications preserved

- Q01: The same 114 unknown bytes in eleven intervals and the 0010d2b8 unexplained_function_bytes anomaly remain. No zero-residue closure or adoption is claimed.
- Q02: Known named complete vendor-section equality and caller/ABI correspondence do not uniquely identify the historical archive/compiler/TU. The selected _snputwc positive is 5.06-b960 only; the old 5.04-b82 negative remains negative.
- Q03: wcsncpy identity evidence does not equate native C++ wchar_t nominally with unsigned short or approve the diagnostic source. The installed public C header contains the unsigned-short wchar_t typedef. The retained native-header C++ refusal and C mismatches remain failures.
- Q04: The wcslen replay adds natural-call/ABI corroboration independent of the target-body comparison, not independently recovered historical import spelling. The new source and names overlay introduce the name explicitly. Under the existing stated body-plus-natural-call/ABI standard this is bounded identity corroboration, not provenance uniqueness, source/profile credit or retrospective reversal of the correct old body-only refusal.
- Q05: The twenty handwritten-assembly names follow exact retained public-macro correspondence and canonical asm declarations, not an authenticated historical NW4C archive/version. A fresh independent original-trace comparison was not completed here.
- Q06: Candidate c012-r4 and harvest inputs are bound to the exact hashes read; future changes require Main reconciliation. The c011-r10 NEW-unit claims are included. All named protected complete records are unchanged.
- Q07: Main supplied the full-suite PASS; this reviewer did not rerun it. The reviewer-only granted replay failed at its own source-format assertion; no second pass or extra grant was requested after Main instructed pause/no further passes. This failure is not a patch defect and does not support a passing catalogue seal.

All evidence and exact limits are sealed by SEAL.json. No source/config/docs/gates edit,
compiler/matcher command, build, acceptance operation, adoption, commit or push occurred.
