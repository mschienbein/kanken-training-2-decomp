# Known issues — checkpoint c002-wave1-r2 (superseded by c002-wave1-r3; never finalized)

38 functions (H11 agent pilot and wave 1), authored by OpenAI-family agents
(`openai-codex/gpt-6.1-sol`; the `author` field per unit is in `config/candidates/c002-wave1.json`).
Catalogue/denominator v3.

## Preserved original behaviour

- `src/wave1/func_00196cb8.c`: for a screen other than 3 or 4 the original calls the error
  handler `func_0019f568` but then still indexes the resource table with group −1, reading before
  `bss_0049cbd0`'s group array. Kept (`ORIGINAL BUG:` comment); matching requires it.
- Inherited from c001: the preserved switch bugs in `src/pilot/game_enum_switch.c`
  (`docs/known-issues-c001-pilot-r4.md`).

## Qualifications

- **Partial types.** Structures name only the fields the matched code touches; `reserved…`
  arrays keep the original offsets. Shared globals use one declaration in `include/game/`
  (`bss_0049ecf0.h` — command system state; `result.h` — one-word C++ result code).
- **C++ (9 new units, 10 in the ledger).** Recorded `--allow-unchecked-cpp` waiver in
  `accept/c002-wave1-r2/prepare.json`: the mechanical gate checks only that a class defined in
  several units is defined identically (ODR, token level, gates v1.3). Declarations, types across
  units and const writes in C++ were checked by hand by both reviewers.
- **Catalogue v3 limits** (`docs/catalogue/V4-BACKLOG.md`): NOP entries are grouped with the
  function they fall into as non-C `multi_entry` routines, so some ordinary C bodies are not
  creditable yet (e.g. 0x19dc20 was matched by an author but is now a group member and was not
  collected).
- Harness limits inherited from c001: matching is byte equality of the in-place relinked image;
  no runtime test; Windows reproduction not yet run.

## Checkpoint history

- `c002-wave1`: prepare PASS; reviews `review-c002-openai` and `review-c002-second` FAIL —
  F01 (both): `struct Result` defined differently in `func_0018fdd4.cpp` and `func_0018fd20.cpp`
  (ODR violation); F02 (openai): headers were not hash-bound by the gates; F03/F02: this file was
  missing. Never finalized.
- `c002-wave1-r2`: `Result` moved to `include/game/result.h` (both units still MATCH); gates v1.3
  bind headers; the interface gate now fails on C++ class-definition disagreements.
