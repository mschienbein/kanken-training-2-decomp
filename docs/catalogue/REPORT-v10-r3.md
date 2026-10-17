# Catalogue v10-r3 — register-effect class repair

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
