# Known issues — checkpoint c005

Current collected content: **182 new candidate functions / 11,056 B in 174 units**:
wave 5 = 37 functions / 1,852 B in 31 units; wave 6 = 73 / 5,540 B in 73 units;
wave 7 = 72 / 3,664 B in 70 units. W7b is still authoring and is not included in
these numbers. All authors are OpenAI-family (`openai-codex/gpt-6.1-sol`);
`author` provenance is retained per unit in `config/candidates/c005.json`.
The separate wave-5 candidate manifest is replaced by that combined manifest.
Denominator v7; gates v1.10; linker model `armlink5-inline-branchnop-v2`
([`LINKER.md`](LINKER.md)). This describes checkpoint content, not acceptance.

Review is single-family under
[`rulings/2026-10-17-reviewer-model-family.md`](rulings/2026-10-17-reviewer-model-family.md).
Two separate fresh-context reviewers must re-derive and seal their records. The
integration pre-review is not an independent acceptance review.

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
- The return-only `func_00137128` / `func_001467e4` have no direct callers or image
  pointer references. Their `void(void)` signatures are explicitly [INFERENCE].
  In contrast, `func_001c0e44` is an evidenced writable array-element callback;
  only its unused void return interpretation remains [INFERENCE].
- The allocation callback used by `func_0016a830` is at vtable +8 and takes the
  allocator plus byte size. Earlier slots are padding. RegistryOwner likewise
  uses an opaque vtable rather than inventing an unused destructor contract.
  Resource cleanup's actually invoked deleting destructor remains natural C++
  `delete`, with a typed ResourceHandle interface.
- ARMCC C++ rejects C flexible-array members. Shared headers retain the same
  fixed-prefix ABI layout, exposing zero-size flexible tails only to C clients;
  current C++ clients access only the shared prefix. No one-element substitute
  arrays or competing struct definitions are used.

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

Natural `memcpy` / `memset` source is kept where its lowering matches the
original helper entry. `func_00154378` deliberately calls the shared general copy
entry `func_001a000c` for word-aligned buffers: natural memcpy currently selects
an unbound aligned helper instead. This preserves the observed call target
without inventing a helper binding or weakening the real word-pointer types.
Compiler/library helper bodies remain uncredited.

## Linker-model qualifications

- None of the 37 wave-5 additions requires a rewrite.
- The newly integrated w6/w7 wrappers use branch-to-next (`branchnop`) rewrites
  where their explicitly typed target is the adjacent catalogued function.
- `func_00160298` has two `inline_insn` getter calls. Its source marks the choice
  of `func_001c6af4` **[INFERENCE]**: the eligible body reads `ItemPage.count`, but
  inlining does not recover the original callee identity.
- Revised sleep wrapper `func_0011ef54` retains its adjacent `func_0011ef58`
  branchnop and signed 64-bit r0/r1 forwarding inference.
- The model qualifies inherited material too: inlined calls prove eligible
  callee bodies, not identities; branch-to-next proves the final adjacent target,
  not whether source order or linker reordering placed it there. Conditional
  single-instruction inlining remains manual-backed. See the eventual receipt's
  `linker_rewrites`, not a claim of recovered original call graphs.

## Revisions to accepted material

The four carried wave-5 revisions remain: w1 `func_00134f04` uses the notification
status halfword, `func_0018fcb4` returns the separate service context, and
`func_00195e84` / `func_00195e94` use the shared signed flag / unsigned mode fields.

Additional revisions are limited to interfaces and common views:

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

The carried c004 sleep and WordPair units also use the shared headers. Six more
c004-only declaration/layout migrations are intentionally deferred until the
separately fixed c004-r3 baseline lands; their exact units/evidence are recorded
in the integrator's private notes. They are the remaining current dry-interface
items, not waived policy findings.

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

Every currently collected function has re-derived MATCH at its destination;
source/header changes receive sequential per-unit rechecks during integration.
The private dry ledger is exactly canonical matches + c004 candidates + c005
candidates. The integration pre-review reads every c005/revised source for
plausibility, typed interfaces, source policy, preserved bugs and linker callee
qualifications; C++ interfaces are checked by hand, not waived semantically.

Only RESULT rows re-derived as MATCH are collected. Other mismatches, extent
reports and library candidates remain outside this manifest. No runtime helper
body is credited, no catalogue boundary is padded, and no candidate is silently
dropped to make a gate pass.

Full-image equality, two fresh builds, the full suite, sealed independent
reviews and finalize are still required for acceptance. This worker performs no
full build/suite or prepare/finalize, changes no canonical ledger, and publishes
nothing. The current c004-r3 cutover and final complete collection/check results
will be reflected here before the integration handoff.
