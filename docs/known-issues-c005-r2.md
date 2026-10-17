# Known issues — checkpoint c005-r2

**190 new candidate functions / 11,668 B in 182 units**:
wave 5 = 37 functions / 1,852 B in 31 units; wave 6 = 73 / 5,540 B in 73 units;
wave 7 = 80 / 4,276 B in 78 units. All wave-6/7 authors are collected.
All authors are OpenAI-family (`openai-codex/gpt-6.1-sol`);
`author` provenance is retained per unit in `config/candidates/c005.json`.
The separate wave-5 candidate manifest is replaced by that combined manifest.
Denominator v7; gates v1.11; linker model `armlink5-inline-branchnop-v2`
([`LINKER.md`](LINKER.md)). This describes checkpoint content, not acceptance.

Review is single-family under
[`rulings/2026-10-17-reviewer-model-family.md`](rulings/2026-10-17-reviewer-model-family.md).
Two separate fresh-context reviewers must re-derive and seal their records. The
integration pre-review is not an independent acceptance review.

## Checkpoint history

- **c005** was prepared PASS on WSL Linux-x86_64 under denominator v7 and
  gates v1.10. Its two build receipts and suite are historical evidence for that
  source state, not for this revision. It was **never finalized**.
- `review-c005-second` **FAIL**: F01 (`func_00134f04` omitted its caller-supplied
  integer); F02 (`func_0010b5f0` / `func_0010b610` omitted callback context words).
- `review-c005-openai` **FAIL**: F01 is the same notification-setter input;
  F02 alleges an additional core input to `func_00181700`; r2 rejects that
  inference with the all-caller counter-evidence below. F03 is the same
  callback-context finding. Its formerly pending review result is resolved.
- **c005-r2** repairs the accepted findings, re-examines F02, sweeps the whole
  class and receives shared-interface evidence from c006 integration. Main's `fe19c9a`
  merge supplies ArgLint/gates v1.11; the frozen `game` compiler profile is
  unchanged. Fresh c005-r2 preparation, independent reviews and finalization are
  the integrator's work. `known-issues-c005.md` remains an immutable record.

## R2 contract revisions and whole-class sweep

The sweep records **351 catalogue-addressed contracts: 21 changed, 330 retained**,
including all **234 definitions** in the final 216-unit review scope and their
**117 callee/header contracts**. It covers all eight empty/return-only definitions,
41 tail-forwarding definitions, 13 C++ definitions, every direct caller from a
cached implementation of `packet._calls`, all 111 static function-pointer
locations, and concrete registrations/dispatches. All r0-r3 and s0-s15/d0-d7
setup and r0/s0 result paths are considered. Twelve callable-field/registration
interfaces and three standard runtime declarations are also retained with notes.
Per-contract before/after decisions, all 3,572 surviving direct branch/call sites,
registration roots and evidence are in `scratch/integrator/c005-r2-NOTES.md` in
Main's checkout and the revision worktree's ignored `scratch/integrator/c005-r2/`.

- `func_00134f04` keeps an unused unsigned core word: `func_00135450` explicitly
  supplies 1. **[INFERENCE]** Original integer spelling/name and semantic role.
- Both installed service callbacks keep an opaque `void *context` word.
  `func_00106a50` installs them through `func_0010cf84` / `func_0010ce58` with null
  contexts; `func_00106e04` clears those registrations. `func_00122b30` loads each
  corresponding stored word immediately before invocation. In the predicate
  case the review described this as a stored state word; registration still
  establishes the same one-word context slot. **[INFERENCE]** Pointee/semantic
  type is unrecovered; null and an unused word do not prove an original type.
- **Rejected F02 — `func_00181700` retains object plus three floats, without r1.**
  All four relevant caller functions are re-examined: `func_00141cd8`,
  `func_001456e8`, `func_001793e4` and `func_001e403c`. Of their six physical calls,
  four have only r0/s0-s2 preparation; even `func_00141cd8` contains two such calls
  besides the flagged r1-zero site. Recorded c006 callers `001793e4` and `001e403c`
  MATCH without r1 preparation (exit 0 each). Adding an explicit unsigned zero
  produces 0x108/0x104 and 0x2c4/0x2c0 size mismatches respectively (exit 1 each).
  The baseline receipts are in c006's `scratch/integrator/c006/matches-handoff.json`;
  exact probe commands/results are in
  `scratch/worktrees/c006/scratch/integrator/c006/geometry-core-word-probes.json`.
  These reported checks were read, not rerun. The two isolated r1-zero writes
  cannot establish a universal C parameter; incidental register reuse remains
  **[INFERENCE]**. The canonical declaration in `screen.h` has only the object
  core word and three floating words. No dummy caller argument is fabricated.
- ArgLint exposes four additional inherited accepted contracts. `func_001149a8`
  returns its typed object pointer, used for address arithmetic after both calls
  from `func_0010cc0c`. Empty `func_0014f7e8` keeps two explicitly supplied core
  words. Empty `func_00164ebc` keeps the repeatedly supplied object, generic
  destroy callback and image-base function pointer. Empty `func_00174480` keeps
  the writable object supplied by its cleanup callers. **[INFERENCE]** Original
  initializer/destructor/registration identities and unrecovered word types.
- The formerly separate Registry/RegistryNode and PoolList/PoolLink APIs use one
  canonical `PoolList` / `PoolLink` representation from `pool_list.h`, including
  `func_00191154`'s three-pointer insertion API. Domain-specific RegistryEntry and
  RegistryOwner names remain. All callers/fields migrate; no aliases or
  provisional by-value iterator interface are retained.
- `func_00140ccc` / `func_00140d1c` use `const SoundRecord *` and packed options at
  `&record->option_flags` (+0x14), retaining their unsigned-char and int results.
  SoundReference/Table replace the obsolete duplicate serialized reference
  views; BankReader uses the same reference while retaining its typed lookup
  result. SoundArchivePlayerView.archive, `func_00131928` and `func_001f2ac8`
  point at the actual SoundArchiveReader (+0x38/+0x3c accesses), not the distinct
  runtime owner whose reader member is +4.
- Notification payload storage is one union with word, UTF-16 and field views;
  `func_00179148` reads `payload.storage.fields.flags` at the same saved byte.
  The independent saved transform rows are exposed at +0xb830.
- StreamQueue is the evidenced 0x34-byte state prefix. StreamManager owns explicit
  32- and 33-event backing vectors; its 0x190-byte layout and queue/pending/ring
  offsets remain unchanged. Accepted initializer `func_00146ff8` now uses this
  shared view rather than a competing MemoryArena. The recursion depth is unsigned.
- MetadataEntry exposes the independently used uintptr_t resource and int index
  at +4/+8. W6ListGroup exposes its later mode/selection words. The provisional
  fixed 21-entry W6ListController vector is **not adopted**: its +0x70 word is not
  yet adjudicated. The established flexible-vector prefix is retained rather than
  asserting an unevidenced bound.
- ResourceServiceState shares the callback pending byte at +2 while keeping
  initialized +8 and busy +0xc distinct, followed by eight opaque lock bytes
  at +0x10 and the handle at +0x18. The callback sweep still accesses only pending.
  GraphicsMutex exposes its independently used handle at +0; `func_0012fab0`
  keeps the established unsigned result and const GraphicsMutex pointer,
  declared once in `graphics_state.h` rather than locally in `func_0018f040`.
- **[INFERENCE]** Empty `func_00137128` retains the potential InputController
  pointer forwarded from `func_00185044` by c006's matched `func_00137118`.
  Its adjacent branchnop site cannot independently recover the erased source
  callee identity/prototype. The shared `input.h` declaration preserves that
  possible object input rather than asserting a no-argument contract.

Final ArgLint on the dry 489-function ledger exits **2**, as documented for its
explicitly unchecked C++ inputs (25 source units, 22 functions without a parsed
C contract). Its **20 raw candidates in 15 functions are all adjudicated**:
two geometry declarations are rejected using the all-caller counter-evidence;
the remaining candidates are armlink self-copy NOPs, epilogue stack-padding
restores, condition-code report temporaries, or mutually exclusive conditional
result paths. All 69 lack-of-local-input notes are reviewed without weakening
forwarded/evidenced inputs. The raw report is retained; no suppression or fake
zero-error claim is substituted for this disposition. Dynamic indirect use and
original source spelling remain **[INFERENCE]** where no concrete invocation is
recoverable.

## Shared interfaces and layout qualifications

The integration uses one shared definition for the input/calibration/sample,
selection, GPU/matrix/radar, serialized archive, sound runtime/voice/channel,
display binding/pane/handle, notification and intrusive-list views. Declarations
come from C/C++-safe `include/game/` headers rather than local C-linkage copies.
Semantic subsystem/type names and unobserved object portions remain partial
reconstructions, not recovered original source names.

- `SelectionState` contains signed halfwords at +0x52/+0x54. The existing enum
  mappings receive the screen pointer explicitly supplied by callers, even though
  their bodies ignore it. No unused argument was invented solely for codegen.
- The service's active byte at +1 is signed, as read by `func_00192c10`; existing
  setters still write only 0/1. The distinct pending-state block has an unsigned
  pending byte at +1 and signed blocking byte at +2.
- `GpuState` retains clear color +0x660 and control +0x770 while sharing the
  evidenced attribute stride/enable/dirty fields. The Matrix34/44 views use one
  layout. **[INFERENCE]** Copy helpers `func_001944f0` / `func_00195334` retain the
  established destination-pointer return contract: they preserve destination r0,
  but inspected callers ignore the result. The source does not fabricate a use
  of that result to imply stronger evidence.
- Resource-index and sound archive metadata are distinct formats, not aliases
  merely because both readers have a metadata pointer at +0x3c. The C definition
  and C++ caller of `func_0014c23c` share a count plus packed-parameter-tail view.
- `SoundArchivePlayerView` has a mutable provider behind its read-only parent.
  The provider's enabled byte and pointer-returning lookup slot +0x0c are typed;
  unobserved virtual slots are padding, not invented method signatures.
- Sound instance views share the handle fields, signed priority adjustment,
  unsigned priority byte, volume and intrusive links. The formerly separate
  player/group views share the same requested-count, pending-list and active
  fields. Explicit casts preserve the observed signed comparisons where the
  common list count is unsigned.
- Audio channel views agree on the state pointer at +0x68, flag halfwords
  +0x1e/+0x6c and ADPCM coefficients +0x7c. Voice-group views also agree on the
  callback/context, byte flags, dirty field and list link. The callback receives
  the mutable group, event number and context; its result is ignored. A competing
  unused float/int field view is reserved storage rather than an invented type.
- `DisplayObject` remains a 0x94-byte prefix, with type bytes at +0x92; bindings
  and queue fields are an explicit extension. Menu text uses a 0x2c-byte prefix
  and the following counted pointer vector, avoiding an invented fixed bound.
  The saved notification payload remains 0xb740 bytes, including the flag at
  payload +0x32 = global +0x3a.
- **[INFERENCE]** `func_001ad6ec` takes a two-float position value object in s0/s1.
  A two-scalar-float interface is ABI-equivalent at this leaf; the shared source
  retains the evidenced caller-compatible value-object interpretation.
- **[INFERENCE]** `func_0018d868` returns the successor iterator retained in r0.
  All inspected callers ignore it. Its registry/node arguments are mutable;
  caller declarations use this one contract rather than conflicting void returns.
- **[INFERENCE]** `func_00169920` forwards its third integer argument as a fourth
  target argument, although the target ignores it. Callers supply an integer
  sentinel; the wrapper is not changed to a weaker signature.
- Empty `func_001467e4` has no direct callers or image pointer references; its
  `void(void)` fallback remains **[INFERENCE]**. Empty `func_00137128` likewise has
  no surviving direct call/pointer, but retains the explicitly qualified
  potential object input described above. `func_001c0e44` is an evidenced writable
  array-element callback; only its unused void return interpretation is [INFERENCE].
- The allocation callback used by `func_0016a830` is at vtable +8 and takes the
  allocator plus byte size. Earlier slots are padding. RegistryOwner likewise
  uses an opaque vtable rather than inventing an unused destructor contract.
  Resource cleanup's actually invoked deleting destructor remains natural C++
  `delete`, with a typed ResourceHandle interface.
- ARMCC C++ rejects C flexible-array members. Shared headers retain the same
  fixed-prefix size and alignment, exposing zero-size flexible tails only to C
  clients; current C++ clients access only the shared prefix. Word padding keeps
  four-byte prefix alignment where the flexible pointer tail supplied it in C.
  No one-element substitute arrays or competing struct definitions are used.
- Stream queues share the 0xb4-byte layout, and both 0x190-byte StreamManager
  instances share their bool full flag and event/ring fields. Bitmap allocation
  and release share the allocator-evidenced signed capacity/used fields; element
  offsets and division by stride remain unsigned.
- **[INFERENCE / ABI qualification]** Destructor registration accepts a generic
  object pointer and a one-pointer `ObjectDestructor`, not a no-argument callback.
  StreamManager and ObjectManager destructors keep their concrete prototypes;
  explicit function-pointer conversions at registration rely on the uniform
  single-word AAPCS object-pointer ABI, not ISO C function-type equivalence.
  The empty ObjectManager destructor's argument follows that registered use.
- **[INFERENCE / address-view qualification]** `func_00185bfc` tests the second
  guard as `data_0024371c.second`, but guard operations reference the direct
  symbol `data_00243720` at that same address. Both volatile-word declarations
  are shared once, with their overlap documented; no second allocation is
  defined. Collapsing the two C address expressions shortens the function from
  92 to 88 bytes. The original split addressing is retained explicitly.

## Compiler-extension and outlining qualifications

The ruling
[`rulings/2026-10-17-armcc-extensions.md`](rulings/2026-10-17-armcc-extensions.md)
applies. `func_001d007c` and `func_001d4410` use ARMCC's `__usat` intrinsic to
saturate the combined priority to seven bits. Each needs that qualification on
its eventual function review row. No new SVC or CP15 named-register declaration
is introduced by these waves.

The shared metadata search `func_00168670` is explicitly `__declspec(noinline)`;
the three-function source marks the outlined-helper interpretation [INFERENCE].
Other small source helpers retain ordinary inline/force-inline annotations, not
injected instructions or dead code.

Natural `memcpy` / `memset` source is kept. Both `__aeabi_memcpy` and the
word-aligned `__aeabi_memcpy4` lowering are independently bound to the same
original entry, so the WordRingWriter retains its real word-pointer types and
ordinary memcpy without a local binding or explicit runtime-helper substitute.
Compiler/library helper bodies remain uncredited.

## Linker-model qualifications

- None of the 37 wave-5 additions requires a rewrite.
- New candidates contain **35 rewrite sites in 32 functions**: 29 `branchnop`,
  two `inline_insn` and four `inline_nop` sites. The branch-to-next targets are
  explicitly typed adjacent catalogued functions.
- `func_00160298` has two `inline_insn` getter calls. Its source marks the choice
  of `func_001c6af4` **[INFERENCE]**: the eligible body reads `ItemPage.count`, but
  inlining does not recover the original callee identity.
- `func_00185bfc` / `func_001916e8` each use two `inline_nop` sites for destructor
  registration and guard release. Both callee identities remain marked
  **[INFERENCE]**, not recovered original call-graph facts.
- Revised sleep wrapper `func_0011ef54` retains its adjacent `func_0011ef58`
  branchnop and signed 64-bit r0/r1 forwarding inference.
- Five revised functions contain six inherited/newly rechecked rewrite sites:
  `func_0011ef54`, `func_0019dc1c`, `func_0017e7c4`, `func_00185cc0` and
  `func_00185d1c`. Their adjacent-target or inlined-callee qualifications remain.
- The model qualifies inherited material too: inlined calls prove eligible
  callee bodies, not identities; branch-to-next proves the final adjacent target,
  not whether source order or linker reordering placed it there. Conditional
  single-instruction inlining remains manual-backed. See the eventual receipt's
  `linker_rewrites`, not a claim of recovered original call graphs.

## Revisions to accepted material

The four carried wave-5 revisions remain: w1 `func_00134f04` uses the notification
status halfword, `func_0018fcb4` returns the separate service context, and
`func_00195e84` / `func_00195e94` use the shared signed flag / unsigned mode fields.

At original c005 preparation, relative to finalized c004-r3, **29 accepted units /
33 functions / 1,824 B** were revised, limited to interfaces and common views:

- `src/pilot/game_enum_switch.c`: both enum mappings' evidenced screen argument.
- `src/pilot/loop_cleanup.c`: typed ResourceHandle pointers/shared declarations.
- `src/pilot/loop_resources.c`: shared allocation-record layout/deallocator.
- `src/pilot/pilot_b.c`: shared deallocator declaration.
- `src/wave1/func_00105058.c`: common allocation state/deallocator declaration.
- `src/wave1/func_00135a28.c`, `func_00135a58.c`, `func_00135a88.c`,
  `func_00135ab8.c`: common buffer state/deallocator declarations.
- `src/wave1/func_001360a4.c`: shared deallocator declaration.
- `src/wave1/func_0019e02c_returning.c`: shared pointer-preserving wrappers/release.
- `src/wave2/func_001795bc.c`: shared expanded SelectionState.
- `src/wave2/func_001791e0.c`: common text prefix/following pointer vector.
- `src/wave2/func_00179298.cpp`: shared display-handle/context/activation interfaces.
- `src/wave2/func_00147260.c`: evidenced registered ObjectManager destructor argument.

After Main finalized c004-r3 and released these edits, the six deferred c004
shared-header migrations were applied to `func_001ef888`, `func_0019dc1c`,
`func_0018d514`, `func_0017e7bc`, `func_0017e7c4` and `func_00185cc0`. The
registration contract also revises `func_00216140` and `func_00185d1c`; the carried
sleep and WordPair revisions remain. Every affected caller is migrated and all
these functions MATCH. No c004 revision remains pending.

## Preserved original bugs

Every listed function carries an `ORIGINAL BUG:` comment; no substitute result,
validation, null guard or exception suppression was added.

- W5 `func_001533b8`: unsupported relative-reference types yield null, but 0x4c
  bytes are still copied from that source.
- W5 `func_001f28cc`: failed relative resolution is followed by source field reads.
- W6 `func_00112c98`: increments the generation and copies an uninitialized
  156-byte local list into global state.
- W6 `func_001678dc` / `func_001e5978`: the scale-setting path can still forward a
  null widget to the renderer. The latter's ordinary caller first guards null.
- W6 `func_001705a8`: a missing chart is still dereferenced by the color loop.
- W6 `func_001e59d4`: cleanup dereferences a missing chart instead of skipping it.
- W7 `func_0014b58c`: a negative configured limit is retained rather than clamped
  to zero, affecting the pruning loop.
- W7 `func_00152d48`: scanning the rounded-up bitmap capacity can expose a padding
  bit beyond the logical pool capacity.
- W7 `func_0015fae4`: page/item/text lookup failures are used without null checks.
- W7 `func_001815a8`: the lookup result is dereferenced before the queued test.

Inherited bugs, including the enum mappings' uninitialized out-of-range result,
remain documented at their source and in earlier checkpoint records.

## Evidence and uncredited scope

Final r2 sequential evidence covers **216 units / 234 functions / 13,764 B**:
every c005 unit and all **34 revised accepted units / 44 functions / 2,096 B**
report MATCH under `game` (216 commands, all exit 0). The five additional accepted
units are `pilot_a.c`, `func_0014f7e8.c`, `func_00164ebc.c`, `func_00174480.c` and
`func_00146ff8.c`. Eleven headers change; all fourteen original-or-affected header
interfaces are swept. Earlier per-unit evidence is retained separately; only the
`verified-*` manifests/logs bind the delivered state.
The historical c005 integration pre-review covered its 223 functions, including source
policy, exact layouts/use, preserved bugs, callee identity and ABI qualifications.
All twelve c005 C++ units and the revised accepted C++ activation caller were
checked by hand; this does not replace fresh independent checkpoint reviews.

The dry ledger is the set union of canonical matches, c004 and c005 candidates.
Since c004-r3 is now accepted, its 181 units already occur in canonical matches
and are included once, after checking that their profile/function sets agree.
Final `interfaces.py check --allow-unchecked-cpp` exits 0: **456 units / 489
functions / 19,288 B**, 431 checked C units, 25 permitted C++ units, **zero
disagreements, zero policy items and no unchecked C**. The twelve other C++ units
are inherited, unrevised material covered by the accepted checkpoint review chain.

Only RESULT rows re-derived as MATCH are collected. Other mismatches, extent
reports and library candidates remain outside this manifest. No runtime helper
body is credited, no catalogue boundary is padded, and no candidate is silently
dropped to make a gate pass.

Full-image equality, two fresh builds, the full suite, sealed independent
reviews and finalize are required anew for c005-r2 acceptance. This worker ran no
full build/suite or prepare/finalize, authored no canonical-ledger change and
published nothing. The final composed interface check exits 0 with zero errors,
zero policy items and no unchecked C; its 25 C++ units are explicitly reported.
