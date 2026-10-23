# C009-r2 sealed failure and C009-r3 callback remediation

## Status

**C009-r2 is SEALED FAIL in both independent reviews.** C009-r3 corrects all
five reported callback-erasure units and the affected accepted callers. Its
32 final sequential bounded smoke combinations all MATCH, exit 0; integrated
gates and two fresh independent reviews remain Main's work. No credit is claimed.
Selection remains `armlink5-inline-branchnop-v6` plus
`anonymous-string-records-v1`, frozen GATES v1.19 and denominator v9-r2.
Main explicitly authorized only NEW `0011f3a8` (268 B) as a checkpoint-only,
non-exhausted code-generation deferral. The proposal retains 675 new functions /
56,784 B and every one of the 1,621 accepted identities.

## Preserved failed checkpoint

`accept/c009/` is the immutable failed preparation at source commit `88aa602`.
Both fresh builds failed the same complete-function extent check: `func_0011d260`
emitted 112 B instead of its required 116 B. G3 passed, and the full suite passed
806 tests with one skip. No reviewer was started on that failed preparation.

The regression came from refining `PipelineStorage` into a flexible-tail view.
Its fixed C prefix became 0x810 B; the initializer had incorrectly continued using
`sizeof(*storage)` for an operation that originally clears 0x854 B. This is a
source-model regression, not permission to weaken the extent check or retire credit.

## Correction and evidence

The retry reuses the correction already preserved in the c010 worktree:
`PIPELINE_STORAGE_INITIALIZED_BYTES = 0x854`, independent of the partial type's
`sizeof`. The original clear call passes the actual input pointer and the constant
2132-byte size. The flexible snapshot tail and all recovered offsets remain intact;
no total allocation size or fixed snapshot count is invented.

Private provenance:

- Existing c010 `CALLBACK-ATOMIC-ADJUDICATION-r1.json`,
  `owner-cutover/factory-facts-pipeline-r1.json`, and the nine passing
  `header-callback-match-r2` witnesses.
- Main's `scratch/integrator/c009-r2/compiled-audit-r1.json` reuses every object from
  the failed c009-a build without recompilation: 2,238 units / 2,297 functions;
  2,296 MATCH, with only `0011d260` failing extent. This reaches every ordinary
  header-dependent accepted unit, not only the earlier 838 review-required rows.
- The retry worktree's `scratch/integrator/c009-r2/pipeline-size-r1/` gives an
  isolated game-profile v6+L1 MATCH of 116/116 B, exit 0.
- Its `snapshot-consumer-r1/` gives `func_001272bc` MATCH of 28/28 B, exit 0,
  retaining the flexible-tail consumer's behavior.

The subsequent c009-r2 preparation at source `f904980` passed all six checks with
actual exit 0. Both fresh builds reproduced the complete original `code.bin`
and compressed image for 2,297 functions / 252,960 B. G3 and the 806-test suite
passed (one skip). The authoritative independent review scope was 839 functions
across 833 units; exact bytes did not close the source-level callable contracts.

## Independent review failures

Both reviews are sealed and registered under `accept/c009-r2/reviews/`.
Each independently re-derived every required function, inspected both complete
receipts, and adjudicated argcheck's actual exit 2, including 78 issues and
412 notes. The 389 mechanically unchecked ABIs remain explicit qualifications.

- **codex-b F01:** `func_00110a70` stores the int-returning `func_001900a8`
  through a `void (*)(void)` cast. The scheduler actually invokes that slot;
  ignoring a result does not make the function types call-compatible.
- **codex-b F02 / codex-a F01:** `func_00214580` converts
  `ResetReference *(*)(ResetReference *)` to the vector runtime's
  `void *(*)(void *)` callback. The runtime actually invokes it. Equal pointer
  representation alone is not a compatible C function type.
- **codex-a F01** also blocks the new singleton/destructor erasure casts in
  `func_001819f4`, `func_001eade0` and `func_00167284`. The other reviewer
  qualified those three because their inferred registration callee is empty;
  that difference does not waive the blocking source-policy finding.

All five units and the same class of callback contracts must be reconciled
before the successor is prepared. No callback cast, declaration-only erasure,
synthetic invoking trampoline, invented return value or dropped accepted
identity is an approved correction. Fresh successor gates and independent
reviews remain required.

## Qualifications retained

The source-policy, R1 identity provenance, L1 witnessed-span, C++ manual ABI-review,
ARMCC-extension, original-bug and counted-loop qualifications in
`docs/known-issues-c009.md` remain mandatory. In particular, `func_001c20e8` requires
a QUALIFIED row. The final `001e3050` correction uses its named `phase` member and
preserves the original 80-byte function; it does not use out-of-bounds `states[8]`.
The two new-function contract deferrals `00113c0c` and `0018ee88` remain explicit.

## Sealed callback findings

Both independent records remain immutable:

- CodexB `review.json` SHA-256
  `e74f84dd63232061c26eeb425d0534f5569d94ccc4e6c77a1906e0ef6b863cdc`:
  scheduler `00110a70` and vector construction `00214580` are blockers.
- CodexA `REVIEW.json` SHA-256
  `663d307b56b54a13fab1ad660b362672980afc3fbf9371a8bc9ec8a87adee17a`:
  the same finding also blocks `001819f4`, `001eade0` and `00167284`.

Discarded results and common register widths do not establish compatible C/C++
function types. The old qualifications on the three empty registration calls
are not promoted to approval.

## C009-r3 genuine vector callback and native member closure

The actual `00156da0` provider now declares and defines
`void *func_00156da0(void *storage)`. Its body recovers a `ResetReference *`
from the supplied element storage, zeroes that element and returns the same
complete pointer. `00214580` passes this function directly to the runtime's
`void *(*)(void *)` input. There is no function-pointer conversion, alias,
adapter, synthetic trampoline or truncated result.

The bounded original-reference census finds exactly one literal identity for
`00156da0`, in `00214580`, and no catalogue direct-entry callers. The supplied
array contains 24 four-byte `ResetReference` elements. The actual runtime at
`001911cc` invokes its constructor argument; its generic API is therefore
implemented by the provider itself rather than asserted over another function.
Fresh isolated witnesses give 12/12 B and 92/92 B MATCH, both exit 0.

The same-class sweep also closes the existing native `RuntimeMember` declaration
with its own `RuntimeMember::RuntimeMember()` definition at `0014de8c`.
The old C source and C declaration are removed. The candidate explicitly moves
the accepted identity to `src/wave2/func_0014de8c.cpp`; C1 and zero-size C2 are
actual same-entry symbols in its complete four-byte emitted section, not names
invented over an external definition. The native owner `0014d2e4` still constructs
32 24-byte members after its eight-byte prefix. Their bounded witnesses MATCH
4/4 B and 36/36 B, exit 0. The other original literal consumer, `001d9820`, has
no live source in this checkpoint; no caller declaration is silently replaced.
The already-compatible `void *(void *)` constructor `001c0e44` is retained;
its sole literal consumer `001ef960` independently MATCHes 36/36 B, exit 0.

Two native placement-construction attempts for `00214580` failed with the correct
92-byte extent but different argument-load/pool ordering. They remain at
`reference-array-native-r1` and `reference-array-native-r2`, actual exit 1.
Their ordering was not forced. The discarded native `ResetReference` name
bindings are absent from the live overlay; the final provider is the genuine
generic C callback above.

## C009-r3 typed no-op registration sweep

All seven explicit erasing registrations are replaced, including four accepted
callers, not just the three new findings. Each source-defined C helper preserves
the concrete object pointer, the callback's complete input/result type and the
image-base function pointer. It neither stores nor invokes the callback.

| Caller(s) | Concrete callable retained | Helper |
| --- | --- | --- |
| `001819f4` | `W10DrawingInput *(W10DrawingInput *)`, `00149a98` | `drawing_input.c` |
| `001eade0` | `StyleOwner *(StyleOwner *)`, `001eb0d0` | `style_owner.c` |
| `00167284` | `AnimationState *(AnimationState *)`, `001b44e8` | `animation_state.c` |
| `00185d1c` | `void(ObjectManager *)`, `00147260` | `object_manager.c` |
| `00185bfc`, `001916e8` | `void(StreamManager *)`, `001812f8` | `stream_manager.c` |
| `0016e270` | `PromptManager *(PromptManager *)`, `001c1970` | `prompt_manager.c` |

The six files live under `src/models/object_registration/`, are declared in
`game/functions.h`, and are all helper-only candidate units under the unchanged
`game` profile. Sequential bounded combinations MATCH all seven complete caller
extents, actual exit 0. Each new registration rewrite has `target: null` and
removes its supplied four-byte helper. Object metadata records word-aligned ARM
mapping, no retention roots, no allocated companion or group and no code/data
relocations. No metadata, compiler flags or selected linker model is patched.

An earlier typed C++ helper failed under v6 because its own exception-index
reference prevented removal (`drawing-registration-r1`, actual exit 2). That
failure remains immutable. The final C helpers are genuine C interfaces for
these C/C++ callers, not exception-metadata suppression or a v8 selection.

Every affected registration row still requires **QUALIFIED** R1 provenance:
the original call identity was erased, no original registration target is
invented, and no invocation-compatible runtime destructor protocol is claimed
from an empty operation. Source comments state this explicitly. In particular,
pointer-returning and void cleanup functions are not blanket-declared equivalent.
The retained empty entries `00164ebc`/`00216140` do not authorize incompatible
callback conversions. Native compiler-generated destructor lowering and the
existing partial-owner/member-descriptor qualifications remain distinct.

C010 separately re-derived `001b44e8` as preserving an `AnimationState *`
receiver/result and migrated its old opaque forward tag without a cast or
layout enlargement. Its `animation-counterpart-r57.json`,
`animation-counterpart-first-r58.json` and `animation-owner-match-r59` evidence
do not establish the erased registration's historical callback identity.

## Compatible C scheduler protocol

The shared slot and every revised declaration now use the same C-language
`ScheduledTaskCallback`, exactly `int (*)(void)`. The actual int-result providers
`001900a8`, `00141c70` and `001415e8` retain their real return statements and
direct-caller results. Neither the new `00110a70` registration nor accepted
`0011f234` casts a callback.

The other supplied procedure definitions remain **C**, not C++:

| Role | Retained definitions |
| --- | --- |
| Scheduled procedures | `0010433c`, `001070e8`, `00107140`, `001109fc`, `00110a28`, `00110a70`, `0011f234`, `0012d328`, `00135ae8`, `001415d0`, `0014562c` |
| Binding/wait/application-state mutation procedures | `00107a88`, `00186944`, `00190688`, `001906b8` |

Their results are unspecified and **MUST be discarded**. This is a real
source-language restriction, not an integer interpretation of residual r0.
[WG14 N1570](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf),
6.9.1 paragraph 12, makes reaching the closing brace undefined **when the caller
uses the value**. These C procedure calls do not use it. Paragraph 6.8.6.4's
constraint still prohibits a bare `return;` in a nonvoid C function: genuine
early exits were restructured, not replaced with fake values.

The restriction does **not** make C++ nonvoid fallthrough safe. `00110a70`
therefore moves from its unaccepted `.cpp` path to `.c`; all fifteen procedure
definitions compile as C under their unchanged frozen profiles. An `extern "C"`
declaration alone would not turn a C++ definition into C. Future ports must retain
the C bodies or recover genuine complete return semantics before changing that.
Nor may a wrapper write `return terminal_procedure();`: that would itself use an
unspecified callee value even if the outer caller later discarded its result.

The complete scoped reference census covers all twelve procedure candidates,
including deferred `0011f3a8`: zero catalogue direct-entry callers and thirteen
literal references. Twelve references feed the known scheduler registrations.
The additional `0010433c` reference in `00141c74` directly installs task zero's
callback/wait pair; it does not invoke that callback. That function's unrelated
indirect call reads `ApplicationControl.callback`, whose void protocol is kept.
The actual dispatcher `0010239c` invokes the shared +8 slot at `001023f4` and
discards its result.

Binding producers, the direct `00110a70` writer and the actual slot consumer were
inspected together. `00107a88` now names task zero's canonical binding rather
than a second `UpdateState` object at `bss_00495c94`. All terminal mutation calls
in the affected source are expression statements; bounded original result-read
candidates and tail paths are recorded separately, not treated as an automatic
ABI proof. No currently supplied body reads one of these unspecified results.

`0019f338` and `00135f80` retain explicit external C procedure contracts with
discard-only results; their defining sources are not supplied by this checkpoint.
Original language and historical result semantics remain unrecovered. Future
definitions must agree with the interface and either be valid C procedures under
this restriction or supply real defined results. The uncompiled callback targets
likewise remain typed external counterparts, not newly credited definitions.

All thirty-two final smoke combinations MATCH under v6+L1, including the fifteen
procedure/store definitions, the three genuine int providers, the restored
84-byte void queue operation, the seven registration combinations and the vector
constructor closure. No warning-suppression flag or invented result was added.
The matcher does not expose all successful compiler diagnostics; Main's native
audit must retain and adjudicate any diagnostics rather than assume none exist.
This is a **QUALIFIED C-only source protocol**, not recovered historical integer
result meanings, historical C ownership, a C++ fallthrough waiver or a portable
whole-program behavioral proof.

## Explicit NEW-only code-generation deferral

Main verified that `0011f3a8` is absent from the accepted ledger and explicitly
authorized only its new 268-byte unit to defer from c009-r3. The authority is
`scratch/integrator/c009-r3-11f3a8-deferral-authorization.json` on Main, copied
privately as `deferral-authorization-r1.json` (SHA-256
`464785f8a37115d67cfcce084be97e1e73e30025e8512af6160ec76357631d82`).
No other candidate is withdrawn and no accepted identity loses credit.

Its valid C mandatory-discard candidate is preserved at
`scratch/integrator/c009-r3/deferred/func_0011f3a8.c`. The function remains open
for further source/TU/permuter work; this is neither global scope reduction nor
an exhausted-residue classification.

The terminal `00186598` queue and `0019ea6c` signaling APIs remain **void**.
The accepted queue body and its original header clients are restored unchanged.
Integer-queue experiments and temporary canonical-cursor refinements remain
private failed attempts, not adopted interfaces or accepted-source regressions.
No missing scalar return or pointer-as-word meaning is invented for that queue.

Complete real queue/caller TU controls are retained too: ordinary combined and
forward-definition contexts inline the real queue operation and mismatch the
callback; the added real `00172f38` context also regresses that caller. An
explicit noinline control preserves the queue's 84-byte MATCH but still mismatches
the callback's tail-call form. Recognizing the annotation is not source-policy
approval. No noinline annotation, queue rehome, synthesized result or invoking
adapter is adopted from these failures.

The earlier `00110a70` quarantine option is preserved only as superseded
investigation evidence. That reported unit is retained, genuinely typed and
MATCHing at 52 bytes. All five reported finding units remain in this checkpoint.

## Evidence, ownership and remaining gates

Private evidence is under `scratch/integrator/c009-r3/`:

- `final-smoke-r1.json`: 32 sequential bounded combinations, 32 actual exit 0.
- `composition-r2.json` and `proposed-matches-r2.json`: exact current metadata.
- `c-procedure-consumers-r1.json`, `c-terminal-consumers-r1.json` and the
  callback-contract closure record: direct/pointer/discard obligations.
- `typed-helper-metadata-r1.json`, the individual final receipts, both complete
  C-route snapshots, the per-attempt exit index and all failed native/vector,
  integer-queue and real-callee contexts. Earlier r1 records stay immutable and
  are not relabeled as the final design.

Pure metadata composition preserves every accepted identity uniquely:
**2,243 proposed units / 2,296 functions / 252,692 B**, comprising the unchanged
1,621 accepted identities / 195,908 B plus 675 new functions / 56,784 B.
Compared with c009-r2's proposal, the only omitted identity is explicitly
authorized `0011f3a8`. The candidate has 682 units: 675 new destinations, six
uncredited helpers and the accepted native-constructor destination.

The sole accepted move is `0014de8c` from
`src/wave2/func_0014de8c.c` to `src/wave2/func_0014de8c.cpp`; its old source is
absent and the destination supplies its own emitted native definition.
Canonical `config/matches.json`, historical prepares/receipts, review seals and
progress history are unchanged. Main's fresh whole-ledger object inspection
must still establish unique emitted ownership, including aliases and COMDATs.

Main must regenerate the full required/dependency scope for these source/header
changes, then run the required combinations, whole-ledger G3, native/signature/
header closure, once-only argcheck with explicit adjudications, full suite and
prepare with both explicit frozen models. Prepare must compile the entire
proposed ledger twice; two fresh independent reviews must judge the C-only
discard contract, every R1 provenance row and the exact authorized deferral.
No heavy gate, full suite, prepare, finalize or independent review was run by
this worker.

`001c20e8`, the fixed 0x854 pipeline clear/flexible tail, the named
`001e3050.phase` store, the previous `00113c0c`/`0018ee88` deferrals and every
other retained qualification remain unchanged.

## C009-r3 sealed review FAIL; C009-r4 callable-contract remediation

This section supersedes the earlier prospective c009-r3 status, without
rewriting its evidence. **C009-r3 prepared PASS** at `dcff700`, with prepare
SHA-256 `9685e51d700e4654a47ac6a9eeff878c570e521ec1c830458cc55486878ef86e`.
Both independent reviews are sealed and registered in
`accept/c009-r3/reviews/`: **codex-a PASS_WITH_QUALIFICATIONS, zero findings;
codex-b FAIL, exactly F01/F02**. The latter seal is
`0055a37c1011ec80e2e246051543da9ff00132114731d1ec5a817256c38443c5`.
The registered history was merged from Main's `00eb9ba`; neither failed
checkpoint nor either review is retried or edited.

### F01: actual native resource values, not incompatible pointer views

Original table cells, discriminants, provider bodies and the pinned NW4C
resource definitions establish three separate one-pointer resource-value
protocols:

| Table | Compatible callable input | Actual supplied provider(s) |
| --- | --- | --- |
| `002437e0` | `nw::gfx::res::ResTextureMapper` by value | `001c8b1c`; the other slot remains an external typed counterpart |
| `002434e4` | `nw::gfx::res::ResLookupTable` by value | `001c6a34`, `001c7f10` |
| `00243510` | `nw::gfx::res::ResTexture` by value | `001c8984`, `001c8990`; the other slots remain external typed counterparts |

The dispatchers now define the actual `ResTextureMapper::Cleanup`,
`ResLookupTable::Cleanup` and `ResTexture::Cleanup` methods. Their callers pass
the native resource value directly. `001c8b1c` reads the mapper's serialized
texture reference and invokes `ResTexture::Cleanup` only when valid.
`001c7f10` clears the reference lookup table's `toTargetLut`;
`001c8984` clears the reference texture's `toTargetTexture`. These are not
audio objects, integer-selected materials, `void*` callbacks or aliases.
The original native discriminants distinguish cube `0x20000009` from shadow
`0x20000021`; the old descriptive field names did not establish those roles.

`include/game/resource_dispatch.h` declares every slot and provider with the
same real native value type. The obsolete C pointer declarations and two
misclassified audio records are removed. No function-pointer cast, erased
object view, union, invoking adapter, compiler/profile change or data credit
is introduced. The original table objects remain external.

Four accepted identities explicitly move from their `.c` files to the same
basename `.cpp` destinations: `001c6a34`, `001c7f10`, `001c8984`, `001c8990`.
Their old definitions are absent. Together with the retained earlier
`0014de8c` move, the candidate declares exactly five accepted moves.
C010's separately recovered `0013888c` must reuse these owners when Main
rebases that checkpoint; its earlier lookup-provider moves are superseded,
not duplicated.

### F02: explicit NEW-only deferrals, true table tails

The providers' own field accesses do not establish a shared complete scene
receiver: `DispatchStateA`, `DispatchStateB` and `ModeDispatchObject` read
mode at `+0x90`, while `DisplayUpdateManager` traverses its actual component
and entry families. The installers and offset-zero resemblance do not make
those free-function types compatible with every partial scene view.

Main explicitly chose a bounded checkpoint correction: do not start a broad
accepted scene-base/native-header migration. Defer the NEW invocation owners
and NEW providers whose credit introduces or keeps a contradiction; retain
every accepted identity and record pre-existing accepted issues for a later
dedicated type-model checkpoint. This is a source-contract deferral, not an
exhausted code-generation classification or a callable-type waiver.

`scene_lifecycle.h` no longer declares one fictitious common prepare receiver.
Its noninvoked `W17SceneVTable` data prefix permits typed lifecycle table-address
stores; separate concrete table tails declare the actual
`DispatchStateA*`, `DispatchStateB*`, `ModeDispatchObject*` or
`W6SelectionScreen*` prepare inputs and `DisplayUpdateManager*` finish input.
No invocation through the prefix is permitted. The unused wrong timed-screen
and command-buffer headers are removed with their only invocation owners.
No native virtual-override relationship is invented.

Every codex-b failed row has an explicit disposition:

| Finding | Required function | C009-r4 disposition |
| --- | --- | --- |
| F01 | `00143190` | Native mapper Cleanup and value-typed table |
| F01 | `00182ae4` | Native lookup Cleanup and real native providers |
| F01 | `001840dc` | Native texture Cleanup and value-typed table |
| F01 | `001c8b1c` | Actual mapper-value provider; typed texture Cleanup |
| F02 | `001acb04` | NEW invocation owner deferred |
| F02 | `001be0c0` | NEW invocation owner deferred |
| F02 | `001ef704` | NEW invocation owner deferred |
| F02 | `001eb7a4` | NEW provider deferred; accepted invocation issues carried |
| F02 | `001ab65c` | Correct isolated provider retained; incompatible NEW caller absent |
| F02 | `001bd9f0` | Correct isolated provider retained; incompatible NEW caller absent |
| F02 | `001ec194` | Correct isolated provider retained; incompatible NEW caller absent |

### Complete same-class census and carried accepted issues

The final physical-slot census covers **2,296 c009-r3 functions / 2,243 units,
231 original indirect sites, with declared source types for all 231**.
Its **333 rows** comprise **35 proven incompatible, 10 compatible and
288 unresolved**, across nine correction groups. Sixteen TLS destructor-array
cells are represented separately. Unresolved populations remain unresolved,
never assumed compatible. Final inventory SHA-256:
`08dfcf03312060010e9ac77a83ed655fe2c36aa904a7a008ac57ff31deef50ff`.

| Group | Proven incompatible rows | Disposition |
| --- | ---: | --- |
| G01 mapper | 1 | Genuine native value protocol |
| G02 lookup | 2 | Genuine native value protocol and explicit accepted moves |
| G03 texture | 1 | Genuine native value protocol and explicit accepted moves |
| G04 scene/member callbacks | 20 | 13 NEW-invocation rows deferred; 7 rows defer NEW providers and carry accepted sites |
| G05 native Pane providers | 6 | Entirely pre-existing accepted source contracts; carried unchanged |
| G06 resource accessor | 1 | Both caller and provider already accepted; carried unchanged |
| G07 material selector | 1 | NEW caller deferred; real selector is a transform pointer, not an integer |
| G08 command stream | 2 | NEW callers deferred; accepted stream/word-cursor migration required |
| G09 seek stream | 1 | NEW caller deferred; unsigned const forwarding-size contract required |

Thus all 35 rows are dispositioned: **4 fixed, 17 NEW-invocation deferrals,
7 NEW-provider deferrals with carried accepted sites, and 7 wholly pre-existing
accepted issues**. Metadata receipt comparison establishes that every carried
accepted caller/provider source below is unchanged from accepted cmodels.
This does not declare their callable contracts correct.

All pre-existing accepted invocation sites are explicitly retained:

| Accepted owner | Original site(s) | Actual incompatible target(s) | Census row(s) |
| --- | --- | --- | --- |
| `001a2814` | `001a2868` | `001eb7a4` | I0121 |
| `001a73e8` | `001a741c` | `001eb7a4` | I0125 |
| `001ab4e4` | `001ab518` | `001eb7a4` | I0129 |
| `001b9ebc` | `001b9ed0`, `001b9f14` | `001b7fc0`, `001eb7a4` | I0149, I0151 |
| `001bd630` | `001bd644`, `001bd688` | `001bbc1c`, `001eb7a4` | I0153, I0155 |
| `001cd2f0` | `001cd31c` | `001cc2e8` | I0174 |
| `001cd884` | `001cd898` | `001cdaa0`, `001cf1dc` | I0178, I0180 |
| `001cd914` | `001cd930`, `001cd950` | `001f01a8`, `001f1184`, `001f01a0`, `001f1174` | I0191, I0192, I0196, I0197 |

The first five owners still need a genuine shared scene/override model.
Pane needs real native provider receivers, `u8` material-count results and
`Material*` results; the return-only color setter must retain all actual inputs.
The resource-accessor wrapper needs the real `ResourceAccessor` receiver and
`Font*` result. Those accepted identities are not withdrawn, relabeled or
silently fixed by omitting a new provider. Main explicitly carries these issues
to a dedicated later type-model checkpoint, not as new c009 regressions.

Private evidence is `scratch/research/indirect-callable-types/` and this
worktree's `scratch/integrator/c009-r4/same-class-disposition-r1.json`.
The latter binds every row, original site/cell, differing types, source hashes,
authority and disposition. The final census correctly classifies I0147 as
unresolved: a source receiver spelling alone did not prove that table.

### Exact C009-r4 deferrals and proposal

All fourteen additional deferrals were NEW in c009-r3. Their exact sources,
hashes, original cells/types and original matching evidence are preserved;
they are absent from the proposed ledger and canonical source tree.

| Function | Target bytes | Reason/group |
| --- | ---: | --- |
| `001acb04` | 52 | Scene A prepare/finish receiver, G04/F02 |
| `001be0c0` | 68 | Scene B prepare/finish receiver, G04/F02 |
| `001ef704` | 140 | Timed-scene prepare/finish receiver, G04/F02 |
| `00162f0c` | 108 | Animation-scene update/finish receiver, G04 |
| `001b7dac` | 84 | Registered member receiver hierarchy, G04 |
| `001e3af4` | 944 | Scene callback8 receiver, G04 |
| `001e84ec` | 552 | Initialization-scene callback8 receiver, G04 |
| `001eb7a4` | 440 | NEW finish provider contradicting accepted receiver models, G04 |
| `001b7fc0` | 52 | NEW TrainingScreen provider versus accepted animated receiver, G04 |
| `001bbc1c` | 52 | NEW mode provider versus accepted animated receiver, G04 |
| `001cfdb4` | 36 | Actual material/animation-transform selector, G07 |
| `001dacc0` | 40 | Actual const MemoryWriteStream receiver, G08 |
| `001dadc4` | 40 | Actual DrawCommandState/word-cursor contract, G08 |
| `001d3e90` | 120 | Unsigned const forwarding-size contract, G09 |

Total additional deferral: **14 functions / 2,728 B**. The prior explicitly
authorized `0011f3a8` deferral remains separate: together these r3/r4 boundary
deferrals are **15 functions / 2,996 B**. Earlier historical contract deferrals
also remain unchanged; this is not a new denominator classification.

Pure composition now proposes **2,229 units / 2,282 functions / 249,964 B**:
all **1,621 accepted identities / 195,908 B**, plus **661 NEW functions /
54,056 B**. The candidate has **672 units**: 661 new destinations, six
helper-only destinations and five explicit accepted move destinations.
New profiles: game **601 / 48,548 B**, middleware **35 / 3,824 B**,
sdk **25 / 1,684 B**.

Relative to accepted cmodels, 151 surviving accepted units revise 156 functions /
21,664 B; the five explicit moved accepted functions add 36 B to the affected
review scope, not new credit. Thus accepted affected destinations total
156 units / 161 functions / 21,700 B. Main must still prove unique emitted
ownership in both full builds; metadata composition is not a substitute.

Header changes are exactly: modified `game/audio.h`, `game/functions.h`,
`game/scene_lifecycle.h`; added `game/resource_dispatch.h`; removed
`game/timed_screen.h` and `game/command_buffer.h`.
Frozen profiles, GATES v1.19, denominator v9-r2 and explicit
`armlink5-inline-branchnop-v6` + `anonymous-string-records-v1` remain unchanged.
The qualified C scheduler, typed no-op registration helpers, original counted
loop, ARMCC extensions, L1/R1 provenance and all earlier retained qualifications
remain mandatory. No c009-r4 prepare or acceptance credit is claimed here.

### C009-r4 measured evidence and remaining gates

The authorized sequential bounded closure completed with **actual exit 0**:
**1,345 credited units / 1,360 credited functions**, every required function
present and MATCH, with zero profile/model disagreements and zero failed units.
It covers all candidate destinations, all revised accepted source units,
all R1 callers, the retained F02 providers, and **947 transitive changed-header
units**. All **22 actual helper-only source units** are supplied through their
receipt-derived `--extra-source` caller combinations; fourteen of those helper
units are direct members of the affected-header scope. The one additional
emitted MATCH row is the uncredited 24-byte runtime
`__ARM_common_memset4_5`, not another credited function.

Every command uses its frozen unit profile, `tools/match.py --show 0`,
`armlink5-inline-branchnop-v6` and `anonymous-string-records-v1`, one compiler
at a time. The dependency graph uses cycle-safe conservative literal-include
reachability. Scope SHA-256:
`71487344e07429dd00bc9b51bea06d1988bd7ace621c70e520a6163ea33990cc`.
The full per-run exit/result/hash closure is
`scratch/integrator/c009-r4/header-closure-r1/RESULTS.json`, SHA-256
`b4618be578d6e9edb91872e3c69d34fc512e17aba41ee3d1d8f4adf2e3166a50`.
Main's author-scale matching grant and actual release are retained alongside it.

The eight native resource units also have independent bounded author-loop
witnesses, all MATCH/exit 0 in their final contexts. The failed initial
procedural-header compilation (exit 2) and two incorrect cube/shadow tag
associations (exit 1 each) are kept under `resource-native-r1/r2/r3`;
the corrected tag association is `resource-native-r4`, exit 0. The signed-tag
control did not fix the mismatch and is not represented as a successful
correction. Exact failed-source hashes bind the two emitted mismatch receipts.
The earlier bounded table-metadata script's KeyError and corrected new revision
are also retained, not hidden or attributed to a source defect.

`HANDOFF.json`, `same-class-disposition-r1.json`,
`contract-deferrals-r3.json`, `witness-summary-r1.json` and
`REMAINING-HEAVY-COMMANDS.json` in the worktree's integrator directory contain
the exact scope, every finding-row disposition, accepted moves, native APIs,
carried accepted sites, exits and hashes.

No full build, suite, whole-ledger G3/argcheck, prepare, finalize or independent
review was run by this worker. Main still owns the integrated typed/native/
signature/header readiness work, once-only argcheck adjudication, G3, the
prepare-owned suite and two fresh full builds, followed by two fresh
independent reviews. The carried accepted type issues and 288 open original
target populations must remain visible; MATCH does not close them.

## C009-r4 pre-review FAIL and bounded static-table correction

The independent source pre-review of **`be8aa40` FAILED** with PF01–PF06:
**23 incompatible static callable/PMF slots across 11 NEW functions / 780 B**.
Its immutable records are
`scratch/prereview/c009-r4/{PREREVIEW.json,PREREVIEW.md,TABLES.json}`.
This is not a registered acceptance review, and the freeze must not be
prepared unchanged. The earlier 231-site census covered original indirect
invocations, not every stored PMF or uninvoked installed-table declaration.
Its scope is not silently broadened to claim that pre-review found no gap.

Main authorized local corrections only inside already-NEW/revised units and
narrow headers, with fresh exact witnesses. A correct local noninvoked table
may stay honestly forward-only; it must not claim native virtual/destructor
identity. Otherwise only the affected NEW functions defer until the canonical
models under `scratch/research/type-models/` land. No accepted identity,
provider signature or accepted native class is withdrawn or changed.

### PF01 and PF05: honest noninvoked under-specifications

**PF01 — `001cfbcc`, `001cfc7c`: retained and MATCH.**
`W17LayoutVTable` is now forward-declared only. The two texture-pane operations
store the independently evidenced address **`00224370`** into their existing
first-word table pointer, but no operation reads/invokes a member of that
table. No callable slots, zero-entry table, native Pane/TextBox subclass,
complete original table extent or receiver conversion is claimed.
The native providers named by the original cells remain unchanged.
The separate dynamically invoked `W17MaterialVTable` is not falsely given
this table's population.

**PF05 — `001db104`: retained and MATCH.**
`TextDecoderBase` is an ordinary descriptive storage record with exactly one
explicit `const TextDecoderDispatch*` member. `TextDecoderDispatch` is
forward-only; its sole source use stores the original address **`00224cc0`**.
There are no virtual members, no compiler vptr plus another dispatch field,
no destructor declaration, no invoked slot and no native-destructor claim
over accepted `001db134(void*)`. The actual constructor and its qualified
two-unsigned diagnostic operation remain; C1/C2 are its own constructor
entries, not destructor aliases.

These are deliberate, documented **under-specifications**, not recovered
complete native types or source-callable interface approvals. The original
address stores and exact object-prefix effects are retained without claiming
the uninvoked cells have a compatible callable population.

The `_ZTV15TextDecoderBase` and five allocator `_ZTV` bindings were c009-only
inferred names: none exists in accepted cmodels' source commit
`85fd18dc7173e0542d8c33db157d03b7910c18c5`, and `heap_allocators.h` was not an
accepted header. Their provenance is preserved privately. No accepted native
form was replaced. Unsupported c009-only ResourceObject constructor/vtable
and deferred reader-getter own-definition names are also removed; the
decoder's actual ordinary constructor bindings remain with corrected evidence.

### PF02/PF03/PF04/PF06: eight further NEW-only deferrals

| Finding | NEW function | Target bytes | Concrete reason and re-entry requirement |
| --- | --- | ---: | --- |
| PF02 | `001db3ec` | 24 | Height/texture-count providers have different actual font receivers. Land the coordinated retail/native font constructor, provider, registry and accessor-return model from `type-models/fontgfx/`, then migrate all users. |
| PF03 | `0010d500` | 64 | Its native destructor slot contradicts accepted `001c5e48(void)`. A genuine one-opaque-pointer record emits 64 B but mismatches at `+0x28`, actual exit 1. |
| PF03 | `0010dcb0` | 56 | Accepted target `001c6ad0(void)` is not a receiver-taking destructor; the honest opaque control emits 52/56 B, mismatches at `+0x24`, exit 1. |
| PF03 | `0010dce8` | 56 | Same issue with accepted `001c6ad4(void)`; opaque control 52/56 B, `+0x24`, exit 1. |
| PF03 | `0010dd20` | 56 | Same issue with accepted `001c6ae0(void)`; opaque control 52/56 B, `+0x24`, exit 1. |
| PF03 | `00116be4` | 56 | Same issue with accepted `001c6ad8(void)`; opaque control 52/56 B, `+0x24`, exit 1. |
| PF04 | `00143318` | 16 | The guessed 30-slot ResourceObject shape crosses distinct installed address points. Land the coordinated resource/accessor, layout-binding, animation and font-manager ownership/interface/extent model before restoring its constructor. |
| PF06 | `001f4cf8` | 64 | Its u16 native-reader PMFs contradict accepted plain C byte/halfword cursor providers. Land genuine native CP1252/UTF16 definitions and all accepted moves/cursor-user migrations with the canonical font/reader model first. |

The PF03 re-entry condition is the canonical allocator/heap and lifetime
cutover from `type-models/services/` and coordinated type-model families,
including actual destructor contracts and preserved accepted-provider
migrations, followed by an exact natural constructor. Their valid opaque
controls and every failed result are kept; no padding, ABI-prefix arithmetic,
fake virtual receiver, alias or allocation/code-generation steering is adopted.
The five NEW units and now-unused allocator header are absent.

The three other deferred source units are absent too. The new
`data_00224d30` declaration and `data_0021c64c` PMF-array claim are removed.
The unused ResourceObject constructor declaration/bindings are removed;
the accepted dynamic release/delete consumer stays intact and remains an
unresolved population, not evidence of a complete 30-slot physical table.
No accepted font, resource, cursor or empty C provider loses credit.

All eight exact pre-review sources, source hashes, findings, slot evidence,
failed controls and explicit re-entry conditions are in
`scratch/integrator/c009-r4/prereview-fixes-r1/DEFERRALS.json` and the candidate.
These are checkpoint-only NEW deferrals, not exhausted-residue classifications.

### Bounded static-class sweep and current counts

The bounded follow-through dispositions all **61** pre-review static/context
tables and all **23** blocking cells: eight false callable claims are withdrawn
by the two honest opaque models; fifteen cells' NEW constructor/getter claims
are explicitly deferred. In function terms, **3 / 388 B are fixed and retained;
8 / 392 B defer**. Dynamic/opaque tables are not recorded as zero-entry tables
or as compatible simply because their members are unspecified.

Fresh sequential v6/L1 `match.py --show 0` closure gives **50 units /
62 credited functions, all MATCH, actual exit 0**. It includes all three
retained owners, **34 transitive changed-header dependents**, nineteen actual
provider units and the accepted resource-accessor caller. The nineteen
provider-source bindings are unchanged, including every accepted provider.
No expected credited row is missing. Scope SHA-256:
`2204543cc85387fa11d251c600f0403dffd1901f5db5629602955c83a10fe0fa`.
Full bounded result SHA-256:
`bd58294af28a5fe8d42f884d9984d6f4a36cf01a4290f71ae5bef7e44b322b30`.
The prior be8aa40-wide witness remains historical rather than being relabeled
as a fresh post-pre-review whole-ledger audit.

**PN01 stays explicitly carried/nonblocking by Main:** `data_00223d98` has
two real font receiver disagreements and only the unchanged accepted
`001c5b8c` installer. Its callable FontVTable completion was introduced during
c009, **not** accepted in cmodels. Neither the unchanged installer source nor
withdrawal of `001db3ec` makes that completion correct or historically accepted.
The fourteen carried accepted invocation/target rows and all 288 unresolved
original target-population rows also remain visible and unresolved.

The current proposal is **2,221 units / 2,274 functions / 249,572 B**:
all **1,621 accepted functions / 195,908 B** plus **653 NEW / 53,664 B**.
New profiles are game **593 / 48,156 B**, middleware **35 / 3,824 B**,
sdk **25 / 1,684 B**. The candidate has **664 units**.
Additional r4 deferrals now total **22 / 3,120 B**; including the earlier
`0011f3a8`, the r3/r4 boundary total is **23 / 3,388 B**.
The five explicit accepted moves and accepted affected scope remain
**156 destinations / 161 functions / 21,700 B**; no further accepted source
unit is revised by this correction.

This correction modifies only the narrow `layout_lifecycle.h`,
`font_resource.h`, `resource_objects.h` headers and removes unused
`heap_allocators.h`; names are cleaned of the unsupported c009-only claims.
No accepted source/type-family cutover, heavy job, integrated suite,
whole-ledger gate, prepare or acceptance review was run here. Main owns the
fresh integrated readiness and prepare boundary. Exact current handoff,
remaining commands and the archived `HANDOFF-be8aa40.json` preserve both states.

## C009-r4 sealed independent FAIL; C009-r5 font/converter correction

Both independent reviews of the prepared c009-r4 source
`82d9cf76762071930f0e5a493241efeb87ad5043` **SEALED FAIL**.
Its prepare was PASS, SHA-256
`c3a9015884a6edc0c21950ed11316898d1aa9850eb9ed37481f4b434bc7c894d`.
Codex-a has exactly **A-F01**, review SHA-256
`45a5577f392c607763391e90acdcd9a5a84f70ad180b36ccd2a063b17a8f3cd8`;
codex-b has exactly **B-F01**, review SHA-256
`97d93877adb9059b844ffd0022624b1b9aa0f3d22e7f63f495116bd01bd1387e`.
The original records/seals under `scratch/review/c009-r4/` remain immutable.
These findings supersede the earlier PN01 carry decision: an introduced
callable completion cannot be grandfathered by an unchanged accepted installer.
The successor is **c009-r5**, on a fresh branch/worktree from Main `83b2a22d`;
neither c009-r4 nor any earlier checkpoint name is retried.

### A-F01 — restore the accepted opaque font table

Accepted cmodels only forward-declared `FontVTable`. C009 introduced its
callable completion, thereby declaring `data_00223d98+0x10` as
`int(const FontResource*)` over accepted
`unsigned func_001f0034(const W12cRegionReader*)`, and `+0x68` with that same
declared type over accepted
`unsigned short func_001efe9c(const ResourceFontView*)`.
No equal-width/offset argument or documented carry made those function types
compatible. Both failed height wrappers invoked the introduced completion.

`game/font.h` now leaves `FontVTable` **incomplete again**. The unused
`GlyphWidths` record and completion-dependent prototypes are removed.
The unrelated recovered `FontResource` fields remain unchanged; this is not a
rollback of the texture/flag model. The accepted `001c5b8c` table-address
installer remains, but no static callable population, zero-slot table or
native font hierarchy is claimed for its opaque table.

The search also finds two NEW glyph-width consumers of the same completion.
All four functions are absent from the accepted ledger, so Main explicitly
authorized these checkpoint-only deferrals:

| Finding | NEW function | Target bytes | Reason |
| --- | --- | ---: | --- |
| A-F01 | `00131540` | 44 | Height invocation requires the incompatible completed table |
| A-F01 | `001f4920` | 44 | Height invocation requires the incompatible completed table |
| A-F01 dependent | `001efcec` | 28 | `get_widths` invocation cannot use the restored incomplete table |
| A-F01 dependent | `001f4994` | 28 | Same completion-dependent glyph-width contract |

Total: **4 NEW functions / 144 B**. Their exact matching predecessors, source
hashes, review finding IDs, reasons and re-entry conditions are preserved in
`scratch/integrator/c009-r5/FONT-DEFERRALS-r1.json`.
Re-entry requires the coordinated canonical receiver/result/table/provider
and glyph-width-user migration from `scratch/research/type-models/fontgfx/`.
No replacement private completion, accessor adapter, erased pointer,
function-pointer cast, alias or newly guessed native identity is substituted.
No accepted provider or identity is removed.

### B-F01 — one real float-to-float converter protocol

The actual original dispatcher `0017e6c8` supplies the phase in s0 and consumes
the selected float result. For selector 2, phase setup at `0017e714` precedes
the call at `0017e718`; selector 4's uncached phase setup at `0017e730`
precedes its call at `0017e734`. The previous `void(void)` identity slot and
`float(void)` random slot dropped these real source-level contracts.

`GraphicsConverters` now has **five `float(float)` fields** in the shared
`audio_voice.h`, including `identity` and `random`. Its C-language callable
types and external BSS declaration agree with every supplied provider.
`00149424` is the genuine identity function
`float func_00149424(float phase) { return phase; }`;
it is not a void no-op relying on residual VFP register state.
`00149388(float phase)` retains the supplied input even though the random
generator does not use it. Its actual returned float computation is unchanged.
Both shared prototypes and the initializer are migrated without casts,
synthetic adapters or invented values.

The initializer's observed 0x200-byte clear and exact five pointer stores
remain. The shared partial record retains its trailing storage rather than
inventing new flags or validation. Role names remain `[INFERENCE]`.
There is no credited source for consumer `0017e6c8` in this checkpoint to
rewrite; the original dispatcher evidence is retained explicitly, not replaced
with a fictitious source consumer.

The three niced, sequential local witnesses all MATCH/exit 0:
`00147478` **88 B**, accepted `00149424` **4 B** and accepted `00149388`
**32 B**. The identity provider was already revised in the r4 proposal;
the random provider adds one revised accepted unit, never new function credit.

### Current successor scope and remaining boundary

Pure composition proposes **2,217 units / 2,270 functions / 249,428 B**:
all **1,621 accepted identities / 195,908 B**, plus **649 NEW / 53,520 B**.
New profiles: game **589 / 48,012 B**, middleware **35 / 3,824 B**,
sdk **25 / 1,684 B**. The candidate has **660 units** and the same five
explicit accepted moves. Surviving accepted source revisions are
**152 units / 157 functions / 21,696 B**; including the five move destinations,
accepted affected scope is **157 units / 162 functions / 21,732 B**.

Relative to r4, only the four newly authorized font consumers defer:
**4 / 144 B**. Relative to r3, r4/r5 contract deferrals total **26 / 3,264 B**;
including earlier `0011f3a8`, the r3–r5 boundary total is **27 / 3,532 B**.
Older `00113c0c`/`0018ee88` and historical deferrals remain separate, unchanged.
No accepted identity loses credit and no new move or denominator change occurs.

The only changed headers in r5 are `game/font.h`, `game/functions.h` and
`game/audio_voice.h`. The required reproof includes every transitive
dependent of all three, the affected providers, the opaque accepted font
installer, and every receipt-bound R1 helper/caller combination.
This work is explicitly non-exclusive, niced and one compiler process at a
time. No interfaces, argcheck, suite, prepare, full build, heavy-slot request
or acceptance step is authorized or run by this worker at this boundary.
Main owns the next integrated gate decision; the recorded r4 PN01 carry is
history, not a continuing waiver of the now-removed callable completion.

### C009-r5 exercised reproof

The complete assigned bounded dependency closure finished with **actual
exit 0: 860 sequential units / 878 credited functions, all MATCH**, with every
expected result present, exact catalogue extent, frozen profile and explicit
v6/L1 model. There are no uncredited extra result rows, failed units, changed
source/header inputs or profile/model disagreements. The inherited process
niceness is **10**; only one compiler process runs at a time.

All **48 `font.h` dependents**, **808 `functions.h` dependents** and
**445 `audio_voice.h` dependents** are included: **849 distinct touched-header
units**. The additional required union includes all affected providers, the
accepted opaque font installer and every receipt-derived R1 caller closure,
with **22 supplied helper sources** through exact `--extra-source` combinations.
The six converter definition/initializer units independently MATCH in this
closure; accepted font providers `001f0034`/`001efe9c` and installer `001c5b8c`
remain byte-identical accepted sources and MATCH unchanged.

Exact scope SHA-256:
`6568e398eded359fd0cc17c97feb977e0d13ceb8a24e20bef9806ec5b81a4f27`.
Full `scratch/integrator/c009-r5/match-r1/RESULTS.json` SHA-256:
`2fa0c785f5f00d68c3f30ca2a16c58a8394b5edd2fb76bf695bbbf0e866cea92`.
Per-command exits, input source snapshots, source/object/result hashes,
helper combinations and unchanged-input bindings are retained. Earlier
successful r4 proofs are history, not relabeled as these r5 measurements.

`HANDOFF.json`, `REMAINING-HEAVY-COMMANDS.json`,
`FONT-DEFERRALS-r1.json`, `CONVERTER-RESOLUTION-r1.json`,
`ACCEPTED-FONT-PROOF-r1.json`, `CALLABLE-SCOPE-r1.json` and
`WITNESS-SUMMARY-r1.json` under `scratch/integrator/c009-r5/` bind the exact
findings, four new deferrals, preserved identities, actual float protocol and
the completed reproof. No heavy gate or heavy-slot request was run; Main's
go-ahead is still required before interfaces/argcheck/prepare/suite.

## C009-r5 sealed review FAIL; C009-r6 cursor fix and native-model deferral

C009-r5 prepared successfully at source
`c99f2c674647e91790dd2f6c4b2b337b5b70c5f3`, with records committed on Main
`e09feab5`. Both independent reviews are registered on Main `18301660`:
codex-a **PASS_WITH_QUALIFICATIONS**, codex-b **FAIL** with two blockers.
The predecessor and its reviews remain immutable. C009-r6 is a fresh branch
and worktree from `18301660`, not another attempt under the failed name.
No preparation, acceptance or new credit is claimed by this remediation.

### F01 — retain the real command-queue completion cursor

`CommandQueue.callback` and the NEW setter `001105a8` incorrectly declared
`void(void)`. Original dispatcher `00186ef0` preserves the incremented queue
cursor across its unlock, explicitly supplies that word before its indirect
tail exit, and does not merely leave an unrelated temporary in the argument
register. The actual registration in `00106740` supplies `0010ace8`; that
provider preserves the incoming cursor and compares it against completion
entry boundaries.

This is a **genuine fix, not a deferral**. The shared header now defines
`CommandQueueCompletionCallback` as `void (*)(int cursor)`, uses it for the
slot and setter, and declares the actual external provider as
`void func_0010ace8(int cursor)`. The setter definition uses the same typedef.
No function-pointer conversion, adapter, alias, fabricated provider or
synthetic registration/dispatch body is introduced.

The catalogue has one direct setter-entry caller, at `001069a8`; a bounded
aligned-address census finds one literal identity for the registered provider.
The registration caller, provider and original dispatcher have no supplied
credited source in this proposal, so their original evidence is retained
explicitly rather than claimed as newly compiled source.
Cursor signedness follows the existing queue field and remains
**[INFERENCE]**: the witnessed conditional completion stores do not establish
a signed-versus-unsigned ordering type. No observed caller consumes a callback
result; the retained void result is a bounded protocol qualification, not a
recovered historical scalar-result claim.

The local setter witness is **24/24 B MATCH, exit 0**. The complete final
reproof also includes every one of the header's **18 transitive dependents**.
Evidence is `CURSOR-RESOLUTION-r1.json`,
`CURSOR-PROVIDER-SEMANTICS-r1.json` and the sealed predecessor's
`CALLBACK-CURSOR-CONTRACT-r2.json`, provider/registration facts and bounded
consumer facts, all hash-bound under `scratch/integrator/c009-r6/`.

### F02 — remove the unauthorized DisplayTarget native-vtable model

The NEW `00117854` constructor used a `Finalize`-only virtual key without
a virtual destructor and introduced `_ZTV13DisplayTarget`. DisplayTarget
is not an accepted native-vtable precedent; its exact constructor bytes did
not authorize that different model.

The complete native class, constructor declaration and virtual key are
removed from `display_target.h`, which now exposes only an opaque receiver.
The NEW constructor leaves `src/` and the candidate manifest; its exact
predecessor source is preserved privately in
`scratch/integrator/c009-r6/deferred/func_00117854.cpp`.
All three c009-only DisplayTarget bindings, C1, C2 and `_ZTV`, are removed
from `config/names.json`. No replacement native family, destructor alias,
second callable view or cast is substituted.

This is a **NEW-only deferral: 1 function / 56 B**. There are no other NEW
source dependents of this completion. Accepted identity `00110358` retains
its explicit unused opaque receiver and freshly MATCHes; no accepted identity
is withdrawn, renamed or deferred. Re-entry requires an exact non-vtable form,
or separately authorized native-family evidence with the real providers and
every user migrated together. `DISPLAY-TARGET-DEFERRALS-r1.json` and
`DEFERRALS-r1.json` retain the exact source hashes, reason and re-entry condition.

### Proactive callback and native-binding sweep

The sweep covers all **811 surviving NEW/revised source units**, their
**972 distinct source/header files** under conservative include reachability,
**124 direct function-pointer declarators**, and the **60 predecessor
declaration changes** against accepted cmodels, adjudicated in **21 families**.
It also retains the typedef-storage, static-table, native-member-descriptor,
compiler-produced callback-reference and original consumer evidence rather
than treating a no-warning lexical search as an ABI proof.

The bounded static population includes **24 tables / 21 typed cells**,
**42 scene-array cells**, **five float converter slots** and **seven native
member descriptors**. All **51 supplied static/scene provider signatures**
are rebound to their actual current source. No additional NEW same-class
blocker was identified. Unknown dynamic provider/receiver/override populations
remain **unknown**, not compatible by default; the previously documented
accepted scene/member/dispatch limitations remain explicit and unresolved.
Opaque font/layout/decoder tables are still under-specifications, not
zero-entry or recovered native tables.

Fresh symbol-metadata inspection of **847 actual objects** from the final
reproof, including helper companions, finds **no emitted `_ZTV` symbol**.
The **11 remaining overlay `_ZTV` bindings** are identical to accepted
cmodels, with no added or changed family; DisplayTarget, allocator,
TextDecoderBase and ResourceObject native bindings are absent.
Dispatch-only virtual declarations do not by themselves claim a native table
binding or a recovered complete provider family. Exact scope, source context,
family dispositions and object hashes are in `CALLABLE-SWEEP-r1.json`,
`SUPPLIED-PROVIDER-REBINDINGS-r1.json`, `ACCEPTED-BOUNDARY-r1.json` and
`NATIVE-OBJECT-SWEEP-r1.json`.

### Current scope, exercised proof and Main-only remaining gates

Pure composition proposes **2,216 units / 2,269 functions / 249,372 B**:
all **1,621 accepted identities / 195,908 B** plus **648 NEW / 53,464 B**.
New profiles: game **588 / 47,956 B**, middleware **35 / 3,824 B**,
sdk **25 / 1,684 B**. The candidate has **659 units**, the same five explicit
accepted moves and **22 proposed helper-only units**. The previous accepted
source-revision scope is unchanged; the cursor fix and constructor deferral
do not withdraw accepted credit or change the denominator.

The new r6 deferral is **1 / 56 B**. Post-r3 contract deferrals now total
**27 / 3,320 B**; including the earlier `0011f3a8` deferral, the manifest
boundary is **28 / 3,588 B**. Older historical `00113c0c`/`0018ee88` and
earlier scopes remain separate and unchanged.

The final sequential, single-process, **niceness 10** `tools/match.py`
closure completed with **actual exit 0: 822 units / 828 credited functions,
all MATCH**. It includes all surviving NEW/revised units, all **19 distinct
touched-header dependents** (18 queue, one opaque DisplayTarget), and
**25 actual R1 caller/helper combinations covering all 22 helper sources**.
Every expected result is present with its exact extent and frozen profile;
there are no extra credited rows, profile/model disagreements, failed units
or changed inputs. Including the earlier local setter proof, all **823
matcher commands exited 0**. Explicit v6/L1 selection, frozen GATES v1.19
and denominator v9-r2 are unchanged.

Scope SHA-256:
`8ada88f5f62b493f47e6c8ce65006fb022d3698c2ead0bb3c76ccd7daaa7020b`.
Full `match-r1/RESULTS.json` SHA-256:
`2bedc494fc2d880211fd0e9fd5db8885a6a07753d2fdf32e30422d1be4082135`.
Per-command exits, source/object/result hashes and input bindings are retained.
Five metadata-query failures (four schema assumptions and a host-interpreter
import) are recorded separately; no matcher failure is hidden or relabeled.

R1 erased-callee provenance, L1 witnessed-span limitations, C-only
mandatory-discard results, C++ mechanical gaps, original bugs, counted loops,
ARMCC extensions and frozen-profile qualifications remain mandatory.
No subagent, whole-ledger interfaces/argcheck, suite, full build, prepare,
review registration, finalize or push was run by this worker.
`HANDOFF.json` and `REMAINING-HEAVY-COMMANDS.json` give Main the exact proposed
ledger and fresh output paths for the remaining gates and the new
`c009-r6` preparation, followed by two fresh independent reviews.

