# cmodels — model-qualified integration milestone

This is an **unaccepted checkpoint candidate**, not a prepared/finalized checkpoint. It lives on
`cmodels-integrate` in `scratch/worktrees/cmodels`, originally based on main `045a47d` / GATES v1.18.
Main retains the serialized c009 → c010 → c011 checkpoint order and owns the later heavy slot.
No accepted ledger, acceptance record or historical receipt was changed.
The source milestone is `a7762e2`. As subsequently requested, main `f24fa0b` (v8 adoption /
GATES v1.19) was merged as `daa067b7bbea80b9e7763308815b4c0a09c6a199` after
`git merge-tree --write-tree HEAD f24fa0b` exited 0. `MERGE.json` records that closure.
Main attested, and merged GATES states, that the relied-on v6 rules are unchanged. The existing
measurements precede this merge; they were not rerun simply to confirm that observation.

## One global selection

- Linker: **`armlink5-inline-branchnop-v6`**.
- Literals: **`anonymous-string-records-v1`** (L1).
- Every candidate and accepted-unit measurement explicitly selected both models and `--show 0`.
- No v8-only source, retired v5/v7 rule, per-unit selection or fabricated helper address was adopted.
  Subsequent v8 adoption on main does not change this worktree's frozen selection.

**Candidate credit: 20 functions / 9,956 B**, in **35 units**: 19 credited source units and 16
required helper-only units. These are candidate totals; accepted progress is unchanged.
There are 19 R1-using functions and 25 distinct removed-helper definitions. Helpers and external
literal data earn no function/byte credit. The L1 owner has one 7,284-byte `.constwstring` span,
49 records and 49 witnesses; only its 2,540-byte function is credited.

Selection arithmetic, without conflating private matches and accepted credit:

| Step | Functions | Bytes |
|---|---:|---:|
| Historical sweep v3 + L1 positives | 23 | 8,376 |
| Historical v4 + L1 adds `func_001ab680` | 24 | 10,588 |
| Exclude four c010-scoped default-model rows | 20 | 10,076 |
| Add tu924 `func_001e0afc`, freshly MATCH under v6 + L1 | 21 | 11,816 |
| Defer `func_001d5864` for accepted-audio contracts, per Main | 20 | 11,468 |
| Add tu970 `func_001b9f18`, freshly MATCH under v6 + L1 | 21 | 12,168 |
| Defer `func_001ab680` for accepted configuration contracts, per Main | **20** | **9,956** |

Main also authorized exactly two v3-only rows outside the original sweep. Both were measured under
this same v6/L1 selection and refused; no second selection was introduced.

## Collected functions

All functions below have full-extent MATCH witnesses from their final integrated source and
required helper sources, with exit 0. Exact source/receipt hashes and paths are in
`scratch/integrator/cmodels/SCOPE.json` and `FINAL-RESULTS-r2.json`.

| Function | Bytes | Frozen profile |
|---|---:|---|
| `func_0010be58` | 60 | game |
| `func_00112628` | 36 | sdk |
| `func_00112b5c` | 36 | sdk |
| `func_001131d4` | 36 | sdk |
| `func_0013f06c` | 164 | middleware |
| `func_0015f890` | 44 | game |
| `func_00168d14` | 168 | game |
| `func_00172e00` | 96 | game |
| `func_001730ec` | 116 | game |
| `func_001a00f8` | 264 | game |
| `func_001a2814` | 480 | game |
| `func_001a3ee0` | 412 | game |
| `func_001a8048` | 412 | game |
| `func_001b8908` | 664 | game |
| `func_001b9f18` | 700 | game |
| `func_001bd68c` | 696 | game |
| `func_001ddde4` | 464 | game |
| `func_001e0afc` | 1,740 | game |
| `func_001ed874` | 828 | game |
| `func_00203a68` | 2,540 | game |

The retained `func_0013f06c` and `func_001bd68c` are the original sweep's valid v3/v6 source
witnesses with C helper TUs. Later native-destructor/exidx variants needing v8 are different
source witnesses and are not substituted here.

## Collection and source integration

- Every new C/C++ source and helper was admitted through `tools/collect.py`, recompiling the
  destination with the explicit v6/L1 selection. No author source was hand-copied into `src/`.
- `config/candidates/cmodels.json` includes all helper-only units with `functions: []`.
- The three linear-buffer callers share one collected `InitializeLinearBufferAllocator` helper
  and one `linear_buffer.h` contract. Their matching SDK profile is retained.
- The tu441 native TU reproduces both `func_001a2814` and `func_001a3ee0`. It alone owns those
  identities. The separately collected r4044 `func_001a3ee0` definition was removed; the other
  r4044 state machine keeps its required accessor. No accepted function is rehomed.
- All collected-source INTEGRATOR markers and local C++ declaration blocks were resolved into
  shared headers. Partial receiver views remain separate where common complete-class identity
  is not evidenced, particularly `TrainingScreen` versus `L041InitializeScreen`.
- `CallbackTarget` uses the same nine descriptive virtual slots as c009's canonical contract.
  The observed transition-completion callback uses slot 5; erased helper provenance remains
  visibly `[INFERENCE]` in every R1 caller and helper.
- Ten new subsystem headers and seven existing-header revisions are bound in SCOPE.json.
  Three accepted source units were revised to consume the shared contracts:
  `src/wave8/func_00172c04.c`, `src/wave8/func_00173f9c.c`, and
  `src/wave10/func_0017a424.c`. All three MATCH under the selected models.
- Exactly one needed symbol binding was adopted: `_ZN11TextChoicesC1Eiz` → `0x00164be8`.
  Its natural constructor relocation is witnessed by the complete L1 caller. The descriptive
  class identity remains `[INFERENCE]`; no C2, destructor, vtable or unrelated overlay names
  were added, and no name was rebound.
- The pinned GPL-3.0-only NintendoWare4Ctr provenance for `FrameHeap::FreeByState` is retained
  and documented in `THIRD_PARTY.md`. No vendor library, ROM, packet or disassembly was added.

## Refused and excluded rows

The exact measured source, profile, helper list, target outcome, diagnostic and receipt hash are
recorded per row in SCOPE.json. The 43 sweep **mismatches / 65,832 B** are not matcher refusals:
comparison completed but the requested function did not reproduce its catalogue extent. The six
sweep **refusals / 15,428 B** are:

| Function | Bytes | Observed selected-model refusal |
|---|---:|---|
| `func_001456e4` | 240 | Local `W16SystemDisplay data_0023ee9c` conflicts with canonical `DisplayQueueFlags`. |
| `func_001721e4` | 620 | The complete tu172 source has C-linkage overload/return-type conflicts against canonical declarations. |
| `func_001a43b8` | 12,336 | Local unsigned-character allocation-tag declaration conflicts with canonical `const char data_002232d5[]`. |
| `func_001b6ec4` | 588 | Unresolved `_ZN16PrimaryParameterC1Ev`; no unsupported address/name binding was invented. |
| `func_001ca104` | 404 | Local `Vector3` redefines the current canonical type. |
| `func_002166cc` | 1,240 | R1 still-reference/call-ABI refusal for `TextCharacterWriter` destruction at the reported print-site offset `+0x1cc`. |

Additional authorized measurements, retained as out-of-pool outcomes:

| Function | Bytes | Observed v6/L1 outcome |
|---|---:|---|
| r342 `func_001e30a0` | 464 | Refused: the historical source redefines canonical `W10DrawingInput` and its `func_0016dbc4` declaration conflicts. |
| r4039 `func_00151724` | 244 | Refused: `sequence_commands` remains referenced at caller `+0x3c` under the ordinary R1 call-ABI filter. Its prior positive receipt was explicitly labeled classifier-monkeypatched by the author and is not frozen-harness evidence. |

Both byte-matching typed-contract deferrals are explicit Main decisions, not model failures:

- **`func_001d5864` / 348 B:** the caller promotes signed `wave_kind` to int for `001d3c78`,
  while the accepted declaration takes unsigned char. It also supplies channel/priority/callback/
  context to `001d9198` and uses an AudioVoice result view, whereas the accepted factory has four
  unsigned inputs and a SoundTrack result view. The actual Channel/Voice layers require careful
  reconciliation; cmodels does not impose a common complete type or pointer-as-integer API.
- **`func_001ab680` / 2,212 B:** the direct `00171a34` call forwards `screen->category` as a full
  int, while `audio_voice.h` declares an unsigned-char option and unsigned flags. The accepted
  ConfigState/ConfigTarget receiver views also differ from the candidate's page/content views.
  The accepted target pointer at `+0x18` agrees with the appended binding, but that alone is not
  complete-class identity. Main routed these shared corrections to IntegrateC010; the provisional
  candidate source/helper and unused page-binding header were removed.

Full evidence is in `scratch/integrator/cmodels/CONTRACT-DEFERRALS.json`.

Four sweep positives remain reserved by c010's broader scope, even where not yet collected:
`func_001c6a68` (104 B), `func_001db404` (48 B), `func_001ef89c` (196 B), and `func_00214d9c`
(164 B). They add no cmodels credit. Final selected identities have no intersection with the
accepted ledger or the recorded c009/c010/c011 scopes/manifests.

The RESULT-only inventory also found a v8-only witness for `func_001a77d4` (208 B), which is outside
this selection. The tu900 `func_001d5694` alternative is a default-v2 MATCH, so its separate v6
witness does not make it an opt-in-only candidate. The older sweep source's mismatch is preserved.

## Accepted-ledger risk assessment and exercised checks

**No observed accepted-unit regression:** the entire existing accepted ledger, not merely a
header sample, MATCHes under v6/L1: **1,527 units / 1,601 functions / 185,952 B**. This conservatively
covers every accepted unit sharing any candidate header or helper.

There were 1,529 matcher invocations: two initial compiler failures could not write long
preprocessor dependency-output paths. Both unchanged units then MATCHed with shorter output
paths. A path-length limit is **[INFERENCE]**; the failed diagnostics and corrected measurements
are both preserved. No source, header, compiler profile or model was weakened to pass them.
No accepted R1 or L1 use was introduced by the model transition in these per-unit results.

The prescribed **15 standard regressions passed** (five unique sources × v2/v3/v4 baselines).
All five sources also MATCH v6/L1 in the accepted-ledger sweep; those chosen-selection witnesses
are referenced rather than rerun unchanged.

The discrete collector-support commit **`11afde0`** adds explicit linker/literal flags and
repeatable same-profile extra-source collection through the matcher's existing combined-object
path. Default manifests retain the old v2/none behavior. Requested checks:

```text
nice -n 10 .venv/bin/python -m unittest tests.test_collect tests.test_match
Ran 48 tests — OK
```

Final admitted source evidence: **19 per-unit commands, all exit 0, 20 functions / 9,956 B**.
Removing the disjoint typed-contract-deferred units did not alter or rerun these witnesses.
Commands were sequential and nice'd. Public summaries:

- `scratch/integrator/cmodels/SCOPE.json`
- `scratch/integrator/cmodels/FINAL-RESULTS-r2.json`
- `scratch/integrator/cmodels/ACCEPTED-RESULTS-r2.json`
- `scratch/integrator/cmodels/STANDARD-REGRESSIONS.json`
- `scratch/integrator/cmodels/RESULT-INVENTORY.json` (1,002 author RESULT manifests; no packet search)
- `scratch/integrator/cmodels/MERGE.json` (requested main adoption merge, selector unchanged)
- `scratch/integrator/cmodels/NEXT.json` (later v8 witness; no new credit or v8 selection)

Not exercised: full build, full suite, G3 interfaces, argcheck/manual ABI audit, prepare,
independent checkpoint review or finalize. Main must merge the serialized predecessor states
and run its normal complete-ledger gates before preparing with these same explicit models.
The passing byte checks do not replace R1 typed/provenance review or L1 pool qualifications.
