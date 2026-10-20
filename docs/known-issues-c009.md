# C009 recovered integration — known issues

## Status

**Ready for Main prepare and independent review with qualifications.** Finalized
main 261fdc9 / cmodels sequence 9 is merged; frozen GATES v1.19 and v9-r2 unchanged.
Every current isolated match and the eventual Main prepare explicitly select
**armlink5-inline-branchnop-v6 + anonymous-string-records-v1** (the accepted
cmodels pair, so no model transition). Earlier v2/none instructions are superseded;
historical receipts retain their original selections and failed evidence.
The canonical MessageLayout receiver is cast-free and is never aliased to
DisplayHandle. Accepted `001b8908`, `001e0afc` and `001ed874` have scoped MATCH
with their supplied helper closure after receiver/reset/visual-owner reconciliation.
Main alone owns full builds, the suite, prepare/finalize and accepted-record writes.

`config/candidates/c009.json` lists **676 units / 676 functions / 57,052 target
bytes**. These are unaccepted totals; final required-source sweep is complete.

| Frozen profile | Units/functions | Target bytes |
| --- | ---: | ---: |
| game | 615 | 51,424 |
| middleware | 36 | 3,944 |
| sdk | 25 | 1,684 |

Previous collect inputs, successful destination receipts, dirty sources and author results
were recovered rather than restarted. L042's recovered `func_001e3af4` is included. Both
w17h SDK alternate MATCH rows (`00190688`, `001dbaac`) are included. Model selection
is explicit rather than inferred from author receipts or optional manifest metadata.

## Current pre-review evidence

- Exact required sweep: **832 units / 838 functions, all MATCH, exit 0**.
- Whole proposed ledger G3: **2,238 units**, 1,728 checked C units, **zero
  disagreements/policy items/unchecked C**, 510 explicitly waived C++ units; exit 0.
- Native canonical probes: **1,940 units, zero compilation failures**. Exit 1
  reports ten nonfatal diagnostic units, all individually qualified (signed raw
  error constants, natural assignment-in-condition, and public imported-header warnings).
- Actual C signatures: **1,500**, zero failures/diagnostics, exit 0; whole **174
  game-header** C++ co-inclusion also exits 0 with no diagnostics.
- Argcheck: **2,297 functions**, 78 reported issues and 412 notes all adjudicated;
  actual exit 2 retains 389 mechanically unchecked function ABIs, not a false parser PASS.
- Complete original indirect inventory: 1,212 sites; zero untyped callable-storage
  candidates and zero required callback-object gaps. All sites dispositioned;
  85 forwarded-type candidates individually adjudicated.
- All **seven integrator pre-review steps** have **838 destination-bound rows**.
  R1 identity provenance, original counted loops, ARMCC extensions, original bugs and
  native ABI limitations remain explicit qualifications for independent reviewers.

Private binding is under `scratch/worktrees/c009/scratch/integrator/c009/`:
`PRE-REVIEW-resume-r1.json` and current `readiness-scope-r11.json`; audited r10 is
retained separately. The integration agent ran no full build, test suite, prepare,
finalize, accepted-record write or push. Main owns those gates and both independent reviews.

The final **ccddf12** correction changes only `001e3050`'s last store to the
named `phase` member instead of indexing beyond `states[0..7]`. It MATCHes all
80 bytes in `phase-store-r1.json`. Headers and callers are unchanged. The full
audit results above remain bound to the **56c333e** boundary; Main performs the
integrated full gates on the correction, with no blanket reattribution of old hashes.


## Preserved historical failure evidence

* The full final MATCH sweep measured all **801 required units / 805 functions**:
  **772 MATCH units, 16 compiler-failure units, 13 byte-mismatch units**.
  All sixteen compilation failures subsequently MATCH after canonical repairs.
  Nine non-counter byte mismatches have scoped MATCH repairs; the original counted
  loop closes the tenth after Main's faithful-control-flow ruling below. All failed
  results remain intact, and changed headers require a fresh final whole-scope sweep.
  Native/signature probes did not run because the dependent shell stopped at matching.

* The stable merged-tree `interfaces-stable-r1.json` exited **1**: 2,206 units,
  1,704 checked, one Heap ODR disagreement, one duplicate-local-prototype policy
  finding and 502 unchecked C++ units, with **zero unchecked C**. Both findings
  were fixed afterward, followed by the native reader correction/withdrawal below.
  Corrected stable `interfaces-stable-r2.json` exits **0**: 2,205 units,
  1,704 checked, zero disagreements, zero policy findings, 501 unchecked C++
  and zero unchecked C. Its exact dry input snapshot is retained.
  Current main f24fa0b is merged. Stable r4 also exits0 on the corrected headers:
  2,204 units, 1,704 checked, zero disagreements/policy findings, 500 unchecked C++
  and zero unchecked C. Restored-loop **r5 exits0**: **2,205 units, 1,704 checked,
  zero disagreements/policy findings, 501 unchecked C++ and zero unchecked C**.

* The intermediate whole-ledger `interfaces --allow-unchecked-cpp` remediation run
  exited **1**: 2,205 units, 1,705 checked, 48 disagreements, 28 inline-declaration
  findings, 500 unchecked (498 C++ and the two ARMCC varargs parser failures).
  Remediation continued while that audit ran. These counts are historical,
  not the corrected stable r2 verdict recorded above.
* The prior **166-game-header C++ co-inclusion probe exited 0** before the
  collection milestone's header changes. It is historical compile-only evidence,
  not a current native-unit, actual-signature or indirect-consumer sweep.
  Those c009-wide probes using c008-r2's reusable helpers remain unexercised.
* The catalogue recheck intersection was `00214580` (MATCH, exit 0) and
  `00203810` (compile exit 2, then deferred on concrete owner-contract evidence).
* The last three-mismatch slice is closed without removing an accepted identity.
  Accepted **`001c4494`** restores its exact accepted **Result/int-output** API and
  source: **MATCH44/44 B, exit0**. New **`0019d5dc`** forwards that same contract:
  **MATCH8/8 B, exit0**. The reader drops naturally from revised-source scope.
  New **`00113c0c`** and **`0018ee88`** are withdrawn per Main's bounded-search
  ruling. Their author MATCH forms conflict with canonical raw-status provider
  returns and structured callback-state/byte-mode contracts respectively. Exact
  author sources/receipts, canonical failed variants and measured exits are preserved
  in **`interface-deferrals-r4.json`** and **`timebox-r1/results.json`**.
  No shadow global/return type, call alias or steering recovers the missing bytes.
  Current revised scope is **122 units / 126 functions**; required union is
  **798 units / 802 functions**, dry ledger **2,203 units / 2,277 functions**.
* The earlier resumed r7 snapshot had 819 required units / 824 functions and
  127 revised accepted units / 131 functions. Subsequent compiler-probe caller
  reconciliation expands the final required scope to **832 units / 838 functions**
  and **142 revised accepted units / 147 functions**. All 1,621 accepted identities
  survive in the 2,238-unit / 2,297-function dry ledger; no owner moves or retirements.
  Current exact inputs are `readiness-scope-r10.json`; earlier inputs/failures remain intact.
* Collection closure is recorded in `collection-closure-r1.json`; its six original
  MATCH measurements remain historical. Current scope retains five: the game reader
  now uses the genuine native API, while the separate middleware reader is withdrawn.
  Eleven collection addresses therefore have exact current deferral reasons.
  Every original author source still matches its retained receipt hash.

Private exact evidence and source bindings are recorded in the worktree's
`scratch/integrator/c009/HANDOFF.json`, `UNRESOLVED-SCOPE.json`,
`interfaces-remediation-r1.json`, `cpp-compat-r5.json` and `contract-deferrals.json`.
The remaining integration state is described in `scratch/integrator/c009-NOTES.md`.

## Approved contract-resolution deferrals

| Function | Reason |
| --- | --- |
| `00164b14` | Native SDK WidgetPlacement by-value record matches, but canonical LayoutDescriptor's flat accepted `0020e460` caller cannot yet migrate byte-identically. Original caller retained; no private duplicate or ABI shim admitted. |
| `001ba338` | L034 exact default MATCH retained privately; AnimationOwner/W6ListController ownership at the shared slot remains unproven. |
| `001bda14` | Same AnimationOwner versus W6ListController relation at +0x4c as accepted `0016c55c`; weakened CallbackTarget redeclaration is not credited. Original source/receipt are under w19a (the approval message called this w19g). |

Main explicitly approved these as `contract-resolution`. Resolve the owner group
in a dedicated reviewed pass; do not invent a common owner merely to combine the rows.
The source/receipt paths and failed natural cutover evidence are in private NOTES/HANDOFF.
L034 `001b8908` is accepted cmodels material, retained at its accepted source and
reopened for shared-header review; it is not a C009 addition.

## Preserved C008 deferrals

None of the prior six has been proven resolved: `001b7110`, `001b6df4`, `001a2fc4`
(AnimationOwner/list owner relation), `0015e60c` (float-output versus unsigned-output
contract), `001f59b8` (canonical FloatPropertyDescriptor caller), and `001c5a08`
(full-word forwarding contract). The previous C008 NOTES/deferred evidence is unchanged.

## Additional evidenced contract deferrals

| Function | Concrete blocker |
| --- | --- |
| `0010cc94` | The proposed extern-C EntryObject constructor contradicts the accepted native `nw::font::ResFontBase` constructor at `00113e78`, and its unsigned-array vtable declaration contradicts canonical FontVTable. The actual derived identity is unresolved; canonical ResFont's constructor is bound elsewhere (`001db3ec`). The shadow source/header were withdrawn, not aliased. |
| `00203810` | Its L045SceneN constructor/destructor declarations disagree with concrete lifecycle definitions, including `001e7470`, `001ec14c` and `001e7fdc`. A shared virtual-dispatch prefix does not prove the full derived owners. The initializer and its 42 shadow prototypes were withdrawn instead of retaining a wrong common-owner erasure. The changed v9-r2 extent remains unclosed. |

Both are recorded under step 3's defer-with-reason option, with private sources
and exact evidence retained in `contract-deferrals.json`. They are not MATCH
or acceptance claims.

## Historical predecessor boundary and remediation

c008-r2 `38b4c01` was merged at `9218542`; the dry ledger no longer uses failed
original c008. The current dry ledger has **2,205 units / 2,279 functions**:
1,601 predecessor functions plus 678 c009 functions. The additional `0016b6a4`
deferral remains excluded.

The source delta against c008-r3 revises **123 predecessor units / 127 functions**,
including shared-header migrations and caller-evidenced constructor/callback input/result
retention. Every revised unit is listed in HANDOFF and still needs fresh final
byte/review closure. Header-only dependents need the complete canonical contract sweep.

Complete c008-r3 `b5e8b8a` is merged through prepared main `f8adff4`, retaining
c009's channel fields and before/after names while adopting all corrected service
slot and AuxCallback contracts. Original `00115000` proves slot14's context-taking
tail consumer (`service-notify-r1.json`). The complete predecessor follow-up also
corrects slots0/1/2 and the callback-state notification-node prefix. Its authoritative
evidence is main-root `scratch/integrator/c008-r3/complete/`; no known fix is unmerged.

## Additional collection-scope deferrals

The original sixteen-address collection boundary currently retains five canonical
destinations and eleven explicit deferrals, including the post-collection reader
withdrawal described below. Earlier MATCH evidence is preserved, not overwritten.
No destructor, runtime or duplicate native-class alias conceals these contracts.

| Function(s) | Exact unresolved contract |
| --- | --- |
| `00214d9c` | Proposed one-object destructors alias the canonical guard-release `00215ec4` and three-input registration `00216140` functions. Separate callback aliases over empty code do not establish compatible lifecycle roles. |
| `0010e1ac` | Delivered RenderResource collides with the stream owner; its shorter ShaderConfiguration subobject and unsigned `+0x138` count disagree with canonical shader extent and ShaderResource's selected-pointer slot. Full owner/lifecycle relation is unresolved. |
| `001671d4` | The delivered 29-slot singleton conflicts with canonical Screen return/consumer contracts and Screen's larger evidenced slot/state extent. Its implicit standard runtime bindings also remain unproven. |
| `001e3380`, `001dce40`, `001dca5c` | Registered native-destructor object/result contracts disagree with accepted void(void) definitions `001e35e0`, `001dd360`, `001dce3c`. Concrete owners and implicit runtime identities are unresolved; these accepted callback inputs remain part of final caller-contract remediation. |
| `0015eae0`, `001acc88`, `001aea80` | Complete singleton/destructor owners at `0015eba4`, `001aced0`, `001af304` are not defined in the ledger. Implicit `__aeabi_atexit`/`__dso_handle` bindings assert a standard runtime identity over the existing void registration and code-address image-base contracts, without a canonical identity proof. |
| `001cf974` | The delivered native-layout shadows duplicate pinned NW4C classes while adding a constructor/initialization format input absent from the pinned TextBox API. Version-specific complete contracts must be recovered without dropping the input or retaining duplicate classes. |

Private `collection-closure-r1.json` retains exact sources, receipts, proposed
names, source/receipt hashes and per-row reasons. This is the user's defer-with-
recorded-reason option, not approval of a false match or accepted credit.

## Further G3 remediation deferrals

| Function(s) | Exact reason |
| --- | --- |
| `0015e5d4` | The definition takes DisplayPaneLink with a material-bearing DisplayPane, while accepted `0015e5c4` forwards DisplaySurface containing a different partial text-field owner. A common concrete owner or subobject relationship is not proven; no cast/shadow type is used to erase it. |
| `00190d0c`, `00190d68` | Their standard-C `va_arg(values,int)` expansion is unsupported by the frozen G3 C parser. The retained audit records both as unchecked C, not waived C++; source correctness is not faulted. No parser suppression, hand-coded va_list ABI or language-waiver conversion is substituted for the missing tool support. |
| `0013156c` | The middleware reader's distinct full native version/namespace and result/receiver/unused-input API are unresolved. Its mutable CodeUnitReader/EncodingSource shadows duplicate the frozen Font/CharStrmReader family physically, but that resemblance does not justify binding the already-owned native member name at a second address or guessing another overload. Original collected MATCH/source/receipt evidence remains preserved; current credit is withdrawn. |

Exact sources and original author receipts remain in private
`interface-deferrals-r2.json`, retaining the previous `interface-deferrals-r1.json`.
The shared SurfaceTextState used by the retained
spacing/size wrappers is a qualified partial field view, not a complete recovered
native class or an alias of DisplayPane. Its actual allocation and native identity
remain unrecovered.

## Native reader API correction

`001f4cf8` now defines the frozen native
`nw::font::Font::GetCharStrmReader(wchar_t) const` signature rather than an
explicit-output mutable shadow: the const aggregate result, const receiver and
unused dummy remain present. Its native definition MATCHes all 64 bytes (exit0),
with the same-address own-definition name adopted. The original method-pointer
table remains external data; no read-method aliases or data credit are added.
`native-reader-attempts-r1.json` preserves the initial missing-NW_RELEASE compile
failure and successful corrected invocation. The frozen native headers and profiles
are unchanged. This scoped success is not a final checkpoint-wide MATCH claim.

## Original counted loop: mandatory qualification

Main ruled `001c20e8`'s original counted backedge faithful, not added steering.
The plain observed loop is restored and MATCHes all20 bytes (exit0), with no
volatile, asm, artificial uses or register forcing. Its purpose remains unknown
(delay or leftover loop), visibly `[INFERENCE]` in the source. The eventual
pre-review and independent review row **must be QUALIFIED** for explicit adjudication.
`counter-loop-ruling-r1.json`, `counter-loop-ruled-r1.json` and
`pre-review-required-qualifications-r1.json` retain the ruling/evidence.
The historical `interface-deferrals-r3.json` concern is superseded, not erased.


