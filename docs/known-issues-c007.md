# c007 integration issues and qualifications

This is an integration record, **not acceptance or an independent review**.
Phase 2 is verified against finalized c006-r3 (`816c68d`); the former c006 release
prerequisite is resolved. Main still owns independent review and prepare/finalize.

| Area | Units | Functions | Catalogue bytes |
|---|---:|---:|---:|
| wave 11, a–j | 113 | 113 | 6,060 |
| L004–L012 matches | 4 | 4 | 2,276 |
| SDK/middleware helpers | 75 | 75 | 5,712 |
| three preferred NW4C imports | 66 | 105 | 9,328 |
| **Total** | **258** | **297** | **23,376** |

Profiles:126 game /62 sdk /70 middleware units. All requested final author results and
preferred NW4C packages are collected. Native NW4C2 00185764 replaces the equivalent
c007 ProfileProbe C owner; it is not duplicate credit.

## Exercised evidence

- **258 candidates /297 functions /23,376 bytes**, plus **34 accepted-source
  revisions**, pass the fresh per-unit sweep: **292 units /331 functions /26,880
  bytes MATCH**, every command exit0. The revisions comprise the original23,
  the ten released c006 replacements and finalized c006-r3's001c5994.
- Adopted-v8 catalogue SHA:
  `e33aad973846b60b5286be6becb5080c9626188fa9a64a6c0b48197b16e8adad`.
  Every surviving candidate is creditable; none needs a v8 re-extent.
- Actual accepted+c007 dry-union G3 exits **0** with `--allow-unchecked-cpp`:
  **1,093 units /889 checked C /204 explicitly unchecked C++**, zero
  disagreements, zero policy findings and zero unchecked C.
- Fresh caller audit exits **2**, not zero: **35 raw candidates in23 functions**
  are individually adjudicated against unchanged issue/prototype/site facts.
  All195 conservative input-setup notes retain declarations.191 functions lack a
  parsed C contract;1,048 unresolved indirect sites remain explicit limitations.
- Canonical-header probe exits **0**: all **91 c007 C++ units /130 functions**
  MATCH while including the62 C++-compatible canonical `include/game` headers
  together, without private duplicate shared types. The unrelated C-only
  `graphics_state.h` flexible-array view is not consumed by these C++ units.
  Finalized001c5994 also passes this probe. The initializer probe preserves its
  source basename for ARMCC's generated symbol; no candidate or binding was changed.
- Full suite exits **0**: **547 tests /1 skipped /0 failed modules**,19 modules.
- The single dry union build, launched through `tools/jobs.py` after a successful
  no-other-build/prepare process check, exits **0**: **1,093 units /1,167 functions
  /73,388 bytes**. Credited functions, `code.bin` and compressed ExeFS code are exact.
- Source-bound seven-step integrator pre-review: **320 rows, PASS**, with the
  existing semantic/C++/indirect-call qualifications retained. It is not an
  independent acceptance review.

Current receipts are under worktree `scratch/integrator/c007/phase2/`:
`matches.json`, `interfaces.json`, `argcheck.json`, `argcheck-adjudications.json`,
`canonical-probe.json`, `canonical-notification-supplement.json`, `suite.json`
and `counts.json`. The dry-build receipt is
`build/c007-phase2-dry-union-20261017-1932/receipt.json`; its jobs exit marker is
under `scratch/jobs/` with the same name. Refreshed `pre-review.json` and
`delivery-bindings.json` remain at the parent artifact root. The worktree-local
`scratch/integrator/c007-NOTES.md` records the phase-2 handoff without editing main.

## Resolved release prerequisite

Merged `c007-phase2` (`e3ee6c3`, including ten-replacement commit `843be9b`) and
finalized c006-r3 main (`816c68d`) without conflicts. All ten replacement hashes
still equal `frozen-revisions-ready.json`; they are actual tracked sources, not
shadow substitutes. Finalized001c5994 uses `audio_voice.h`, and the removed
`sound_notification.h` is neither restored nor referenced.

## Classification and explicit deferral

- 00130f90 (`__ARM_common_memclr4_8`) and00183458 (`__ARM_common_memcpy4_5`) are
  compiler-generated runtime, not source-authored C credit. Bindings/compiler emissions
  may remain, but neither helper body is falsely credited.
- Per Main, **00185bb0 is deferred**: it matches only in its original four-method NW4C TU
  and needs accepted0018d868/00191154/00191178 rehomed there. Duplicate accepted
  out-of-line bodies were not retained as uncredited codegen context.

## Source/ABI qualifications

Descriptive names, partial identities and library ownership remain [INFERENCE]. Shared
SVC/CP15/TLS bindings, return-address intrinsics, byte/halfword union marshalling,
parameter homing, register/inline boundaries and used CurveAdshr inline helpers are
explicit qualifications, not recovered original spelling. Only
`armlink5-inline-branchnop-v2` is used; opt-in v3/literal records earn no credit. All28
function rewrite records were reviewed, including source-commented erased137150 identities.

Two zero-r1 callers of181448 do not uniquely establish a required second input; the
one-object pass-through181420 interface is retained with that uncertainty explicit.
17da08 retains a full-word flag, not unsupported bool narrowing.1e807c uses the native
C++ MemberCallback API, not an invented three-integer prototype.194a34's returned
WordPair pointer follows new natural callers;10be94 no longer invents a one-word lock
layout when it only passes an opaque allocator address. Opaque callback identity,
cross-language partial views and unused original inputs remain qualified.

The c008 interface cutover shares `PackageFile`/`PackageView` through
`game/package_file.h`;00193120 has no private duplicate records.00140e88 returns
`bool` under the shared `game/archive_resource.h` declaration, preserving its recorded
SDK-profile MATCH. The four c007 units consuming these touched headers/sources all
re-MATCHed (phase1 receipts under `scratch/integrator/c007/phase1/`). No existing c007
source caller declares00140e88; c008 owns the natural bool-returning00183eb0 caller.
The resource header includes`<stdbool.h>` and forward-declares`ArchiveIndex` itself,
so c008's reverse include order cannot leave the bool contract or tag undeclared.
Its three current dependants re-MATCHed after that include-order correction.

The four0017a7e4 wrappers now pass canonical`NameTable`pointer tables, not addresses
of invented opaque`CommandTarget`objects. The three existing arrays keep their
`property_descriptor.h`bounds[6]/[4]/[2]. [INFERENCE] Private initializer002131b4
independently supplies consecutive`NameTable`constructor results at00290e98;
that fourth array deliberately keeps an unspecified bound pending an accepted owner.
The common dispatcher reads descriptor category/identifier words and a sentinel.
All40 direct/transitive header dependants re-MATCHed; targeted G3 exits0 with
15 checked C units,25 explicitly unchecked C++ units, zero disagreements/policy.

GPL notices and the pinned [NintendoWare4Ctr source](https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd)
are preserved. `THIRD_PARTY.md` and `src/nw4c/INTEGRATED-PROVENANCE.json` distinguish supplied
provenance hashes from reconciled files and preserve Khronos MIT notices/fact sources.
These adapters are not copied proprietary SDK implementations or a native portability claim.
No original bytes/disassembly, packet contents or keys are part of this record.

## Acceptance (checkpoint c007, finalized)

- **Prepare** (`accept/c007/prepare.json`): PASS — two identical fresh builds, suite 547 tests /
  1 skipped / 0 failed modules, interfaces pass with `--allow-unchecked-cpp`. The prepared scope
  declares **33 revised accepted functions** and **29 linker-rewrite records**; the integration
  prose above (34 revisions / 28 rewrite records) is superseded by those prepared counts
  (review `second` F08, minor).
- **Reviews** (sealed in `accept/c007/reviews/`), both PASS_WITH_QUALIFICATIONS:
  - `openai`: 115 PASS · 215 QUALIFIED · 0 FAIL of 330 rows; F02 minor — two revised IPC wrappers
    keep three disclosed scheduling barriers each; F01/F03–F08 notes (extensions, two erased
    callee identities, caller contracts, NW4C GPL provenance and 205 names additions with
    whole-match evidence, 15 preserved original bugs, unchecked C++ interfaces, single-family
    independence).
  - `second`: 108 PASS · 222 QUALIFIED · 0 FAIL of 330 rows; F01–F07 notes, F08 minor (counts
    above).
- Runtime helpers `0x00130f90` / `0x00183458` and deferred `func_00185bb0` receive no credit.

**Finalized** 2026-10-17, history seq 7: 1,167 functions / 73,388 B accepted (6.50% of 1,129,400 B).
