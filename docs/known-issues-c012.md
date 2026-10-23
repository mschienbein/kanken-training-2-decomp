# c012 known issues and qualifications — r4

## Reconstructed source scope, not acceptance

This candidate starts at finalized main `5820254f` (c010-r8, history sequence 11),
not the old unfinalized cmodels2-r3 lineage. Gates are v1.20; the explicit selection
is **`armlink5-inline-branchnop-v6` / `anonymous-string-records-v1`**. No U1 or
per-unit model override is admitted.

- **26 NEW functions / 4,500 B**, plus **30 explicit moves / 908 B**: **56 claims /
  5,408 B** in **41 candidate units**, including one zero-credit helper-only unit.
- All **2,682 accepted identities** survive. The unaccepted proposed ledger has
  **2,708 functions / 293,080 B**, with all **25 helper-only units** retained.
- **49 tracked NEW holds / 8,588 B** in this
  re-derivation scope; the proposed accepted `001c5294` move is separately withheld
  at its unchanged final owner. No accepted identity is deferred or removed.
- The three previous holds `0010c138`, `001b4310`, `00111114` remain uncredited:
  steering-dependent spellings and the narrowed decoder interface were not restored.
- The older 149-row census/author deferral snapshot is preserved as historical
  evidence in `HISTORICAL-DEFERRALS-r3.json`, not silently relabeled as a fresh
  current audit or counted again in this 103-identity re-derivation scope.

The complete current-ledger source reproof ran **2,618 serial niced matcher commands,
all exit 0**: **2,708/2,708 required identities MATCH**, including the natural contexts
for all **25 helper-only units**. Every emitted comparison in every selected command
is MATCH; the old failing co-emitted function subsets are not called whole-TU passes.
Evidence: `scratch/integrator/c012/r4/match-first-r1/results.json` and
`REPROOF-FIRST-r1.json`. These are bounded source comparisons, not two fresh G4
full-image/BLZ receipts, an emitted-owner audit, G3/argcheck, independent reviews,
preparation, finalization or new accepted progress.

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
- The parameter scenes use genuine constructed `ParameterValue`-derived values
  and the existing native `MemberCallback`/`CallbackTarget` interface, never C PMF
  decoding. Complete scene hierarchy/provider obligations remain an integrator and
  independent-review prerequisite; byte equality is not that proof.
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

## Integrator boundary

Do not treat this candidate or the full bounded source comparison as checkpoint
readiness. Main must first integrate finalized c011/ctypes1/c014 state, preserve all
current callback/opaque-owner rulings, refresh source dependencies, run serialized
whole-ledger interfaces and caller audit, adjudicate every result, then perform the
ordinary clean prepare with two fresh full builds and suite, two independent reviews
and finalize. No heavy gate or push was run by this worker.
