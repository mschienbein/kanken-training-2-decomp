# cmodels2 — v8 / L1 static integration

Branch `cmodels2-integrate`, worktree `scratch/worktrees/cmodels2`, initially based on
`85fd18dc7173e0542d8c33db157d03b7910c18c5`. GATES v1.19 authorizes the selected v8 rule.
This is an **unaccepted candidate**, not a prepared/finalized checkpoint. No acceptance record,
accepted ledger or historical receipt was edited. Full builds, the full suite and prepare remain
reserved for Main's heavy slot.

## Frozen checkpoint-global selection and scope

- Linker: **`armlink5-inline-branchnop-v8`**.
- Literals: **`anonymous-string-records-v1`**.
- Candidate: **1 function / 296 B**, two units (one credited source, one helper-only source).
- Function: **`func_001e958c`**, frozen `game` profile.
- Candidate manifest: `config/candidates/cmodels2.json`.
- Exact routing, source/receipt hashes and qualifications: `scratch/integrator/cmodels2/SCOPE.json`.

Every admitted source was collected with `tools/collect.py` using both explicit choices. The
helper-only unit stays in the candidate with `functions: []` and earns no catalogue credit.
No symbol names, aliases, profiles, existing shared headers or accepted sources were changed.

## Complete-unit isolation, not partial acceptance

The newest nf045 `menu_teardown.cpp` reproduces the 296-byte target under v8 but its complete TU
also emits matched context `001e96b4` and mismatching text cleanup `001eb0d0`. The candidate
preserves the target's natural panel traversal, item cleanup and helper placement while omitting
both context definitions, their unused text types and their private helpers. Author files and
receipts remain unchanged.

The isolated source includes the new shared `game/menu_resources.h`. Both it and the separate
`ui_geometry.cpp` helper use exactly the same Point/Rectangle lifetime definitions. Item's
identifier remains at **+0xa**, distinct from AnimationChild's **+8** field. Its display-release
receiver is **ResourceEvent***, matching c009's actual `func_00183208` contract, not an assertion
that ResourceEvent and DisplayObject are one complete class. No StyleOwner/text-screen unification
or competing `001e96b4` declaration is delivered. IntegrateC010 reserved this header and confirmed
those receiver boundaries.

Final collected-unit result: **MATCH, exit 0, 296/296 B**, and exactly one emitted catalogued
function. The three candidate-only NOP rewrites are at `+0x74`, `+0xc8` and `+0xd4`.
The same isolated unit under **v6 + L1 refuses** its own Point destructor index reference.

V8 removes two natural four-byte helper entities, each with genuine same-entry D1/D2 aliases:

- `MenuResources::Point::~Point`
- `MenuResources::Rectangle::~Rectangle`

Each owns one exclusive eight-byte `.ARM.exidx` companion; no extab is removed. The **8 helper
code bytes and 16 unwind bytes earn no credit**. Their historical names, identities, outlining,
existence and TU membership remain visibly **[INFERENCE]**. V8 is not general unwind stripping,
proof of original exception flags, or a reconstructed standalone native-armlink image.

## Exhaustive pool disposition

The effective newest-source sweep contains **132 identities / 148,272 B**. Its two late appendices
replace source evidence without double-counting identities. `POOL-ROUTING.metadata.json` gives every row's
measured v6/L1 and v8/L1 outcome, holds, owner and disposition:

| Disposition | Functions | Bytes |
|---|---:|---:|
| Already in cmodels | 20 | 9,956 |
| Reserved by c009/c010/c011 | 11 | 1,504 |
| Explicit non-model hard holds | 23 | 4,108 |
| Explicit inherited Main contract holds | 2 | 2,560 |
| Released c010 rows, typed-contract deferred | 2 | 244 |
| c011 default-model lifecycle deferral | 1 | 284 |
| No exact model-matrix target witness | 72 | 129,320 |
| **Admitted** | **1** | **296** |

The last-but-one category includes model refusals and completed mismatches; it does not claim
that all 72 compared successfully or that source search was exhausted. The supplied sweep
receipts retain the precise difference/refusal outcomes.

### Why the other reported v8-only source is not collected

`func_001a77d4` (208 B) was only a context MATCH in tu917's mixed v8 TU. IntegrateC010 supplied and
retains the independent tu604 **complete game/v2/none MATCH**, already in c010's manifest:

- owner: `src/checkpoint10/collect-r2/func_001a77d4.cpp`
- source SHA-256: `fe8a4c7eedf44d00fd1f84d8b0c49352f66b7a5dfcdfd6e3bab63e7af0589313`
- fresh receipt: c010's `scratch/integrator/c010/collected-match-r2/.../func_001a77d4.result.json`

Splitting/adopting the tu917 alternative would duplicate that identity and invent a model
requirement. cmodels2 therefore preserves c010 ownership instead. The nine unrelated helper
companions in the mixed tu917 receipt are not credited or imported.

### Contract deferrals and late queue

`CONTRACT-DEFERRALS.json` records exact evidence and the selected-model measurements:

- **`001d5864` / 348 B** and **`001ab680` / 2,212 B** remain excluded by Main, despite c010's
  source-backed prerequisites. No all-consumer/G3/argcheck clearance was implied or imported.
- **`001db404` / 48 B** MATCHes v8/L1, but c010 released it only as a ResourceFontVTable versus
  canonical FontVTable/native ResFontBase typed-contract deferral. Its author already has a
  default-model MATCH; model selection does not prove the class/vtable/return identity.
- **`001ef89c` / 196 B** MATCHes v8/L1 with its author overlay, but the native HeapArena/AllocatorBase
  view contradicts the existing HeapArena/ManagedExpHeap contract and needs an unadopted base
  destructor name. No name or invented common layout was adopted.
- **`001d5694` / 284 B** MATCHes v8/L1 but its RESULT already records a default-v2 MATCH with no
  rewrite or removed helper. IntegrateC011 owns its default-model accepted-lifecycle deferral:
  CachedSoundLoaderState versus native PlayerHeapDataManager/SoundArchiveLoader. Four proposed
  names remain unimported.

The live `model-rows-next.json` initially listed `001e958c` and `001ed874`. The latter is already
in cmodels and is covered by the complete-ledger v8 measurement, not new credit. A later append
added residual-families **`00169018` / 144 B**. It was measured **MATCH, exit 0** under v8/L1 but is
**typed-contract deferred**: it passes WidgetContainer* to `00177b84`, while prepared cmodels uses
MessageLayout* and c009's callee source uses WidgetActivity (seven handles, state at +0x1c). Those
may be overlapping partial receiver views, but equality does not prove one complete owner. Main
was notified; no cast/private conflicting declaration or invented union was introduced. The late
queue snapshot, exact command and receipt are retained.

## Whole-ledger selected-model evidence

To cover all prior accepted functions **and every prepared cmodels addition/helper**, the
per-unit sweep used Main's prepared PASS `accept/cmodels/matches.json` as its explicitly labeled
prospective ledger. It checked:

- **1,546 credited source-unit commands — all exit 0**.
- **1,621 functions / 195,908 B — all MATCH**.
- **16 helper-only ledger units**, all compiled in their owning caller closures.
- The five unique standard-regression source units are included in those selected-model results.
  The historical 15 v2/v3/v4 baseline witnesses remain unchanged in cmodels; no unchanged baseline
  was rerun merely to confirm it.
- Headers, model harness, catalogue, profiles and names stayed unchanged during the sweep.
- Commands were sequential, `nice -n 10`, `tools/match.py`, `--show 0`, using short output paths.

No accepted unit has an observed byte, extent or compile regression under v8/L1. Existing model
transition review still reopens **19 R1 functions and the one L1-owning function**. The new cleanup
adds its own typed CFG/helper/complete-unwind obligations. `MODEL-TRANSITION.json` binds each
existing affected source, helper closure and receipt. Passing per-unit checks do not prove a
joint whole build's overlap/unique-owner/full-image gates and do not waive independent review.

There is one separately reported **typed integration risk**, not a byte regression:
`func_001b8908` calls `00177b84` with the prepared MessageLayout view, while c009's matching
callee defines WidgetActivity. It is the same fork that defers the late `00169018` caller.
`ACCEPTED-TYPE-RISKS.json` records the exact fields and owner paths. Main was notified of the
specific prepared caller; no all-consumer ABI clearance is claimed here.

Records:

- `scratch/integrator/cmodels2/SCOPE.json`
- `scratch/integrator/cmodels2/ACCEPTED-V8.json`
- `scratch/integrator/cmodels2/ACCEPTED-TYPE-RISKS.json`
- `scratch/integrator/cmodels2/SOURCE-PROVENANCE.json`
- `scratch/integrator/cmodels2/CANDIDATE-WITNESS.metadata.json` (no instruction words)
- `scratch/integrator/cmodels2/MODEL-TRANSITION.json`
- `scratch/integrator/cmodels2/POOL-ROUTING.metadata.json`
- `scratch/integrator/cmodels2/CONTRACT-DEFERRALS.json`
- `scratch/integrator/cmodels2/MODEL-ROWS-NEXT.snapshot.json`
- `scratch/integrator/cmodels2/RELEASED-OUTCOMES.json`
- `scratch/integrator/cmodels2/LATE-00169018.json`

No full suite, full build, G3, argcheck/manual full ABI audit, prepare, independent checkpoint
review or finalize was run. Before handoff, Main's finalized cmodels main tip must be merged;
this initial source milestone is deliberately not represented as that completed merge.
