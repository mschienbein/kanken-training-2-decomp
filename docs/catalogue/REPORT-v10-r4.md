# Catalogue v10-r4 — raw kernel ABI repair

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
