# Candidate catalogue revision v4 — 2026-10-16

## Status and provenance

**PASS: complete candidate; not adopted. No canonical catalogue, accepted ledger, checkpoint, source tree, history, or receipt in the main checkout was edited.**
Runtime identity: **OpenAI — openai-codex/gpt-6.1-sol**.

- Worktree: `scratch/worktrees/catalogue-v4`, branch `catalogue-v4`, created from `armlink-inline` at **c11a0ad**. Only the assigned tracked files `tools/analyze.py` and `tests/test_analyze.py` were changed there.
- Candidate: [`catalogue-v4.json`](catalogue-v4.json). This report is `scratch/analyze/REPORT-v4.md` in the main checkout.
- Private evidence and reproducible checks: `scratch/worktrees/catalogue-v4/scratch/verification/` (called **V** below). These contain source, objects and metadata; no original executable bytes or disassembly were printed.
- `orig` and `.cache/toolchain` are symlinks to the main checkout. Every Python command used `/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python`; all compiles were sequential. Only `tests.test_analyze` was run, not any other test module or the full suite.
- Boundary interpretation uses **armlink5-inline-branchnop-v1**, as documented in the branch’s `docs/LINKER.md`. No production linker implementation was changed by this assignment.

| Input / edited source | SHA-256 |
| --- | --- |
| Adopted v3 `config/catalogue.json` in the worktree | `d8fc286cd6daf2b201dac18afa20f067fee983333bbb317a4123f7417e104ad7` |
| Candidate v4 | `ccb9168b75df6825bf1ee7b1ff7e86feb8f79c8b9a236b444a30bd8256f0f571` |
| `orig/code.bin` | `a367b5dcd65e3924c93cf3fbaee5d4d40f9c5205d1287912784a974d5e405361` |
| Accepted `config/matches.json` (c002-wave1-r3) | `9b0fe030bd582e3a5ca28b1764ab6ca962bbc16e2aa4ed525df66192f0531c19` |
| `docs/LINKER.md` | `2e06403883154833cc138d0049a44f6c633b8f64fb38eab556477eaf075d3e3b` |
| `tests/test_analyze.py` | `c739d4b038f30142506d989be6d1c565d3336c189d43593c02b0d53d989c999d` |
| `tools/analyze.py` | `6144aa240de427db058d71fbde27b88ed5119b294d189e0b5267c0c1f36af390` |
| `tools/inplace.py` | `dea627025ecaaa2e9f3ff88603efe426b14a31a7bb2fb6a760447bfc786fbc6f` |

## Counts versus v3

| Measure | v3 | v4 | Delta |
| --- | ---: | ---: | ---: |
| Top-level function/routine records | 4,351 | 4,437 | +86 |
| Callable starts, including explicit interior entries | 4,472 | 4,530 | +58 |
| Creditable ordinary functions (`kind == function`) | 4,231 | 4,345 | +114 |
| Creditable extent bytes | 1,108,426 | 1,111,866 | +3,440 |
| Non-C multi-entry owners | 114 | 86 | -28 |
| Veneers | 6 | 6 | +0 |
| Unknown bytes | 8,836 | 8,836 | +0 |
| Unknown ranges | 194 | 194 | +0 |
| Semantic anomalies | 82 | 82 | +0 |
| Function-owned explained bytes | 1,135,300 | 1,135,312 | +12 |
| Decoded instruction bytes | 1,054,474 | 1,054,486 | +12 |
| Decoded instructions | 264,698 | 264,701 | +3 |
| Pool/table bytes | 39,968 | 39,968 | +0 |
| Literal bytes | 40,858 | 40,858 | +0 |
| Padding bytes | 172 | 160 | -12 |
| Sum of routine extents | 1,136,786 | 1,136,798 | +12 |
| Fallthrough groups | 92 | 71 | -21 |
| Explicit secondary entries | 121 | 93 | -28 |
| NOP-labelled entries inside non-C groups | 28 | 0 | -28 |
| Removed weak starts | 61 | 59 | -2 |
| Noreturn owners | 5 | 6 | +1 |

The **+114 creditable functions / +3,440 B** is a denominator correction, not accepted C progress. The 28 formerly non-C NOP groups contribute 56 ordinary functions and 3,428 B; 55 leading-NOP extents become 110 ordinary functions without changing their aggregate bytes; three pointer-referenced padding words add three functions / 12 B. All 4,351 old top-level addresses and all 4,472 old callable entries remain present in the same ISA. There are **58 newly evidenced callable addresses**, 28 old secondary starts promoted to top-level bodies, and no removed records.

## Implemented boundary rules

1. Run the v3 discovery, boundary refinement, literal ownership and missing-target recovery first. At an evidenced ARM section start beginning with `mov r0,r0`, promote `address + 4` to an ARM start only when it is already decoded code. A secondary label inside an existing body is not by itself a section-start witness and stays multi-entry. Preserve existing evidence; newly recovered bodies carry `linker_branchnop` evidence.
2. Trace a recognized four-byte wrapper as a logical tail edge into the following section, not a generic fallthrough. This prevents NOP-only groups from making an ordinary body non-C. The logical edge does not become a fictitious BL/B source in entry metadata. Other fallthrough or externally callable interior entries still produce `multi_entry` records; synthetic coverage exercises this case.
3. A rodata/data pointer or non-switch literal-pool pointer to an unowned `mov r0,r0` word immediately before an independently known ARM entry establishes the padding word as a callable wrapper. This is independent of the old 32-instruction pointer-plausibility probe, which cannot recognize a NOP falling into a long callee.
4. Newly split bodies retain pointer evidence even if the address was formerly hidden in an enclosing extent. Thus the pointer-only return at `0x1ef884` is now its own ordinary four-byte function.
5. Architectural NOP hints, conditional MOVs, unreferenced padding, interior removed-call NOPs and trailing NOPs are not blanket split points. The trailing-NOP idiom in V4-BACKLOG item 3 is explained by the supplied linker model, not by inventing another boundary heuristic.
6. Additive metadata: `linker_tail_splits` lists each wrapper and its next-instruction target; `stats.linker_tail_wrappers` is **86**. Schema 1 and existing public function signatures are unchanged. Recovered weak starts are removed from the old merge ledger, and retained merge owners follow the final containing code record.

### Pointer witnesses and recovered empty function

| Pointer storage address | Storage / owning function | Entry target | Result |
| --- | --- | --- | --- |
| `0x00223960` | rodata | `0x001ef884` | ordinary 4 B function |
| `0x002243f8` | rodata | `0x0016a994` | ordinary 4 B function |
| `0x00110a98` | pool of `0x00110a70` | `0x001900a8` | ordinary 4 B function |
| `0x0019c658` | pool of `0x0019bc7c` | `0x00186eec` | ordinary 4 B function |

The three newly recognized padding wrappers are `0x16a994`, **`0x186eec`**, and `0x1900a8`. The body at `0x1ef884` is evidenced by the exact rodata pointer at **`0x223960`**. An additional C probe compiled these three wrappers plus the empty body: **4/4 MATCH**, with three `branchnop` rewrites and no rewrite for the empty function.

## Validation and preservation

- `validate_catalogue(candidate, original_image, exheader_layout)` returned **`[]`**. `functions_by_address` accepted all extents without overlap. The map tiles all **1,327,104 mapped bytes** exactly once.
- The private original-image unit test built the catalogue twice and asserted byte-identical serialized output and statistics, in addition to validation and the known literal, entry and pointer cases.
- All **19,991 direct BL/BLX sites** and **26,271 direct B sites** retain exactly the same `(site, transfer kind, target, target ISA)` tuples. There are **zero added/lost direct edges**. The only newly decoded instructions are the three pointer-referenced padding words.
- All 86 remaining multi-entry records and all six veneer records are byte-for-byte identical as JSON objects to v3. All unknown ranges, non-text ranges and 17 literal-conflict records are identical. The literal extent-change counters and object counts are unchanged; no v3 literal recovery was lost.
- Semantic anomaly counts and kinds remain unchanged at **82**. Two owner-address-only `unexplained_function_bytes` records move from `0x100078` to `0x10007c` and from `0x1c3abc` to `0x1c3ac0`, because their original wrapper no longer owns the body’s unknown holes. No anomaly is suppressed and no new anomaly category is introduced.
- Two previously removed weak starts, `0x100d6c` and `0x1ef884`, are restored; the merge ledger shrinks from 61 to 59 entries. All surviving merge records are unchanged.
- The new separate `0x100d6c` body is noreturn (`svc_exit_process`), so noreturn owners grow from five to six. The wrapper `0x100d68` now derives its existing noreturn property from its tail. At `0x100000`, only `noreturn_reasons` changes (`noreturn_call` → `noreturn_tail`); its bytes, extent, kind, evidence and entries are unchanged. This is the sole modified old record outside the 83 wrapper starts.

### Initial comparison-check failure, retained

The first metadata-check command exited **1**, because the private comparison harness incorrectly demanded literal equality of the anomaly address list. Production validation was already `[]`, all counts were already unchanged, and every other check passed. The two owner relocations above explain the difference. The comparator was changed to require precisely the modeled owner relocation for `unexplained_function_bytes`, preserving every other address/kind pair. The original failing command/output remains in **`V/metadata-command.json`**; the corrected check exits **0**, recorded in **`V/metadata-command-final.json`**. After the initial successful 78-test run, a defensive guard and regression were added so an interior secondary NOP label cannot falsely claim a new wrapper section. The final **79-test run passed**, and regeneration produced the **identical candidate hash**, so all earlier match results remain bound to the final catalogue. `V/metadata-before-secondary-entry-guard.json` preserves the earlier metadata; the final source hashes/checks are in `V/metadata.json` and `V/metadata-command-post-guard.json`. No source changed after the final test run.

## Compile/link match checks

All checks use the actual v4 candidate, not the linker agent’s temporary split catalogue. The baseline split-point selection is the same one in `research/linker/nop_wrappers.py`: 28 v3 `nop_entry` starts plus 55 leading-NOP starts. The generated C is exactly `extern void g(void); void f(void) { g(); }`, with address-named functions and the next start as g.

| Probe / ledger | Functions | Linker model | Result | Rewrite count |
| --- | ---: | --- | --- | ---: |
| v3 NOP-entry wrappers | 28 | armlink5-inline-branchnop-v1 | **28 MATCH** | 28 branchnop |
| v3 leading-NOP wrappers | 55 | armlink5-inline-branchnop-v1 | **55 MATCH** | 55 branchnop |
| Both wrapper groups, negative control | 83 | strict-v1 | **83 MISMATCH** (expected; both commands exit 1) | 0 |
| Newly pointer-referenced wrappers + empty body | 4 | armlink5-inline-branchnop-v1 | **4 MATCH** | 3 branchnop |
| Accepted ledger, 45 translation units | 55 | armlink5-inline-branchnop-v1 | **55 MATCH** | 0 |

The accepted ledger’s **55 functions / 2,680 B** have **identical entire catalogue records** to v3 and remain `kind: function`. Every accepted source was compared through the ordinary matching CLI against v4; no function was skipped. This is a per-unit match check, **not a new full-image acceptance build or new credit**. Source rules and the supplied linker model’s qualifications still apply.

### Commands, exits and output locations

The worktree-creation command ran from `/Users/mooki/Code/garden`. The symlink setup and all test/catalogue/check/match commands ran from `/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v4`.

```sh
git -C /Users/mooki/Code/kanken-training-2-decomp worktree add scratch/worktrees/catalogue-v4 -b catalogue-v4 armlink-inline
mkdir -p .cache && ln -s /Users/mooki/Code/kanken-training-2-decomp/orig orig && ln -s /Users/mooki/Code/kanken-training-2-decomp/.cache/toolchain .cache/toolchain
/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python -m unittest tests.test_analyze -v
/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out /Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v4.json
/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python scratch/verification/catalogue_checks.py metadata
/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python scratch/verification/catalogue_checks.py matches
/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python tools/match.py scratch/verification/pointer_entries.c --profile game --catalogue /Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v4.json --out scratch/verification/matches/pointer-entries --show 0 --linker-model armlink5-inline-branchnop-v1
```

- Worktree creation and symlinks: **exit 0**. Git reported a clonefile optimization fallback to a plain checkout; worktree creation succeeded at c11a0ad. No other git operation was run.
- Final analyzer module tests: **exit 0; 79 tests, OK, 94.087 s wall time**; complete stdout/stderr and argv in `V/tests-test_analyze-final.json`. The earlier 78-test passing run is retained in `V/tests-test_analyze.json`.
- Final catalogue generation: **exit 0; 45.984 s wall time**; `V/catalogue-command-final.json`; output is `scratch/analyze/catalogue-v4.json` in the main checkout. The first generation log remains in `V/catalogue-command.json`; both candidates have the same hash.
- Final metadata validation: **exit 0; 5.024 s wall time**; all 12 preservation checks true in `V/metadata.json`; full command/output in `V/metadata-command-post-guard.json`. The earlier corrected comparison remains in `V/metadata-command-final.json`.
- Sequential wrapper / accepted-ledger driver: **exit 0; 28.34 s wall time**. All 49 inner CLI argv arrays, working directories, exits and stdout/stderr are preserved in `V/match-commands.json`. The two strict-model commands deliberately exit 1 and are checked as expected negative controls; every default-model CLI exits 0. `V/match-check.json` binds result paths/hashes and exact expected functions.
- Extra pointer-entry CLI: **exit 0; 0.560 s wall time**; `V/pointer-command.json`, `V/matches/pointer-entries/pointer_entries.result.json`.
- No full build, full test suite, formatter, denominator recording, checkpoint preparation/finalization, commit or publication was performed. Adoption and any full-image acceptance build remain integrator actions, not an unfinished part of this candidate assignment.

## Complete changed-record inventory

**84 modified + 86 added + 0 removed = 170 changed top-level records**; the other **4,267 old records are identical**. All changed record addresses, old/new classifications/sizes, entry ownership and changed field names are listed below. `new record` means no old top-level record; where applicable, the address was already an explicit entry of the listed old owner. For added records all schema fields are new. The exact complete before/after JSON for every row is also preserved in `V/changed-records.json` (hash below), so evidence and source-list changes are not omitted by the compact table.

| Address | Status | v3 top-level record / entry owner | v4 top-level record | Changed fields |
| --- | --- | --- | --- | --- |
| `0x00100000` | modified | `function`, 32 B, 1 entry/entries | `function`, 32 B, 1 entry/entries | `noreturn_reasons` |
| `0x00100078` | modified | `function`, 16 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `literals`, `size` |
| `0x0010007c` | added | no top-level record | `function`, 12 B, 1 entry/entries | all fields (new record) |
| `0x00100d68` | modified | `function`, 8 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `noreturn_reasons`, `size` |
| `0x00100d6c` | added | no top-level record | `function`, 4 B, 1 entry/entries | all fields (new record) |
| `0x0010c338` | modified | `function`, 68 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `size` |
| `0x0010c33c` | added | no top-level record | `function`, 64 B, 1 entry/entries | all fields (new record) |
| `0x0010c37c` | modified | `function`, 200 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0010c380` | added | no top-level record | `function`, 196 B, 1 entry/entries | all fields (new record) |
| `0x0010e67c` | modified | `function`, 412 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0010e680` | added | no top-level record | `function`, 408 B, 1 entry/entries | all fields (new record) |
| `0x00110aa4` | modified | `function`, 548 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00110aa8` | added | no top-level record | `function`, 544 B, 1 entry/entries | all fields (new record) |
| `0x00114fa4` | modified | `function`, 44 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00114fa8` | added | no top-level record | `function`, 40 B, 1 entry/entries | all fields (new record) |
| `0x0011ef48` | modified | `function`, 12 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `size` |
| `0x0011ef4c` | added | no top-level record | `function`, 8 B, 1 entry/entries | all fields (new record) |
| `0x0011ef54` | modified | `function`, 12 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `size` |
| `0x0011ef58` | added | no top-level record | `function`, 8 B, 1 entry/entries | all fields (new record) |
| `0x00121764` | modified | `function`, 80 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00121768` | added | no top-level record | `function`, 76 B, 1 entry/entries | all fields (new record) |
| `0x001217b4` | modified | `function`, 132 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001217b8` | added | no top-level record | `function`, 128 B, 1 entry/entries | all fields (new record) |
| `0x00121bc4` | modified | `function`, 120 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00121bc8` | added | no top-level record | `function`, 116 B, 1 entry/entries | all fields (new record) |
| `0x00121da0` | modified | `function`, 60 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00121da4` | added | no top-level record | `function`, 56 B, 1 entry/entries | all fields (new record) |
| `0x00121ddc` | modified | `function`, 60 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00121de0` | added | no top-level record | `function`, 56 B, 1 entry/entries | all fields (new record) |
| `0x001224a0` | modified | `function`, 92 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001224a4` | added | no top-level record | `function`, 88 B, 1 entry/entries | all fields (new record) |
| `0x001224fc` | modified | `function`, 16 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00122500` | added | no top-level record | `function`, 12 B, 1 entry/entries | all fields (new record) |
| `0x0012250c` | modified | `function`, 16 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00122510` | added | no top-level record | `function`, 12 B, 1 entry/entries | all fields (new record) |
| `0x00122788` | modified | `function`, 264 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0012278c` | added | no top-level record | `function`, 260 B, 1 entry/entries | all fields (new record) |
| `0x00122890` | modified | `function`, 44 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `size` |
| `0x00122894` | added | no top-level record | `function`, 40 B, 1 entry/entries | all fields (new record) |
| `0x001228bc` | modified | `function`, 20 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001228c0` | added | no top-level record | `function`, 16 B, 1 entry/entries | all fields (new record) |
| `0x001228d0` | modified | `function`, 32 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `size` |
| `0x001228d4` | added | no top-level record | `function`, 28 B, 1 entry/entries | all fields (new record) |
| `0x00122984` | modified | `function`, 52 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `size` |
| `0x00122988` | added | no top-level record | `function`, 48 B, 1 entry/entries | all fields (new record) |
| `0x001229b8` | modified | `function`, 24 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001229bc` | added | no top-level record | `function`, 20 B, 1 entry/entries | all fields (new record) |
| `0x001229d0` | modified | `function`, 76 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001229d4` | added | no top-level record | `function`, 72 B, 1 entry/entries | all fields (new record) |
| `0x00122a1c` | modified | `function`, 20 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00122a20` | added | no top-level record | `function`, 16 B, 1 entry/entries | all fields (new record) |
| `0x00122a68` | modified | `function`, 104 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00122a6c` | added | no top-level record | `function`, 100 B, 1 entry/entries | all fields (new record) |
| `0x00125b50` | modified | `function`, 8 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `size` |
| `0x00125b54` | added | no top-level record | `function`, 4 B, 1 entry/entries | all fields (new record) |
| `0x0012d5c0` | modified | `multi_entry`, 108 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x0012d5c4` | added | entry of `0x0012d5c0` (no own record) | `function`, 104 B, 1 entry/entries | all fields (new record) |
| `0x0012d62c` | modified | `function`, 84 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0012d630` | added | no top-level record | `function`, 80 B, 1 entry/entries | all fields (new record) |
| `0x0012f518` | modified | `function`, 88 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0012f51c` | added | no top-level record | `function`, 84 B, 1 entry/entries | all fields (new record) |
| `0x0012fb38` | modified | `function`, 16 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0012fb3c` | added | no top-level record | `function`, 12 B, 1 entry/entries | all fields (new record) |
| `0x0012fb48` | modified | `function`, 16 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0012fb4c` | added | no top-level record | `function`, 12 B, 1 entry/entries | all fields (new record) |
| `0x001363b0` | modified | `function`, 84 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001363b4` | added | no top-level record | `function`, 80 B, 1 entry/entries | all fields (new record) |
| `0x00136404` | modified | `function`, 68 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00136408` | added | no top-level record | `function`, 64 B, 1 entry/entries | all fields (new record) |
| `0x00136858` | modified | `multi_entry`, 32 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x0013685c` | added | entry of `0x00136858` (no own record) | `function`, 28 B, 1 entry/entries | all fields (new record) |
| `0x0013706c` | modified | `function`, 64 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00137070` | added | no top-level record | `function`, 60 B, 1 entry/entries | all fields (new record) |
| `0x001370ac` | modified | `function`, 108 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001370b0` | added | no top-level record | `function`, 104 B, 1 entry/entries | all fields (new record) |
| `0x0013714c` | modified | `function`, 8 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `size` |
| `0x00137150` | added | no top-level record | `function`, 4 B, 1 entry/entries | all fields (new record) |
| `0x0013776c` | modified | `function`, 52 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00137770` | added | no top-level record | `function`, 48 B, 1 entry/entries | all fields (new record) |
| `0x001377a0` | modified | `function`, 208 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001377a4` | added | no top-level record | `function`, 204 B, 1 entry/entries | all fields (new record) |
| `0x001378d4` | modified | `multi_entry`, 176 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x001378d8` | added | entry of `0x001378d4` (no own record) | `function`, 172 B, 1 entry/entries | all fields (new record) |
| `0x0013e7d0` | modified | `function`, 64 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0013e7d4` | added | no top-level record | `function`, 60 B, 1 entry/entries | all fields (new record) |
| `0x00141c70` | modified | `multi_entry`, 104 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x00141c74` | added | entry of `0x00141c70` (no own record) | `function`, 100 B, 1 entry/entries | all fields (new record) |
| `0x00142640` | modified | `function`, 68 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00142644` | added | no top-level record | `function`, 64 B, 1 entry/entries | all fields (new record) |
| `0x00144220` | modified | `multi_entry`, 8 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x00144224` | added | entry of `0x00144220` (no own record) | `function`, 4 B, 1 entry/entries | all fields (new record) |
| `0x00147994` | modified | `function`, 116 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00147998` | added | no top-level record | `function`, 112 B, 1 entry/entries | all fields (new record) |
| `0x0014a988` | modified | `function`, 80 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0014a98c` | added | no top-level record | `function`, 76 B, 1 entry/entries | all fields (new record) |
| `0x0014a9d8` | modified | `function`, 188 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0014a9dc` | added | no top-level record | `function`, 184 B, 1 entry/entries | all fields (new record) |
| `0x0014c338` | modified | `function`, 40 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0014c33c` | added | no top-level record | `function`, 36 B, 1 entry/entries | all fields (new record) |
| `0x00156de8` | modified | `function`, 284 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x00156dec` | added | no top-level record | `function`, 280 B, 1 entry/entries | all fields (new record) |
| `0x0015e2b4` | modified | `function`, 104 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0015e2b8` | added | no top-level record | `function`, 100 B, 1 entry/entries | all fields (new record) |
| `0x0015e31c` | modified | `function`, 152 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x0015e320` | added | no top-level record | `function`, 148 B, 1 entry/entries | all fields (new record) |
| `0x00166eac` | modified | `multi_entry`, 112 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x00166eb0` | added | entry of `0x00166eac` (no own record) | `function`, 108 B, 1 entry/entries | all fields (new record) |
| `0x00166f1c` | modified | `multi_entry`, 228 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x00166f20` | added | entry of `0x00166f1c` (no own record) | `function`, 224 B, 1 entry/entries | all fields (new record) |
| `0x00169874` | modified | `multi_entry`, 128 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x00169878` | added | entry of `0x00169874` (no own record) | `function`, 124 B, 1 entry/entries | all fields (new record) |
| `0x0016a994` | added | no top-level record | `function`, 4 B, 1 entry/entries | all fields (new record) |
| `0x0016f39c` | modified | `multi_entry`, 336 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x0016f3a0` | added | entry of `0x0016f39c` (no own record) | `function`, 332 B, 1 entry/entries | all fields (new record) |
| `0x0017e358` | modified | `multi_entry`, 128 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x0017e35c` | added | entry of `0x0017e358` (no own record) | `function`, 124 B, 1 entry/entries | all fields (new record) |
| `0x00181e40` | modified | `multi_entry`, 68 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x00181e44` | added | entry of `0x00181e40` (no own record) | `function`, 64 B, 1 entry/entries | all fields (new record) |
| `0x00182718` | modified | `multi_entry`, 20 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x0018271c` | added | entry of `0x00182718` (no own record) | `function`, 16 B, 1 entry/entries | all fields (new record) |
| `0x00183030` | modified | `multi_entry`, 64 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x00183034` | added | entry of `0x00183030` (no own record) | `function`, 60 B, 1 entry/entries | all fields (new record) |
| `0x001852f4` | modified | `multi_entry`, 68 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x001852f8` | added | entry of `0x001852f4` (no own record) | `function`, 64 B, 1 entry/entries | all fields (new record) |
| `0x00185b9c` | modified | `multi_entry`, 20 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x00185ba0` | added | entry of `0x00185b9c` (no own record) | `function`, 16 B, 1 entry/entries | all fields (new record) |
| `0x00186484` | modified | `multi_entry`, 16 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x00186488` | added | entry of `0x00186484` (no own record) | `function`, 12 B, 1 entry/entries | all fields (new record) |
| `0x00186eec` | added | no top-level record | `function`, 4 B, 1 entry/entries | all fields (new record) |
| `0x0018f7ec` | modified | `multi_entry`, 8 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x0018f7f0` | added | entry of `0x0018f7ec` (no own record) | `function`, 4 B, 1 entry/entries | all fields (new record) |
| `0x001900a8` | added | no top-level record | `function`, 4 B, 1 entry/entries | all fields (new record) |
| `0x0019721c` | modified | `multi_entry`, 460 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x00197220` | added | entry of `0x0019721c` (no own record) | `function`, 456 B, 1 entry/entries | all fields (new record) |
| `0x0019dc1c` | modified | `multi_entry`, 228 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x0019dc20` | added | entry of `0x0019dc1c` (no own record) | `function`, 224 B, 1 entry/entries | all fields (new record) |
| `0x0019eb48` | modified | `multi_entry`, 24 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x0019eb4c` | added | entry of `0x0019eb48` (no own record) | `function`, 20 B, 1 entry/entries | all fields (new record) |
| `0x0019f2e4` | modified | `multi_entry`, 64 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x0019f2e8` | added | entry of `0x0019f2e4` (no own record) | `function`, 60 B, 1 entry/entries | all fields (new record) |
| `0x0019ffa4` | modified | `multi_entry`, 104 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x0019ffa8` | added | entry of `0x0019ffa4` (no own record) | `function`, 100 B, 1 entry/entries | all fields (new record) |
| `0x001b472c` | modified | `function`, 324 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001b4730` | added | no top-level record | `function`, 320 B, 1 entry/entries | all fields (new record) |
| `0x001b54e8` | modified | `function`, 272 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001b54ec` | added | no top-level record | `function`, 268 B, 1 entry/entries | all fields (new record) |
| `0x001c3abc` | modified | `multi_entry`, 404 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `literals`, `pools`, `size` |
| `0x001c3ac0` | added | entry of `0x001c3abc` (no own record) | `function`, 400 B, 1 entry/entries | all fields (new record) |
| `0x001c5b84` | modified | `function`, 8 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `size` |
| `0x001c5b88` | added | no top-level record | `function`, 4 B, 1 entry/entries | all fields (new record) |
| `0x001c6a34` | modified | `function`, 52 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001c6a38` | added | no top-level record | `function`, 48 B, 1 entry/entries | all fields (new record) |
| `0x001c6ad8` | modified | `function`, 8 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `size` |
| `0x001c6adc` | added | no top-level record | `function`, 4 B, 1 entry/entries | all fields (new record) |
| `0x001d32e0` | modified | `function`, 144 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `size` |
| `0x001d32e4` | added | no top-level record | `function`, 140 B, 1 entry/entries | all fields (new record) |
| `0x001d54ac` | modified | `multi_entry`, 124 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x001d54b0` | added | entry of `0x001d54ac` (no own record) | `function`, 120 B, 1 entry/entries | all fields (new record) |
| `0x001dacbc` | modified | `multi_entry`, 44 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x001dacc0` | added | entry of `0x001dacbc` (no own record) | `function`, 40 B, 1 entry/entries | all fields (new record) |
| `0x001dadc0` | modified | `multi_entry`, 44 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x001dadc4` | added | entry of `0x001dadc0` (no own record) | `function`, 40 B, 1 entry/entries | all fields (new record) |
| `0x001db130` | modified | `multi_entry`, 8 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `size` |
| `0x001db134` | added | entry of `0x001db130` (no own record) | `function`, 4 B, 1 entry/entries | all fields (new record) |
| `0x001e9818` | modified | `multi_entry`, 300 B, 2 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `entries`, `kind`, `pools`, `size` |
| `0x001e981c` | added | entry of `0x001e9818` (no own record) | `function`, 296 B, 1 entry/entries | all fields (new record) |
| `0x001eafa0` | modified | `function`, 304 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `pools`, `size` |
| `0x001eafa4` | added | no top-level record | `function`, 300 B, 1 entry/entries | all fields (new record) |
| `0x001ef880` | modified | `function`, 8 B, 1 entry/entries | `function`, 4 B, 1 entry/entries | `code_ranges`, `code_size`, `size` |
| `0x001ef884` | added | no top-level record | `function`, 4 B, 1 entry/entries | all fields (new record) |

## Accepted-function check inventory

| Function | Extent B | Catalogue record | V4 match |
| --- | ---: | --- | --- |
| `func_0010e664` | 12 | identical, ordinary/creditable | MATCH |
| `func_001149a8` | 24 | identical, ordinary/creditable | MATCH |
| `func_00136930` | 40 | identical, ordinary/creditable | MATCH |
| `func_00140dac` | 40 | identical, ordinary/creditable | MATCH |
| `func_001179b4` | 16 | identical, ordinary/creditable | MATCH |
| `func_001263b8` | 20 | identical, ordinary/creditable | MATCH |
| `func_00122350` | 16 | identical, ordinary/creditable | MATCH |
| `func_00103798` | 40 | identical, ordinary/creditable | MATCH |
| `func_001091d4` | 56 | identical, ordinary/creditable | MATCH |
| `func_001103d4` | 16 | identical, ordinary/creditable | MATCH |
| `func_00103258` | 136 | identical, ordinary/creditable | MATCH |
| `func_00103358` | 116 | identical, ordinary/creditable | MATCH |
| `func_00105084` | 64 | identical, ordinary/creditable | MATCH |
| `func_00105fa8` | 100 | identical, ordinary/creditable | MATCH |
| `func_0010d604` | 64 | identical, ordinary/creditable | MATCH |
| `func_001718ec` | 168 | identical, ordinary/creditable | MATCH |
| `func_00171c7c` | 168 | identical, ordinary/creditable | MATCH |
| `func_001670e4` | 8 | identical, ordinary/creditable | MATCH |
| `func_0011e928` | 72 | identical, ordinary/creditable | MATCH |
| `func_0018fdd4` | 40 | identical, ordinary/creditable | MATCH |
| `func_00195254` | 224 | identical, ordinary/creditable | MATCH |
| `func_00102cb0` | 32 | identical, ordinary/creditable | MATCH |
| `func_00105058` | 44 | identical, ordinary/creditable | MATCH |
| `func_00110358` | 4 | identical, ordinary/creditable | MATCH |
| `func_001107d0` | 4 | identical, ordinary/creditable | MATCH |
| `func_0011362c` | 4 | identical, ordinary/creditable | MATCH |
| `func_0011e970` | 4 | identical, ordinary/creditable | MATCH |
| `func_0011e974` | 4 | identical, ordinary/creditable | MATCH |
| `func_0011e978` | 4 | identical, ordinary/creditable | MATCH |
| `func_0011e998` | 4 | identical, ordinary/creditable | MATCH |
| `func_0019e02c` | 96 | identical, ordinary/creditable | MATCH |
| `func_0012ced8` | 4 | identical, ordinary/creditable | MATCH |
| `func_0012d1c8` | 4 | identical, ordinary/creditable | MATCH |
| `func_0012d4f0` | 4 | identical, ordinary/creditable | MATCH |
| `func_0012f4f0` | 28 | identical, ordinary/creditable | MATCH |
| `func_0012f50c` | 12 | identical, ordinary/creditable | MATCH |
| `func_0018fcb4` | 12 | identical, ordinary/creditable | MATCH |
| `func_00195e84` | 16 | identical, ordinary/creditable | MATCH |
| `func_00195e94` | 16 | identical, ordinary/creditable | MATCH |
| `func_0011f080` | 132 | identical, ordinary/creditable | MATCH |
| `func_00134f04` | 20 | identical, ordinary/creditable | MATCH |
| `func_00135a28` | 48 | identical, ordinary/creditable | MATCH |
| `func_00135a58` | 48 | identical, ordinary/creditable | MATCH |
| `func_00135a88` | 48 | identical, ordinary/creditable | MATCH |
| `func_00135ab8` | 48 | identical, ordinary/creditable | MATCH |
| `func_001360a4` | 56 | identical, ordinary/creditable | MATCH |
| `func_00196cb8` | 96 | identical, ordinary/creditable | MATCH |
| `func_0019f450` | 48 | identical, ordinary/creditable | MATCH |
| `func_0018fcc0` | 96 | identical, ordinary/creditable | MATCH |
| `func_0018fd20` | 72 | identical, ordinary/creditable | MATCH |
| `func_0019d5c4` | 24 | identical, ordinary/creditable | MATCH |
| `func_001c44ec` | 20 | identical, ordinary/creditable | MATCH |
| `func_001c4500` | 56 | identical, ordinary/creditable | MATCH |
| `func_001d2da4` | 52 | identical, ordinary/creditable | MATCH |
| `func_001d3524` | 80 | identical, ordinary/creditable | MATCH |

## Evidence artifact hashes

Paths in this table are relative to **V**.

| Artifact | SHA-256 |
| --- | --- |
| `catalogue_checks.py` | `eb17bfbc817bd8c22c4142c13ff64bb43c052a5c975db885fdc12a2ef115b721` |
| `changed-records.json` | `0f0a75af79eba41d4204ed5ed319930095eedfa89292a99163761ed453b009d9` |
| `metadata.json` | `0d9bbb109d93746d3e036e61a8487fa790fe22ac0c1708ce3ef0ac60283a026d` |
| `metadata-before-secondary-entry-guard.json` | `ba2e5f96a67eade0103c9a28183d884e54fd1a2fadf3023448758a9fba80f3f7` |
| `metadata-command.json` | `a81093fba5e3b920c763eb0cf955df0ac4bea40637eed352f384e48d5b516d83` |
| `metadata-command-final.json` | `314d4317e0436cd5aad3544f45bf17d6f0ea787837f0fe13e3ab37684b6631c3` |
| `metadata-command-post-guard.json` | `7e2269f65c3ef17e3beeed0a9f8dbc878964221b3c573c559e6adef74b19cade` |
| `tests-test_analyze.json` | `e7d7fa55caaeaba277a4717d88847ebd218bd5ab4772149b48c52e78e44ae3c2` |
| `tests-test_analyze-final.json` | `4ab4a114f41120b1bdf76e4a083b47f44ef7fa4783fe895bede5336bbe4d5e16` |
| `catalogue-command.json` | `4500fd92bc2934a4ff53c7eb69fe0f971a07c59426b9e21b59c102852aa762c7` |
| `catalogue-command-final.json` | `05abedee6b4350204449ea3ac064ae55183654137488534dae619e7a37202d3f` |
| `match-commands.json` | `35cc198f899eb8c59853e09674004c22deca022eff825265812cf454d3b85d72` |
| `match-check.json` | `7bec9c5f99d4ffe3bbac2cc6f7f6b868247c728d15fe8cb49f7aa13325eeaa94` |
| `pointer-command.json` | `4e966886fb6ca9bf915c5f3d2b4ecde84b59e82156c90a516be42fdc845d5b61` |
| `matches/pointer-entries/pointer_entries.result.json` | `3e580a14d46d0f619f56dc265dde6e2fadcfc8b8eabbdae8dcb7d5de0a7ee12e` |

### Individual match outputs

Paths below are relative to V. Every result JSON binds source/object/compiler/catalogue hashes, flags, model, compared sizes, statuses and per-function rewrite records.

| Group / unit | Functions | Exit | Result JSON | SHA-256 |
| --- | ---: | ---: | --- | --- |
| `leading_nop` | 55 | 0 | `matches/leading_nop/leading_nop.result.json` | `0c1f1cf6799cdbf333a195899e3493d2cf258cb7ea514a12a04c6721802d4715` |
| `leading_nop-strict` | 55 | 1 | `matches/leading_nop-strict/leading_nop.result.json` | `cc3bacb4966a64d64fecb5b05a9b9c601a03f5369bbfe6e38efb8cd48a154d78` |
| `nop_entry` | 28 | 0 | `matches/nop_entry/nop_entry.result.json` | `8f5abae2ede609687eaffb3888cae47fe0beb8bf12dc39a8e1358112465dce33` |
| `nop_entry-strict` | 28 | 1 | `matches/nop_entry-strict/nop_entry.result.json` | `dc2d0883bb07a0d0eea4fc84443f5196e4b14df3e68fccb818883bc156c0f14b` |
| `accepted-00-pilot_a` | 7 | 0 | `matches/accepted-00-pilot_a/pilot_a.result.json` | `f8f739ac7bd0e38c1d7bd49dee3a7317e83b73a09eaf2c24a84042ac1dcb2b40` |
| `accepted-01-pilot_b` | 3 | 0 | `matches/accepted-01-pilot_b/pilot_b.result.json` | `d73d3df6e4f4755116794c5d0ed43a24e0416d8e2cd587784cebdfc8f756c391` |
| `accepted-02-loop_cleanup` | 2 | 0 | `matches/accepted-02-loop_cleanup/loop_cleanup.result.json` | `47686c0e5a9c0c5fddbdfb27472925a064e37ec06b82289ee6d3ae9ff7a68167` |
| `accepted-03-loop_resources` | 1 | 0 | `matches/accepted-03-loop_resources/loop_resources.result.json` | `4b8f06588613d624e6b7bb69b49ba1a342bb3a8225e5c793faca7eb4136fcee1` |
| `accepted-04-loop_state` | 1 | 0 | `matches/accepted-04-loop_state/loop_state.result.json` | `a4a10eb666fa1adffe54cc47cb8e93e371b3a31683aa6714c32a8c4362a3d596` |
| `accepted-05-game_heap` | 1 | 0 | `matches/accepted-05-game_heap/game_heap.result.json` | `ca4111adbd1ecf87823e4a66d141181a76537a7cb1b8c5e12a6b2e1a00c798ed` |
| `accepted-06-game_enum_switch` | 2 | 0 | `matches/accepted-06-game_enum_switch/game_enum_switch.result.json` | `9e991428580612dfe719ce535e8392aa931ce1d17fe52c974c5816d05c4a887d` |
| `accepted-07-func_001670e4` | 1 | 0 | `matches/accepted-07-func_001670e4/func_001670e4.result.json` | `aca5bac1895499934064546c1ad68b50623bcfb0bcb083554dcf586487b30285` |
| `accepted-08-func_0011e928` | 1 | 0 | `matches/accepted-08-func_0011e928/func_0011e928.result.json` | `c014b9ace8970490ae7a1a3e4a5f3facda5e55f3e5c8a595e03d4490f9451b61` |
| `accepted-09-func_0018fdd4` | 1 | 0 | `matches/accepted-09-func_0018fdd4/func_0018fdd4.result.json` | `9e2d7d575d7807102b07c44c3efe7b74056ab2f8e77c697804d370730ff9be52` |
| `accepted-10-func_00195254` | 1 | 0 | `matches/accepted-10-func_00195254/func_00195254.result.json` | `080c42de10f9bcda963de7a129d91f8e0f44cedb7fe414a93c5934d69de422c5` |
| `accepted-11-func_00102cb0` | 1 | 0 | `matches/accepted-11-func_00102cb0/func_00102cb0.result.json` | `48353b5003054595e6fcc2e6b33ae97b6bb2245647b1afabc1ce8eb91c448438` |
| `accepted-12-func_00105058` | 1 | 0 | `matches/accepted-12-func_00105058/func_00105058.result.json` | `bd02680c2671644814694fd21fd315f1ad6489d6fbfeadc2f1f2aa218388d89d` |
| `accepted-13-func_00110358` | 1 | 0 | `matches/accepted-13-func_00110358/func_00110358.result.json` | `a4596f5259469a2bbf3cd87c4cc2e7eadbf140e75a9e64ff8a393f0da3ba005a` |
| `accepted-14-func_001107d0` | 1 | 0 | `matches/accepted-14-func_001107d0/func_001107d0.result.json` | `5b8259dbdbf82ecdceef14d409d5600a8773ddd5f6f4d2064285f23a7ad88995` |
| `accepted-15-func_0011362c` | 1 | 0 | `matches/accepted-15-func_0011362c/func_0011362c.result.json` | `75a7a7eebedcaae6fdca85188658029c7d9c8d167b027485b89b3a8de6b9ec49` |
| `accepted-16-func_0011e970` | 1 | 0 | `matches/accepted-16-func_0011e970/func_0011e970.result.json` | `4a4f80020e2ba22f29149c240dc06f215502d0be83e97c3a5bcbf663622cb8a0` |
| `accepted-17-func_0011e974` | 1 | 0 | `matches/accepted-17-func_0011e974/func_0011e974.result.json` | `8f8c01678e6ea1288c8960c7579c849805028eaa45edbd7475e26a06a2b75d76` |
| `accepted-18-func_0011e978` | 1 | 0 | `matches/accepted-18-func_0011e978/func_0011e978.result.json` | `d3249a778a5166e9c5119480fdc5f23d87146d0d3e2fa25679a05816726059b9` |
| `accepted-19-func_0011e998` | 1 | 0 | `matches/accepted-19-func_0011e998/func_0011e998.result.json` | `9483872659c2584cf4f171d8d7e40d5a0f1afc16c1dfeb372cc85600d3e64942` |
| `accepted-20-func_0019e02c_returning` | 1 | 0 | `matches/accepted-20-func_0019e02c_returning/func_0019e02c_returning.result.json` | `c8b2986b2193ee1e8dfa83b6f7a738c6f97c100f7252746322cb1edb1bba58c0` |
| `accepted-21-func_0012ced8` | 1 | 0 | `matches/accepted-21-func_0012ced8/func_0012ced8.result.json` | `6020e476a3e39a3e8f779ce03a95733d3a7138d847d9c5aa0302cbdd57b0d931` |
| `accepted-22-func_0012d1c8` | 1 | 0 | `matches/accepted-22-func_0012d1c8/func_0012d1c8.result.json` | `9ea60f0f72d7f8d0e720219e743901110cee4f8c134613a151b9ecc4cf3b0451` |
| `accepted-23-func_0012d4f0` | 1 | 0 | `matches/accepted-23-func_0012d4f0/func_0012d4f0.result.json` | `c7f880acae1aa610bee647e9d729c0098d9b77e537d2e06f72b120f30e9f05e2` |
| `accepted-24-func_0012f4f0` | 1 | 0 | `matches/accepted-24-func_0012f4f0/func_0012f4f0.result.json` | `5a5e0daeee9cc21653a900f8b4933749bd28bb0dde7f61a668696d5ed4443877` |
| `accepted-25-func_0012f50c` | 1 | 0 | `matches/accepted-25-func_0012f50c/func_0012f50c.result.json` | `653c7f5ae922f53a6b11869da58c899cdf569cfc2ab7a980d39cf7e5b12ed60b` |
| `accepted-26-func_0018fcb4` | 1 | 0 | `matches/accepted-26-func_0018fcb4/func_0018fcb4.result.json` | `bfd17e18ea2a3cb4d012f0e8865c07df0bd60ec1189d2f090c65258b0e7686a2` |
| `accepted-27-func_00195e84` | 1 | 0 | `matches/accepted-27-func_00195e84/func_00195e84.result.json` | `91c8d20d44d06914a7ed32948f8821e825592b5ba7bb7207c46a29c9b6994e01` |
| `accepted-28-func_00195e94` | 1 | 0 | `matches/accepted-28-func_00195e94/func_00195e94.result.json` | `09bc89e37effa054ec25bb03f9bd0390913e922dca5aad212f293d4adc596c79` |
| `accepted-29-func_0011f080` | 1 | 0 | `matches/accepted-29-func_0011f080/func_0011f080.result.json` | `443afcd7243af6389a4ac99454e5e12c2a94d4eb7bdb04dfa1ea83cb4d753761` |
| `accepted-30-func_00134f04` | 1 | 0 | `matches/accepted-30-func_00134f04/func_00134f04.result.json` | `22af32a38e0107888d88b2d6e8fd5509f97185e2357a251f67a69e132724268f` |
| `accepted-31-func_00135a28` | 1 | 0 | `matches/accepted-31-func_00135a28/func_00135a28.result.json` | `1785a227189272fa07e74e3728909313b7a7ba6df1ac5bafe5b3cf1f8e95a732` |
| `accepted-32-func_00135a58` | 1 | 0 | `matches/accepted-32-func_00135a58/func_00135a58.result.json` | `6d88e05c3115827be5447e41ee729ed97d3b9a05a88cdf3c8c95d6cac73b3b4d` |
| `accepted-33-func_00135a88` | 1 | 0 | `matches/accepted-33-func_00135a88/func_00135a88.result.json` | `c087c934112184018c65a7a55675cf02005493730ea564cf58670bb664b20c46` |
| `accepted-34-func_00135ab8` | 1 | 0 | `matches/accepted-34-func_00135ab8/func_00135ab8.result.json` | `d31e36fd726c5ad718c2ec9cbb53f8c6cf634054b37fa0a27aabad827dd86c72` |
| `accepted-35-func_001360a4` | 1 | 0 | `matches/accepted-35-func_001360a4/func_001360a4.result.json` | `38fcf8f3d3abde46739eef37ba24f3be8dfe16a11f0401af13c5350e20fbeefb` |
| `accepted-36-func_00196cb8` | 1 | 0 | `matches/accepted-36-func_00196cb8/func_00196cb8.result.json` | `703a580f141ce26a989c59ad155ca1fdf04ae2dbdc662c56bbc99032080311a4` |
| `accepted-37-func_0019f450` | 1 | 0 | `matches/accepted-37-func_0019f450/func_0019f450.result.json` | `e83721f526df3b1467b1299f415b78ee9fd21d2a78829b93d444b25f49c6861a` |
| `accepted-38-func_0018fcc0` | 1 | 0 | `matches/accepted-38-func_0018fcc0/func_0018fcc0.result.json` | `aeb9328729396d1e1afab13021c1e2947ce30e79020b2082a5368c64f3dec446` |
| `accepted-39-func_0018fd20` | 1 | 0 | `matches/accepted-39-func_0018fd20/func_0018fd20.result.json` | `75e55482013dd02d94b9246d5672f012fd3756fa205a40749029fbe8fb1b9db9` |
| `accepted-40-func_0019d5c4` | 1 | 0 | `matches/accepted-40-func_0019d5c4/func_0019d5c4.result.json` | `7ddd7e08cb10ef34f578f0494044a4b6f7ec1e521bdec76599c949a6eb59fc9d` |
| `accepted-41-func_001c44ec` | 1 | 0 | `matches/accepted-41-func_001c44ec/func_001c44ec.result.json` | `c7fa031877092e24155af713ba711278371f0a058c1b1be57cf21ce88fe2dd21` |
| `accepted-42-func_001c4500` | 1 | 0 | `matches/accepted-42-func_001c4500/func_001c4500.result.json` | `fb3d5df536b510bf232fa2d192fa0548f9ca674ca2db3bbd94839feede7bdb82` |
| `accepted-43-func_001d2da4` | 1 | 0 | `matches/accepted-43-func_001d2da4/func_001d2da4.result.json` | `4f74c33d2c8a10334d82cd2f76e72dcddd5f4758cc081bc790dd57f3d820ae32` |
| `accepted-44-func_001d3524` | 1 | 0 | `matches/accepted-44-func_001d3524/func_001d3524.result.json` | `6d3838cdee2bc9c65b7cc881163d551e422713c0161378550360545268a251c4` |

## Remaining qualifications

- This candidate is ready for independent review and integrator adoption, but is not a denominator-history revision or an accepted checkpoint.
- The existing 82 semantic anomalies / 8,836 unknown bytes remain openly recorded; v4 does not claim to resolve unrelated section-layout uncertainty.
- As stated in `docs/LINKER.md`, the final image proves a tail to the next section and the modeled linker rewrite, not whether section adjacency came from `--tailreorder` or source/link order. Removed-call target identity remains subject to the existing linker-model qualifications.
- No blocker or unimplemented item remains within the assigned v4 boundary/pointer scope.
