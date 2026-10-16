# Candidate catalogue revision v2 — 2026-10-16

## Status, scope and provenance

**Candidate only: not adopted, not a frozen denominator, and not C/C++ match credit.**
Runtime identity: family **OpenAI**, model **openai-codex/gpt-6.1-sol**.
Only `tools/analyze.py`, `tests/test_analyze.py`, this report and the assigned scratch
candidate were written. `config/catalogue.json` and denominator history were not edited.
No builds, full test suite, formatter, git command or publication was run.
No original bytes or disassembly are included in these changes or report.

Input remains `orig/code.bin`, 1,327,104 mapped bytes,
SHA-256 `a367b5dcd65e3924c93cf3fbaee5d4d40f9c5205d1287912784a974d5e405361`. Layout comes from `ctrfmt.parse_exheader`.
Generator uses installed Capstone 5.0.7.
The comparison baseline is canonical schema-v1 catalogue SHA-256
`06027f34eb7e89666c138293a31393cbafc90d9c66862add8f3d9df571aedf13`, the v1 author's report `scratch/analyze/REPORT.md`,
and the unchanged denominator history. Existing schema-v1 fields and the signatures
of `build_catalogue`, `validate_catalogue`, `serialize_catalogue` remain stable.

## Boundary and termination rules

- A decoded PC-relative **load** constrains all referenced pool words to its own
  ARMCC split section, including pools skipped by a branch with later code using
  them backwards. Address-taking references are not equated to loads: their eight
  residual anomalies remain visible rather than forcing speculative section merges.
- Constraints may span multiple adjacent weak candidates. Other merge witnesses
  are branches into another candidate's body and actual fallthrough with no
  recognized return in the preceding candidate. An entry-to-entry tail branch
  alone is **not** sufficient (the existing tail-call regressions still pass).
- Weak evidence is `tail_call`, `prologue`, an isolated `pointer`, or unproved
  `call_scan`. Both original call-scan-only starts were mergeable because their
  supposed call sources were never observed executable. Every observed BL/BLX
  start is protected, even when the caller is inside the proposed merged section;
  this is stricter than protecting only external callers.
- Callback/vtable protection uses aligned pointer runs to at least two distinct
  starts in rodata, data or recovered text pools, allowing one intervening null
  callback slot. Address-switch tables are excluded. **636 table-target starts are
  protected; none disappeared.** Veneer-pattern/veneer-target evidence and mixed
  ISA boundaries are also protected. An incoming tail edge from outside a proposed
  group prevents deleting that entry.
- Removed internal labels remain trace roots within their retained section, so
  pointer-named/otherwise disconnected fragments do not silently vanish. A merge
  probe must retain previously decoded instructions, avoid code/pool overlap and
  preserve every observed call's target/ISA. Each removal has evidence, reasons,
  witness addresses and a final `retained_address` in `boundary_merges`.
- Noreturn inference first requires a recovered graph with **no recognized returns
  and no open exits**. Incomplete decode, fallthrough into another start, unresolved
  switches and unknown indirect exits disqualify it. Closed tail cycles are solved
  together. ExitProcess (`svc` service 3, ARM or Thumb) and closed loops seed the
  result; terminating direct calls propagate it through fresh traces. Conditional
  calls/exits retain their returning paths. The additive per-function fields are
  `noreturn` and `noreturn_reasons`.
- Start discovery and callback-table protection finish **before** new termination
  pruning. This prevents entries evidenced by pools on subsequently pruned paths
  from disappearing without a merge record. A dedicated synthetic regression
  covers both entry retention and table protection across ExitProcess pruning.

These are evidence-based recovery rules, not observed original linker section
headers. Exact source section identity outside the ARMCC split-section assumption
remains **[INFERENCE]**, especially for handwritten/startup/library code.

## Before / after

| Measure | v1 | v2 candidate | Change |
| --- | ---: | ---: | ---: |
| All starts | 4,504 | 4,443 | -61 |
| ARM starts, including veneers | 4,433 | 4,372 | -61 |
| Thumb starts | 71 | 71 | 0 |
| Function kind, excluding veneers | 4,498 | 4,437 | -61 |
| Veneers | 6 | 6 | 0 |
| Sum of function extents (`size`) | 1,136,366 B | 1,136,402 B | +36 B |
| Explained function-owned bytes | 1,102,870 B | 1,103,010 B | +140 B |
| Decoded instruction bytes | 1,054,490 B | 1,054,474 B | -16 B |
| Referenced pool/table bytes | 48,380 B | 48,536 B | +156 B |
| Decoded instructions | 264,702 | 264,698 | -4 |
| Observed direct BL/BLX sites | 19,991 | 19,991 | 0 |
| Recorded direct tail edges | 1,296 | 1,200 | -96 |
| Switch tables | 142 | 142 | 0 |
| Other indirect exits | 155 | 155 | 0 |
| Unknown text bytes | 39,470 B | 39,338 B | -132 B |
| Unknown ranges | 2,232 | 2,224 | -8 |
| Padding bytes | 1,968 B | 1,960 B | -8 B |

Function extents are **not** the explained byte count: unknown holes stay unknown
in the tiling map even when enclosed by a merged section. The 140-byte gain is
156 more pool/table bytes minus 16 instruction bytes pruned by termination.
Data (51,464 B), rodata (123,064 B) and segment padding (8,268 B) are unchanged;
all range kinds still sum to **1,327,104 B**.

| Anomaly | v1 | v2 candidate | Change |
| --- | ---: | ---: | ---: |
| `fallthrough_into_start` | 115 | 92 | -23 |
| `literal_outside_function` | 139 | 14 | -125 |
| `undecodable_reachable_code` | 1 | 1 | +0 |
| `unexplained_function_bytes` | 95 | 90 | -5 |
| **Total** | **350** | **197** | **-153** |

`validate_catalogue` returns **[]**: map tiling, nonoverlap, aligned pools, instruction
decoding, BL/BLX target starts and ISA, extents, and segment padding all pass.
The original and candidate sets of **(call site, target, ISA)** are identical:
19,991 each, zero lost or added. There are no newly introduced starts, and the
61 vanished v1 addresses equal the 61-entry merge log exactly.

## Merge summary and all disappearing v1 identities

Reason counts are **not disjoint**: a boundary may have multiple independent
witnesses. The exact combinations below sum to 61.

| Reason | Boundaries |
| --- | ---: |
| `branch_into_body` | 38 |
| `section_literal` | 28 |
| `fallthrough_without_return` | 17 |

| Exact reason combination | Boundaries |
| --- | ---: |
| `branch_into_body` | 19 |
| `branch_into_body+section_literal` | 19 |
| `fallthrough_without_return` | 14 |
| `fallthrough_without_return+section_literal` | 3 |
| `section_literal` | 6 |

| Original weak evidence | Removed starts |
| --- | ---: |
| `call_scan` | 2 |
| `pointer` | 10 |
| `pointer+tail_call` | 2 |
| `prologue` | 9 |
| `tail_call` | 38 |

The table is the **complete** set of v1 function addresses that disappear, with
final surviving owners, not intermediate owners. No original BL/BLX-evidenced or
protected callback/vtable target is in this list.

| Disappearing v1 address | Retained address | Old evidence | Reasons |
| --- | --- | --- | --- |
| `0x100D6C` | `0x100D68` | tail_call | fallthrough_without_return |
| `0x10D380` | `0x10D27C` | pointer | fallthrough_without_return |
| `0x10D3B0` | `0x10D27C` | tail_call | branch_into_body |
| `0x10D3DC` | `0x10D27C` | tail_call | branch_into_body, section_literal |
| `0x110A00` | `0x1109FC` | prologue | fallthrough_without_return |
| `0x110A2C` | `0x110A28` | prologue | fallthrough_without_return |
| `0x12D148` | `0x12D104` | pointer | fallthrough_without_return |
| `0x135F90` | `0x135F80` | prologue | section_literal |
| `0x136110` | `0x136104` | prologue | fallthrough_without_return |
| `0x1487E4` | `0x1487CC` | pointer | fallthrough_without_return |
| `0x1487EC` | `0x1487CC` | tail_call | section_literal |
| `0x189880` | `0x187828` | call_scan | branch_into_body, section_literal |
| `0x18988C` | `0x187828` | call_scan | fallthrough_without_return |
| `0x189890` | `0x187828` | tail_call | branch_into_body, section_literal |
| `0x189ABC` | `0x187828` | tail_call | branch_into_body, section_literal |
| `0x189D54` | `0x187828` | tail_call | branch_into_body, section_literal |
| `0x189FE0` | `0x187828` | tail_call | branch_into_body |
| `0x18A440` | `0x187828` | tail_call | branch_into_body, section_literal |
| `0x18A684` | `0x187828` | tail_call | branch_into_body, section_literal |
| `0x18A8D0` | `0x187828` | tail_call | branch_into_body, section_literal |
| `0x18AB28` | `0x187828` | tail_call | branch_into_body, section_literal |
| `0x18AF24` | `0x187828` | tail_call | branch_into_body |
| `0x18B258` | `0x187828` | tail_call | branch_into_body |
| `0x18B55C` | `0x187828` | tail_call | branch_into_body, section_literal |
| `0x18B6B8` | `0x187828` | tail_call | branch_into_body |
| `0x18B7F8` | `0x187828` | tail_call | branch_into_body |
| `0x18B810` | `0x187828` | tail_call | branch_into_body |
| `0x18B904` | `0x187828` | tail_call | branch_into_body |
| `0x18B950` | `0x187828` | tail_call | branch_into_body |
| `0x18BA68` | `0x187828` | tail_call | branch_into_body |
| `0x18BC34` | `0x187828` | tail_call | branch_into_body, section_literal |
| `0x18BDAC` | `0x187828` | tail_call | branch_into_body, section_literal |
| `0x18BEE8` | `0x187828` | tail_call | branch_into_body |
| `0x18C190` | `0x187828` | tail_call | branch_into_body |
| `0x18ED18` | `0x18EC7C` | prologue | branch_into_body |
| `0x18EDB4` | `0x18EC7C` | prologue | branch_into_body |
| `0x1A8024` | `0x1A7B54` | pointer | section_literal |
| `0x1A8038` | `0x1A7B54` | pointer | fallthrough_without_return |
| `0x1B87E0` | `0x1B8424` | pointer | branch_into_body |
| `0x1B87EC` | `0x1B8424` | tail_call | branch_into_body |
| `0x1B8808` | `0x1B8424` | tail_call | branch_into_body |
| `0x1B889C` | `0x1B8424` | tail_call | section_literal |
| `0x1B88BC` | `0x1B8424` | tail_call | section_literal |
| `0x1C3108` | `0x1C3100` | prologue | fallthrough_without_return |
| `0x1CC66C` | `0x1CC530` | pointer+tail_call | fallthrough_without_return |
| `0x1E0084` | `0x1DFE88` | pointer | branch_into_body, section_literal |
| `0x1E00A0` | `0x1DFE88` | tail_call | branch_into_body, section_literal |
| `0x1E0114` | `0x1DFE88` | tail_call | branch_into_body, section_literal |
| `0x1E02A0` | `0x1DFE88` | tail_call | branch_into_body |
| `0x1E0564` | `0x1DFE88` | tail_call | branch_into_body |
| `0x1E05C0` | `0x1DFE88` | tail_call | branch_into_body, section_literal |
| `0x1E0610` | `0x1DFE88` | tail_call | branch_into_body, section_literal |
| `0x1E0690` | `0x1DFE88` | tail_call | branch_into_body, section_literal |
| `0x1E0740` | `0x1DFE88` | tail_call | branch_into_body, section_literal |
| `0x1E0968` | `0x1DFE88` | tail_call | section_literal |
| `0x1EF820` | `0x1EF818` | prologue | fallthrough_without_return |
| `0x1EF884` | `0x1EF880` | pointer | fallthrough_without_return |
| `0x1EF970` | `0x1EF960` | prologue | fallthrough_without_return, section_literal |
| `0x1F0010` | `0x1F0008` | pointer | fallthrough_without_return |
| `0x202BF8` | `0x1F7D7C` | pointer+tail_call | fallthrough_without_return, section_literal |
| `0x210004` | `0x20FA08` | pointer | fallthrough_without_return, section_literal |

### Supplied pilot identities

All ten addresses remain present with **identical address, size, code_size, ISA,
kind, pools and code_ranges** (not merely unchanged entry addresses):

```text
0x10E664 0x1149A8 0x136930 0x140DAC 0x1179B4 0x1263B8 0x122350 0x103798 0x1091D4 0x1103D4
```

The integrator must still compare the complete disappearing list with the current
accepted ledger before adopting a denominator revision. No acceptance or progress
record was changed by this work.

## Ten address-only spot checks

These are author checks against decoded original-image metadata, **not independent
review** or function naming. For every recorded witness in each sampled merge,
the decoded reference/branch/continuation was checked; pool witnesses are owned by
the retained function. All checks passed. Only addresses and generic relationships
are shown, with no original bytes or disassembly.

| Removed start | Final owner | Rechecked address relationship (one witness per reason) |
| --- | --- | --- |
| `0x100D6C` | `0x100D68` | unterminated continuation: `0x100D68` → `0x100D6C` |
| `0x10D380` | `0x10D27C` | unterminated continuation: `0x10D37C` → `0x10D380` |
| `0x10D3DC` | `0x10D27C` | branch into body: `0x10D404` → `0x10D34C`; pool reference: `0x10D2CC` → `0x10D408` |
| `0x110A00` | `0x1109FC` | unterminated continuation: `0x1109FC` → `0x110A00` |
| `0x135F90` | `0x135F80` | pool reference: `0x135F80` → `0x13602C` |
| `0x189880` | `0x187828` | branch into body: `0x189A34` → `0x187D08`; pool reference: `0x1898C8` → `0x189568` |
| `0x18ED18` | `0x18EC7C` | branch into body: `0x18ED08` → `0x18EE40` |
| `0x1A8024` | `0x1A7B54` | pool reference: `0x1A7BE4` → `0x1A8044` |
| `0x1E00A0` | `0x1DFE88` | branch into body: `0x1E0168` → `0x1DFF88`; pool reference: `0x1DFE98` → `0x1E0190` |
| `0x210004` | `0x20FA08` | unterminated continuation: `0x210000` → `0x210004`; pool reference: `0x20FFBC` → `0x210068` |

## Noreturn and remaining fallthroughs

**Five** recovered functions have `noreturn=true`:

| Address | Recovered normal-flow reasons |
| --- | --- |
| `0x100000` | no_return_sites, noreturn_call |
| `0x100D68` | no_return_sites, svc_exit_process |
| `0x100D74` | no_return_sites, noreturn_tail |
| `0x10D27C` | no_return_sites, svc_exit_process |
| `0x113CA8` | closed_loop, no_return_sites |

An unconditional direct BL/BLX to one of these stops tracing at that call;
a conditional call does not. The five-function conclusion is qualified by the
recovered graph, recognized return forms and normal 3DS ExitProcess semantics;
it is not a dynamic execution/exception/unwinding proof.

Of the original 115 fallthrough boundaries, **23 disappeared through weak-entry
merges; 92 remain**, with no new fallthrough address. The 92 targets consist of
**82 with BL/BLX evidence** and 10 weaker protected/unresolved entries (six
pointer-only, three tail-only, one prologue+tail). None of the 82 observed-call
starts was erased to make these anomalies disappear.

Only **one** remaining boundary is immediately preceded by a direct call:
`0x121410` calls `0x1A0094`, then falls through to protected start `0x121414`.
`0x1A0094` has no local recognized return, but tails to `0x19F568`, which tails to
`0x134E7C`, whose recognized return is at **`0x134E80`**. Therefore zero local
return sites does **not** justify marking that callee noreturn. The other 91
remaining boundaries are not immediately preceded by a decoded direct BL/BLX.
They remain honest boundary/shared-entry/decoder ambiguities, not suppressed errors.

Complete residual fallthrough target addresses:

```text
0x121414 0x12D0B8 0x13685C 0x141C74 0x143350 0x148220 0x14E218 0x1526E0
0x15E60C 0x166F20 0x169878 0x16992C 0x16B260 0x16B5FC 0x16DBC4 0x16E1D0
0x16E5DC 0x16E648 0x16E7E0 0x16F208 0x1702FC 0x170DC0 0x170E58 0x171A34
0x173D8C 0x1780C4 0x178E98 0x179A48 0x17A558 0x17A71C 0x17A9B4 0x17E35C
0x17FE98 0x180388 0x180768 0x180884 0x180AE0 0x181100 0x181328 0x181448
0x181734 0x181E44 0x18271C 0x182994 0x183034 0x183724 0x183900 0x183F18
0x1852F8 0x1856D8 0x185BA0 0x186488 0x186884 0x186FC8 0x18D868 0x18EF04
0x18F6C4 0x19063C 0x191154 0x19124C 0x191458 0x191AEC 0x19242C 0x1944F0
0x194A34 0x1951DC 0x195AD0 0x195D74 0x195E64 0x195EC4 0x1963F8 0x197220
0x19C9D4 0x19D798 0x19DC20 0x19EA6C 0x19EB4C 0x19EC88 0x19EDCC 0x19F2E8
0x19F568 0x19F9A0 0x19F9AC 0x19FFA8 0x1C3AC0 0x1CF448 0x1D54B0 0x1DAC84
0x1DACC0 0x1DAD48 0x1DADC4 0x1DB134
```

The 14 residual `literal_outside_function` records consist of eight address-taking
references (not pool loads), plus six actual Thumb pool-load references. The six
loads are `0x100532`, `0x100538`, `0x19F958`, `0x19F95A`, `0x19F9CE`, `0x19F9D8`.
Their required spans cross protected `veneer_target` entries at `0x100580`,
`0x10059C` or `0x19FAA8`; these were not merged away. The undecodable reachable
site at `0x1964E8` also remains unresolved; it was not hidden by a blanket
zero-return-site heuristic.

## Unknown holes after merging

| Unknown category | v1 | v2 candidate |
| --- | ---: | ---: |
| Inside candidate extents | 31,648 B / 2,082 ranges / 92 functions | 31,548 B / 2,078 ranges / 87 functions |
| Between candidate extents | 7,822 B / 150 ranges | 7,790 B / 146 ranges |
| After the nearest preceding function's last recognized return | 10,860 B / 418 ranges | 10,756 B / 411 ranges |
| Same, with no recovered reference into the hole | **10,316 B / 393 ranges** | **10,316 B / 393 ranges** |

The last row was checked against recovered PC-relative references, direct branch/
call targets, and pointer-sized words in rodata/data/known text pools. It is not a
claim that every unknown word or hidden indirect edge was understood. Those
**10,316 bytes are still unknown**, unchanged. Dead code, unreferenced literal
arrays or alignment are possible **[INFERENCE]**, not new classifications. The
existing zero-word/architectural-NOP padding rule was not broadened.

There are 90 `unexplained_function_bytes` anomalies: 87 extents contain unknown
holes and three contain only padding holes. Ten largest unknown ranges remain:

| Address | Bytes | Position |
| --- | ---: | --- |
| `0x18E270` | 1,268 | between extents |
| `0x18433C` | 956 | between extents |
| `0x110CF0` | 440 | between extents |
| `0x210824` | 320 | between extents |
| `0x20E28C` | 292 | between extents |
| `0x18EAEC` | 268 | between extents |
| `0x214EF4` | 248 | between extents |
| `0x215950` | 248 | between extents |
| `0x14B358` | 224 | between extents |
| `0x1F58BC` | 212 | between extents |

## Exercised acceptance and evidence

Commands ran sequentially, with only `tests.test_analyze` as the test module.
The final module run passed **49 tests**, including the original-image repeatability
and hard-invariant test. Added synthetic coverage includes mid-function pools,
multi-boundary constraints, prologue/isolated-pointer internal labels, protecting
BL/BLX/table/external-tail targets, branch-body/continuation merges, ARM/Thumb
ExitProcess, infinite and tail cycles, transitive noreturn calls, conditional exits,
incomplete/indirect exits, unchanged unknowns, and discovery-before-pruning.

| Exact command | PYTHONHASHSEED | Exit | Wall time | Output path |
| --- | ---: | ---: | ---: | --- |
| `.venv/bin/python -m unittest tests.test_analyze -v` | 17 | 0 | 111.254 s | This report, command transcript below |
| `.venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-v2.json` | 23 | 0 | 35.516 s | `scratch/analyze/catalogue-v2.json` |
| `.venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-v2.json` | 71 | 0 | 33.694 s | `scratch/analyze/catalogue-v2.json` |

The two final CLI runs wrote the same assigned output path; the first byte string
was retained in memory before the second run and compared to the second. They are
**byte-identical across hash seeds 23 and 71**, SHA-256
`65d1c56942a759901f27311e8fe722ed2f67c0a5b09b1f2e69f58225d3374e89`. CLI timers were 35.216 s and 33.440 s;
wall times above are observations on the shared laptop, not a performance guarantee.

Development failures are retained in the command transcript below, not represented
as passing runs: the first 47-test run had two synthetic-fixture assumptions wrong
(interior pointers are not automatically entry seeds); the next 48-test run exposed
an intentionally protected external tail target in the revised fixture. The final
branch-body fixture uses an actual gap prologue, and the noreturn boundary fixture
has a real external call witness. The existing protection was not weakened to make
them pass. A development CLI additionally exposed four starts disappearing during
eager ExitProcess discovery; pruning was moved after discovery and a dedicated
entry/table-retention regression added. The final 61-address delta matches its log.

### Final artifact hashes

| Path | SHA-256 |
| --- | --- |
| `tools/analyze.py` | `4c384e9f2dc272470224f4b48c33d2a17cd8b2fbc348cf2b075a45659bb27316` |
| `tests/test_analyze.py` | `1b116f652851c0eaebbeef050c76ff76f7bb8e30ec78fd647332698ccfa56dcb` |
| `config/catalogue.json` | `06027f34eb7e89666c138293a31393cbafc90d9c66862add8f3d9df571aedf13` |
| `docs/denominator-history.jsonl` | `d0506c3dcf03b8cce31b13b7bb449a3826b1a461df0daa8d2ccd8aa761c3afde` |
| `scratch/analyze/catalogue-v2.json` | `65d1c56942a759901f27311e8fe722ed2f67c0a5b09b1f2e69f58225d3374e89` |

### Command transcript (including failed development runs)

Output from unit tests is persisted here rather than in an extra generated log.
The early development CLI output is not the delivered candidate; only the final
byte-identical pair and hashes above describe `catalogue-v2.json` now.

```json
[
  {
    "command": ".venv/bin/python -m unittest tests.test_analyze -v",
    "hash_seed": 17,
    "exit_code": 1,
    "seconds": 31.031,
    "stdout": "",
    "stderr": "test_adr_literal_pool (tests.test_analyze.CatalogueTests.test_adr_literal_pool) ... ok\ntest_arm_address_switch_table (tests.test_analyze.CatalogueTests.test_arm_address_switch_table) ... ok\ntest_arm_branch_switch_table (tests.test_analyze.CatalogueTests.test_arm_branch_switch_table) ... ok\ntest_arm_literal_veneer (tests.test_analyze.CatalogueTests.test_arm_literal_veneer) ... ok\ntest_arm_lo_switch_has_exclusive_bound (tests.test_analyze.CatalogueTests.test_arm_lo_switch_has_exclusive_bound) ... ok\ntest_bad_pool_alignment_is_detected (tests.test_analyze.CatalogueTests.test_bad_pool_alignment_is_detected) ... ok\ntest_bl_to_following_function (tests.test_analyze.CatalogueTests.test_bl_to_following_function) ... ok\ntest_branch_into_body_merges_weak_label (tests.test_analyze.CatalogueTests.test_branch_into_body_merges_weak_label) ... ERROR\ntest_branch_table_without_nearby_compare (tests.test_analyze.CatalogueTests.test_branch_table_without_nearby_compare) ... ok\ntest_call_to_bad_target_is_anomaly (tests.test_analyze.CatalogueTests.test_call_to_bad_target_is_anomaly) ... ok\ntest_call_to_infinite_loop_terminates_fallthrough (tests.test_analyze.CatalogueTests.test_call_to_infinite_loop_terminates_fallthrough) ... ERROR\ntest_closed_tail_cycle_is_noreturn (tests.test_analyze.CatalogueTests.test_closed_tail_cycle_is_noreturn) ... ok\ntest_conditional_noreturn_call_keeps_returning_path (tests.test_analyze.CatalogueTests.test_conditional_noreturn_call_keeps_returning_path) ... ok\ntest_cross_isa_pool_does_not_merge_blx_target (tests.test_analyze.CatalogueTests.test_cross_isa_pool_does_not_merge_blx_target) ... ok\ntest_external_tail_call_protects_weak_start (tests.test_analyze.CatalogueTests.test_external_tail_call_protects_weak_start) ... ok\ntest_incomplete_callee_is_not_noreturn (tests.test_analyze.CatalogueTests.test_incomplete_callee_is_not_noreturn) ... ok\ntest_internal_unknown_hole_is_explicit (tests.test_analyze.CatalogueTests.test_internal_unknown_hole_is_explicit) ... ok\ntest_isolated_pointer_can_name_an_internal_label (tests.test_analyze.CatalogueTests.test_isolated_pointer_can_name_an_internal_label) ... ok\ntest_leaf_and_padding (tests.test_analyze.CatalogueTests.test_leaf_and_padding) ... ok\ntest_literal_constraint_merges_multiple_weak_boundaries (tests.test_analyze.CatalogueTests.test_literal_constraint_merges_multiple_weak_boundaries) ... ok\ntest_map_gap_is_detected (tests.test_analyze.CatalogueTests.test_map_gap_is_detected) ... ok\ntest_mid_function_pool_does_not_merge_bl_target (tests.test_analyze.CatalogueTests.test_mid_function_pool_does_not_merge_bl_target) ... ok\ntest_mid_function_pool_merges_prologue_only_start (tests.test_analyze.CatalogueTests.test_mid_function_pool_merges_prologue_only_start) ... ok\ntest_mid_function_pool_merges_tail_only_start (tests.test_analyze.CatalogueTests.test_mid_function_pool_merges_tail_only_start) ... ok\ntest_nonzero_page_padding_is_anomaly (tests.test_analyze.CatalogueTests.test_nonzero_page_padding_is_anomaly) ... ok\ntest_noreturn_call_does_not_absorb_unreferenced_return_code (tests.test_analyze.CatalogueTests.test_noreturn_call_does_not_absorb_unreferenced_return_code) ... ok\ntest_noreturn_propagates_through_calls_to_exit_process (tests.test_analyze.CatalogueTests.test_noreturn_propagates_through_calls_to_exit_process) ... ok\ntest_original_invariants_and_stability (tests.test_analyze.CatalogueTests.test_original_invariants_and_stability) ... ok\ntest_other_or_conditional_svc_can_return (tests.test_analyze.CatalogueTests.test_other_or_conditional_svc_can_return) ... ok\ntest_pointer_function_and_prologue_gap (tests.test_analyze.CatalogueTests.test_pointer_function_and_prologue_gap) ... ok\ntest_pointer_tables_protect_weak_starts (tests.test_analyze.CatalogueTests.test_pointer_tables_protect_weak_starts) ... ok\ntest_return_forms_and_prefetch (tests.test_analyze.CatalogueTests.test_return_forms_and_prefetch) ... ok\ntest_switch_labels_are_not_callback_table_entries (tests.test_analyze.CatalogueTests.test_switch_labels_are_not_callback_table_entries) ... ok\ntest_tail_call_after_setup_and_padding (tests.test_analyze.CatalogueTests.test_tail_call_after_setup_and_padding) ... ok\ntest_tail_call_after_single_lr_restore (tests.test_analyze.CatalogueTests.test_tail_call_after_single_lr_restore) ... ok\ntest_tail_call_branch (tests.test_analyze.CatalogueTests.test_tail_call_branch) ... ok\ntest_thumb_bl_target (tests.test_analyze.CatalogueTests.test_thumb_bl_target) ... ok\ntest_thumb_blx_to_arm_target (tests.test_analyze.CatalogueTests.test_thumb_blx_to_arm_target) ... ok\ntest_thumb_exit_process_is_noreturn (tests.test_analyze.CatalogueTests.test_thumb_exit_process_is_noreturn) ... ok\ntest_thumb_function_reached_by_blx (tests.test_analyze.CatalogueTests.test_thumb_function_reached_by_blx) ... ok\ntest_thumb_interworking_veneer (tests.test_analyze.CatalogueTests.test_thumb_interworking_veneer) ... ok\ntest_thumb_literal_and_adr_pools (tests.test_analyze.CatalogueTests.test_thumb_literal_and_adr_pools) ... ok\ntest_thumb_prologue_in_gap (tests.test_analyze.CatalogueTests.test_thumb_prologue_in_gap) ... ok\ntest_trailing_literal_pool (tests.test_analyze.CatalogueTests.test_trailing_literal_pool) ... ok\ntest_unknown_bytes_are_not_absorbed (tests.test_analyze.CatalogueTests.test_unknown_bytes_are_not_absorbed) ... ok\ntest_unknown_indirect_exit_is_not_noreturn (tests.test_analyze.CatalogueTests.test_unknown_indirect_exit_is_not_noreturn) ... ok\ntest_vfp_double_literal_pool (tests.test_analyze.CatalogueTests.test_vfp_double_literal_pool) ... ok\n\n======================================================================\nERROR: test_branch_into_body_merges_weak_label (tests.test_analyze.CatalogueTests.test_branch_into_body_merges_weak_label)\n----------------------------------------------------------------------\nTraceback (most recent call last):\n  File \"/Users/mooki/Code/kanken-training-2-decomp/tests/test_analyze.py\", line 174, in test_branch_into_body_merges_weak_label\n    self.assertEqual(result[\"boundary_merges\"][0][\"reasons\"], [\"branch_into_body\"])\n                     ~~~~~~~~~~~~~~~~~~~~~~~~~^^^\nIndexError: list index out of range\n\n======================================================================\nERROR: test_call_to_infinite_loop_terminates_fallthrough (tests.test_analyze.CatalogueTests.test_call_to_infinite_loop_terminates_fallthrough)\n----------------------------------------------------------------------\nTraceback (most recent call last):\n  File \"/Users/mooki/Code/kanken-training-2-decomp/tests/test_analyze.py\", line 308, in test_call_to_infinite_loop_terminates_fallthrough\n    self.assertFalse(functions[BASE + 4][\"noreturn\"])\n                     ~~~~~~~~~^^^^^^^^^^\nKeyError: 1048580\n\n----------------------------------------------------------------------\nRan 47 tests in 30.918s\n\nFAILED (errors=2)\n"
  },
  {
    "command": ".venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-v2.json",
    "hash_seed": 23,
    "exit_code": 0,
    "seconds": 18.4,
    "stdout": "4443 functions; 39338 unknown bytes; 197 anomalies; 18.278s; scratch/analyze/catalogue-v2.json\n",
    "stderr": ""
  },
  {
    "command": ".venv/bin/python -m unittest tests.test_analyze -v",
    "hash_seed": 17,
    "exit_code": 1,
    "seconds": 52.189,
    "stdout": "",
    "stderr": "test_adr_literal_pool (tests.test_analyze.CatalogueTests.test_adr_literal_pool) ... ok\ntest_arm_address_switch_table (tests.test_analyze.CatalogueTests.test_arm_address_switch_table) ... ok\ntest_arm_branch_switch_table (tests.test_analyze.CatalogueTests.test_arm_branch_switch_table) ... ok\ntest_arm_literal_veneer (tests.test_analyze.CatalogueTests.test_arm_literal_veneer) ... ok\ntest_arm_lo_switch_has_exclusive_bound (tests.test_analyze.CatalogueTests.test_arm_lo_switch_has_exclusive_bound) ... ok\ntest_bad_pool_alignment_is_detected (tests.test_analyze.CatalogueTests.test_bad_pool_alignment_is_detected) ... ok\ntest_bl_to_following_function (tests.test_analyze.CatalogueTests.test_bl_to_following_function) ... ok\ntest_branch_into_body_merges_weak_label (tests.test_analyze.CatalogueTests.test_branch_into_body_merges_weak_label) ... FAIL\ntest_branch_table_without_nearby_compare (tests.test_analyze.CatalogueTests.test_branch_table_without_nearby_compare) ... ok\ntest_call_to_bad_target_is_anomaly (tests.test_analyze.CatalogueTests.test_call_to_bad_target_is_anomaly) ... ok\ntest_call_to_infinite_loop_terminates_fallthrough (tests.test_analyze.CatalogueTests.test_call_to_infinite_loop_terminates_fallthrough) ... ok\ntest_closed_tail_cycle_is_noreturn (tests.test_analyze.CatalogueTests.test_closed_tail_cycle_is_noreturn) ... ok\ntest_conditional_noreturn_call_keeps_returning_path (tests.test_analyze.CatalogueTests.test_conditional_noreturn_call_keeps_returning_path) ... ok\ntest_cross_isa_pool_does_not_merge_blx_target (tests.test_analyze.CatalogueTests.test_cross_isa_pool_does_not_merge_blx_target) ... ok\ntest_discovery_and_table_protection_precede_exit_pruning (tests.test_analyze.CatalogueTests.test_discovery_and_table_protection_precede_exit_pruning) ... ok\ntest_external_tail_call_protects_weak_start (tests.test_analyze.CatalogueTests.test_external_tail_call_protects_weak_start) ... ok\ntest_incomplete_callee_is_not_noreturn (tests.test_analyze.CatalogueTests.test_incomplete_callee_is_not_noreturn) ... ok\ntest_internal_unknown_hole_is_explicit (tests.test_analyze.CatalogueTests.test_internal_unknown_hole_is_explicit) ... ok\ntest_isolated_pointer_can_name_an_internal_label (tests.test_analyze.CatalogueTests.test_isolated_pointer_can_name_an_internal_label) ... ok\ntest_leaf_and_padding (tests.test_analyze.CatalogueTests.test_leaf_and_padding) ... ok\ntest_literal_constraint_merges_multiple_weak_boundaries (tests.test_analyze.CatalogueTests.test_literal_constraint_merges_multiple_weak_boundaries) ... ok\ntest_map_gap_is_detected (tests.test_analyze.CatalogueTests.test_map_gap_is_detected) ... ok\ntest_mid_function_pool_does_not_merge_bl_target (tests.test_analyze.CatalogueTests.test_mid_function_pool_does_not_merge_bl_target) ... ok\ntest_mid_function_pool_merges_prologue_only_start (tests.test_analyze.CatalogueTests.test_mid_function_pool_merges_prologue_only_start) ... ok\ntest_mid_function_pool_merges_tail_only_start (tests.test_analyze.CatalogueTests.test_mid_function_pool_merges_tail_only_start) ... ok\ntest_nonzero_page_padding_is_anomaly (tests.test_analyze.CatalogueTests.test_nonzero_page_padding_is_anomaly) ... ok\ntest_noreturn_call_does_not_absorb_unreferenced_return_code (tests.test_analyze.CatalogueTests.test_noreturn_call_does_not_absorb_unreferenced_return_code) ... ok\ntest_noreturn_propagates_through_calls_to_exit_process (tests.test_analyze.CatalogueTests.test_noreturn_propagates_through_calls_to_exit_process) ... ok\ntest_original_invariants_and_stability (tests.test_analyze.CatalogueTests.test_original_invariants_and_stability) ... ok\ntest_other_or_conditional_svc_can_return (tests.test_analyze.CatalogueTests.test_other_or_conditional_svc_can_return) ... ok\ntest_pointer_function_and_prologue_gap (tests.test_analyze.CatalogueTests.test_pointer_function_and_prologue_gap) ... ok\ntest_pointer_tables_protect_weak_starts (tests.test_analyze.CatalogueTests.test_pointer_tables_protect_weak_starts) ... ok\ntest_return_forms_and_prefetch (tests.test_analyze.CatalogueTests.test_return_forms_and_prefetch) ... ok\ntest_switch_labels_are_not_callback_table_entries (tests.test_analyze.CatalogueTests.test_switch_labels_are_not_callback_table_entries) ... ok\ntest_tail_call_after_setup_and_padding (tests.test_analyze.CatalogueTests.test_tail_call_after_setup_and_padding) ... ok\ntest_tail_call_after_single_lr_restore (tests.test_analyze.CatalogueTests.test_tail_call_after_single_lr_restore) ... ok\ntest_tail_call_branch (tests.test_analyze.CatalogueTests.test_tail_call_branch) ... ok\ntest_thumb_bl_target (tests.test_analyze.CatalogueTests.test_thumb_bl_target) ... ok\ntest_thumb_blx_to_arm_target (tests.test_analyze.CatalogueTests.test_thumb_blx_to_arm_target) ... ok\ntest_thumb_exit_process_is_noreturn (tests.test_analyze.CatalogueTests.test_thumb_exit_process_is_noreturn) ... ok\ntest_thumb_function_reached_by_blx (tests.test_analyze.CatalogueTests.test_thumb_function_reached_by_blx) ... ok\ntest_thumb_interworking_veneer (tests.test_analyze.CatalogueTests.test_thumb_interworking_veneer) ... ok\ntest_thumb_literal_and_adr_pools (tests.test_analyze.CatalogueTests.test_thumb_literal_and_adr_pools) ... ok\ntest_thumb_prologue_in_gap (tests.test_analyze.CatalogueTests.test_thumb_prologue_in_gap) ... ok\ntest_trailing_literal_pool (tests.test_analyze.CatalogueTests.test_trailing_literal_pool) ... ok\ntest_unknown_bytes_are_not_absorbed (tests.test_analyze.CatalogueTests.test_unknown_bytes_are_not_absorbed) ... ok\ntest_unknown_indirect_exit_is_not_noreturn (tests.test_analyze.CatalogueTests.test_unknown_indirect_exit_is_not_noreturn) ... ok\ntest_vfp_double_literal_pool (tests.test_analyze.CatalogueTests.test_vfp_double_literal_pool) ... ok\n\n======================================================================\nFAIL: test_branch_into_body_merges_weak_label (tests.test_analyze.CatalogueTests.test_branch_into_body_merges_weak_label)\n----------------------------------------------------------------------\nTraceback (most recent call last):\n  File \"/Users/mooki/Code/kanken-training-2-decomp/tests/test_analyze.py\", line 173, in test_branch_into_body_merges_weak_label\n    self.assertEqual(len(result[\"functions\"]), 1)\nAssertionError: 3 != 1\n\n----------------------------------------------------------------------\nRan 48 tests in 52.071s\n\nFAILED (failures=1)\n"
  },
  {
    "command": ".venv/bin/python -m unittest tests.test_analyze.CatalogueTests.test_branch_into_body_merges_weak_label tests.test_analyze.CatalogueTests.test_fallthrough_without_return_merges_prologue -v",
    "hash_seed": 17,
    "exit_code": 0,
    "seconds": 0.427,
    "stdout": "",
    "stderr": "test_branch_into_body_merges_weak_label (tests.test_analyze.CatalogueTests.test_branch_into_body_merges_weak_label) ... ok\ntest_fallthrough_without_return_merges_prologue (tests.test_analyze.CatalogueTests.test_fallthrough_without_return_merges_prologue) ... ok\n\n----------------------------------------------------------------------\nRan 2 tests in 0.002s\n\nOK\n"
  },
  {
    "command": ".venv/bin/python -m unittest tests.test_analyze -v",
    "hash_seed": 17,
    "exit_code": 0,
    "seconds": 111.254,
    "stdout": "",
    "stderr": "test_adr_literal_pool (tests.test_analyze.CatalogueTests.test_adr_literal_pool) ... ok\ntest_arm_address_switch_table (tests.test_analyze.CatalogueTests.test_arm_address_switch_table) ... ok\ntest_arm_branch_switch_table (tests.test_analyze.CatalogueTests.test_arm_branch_switch_table) ... ok\ntest_arm_literal_veneer (tests.test_analyze.CatalogueTests.test_arm_literal_veneer) ... ok\ntest_arm_lo_switch_has_exclusive_bound (tests.test_analyze.CatalogueTests.test_arm_lo_switch_has_exclusive_bound) ... ok\ntest_bad_pool_alignment_is_detected (tests.test_analyze.CatalogueTests.test_bad_pool_alignment_is_detected) ... ok\ntest_bl_to_following_function (tests.test_analyze.CatalogueTests.test_bl_to_following_function) ... ok\ntest_branch_into_body_merges_weak_label (tests.test_analyze.CatalogueTests.test_branch_into_body_merges_weak_label) ... ok\ntest_branch_table_without_nearby_compare (tests.test_analyze.CatalogueTests.test_branch_table_without_nearby_compare) ... ok\ntest_call_to_bad_target_is_anomaly (tests.test_analyze.CatalogueTests.test_call_to_bad_target_is_anomaly) ... ok\ntest_call_to_infinite_loop_terminates_fallthrough (tests.test_analyze.CatalogueTests.test_call_to_infinite_loop_terminates_fallthrough) ... ok\ntest_closed_tail_cycle_is_noreturn (tests.test_analyze.CatalogueTests.test_closed_tail_cycle_is_noreturn) ... ok\ntest_conditional_noreturn_call_keeps_returning_path (tests.test_analyze.CatalogueTests.test_conditional_noreturn_call_keeps_returning_path) ... ok\ntest_cross_isa_pool_does_not_merge_blx_target (tests.test_analyze.CatalogueTests.test_cross_isa_pool_does_not_merge_blx_target) ... ok\ntest_discovery_and_table_protection_precede_exit_pruning (tests.test_analyze.CatalogueTests.test_discovery_and_table_protection_precede_exit_pruning) ... ok\ntest_external_tail_call_protects_weak_start (tests.test_analyze.CatalogueTests.test_external_tail_call_protects_weak_start) ... ok\ntest_fallthrough_without_return_merges_prologue (tests.test_analyze.CatalogueTests.test_fallthrough_without_return_merges_prologue) ... ok\ntest_incomplete_callee_is_not_noreturn (tests.test_analyze.CatalogueTests.test_incomplete_callee_is_not_noreturn) ... ok\ntest_internal_unknown_hole_is_explicit (tests.test_analyze.CatalogueTests.test_internal_unknown_hole_is_explicit) ... ok\ntest_isolated_pointer_can_name_an_internal_label (tests.test_analyze.CatalogueTests.test_isolated_pointer_can_name_an_internal_label) ... ok\ntest_leaf_and_padding (tests.test_analyze.CatalogueTests.test_leaf_and_padding) ... ok\ntest_literal_constraint_merges_multiple_weak_boundaries (tests.test_analyze.CatalogueTests.test_literal_constraint_merges_multiple_weak_boundaries) ... ok\ntest_map_gap_is_detected (tests.test_analyze.CatalogueTests.test_map_gap_is_detected) ... ok\ntest_mid_function_pool_does_not_merge_bl_target (tests.test_analyze.CatalogueTests.test_mid_function_pool_does_not_merge_bl_target) ... ok\ntest_mid_function_pool_merges_prologue_only_start (tests.test_analyze.CatalogueTests.test_mid_function_pool_merges_prologue_only_start) ... ok\ntest_mid_function_pool_merges_tail_only_start (tests.test_analyze.CatalogueTests.test_mid_function_pool_merges_tail_only_start) ... ok\ntest_nonzero_page_padding_is_anomaly (tests.test_analyze.CatalogueTests.test_nonzero_page_padding_is_anomaly) ... ok\ntest_noreturn_call_does_not_absorb_unreferenced_return_code (tests.test_analyze.CatalogueTests.test_noreturn_call_does_not_absorb_unreferenced_return_code) ... ok\ntest_noreturn_propagates_through_calls_to_exit_process (tests.test_analyze.CatalogueTests.test_noreturn_propagates_through_calls_to_exit_process) ... ok\ntest_original_invariants_and_stability (tests.test_analyze.CatalogueTests.test_original_invariants_and_stability) ... ok\ntest_other_or_conditional_svc_can_return (tests.test_analyze.CatalogueTests.test_other_or_conditional_svc_can_return) ... ok\ntest_pointer_function_and_prologue_gap (tests.test_analyze.CatalogueTests.test_pointer_function_and_prologue_gap) ... ok\ntest_pointer_tables_protect_weak_starts (tests.test_analyze.CatalogueTests.test_pointer_tables_protect_weak_starts) ... ok\ntest_return_forms_and_prefetch (tests.test_analyze.CatalogueTests.test_return_forms_and_prefetch) ... ok\ntest_switch_labels_are_not_callback_table_entries (tests.test_analyze.CatalogueTests.test_switch_labels_are_not_callback_table_entries) ... ok\ntest_tail_call_after_setup_and_padding (tests.test_analyze.CatalogueTests.test_tail_call_after_setup_and_padding) ... ok\ntest_tail_call_after_single_lr_restore (tests.test_analyze.CatalogueTests.test_tail_call_after_single_lr_restore) ... ok\ntest_tail_call_branch (tests.test_analyze.CatalogueTests.test_tail_call_branch) ... ok\ntest_thumb_bl_target (tests.test_analyze.CatalogueTests.test_thumb_bl_target) ... ok\ntest_thumb_blx_to_arm_target (tests.test_analyze.CatalogueTests.test_thumb_blx_to_arm_target) ... ok\ntest_thumb_exit_process_is_noreturn (tests.test_analyze.CatalogueTests.test_thumb_exit_process_is_noreturn) ... ok\ntest_thumb_function_reached_by_blx (tests.test_analyze.CatalogueTests.test_thumb_function_reached_by_blx) ... ok\ntest_thumb_interworking_veneer (tests.test_analyze.CatalogueTests.test_thumb_interworking_veneer) ... ok\ntest_thumb_literal_and_adr_pools (tests.test_analyze.CatalogueTests.test_thumb_literal_and_adr_pools) ... ok\ntest_thumb_prologue_in_gap (tests.test_analyze.CatalogueTests.test_thumb_prologue_in_gap) ... ok\ntest_trailing_literal_pool (tests.test_analyze.CatalogueTests.test_trailing_literal_pool) ... ok\ntest_unknown_bytes_are_not_absorbed (tests.test_analyze.CatalogueTests.test_unknown_bytes_are_not_absorbed) ... ok\ntest_unknown_indirect_exit_is_not_noreturn (tests.test_analyze.CatalogueTests.test_unknown_indirect_exit_is_not_noreturn) ... ok\ntest_vfp_double_literal_pool (tests.test_analyze.CatalogueTests.test_vfp_double_literal_pool) ... ok\n\n----------------------------------------------------------------------\nRan 49 tests in 110.774s\n\nOK\n"
  },
  {
    "command": ".venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-v2.json",
    "hash_seed": 23,
    "exit_code": 0,
    "seconds": 35.516,
    "stdout": "4443 functions; 39338 unknown bytes; 197 anomalies; 35.216s; scratch/analyze/catalogue-v2.json\n",
    "stderr": ""
  },
  {
    "command": ".venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-v2.json",
    "hash_seed": 71,
    "exit_code": 0,
    "seconds": 33.694,
    "stdout": "4443 functions; 39338 unknown bytes; 197 anomalies; 33.440s; scratch/analyze/catalogue-v2.json\n",
    "stderr": ""
  }
]
```

### Exact metadata-diagnostic argv and exits

Each `-c` argument below is the exact source supplied to the project interpreter
via `subprocess.run` (no shell). Output consisted only of metadata, counts and
addresses and is summarized in the preceding sections. No original instruction
text or image byte values were emitted. All five diagnostic commands exited 0.
The first diagnostic also returned zero hard violations and checked every witness
of each of the ten sampled merges. No catalogue regeneration was needed for these
metadata checks.

```json
[
  {
    "argv": [
      ".venv/bin/python",
      "-c",
      "from tools import analyze, ctrfmt\nfrom pathlib import Path\nfrom bisect import bisect_right\nfrom collections import Counter\nimport hashlib, json, struct\nold=json.loads(Path('config/catalogue.json').read_text()); new=json.loads(Path('scratch/analyze/catalogue-v2.json').read_text())\na=analyze.Analyzer(Path('orig/code.bin').read_bytes(),ctrfmt.parse_exheader(Path('orig/exheader.bin').read_bytes()))\ndef instructions(catalogue):\n result=[]\n for f in catalogue['functions']:\n  for lo,hi in f['code_ranges']:\n   while lo<hi:\n    isa='arm' if f['kind']=='veneer' and f['isa']=='thumb' and lo==f['address']+4 else f['isa']\n    ins=a.decode(lo,isa); result.append((lo,ins,f['address'])); lo+=ins.size\n return result\nold_ins=instructions(old); new_ins=instructions(new)\nold_fs={f['address']:f for f in old['functions']}; new_fs={f['address']:f for f in new['functions']}\nremoved=sorted(old_fs.keys()-new_fs.keys()); merges={m['address']:m for m in new['boundary_merges']}\npilots=[0x10e664,0x1149a8,0x136930,0x140dac,0x1179b4,0x1263b8,0x122350,0x103798,0x1091d4,0x1103d4]\nfields=('address','size','code_size','isa','kind','pools','code_ranges')\nfor address,f in old_fs.items(): a.seeds[address]=analyze.Seed(f['isa'],set(f['evidence']))\nprotected=a._pointer_table_starts(a.traces())\nold_calls={(site,ins.target) for site,ins,_ in old_ins if ins.call and ins.target is not None}\nnew_calls={(site,ins.target) for site,ins,_ in new_ins if ins.call and ins.target is not None}\ndef unknowns(catalogue, decoded):\n fs=catalogue['functions']; starts=[f['address'] for f in fs]; returns={f['address']:[] for f in fs}; refs=set()\n for site,ins,owner in decoded:\n  if ins.returns: returns[owner].append(site+ins.size)\n  if ins.target is not None: refs.add(ins.target[0])\n  for lo,hi in ins.pools: refs.update(range(lo,hi,4))\n for segment in (a.layout.rodata,a.layout.data):\n  if segment.size:\n   offset=a.layout.file_offset(segment.address)\n   refs.update(struct.unpack_from('<I',a.image,offset+i)[0]&~1 for i in range(0,segment.size-3,4))\n for f in fs:\n  for lo,hi in f['pools']: refs.update(a.word(word)&~1 for word in range(lo,hi,4))\n out=Counter(); largest=[]\n for r in catalogue['ranges']:\n  if r['kind']!='unknown': continue\n  index=bisect_right(starts,r['start'])-1; f=fs[index] if index>=0 else None; size=r['end']-r['start']\n  inside=f is not None and r['end']<=f['address']+f['size']; label='inside_extents' if inside else 'between_extents'\n  out[label+'_bytes']+=size; out[label+'_ranges']+=1\n  if f is not None and returns[f['address']] and max(returns[f['address']])<=r['start']:\n   out['after_last_return_bytes']+=size; out['after_last_return_ranges']+=1\n   if not any(r['start']<=ref<r['end'] for ref in refs): out['unreferenced_after_last_return_bytes']+=size; out['unreferenced_after_last_return_ranges']+=1\n  largest.append({'address':r['start'],'bytes':size,'position':label})\n return {'counts':dict(out),'largest':sorted(largest,key=lambda r:(-r['bytes'],r['address']))[:10]}\nspots=[0x100d6c,0x10d3dc,0x110a00,0x135f90,0x189880,0x18ed18,0x1a8024,0x1b88e0,0x1e00a0,0x210004]\nspots=[address for address in spots if address in merges]\nfor address in sorted(merges):\n if len(spots)>=10: break\n if address not in spots: spots.append(address)\nchecked=[]\nfor address in sorted(spots):\n m=merges[address]; f=new_fs[m['retained_address']]; witnesses=[]\n for witness in m['witnesses']:\n  ins=a.decode(witness['address'],f['isa']); target=witness['target']; reason=witness['reason']\n  valid=(not ins.address_reference and any(lo<=target<hi for lo,hi in ins.pools) and any(lo<=target<hi for lo,hi in f['pools'])) if reason=='section_literal' else (ins.branch and ins.target is not None and ins.target[0]==target) if reason=='branch_into_body' else witness['address']+ins.size==target\n  if not valid: raise AssertionError((address,witness))\n  witnesses.append(witness)\n checked.append({'address':address,'retained_address':m['retained_address'],'reasons':m['reasons'],'witnesses':witnesses})\nold_falls={r['address'] for r in old['anomalies'] if r['issue']=='fallthrough_into_start'}; new_falls={r['address'] for r in new['anomalies'] if r['issue']=='fallthrough_into_start'}\nnew_starts=sorted(new_fs); residual=[]\nfor target in sorted(new_falls):\n index=bisect_right(new_starts,target)-2; f=new_fs[new_starts[index]]\n last=[(site,ins) for site,ins,owner in new_ins if owner==f['address'] and site+ins.size==target]\n site,ins=last[0]; residual.append({'address':target,'preceding_function':f['address'],'site':site,'last_call':ins.target[0] if ins.call and ins.target else None,'target_evidence':new_fs[target]['evidence']})\nout={'hard_violations':analyze.validate_catalogue(new,a.image,a.layout),'removed_equals_merge_log':set(removed)==set(merges),'added_starts':sorted(new_fs.keys()-old_fs.keys()),'removed':[{**merges[address]} for address in removed],'protected_removed':sorted(protected&set(removed)),'pilots':[{ 'address':address,'unchanged':address in new_fs and all(old_fs[address][field]==new_fs[address][field] for field in fields)} for address in pilots],'old_call_sites':len(old_calls),'new_call_sites':len(new_calls),'lost_call_sites':sorted(old_calls-new_calls),'new_call_sites_only':sorted(new_calls-old_calls),'old_unknowns':unknowns(old,old_ins),'new_unknowns':unknowns(new,new_ins),'spots':checked,'resolved_fallthroughs':sorted(old_falls-new_falls),'new_fallthroughs_only':sorted(new_falls-old_falls),'residual_fallthroughs':residual,'residual_literal_references':[{'address':r['address'],'address_reference':a.decode(r['address'],new_fs[new_starts[bisect_right(new_starts,r['address'])-1]]['isa']).address_reference} for r in new['anomalies'] if r['issue']=='literal_outside_function'],'merge_evidence_counts':dict(Counter('+'.join(m['evidence']) for m in merges.values())),'merge_reason_combinations':dict(Counter('+'.join(m['reasons']) for m in merges.values()))}\nprint(json.dumps(out,sort_keys=True))"
    ],
    "exit_code": 0,
    "output_path": "scratch/analyze/REPORT-v2.md (metadata findings above)"
  },
  {
    "argv": [
      ".venv/bin/python",
      "-c",
      "from tools import analyze, ctrfmt\nfrom pathlib import Path\nimport json\nc=json.loads(Path('scratch/analyze/catalogue-v2.json').read_text()); a=analyze.Analyzer(Path('orig/code.bin').read_bytes(),ctrfmt.parse_exheader(Path('orig/exheader.bin').read_bytes()))\nf=next(f for f in c['functions'] if f['address']==1704084)\nreturns=[]\nfor lo,hi in f['code_ranges']:\n while lo<hi:\n  ins=a.decode(lo,f['isa'])\n  if ins.returns: returns.append(lo)\n  lo+=ins.size\nprint(json.dumps({'callee':f['address'],'return_sites':returns,'noreturn':f['noreturn']}))"
    ],
    "exit_code": 0,
    "output_path": "scratch/analyze/REPORT-v2.md (metadata findings above)"
  },
  {
    "argv": [
      ".venv/bin/python",
      "-c",
      "from tools import analyze, ctrfmt\nfrom pathlib import Path\nimport json\nc=json.loads(Path('scratch/analyze/catalogue-v2.json').read_text()); a=analyze.Analyzer(Path('orig/code.bin').read_bytes(),ctrfmt.parse_exheader(Path('orig/exheader.bin').read_bytes()))\nfor f in c['functions']: a.seeds[f['address']]=analyze.Seed(f['isa'],set(f['evidence']))\nstarts=sorted(a.seeds); i=starts.index(0x1a0094); trace=a.trace(starts[i],starts[i+1])\nprint(json.dumps({'address':hex(trace.address),'end':hex(trace.end),'tails':[(hex(t),isa,hex(site)) for t,isa,site in sorted(trace.tails)],'calls':[(hex(t),isa,hex(site)) for t,isa,site in sorted(trace.calls)],'issues':[(hex(site),issue) for site,issue in sorted(trace.issues)],'open_exits':[hex(site) for site in sorted(trace.open_exits)],'indirect_exits':trace.indirect_exits,'last_sites':[hex(site) for site in sorted(trace.code)[-4:]]}))"
    ],
    "exit_code": 0,
    "output_path": "scratch/analyze/REPORT-v2.md (metadata findings above)"
  },
  {
    "argv": [
      ".venv/bin/python",
      "-c",
      "from tools import analyze, ctrfmt\nfrom pathlib import Path\nimport json\nc=json.loads(Path('scratch/analyze/catalogue-v2.json').read_text()); a=analyze.Analyzer(Path('orig/code.bin').read_bytes(),ctrfmt.parse_exheader(Path('orig/exheader.bin').read_bytes()))\nfs={f['address']:f for f in c['functions']}; work=[0x19f568]; seen=set(); out=[]\nwhile work:\n start=work.pop()\n if start in seen or start not in fs: continue\n seen.add(start); f=fs[start]; returns=[]; exits=[]\n for lo,hi in f['code_ranges']:\n  while lo<hi:\n   ins=a.decode(lo,f['isa'])\n   if ins.returns: returns.append(lo)\n   if ins.branch and ins.target and ins.target[0] in fs and ins.target[0]!=start: exits.append(ins.target[0])\n   lo+=ins.size\n out.append({'address':hex(start),'return_sites':[hex(site) for site in returns],'tail_candidates':[hex(target) for target in sorted(set(exits))]})\n if not returns: work.extend(sorted(set(exits)))\nprint(json.dumps(out))"
    ],
    "exit_code": 0,
    "output_path": "scratch/analyze/REPORT-v2.md (metadata findings above)"
  },
  {
    "argv": [
      ".venv/bin/python",
      "-c",
      "from tools import analyze, ctrfmt\nfrom pathlib import Path\nfrom bisect import bisect_right\nimport json\nc=json.loads(Path('scratch/analyze/catalogue-v2.json').read_text()); a=analyze.Analyzer(Path('orig/code.bin').read_bytes(),ctrfmt.parse_exheader(Path('orig/exheader.bin').read_bytes()))\nfs=c['functions']; starts=[f['address'] for f in fs]; out=[]\nfor r in c['anomalies']:\n if r['issue']!='literal_outside_function': continue\n index=bisect_right(starts,r['address'])-1; f=fs[index]; ins=a.decode(r['address'],f['isa'])\n if ins.address_reference: continue\n for lo,hi in ins.pools:\n  other=bisect_right(starts,lo)-1; j,k=sorted((index,other)); crossed=fs[j+1:k+1]\n  out.append({'site':hex(r['address']),'source':hex(f['address']),'isa':f['isa'],'pool':hex(lo),'crossed':[{'address':hex(g['address']),'isa':g['isa'],'kind':g['kind'],'evidence':g['evidence']} for g in crossed]})\nprint(json.dumps(out))"
    ],
    "exit_code": 0,
    "output_path": "scratch/analyze/REPORT-v2.md (metadata findings above)"
  }
]
```

## Residual risks and adoption boundary

- The 197 anomalies and 39,338 unknown bytes remain real, published uncertainty.
  There is no claim of a fully recovered denominator or compiler/linker section map.
- Branch-to-body/unterminated weak-entry coalescence is **[INFERENCE]** when no pool
  witness exists; independent review should emphasize those cases and the large
  merged owner at `0x187828`. Callback-table protection is deliberately conservative
  and cannot recognize unreferenced tables in still-unknown text or arbitrary sparse
  callback layouts beyond the stated rule.
- Two unobserved raw-call-scan starts disappear; real call-target starts do not.
  Extra entry labels inside handwritten/shared routines and unrecognized indirect
  transfers remain a risk. Pools conflicting with observed code are not silently
  reclassified, and protected veneer destinations are not relaxed.
- Noreturn is a property of the recovered normal-flow graph, not a global absence of
  return opcodes and not proof about exceptions, longjmp, corruption or OS behavior.
  The returning tail chain above demonstrates why a broader zero-return heuristic
  would be unsafe.
- Same-entry identities can change extents as sections coalesce. All ten supplied
  pilots were checked fully unchanged; the integrator must apply its accepted-ledger
  and denominator-history review gates before any canonical adoption. This report
  does not authorize publication, credit, a build or denominator-history edits.
