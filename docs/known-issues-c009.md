# C009 recovered integration — known issues

## Status

**C008-r3 merged, c009 final checks pending; not review-ready or accepted.**
Continued `9d6f051`, collection `b63317e` and remediation `d031cce` without restarting searches.
The original sixteen-address boundary is closed by six canonical MATCH destinations
and ten recorded deferrals. Three further interface/tooling deferrals are below.
`docs/GATES.md` stays v1.16. Main owns full builds/suite and acceptance.
The user requested wrap-up at the merged continuation milestone. Later canonical
source edits are preserved but unexercised; no merged-tree gate success or pre-review
PASS is claimed. Resume from the committed NOTES/HANDOFF, not an earlier author search.

`config/candidates/c009.json` lists **679 units / 679 functions / 57,276 target
bytes**. These are unaccepted scope totals; newer source remediation is not yet rematched:

| Frozen profile | Units/functions | Target bytes |
| --- | ---: | ---: |
| game | 616 | 51,528 |
| middleware | 38 | 4,064 |
| sdk | 25 | 1,684 |

Previous collect inputs, successful destination receipts, dirty sources and author results
were recovered rather than restarted. L042's recovered `func_001e3af4` is included. Both
w17h SDK alternate MATCH rows (`00190688`, `001dbaac`) are included. v3/literal-record
results are not silently admitted into this default-model checkpoint.

## Blocking evidence

* The intermediate whole-ledger `interfaces --allow-unchecked-cpp` remediation run
  exited **1**: 2,205 units, 1,705 checked, 48 disagreements, 28 inline-declaration
  findings, 500 unchecked (498 C++ and the two ARMCC varargs parser failures).
  Remediation continued while that audit ran. It is not a current final count,
  and no exit-zero G3 report exists.
* The prior **166-game-header C++ co-inclusion probe exited 0** before the
  collection milestone's header changes. It is historical compile-only evidence,
  not a current native-unit, actual-signature or indirect-consumer sweep.
  Those c009-wide probes using c008-r2's reusable helpers remain unexercised.
* The catalogue recheck intersection was `00214580` (MATCH, exit 0) and
  `00203810` (compile exit 2, then deferred on concrete owner-contract evidence).
* Final all-new/revised matching, complete argcheck adjudication and the seven-step
  pre-review have **not** run after these changes. No full build, suite, prepare,
  finalize, history edit or push was performed.
* Collection closure is recorded in `collection-closure-r1.json`: canonical
  ResourceDescriptor/index-list initializers, member-array construction, style
  owner and both typed reader setups are collected; ten contract blockers are
  explicitly deferred. Every original author source matches its retained receipt hash.

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
original c008. The current dry ledger has **2,206 units / 2,280 functions**:
1,601 predecessor functions plus 679 c009 functions. The additional `0016b6a4`
deferral remains excluded.

The source delta against c008-r3 revises **122 predecessor units / 126 functions**,
including shared-header migrations and caller-evidenced constructor/callback input/result
retention. Every revised unit is listed in HANDOFF and still needs fresh final
byte/review closure. Header-only dependents need the complete canonical contract sweep.

Merged c008-r3 `642d725079b6a2154bc6f06fa554dcbeb063bcea`, retaining c009's channel
fields and adopting corrected service slots 11/13/15 plus AuxCallback's scalar
sample-count chain. Original `00115000` also proves slot14's context-taking tail
consumer (`service-notify-r1.json`); c009 keeps its before/after names and correct
types. FixC008R3 acknowledged that missed disposition and is preparing a preserving
follow-up/audit correction. Final c009 checks must include that follow-up.

## Additional collection-scope deferrals

The original sixteen-address collection boundary is closed by six new canonical
MATCH destinations and the following ten explicit deferrals. No destructor, runtime
or duplicate native-class aliases were adopted to conceal these contracts.

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

Exact sources and original author receipts remain in private
`interface-deferrals-r1.json`. The shared SurfaceTextState used by the retained
spacing/size wrappers is a qualified partial field view, not a complete recovered
native class or an alias of DisplayPane. Its actual allocation and native identity
remain unrecovered.
