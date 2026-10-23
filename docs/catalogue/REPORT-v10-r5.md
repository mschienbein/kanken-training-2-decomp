# Catalogue v10-r5 — data-endian pool-value admission repair

## Scope and immutable failed finding

This is a source-only successor to **3de76111a448dfaca260fab5630762edb27feb0c**
(`catalogue-v10-r4`), not adoption of that sealed-failed proposal. The restart
review at `scratch/review/catalogue-v10-r4/restart-20261020` sealed **FAIL** for
**V10R4-R01**. Its REVIEW, REPORT, REVIEW.json, SEAL.json, endian.json and exact
hand-encoded synthetic probe were read before editing. The finding is accepted
as ground truth; the unchanged failed head was not rerun.

The failed proposal SHA-256 is
`dacef0040a70f33bf8ec9c2f2a427a02624dcd55e960ea0844c1717321294bda`.
The sealed review demonstrates an ABI-preserving caller entering SETEND BE,
loading a pool pointer, reading a byte, restoring LE and returning. The old
reader recreates an unconditional little-endian fact after clearing prior facts,
and the actual writer falsely changes **[0x0010002c,0x0010002d)** to owner-null
rodata. Synthetic execution instead reads **0x2c001000**; exact `959311b` retains
the supposed target as unknown. Structural validation alone did not catch it.
All r4 and earlier failed/sealed records remain immutable.

## Repair boundary

Pool-derived values now require an established **reachable, all-incoming-path
little-endian state before the load**. A narrow processor-effect certificate
validates ordinary encodings independently of missing decoder register writes,
flags or groups. Exact SETEND encodings establish LE/BE; conditional changes join
both outcomes. Unsupported processor/status/coprocessor changes, exception
restores, raw SVC, computed/switch transfers, Thumb IT and incomplete decoding
refuse this owner-state certificate, even in an unreachable owned suffix.
Restoring LE cannot erase an opaque proof obligation.

Only primary public entries carry the existing target LE ABI obligation. Secondary
labels start unknown. Reachable direct incoming, back and cross-owner edges join
state rather than resetting an entry to LE. Ordinary calls retain LE only from a
proved LE call entry; an unknown/BE call is not an invented restoration. Explicit
SETEND LE before a *new* load can restore known state, but never retroactively
validate a previously refused pool value. Register values still do not flow through
joins; every incoming direct target resets local register facts.

Thumb literal decoding records the pool location/register instead of eagerly
reading its contents. Both read-footprint and adjacent Thumb-PC address creation
use the same LE certificate before interpreting the word. Existing pointer/memory
bounds remain; a pool fact also requires the actual aligned PC-relative load, not
a rounded or writeback pool address. Immediate constants and direct PC byte-read
addresses do not depend on data endianness. Refused pool values emit veto-only
uncertainty so their missing pointee proof cannot become a no-read alignment proof.

This is deliberately conservative, not a broader architectural/control-flow proof,
interprocedural recovery, gate revision or linker-model extension. Raw-SVC output
register invalidation and ordinary callee-saved register preservation are retained.
Library classifications, canonical names/config, accepted identities, profiles,
model identities and every historical receipt are unchanged files. No v11 runtime
object-backed closure is included; that integration is independent.

Public semantics: [AAPCS32 core registers](https://github.com/ARM-software/abi-aa/blob/main/aapcs32/aapcs32.rst#core-registers)
permits temporary data-endianness changes but requires designated endianness at
public interfaces. [SETEND](https://mikhailarkhipov.github.io/ARM-doc/A32/setend.html)
changes E; [MSR](https://mikhailarkhipov.github.io/ARM-doc/A32/msr_r.html) can write
CPSR.E, and [RFE](https://mikhailarkhipov.github.io/ARM-doc/A32/rfe.html) restores
processor state. No empty decoder-effect inventory supplies those proofs.

## Fresh revision and checks

`research/catalogue_v10_r5_evidence.json` is a new immutable hash-bound child of
untouched r4 evidence. Ordinary generation selects **catalogue-v10-r5** and binds
this child/repair policy in its source/input/review metadata. No r4 artifact is
replaced or relabeled.

Targeted tests exercise the actual public `build_catalogue(..., closure={})` range
writer, including the exact sealed counterexample, ARM/Thumb positives and
negatives, conditional predecessor joins, secondary/primary entry bypasses,
back/cross-owner edges, explicit restoration, ordinary-call boundaries, status and
exception changes, Thumb literal/PC-tagged callback paths, exact read widths,
unaligned pools, conditional veto-only reads and alignment refusal. These are not
source-text or mock-wiring tests.

Each source/test revision finished before its focused run. New exclusive
`scratch/closure/v10-r5` receipts retain every exit; the tracked metadata summary
is `research/catalogue_v10_r5_checks.json`.

| Exercised check | Exit | Actual result |
| --- | ---: | --- |
| `python -B -m unittest tests.test_catalogue_endian`, initial r1 | 1 | 17 methods / seven fixture assertion failures, retained below |
| Same focused module after fixture corrections, r2 | 0 | **17 methods / zero failures, errors or skips**; 0.138 s unittest / 0.303 s captured wall |
| `python -B -m unittest tests.test_catalogue_endian_cli` | 0 | **One real CLI method / zero failures or skips**; r5 output and child-hash public refusal, 0.446 s wall |
| Earlier corrected synthetic public probe | 0 | Unknown target, no false footprint; actual **0x2c001000** byte read and ABI/LE return preserved; 0.577 s wall |
| Final-head focused modules, r3 | 0 | **22 methods / zero failures, errors or skips**; 21 endian consumers + actual CLI; 0.422 s unittest / 0.554 s wall |
| Final-head corrected public probe, once | 0 | Unknown target and no false footprint, same actual byte read; ABI/LE return preserved; 0.166 s wall |

The actual writer's containing range is **[0x00100028,0x00100034)**, kind unknown,
owner null. `validate_catalogue` returns no structural issues; the erroneous
one-byte rodata claim is absent without suppressing a warning/exception. The real
CLI negative also preserves its nonzero exit and `v10-r5 evidence differs` stderr,
without writing a refused proposal. The public emulator result SHA-256 is
`91683b25d23f39aa57c8c0fc4e31984f14216682d2204d36f580cb6e35687c89`;
the earlier passing source-version probe and its result are retained too.

**Initial author-fixture failure is preserved, not retried unchanged.** Existing
pool/address ownership already classified some intended PC-relative positive
targets as function-owned rather than unowned rodata. Two secondary-entry cases
lacked a genuine rooted caller and a forward callback was already absorbed by the
existing addressed-data ownership rule. Corrections kept the actual public writer:
expect its existing function ownership for PC-relative positives, root the real
secondary caller with a prologue, and put the callback before its reader. These
were fixture-only corrections. The final source revision then closed opaque
veneer ingress and unrecorded secondary entry paths, and required unconditional
adjacent Thumb-PC fact definitions. Four more consumer tests cover veneer paths,
preserved SVC invalidation, modeled multi-register effects and branch restoration.
Final focused checks ran after these edits. No mock/source-text substitute or
gate relaxation was introduced.

No current-image generation, full suite, build, compiler/matcher, linter or
formatter was run. One probe ran on each of two substantive source revisions;
the final-head corrected synthetic emulator scenario was run once only, with new
immutable result and receipt names. Neither unchanged failed input was retried.

## Remaining adoption prerequisites

Main must run the integrated full suite, reproduce a fresh v10-r5 proposal,
recheck complete conservation/protection/accounting and current accepted/candidate
compatibility, then obtain a fresh independent review of the complete proposal
before any adoption. **No current-image delta or unchanged-image count is asserted
without that generation check.** The sealed r4 report's 360 unknown bytes/36
intervals and ten anomalies are prior observations, not newly generated r5 totals.
Existing library extent qualifications (22/10/620, never 26/20/632), unresolved
identities and all gate/credit obligations remain in force. No denominator record,
accepted credit, main merge, closure adoption or push is performed here.
