# exidx-removal — corrected v8 handoff

## Identity and discrete commits

- Branch/worktree: `exidx-removal`, `scratch/worktrees/exidx-removal`.
- Given corrected-v6 base: **`5bb20a9a3251e83cddc37d6b64a166679ebd8e51`**.
- **P1/group-closure and v8/retirement commit:**
  `d64767e71760c7e6337fe40a0bc3f398c78aa360`.
- **P2/complete compact-extab implementation commit:**
  **`75c4aa51fdc2a00e82ce5b60a6552ff6904eb50b`**.
- New immutable identity: **`armlink5-inline-branchnop-v8` = v6 + corrected complete
  own-exidx/extab elimination**. V7 is retired, never adopted; its rules are not edited under
  the same selectable name. V5 remains retired. Defaults remain v2 / literal `none`.
- Author: `mschienbein <mschienbein@users.noreply.github.com>`.

The sealed v7 review (`scratch/review/exidx-v7/REVIEW.md`, `SEAL.json`) is **FAIL**. Its two
counterexamples are ground truth, not rerun to reconfirm them. The old v7 receipts, rejected
exact draft (`scratch/exidx/GATES-DRAFT.md`) and review are unchanged. Historical v7 handoff
text remains in commit `88f5c38f4b014817cbcd2c64eebc710e4f3f8dc9`; no v7 record is relabeled
as v8 evidence or approval.

**New exact gate draft:** `scratch/exidx/v8/GATES-DRAFT.md` (proposed v1.19, unreviewed).
**New compact evidence ledger:** `scratch/exidx/v8/EVIDENCE.json`.
Complete new private commands, receipts, objects and reproduction drivers are under
`scratch/exidx/v8/`. Implementation documentation is `docs/LINKER.md`.

No active GATES edit, adoption, full suite, full build, prepare/finalize or push was performed.
The given v6 base and all its qualifications remain prerequisites, not independently approved
by this work. Main must obtain a fresh independent v8 implementation/exact-text review and run
the full suite after integration; activation waits for the active frozen checkpoint to close.
No author directory or copied author source was edited, no `packets/` was read, and no ROM
payload or original disassembly was printed or committed. Main's `.venv` and the existing
symlinked private inputs/toolchain were used.

## P1: complete group/copy closure, not matching signatures alone

**Before (sealed v7 F01):** a surviving catalogued caller, the removed helper and the helper's
own exidx could share one COMDAT group. V7 discarded the helper/index while keeping the caller
and issued successful exact caller output. Matching companion/text group signatures did not
prove that the group was removable.

**Corrected v8 rule:** before any own-unwind reference is exempted, every allocated member of
each affected COMDAT group and every supplied same-signature copy must already be independently
selected v6-eligible R1 code or a fully validated own-exidx/extab companion. A kept caller,
different kept function, bound entity, ordinary placed/unplaced data/BSS, unsupported code or
unrepresented allocated member prevents the entire exemption. No arbitrary unused member is
swept into general dead-code/data elimination. All selected code entities still need an actual
eligible inlined call, or the unchanged identical-used-copy proof.

ELF reading now preserves complete allocated section-group membership separately from the
frozen section-placement set, including zero-sized allocated witnesses that placement omits.
Object combination remaps every witness within its object, preserving absent-section witnesses
rather than dropping them. Both the ELF membership and all visible same-signature sections
are checked; all supplied copies are included. Unsupported non-COMDAT/unknown group semantics
fail closed. This guard applies to the new unwind exemption, not a reinterpretation of v6 R1.

Coverage includes kept callers/functions, bound and unplaced data/BSS, unsupported and uncalled
helper members, a live member hidden in another copy in both object orders, missing witnesses,
unknown/non-COMDAT flags, valid complete groups and multiple independently used helpers. A real
ARMCC COMDAT template fixture is linked successfully, then a zero-size allocated group member
is appended by changing **only ELF metadata**; the reader preserves it and v8 refuses the group
in both combined-object orders. No instruction/unwind payload or relocation is patched.

## P2: complete compact extab, with a separate descriptor terminator

**Before (sealed v7 F02):** v7 accepted truncated personality-0/1/2 extab of 4/8/8 bytes while
refusing complete 8/12/12-byte examples. The size rule counted only header/unwind words and
omitted the empty descriptor list's mandatory zero terminator.

**Corrected v8 rule:** `_compact_unwind_prefix_size` counts only the header/unwind words. The
out-of-line entry must have exactly one further zero word, whose value is checked independently
at the end of that prefix. Complete size is:

| Personality | Header/unwind prefix | Complete empty-descriptor extab |
|---|---:|---:|
| 0 | 4 bytes | **8 bytes** |
| 1 or 2 | `4 * (1 + N)` | **`4 * (2 + N)`** |

N is the encoded count of **additional unwind words**, not including the descriptor terminator.
For N=1 the long examples are 12 bytes. Missing/partial/nonzero terminators, incomplete/excess
unwind words, nonempty descriptors, descriptor relocations, combined/trailing entries, generic
formats and interior targets remain refusals. A one-word compact personality-0 entry directly
inside exidx is separate and remains supported without an extab/descriptor word.

Fixtures and exact GATES text are corrected with the implementation. Positives cover all three
personalities, long counts 0/1/3/255, exact NONE dependencies, and the unchanged inline-index
case. Negatives cover the complete class of framing errors above, not just the three reported
sizes.

Primary framing source: [EHABI32 personality routine exception-handling entries](https://github.com/ARM-software/abi-aa/blob/main/ehabi32/ehabi32.rst#personality-routine-exception-handling-table-entries).
The [ELF section-group unit rule](https://gabi.xinuos.com/elf/03-sheader.html#section-groups)
requires interrelated group sections to be included/omitted together. [LLVM's ARM ELF producer](https://github.com/llvm/llvm-project/blob/main/llvm/lib/Target/ARM/MCTargetDesc/ARMELFStreamer.cpp)
also emits the zero when no handler data is provided. The armlink-specific elimination argument
remains the documented/inferred one in `docs/LINKER.md`; no native armlink execution is claimed.

## All other own-unwind constraints remain mandatory

V8 first requires every unchanged corrected-v6 R1 condition: a complete source-defined eligible
4/8-byte ARM helper, no original binding through aliases/copies, no roots, correct entry/COMDAT
proof, I1–I3, original call-site ABI/CFG consistency and elimination of every other reference.
Only its own readonly allocated non-executable SHT_ARM_EXIDX/SHF_LINK_ORDER section may be
removed: sh_link to the exact text section, one complete eight-byte index, exact entry-only
PREL31 key, no shared/combined/duplicate entry and no kept-function key. Extab must be exclusive
readonly SHT_PROGBITS, exact complete supported compact form, and reached only by its own
index's offset-four PREL31. The index has no other incoming allocated reference.

Every supplied allocated section, even unplaced, is censused. Other incoming references,
required original placement, unsupported outgoing relocations/dependencies, independent names,
roots, bindings, foreign ownership and link-order dependencies refuse removal. Only structural
PREL31 and the exact matching offset-zero undefined personality NONE dependency are modeled in
metadata. No original unwind bytes or fabricated original helper address is introduced. A kept
function's separate index is never selected for elimination.

Receipts retain `unwind_sections` and target-null per-call rewrites with complete defining
source/object/profile binding and object-local ownership. Helpers and metadata earn no credit.
All typed-contract, manual ABI, source-policy, visible [INFERENCE] and explicit QUALIFIED R1
historical-identity obligations remain mandatory. Group closure and valid table framing do not
recover the original helper's identity or call graph.

## Exercised correction and focused regressions

Requested focused command, **exit 0, 226 tests, OK** (no full suite):

```text
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -m unittest \
  tests.test_inplace tests.test_match tests.test_literal_pools
```

Receipt: `scratch/exidx/v8/focused-tests.json`. This includes eleven additional test methods
beyond the previous 215-test run, including the actual ELF zero-size group witness. Existing
alias/CFG/effect/placement/literal-model regressions remain active. V7 is explicitly tested as
retired and absent from selectable models, with the F01/F02 reason; no v7 rule is executed.

`scratch/exidx/v8/corrections.py` exercises **22 new-v8-only cases**, all expected outcomes:
**5 exact caller links and 17 fail-closed refusals**. Command exited **0**. The program binds the
sealed review/report hashes as the before evidence and explicitly records `v7_rerun: false`.
Outputs: `corrections.json`, `corrections-command.json`.

| Sealed counterexample/class | Before, v7 review | After, v8 |
|---|---|---|
| Kept caller shares helper/index COMDAT group | Incorrect success | Refused before rewrite/removal publication |
| Another copy contains a different kept member | Missing whole-group guarantee | Refused in both object orders |
| Ordinary bound/unplaced data or missing allocated member | Signature-only proof insufficient | Refused |
| Complete removable helper/own-index group | Positive control | Exact caller link |
| Personality-0 complete 8 B | Incorrect refusal | Exact caller link |
| Personality-1/2 complete 12 B (N=1) | Incorrect refusal | Exact caller links |
| Personality-0/1/2 truncated 4/8/8 B | Incorrect success | Refused |
| Nonempty descriptors or trailing zero/combined data | Unsupported | Refused |
| Direct compact personality-0 inside exidx | Positive control | Exact caller link |

## Re-smoke input context and every natural outcome

The same 27 frozen copied source/profile cases cover all named authors. Sources/helpers/names
remain those bound in `scratch/exidx/v7/inputs.json`; v8's `inputs.json` preserves the closure
and source hashes. Current worktree config and all **471 headers** exactly equal the reviewed
v7 frozen context (`v8/context.json`); no runtime include override, compiler/profile/flag change,
source shim or new original binding was used. All executed commands use this worktree's matcher
and explicit v6/v8 with `--show 0`.

The two r41224 source/profile cases already have reported current-header compiler failures
(`PaneMaterialView` undefined). They were **not rerun merely to confirm that failure**. Their
four historical v6/v7 exit-2 receipts remain explicitly referenced, never relabeled as v8
receipts or assigned invented fresh exits. All other 25 source/profile cases were re-smoked
under both selectors: **50 fresh commands**.

Exits: 0 = every surviving function MATCH; 1 = completed comparison differs; 2 = compile/link
refusal. Sizes are candidate/target bytes; `+0x...` is first differing offset.

| Frozen case | Profile | V6 fresh result | V8 fresh result |
|---|---|---|---|
| `tu659-final` | game | 2: exidx reference | 1: MISMATCH 2444/2444; +0x39d |
| `r41103-game` | game | 2: exidx reference | 1: MISMATCH 1236/1248; +0x10 |
| `r41154-game` | game | 2: exidx reference | 1: MISMATCH 12320/12292; +0x24 |
| `r41103-sdk` | sdk | 2: exidx reference | 1: MISMATCH 1204/1248; +0x4 |
| `r41154-sdk` | sdk | 2: call-ABI refusal | 2: call-ABI refusal |
| `r41103-middleware` | middleware | 2: exidx reference | 1: MISMATCH 1244/1248; +0x4 |
| `r41154-middleware` | middleware | 2: exidx reference | 1: MISMATCH 12416/12292; +0x5 |
| `r41128-early` | game | 2: exidx reference | 1: MISMATCH 5236/5180; +0x29 |
| `r41128-final` | game | 2: call-ABI refusal | 2: call-ABI refusal |
| `r4026-member-hooks` | middleware | 2: exidx reference | **0: MATCH 164/164** |
| `r4076-middleware` | middleware | 2: exidx reference | 1: MISMATCH 140/140; +0x14 |
| `r4076-game` | game | 2: exidx reference | 1: MISMATCH 140/140; +0x5 |
| `r4088-cpp-hook` | game | 2: exidx reference | 1: MISMATCH 804/808; +0x30 |
| `r41207-cpp-result` | game | 2: exidx reference | 1: MISMATCH 60/60; +0x2f |
| `r41224-game` | game | Not rerun; reported compiler failure | Not run; same unresolved prerequisite |
| `r41224-middleware` | middleware | Not rerun; reported compiler failure | Not run; same unresolved prerequisite |
| `r41234-game` | game | 2: exidx reference | 1: MISMATCH 188/192; +0x0 |
| `r41234-sdk` | sdk | 2: exidx reference | 1: MISMATCH 176/192; +0x0 |
| `r41234-middleware` | middleware | 2: exidx reference | 1: MISMATCH 188/192; +0x0 |
| `r41262-001bd68c` | game | 2: exidx reference | **0: MATCH 696/696** |
| `r41262-001c09e8` | game | 2: exidx reference | 1: MISMATCH 840/840; +0xa8 |
| `r41275-cpp-predicate` | middleware | 2: exidx reference | 1: MISMATCH 640/636; +0x24 |
| `r3L25-game` | game | 2: call-ABI refusal | 2: call-ABI refusal |
| `r3L25-middleware` | middleware | 2: exidx reference | 1: MISMATCH 3332/3340; +0xc |
| `r41307-001e722c-complete` | game | 2: exidx reference | 1: MISMATCH 360/364; +0x18 |
| `r41307-001e7db0-complete` | game | 2: exidx reference | 1: MISMATCH 344/348; +0x9 |
| `r41307-001e395c-complete` | game | 2: exidx reference | 1: MISMATCH 440/408; +0x38 |

Fresh v6: **25 exit-2**. Fresh v8: **2 exit-0, 20 exit-1, 3 exit-2**. The same independent
ABI blockers remain r41154/sdk `+0x7f8`, r41128/final `+0x35c`, r3L25/game `+0x668`; neither
correction bypasses them. Every executed command/exit and actual receipt/hash is recorded in
`v8/summary.json` and `v8/EVIDENCE.json`, with logs under `v8/smoke/CASE/{v6,v8}/`.

### Fresh v8 MATCH receipts — no acceptance claim

- `func_0013f06c`, middleware, 164/164:
  `scratch/exidx/v8/smoke/r4026-member-hooks/v8/func_0013f06c_v3.result.json`, SHA-256
  **`d5327a047a905e8860d9a46f6961fb0c3abb1913b444e5826c74fd282b643814`**.
- `func_001bd68c`, game, 696/696:
  `scratch/exidx/v8/smoke/r41262-001bd68c/v8/func_001bd68c.result.json`, SHA-256
  **`da529546074c412bdb545d544aaaed14527cbbbfba05fe04737c8c9bd3240ac2`**.

These are the same private identity witnesses already independently confirmed by the v7 review,
not new accepted functions. Historical helper identity remains [INFERENCE]/QUALIFIED. Full typed
contract and source review remains mandatory; in particular the pre-existing drawing-screen
INTEGRATOR markers/unintegrated declarations are not a G3/G6 pass.

## Accepted-function regressions

Every one of the **15 supplied command rows** was rerun under v6 and v8: **30 commands, all
exit 0**, complete exact extents, zero rewrites and zero removed helpers. The input manifest
is `scratch/worktrees/r1-jumptables/scratch/integrator/r1-jumptables/r3/regression-commands.json`.
It contains five unique functions repeated under three historical selectors; execution did
not deduplicate the rows.

| Function | Original rows | Exact bytes | V6 exits | V8 exits |
|---|---|---:|---|---|
| `func_00105084` | 1, 6, 11 | 64 | 0, 0, 0 | 0, 0, 0 |
| `func_0010d604` | 2, 7, 12 | 64 | 0, 0, 0 | 0, 0, 0 |
| `func_0011e928` | 3, 8, 13 | 72 | 0, 0, 0 | 0, 0, 0 |
| `func_0018fdd4` | 4, 9, 14 | 40 | 0, 0, 0 | 0, 0, 0 |
| `func_00195254` | 5, 10, 15 | 224 | 0, 0, 0 | 0, 0, 0 |

Commands/receipts: `scratch/exidx/v8/accepted/rowNN-func_XXXXXXXX/{v6,v8}/`; every receipt path,
SHA-256 and exit is in `v8/EVIDENCE.json`.

## Frozen-model equivalence

Extracted linker/registry source blobs directly from given base `5bb20a9`; the fingerprint
program loads that base and current code in separate processes and links identical already
compiled inputs. **40 cases × five active frozen models = 200 outcomes compare byte-identically**
for **strict-v1 / v2 / v3 / v4 / v6**. Both commands exit **0**. V5's retirement refusal remains
unchanged. V7 retirement is tested separately, not falsely compared as an active old policy.

Records include placed-byte hashes, placement metadata, full rewrites, removed entities and
literal rebindings, or exact refusal type/reason. Coverage is 25 compiled natural cases, five
accepted functions and ten synthetic full/alias/unwind/pointer/root/explicit-binding cases.
The known compiler-failure cases have no objects and are not counted as linker cases.

Both serialized fingerprints have SHA-256:
**`f4fa701d4bc700e043263be45e95dfe6310eebe6d50950bd226d4af5fbc788bc`**.
Artifacts: `v8/frozen-equivalence.json`, `frozen-baseline.json`, `frozen-current.json`, including
exact commands and base-code hashes. This is finite evidence, not a mathematical proof over all
malformed objects. Frozen selectors do not enter the new companion/group/descriptor logic.

## Original-image inference and remaining limits

The existing structural census and its inference were independently reproduced/confirmed by the
sealed review and are retained unchanged at `scratch/exidx/v7/image-census.json`, SHA-256
`65cd4894320bc457706cbb4c333f0104c94fdab71f7559f5c8fb1df7279d4afe`. It was not rerun solely
to reconfirm a reported observation. No authenticated original exidx table is identified;
absence and original exception-link flags remain **[INFERENCE]**, never a stripping premise.
A flat image lacks original section headers/table bounds. Candidate CANTUNWIND fragments do
not establish corresponding original entries, and group/format corrections do not change that.

No native armlink execution, complete standalone all-source link, full original unwind-table
placement or exception-behavior proof is claimed. Exact caller bytes, group closure and complete
compact framing do not recover historical helper identity. Independent v8 review, Main's full
suite and the explicit frozen adoption path remain required.

Committed artifacts are this updated handoff, `docs/LINKER.md`, the new exact v8 gate proposal
and compact evidence ledger. Old v7 evidence and rejected text remain unchanged. Complete new
private receipts/objects/header-bound input records and reproduction drivers stay in the
worktree; no compiler, ROM payload, packet, disassembly or author candidate source is committed.
