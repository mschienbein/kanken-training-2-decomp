# Independent harness-oracle review — round 3

## Verdict: FAIL — harness oracle ready to start decompiling

**Reviewer:** ReviewHarnessOracle. **Runtime-reported family/model:** OpenAI / `openai-codex/gpt-6.1-sol`.

Scope: exactly the round-2 allocation/COMDAT/absolute-symbol/NONE/shared-function fixes, build occupancy, and the requested round-1/round-2 behavioral regression pass. This is not a new analyzer-heuristic audit, pilot adoption, or D-gate certification. Only `scratch/review/round3/` was written. Canonical files, older review artifacts/seals, accepted build records and Git were not modified. No `boot9.bin` was read. Author disposition documents/other reviewer analyses were not read before sealing this verdict.

**The five specific round-2 examples are now repaired, and the requested legitimate controls pass.** However, the COMDAT repair still accepts an impossible reference binding: the same one-entity COMDAT data member is assigned two different, disjoint addresses in different credited units. Both functions report MATCH and the full build reports PASS. This changes literal words inside the credited functions; it is not merely a deferred data-layout/header/unwind/full-armlink qualification.

## Remaining finding

| ID | Severity | Evidence | Minimal repair |
|---|---|---|---|
| R3-01 | **blocker** | **One COMDAT entity can split into two addresses while both credited functions and the full image PASS.** `tools/inplace.py:184-197` deduplicates only keys containing `(signature, start, end)`; it never rejects a repeated signature/member at a *different* disjoint span. `tools/build.py:84-88,114-117` gathers per-object placements and calls only that overlap check. Each unit independently resolves its defined member through `tools/inplace.py:333-338`. With two ordinary C++ TUs instantiating `Holder<1>::value`, both real ARMCC objects have the same COMDAT signature `_ZN6HolderILi1EE5valueE`. The synthetic original requires one address-returning function's literal to be `0x102000` and the other's `0x102004`. The harness infers those two placements, sees no overlap, reports both MATCH, then **PASS: 2 functions / 24 bytes**, all receipt checks true. The catalogue has a complete tiled map, `analyze.validate_catalogue` returns `[]`, and the denominator chain verifies. Reproducer: `probe_comdat_identity.py`; evidence: `comdat-identity-results.json`, `comdat-divergent-placement/{match_comdat_a,match_comdat_b,full_build}.json`, and `comdat-divergent-placement/build/divergent-comdat/receipt.json`. | Select/bind COMDAT groups globally by identity, not by overlap coincidence. Keep a canonical member-address map for each selected group, resolve/check every unit's references against it, and fail before credit if another copy implies a different location for the same member. Preserve member identity/ownership within multi-member groups; a group signature alone does not make different members interchangeable. Keep the same-address legitimate copy control and independent-static non-aliasing controls. Add this **disjoint-address** negative case; the current test at `tests/test_inplace.py:276-282` only covers coincident copies and physically overlapping incompatible spans. |

### Concrete proof that this is a current credited-function false PASS

The two sources are legal duplicated template definitions, as commonly supplied by a header:

```cpp
template<int I> struct Holder { static int value; };
template<int I> int Holder<I>::value = 17;
extern "C" int *func_00100000() { return &Holder<1>::value; }
// In the other TU the otherwise identical function is named func_00100040.
```

Both symbols refer to **the same C++ object**. Both actual ARMCC objects put the object in `.data__ZN6HolderILi1EE5valueE`, in `GRP_COMDAT` group `_ZN6HolderILi1EE5valueE`, size 4, alignment 4. Their ABS32 references have the same entity and zero offset/addend. Once a legitimate linker selects one group copy at address `G`, both pointer literals must equal `G`.

The synthetic original instead contains:

- `func_00100000` literal at function `+8`: `0x102000`;
- `func_00100040` literal at function `+8`: `0x102004`;
- both pointed-to data cells contain 17.

There is no single `G` satisfying both fields. Selecting `G=0x102000` makes the second compiled function differ; selecting `G=0x102004` makes the first differ. Any other address also fails. The accepted sources therefore cannot reproduce both credited functions under the one-entity link semantics, even though independent per-unit inference can manufacture each match. This requires neither full game reconstruction nor header/unwind recovery to demonstrate.

`probe_comdat_identity.py` used the current-round real compiled template fixtures for independent group metadata and compiled both sources afresh through the unmodified copied build tool. The original synthetic instruction words were encoded independently (`ldr r0,[pc]`, `bx lr`, pointer word). All inputs live in the assigned round-3 directory. The complete map and hash-chain prerequisites are satisfied, so this is not a malformed-catalogue or missing-history escape.

The [ELF section-group contract](https://gabi.xinuos.com/elf/03-sheader.html#section-groups) defines duplicate COMDAT groups by signature and permits only one duplicate group to be retained. No native armlink execution is claimed; the actual compiler symbols/group metadata and the one-selected-entity constraint are the independent evidence.

## Focused fix/compatibility results

The following are **observed current-tree executions**, not merely accepted integrator claims:

| Case | Round-3 outcome / evidence |
|---|---|
| R2-01 required 8-byte alignment at `0x102004` | Both comparison and full build now reject with `not aligned to 8`. `r2-replay/focused-results.json`. |
| R2-02 legitimate duplicate COMDAT, same entity at the same span | Both comparisons and the full build now PASS. This fixes that prior false FAIL, but does not close R3-01's identity consistency. `r2-replay/focused-results.json:comdat_data_build`. |
| R2-03 odd SHN_ABS data plus addend | Target remains `0x102001`, Thumb=false; ABS32 result is correctly `0x102002`. `r2-replay/focused-results.json:absolute_odd_object`. |
| R2-04 live NONE dependency on unplaced section | Fails closed with `depends on required-data, which no reference places`; it is not silently dropped. Null-symbol V4BX still passes. `r2-arithmetic-replay/arithmetic-results.json`. |
| R2-05 different function offsets in one contiguous section | Explicit `--split_sections` rejection before EXTENT reporting. `r2-replay/focused-results.json:adjacent_unsplit_compare`. |
| **Real C1/C2 constructor aliases at the same section offset** | MATCH and full-build PASS; one physical function / 8 bytes. The size-zero C2 alias is preserved as an alias rather than rejected as a second offset. `r2-replay/focused-results.json:{ctor_alias_compare,ctor_alias_build}`. |
| **One `.data` section referenced by several functions** | Both functions MATCH and full build PASS, 2 / 24 bytes. References use one section symbol with addends 0/4; no false duplicate-occupancy rejection. `r2-replay/focused-results.json:{shared_data_compare,shared_data_build}`. |
| Named byte-offset symbols, internal Thumb REL32/PREL31, internal THM_JUMP11/8 | All targeted positive links match. `r2-arithmetic-replay/arithmetic-results.json`. |
| ARM BL/BLX input (including cond `0xF`), Thumb BL/BLX alignment, local/static and mixed-ISA calls | Independent decoded destinations agree in the 9 ARM and 6 Thumb cases; local/static and mixed-ISA positive links still match. `r1-replay/relocation-probes/results.json`, `r1-replay/followup-results.json`. |

## Round-1/round-2 regression pass

`replay_regressions.py` executed **11 sequential behavioral replay jobs** against current modules. The original sealed probes were not edited. `replay-ledger.json` binds each original probe and its adapted execution copy by SHA-256.

Adaptations were limited to fresh ROOT/OUT locations, diagnostic serialization for formerly-failing linker calls that now return dataclasses, and retaining top-level diagnostics after rejection paths that no longer produce a receipt/result file. The two already-known invalid compiler-attribute fixture spellings were omitted; the working mixed-ISA and `-O0` C++ replacements were exercised. The obsolete round-1 loop whose sole purpose was supplying a previously missing `analyze.py` history-hash prerequisite was omitted; it is already present in the replay setup. Seal/hash utility scripts were not replayed as behavioral tests.

`regression-summary.json` classifies **28 expected behavioral outcomes, all observed**. This is deliberately not a blanket “every old script prints success” claim:

- Independent writable objects still cannot alias within a unit or across units; the original initialized/uninitialized/single-TU attacks fail closed at the appropriate link/build stage.
- The overlapping catalogue is rejected by comparison and by the build's denominator gate; later writes cannot resurrect a credited mismatch.
- Reserved-name overlays are rejected by build and progress; classified veneers are refused. For build eligibility, `complete_regression_prereqs.py` supplied the new history prerequisite and used a fresh build name, so the actual kind guard—not merely missing history—was exercised.
- PREL31's Thumb result and overflow rejection are correct; internal branch/data placement and V4BX/NONE distinctions retain the expected outcomes.
- BLZ rejects the enormous footer before allocation and accepts exact-size normal inputs. The replayed plaintext import succeeds with **51 identical extracted file hashes/layout** compared with the existing import, and the independently hashed raw RomFS region matches its pin. The 23 synthetic codec roundtrips and 20,000 bounded malformed inputs retain their expected outcomes. No encrypted import or console-key access was performed.
- The old extra arithmetic matrix contains **one explicitly unverified legacy case**: PC24 BL-to-Thumb is now conservatively rejected as requiring a veneer. Its earlier direct-conversion expectation was not relabeled a passing test. Historical native-armlink behavior for that deprecated relocation was not established; the qualification is retained from round 2. The other 12 cases in that additional matrix pass.

The 7 retained top-level replay diagnostics are expected consequences of the repaired rejections (missing receipt/result files after rejection, a dependent print variable, and the direct single-TU overlap link). They remain visible in `replay-ledger.json`; process exit zero is **not** the acceptance oracle for those probes. The classified command results/equalities, not exit-zero alone, ground the 28-outcome summary.

## Exact verification performed

1. **One requested regression replay:**
   ```sh
   env PYTHONDONTWRITEBYTECODE=1 .venv/bin/python scratch/review/round3/replay_regressions.py
   ```
   11 jobs, sequential; all writes in new round-3 subdirectories. Saved commands/stdout/stderr and exception ledgers are retained.
2. **One new COMDAT consistency experiment:**
   ```sh
   env PYTHONDONTWRITEBYTECODE=1 .venv/bin/python scratch/review/round3/probe_comdat_identity.py
   ```
   Validated map `[]`, successful denominator verification, two MATCH results, then the false-PASS receipt described above. The group member is placed at `0x102000` for one unit and `0x102004` for the other.
3. **One full suite run:**
   ```text
   env TMPDIR=/Users/mooki/Code/kanken-training-2-decomp/scratch/review/round3/tmp \
       PYTHONDONTWRITEBYTECODE=1 .venv/bin/python -m unittest discover -s tests -t .
   Ran 65 tests in 18.177s — OK
   ```
   `suite-result.json` records the observed output. The passing suite does not cover the new disjoint-address identity failure.
4. **Eligibility prerequisite completion and saved-result classification:** `complete_regression_prereqs.py` ran once; `summarize_regressions.py` classified existing saved results without rerunning compilers/tests.
5. **Input/receipt bindings:** `bind_inputs.py` re-hashed 33 reviewed originals (including the actual compiler/launcher and private input identities) plus 15 new false-PASS receipt artifacts. All checks agree. The false-PASS receipt's harness hashes match the actual reviewed `tools/build.py` (`a045798e…`) and `tools/inplace.py` (`2441e8dd…`). This was not an old-harness receipt or a mocked compilation.

Only one compiler-heavy job ran at a time. The worktree was never committed, adopted or credited. The 8 GiB disk reserve was checked before the replay. No destructive cleanup was performed.

## Qualifications retained, not promoted to blockers

Per the assignment, the matches-ledger trust boundary, transitive-header/source-tree binding, unplaced `.ARM.exidx`/unwind metadata, and a complete native armlink image remain **D-gate/inherited qualifications**, not separate blockers. This review does not demand their completion as a substitute for fixing the current function oracle. R3-01 is different: selecting one current compiled data entity changes literal words in the very functions being credited.

The original pilot remains unaccepted; open compiler/stratification/registration/pilot gates and unknown catalogue ranges are not silently closed by these synthetic tests. Native Windows execution and cross-host determinism remain unverified. BSS/input-identity conclusions are unchanged; the supplied plaintext import was re-exercised without keys. The H3 wording/design change remains a visible contract qualification, not a newly implemented split-and-relink proof or independent authorization to alter acceptance policy.

## Seal

`review.json` contains the structured verdict and exact runtime model/family. `reviewed-inputs-r3.json` and `false-pass-receipt-bindings.json` bind inputs/evidence; `input-stability-r3.json` records final rehashes. `OUTPUTS-r3.sha256` seals only the round-3 directory. Earlier review files and seals are untouched. No repair is claimed applied, and no source adoption or progress credit is granted.

---

## Integrator dispositions (round 3)

| ID | Fix | Regression evidence |
|---|---|---|
| R3-01 | `reject_overlaps` binds every COMDAT member (group signature + member section name) to one canonical span across all units; a second placement at any other span is an error even without overlap. `build.py` and `place()` pass `inplace.comdat_key(section)` | `test_one_comdat_entity_has_one_address`; suite OK |

Process deviation (disclosed): while checking the fix, the integrator re-ran
`scratch/review/round3/probe_comdat_identity.py` in place. It rewrote
`scratch/review/round3/comdat-identity-results.json` (sealed sha256 `36e3fdff…`, now `f26c2cbc…`;
its build step was refused because the sealed build directory already existed). `REVIEW-r3.md`
and the fixture image are unchanged and still match the seal. The round-4 reviewer replays the
case in fresh directories.
