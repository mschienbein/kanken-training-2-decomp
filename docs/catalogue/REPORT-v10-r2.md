# Catalogue v10-r2 — isolated general-rule guard checkpoint

## Status and scope

This checkpoint repairs the three sealed v10 general-rule soundness findings as one
isolated change, with **no library or initializer-eligibility reclassification**.
It is not adoption-ready by assertion: Main owns the clean full suite and independent
re-review. No accepted credit, canonical source/header/config/history, or frozen gate
is changed. The read-only seal is preserved at `scratch/review/catalogue-v10/`.

The supplied real image has **4,743 owners / 4,662 ordinary owners / 1,133,322 ordinary
extent B**, **360 unknown B / 36 intervals**, and **10 anomalies**. Every function
record is identical to the reviewed `7b5f857f…` proposal. The more conservative
alignment proof reopens 18 B and eight old extent issues rather than hiding them.
Zero unknown/zero anomalies is **not achieved**.

This is the guard-only checkpoint of the requested v10-r2 stack. Its CLI metadata
still carries the inherited `catalogue-v10` revision tag and original frozen v10
classification input; a separate classification commit will identify the combined
revision as `catalogue-v10-r2`. No pending library rows are silently included here.
The adopted accounting baseline remains v9-r2 `959311b`, SHA-256
`d13a8c794676c135b3bcd03676bf1803c5b5215455428ee4177ed911311c3d64`.

## Sealed findings and general-class repairs

The sealed review binds code `6c5aa76` / report `69404ee`, proposal SHA-256
`7b5f857fc160083b100440834e4e92557431962b868d03f5b8e2895c0de5e00c`.
Its counterexamples are facts and were not rerun unchanged to reconfirm them.
The review and seal are read-only; neither was edited.

### V10-R01: conditional calls invalidate caller-saved facts

`read_footprints` now invalidates r0-r3/r12/LR facts before the conditional-instruction
continue. Taken/untaken call paths therefore retain only facts they can agree on.
The same handling covers direct BL and register BLX and does not discard preserved
r4-r11 facts. Regressions exercise every tracked caller-saved data register, direct
conditional/unconditional controls, a conditional register call, and a callee-saved
positive control. The stale pointee is not classified as data.

### V10-R02: ordinary return proofs cannot be exception returns

Flag updates are rejected before terminal acceptance, including MOVS-to-PC's SPSR
restore behavior. A terminal must be an ordinary BX LR or unshifted, non-flag-setting
MOV PC,LR. Flag-setting/exception-return and shifted/flag-dependent MOV-PC variants
cannot create an ordinary owner. Regressions reach both the symbolic proof and the
consuming owner writer, with an ordinary MOV-PC return positive control.

### V10-R03: complete load classes and fail-closed negative-reference evidence

A single architectural read model replaces duplicated unsigned-only width checks.
It covers signed/unsigned byte and halfword, unprivileged variants, exclusive and
swap reads, word/doubleword, scalar VFP, all four core block address modes, VFP block
modes and implicit POP/VPOP reads. It uses old-base effective addresses for indexed/
writeback forms, correct widths and 32-bit address arithmetic. Statically resolved
constant index shifts are evaluated; unbounded indexed addresses do not supply an
ownership bound. Register shifts/RRX without the required value/flag proof fail
closed. Malformed or unmodeled load classes, including implicit loads and memory
operands whose READ flags are missing, are conservative vetoes, never invented data.

Conditional exact reads and partially mapped reads can also veto alignment without
claiming an unconditional complete object. Unknown forms/addresses can veto a broad
span; their absence from the positive read inventory is never treated as proof of
no reference. Known store-only and prefetch classes do not become memory reads merely
because access flags are absent. Generic incoming pointer aliasing is not given an
invented image address: the positive inventory is explicitly for statically recovered
image references. Veto-only evidence is recorded separately as
`closure_evidence.read_vetoes` and cannot enter the rodata ownership writer.

The consuming signed-byte/signed-halfword fixtures now retain referenced data as
unowned rodata rather than padding. Conditional/unmodeled/unbounded controls retain
unknown bytes. Unit coverage checks all modeled scalar widths, core/VFP block modes,
missing-access-flag fallback, unknown shapes, constant-index and partial-map behavior.

## Exact real-image deltas against the reviewed proposal

**Function-record deltas: none (0/4,743).** Extents, kinds, code/pools/literals, every
entry/ISA, classification evidence and protected complete records are unchanged.
F1 and F2 do not change an owner in this image; the review did not claim their
counterexample patterns occurred here.

F3's complete negative-reference accounting conservatively withdraws all nine prior
zero-halfword alignment claims. There are 263 unmodeled/unbounded static-read veto
records and 482 conditional-read veto records. The broad vetoes do not prove any
particular runtime read reaches one of these halfwords: they show why the former
proof of absence is insufficient. No new data extent or neighboring pool is borrowed.

| Interval, half-open | B | Before → guard checkpoint | Reason |
| --- | ---: | --- | --- |
| 0x00100396–0x00100398 | 2 | padding → unknown | Complete negative-reference proof is unavailable; unmodeled/unbounded static reads veto alignment, not establish data ownership. |
| 0x001005ea–0x001005ec | 2 | padding → unknown | Complete negative-reference proof is unavailable; unmodeled/unbounded static reads veto alignment, not establish data ownership. |
| 0x001007ce–0x001007d0 | 2 | padding → unknown | Complete negative-reference proof is unavailable; unmodeled/unbounded static reads veto alignment, not establish data ownership. |
| 0x001008c6–0x001008c8 | 2 | padding → unknown | Complete negative-reference proof is unavailable; unmodeled/unbounded static reads veto alignment, not establish data ownership. |
| 0x001009fe–0x00100a00 | 2 | padding → unknown | Complete negative-reference proof is unavailable; unmodeled/unbounded static reads veto alignment, not establish data ownership. |
| 0x00100ada–0x00100adc | 2 | padding → unknown | Complete negative-reference proof is unavailable; unmodeled/unbounded static reads veto alignment, not establish data ownership. |
| 0x00100c7a–0x00100c7c | 2 | padding → unknown | Complete negative-reference proof is unavailable; unmodeled/unbounded static reads veto alignment, not establish data ownership. |
| 0x0019f622–0x0019f624 | 2 | padding → unknown | Complete negative-reference proof is unavailable; unmodeled/unbounded static reads veto alignment, not establish data ownership. |
| 0x0019f696–0x0019f698 | 2 | padding → unknown | Complete negative-reference proof is unavailable; unmodeled/unbounded static reads veto alignment, not establish data ownership. |

These are every changed physical-map interval. Padding decreases **42 → 24 B**;
unknown increases **342 → 360 B**, intervals **27 → 36**. Function/data/rodata/segment
padding byte totals are unchanged. All 18 B were unknown in the adopted v9-r2 table,
so no new unknown byte lies outside that original accounting set.

Eight original unexplained-extent issues are restored (none is a new issue relative
to adopted v9-r2). No old anomaly is removed; the two prior unresolved anomalies stay.

| Owner | Restored issue |
| --- | --- |
| 0x00100360 | unexplained_function_bytes |
| 0x001005ca | unexplained_function_bytes |
| 0x0010064c | unexplained_function_bytes |
| 0x001008ac | unexplained_function_bytes |
| 0x001009de | unexplained_function_bytes |
| 0x00100bb0 | unexplained_function_bytes |
| 0x0019f5c4 | unexplained_function_bytes |
| 0x0019f63c | unexplained_function_bytes |

Owners **4,743 → 4,743**; ordinary **4,662 → 4,662**; ordinary extent B
**1,133,322 → 1,133,322**; unknown **342 → 360**; anomalies **2 → 10**.
All 4,776 retained/current entries and the 1,602 complete protected records remain
unchanged. Structural issues and protected changes remain empty. Machine-readable
before/after statistics, every range/record/anomaly delta and seal identities are
preserved in `scratch/closure/v10-r2-guards-differences.json`.

## Reproduction and exercised checks

Only focused tests were requested and run:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -m unittest tests.test_analyze.CatalogueClosureTests tests.test_analyze.CatalogueTests.test_plain_catalogue_command_includes_review_and_reproduces_all_bytes
```

**Exit 0: 24 tests, OK** (0.063 s unittest-reported). Receipt:
`scratch/closure/v10-r2-focused-final.json`. The earlier focused class-only run also
exited 0 (22 tests); it is preserved separately. An initial Eval variable-setup
NameError happened before any product/test process started; it is recorded as such,
not a failing product case. No inplace merge-conflict incident affected this worktree.
No full test suite, build, compiler/matcher, linter or formatter was run here.

Exact sequential real-image generation commands (no piped gates):

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-v10-r2-guards-final.json
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-v10-r2-guards-final-rerun.json
```

Both exit 0; complete outputs are identical, **6,691,703 B**, SHA-256
`f4d12305e04333a2ef887ca7844916fb32382fc9c89258a2468597550e4574c8`.
Captured wall times: 41.224 s / 43.809 s. Receipt:
`scratch/closure/v10-r2-guards-final-generation.json`. The first draft checkpoint and
all earlier outputs remain preserved; no saved failure or sealed evidence is erased.

Main should run the full suite on the committed clean checkout and dispatch an
independent review. This checkpoint does not claim that the sealed findings have
been independently cleared or that the residual denominator is closed.

## Complete remaining unknown intervals at this checkpoint

The original v10 report retains complete 87-original/84-adopted interval and 37-issue
accounting. The newly generated proposal recomputes those ledgers with the reopened
nine halfwords. All **36 remaining intervals / 360 B** are listed below.

| Remaining interval, half-open | B | Unresolved prerequisite |
| --- | ---: | --- |
| 0x00100020–0x00100024 | 4 | A bounded closed instruction fragment exists, but no executable-entry/caller/section witness establishes its independent identity. Return-only/tail-only or plausible closed decoding is not used to invent an entry or padding. |
| 0x00100076–0x00100078 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x0010012e–0x0010015e | 48 | A bounded closed instruction fragment exists, but no executable-entry/caller/section witness establishes its independent identity. Return-only/tail-only or plausible closed decoding is not used to invent an entry or padding. |
| 0x00100396–0x00100398 | 2 | Previously proposed zero alignment is reopened because unmodeled/unbounded static reads prevent a complete no-reference proof. No claim that an actual runtime read reaches these bytes is made. |
| 0x0010051e–0x00100520 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x001005ea–0x001005ec | 2 | Previously proposed zero alignment is reopened because unmodeled/unbounded static reads prevent a complete no-reference proof. No claim that an actual runtime read reaches these bytes is made. |
| 0x001007ce–0x001007d0 | 2 | Previously proposed zero alignment is reopened because unmodeled/unbounded static reads prevent a complete no-reference proof. No claim that an actual runtime read reaches these bytes is made. |
| 0x001008c6–0x001008c8 | 2 | Previously proposed zero alignment is reopened because unmodeled/unbounded static reads prevent a complete no-reference proof. No claim that an actual runtime read reaches these bytes is made. |
| 0x0010090a–0x0010090c | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x00100942–0x00100944 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x001009fe–0x00100a00 | 2 | Previously proposed zero alignment is reopened because unmodeled/unbounded static reads prevent a complete no-reference proof. No claim that an actual runtime read reaches these bytes is made. |
| 0x00100a6a–0x00100a6c | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x00100ada–0x00100adc | 2 | Previously proposed zero alignment is reopened because unmodeled/unbounded static reads prevent a complete no-reference proof. No claim that an actual runtime read reaches these bytes is made. |
| 0x00100b26–0x00100b28 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x00100c7a–0x00100c7c | 2 | Previously proposed zero alignment is reopened because unmodeled/unbounded static reads prevent a complete no-reference proof. No claim that an actual runtime read reaches these bytes is made. |
| 0x00100c92–0x00100c94 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x00100d70–0x00100d74 | 4 | A bounded closed instruction fragment exists, but no executable-entry/caller/section witness establishes its independent identity. Return-only/tail-only or plausible closed decoding is not used to invent an entry or padding. |
| 0x0010d3a8–0x0010d3b0 | 8 | Eight-byte sparse hole in 0x0010d2b8 after process-exit handling: a closed instruction fragment is not an evidenced independent entry, bounded data object or section-owned dead continuation. Original process-exit behavior alone does not establish lexical ownership of unreachable bytes. |
| 0x001415b4–0x001415b8 | 4 | A bounded closed instruction fragment exists, but no executable-entry/caller/section witness establishes its independent identity. Return-only/tail-only or plausible closed decoding is not used to invent an entry or padding. |
| 0x001569b0–0x00156a30 | 128 | A real indexed read uses an address base before this gap, not a directly bounded object reference. The local abstract-value probe cannot bound its index within the image; no preserved matching library object/source or complete format bound is available. Do not borrow the preceding owner or classify this as a lookup table by appearance. |
| 0x0016b6a2–0x0016b6a4 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x00181eaa–0x00181eac | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x001911ee–0x001911f0 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x00191c92–0x00191c94 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x001944ac–0x001944e8 | 60 | A real indexed read uses an address base before this gap, not a directly bounded object reference. The local abstract-value probe cannot bound its index within the image; no preserved matching library object/source or complete format bound is available. The installed compiler inventory has executable/header payloads but no matching runtime archive. Do not borrow the preceding owner or classify this as a lookup table by appearance. |
| 0x001964e8–0x00196504 | 28 | Exceptional-dispatch return-address descriptor: missing exact descriptor width/format and original 0x00145570 nonreturn/continuation contract. The retained 404 normal f2ul ABI cases exclude the exceptional path. A BL-adjacent record and plausible trailing instructions are not a closed entry/data witness. |
| 0x0019d5c2–0x0019d5c4 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x0019f622–0x0019f624 | 2 | Previously proposed zero alignment is reopened because unmodeled/unbounded static reads prevent a complete no-reference proof. No claim that an actual runtime read reaches these bytes is made. |
| 0x0019f696–0x0019f698 | 2 | Previously proposed zero alignment is reopened because unmodeled/unbounded static reads prevent a complete no-reference proof. No claim that an actual runtime read reaches these bytes is made. |
| 0x0019f6d6–0x0019f6d8 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x0019f910–0x0019f912 | 2 | A bounded closed instruction fragment exists, but no executable-entry/caller/section witness establishes its independent identity. Return-only/tail-only or plausible closed decoding is not used to invent an entry or padding. |
| 0x0019fd6a–0x0019fd6c | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x0019ffa2–0x0019ffa4 | 2 | Nonzero halfword fill: no retained section/symbol/linker-placement witness proves alignment ownership. Six retained Thumb objects have section size equal to symbol size and alignment 2; they do not establish this fill. Not guessed to be padding. |
| 0x001a00b4–0x001a00b8 | 4 | Retained ARM/Thumb CFG probes and reference census supply no complete executable-entry, bounded referenced-data/format or section-alignment witness. The legal owner/boundary remains unproved. |
| 0x001c7748–0x001c775c | 20 | Retained ARM/Thumb CFG probes and reference census supply no complete executable-entry, bounded referenced-data/format or section-alignment witness. The legal owner/boundary remains unproved. |
| 0x001cd2e6–0x001cd2e8 | 2 | Retained ARM/Thumb CFG probes and reference census supply no complete executable-entry, bounded referenced-data/format or section-alignment witness. The legal owner/boundary remains unproved. |
