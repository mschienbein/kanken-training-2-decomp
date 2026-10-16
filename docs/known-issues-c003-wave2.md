# Known issues — checkpoint c003-wave2

48 new functions from wave 2 (four OpenAI-family authors, `openai-codex/gpt-6.1-sol`; `author`
per unit in `config/candidates/c003-wave2.json`), plus revisions to accepted material made by the
integrator (Claude Opus) while unifying interfaces. Catalogue/denominator v3; gates v1.5.

## Revisions to accepted material (`prepare.json` `revised_units` / `revised_headers`)

- `src/pilot/game_enum_switch.c` (c001): `func_001795bc` is now decompiled and takes no
  argument (it returns a halfword of the selection state at 0x28c1f0), so `func_001718ec` and
  `func_00171c7c` lose their unused `object` parameter; the prototype comes from
  `include/game/selection.h`. Both functions still MATCH.
- `include/game/bss_0049ecf0.h` (c002): `struct Command` and `struct CommandQueue` (previously
  local to `src/wave1/func_00195254.c`, and defined differently in wave 2's `func_00110634.c`)
  are merged here; the byte at +0x18 is named `ready` instead of being written through a cast.
- `src/wave1/func_00195254.c` (c002): local struct definitions removed (now from the header).

## Qualifications

- **Partial types and integrator-chosen names.** Where two authors named the same object
  differently, the integrator picked one name: `struct Widget`/`struct NameTable` in
  `include/game/screen.h` (was `DisplayObject`/`AnimationTable` and `Widget`/`LabelSet`);
  distinct types that shared a tag were renamed (`struct ItemPage`/`ItemPageSet` in
  `include/game/item_pages.h`). `Screen.slots` is sized to the highest index used (0x35).
- `include/game/render_state.h`: `struct SlotList` carries `__attribute__((aligned(8)))` as
  written by its author; check that it is required for the match rather than incidental.
- **C++:** one new unit (`src/wave2/func_00179298.cpp`) under the recorded waiver; the gate checks
  class-definition agreement (ODR) only.
- Inherited: preserved original bugs in `src/pilot/game_enum_switch.c` and
  `src/wave1/func_00196cb8.c`; catalogue v3 limits (`docs/catalogue/V4-BACKLOG.md`); harness
  limits (byte equality of the relinked image, no runtime test, no Windows reproduction yet).
