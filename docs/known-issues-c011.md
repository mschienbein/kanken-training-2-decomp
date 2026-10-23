# c011-r8 candidate — folded intake, callback closure and qualifications

This is an **unaccepted candidate** based on **c010-r8 `f78df906`**, merging
c011-r7 `8359b859` and intake branch c013 `6b438040`. **C013 is folded into c011;
there will be no separate c013 checkpoint.** Its active manifest and standalone
known-issues document are removed. Its immutable evidence and original document
are archived under `scratch/integrator/c011/r8/folded-c013/`; their old internal
paths describe the historical intake context, not current gate inputs.

Finalized c009-r6, denominator **v13-r2**, frozen gates **v1.19**, and explicit
**armlink5-inline-branchnop-v6 / anonymous-string-records-v1** remain. Main must
finalize the applicable c010-r8 and recheck the exact source composition before
preparing this single c011 candidate. No acceptance record or history is revised.

## Current scope and ownership

- **71 candidate units / 88 claims:** **79 NEW functions / 11,080 B**, plus
  **9 explicit stable-identity moves / 872 B**. All claims are ordinary creditable
  functions under the frozen catalogue.
- All **2,682 c010-r8 retained predecessor identities / 288,580 B** survive.
  The proposed ledger contains **2,685 units / 2,761 functions / 299,660 B**,
  including all **24 helper-only units**. These are proposed figures, not credit.
- Folded intake adds **3 NEW / 908 B**: `0012e68c` (296 B, middleware),
  `0010aa28` (280 B, game), `001c65e4` (332 B, game). Existing source paths under
  `src/checkpoint13/` are retained as provenance; they are owned only by the c011
  manifest. There are no intake moves or new name bindings.
- Prior **111 c011 holds / 21,832 B** survive. Four intake holds / 844 B add only
  **3 distinct holds / 516 B**, because `001d01dc` was already held. Current
  **c011: 114 holds / 22,348 B**. With **94 c010-r8 holds / 8,404 B** and
  **28 c009 holds / 3,588 B**, there are **236 distinct combined holds**.
- The nine move origins match current c010-r8 owners; four emptied source paths
  stay absent. `00155ab4` remains at its prepared c010 owner. Accepted bank
  accessor `001f4500` remains solely at `src/wave9/func_001f4500.c`; no nm01 TU,
  native duplicate, speculative bank overlay or implicit move is admitted.

Current evidence entry point: `scratch/integrator/c011/r8/HANDOFF.json`.
`FOLD-C013-r1.json` maps every archived intake record without rewriting its bytes.
Only the r8 proposed ledger, source bindings, matcher/rewrite closure and Main
commands are current; old c013/c011-r7 proposals are not standalone gate inputs.

## C010-r8 rebase and exact callback repair

The generic array callbacks `0014d664`/`001c4b10`, sound-loader thread entry and
three generic atexit callbacks retain c010-r8's exact invoking protocols. Its
five additional NEW-only holds (`0014b54c`, `00107ce4`, `0014b85c`, `001d4078`,
`001e5a20`) remain excluded. No c011 object references a held identity or any
of the six revised c010-r8 provider bodies. All inherited predecessor bindings
and current canonical declarations remain.

The parent `RUNTIME-CALLBACK-CLOSURE.json` exposed a real intake mismatch:
original caller `00104e9c`, site `00104f3c`, registers `0010aa28` through runtime
`0019dd50`, whose callback type is **`void(void *, void *)`**. The delivered
one-input declaration was not sufficient. The actual worker definition and
`game/runtime.h` now retain **both argument and context**, with the unused second
input explicit. The accepted `ThreadStartContext.entry` and `0012d104` invoker
supply those same two inputs. A bounded preprocessed C type comparison proves
both declaration and definition exactly equal the runtime's structural type key.
Focused and final complete byte proofs **MATCH 280/280**, exit 0. No alternate
thread runtime, callback cast or fabricated return is introduced.

The worker still uses the canonical completed BufferOwner ring: signed indices,
256 CopyRequest entries, existing buffer/count/owned/state/active fields, stop
flag and WorkerThread tail. It clears active only after draining and returns on
stop without clearing it. Captured SignalState and field roles remain
`[INFERENCE]`; signed-length copying and existing invalid-input behavior are not
changed by added validation or synchronization.

## Callback/slot providers: exact where known, qualified where opaque

Main explicitly ruled that an unrecovered invoker alone does **not** require a
hold when the table remains opaque, no selected source invokes through its
storage, no provider native identity is claimed, body/direct-caller types are
evidenced, and no new integer/function-pointer initializer cast is introduced.
These three rows therefore remain credited **QUALIFIED**, not silently compatible:

| Provider | Storage | Per-row qualification |
| --- | --- | --- |
| `001d033c` | `002245ec + 0x1c` (`00224608`) | **Invoking population and protocol unrecovered; address stored only.** Storage stays the existing opaque unsigned array. The own C body is `void(LinkedOwner *)`, forwarding its actual OwnedLink field to the matching C helper. No callable slot completion, selected typed invocation, native alias or new function-address initializer is introduced. |
| `001d222c` | `002246b8 + 0x1c` (`002246d4`) | **Invoking population and protocol unrecovered; address stored only.** The own C body/direct helper share LinkedView pointers and void result; the existing table stays opaque. No native identity, callable table completion, selected invocation or pointer/integer cast is added. |
| `001c8894` | `002434ec` | **Invoking population and protocol unrecovered; address stored only.** The provider's actual body uses `IAllocator *`, a real ResShape value and void result. No source table declaration/completion, selected table invocation, native provider alias or new function-address initializer is introduced. |

A fresh allocated-data function-address census finds exactly these three scoped
storage candidates, with only addresses and hashes recorded. It does not infer
neighboring extents, homogeneous slot signatures or a native class from proximity.
The original storage is unchanged. Each condition and body/direct-helper witness
is explicit in `CALLBACK-SLOT-CONTRACTS-r1.json`.

Other known callable interfaces retain their actual types: the four-input release
callback, genuine native allocator calls and FxDelay methods, and native static
Pair/Scalar lifetime registration on their real classes. Compiler-generated
native constructor/destructor ABI is distinguished from ordinary C callback
providers; no source-level callback conversion or guessed class is used. Native
R1 lifetime/registration provenance remains qualified.

## Folded native signal and language integration

`TransformNode::CreateCallbacks` uses canonical Signal2/Slot2 types. The known
slot protocol is **`void(TransformNode *, SceneContext *)`** on its genuine native
Slot2 receiver. No concrete slot provider or function-address array is imported:
the new factory creates empty canonical signal storage, using its existing
allocator and slot-list interfaces. The wrapped/variable MoveArray constructors
initialize fresh capacity storage before their existing setters; there is no
source-local specialization or second Signal/TransformNode model. Eventual
external provider populations remain unrecovered, not declared compatible.

The language selector uses `game/language.h` for the existing constant byte,
actual shared result/service interfaces and source-level regional masks. Approved
`__current_pc` use remains explicit. **`ORIGINAL BUG:`** the configured language
is assumed below 32. Region/helper/TU identity and middleware ownership remain
`[INFERENCE]`; other compiler profiles are not claimed remeasured.

## Folded intake holds

| Identity | Bytes | Preserved boundary |
| --- | ---: | --- |
| `001d9820` | 156 | Competing ConstructedPaneObject/AudioVoice construction and member/registration/timing owner models; no allocation-result cast or native alias. |
| `001d01dc` | 328 | Already a c011 hold. Native SoundArchive count/reader would alias a retained C ArchiveHandle provider; Window context also remains unreconciled. Counted once. |
| `001f4408` | 180 | Bank chunk/reference decoder still uses forbidden byte-offset resolution and unproved serialized array extents. No nm01 overlay or accessor duplicate admitted. |
| `001f46d0` | 180 | Same unresolved bank decoder; tag-evidenced casts do not waive its separate storage/extent obligations. |

All earlier bank/list/result/native-facade, font, texture, event, owner, pool and
audio exclusions remain. Genuine canonical ServiceResult attempts that failed
in r7 stay failed evidence; no held implementation or alias is reintroduced.

## Exercised complete proof and Main boundary

The final complete proposal ran **2,661 sequential, niced, single-process
`tools/match.py --show 0` commands**, all **exit 0**. All **2,761 required functions
MATCH**, with all **24 helper-only definitions** supplied in natural contexts,
under frozen profiles and explicit v6/L1. The separate exact-worker focus also
exited 0: **2,662 actual matcher commands, all exit 0**. The final source-bound
rewrite projection is **`scope_complete=true`**; this is not a two-fresh-build G4
certificate. Every runtime.h/MoveArray dependent is covered by the full sweep.

Fresh object inspection finds no new defined vtable, no C/native facade edge and
no candidate references to any of the 236 holds or revised parent provider
bodies. Known runtime provider contracts are compared against the frozen parent
closure; opaque rows retain Main's exact qualification above. Published metadata
omits fixed instruction encodings and retains untouched raw receipt hashes.

R1 helper identity/call graph remains `[INFERENCE]` with QUALIFIED review rows.
L1 proves only its witnessed literal span, not complete historical pool extent,
order, ownership or data credit. Native aliases, partial extents, preserved bugs,
ARMCC extensions and C-only MUST-discard callback qualifications remain. No
accepted bank/list/result/scene/font family is declared recovered by exclusion.

No whole-ledger interfaces/argcheck, full build, suite, prepare, review registration,
finalize, push, subagent or task tool ran here. Main must finalize the applicable
c010-r8, integrate this complete single-candidate transaction, run/adjudicate
serialized whole-ledger gates including unchecked C++ and unique emitted
ownership, prepare with two fresh builds/the suite, and obtain two independent
bound reviews. Exact commands are in `r8/REMAINING-HEAVY-COMMANDS.json`. Input or
composition drift requires fresh affected proof under unused immutable names.
