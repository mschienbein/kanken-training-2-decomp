# Known issues — checkpoint c003-wave2-r3

48 new functions from wave 2 (four OpenAI-family authors, `openai-codex/gpt-6.1-sol`; `author`
per unit in `config/candidates/c003-wave2.json`), plus revisions to accepted material made by the
integrator (Claude Opus) while unifying interfaces. Catalogue/denominator v3; gates v1.7.

## Revisions to accepted material (`prepare.json` `revised_units` / `revised_headers`)

- `src/pilot/game_enum_switch.c` (c001): `func_001795bc` is now decompiled and takes no
  argument (it returns a halfword of the selection state at 0x28c1f0), so `func_001718ec` and
  `func_00171c7c` lose their unused `object` parameter; the prototype comes from
  `include/game/selection.h`. Both functions still MATCH.
- `include/game/bss_0049ecf0.h` (c002): `struct Command` and `struct CommandQueue` (previously
  local to `src/wave1/func_00195254.c`, and defined differently in wave 2's `func_00110634.c`)
  are merged here; the byte at +0x18 is named `ready` instead of being written through a cast.
- `src/wave1/func_00195254.c` (c002): local struct definitions removed (now from the header).

- Shared declarations moved into headers for the v1.6 C++ redeclaration gate, revising accepted
  c002 units (no code change; all still MATCH): `include/game/runtime.h` (`func_001852f4`,
  `func_0019f568`) — `func_0019e02c_returning.c`, `func_00102cb0.c`, `func_00196cb8.c`,
  `func_001c44ec.cpp`, `func_001c4500.cpp`, `func_001d2da4.cpp`, `func_001d3524.cpp`,
  `func_0011f080.cpp`; `include/game/resource.h` (C++ `class Resource` and `struct
  ResourceOwner`, previously defined identically in two units each; `func_0019d5c4`) —
  `func_0019d5c4.cpp`, `func_0018fcc0.cpp`. `include/game/screen.h` now completes `struct DisplayObject`
  (from `func_0017a9a0`; r2 had named it `Widget`, colliding with c001's heap `Widget`) and is C++-safe; `include/game/text.h` declares `func_0017adc4`.

## Qualifications

- **Partial types and integrator-chosen names.** Where two authors named the same object
  differently, the integrator picked one name: `struct DisplayObject`/`struct NameTable` in
  `include/game/screen.h` (was `DisplayObject`/`AnimationTable` and `Widget`/`LabelSet`);
  distinct types that shared a tag were renamed (`struct ItemPage`/`ItemPageSet` in
  `include/game/item_pages.h`). `Screen.slots` is sized to the highest index used (0x35).
- **C++:** one new unit (`src/wave2/func_00179298.cpp`) under the recorded waiver; the gate checks
  class-definition agreement (ODR, token level) and the absence of local `extern "C"`
  redeclarations of symbols shared with other units (gates v1.6b); declaration types and const
  writes in C++ were checked by hand by both reviewers.
- Inherited: preserved original bugs in `src/pilot/game_enum_switch.c` and
  `src/wave1/func_00196cb8.c`; catalogue v3 limits (`docs/catalogue/V4-BACKLOG.md`); harness
  limits (byte equality of the relinked image, no runtime test, no Windows reproduction yet).

## Checkpoint history

- `c003-wave2`: prepare PASS; `review-c003-openai` FAIL — F01 C++ caller `func_00179298.cpp`
  used the renamed `DisplayObject`/`AnimationTable` types; F02 finalize would have refused the
  declared revisions (gate bug); F03 `[1]` tails in `func_001770cc.c`/`func_001791e0.c`; F04
  incidental `aligned(8)` on `SlotList`. Never finalized.
- `c003-wave2-r2`: all four fixed (flexible array members, also for `GraphicsContext.buffers`;
  `aligned(8)` removed; gates v1.6).
- `c003-wave2-r2`: `review-c003-openai` PASS_WITH_QUALIFICATIONS (note: stale gate wording in this
  file); `review-c003-second` FAIL — F01 `screen.h` completed `struct Widget` (0x94 B display
  object) while accepted `src/pilot/game_heap.cpp` uses a different 0x48-byte `Widget`. Never
  finalized.
- `c003-wave2-r3`: the display object is `struct DisplayObject` again (the wave-2 author's name);
  gates v1.7 make header class names one namespace for C++ units, which also moved accepted
  `src/wave1/func_0018fd20.cpp` onto `include/game/resource.h` (its `Resource` is the same
  file-resource interface).
