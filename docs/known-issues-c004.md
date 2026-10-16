# Known issues — checkpoint c004

196 new functions — wave 3 (66) and wave 4 (130: 86 armlink-NOP'd tail-call wrappers, 8 empty
functions, 36 bodies) — by OpenAI-family authors (`openai-codex/gpt-6.1-sol`; `author` per unit
in `config/candidates/c004.json`), plus interface revisions to accepted units. Denominator v5;
gates v1.8; linker model `armlink5-inline-branchnop-v2` (`docs/LINKER.md`).

## Linker model qualifications (`prepare.json` `linker_rewrites`, 89 functions)

- A call the original image holds as `mov r0, r0` was inlined by armlink because its callee is a
  return-only function. The bytes prove only that; **which** return-only function the source calls
  is `[INFERENCE]` unless a pointer or neighbour evidences it (marked in the source).
- A 4-byte wrapper `mov r0, r0` is a tail call to the function immediately after it. Its C
  signature (`void f(void) { g(); }` vs forwarding arguments) compiles to the same branch; the
  sources take it from the callee's known prototype or callers' use, else `void (void)` with a note.
- Empty functions (`bx lr`): parameters and return type cannot be recovered from the bytes; the
  sources use `void (void)` unless callers or callbacks evidence a contract (e.g. identity
  functions `void *f(void *p)` whose result accepted code uses).
- The conditional single-instruction inlining rule comes from the armlink manual only; no credited
  function here depends on it unless listed in `linker_rewrites` with a conditional callee.

## Revisions to accepted material (`revised_units` / `revised_headers`)

Made by the integration pass (agent IntegrateC004; evidence in the commit "Unify c004 interfaces
from caller and callback evidence"): shared prototypes moved into `include/game/runtime.h`,
`bss_0049ecf0.h` (command-mutex helpers `func_0012fb38`/`func_0012fb48`), `render_state.h`;
accepted units now include them instead of local declarations (`func_00195254.c`,
`func_0011f080.cpp`, `func_0013db3c.c`, `func_00147260.c`, `func_00110634.c`, `func_00127294.c`).
Evidence-based contract decisions: `func_00122500` returns `int` (callers test r0);
`func_00122510` is `void` (no caller reads r0, `[INFERENCE]` for indirect callers);
`func_0018f7f0`, `func_001ef880`/`func_001ef884`, `func_0013714c`/`func_00137150` are identity
functions `void *(void *)` (accepted callers use the returned pointer / a typed destroy callback).

## Not credited

- `func_00100d6c` (SVC 3): policy-pending (`__svc`); `0x17da60` is byte-identical to ARMCC's
  compiler-generated `__ARM_common_memcpy4_6` helper (classification, not C credit).

## Inherited

Partial types; preserved original bugs (`src/pilot/game_enum_switch.c`,
`src/wave1/func_00196cb8.c`); C++ checked mechanically only for ODR, header class-name collisions
and local redeclarations; byte equality of the relinked image; no runtime test; no Windows
reproduction yet.
