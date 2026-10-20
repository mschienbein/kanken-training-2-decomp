# C009-r2 — preserved initialization extent

## Status

Remediated; awaiting a fresh prepare and two independent reviews. No new credit is
claimed by this document. Selection remains `armlink5-inline-branchnop-v6` plus
`anonymous-string-records-v1`, frozen GATES v1.19 and denominator v9-r2.
The candidate manifest still contains 676 new functions / 57,052 B; all 1,621
previously accepted identities remain present. The retry also reopens the corrected
accepted `src/wave2/func_0011d260.c`.

## Preserved failed checkpoint

`accept/c009/` is the immutable failed preparation at source commit `88aa602`.
Both fresh builds failed the same complete-function extent check: `func_0011d260`
emitted 112 B instead of its required 116 B. G3 passed, and the full suite passed
806 tests with one skip. No reviewer was started on that failed preparation.

The regression came from refining `PipelineStorage` into a flexible-tail view.
Its fixed C prefix became 0x810 B; the initializer had incorrectly continued using
`sizeof(*storage)` for an operation that originally clears 0x854 B. This is a
source-model regression, not permission to weaken the extent check or retire credit.

## Correction and evidence

The retry reuses the correction already preserved in the c010 worktree:
`PIPELINE_STORAGE_INITIALIZED_BYTES = 0x854`, independent of the partial type's
`sizeof`. The original clear call passes the actual input pointer and the constant
2132-byte size. The flexible snapshot tail and all recovered offsets remain intact;
no total allocation size or fixed snapshot count is invented.

Private provenance:

- Existing c010 `CALLBACK-ATOMIC-ADJUDICATION-r1.json`,
  `owner-cutover/factory-facts-pipeline-r1.json`, and the nine passing
  `header-callback-match-r2` witnesses.
- Main's `scratch/integrator/c009-r2/compiled-audit-r1.json` reuses every object from
  the failed c009-a build without recompilation: 2,238 units / 2,297 functions;
  2,296 MATCH, with only `0011d260` failing extent. This reaches every ordinary
  header-dependent accepted unit, not only the earlier 838 review-required rows.
- The retry worktree's `scratch/integrator/c009-r2/pipeline-size-r1/` gives an
  isolated game-profile v6+L1 MATCH of 116/116 B, exit 0.
- Its `snapshot-consumer-r1/` gives `func_001272bc` MATCH of 28/28 B, exit 0,
  retaining the flexible-tail consumer's behavior.

These are bounded remediation witnesses, not two fresh whole-ledger builds. The new
prepare must recompile the complete ledger, run G3 and the suite, and bind all current
source/header hashes. Its actual required review set is authoritative.

## Qualifications retained

The source-policy, R1 identity provenance, L1 witnessed-span, C++ manual ABI-review,
ARMCC-extension, original-bug and counted-loop qualifications in
`docs/known-issues-c009.md` remain mandatory. In particular, `func_001c20e8` requires
a QUALIFIED row. The final `001e3050` correction uses its named `phase` member and
preserves the original 80-byte function; it does not use out-of-bounds `states[8]`.
The two new-function contract deferrals `00113c0c` and `0018ee88` remain explicit.
