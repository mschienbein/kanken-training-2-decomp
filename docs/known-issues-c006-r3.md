# Known issues — checkpoint c006-r3

**381 new functions / 30,724 B in 379 units** (waves 8–10, large batches L001–L003, SDK batches
x01–x11) plus **10 revised accepted units** (`src/wave1/func_0011f080.cpp`, `src/wave1/func_0019f450.c`,
`src/wave2/func_001770cc.c`, `src/wave2/func_00179398.c`, `src/wave2/func_001b7c9c.c`,
`src/wave4/func_0013e71c.c`, `src/wave4/func_001470c8.c`, `src/wave4/func_0014a988.c`,
`src/wave4/func_0019dc04.c`, `src/wave6/func_001963e8.c`), all preserving their accepted bytes.
Profiles: 363 units `game`, 7 `sdk`, 9 `middleware`. Denominator **v8**; gates **v1.13**; linker
model `armlink5-inline-branchnop-v2`. Per-function integration notes, qualifications and the
pre-review record are in [`known-issues-c006.md`](known-issues-c006.md), which stays the detailed
reference for this content.

## Checkpoint history

- **c006** — prepare FAILED on the test suite only (`tests.test_permute` synthetic fixture lacked
  `Context.literal_model` after a tooling merge). No review took place. Records kept in `accept/c006/`.
- **c006-r2** — prepare PASS. `second` review PASS_WITH_QUALIFICATIONS; `openai` review **FAIL**:
  F01 (blocker) `func_001c5994` used private `SoundNotificationForwarder`/`SoundNotification` types
  that conflict with the canonical `AudioChannel`/`AudioSlot` contract (ARMCC error #338 when the
  canonical header is included). Records kept in `accept/c006-r2/`.
- **c006-r3** — `func_001c5994` rewritten on the canonical types (MATCH); the obsolete private
  header was removed; every dependant of the touched headers re-MATCHed; every c006 C++ unit was
  compiled with its canonical headers included (88/88 MATCH, no other conflicts). Prepare PASS
  (two identical fresh builds; suite 547 tests, 1 skipped; interfaces pass with
  `--allow-unchecked-cpp`, 113 C++ units reviewed by hand).

## Reviews (both PASS_WITH_QUALIFICATIONS, sealed in `accept/c006-r3/reviews/`)

- **openai:** 273 PASS · 118 QUALIFIED · 0 FAIL of 391 rows; F01 note (68 ARMCC-extension users),
  F02 minor (disclosed codegen steering and representation choices are not uniquely recovered
  syntax), F03–F07 notes (erased inline-callee identities; unresolved callback/unused-input
  contracts; 13 preserved original bugs; non-game profiles and names additions inferred; one
  token-identical duplicate `FramebufferBinding` redeclaration outside c006's interface scope).
- **second:** 271 PASS · 120 QUALIFIED · 0 FAIL; F01–F08 notes (extensions, 37 linker rewrites
  re-derived, representation choices, canonical-header probes, caller contracts with argcheck
  exit 2 for unchecked C++ coverage, preserved bugs and terminal paths, compiler-profile
  provenance, single-family review).

## Qualifications carried by the accepted content

- Approved ARMCC extensions (`__svc` declarations in `include/ctr/svc.h`, CP15 named registers in
  `include/ctr/cp15.h`, intrinsics incl. `__current_pc`) per
  `rulings/2026-10-17-armcc-extensions.md`.
- Disclosed codegen steering: `func_001c44c0` keeps a `__schedule_barrier` (diagnostic-PC
  ordering); `func_00193414`/`func_00193420` keep volatile halfword views. These are qualifications,
  not claims about the original source.
- Five armlink return-only inlinings erase callee identity; each site is marked `[INFERENCE]`.
- Unresolved callback arity, unused inputs and some result identities remain qualified per row.
- SDK/middleware ownership of the 16 non-game-profile units is inferred from the profile witness.
- Review independence is fresh-agent, single model family (`rulings/2026-10-17-reviewer-model-family.md`).

**Finalized** 2026-10-17, history seq 6: 870 functions / 50,012 B accepted (4.43% of 1,129,400 B).
