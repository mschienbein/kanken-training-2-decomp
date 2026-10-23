# c011-r11 — accepted (history seq 12); source-policy remediation and folded c013/c014 intake

**Accepted 2026-10-23 as history seq 12:** 2,770 functions / 301,020 B (+88 functions /
12,440 B over c010-r8). Prepared at dd1c5f2e (prepare.json sha256 9d72ce48…). Reviews codex-b
and codex-a both PASS_WITH_QUALIFICATIONS with 0 findings (accept/c011-r11/reviews/). Every
qualification in those records stands. The 117 holds (116 NEW-only holds + F04 `001ce05c`) and
`0017e764`, which belongs to c012, are uncredited. The text below is the candidate record.

This was a **single candidate** descending from c011-r8 `f9ea308e`.
It retains c013's earlier fold and now merges **c014 `479eb7f4`** into c011.
**No separate c013 or c014 checkpoint exists.** Historical intake manifests,
documents, agreements and proof records are archived unchanged under the r8/r9
fold directories; their old internal paths and checkpoint order are provenance,
not current gate inputs.

The accepted predecessor is finalized **c010-r8: 2,682 functions / 288,580 B**.
Main has adopted **GATES v1.20**. This author branch leaves its historical gate
files untouched and selects only the unchanged **armlink5-inline-branchnop-v6 /
anonymous-string-records-v1** algorithms. Integration must retain Main's adopted
v1.20 files; no U1/v18 selection or gate-design change is proposed here. The
interfaces checker used here is byte-identical to Main's failed-merge checker.
Denominator remains **v13-r2**.

## R11 source-policy remediation

Both independent r10 reviews FAILed despite exact matcher proofs. Codex-a's
three findings are the same AnimationChild, HWR-root and names-table issues
as codex-b F01, F03 and F02 respectively; codex-b F04 adds the packet overlap.
R11 changes no acceptance records, frozen gates or linker/literal selection.

- **F01 fixed, 72 B:** `00171d40` reads the canonical `AnimationChild.object`
  and `.table` members directly; no memcpy prefix view remains.
- **F02 fixed, 40 B:** `W8TextTable` owns its real `TextTable names` member at
  `+0x84`; the accessor returns `&table->names`, not a one-past-object cast.
- **F03 fixed, 456 B:** `data_00243830` is one `HwrCandidateStorage *` root.
  The accepted `0014db18` initializer publishes the canonical typed
  `bss_004de5cc` object; all selected users share that declaration. Its 36-B
  accepted body and the new consumer both re-prove MATCH. The candidate
  explicitly records it in `revised_accepted_units` metadata, not as NEW
  credit or a move; prepare's normal source-hash detection reopens its review.
- **F04 held, 276 B:** three compliant bank-owned packet representations
  produce 268/272/272 B rather than the required 276 B (matcher exits 1/1/1).
  The NEW singleton is removed from the manifest, and its source, exclusive
  types and declarations are deleted. `graphics_address.h` retains its other
  live descriptor/address interfaces. No accepted declaration is weakened.
  The manifest's hold names F04 and `scratch/c011-r11/F04/DISPOSITION.json`.

All failed attempts retain source/header snapshots, commands, exits and matcher
receipts under `scratch/c011-r11/`. The integrated-base dependent closure,
including the conservative shared-header R1 expansion, is **78 sequential
matcher commands, all exit 0: 81 credited functions MATCH**, with all
**24 helper-only definitions** supplied in their natural caller contexts.
`CLOSURE-PLAN.json`, `CLOSURE-RESULTS.json` and `MATCH-COMMANDS.json` record
the scope and every exit. R11's separately granted whole-ledger interfaces
check ran **once, exit 0: 2,694 units, 0 disagreements, 0 policy items**.
Its **733 unchecked C++ units** retain the explicit parser-coverage waiver;
1,961 units are checked. `INTERFACES.json`, the exact request/grant, captured
output and immediate atomic heavy-slot release are retained in the same
directory. No full build, suite or acceptance operation was run for r11.

## Current combined scope

- **80 candidate units / 97 claims:** **88 NEW functions / 12,440 B**, plus
  **9 explicit stable-identity moves / 872 B**. All claims are ordinary creditable
  functions, not classified runtime or library credit.
- The current proposal is **2,694 units / 2,770 functions / 301,020 B**, including
  all **24 helper-only units**. Every accepted identity survives. Four emptied
  move-source paths are absent; `00155ab4` stays at its accepted c010 owner.
- C013 contributes **3 NEW / 908 B**; C014 contributes **10 NEW / 1,636 B** with
  no extra moves. Existing `src/checkpoint13/` and `src/checkpoint14/` paths are
  owned only by the combined c011 manifest, not separate checkpoints.
- C011 has **117 distinct holds / 22,808 B**. C014's three holds / 604 B add only
  **two distinct holds / 184 B**, because `001ae140` was already held. C009's
  **28 / 3,588 B** and c010-r8's **94 / 8,404 B** remain predecessor-owned:
  **239 combined distinct holds**. No rejected body or speculative name overlay
  is reintroduced.

R11 entry point: `scratch/c011-r11/`. Its composed ledger and dependent
closure supersede the affected r10 inputs. The complete r9 baseline evidence
is retained at `scratch/integrator/c011/r9/HANDOFF.json`; earlier unfurled or
pre-c014 ledgers must not drive combined gates.

## Main interfaces FAIL remediation

The immutable merge-main result reported **one ODR disagreement and eight
inline-declaration policy items**. It was not rerun unchanged to challenge it.

### Distinct choice objects, not a layout alias

C011's data record is now **ChoiceGridState**, with the same fields and offsets;
only its descriptive type name and matching function declaration/definition
change. The accepted polymorphic **ChoiceController** in
`src/checkpoint10/func_0015e8c4.cpp` is untouched.

The distinction has actual receiver-role evidence: in original context
`001b3434`, word zero of the incoming data receiver is loaded at `001b34fc` and
supplied as the scalar divisor to `__aeabi_idivmod` at `001b3504`; the table
pointer is read at `+0x7a4`. C011 `001b376c` accesses the 23-entry, 0x54-stride
array at `+0x18`. The accepted ChoiceBinding consumer instead dereferences the
controller's word zero as its virtual table and calls setActive. A generic
invented name never established one object identity. No base/derived relation,
vptr reinterpretation, cast or second model of the accepted class is asserted.
The grid name/roles remain `[INFERENCE]`; the uncredited context is semantic
field evidence, not a claimed full-function MATCH or CFG proof.

### One canonical declaration per credited interface

All nine reported local-declaration sites were removed in favor of existing
canonical headers, except the missing `00138274` declaration, which is now in
`game/applet.h` and included by its definition. Exact normalized types agree.

The required whole-scope sweep also found **six additional declaration sites**
whose providers are C++ and therefore escaped the C-only definition check:
`0010bff4`, `0010c0a4`, `0015e830`, `00139174`, `001835b8`, and folded getter
`00192a7c`. Their callers now use the canonical headers too. The application
configuration and service-client headers expose only their real shared scalar
C APIs to C, keeping C++ implementation declarations guarded.

**13 accepted caller units** have declaration-only revisions; their function
body token hashes remain identical and each re-proves MATCH. The old local
`00139174` declaration incorrectly returned int; the actual provider/shared
header returns unsigned. The accepted forwarding caller keeps its own public
int result while using the correct callee declaration and still MATCHes.
Including c014's coherent field/type cutovers, **31 surviving accepted source
units / 31 functions** were revised in r9. R11 additionally revises accepted
`0014db18`, so **32 surviving accepted source units / 32 functions** require
renewed acceptance review.

The final scoped sweep covers **410 qualified class/struct/union identities**
from c011/c014 sources and changed headers, compared against every project
header and selected C/C++ source. Namespace/template/local identities are
respected. It finds no differing definitions or remaining local C++ declarations
of c011-credited functions. The source declaration search has only an unselected
old texture attempt and two ordinary call statements, not live declarations.

## Earlier r9 whole-ledger interfaces gate

Main granted the exclusive heavy slot for **exactly one combined run**:

**Exit 0 — 0 disagreements, 0 policy items, 2,695 units.**

All 734 unchecked entries are the explicitly allowed C++ coverage limitation;
there is no C parse/preprocess error. This is a real whole-ledger result, not the
bounded pre-review or the scoped metadata sweeps. The request, grant, command,
output hashes and immediate atomic release are retained in r9. No second heavy
command was run and no failing result was overwritten.

## Folded c014 integration and TaskPool ownership

The ten units retain their frozen profiles and natural partitions:
`001f29b4`, `00122360`, `00154674`, `00136450`, `00192a7c`, `00157500`,
`0018191c`, `001acb38`, `0014cc1c`, `0014242c`.

- Serialized byte parameters use the pinned typed Util::Table/reference
  interfaces, not a synthesized indexed byte displacement. Seven-byte output,
  narrowed count and original unchecked reference behavior remain qualified.
- Aligned spans share AlignedBuffer; the unrelated LinearBuffer allocator is
  not aliased to it. Service string records/copy declarations and the real
  handle-bearing object plus checked `+4` handle word are shared.
- HWR records are canonical, preserving the invalid-template sentinel and
  fixed 32-slot backshift. Image activation uses canonical W10DrawingInput,
  unsigned shared fields and a signed local snapshot; Resource(1,3) reaches the
  actual image initializer. Menu modes extend only the evidenced W23FlowScreen
  prefix, not a private replacement class.
- DSP settings are the **real AudioDspSettings subobject at parent +4**. Every
  prior width/offset and the volume subobject survive; all fourteen typed users
  migrate coherently, including the c011 moved helper destination. No cast to
  an AuxiliaryFilterMode shadow or changed access width is used.
- Mapped memory uses the actual writable MappedMemoryBlock and all five raw
  service inputs. The borrowed operation is not another mapping-record model;
  numeric mapping-address conversion is intentional.

The exact shared **`game/task_pool.h`** model agreed with c012-r4 remains
byte-identical (SHA-256 `fdecc7599595eac61674d0984c21cb87d742d9cac4a8cd9b068972daf404115c`):
initialized byte, padding, active list at `+4`, available list at `+0x10`.
**0x1c is only the known prefix, not an allocation extent.** Both old accepted C
prefix copies are removed; canonical PoolList/PoolLink and actual
AudioVoiceGroup.link entries are used, never a native-list alias.

**`0017e764` remains c012-owned: no active c011 definition, extra source or
credit.** Its previously held *interface* is now closed by this shared prefix,
the two accepted-client type migrations, explicit Main/peer agreement and the
historical private 44-B witness. Its body/credit remain external. This is the
one explicitly adjudicated credit-held external call, not a silently unresolved
contract-held dependency. `0017df84` remains held for the separate unresolved
queue/signal contract.

Accepted `0015477c` retains its old offsetof/byte-subtraction owner recovery;
its body is unchanged. This is separately recorded inherited attention, not a
new approval or a reason to overwrite a later ctypes1 correction. The private
archived pool witness is evidence only, never a combined matcher input.

C014 holds remain: `001ae140` (420 B, descriptor/NameTable hierarchy; deduped),
`0014aa94` (96 B, game/native Material bridge plus unmatched context), and
`0017df84` (88 B, unresolved held queue endpoint/signal family). No matching
companion is duplicated. Accepted `001f4500` retains its sole original C owner.

## Callback and source qualifications retained

The worker `0010aa28` keeps the exact **void(void*,void*)** runtime protocol,
including its unused context. The folded channel callback `00154674` implements
**void(AudioChannel*,AudioVoiceGroup*)** and invokes the nested group callback as
**void(AudioVoiceGroup*,int,void*)**. Native Signal2/Slot2 factories construct
empty canonical storage; no concrete slot provider or erased callback is added.
Compiler-generated native static lifetimes remain distinct from hand-cast C
callbacks, with their R1 provenance qualified.

Main's honest-under-specification ruling remains per row:

| Provider | Storage | Qualification |
| --- | --- | --- |
| `001d033c` | `002245ec+0x1c` | **Invoking population and protocol unrecovered; address stored only.** Existing opaque array; body/direct C helper types evidenced; no callable completion, selected table invocation, native identity or new integer/function-pointer initializer cast. |
| `001d222c` | `002246b8+0x1c` | **Invoking population and protocol unrecovered; address stored only.** Same explicit conditions; LinkedView body/helper protocol is not guessed from a virtual slot. |
| `001c8894` | `002434ec` | **Invoking population and protocol unrecovered; address stored only.** Real allocator/resource-value body, no source table completion or native provider alias. |

All previous bank/list/result/native-facade, texture, event, pool, font and audio
holds remain. The language/service-handle `__current_pc` uses remain approved
intrinsic qualifications; the language-below-32 assumption, fixed string-table
capacity, discarded history and other `ORIGINAL BUG:` behavior are preserved.
SDK/middleware profiles are byte witnesses, not historic ownership certificates.
Helper/type/TU names and incomplete dynamic populations remain `[INFERENCE]`.
No accepted family is declared recovered merely because a NEW caller is held.

## Earlier r9 complete proof and remaining Main work

The r9 **combined** proposal ran **2,671 sequential niced single-process
`tools/match.py --show 0` commands, all exit 0**; all **2,771 required functions
MATCH**, with all **24 helper-only definitions** supplied in natural contexts.
No `0017e764` body is supplied or compared. The earlier 2,661-command r9 pass
predates the c014 scope addition and is preserved as such; **5,332 actual matcher
commands total, all exit 0**. The current complete rewrite projection uses only
the final combined receipts and has `scope_complete=true`.

The requested one interfaces check was exercised as reported above. No argcheck,
full build, suite, prepare, review registration, finalize, push, subagent or task
tool ran here. Main must integrate the complete committed source, retain adopted
gates/tools, adjudicate the C++/opaque/inherited attention, run complete-rewrite
argcheck, prepare with two fresh complete builds/the suite, and obtain two
independent bound reviews. Exact current commands and immutable paths are in
`r9/REMAINING-HEAVY-COMMANDS.json`; any compiler-input or composition drift needs
new affected proof and, for another heavy run, a new Main grant.
