# Status

Current entry on top; older entries are kept as history.

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
