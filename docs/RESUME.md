# Resume

Current entry on top.

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
   Expected: `setup.py` lists 14 compiler banners (natively, no wibo); 60 tests pass.

### 2. Re-prove the laptop results on Windows (cross-host determinism)

```bat
.venv\Scripts\python.exe tools\analyze.py catalogue --out scratch\analyze\catalogue.json
certutil -hashfile scratch\analyze\catalogue.json SHA256
.venv\Scripts\python.exe tools\denominator.py verify
.venv\Scripts\python.exe tools\build.py --name <date>-desktop-baseline-zero-c
.venv\Scripts\python.exe tools\build.py --name <date>-desktop-pilot-candidate --matches config\candidates\pilot.json
```

Read `docs/reviews/2026-10-15-harness-oracle-review.md` (findings, fixes, qualifications).

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
