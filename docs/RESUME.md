# Resume

Current entry on top.

## 2026-10-26 — c015 accepted; held-contract residue ruling; 16 subagents (current)

**State:** `docs/STATUS.md` top entry. Accepted 2,827 functions / 309,728 B (seq 14, c015);
denominator v16 (4,545 / 1,120,082 B); gates v1.20. **Concurrency cap: 16 subagents** (user,
2026-10-25), all on the `subagent` role (`openai-codex/gpt-6.1-sol:max`). The user asked to clean up
after every batch to save disk space.

Live state: `scratch/integrator/PIPELINE-LIVE-20261023.md`, top sections. After a kernel loss, reload
`scratch/integrator/rotation.py` and then `scratch/integrator/main_helpers.py`, both by absolute
path. Open user questions: `scratch/integrator/USER-QUESTIONS-PENDING.md`.

Remaining, in order:
1. **c016** (branch c016, C016Intake): merge post-c015 main, whole-ledger interfaces, argcheck
   (baseline = c015's main argcheck), prepare, two reviewers, finalize.
2. **Permuter rows:** sweep-11 (13 rows) and sweep-13 (4 rows), in heavy-slot gaps.
3. **Final residue:**
   - run `scratch/integrator/held-ruling/build_held_ruling.py --apply` on the final RESIDUE draft;
     it writes the companion `docs/rulings/2026-10-25-held-contract-residue.json` and the user-ruling
     holds;
   - regenerate the table and put its STATUS-ready section in STATUS (unknown-byte ranges and the
     anomaly per the 2026-10-25 ruling);
   - push with CI green.

## 2026-10-25 — c012-r6 accepted; catalogue v15 and v16 adopted; 16 subagents

**State:** `docs/STATUS.md` top entry. Accepted 2,796 functions / 305,008 B (seq 13, c012-r6);
denominator v16 (4,545 / 1,120,082 B); gates v1.20. **Concurrency cap: 16 subagents** (user,
2026-10-25), all on the `subagent` role (`openai-codex/gpt-6.1-sol:max`). The user asked to clean up
after every batch to save disk space.

Live state: `scratch/integrator/PIPELINE-LIVE-20261023.md`, top sections (dated 2026-10-25).
After a kernel loss, reload `scratch/integrator/rotation.py` and then
`scratch/integrator/main_helpers.py`, both by absolute path.

Done since the c012-r6 finalize: catalogue v15 and v16 adopted; residue table generator
(`tools/residue.py`) landed.

Next, in order:
1. **c015:** merge main and fold in recon-small (4 functions), then argcheck, prepare and two
   reviewers.
2. **Scene native-class migration:** genuine C++ virtual classes and member pointers, from
   `scratch/integrator/dispatch-plan/NATIVE-CLASS-BRIEF.md`; 87 functions.
3. **Held-contract waves**, from `scratch/integrator/contract-waves/WAVES.json`; also check
   `scratch/integrator/native-class-survey/` for the 22 blocked small families.
4. **Linker v19** (Thumb weak-undefined calls; branch `linker-thumb`): only if the user accepts the
   inferred NOP encoding or supplies an armlink licence (question Q1 in
   `scratch/integrator/USER-QUESTIONS-PENDING.md`).
5. Permuter sweeps 11 and 12; final residue table in STATUS.

## 2026-10-23 (night) — resumed with 2 subagents; denominator v14 adopted

**State:** `docs/STATUS.md` top entry. Accepted 2,770 functions / 301,020 B (seq 12, c011-r11);
denominator **v14** (4,546 / 1,120,094 B); gates v1.20. Same host and inputs as below.
**Concurrency cap: 2 subagents**, nested ones included (`scratch/integrator/rotation.py`
ACTIVE_TARGET=2). Pipeline agents take slots before authors.

Live state: `scratch/integrator/PIPELINE-LIVE-20261023.md`, top section "RESUMED". Order:
1. **c012-r5** = main + ctypes1 r4 + c012 r4, on branch `c012-r5` (worktree
   `scratch/worktrees/c012-r5`, base eb37e93e). C012R3Codex is doing the merge, the combined
   manifest and the closure re-proof.
   - Then Main runs `policy_lint.py`, interfaces, argcheck and prepare, followed by two reviewers.
   - Before prepare, re-check that the committed manifest claims none of v14's 25 non-C owners.
2. **c015 intake** from `scratch/integrator/HARVEST-LIST.json` and RESIDUE pending_intake.
3. **Ladder:**
   - a census supplement for rungs 1–3 (today's fa and tr batches);
   - author batches for the remaining missing rungs;
   - sweep-11 chunks (local, heavy slot).
4. **Final accounting:**
   - land `tools/residue.py` (`residue-table` at 667c2d5b, reviewed independently first);
   - publish the residue table and the list of items blocked on the user in STATUS.

## 2026-10-23 (evening) — c011-r11 accepted; PAUSED by the user

**State:** `docs/STATUS.md` top entry. Accepted 2,770 functions / 301,020 B (seq 12, c011-r11);
denominator v13-r2; gates v1.20. Same host and inputs as below. No new subagents are started;
the catalogue-v14 review was the last one in flight at the pause. The
pause was requested to conserve usage; the last concurrency cap was **4 subagents**, nested
agents included. Ask the user what cap applies before resuming.

Resume in this order. The live detail is in `scratch/integrator/PIPELINE-LIVE-20261023.md`; see
its PAUSE section and checkpoint table.
1. **Catalogue v14** (branch `catalogue-v14` at 5b8f88dd, worktree `scratch/worktrees/catalogue-v14`).
   - The r1 review (`scratch/review/catalogue-v14/codex/`) FAILed on one major finding, F01:
     `tests/test_catalogue_v14.py` pins 11 aggregate totals and 2 prose strings. Accounting,
     protection, metadata consumers, input refusals and history reachability all passed.
   - Its single replay stopped on a reviewer-side script assertion, so the fresh assembly-trace
     comparisons and the public regeneration are still owed.
   - Next: revive CatalogueV14Codex for v14-r2 (behavioural tests), then an r2 review that
     completes the replay with one new heavy grant.
   - On PASS, adopt between checkpoints: merge the branch, check `tools/denominator.py verify`
     and the suite, then update STATUS.
2. **Combined checkpoint c012-r5** = main + ctypes1 r4 (`types-compose-r3` at 93921aa2, merges
   cleanly) + c012 r4 (`c012-r4` at 4a9cf7b5, about 70 conflicts against main).
   - Revive C012R3Codex to merge, resolve semantically, combine the candidate JSON, and re-prove
     the closure.
   - Then run Main's `scratch/integrator/policy_lint.py`, interfaces, argcheck and prepare,
     followed by one review pair.
   - The review contract may reuse the r11 delta pattern
     (`scratch/integrator/contracts/c011-r11-review-contract.md`) only for an r+1 round.
   - Main ruled that CM2-F01/F03 are known issues in accepted code, not c012 blockers.
3. **c015 intake** from `scratch/integrator/HARVEST-LIST.json` (57 entries).
4. **Escalation ladder:**
   - `scratch/integrator/queue.json` (`rotation.py fill()`, about 58 batches);
   - permuter sweep-11 chunks 4 onward (delete `scratch/permute/sweep-11/STOP-AFTER-ROW.json`
     first).
5. **Final accounting:**
   - land `tools/residue.py` (branch `residue-table` at 3853f79d);
   - regenerate the residue table after the last checkpoint;
   - publish the STATUS residue section and the list of items blocked on the user (ARM Compiler
     4.1 runtime libraries; the CM2 vtable ruling).

## 2026-10-23 (later) — c010-r8 accepted; 16-subagent cap

**State:** `docs/STATUS.md` top entry. Accepted 2,682 functions / 288,580 B (seq 11, c010-r8);
denominator v13-r2; gates v1.20 (adopted 5ea97031, linker v18 available). Same host and inputs as below.

Changes to the restart sequence below:
- **Concurrency:** the user raised the cap to **16 concurrent subagents**, nested agents included.
  Every task still says "never use the task tool". Keep two slots free when a checkpoint's
  reviewers are about to spawn.
- **Live state:** `scratch/integrator/PIPELINE-LIVE-20261023.md`, which lists branch heads, agents,
  heavy-slot grants, the step-5c pool and the harvest list. Matches waiting for an intake are in
  `scratch/integrator/HARVEST-LIST.json`.
- **Authors:** `scratch/integrator/rotation.py fill()` launches from `queue.json`. The `nm*` prefix
  is the step-5c near-miss third pass (pool in `scratch/integrator/STEP5C-POOL-r1.json`), and it
  runs ahead of the remaining `np*` batches.
- **Next order** (GATES v1.20 and linker v18 are adopted):
  1. c011-r8 (folds in c013).
  2. ctypes1 r2.
  3. c014 intake.
  4. c012 re-derivation.
  5. cmodels2 and the native MenuScene family under v18.

## 2026-10-23 — c009-r6 accepted; 8-subagent cap

**State:** `docs/STATUS.md` top entry. Accepted 2,269 functions / 249,372 B (seq 10, c009-r6);
denominator v9-r2; gates v1.19. Same host and inputs as below.

Changes to the restart sequence below:
- **Concurrency:** the user capped this run at **8 concurrent Codex `gpt-6.1-sol:max` subagents**,
  nested agents included. Every task text says "never use the task tool". Main cannot cancel a
  subagent's children; only cancelling the parent works.
- **Live state:** `scratch/integrator/PIPELINE-LIVE-20261023.md` (branch heads, agents, heavy-slot
  queue). `scratch/integrator/PIPELINE.md` points to it.
- **Permuter sweep-10:** run by Main without an agent. Run `driver.py request`, write
  `HEAVY-SLOT-GRANTED-N.json` with the request sha, then `driver.py run-chunk N`. A gate can
  pre-empt a chunk via `STOP-AFTER-ROW.json` (delete it afterwards).
- **Next checkpoint order:** catalogue v13-r2 adoption, then c010-r6, ctypes1, c011-r4 and c012.
  cmodels2 and the native MenuScene family wait for linker U1 (v14 in review).

## 2026-10-20 — resumed WSL desktop

**State:** `docs/STATUS.md` top entry. Accepted 1,621 functions / 195,908 B (seq 9, cmodels);
denominator v9-r2; gates v1.19. Host: WSL2 Ubuntu on the Windows desktop
(`~/code/kanken-training-2-decomp`; private inputs in `~/code/3ds-experiments/inputs/`, imported
into `orig/`; `baserom.3ds` is an ignored symlink to the decrypted dump). Git pushes through
`gh auth setup-git`.

Restart sequence: linker models v4/v6/v8 are adopted. The finalized ledger selects linker v6
plus literal L1, so later checkpoints explicitly select at least that pair. Frozen profiles:
game/sdk/middleware. Resume preserved work rather than restarting completed batches.
The user-set maximum and active target are 64 **Codex `gpt-6.1-sol:max`** subagents;
keep useful slots full and pause dispatch if Codex usage is exhausted, without silently
switching providers (`docs/rulings/2026-10-20-subagent-codex.md`). Current queue and ownership
live in the private pipeline file.

1. Read the integrator's private live state, `scratch/integrator/PIPELINE.md` (checkpoint stage,
   agents in flight, batch queue, exclusions, retry pool, residue), and the author rules
   `scratch/integrator/AUTHOR-BRIEF.md`. Private scratch does not travel between machines.
2. Checkpoint loop: collect authors' `RESULT.json` in a worktree (`tools/collect.py`), unify
   declarations into `include/` (`tools/interfaces.py check` on the dry ledger: zero errors and zero
   policy items), pre-review, merge after the previous checkpoint finalizes, then
   `tools/jobs.py run --name <cp>-prepare -- .venv/bin/python tools/accept.py prepare ...`; check
   `exit_code` 0 **and** `prepare.json` PASS; two fresh reviews (`docs/REVIEWING.md`); finalize
   `--min-reviews 2`; `accept.py verify`; known issues + STATUS/RESUME; commit; push.
3. Authoring: `tools/triage.py --exclude scratch/integrator/exclude.json --avoid svc,thumb,exclusive
   batches`; functions >512 B in focused batches, >2 KB solo; ARMCC-extension functions use
   `include/ctr/`. Near misses go through `tools/permute.py` (CPU only) and step-5 near-miss
   finishing (a fresh author diagnoses the residual) before they enter the residue table.
4. Next: c009 prepare and reviews (selection linker v6 + literal L1, the finalized ledger's pair),
   then c010, c011 and a v8-selected model checkpoint (cmodels2); catalogue v10-r4 review and
   adoption (runtime-library classification, raw-SVC repair); residue table in STATUS.


## 2026-10-16 (night) — hand-off from the Mac to the desktop

**State** (details: `docs/STATUS.md` top entry): accepted **103 functions / 5,056 B**
(c001-pilot-r4, c002-wave1-r3, c003-wave2-r3; history seq 3). Denominator **v6** (4,549
creditable functions / 1,129,946 B). Gates **v1.8**. Linker model `armlink5-inline-branchnop-v2`.

**CI on `main` is red, and that is expected:** `accept.py verify` refuses accepted sources that
checkpoint c004 revises (interface unification) until a c004 successor finalizes. It turns
green at step 3 below. Do not "fix" it by reverting the revisions.

Setup: the Windows steps in the 2026-10-15 entry below, with current expected values (catalogue
hash, test count, accepted-ledger build) in the private `3ds-experiments` repo:
`context/session-handoffs/mac-to-pc-2026-10-16.md`. Private inputs come from
`3ds-experiments/inputs/`, never committed. That entry's "both private", test count,
catalogue hash and `pilot.json` are history.
`scratch/` was not transferred (author results, packets, review evidence stay on the Mac).

Restart sequence (accepted now: 489 functions / 19,288 B, seq 5 c005-r2; gates v1.13; profiles game/sdk/middleware):

1. **c004-r2** (196 new functions from waves 3–4 + 6 revised accepted units; `main` already has
   both reviews' fixes and `docs/known-issues-c004-r2.md`):
   `tools/jobs.py run --name c004-r2-prepare -- <python> tools/accept.py prepare --checkpoint c004-r2 --candidates config/candidates/c004.json --allow-unchecked-cpp`.
   This is also the first cross-host run of the gates: both fresh builds must still produce the
   original image, as on the Mac. `jobs.py run` returns 0 as soon as the job starts, and a failed
   prepare writes the same `scratch/jobs/c004-r2-prepare/exit` marker. Before any review, check
   that the marker says `"exit_code": 0` **and** `accept/c004-r2/prepare.json` says
   `"result": "PASS"`. Otherwise read the job's `log.txt`, fix, and prepare under a new name.
2. Two fresh reviews per `docs/REVIEWING.md` (202 rows, including step 6 for the 89 functions
   with linker rewrites): reviewer `review-c004-openai` (astra-reviewer) and
   `review-c004-second` (reviewer). The c004 records in `accept/c004/reviews/` show what each
   found last time.
3. `accept.py finalize --checkpoint c004-r2 --min-reviews 2 && accept.py verify && git add … && git commit … && git push`.
   Capture exit codes; never pipe a gate into `tail`/`grep` before `&&`.
4. **c005** (branch `c005-wave5`: 37 wave-5 functions collected): merge into `main`, then do the
   interface unification listed in `docs/wip/c005-INTEGRATION.md` in a worktree (3 disagreements,
   5 policy items), prepare `c005`, two reviews, finalize.
5. Next waves: `tools/triage.py --avoid svc,thumb,exclusive batches …` against v6 (v6 made ~200
   more functions creditable). Authors include `include/game/*.h` and never write local headers;
   before each prepare run `tools/interfaces.py check` on a dry ledger (accepted + candidates) and
   unify declarations first — that is now the main integration cost. On the desktop raise
   `tools/build.py --jobs` and the author count.

Display follow-up fixed (display only, not the oracle): `match.render` and `packet.disassemble`
share a catalogue-bounded decoder. Instructions must fit wholly inside `code_ranges`; literals,
pools and padding remain bounded raw/data spans. The v6 `func_0010007c` 3-byte literal at
`[0x100085,0x100088)` no longer lets the word at `0x100084` decode as an instruction.
Synthetic regressions in `tests.test_match` and `tests.test_packet` cover mid-word boundaries,
adjacent literals/pools, padding, mixed-width Thumb instructions and multiple code/entry islands.
Match diff columns stay address-aligned even when the two sides use different Thumb widths.
Focused validation: these modules plus `tests.test_compid` pass (35 tests); the private packet
check emits separate 1-byte padding and 3-byte literal rows at `0x100084` and `0x100085`.

Open user decision (unchanged): may ARMCC `__svc(N)` and named-register CP15 variables count as C?
Until ruled, SVC/CP15 wrappers are `library_candidate`.

## 2026-10-16 — decompiling on the Mac (work also resumable on the Windows desktop)

State: `docs/STATUS.md` top entry. Accepted: c001-pilot-r4 (17 functions). Gates: `docs/GATES.md`
v1.2. The Windows setup steps in the next entry still apply; the expected test count is now
~180 (`tools/test_suite.py`), and the cross-host check is to re-run
`tools/accept.py prepare` for a new checkpoint name on the desktop and compare its build outputs
with `accept/c001-pilot-r4/prepare.json`.

Loop for each wave (integrator):

1. `tools/triage.py --exclude <in-flight.json> batches --size 15 --count N` → batch files; one
   author agent per batch (contract: AGENTS.md author loop; deliver `scratch/authors/<agent>/RESULT.json`).
2. Re-derive every claimed MATCH with `tools/match.py`, copy sources into `src/<area>/`, write
   `config/candidates/<cp>.json`, commit (G0 needs a clean tree).
3. `tools/jobs.py run --name <cp>-prepare -- .venv/bin/python tools/accept.py prepare --checkpoint <cp> --candidates ...`
4. Two reviewers per `docs/REVIEWING.md`; fix any finding in a new checkpoint name; finalize with
   `--min-reviews 2`; write `docs/known-issues-<cp>.md`; commit `accept/`, ledger, history; push.
5. Fold boundary findings (EXTENT, string tails, multi-entry) into the next catalogue version via
   `tools/denominator.py record`.

Agents that edit tracked tools work in a git worktree (`scratch/worktrees/<name>`), never in the
main checkout.

## 2026-10-15 — continue on the Windows desktop

The harness was built and proven on the macOS laptop (see `docs/STATUS.md`). Work continues on the
Windows desktop (native `armcc.exe`, more CPU/RAM/disk).

### 1. Set up the desktop (once)

1. Install Git, [`uv`](https://docs.astral.sh/uv/getting-started/installation/) (PowerShell:
   `powershell -ExecutionPolicy ByPass -c "irm https://astral.sh/uv/install.ps1 | iex"`) and the
   GitHub CLI; `gh auth login`.
2. `git clone https://github.com/mschienbein/kanken-training-2-decomp` and
   `git clone https://github.com/mschienbein/3ds-experiments` (both private).
   `.gitattributes` forces LF line endings so file hashes match the laptop.
3. Copy the private inputs **outside Git** (USB/LAN) into `3ds-experiments\inputs\`:
   `kanken-training-2\hr-kantra2.3ds` (encrypted, sha256 `23ba7aaf…`) or the decrypted copy
   (`5874c3af…`), and `console\boot9.bin` (only needed for the encrypted dump). Keep `boot9.bin`
   private; never print or commit it.
4. In `kanken-training-2-decomp`:
   ```bat
   uv sync
   .venv\Scripts\python.exe tools\setup.py
   .venv\Scripts\python.exe tools\rom.py import --rom ..\3ds-experiments\inputs\kanken-training-2\hr-kantra2.3ds --boot9 ..\3ds-experiments\inputs\console\boot9.bin --romfs
   .venv\Scripts\python.exe tools\rom.py verify
   .venv\Scripts\python.exe -m unittest discover -s tests -t .
   ```
   Expected: `setup.py` lists 14 compiler banners (natively, no wibo); 66 tests pass.

### 2. Re-prove the laptop results on Windows (cross-host determinism)

```bat
.venv\Scripts\python.exe tools\analyze.py catalogue --out scratch\analyze\catalogue.json
certutil -hashfile scratch\analyze\catalogue.json SHA256
.venv\Scripts\python.exe tools\denominator.py verify
.venv\Scripts\python.exe tools\build.py --name <date>-desktop-baseline-zero-c
.venv\Scripts\python.exe tools\build.py --name <date>-desktop-pilot-candidate --matches config\candidates\pilot.json
```

Read `docs/reviews/` (four review rounds; round 4 PASS_WITH_QUALIFICATIONS lists the qualifications).

Expected: catalogue sha256 `06027f34eb7e89666c138293a31393cbafc90d9c66862add8f3d9df571aedf13`
(identical to `config/catalogue.json`); both builds PASS; the pilot credits 10 functions / 280 B.
If the catalogue hash differs, stop and compare capstone versions before anything else.

### 3. Next work (in order)

1. **H5 compiler:** finish identification (`docs/COMPILER.md` "Next"): `-O2` vs `-O3`, C++ flags
   (`--cpp`, RTTI/exceptions), library regions (CTR-SDK, NW4C, PUX HWR, C runtime). Freeze the
   `game` profile in `config/compiler_profiles.json` with evidence.
2. **Review and accept the pilot:** independent review of `src/pilot/*.c`, two fresh `build.py`
   receipts, full suite, then move the units into `config/matches.json` and record progress.
3. **H11 agent pilot:** ~10 stratified functions (leaf, calls, pools, switch, loops, VFP, C++
   virtual, Thumb) by author agents using `tools/packet.py` + `tools/match.py`; record time,
   attempts and model per function.
4. **Catalogue v2:** fold in `EXTENT` results from `match.py` and review the 350 anomalies; record
   with `tools/denominator.py record --note ...`.
5. **Scale:** triage tool, parallel author waves with one integrator, reviews, accepted
   checkpoints; ByteMatch once the owner approves making the repository public
   (`docs/BYTEMATCH.md`).

### Facts worth knowing

- String literals are emitted inside the function's own section (reached by PC-relative `add`),
  so they count in the function's extent; const tables go to separate sections whose placement
  `inplace.py` infers and verifies.
- The decomp.me compiler archive contains only `armcc.exe` (no `armlink`/`fromelf`/`armasm`). The
  hybrid build proves credited function bytes; a full original-linker build is a later gate.
- The game passes floats in VFP registers; code is ARM with ~71 Thumb functions.
