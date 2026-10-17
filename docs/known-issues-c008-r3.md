# c008-r3 known issues and qualifications

Accepted at history **seq 8** (2026-10-17): **434 new functions** plus **29 revised accepted
units (35 functions)**. Ledger after finalize: **1,601 / 4,624 creditable functions, 185,952 /
1,131,454 B** (denominator v9-r2). Gates v1.16, explicit selection
`armlink5-inline-branchnop-v2` / `none`. No R1 removed-callee, L1 literal-record, model
transition or ledger move is used.

Both independent reviews passed with qualifications and **no findings**:
`accept/c008-r3/reviews/openai.json`, `accept/c008-r3/reviews/second.json` (narratives and
adjudications are private reviewer evidence).

## How c008 got here

- **c008** (prepared `a7c57ba`): both reviews FAIL on the same four interface-contract defects
  (callback-slot and header contracts). Not finalized.
- **c008-r2** (prepared `0b861a1`): the four defects fixed plus a same-class contract sweep;
  `func_0016b6a4` deferred to c009 (no natural cleanup identity makes its directory owner the
  accepted `FileInputStream` owner). One review FAIL on two service callback slots (11, 15), one
  PASS_WITH_QUALIFICATIONS. Not finalized.
- **c008-r3** (prepared `f8adff4`): corrected the original service callback contracts for slots
  0, 1, 2, 11, 14 and 15, the auxiliary (planar audio) callback's channel-pointer/count/context
  arguments, and the notification-list/context contracts, each from the original consumers.
  Same 434 candidate units as c008-r2.

## Qualifications carried by the accepted code

- **ARMCC extensions:** 25 function rows use the shared `__svc`/CP15 declarations or compiler
  intrinsics (approved project ruling); no embedded assembly implementations.
- **Erased callees:** six armlink `inline_nop` rewrites in `func_0016e270`, `func_001bbc0c`,
  `func_001cf6dc` and `func_001ef818` do not prove the erased callees' identities; their source
  names stay `[INFERENCE]`.
- **Inferred types and roles:** descriptive class/partial-object names, some unused input roles,
  the message callback's full int result width/signedness, a zero-valued command width and the
  auxiliary sample/count signedness remain `[INFERENCE]`; a MATCH does not prove historical types.
- **Notification list:** the poll-list head observed in the original is `bss_0049eeac+4`. The
  unused `ServiceCallbackState.notifications` prefix is not proven to own that list; no credited
  function reads it.
- **Caller audit:** `tools/argcheck.py` is conservative, not an arity oracle. All 45 candidates and
  285 notes were adjudicated (32 false positives, 11 qualified, 2 resolved). Whole-ledger limits
  remain: ~217 functions without a parsed prototype and ~1,050 unresolved indirect call sites.
- **C++:** 99 required C++ units and all 209 consumers of the 85 revised headers were checked with
  native frozen-compiler canonical-header probes in addition to the mechanical G3 waiver.
- **Original bugs:** 16 units keep documented `ORIGINAL BUG` behaviour for exactness.
- **Names:** 43 new bindings: 19 same-address self-definitions (placement only) and 24
  undefined-reference witnesses, of which 22 survive as target-address witnesses; two erased
  `EventPart` destructor calls do not prove historical names.
- **Profiles:** 106 units match under the frozen `sdk`/`middleware` ARMCC 4.1 recipes. That
  establishes exactness, not authenticated vendor builds or ownership.

## Deferred

- `func_0016b6a4` (to c009 scope; no credit here).
