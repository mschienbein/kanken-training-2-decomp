# Gate design — frozen v1.12 (2026-10-17)

Revisions:
- **v1.12** (2026-10-17): G3 const-write check fix. Indexing through a pointer (`obj->ptr[i] = v`)
  writes the pointee, so the pointer's own constness (a pointer member of a const object) no
  longer makes it a const write; indexing an array member of a const object still is one. Cause: a
  false positive on c006's `func_00183bc8` (`const ArchiveReader *` whose `cache` array is
  writable). Regression tests in `tests/test_interfaces.py`.
- **v1.11** (2026-10-17): reviews add step 7 (`docs/REVIEWING.md`): the caller-contract audit
  `tools/argcheck.py` over the prepared ledger, every reported candidate adjudicated; the
  integrator runs the same audit before each prepare. Cause: c004-r2 and c005 both failed review on
  empty/wrapper/callback prototypes that dropped caller-evidenced arguments. Also: compiler profiles
  `sdk` and `middleware` (ARMCC 4.1 b791, `docs/COMPILER.md`) join `game`; a unit's ledger profile
  is the one its bytes match, and reviewers check that non-`game` units are plausibly SDK/middleware.
- **v1.10** (2026-10-17): G3 (`tools/interfaces.py`) implements the v1.9 source policy and two
  integration safeguards. (a) CP15 named-register variables (`register` storage, `__asm("cp15:...")`)
  are audited as ordinary declarations; `__asm` statements, blocks, assembly functions and
  core-register bindings (`__asm("r0")`) still fail. `__svc` declarations keep their ABI
  annotation. (b) The pinned compiler's standard C headers audit cleanly (`__nothrow`, format
  annotations, anonymous-union pragmas recognised; `__int64` normalised; `__softfp` kept); packing
  and unknown attributes/pragmas still fail closed. (c) A unit source containing an `INTEGRATOR:`
  marker comment (the authors' integration to-do marker) fails the audit. Shared declarations:
  `include/ctr/svc.h` (raw kernel ABI for every SVC that `__svc` can express; r4/r5 calls such as
  ControlMemory deliberately undeclared) and `include/ctr/cp15.h`. Cause: the ARMCC-extensions
  ruling, and c005's units that include <string.h> being marked unchecked.
- **v1.9** (2026-10-17): source policy follows the user ruling
  `docs/rulings/2026-10-17-armcc-extensions.md`: `__svc(N)` declarations, CP15 named-register
  variables and ARMCC intrinsics count as C (declared once in a shared header; each use a
  qualification on the function's review row); `__asm` statements/blocks and embedded-assembler
  functions stay refused. Reviewers apply it under G6 (`docs/REVIEWING.md` step 2). The G3
  interface parser does not yet accept named-register declarations (`__asm` raises an interface
  error), so no unit using them can pass prepare until a later revision adds that support.
  Also recorded: every subagent, both reviewers included, runs one model family
  (`docs/rulings/2026-10-17-reviewer-model-family.md`); the cross-family clause below remains
  "where possible".
- **v1.8** (2026-10-16): the linker model is part of acceptance. Builds use the armlink emulation
  `armlink5-inline-branchnop-v2` (`docs/LINKER.md`; independent review PASS_WITH_QUALIFICATIONS in
  `scratch/review/armlink-inline/REVIEW-v2.md`); `prepare` records `linker_model` and every
  emulated rewrite inside new or revised units (`linker_rewrites`), G4 requires both builds to use
  the same model, the history entry records the model, and reviewers check each rewrite's callee
  choice (`docs/REVIEWING.md` step 6).
- **v1.7** (2026-10-16): class/struct/union names are one project-wide namespace for C++ units.
  G3 compares every definition in a project header with same-named definitions in C++ units (ODR),
  and refuses a C++ forward declaration of a header-defined name in a unit that does not include
  that header (transitively). Cause: `c003-wave2-r2` review F01 — `include/game/screen.h`
  completed `struct Widget` as a 0x94-byte display object while accepted `game_heap.cpp` forward-
  declared a different 0x48-byte `Widget`; the v1.6 check read only `.cpp` text.
- **v1.6** (2026-10-16): (a) finalize is revision-aware — the drift check exempts exactly the
  prepare-declared `revised_units`/`revised_headers` (already pinned to their prepared hashes) and
  still refuses any other change to accepted files; before this, v1.4 revisions could never be
  finalized (`c003-wave2` review F02). (b) G3 refuses a local `extern "C"` declaration in a C++
  unit of an address-named symbol that a C unit or header also declares, or that two C++ units
  declare: shared symbols come from `include/` headers, because C++ declarations are not
  type-checked (`c003-wave2` F01, a C++ caller still using renamed types).
- **v1.5** (2026-10-16): a review binds exactly the prepared files — the current hashes of the
  sources and headers named in `prepare.json` — and lists headers added since prepare
  (`headers_added_since_prepare`) without being unbound by them. Finalize still requires the whole
  header tree to equal the prepared one. Cause: `c002-wave1-r2` — the integrator created two
  unrelated wave-2 headers while the second review ran, so its record hashed a different tree and
  finalize refused it (correctly under v1.4). r2 stays unfinalized; its successor gets fresh
  reviews. Integrator rule: no new files under `include/` while a checkpoint is under review.
- **v1.4** (2026-10-16): revised accepted material is reviewed again. `prepare` records
  `revised_units` (accepted units whose source differs from the last accepted entry) and
  `revised_headers`; G6 requires a verdict row for every function in a revised unit, as for new
  units. Cause: unifying wave 2 needed a correction to an accepted c001 prototype
  (`func_001795bc` takes no argument); without this, such an edit would be rebuilt but unreviewed.
- **v1.3** (2026-10-16): headers bound like sources. `prepare`, each review and the history entry
  record the SHA-256 of every project header (all files under `include/`, plus `.h/.hh/.hpp/.inc`
  under `src/`); G7 refuses a review bound to other header hashes or a header changed since
  prepare; G8 refuses an accepted header that changed or disappeared (new headers are allowed).
  Cause: `c002-wave1` review F02 — the shared `include/game/bss_0049ecf0.h` was unbound, so a
  header edit after review could have changed credited objects unnoticed.
- **v1.2** (2026-10-16): G0 clean tree. `prepare` refuses a working tree with any uncommitted or
  untracked non-ignored file and records the HEAD `commit`; reviews and finalize bind to that
  prepare. Cause: `c001-pilot-r3`'s suite ran against another agent's in-progress
  `tools/analyze.py` edit in the shared checkout and failed. Agents that edit tracked tools work
  in their own git worktree from now on. Test `tests/test_accept.py::test_prepare_refuses_a_dirty_tree`.
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
| G0 | Clean tree | `prepare` | any uncommitted or untracked non-ignored file; the HEAD commit is recorded |
| G1 | Denominator | `prepare` | `config/catalogue.json` is not the recorded denominator (`tools/denominator.py verify`) |
| G2 | Ledger composition | `prepare` | a candidate unit is already accepted |
| G3 | Interfaces | `prepare` → `accept/<cp>/interfaces.json` | any declaration/definition or struct-layout disagreement, const-contract violation or policy error across the **whole ledger**; C++ units unchecked unless `--allow-unchecked-cpp` (recorded; reviewers then check those units' interfaces by hand) |
| G4 | Two fresh builds | `prepare` | either `tools/build.py` build (fresh: no object cache) fails, or outputs differ. Each build: every credited function an ordinary catalogued function, exact size, equal to the original on its own before being written; no overlapping sections across units; no duplicate global definitions; `code.bin` equal to the original; recompressed `.code` equal to the stored ExeFS `.code` |
| G5 | Full suite | `prepare` | any module fails (`tools/test_suite.py`, sharded by module) |
| G6 | Review | `review` | the record fails the schema: verdict row (PASS/QUALIFIED/FAIL + method) for every new function and every function in a revised accepted unit, unique finding ids F01…, severities, verdict consistency (blocker or FAIL row ⇒ FAIL; QUALIFIED or major ⇒ not PASS; PASS_WITH_QUALIFICATIONS ⇒ stated qualifications) |
| G7 | Finalize once | `finalize` | prepare not PASS, fewer than `--min-reviews` (default 1) reviews bound to this exact prepare, any FAIL review, a review bound to other source or header hashes, sources/headers/profiles/catalogue/ledger changed since prepare, history broken |
| G8 | Ledger binding | `verify` (CI) | `config/matches.json` ≠ last finalized ledger, a credited source ≠ its accepted hash, an accepted header changed or removed, or credit without any checkpoint |

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
