# Independent review: candidate catalogue v8-r2

## Verdict

**PASS_WITH_QUALIFICATIONS** for candidate **`e33aad973846b60b5286be6becb5080c9626188fa9a64a6c0b48197b16e8adad`**, branch `catalogue-v8`, commit **`b52f08b1b6f17be29c03634c17c9cbd078ecd65c`**.

The prescribed plain analyzer command reproduces the complete candidate byte-for-byte. The post-classification whole-record invariant is fixed; all **870 unique accepted/prepared protected owners / 50,012 extent bytes** are unchanged, including the current main ledger with c005-r2 and the inspected c006 branch candidate. Both reported wrapper/body boundaries are now supported by independently decoded image structure and preserved as 8+8 partitions. The previous SWP/STREX counterexamples, and eight further atomic/exclusive variants, are refused by both recovery rules. **`tests.test_analyze`: 140 tests, OK, exit 0; no skips.**

The qualifications are evidentiary, not newly discovered byte-accounting defects:

- **R05 remains a retained-proof qualification.** Historical compiler/emulation executions are not fresh review executions; the two source worktrees remain absent. Fresh original-extent hash comparisons and retained corrected-grid/source identities are distinguished below from inherited execution claims.
- **[INFERENCE]** The two new boundaries use the adopted erased-tail-branch interpretation plus the guarded field-read/closed-transfer shape, not recovered source-section symbols. Leading-entry dependence retains the declared call-interface may-read model, including the specific r1 qualification at `0x002102b4`.
- The inspected **unprepared c007 WIP snapshot is not wholly unchanged**: it includes one correctly classified outlined compiler helper. The integrator explicitly clarified that c007 is outside accepted/prepared protection and that this helper is being dropped. That observation is recorded rather than hidden or treated as an adoption blocker; no subsequent removal is claimed observed.

This review does not adopt the catalogue or award accepted C credit. No tracked file, source/header, configuration, compiler profile, ledger or history was edited. No build, full suite, compiler/emulation probe, formatter, prepare/finalize, commit or push was run. Persistent review outputs are this report, the required rerun, and private packets under this directory. Packet contents contain original material and must not be published; none is quoted here.

## R01–R06 disposition

| Previous finding | Status | Independent result |
| --- | --- | --- |
| **V8-R01 — prescribed regeneration mismatch** | **Resolved** | Exactly `tools/analyze.py catalogue --out scratch/review/catalogue-v8-r2/rerun.json`, with the required Python from the specified worktree, exits 0 and produces the supplied SHA-256 and complete byte string. Classification and revision metadata are now integrated into this entry point; there is no second generation/postprocessing step. |
| **V8-R02 — classification bypasses protection** | **Resolved for accepted/prepared scope** | `tools/analyze.py:2047-2049` applies the split pass, then classifications, then complete-record protection. `check_protected_functions` at `1850-1858` compares the entire owner dictionary. Regression tests at `tests/test_analyze.py:1280-1324` and `1326-1361` cover kind mutation, evidence-only mutation, automatic c005/c006 loading and bundled-conflict withholding. Independent synthetic kind/evidence mutations are rejected. Current main matches and the inspected c006 records are identical, not merely equal in address/size. See the explicit c007 WIP observation below. |
| **V8-R03 — `0x001efcd0` / `0x001efcd8` boundary omitted** | **Resolved under the adopted linker model** | Fresh v7/r2 packets and raw image decoding confirm an exact linker-tail site at `0x001efcd4`, nonstoring SP/LR-independent prefix, complete two-instruction scalar field-read/ordinary-return suffix, and no internal control/data crossing. Candidate partitions the old 16-B owner into 8+8 without changing coverage. |
| **V8-R04 — `0x00131948` / `0x00131950` boundary omitted** | **Resolved under the adopted linker model** | The exact tail site is `0x0013194c`. The suffix is a scalar field read followed by a direct unconditional tail to retained ARM owner `0x00139174`, which is not classified noreturn. Both 8-B parts close under this transfer interpretation, with no internal crossing or pool/literal ownership. |
| **V8-R05 — proof-retention note** | **Qualification retained and accurately disclosed** | The immutable snapshot, unchanged classification records and reports retain old-proof provenance. Eight original whole-extent digests freshly match the published named compiler-section identities. The corrected three-function grid has the correct image/source identities and 16 offset-zero failures per function. The old 3,552 ABI executions, natural-caller MATCHes, five declaration rejections and compiler grid were not rerun. |
| **V8-R06 — incomplete memory-side-effect guards** | **Resolved** | Shared explicit predicate `tools/analyze.py:48-67` is used by leading recovery at `1001-1003` and pointer-prefix recovery at `1084-1088`. It rejects swap, exclusive stores/loads and reservation operations independently of operand WRITE flags. The previous two cases still lack Capstone MEM WRITE flags but are now rejected by both rules. Ten atomic/exclusive forms and benign-load positive controls pass fresh synthetic checks and the committed regressions. |

## Inputs, identities and exercised checks

| Input/output | Identity |
| --- | --- |
| Worktree HEAD | `b52f08b1b6f17be29c03634c17c9cbd078ecd65c` |
| Adopted v7 `config/catalogue.json` | SHA-256 `b303210af5d4f8395044bb8cd31508d7ee801b4640704c1253a433e68e1c88d3` |
| Immutable original v8 input | SHA-256 `2fe7c9767b57a60a1b5a43b4fdc2c2ac0651a30d580341f75f183230e1dc3d8c` |
| Supplied v8-r2 | SHA-256 **`e33aad973846b60b5286be6becb5080c9626188fa9a64a6c0b48197b16e8adad`** |
| Independent `rerun.json` | SHA-256 **`e33aad973846b60b5286be6becb5080c9626188fa9a64a6c0b48197b16e8adad` — IDENTICAL** |
| Original image | SHA-256 `a367b5dcd65e3924c93cf3fbaee5d4d40f9c5205d1287912784a974d5e405361` |
| Worktree `tools/analyze.py` | SHA-256 `f7b2b35bfa9e68bdd7de5ec7c286817142e630804db1a3647b661c4537bbd70e` |
| Worktree `tools/catalogue_review.py` | SHA-256 `341c3a1d8695e80f423cd4f952f7943be1a93129a8490c52e09fd7b49e8a432d` |
| Worktree `tests/test_analyze.py` | SHA-256 `698fac44879123a017453901d2397be2c1483dca41153f40611d3b5ed3bf8ffe` |
| Worktree evidence snapshot | SHA-256 `a7c09fd1a0bfa3876e30ff93af2e9cf9d869954f0c4ed758edea98cc857bddf8` |
| Worktree `docs/LINKER.md` | SHA-256 `a0d1e0fc40e316b8a695782a5a8182646e7c22e14e7bba0e06d2d2ab2f23cd45` |

All five reviewed worktree implementation/test/evidence/documentation files equal their exact `git show b52f08b:<path>` contents. Those reads and the worktree HEAD query all exited 0. Main's current catalogue and names overlay remain byte-identical to the reviewed worktree inputs. Main's accepted ledger has advanced, as accounted for below.

Commands ran from `scratch/worktrees/catalogue-v8`, with `PYTHONDONTWRITEBYTECODE=1`:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out scratch/review/catalogue-v8-r2/rerun.json
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -m unittest tests.test_analyze -v
```

The worktree's `scratch/review/catalogue-v8-r2` is a symlink to this review directory, so the exact prescribed relative output argument retains the rerun alongside this report.

| Exercised operation | Exit | Observed result |
| --- | ---: | --- |
| Prescribed plain analyzer regeneration | **0** | 4,592 owners; 7,724 unknown bytes; 70 anomalies; 31.017 s reported / 31.10 s captured wall; complete candidate byte equality |
| `tests.test_analyze`, one run | **0** | **140 tests, OK**, no skips; 54.640 s unittest / 54.75 s captured wall |
| `git show c006-integrate:config/candidates/c006.json` | **0** | 381 functions; exact hash/blob below; `func_00152964` absent |
| c007 branch tree and candidate inspection | **0**, both | 193-function unprepared WIP snapshot; one classification-only conflict, explicitly dispositioned below |
| `tools/packet.py` | **0**, all **16** | Two v7 boundary-owner packets plus 14 r2 packets: all 12 new owners and the two retained wrappers. Every packet's instruction-address coverage equals its catalogue code ranges |
| Independent image/CFG/setup audit | **0** | Both split proofs, 12 new-owner evidence, all 22 nonstoring leading repairs and callback value origin confirmed; structural validation `[]` |
| Independent synthetic guard audit | **0** | Both previous counterexamples plus eight additional forms refused in both entry rules; kind/evidence-only protected mutations rejected; benign reads accepted |
| Independent classification/digest/grid audit | **0** | All 27 evidence records accounted for; eight whole-extent digests match; eight original service contracts and corrected grid identities confirmed |
| Independent residual literal/decoding audit | **0** | All 15 outside-literal references confirmed from architectural PC calculations; unresolved decode site remains undecodable; crossing/overlap predicates remain explicit |
| Independent JSON interval/accounting audit | **0** | Every old unknown interval and anomaly disposition accounted for; all extent, creditable and physical-map sums agree |

No check was piped through output filtering before its outcome was judged. Manual probes are not added to the unittest count. Four preliminary **review-harness** invocations exited 1: one Python generator/subscript mistake, then assumptions that all outside references were memory operands, every undecodable site had a containing owner, and inferred addressed spans began at the unrounded target. The harness was corrected for PC-address-producing forms, the unowned decode site and conservative word-aligned inferred bounds. Final audits exited 0; no candidate/source/test change or failed product-test retry was involved.

## Whole-record protection, including current c005-r2 acceptance

The fresh comparisons resolve manifest symbols through the actual names overlay and compare **complete adopted owner records**, including kind, evidence, entries, code ranges, pools, literals and noreturn fields.

| Manifest inspected | SHA-256 | Functions | Extent bytes | Complete-record changes |
| --- | --- | ---: | ---: | ---: |
| Worktree `config/matches.json` | `fd31d2cd50424520fe6e7444fe70b553dc76e1b2a2abb377eb61e8ef85cd11e4` | 299 | 7,620 | **0** |
| Worktree `config/candidates/c005.json` | `514701fedc425d536f37fed07472173d99e5fd0015d837265ca3fcd04dff7df5` | 190 | 11,668 | **0** |
| **Current main `config/matches.json`, including c005-r2** | `55443148b88a369ce624894cd20e0b0d0b39151cc461a899b8560cd1a700ba23` | **489** | **19,288** | **0** |
| `c006-integrate:config/candidates/c006.json` | `b760840461a9a00f239e55b277799222c2203a7b4ff972e051aaf217690c5781` | **381** | **30,724** | **0** |

The inspected c006 Git blob is **`a4ef5151bdf965bfc7de9456be856be0f7263204`**, matching candidate provenance. It does not include `func_00152964`. Current accepted matches plus c006 are **870 distinct owners / 50,012 B**, all ordinary top-level functions and all unchanged. The candidate's older ledger hash is not passed off as the current ledger hash: its separately included c005 candidate protects the same newly accepted owners, and the independent current-ledger comparison closes that timing gap. Rerun SHA equality is for the specified fixed worktree inputs; no claim is made that a changed input hash serializes to the same candidate.

The code checks final records after classification, rather than relying only on filtering bundled evidence. Explicit conflicting classification input reaches that invariant and fails. The well-formed synthetic runtime reclassification and separate evidence-only mutation both raise the protected-function error; their baseline input record is not mutated. The CLI regression also demonstrates explicit, recorded withholding for bundled conflicts.

### O01 — c007 WIP observation, not a protected-checkpoint failure

Fresh `git show c007-integrate:config/candidates/c007.json` read a **193-function / 14,664-B** snapshot:

- SHA-256: **`4cc7dbdf81a5b785ab1599cb42ede3a2a05e642bc0e390544b1a8e6fe164fcec`**.
- Git blob: **`86371dd72e19051b68b428a3be16e85cdff9b72f`**.
- **192 complete owner records are unchanged.** The one difference is `func_00130f90`, 16 B: ordinary `function` becomes `runtime` and gains classification evidence.
- Its original whole-extent digest freshly equals the retained named, relocation-free ARMCC `__ARM_common_memclr4_8` identity. No defect in that runtime classification was found.

The automatic loader names existing c004/c005/c006 files and the c006 branch blob; it does not automatically include c007. During this review the integrator explicitly clarified that **c007 is unprepared WIP, not an accepted/prepared/in-review checkpoint**, and that the outlined helper is being dropped, consistently with the earlier c006 helper decision. Under that clarified protection scope this is an observation, not R02 remaining open. This report does **not** assert all inspected c007 rows were unchanged or that the announced removal has already occurred.

## Image boundaries and all 12 new owners

### The two R03/R04 partitions

Fresh original-image decoding was independent of candidate acceptance booleans. Both old owners have a complete 16-B ARM code range, no pool/literal and no intervening interior entry. The wrapper prefix does not modify SP/LR/PC or memory; the exact erased linker-tail word is executed at the stated site. Each suffix is an unconditional, nonindexed, non-writeback r0-based scalar field read into r0 followed by a closed return/transfer. No direct branch or data reference crosses the proposed internal boundary.

| Old v7 owner | Exact tail site | r2 wrapper | r2 successor | Independent suffix closure |
| --- | --- | --- | --- | --- |
| `0x00131948`, 16 B | `0x0013194c` | `0x00131948`, 8 B | `0x00131950`, 8 B | Direct unconditional tail to retained ARM top-level owner `0x00139174`, not classified noreturn |
| `0x001efcd0`, 16 B | `0x001efcd4` | `0x001efcd0`, 8 B | `0x001efcd8`, 8 B | Ordinary LR return |

`field_read_successor` at `tools/analyze.py:1501-1528` supplies a generalized shape witness; the normal crossing, literal, frame, entry and complete-protection vetoes remain in force. The committed tests at `tests/test_analyze.py:424-480` cover scalar widths, ordinary returns, known returning tails, noreturn refusal, conditional/writeback/stack/long-suffix refusal, atomic prefixes and control/data crossings. The whole-image regression explicitly expects both partitions at `1467-1473`. This does not make every leaf NOP a split witness.

**[INFERENCE]** Physical geometry and the guarded interpretation are supported; authenticated original section/function symbols are not recovered. The known tail target is not described as a newly rerun whole-program return proof.

### Twelve newly catalogued owners relative to v7

| New owner | Extent | Independently checked evidence |
| --- | ---: | --- |
| `0x00110cc8` | 484 B | Pool pointer at `0x00107f30` resolves exactly; all 116 instructions / 464 code bytes reached, 20 pool bytes, 19 internal branch edges, one return, no call/open exit |
| `0x00131950` | 8 B | R04 field-read/closed-tail boundary above; existing owned bytes, not new physical coverage |
| `0x0014b358` | 224 B | Pool pointer at `0x00180c7c` resolves exactly; all 56 instructions reached, one internal branch edge, one return, no call/open exit or pool |
| `0x00180570` | 8 B | Rodata pointer at `0x002239c0`; bounded nonstoring setup ends at the exact tail immediately before unchanged v7 owner `0x00180578` |
| `0x0019d5dc` | 8 B | Rodata pointer at `0x002239d0`; same guarded setup/tail proof into unchanged `0x0019d5e4` |
| `0x001c5260` | 12 B | Newly restored owned pool pointer at `0x00214d7c`; all three instructions reached, ordinary return, no pool/call/open exit |
| `0x001d5c98` | 8 B | Rodata pointer at `0x0022492c`; exact tail into unchanged `0x001d5ca0` |
| `0x001d7864` | 8 B | Rodata pointer at `0x002249dc`; exact tail into unchanged `0x001d786c` |
| `0x001d80a8` | 8 B | Rodata pointer at `0x002249d8`; exact tail into unchanged `0x001d80b0` |
| `0x001d89dc` | 8 B | Rodata pointer at `0x00224a50`; exact tail into unchanged `0x001d89e4` |
| `0x001efcd8` | 8 B | R03 field-read/return boundary above; existing owned bytes, not new physical coverage |
| `0x001f3e78` | 20 B | Rodata pointer at `0x0022496c`; one argument-memory read plus three register/constant setup instructions, no store/SP/LR change; exact tail into unchanged `0x001f3e8c` |

All ten pointer values above were resolved from the original mapped image, not copied from an acceptance flag. Every pointer-prefix successor remains equal to its complete v7 record. Six prefixes are register-only; `0x001f3e78` is correctly called **nonstoring**, not register-only.

The callback's value origin is also independently confirmed: owner `0x00107f0c` loads the owned-pool callback pointer into **r1** at `0x00107f10`, then passes it at `0x00107f1c` to `0x0019f338`. The still-unmodified incoming value reaches the registration-state store at `0x0019f36c` before the first r1 definition. This is register/value-origin metadata only; no instruction text or image bytes are reproduced.

Private packets are under `packets/v7/` and `packets/v8-r2/`. There is no invented v7 function packet for a previously uncatalogued address: v7 packets are the two actual old boundary owners; all 12 new owners have r2 packets. All 16 CLI invocations exited 0, and all ARM instruction-address sets agree with their catalogue code ranges.

### The 22 retained leading repairs

All 22 v8 leading repairs remain identical in r2. Fresh decoding checks their **35 restored setup instructions / 140 instruction bytes**: unconditional core-register setup, no memory-writing/atomic class, no writeback or SP/LR/PC output, and no call/branch. Original body/pool/literal coverage is retained. Added pool ownership is **88 B**, extent growth **152 B**, and **76 B** fill holes already inside old extents rather than growing the denominator.

Corrected starts checked: `0x00107140`, `0x00113ca4`, `0x001456e4`, `0x00195a50`, `0x001c6440`, `0x001cb10c`, `0x001ce67c`, `0x001f50d8`, `0x001f5170`, `0x001f5674`, `0x001f5780`, `0x001f580c`, `0x001f586c`, `0x00203810`, `0x00205280`, `0x00206938`, `0x002102b4`, `0x00214580`, `0x00214d18`, `0x00214d9c`, `0x00214e40`, `0x00215120`.

The explicitly reported `0x00113ca4` owner ends at **`0x00113da0`**, with pool **`[0x00113d9c, 0x00113da0)`**. The `0x001f50d8` setup and pool are retained. The unchanged may-read limitation is visible in both evidence provenance and `docs/LINKER.md`: calls are treated as potentially reading r0–r3, not as recovered prototypes. The earlier finer explicit-use proof is not claimed freshly rerun here.

## R06 synthetic counterexamples and actual setup safety

The manual review replay uses the synthetic swap/store-exclusive cases corresponding to the previous review's counterexamples. Their core-result writes and missing Capstone MEM WRITE flags remain; acceptance has changed from true to false in **both** recovery rules.

| Synthetic forms | Number | Pointer-prefix accepted | Leading repair promoted |
| --- | ---: | --- | --- |
| SWP, SWPB | 2 | **No**, both | **No**, both |
| STREX, STREXB, STREXH, STREXD | 4 | **No**, all | **No**, all |
| LDREX, LDREXB, LDREXH, LDREXD | 4 | **No**, all | **No**, all |

Exclusive loads are rejected for reservation-state effects, not falsely called ordinary memory stores. The predicate also names reservation clearing and acquire/release/store families. The suite's swap/store-exclusive leading regression explicitly asserts that the WRITE flag is absent; refusal is therefore not an accidental consequence of that flag changing. Benign argument-memory reads remain accepted in fresh pointer and leading positive controls.

The actual image setup population is nonstoring: **35 leading setup instructions + 10 pointer-wrapper setup instructions = 45**. The stricter guards do not change any of those supplied-image prefix decisions. The two added field-read wrapper prefixes independently satisfy the same memory/SP/LR safety constraints.

## Classification review against the selected evidence standard

All **27** candidate classification-evidence records equal both the immutable snapshot and the original reviewed v8 records. Their extents/ISAs/image identities match the actual catalogue. The bases are **11 compiler-ABI, eight compiler-generated whole-body identities, eight unrepresentable-ABI/shape rows**. A nonempty evidence field is not itself a proof; the records were checked against their retained sources and the original image.

### Compiler-generated whole bodies: eight fresh original hash comparisons

Each fresh digest hashes the entire original function extent locally without emitting its contents. All eight equal the published named, relocation-free ARMCC-generated section digest in `scratch/integrator/runtime-helpers/REPORT.md:223-240`.

| Address | Extent | Retained named compiler identity | Fresh whole-original digest comparison |
| --- | ---: | --- | --- |
| `0x00126f2c` | 24 B | `__ARM_common_memset4_5` | Equal |
| `0x00130f90` | 16 B | `__ARM_common_memclr4_8` | Equal |
| `0x00131958` | 20 B | `__ARM_common_memcpy4_8` | Equal |
| `0x00152964` | 20 B | `__ARM_common_memclr4_12` | Equal |
| `0x0017da60` | 20 B | `__ARM_common_memcpy4_6` | Equal |
| `0x00183458` | 20 B | `__ARM_common_memcpy4_5` | Equal |
| `0x001d481c` | 16 B | `__ARM_common_memclr4_8` | Equal |
| `0x001d5560` | 16 B | `__ARM_common_memclr4_5` | Equal |

Separate original addresses remain separate identities. No overlay binding is invented for an outlined helper lacking a natural-caller binding proof. In particular the c007 WIP observation does not invalidate the fresh whole-body identity at `0x00130f90`.

### Compiler ABI: retained contracts plus emission or bound whole-caller proof

| Owner | Extent | Identity | Retained evidence checked, not freshly executed |
| --- | ---: | --- | --- |
| `0x001070c0` | 40 B | `__aeabi_lasr` | 404 ABI cases and natural operator probe emission |
| `0x00186454` | 48 B | `__aeabi_uidivmod` | Current names binding, whole 64-B division caller MATCH and 530 ABI cases |
| `0x001864dc` | 64 B | `__aeabi_memclr` / `__aeabi_memclr4` | Current same-address bindings, two whole natural zero-fill caller proofs and 50 ABI cases; **remains multi_entry** |
| `0x0018cb4c` | 16 B | `__aeabi_memset` | Current binding, whole 84-B constructor/fill caller MATCH and 50 ABI cases; AEABI argument order retained |
| `0x0018d204` | 80 B | `__aeabi_ldivmod` | 530 ABI cases plus signed division/remainder helper emission; no unsupported whole-caller claim |
| `0x001911cc` | 34 B, Thumb | `__aeabi_vec_ctor_nocookie_nodtor` | Current binding, whole 36-B natural C++ member-array constructor MATCH and 17 original-helper cases |
| `0x001941f8` | 692 B | `__aeabi_uldivmod` | 530 ABI cases plus unsigned division/remainder helper emission |
| `0x00196498` | 80 B | `__aeabi_f2ulz` | 404 ABI cases plus helper emission; finite, nonnegative, in-range conversion domain retained |
| `0x00196504` | 8 B | `__aeabi_ul2f` | 407 ABI cases plus helper emission; base/softfp ABI and shared-core relationship retained |
| `0x0019f6d8` | 556 B | `__aeabi_idivmod` | 530 ABI cases plus signed operator emission; **remains multi_entry**, no invented names binding |
| `0x001a000c` | 136 B | `__aeabi_memcpy` / `__aeabi_memcpy4` | Current same-address bindings, whole 28-B and 64-B natural caller MATCHes and 50 ABI cases |

The 19 runtime-evidence owners are these 11 plus the eight whole-body rows. Only **17 previously ordinary owners / 1,286 B** become noncreditable: the two already excluded multi-entry owners are not subtracted again, and their complete entry topology remains intact.

The runtime report freshly hashes to **`cdd63c35f704b0f46796efd65442d9df1935fab0de24881c56f645b5a646150a`**, matching retained provenance. The former `runtime-helpers` and `armcc-ext` worktrees are absent. Thus compiler-emitted-name/caller/emulation claims remain inherited report evidence; fresh hash checks do not pretend to recreate the missing compiler objects or execution receipts.

### Kernel exclusions: hard ABI limit or specifically retained no-frame proof

Fresh original-image decoding confirms one expected service number and no call in each of these eight exact functions. Required high argument-register preparation and raw result-register consumption are consistent with the retained contracts.

| Address | Extent | Service | Accepted evidence standard |
| --- | ---: | --- | --- |
| `0x00100d6c` | 4 B | ExitProcess, `0x03` | Original no-frame shape; corrected retained 16-cell grid fails at offset zero in every cell |
| `0x001109cc` | 48 B | QueryMemory, `0x02` | Original consumes raw r1–r5 results; retained ARMCC #1505 rejection for six raw result words, r0–r5 |
| `0x0011ef08` | 36 B | CreateThread, `0x08` | Required processor-ID input in r4; retained #1501 declaration limit |
| `0x0012d148` | 8 B | ExitThread, `0x09` | Original no-frame shape; same corrected retained 16-cell grid |
| `0x0012d168` | 36 B | WaitSynchronizationN, `0x25` | Required timeout-high input in r4; retained #1501 limit |
| `0x00134e7c` | 8 B | Break, `0x3c` | Original no-frame shape; same corrected retained 16-cell grid |
| `0x00185108` | 24 B | ArbitrateAddress, `0x22` | Required timeout inputs in r4/r5; retained #1501 limit |
| `0x00195cd8` | 36 B | ControlMemory, `0x01` | Required permission input in r4; retained #1501 limit |

These exclusions total **200 B**: five hard-limit adapters / 180 B plus three no-frame stubs / 20 B. The retained `research/compid/r2/ctr_wrappers_grid_compid.json` has the correct original image identity, exactly three functions and **16 `+0` failures each**. Its recorded source digest equals the actual corrected `research/compid/r2/library_ctr_wrappers.c`. This is inspection of a finite retained shape grid, not 48 fresh compiler runs or a universal proof about every possible C source.

The compiler-constraint report freshly hashes to **`66a2f95a6deda10781d3f20b4a5c4a5adf15d3d717c92cc56c46a37a195bb2d3`**, matching retained provenance. **[INFERENCE]** SDK/archive/handwritten authorship beyond the demonstrated ABI/shape constraint is not authenticated.

### Exclusion safeguards

- No `library` kind is introduced. `strlen` at `0x00192b78` remains an ordinary 96-B creditable owner; a name/whole-caller binding is not an authenticated exact libc implementation.
- Of the **67** retained Thumb residue dispositions, the one bound vector-construction helper is `runtime`; the other **66 remain ordinary/creditable**. ISA is not provenance.
- All nine shape-only service adapters remain ordinary: `0x001109b4`, `0x0011ef2c`, `0x0011ef4c`, `0x0011ef58`, `0x0012d0ec`, `0x0012d150`, `0x00145610`, `0x001565b0`, `0x0018f7d4`.
- There is no additional classification of ordinary string/math, larger IPC or inlined-SVC code merely for semantics, neighborhood or instruction choice.

Public contract references retained by the evidence, not original-image fingerprint claims: [Arm runtime ABI](https://github.com/ARM-software/abi-aa/blob/main/rtabi32/rtabi32.rst), [3dbrew Kernel ABI](https://www.3dbrew.org/wiki/Kernel_ABI), [libctru service wrappers](https://github.com/devkitPro/libctru/blob/master/libctru/source/svc.s), [ARMCC `__svc` reference](https://support.arm.com/documentation/dui0472/m/Compiler-specific-Features/--svc), and [Arm SWP semantics](https://developer.arm.com/documentation/ddi0406/c/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/SWP--SWPB). No newly fetched documentation shell or fresh external/compiler execution is claimed.

## Unknown-byte and anomaly dispositions

Every one of the **189** v7 unknown intervals was independently intersected with the actual candidate map, ignoring the report's reason strings. The resulting start/end/kind/owner partitions exactly equal all candidate accounting rows and conserve every byte.

- **49 complete old intervals / 980 B** become evidenced function code/pool ownership.
- **140 complete old intervals / 7,724 B** remain unknown; no interval is partially hidden or newly created.
- No old unknown byte becomes guessed data or padding.
- A further **36 formerly padding bytes** become evidenced code/pool ownership.
- Added ownership is exactly **1,016 B = 908 instruction bytes + 108 pool bytes**. No previous code, pool or addressed-literal coverage is lost.

| Recovery group | Former unknown | Former padding | Added ownership |
| --- | ---: | ---: | ---: |
| 22 leading repairs | 228 B | 0 B | 228 B |
| Callback `0x00110cc8` | 476 B | 8 B | 484 B |
| Long body `0x0014b358` | 224 B | 0 B | 224 B |
| Leaf `0x001c5260` | 12 B | 0 B | 12 B |
| Seven pointer-prefix wrappers | 40 B | 28 B | 68 B |
| Two r2 internal splits | 0 B | 0 B | 0 B |
| **Total** | **980 B** | **36 B** | **1,016 B** |

All **83** old anomaly dispositions were independently compared after the 22 legitimate owner rekeys: **13 resolved, 70 open, no new issue**. Every resolution is an old unexplained hole now exactly covered by restored pool ownership:

| Old owner → repaired owner | Closed hole |
| --- | ---: |
| `0x001ce684` → `0x001ce67c` | 8 B |
| `0x001f50dc` → `0x001f50d8` | 4 B |
| `0x001f5180` → `0x001f5170` | 12 B |
| `0x001f5678` → `0x001f5674` | 4 B |
| `0x001f5788` → `0x001f5780` | 8 B |
| `0x001f5810` → `0x001f580c` | 4 B |
| `0x001f5870` → `0x001f586c` | 4 B |
| `0x00203814` → `0x00203810` | 4 B |
| `0x00205284` → `0x00205280` | 4 B |
| `0x0020693c` → `0x00206938` | 4 B |
| `0x00214588` → `0x00214580` | 4 B |
| `0x00214d28` → `0x00214d18` | 8 B |
| `0x00215130` → `0x00215120` | 8 B |
| **Total** | **76 B** |

The **70 still-open** issues are:

| Issue | Count | Independent disposition check |
| --- | ---: | --- |
| Unexplained function bytes | 28 | Every owner still has a genuine hole; their holes total **1,410 B** |
| Literal outside function | 15 | Original ARM/Thumb PC-relative memory/address-producing forms independently resolve outside the owning extent |
| Addressed literal crosses start | 7 | Recorded inferred bound crosses the independently retained next start |
| Addressed literal overlaps code | 5 | Conservative word-aligned inferred span intersects retained code; no data claim steals that code |
| Shared addressed literal | 14 | All reference sites map exactly to retained shared-ownership conflicts; sharing remains unresolved, not converted into exclusive data |
| Undecodable reachable code | 1 | `0x001964e8` still does not decode; no invented descriptor/nonreturn resolution or ownership is asserted |

The issue at old `0x001456e8` is still open at repaired `0x001456e4`, not counted as resolved. All literal-conflict accounting and all residual unknown ranges remain explicit. This review does not claim that every remaining unknown interval is provably incapable of containing a future recoverable entry.

## Creditable totals, extent sums and complete preservation

| Measure | Adopted v7 | Original v8 | Reviewed v8-r2 |
| --- | ---: | ---: | ---: |
| Top-level owners | 4,580 | 4,590 | **4,592** |
| Callable starts/entries | 4,604 | 4,614 | **4,616** |
| Creditable ordinary functions | 4,557 | 4,542 | **4,544** |
| Creditable extent bytes | 1,129,946 | 1,129,400 | **1,129,400** |
| All-kind extent sum | 1,136,930 | 1,137,870 | **1,137,870** |
| Physically function-owned bytes | 1,135,444 | 1,136,460 | **1,136,460** |
| Instruction bytes | 1,054,598 | 1,055,506 | **1,055,506** |
| Pool bytes | 39,988 | 40,096 | **40,096** |
| Addressed-literal bytes | 40,858 | 40,858 | **40,858** |
| Unknown bytes / intervals | 8,704 / 189 | 7,724 / 140 | **7,724 / 140** |
| Anomalies | 83 | 70 | **70** |

Independently recomputed kinds: **4,544 function + 17 runtime + 8 assembly + 17 multi_entry + 6 veneer = 4,592**. ISA counts are **4,523 ARM + 69 Thumb**. All credited counts and byte sums equal the serialized stats.

Exact balances:

- Creditable functions: **4,557 + 10 recovered owners + 2 split bodies − 17 runtime − 8 assembly = 4,544**.
- Creditable bytes: **1,129,946 + 152 leading extent growth + 788 original new extents − 1,286 runtime − 200 assembly = 1,129,400**.
- All-kind extents: **1,136,930 + 152 + 788 = 1,137,870**. The two new 8-B split bodies are balanced by 16 B of wrapper shrinkage; they add no extent bytes.
- Physical ownership: **1,016 B = 940 B extent growth + 76 B closed old holes = 980 B formerly unknown + 36 B formerly padding**.
- Owned subranges: **1,055,506 code + 40,096 pool + 40,858 addressed literals = 1,136,460 B**.
- Remaining extent holes: **1,137,870 − 1,136,460 = 1,410 B**, down from **1,486 B**. Extent sums are not full explained-byte coverage.

Ranges independently tile the same **1,327,104 mapped bytes**, without overlapping function extents. Data, rodata and segment-padding totals are unchanged. Exactly the 22 weak-prologue old callable starts are replaced; every other old start/secondary entry survives in the same ISA. There are 34 added starts: 22 corrected starts plus 12 new owners, for a net +12.

Relative to original v8, **4,588 owner records are identical**. Only the two 16-B wrappers change `size`, `code_size` and `code_ranges`; two 8-B ordinary ARM owners are added. The two physical range partitions cover exactly the same bytes, with unchanged range kinds. All **137 prior split records** survive identically. The 776-word inventory becomes **139 split + 93 already separate + 505 in-frame + 31 control-flow crossing + 1 literal crossing + 5 lacking successor proof + 1 unowned tail + 1 unexecuted slot = 776**; no crossing population is admitted.

## Other observations and adoption recommendation

**O02 — minor report counting typo.** REPORT-v8-r2 says “all 12 changed `revision_review` fields” at line 125, but its list and an independent key-by-key comparison contain **11**: `name`, `c006_present`, `protected_rows`, `protected_summary`, `preservation`, `changed_records`, `input_sha256`, `provenance`, `retained_evidence_source_sha256`, `retained_evidence_input_sha256`, `withheld_classifications`. This is a documentation count error, not an omitted candidate mutation or denominator error. The report's nine changed top-level fields and seven changed stats fields are otherwise consistent with the candidate comparison.

**No new blocking finding was established.** R01, R02, R03, R04 and R06 are resolved for the reviewed candidate and clarified checkpoint-protection scope. R05's inherited-proof limitation and the explicitly labelled boundary/may-read interpretations remain qualifications. Adoption of this exact candidate is supported **with those qualifications visible**, the unprepared c007 observation not misrepresented as protected equality, and without calling catalogue corrections accepted C progress. Adoption/finalization and any evolving checkpoint decisions remain integrator-owned.
