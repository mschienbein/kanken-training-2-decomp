# Known issues — checkpoint c006

## Scope and status

**Candidate integration, not accepted credit.** The manifest contains **379 units / 381 functions /
30,724 catalogue bytes**, authored by the OpenAI-family wave 8–10, L001–L003 and x01–x11 workers
(`author` is retained per unit). FinishC006 completed the earlier IntegrateC006 integration.

| Destination | Units | Functions | Catalogue bytes |
|---|---:|---:|---:|
| `src/wave8` | 102 | 102 | 3,952 |
| `src/wave9` | 92 | 94 | 5,960 |
| `src/wave10` | 121 | 121 | 16,800 |
| `src/sdk` | 64 | 64 | 4,012 |

Profiles: **363 `game`, seven `sdk`, nine `middleware`**. Verification uses **adopted catalogue
v8**, SHA-256 `e33aad973846b60b5286be6becb5080c9626188fa9a64a6c0b48197b16e8adad`.
No candidate overlaps a v8 non-creditable owner or changed boundary; the specific v8 splits and
re-extents flagged by Main are outside this manifest. Frozen profiles and canonical credit were
not changed here. The composed dry ledger is canonical matches plus c005 and c006 candidates,
duplicate-checked: **835 units / 870 functions**, based on finalized c005-r2
(history sequence 5; 489 accepted functions).

The supplied snapshot already contained every closed-scope author's final MATCH row. The
remaining w10j notification rows and previously pending narrow IPC wrappers had been collected
before that snapshot; none was silently omitted. The one newly excluded row is listed below.
There is no unresolved collector refusal in this closed scope. Later waves, large-function
batches and c007 are not included.

## Revisions to accepted units

Every row below remains `game` and has a current-source MATCH receipt. Together with the new
manifest these are **389 checked units / 391 functions**.

| Accepted source | c006 change |
|---|---|
| `src/wave1/func_0011f080.cpp` | Canonical resource-table entry/global and native object cleanup view. |
| `src/wave1/func_0019f450.c` | Shared frame-timing state. |
| `src/wave2/func_001770cc.c` | Shared animation owner/state. |
| `src/wave2/func_00179398.c` | Explicit widget-controller prefix. |
| `src/wave2/func_001b7c9c.c` | Shared NameTable and widget-slot state representation. |
| `src/wave4/func_0013e71c.c` | Shared mapped-memory state and size. |
| `src/wave4/func_001470c8.c` | ObjectManager lock uses the shared StreamMutex. |
| `src/wave4/func_0014a988.c` | Forward the writable HwrImage pointer, not an invented integer enable flag. |
| `src/wave4/func_0019dc04.c` | Shared mapped-memory state and memory pointer. |
| `src/wave6/func_001963e8.c` | Include the canonical BoundDisplayObject queue interface instead of local ResourceEvent declarations. |

The last revision was applied only **after c005-r2 finalized**, in the separately named commit
`3e9328a`. The historical c005-r2 requests concerning SoundRecord, notification storage,
PoolLink/PoolList, stream queues and metadata entries were already resolved by the merged r2
baseline. **No c005-r2 source revision needed by c006 remains pending.** Successor-only
exp-heap, graphics-resource, NW const-buffer and ServiceObject changes belong to c007, not this
checkpoint.

Shared source interfaces use PoolLink/PoolList rather than the competing Registry types;
NameTable/RenderParameter rather than duplicate descriptor spellings; distinct device-latch and
touch snapshots; one notification storage union; and the shared character-controller prefix.
The list-controller view exposes twenty evidenced slots, an unidentified word at +0x70 and the
refresh byte at +0x74, not an invented twenty-first slot. Semantic names and unrecovered original
class identities remain **[INFERENCE]**.

The supplementary shared-callee audit also repaired `func_001855fc` in
`include/game/region_heap.h`: its result is **`int`, not `void`**. Original calls at `00125a7c`
and `00147644` consume r0. Signed source spelling remains **[INFERENCE]**. This header repair
does not claim another credited function; the complete v8 matching pass includes it.

## Classified, not credited

`func_00152964` / **20 bytes** was removed from the candidate manifest and its uncredited source
removed from `src/`. The v8 evidence identifies the compiler-emitted
`__ARM_common_memclr4_12`; a matching three-pointer reset is not independent C credit for a
compiler runtime helper. All **27 admitted classification rows** in the supplied
`scratch/analyze/REPORT-v8.md` were intersected with the c006 manifest: this was the only hit.
Main adopted that classification and denominator on main; c006 was then reverified under v8.

## Non-game profile plausibility

The following are SDK-style IPC or filesystem objects, not game/menu functions. This is a source
and ABI plausibility judgment **[INFERENCE]**, not authenticated library-archive ownership.
ARMCC 4.1 build 791 is a measured reproducer, not a claim that the original compiler build was
uniquely recovered; see [COMPILER.md](COMPILER.md#middleware-profiles-frozen-2026-10-17).

| Profile | Functions | Observed source shape |
|---|---|---|
| `sdk` | `0010d418`, `00115a98`, `001235f8`, `0012ff20`, `00136878`, `00148118`, `0014aed0` | TLS service-command marshalling, narrow request fields, synchronous kernel request and result extraction. |
| `middleware` | `0012fa6c`, `0014af70`, `00185120`, `001863fc`, `0018f068`, `001c4ec0`, `001c4ef0`, `001c4f80` | The same SDK-style service request/response population, including output words/handles. |
| `middleware` | `001c4d9c` | Filesystem-object pool allocation, vtable/handle transfer, close-on-allocation-failure. |

The five formerly pending volatile-parameter wrappers now use address-taken byte/halfword
marshalling under frozen `sdk`; **no volatile parameter spill steering is credited for them**.
`0012ff20` and `00136878` use the same natural reference helper; `0012fa6c` uses a width-preserving
`memcpy`. Profile names alone do not establish SDK/middleware identity.

## ARMCC instruction-extension qualifications

The [user ruling](rulings/2026-10-17-armcc-extensions.md) admits the documented extensions, not
inline assembly substitutes. SVC declarations live once in `include/ctr/svc.h`; TLS and ARMv6
barrier named-register declarations live once in `include/ctr/cp15.h`. Intrinsics use ARMCC's
built-in declarations. **68 scoped functions** use instruction extensions:

- **TLS + SendSyncRequest + explicit scheduling barriers:** `0010bc9c`, `0010c444`,
  `001124dc`, `00112518`, `00113404`, `00113c74`, `00113ddc`, `00115ad4`, `00123634`,
  `00123664`, `0012fab0`, `0012feb0`, `0012fee8`, `00136278`, `00137bf8`, `00137c20`,
  `00137c5c`, `00137cd4`, `0013823c`, `001382c0`, `001426a8`, `00142718`, `00142a2c`,
  `00143020`, `00145fcc`.
- **TLS + SendSyncRequest:** `0010d418`, `00115a98`, `001235f8`, `0012fa6c`, `0012ff20`,
  `00136878`, `00148118`, `0014aed0`, `0014af70`, `00185120`, `001863fc`, `0018f068`,
  `001c4ec0`, `001c4ef0`, `001c4f80`.
- **TLS only:** `0010be14`, `0010bf74`, `00121364`, `00140fcc`, `00192bd8`.
- **MapMemoryBlock:** `0010bf88`, `00136754`.
- **CloseHandle + diagnostic current-PC intrinsic:** `0011516c`.
- **CloseHandle:** `0012192c`, `0012e7b4`, `00143ac8`, `001bb9b4`, `001c4d1c`, `001c4d9c`,
  `001c51a4`, `001c526c`, `001c5294`, `001c52bc`, `001c5738`.
- **CloseHandle + UnmapMemoryBlock:** `00182198`.
- **CP15 synchronization barrier and its scheduling barriers:** `0013e810`, `0013e9d8`,
  `0014e078`.
- **CP15 memory barrier and its scheduling barriers:** `0014d298`.
- **Diagnostic current-PC intrinsic:** `00146dfc`, `001c4494`, `001ef984`.
- **Diagnostic current-PC intrinsic + explicit scheduling barrier:** `001c44c0`.

These source-level qualifications do not prove that the original source contained the same
intrinsic spellings. The barrier helpers express the approved ARM11 CP15 operations, not inline
assembly. No required-r4/r5 syscall ABI has been replaced with a misleading four-register wrapper.

## Explicit codegen-steering and representation qualifications

These are not silently presented as recovered original source:

- **`001c44c0`: fake-match/codegen-steering qualification.** Its scheduling barrier preserves
  diagnostic-PC capture before the failure test. The original intrinsic is not proved.
- **`00193414`, `00193420`:** volatile halfword views retain full-width reads before truncation;
  original volatile source types are not established.
- **`001124dc`, `00112518`, `00113404`:** volatile IPC command-buffer views and scheduling order
  are retained and disclosed; volatility is not a uniquely recovered source type.
- **`0017e790`: codegen-steering qualification.** The value-position adapter retains the
  canonical pointer-based PoolList insertion interface and the actual three-word forwarding.
  A direct pointer-only call produced 12 bytes rather than the original 16. The adapter's
  presence in original C++ is **[INFERENCE]**; no competing C-linkage iterator prototype remains.
- **`001c32e0`, `001c3748`, `001c3d00`: fake-match/codegen-steering qualification.** The empty
  counted loops and live counter results reproduce 20-byte loops. Shorter constant returns
  produced eight bytes; void empty loops produced four. All observed callers pass the target
  group and discard the result, so the source-level integer return contract is unresolved.
  No erased original template operation is invented.
- **`001bb9b4`, `001c4d1c`, `001c51a4`:** codegen-sensitive container-of reconstruction returns
  the owner pointer from the traversed handle/member. These equivalent source expressions are
  **[INFERENCE]**, not recovered original syntax.
- **`0018269c`, `001851f0`, `001971a0`:** the common source uses ARMCC `noinline` on the first
  two leaf queries to preserve independently observed call boundaries. This is an explicit
  compiler-attribute qualification, not evidence of original source attributes.
- **`001dbcec`:** `bss_00280af8` is the interior symbol for the same canonical payload at
  `bss_00280af0 + 8`, not another storage allocation or an incompatible payload layout. The
  source's literal-root symbol choice is **[INFERENCE]**; directly rooting through the outer
  object's member changed codegen. UTF-16, word-copy and field/history clients share one union.

No inline/embedded assembly, encoded executable arrays, ROM-byte dump, original disassembly or
copied proprietary SDK source was introduced by this integration.

## Preserved original bugs

Each has an `ORIGINAL BUG:` comment in its source:

- `001512b8`: negative child indices are not rejected.
- `001528dc`: a missing prefetch block is dereferenced before checking its signature.
- `00170d50`: append capacity is not reduced by the existing length, and no new terminator is
  written.
- `001dbcec`: overlapping history ranges use `memcpy`, not `memmove`.
- `00164d5c`, `00167860`: allocation failure is not checked before filling the pointer array.
- `00178f28`: a terminator is written after exhausting the supplied byte limit.
- `00179160`, `001e403c`: the original dereferences an assumed-present display/cursor object.
- `0017e2e8`: mandatory sequence blocks are assumed present before advancing past their headers.
- `00146dfc`: an invalid seek origin leaves the translated origin undefined.
- `0014766c`, `001476ac`: a missing archive block still dereferences a null reference.

Input bounds and partial-layout extents not independently recovered remain qualifications, not
new validation behavior. Diagnostic continuations after terminal error-report paths do not prove
normal returned values or initialized output values on those paths.

## Caller-contract adjudication

The ordinary dry-ledger audit reports **30 raw candidates in 19 functions**, with **123
input-observation notes**. A supplementary audit of **177 uncredited callee contracts** (101
shared declarations and 76 additional C-visible callees, including the standard `strlen` alias)
checks **1,047 functions**: **45 raw candidates in 32 functions / 154 input notes**. Every row
is adjudicated without suppressing the raw reports. The final augmented audit covers 10,140
direct sites, 545 tail sites, five resolved callbacks and 15 restored inline sites. These
additional contracts are audit inputs, **not matching credit**. Core and VFP allocation were
considered independently.

| Function(s) | Disposition |
|---|---|
| `00102cb0`, `00105084`, `001071cc`, `001074d0` | Reported return reads are armlink self-copies, not result consumption. |
| `00103258`, `00103798`, `00104f90` | Reported r0 setup is an armlink self-copy, not a supplied input. |
| `0015e948`, `00164ebc`, `00174480`, `001816e0`, `001826fc`, `001852f4`, `00195a08` | Extra r3 writes restore epilogue stack padding. Existing genuine inputs are retained. |
| `0016a178` | Condition-sensitive continuation analysis finds no live returned-r0 read at the reported candidate, starting with all 16 NZCV assignments and conservatively allowing unknown flag updates. This is a local proof, not a global claim about all future paths. |
| `00181700` | Reject the speculative extra r1. Four of six physical calls in `00141cd8`/`001456e4`/`001793e4`/`001e403c` have no dedicated r1 setup. The two isolated zeros do not establish a universal parameter; r3 witnesses are stack padding. Existing explicit-r1 probes for both c006 callers mismatch by four bytes and were not rerun. |
| `00191134` | r1 computes the condition codes for the conditional timing-getter call, not a dedicated input. |
| `001971a0` | s0 is a caller temporary consumed after the pointer result; the resolver and both actual helpers have no floating-register access. |
| `0019d6c4` | 33 r2 condition-code computations and two r3 epilogue restores, not extra arguments. |

The supplementary callee-contract candidates have these additional dispositions:

| Function(s) | Disposition |
|---|---|
| `0015e474`, `0015e684`, `00195ad0`, `0016bde0`, `00186944`, `00190688`, `0019f338` | Extra r3 witnesses restore epilogue stack padding. |
| `00195ec4` | Residual r1/r2 at `00213670` belong to an erased destructor-registration operation at `00213660`, not mutex inputs. The registration identity is **[INFERENCE]**; the actual mutex helper consumes only r0. |
| `0019dfbc` | Condition-code temporaries and padding, plus 22 timeout-high r3 values already consumed by an intervening raw WaitSynchronization1 SVC `0x24`; the tool does not model that raw SVC argument consumption. |
| `0019f568` | Core-word witnesses are condition-code temporaries/self-copies/padding. The apparent return read belongs to a terminal fatal-diagnostic continuation, not normal return-value proof. |
| `00136630` | The apparent returned-r0 read is an MRC into r0: the CP15 TLS read replaces r0. Decoder operand-access metadata misclassifies this as consumption of the lock-call result. |
| `00181448` | Two of seven calls prepare r1=0; five do not dedicate r1, and the queue routine reads only incoming r0. Reject a speculative universal extra parameter; unused original inputs are not proved absent. |
| `00192afc` | r2 witnesses compute condition codes. One apparent result read is a self-copy; the other is a conditional failure-report continuation through `00186294`. The terminal-diagnostic qualification remains, not an invented normal result. |

All **154 augmented input notes** retain the declarations: forwarded values, earlier setup and
already-consumed writes are not grounds to remove a parameter. Source/shared-callee/caller/
callback evidence is considered as applicable; unused inputs and unresolved invocations remain
qualified. Names, opaque pointees and original integer spellings remain **[INFERENCE]** where
the binary does not uniquely establish them.

Additional explicit interface limitations:

- `001c457c` and `001c47a8` are entries in the resource vtable installed by `001c47b8`. Their
  otherwise-unused object input is now retained. Additional unused operation inputs are
  unresolved, not proved absent by constant-return bodies.
- Static zero-result callbacks `001c7a1c` and `001c7d74` have no resolved invocation. Their
  unused inputs and integer-zero versus null-pointer source identity remain qualified.
- **113 C++ units** are explicitly unchecked by the mechanical C parser; all **89 scoped C++
  sources** were read manually with their shared declarations, native constructors and used
  virtual/member/callback contracts. The ordinary audit has **102 functions** without a parsed
  C prototype; borrowed C declarations reduce this to **69** in the supplementary audit, without
  mechanically certifying the C++ sources.
- The caller tool retains **1,048 unresolved indirect sites** across the image. These are not a
  claim of absent indirect consumers, and equal field offsets were not treated as object identity.

## Exercised evidence and remaining acceptance work

Interpreter: `/home/michael/code/kanken-training-2-decomp/.venv/bin/python`; cwd:
`scratch/worktrees/c006`. Private, source-bound records are under
`scratch/integrator/c006-finish/` in that worktree.

1. `tools/match.py SOURCE --profile PROFILE --out ... --show 0`: one complete adopted-v8 pass,
   **389 current-source MATCH receipts / 391 functions, all 389 commands exit 0**, in
   `matches-v8.json`. Earlier mismatches and shorter-source experiments remain separately
   recorded, not overwritten or called passes.
2. `tools/interfaces.py check --matches scratch/integrator/c006-finish/dry-matches.json
   --allow-unchecked-cpp --json ...`: **exit 0; 835 units; 722 checked C; 113 explicitly
   unchecked C++; zero errors, zero policy items, zero unchecked C**.
3. `tools/argcheck.py --matches scratch/integrator/c006-finish/dry-matches.json --json ...`:
   **exit 2**, the tool's unchecked-C++ outcome, not a raw PASS. `argcheck-v8.json` retains the
   30 candidates / 123 notes; `argcheck-adjudications.json` closes every row.
   The supplementary callee audit with repeated `--matches`, source overrides and `--rewrites`
   also exits **2**; `argcheck-all-callees-v8-final.json` retains all 45 candidates / 154 notes,
   closed in `argcheck-all-callee-adjudications-v8.json`. Sanitized independently derived facts
   are in `caller-derived-facts-all-callees-v8.json`, `shared-callee-derived-facts.json` and
   `local-callee-derived-facts-v8.json`. The `strlen` alias repair changes no candidate evidence.
4. The source-policy inventory has no remaining integration marker or local C++ extern-C copy.
   `pre-review.json` contains **391 source-bound function rows covering all seven steps of
   [REVIEWING.md](REVIEWING.md)**, including the qualifications above. It is an integrator
   pre-review, **not an independent reviewer certificate**.
   `delivery-bindings.json` binds the final reports, all 835 dry-ledger sources, 51 headers and
   relevant configuration hashes. `interfaces-v8.json` is the final interface report.

No full build, full suite, c006 prepare, sealed independent review, finalize, push or main-branch
commit was performed by this worker. Main must run those acceptance stages and re-review the
revised accepted units before c006 can receive credit. No runtime-behavior equivalence or unique
original source reconstruction is claimed by a per-unit byte match.
