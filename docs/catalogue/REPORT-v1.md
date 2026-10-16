# Candidate function catalogue — 2026-10-15

## Status and provenance

**Implemented and exercised; candidate only, not an accepted/frozen denominator.**
The complete mapped image is accounted for exactly once, but **39,470 text bytes
remain unknown and 350 boundary ambiguities are retained**. Nothing here establishes
C/C++ matches, SDK identity, names, or source acceptance.

- Runtime identity: family **OpenAI**, model **openai-codex/gpt-6.1-sol**.
- Host: macOS arm64; project interpreter Python **3.12.15**; installed Capstone
  **5.0.7** (the generator records the installed version, not the anticipated 5.0.9).
- Input: `orig/code.bin`, **1,327,104 bytes**, SHA-256
  `a367b5dcd65e3924c93cf3fbaee5d4d40f9c5205d1287912784a974d5e405361`.
- Segment addresses/sizes are parsed with `ctrfmt.parse_exheader`, never hard-coded
  into discovery. No network or subprocesses are used by the analyser itself.
- Output: `scratch/analyze/catalogue.json`; SHA-256
  `06027f34eb7e89666c138293a31393cbafc90d9c66862add8f3d9df571aedf13`.
- No ROM bytes, instruction text, or original disassembly are included in the tool,
  tests, catalogue, report, or evidence logs. Tests use hand-encoded synthetic inputs;
  only the optional private-input smoke test reads the original image.

## Results

| Category | Count |
| --- | ---: |
| All starts | **4,504** |
| ARM functions | 4,427 |
| Thumb functions | 71 |
| ARM linker veneers | 6 |
| Thumb linker veneers in this input | 0 |
| Recognized direct BL/BLX sites | 19,991 |
| Direct-call target/ISA mismatches | **0** |
| Recorded direct tail edges | 1,296 |
| Recognized switch dispatches | 142 |
| Other indirect exits | 155 |
| Decoded instructions | 264,702 |
| Claimed instruction bytes | 1,054,490 |
| Claimed word-aligned literal/table bytes | 48,380 |
| Discovery rounds | 7 |

Evidence labels are **not disjoint**. `bl_target` also covers Thumb BL;
`blx_target` denotes an immediate call changing ISA. `pointer` is a plausible entry
referenced by an aligned word in rodata, data, or a discovered text pool. Branch
address-table entries are excluded from pointer-start promotion. `call_scan` means
an aligned raw call-shaped word whose source has not been proved executable.

| Start evidence | Count |
| --- | ---: |
| bl_target | 3359 |
| blx_target | 43 |
| call_scan | 2 |
| entry | 1 |
| pointer | 787 |
| prologue | 151 |
| tail_call | 460 |
| veneer_pattern | 6 |
| veneer_target | 6 |

**3,393 / 4,504 starts (75.333%)** have observed BL/BLX evidence; 9 starts have both
BL and BLX labels. **106 starts are prologue-only**. Two starts are call-scan-only:
`0x189880` and `0x18988C`; their caller bytes are not claimed executable. These are
explicitly weaker candidates, not observed call destinations.

### Complete-image accounting

| Range kind | Bytes |
| --- | ---: |
| data | 51,464 |
| function | 1,102,870 |
| padding | 1,968 |
| rodata | 123,064 |
| segment_padding | 8,268 |
| unknown | 39,470 |
| **Total mapped bytes** | **1,327,104** |

The 39,470 unknown bytes are **3.449246% of real text**, or **2.974145% of the
mapped image**. There are **2,232 unknown ranges**:

- **31,648 bytes / 2,082 ranges** lie inside the extents of 92 candidate functions,
  but outside their actual decoded-instruction and referenced-pool spans.
- **7,822 bytes / 150 ranges** lie between candidate extents.

Those bytes remain `unknown`, even inside a function extent: they are not silently
absorbed into `function` ranges. Unreferenced literal-array elements, skipped code,
and other embedded data cannot be distinguished solely from the recovered edges.
That possible interpretation is **[INFERENCE]**, not a data classification.
The table below locates the ten largest remaining gaps without printing contents.

| Gap address | Bytes | Position |
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

## Boundaries, pools and schema interpretation

The tool recursively follows conditional/unconditional branch edges, stops at
recognized returns and direct/indirect tail exits, and follows switch cases. ARM
branch tables and absolute-address tables are recognized, including the difference
between LO's exclusive bound and LS's inclusive bound. A nearby comparison is
searched within 64 bytes; contiguous unconditional branch tables can also be
recognized without a nearby comparison. ARM and Thumb immediate calls are decoded
in their respective modes. Both linker veneer forms have synthetic coverage.

`size` is entry through the last **owned** instruction or referenced pool word;
`code_size` is entry through the last decoded instruction, **not** the number of
instruction bytes. Trailing unreferenced words are not guessed to be section data.
The additional `code_ranges` field is necessary to distinguish reachable instruction
spans from embedded pools and unexplained holes. `ranges` owns only explained code
and pool bytes, not every byte enclosed by a candidate's extent. Consumers must not
assume `[address,address+code_size)` contains instructions without gaps.

References to another candidate's pool, or beyond a candidate boundary, are retained
as `literal_outside_function` rather than extending one function across another.
Thus ambiguous shared-pool/section boundaries are explicitly **not** claimed solved.
ARM unconditional-space prefetch encodings are not confused with literal loads.
For the Thumb interworking veneer, validation decodes its final branch in ARM mode;
this is an explicit linker-defined mixed-ISA exception, not normal Thumb code.

## Anomalies and remaining unknowns

`validate_catalogue` reports **zero hard invariant violations** on this input:
all 19,991 observed direct-call sites land on a catalogued start in the correct ISA;
function extents do not overlap; all 264,702 declared instructions decode; owned
pool ranges are aligned, nonoverlapping words; code/pools do not overlap; all **8,268
segment-page-padding bytes are zero**; and ranges tile every mapped byte once.

The following **350 semantic/boundary anomalies remain in the JSON**, independently
of those structural checks:

| Issue | Count | Interpretation and disposition |
| --- | ---: | --- |
| `literal_outside_function` | 139 | A decoded PC-relative reference leaves the candidate's ownership partition. Keep the reference anomaly; do not create overlapping extents or steal a neighbour's pool. |
| `fallthrough_into_start` | 115 | A decoded path reaches the next discovered start without a recognized terminating exit. Boundaries are partitioned, but possible alternate/shared entries or over-splitting remain **[INFERENCE]**. |
| `unexplained_function_bytes` | 95 | 92 extents contain explicit unknown holes, quantified above; 3 have only zero/NOP-padding holes. All holes are absent from `code_ranges` and retain `unknown` or `padding` map classifications. |
| `undecodable_reachable_code` | 1 | At `0x1964E8`, after a direct call at `0x1964E4` to `0x145570`. The word is not claimed code. Whether the path is nonreturning, intentionally invalid, or incorrectly bounded is unresolved. The callee does have two recognized return sites, so this is **not** asserted to be a noreturn helper. |

The 139 outside-literal references classify mechanically as follows. `owned_pool`
means another candidate currently owns that word; it does not prove that the
candidate boundaries reproduce the original input sections.

| Source ISA : direction : current target range | References |
| --- | ---: |
| arm:backward:owned_pool | 60 |
| arm:forward:owned_code | 5 |
| arm:forward:owned_pool | 21 |
| arm:forward:padding | 2 |
| arm:forward:unknown | 43 |
| thumb:forward:padding | 1 |
| thumb:forward:unknown | 7 |

In aggregate: **81** references land in another owned pool, **50** in unknown bytes,
**5** in owned instruction bytes, and **3** in currently classified zero/NOP padding.
The last two groups can include address-taking rather than ordinary pool data, or
inaccurate candidate boundaries; that explanation remains **[INFERENCE]**. They
are not suppressed. Outside references include a backward pool reference from
`0x1898C8` into `0x189568`, and a forward reference from `0x100532` into `0x1005BC`.

Of the 115 fallthrough target starts, 82 have some BL/BLX evidence, 18 are
pointer-only/pointer-plus-tail, 8 carry prologue evidence, 5 are tail-only, and 2
are call-scan-only. Examples: `0x100D68` → `0x100D6C`, `0x10D27C` → `0x10D380`, `0x1109FC` → `0x110A00`.
These counts explain why blindly deleting every fallthrough start would destroy
observed call targets. Independent boundary review is still needed before freezing.

## Ten reproducible random spot checks

Uniform sample from all starts using Python `random.Random(20261015).sample(...,10)`;
sorted by address below. These are mechanical checks of decoded span/pool metadata,
not an independent disassembly review or function naming exercise. All ten have no
trace-local anomaly, their spans decode, and their recorded pool words are aligned.
All ten happen to be ARM; all 71 Thumb starts are covered by whole-catalogue
validation, and Thumb/BLX/interworking mechanics have dedicated synthetic tests.
Zero recognized return sites is reported literally, not treated as proof of noreturn.

| Address | ISA | Size | Code end offset | Instructions | Pool bytes | Start witness | Recognized returns |
| --- | --- | ---: | ---: | ---: | ---: | --- | ---: |
| `0x102CD0` | arm | 124 | 116 | 29 | 8 | bl_target | 1 |
| `0x10BD7C` | arm | 152 | 144 | 36 | 8 | bl_target | 2 |
| `0x1107B4` | arm | 16 | 16 | 4 | 0 | pointer | 1 |
| `0x1540AC` | arm | 376 | 368 | 92 | 8 | bl_target | 0 |
| `0x155B24` | arm | 512 | 508 | 127 | 4 | tail_call | 0 |
| `0x15EAE0` | arm | 176 | 156 | 39 | 20 | bl_target | 1 |
| `0x1C73CC` | arm | 284 | 272 | 68 | 12 | tail_call | 2 |
| `0x1CFD3C` | arm | 4 | 4 | 1 | 0 | pointer | 0 |
| `0x1DB958` | arm | 4 | 4 | 1 | 0 | pointer | 1 |
| `0x1F1FF0` | arm | 16 | 16 | 4 | 0 | pointer | 0 |

## Exercised acceptance

All commands ran sequentially in the repository with the project interpreter.
The different `PYTHONHASHSEED` values also exercise set/dict ordering independence.

| Exact command | Hash seed | Exit | Wall time | Output |
| --- | ---: | ---: | ---: | --- |
| `.venv/bin/python -m unittest tests.test_analyze -v` | 17 | 0 | 7.764 s | 28 tests passed, including original-image repeatability/invariants |
| `.venv/bin/python tools/analyze.py catalogue` | 23 | 0 | 3.515 s | `scratch/analyze/catalogue.json` (tool timer 3.443 s) |
| `.venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-repeat.json` | 71 | 0 | 3.513 s | `scratch/analyze/catalogue-repeat.json` (tool timer 3.445 s) |

The two CLI JSON outputs are **byte-identical**, including statistics and ordering,
with SHA-256 `06027f34eb7e89666c138293a31393cbafc90d9c66862add8f3d9df571aedf13`.
Both catalogue runs are well below the two-minute requirement. No full-repository
suite, build, formatter, or publication command was run. Windows execution was not
available; cross-platform compatibility is based on pathlib/stdlib and absence of
shell-outs, not a claimed Windows test.

Full exact arguments, exit codes, stdout/stderr and measurements are retained in
`scratch/analyze/verification-final.json`; report evidence and the exact diagnostic
Python source are included there. Earlier passing command logs and the initial
failed-test disposition are in `scratch/analyze/verification-development.json`.
The first test attempt failed due to cross-segment padding-range coalescing and an
initial 12-round discovery cap; those implementation causes were fixed. LO switch
bounds and unconditional prefetch decoding were also corrected using synthetic
regressions, rather than hiding the resulting conflicts.

## Confidence and hand-off

- **High confidence in the measured structural accounting**, ISA decoding of
  declared code, direct-call target closure, word alignment and determinism.
- **Moderate confidence in function/section boundaries**, not a percentage claim:
  75.333% have executable-call evidence, but 106 are prologue-only, 2 are raw-scan
  only, 95 extents are sparse (92 with unknowns, 3 with padding-only holes), and 139 outside-literal plus 115 fallthrough
  observations qualify the ownership partition. A unique original section boundary
  cannot be derived for every such case from these observations alone.
- **No claim that unknown bytes are dead or data**, and no attempt to assign them
  to neighbouring functions merely to improve coverage numbers.
- Main should independently review these candidate ambiguities before any H4 freeze
  or denominator adoption. This task does not authorize catalogue/config promotion.

Source SHA-256:

- `tools/analyze.py`: `a2ef84438f418451884dea4c1daf4ae67f9c6e31710663a8c8ce8054f57cbd03`
- `tests/test_analyze.py`: `dac3b6d6512fe4b07d250e288162f33d30119f780f8a9bb7fc66f3ffb21127d2`
