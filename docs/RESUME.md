# Resume

Current entry on top.

## 2026-10-20 — c010 source-contract handoff (unaccepted)

The `c010-integrate` draft contains **496 units / 509 claims**: **499 proposed new
functions / 47,304 B** and **10 explicit accepted-identity moves / 240 B**. Accepted
credit is unchanged. Two earlier contract deferrals, `001685c0` and `00168618`,
have bounded canonical witnesses. Another **49 new units / 51 functions / 9,252 B**
are explicitly deferred with their source, prior receipts and exact re-entry
conditions preserved privately; no accepted identity is withdrawn.

The source cutovers cover the shared resource-constructor/dispatch prefix,
full-width constructor counts and returned receiver, canonical audio and session
lifecycles, static handle subobjects, DSP pointer banks, publication globals,
numeric owners and native resource-wrapper interfaces. Native glyph output and
the target glyph format remain distinct; font caches are at `+0x14/+0x16`.
`PIPELINE_STORAGE_INITIALIZED_BYTES = 0x854` and its flexible snapshot tail remain
unchanged. Erased identities and partial native ownership remain qualified.

Private `scratch/integrator/c010/SOURCE-READY-r68.json` and its inventory bind the
current proposal and bounded observations, including preserved failures. The
typed canonical variants of `001273a0`, `0017f7f8` and `001ce8b8` remain measured
byte-match residue, not exhausted searches or accepted implementations.

Next import only Main's frozen c009-r3 correction, reconcile its callable and
native-constructor dependencies, then obtain Main's heavy slot for the complete
header-dependent MATCH/helper closure, G3, native/signature probes, original
callback audit, argcheck, fresh full builds and independent checkpoint reviews.
No full build, suite, prepare, finalize or acceptance claim follows from this
source-only handoff. Selection remains explicit linker **v6 + literal L1**.

## 2026-10-20 — resumed WSL desktop (current)

**State:** `docs/STATUS.md` top entry. Accepted 1,621 functions / 195,908 B (seq 9, cmodels);
denominator v9-r2; gates v1.19. Host: WSL2 Ubuntu on the Windows desktop
(`~/code/kanken-training-2-decomp`; private inputs in `~/code/3ds-experiments/inputs/`, imported
into `orig/`; `baserom.3ds` is an ignored symlink to the decrypted dump). Git pushes through
`gh auth setup-git`.

Restart sequence: linker models v4/v6/v8 are adopted. The finalized ledger selects linker v6
plus literal L1, so later checkpoints explicitly select at least that pair. Frozen profiles:
game/sdk/middleware. Resume preserved work rather than restarting completed batches.
The user-set maximum and active target are 64 subagents; keep useful slots full and back off
only if fresh provider throttling recurs (`docs/rulings/2026-10-20-subagent-concurrency.md`).
Current throttling, queue and ownership live in the private pipeline file.

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
