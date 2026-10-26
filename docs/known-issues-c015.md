# c015 — accepted (history seq 14); harvested author matches and folded reconciliation

**Accepted 2026-10-26 as history seq 14:** 2,827 / 4,545 functions, 309,728 / 1,120,082 B (+31 functions / +4,720 B
over c012-r6), denominator v16 (history v13). Prepared at f483d795 (prepare.json sha256 9c208012…; 30 new units, 6
revised units, 31 dependency reviews, 56 model-use units, 25 revised headers; suite 1,205 tests). Both reviews are
PASS_WITH_QUALIFICATIONS with 0 findings (accept/c015/reviews/):
- codex-a: 69 rows, 31 PASS / 38 QUALIFIED; REVIEW.json sha256 48eb75d3…
- codex-b: 69 rows, 16 PASS / 53 QUALIFIED; REVIEW.json sha256 fc19f055…
Every qualification in those records stands. Both reviewers' argcheck runs gave 99 issues / 535 notes. Their fact delta
against the d93aa6c7 baseline agrees with Main's: 0 new issues and 3 new notes, all freshly adjudicated. The 91 holds
are uncredited, and the user ruled every held-contract function documented residue
(docs/rulings/2026-10-25-held-contract-residue.md).

The text below is the candidate record, written before the second review finished.

## Candidate record

**Prepared, not finalized.** The prepared subject is
`f483d7953a2c8ad98ff2e276434d3a3b711a571f`; its source merge is `bdee795f`
(Main `88016d37` plus c015 `5de02d31`). `accept/c015/prepare.json` is **PASS**,
SHA-256 `9c20801254a6e70c01f56bdea823e47853985fb8aad5ed272f5c3ed2a010ca8e`.
At drafting, codex-a is **PASS_WITH_QUALIFICATIONS, 0 findings**
(`scratch/review/c015/codex-a/{REVIEW.json,REVIEW.md,SEAL.json}`); the second
independent review is still in progress. No acceptance or finalized history
sequence is asserted by this candidate record.

The accepted predecessor is **c012-r6, history seq 13: 2,796 functions /
305,008 B**. C015's manifest baseline is finalized-main lineage `c3aa3cef`;
the later Main merge carries the adopted rulings without changing this intake's
31 NEW claims. The selected algorithms remain **armlink5-inline-branchnop-v6 /
anonymous-string-records-v1** under frozen **GATES v1.20**. This checkpoint does
not adopt a new linker/literal model, Thumb profile or source-rule waiver.

Paths below use **`E/` = `scratch/integrator/c015/`**, **`S/` =
`scratch/integrator/recon-small/`**, and **`A/` =
`scratch/review/c015/codex-a/`**. These are repository-relative evidence paths,
not alternate gate inputs. Historical intermediate receipts retain their original
hashes, failures and qualifications; final combined inputs supersede them where
explicitly recorded.

## Current combined scope

- **30 NEW source units / 31 NEW functions / 4,720 B**, all ordinary creditable
  owners under catalogue v16; no runtime/library or helper/data credit is added.
- **6 surviving accepted source-revised units / 7 functions**, separately listed
  below. Hash-identical sources affected only through shared headers are not
  inflated into revised-source claims.
- **0 accepted moves.** Moving unaccepted recon-small staging into
  `src/checkpoint15/recon-small/` and deleting the old unaccepted
  `src/checkpoint10/func_0014b85c.c` draft are not accepted-identity moves or losses.
- **91 distinct holds / 17,848 B**, all uncredited: initial harvest **27 /
  8,092 B**, assigned-family reconciliation **17 / 3,868 B**, and folded
  recon-small **47 / 5,888 B**. This is c015's hold snapshot, not a census of
  every predecessor or later-wave hold.
- The prepared dry ledger contains **2,749 units / 2,827 functions / 309,728 B**.
  The denominator is **catalogue v16 / history v13: 4,545 / 1,120,082 B**.
  `func_0019e08c` is library `_ZdaPv` and is never a c015 claim.

`config/candidates/c015.json`, `E/HANDOFF.json` and `accept/c015/prepare.json`
are the scope authorities. Family labels below describe current contracts; they
do not claim recovery of every original source spelling or translation unit.

### Harvested canonical-header intake — 19 NEW / 2,364 B

| Function | B | Family / current canonical contract |
|---|---:|---|
| `func_0010b598` | 52 | FramebufferState / resource-record initialization |
| `func_00136958` | 148 | Newline writer / count operation |
| `func_001376fc` | 112 | GPU transfer / GraphicsTransferCommand |
| `func_0013e260` | 88 | CTR IPC unsigned wire-word marshalling |
| `func_00147390` | 68 | ChannelBuffer initialization |
| `func_00149474` | 32 | ServiceWorker / ObjectManager service callback |
| `func_0014ce8c` | 140 | ServiceCoordinator / coefficient settings |
| `func_001652ec` | 308 | Tu582Vector2 / Orientation |
| `func_0016bf90` | 144 | L044TrainingFrame / WidgetSlot allocation |
| `func_0016e798` | 72 | CharacterPanel / embedded WidgetController |
| `func_00174210` | 276 | W6Selector / SelectorReference |
| `func_0017df84` | 88 | AudioWorkRequest / embedded SignalState |
| `func_001b6c90` | 156 | MenuOptionGroup / six DisplayHandle pointers |
| `func_001c8994` | 172 | Native geometry-resource wrappers / attribute cleanup |
| `func_001cdcfc` | 240 | Native Group / PaneLink construction |
| `func_001d2204` | 40 | Render-entry / real subobject lifetime chain |
| `func_001d603c` | 64 | W15iWavePlayer / address-only WavePlayerType group |
| `func_001efaac` | 84 | DateTime component operation |
| `func_001f586c` | 80 | CtrHandle / mutex / exact cleanup callback |

### Assigned-family reconciliation — 7 NEW / 1,340 B

| Function | B | Family / current canonical contract |
|---|---:|---|
| `func_001026dc` | 448 | SINF-BINDING / camera defaults |
| `func_001137c4` | 296 | INPUT-SERVICE |
| `func_00145e3c` | 72 | SERVICE-RESOURCE |
| `func_0014b85c` | 124 | FRAME-TIMING-CALLBACK |
| `func_0018ef74` | 164 | SERVICE-RESOURCE |
| `func_001c19e0` | 64 | RESOURCE-CACHE-CONSTRUCTOR |
| `func_001c5530` | 172 | SERVICE-RESOURCE |

The five collected families have separate `E/reconcile/<FAMILY>/DECISION.json`
records. SERVICE-RESOURCE retains exact `void(void *)` cleanup providers and one
session/state owner. INPUT-SERVICE uses the actual InputService and StaticHandle
members, an unsigned six-output provider, and typed shared-memory banks rather
than runtime byte offsets. FRAME-TIMING-CALLBACK follows the real ServiceWorker
update/context registration and invocation chain; ObjectManager is the context,
and the competing FrameTimingState model is retired. The separate resource cache
has four real records and an address-only table, not a ResourceProvider alias.

### Folded recon-small — 4 NEW / 752 B

| Function | B | Family / current canonical contract |
|---|---:|---|
| `func_0010af8c` | 340 | FILESYSTEM-WORKER |
| `func_00112fb8` | 232 | PROPERTY-LOOKUP |
| `func_00190d0c` | 92 | VAARG-PARSER |
| `func_00190d68` | 88 | VAARG-PARSER |

Recon-small `47945ebc` is folded into this single candidate. **There is no
separate c016 checkpoint or live c016 manifest.** Its archived input is
`E/fold-recon-small/C016-INPUT.json`; the source atoms now live only under
`src/checkpoint15/recon-small/` for this intake. Its three collected families and
22 blocked families are preserved in `S/<FAMILY>/BLOCKED.json` and
`E/fold-recon-small/HANDOFF.json`.

PROPERTY-LOOKUP keeps PropertyTable/PropertyReference distinct from the attribute
mapping records and preserves the caller-evidenced **64-byte hidden result**.
FILESYSTEM-WORKER uses the actual OptionalAllocation pointer/count/value/state/
stop members and **44-byte work items**, with the exact invoking thread type
**`void(void *, void *)`**, including the unused context input. VAARG-PARSER uses
standard `va_arg` and real W6Category members; no alternate category overlay or
interfaces-tool change is needed.

### Optional rv02 — 1 NEW / 264 B

| Function | B | Family / current canonical contract |
|---|---:|---|
| `func_00122680` | 264 | Optional rv02 / bounded scalar screening |

`tools/collect.py` took this dependency-free row cleanly, and the destination
source separately MATCHes. Evidence: `E/fold-recon-small/RV02-COLLECTED.json`.
The source preserves bounded scalar screening and unsigned wrapping; it is not
represented as a general primality algorithm or a new helper/header contract.

## Accepted source revisions — 6 units / 7 functions

| Accepted source unit | Functions | Why reopened |
|---|---|---|
| `src/checkpoint10/func_00132344.c` | `func_00132344` | Replace the local GPU-transfer declaration with `game/graphics_state.h`; retain the eight-input forwarding contract. |
| `src/checkpoint10/func_00121838.c` | `func_00121838` | Move shared service-lock declarations into `game/service_lock.h`; remove the stale local declaration/comment, not the operation. |
| `src/wave11/func_001312a4.c` | `func_001312a4` | Use the actual ObjectManager sample pointer, capacity and count members instead of generic word storage; one frame-timing owner. |
| `src/checkpoint11/intake-r7/np024/attributes_accessors.c` | `func_0018fdfc`, `func_0018fed0` | Remove local AttributeMapping/AttributeRecord/AttributeCursor copies; include their canonical `game/property_text.h` declarations. |
| `src/sdk2/func_001267c0.c` | `func_001267c0` | Centralize the identical AttributeMapping and actual attribute owner/table/cursor layout in `game/property_text.h`. |
| `src/wave9/func_001267f4.c` | `func_001267f4` | Use the one shared AttributeMapping/AttributeRecord/AttributeCursor contract from `game/property_text.h`. |

The exact accepted/current source hashes and function ownership are in the
manifest's `revised_accepted_units` and `surviving_revised_accepted_units`.
These revisions earn no NEW credit. Header-only re-proofs are a different
inventory, not moves or hundreds of source rewrites.

## Closure, preparation and retained limitations

### Shared-header closure and historical index qualification

The final hash-bound inventory is
**`E/fold-recon-small/HEADER-CLOSURE-REPROOFS-v16.json`**: **1,614 header-only
units / 1,628 functions**, excluding the six source-revised units above. It names
all **25 changed canonical headers**. Incremental family proofs and the final
folded closure are combined; no claim is made that their overlapping function
counts can be added together.

**The historical top-level `E/HEADER-CLOSURE-REPROOFS.json` is not the final
complete index.** Codex-a found that its 900-unit list omits **718 additional
affected unchanged accepted units after folding**. Those units are covered by
the final folded inventory or the separately re-proved source revisions; **none
is missing from final combined evidence**. The exact omission list and independent
transitive graph are in `A/HEADER-CLOSURE-AUDIT.json`. That conservative graph
finds **1,603 affected accepted units / 1,625 functions**; the review independently
samples **17 units spanning 19 changed headers**, besides matching every new and
revised function. This is an indexing/provenance qualification, not permission
to omit a closure or treat identical source hashes as identical include inputs.

The 25 paths are `include/game/` plus:
`audio_dsp.h`, `audio_tasks.h`, `camera_defaults.h`, `character_panel.h`,
`filesystem_worker.h`, `frame_time.h`, `frame_timing.h`, `framebuffer.h`,
`geometry_resources.h`, `graphics_state.h`, `input.h`, `input_memory.h`,
`object_manager.h`, `property_text.h`, `render_binding.h`, `resource_cache.h`,
`runtime.h`, `screen.h`, `selection.h`, `service_lock.h`, `service_state.h`,
`sound_pool.h`, `static_objects.h`, `texture_coordinates.h`, `wave_player.h`.
Their hashes are in the final inventory and prepare's revised-header bindings.

### Recorded proof and gate outcomes

| Evidence / scope | Recorded outcome |
|---|---|
| `E/PROOF-SUMMARY.json`, initial canonical closure | 903 contexts, all matcher exits 0; later changed inputs are superseded by their family/folded proofs. |
| `E/reconcile/INPUT-SERVICE/closure-r1/results.json` | 1,439 contexts / 1,473 functions, all successful; preserves complete changed-input closure. |
| `E/fold-recon-small/closure-r1/results.json` and `E/fold-recon-small/HANDOFF.json` | Final folded closure: 1,416 contexts / 1,452 functions, all matcher exits 0; driver exit 0. |
| `E/fold-recon-small/POLICY-LINT-FINAL-r2.json`, `E/fold-recon-small/LINT-DISPOSITIONS-FINAL.json` | Exit 0; 9 pattern leads + 14 proximity leads, all dispositioned; 0 second-model reports and 0 unadjudicated leads. |
| `E/fold-recon-small/INTERFACES-r2.json`, `E/fold-recon-small/INTERFACES-RUN-r2.json` | Granted whole dry-ledger run: exit 0, 2,749 units, 0 disagreements, 0 policy items; **765 C++ units unchecked** under the explicit waiver. |
| `accept/c015/prepare.json`, `accept/c015/receipts/{c015-a,c015-b}.json` | Two fresh full-mode 2,749-unit builds PASS with full-image/BLZ equality and equal credited totals; 1,205-test suite exit 0, 1 skipped, 0 failed modules; prepare PASS. |
| `A/REVIEW.json`, `A/REVIEW.md`, `A/MATCH-RUNS.json` | Independent review: 69 required rows (31 NEW + 7 revised + 31 reopened R1), 31 PASS / 38 QUALIFIED, 0 findings; 87 comparisons including 85 successful contexts. Two initial sibling-helper refusals were closed with complete unchanged helpers and genuine sibling callers, not trimming or fake uses. |

These are existing author, Main and reviewer receipts, not checks re-run for this
document. Preparation and one sealed review do not replace the other independent
review or finalization.

### V6 / L1 and mechanically unchecked C++

- **V6/R1 remains QUALIFIED.** All **31 reopened R1 callers** retain explicit
  provenance qualifications. Matching after the selected declared rewrite proves
  those modelled bytes, not removed-helper historical identity, original calls,
  outlining, original TU membership or an actual historical armlink execution.
  Complete helper sources remain uncredited; codex-a exercised all **25 helper
  sources** and the full **56-unit R1/L1 model-use closure**, including sibling
  contexts. Evidence: prepare model-use/rewrite records and `A/REVIEW.json`.
- **L1 remains QUALIFIED.** Codex-a's ELF/record/relocation census for unchanged
  `func_00203a68` covers **49 terminated records / the complete 7,284-byte
  witnessed span**, with whole-record starts, alignment, uniqueness, padding and
  bijection. It does not establish original complete pool ownership, sharing,
  surrounding records, ordering or an ordinary standalone all-source link. No
  extra data credit or extra required-function row follows.
- **The 765-unit C++ waiver is explicit parser-coverage limitation, not a type
  waiver.** `--allow-unchecked-cpp` makes interfaces exit 0 without mechanically
  checking those C++ units. Exact invoking callback types, native members,
  providers, registration writers and actual consumers therefore retain manual
  source/ABI review. Evidence: `A/CALLABLE-INVENTORY.json` and the per-function
  review rows. No callback cast, native/free-function alias, invented slot or
  discarded caller-evidenced input is licensed by this waiver.
- Existing accepted scene/PMF/source-model qualifications in unchanged R1
  dependencies remain scoped. C015 does not newly complete or extend those
  unresolved models merely because a shared header re-proves.

### New qualified source rows and exact boundary limits

| Function | Qualification / retained limit | Evidence |
|---|---|---|
| `func_0010af8c` | Approved ARMCC `__current_pc`; two-input thread callback and real allocation/work-item owners are manually checked. Casts are only from genuine opaque argument/allocation boundaries to their actual types. | `A/REVIEW.json`; `S/FILESYSTEM-WORKER/BLOCKED.json` |
| `func_0013e260` | Shared ARMCC CP15 TLS/IPC and `__svc` interfaces; unsigned wire words, pointer-address serialization and exact CtrResult conversion, not runtime union reinterpretation or inline assembler. | `A/REVIEW.json`; `E/HANDOFF.json` |
| `func_001c19e0` | ResourceCache has four real entries; incomplete CacheVTable is stored address-only, never completed as callable slots or asserted as a native-class alias. | `E/reconcile/RESOURCE-CACHE-CONSTRUCTOR/DECISION.json`; `A/REVIEW.json` |
| `func_001cdcfc` | Canonical native Group/PaneLink and existing virtual destructor/key protocol; complete 240-byte extent includes the trailing pool. C1 size 236 and C2 size 0 do not permit borrowed aliases. Historical spelling and serialized fixed-name-tail extent/alignment remain `[INFERENCE]`. | `E/NAMES-ADMISSION.json`; `A/REVIEW.json` |
| `func_001d2204` | Actual enclosing/secondary/tertiary C++ lifetime order; incomplete RenderDeleteVTable publications remain address-only, with no callable completion or recovered native-table identity. | `A/REVIEW.json`; `E/HANDOFF.json` |
| `func_001d603c` | Canonical W15iWavePlayer and real registry subobjects; one contiguous 24-word WavePlayerType address group, ending at the shared base boundary, is not a callable table or second player model. | `A/REVIEW.json`; `E/HANDOFF.json` |
| `func_001f586c` | CtrHandle-to-identical-CtrHandle memcpy and exact `void(void *)` cleanup provider; the evidenced empty registration implementation does not prove a live atexit mechanism or historical registration provenance. | `A/REVIEW.json`; `A/CALLABLE-INVENTORY.json` |

The Group fixed-name tail is a serialized-resource boundary, not byte-offset
access to a runtime object. Bounds/alignment are not independently recovered from
a body MATCH. The separate bank SplitRegions/GetRegion family below remains held;
its computed split displacement is not silently admitted under serialized-offset
qualification. The standing serialized-format note is
`scratch/integrator/contracts/SERIALIZED-FORMAT-ACCESS-NOTE.md`.

Original unchecked bounds and failure behavior are preserved, not repaired to
make source look safer: INPUT-SERVICE ignores the mapping result before
publishing pointers; VAARG-PARSER retains its original unchecked 9/12 capacities.
Null-allocation and other unchecked behavior noted in the function reviews remain
qualified where present. Exact bytes are not a safety proof.

### Names additions — five, no existing address rebindings

| Binding | Address | Admitted evidence / limit |
|---|---|---|
| `_ZN2nw3lyt5GroupC1EPKNS0_3res5GroupEPNS0_4PaneE` | `0x001cdcfc` | Canonical same-address native Group constructor self-definition; original class spelling remains `[INFERENCE]`. |
| `_ZN2nw3lyt5GroupC2EPKNS0_3res5GroupEPNS0_4PaneE` | `0x001cdcfc` | Independently agreeing original binding and compiler complete/base constructor alias; no second function credit. |
| `_ZTVN2nw3lyt5GroupE` | `0x002241e0` | Own constructor table reference; installed address point is `0x002241e8`, after the ABI header; not a function alias. |
| `__hardfp_sinf` | `0x0019d1ec` | Adopted complete library-body identity plus exact `float(float)` math.h/provider ABI proof; no guessed polynomial identity or new library credit. |
| `__hardfp_cosf` | `0x0019d074` | Same independent adopted-library and exact hard-float provider proof; no historical archive-variant claim. |

Evidence: `E/reconcile/NAMES-ADMISSION-FINAL.json`,
`E/reconcile/SINF-BINDING/PROVIDER-EVIDENCE.json`,
`E/fold-recon-small/CURRENT-CATALOGUE-CLAIMS-v16.json`, and `A/REVIEW.json`.
The earlier names-admission file retains its v15/base metadata as historical
provenance; the final prepare binds v16 and names SHA-256
`4b822cffd082a63d5b57970071bd9c5f8a3f1f0532256a0d647ba51ac588926e`.

### Argcheck — three new notes, not a clean audit

Main and codex-a each retain a separately granted audit. The reported audit exit
is **2**, with **99 issues / 535 notes over 2,827 functions**, not a clean ABI
pass. The exact accepted-baseline fact comparison yields **0 new issues,
3 new notes, 0 removed/changed baseline facts**. Only exact unchanged facts carry
the expressly permitted c012-r6 adjudications; new facts are freshly adjudicated.

All three new facts are `declared_inputs_not_observed`:

| Function / declaration | Reported inputs | Disposition and evidence |
|---|---|---|
| `func_001376fc`, `include/game/graphics_state.h` | `r0`–`r3` | Local-window false positive: accepted `func_00132344` forwards these unchanged, also preserving four stacked inputs. Keep all eight parameters and the exact shared declaration. `A/ARGCHECK-ADJUDICATIONS.json`, A01. |
| `func_001376fc`, `src/checkpoint15/func_001376fc.c` | `r0`–`r3` | Same separately reported definition fact; the body consumes the incoming pointer/unsigned values. Keep all eight inputs, not a narrowed helper shape. `A/ARGCHECK-ADJUDICATIONS.json`, A02. |
| `func_0017df84`, `include/game/audio_tasks.h` | `r0` | Getter-result false positive: `func_0017df50` supplies the real `func_00185de4()` AudioTaskManager result; all three reported tails likewise forward that unmodified result. Retain the manager and exact manually reviewed C++ provider type. `A/ARGCHECK-ADJUDICATIONS.json`, A03; `A/TAIL-MANAGER-AND-EVENT-CONTRACTS.json`. |

The facts are frozen in `E/MAIN-ARGCHECK-FACTS-r1.json`; independent fact
comparison and dispositions are in `A/ARGCHECK-FACT-DIFF.json` and
`A/ARGCHECK-ADJUDICATIONS.json`. Local argument/result visibility, stack/general
aliasing and unchecked C++ remain limitations, not missing-input proofs.

## Rulings and documented residue

- **Explicit typed dispatch tables:**
  `docs/rulings/2026-10-25-explicit-dispatch-tables.md` permits a single exact
  typed table when a genuine native form is proved unable to reproduce the
  bytes. Every cell/provider type, installing writer, actual receiver and table
  extent still needs evidence; genuine C++ remains preferred when it matches.
  The ruling is not an automatic release of old native-table holds. C015's
  three qualified opaque table families stay address-only, not simultaneously
  opaque and callable. Existing scene/PMF qualifications are not expanded.
- **Thumb residue/model boundary:**
  `docs/rulings/2026-10-25-unresolved-bytes-and-thumb.md` documents irreducible
  unknown/anomaly residue without credit. The later
  `docs/rulings/2026-10-25-thumb-linker-model.md` requires a separately reviewed
  gate/model revision before Thumb return-only branch-to-NOP library identities
  can be admitted; it is not selected by this v6/L1 checkpoint. Unproved bridge
  `0x0019f9a4` remains residue. No Thumb profile or authored Thumb C is added here.
- **Held-contract residue:**
  `docs/rulings/2026-10-25-held-contract-residue.md` makes `held_contract` and
  `header_blocked` functions **documented residue**. The user declined
  equivalent-callback-cast, punning and VFE/null-vtable-cell waivers. The
  91 c015 rows below remain uncredited and do not acquire weaker contracts or
  model exceptions. The project-wide final residue inventory belongs to STATUS
  and the ruling's machine-readable companion, not this candidate snapshot.
  New ownership/ABI/lifetime evidence or lawful matching source may reopen a
  function through an ordinary checkpoint; documented residue is not credit.
- **Catalogue v16:** adopted lineage `c3aa3cef`,
  `docs/catalogue/REPORT-v16.md`, `docs/catalogue/REVIEW-v16.md` and denominator
  history v13 classify only `func_0019e08c` / **12 B** as library `_ZdaPv`.
  They do not add C credit or alter other owner extents. C015 and recon-small
  exclude that function. Final prepare binds catalogue SHA-256
  `8fbf9c14089dc98e25a4dd918f47f08ee0bb032c16c4e06917ad85028681f3ee`.

## Complete withheld input rows — 91 / 17,848 B, no credit

Each function has a one-line primary reason and evidence pointer. Families with
shared blockers remain whole transactions, not arbitrarily narrowed matching
subsets. Pre-ruling native-only wording in historical evidence is provenance;
the current blocker is the missing exact owner/provider/source proof stated
below, not a continued prohibition of the now-allowed dispatch representation.

Codex-a checked that every held function is absent from credited definitions,
that held-only private models were not imported, and that no accepted identity
was lost. See `A/REVIEW.json` and `A/REVIEW.md`. No alias, callback cast, storage
view or invented callee is introduced to get around these holds.

### Original harvested inputs — 27 / 8,092 B, grouped by primary reason

| Primary reason / family | Function | B | One-line disposition / missing transaction | Evidence |
|---|---|---:|---|---|
| Owner / provider identity | `func_0010e3a0` | 708 | SortUpdateEntry versus sound-group ownership and the native array-constructor callback protocol are not one proved transaction. | `E/INPUT-INVENTORY.json` |
| Owner / provider identity | `func_00124b0c` | 700 | Pool-element owners and actual destructor-slot providers are unproved; guessed Resource728/488/12952 virtual classes are not admitted. | `E/INPUT-INVENTORY.json` |
| Owner / provider identity | `func_00143710` | 268 | The native SoundDataManager archive pointer cannot be supplied to the accepted C SoundArchive endpoint without an unproved owner alias. | `E/INPUT-INVENTORY.json` |
| Owner / provider identity | `func_001686dc` | 748 | PracticeItem/item-text and PracticeScreen/controller cutovers lack the necessary caller, receiver and field-ownership evidence. | `E/INPUT-INVENTORY.json` |
| Owner / provider identity | `func_0016d6dc` | 1036 | SearchMenu duplicates TrainingScreen; the L041Dialog receiver, CharacterPanel conversion and inferred removed input helper are not evidenced. | `E/INPUT-INVENTORY.json` |
| Owner / provider identity | `func_0016f288` | 276 | The canonical form mismatches; the matching variant invents an erased rectangle callee and duplicates TouchMotion/AnimationState owners. | `E/INPUT-INVENTORY.json` |
| Owner / provider identity | `func_0017e3d8` | 124 | A native AnimationTrack setter class and the exact float member-function providers are not established by the C-backed track owner. | `E/INPUT-INVENTORY.json` |
| Owner / provider identity | `func_001ae140` | 420 | The root is `ResourceDescriptor *[13]`, but the dispatcher requires NameTable pointers; equal pointer width proves no owner identity. | `E/INPUT-INVENTORY.json` |
| Owner / provider identity | `func_001b6a0c` | 380 | The initializer owns `ResourceDescriptor *[14]`, while the draft publishes NameTable pointers to a different consumer contract. | `E/INPUT-INVENTORY.json` |
| Owner / provider identity | `func_001cb088` | 132 | Native AnimGroup/MoveArray default lifetime and key/provider ownership remain unresolved; fully zeroed ABI-shadow storage is not admissible. | `E/INPUT-INVENTORY.json` |
| Owner / provider identity | `func_001d01dc` | 328 | Native/C SoundArchive receiver identity is unreconciled; removing the duplicate Window model does not establish the missing native endpoint. | `E/INPUT-INVENTORY.json` |
| Owner / provider identity | `func_001d9050` | 64 | SequencePlayer/WavePlayer common-base table and slot-provider identity is unproved; a second PlaybackVTableGroup view is not admitted. | `E/INPUT-INVENTORY.json` |
| Callback / scalar ABI | `func_00145da8` | 96 | The unsigned atomic state and signed arbiter pointer lack one canonical storage contract; an unsigned-to-int pointer reinterpretation is not allowed. | `E/INPUT-INVENTORY.json` |
| Callback / scalar ABI | `func_001b4870` | 88 | The return-this form matches, but the invoking ObjectDestructor requires `void(void *)`; that exact callback trial mismatches and cannot be cast away. | `E/INPUT-INVENTORY.json` |
| Callback / scalar ABI | `func_001d9820` | 156 | Native array-construction callbacks do not have the invoking `void *(*)(void *)` runtime type; the shared pane lifetime/provider cutover is unproved. | `E/INPUT-INVENTORY.json` |
| Storage punning / second view | `func_0014aa94` | 96 | The source reinterprets C Material as native Material and byte-addresses runtime parameter storage; neither operation has a single canonical owner model. | `E/INPUT-INVENTORY.json` |
| Storage punning / second view | `func_001563a0` | 156 | AudioChannelState/RecordState prefix views conflict with the canonical pending AudioSlot pointer and unsigned value member. | `E/INPUT-INVENTORY.json` |
| Storage punning / second view | `func_0017418c` | 108 | SavedNotification text/fields/selection/word union views lack one complete producer-evidenced payload model. | `E/NOTIFICATION-UNION-HOLD.json` |
| Storage punning / second view | `func_00196144` | 236 | Byte-derived registration nodes and the DisplayObject binding overlay lack producer/tag evidence for the claimed object identities. | `E/INPUT-INVENTORY.json` |
| Storage punning / second view | `func_001b8238` | 492 | Using both unrelated TrainingControls list/animation union arms would extend inherited punning without one producer-evidenced controller model. | `E/PRE-REVIEW-FINAL-HOLDS.json` |
| Storage punning / second view | `func_001d8638` | 220 | Four SequencePlayer reinterpret views lack genuine embedded base, ramp and volatile-variable subobjects. | `E/INPUT-INVENTORY.json` |
| Storage punning / second view | `func_001ea6d8` | 420 | Horizontal/display receivers remain incompatible, and the vector-to-float-array runtime copy is not an identical-type field copy. | `E/INPUT-INVENTORY.json` |
| Serialized record boundary | `func_001f4408` | 180 | SplitRegions computes a header/padded-bounds displacement instead of following an evidenced carried reference; canonical split/range descriptors are missing. | `E/INPUT-INVENTORY.json` |
| Serialized record boundary | `func_001f46d0` | 180 | The shared SplitRegions/GetRegion record and computed-displacement boundary remain unproved; the same author TU is not imported twice. | `E/INPUT-INVENTORY.json` |
| Honest canonical mismatch | `func_0013e18c` | 212 | The real HeapManager.vram member form is 216 versus 212 B (exit 1); an independent extern at the interior address would be a second root. | `E/PRE-REVIEW-FINAL-HOLDS.json` |
| Honest canonical mismatch | `func_001812f8` | 48 | The real StreamManager queue-member form remains 48 B but mismatches (exit 1); byte-subtraction owner recovery is forbidden. | `E/INPUT-INVENTORY.json` |
| Honest canonical mismatch | `func_00216594` | 220 | The sole TextWriterState/TextBounds/UTF-16 cursor form mismatches (exit 1); the competing native writer model cannot be restored. | `E/INPUT-INVENTORY.json` |

### Assigned reconciliation families — 17 / 3,868 B

| Primary reason / family | Function | B | One-line disposition / missing transaction | Evidence |
|---|---|---:|---|---|
| CHILD-VIRTUAL | `func_001cfdb4` | 36 | The installed setter cell is identified, but exact Material/AnimationLink receivers, slot providers and table ownership remain unproved. | `E/reconcile/CHILD-VIRTUAL/DECISION.json` |
| CONTROL-TRIPLET | `func_001ae908` | 220 | Triplet identity is settled, but enclosing-owner/input and native TouchBounds provider contracts remain incompatible. | `E/reconcile/CONTROL-TRIPLET/DECISION.json` |
| CONTROL-TRIPLET | `func_001b43bc` | 300 | Triplet identity is settled, but enclosing-owner/input and native TouchBounds provider contracts remain incompatible. | `E/reconcile/CONTROL-TRIPLET/DECISION.json` |
| CONTROL-TRIPLET | `func_001b6310` | 220 | Triplet identity is settled, but enclosing-owner/input and native TouchBounds provider contracts remain incompatible. | `E/reconcile/CONTROL-TRIPLET/DECISION.json` |
| RESOURCE-OBJECT-CONSTRUCTOR | `func_00143318` | 16 | The manual vptr record conflicts with canonical virtual ResourceObject; the real owner and complete installed table/provider transaction are unproved. | `E/reconcile/RESOURCE-OBJECT-CONSTRUCTOR/DECISION.json` |
| RESULT-COLLECTION | `func_001c1094` | 96 | Return-this cleanup casts, DisplayObject-to-ResourceEvent reinterpretation and the conflicting L044TrainingFrame companion receiver remain unresolved. | `E/reconcile/RESULT-COLLECTION/DECISION.json` |
| RESULT-COLLECTION | `func_001c15a4` | 92 | Return-this cleanup casts, DisplayObject-to-ResourceEvent reinterpretation and the conflicting L044TrainingFrame companion receiver remain unresolved. | `E/reconcile/RESULT-COLLECTION/DECISION.json` |
| RESULT-COLLECTION | `func_001c1c18` | 92 | Return-this cleanup casts, DisplayObject-to-ResourceEvent reinterpretation and the conflicting L044TrainingFrame companion receiver remain unresolved. | `E/reconcile/RESULT-COLLECTION/DECISION.json` |
| SCENE-ENVIRONMENT | `func_0012450c` | 164 | The honest SceneEnvironment::Reset stays 164 B but mismatches at +5 under game/v6/L1; no shadow layout or native-name adoption is admitted. | `E/reconcile/SCENE-ENVIRONMENT/DECISION.json` |
| STREAM-LOAD | `func_001d67c8` | 248 | The dispatcher supplies incompatible whole-owner/setup/data views; genuine request/header subobjects and decoder/arena callback providers are unproved. | `E/reconcile/STREAM-LOAD/DECISION.json` |
| STREAM-LOAD | `func_001d7318` | 1060 | The dispatcher supplies incompatible whole-owner/setup/data views; genuine request/header subobjects and decoder/arena callback providers are unproved. | `E/reconcile/STREAM-LOAD/DECISION.json` |
| STREAM-VOICE-OWNER | `func_001d51c0` | 196 | AnimationTrack/AudioVoice receivers, native SoundVoice bases/extent and work-item/synchronization/registry ownership are not one proved model. | `E/reconcile/STREAM-VOICE-OWNER/DECISION.json` |
| STREAM-VOICE-OWNER | `func_001d54b0` | 120 | AnimationTrack/AudioVoice receivers, native SoundVoice bases/extent and work-item/synchronization/registry ownership are not one proved model. | `E/reconcile/STREAM-VOICE-OWNER/DECISION.json` |
| STREAM-VOICE-OWNER | `func_001d7edc` | 128 | AnimationTrack/AudioVoice receivers, native SoundVoice bases/extent and work-item/synchronization/registry ownership are not one proved model. | `E/reconcile/STREAM-VOICE-OWNER/DECISION.json` |
| STREAM-VOICE-OWNER | `func_001d7f5c` | 332 | AnimationTrack/AudioVoice receivers, native SoundVoice bases/extent and work-item/synchronization/registry ownership are not one proved model. | `E/reconcile/STREAM-VOICE-OWNER/DECISION.json` |
| STREAM-VOICE-OWNER | `func_001d80b0` | 316 | AnimationTrack/AudioVoice receivers, native SoundVoice bases/extent and work-item/synchronization/registry ownership are not one proved model. | `E/reconcile/STREAM-VOICE-OWNER/DECISION.json` |
| STREAM-VOICE-OWNER | `func_001d82b0` | 232 | AnimationTrack/AudioVoice receivers, native SoundVoice bases/extent and work-item/synchronization/registry ownership are not one proved model. | `E/reconcile/STREAM-VOICE-OWNER/DECISION.json` |

### Folded recon-small holds — 47 / 5,888 B across 22 families

| Primary reason / family | Function | B | One-line disposition / missing transaction | Evidence |
|---|---|---:|---|---|
| COMMAND-CURSORS | `func_001dacc0` | 40 | Both installed InputDeviceVTable publications are identified, but their full receiver/provider protocol is unproved; unused virtual signatures cannot be guessed. | `S/COMMAND-CURSORS/BLOCKED.json` |
| COMMAND-CURSORS | `func_001dadc4` | 40 | Both installed InputDeviceVTable publications are identified, but their full receiver/provider protocol is unproved; unused virtual signatures cannot be guessed. | `S/COMMAND-CURSORS/BLOCKED.json` |
| DIRECTORY-NATIVE-IDENTITY | `func_0016b6a4` | 200 | The proposed U8Directory cleanup address is a FileInputStream capability-query provider; an erased call does not establish directory lifetime identity. | `S/DIRECTORY-NATIVE-IDENTITY/BLOCKED.json` |
| DRAWING-ARRAY-RUNTIME | `func_0014b4b0` | 156 | Correct callback typing alone cannot close Picture/aux-player byte-parent recovery or the TexCoordAry versus DisplayVertexArray owner conflict. | `S/DRAWING-ARRAY-RUNTIME/BLOCKED.json` |
| FONT-SLOTS | `func_00101f64` | 272 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| FONT-SLOTS | `func_0010cc94` | 24 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| FONT-SLOTS | `func_00113e20` | 88 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| FONT-SLOTS | `func_00131540` | 44 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| FONT-SLOTS | `func_0013156c` | 64 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| FONT-SLOTS | `func_0013ea40` | 116 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| FONT-SLOTS | `func_0019d9d8` | 60 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| FONT-SLOTS | `func_001cf83c` | 312 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| FONT-SLOTS | `func_001da39c` | 208 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| FONT-SLOTS | `func_001daf74` | 60 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| FONT-SLOTS | `func_001db3ec` | 24 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| FONT-SLOTS | `func_001efcec` | 28 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| FONT-SLOTS | `func_001efd08` | 120 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| FONT-SLOTS | `func_001efeac` | 56 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| FONT-SLOTS | `func_001f4920` | 44 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| FONT-SLOTS | `func_001f4994` | 28 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| FONT-SLOTS | `func_001f49b0` | 124 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| FONT-SLOTS | `func_001f4b58` | 56 | The complete 18-member FONT-SLOTS provider/cache/GlyphWidths owner transaction is unproved; guessed callable slots or an appended cache model are not admitted. | `S/FONT-SLOTS/BLOCKED.json` |
| GFX-OWNING-ARRAY | `func_001c6440` | 360 | The nested array prefix is evidenced, but the containing owner and complete installed table/provider protocol are unproved; no UiResourceContext shadow is admitted. | `S/GFX-OWNING-ARRAY/BLOCKED.json` |
| GRAPHICS-HEAP | `func_001e3794` | 36 | The root has incompatible GraphicsHeap/W19RegisteredObject/native Heap publications; exact installed allocation/lifetime providers are unreconciled. | `S/GRAPHICS-HEAP/BLOCKED.json` |
| GRAPHICS-HEAP | `func_001e37b8` | 36 | The root has incompatible GraphicsHeap/W19RegisteredObject/native Heap publications; exact installed allocation/lifetime providers are unreconciled. | `S/GRAPHICS-HEAP/BLOCKED.json` |
| GRAPHICS-HEAP | `func_001f5780` | 84 | The root has incompatible GraphicsHeap/W19RegisteredObject/native Heap publications; exact installed allocation/lifetime providers are unreconciled. | `S/GRAPHICS-HEAP/BLOCKED.json` |
| GRAPHICS-RESOURCES | `func_00104804` | 548 | The root mixes renderer, scalar words and GraphicsResource pointers; a homogeneous CleanupElement pointer array and exact release providers are unproved. | `S/GRAPHICS-RESOURCES/BLOCKED.json` |
| HEAP-FACTORIES | `func_0010d500` | 64 | Depends on the unresolved allocating Heap owner/protocol; destructor-only author classes do not establish the actual allocate/release providers. | `S/HEAP-FACTORIES/BLOCKED.json` |
| HEAP-FACTORIES | `func_0010dcb0` | 56 | Depends on the unresolved allocating Heap owner/protocol; destructor-only author classes do not establish the actual allocate/release providers. | `S/HEAP-FACTORIES/BLOCKED.json` |
| HEAP-FACTORIES | `func_0010dce8` | 56 | Depends on the unresolved allocating Heap owner/protocol; destructor-only author classes do not establish the actual allocate/release providers. | `S/HEAP-FACTORIES/BLOCKED.json` |
| HEAP-FACTORIES | `func_0010dd20` | 56 | Depends on the unresolved allocating Heap owner/protocol; destructor-only author classes do not establish the actual allocate/release providers. | `S/HEAP-FACTORIES/BLOCKED.json` |
| HEAP-FACTORIES | `func_00116be4` | 56 | Depends on the unresolved allocating Heap owner/protocol; destructor-only author classes do not establish the actual allocate/release providers. | `S/HEAP-FACTORIES/BLOCKED.json` |
| HWR-OUTPUT | `func_0014a9dc` | 184 | Word/halfword flags have no matching lawful shared representation: the word trial changes accepted 76 to 88 B, the halfword trial changes new 184 to 196 B. | `S/HWR-OUTPUT/BLOCKED.json` |
| INPUT-CONSTRUCTION | `func_0010cc0c` | 136 | Competing InputBufferState constructor overlays bind different providers; actual native subobjects and the installed table protocol remain unproved. | `S/INPUT-CONSTRUCTION/BLOCKED.json` |
| INPUT-CONSTRUCTION | `func_00116eb0` | 144 | Competing InputBufferState constructor overlays bind different providers; actual native subobjects and the installed table protocol remain unproved. | `S/INPUT-CONSTRUCTION/BLOCKED.json` |
| REGION-HEAP | `func_00146758` | 140 | Native HeapBase::FillFreeMemory is not the competing C RegionHeap owner; native HeapBase/FrameHeap clients and hook/lifetime closure remain unmigrated. | `S/REGION-HEAP/BLOCKED.json` |
| REQUEST-EXCHANGE | `func_00190450` | 144 | Five-versus-six-input receive declarations require an original ABI proof for optional outputs, stacked timeout and full integer result before the giant caller migration. | `S/REQUEST-EXCHANGE/BLOCKED.json` |
| REVERB-CONTEXT | `func_001d28f4` | 120 | Native FxReverb::Finalize conflicts with the ResourceCleanup/CompositeResource byte-overlay owner; containing lifetime and table/provider identity are unproved. | `S/REVERB-CONTEXT/BLOCKED.json` |
| SCALAR-CONVERTERS | `func_0017e6c8` | 156 | Five float(float) cells are proved before zeroed reserved storage, but the real trailing callback-array extent and unchecked byte-index domain are not. | `S/SCALAR-CONVERTERS/BLOCKED.json` |
| SLOT-BANK | `func_001b4f58` | 196 | The canonical C-bounds form mismatches at +13 (196 B); the natural local-bounds form is 184 versus 196 B, with no lawful constructor alias or steering. | `S/SLOT-BANK/BLOCKED.json` |
| SNPRINTF-BINDING | `func_00136510` | 208 | The provider is __0snprintf, not the proposed __2snprintf alias; both honest 208-B __0snprintf trials mismatch at +4. | `S/SNPRINTF-BINDING/BLOCKED.json` |
| SOUND-OPERATION | `func_001d7e6c` | 112 | The real SoundVoiceObject member/slot/subobject provider cutover remains unproved; this is an owner-contract hold, not a current inter-agent reservation. | `S/SOUND-OPERATION/BLOCKED.json` |
| STREAM-PLAYER-STOP | `func_001532e8` | 208 | The opaque StreamPlayer table lacks an evidenced Stop-slot provider/full receiver protocol and a closed prefetch-reference source-atom lifetime. | `S/STREAM-PLAYER-STOP/BLOCKED.json` |
| TEVSTAGE-RUNTIME | `func_0014b54c` | 64 | The exact void*(void*) callback and canonical TevStage are established, but natural Set/placement trials are 172/180 versus 64 B under both profiles. | `S/TEVSTAGE-RUNTIME/BLOCKED.json` |
| TEXT-WRITER-CURSOR | `func_001af644` | 248 | MarkupStateR41258 duplicates TextFlowState; flag width and the real writer/font helper contracts remain unreconciled. | `S/TEXT-WRITER-CURSOR/BLOCKED.json` |
| THREAD-ENTRY | `func_00107ce4` | 216 | The exact void(void*,void*) callback keeps its unused context but all canonical trials are 220 versus 216 B; dropping that input is forbidden. | `S/THREAD-ENTRY/BLOCKED.json` |
| WORK-COMMAND | `func_00181a4c` | 96 | The fifth word has no proved scalar/pointer/token variant contract; selecting a float union arm only to reproduce VFP copies is forbidden punning. | `S/WORK-COMMAND/BLOCKED.json` |

The hold list is exhaustive for the current c015 manifest. Reserved or merely
surveyed later-wave families are not silently added as claims or counted twice.
A private MATCH, an address-only table, equal pointer width, an erased call or
an inherited accepted union does not discharge any of these missing contracts.
