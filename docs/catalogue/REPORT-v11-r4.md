# Catalogue v11-r4 — reject mode-sensitive leaf transfers

## Preserved failure

The independent review of source `f000c86be44aa2f02f5ea17ca851ea2dc9fa6724`
and proposal SHA `172ba50aef9b656fee046ed2d347923fd4a487769f69b7b99cde08a1071ce463`
sealed **FAIL V11R3-CODEX-R01**. The public synthetic writer created a 12-byte
ordinary owner from a decoder-marked user-bank transfer, with no structural
validation issue. The complete-copy certificate had treated those operands as
ordinary current-bank registers.

That failure is accepted as ground truth; the unchanged failed head was not
rerun. The sealed review, proposal, all earlier failures and r3 evidence remain
immutable. The review found no such transfer among the 42 recovered real-image
leaves; that observation is historical, not a new unchanged-image measurement.

## Corrected admission boundary

`constant_copy_leaf` now refuses `decoded.usermode` before interpreting any
ordinary leaf effect. This covers both constant-copy and fixed-record attempts,
including loads, stores and banked transfer variants that would otherwise reach
the shared multiple-register path. Ordinary current-bank transfers retain their
existing requirements: complete closed effects, exact footprint, restored stack
and preserved registers. No later endian veto is used to excuse an already
created owner.

User-bank transfer behavior depends on processor mode; the architectural rules
are not established by register widths or a plausible decoder classification.
Sources: [Arm LDM reference](https://support.arm.com/documentation/dui0801/i/A32-and-T32-Instructions/LDM)
and [LDM user-register semantics](https://mikhailarkhipov.github.io/ARM-doc/A32/ldm_u.html).
No native/emulated execution or trap is claimed.

The distinct `research/catalogue_v11_r4_evidence.json` child binds unchanged r3
input SHA `7a1a2aa5e5344061fe2d4be7a82e55a726723c295f6c065d3d5623263f2d39e4`,
the image and exact refusal policy. Public loading rejects a changed parent,
image, revision or policy; no old revision is reinterpreted. The generator and
review metadata identify r4 and retain the entire r4/r5/v11/r2/r3 ancestor chain.
R3's protected metadata transitions, provisional-entry withdrawals, runtime216B
and selector2B proofs, adopted/proposed identities and all qualifications remain
unchanged. Frozen GATES v1.19 and every accepted source/header/config/history are
untouched.

## Exercised checks

Source and fixture edits completed before these checks. Private receipts remain
under `scratch/closure/v11-r4/`:

- `focused-r1.json`: actual exit0; **54 tests** in 3.798 seconds, including public
  owner-creation negatives, ordinary-transfer positive control, current CLI
  evidence-chain behavior, protected-record consumers and tampered-policy refusal.
- `public-smoke-r1.json`: actual exit0 from a standalone `build_catalogue` API
  invocation, not a unittest wrapper or mock. The ordinary synthetic transfer
  creates its 12-byte owner. User-bank load and store cases create no owner and
  retain all 12 bytes as unknown; structural issues remain empty in all cases.

Only hand-encoded synthetic inputs were used. No original executable bytes,
disassembly or licensed library payload are emitted. The successful synthetic
checks are not full-image stability or adoption.

## Remaining acceptance work

Run the integrated full suite and one fresh current-image r4 CLI generation,
check complete conservation and current accepted/prepared/candidate compatibility,
and obtain a fresh independent full-proposal review. Do not copy r3's measured
4,741/4,579/1,123,188 totals or 178 unknown bytes as a new r4 observation before
that generation. The remaining unknowns/anomalies are not closed by this guard.
No source/data/denominator credit, Main merge, denominator record or push occurs
in this source-only repair.
