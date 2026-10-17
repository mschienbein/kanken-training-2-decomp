# c008-r2 remediation and handoff

This is an integrator remediation/pre-review record, **not acceptance or an independent review**.
Both c008 FAIL records remain immutable. No prepare/finalize, acceptance/adoption, history edit,
full build, full suite, formatter, lint or push was run by this slice.

## Baseline and proposed selection

- Branch/worktree: `c008-r2`, `scratch/worktrees/c008-r2`.
- Started at `8bf3fe821c2172b94522fb334e7a8c5aa5512173`; then fast-forwarded current main
  `959311b45dc6ba2e373fbbec83a9b4b1e42ccae0` as requested. The merge brings main-owned
  C++ permuter tooling, sealed reviews and denominator v9-r2; none was edited here.
- Main confirmed all 1,602 protected records, including every c008 row, unchanged in v9-r2.
  All **37 directly changed units were rematched after that merge**, each exit 0.
  `baseline-merge.json` and `post-merge-match-index.json` bind this handoff.
- Frozen profiles and selection remain **armlink5-inline-branchnop-v2 / none**. No R1/L1,
  new alias, changed compiler flag or denominator operation is proposed by this slice.
- `candidates.json` is the successor-only proposal: **434 new units/functions**, 112,564 B.
  It is the original candidate manifest minus the deferred 200-byte `func_0016b6a4` row.
  The dry ledger is **1,527 units / 1,601 functions / 185,952 B**; no accepted identity is lost.
  Frozen `config/candidates/c008.json`, ledgers and c008 artifacts were not rewritten.
- The complete pre-review scope is **463 units / 469 functions**: 434 new plus 29 revised
  accepted units containing 35 functions. Eight accepted units are newly reopened by r2.
  Profiles: 357 game / 36 sdk / 70 middleware; 364 C / 99 C++ units.

## Original review findings

The reviews agree on the same four blockers with different numbering:

| This record | OpenAI | Second | Disposition |
|---|---|---|---|
| F01 directory/capability identity | F01 | F02 | **Deferred to c009**, not weakened |
| F02 applet byte/result contract | F02 | F03 | Fixed, including accepted dispatcher/callee |
| F03 input owner/full-word status | F03 | F04 | Fixed using C009's retained exact source/evidence |
| F04 audio task manager owner | F04 | F01 | Fixed on canonical AudioTaskManager |

F01 has no natural witness making a U8Directory a FileInputStream or identifying a real
cleanup callee at the erased call. A cast, null argument or fabricated helper would only hide
that missing identity. The new source and its proposed declarations were removed from the
successor. Its exact historical source is preserved in `deferred/func_0016b6a4-c008.c`, and the
original diagnostic is in `retained/openai/F01-diagnostic.txt`. The accepted
`src/wave8/func_001f18c0.cpp` definition is unchanged. This row cannot re-enter a candidate
without coherent natural identity evidence and exact reproduction; no replacement call is claimed.

F02 now has one `CtrResult(unsigned, unsigned char)` endpoint. Its accepted C++ definition
includes the canonical header, and both forwarding wrappers are byte-wide. Keeping the accepted
outer dispatcher word-wide added a mask and mismatched; that real failed attempt is retained.

F03 uses `InputSessionContext *` and `int`, preserves the unused owner and forwards all status
bits. C009 supplied the exact source/comment and retained 12/12-B callee plus 16/16-B caller
proof; both are exercised again here. `func_001821f4` is a **private witness, not new credit**.
C009 was told not to carry a duplicate revision. Its later backend callback/scalar refinements
remain C009-owned; r2 owns the shared mutex-at-0x34/resetter cutover.

F04 removes the unrelated AudioWorker representation and declares the stop operation with the
canonical AudioTaskManager returned unadjusted by `func_00185de4`.

## Same-class sweep and every disposition

`probe_contracts.py` examines every original c008 new/revised unit (455 included, one explicitly
deferred) and 502 additional accepted units selected by canonical symbol overlap. It compiles
native definitions with relevant canonical headers, including existing C++ callees. C definitions
also get deliberately strict C-as-C++ body probes. `probe_signatures.py` separately derives the
actual C definition declarations from the compiler-preprocessed AST and compiles their C++
C-linkage signatures, so C-only body syntax is not mistaken for a conflicting ABI.

`sweep_forwarding.py` walks original CFGs using the existing argcheck decoder and exact symbolic
merge, propagating canonical globals, entry parameters, call results and known members. It emits
only abstract type/address/provenance facts, never original bytes or instruction text. Every one
of the 469 functions has a coverage row; 1,060 canonical function contracts and 151 globals are
indexed over the current 4,699-record catalogue. Stack spills, virtual calls, raw-storage typing,
C++ inheritance and indirect aliasing remain explicit analysis limits.

The sweep found additional real nominal/width/prototype defects, not only the reviewers' four.
Canonical representations were extended or factored into actual containing members; casts and
obsolete owner/value types were removed rather than renamed into aliases. Scalar promotions,
array decay, explicit first-member addresses, C++ base conversion and allocation-storage reuse
were adjudicated separately from true owner contradictions.

| ID | Disposition | Change/reason |
|---|---|---|
| F01 | deferred-to-c009 | Remove the unproven directory cleanup call identity from the successor rather than cast U8Directory to accepted const FileInputStream. Delete this new unit and its two proposed declarations; leave the accepted stream definition unchanged. |
| F02 | fixed | One CtrResult(unsigned,unsigned char) SDK boundary, canonical header included by its accepted C++ definition, and byte-wide forwarding wrappers all the way through the accepted dispatcher. |
| F03 | fixed | Canonical InputSessionContext* and int status, preserving unused owner and the unmodified machine word. Reuse C009 exact source/comment bytes; fresh callee and natural caller witnesses match. |
| F04 | fixed | Use the canonical AudioTaskManager returned unadjusted by func_00185de4; remove AudioWorker and declare the stop API in audio_tasks.h. |
| S01 | fixed | Canonical unsigned-byte endpoints write the object representation of signed result storage through an explicit unsigned-char view. Signed interpretation is unchanged; the nonmatching unsigned-storage alternative is retained. |
| S02 | fixed | InputSessionContext owns active at 0 and StreamMutex at 0x34. The old selected word is that mutex depth, not another owner; the lazy initializer and accepted resetter now use the same canonical type. |
| S03 | fixed | HandleManager now has its evidenced mutex prefix and 24-channel vector. Its accepted resetter uses mutex.depth; all three channel consumers use the real owner, with no duplicate owner structs. |
| S04 | fixed | MenuItem getter results stay MenuItem pointers. Expose its alternate TextTable and metadata words in the canonical representation and remove unrelated W12c item/action views and casts. |
| S05 | fixed | The forwarded data_0023ecf4 value is ItemPageSet*. Its signed variant/group bytes are now exposed in that canonical prefix, replacing the unrelated descriptor type. |
| S06 | fixed | ArchiveResourceTable is already the table member type. The helper now reads its canonical file_table_offset and the caller no longer casts it to W12cArchiveTables. |
| S07 | fixed-with-view-qualification | Material interfaces use canonical DisplayPane, including its observed material-bearing +0x13c view. Preserve DisplayVertexPane vertices at +0x150. This is not a claim about every original pane class size. |
| S08 | fixed | The common allocation/free prefix is ManagedExpHeap. HeapArena explicitly embeds it, as LockedExpHeap already does; migrate every source caller and field access to the real base member without pointer casts. |
| S09 | fixed-with-containing-prefix-qualification | SoundSubsystem explicitly contains its master AudioLinearRamp at zero; existing status bytes belong to that prefix. Per-channel AuxFloatRamp remains a distinct shorter value type. |
| S10 | fixed | The natural wrapper forwards SharedMemory**; the IPC output now has that canonical pointer-to-pointer type, and the handle output is CtrHandle*. The serialized address is converted once from its IPC word. |
| S11 | fixed | Getter results retain W12cSoundDetail and BankInstrumentInfo identities. Complete their observed relative-reference prefixes and remove the unrelated BinaryReferenceBlock view. |
| S12 | fixed | Replace the real implicit __aeabi_memcpy4 declaration with its shared void(void*,const void*,size_t) ARM EABI contract in runtime.h. The already proven binding is unchanged. |
| S13 | fixed | One file-scope opaque SelectionHintPanel tag prevents distinct prototype-scope identities in selection.h. Its descriptive role remains INFERENCE. |
| S14 | shared-implementation-reuse | Remove the duplicate private ctrIpcStaticBufferDescriptor implementation and include ctr/ipc.h. This resolves artificial canonical co-inclusion and follows the existing shared helper convention, without changing the command or bytes. |

`findings.json` lists exact functions and evidence paths for every row. The initial 59 and final
40 forwarding candidates have **99 individual adjudications** in `forwarding-adjudications.json`.
All confirmed defects in the included proposal are closed. The remaining raw candidates are
explicit member/base conversions, promotions, array decay, qualified construction/cookie uses or
an invalid byte-type inference from opaque word-loaded storage—not silent ABI passes.

The DisplayPane material-bearing extent is a **partial view**, not a recovered size of every
original pane class. Other descriptive roles, opaque owners, original class spellings, indirect
callbacks and erased call identities remain INFERENCE. Original-bug and approved ARMCC-extension
qualifications from both failed c008 reviews are retained per function in `pre-review.json`.

## Exercised checks and actual exits

All behavior checks were serialized. Every match invocation used `--show 0`, its frozen unit
profile and explicit v2/none selections. Receipts contain no original bytes or disassembly.

| Check | Observed result |
|---|---|
| Complete c008-r2 source scope before main merge | 463 sequential match commands, **469 functions MATCH**, all exits 0; `match-index.json`, `matches/` |
| Direct r2 changes after main/v9-r2 merge | **37/37 units MATCH**, all exits 0; `post-merge-match-index.json`, `post-merge-matches/` |
| G3 dry-ledger interfaces | **exit 0**, 1,527 units, 1,227 checked C, 300 explicitly unchecked C++, zero disagreements/unchecked C; 8 nonfatal policy records retained in `interfaces.json` |
| Final broad canonical probes | 1,742 commands, **all 455 included c008 native units pass; all 172 selected actual C++ units pass**. Driver exit 1 retains 77 synthetic/injected failures, each adjudicated—not suppressed |
| Actual C definition C++ C-linkage signatures | **364/364 units pass**, exit 0; `signature-probes.json` |
| Current-catalogue argcheck over all 469 required functions | Tool **exit 2** (wrapper exit 1): 11 candidates / 5 functions, all adjudicated (9 rejected, 2 qualified); 92 notes retained; 38 functions without a parsed contract and 1,055 unresolved indirect sites remain explicit |
| Final canonical C++ forwarding bridge | **exit 0**; checks the real shared headers for all repaired owner/value boundaries |
| Natural full-word status caller | **MATCH 16/16 B, exit 0**, post-merge, game/v2/none; `witnesses/func_001821f4.result.json` |
| Status callee | **MATCH 12/12 B, exit 0**; `post-merge-matches/wave14/func_001462d4.result.json` |
| Integrator pre-review | Exactly **469 rows**, all seven REVIEWING.md steps represented; 211 READY_FOR_REVIEW / 258 READY_WITH_QUALIFICATIONS; **not an independent review** |

The remaining 77 broad-probe failures are 11 artificial native-C header co-location collisions
and 66 C-as-C++ body-language failures. They are not failing included native definitions. The
separate signature probes and whole-ledger G3 discharge actual C declarations without pretending
that flexible arrays, implicit C void-pointer conversions or C guard initialization are C++ source.
`compiler-probes.json` embeds every raw compiler command/exit/diagnostic from all three runs;
`compiler-adjudications.json` adjudicates all 236 raw failures, including fixed real defects.

`argcheck-adjudications.json` covers every candidate and note. Qualified `func_00195ec4` setup
retains uncertainty about erased registration identity; exact natural mutex constructors do not
justify inventing r1/r2 mutex inputs. Core and VFP inputs are not conflated.

The included functions retain 27 v2 rewrites in 25 functions (21 branch no-ops, six return-only
call erasures). F01's constant-result rewrite is no longer proposed. No R1 helper removal, L1
rebinding or extra helper/alias credit is introduced.

### Failed attempts and fixture errors remain visible

- The initial word-wide applet dispatcher mismatched at 68/68 B after the new byte endpoint.
  The accepted dispatcher is now byte-wide and exactly matches.
- Unsigned result-storage alternatives for `func_0012fda0` (52/48 B) and `func_00186538`
  (100/96 B) mismatched. Final code keeps signed storage and uses a legal unsigned-character
  representation view; no register, volatile, asm or dead-code steering was added.
- The first signature driver stopped on an additionally revised accepted unit missing from its
  broad-probe lookup; the driver now uses that source's actual project includes as fallback.
- The first compile-only bridge omitted `game/sound_controller.h`; it was repaired by including
  the real header, not by writing a private declaration.

`failed-attempts.json` preserves all these exits, outputs and mismatch receipts. The inherited
review/C009 failures and provenance hashes are under `retained/`. No failed gate or historical
record was overwritten or retried unchanged to manufacture a pass.

## Exact directly changed unit list

These 37 units, plus the one explicitly removed/deferred source below, are the complete r2 source
delta from the starting tree. `changed-units.json` binds every function, profile and source hash.
The eight additionally revised accepted units are the sdk/sdk2 entries, the two wave11 heap
units, wave5 dispatcher, wave6 heap dispatcher and both wave9 resetters.

| Source | Frozen profile |
|---|---|
| `src/retries/func_0012349c.c` | `sdk` |
| `src/retries/func_001234e4.c` | `sdk` |
| `src/retries/func_0012f170.c` | `middleware` |
| `src/sdk/func_0012ff20.cpp` | `sdk` |
| `src/sdk2/func_0014603c.c` | `middleware` |
| `src/wave11/func_0019d068.c` | `game` |
| `src/wave11/func_0019dd00.c` | `sdk` |
| `src/wave12/func_001313f4.c` | `middleware` |
| `src/wave12/func_0014856c.c` | `game` |
| `src/wave12/func_00168e80.c` | `game` |
| `src/wave12/func_001790a8.c` | `game` |
| `src/wave12/func_001dd4f8.c` | `game` |
| `src/wave12/func_001eb688.c` | `game` |
| `src/wave12/func_001eb694.c` | `game` |
| `src/wave12/func_001f22ac.c` | `game` |
| `src/wave12/func_001f2b98.c` | `game` |
| `src/wave13/func_0014aaf4.c` | `game` |
| `src/wave13/func_0014ab30.c` | `game` |
| `src/wave13/func_00154224.c` | `middleware` |
| `src/wave13/func_0015e9dc.c` | `game` |
| `src/wave13/func_0017fda4.cpp` | `game` |
| `src/wave13/func_00181100.cpp` | `game` |
| `src/wave13/func_001838ec.c` | `game` |
| `src/wave14/func_0012fda0.c` | `sdk` |
| `src/wave14/func_001462d4.c` | `game` |
| `src/wave14/func_00154f78.c` | `middleware` |
| `src/wave14/func_00177740.c` | `game` |
| `src/wave14/func_00186538.c` | `game` |
| `src/wave14/func_001923d4.c` | `sdk` |
| `src/wave14/func_00193a6c.c` | `game` |
| `src/wave14/func_001e37e0.c` | `game` |
| `src/wave14/func_001f22c4.c` | `middleware` |
| `src/wave14/func_001f239c.c` | `middleware` |
| `src/wave5/func_00115040.c` | `game` |
| `src/wave6/func_0019dc20.c` | `game` |
| `src/wave9/func_0012f1b4.c` | `game` |
| `src/wave9/func_0013712c.c` | `game` |

Removed/deferred: `src/wave12/func_0016b6a4.c` (game, 200 B; no accepted credit removed).

### Shared header revisions (20)

- `include/game/applet.h`
- `include/game/archive.h`
- `include/game/audio_tasks.h`
- `include/game/audio_voice.h`
- `include/game/aux_effect.h`
- `include/game/display_animation.h`
- `include/game/event_route.h`
- `include/game/exp_heap.h`
- `include/game/input_runtime.h`
- `include/game/item_pages.h`
- `include/game/item_text.h`
- `include/game/material.h`
- `include/game/resource_font.h`
- `include/game/runtime.h`
- `include/game/screen.h`
- `include/game/selection.h`
- `include/game/service_runtime.h`
- `include/game/service_state.h`
- `include/game/sound_controller.h`
- `include/game/text.h`

## Evidence packaging and main handoff

Git retains the reusable probe/check scripts, all 463 pre-merge plus 37 post-merge safe JSON
match receipts, complete compiler/signature/forwarding/argcheck/interface data, pre-review rows,
failed-attempt records, retained provenance, the successor proposal and the historical deferred
source. Object files, dependency files and generated compiler fixtures remain private in the
worktree; aggregates embed their diagnostics and actual exits. No original bytes, packets,
disassembly, secrets or toolchain binaries are staged.

Main owns integrating this branch, the full suite/full builds, creating the successor candidate
configuration, preparing c008-r2 and obtaining **two fresh independent reviews**. The old c008
FAIL records cannot be reused as r2 approval. The successor proposal is ready in `candidates.json`;
all profile/model/header and previously accepted-source revisions must be bound by that fresh prepare.
