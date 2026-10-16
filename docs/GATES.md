# Gate design — frozen v1.1 (2026-10-16)

Revisions:
- **v1.1** (2026-10-16): `prepare.json` again records the per-gate `checks` map (v1 tooling dropped
  it through an editing error; gate logic and `result` were unaffected). Regression test
  `tests/test_accept.py::test_prepare_records_every_gate_result`. The defective record
  `accept/c001-pilot/` is kept unfinalized (see its DEFECT.md); the candidates were re-prepared as
  `c001-pilot-r2`.

Changing anything below needs a recorded revision here (new version, date, reason) **before** the
next checkpoint is prepared. A batch never runs under a gate design that changed mid-batch.

## Author loop (no gate, private)

`tools/packet.py` → C/C++ → `tools/match.py --profile game` until `MATCH`. Per-function oracle:
compile with the frozen profile, strict in-place link (`tools/inplace.py`), byte compare over the
catalogued extent. Fails closed on: unresolved or unbound symbols, aliased or misaligned sections,
contradictory COMDAT copies, unsupported relocations, veneer needs, multi-function sections.

## Checkpoint gates (`tools/accept.py`)

| # | Gate | Where | Fails when |
|---|---|---|---|
| G1 | Denominator | `prepare` | `config/catalogue.json` is not the recorded denominator (`tools/denominator.py verify`) |
| G2 | Ledger composition | `prepare` | a candidate unit is already accepted |
| G3 | Interfaces | `prepare` → `accept/<cp>/interfaces.json` | any declaration/definition or struct-layout disagreement, const-contract violation or policy error across the **whole ledger**; C++ units unchecked unless `--allow-unchecked-cpp` (recorded; reviewers then check those units' interfaces by hand) |
| G4 | Two fresh builds | `prepare` | either `tools/build.py` build (fresh: no object cache) fails, or outputs differ. Each build: every credited function an ordinary catalogued function, exact size, equal to the original on its own before being written; no overlapping sections across units; no duplicate global definitions; `code.bin` equal to the original; recompressed `.code` equal to the stored ExeFS `.code` |
| G5 | Full suite | `prepare` | any module fails (`tools/test_suite.py`, sharded by module) |
| G6 | Review | `review` | the record fails the schema: verdict row (PASS/QUALIFIED/FAIL + method) for every new function, unique finding ids F01…, severities, verdict consistency (blocker or FAIL row ⇒ FAIL; QUALIFIED or major ⇒ not PASS; PASS_WITH_QUALIFICATIONS ⇒ stated qualifications) |
| G7 | Finalize once | `finalize` | prepare not PASS, fewer than `--min-reviews` (default 1) reviews bound to this exact prepare, any FAIL review, sources/profiles/catalogue/ledger changed since prepare, history broken |
| G8 | Ledger binding | `verify` (CI) | `config/matches.json` ≠ last finalized ledger, a credited source ≠ its accepted hash, or credit without any checkpoint |

Reviewer independence: the reviewer is never the integrator and never the author; at least one
review per checkpoint is from a model family other than the author's, where possible.

Records: `accept/<cp>/` (exclusive-create JSON; FAIL kept), `docs/progress-history.jsonl`
(hash-chained), `docs/denominator-history.jsonl` (hash-chained).

## Scale plan for the Windows desktop (Shiren lessons 15–22)

| Lesson | Status here |
|---|---|
| 15 mechanical checks before review | G3 (interfaces). UB lint (signed-shift overflow, out-of-bounds index) not yet automated |
| 16 fixed review schema | G6, frozen in v1 |
| 17 detached long jobs | `tools/jobs.py run --name … -- …` (exit marker in `scratch/jobs/<name>/exit`) |
| 18 parallel build, sharded suite, object cache | `build.py` compiles in parallel (`--jobs`, default CPU count); `tools/test_suite.py` shards by module; exact-input object cache (`.cache/objcache`, keyed by compiler hash, flags, include path, source and every header from `--depend`); acceptance builds are `--fresh` |
| 18a queue time vs runtime | `test_suite.py` and `jobs.py` record wall time per module/job; no semaphore yet (single-host, light jobs) |
| 19 native toolchain | armcc.exe is a 32-bit Windows PE: native on the desktop, wibo + Rosetta on the Mac. Cross-host byte identity to be proven by the desktop re-run of c001 |
| 20–21 compressed evidence, safe pruning | not needed yet (evidence is small JSON); before any pruning tool exists, nothing outside `scratch/` is deleted |
| 22 freeze gates before a batch | this file |

Next speedups when the function count grows: shard `tests/test_analyze.py` (it alone sets the
suite floor, ~45 s, by analysing the real image twice), and cache match results keyed by object
hash + catalogue + image + harness hashes.
