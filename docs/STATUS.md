# Status

Current entry on top; older entries are kept as history.

## 2026-10-25 — c012-r6 accepted (+26 functions); 4.1 libraries; three user rulings

**Accepted:** 2,796 functions / 305,008 B (seq 13: … c011-r11, c012-r6). Denominator **v14**:
4,546 creditable functions / 1,120,094 B (**27.23%**). Gates **v1.20**.

- **c012-r6** combines ctypes1 r4 (63 revised accepted units, typed callback providers) with
  c012 r4 (new functions and 31 accepted moves). It adds 26 functions / 3,988 B.
  - r5 failed both reviews: two scene member-callback initialisers and a native font reader
    called through mismatched provider types, and the merge deleted 43 unrelated evidence files.
    r6 holds those 3 functions and restores the files. Both reviews PASS_WITH_QUALIFICATIONS
    with no findings.
  - Before prepare, Main moved two caller declarations into shared headers, and an
    adjudication agent fixed one genuine pointer/value contract defect (`func_001543b8`).
- **User rulings** (docs/rulings/):
  - explicit typed dispatch tables are allowed, under conditions;
  - unresolved catalogue bytes are resolved by documenting them as residue;
  - the Thumb library candidates waited for the ARM Compiler 4.1 libraries;
  - the linker model will be extended to Thumb branch-to-NOP through a gate revision.
- **ARM Compiler 4.1 libraries** (builds 791 and 894, MD5-verified downloads from Arm) are now
  local. The strict sweep proves `_ZdaPv` (`func_0019e08c`).
  - Two printf helpers wait for the Thumb linker model (weak-undefined call resolution).
  - `snprintf` waits for catalogue v15, which proves its callee `_snputc`.
- **In progress:**
  - c015 (19+ new functions, plus held-contract reconciliation);
  - catalogue v15, in review (suite passes);
  - the linker v19 implementation (Thumb weak-undefined calls);
  - a migration design for the scene and menu dispatch tables (87 functions unlocked by the
    ruling, less 7 PMF functions that stay blocked);
  - the residue table generator, which passed review and lands next;
  - permuter sweeps 11 and 12.

## 2026-10-23 (night) — denominator v14 adopted; resumed with 2 subagents

**Accepted:** 2,770 functions / 301,020 B (seq 12, unchanged). Denominator **v14** (history v11):
4,546 creditable functions / 1,120,094 B (**26.88%**). Gates **v1.20**.

- **Catalogue v14** classifies 25 functions as non-C, so the creditable count drops by
  25 functions / 1,580 B. Their extents, entries and every accepted record are unchanged.
  - Vendor library, proven against the ARM Compiler 5.04/5.06 runtime archives: `_snputwc`,
    `strcat`, `wcsncpy`, `__cxa_guard_acquire` and `wcslen`.
  - Handwritten assembly: the 20-member NW4C `FastWordCopyAsm_*` family. Each member's operation
    trace matches the public macro expansion.
  - Review: r1 FAIL on one finding (tests pinned totals). r2 PASS_WITH_QUALIFICATIONS with no
    open findings, including a fresh regeneration byte-identical to the proposal
    (`docs/catalogue/REVIEW-v14-r1.md`, `REVIEW-v14-r2.md`). Full suite: 1,109 tests passing on
    the adopted tree.
  - Unknown bytes (114 B in 11 intervals) and the single anomaly are unchanged.
- **Resumed** at the user's request with a cap of **2 concurrent subagents**.
  - c012-r5 combines ctypes1 r4 and c012 r4, merged onto main with their conflicts resolved.
    About 29 new functions / 4.8 KB and 31 moves; closure re-proof in progress.
  - The residue generator (`residue-table` branch) now reads permuter sweep-11. The draft table
    `RESIDUE-r4` lists 115 functions still missing step 4, down from 162.

## 2026-10-23 (evening) — c011-r11 accepted (+88 functions); pipeline paused

**Accepted:** 2,770 functions / 301,020 B (seq 12: … c010-r8, c011-r11). Denominator **v13-r2**:
4,571 creditable functions / 1,121,674 B (**26.84%**). Gates **v1.20**.

- **c011-r11** adds 88 functions / 12,440 B, folding in the c013 and c014 intakes, with explicit
  linker v6 plus literal L1, 9 accepted moves and 32 revised accepted units. Review history:
  - r8 failed Main's whole-ledger interfaces gate: a `ChoiceController` ODR clash and 8 inline
    declaration items. r9 fixed them.
  - r10: both reviews FAIL despite exact bytes. The findings were a memcpy prefix overlay, a
    one-past-end table cast, a cast of byte storage to a typed workspace, and two overlapping
    externs modelling one packet.
  - r11 fixed the first three compliantly, migrating the accepted initializer `0014db18` to the
    canonical typed root, and held `001ce05c` (276 B). Both reviews PASS_WITH_QUALIFICATIONS
    with no findings.
  - Each r11 reviewer carried 149 of 157 rows forward from its own sealed r10 review, after
    re-proving the matches and confirming the sources and include closures were byte-identical.
    This cut review time about 45%.
  - 117 functions stay held and uncredited (`docs/known-issues-c011.md`).
- **Pipeline paused at the user's request** after this checkpoint. The concurrency cap was 4
  subagents, to conserve usage.
- **Ready next, in order** (detail in `docs/RESUME.md`):
  1. **Catalogue v14:** reclassifies 5 vendor-library functions (`_snputwc`, `strcat`, `wcsncpy`,
     `__cxa_guard_acquire`, `wcslen`) and the 20-function NW4C `FastWordCopyAsm_*` assembly family
     as non-C. That is −25 creditable functions / −1,580 B, with accepted records unchanged.
     The full suite passes (1,104 tests). The r1 review FAILed on one major finding: its new
     tests pin aggregate totals instead of testing behaviour. Its accounting, protection and
     metadata checks all pass, and its fresh assembly-trace and regeneration replay is still
     incomplete. Revision v14-r2 and a second review come next.
  2. **Combined checkpoint c012-r5:** ctypes1 r4 (pre-review PASS) plus c012 r4 (29 new functions
     / 4,856 B and 30 moves, with cmodels2 folded in). ctypes1 merges cleanly; c012 r4 conflicts
     in about 70 files and needs a merge pass.
  3. **c015:** about 57 harvested author matches.
- **Blocked on the user:**
  - The remaining Thumb C-library family (about 50 functions) does not match the local ARM
    Compiler 5.04/5.06 libraries. It needs the 4.1 runtime libraries or a ruling.
  - CM2-F01/F03 (scene vtable and member-callback contracts in accepted code) need a ruling on
    the vtable key-function pattern.

## 2026-10-23 (later) — c010-r8 accepted (+413 functions); gates v1.20 adopted

**Accepted:** 2,682 functions / 288,580 B (seq 11: … c009-r6, c010-r8). Denominator **v13-r2**:
4,571 creditable functions / 1,121,674 B (**25.73%**). Gates **v1.20** (adopted 5ea97031,
between checkpoints).

- **c010-r8** adds 413 functions / 39,208 B with explicit linker v6 plus literal L1. It took three
  review rounds:
  - r6: both reviews FAIL. Array-runtime callbacks, envelope object identity, a BasicSound slot,
    a base-wrapper downcast, a C call into a native member, and a non-adopting provider.
  - r7: one review FAIL. Three more array-runtime callbacks.
  - r8: closed the class with an exhaustive runtime-callback table (array runtime, atexit, thread
    entries, native-member slots). Both reviews PASS_WITH_QUALIFICATIONS with no findings.
  - 94 functions / 8,404 B stay deferred and uncredited (`docs/known-issues-c010.md`).
- **Inherited attention:** the table also found 48 definite callback type/role mismatches, 7 custom-family
  rows and 26 unmodeled rows on providers accepted in c001–c009. ctypes1 r2 fixes 32 of them
  exactly; the rest are held with reasons.
- **Gates v1.20 adopted (exact reviewed text):** linker model `armlink5-inline-branchnop-v18` adds U1,
  removal of an unused deleting destructor from a single directly compiled object, with complete
  COMDAT/section-group closure.
  - The implementation passed review after v11–v17 each failed one round. The exact gate text
    passed on its second round.
  - GATES.md is byte-identical to the approved text. LINKER.md is the approved text plus main's
    disjoint catalogue sections (`scratch/integrator/gates-v1.20-adoption-evidence.json`, private).
  - Integrated suite: 1,092 tests, 0 failed modules; `accept.py verify` binds all 11 checkpoints.
  - This unblocks cmodels2 and the native MenuScene destructor family. Each needs explicit v18
    selection and reopens its affected R1 callers.
- **Ready for gates, in order:**
  - c011-r8: 79 functions / 11,080 B, with c013's intake folded in. Pre-review running.
  - ctypes1 r2: 55 units and 18 headers revised, 1 new function. Pre-review running.
  - c014 intake: step-5c matches. Being integrated.
  - c012: 71 functions, needs re-derivation.
- **Escalation:**
  - Permuter sweep-10 finished: 1,000 attempts. Its one match used steering and was later
    re-found naturally.
  - New step 5c, a third author pass on the 615 unaccepted functions within 64 bytes, has
    154 batches. The first ~50 produced about 20 matches; many residuals are 1–6 bytes of
    register-role or scheduling difference.
  - 184 author matches remain uncollected because of contract holds. They feed the residue table.

## 2026-10-23 — c009-r6 accepted (+648 functions); denominator v13-r2 adopted

**Accepted:** 2,269 functions / 249,372 B (seq 10: … cmodels, c009-r6). Denominator **v13-r2**
(history v10): 4,571 creditable functions / 1,121,674 B (**22.23%**). Gates **v1.19**.

- **c009-r6** adds 648 functions / 53,464 B (waves 15–20, retries, solos) with explicit linker v6
  plus literal L1. It took six rounds: each independent review that failed found cross-unit callable
  contracts that disagreed with the original program. These were callback type erasure,
  resource-dispatch tables, scene vtables, a completed font table typing an existing static table,
  converter slots that needed `float(float)`, a queue callback that drops its cursor argument, and
  an unauthorized DisplayTarget vtable. Each was fixed where an exact fix existed; otherwise only
  the new function was deferred. 28 functions / 3,588 B stay deferred and uncredited. Both r6
  reviews PASS_WITH_QUALIFICATIONS with no findings (`docs/known-issues-c009-r2.md`).
- **Ready for gates, in order:**
  - c010-r6: 424 functions / 40,480 B. Pre-review fixes applied; a targeted re-check is running.
  - ctypes1 (composed type models): 190 revised functions and 2 new. Interfaces clean; pre-review
    running.
  - c011-r4: pre-review findings resolved by deferral.
  - c012: 71 new functions / 12,472 B.
- **Blocked:**
  - cmodels2: two accepted scene/PMF contract findings need the native scene family.
  - The native MenuScene destructor family needs linker model U1, now at v14 after four review
    rounds that each found one major issue and no blockers.
- **Denominator v13-r2** adopted after c009-r6 finalized:
  - 90 owners reclassified as vendor runtime library, plus 6 more assembly owners.
  - Unknown bytes 4,408 → 114; anomalies 37 → 1.
  - All 2,269 accepted functions keep identical extents.
  - v13 failed its first review (two fail-closed gaps in the generator's inputs); v13-r2 passed
    with no findings.
  - Details: `docs/catalogue/REPORT-v13-r2.md`, `REVIEW-v13-r1.md`, `REVIEW-v13-r2.md`.
- **Escalation:** the step-5b second pass continues, with 108 batches queued. The permuter sweep
  (≤32-byte residuals) has 127 rows left and has found one match, which needs a natural
  rewrite because the permuter's version uses steering.

## 2026-10-18 (later) — cmodels accepted (linker v6 + L1); linker model v8 adopted

**Accepted:** 1,621 functions / 195,908 B (seq 9: … c008-r3, cmodels). Denominator **v9-r2**:
4,624 creditable functions / 1,131,454 B (17.31%). Gates **v1.19**. CI green.

- **cmodels** (20 functions + 3 revised accepted units, 16 helper-only units): the first
  checkpoint selecting opt-in models, `armlink5-inline-branchnop-v6` plus L1 string-record
  placement. Both reviews PASS_WITH_QUALIFICATIONS, no findings (`docs/known-issues-cmodels.md`).
- **Linker models:** v4 (bounded switch tables, GATES v1.17) and v6 (entry aliases, v1.18) were
  adopted; v8 (v6 plus removal of a removed helper's own exception-index entry with its complete
  COMDAT group, v1.19) was adopted after v7 failed review. Default stays v2/none.
- **Escalation step 5 (near-miss finishing):** a fresh author diagnoses each unmatched function
  within a few bytes of the original and makes the smallest natural change; so far roughly one in
  six such functions matches. A residual-family study found validated idioms (typed storage
  accessors, genuine same-TU callee visibility, helper argument order) and precise negatives
  (VFP load writeback, exclusive-store status register).
- **Next:** c009 (~678 functions), c010 (~548), c011 and a v8-selected model checkpoint;
  catalogue v10-r4 (raw-SVC repair plus 12 more runtime-library owners) is in review.

## 2026-10-18 — c008-r3 accepted; denominator v9-r2; runtime libraries identified

**Accepted:** 1,601 functions / 185,952 B (seq 8: … c007, c008-r3). Denominator **v9-r2**: 4,624
creditable functions / 1,131,454 B (16.43%). Gates **v1.16**. CI green.

- **c008-r3** (waves 12–14, retries r302–r343, solos, idivmod: 434 functions + 29 revised units):
  c008 and c008-r2 each failed review on cross-unit callback/header contracts; r3 corrected the
  original service callback slots, the auxiliary audio callback and the notification list from the
  original consumers and passed both reviews with qualifications (`docs/known-issues-c008-r3.md`).
- **Denominator v9-r2:** reviewed boundary/initializer/veneer recovery and shared UTF-16 bounds;
  unknown bytes 7,724 → 4,408, anomalies 70 → 37 (`docs/catalogue/REPORT-v9-r2.md`). The v10
  revision (unknown 360 B, 10 anomalies) is in its third review round.
- **Runtime libraries:** the game's own toolchain build (Arm Compiler 5.04 update 2, build 82) was
  obtained under the user's license (local only). Whole-body relocation-masked comparison
  authenticates 36 of the 54 Thumb functions and 68 more owners as vendor runtime code, plus 87
  helper names; the classification goes through the next reviewed catalogue revision.
- **Next:** adopt the reviewed linker models v4 (bounded switch tables) and v6 (zero-size D2
  destructor aliases; v5 retired); c009 (679 functions) and c010 (438) are integrating, c011 has
  started; a dedicated checkpoint will credit model-qualified rows; the escalation ladder (fresh
  author → whole-TU rewrite → permuter) continues over ~1,550 unmatched functions.

## 2026-10-17 (late night) — c007 accepted: wave 11, SDK/middleware, NW4C library imports

**Accepted:** 1,167 functions / 73,388 B (seq 7: … c006-r3, c007). Denominator **v8**: 4,544
creditable functions / 1,129,400 B (6.50%). Gates **v1.13**. CI green.

- **c007** (297 functions + 33 revised accepted functions): wave 11, large batches L004–L012,
  CTR-SDK IPC wrappers (natural address-taking marshalling), ARMCC 4.1 `sdk`/`middleware`
  recoveries, and NintendoWare4Ctr library code adapted from the GPL-3.0 public decompilation
  (`THIRD_PARTY.md`). First prepare passed; both reviews PASS_WITH_QUALIFICATIONS
  (`docs/known-issues-c007.md`).
- **Next:** gate revisions v1.14–v1.16 (string-record placement L1, removed-inlined-callee linker
  model R1, accepted-function rehoming; each independently reviewed) land before c008; c008
  (waves 12–14 + retries, ~440 functions) is integrating; escalation batches (best attempt + diff)
  are re-authoring ~600 near misses; denominator v9 (runtime/library classification, boundary
  fixes, unknown bytes/anomalies) is being built.

## 2026-10-17 (night) — c006-r3 accepted; denominator v8; every function attempted

**Accepted:** 870 functions / 50,012 B (seq 6: … c005-r2, c006-r3). Denominator **v8**: 4,544
creditable functions / 1,129,400 B (4.43%). Gates **v1.13**. CI green.

- **c006-r3** (waves 8–10, L001–L003, SDK batches x01–x11: 381 functions + 10 revised units):
  c006 failed prepare on a broken test fixture; c006-r2 failed one review (one unit used private
  sound types that conflict with the canonical `AudioChannel`/`AudioSlot`); r3 fixed it, swept all
  88 C++ units against their canonical headers, and passed both reviews with qualifications
  (`docs/known-issues-c006-r3.md`).
- **Denominator v8:** compiler-runtime/kernel-stub classification under a stated evidence standard,
  recovered leading instructions, two more tail-call splits; first review failed (R01–R06),
  re-review passed with qualifications (`docs/catalogue/REPORT-v8.md`, `REVIEW-v8*.md`).
- **Bindings:** `__aeabi_ldivmod` and `__aeabi_idivmod` proven by whole-function matches.
- **Harness (opt-in, not yet gates):** removed-inlined-callee linker model (three reviews; last
  PASS_WITH_QUALIFICATIONS), anonymous string-record placement (review PASS_WITH_QUALIFICATIONS),
  accepted-function rehoming (in review); permuter fixes; full-suite rule before every prepare.
- **Authoring:** every creditable function has now had at least one attempt or is in flight.
  ~965 further author matches (~152 KB) await checkpoints c007 (320 functions, ready) and c008
  (integrating); ~850 mismatches enter the escalation steps (census in private scratch).
- **Rulings:** subagent fallback to Sonnet 5.5 on a Codex usage limit
  (`docs/rulings/2026-10-17-subagent-model-fallback.md`).

## 2026-10-17 (evening) — c005-r2 accepted; SDK/middleware compiler found; library reuse

**Accepted:** 489 functions / 19,288 B (seq 5: … c004-r3, c005-r2). Denominator **v7**: 4,557
creditable functions / 1,129,946 B (1.71%). Gates **v1.13**. CI green.

- **c005-r2** (waves 5–7, 190 functions + 34 revised units): c005 failed both reviews on three
  dropped caller-evidenced arguments; r2 fixed them, rejected one finding with caller evidence that
  both reviewers re-derived, and swept 351 contracts (`docs/known-issues-c005-r2.md`).
- **Compiler:** CTR-SDK and middleware were built with ARMCC 4.1; frozen profiles `sdk` (-O2) and
  `middleware` (-O3) on build 791 (`docs/COMPILER.md`), from a 352k-compile study.
- **Reuse (GPL-3.0, `LICENSE`, `THIRD_PARTY.md`):** public NW4C decompilation adapted with
  `tools/fingerprint.py` (115 functions confirmed so far, pending checkpoints).
- **Harness:** caller-contract audit `tools/argcheck.py` (review step 7), Ghidra author aid
  `tools/decompile.py`, G3 fixes (standard headers, CP15 registers, const provenance,
  namespace-aware C++ identity), runtime/C-library bindings, case-insensitive dependency paths.
- **In flight (private):** c006 (waves 8–10 + SDK batches, ~380 functions) and c007 (wave 11,
  large functions, SDK/IPC template, NW4C) integrating; ~60 parallel authors including 16–45 KB
  static-initializer functions (two already match: 0x1f7d7c 44,844 B, 0x209ec8 16,752 B);
  denominator v8 under revision after a failed review.

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
