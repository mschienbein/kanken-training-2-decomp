# Status

Current entry on top; older entries are kept as history.

## 2026-10-17 — WSL desktop: c004-r3 accepted; harness widened for parallel authoring

**Accepted:** 299 functions / 7,620 B (seq 4: c001-pilot-r4, c002-wave1-r3, c003-wave2-r3,
c004-r3). Denominator **v7**: 4,557 creditable functions / 1,129,946 B (0.674%). Gates **v1.10**.
Linker model `armlink5-inline-branchnop-v2`. CI's ledger check is green again.

- **c004-r3** (waves 3+4, 196 functions + 6 revised units): c004-r2 was prepared on WSL (first
  Linux-x86_64 prepare; both fresh builds byte-identical) and FAILed `review-c004-second`
  (func_001f4fac dropped two caller-evidenced arguments). r3 swept all 240 empty/wrapper/callee
  contracts against 3,270 call sites and 214 pointer uses (12 changed) and passed both reviews
  without findings (`docs/known-issues-c004-r3.md`).
- **Rulings (user):** all subagents incl. both reviewers run one model family
  (`docs/rulings/2026-10-17-reviewer-model-family.md`); documented ARMCC extensions — `__svc`,
  CP15 named registers, intrinsics — count as C (`docs/rulings/2026-10-17-armcc-extensions.md`,
  gates v1.9/v1.10; shared declarations in `include/ctr/svc.h`, `include/ctr/cp15.h`).
- **Denominator v7:** 8 more armlink tail-call boundaries (VFP pass-through tails, restored-frame
  setters), independently reviewed (`docs/catalogue/REPORT-v7.md`, `REVIEW-v7.md`).
- **Harness:** packet/match display bounded by catalogue code ranges; wibo `Z:/` dependency paths
  (system headers crashed the object cache on Linux); odd data literals no longer shown as Thumb
  pointers; `match.py --names` proposed bindings; runtime-helper and C-library bindings in
  `config/names.json`, each proven by a whole-function match (`__aeabi_uidivmod`,
  `__aeabi_memcpy/memcpy4`, `__aeabi_memclr/memclr4`, `__aeabi_memset`, `strlen`,
  `__aeabi_vec_ctor_nocookie_nodtor`); G3 audits standard headers and CP15 named registers and
  refuses leftover `INTEGRATOR:` markers; source permuter `tools/permute.py` (`docs/PERMUTER.md`).
- **In flight (private, not accepted):** c005 = wave 5 (37) + waves 6/7 being unified in a
  worktree; waves 8–10, large-function and ARMCC-extension batches authored by ~20 parallel
  agents (≈300 further candidate matches so far); middleware/library ownership census.

## 2026-10-16 (night) — Mac session ends; c004-r2 ready to prepare on the desktop

**Accepted:** 103 functions / 5,056 B (seq 3). Denominator **v6**: 4,549 creditable functions /
1,129,946 B (0.445%). Restart steps: `docs/RESUME.md` top entry.

- **c004** (waves 3+4, 196 functions + 6 revised units): prepare PASS; `review-c004-openai`
  PASS_WITH_QUALIFICATIONS (F01 minor: undisclosed preserved null dereferences);
  `review-c004-second` FAIL (five wrapper/callee interface types inferred too weakly). All six
  findings are fixed on `main` (all functions still MATCH; `docs/known-issues-c004-r2.md`); the
  successor `c004-r2` needs prepare + two fresh reviews. Until it finalizes, CI's ledger check is
  red by design (accepted units revised by c004's interface unification).
- **Wave 5:** 37/80 matched, collected on branch `c005-wave5` with its interface to-do list
  (`docs/wip/c005-INTEGRATION.md`).
- **Catalogue v6:** all 270 armlink tail-call NOP sites classified; 129 merged boundaries split
  (e.g. 0x112c80 | 0x112c98); every v5 start/entry preserved; accepted/c004/c005 unchanged.
- Session totals since the first accepted checkpoint: 5 author waves (≈340 functions attempted,
  ≈340 claimed matches incl. wrappers; 336 now in accepted + c004 + c005), armlink emulation,
  catalogue v3→v6, gates v1.2→v1.8. Review rounds found real defects every time (ODR, type
  collisions, gate bugs, weak signatures); each became a mechanical check where possible.

## 2026-10-16 (evening) — c003 accepted; armlink emulation; catalogue v5; c004 integrating

**Accepted:** c001 (17) + c002 (38) + c003 (48 new, 13 revised) = **103 functions / 5,056 B**
(0.455% of denominator v5: 4,350 creditable functions / 1,111,934 B). History seq 3.

- **armlink link-time rewrites emulated** (`docs/LINKER.md`, model `armlink5-inline-branchnop-v2`):
  calls to return-only functions → `mov r0, r0` (505 sites), calls to some one-instruction
  functions → the instruction, tail branches to the next function → `mov r0, r0` (270 sites); the
  compiler's own `nop` words are a separate population. Found from an author's mismatch report;
  implemented by an Opus agent, reviewed independently (v1 FAIL: same-object callee bypass; v2
  PASS_WITH_QUALIFICATIONS). Gates v1.8 record the model and every rewrite.
- **Catalogue v4/v5:** the "NOP entries" are C tail-call wrappers (86) — now ordinary functions;
  pointer-only empty functions and a pointer-tail gap recovered; every v3 start/entry preserved.
- **Gates v1.6–v1.8** from c003's reviews: revision-aware finalize, C++ local redeclaration
  refusal, header class names one namespace for C++ (header ODR + forward-declaration collisions),
  linker model binding.
- **Throughput:** wave 3 66/80 claimed; wave 4 130/155 (94 wrappers/empties + 36 bodies) under
  the new model. Integration (unifying declarations across independent authors) is now the
  bottleneck: c004 needs 29 interface fixes before prepare.
- **Process slips (both corrected):** a chained `;` produced a false "ACCEPTED" commit message
  (3ab0516); a `suite | tail && push` pushed a merge whose suite had one failing test module
  (fixed within minutes in the next commit). Rule now: capture exit codes, never pipe a gate.

In flight: c004 interface unification (agent, worktree `c004-unify`); wave 5 (4 authors × 20).

Open policy question for the user (unchanged): `__svc(N)` and named-register CP15 variables as C.

## 2026-10-16 (afternoon) — c002 accepted; wave 2 under review; wave 3 authoring

**Accepted:** c001-pilot-r4 (17) + c002-wave1-r3 (38) = **55 functions / 2,680 B (0.242%)** of the
v3 denominator (4,231 creditable functions / 1,108,426 B). History seq 2.

- **Catalogue v3 adopted** (denominator v3): in-section string literals, 114 multi-entry routines
  classified non-C with recorded entries; unknown bytes 39,338 → 8,836. Remaining modelling
  issues: `docs/catalogue/V4-BACKLOG.md` (NOP entries).
- **Throughput:** H11 pilot 4/10 (`docs/pilot-h11.md`); wave 1 35/60 (re-derived 34 + 4 pilot);
  wave 2 48/80 claimed and re-derived. Ordinary game C matches in 1–5 attempts; misses are SDK
  svc/CP15 wrappers (policy pending), C runtime, catalogue gaps and register-allocation near misses.
- **Gates v1.3–v1.5** (`docs/GATES.md`), each from a real defect: headers bound like sources
  (c002 F02); C++ class definitions must agree across units (ODR, c002 F01); revised accepted
  units are re-reviewed; reviews bind exactly the prepared files.
- **Integration tools:** `tools/collect.py` (re-derives author matches at their destination),
  `tools/triage.py --avoid svc,thumb,exclusive`, shared declarations in `include/game/`.
- **Correction:** commit `3ab0516` says "c002-wave1-r2 ACCEPTED" but finalize had refused it (a
  review was bound to a header tree containing wave-2 headers created during the review); the
  commit only added the review records. Fixed by gates v1.5 and successor r3. Cause of the wrong
  message: commit chained after finalize with `;` instead of `&&`.

In flight: c003-wave2 (48 new + 3 revised functions) under two reviews; wave 3 (80 functions,
three authors).

Open policy question for the user (unchanged): `__svc(N)` and named-register CP15 variables as C.

## 2026-10-16 — decompiling on the Mac: first accepted checkpoint (c001-pilot-r4)

**Accepted:** 17 functions / 1,096 B (0.096% of 1,136,354 code bytes; denominator v2, 4,437
creditable functions). History: `docs/progress-history.jsonl` seq 1; record `accept/c001-pilot-r4/`;
known issues `docs/known-issues-c001-pilot-r4.md`. Two independent reviews (OpenAI family and a
separate reviewer agent) each re-derived 17/17 MATCH.

Since the laptop harness entry:
- Compiler frozen (H5): profile `game` = ARMCC 5.04 `-O3 -Otime` ARM (`docs/COMPILER.md`).
- Catalogue v2 adopted (denominator v2): 61 mid-function-pool / branch-into-body merges, noreturn
  inference; 4,443 starts, 197 anomalies (`docs/catalogue/REPORT-v2.md`).
- Oracle hardening: every global definition bound to its address (unbound globals refused,
  cross-unit duplicates refused); ARMCC `__switch$$` labels; fixed-placement mismatches reported as
  bytes; compid fails closed per source; packet negative PC literal fix.
- Gates v1.2 (`docs/GATES.md`): G0 clean tree + commit binding, interface check (G3), two fresh
  builds, sharded suite, structured review records, single finalize, CI ledger binding.
- Speed (Shiren lessons 15–22): parallel compile, exact-input object cache, sharded suite
  (~50 s, 178 tests), detached jobs (`tools/jobs.py`), triage + batches (`tools/triage.py`).
- Process record: c001-pilot (record defect), c001-pilot-r2 (review FAIL: int vs config pointer),
  c001-pilot-r3 (suite against another agent's in-progress edit) — all kept, never finalized.

In flight: H11 agent pilot (10 stratified functions; AuthorPilotB 3/5 MATCH), wave 1 (3 authors ×
15 functions), catalogue v3 (in-section string literals, NOP entries, multi-entry routines,
missing tail-branch targets).

Open policy question for the user: may ARMCC `__svc(N)` declarations and named-register
variables (`__asm("cp15:...")` bindings) count as C? They are needed for CTR-SDK-style wrappers
and some inlined game code (e.g. 0x106afc). Until ruled, nothing using them is credited.

## 2026-10-15 (final laptop entry) — oracle review PASS_WITH_QUALIFICATIONS; ready to decompile

- Four review rounds by `astra-reviewer` (records in `docs/reviews/`): round 1 FAIL (11 findings),
  round 2 FAIL (5), round 3 FAIL (1: COMDAT identity), round 4 **PASS_WITH_QUALIFICATIONS** with
  no remaining findings. Every finding has a regression test; suite **66 tests OK**.
- Qualifications carried forward (D-gate scope): `config/matches.json` is trusted as the accepted
  ledger (bind units to accepted receipts in an acceptance tool); receipts do not bind transitive
  headers; `.ARM.exidx`/unwind and a full original-armlink image are unproven; RomFS lower IVFC
  levels are not traversed (whole-CCI pin covers the supplied dump).
- Process deviation: the integrator re-ran one round-3 probe in place, rewriting one sealed result
  file (disclosed in the r3 record; round 4 re-derived the case in fresh directories).
- Next: `docs/RESUME.md` on the Windows desktop.

## 2026-10-15 (late) — independent oracle review: FAIL → all findings fixed

- `astra-reviewer` found two false-PASS blockers (aliased data sections across/within units;
  overlapping catalogue extents concealing a wrong function) and nine major/minor relocation,
  naming, eligibility and robustness issues. All fixed with regression tests; the reviewer's
  reproducers now fail closed. Record and dispositions: `docs/reviews/2026-10-15-harness-oracle-review.md`.
- Post-fix receipts PASS: `build/2026-10-15-laptop-baseline-zero-c-r2`,
  `build/2026-10-15-laptop-pilot-candidate-r2` (10 functions / 280 B). Suite: 60 tests OK.
- C denominator now excludes classified kinds (6 veneers): 4,498 functions / 1,136,318 B.
- Round-2 review of the fixes requested; its verdict goes to the same review file when sealed.

## 2026-10-15 (late) — harness ready for decompilation; moving to the Windows desktop

Harness gates (`docs/ACCEPTANCE.md`):

| Gate | State | Evidence |
|---|---|---|
| H1 input identity | done | `config/title.json`; `rom.py` refuses unknown dumps (test) |
| H2 extraction | done | 51 files, hash chain verified; encrypted+boot9 and decrypted inputs give identical trees |
| H3 zero-C baseline | done | `build/2026-10-15-laptop-baseline-zero-c/receipt.json` PASS (`code.bin` + `.code` exact) |
| H4 denominator | done (v1) | `config/catalogue.json` 4,504 functions / 1,136,366 B; `docs/denominator-history.jsonl` v1; 39,470 unknown text bytes and 350 boundary anomalies recorded for revision |
| H5 compiler | partial | ARMCC 5.04 `-Otime` leading; 10 probe functions match; `-O2`/`-O3` and C++/library flags open (`docs/COMPILER.md`) |
| H6 match loop | done | `match.py`, `compid.py`, `packet.py`; strict `inplace.py` with tests; independent review PASS_WITH_QUALIFICATIONS (round 4) |
| H7 integration | done (candidate) | `build/2026-10-15-laptop-pilot-candidate/receipt.json` PASS: 10 functions / 280 B from C |
| H8 progress | done (stub) | `ci/progress_report.py` + workflow; ByteMatch needs a public repo |
| H9 tests | done | 66 tests (`python -m unittest discover -s tests -t .`) |
| H10 docs | done | README, AGENTS, ACCEPTANCE, COMPILER, BYTEMATCH, RESUME |
| H11 pilot | open | agent pilot of ~10 stratified functions on the desktop |

Accepted progress: none (0 functions). The 10 pilot functions are candidates in
`scratch/pilot/` (private) until reviewed and adopted.

## 2026-10-15 — harness bootstrap (H-gates in progress)

- Inputs: encrypted cartridge dump (sha256 `23ba7aaf…`) and a decrypted copy (`5874c3af…`) both
  import to identical outputs. Decryption uses pyctr 0.7.6 with the user's own `boot9.bin`; every
  byte is checked against the NCCH hash chain (exheader hash, ExeFS superblock and per-file hashes,
  RomFS superblock hash).
- `code.bin`: 1,327,104 B (`a367b5dc…`); text 0x1175F4 B @ 0x100000, rodata 0x1E0B8 B @ 0x218000,
  data 0xC908 B @ 0x237000, BSS 0x29F9E4 B. No CROs. RomFS: 46 files, 115.8 MB (archives
  `arcApp*Dat.bin`, `arcJitenPrmDat.bin`, `arcSikenPrmDat.bin`, fonts, audio, shaders).
- **ExeFS `.code` recompression is byte-exact:** our backward-LZ77 encoder (3dstool's greedy policy)
  turns `code.bin` back into the stored `.code` byte for byte.
- SDK tags from the plaintext NCCH plain region: CTR-SDK 11.5.2, NW4C 3.7.5, PUX HWR 2.0.1.
- Code is mostly ARM with heavy VFP use; Thumb is rare. No assert strings or source paths survive.
