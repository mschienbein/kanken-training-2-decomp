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

These are bounded remediation witnesses, not two fresh whole-ledger builds. The new
prepare must recompile the complete ledger, run G3 and the suite, and bind all current
source/header hashes. Its actual required review set is authoritative.

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

