# Independent denominator re-review — catalogue v9-r2

## Sealed verdict

**PASS_WITH_QUALIFICATIONS. overall_correctness: correct. Confidence: 0.99.**

**Adoption-ready as an honest intermediate denominator revision** for the exact proposal SHA-256 **7bc3fd8bc866ee5827a12eb6e86715ab1464c05830f4adb2d0b0071e39996ac3**, at **a930af3c65ff0a74fa4596b80e42038ae3788a26** (fix 4891c6d6b923f3e510001a41f5f19ecfa8e45c04 plus its isolated test-fixture correction), on v9 **9b1808944d2fed376aa04f523d15c13bb8bd4dbc**. Both prior blocking findings are resolved. **No new actionable patch-introduced finding or adoption blocker was established.** This is not catalogue closure, adoption, checkpoint approval or accepted C credit.

The exact proposal was installed as the adopted baseline only in an isolated copy under this review directory. The ordinary CLI then exited 0, retaining all function records, entries, ranges, statistics and anomalies exactly. Independent full-delta accounting confines the physical change to nine formerly unknown bytes completing three shared UTF-16 objects. The complete 4,699 function records remain identical to v9.

This review was sealed before reading any other v9-r2 review. The previous sealed v9 FAIL was an input whose findings were independently checked, not a substitute for this analysis. No nested agents were used. Required rules read: AGENTS.md, docs/ACCEPTANCE.md, docs/GATES.md, docs/REVIEWING.md and both docs/catalogue/REVIEW-v8*.md standards. No tracked file or other worktree was edited. No compiler, emulator, build, test suite, linter, formatter, prepare/finalize, adoption, commit or push was executed. All writes are confined to this ignored review directory. No ROM bytes, disassembly, packets or secrets are reproduced in this report or its evidence outputs.

## Findings and prior-FAIL disposition

**Current findings: none.**

| Prior finding | Disposition | Independently observed evidence |
| --- | --- | --- |
| F1 / V9-R02, post-adoption regeneration rejects legacy labels | **Resolved** | Actual CLI generation with the exact r2 proposal as config/catalogue.json exits 0. All five legacy secondary labels, all 4,730 address/ISA pairs, all complete function records, ranges and stats survive. Five isolated valid carry-forwards reproduce their exact owner records; 45 malformed variants are refused. |
| F2 / V9-R01, shared wide object incorrectly bounded as one narrow byte | **Resolved** | All three exact targets are complete four-byte UTF-16 objects, unowned rodata. The retained r4023 object freshly links to the original over all 128 candidate bytes against the unchanged 124-byte catalogue owner, without rewrites or literal rebindings. Its complete four-byte object equals all three original target spans. Independent pool users prove sharing; no extent or function ownership grows. |

### F1: exact adopted-baseline generation, not an API-only substitute

The review driver copied the a930af3 analyzer, catalogue_review and ctrfmt modules, the frozen protection/name/config inputs and r2 evidence into **adopted-copy/**. It wrote the exact supplied proposal as that copy of **config/catalogue.json**, checked its complete SHA-256, and ran the copied **tools/analyze.py catalogue --out .../adopted-regenerated.json** with the project interpreter. Original inputs were read through a symlink; no canonical baseline or worktree input was replaced. The pinned c008 Git blob remained available through the containing repository.

- Exit **0**, stderr empty; **29.798 s** captured wall / 29.702 s analyzer time.
- **4,699 owners; 4,408 unknown B; 37 anomalies.**
- **4,699/4,699 complete function records identical** to both exact v9 and r2.
- All **4,730/4,730** prior entry address/ISA pairs retained; all ranges, stats, anomalies, ownership and denominator values identical to r2.
- Adopted output SHA-256: **ce57b084d46fb1e3917a19a455db0051cab1ff7c58ae4d62d6cfe3bfb0b8628d**.

Full JSON equality with the pre-adoption proposal is neither claimed nor expected: the baseline digest and baseline-relative review accounting now refer to r2 instead of v8. The only changes are that digest binding; the five carry-forward rows now report previous evidence legacy_prologue and the previous full owner sizes; and recomputed baseline-relative unknown/anomaly/changed-record/boundary/extent/preservation summaries. All exact changed paths are retained in **accounting.json**. No executable/data record, physical range or statistic changes in this transition.

The five preserved labels are **0x001f5990 -> 0x001f5980**, **0x002069f0 -> 0x002069ec**, **0x00212684 -> 0x00212674**, **0x002136d0 -> 0x002136c4** and **0x00214fec -> 0x00214fe8**. Every owner remains multi_entry, uncredited, with an aligned ARM secondary label and no incoming sources. The new accepted-baseline branch requires the same initializer owner, exact legacy label form, matching ISA, a valid instruction boundary and preservation of every previous code/pool/literal span. For each of the five actual records, focused mutations of owner, owner kind, label evidence, incoming sources, entry kind, code coverage, initializer evidence, ISA and alignment are refused (**45/45**). This does not establish independent callability for the legacy guesses.

### F2: all three complete shared objects

| Target | Address-taking owner / site | Typed text consumer | Other independent pool users | Result |
| --- | --- | --- | ---: | --- |
| 0x001b54e4 | 0x001b5374 / 0x001b53e4 | 0x0016e47c | 1 | Four-byte unowned rodata |
| 0x001b8234 | 0x001b8198 / 0x001b8208 | 0x001eafa0 | 3 | Four-byte unowned rodata |
| 0x001bc29c | 0x001bc220 / 0x001bc278 | 0x001eafa0 | 4 | Four-byte unowned rodata |

Every frozen reference decodes to its exact target in r2, and each reference supplies r2. Canonical func_001eafa0 and screen.h use const unsigned short *text. The first object uses the alternate text-slot constructor func_0016e47c; its retained natural source forwards unsigned-short text through the canonical code-unit-length/copy interfaces. The sharing census uses actual original pool words, not just the shared_literals owner list. Full four-byte equality with the retained natural object independently corroborates the bounds at all three targets.

The original natural source/object/receipt digests agree with the frozen r2 witness. The object is **game / armlink5-inline-branchnop-v2 / none**, **128 candidate B / 124 owner B**, complete equality, no rewrite, no literal permutation. This remains **match_extent_differs**, not accepted source credit and not exclusive ownership. No formerly owned code, pool or literal span intersects the three four-byte data objects.

The typed parser accepts the complete ideographic-whitespace object rather than using the leading narrow NUL. Focused synthetic checks also accept typed whitespace and refuse a non-whitespace control, an unterminated object and an unpaired surrogate. Untyped narrow parsing is unchanged. Exact per-owner/reference witnesses select UTF-16; whitespace appearance alone is not a width inference.

## Complete v9 -> v9-r2 accounting

The reviewed patch changes only **tools/analyze.py**, **tests/test_analyze.py**, **docs/LINKER.md**, new **docs/catalogue/REPORT-v9-r2.md**, and new **research/catalogue_v9_r2_evidence.json**. Seven principal implementation/test/evidence/report files, including the unchanged old evidence and catalogue_review module, were compared with their exact a930af3 Git blobs; all agree. The old v9 proposal, old evidence and old report remain separate historical inputs.

### Physical and denominator delta

| Measure | Exact v9 | v9-r2 | Delta |
| --- | ---: | ---: | ---: |
| Top-level function records | 4,699 | 4,699 | 0; every record identical |
| Retained entry address/ISA pairs | 4,730 | 4,730 | 0 |
| Ordinary denominator functions / extent B | 4,624 / 1,131,454 | 4,624 / 1,131,454 | 0 |
| All-kind extent bytes | 1,141,022 | 1,141,022 | 0 |
| Function-owned physical B | 1,139,722 | 1,139,722 | 0 |
| Instruction / pool / addressed-literal B | 1,057,946 / 40,792 / 40,984 | Same | 0 |
| Unowned rodata B | 123,157 | 123,166 | +9 |
| Unknown B / ranges | 4,417 / 87 | 4,408 / 84 | -9 / -3 |
| Padding B | 76 | 76 | 0 |
| Data / segment-padding B | 51,464 / 8,268 | Same | 0 |
| Anomalies | 37 | 37 | 0; complete rows identical |

Exactly these half-open intervals change **unknown -> unowned rodata**:

- **[0x001b54e5, 0x001b54e8)**, 3 B.
- **[0x001b8235, 0x001b8238)**, 3 B.
- **[0x001bc29d, 0x001bc2a0)**, 3 B.

All other byte categories and owners are identical. Each former one-byte shared rodata span therefore becomes its complete four-byte object, removing exactly one three-byte unknown interval. No new padding, hidden gap, exclusive data claim, extent growth, classification, function, topology change or C-credit change is introduced.

Independent segment-by-segment interval intersection confirms exact tiling of **1,327,104 B**. Code/pool/literal spans neither overlap one another nor disagree with physical function-owner assignments. Every owned byte is retained. **1,300 B** of holes inside function extents remain explicitly unowned; denominator extent totals do not mean that those holes are code.

### Every changed serialization/input field

Only five proposal top-level fields differ from v9: **generator, ranges, revision_review, shared_literals, stats**. The shared-literal changes are exactly the three corrected object ends. The only changed stats are **bytes_by_range_kind.rodata (+9)**, **bytes_by_range_kind.unknown (-9)** and **unknown_ranges (-3)**.

Only five revision_review fields differ: **boundary_queue, input_sha256, name, provenance, unknown_accounting**. The queue changes correct the two previously explicit narrow-empty-string descriptions; all other queue records remain equal. Input/source hashes and revision/evidence-path labels bind the new generator/evidence. Unknown accounting partitions include the additional nine data bytes.

The new evidence differs from old evidence only in: the two corrected boundary-queue decisions/descriptions; three appended exact UTF-16 owner/target/reference rows; the r2 revision label; prior-review correction provenance; and the complete pinned shared-wide-object witness. **All seven previous encoding rows, all 35 classification records, all initializer, protection, author, Thumb, runtime, helper and kernel witnesses are unchanged.** No source/classification policy is broadened into a new vendor/library ownership claim.

All **140 v8 unknown intervals / 7,724 B** were independently intersected with the actual r2 map; every partition and original-interval digest agrees with recorded accounting. They now comprise **56 resolved, five partial and 79 open original intervals**, yielding **84 current unknown ranges / 4,408 B**. All **70 v8 anomaly dispositions** are accounted for: **35 resolved, 35 open**, plus the same two v9 Thumb-hole anomalies, for **37** total. There is no anomaly suppression in this correction.

## Preserved v9 properties

### Complete protected records

Names were resolved independently and complete records compared to the adopted v8 catalogue, not just addresses/sizes. All frozen manifests and the pinned c008 blob retain their hashes. The protection union is **1,602 identities / 186,152 B**, **zero changed complete records**. All remain ordinary top-level functions.

- Current main accepted ledger: **1,167 identities / 73,388 B**, complete-record identical.
- Pinned c008 blob **ec64c31302ef6631530d909e0e3ec3bd8494f33a**: **435 identities / 112,764 B**, complete-record identical; SHA-256 **d8f49a991dde5c659f76f833de228342ff8fce9726288f55abae3f27055596b2**.
- The worktree ledger and c004/c005/c006/c007 snapshots overlap these populations; their counts are not additional credit. Per-manifest identities/hashes and equality results are in **accounting.json**.

### Initializer and Thumb topology

Original self-relative table arithmetic independently reproduces **160 distinct address/ISA pairs** over **[0x00235e38, 0x002360b8)**, all final entries: **67 prior callable / 93 newly recovered relative to v8**. Every target record, original table bound and initializer owner is unchanged from v9. Actual retained natural C++ objects under game, sdk and trial-game-thumb each contain the four-byte .init_array section with R_ARM_TARGET1. This is a section-ABI witness, not authenticated historical vendor symbol identity.

All **56** unresolved Thumb residue rows remain ownership-unclassified: **54 ordinary / two multi_entry**. The two v9 topology exclusions are unchanged: the owner at **0x00100084** retains old entry **0x001000a0**, and **0x0010030c** retains the interior entry **0x00100312**. Complete function entries/code ranges, original image, veneer records, all **776** linker-NOP inventory records and all **143** split records are unchanged. No incoming edge or secondary label is removed to manufacture C eligibility. All **4,616 v8 entries** also remain present.

### Runtime, named helper and no-frame witnesses

All **35** admitted classification records equal v9 exactly (27 inherited v8 records and eight v9 additions); no library classification is added. The unchanged entire function records and original image establish no extent/topology regression. In addition:

- **Seven retained natural emission objects** were independently rehashed and parsed: four ABI-emission profiles with actual vec_dtor and pure-virtual relocations, plus three initializer-section objects. Relevant section sizes/relocations agree with retained receipts. No compiler was run.
- Three retained natural array-delete caller objects, **game/sdk/middleware**, were freshly linked to the original: **52/52 B each**, no rewrites. Source/object hashes agree; this is existing-object verification, not fresh compilation or accepted credit.
- The actual named, relocation-free **20-B __ARM_common_memclr4_10** section equals the entire original extent at **0x001847c8**; its STT_FUNC size, section identity and digest agree with the frozen witness.
- The retained **63 original ABI behavior cases** and their image identity remain consistent and all recorded cases pass. These historical executions were inspected, not rerun here. No new exception-propagation claim is made for the nonthrowing destructor model.
- The retained three-function no-frame grid has **48 +0 cells**, no compile failures and no matching cell; public sources and grid digest agree. Original decoding confirms exact services **0x2b / 0x28 / 0x27** at **0x0011ef2c / 0x0011ef4c / 0x001565b0**, without ordinary calls or pushes. All three remain qualified assembly classifications under the v8 finite-grid standard, not a universal impossibility-of-C claim.
- **0x00145610** remains the ordinary **28-B CreateMemoryBlock** adapter, service **0x1e**, not the DuplicateHandle function. Negative source trials alone do not exclude it.

**67 explicitly pinned evidence files** match their hashes, resolving public paths in the proposal worktree and private paths in main scratch. Additional actual emission/caller source/object identities were checked as described above. Old ABI/compiler executions, proprietary archive ownership and missing historical worktree provenance are not promoted to fresh proof.

### Consuming-side routing

The producer and consumer paths were read together. Frozen encoding rows select the typed parser at exact references; include_address_literals creates shared intervals, and build_catalogue explicitly maps those bytes as rodata with owner null before padding classification. There is no silently dropped fourth byte. Legacy entries are retained before classifications/final complete-record protection; validate_catalogue checks their ISA/instruction boundary and owner kind. inplace.Resolver explicitly routes every recorded entry and ISA: independent lookup succeeds correctly for **all 4,730 pairs**, including the five legacy labels. image.creditable and progress_report still exclude every non-function kind. Boundary queue decisions are forwarded metadata, not dispatch commands. No new value is silently discarded by a consuming switch/filter/registry.

## Exercised evidence and limitations

Reviewer-executed, without any full suite/build:

| Operation | Exit | Result |
| --- | ---: | --- |
| Exact-r2-as-adopted ordinary CLI | 0 | Full records/ranges/stats invariant; 4,730 entries retained |
| Independent complete accounting/protection/source/blob audit | 0 | All assertions pass; exact nine-byte delta |
| Focused existing-object / classification / legacy-guard audit | 0 | Wide object 128 B; three callers 52 B each; seven emission objects; helper equality; five valid plus 45 refused legacy cases |
| Focused parser and main-artifact inspection | 0 | Four synthetic parser cases; all seven old typed rows unchanged; main output hashes verified |

One preliminary **review-harness** witness run exited 1 because it assumed all three text owners call 0x001eafa0. Actual decoded call targets showed that 0x001b5374 instead calls the typed 0x0016e47c constructor. Its retained source/canonical halfword interfaces were inspected, the incorrect harness assertion was narrowed to the actual three consumers, and the corrected audit passed. No product file was changed and no unchanged failing product check was rerun.

**Main-executed, not reviewer-executed:** the retained clean-a930af3 suite receipt reports **566 tests, one skipped, no failed modules, exit 0** (83.959 s). Both retained normal CLI output artifacts hash to the exact proposal. The earlier 4891c6d isolated-fixture failure is preserved in REPORT-v9-r2.md; a930af3 updates that fixture to the required r2 evidence filename/revision and asserts the serialized path. No suite was rerun by this reviewer merely to confirm main observations.

### Qualifications and adoption boundary

1. **Intermediate revision only:** **4,408 unknown B / 84 ranges and 37 anomalies remain open**, including the original exceptional dispatch at 0x001964e8. They are honestly accounted and are not blockers to this intermediate adoption; they would block a claim of complete image recovery/decomp completion.
2. **No new C credit:** all denominator records/creditability are unchanged from v9. Shared literals remain external unowned data. The r4023 wide witness remains an unaccepted EXTENT candidate.
3. **Retained proof is not fresh execution:** no compiler or ABI emulator was run. v8 historical runtime/declaration/grid evidence retains its original limitations. Trial Thumb profiles do not identify or freeze a compiler profile; no vendor library/archive body is authenticated.
4. **Inference remains explicit:** the initializer walker is admitted by compiler section-ABI role, not recovered historical symbol spelling; erased-tail boundaries and the inherited leading-call may-read model retain their v8/v9 qualifications; preserved legacy labels are conservative topology, not proved independent callable C functions. Nonthrowing destructor probes do not establish exception propagation.
5. **Scope is exact:** approval covers these two fixes plus fixture correction and this proposal identity, not subsequent catalogue-v10 closure work or integration changes in newer main tooling. Adoption/history updates and checkpoint gates remain integrator-owned. No adoption was performed.

## Retained review artifacts

- **REVIEW.md**, **SEAL.json**: this verdict and hash-bound seal.
- **adopted_probe.py**, **adopted-command.json**, **adopted-regenerated.json**, **adopted-copy/**: exact adopted-baseline CLI reproduction and isolated inputs.
- **accounting_probe.py**, **accounting.json**: independent record/entry/protection/map/accounting/identity checks and exhaustive changed-field paths.
- **witness_probe.py**, **witnesses.json**: retained-object comparisons, actual sharing/reference metadata, helper/emission/grid checks and focused legacy-guard evidence.
- **supplementary.json**: focused parser outcomes and inspected main output/suite identities.

**Adoption recommendation: PASS_WITH_QUALIFICATIONS for this exact intermediate proposal. No unresolved review blocker remains.**
