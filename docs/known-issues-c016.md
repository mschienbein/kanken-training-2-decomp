# c016-r2 — accepted (history seq 15); F01 withdrawal and retained current contracts

**Accepted 2026-10-26 as history seq 15:** 2,832 / 4,545 functions, 310,736 / 1,120,082 B (+5 functions / +1,008 B
over c015), denominator v16 (history v13). Prepared at 9f7c9e08 (prepare.json sha256 5a54d5be…; 5 new units, 10
revised units; suite 1,205 tests).
- The first round (prepared bba58bef) is recorded on main b29069ff: codex-a PASS_WITH_QUALIFICATIONS; codex-b FAIL, F01.
- F01: two type models for the 0x00242fc8 command-buffer cursor. r2 fixes it by withdrawal:
  - the func_001306c8 native move is withdrawn, and its accepted C source is restored exactly;
  - func_00115f5c, func_001240c8, func_001303a8, func_001307b8 and func_001387f4 are held;
  - a single cursor model would have had to reopen the accepted union-punning bodies 00114778/001147c0.
- r2 reviews: codex-a and codex-b are both PASS_WITH_QUALIFICATIONS with 0 findings, and F01 is closed. Their
  argcheck runs gave 100 / 535 and 98 / 535 (codex-b with receipts), all dispositioned. Every qualification in
  those records stands.
- The 15 holds are uncredited and fall under docs/rulings/2026-10-25-held-contract-residue.md.

The text below is the candidate record.

## Candidate record

**Remediated candidate, not acceptance.** The failed c016 predecessor was prepared
at `bba58bef76fb7d0bacce6743658c00e923beb6fb`; its prepare record is
`6154a4885766f6972ed49721f461612f67ab495803b25245c65a6671a6d1659f`.
Both reviews are registered on main `b29069ff`: codex-a is
**PASS_WITH_QUALIFICATIONS, 0 findings**; codex-b is **FAIL, one P1 finding F01**.
Exact function bytes and passing mechanical gates did not close the published
cursor object's source/type contract. `accept/c016/` and the historical receipts
remain immutable and unfinalized.

c016-r2 merges that review-registration main and takes Main's authorized fallback,
including withdrawal of the native primitive provider. This record describes the
current retained scope, fresh unit comparisons, scoped interfaces and exact F01
disposition. It asserts no c016-r2 prepare, independent-review, argcheck-adjudication,
full-build or finalization result; Main supplies those checkpoint facts.

The accepted predecessor remains **c015, history seq 14: 2,827 functions /
309,728 B**, finalized at main `45d62e6d`. The denominator remains **catalogue
v16 / history v13: 4,545 functions / 1,120,082 B**. Selections stay
**armlink5-inline-branchnop-v6 / anonymous-string-records-v1**, frozen
**GATES v1.20**. There is no new compiler profile, linker/literal algorithm,
VFE rule or source-rule exception.

Paths use **`E/` = `scratch/worktrees/c016/scratch/integrator/c016/`**,
**`R/` = `E/r2/`**, **`S/` = `E/imports/scene-env/`**,
**`H/` = `E/imports/held-fixes/`**, **`W1/` = `E/imports/wave-w1/`** and
**`W3/` = `E/imports/wave-w3/`**. Expanded paths are main-checkout-relative private
evidence, not alternate gate inputs. Historical failures and qualifications are
retained rather than overwritten.

## Current c016-r2 scope

- **5 NEW functions / 1,008 B**, in five source units: four `game`, one `sdk`.
- **10 in-place accepted source-revised units / 10 functions**, unchanged from
  the retained c016 source baseline. They earn no NEW credit.
- **0 accepted moves.** `src/checkpoint10/func_001306c8.c` is restored exactly to
  its accepted c015 form; the native `.cpp` move is withdrawn.
- **15 distinct holds / 3,132 B**, all uncredited: the prior ten **1,924 B** plus
  five F01 withdrawals **1,208 B**. The primitive provider is held atomically
  with its required native caller cutover, not admitted as a passing subset.
- **2 accepted matrix-source deferrals** remain unchanged at `d93aa6c7`.
  They are neither NEW holds nor reopened source revisions.
- The dry ledger contains **2,754 units / 2,832 function identities**; its
  catalogue byte sum is **310,736 B**, not accepted progress. Library
  `func_0019e08c` (`_ZdaPv`), c015 NEW claims, scene-native, scene-font and the
  gate-failed W2 transaction are not imported as c016-r2 claims.

Authorities are `config/candidates/c016.json` (target `c016-r2`),
`R/F01-REMEDIATION.json`, `R/PROPOSED-LEDGER.json`, `R/SOURCE-DELTA.json` and the
current proof/scoped-interface records. Original native/descriptive spelling,
class/TU history and inferred duration/payload roles retain their qualifications.

### Retained NEW functions — 5 / 1,008 B

All five have fresh complete-context **MATCH** outcomes under the selected
v6/L1 algorithms. These comparisons are not checkpoint acceptance.

| Function | B | Profile | Current reconciliation |
|---|---:|---|---|
| `func_001305c0` | 264 | game | Native ActivateContext uses the evidenced aligned SceneEnvironment, exact material-activator interface and shared ShaderProgram/RenderContext/MaterialHash endpoint declarations; no new native-cursor access is admitted. |
| `func_00117ad8` | 108 | sdk | Canonical FileArchiveHandle, complete 64-bit token, writable outputs and six-input filesystem provider; no duplicate token/protocol model. |
| `func_0011ef60` | 288 | game | Canonical transition state, one-millisecond sleep and exact typed callback registrations/null inputs. |
| `func_001233d0` | 204 | game | Canonical DisplayFramebufferPair and exact `void *(*)(void *)` constructor provider, preserving original initialization/query behavior. |
| `func_00190450` | 144 | game | Canonical RequestRecord and genuine optional outputs, complete event result and aligned signed 64-bit timeout, with explicit byte conversion. |

The original fx01/fx03 four-row table is no longer a claim table: those four
renderers and native `func_001387f4` are now the explicit F01 hold rows below.
Only the ActivateContext function binding remains from the thirteen bindings
newly added by the original c016 native-graphics transaction.

## F01 remediation — authorized fallback, not a cursor-model waiver

The codex-b finding establishes that `data_00242fc8` is published as
`unsigned char *` by game declarations, while `__cb_current_command_buffer`
is published as `unsigned *` by the native renderer; both bind storage
**`0x00242fc8`**. The moved accepted renderer and four NEW renderers emitted
live accesses through the word-pointer alias. Exact ARM bytes did not make those
two pointer-object models compatible. The finding is accepted as reported,
without rerunning unchanged gates merely to confirm it.

The preferred project-wide word-cursor cutover reaches every old byte-cursor
user, including the explicitly deferred matrix bodies `func_00114778` and
`func_001147c0`. Retiring their byte spelling without repairing their existing
union punning would reopen uncertifiable rows. Main expressly forbade that
reopening and authorized the fallback **with `func_001387f4` also held**.

The exact shared blocker for all five new holds is:

> A coherent 0x00242fc8 command-buffer cursor model and the native ResPrimitive
> caller/provider cutover require reopening the accepted union-punning bodies
> func_00114778 and func_001147c0. Those bodies must remain unchanged; reopening
> without a complete lawful matrix/command owner repair is not certifiable.
> The transaction is documented held-contract residue; no callback cast,
> punning, alias bridge or VFE relaxation is admitted.

Implementation and boundaries:

1. Restore `src/checkpoint10/func_001306c8.c` exactly, accepted SHA-256
   `0a4f89533986391beec185b6283d8fd0e17c884a64cfbf016ee16cc626d92595`;
   remove the native `.cpp` owner and the declared move. The existing 240-byte
   identity is not withdrawn or re-awarded.
2. Withhold `func_00115f5c`, `func_001240c8`, `func_001303a8`, `func_001307b8`
   and `func_001387f4`. The last provider did not itself receive the direct
   cursor-reference FAIL row, but retaining it with the exact restored
   `void func_001387f4(RenderDrawList **, int)` caller would leave an incompatible
   native `ResPrimitive::SetupDrawCommand(bool)` transaction. No adapter or
   C/member alias is invented.
3. Preserve all six withdrawn C++ sources and their unused command header by
   reviewed, content-preserving moves to `R/withdrawn/`. Retire their public
   source paths, `game/render_commands.h` and twelve now-unused new native
   function bindings. Keep only the evidenced ActivateContext new binding.
4. Leave both matrix files exactly unchanged. The existing native cursor
   binding/declarations are not falsely presented as removed or unified;
   pre-existing accepted model limitations remain published, not repaired or
   newly licensed by this fallback.

`R/CURSOR-REFERENCE-AUDIT.json` records compiled relocation metadata for all five
retained NEW primary objects and the restored C object. The NEW objects contain
**no live native-cursor relocations**; the restored C object uses its original
`data_00242fc8` relocation. This is an object-reference scope statement, not a
claim that every original callee or every pre-existing program-wide cursor
interpretation has been independently recovered.

Evidence: codex-b `REVIEW.md`, `FINDINGS.json` and `CURSOR-ALIAS-EVIDENCE.json`
under `scratch/review/c016/codex-b/`; `R/review-input/` preserves the finding
metadata; `R/PREFERRED-FEASIBILITY.json`, `R/F01-REMEDIATION.json`,
`R/WITHDRAWAL-MOVES.json` and `R/SOURCE-DELTA.json` bind the authorized response.

## Inputs and reconciled boundaries

This inventory distinguishes the original c016 delivery from the smaller
current c016-r2 claim set. Historical MATCHes are not carried as claims for
withdrawn functions.

### scene-env — FixedSizeArray and ActivateContext

The scene-env input `70e33056` restores **32-byte cache-line alignment** of
FixedSizeArray's existing inline buffer using the non-dependent
`os::IAllocator::CACHE_LINE_ALIGNMENT`. The equivalent dependent expression was
rejected by ARMCC 4.1; the compatibility fix reuses the established allocator
constant without adding runtime fields, a second class model or manual gaps.
The recovered declaration/alignment is **[INFERENCE]**, supported by independent
constructor/reset/enclosing-caller observations and the complete native
ActivateContext comparison.

The upstream final re-proof covers **32 accepted units / 38 functions /
5,948 B**, all MATCH: **29 game units / 32 functions** and **3 middleware units /
6 functions**. No accepted dependency unit in that upstream closure uses `sdk`;
separate native instantiations compiled and linked under both `sdk` and
`middleware`. Their source/profile residue is not misreported as a MATCH or a
profile reassignment. The additional emitted library comparison earns no credit.

Evidence: `S/RESULT.json`, `S/armcc41/RESULT.json`,
`S/armcc41/ACCEPTED-REPROOF.json`,
`S/armcc41/COMPILER-DEPENDENCIES.json` and `S/armcc41/PROFILES.json`.
The retained 264-byte ActivateContext candidate was collected and reconciled in
c016; its local integration declarations were replaced by
`include/game/render_context.h`. The accepted hash reset provider now receives
its real nested MaterialHash, not an eleven-word RenderCache view.

### fx01 and fx03 — historical native graphics harvest, now withheld

HARVEST-LIST entries 63–66 supply fx03's matrix palette and fx01's three native
renderer methods. The three fx03 bindings and fx01's integration bindings follow
the recorded native caller/provider evidence; their overlapping matrix-copy
binding agrees, and no existing name is rebound. Native method spelling and
original source boundaries remain **[INFERENCE]**.

The five initial graphics sources, including ActivateContext, were re-derived by
`tools/collect.py` at their destinations under default v2/none before the final
v6/L1 closure proofs. The sources use canonical native owners and resource values,
not descriptive prefix casts or callback adapters. Material activation uses
`IMaterialActivator::Activate(RenderContext *, const Material *)` with its exact
native invoking interface.

Evidence: `E/COLLECT-fx01.json`, `E/COLLECT-fx03.json`,
`E/COLLECT-scene-env.json`, `E/COLLECT-RUN.json`, and the original
`scratch/authors/fx01/names/integration-matches.json` and
`scratch/authors/fx03/names/final-matches.json`.

All four fx01/fx03 NEW functions are **withheld in c016-r2** by F01. Their historical destination MATCHes and native-name evidence remain provenance, not current claims. The transaction-only bindings were retired; no coherent cursor model is claimed from those matches.

### held-fixes — native primitive transaction withdrawn atomically

Held-fixes `545491034e` originally supplied native `func_001387f4` together with
the accepted `func_001306c8` C-to-C++ move. c016-r2 admits **neither part**: the
accepted C source is restored exactly and the NEW native provider is held. The
restored caller declares `void func_001387f4(RenderDrawList **, int)`, while the
withdrawn provider is `ResPrimitive::SetupDrawCommand(bool)`; retaining only the
new provider would leave another incompatible owner/type interface. No C/member
alias or cast bridge is substituted.

The unused `game/render_commands.h` declaration and the twelve now-unused native
function bindings added for the withdrawn transaction are retired. Public source
withdrawals and that header are content-preservingly moved into private evidence;
none is a new accepted-identity move. The pre-existing native SetupDrawCommand
name binding is not a new c016-r2 provider definition or a compatibility shim.

Evidence: `H/RESULT.json`, `H/NATIVE-PRIMITIVE/RECONCILED.json`,
`R/F01-REMEDIATION.json` and `R/WITHDRAWAL-MOVES.json`. The original eight
held-fixes rows remain uncredited, and the primitive provider joins the five
new F01 holds below.

### W1 — current contracts, minus the two matrix deferrals

W1 `e9d1337a` contributes the archive-token and transition-poll functions plus six
accepted declaration/owner publications. It preserves the distinct scalar-key
registry owners, the actual ShaderResource configuration/selection, the existing
byte-cursor protocol and SoundArchiveRequestState. These limited publications do
not release the wider HEAP-REGISTRY, GPU-COMMAND-CURSOR or native shader-owner
residue families.

The full-word setter and graphics-relay transactions still fail their honest
canonical comparisons and are held. Main separately deferred the two purely
mechanical matrix-source declaration retirements after intake pre-review found
their pre-existing union punning.

Evidence: `W1/WAVES.json`, `W1/FINAL-MATCHES.json`,
`W1/FAILED-CANONICAL-TRANSACTION.json` and
`E/DEFERRED-MATRIX-REVISIONS.json`. The branch's terminated whole-ledger attempt
is no verdict; its scoped interfaces result does not replace c016's final audit.

### W3 — the actual func_00191e18 receive ABI

W3 `17c9e343c` closes the framebuffer/request transaction using original ABI
observations across **all six direct receive callsites**. The protocol retains:

- a **full integer event result**, including `-1`, not bool/byte normalization;
- all **six logical inputs**, including genuine optional memory and size outputs;
- a by-value **signed 64-bit timeout**, with a live high word and the original
  aligned stacked pair, not an incidental RequestWait object or extra argument;
- one AppletTimeout/data_00242a40 declaration in `game/service_protocol.h`.

The accepted applet caller forwards the complete result to the accepted sink;
only the sink's explicit byte store truncates it. The request exchange similarly
uses an explicit unsigned-byte conversion. The accepted request initializer/call
body remains unchanged after its duplicate prototype is retired. The framebuffer
constructor uses the exact existing generic provider rather than a native
constructor alias, and does not initialize fields that the original constructor
left uninitialized. These comparisons are not independent safety proofs for
unchecked failure paths or trusted serialized-resource bounds/alignment.

Evidence: `W3/ABI-GATE.json`, `W3/ABI-SUPPLEMENT.json`,
`W3/SOURCE-IDENTITIES.json`, `W3/MATCH-ATTEMPTS.json` and `W3/WAVES.json`.
Main authorized the extra accepted event-sink/applet.h closure. The final-main
framebuffer.h conflict was additive: c016's DisplayMemoryLayout wire value and
c015's ResourceCollectionEntry/FramebufferState owner were both preserved,
without equating them.

### Final cutoff and exclusions

fx04 closed with **0 new target MATCHes**; its ResetLightSet context is already
accepted and is not credited again. Main closed nc01–nc06 with **0 new MATCHes**.
The input set is final at c015's finalization cutoff. Scene-native, scene-font,
the gate-failed W2 transaction and library `_ZdaPv` are excluded.
Evidence: `E/FX04-DISPOSITION.json` and `E/FINAL-INPUT-CUTOFF.json`.

## Accepted in-place source revisions — 10 units / 10 functions

All ten have current complete-context MATCH evidence. They earn no NEW credit.
The former native primitive destination is no longer a candidate or revised
unit; its accepted C predecessor is restored exactly, as recorded below.

| Accepted source unit | Function | Why reopened |
|---|---|---|
| `src/checkpoint10/func_00191c94.cpp` | `func_00191c94` | Replace RenderCache with the real RenderContext::MaterialHash and eleven named fields; preserve the exact flag-to-field mapping and unsigned flags interface. |
| `src/large/func_0010de68.c` | `func_0010de68` | Publish the actual ShaderResource definition in `game/shader.h`; retain its full configuration, selection, allocation and program members. |
| `src/wave16/func_00107140.c` | `func_00107140` | Retire the identical local `func_00135f1c(int)` declaration in favor of `game/startup.h`; preserve existing callback and discarded-result contracts. |
| `src/wave18/func_0019b574.c` | `func_0019b574` | Use the existing canonical unsigned-char cursor declaration from `game/command_buffer.h`, without changing storage to word pointers. |
| `src/wave19/func_0011774c.c` | `func_0011774c` | Publish distinct OwnerIdentifierNode/OwnerIdentifierRegistry and exact `int(registry *, int)` interface; retain the returned count/fatal path. |
| `src/wave19/func_0019f518.c` | `func_0019f518` | Publish IdentifierNode/IdentifierRegistry and exact `void(registry *, int)` interface; remove identical local definitions, not the operation. |
| `src/wave8/func_001d4174.c` | `func_001d4174` | Publish SoundArchiveRequestState and exact `void(state *, ArchiveHandle *)`; do not equate it with native SoundArchiveLoader. |
| `src/checkpoint10/func_0010d454.c` | `func_0010d454` | Use the evidenced six-input receive protocol and forward its complete integer event instead of an incorrect bool result. |
| `src/wave14/func_00115628.c` | `func_00115628` | Accept the caller's full-word event and explicitly store one unsigned byte; remove the false Boolean-normalization contract. |
| `src/wave5/func_00145840.c` | `func_00145840` | Retire the identical local request-exchange declaration in favor of `game/service_state.h`; retain the RequestRecord initializer and call body. |

Exact accepted/current hashes are in the manifest's
`revised_accepted_units` and `surviving_revised_accepted_units` and in
`R/F01-REMEDIATION.json` and the fresh `R/MATCH-RESULTS.json` receipts. The
original per-function reconciliation record remains historical provenance.

### Accepted C restoration — func_001306c8 / 240 B, no move

The restored `src/checkpoint10/func_001306c8.c` is exactly its accepted c015
source, not a new C implementation or shim. Its complete 240-byte function
freshly MATCHes. The `.cpp` destination and native move are absent from the new
manifest, and the new native primitive provider is held rather than coupled to
an incompatible restored declaration. No accepted identity, source function or
byte credit is lost.

## Current re-proofs, scoped interfaces and carry-forward limits

Every required retained/restored extent in the explicit withdrawal closure is
freshly re-proved: **83 / 83 credited functions**,
**76 source units**, **74 complete comparison contexts**,
all matcher exits **0**. The scope includes the five remaining NEW claims, ten
in-place revisions, the exact accepted C restoration, the complete current
FixedSizeArray include closure and all inherited reopened R1 callers with real
helper atoms. Two shared-accessor comparisons include their genuine sibling
callers, following the established source-atom convention; no helper is trimmed
or given a fake use.

The current alignment closure is **32 units /
38 credited functions** under each unit's frozen profile. The upstream
scene-env evidence is retained separately; overlapping upstream/current function
counts are not added as credit. Uncredited helper compilation and additional
emitted library comparisons likewise add no credit.

| Current evidence / scope | Recorded outcome |
|---|---|
| `R/SCOPE.json`, `R/MATCH-RESULTS.json` | 83/83 required functions MATCH; 74 fresh complete contexts, all exit 0. |
| `R/CURSOR-REFERENCE-AUDIT.json` | Five retained NEW primary objects have no live native-cursor relocation; exact accepted C restoration uses the old byte-pointer symbol. Metadata only, no original bytes/disassembly. |
| `R/INTERFACES-SCOPED.json`, `R/INTERFACES-SCOPED-RUN.json` | **Unit-scoped only:** exit 0, 101 selected units, 0 disagreements, 0 policy items; **42 C++ units unchecked** under the explicit waiver. This is not a whole-ledger result. |
| `R/SOURCE-DELTA.json` | Exact source/header path delta relative to `7b13d779`; current target-extent proofs and separate object-container hashes, without assuming ELF-container identity. |

The final c016-r1 header index, `E/HEADER-CLOSURE-REPROOFS.json`, remains historical
provenance (**1,681 unchanged-source units / 1,710 functions**, including eleven
helper-only units). Its full post-main coverage was 1,731 functions. It is not
mislabelled as the current smaller c016-r2 scope or as extra accepted credit.
Similarly, the r1 whole-ledger interfaces PASS (**2,759 units / 773 unchecked
C++ units**) belongs to failed c016. It did not detect or waive F01 and is not a
current c016-r2 whole-ledger certificate. Main's new prepare owns that gate.

**No object-container identity is assumed.** The freshly compiled primary
objects have separately recorded hashes; the 73 primary objects with prior
worker receipts differ as containers despite identical source hashes and exact
target extents. Review carry-forward must bind the current receipts and exact
source/include inputs, not blindly reuse an earlier object hash. No changed
retained target bytes were observed in the freshly compared scope; a new full
image is not claimed before Main's c016-r2 builds.

### Exact source delta relative to 7b13d779

| Path | c016-r2 change |
|---|---|
| `src/checkpoint10/func_001306c8.c` | Restore exact accepted C source. |
| `src/checkpoint10/func_001306c8.cpp` | Withdraw native accepted-move destination; private content-preserving archive. |
| `src/checkpoint16/func_00115f5c.cpp` | Withdraw NEW claim to documented residue; private archive. |
| `src/checkpoint16/func_001240c8.cpp` | Withdraw NEW claim to documented residue; private archive. |
| `src/checkpoint16/func_001303a8.cpp` | Withdraw NEW claim to documented residue; private archive. |
| `src/checkpoint16/func_001307b8.cpp` | Withdraw NEW claim to documented residue; private archive. |
| `src/held-fixes/gfx_ResPrimitive__SetupDrawCommand.cpp` | Withdraw NEW provider atomically with native caller; private archive. |

The only header-path change is retirement of the now-unused
`include/game/render_commands.h`. All other source units, including the five
remaining NEW and ten in-place revisions, retain their exact source contents
relative to `7b13d779`. Other non-source changes are the candidate manifest,
retired transaction-only function bindings in `config/names.json`, and this
refreshed candidate record. `R/SOURCE-DELTA.json` distinguishes source changes,
withdrawn identities, target MATCH evidence and non-identical ELF containers.

### Model use and source/ABI qualifications

- **V6/R1 stays QUALIFIED.** All 31 inherited R1 callers retain their explicit
  source provenance notes and complete real helper atoms; helpers remain
  uncredited. Matching modelled bytes does not recover helper identity, original
  outlining/TU membership or historical armlink execution.
- **L1 stays QUALIFIED.** The accepted predecessor's literal-owner/model-use
  qualifications persist, including the unchanged `func_00203a68` owner. No new
  independent literal-record census, original complete-pool ownership, sharing,
  historical ordering or extra data credit is asserted here.
- **C++ remains mechanically unchecked in the stated scope.** The fresh
  42-unit scoped waiver and historical 773-unit r1 waiver are parser limits,
  not permission for incompatible pointer objects, callback casts, invented
  native/free-function aliases or dropped inputs. Exact native material
  activation and generic framebuffer constructor interfaces remain manually
  reviewed in the retained NEW source contracts.
- **Unchanged accepted model limits remain published.** Scene-vtable/member-
  callback receiver interpretations, TrainingControls and guard byte/word views
  in inherited R1 dependencies retain their c015/c016 review qualifications.
  They are not repaired, completed or newly licensed by this withdrawal. The
  retained NEW contracts do not invoke or extend those held scene/font models.
- **Names/alignment remain [INFERENCE].** ActivateContext's native spelling,
  original TU/class history and recovered 32-byte cache-line declaration are
  supported observations, not debug-source recovery or a runtime-safety proof.
  The four withdrawn native renderer identities remain only held provenance.
- **No new matrix guard-release claim remains.** With `func_001307b8` held, its
  identity-static/guard-release note is historical only. Existing accepted
  guard-release/R1 qualifications persist where still used.
- **Approved ARMCC intrinsic retained:** revised `func_0010d454` keeps the
  existing `__current_pc()` diagnostic use. This is not inline assembly or
  scheduling steering. C-only procedure callbacks retain their existing exact
  interfaces and discarded-result obligations.

These are existing integration records, not checks re-run for this document.
No c016-r2 full build/suite, whole-ledger interfaces, argcheck adjudication,
independent review or finalization is asserted by the worker record.

## Complete withheld input rows — 15 / 3,132 B, no credit

The user's held-contract/header-blocked residue ruling remains binding:
`docs/rulings/2026-10-25-held-contract-residue.md`. The prior ten blockers remain
unchanged, and all five additional F01 rows carry the same exact cursor/native
primitive transaction blocker given above. No callback-cast, punning or VFE
relaxation is pursued.

| Family | Function | B | One-line blocker | Evidence |
|---|---|---:|---|---|
| SOUND-DATA-MANAGER | `func_0014381c` | 64 | An external constructor table-address witness does not prove complete primary/secondary tables, lifetime/key and slot/provider closure; adopting the accepted freeing provider would require the excluded D0 transaction. | `H/SOUND-DATA-MANAGER/BLOCKED.json` |
| MEMORY-FILE-STREAM | `func_001d3f58` | 72 | Isolated Read MATCH does not reconcile the accepted MemoryInputStream owner with native lifetime/key, inherited stream providers and installed capability slots. | `H/MEMORY-FILE-STREAM/BLOCKED.json` |
| LAYOUT-BINARY-PROTOCOL | `func_001cbf34` | 168 | Native validator result/input widths and Pane/list insertion providers are unreconciled; runtime byte-offset node recovery or a validator-only subset is not admitted. | `H/LAYOUT-BINARY-PROTOCOL/BLOCKED.json` |
| LAYOUT-BINARY-PROTOCOL | `func_001ce9e8` | 676 | Layout::Build depends on the same unproved canonical validator and native Pane/list owner/provider transaction; no typed alias or byte-offset helper closes it. | `H/LAYOUT-BINARY-PROTOCOL/BLOCKED.json` |
| SERVICE-RESULT-FILTERS | `func_001266e4` | 72 | Both filters match in isolation, but their genuine co-emitted `func_0019e81c` translator still mismatches; no passing subset or shortened translator is imported. | `H/SERVICE-RESULT-FILTERS/BLOCKED.json` |
| SERVICE-RESULT-FILTERS | `func_0012672c` | 148 | The same complete filter/translator source atom fails; isolating the passing filter does not reconcile the real companion. | `H/SERVICE-RESULT-FILTERS/BLOCKED.json` |
| GRADE-MASK | `func_0017418c` | 108 | SavedNotification payload still has incompatible word/text/field/selection union views; the isolated UTF-16 load MATCH does not establish one producer-evidenced payload. | `H/GRADE-MASK/BLOCKED.json` |
| TRAINING-OPTIONS | `func_001b8238` | 492 | TrainingControls overlays unrelated list/animation owners; exact setter callbacks do not establish a real common controller or permit extending the punning. | `H/TRAINING-OPTIONS/BLOCKED.json` |
| W1 / FORWARD-FULLWORD | `func_001c5a08` | 20 | Honest full-word input still differs in two bytes at a complete 20-byte extent; narrowed byte formals, a second owner and steering remain forbidden. | `W1/FAILED-CANONICAL-TRANSACTION.json` |
| W1 / GRAPHICS-RELAY | `func_0012f5a0` | 104 | Canonical scalar relay/result and accepted `func_00137b48` prerequisite repair do not both match; the accepted provider/graphics header remain unchanged. | `W1/FAILED-CANONICAL-TRANSACTION.json` |
| F01 / CURSOR-PRIMITIVE-CUTOVER | `func_00115f5c` | 264 | A coherent `0x00242fc8` cursor model and native ResPrimitive caller/provider cutover require reopening accepted punning bodies `00114778`/`001147c0`, which must remain unchanged. | `R/F01-REMEDIATION.json` |
| F01 / CURSOR-PRIMITIVE-CUTOVER | `func_001240c8` | 168 | A coherent `0x00242fc8` cursor model and native ResPrimitive caller/provider cutover require reopening accepted punning bodies `00114778`/`001147c0`, which must remain unchanged. | `R/F01-REMEDIATION.json` |
| F01 / CURSOR-PRIMITIVE-CUTOVER | `func_001307b8` | 232 | A coherent `0x00242fc8` cursor model and native ResPrimitive caller/provider cutover require reopening accepted punning bodies `00114778`/`001147c0`, which must remain unchanged. | `R/F01-REMEDIATION.json` |
| F01 / CURSOR-PRIMITIVE-CUTOVER | `func_001303a8` | 392 | A coherent `0x00242fc8` cursor model and native ResPrimitive caller/provider cutover require reopening accepted punning bodies `00114778`/`001147c0`, which must remain unchanged. | `R/F01-REMEDIATION.json` |
| F01 / CURSOR-PRIMITIVE-CUTOVER | `func_001387f4` | 152 | A coherent `0x00242fc8` cursor model and native ResPrimitive caller/provider cutover require reopening accepted punning bodies `00114778`/`001147c0`, which must remain unchanged. | `R/F01-REMEDIATION.json` |

The prior ten rows total **1,924 B** and the five F01 withdrawals total
**1,208 B**. This is the complete current manifest snapshot, not a census of all
previous residue. Historical private MATCHes, an address-only table, matching
pointer widths or pre-existing accepted punning do not close these missing
contracts. The ruling changes no gate and credits nothing; no further
reconciliation or forbidden repair is attempted in c016-r2.

## Accepted matrix-command bodies — two mechanical revisions deferred

| Accepted identity / source | Pre-existing issue and disposition |
|---|---|
| `func_00114778`, `src/wave5/func_00114778.c` | Matrix44Command is accessed through matrix and command-word union views. Main deferred W1's declaration-only edit and kept the accepted source exactly at `d93aa6c7`. |
| `func_001147c0`, `src/wave5/func_001147c0.c` | Matrix34Command/ModelMatrixPacket retain multiple union views. Main deferred W1's declaration-only edit and kept the accepted source exactly at `d93aa6c7`. |

W1 proposed only replacing identical local unsigned-char cursor externs with
`game/command_buffer.h`. Reopening those bodies would require a separate complete
matrix/command owner repair under the standing no-punning rule. No such repair
or rule relaxation is attempted here. The two sources retain their local
externs; no NEW c016 function depends on those file-local declarations.

These are **unchanged accepted units, not NEW credit, NEW holds or c016 source
revisions**. Existing credit is neither withdrawn nor re-awarded, and their
accepted status is not a new endorsement of the pre-existing punning. Evidence:
manifest `deferred_mechanical_revisions`, `E/DEFERRED-MATRIX-REVISIONS.json`,
`E/PRE-REVIEW-r2.json` and `E/PRE-REVIEW-FINAL.json`.

## Rulings referenced and unchanged boundaries

- **Held contracts are documented residue:**
  `docs/rulings/2026-10-25-held-contract-residue.md`. The user declined
  proven-equivalent callback casts, a punning waiver and a VFE/null-vtable-cell
  model. c016 retains each exact blocker and undertakes no further held-contract
  reconciliation or relaxation.
- **Documented ARMCC extensions count as C:**
  `docs/rulings/2026-10-17-armcc-extensions.md`. The retained `__current_pc()` use
  is qualified; inline/embedded assembly, byte blobs and steering remain refused.
- **Explicit typed dispatch tables:**
  `docs/rulings/2026-10-25-explicit-dispatch-tables.md` does not release an
  unproved table/provider/receiver family. c016 adds no explicit-table fallback,
  imports neither scene-native nor scene-font, and prefers the matching genuine
  native C++ graphics forms actually delivered. Exact provider/caller types and
  the one-model rule remain mandatory.
- The unchanged v6/L1 selections and their provenance obligations remain governed
  by `docs/GATES.md`, `docs/LINKER.md` and `docs/ACCEPTANCE.md`; the model choices
  are not a new user ruling or acceptance. No U1/D0/VFE transaction is selected
  for this intake.
