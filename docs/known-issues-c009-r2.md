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


## Sound type foundation — types-sound (2026-10-20)

This separate foundation branch completes the reachable native Voice/VoiceManager,
hardware channel, RecordState/ADPCM/waveform/envelope, StreamChannel-constructor
and target16-byte CWAR reader cutovers. It revises **58 accepted source units /
58 functions** and **17 c009 NEW source units /17 functions**, without adding
credit. `c009t.json` declares **52 accepted moves**; fourteen c009 candidate
source paths change to C++. Existing c009 moves remain in force.

All **672 affected units /684 credited identities** match sequentially under
their original profiles and explicit v6/L1. Their673 objects, including the
receipt-derived R1 helper context, have exactly one affected-scope emitted
owner for every684 credited identities inspected. The **17 header** hunk/hash
closure, full receipts, every failed exit and blocked-family disposition are
recorded in `scratch/integrator/types-sound/`. No heavy acceptance gate or
independent review was run by this worker.

The dated `MODEL-AMENDMENT-2026-10-20.md` corrects RecordState+8 from an inferred
integer to `const AdpcmLoopState*`: original00153bd8 stores the exact destination
of the preceding ADPCM copy. It also records the directly evidenced signed
loop byte and shared native envelope byte-status storage. No pointer/integer
cast or record overlay implements those facts.

Complete archive/hook producers, native null-D0/key-function ownership, the
remaining sequence/player/decoder families and G09 forwarding-stream protocol
remain open. Main explicitly authorized leaving their accepted models
unchanged rather than landing a partial native alias. **Zero deferred
functions /zero bytes are restored**. Later-checkpoint re-entry prerequisites
are enumerated in `REMAINING.json`; local loader-only MATCHes are not admission.
