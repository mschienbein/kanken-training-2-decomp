# Agent rules — 漢検トレーニング2 decomp

Target: the Japanese retail cartridge `CTR-P-B2KJ`; matching target `orig/code.bin` (sha256
`a367b5dc…`, pinned in `config/title.json`). Private inputs live in git-ignored `orig/`, produced by
`tools/rom.py import`. Read `README.md`, `docs/STATUS.md` (current entry on top) and
`docs/ACCEPTANCE.md` before changing anything.

## Roles

- **Integrator (Main):** one long-lived session. Owns shared state: `config/`, the function
  catalogue, symbol/split configuration, compiler profiles, link layout, headers shared across
  units, and every accepted receipt. Routes work, verifies results, writes decision records. Never
  reviews its own work.
- **Authors:** one function or one translation unit each, in a private scratch directory
  (`scratch/<agent>/`). They produce candidate source plus a `result.json`; they never edit
  canonical source, config or catalogues.
- **Reviewers:** independent of the author (other model family where possible). They re-derive
  results from the candidate and the tools, and seal their verdict before reading anyone else's.

## Single-writer rule

Only the integrator runs state-changing steps: adopting candidates, editing shared headers or
config, regenerating splits/link scripts, building accepted receipts, recording progress. Authors may
compile and compare their own candidate in isolation. Two state-changing runs never overlap.

## What counts

- **Matched:** the function's bytes come from compiled C/C++ in a full `code.bin` build that equals
  `orig/code.bin`, reproduced by two fresh builds, with relocations checked strictly (not objdiff's
  lenient default) and the source rules below respected.
- **Classified, never credited as C:** SDK/middleware proven identical to a known library build,
  compiler runtime helpers, linker veneers, handwritten assembly, literal pools and jump tables
  (owned by their function), data with format evidence.
- A split-and-relink of original bytes proves the splitter, not the source. An objdiff percentage is
  a diagnostic, not acceptance.

## Source rules

No inline assembly standing in for C. No byte blobs standing in for code or data. Real types on
every cross-unit interface. A fake match (dead code to steer register allocation, etc.) is allowed
only as a recorded qualification. Preserved original bugs carry an `ORIGINAL BUG:` comment.

## Author loop

1. `tools/packet.py func_XXXXXXXX` → `scratch/packets/func_XXXXXXXX.md` (symbolized disassembly,
   pool words, callers/callees, data refs) and a C skeleton. Packets are private (original
   disassembly); never commit them.
2. Write C in your scratch directory. Name functions `func_XXXXXXXX` (or a `config/names.json`
   name); declare callees and globals as `extern` with the `func_`/`data_`/`bss_XXXXXXXX` names
   the in-place linker resolves.
3. `tools/match.py <file.c> --profile <profile> --out scratch/<agent>/...` until every function
   reports `MATCH`. `EXTENT` means the bytes reproduce the original but the catalogue boundary is
   wrong: report it to the integrator, do not pad the source.
4. Deliver: the source path, the `result.json`, profile, attempts, and remaining unknowns.
   The integrator credits units only through a `tools/build.py` receipt.

## Evidence

- Records are immutable: versioned names, never overwritten; failures are kept.
- A failed gate is recorded and its cause fixed. Rerunning unchanged input is not evidence.
- Label anything not directly observed as [INFERENCE].
- User decisions are recorded as rulings in `docs/rulings/`; agents surface choices, they do not
  make policy.

## Operations

- Run tools with the project interpreter: `.venv/bin/python tools/<tool>.py ...` (after `uv sync`).
- Heavy jobs (full builds, full test suites, whole-tree audits) run one at a time per machine; the
  host is shared with other decomp sessions.
- Keep at least 8 GiB disk free. Never delete accepted build records.
- Never print, log or commit `boot9.bin`, key material, ROM bytes or disassembly of original code.
