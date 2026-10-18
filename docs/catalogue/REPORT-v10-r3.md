# Catalogue v10-r3 — register-effect repair and authenticated-library proposal

## Final combined proposal: separate authenticated-library change

The isolated register-effect repair is commit `7750dfcab25110fbb60dac92d50cf09e95b19381`,
directly atop `6dd9832fcd2d0e35a40d85f37ff1c79b7075bf73`. The next discrete commit
adds only the independently reviewable runtime-library classification, evidence,
regressions and documentation described here. The guard-only historical checkpoint
and its exact reproduction remain below. Neither commit adopts a catalogue or awards
matching credit; canonical source, headers, configuration, names and history are untouched.

Final candidate: **4,743 owners; 4,594 ordinary / 1,124,038 ordinary extent B;
360 unknown B / 36 intervals; 10 unresolved anomalies**. Against both the repaired
guard checkpoint and `f4d12305…`: owners **0**, ordinary **−68 / −9,284 B**,
unknown **0 B / 0 intervals**, anomalies **0**. Relative to adopted v9-r2:
owners **+44**, ordinary **−30 / −7,416 B**, unknown **−4,048 B**, anomalies **−27**.
Topology exclusions remain separate: **25 multi-entry owners**; other non-C kinds
are **68 library, 22 runtime, 16 assembly and 18 veneer**. Initializer-owner
eligibility remains deliberately unchanged as recorded in `REPORT-v10.md`.

### Frozen r6 provenance and bounded claims

Source: `scratch/libid/proposal-504b82-r6/`, exact licensed Arm Compiler **5.04
update 2 build 82**, not 5.06. Source tool commits are `6a35ed7`, `94a72a5` and
`4707cb7`. The r6 source classification SHA is
`826978378482b01fe3be83f27765ca25ceba2f30d79f0ba330679b0842e3eacf`;
its names-proposal SHA is `d5e0ee80619ecd797775beadc566c8e7be6ad514904f2df26967dd7ced0a5502`.
R6's classifications and 87 proposed names are unchanged from r5. Its additional
bounded audit made **13,226 comparisons across 291 + 80 disjoint owners**, with
zero new hits; supported exact AAELF relocation masks include 4/9/11/96/104,
while Thumb2-only 51/54 remain refused. This is not a universal absence proof.

New tracked input `research/catalogue_v10_r3_evidence.json`, SHA-256
`74a8f68ed5125c6dbfa1c7ff57efa981c9a24ece00160af9c1a4f00daefb9c7d`, freezes the 68 source classification records unchanged,
the selected full-owner/alias/relocation qualifications for **82 authenticated
owners**, 57 prior-classification audit rows (14 authenticated), **206 protected
coincidence flags**, 87 proposed name bindings, and digest-bound source receipts.
It separately authenticates its parent v10 input and original image. Generation
requires neither the private r6 directory nor the licensed archives. No image
bytes, vendor object payloads, disassembly, keys or license data are tracked.

Admission requires exact current extent/ISA and original whole-extent hash,
authenticated complete vendor-body identity, and the selected same-object symbol
alias group. Relocation-bound identities retain their per-row binding and
natural-caller/name-anchor qualifications; historical linkage of a particular
archive member is not proved. Names supported only by equal bytes at a different
object are rejected. Duplicate identities, wrong image/extent/ISA/digest and
protected-coincidence collisions fail closed. This is function-specific evidence,
not relabeling hard C by appearance.

All **68** proposed new classifications apply (**27 ARM / 6,176 B; 41 Thumb /
3,108 B**). Exactly those 68 complete function records change, and only their
`kind` and `classification_evidence` fields change. All geometry, code/pool
boundaries, entries/ISAs, incoming evidence and every other function field are
identical. Physical ranges, anomaly records and complete closure evidence are
byte-for-byte JSON-equal to the repaired guard checkpoint. The 206 protected
coincidence records and all 14 authenticated prior non-C records are unchanged.
The four prior Thumb runtime owners at `001008ac`/36 B, `00100c80`/18 B,
`00100c9e`/10 B and `0019f6b4`/34 B gain supplemental vendor authentication only;
their complete old runtime records/provenance remain unchanged.

All **87 names remain proposals**, with **zero canonical names applied**. The
206 tiny-body coincidences (1,724 B) are flags only and remove no C credit.
The existing 692-B `__aeabi_uldivmod`, 80-B `__aeabi_f2ulz` and 136-B
`__aeabi_memcpy` do not gain complete vendor authentication. No neighboring pool
is borrowed, and a dispatcher name does not establish the `001964e8` descriptor
format, its nonreturn semantics or continuation closure.

### Exact reproduction and focused checks

From this worktree, sequential commands, each captured without a piped gate:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -m unittest tests.test_analyze.CatalogueClosureTests tests.test_analyze.CatalogueTests.test_plain_catalogue_command_includes_review_and_reproduces_all_bytes
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-v10-r3.json
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-v10-r3-rerun.json
```

Focused result: **exit 0, 34 tests, OK**, including the prior full register-write
class guards and new library identity/alias/flag negatives. Both real-image
generations exit **0**, producing identical **8,337,106-B** files, SHA-256
`846ee2e663fab4dd8129fb2d0579e4e40c552ba2fec7bcde062ee468ca03dd0c`.
Captured generation wall times: 55.922 s and 51.317 s. Receipts:
`scratch/closure/v10-r3-library-focused.json`, `v10-r3-generation.json` and
`v10-r3-library-differences.json`. The last records every changed owner and
complete before/after statistics; the guard-only ledger separately records its
exact two provenance-value changes against `f4d12305…`. Remaining differences
are version/source/input provenance and the explicit library review evidence.
No full suite, build, compiler/matcher, linter or formatter was run here; Main
performs clean-checkout verification and a fresh independent review of the stack.

### Every newly classified owner

Each row changes ordinary → library, by the bounded complete-body evidence above;
no extent or entry changes. Alias qualifications and full source digests are in
the frozen metadata and generated `revision_review.library_identification`.

| Address | ISA | Extent B | Selected vendor symbol |
| --- | --- | ---: | --- |
| `00100048` | THUMB | 30 | `toupper` |
| `0010010c` | THUMB | 8 | `_n$strtok_r` |
| `0010015e` | THUMB | 168 | `_strtoul` |
| `00100206` | THUMB | 138 | `__strtoll` |
| `00100290` | THUMB | 124 | `__strtoull` |
| `0010039c` | THUMB | 68 | `_wcrtomb` |
| `001003e0` | THUMB | 72 | `__strtok_internal` |
| `00100428` | THUMB | 30 | `_chval` |
| `00100446` | THUMB | 216 | `_strtoull` |
| `00100528` | THUMB | 88 | `_printf_hex_common` |
| `00100580` | THUMB | 28 | `_printf_int_hex$int$ll$ptr` |
| `0010059c` | THUMB | 10 | `_printf_ll_hex$int$ll$ptr` |
| `001005ca` | THUMB | 38 | `_printf_char_common` |
| `001005f0` | THUMB | 10 | `_sputc` |
| `0010060a` | THUMB | 34 | `strcspn` |
| `0010062c` | THUMB | 32 | `strspn` |
| `0010064c` | THUMB | 392 | `__printf$flags$sizespec$widthprec` |
| `001007d4` | THUMB | 36 | `strncat` |
| `001007f8` | THUMB | 18 | `mbsinit` |
| `00100884` | THUMB | 40 | `wcsncmp` |
| `001008d0` | THUMB | 12 | `towlower` |
| `001008e4` | THUMB | 38 | `_printf_charcount` |
| `00100944` | THUMB | 144 | `_printf_mbtowc` |
| `001009de` | THUMB | 38 | `_printf_wchar_common` |
| `00100a14` | THUMB | 86 | `_printf_wc` |
| `00100a74` | THUMB | 68 | `_printf_longlong_oct$int$ll` |
| `00100ab8` | THUMB | 24 | `_printf_int_oct$ll` |
| `00100b28` | THUMB | 136 | `_printf_fp_infnan` |
| `00100bb0` | THUMB | 208 | `_btod_etento` |
| `001180b8` | ARM | 324 | `__mathlib_rredf2` |
| `00134e84` | ARM | 128 | `__aeabi_memmove4` |
| `001415b8` | ARM | 24 | `__aeabi_lmul` |
| `00145570` | ARM | 160 | `__fpl_fnaninf` |
| `0014a860` | ARM | 296 | `_frem` |
| `0014f6b0` | ARM | 44 | `__ARM_fpclassifyf` |
| `00155208` | ARM | 1888 | `__hardfp_powf` |
| `00155ddc` | ARM | 16 | `__aeabi_f2uiz` |
| `001565c8` | ARM | 44 | `__rt_udiv10` |
| `00156b0c` | ARM | 48 | `_btod_ediv` |
| `00156b3c` | ARM | 612 | `__btod_mult_common` |
| `0016b678` | THUMB | 42 | `wcscasecmp` |
| `00181e84` | ARM | 16 | `__mathlib_flt_underflow` |
| `00181e9e` | THUMB | 12 | `__set_errno` |
| `00182cc4` | ARM | 216 | `__hardfp_fmodf` |
| `00186a6c` | ARM | 144 | `__hardfp_ceilf` |
| `0018c66c` | ARM | 104 | `strcpy` |
| `0018f7f4` | ARM | 108 | `strncpy` |
| `0018f860` | ARM | 240 | `strncmp` |
| `0018fe60` | THUMB | 112 | `strtol` |
| `00192720` | ARM | 116 | `__aeabi_memcpy4` |
| `00192b78` | ARM | 96 | `strlen` |
| `00195be4` | ARM | 244 | `strcmp` |
| `0019d074` | ARM | 376 | `__hardfp_cosf` |
| `0019d1ec` | ARM | 456 | `__hardfp_sinf` |
| `0019de40` | ARM | 200 | `strlcpy` |
| `0019f5c4` | THUMB | 112 | `_printf_longlong_dec` |
| `0019f63c` | THUMB | 108 | `_printf_int_dec` |
| `0019fd20` | ARM | 48 | `_btod_emul` |
| `0019fd50` | THUMB | 26 | `tolower` |
| `0019fd6c` | ARM | 152 | `_ll_udiv10` |
| `0019fe78` | THUMB | 44 | `_printf_pre_padding` |
| `0019fea4` | THUMB | 34 | `_printf_post_padding` |
| `0019fec6` | THUMB | 22 | `_printf_truncate_signed` |
| `0019fedc` | THUMB | 22 | `_printf_truncate_unsigned` |
| `0019fef2` | THUMB | 176 | `_printf_int_common` |
| `001a0098` | ARM | 12 | `__aeabi_errno_addr` |
| `001a00b8` | THUMB | 64 | `_mbrtowc` |
| `001f4f6c` | ARM | 64 | `__ARM_fpclassify` |

### Still not closure

**360 unknown B and 10 unresolved anomalies remain.** The complete 36-interval
and per-old-range/anomaly accounting remains in `REPORT-v10-r2.md` and the
machine-readable inherited ledger. No new ownership proof is asserted by this
classification change. Outstanding requirements remain bounded indexed-data
extents, exceptional-dispatch format/nonreturn/continuation evidence, the
process-exit continuation, and evidenced ownership for unreferenced/all-ones/zero
alignment fragments. The nine alignment withdrawals and eight restored anomalies
remain intact. The original v10 and sealed review artifacts are preserved.

## Guard checkpoint status

This discrete repair is directly atop `6dd9832fcd2d0e35a40d85f37ff1c79b7075bf73`.
It addresses sealed V10R2-R01 and the general register-write class, not only the
reported SVC opcode. Runtime-library classifications remain a separate change;
no initializer eligibility, accepted credit, canonical source/header/config/history,
or frozen validation gate is changed by this checkpoint.

The real-image proposal has **4,743 owners**, **4,662 ordinary owners / 1,133,322
ordinary extent B**, **360 unknown B / 36 intervals**, and **10 anomalies**.
Zero unknown/zero anomalies remains unachieved. Main owns full-suite verification
and a fresh independent review. This report does not declare the sealed finding
independently cleared or the proposal adoption-ready.

## Sealed input and actual cause

The read-only review at `scratch/review/catalogue-v10-r2/REVIEW.md` and `SEAL.json`
binds the prior proposal SHA-256
`f4d12305e04333a2ef887ca7844916fb32382fc9c89258a2468597550e4574c8`.
Its reported SVC counterexample is a fact and was not rerun unchanged to reconfirm it.
The review/SEAL files were not edited.

The earlier reader used only Capstone's aggregate `regs_access()` writes, plus a
manual BL/BLX invalidation. That is not a complete destination inventory: a returning
SVC is not decoded as an ordinary call, and raw service result registers are absent
from the aggregate write list. Capstone also omits both LDREXD outputs and marks
MRC/MRRC output operands as READ or zero in the synthetic metadata diagnosis.
Consequently a later read could consume a stale image-pointer fact even though its
base register had been overwritten.

The raw contract is declared in `include/ctr/svc.h:4-19,48-66`, notably line 62:
svcGetSystemTick returns a 64-bit result in r0/r1, not through an output pointer.
The same header identifies the kernel/ABI sources. This repair conservatively uses
the full r0-r3/r12/LR core call-clobber set for every returning SVC; it does not claim
that each individual service actually overwrites every member of that set. No native
kernel execution or universal syscall-model proof is claimed.

## Class-level register transfer rules

`_register_writes` now centralizes destination invalidation before the conditional
instruction exit and before any subsequent address fact can be consumed:

- Union the decoder's `regs_write` implicit effects, aggregate writes, and explicit
  operand WRITE flags; none is silently substituted for the others.
- Model both outputs of long multiply/accumulate pairs and LDRD/LDREXD, full LDM/VLDM
  and POP/VPOP register lists, and scalar/exclusive/swap outputs. Missing access flags
  do not make those destination registers read-only.
- Model MRC/MRRC/MRS core outputs explicitly, including the second MRRC destination.
- Invalidate every writeback base and implicit stack update. A destination that is
  also an addressing base is invalidated like any other destination.
- Model ordinary single-destination arithmetic and read-only comparisons/stores/
  hints without unnecessarily discarding unrelated preserved register facts.
- For an unmodeled instruction, invalidate every explicit register operand together
  with decoded implicit writes. If no trustworthy destination inventory exists,
  discard every tracked fact. No omitted WRITE flag preserves a possible destination.
- Model BL/BLX and SVC ABI clobbers uniformly, including conditional forms. Callee-
  saved facts survive only when no instruction effect says they were overwritten.
- Reintroduce values only through the existing explicit constant/address operations.
  A shifted MOV operand is not treated as an unshifted register copy.

Read addresses for the current instruction are evaluated from the pre-write snapshot,
so an actual load's old-base access is still represented correctly. Its destinations
and writeback base cannot then supply the old value to a later load. Veto-only read
inventory still never creates data ownership.

## Synthetic evidence and focused verification

The exact SVC fail-before evidence remains the sealed review's
`remaining_probe.py` / `remaining.json`. Its modeled zero r0/r1 result is synthetic
execution, not native kernel execution. New regressions cover conditional and
unconditional SVC across all five caller-saved data registers, MRC/MRRC/MRS,
UMULL/SMULL pairs, LDRD/LDM writeback lists, LDREX/SWP, explicit/implicit writes,
unknown destination effects, stack updates, and a write to the current load base.
Prior BL/BLX, ordinary-return, signed-read, mapping and alignment guards remain covered.

A new actual multi-destination counterexample loads a pointer into r1, executes
LDREXD r0/r1, then uses r1 for a signed-halfword read. Capstone 5's empty write list
formerly allowed two never-read bytes to become rodata. The newly authored regression
was run once against the exact pre-fix producer and failed as requested. A shifted-
move control in the same invocation already passed before the fix; it is not reported
as an additional demonstrated failure. Receipt:
`scratch/closure/v10-r3-fail-before.json` — **exit 1, two tests, one failure**.
The separate public-instruction metadata diagnosis is
`scratch/closure/v10-r3-write-diagnosis.json` (exit 0). No original instruction bytes
or disassembly are part of these synthetic fixtures.

After the repair, the exact focused command was:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -m unittest tests.test_analyze.CatalogueClosureTests tests.test_analyze.CatalogueTests.test_plain_catalogue_command_includes_review_and_reproduces_all_bytes
```

**Exit 0: 31 tests, OK**, 0.068 s unittest-reported. Receipt:
`scratch/closure/v10-r3-focused-final.json`. No full suite, build, compiler/matcher,
linter or formatter was run. Main performs clean-checkout combined verification.

## Exact real-image reproduction and every delta

From `scratch/worktrees/catalogue-v10`, sequential commands with captured exits
(no piped gates):

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-v10-r3-guards-final.json
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out scratch/analyze/catalogue-v10-r3-guards-final-rerun.json
```

Both exit **0**, producing byte-identical **6,691,703-B** outputs, SHA-256
`7176331fa60344e02fd537a02b91c2d2d6c038d3ccbb08dc07c5ef3e9a100fcd`.
Captured wall times are 54.720 s and 59.243 s. Receipt:
`scratch/closure/v10-r3-guards-final-generation.json`.

The complete recursive comparison against `f4d12305…` finds exactly two changed
JSON values:

1. `generator.source_sha256.tools/catalogue_closure.py`
2. `revision_review.input_sha256.tools/catalogue_closure.py`

**Function-record deltas: zero. Physical-range deltas: zero. Anomaly deltas: zero.
Statistics and complete closure evidence, including accepted reads and veto records,
are identical.** Every prior entry/ISA and complete protected record remains unchanged.
There is no occurrence-based claim about the sealed synthetic pattern in this image.
The new overall digest changes only because keeping the old generator hash would
misstate provenance. Full comparison and read-only review hashes:
`scratch/closure/v10-r3-guards-final-differences.json`.

The nine earlier alignment withdrawals, eight restored old extent anomalies,
36 remaining intervals and original/adopted accounting remain exactly as documented
in `REPORT-v10-r2.md`; original v10 evidence and reports are preserved unchanged.
This guard-only checkpoint intentionally retains the inherited catalogue-v10 CLI
revision tag until the separately requested classification stack is incorporated;
its exact source/output hashes, not an implied adoption, identify this repair.
