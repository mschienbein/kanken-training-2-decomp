# Catalogue v10-r4 — raw kernel ABI repair and bounded r7 identities

## Final two-commit proposal

Stack: `2116ead` → raw-SVC repair
**`6f8a5bae101222cded2e17789fefd055220c987a`** → separate bounded r7
classification change. The old commits are immutable. This report preserves the
isolated SVC checkpoint below; the present section records only the separate r7
extension and combined verification. No adoption or accepted matching credit is
authorized. Names go through checkpoint integration, not canonical catalogue edits.

Final proposal: **4,743 owners; 4,582 ordinary / 1,123,224 ordinary extent B;
360 unknown B / 36 intervals; 10 unresolved anomalies**. Kinds remain separate:
**80 library, 22 runtime, 16 assembly, 25 multi-entry and 18 veneer** besides the
ordinary owners. Topology exclusions are not counted as library/assembly ownership.

| Change | Owners | Ordinary owners | Ordinary extent B | Unknown B / intervals | Anomalies |
| --- | ---: | ---: | ---: | ---: | ---: |
| SVC repair vs `d1af9a71…` | 0 | 0 | 0 | 0 / 0 | 0 |
| r7 extension vs SVC checkpoint | 0 | −12 | −814 | 0 / 0 | 0 |
| Combined vs `d1af9a71…` or `846ee2e6…` | 0 | −12 | −814 | 0 / 0 | 0 |
| Combined vs adopted v9-r2 | +44 | −42 | −8,230 | −4,048 / −48 | −27 |

### R7 frozen evidence and unchanged-extent admission

Source research commit **`c80a50e37e74eb1d50bc0b9e9500d7a7fb8f0663`**,
`scratch/libid/proposal-504b82-r7/handoff.json`. Exact licensed Arm Compiler
**5.04 update 2 build 82** only; no 5.06 input or new linker/admission rule.
The r7 80-record proposal SHA is
`594ea66b0dc97076fdf853fedf5d96b34c3a6ba27d4ec0842e58af6ac9634f0b`.
Its old 68 records are identical to r6; the **12 new records / 814 B** are copied
unchanged, not synthesized from semantic similarity.

New tracked child `research/catalogue_v10_r4_evidence.json`, SHA-256
`4db813af5c65a7018633080901fa3d556afcd1a1ce577fae93001eea5a83879e`, authenticates its untouched r3
parent SHA `74a8f68ed5125c6dbfa1c7ff57efa981c9a24ece00160af9c1a4f00daefb9c7d`
and original image. It freezes the 12 records, compact actual same-object owner
proofs, all 26 bounded decisions, final declared-extent adjudication and **32
verified source-file digests**. Nine new owners retain selected real-object aliases
from r7; the three pool-qualified rows normalize only their single actual STT_FUNC
declaration, not aliases from equal bytes at different objects. Prior r3 evidence,
82 authenticated-owner proof rows, 206 protected flags and 14 supplemental prior
non-C authentications remain intact. The combined authenticated set is **94 owners /
11,214 B**; classification is **80 / 10,098 B** (27 ARM / 6,176 B and 53 Thumb /
3,922 B). The difference is previously nonordinary authentication, not new credit.

The loader rejects declared-size growth, non-function declarations, wrong-image
adjudications, failed full-body equality, interior foreign-tail claims and any
attempt to classify a row explicitly unresolved in r7. Existing complete
image/extent/ISA/original-hash, selected-symbol, duplicate and protection checks
remain in force. No private author directory or licensed archive is scanned by
the generator. Archive payloads, original instructions and license data remain
private; frozen metadata contains digests/addresses/qualified identities only.

| New owner | Extent B | Selected vendor identity | Bounded basis |
| --- | ---: | --- | --- |
| `00100066` | 8 | `strtoll` | bounded natural-caller/ABI identity |
| `0010006e` | 8 | `strtoull` | bounded natural-caller/ABI identity |
| `001005a6` | 22 | `_printf_hex_ptr$int$ll$ptr` | unchanged declaration; foreign tail excluded |
| `0010082e` | 14 | `_is_digit` | bounded natural-caller/ABI identity |
| `0010083c` | 72 | `__c89vswprintf` | bounded natural-caller/ABI identity |
| `0010092a` | 16 | `_printf_char` | bounded natural-caller/ABI identity |
| `0010093a` | 8 | `_printf_string` | bounded natural-caller/ABI identity |
| `00100ad0` | 10 | `_printf_ll_oct$int` | unchanged declaration; foreign tail excluded |
| `00100ae4` | 12 | `__rt_SIGPVFN$formal` | bounded natural-caller/ABI identity |
| `00100b0e` | 16 | `_printf_wchar` | bounded natural-caller/ABI identity |
| `00100b1e` | 8 | `_printf_wstring` | bounded natural-caller/ABI identity |
| `0019faa8` | 620 | `_printf_fp_dec_real` | unchanged declaration; foreign tail excluded |

The three declared extents stay exactly **22 / 10 / 620 B**, never 26 / 20 / 632.
The final `scratch/libid/followup-r7/extent-adjudication.json` rejects the suggested
growth and supersedes, without deleting, the provisional extent-corrections file.
All **eight foreign reference sites** remain recorded, including both real
`_fp_digits` continuations and the two address-forming sites (not counted as loads).
The extra **26 B** are not assigned to these functions or counted as non-C extent.
The declaration proof has no PC-addressed data dependence of its own. Existing
physical ownership, shared/unowned data and outside-literal anomalies are unchanged.
The existing REPORT-v2 / REVIEW-v9-r2 shared-tail policy is not relaxed.

R7's recorded bounded declared-body ambiguity audit covers 248 exact-library
archives: **7,403 new comparisons, three reused, 42 masked hits / 22 viable variants**,
20 target contradictions refused and zero conflicting viable body/mask families.
Four Thumb2-only relocation refusals and three structurally non-whole declarations
remain refusals. These are inherited source receipts, **not a scan rerun here**.
Exact re-encoding still does not establish a historical archive variant or erase
explicit inferred-binding/caller limitations. The signed/unsigned wrapper and
wide API caller match evidence is likewise reused, never recompiled here.

### Ten identities remain deliberately unresolved

| Owner | Extent B | Preserved missing-proof class |
| --- | ---: | --- |
| `0010011c` | 18 | exact current extent |
| `00100360` | 60 | exact current extent |
| `0010080a` | 36 | thumb call slot difference |
| `00100914` | 22 | thumb call slot difference |
| `00100a04` | 16 | structural unresolved |
| `00100af8` | 22 | thumb call slot difference |
| `00100c9c` | 2 | exact current extent |
| `0018fd68` | 80 | thumb call slot difference |
| `0019f9a4` | 6 | structural unresolved |
| `0019fe04` | 116 | thumb call slot difference |

The 18-B allocator-dependent body lacks a proved vendor malloc binding; the 60-B
snprintf family lacks a historical name anchor despite exact-family bytes and a
preserved 192/208-B natural-caller mismatch; the 2-B return lacks any natural
identity anchor. Five changed Thumb call slots still require a reviewed Thumb BL
inline model and retain their real exit-2 `_mbdata`/multi-function input refusals.
The `_snputwc` mismatch and 6-B cross-owner bridge remain unresolved. All ten
complete current function records are identical to the adopted baseline, as
asserted by the requested original-image module test. No unsupported rule or
placeholder retarget was added.

The derived unresolved Thumb queue follows the same reviewed owner-based rule:
**20 → 11 rows**, removing exactly nine newly classified-owner rows. All eleven
retained rows are unchanged: nine ordinary and the two preserved topology rows.
The historical 56-row source remains immutable; **45 classified / 11 unresolved**
is asserted as an exact address-set complement, not a lowered standalone count.

### Names remain outside this integration

R7's **100 proposed name bindings (+13)** remain in its separate names-proposal,
SHA `07b7c1b0af6acb385bb3758417de80c22bb9077f4bf8903359085a9fb42ab45b`,
for checkpoint integration and independent names review. The child embeds **zero
new name bindings** and applies **zero canonical names**. The earlier 87 review-only
bindings stay in their frozen r3 evidence unchanged; no new alias is smuggled in
through the classification extension. The 206 protected coincidences remain
unauthenticated flags and remove no accepted or candidate C credit.

### Requested verification and exact reproduction

After both changes, from `scratch/worktrees/catalogue-v10`, without piped gates:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -m unittest tests.test_analyze
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-v10-r4.json
```

**Module exit 0: 197 tests, OK**, 176.067 s unittest-reported, including all four
sealed QueryMemory regressions, 720 extra raw-service/predicate cases, ordinary
call-preservation controls, declared-bound negatives, both mandatory frozen-child
fixture inputs, complete protected records, original-image conservation and the
exact classified-residue complement. The original-image stability test generates
twice and asserts byte-identical complete outputs. No full suite, build,
compiler/matcher, linter or formatter was run here.

Final independent CLI reproduction exits **0**, captured wall **57.442 s**, with
**8,591,334 B**, SHA-256:
**`dacef0040a70f33bf8ec9c2f2a427a02624dcd55e960ea0844c1717321294bda`**.
Receipts: `scratch/closure/v10-r4-test-analyze.json` and `v10-r4-generation.json`.

### Exact per-commit accounting

The isolated SVC proposal SHA is
`1e9a8c789e18d10730cb56fd4f0f3a5f66e3525545981f1090fccbb7492de498`.
Its ledger `scratch/closure/v10-r4-svc-differences.json` records all eight changed
JSON paths versus `d1af9a71…` and the extra pre-existing residue/provenance changes
versus `846ee2e6…`. It changes **zero function records or physical intervals**.

The r7 ledger `scratch/closure/v10-r4-r7-differences.json` lists every recursive
changed JSON path versus that SVC checkpoint, `d1af9a71…` and `846ee2e6…`, every
changed owner/size/ISA/symbol and full before/after statistics. Exactly **12
function records change only `kind` and `classification_evidence`**. The old 68
library records, 82 authentication proof rows, protected coincidence records and
all ten unresolved owners are preserved. Every entry/ISA, code/pool/literal span,
complete physical range, anomaly, **all 84 adopted-old-range partitions and all 37
old anomaly dispositions**, and complete closure evidence including the new raw
SVC audit, are identical between the SVC and r7 checkpoints. **Physical interval
differences: none**, against either requested old digest. The new SHA reflects
classification/review/provenance and derived residue metadata, not byte ownership.

The remaining **360 unknown B / 36 intervals and ten anomalies** are not hidden
or relabeled as library/padding. Indexed-data bounds, exceptional-dispatch
format/nonreturn/continuation, process-exit continuation and evidence for remaining
alignment/unreferenced fragments are still missing. Initializer eligibility stays
unchanged. Main runs the clean full suite and fresh independent review of the
new delta with a no-regression check of the prior stack; this report is not an
adoption or independent-clearance declaration.

## Isolated raw-SVC checkpoint

This discrete change is directly atop `2116ead2558c7c7d9e8e1235e367a99d97b6df80`.
All earlier commits, frozen evidence, proposals and sealed reviews remain intact.
The separately requested r7 library update follows in a different commit; no r7
classification or extent adjustment is part of this checkpoint. Canonical sources,
headers, config, names, history, gates and accepted credit are untouched.

The sealed finding V10R3-R01 is accepted as fact, not rerun unchanged before the
fix. Review inputs: `scratch/review/catalogue-v10-r3/REVIEW.md`, `SEAL.json`,
`svc_outputs_probe.py` and `svc-outputs.json`. The public hand-encoded four callers
are copied into negative regressions; no original instruction bytes or disassembly
are included. The earlier AAPCS set omitted raw QueryMemory outputs in r4/r5.
Ordinary-call callee-saved preservation is not a raw-kernel output contract.

## General service rule and public sources

The ordinary BL/BLX set stays separate and retains r4-r7 facts. Raw SVC effects
are selected only from a single statically decoded, in-range immediate. The table
below documents complete raw result registers, not pointer-output wrapper types.
Every listed complete contract conservatively invalidates **r0-r7, IP, LR and
CPSR/APSR/NZCV**, unioned with decoder writes and documented outputs. This is a
conservative analysis policy, not a claim that each service overwrites all of them.
A missing/dynamic/malformed number, unlisted service or incomplete output contract
**discards every address fact**, including r8-r11; no ordinary-call preservation
assumption fills the gap. Break is explicitly incomplete and uses that refusal.
Invalidation precedes the conditional-instruction exit, so an unknown predicate
cannot retain a potentially overwritten address. No flag value is propagated or
used to prove a predicate. No global syscall CFG/nonreturn rule was broadened.

Cited sources:

- [3dbrew Kernel ABI, pinned revision 23691](https://www.3dbrew.org/w/index.php?title=Kernel_ABI&oldid=23691#System_calls): consecutive raw outputs and QueryMemory r0-r5, including state/page flags.
- [3dbrew SVC](https://www.3dbrew.org/wiki/SVC#System_calls): service identities and the explicit warning that userland pointer-output signatures are not the raw ABI.
- [libctru raw marshalling, commit cb9d682f652e210824d5ccb3a94468dba207978e](https://github.com/devkitPro/libctru/blob/cb9d682f652e210824d5ccb3a94468dba207978e/libctru/source/svc.s): QueryMemory saves/restores r4-r6 and marshals raw r1-r5; 64-bit Info wrappers marshal r1/r2; CreateMemoryBlock marshals r1. These source implementations corroborate the register table, not historical linkage to this image.
- Existing `include/ctr/svc.h:4-19,48-66` independently distinguishes raw ABIs, including ARMCC's r0-r3 limitations. It is unchanged.

## Reproducible service audit

Generation inventories every decoded instruction within every current function's
code ranges, without inferring executable instructions from unknown/data bytes.
There are **293 owned SVC sites / 25 service numbers**. The three adjacent Info
contracts with zero sites are covered for adversarial/synthetic callers too.
The generated `closure_evidence.raw_svc_sites` records owner/site/ISA, immediate,
condition, documented outputs and applied invalidation; `revision_review.raw_svc_contracts`
records the cited table and policy. No original opcode or payload is serialized.

| SVC | Service | Raw outputs | Owned sites |
| --- | --- | --- | ---: |
| 01 | ControlMemory | r0-r1 | 1 |
| 02 | QueryMemory | r0-r5 | 1 |
| 03 | ExitProcess | none; nonreturning | 3 |
| 08 | CreateThread | r0-r1 | 1 |
| 09 | ExitThread | none; nonreturning | 1 |
| 0a | SleepThread | none | 5 |
| 14 | ReleaseMutex | r0 | 1 |
| 17 | CreateEvent | r0-r1 | 1 |
| 18 | SignalEvent | r0 | 5 |
| 19 | ClearEvent | r0 | 5 |
| 1e | CreateMemoryBlock | r0-r1 | 1 |
| 1f | MapMemoryBlock | r0 | 3 |
| 20 | UnmapMemoryBlock | r0 | 2 |
| 21 | CreateAddressArbiter | r0-r1 | 1 |
| 22 | ArbitrateAddress | r0 | 1 |
| 23 | CloseHandle | r0 | 91 |
| 24 | WaitSynchronization1 | r0 | 23 |
| 25 | WaitSynchronizationN | r0-r1 | 1 |
| 27 | DuplicateHandle | r0-r1 | 1 |
| 28 | GetSystemTick | r0-r1 | 23 |
| 29 | GetHandleInfo | r0-r2 | 0 |
| 2a | GetSystemInfo | r0-r2 | 0 |
| 2b | GetProcessInfo | r0-r2 | 1 |
| 2c | GetThreadInfo | r0-r2 | 0 |
| 2d | ConnectToPort | r0-r1 | 1 |
| 32 | SendSyncRequest | r0 | 118 |
| 35 | GetProcessId | r0-r1 | 1 |
| 3c | Break | incomplete; all address facts refused | 1 |

ControlMemory/CreateThread/ArbitrateAddress/WaitSynchronizationN also use raw
r4/r5 inputs. Inputs are not mistaken for preserved outputs: the conservative
r0-r7 rule covers them regardless. GetSystemInfo is absent from this image's
owned code, not silently omitted from the audited rule. ExitThread's documented
nonreturn is not used to add a new CFG terminal in this repair.

## Regressions and exercised generation

Added regressions retain the review's four ABI-preserving QueryMemory callers
(r4/r5, unconditional/taken NE), checking the actual range writer and structural
validation. Additional cases cover CreateMemoryBlock/GetSystemInfo/GetProcessInfo,
Break and an unlisted service across all 15 ordinary predicates: **720 synthetic
service/register/predicate combinations**, including r8-r11 for unknown contracts.
The complete table's register writes/flag invalidation and malformed/dynamic
service refusal are checked directly. Eight ordinary BL controls preserve r4-r7,
including conditional calls. These are synthetic analysis cases, not native
kernel execution; no new behavior-emulation claim is made.

Per Main's sequencing instruction, `tests.test_analyze` is run after both discrete
changes land, not mid-stack. This checkpoint's real-image reproduction is:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-v10-r4-svc.json
```

**Exit 0**, 56.458 s captured wall time, **8,380,562 B**, SHA-256:
`1e9a8c789e18d10730cb56fd4f0f3a5f66e3525545981f1090fccbb7492de498`.
Receipt: `scratch/closure/v10-r4-svc-generation.json`.

Against `d1af9a71e983fbe06867e972875e8dfc9d5f735d9e89f695e78892f267f69d5c`,
exactly eight JSON paths change: the two source hashes for each of analyze.py and
catalogue_closure.py, generator/review revision tags, the SVC site inventory and
the cited service-contract table. **All function records, physical ranges,
statistics, anomalies and complete old-interval/anomaly accounting are identical**.
Against `846ee2e6…`, the already committed 56-to-20 residue correction and its
catalogue_review.py provenance hashes also differ. The full recursive path lists
and explicit zero interval differences are in
`scratch/closure/v10-r4-svc-differences.json`; the earlier residue ledger identifies
every one of the 36 classified-owner rows removed. No prior evidence is replaced.

Checkpoint totals remain **4,743 owners; 4,594 ordinary / 1,124,038 ordinary extent B;
360 unknown B / 36 intervals; 10 unresolved anomalies**. The exact per-old-range
and anomaly accounting is inherited unchanged from the reviewed r3 proposal.
No unknown interval is closed and no ownership is reclassified by this guard fix.
Full closure remains unachieved; Main owns full-suite verification, fresh
independent review and any adoption.
