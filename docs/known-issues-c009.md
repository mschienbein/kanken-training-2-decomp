# C009 recovered integration — known issues

## Status

**Readiness on finalized main bb60cd7; not review-ready or accepted.**
Includes finalized c008-r3 (sequence 8), preserving c009's typed slot13/14 work.
The original sixteen-address boundary currently retains five canonical destinations
and eleven recorded deferrals: the earlier collected `0013156c` was withdrawn
on a native C++ API qualification. Three other interface/tooling deferrals remain.
`docs/GATES.md` stays v1.16. Main owns full builds/suite and acceptance.
Main resumed the committed continuation without restarting searches. Checks run
nice'd and sequentially. Stable merged-tree G3 r3 exits0, but later canonical
repairs need a new final gate/sweep. Thirteen byte mismatches remain; no pre-review PASS.

`config/candidates/c009.json` lists **678 units / 678 functions / 57,212 target
bytes**. These are unaccepted scope totals; newer source remediation is not yet rematched:

| Frozen profile | Units/functions | Target bytes |
| --- | ---: | ---: |
| game | 616 | 51,528 |
| middleware | 37 | 4,000 |
| sdk | 25 | 1,684 |

Previous collect inputs, successful destination receipts, dirty sources and author results
were recovered rather than restarted. L042's recovered `func_001e3af4` is included. Both
w17h SDK alternate MATCH rows (`00190688`, `001dbaac`) are included. v3/literal-record
results are not silently admitted into this default-model checkpoint.

## Blocking evidence

* The full final MATCH sweep measured all **801 required units / 805 functions**:
  **772 MATCH units, 16 compiler-failure units, 13 byte-mismatch units**.
  All sixteen compilation failures subsequently MATCH after canonical repairs
  (`compile-repair-r1.json`, `compile-repair-r2.json`). Earlier failed results stay
  intact, and the changed headers require a fresh final whole-scope sweep.
  Native/signature probes did not run because the dependent shell stopped at matching.

* The stable merged-tree `interfaces-stable-r1.json` exited **1**: 2,206 units,
  1,704 checked, one Heap ODR disagreement, one duplicate-local-prototype policy
  finding and 502 unchecked C++ units, with **zero unchecked C**. Both findings
  were fixed afterward, followed by the native reader correction/withdrawal below.
  Corrected stable `interfaces-stable-r2.json` exits **0**: 2,205 units,
  1,704 checked, zero disagreements, zero policy findings, 501 unchecked C++
  and zero unchecked C. Its exact dry input snapshot is retained. Finalized Main
  `bb60cd7` is merged and stable r3 also exits0. Later compilation repairs require
  a new final gate on their changed-header tree.

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
* Remaining byte mismatches: `00113c0c`, `0018ee88`, `001c1d4c`, `001865ec`,
  `0011db78`, `0012ccd8`, `00141514`, `001c20e8`, `001eb1cc`, `001105c0`,
  `001eb190`, `001c4494`, `0019eb24`. Four queue-tail shadow records incorrectly
  extend the already-complete canonical CommandQueue. Other fixes require actual
  width/offset/ABI provenance; no steering is admitted. Exact rows remain in HANDOFF.
* Final all-new/revised matching, complete argcheck adjudication and the seven-step
  pre-review have **not** run after these changes. No full build, suite, prepare,
  finalize, history edit or push was performed.
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
L034 `001b8908` remains selected-model/v3 scope, not a default C009 addition.

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

## Predecessor boundary and newer pending remediation

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

