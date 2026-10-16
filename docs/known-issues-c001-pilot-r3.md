# Known issues — checkpoint c001-pilot-r3

Scope: 17 functions / 1,096 B in `src/pilot/` (7 units), compiled with profile `game`.

## Preserved original behaviour

- `src/pilot/game_enum_switch.c` (`func_001718ec`, `func_00171c7c`): an out-of-range selector
  returns an uninitialised value. Preserved with `ORIGINAL BUG:` comments; a "safe" default does
  not match. Whether valid callers can reach that path is not established.

## Source-language qualifications

- `src/pilot/pilot_a.c` (`func_00140dac`): the entry table is a flexible array member (`e[]`) whose
  bound is not recovered; the index is unchecked, as in the original.
- Struct layouts across the pilot units are partial (padding arrays, placeholder names); they are
  consistent with every use in their unit but are not recovered type definitions.
- `src/pilot/game_heap.cpp` (C++): `Heap`, `Widget` and `WidgetConfig` are partial or incomplete
  declarations (only the members this function uses). The unit is **not mechanically
  interface-checked** (the C++ waiver is recorded in `accept/c001-pilot-r3/prepare.json`); its
  interfaces were checked by hand in review.

## Inherited harness limits (see `docs/reviews/`, `docs/GATES.md`)

- Credited bytes are proven by the in-place hybrid build (`code.bin` and the stored ExeFS `.code`
  reproduced exactly); a full original-armlink link, section ordering, veneers and unwind tables
  are not proven.
- RomFS lower IVFC levels are not traversed at import (the whole-CCI pin covers the supplied dump).
- Cross-host reproduction (Windows desktop) is pending.

## History

- `c001-pilot`: prepare record defective (missing per-gate checks); never finalized.
- `c001-pilot-r2`: review `review-c001-second` FAIL (F01: `int` where the caller passes a
  configuration pointer in `game_heap.cpp`); review `review-c001-openai` PASS_WITH_QUALIFICATIONS
  (F01: unbounded `e[1]` indexing; F02: this file was missing). Never finalized. All three
  findings are fixed in r3.
