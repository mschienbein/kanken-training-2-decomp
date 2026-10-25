# c012-r6 — accepted (history seq 13); combined ctypes1 + c012 known issues and qualifications

**Accepted 2026-10-25 as history seq 13:** 2,796 functions / 305,008 B (+26 functions / 3,988 B over
c011-r11), denominator v14. Prepared at 4194a785 (prepare.json sha256 42f911ff…; 42 new units, 64
revised units, 31 moves). Reviews codex-b and codex-a are both PASS_WITH_QUALIFICATIONS with 0
findings (accept/c012-r6/reviews/). Every qualification in those records stands.
- r5 (18efd7bb) failed both reviews on 3 blockers. The functions 001e395c, 001e722c and 001f4cf8 are
  held and uncredited, and the 43 deleted evidence files were restored.
- Rulings from 2026-10-25 that may later reopen held families: explicit typed dispatch tables
  (docs/rulings/2026-10-25-explicit-dispatch-tables.md); Thumb linker model
  (docs/rulings/2026-10-25-thumb-linker-model.md).
The text below is the candidate record.

## Standalone hold reconciliation candidates (2026-10-25)

`config/candidates/held-fixes.json` records later proposed contract repairs, not
accepted c012 credit. The native `ResPrimitive::SetupDrawCommand` candidate
(`001387f4`, 152 B) now has a byte-identical accepted caller transaction:
`001306c8` moves explicitly from C to C++ and becomes the canonical
`RenderContext::RenderPrimitive(ResPrimitive)` member. The renderer/resource
shadow records and C member facade are retired; both complete units MATCH.
The native word-copy binding uses the already adopted catalogue-v14 assembly
identity and earns no additional credit. Evidence and exact remaining standalone
holds are under `scratch/integrator/held-fixes/`; Main still owns checkpoint
preparation and independent acceptance review.

The standalone survey's seven groups have the following dispositions:

| Group | Candidate functions / B | Disposition |
|---|---:|---|
| Native primitive | 1 / 152 | Reconciled; canonical native caller/provider units both MATCH, with one accepted caller move/revision (240 B). |
| Sound data manager | 1 / 64 | Held: complete primary/secondary table and native lifetime/provider closure is missing; deleting-cleanup adoption is outside the no-D0 boundary. |
| Memory file stream | 1 / 72 | Held: the externalized implicit destructor/key, complete installed slots and inherited stream providers lack a proved native transaction. |
| Layout binary protocol | 2 / 844 | Held: native validator/header and Pane/intrusive-list provider contracts are not one source-policy-safe model. |
| Service result filters | 2 / 220 | Held: both filters MATCH with the complete canonical translator visible, but the co-emitted translator still mismatches; no passing subset is adopted. |
| Grade mask | 1 / 108 | Held: newer c015 notification union-storage hold remains authoritative. |
| Training options | 1 / 492 | Held: newer c015 unrelated-controller union hold remains authoritative. |

Thus only **1 NEW function / 152 B** is proposed; **6 groups / 8 functions /
1,800 B** remain uncredited. Each held group has an exact `BLOCKED.json`
prerequisite. No scene, c015-credit or library claim is duplicated. No new
destructor/key, VFE selection, runtime byte-offset view or union view is added.
Touched-unit interfaces exits **0** for the two C++ units, with the explicit
unchecked-C++ waiver; whole-ledger interfaces is deferred to c016's single
granted integration slot under Main's revised instruction.

## Current r6 remediation

Revision r6 starts from c012-r5 `4c14ff9f` and retains the explicit **v6 / L1**
selection under frozen gates **v1.20**. Both independent r5 reviews failed on the
same three blockers; their finding numbers differ for the first two:

- **Scene/member callbacks — codex-b F01 / codex-a F02:** NEW `001e395c`
  (408 B) and `001e722c` (364 B) are held, not credited. Their queued native
  descriptors select slot +12, whose actual providers remain C functions with
  different receiver contracts. Re-entry requires the genuine native hierarchy,
  adopted exact slot providers and deferred invoker together. Their sources and
  the otherwise-unused `scene_parameters.h`, including `Tu1140ParameterScene`,
  are removed; canonical accepted declarations are unchanged.
- **Font provider — codex-b F02 / codex-a F01:** NEW `001f4cf8` (64 B) is held,
  not credited. Its native `GetCharacterCode` call reaches C providers not adopted
  as `Font`/`ResFontBase` overrides. This is the only ctypes1 NEW reader; all
  ctypes1 revised units, native declarations and accepted provider types remain.
  No alias, cast or second model bridges the unresolved interface.
- **Evidence preservation — both F03:** all **43** unrelated Main `4a4da5cd`
  files listed by codex-b are restored byte-identically at their original paths,
  including `scratch/integrator/c011-NOTES.md` and `c011-r3/**`. The only remaining
  deletions from that Main tree are the **17** old source paths covered by the
  **31** declared moves.

Current candidate scope is **26 NEW / 3,988 B**, plus **31 moves / 940 B**:
**57 candidate claims / 4,928 B** in **42 units**. All **2,770** accepted identities
survive; the proposed ledger has **2,796 functions / 305,008 B** in **2,719 units**,
including all **25** helper-only units. **64 NEW holds / 14,520 B** remain
uncredited. Both revised-accepted-unit lists are retained unchanged from r5.
The remediation evidence, focused re-proofs and lint dispositions are under
`scratch/integrator/c012/r6/`; they do not constitute fresh acceptance gates.

## Preserved r5 combined scope and governing rulings

The following r5 counts and readiness statements are historical; the r6 scope
and holds above take precedence.

This candidate uses finalized main `69854e8e` (c011-r11, sequence 12), canonical
ctypes1-r4 `93921aa2` through `eb37e93e`, and c012-r4 `4a9cf7b5`. There is one
candidate, `config/candidates/c012.json`, with explicit **v6 / L1**, no U1.
Adopted denominator **v14** is merged from main `4a4da5cd`; none of its 25
reclassified owners appears in NEW, move, revised-unit or proposed-ledger claims.
Every closure identity has byte-identical full catalogue rows and protected
address/size/kind/isa/entries projections. Main authorizes carrying the observed
pre-v14 proof after the source/header bindings are mechanically unchanged; no
second full proof is run. Prepare still requires its two fresh full builds.
The separate ctypes manifest is archived in the r5 fold evidence.

- **29 NEW / 4,824 B** and **31 explicit accepted moves / 940 B**: **60 candidate
  claims / 5,764 B**, in **45 units**, including the zero-credit page-kind helper.
- All **2,770 accepted identities** survive. The proposed ledger has **2,799
  functions / 305,844 B**, **2,722 units**, including **25 helper-only units**.
- All **63 ctypes revised units** are tracked, plus the compatible c012 text-table
  revision. **61 surviving accepted source units actually change hash** versus
  main; three other ctypes revisions are covered by declared moves of `0014b438`,
  `00126f44`, `0018ce64`. This is **64 tracked revision units**, not 64 extra credit.
- **61 current NEW holds / 13,684 B** remain uncredited. NEW `00171208` (96 B) is
  additionally held: its receiver/result cannot use canonical TransitionCompletion
  and MenuTransitionState as one native PMF owner. Its canonical-declaration compile
  fails; no reinterpret cast, byte owner recovery, C facade or second class model
  is added. Accepted `00172e00`, its setter and transition header stay canonical.
- **Main's r5 ruling:** CM2-F01/F03 are known issues on unchanged ACCEPTED callers,
  outside this c012 revision scope, **not c012-r5 blockers**. Their original findings
  remain published below; no repair is claimed. Holding NEW `00171208` does not
  withdraw accepted credit. All other current holds and c009 reservations remain.

Main/c011/ctypes declarations win every merge conflict. All unrelated canonical
source/header/declaration/manifest deletions are restored. Only definitions named
by the 31 declared accepted move rows are removed; each of 17 retired source files
has its entire identity set accounted for. `MOVES-AUDIT-r1.json` binds the moves.

The signed canonical `RestorableState` fix from `4a9cf7b5` is retained; `00135908`
reads its actual `current` member, never a parallel scalar global. The touch NEW
body follows the genuine `InputSnapshot.pointer` subobject. Its three memcpy calls
copy one float into another float of the same type and extent; no runtime prefix,
union, integer overlay, storage reinterpretation or class recovery is involved.
The counted/page NEW bodies use canonical ItemPage, its signed kind member, and
W8EntryList. No W8ItemEntry shadow model is retained. All canonical declarations
that r4 had dropped are preserved.

The complete dependent proof conservatively includes the entire composed ledger,
not just selected new functions: every NEW, move, revised accepted unit, and every
accepted source/include-dependent unit. Genuine paired callers consume the two
existing accessor families; distinct canonical owners and every emitted comparison
remain explicit. The final proof, lint dispositions and granted Main-only interfaces
result are recorded under `scratch/integrator/c012/r5/`. These source comparisons
are not two fresh full-image/BLZ builds, suite, independent review or acceptance.

## Preserved r4 record — historical counts and readiness only

The remaining sections preserve r4 input dispositions and qualifications. Their
r4 counts and old “blocking/do not prepare” wording are **historical**, not the
current scope or readiness policy. The governing r5 ruling and scope above take
precedence. Serialized-format, native-owner, R1/L1 and source-provenance
qualifications below continue wherever their functions remain in this candidate.

## Reconstructed source scope, not acceptance

This candidate starts at finalized main `5820254f` (c010-r8, history sequence 11),
not the old unfinalized cmodels2-r3 lineage. Gates are v1.20; the explicit selection
is **`armlink5-inline-branchnop-v6` / `anonymous-string-records-v1`**. No U1 or
per-unit model override is admitted.

- **29 NEW functions / 4,856 B**, plus **30 explicit moves / 908 B**: **59 claims /
  5,764 B** in **44 candidate units**, including one zero-credit helper-only unit.
- All **2,682 accepted identities** survive. The unaccepted combined proposed ledger
  has **2,711 functions / 293,436 B**, with all **25 helper-only units** retained.
- **60 tracked NEW holds / 13,588 B** remain uncredited: the independent c012 49,
  all ten cmodels2-r5 holds, and the newly withheld `001456e4`. The proposed accepted
  `001c5294` move is separately withheld at its unchanged final owner. No accepted
  identity is deferred or removed.
- The three previous holds `0010c138`, `001b4310`, `00111114` remain uncredited:
  steering-dependent spellings and the narrowed decoder interface were not restored.
- The older 149-row census/author deferral snapshot is preserved as historical
  evidence in `HISTORICAL-DEFERRALS-r3.json`, not silently relabeled as a fresh
  current audit or counted again in this 103-identity re-derivation scope.

Before folding cmodels2-r5, the independent c012 source reproof ran **2,618 serial
niced matcher commands, all exit 0**: **2,708/2,708 required identities MATCH**,
including natural contexts for all **25 helper-only units**. Every emitted comparison
was MATCH; failing co-emitted function subsets are not called whole-TU passes.
Evidence: `scratch/integrator/c012/r4/match-first-r1/results.json` and
`REPROOF-FIRST-r1.json`. This component proof is not the combined reproof. Neither
source comparison is two fresh G4 full-image/BLZ receipts, an emitted-owner audit,
G3/argcheck, independent reviews, preparation, finalization or new accepted progress.

The fresh **combined** proof passes **2,619 sequential niced natural-context commands,
all exit 0**, with **2,711/2,711 required identities MATCH**, every emitted comparison
MATCH and all **25 helper-only units** covered. The actual driver exit is **0**.
Evidence: `match-combined-r2/results.json`, `REPROOF-COMBINED-r2.json` and
`DRIVER-EXIT-COMBINED-r2.json` under `scratch/integrator/c012/r4/`.

The preceding combined attempt is retained: **2,621 commands**, with **2,619 exit 0**
and **two exit 2** (actual driver exit **1**). Each failed standalone caller left
one existing accessor definition without an actual inline consumer. The correction
preserves the supplied component's genuine paired-caller contexts: `001ddde4` with
`00169018`, and `001730ec` with `001e5d38`. Both distinct canonical source owners
remain explicit; no helper was split, seeded, suppressed or given invented credit.
`NATURAL-CONTEXT-CLOSURE-r1.json` and `COMPLETE-SCOPE-r3.json` bind those contexts.
No source, type, header, name, selected model or function count changed for this
context correction, and no failing emitted subset is called a pass.

## Final-owner moves and callback authority

Every one of the old **32 move inputs** is adjudicated in `MOVES-AUDIT-r1.json`.
`001d4078` is **not accepted** in the final ledger and is held as potential NEW,
never declared as a move. `001c5294` stays at `src/sdk/func_001c5294.c`; its actual
array/atexit interface repair is still held by ctypes1. The other 30 rows name the
actual final `config/matches.json` owners, not stale c010 candidate paths.

The five legacy c010 rows are explicit: `0019ecf8`, `001db960`, `0021092c` and
`001757dc` are rebound from their final owners; `001d4078` is held. All current
c010-r8 signature/body fixes win. `0014b438` uses the actual **`void *(void *)`**
array-runtime interface, initializes actual `HwrIndexValue` storage internally and
returns that storage. It is not a native `MaterialColor` constructor alias.

`001cf418` remains unchanged at its accepted final owner. The rejected native TextBox
experiment is not an adopted private-field/header rewrite. TypesCompose F02 requires
the entire ResourceEntry/ResourceTable publication, wrapper and cleanup `0011f080`
owner transaction; pointer-table clustering is not complete table/owner proof.

## One canonical TaskPool model

The exact **611-B** header is mutually agreed with c014, SHA-256
`fdecc7599595eac61674d0984c21cb87d742d9cac4a8cd9b068972daf404115c`:
initialized byte at zero, `PoolList active` at `+4`, `available` at `+0x10`.
**0x1c is only the known prefix, not an allocation extent.** Both lists contain
actual `AudioVoiceGroup.link` nodes; no native Voice/LinkList alias is asserted.
The two accepted C prefix definitions are centralized through `audio_voice.h`.
Their bodies are unchanged and both focused comparisons MATCH.

`0017e764` is solely c012-owned and has one genuine typed release body; the unrelated
unmatched `0017df84` context is not adopted. The new `00131464` drain remains held:
the prefix does not authorize offsetof/byte subtraction. The old accepted
`0015477c` owner recovery is inherited attention, not newly approved by this header
migration. Both handoffs bind `TASK-POOL-AGREEMENT-r1.json`; c014 confirmation and
commit `479eb7f4` remain the integration authority before c012.

## Qualified serialized-format rows

Main's ruling distinguishes an encoded serialized offset from a constant byte offset
chosen to access a runtime-object field. All four conditions are recorded per row:

| Function | (a) Offset origin | (b) Typed boundary and single model | (c) Canonical helper | (d) Original bounds/alignment |
|---|---|---|---|---|
| `00116e30` | metadata_offset and count_offset_words are read from the serialized HWR header. The two-word count-section prefix is format structure, not a constant byte offset into a runtime object. | Each resolved address becomes one const HwrResourceMetadata or HwrResourceCounts pointer at the format boundary; all subsequent accesses are typed members. There is no runtime overlay, union view, memcpy reinterpretation or alternate model of an existing native object. | No canonical HWR offset resolver exists in this tree. The one small local resource_counts helper handles the encoded word-offset section; metadata is converted once at its only boundary use. | [INFERENCE] The original trusts in-bounds, naturally word-aligned records. file_size remains an ignored input; no bounds check, alignment mask or extra assumption-changing transformation is added. |
| `0010cb9c` | section_word_offset comes from the serialized record and indexes its addressed word table; no constant runtime-field byte offset is used. | The body stays in its one SerializedPacketResource word-table type and obtains counts as typed unsigned elements, with no representation-reconciliation cast. | The source-local packet_size/add_packet operations operate on values, not raw runtime storage; no native PackedResource alias is declared. | [INFERENCE] Existing valid-file/table bounds, word alignment and unsigned arithmetic are preserved, including the ignored input_size. |
| `00153ebc` | The CSPT reference table carries its own serialized offsets. 0x4000 selects a block type; it is not a byte displacement into an object. | The canonical GetBlock result is converted once to const PrefetchFileBlock* at the format boundary. | Uses the existing canonical PrefetchFileHeader::GetBlock / reference-table resolver, not a private header or another offset helper. | [INFERENCE] The original valid-reference bounds/alignment assumptions remain unchanged. No extra validation, field overlay or data credit is introduced. |

This is not permission for a runtime-object overlay, union/byte-storage view,
memcpy reinterpretation, invented native alias or extra validation. All runtime
byte-owner/shadow-view holds below remain held.

## Other retained qualifications

- `00173078` supplies its separate same-profile signed property getter, earns no
  helper credit and matches under selected v6/L1. **[INFERENCE]** Historical helper
  identity, outlining and original call graph are unrecovered. G6 requires an
  explicit **QUALIFIED R1/F03** row and all-path typed contract review.
- NEW parameter-scene initializers `001e395c` and `001e722c` are now held under
  codex-b F01 / codex-a F02. Matching value construction and descriptor transport
  did not adopt the actual native virtual providers or establish their receiver
  hierarchy; no current credit or closure claim is made for those initializers.
- `001543b8` invokes the current canonical `AuxCallback(int **, unsigned, void *)`
  interface. C010-r8 classifies original provider `00152c88` as unclaimed, not an
  adopted conflicting type. Sample numerical representation remains **[INFERENCE]**;
  no callback cast or erased specialized runtime is added.
- `00186538` preserves the actual accepted signed-byte output operation. The
  unsigned-char endpoint accesses one scalar character representation, not a
  runtime-class overlay. The alternative numeric-conversion spelling mismatched
  and is not claimed as an exact repair.
- Empty/return-only entries retain the final caller-evidenced inputs. `002175ec`
  is the original matching zero-status body with both control words preserved,
  not an invented no-op implementation.
- All three new name bindings are actual selected object symbols: two independently
  referenced empty constructor bounds and one compiler-generated initializer
  self-definition. **95 unselected research proposals are not imported.** No native
  member/destructor/vtable alias is introduced by this name cutover.

## Complete withheld input rows

These are source/type decisions, not a rejection of the original private body MATCH.
Held candidate sources/headers are quarantined outside `src/` and `include/`.

| Identity | Failure class | Disposition / missing exact transaction |
|---|---|---|
| `00107dec` | unmodeled-callback-family | The c010-r8 callback table marks startup invocation at0019bd04 unestablished for this provider. The old allocator also depends on a non-final manager prefix. Do not guess a four-input startup interface or restore that model. |
| `0010e2e0` | unauthorized-vtable | The old TransformObjectW8C model manually stores a vtable pointer and is absent from final matrix.h. Do not restore it over the final native matrix/transform model. |
| `00117f2c` | one-model-per-class | Final Framebuffer has no attachments array. The alternate attachment/resource interpretation lacks the actual producer/owner and extent transaction; the finalized allocator/header wins. |
| `001229d4` | unmodeled-callback-family | Final GraphicsContext intentionally leaves the callback region opaque. Seven guessed GraphicsCallback slots do not establish each actual invoker/provider exact callable type; no erased callback table is published. |
| `00126330` | C-facade-into-native-member | CalendarTime::CalendarTime aliases accepted C001861ec(WordPair*,...) returning WordPair*. The ordinary array runtime also requires its exact void*(void*) provider; no spare native-constructor treatment is allowed. |
| `001266e4` | non-adopting-context | The old exact callers depend on same-TU visibility of an uncredited mismatching0019e81c translator. Removing that non-adopted body makes both callers mismatch; a passing function subset is not complete closure. |
| `0012672c` | non-adopting-context | The old exact callers depend on same-TU visibility of an uncredited mismatching0019e81c translator. Removing that non-adopted body makes both callers mismatch; a passing function subset is not complete closure. |
| `00131464` | byte-offset-owner-recovery | The drain recovers AudioVoiceGroup from PoolLink by offsetof byte subtraction. The agreed TaskPool prefix does not authorize this new container-of access. |
| `001387f4` | C-facade-into-native-member | Final accepted001306c8 still calls a C facade func001387f4(RenderDrawList**,int). The proposed provider is real ResPrimitive::SetupDrawCommand(bool); a genuine ResPrimitive value-wrapper/caller transaction is missing, not a cast or spare alias. |
| `0014381c` | native-slot-provider | The proposed new SoundDataManager constructor installs the native vtable over existing non-adopting slot owners:001d0324 is DirectSoundLoaderState cleanup,001f1864 is PointerTableOwner lookup, and001f1800 is EventDispatcher::dispatch(RoutedEvent*). No complete actual native provider/owner transaction is adopted. Bounded pointer identities are evidence of references, not table-extent proof. |
| `001456e4` | unevidenced-downcast | Fold review traces the real `00181588`/`0013566c` producer to `00183168` and the canonical `data_00242238` ResourceEvent pool. Accepted `00135268` allocates ResourceEvent records. No independent tag/producer proves BoundDisplayObject, while accepted `00181700`/`00181448` require that owner. Main explicitly holds the new body, its setter helper and header. See `FOLDED-DISPLAY-PRODUCER-r1.json`; offset coincidence is not an alias or an adopting provider transaction. |
| `0014b6f8` | native-slot-provider | The proposed current-request field, SignalState completion and execute vtable would replace final AudioTaskManager/AudioWorkRequest members without the full real execute-slot provider family. Callback width alone is not that type proof. |
| `00150124` | steering-or-unproved-value-model | The old AudioMixMatrix adds a zeroing constructor followed by an unconditional all-zero reset, without a supplied adopting matrix/helper family. Do not restore redundant work or a guessed matrix API solely for the byte witness. |
| `0015ea0c` | illegal-downcast | The new caller reinterprets canonical DisplayBinding* as ColoredDisplayBinding* without a tag/producer proof for that concrete object; no C linkage/name addition can close that owner mismatch. |
| `0015f84c` | one-model-per-class | Native W11EntryCommand construction conflicts with the existing W8ItemEntry public owner/constructor interface. Equal offsets are not a class alias or a native provider adoption. |
| `0017433c` | one-model-per-class | The old PlaybackHandle/RuntimePlaybackChannel/TaskRuntime.channels model is absent from the final root. Do not recreate a parallel playback/animation owner or cast its controller. |
| `0017fe04` | non-adopting-callback-provider | MaterialColor::MaterialColor aliases0014b438, whose actual adopted callback is void*(void*) over HwrIndexValue. ctypes1 confirms no native MaterialColor/MaterialInit transaction; no alias to the C record is admitted. |
| `00183d3c` | non-adopting-provider | The native loader targets member aliases for accepted C ArchiveHandle/ArchiveReader/WaveArchive providers, and its complete body mismatches under final headers. No C/native receiver or result facade is restored. |
| `0018d51c` | C-facade-into-native-member | The bank loader declares native00183d3c as a separate C ArchiveLoader operation and constructs a competing BankReader/BankFile view. The exact native loader/archive/allocator family is not adopted. |
| `0019000c` | native-slot-provider | TransferStream invents a virtual owner with reserved void slots and Result-valued seek/read, without the actual slot-provider/return-owner closure. Do not alias it to the different byte-count-returning FileStream. |
| `00195900` | illegal-downcast | The animation loader reinterprets DisplayObject* as unrelated W16AnimatedDisplay* with no actual producer/tag containment proof. No second native/slot owner is published. |
| `001ada1c` | incomplete-type-family | The guarded W22ResourceGroup assumes unadopted native001ae908/001ae9e4 constructor/destructor owners and a one-word mask base; the complete, exactly witnessed family is not supplied. |
| `001b50bc` | non-adopting-provider | The native W22LargeResourceGroup destructor alias targets accepted C001b63ec(W24ExtendedWidgetScreen*) returning that separate owner. No native/group identity is established by geometry. |
| `001b5a34` | illegal-downcast | Final WidgetSlot.object is ResourceEvent*. The old source treats it as DisplayObject* and then BoundDisplayObject* based on a tag without proving the actual containing owner. Preserve final ResourceEvent/base semantics and the color-family boundary. |
| `001b5cc0` | illegal-downcast | Final WidgetSlot.object is ResourceEvent*. The old source treats it as DisplayObject* and then BoundDisplayObject* based on a tag without proving the actual containing owner. Preserve final ResourceEvent/base semantics and the color-family boundary. |
| `001c1c74` | incomplete-type-family | The guarded SingletonHolder/Storage family requires an external native cleanup owner001c1f34 not adopted in this delta; names-admission alone does not supply the complete owner/provider transaction. |
| `001c3068` | incomplete-type-family | SoundSystemState depends on unadopted SoundManagerState/SoundActorState constructor/vtable models. The private complete class family and its nominal field/callback contracts are not a final-main owner transaction. |
| `001c5294` | callback-erasure | Do not move/re-adopt the existing KernelHandleOwner*(KernelHandleOwner*) provider through a runtime void(void*) interface. It remains accepted unchanged at its final owner; ctypes1 explicitly holds the honest generic repair. |
| `001c9e08` | unproved-constructor | The local inline empty nn::math::MTX44 default constructor fills a missing canonical SDK definition to support the identity holder. No independent canonical constructor adoption is supplied; do not turn an inferred no-op into a project-wide class implementation. |
| `001cbf34` | non-adopting-provider | The native AnimResource calls a native IsValidBinaryFile signature through an alias to C001813a0, whose actual BinaryFileHeader/return/argument types are separate. No header-layout alias or copied header value is introduced. |
| `001cc4cc` | one-model-per-class | The cleanup uses private TextureCacheNode instead of the genuine native TextureRefLink owner, and the full TU has an unmatched RegistTexture body plus a native LinkList member alias into C00191154. No fake node/list class or mismatching context is adopted. |
| `001ce9e8` | non-adopting-provider | Native Layout::Build reaches Pane::AppendChild through a member alias to accepted C provider001822fc, and casts the separate binary-header models. The actual native Pane/provider and canonical-header transaction is not adopted on5820254f. |
| `001cf448` | protected-native-owner-transaction | The ResourceEntry/ResourceTable.entries/native TextBox publication, wrapper and cleanup0011f080 transaction remains held (TypesCompose F02). A narrow private-header/member rewrite or table-cluster proximity is not owner proof. Keep final private TextBox/MakeUniform slot and accepted001cf418 unchanged. |
| `001d3f18` | unauthorized-vtable | The private BufferedFileStream model lacks the required explicit virtual-destructor/key-function owner transaction and uses a new bound vtable over non-adopting stream providers. Keep final native stream declarations. |
| `001d3f58` | incomplete-native-slot-family | The Read body itself MATCHes as native MemoryFileStream::Read, and the native constructor/Close/Seek already exist. [INFERENCE] The full family is still not one exact owner/interface transaction: accepted capability providers remain free functions over the separate MemoryInputStream record. No receiver alias or partial cleanup/capability cutover is claimed; hold the new fragment pending that canonical transaction. |
| `001d4010` | unmodeled-callback-family | Final c010-r8 explicitly deferred001d4078: the result/context callback family is unmodeled. It is not an accepted move. Old ArchiveTransfer/context mutex/result declarations would reverse that closure; both dependent new claims stay held. |
| `001d4078` | unmodeled-callback-family | Final c010-r8 explicitly deferred001d4078: the result/context callback family is unmodeled. It is not an accepted move. Old ArchiveTransfer/context mutex/result declarations would reverse that closure; both dependent new claims stay held. |
| `001dac54` | one-model-per-class | Its forwarding argument is DrawCommandState*, while final accepted001dac84 is MemoryWriteStream*. Keep the finalized provider; do not retag it or add a pointer/byte-storage bridge. |
| `001e37ec` | native-slot-provider | The opaque AllocatorInterface and unused instance do not establish the actual virtual allocator slot owner/invoking signature. No new C/native allocator facade or guessed slot provider is published. |
| `001ea1cc` | one-model-per-class | The old W11jNumberFormat.flags and color receiver/callee contracts conflict with the final number-format/color models. No reserved-word refinement or narrowed shadow color declaration is used to compile it. |
| `001ebda4` | non-adopting-callback-provider | The proposed native virtual destructor maps to final C001ebe1c(void). No native ServiceSingleton owner/key-function/actual cleanup provider is adopted. |
| `001f1cf0` | one-model-per-class | The final SoundWaveReader info is not the proposed native WaveFile::InfoBlockBody. Do not complete the opaque canonical reader with a competing native interpretation; the output/reader family needs one real owner transaction. |
| `001f563c` | non-adopting-callback-provider | StaticStreamMutex globals would replace final StreamMutex objects and alias a native destructor to accepted C001c519c(void). This is not the exact __aeabi_atexit void(void*) provider/owner transaction. |
| `001f57d4` | non-adopting-callback-provider | StaticStreamMutex globals would replace final StreamMutex objects and alias a native destructor to accepted C001c519c(void). This is not the exact __aeabi_atexit void(void*) provider/owner transaction. |
| `002108b0` | non-adopting-callback-provider | The native StartupCallbackState destructor alias targets final C001db960(void*). The descriptor contains a code pointer; do not disguise a still-unproved native/table owner as a C record initializer or spare destructor alias. |
| `00212650` | non-adopting-callback-provider | The proposed native KernelHandleOwner destructor aliases accepted C001c5294. Its real generic callback repair is held by ctypes1 after a size mismatch; no native initializer or altered scalar/global owner is adopted. |
| `00212ae8` | non-adopting-callback-provider | StaticStreamMutex globals would replace final StreamMutex objects and alias a native destructor to accepted C001c519c(void). This is not the exact __aeabi_atexit void(void*) provider/owner transaction. |
| `00212b20` | non-adopting-callback-provider | StaticAllocation::~StaticAllocation aliases C001d3370(Allocation*) returning Allocation*. A first-member offset match does not adopt the actual native destructor and invoking callback contract. |

The previous `0010c138` and `001b4310` steering holds and `00111114` decoder-arity hold
remain explicit in `DISPOSITIONS-r2.json`. No volatile, register forcing, barrier,
byte-storage substitute, function-pointer cast or fabricated callee was used to
recover their private matches.

## Folded cmodels2-r5 disposition

The supplied `5a75bf13` history is folded only after independent c012 commit
`8c94c419cf2911c8e1616d20ed0435d6565aedfb`. Its four proposed NEW identities are
individually adjudicated: `00169018` (144 B), `00171208` (96 B) and `001e5d38`
(116 B) remain qualified source candidates; `001456e4` (240 B) and its newly
introduced setter/header are held by the producer finding above. The component's
four revised accepted functions (280 B) and two revised accepted helpers remain;
together with the TaskPool cutover this is six revised accepted functions (416 B).
These are revisions, not NEW or move credit.

The imported deletion of `src/checkpoint10/alternate1/func_001650f8.c` had no
manifest move. Its exact finalized-main source is restored, preserving all six
accepted identities (`001650f8`, `001f1ebc`, `001f1ed8`, `001f1f20`, `001f1f34`,
`001f1f4c`). `FOLDED-ACCEPTED-SOURCE-RESTORATION-r1.json` binds that decision.

There is one active candidate, `config/candidates/c012.json`, including every
tracked NEW hold. The superseded component manifest and original issue document
are explicitly rehomed under `scratch/integrator/c012/r4/folded-inputs/`; its
original evidence stays at `scratch/integrator/cmodels2-r5/`. Its proposed counts
and 86-command bounded proof are historical component facts, not cumulative counts
or a replacement for the combined proof. **CM2-F01 and CM2-F03 remain blocking,
outside the credited delta; they are not repaired.** All ten component holds,
including CM2-F02 `001e958c`, stay uncredited. The existing c011 ownership note for
`001d5694` remains, and the four c009-owned deferrals are not acquired.

The canonical counted-entry and completion receiver/result models are reused;
there is no new callback erasure, PMF decoding, native alias, vtable-name binding,
U1 selection or byte-storage bridge. The explicit text-resource table retains
CM2-F04's typed live cell and **[INFERENCE]** extent qualification; it does not
prove a native class identity. Final c010-r8 runtime callback closure remains
authoritative. The three retained NEW bodies are not provider rows in that closure.


## Integrator boundary

Do not treat this candidate or the full bounded source comparison as checkpoint
readiness. Main must first integrate finalized c011/ctypes1/c014 state, preserve all
current callback/opaque-owner rulings, refresh source dependencies, run serialized
whole-ledger interfaces and caller audit, adjudicate every result, then perform the
ordinary clean prepare with two fresh full builds and suite, two independent reviews
and finalize. No heavy gate or push was run by this worker.

## Preserved cmodels2-r5 qualifications

The following component findings are carried forward verbatim. Its native-family
investigation and the component-only scope/proof are preserved in the explicitly
rehomed original document and `scratch/integrator/cmodels2-r5/`; they are not
claims of repairs or combined checkpoint readiness.

- **CM2-F01 remains blocking:** the real scene lifecycle providers and their native/
  explicit table interfaces require one coherent exact migration. Keeping accepted
  forms is not a claim that the previously identified slot contracts are repaired.
- **CM2-F03 remains blocking:** the three accepted scene/member-callback relationships
  need the genuine shared native hierarchy and providers. The PMF is actually invoked;
  receiver erasure, unrelated record aliases and hand-decoding are not repairs.
- **CM2-F02:** only NEW `001e958c` (296 B), its geometry helper and its private owner
  header stay quarantined. The accepted c010 cleanup/provider forms win composition.
- **CM2-F04:** retain the R4 typed two-cell text-resource address-point table and its
  explicit extent qualification. Its live cell is `void(W22TextResources *)`; no arity
  is invented for the null/VFE cell, no native class identity or data credit is claimed.
- **10 tracked NEW holds / 4,760 B** remain: `0017a49c`, `001ab680`, `001acc88`,
  `001acf28`, `001bc5ac`, `001d5694`, `001d5864`, `001db404`, `001ef89c`, `001e958c`.
  Historical failures, v3-only qualifications and c011/audio ownership remain intact.
  The nine unchanged accepted destructor forms are not counted as NEW holds.

No exact native root/derived transaction was recovered under the authorized
virtual-destructor/actually-undefined-key/external-`_ZTV` pattern. The component's
changed inline-root arrangement left no surviving catalogued root D1; eight
derived D1 comparisons were four bytes short under v18/L1. U1 did not waive D1
equality. All nine accepted destructor forms remain unchanged; no synthetic slot,
pure-qualifier change, native-to-C alias, reference suppression or PMF decoding
is adopted. The old v18 refusal and changed-source failures are preserved rather
than rerun. `NATIVE-FAMILY-DECISION.json` binds their exact owners and evidence.

The component's model inventory reopens all 28 accepted R1 callers and its one L1
owning function if a linker-model/header/helper change affects them. Its original
four NEW R1 callers are reduced to the three retained here; the display initializer
and setter receive no helper, data or unwind credit. R1 helper identity, outlining,
original existence and historical TU/call graph remain **[INFERENCE]/QUALIFIED**.
L1 retains its pool-extent, sharing, ordering and historical-TU qualifications.
There is one global v6/L1 selection; no component proof label can override it.


## r5 exercised proof and policy triage

The complete source proof passed **2,695 sequential niced contexts**, every matcher
exit **0**, with **2,799/2,799 required identities MATCH** and all **25 helper-only
units** covered. Every emitted comparison MATCHes and canonical owners remain
distinct. The actual driver exit is **0**. `REPROOF-r1.json` and
`V14-PROOF-CARRY-r1.json` bind the protected carry to adopted v14; no second full
proof was run and no source/header changed afterward.

Main's requested lint exited **0**: **23 source leads** (eight C pointer casts,
three memcpy calls, six static pointer casts, two opaque-union mentions and four
shared-ring volatile qualifiers), **six proximity leads**, **zero second-model
hits**. Every lead has an individual evidence-backed disposition in
`POLICY-LINT-DISPOSITIONS-r1.json`; none requires a source/header change. Lint is
triage, not an independent G6 certificate.

The additional reviewed serialized-font boundary `0019d8c4` retains all four
format conditions: **(a)** headerSize and block.size come from serialized records,
and TGLP is the payload tag, not an arbitrary runtime-object displacement;
**(b)** the canonical FontBlock.data payload is converted once to documented
FontGlyphInfo and then accessed through that type, without a runtime font-class
overlay; **(c)** canonical C font.h supplies these format records and its validator,
not a block-address resolver, so the existing typed boundary is retained; **(d)**
**[INFERENCE]** original valid-file/block bounds and alignment remain trusted.
No new byte walker, second format model, validation fallback or data credit is added.

