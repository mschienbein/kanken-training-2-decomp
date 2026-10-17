# Compiler identification

Status: **frozen (gate H5, 2026-10-16).** Game code: profile **`game`** in
`config/compiler_profiles.json` = ARM Compiler 5.04 (archive build `armcc_504_82`, banner
`Tool: armcc [5040081]`) with
`--cpu=MPCore --fpmode=fast --apcs=/interwork --arm --split_sections --signed_chars -O3 -Otime`.
`.c` files compile as C, `.cpp` as C++ (armcc's default by suffix).

## Middleware profiles (frozen 2026-10-17)

CTR-SDK and middleware code was built with an older compiler than the game. Two further profiles,
both on ARM Compiler 4.1 (archive build `armcc_41_791`) with the game's other flags:

| Profile | Optimisation | Clean-source matches | Of these, fail under `game` |
|---|---|---:|---:|
| `sdk` | `-O2 -Otime` | 54 functions / 3,260 B | 53 |
| `middleware` | `-O3 -Otime` | 57 functions / 3,932 B (47 SDK-looking, 4 NW4C-looking, 4 probable PUX HWR, 2 utilities) | 55 |

Evidence (private study, 2026-10-17, agent ProfileProbe, OpenAI family): 352,363 compiler runs over
14 ARMCC builds × 128 flag sets × 194 functions × 312 source hypotheses; 35 of 143 author near misses
matched under ARMCC 4.1 and under no 5.04 setting. Natural source only (no `volatile` or scheduling
steering): the SDK's narrow byte/halfword IPC wrappers reproduce their parameter spills only with
address-taking marshalling (C++ `const&` helpers, a union view through `&param`, `memcpy`) compiled
by 4.1. Build 791 is the common reproducer: NW4C-looking `0x138fe4` matches only under 4.1 b791
`-O3 -Otime`; the eight actual 4.1 builds otherwise agree on the SDK witnesses, so b791 is a
measured reproducer, not an authenticated original build. `-O2` vs `-O3` is per population:
PUX HWR `0x17d67c` (556 B) matches only at `-O3`; SDK IPC wrappers `0x15a44`, `0x13e7d4`,
`0x142644` only at `-O2`.

Rule: a unit records the profile its bytes match (`profile` in the ledger); game code stays on
`game`. A reviewer checks that a non-`game` unit is plausibly SDK/middleware code.

## Round 2 result (2026-10-16)

Full evidence: `research/compid/r2/REPORT.md` (worker CompilerIdR2, OpenAI family), re-derived by
the integrator with `--profile game`: all 11 functions MATCH.

| Address | Feature | Source (`research/compid/r2/`) |
|---|---|---|
| 0x103258, 0x103358 | pointer-advancing / bound-reloading loops with calls | `loop_cleanup.c` |
| 0x105084 | fixed-count resource loop, indexed structs | `loop_resources.c` |
| 0x105fa8 | strided initialisation loop, mixed field widths | `loop_state.c` |
| 0x103798 | global via literal pool, conditional BL | `game_core.c` |
| 0x1091d4 | int→float VFP conversions, store scheduling | `game_core.c` |
| 0x10e664 | leaf field store | `game_core.c` |
| 0x1103d4 | VFP divide, float return | `game_core.c` |
| 0x10d604 | C++ class: virtual allocation call, null check, tail call | `game_heap.cpp` |
| 0x1718ec, 0x171c7c | 12-entry switch jump tables (preserved out-of-range bug, `ORIGINAL BUG:`) | `game_enum_switch.c` |

- **`-O3` over `-O2`:** four loop functions (0x103258, 0x103358, 0x105084, 0x105fa8) match only
  at `-O3 -Otime`; 0x105fa8 also rejects `--loop_optimization_level=0`. Exactly one of twelve grid
  cells matches every source.
- **Libraries:** three NW4C-region and three probable PUX-region functions also match `game`;
  CTR-SDK intrinsic-style wrappers did not match any probe yet. No separate library build is
  identified; library code is classified, not credited, until its ownership is settled.
- **Open:** original spelling of C++ switches (RTTI/exceptions/VFE flags don't change these
  probes), source shapes for 0x1033cc / 0x10c498, Thumb function ownership.

## Round 1 notes

## Method

`tools/compid.py` compiles hand-written probe C under a grid of compiler builds × flag sets and
links every result in place against the original (`tools/inplace.py`), so a "Y" means the probe
reproduces the original function byte for byte, relocations included. Probe sources are written
once and used unchanged for every grid cell. Probes and raw results live in `scratch/compid/`
(private: the probes name original addresses).

Base flags in every cell: `--cpu=MPCore --fpmode=fast --apcs=/interwork --arm --split_sections
--signed_chars`.

## Evidence so far (2026-10-15, macOS arm64 via wibo 1.2.0)

| Function | Feature | 4.0 b821 | 4.1 b894–b1454 | 5.04 b82 |
|---|---|---|---|---|
| 0x10e664, 0x1149a8, 0x1179b4, 0x122350, 0x1263b8 | leaf loads/stores, conditional return | Y | Y | Y |
| 0x136930 | branchy leaf (`-Otime` vs `-Ospace` split) | N | Y (Otime) | Y (Otime) |
| 0x140dac | array indexing, early return | N | Y (Otime) | Y (Otime) |
| 0x103798 | global via literal pool + BL | — | Y | Y |
| 0x1103d4 | VFP divide, float return in s0 | — | Y | Y |
| 0x1091d4 | VFP convert + store order | — | **N in all 24 statement orders** (b1049, b1440) | **Y** (2 of 24 orders) |
| 0x1033cc | loop after allocation | — | N | N (source shape open; `-O3` unrolls ×2, the original does not) |
| 0x10c498 | two virtual calls + float divides | — | N | near: only the order of two `vmov.f32` saves differs |

Conclusions:

- RVCT 4.0 is ruled out; `-Ospace` is ruled out for game code (0x136930, 0x140dac).
- 4.1 builds cannot reproduce 0x1091d4's store scheduling under any statement order; 5.04 can.
- `-O2` versus `-O3` is unresolved: 0x1033cc is not unrolled in the original, which `-O3 -Otime`
  does by default. Find a source shape that matches under one of them before freezing.
- Hard-float vs soft-float linkage made no difference on these probes; the original passes and
  returns floats in VFP registers (`s0`, `s1`).
- The archive's `armcc_41_561` reports `RVCT4.0 [Build 902]`; treat archive ids as labels.

Pilot (candidate, not accepted): the ten matching probe functions, credited under
`trial-504-82-O3-Otime`, give a PASS hybrid build (`build/2026-10-15-laptop-pilot-candidate`): 10
functions / 280 bytes from compiled C, `code.bin` identical, `.code` recompresses identically.

## Next

1. Resolve `-O2`/`-O3` with loop-bearing functions; resolve 0x10c498's save order (likely source
   shape).
2. Probe C++ specifics (the game is C++: virtual calls through vtables): `--cpp`, RTTI and exception
   flags, `--force_new_nothrow`.
3. Fingerprint library regions separately (CTR-SDK, NW4C, PUX HWR, C runtime): prebuilt
   libraries may use other builds/flags. Note the recurring `mov r0, r0` before `bx lr` in some
   regions.
4. Reach ≥10 independent matches across distinct features (gate H5), then freeze the profile as
   `game` in `config/compiler_profiles.json`.
