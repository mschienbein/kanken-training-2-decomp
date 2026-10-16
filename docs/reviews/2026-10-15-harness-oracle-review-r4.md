# Independent harness-oracle review — round 4

## Verdict: PASS_WITH_QUALIFICATIONS — harness oracle ready to start decompiling

**Scope:** the matching/integration-oracle repair reviewed in this short round, not blanket H1–H11 completion, pilot adoption, or D-gate acceptance. **Reviewer:** ReviewHarnessOracle. **Runtime-reported family/model:** OpenAI / `openai-codex/gpt-6.1-sol`.

**Remaining findings in the reviewed repair and requested controls: none observed.** R3-01 is closed by the new global canonical span check. The divergent-COMDAT reproduction now fails at the intended integration gate, while the legitimate same-span COMDAT control still passes. The full suite was run once and passed all 66 tests.

All new writes are confined to `scratch/review/round4/`. Canonical files, earlier review artifacts and accepted records were not modified by this reviewer; no Git command, destructive cleanup, adoption, progress credit or `boot9.bin` access was performed. Author disposition documents/other reviewer analyses were not read before this verdict.

## Repair re-derived from current source

- `tools/inplace.py:184-202` now keeps a canonical span for each COMDAT key and rejects a second distinct span **even when the ranges do not overlap**.
- `tools/inplace.py:209-210` identifies a member by `(group signature, member section name)`, rather than treating a group signature alone as every member's identity.
- Both local placement (`tools/inplace.py:288-290`) and cross-unit build occupancy (`tools/build.py:84-88,114-117`) use that same key and check.
- `tests/test_inplace.py:285-289` now covers the formerly accepted disjoint-address contradiction. The existing same-span copy/independent-section cases remain present.

This closes the actual R3-01 cause: the two units can no longer give one tested COMDAT member two different addresses and earn a PASS receipt. No full native-linker implementation was substituted for the requested fix.

## Fresh negative and positive reproductions

`probe_comdat_pair.py` constructs both cases from new C++ source files, independent synthetic instruction words, complete tiled catalogues, and fresh scratch roots. It uses the real pinned ARMCC through the existing wrapper, with sequential compiler calls. It does **not** read the modified round-3 result file or reuse an earlier build directory/receipt.

Both TUs instantiate `Holder<1>::value` (initialized int 17), giving the actual key:

```text
(_ZN6HolderILi1EE5valueE, .data__ZN6HolderILi1EE5valueE)
```

| Case | Prerequisites / per-unit results | Full-build outcome |
|---|---|---|
| `divergent/`: same member inferred at `0x102000` and `0x102004` | Catalogue validation `[]`; history record and verify succeed; both isolated comparisons MATCH | Exit **2**, explicitly reports the two conflicting COMDAT spans; **no PASS receipt exists** |
| `same-span/`: both copies inferred at `0x102000` | Catalogue validation `[]`; history record and verify succeed; both comparisons MATCH | Exit **0**, receipt **PASS**, 2 functions / 24 bytes; credited-functions, code.bin and packed-code equality checks all true |

The negative case's isolated MATCH results are not acceptance: each TU alone has a valid placement. The full build now detects their incompatible shared identity, as required. Evidence is in `comdat-pair-results.json`, the per-case command JSON files, and `same-span/build/round4/receipt.json`.

## Exact verification

1. Ran once:
   ```sh
   env PYTHONDONTWRITEBYTECODE=1 .venv/bin/python scratch/review/round4/probe_comdat_pair.py
   ```
   Both expected outcomes were observed. Cases and output directories were new; no old receipt name was reused.
2. Ran the full suite **exactly once**:
   ```text
   env TMPDIR=/Users/mooki/Code/kanken-training-2-decomp/scratch/review/round4/tmp \
       PYTHONDONTWRITEBYTECODE=1 .venv/bin/python -m unittest discover -s tests -t .
   Ran 66 tests in 13.291s — OK
   ```
   `suite-result.json` records the observed result. No unchanged failure was rerun for confirmation.
3. `bind_inputs.py` re-hashed **33 reviewed originals** and **45 fresh fixture artifacts**. Copied harnesses match the current reviewed sources; the positive receipt's listed source/object/output/harness hashes all agree. Current reviewed `inplace.py` SHA-256 is `c28e7fe3a9f471d0a6a080abfe1c5dd349a70bfa59dbbebd994c16efcfaa1942`; `build.py` is `376536603b034a0ade2a758a8dfa11ebf6203810fa70aedc196964cfae9a3314`. See `reviewed-inputs-r4.json` and `fixture-bindings-r4.json`.

No native Windows job, native armlink run, new ROM import, fresh real-game acceptance build or full historical replay corpus was run in this short round. The round-3 legitimate constructor-alias/shared-data controls and broader regression observations are inherited evidence, not mislabeled new round-4 executions. This round newly exercises the requested same-span COMDAT control and the full current suite.

## Evidence-integrity disclosure

The integrator reported accidentally rerunning `scratch/review/round3/probe_comdat_identity.py` in place. At least the disclosed `round3/comdat-identity-results.json` no longer matches its old seal (reported SHA-256 prefix `36e3fdff…` changed to `f26c2cbc…`); the old build name was refused, and the integrator reports that `REVIEW-r3.md` and the fixture image still match their prior seal.

I accepted that disclosure as ground truth, did not rerun a check merely to confirm it, and did **not** restore files or rewrite the old seal. This review makes no assertion that the entire old round-3 output set is untouched or currently seal-valid. Its new conclusion rests on the fresh round-4 sources, images, commands and hashes only. The disclosed historical evidence mutation is recorded transparently and is not treated as a new oracle-code finding.

## Qualifications — not blockers for this scoped oracle verdict

- **D-gate source/evidence authority:** `matches.json` remains an integrator-trusted ledger; complete transitive-header/source-tree binding and final accepted receipt/review history remain acceptance work, not guarantees supplied by this short review.
- **Hybrid scope:** uncredited original bytes, `.ARM.exidx`/unwind metadata, broader global-linker reconstruction and a complete native armlink image remain outside the current hybrid proof. No preserved original data is promoted to C credit by this verdict.
- **Compatibility boundaries:** native Windows/cross-host determinism and historical PC24 BL-to-Thumb direct conversion remain unverified. No new execution claim is made for those paths.
- **Project gates:** the pilot is not adopted, accepted progress is not changed, and compiler stratification/registration/pilot work, catalogue unknowns and anomalies are not silently declared complete. This is not a certificate that every H1–H11 requirement or later D gate has passed.
- **Contract history:** the earlier H3 wording/design change is not newly implemented split-and-relink evidence or independent authorization of acceptance-policy changes.
- **COMDAT scope:** the observed fix is sound for the actual keyed member/control cases exercised here; this is not a claim to have exhaustively implemented or validated all native-linker group-selection behaviors.

## Seal

`review.json` contains the structured scoped verdict and exact runtime model/family. `input-stability-r4.json` records final input rehashes; `OUTPUTS-r4.sha256` seals only this new round-4 directory. No repair was applied by the reviewer, and no adoption or progress credit is granted.
