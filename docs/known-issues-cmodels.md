# cmodels known issues and qualifications

Accepted at history **seq 9** (2026-10-18): **20 new functions / 9,956 B** plus **3 revised
accepted units** (`src/wave10/func_0017a424.c`, `src/wave8/func_00172c04.c`,
`src/wave8/func_00173f9c.c`) and **16 helper-only units** that earn no credit. Ledger after
finalize: **1,621 / 4,624 creditable functions, 195,908 / 1,131,454 B (17.31%)** (denominator
v9-r2). Gates v1.19, explicit selection `armlink5-inline-branchnop-v6` /
`anonymous-string-records-v1` (first checkpoint using an opt-in linker model and L1).

Both independent reviews passed with qualifications and **no findings**:
`accept/cmodels/reviews/openai.json` (3 PASS / 20 QUALIFIED credited rows, plus 25 zero-credit
helper rows and 23 legacy alias-placement rows) and `accept/cmodels/reviews/second.json`
(3 PASS / 20 QUALIFIED). Both re-derived the complete fresh receipts: 1,621 functions /
195,908 B, exact extents, unique emitted ownership, no duplicate helper definitions.

## Scope and how it was selected

- Rows whose best natural source is refused or mismatched under the default v2/none model and
  matches under the adopted v6 linker model and/or the L1 literal-record placement, from the model
  sweeps (`scratch/model-sweep/`) and author witnesses. Integration: `cmodels-integrate`
  (`85fd18d`), collector support for explicit linker/literal models and extra sources.
- Every accepted unit from c001–c008-r3 was re-measured under v6 + L1 before prepare and still
  matches; the three revised accepted units are contract corrections required by the new callers.

## Qualifications carried by the accepted code

- **R1 removed helpers:** 30 rewrite sites and 25 removed helpers. A removed helper's original
  identity, existence and historical call graph are not proven by the match; helper names and
  source shapes stay `[INFERENCE]`. Typed incoming/outgoing contracts were discharged per caller
  (GATES F03).
- **L1 literal records:** 49 anonymous string records in one 7,284-byte witnessed span were
  placed by whole-record identity. The original pool's complete extent, ownership, sharing with
  uncompiled units and ordering algorithm remain unknown; no data credit is granted.
- **v6 aliases:** zero-size entry aliases and independently bound legacy placements follow the
  v1.18 boundary; the 23 legacy alias placements were reviewed separately.
- **Inferred receivers and ABI obligations:** descriptive class/receiver identities and some
  unused inputs remain `[INFERENCE]`; argcheck results are conservative and were adjudicated, not
  treated as an all-pass audit.

## Deferred (no credit here)

- `func_001d5864` (348 B): signed-byte kind and audio callback/result contracts conflict with
  accepted `archive.h` / `sound_track.h` declarations; routed to c010's audio reconciliation.
- `func_001ab680` (2,212 B): passes a full-int category where `audio_voice.h` declares an
  unsigned-char option; routed to c010.
- `func_001e30a0`, `func_00151724`: v3-only witnesses; refused under v6 + L1 (canonical-type
  collision; R1 call-ABI filtering).
- 43 sweep mismatches and 6 compile/name/R1 refusals remain outside credit, documented in the
  integration notes.

## Follow-up

- `func_00177b84` receiver contract: the cmodels consumer
  (`src/models/func_001b8908/func_001b8908_removed.cpp`) uses `MessageLayout *`, while the c009
  definition models a different widget receiver. c009 adjudicates one canonical receiver type
  from the original callers before its prepare.
