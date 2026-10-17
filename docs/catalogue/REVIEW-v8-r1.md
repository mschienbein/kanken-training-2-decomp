# Independent review: candidate catalogue v8

## Verdict

**FAIL** for the supplied candidate **`2fe7c9767b57a60a1b5a43b4fdc2c2ac0651a30d580341f75f183230e1dc3d8c`**, branch `catalogue-v8`, commit **`4cf77e7939ea7021a2d99a0bed9fe94543d9e144`**.

The prescribed analyzer command does **not** reproduce that candidate. Independent review also found two new guard-contract defects and confirmed both subsequently reported wrapper/body boundaries are still unhandled. These findings prevent an unconditional adoption recommendation.

The supplied candidate's **22 start corrections, 10 newly owned functions, 17 ordinary-function runtime classifications, eight kernel-ABI classifications, two additional multi-entry evidence annotations, protected records, and arithmetic totals** are accounted for below. No actual false split or loss of previously owned instruction/data bytes was demonstrated in this particular image. The leading-entry analysis has the explicitly stated may-read qualification below; the compiler classifications have retained-evidence rather than fresh-probe provenance.

This is an independent catalogue review, not adoption or accepted C credit. No tracked file, configuration, source, header, compiler profile, ledger or history was edited. No build, full suite, compiler probe, formatter, prepare/finalize, commit or push was run. Review outputs and private packets are confined to `scratch/review/catalogue-v8/`. Packet bodies contain private original material and must not be published; none is quoted here.

## Findings

| ID | Severity | Subject | Summary |
| --- | --- | --- | --- |
| V8-R01 | major | Prescribed regeneration is not candidate-identical | `tools/analyze.py catalogue --out …/rerun.json` exits 0 but produces SHA-256 `685dbb7e4466fdbda3216d8adf7e94c95bfb888e841974542198a8e67ed1b2b0`, not the supplied candidate hash. It has no classifications, 4,567 creditable functions / 1,130,886 B, and no `revision_review`. The supplied candidate is instead produced by the separately committed `research/catalogue_v8.py`, which injects 27 classification records and review metadata. REPORT-v8 documents that different entry point; this is not a claim that no reproduction recipe exists. It is a failure of the specifically requested plain-analyzer reproduction and denominator equality. |
| V8-R02 | medium | Classification bypasses whole-record protection | The complete protected-record check is at `tools/analyze.py:1741-1744`, before `build_catalogue` applies classifications at `1978-1979`. A review-only synthetic protected ordinary function plus well-formed classification evidence returns successfully with its kind changed to `runtime`: address/size are preserved, but the protected record is not. The supplied image does not exercise the conflict: all requested protected records are identical. The final protected-record invariant nevertheless needs to hold after classification as well. |
| V8-R03 | medium | Unhandled `0x001efcd0` wrapper/body boundary | Both v7 and v8 retain the same 16-B owner. The exact linker-tail site is `0x001efcd4`, and `0x001efcd8` is neither a start nor an entry. The wrapper prefix does not write SP/LR; the two-instruction suffix reads memory and returns; no control/data crossing is recorded. V8 still reports `no_independent_successor_witness`. This is an inherited conservative omission, not a newly introduced false split. [INFERENCE] Under the adopted erased-tail-branch interpretation, an 8+8 partition is the relevant missing boundary. |
| V8-R04 | medium | Unhandled `0x00131948` wrapper/body boundary | The unchanged 16-B owner still contains the exact linker-tail site at `0x0013194c`; `0x00131950` is neither a start nor an entry. Its prefix does not write SP/LR, and its two-instruction suffix is a memory-read/return body with no recorded crossing. The disposition remains `no_independent_successor_witness`. This is the second reported inherited omission, with the same model-based 8+8 interpretation and qualification as V8-R03. |
| V8-R05 | note | Classification proof retention | The cited runtime-helper and ARMCC-extension private worktrees are absent. Their retained reports remain available, and all eight published common-helper body digests independently match the original extents. The corrected three-function no-frame compiler-grid JSON and its source are retained and identity-consistent. The 3,552 ABI executions, natural-caller matches and five rejected raw-ABI declarations were **not rerun** here; their proof status is inherited from the cited reports, not fabricated as fresh review results. |
| V8-R06 | medium | Memory-side-effect guards are incomplete in both new entry rules | Review-only synthetic atomic swap and store-exclusive setups are admitted both by `_pointer_prefix_wrapper` and by leading-instruction promotion. These instruction classes can write memory while also writing a core result register. The pointer-prefix rule has no explicit memory-write exclusion; the leading-prefix operand-access test misses them because Capstone leaves the memory WRITE access flag unset for these forms. No such instruction occurs in the supplied 29 recovered setups. Reject memory-writing/atomic-store classes explicitly rather than relying solely on the result-register filter or operand WRITE flags. REPORT-v8's blanket description of the seven pointer prefixes as “register-only” is also inaccurate: `0x001f3e78` contains a benign memory load. |

The two residual boundary findings were explicitly requested during review. They were already present among v7's seven conservative retentions; their existence is not a regression introduced by v8. Resolving them must not mean indiscriminately splitting every leaf NOP: the original independent-entry and crossing safeguards still matter.

## Inputs, source identity and rerun

| Item | SHA-256 / identity |
| --- | --- |
| Adopted v7 `config/catalogue.json` | `b303210af5d4f8395044bb8cd31508d7ee801b4640704c1253a433e68e1c88d3` |
| Supplied `scratch/analyze/catalogue-v8.json` | `2fe7c9767b57a60a1b5a43b4fdc2c2ac0651a30d580341f75f183230e1dc3d8c` |
| Independent `scratch/review/catalogue-v8/rerun.json` | **`685dbb7e4466fdbda3216d8adf7e94c95bfb888e841974542198a8e67ed1b2b0` — NOT IDENTICAL** |
| Original image identity | `a367b5dcd65e3924c93cf3fbaee5d4d40f9c5205d1287912784a974d5e405361` |
| Worktree `tools/analyze.py` | `6292e93f25ba8555d6cc1a71f6dbf601f7db322a89a9122f56a664d156b5841f` |
| Worktree `tests/test_analyze.py` | `3ca79bd4edf19a96033aa5b07613c3d016b746121f11b34044fcf28a5177c4f0` |
| Worktree `research/catalogue_v8.py` | `6749a203a32e2e0e1a6de86efed77ee298435e719a204604303fc54ad8560f19` |

All three worktree files match their exact `git show 4cf77e7:<path>` contents. Contrary to the two-file target shorthand, the commit contains **three** tracked files; REPORT-v8 correctly discloses the research generator as the third.

Commands ran from `scratch/worktrees/catalogue-v8` with `/home/michael/code/kanken-training-2-decomp/.venv/bin/python` and `PYTHONDONTWRITEBYTECODE=1`:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out /home/michael/code/kanken-training-2-decomp/scratch/review/catalogue-v8/rerun.json
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -m unittest tests.test_analyze -v
```

| Exercised operation | Exit | Result |
| --- | ---: | --- |
| Requested plain-analyzer regeneration | 0 | 4,590 owners; 7,724 unknown bytes; 70 anomalies; different classification/creditable totals; 23.572 s captured wall time |
| `tests.test_analyze` | **0** | **126 tests, OK**; 48.297 s captured wall time |
| Private `tools/packet.py` invocations with explicit v7/v8 catalogues | 0, all **116** | 73 v7 and 43 v8 packets; every header size agrees; all 115 ARM packets have exactly the expected instruction-address coverage; the one Thumb packet is not subjected to a fixed-four-byte address assumption |
| Independent image/CFG/dependency and structural audit | 0 | New long bodies close, new leaf closes, all structural validation issues absent |
| Classification/digest/grid audit | 0 | Eight common-helper body hashes agree; all supplied core differences explained by the 27 classifications |
| Synthetic protected-classification audit | 0 | Demonstrates the V8-R02 invariant failure |
| Synthetic memory-side-effect audits | 0 | Demonstrate both admitted atomic-memory-writing forms in both recovery rules, V8-R06 |

No check was piped through output filtering before its exit was recorded. The additional audit cases are not added to the unittest count of 126.

### Exact explanation of the hash mismatch

Only three top-level fields differ between the plain rerun and the supplied candidate:

- `functions`: the same 4,590 addresses and extents, but **27 records** gain classification evidence; 17 kinds become `runtime`, eight become `assembly`, and two remain `multi_entry`.
- `stats`: only `functions_by_kind`, `classified_functions`, `creditable_functions` and `creditable_bytes` differ.
- `revision_review`: present only in the supplied candidate.

Applying the supplied 27 evidence records to the independently generated catalogue makes the complete core catalogue equal to the supplied candidate **with `revision_review` removed**. This localizes the discrepancy; it does not turn the requested byte-level SHA comparison into a pass. No alternative author-script run is represented as a fresh exact reproduction.

## Analyzer review: retained safeguards and new rules

### Existing partition guards

The reviewed patch does not alter the internal tail-partitioning implementation. The exact executed unconditional linker-NOP requirement, independent-successor gate, direct/switch control-crossing vetoes, literal/data-crossing vetoes, saved-stack-LR handling, additional-interior-entry gate and pre-classification complete-record protection remain present. There is no discovery/refinement pass after the tail partition.

Independent metadata comparison confirms:

| Whole-image linker-NOP disposition | v7 | v8 |
| --- | ---: | ---: |
| Applied internal split | 137 | 137 |
| Already separate | 86 | 93 |
| In-frame, not a tail candidate | 505 | 505 |
| Crossing control flow | 31 | 31 |
| Crossing literal/data ownership | 1 | 1 |
| No independent successor witness | 7 | 7 |
| Unowned tail without a head start | 8 | 1 |
| Unexecuted switch slot | 1 | 1 |
| **Complete population** | **776** | **776** |

All 137 prior internal split sites survive. The seven newly recovered pointer prefixes account for the seven transfers from “unowned tail” to “already separate.” No crossing population is silently admitted. V8-R02 concerns the new mutation **after** the inherited protection check, not removal of the old tail-split veto.

### Leading instructions

All 22 actual prefixes were independently decoded. They occupy previously unowned bytes before a weak prologue-only start, have no incoming callee-saved-register dependence, contain unconditional core-register setup, and do not overwrite SP/LR/PC. Every referenced literal lies within the resulting owner without intersecting code. No moved old start has independent entry evidence, and no requested protected owner is moved.

The input analysis is intentionally a **may-read** model: `tools/analyze.py:919-921` treats every call as consuming all of `r0-r3`. That is not a recovered callee prototype or proof that every supplied value influences the callee. A finer explicit-read search independently connects 21 complete prefixes, and the `r0` component at `0x002102b4`, to actual body/callee reads. The `r1` setup at that remaining site relies on the declared call-interface may-read assumption. Its physical start/extent recovery is therefore **model-qualified**, not evidence of a demonstrated wrong start. This limitation should remain visible rather than being called a precise ABI-contract proof.

The synthetic cases in V8-R06 show that the implementation's general side-effect-free promise is incomplete even though these actual prefixes are nonstoring. A separate explicit instruction-class audit, not merely Capstone's operand access flags, confirms all actual 29 leading/pointer setups use only register/constant setup, integer arithmetic and memory loads: **13 + 6 + 26 = 45 instructions**, with no memory-writing instruction class.

### Pointer-only bodies and prefixes

The two long pointer-only bodies were independently walked as finite CFGs using decoded control transfers, not the candidate's acceptance booleans. Every code instruction is reached, no edge enters a pool or an unowned hole, all branches remain in the body, and each has one closed return. Their pointer-source words were independently resolved from original owned pools. The callback additionally has an actual loaded-pointer call-argument witness and registration-state store, described below.

The seven pointer-prefix wrappers end at the exact linker-tail word immediately before an independently preserved v7 entry. They do not modify SP/LR, call, branch early, own pools, or depend on uninitialized condition flags. Six have register-only setup; `0x001f3e78` loads argument memory and then performs register setup. It has no memory write and does not invalidate the actual tail-boundary evidence. It does invalidate a blanket “register-only” description and helps motivate an accurate, explicit nonstoring predicate rather than the incomplete current one.

### Classification kinds

The new application function validates image hash, address, extent, ISA, basis, requested kind and a nonempty basis-specific evidence field before mutation. The whole list is checked transactionally; unsupported bases and duplicate addresses are rejected. Actual proof content is an external review responsibility, not authenticated merely by a nonempty string.

`runtime` and `assembly` are consistently noncreditable to existing consumers via `image.creditable`; physical ranges remain `function` ownership. The two already noncreditable multi-entry owners retain their kind and complete entry topology. No ordinary library classification is admitted without known-library identity. The missing final protected-record check is V8-R02.

## Per-change confirmation: all 22 boundary corrections

“Pool +B” means newly owned pool bytes, including recovered interior holes; it is not necessarily extent growth. All rows preserve the prior body coverage. All are confirmed within the leading-entry model above; the one call-may-read qualification is marked explicitly.

| v7 start / size | v8 start / size | Restored setup | Pool +B | Independent disposition |
| --- | --- | --- | ---: | --- |
| `0x00107144` / 136 | `0x00107140` / 140 | `r0`, 4 B | 0 | Confirmed; constant setup supplies body input; independent pointer witness also present |
| `0x00113ca8` / 244 | `0x00113ca4` / 252 | `r5`, 4 B | 4 | Confirmed; literal setup feeds body; restored pool ends at the correct owner end |
| `0x001456e8` / 236 | `0x001456e4` / 240 | `r3`, 4 B | 0 | Confirmed; constant setup supplies body input; the existing unexplained-byte issue remains, rekeyed to the corrected owner |
| `0x00195a54` / 124 | `0x00195a50` / 128 | `r0`, 4 B | 0 | Confirmed; constant setup reaches a callee input; independent pointer witness |
| `0x001c6444` / 352 | `0x001c6440` / 360 | `r1`, 4 B | 4 | Confirmed; literal setup supplies body input; noncrossing restored pool |
| `0x001cb110` / 224 | `0x001cb10c` / 232 | `r1`, 4 B | 4 | Confirmed; literal setup supplies body input; noncrossing restored pool |
| `0x001ce684` / 420 | `0x001ce67c` / 428 | `r0, ip`, 8 B | 8 | Confirmed; both values feed the body; old 8-B interior hole closes |
| `0x001f50dc` / 148 | `0x001f50d8` / 152 | `r0`, 4 B | 4 | Confirmed; static-initialization object setup reaches the callee; old pool hole closes |
| `0x001f5180` / 152 | `0x001f5170` / 168 | `r0-r3`, 16 B | 12 | Confirmed; array-construction setup and saved argument/body uses; old 12-B pool hole closes |
| `0x001f5678` / 264 | `0x001f5674` / 268 | `r1`, 4 B | 4 | Confirmed; literal setup supplies body input; old pool hole closes |
| `0x001f5788` / 76 | `0x001f5780` / 84 | `r0-r1`, 8 B | 8 | Confirmed; both setup values feed body; old pool hole closes |
| `0x001f5810` / 92 | `0x001f580c` / 96 | `r0`, 4 B | 4 | Confirmed; setup reaches callee input; old pool hole closes |
| `0x001f5870` / 76 | `0x001f586c` / 80 | `r0`, 4 B | 4 | Confirmed; setup supplies body input; old pool hole closes |
| `0x00203814` / 596 | `0x00203810` / 600 | `r0`, 4 B | 4 | Confirmed; setup reaches callee input; old pool hole closes |
| `0x00205284` / 108 | `0x00205280` / 112 | `r0`, 4 B | 4 | Confirmed; setup supplies body input; old pool hole closes |
| `0x0020693c` / 112 | `0x00206938` / 116 | `r0`, 4 B | 4 | Confirmed; setup supplies body input; old pool hole closes |
| `0x002102bc` / 868 | `0x002102b4` / 876 | `r0-r1`, 8 B | 0 | Confirmed physical recovery; `r0` dependence witnessed; `r1` component qualified by the rule's all-argument call may-read model |
| `0x00214588` / 84 | `0x00214580` / 92 | `r0-r1`, 8 B | 4 | Confirmed; both setup values feed body; old pool hole closes |
| `0x00214d28` / 116 | `0x00214d18` / 132 | `r0-r3`, 16 B | 8 | Confirmed; array-construction setup/body uses; restored pool also supplies the new constructor entry pointer |
| `0x00214da0` / 160 | `0x00214d9c` / 164 | `r0`, 4 B | 0 | Confirmed; setup supplies body input; existing pool ownership retained |
| `0x00214e44` / 176 | `0x00214e40` / 180 | `r0`, 4 B | 0 | Confirmed; setup supplies body input; existing pool ownership retained |
| `0x00215130` / 236 | `0x00215120` / 252 | `r0-r3`, 16 B | 8 | Confirmed; array-construction setup/body uses; old pool hole closes |

The explicitly reported `0x00113ca4` owner ends at **`0x00113da0`**, and its restored literal range is **`[0x00113d9c, 0x00113da0)`**. The explicitly reported `0x001f50d8` setup and its pool are restored as claimed.

Together these rows add **140 instruction bytes and 88 pool bytes** of ownership. Extents grow by **152 B**; the other **76 B** fill already-existing extent holes. This distinction is essential to the byte accounting.

## Per-change confirmation: all 10 new functions

| New start | Extent | Independently resolved pointer source | Confirmation |
| --- | ---: | --- | --- |
| `0x00110cc8` | 484 B | Owned pool at `0x00107f30` | Confirmed: 116 reachable instructions / 464 code bytes, 20 pool bytes, 19 internal branch edges, one return at `0x00110e94`, no calls or open exits |
| `0x0014b358` | 224 B | Owned pool at `0x00180c7c` | Confirmed: 56 reachable instructions, one internal branch edge, one return at `0x0014b434`, no pool/code overlap or open exit |
| `0x00180570` | 8 B | Rodata at `0x002239c0` | Confirmed: bounded argument setup; exact tail at `0x00180574` into retained `0x00180578` |
| `0x0019d5dc` | 8 B | Rodata at `0x002239d0` | Confirmed: bounded argument setup; exact tail at `0x0019d5e0` into retained `0x0019d5e4` |
| `0x001c5260` | 12 B | Newly restored owned pool at `0x00214d7c` | Confirmed: complete three-instruction leaf with return at `0x001c5268`; no pool or open edge; pointer witness depends on the evidenced leading repair at `0x00214d18` |
| `0x001d5c98` | 8 B | Rodata at `0x0022492c` | Confirmed: exact tail at `0x001d5c9c` into retained `0x001d5ca0` |
| `0x001d7864` | 8 B | Rodata at `0x002249dc` | Confirmed: exact tail at `0x001d7868` into retained `0x001d786c` |
| `0x001d80a8` | 8 B | Rodata at `0x002249d8` | Confirmed: exact tail at `0x001d80ac` into retained `0x001d80b0` |
| `0x001d89dc` | 8 B | Rodata at `0x00224a50` | Confirmed: exact tail at `0x001d89e0` into retained `0x001d89e4` |
| `0x001f3e78` | 20 B | Rodata at `0x0022496c` | Confirmed: one argument-memory load plus three register/constant setup operations, no store/SP/LR change; exact tail at `0x001f3e88` into retained `0x001f3e8c` |

For the callback, the pointer from `0x00107f30` is loaded into **r1** by owner `0x00107f0c` and passed at call site **`0x00107f1c`** to **`0x0019f338`**. The latter stores the still-unmodified incoming callback in its registration record through a multiple-register memory store at **`0x0019f36c`**, before the first definition of r1. This was checked as register/value-origin metadata; no packet body or original instruction is reproduced here.

For genuinely new starts, v7 has no function record to packetize. The review therefore retains their v8 packets plus v7 neighboring-owner/context packets, rather than pretending a v7 packet exists for an uncatalogued address.

## Per-change confirmation: runtime classifications

The integrator's selected standard permits original ABI-contract behavior plus matching compiler helper emission, a bound whole natural caller MATCH, or named relocation-free compiler-generated whole-body identity. Ordinary libc/libm needs the stronger known-library identity. These are different gates.

The following are the **17 ordinary-function to runtime changes / 1,286 B**. “Confirmed” means the exact image/extent/ISA and stated evidence meet that selected standard with V8-R05's retention qualification; it does **not** mean the old compiler/emulation probes were freshly executed.

| Address | Extent | Identity | Per-change evidence confirmation |
| --- | ---: | --- | --- |
| `0x001070c0` | 40 B | `__aeabi_lasr` | Confirmed: retained 404 original-ABI cases plus ARMCC `probe_lasr` emission; no unsupported whole-caller MATCH claim |
| `0x00126f2c` | 24 B | `__ARM_common_memset4_5` | Confirmed: independently computed whole-original-extent digest equals the published named, relocation-free compiler-section identity |
| `0x00130f90` | 16 B | `__ARM_common_memclr4_8` | Confirmed: independent whole-extent digest equality |
| `0x00131958` | 20 B | `__ARM_common_memcpy4_8` | Confirmed: independent whole-extent digest equality |
| `0x00152964` | 20 B | `__ARM_common_memclr4_12` | Confirmed: independent whole-extent digest equality |
| `0x0017da60` | 20 B | `__ARM_common_memcpy4_6` | Confirmed: independent whole-extent digest equality; no invented overlay binding |
| `0x00183458` | 20 B | `__ARM_common_memcpy4_5` | Confirmed: independent whole-extent digest equality |
| `0x00186454` | 48 B | `__aeabi_uidivmod` | Confirmed: current names-overlay binding, retained natural unsigned-division caller MATCH at `0x0017e5f4` over 64 B, plus 530 ABI cases |
| `0x0018cb4c` | 16 B | `__aeabi_memset` | Confirmed: current binding, retained natural packed-settings/fill caller MATCH at `0x001d0110` over 84 B, plus 50 ABI cases; AEABI fill argument order is distinguished from ordinary memset |
| `0x0018d204` | 80 B | `__aeabi_ldivmod` | Confirmed: retained 530 signed-64 ABI cases plus emitted division/remainder helper symbols; no whole-caller proof is asserted |
| `0x001911cc` | 34 B, Thumb | `__aeabi_vec_ctor_nocookie_nodtor` | Confirmed: current binding and retained natural member-array constructor MATCH at `0x0014d2e4` over 36 B, plus 17 original-helper cases |
| `0x001941f8` | 692 B | `__aeabi_uldivmod` | Confirmed: retained 530 unsigned-64 ABI cases plus emitted unsigned division/remainder helper symbols |
| `0x00196498` | 80 B | `__aeabi_f2ulz` | Confirmed: retained 404 ABI cases plus emitted helper; finite, nonnegative, in-range conversion contract, not an invented exceptional-input coverage claim |
| `0x00196504` | 8 B | `__aeabi_ul2f` | Confirmed: retained 407 ABI cases plus emitted helper; base/softfp ABI and shared conversion-core relationship are retained |
| `0x001a000c` | 136 B | `__aeabi_memcpy` / `__aeabi_memcpy4` | Confirmed: current same-address aliases and retained whole natural callers at `0x00182744` / 28 B and `0x00154378` / 64 B, plus 50 ABI cases |
| `0x001d481c` | 16 B | `__ARM_common_memclr4_8` | Confirmed: independent whole-extent digest equality; remains a separate address identity from `0x00130f90` |
| `0x001d5560` | 16 B | `__ARM_common_memclr4_5` | Confirmed: independent whole-extent digest equality despite no recorded direct transfer |

The eight common-helper hash comparisons are all true in `classification-audit.json`; these comparisons hash original extents locally without emitting their contents.

Two further evidence additions do **not** remove more C credit:

| Owner | Extent | Evidence addition | Structural confirmation |
| --- | ---: | --- | --- |
| `0x001864dc` | 64 B | `__aeabi_memclr` / `__aeabi_memclr4`: two retained natural zero-fill caller proofs and 50 ABI cases | Remains `multi_entry`; exact prior extent and all entries retained |
| `0x0019f6d8` | 556 B | `__aeabi_idivmod`: 530 retained ABI cases plus emitted signed division/remainder symbols | Remains `multi_entry`; exact prior extent and all entries retained; no claimed natural-caller MATCH |

Thus **19 runtime-evidence owners** comprise 17 newly excluded ordinary functions and two already excluded multi-entry owners. The candidate's 27 classification records are **19 + 8**, not 27 newly removed ordinary functions.

### Ordinary library and Thumb safeguards

- There are **no `library` kinds** introduced. In particular, `strlen` at `0x00192b78` stays an ordinary 96-B creditable function despite a names-overlay/natural-caller proof: that does not authenticate an exact library implementation.
- All 67 Thumb residue dispositions are retained: the one bound array-construction helper is `runtime`; the other **66 remain creditable**. Thumb mode is not used as a provenance classification.
- All nine shape-only literal SVC adapters remain ordinary: `0x001109b4`, `0x0011ef2c`, `0x0011ef4c`, `0x0011ef58`, `0x0012d0ec`, `0x0012d150`, `0x00145610`, `0x001565b0`, `0x0018f7d4`.
- No larger IPC/SVC-bearing function or ordinary string/math implementation is excluded merely for neighborhood, semantics, privileged instruction use or a runtime-shaped call graph.

## Per-change confirmation: all eight kernel-ABI classifications

All eight exact image functions contain the expected single service number. The five hard-limit rows are admitted for an unrepresentable raw register contract, not authenticated SDK ownership. The three no-frame rows use the specifically retained corrected compiler-grid evidence, not a generic failed C attempt.

| Address | Extent | Kernel ABI / service | Standard and confirmation |
| --- | ---: | --- | --- |
| `0x00100d6c` | 4 B | ExitProcess / `0x03` | Confirmed original no-frame shape; retained corrected 16-cell grid fails at offset zero in every cell; additional negative attempt is reported but not freshly rerun |
| `0x001109cc` | 48 B | QueryMemory / `0x02` | Confirmed raw-result marshaling consumes r1-r5; retained #1505 proof rejects six result words, r0-r5, in ARMCC integer-result registers |
| `0x0011ef08` | 36 B | CreateThread / `0x08` | Confirmed literal service adapter; retained #1501 proof for required processor-ID input in r4 |
| `0x0012d148` | 8 B | ExitThread / `0x09` | Confirmed no-frame shape; same specifically corrected 16-cell negative grid |
| `0x0012d168` | 36 B | WaitSynchronizationN / `0x25` | Confirmed literal service adapter; retained #1501 proof for timeout-high input in r4 |
| `0x00134e7c` | 8 B | Break / `0x3c` | Confirmed no-frame shape; same specifically corrected 16-cell negative grid |
| `0x00185108` | 24 B | ArbitrateAddress / `0x22` | Confirmed literal service adapter; retained #1501 proof for timeout inputs in r4/r5 |
| `0x00195cd8` | 36 B | ControlMemory / `0x01` | Confirmed literal service adapter; retained #1501 proof for permission input in r4 |

Total newly excluded assembly extents: **200 B**. The corrected grid's original-image hash agrees, its probe-source SHA-256 agrees with the retained source, and each of its three function rows has exactly **16 offset-zero failures**. This is inspection of retained evidence, not 48 newly run compiler checks.

SDK/archive/handwritten ownership beyond the demonstrated ABI/shape constraint remains **[INFERENCE]**. Public references corroborate contracts, not byte identity:

- [Arm runtime ABI](https://github.com/ARM-software/abi-aa/blob/main/rtabi32/rtabi32.rst): divmod, shifts, conversions, AEABI memory argument order and vector construction.
- [3dbrew Kernel ABI](https://www.3dbrew.org/wiki/Kernel_ABI): raw register ordering/results, including ControlMemory, QueryMemory and CreateThread.
- [libctru public service wrappers](https://github.com/devkitPro/libctru/blob/master/libctru/source/svc.s): additional raw timeout/marshaling contracts, notably ArbitrateAddress and WaitSynchronizationN; not a fingerprint of this image.
- [Arm SWP instruction semantics](https://developer.arm.com/documentation/ddi0406/c/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/SWP--SWPB): the atomic-swap class writes memory, which is relevant to V8-R06. The supplied ARMCC `__svc` documentation URL rendered only a documentation shell during this review; no text from that shell is claimed as independent compiler-limit evidence.

## Unknown bytes: every disposition re-derived

All **189** v7 unknown intervals were intersected with the actual v8 map independently of `revision_review` and compared with its accounting rows. The partitions match exactly and conserve every byte.

- **49 whole old unknown ranges / 980 B** become function instruction/pool ownership.
- The other **140 ranges / 7,724 B** remain unknown, with the same interval coverage. There are no newly created unknown bytes.
- No old unknown byte is silently converted to guessed data or padding.
- Every newly claimed unknown partition belongs to one of the 22 evidenced repairs or 10 new starts above.
- An additional **36 formerly padding bytes** become evidenced code/pool ownership, confined to the newly recovered callback/pointer-prefix regions.

| Recovered ownership group | Old unknown bytes consumed | Old padding bytes consumed | Added owned bytes |
| --- | ---: | ---: | ---: |
| 22 leading repairs | 228 | 0 | 228 |
| Callback `0x00110cc8` | 476 | 8 | 484 |
| Long body `0x0014b358` | 224 | 0 | 224 |
| Leaf `0x001c5260` | 12 | 0 | 12 |
| Seven pointer-prefix wrappers | 40 | 28 | 68 |
| **Total** | **980** | **36** | **1,016** |

The 1,016 newly owned bytes independently decompose into **908 instruction bytes + 108 pool bytes + 0 literal bytes**. All old instruction, pool and addressed-literal coverage is retained.

Still-open ranges are not asserted to be data merely because of plausible decoding, string-like content, pointer-shaped values or address proximity. Existing literal conflicts remain open rather than being solved by stealing executable ownership.

## Anomalies: all 83 old dispositions re-derived

The old and new anomaly sets were compared after the 22 legitimate owner-address replacements. There are **13 resolved** and **70 open** issues, with no new issue other than the expected rekeying of one still-open owner issue.

Every resolution is an `unexplained_function_bytes` issue whose old hole is now exactly covered by recovered pool ownership:

| Old owner | Corrected owner | Old unexplained hole | New unexplained hole |
| --- | --- | ---: | ---: |
| `0x001ce684` | `0x001ce67c` | 8 B | 0 B |
| `0x001f50dc` | `0x001f50d8` | 4 B | 0 B |
| `0x001f5180` | `0x001f5170` | 12 B | 0 B |
| `0x001f5678` | `0x001f5674` | 4 B | 0 B |
| `0x001f5788` | `0x001f5780` | 8 B | 0 B |
| `0x001f5810` | `0x001f580c` | 4 B | 0 B |
| `0x001f5870` | `0x001f586c` | 4 B | 0 B |
| `0x00203814` | `0x00203810` | 4 B | 0 B |
| `0x00205284` | `0x00205280` | 4 B | 0 B |
| `0x0020693c` | `0x00206938` | 4 B | 0 B |
| `0x00214588` | `0x00214580` | 4 B | 0 B |
| `0x00214d28` | `0x00214d18` | 8 B | 0 B |
| `0x00215130` | `0x00215120` | 8 B | 0 B |
| **Total** | | **76 B** | **0 B** |

The still-open `unexplained_function_bytes` issue at `0x001456e8` moves to `0x001456e4`; it is **not** counted as resolved. Unexplained-byte issues decline from 41 to 28. All other issue categories retain their counts: seven address-literal crossings, five address-literal/code overlaps, 15 out-of-function literals, 14 shared addressed literals, and one undecodable reachable-code issue. Their unresolved status is not erased by classification.

## Protected records and complete preservation

Manifest addresses were resolved from the actual names overlay, then complete adopted/candidate records were compared. These are record checks, not merely address/size comparisons.

| Manifest | SHA-256 | Functions | Extent bytes | Whole records identical |
| --- | --- | ---: | ---: | ---: |
| `config/matches.json` | `fd31d2cd50424520fe6e7444fe70b553dc76e1b2a2abb377eb61e8ef85cd11e4` | 299 | 7,620 | **299/299** |
| `config/candidates/c004.json` | `2626c2911fae00d433e162c87b8835cac0415fd0baf6e406610cd96666130af8` | 196 | 2,564 | **196/196** |
| `config/candidates/c005.json` | `514701fedc425d536f37fed07472173d99e5fd0015d837265ca3fcd04dff7df5` | 190 | 11,668 | **190/190** |

C004 is already included in the accepted manifest; there are **489 unique protected addresses / 19,288 B**, not 685 distinct functions. Every protected address remains an ordinary top-level function. There are no changed protected records or size exceptions in the supplied candidate. No future/uninspected C006 contents are claimed protected.

Further complete-record results:

- Exactly **22 old starts are removed**, precisely the listed weak-prologue repairs; exactly their 22 old callable address/ISA pairs are replaced.
- Exactly **32 starts are added**: the 22 corrected starts plus the 10 newly discovered functions.
- Exactly **27 same-address old records change**, and every change is one listed classification/evidence addition.
- The other **4,531 old owner records are identical**.
- All other old starts and secondary entries survive in the same ISA.
- The same segments still tile **1,327,104 mapped bytes**; independent structural validation returns **`[]`**.

V8-R02 is therefore a future/conflicting-input guard defect, not an allegation that the supplied candidate changed an accepted function.

## Denominator and extent arithmetic

| Measure | Adopted v7 | Supplied v8 | Plain analyzer rerun |
| --- | ---: | ---: | ---: |
| Top-level owners | 4,580 | 4,590 | 4,590 |
| Callable starts/entries | 4,604 | 4,614 | 4,614 |
| Creditable ordinary functions | 4,557 | **4,542** | 4,567 |
| Creditable extent bytes | 1,129,946 | **1,129,400** | 1,130,886 |
| Sum of all function extents | 1,136,930 | 1,137,870 | 1,137,870 |
| Physically function-owned bytes | 1,135,444 | 1,136,460 | 1,136,460 |
| Instruction bytes | 1,054,598 | 1,055,506 | 1,055,506 |
| Pool bytes | 39,988 | 40,096 | 40,096 |
| Addressed-literal bytes | 40,858 | 40,858 | 40,858 |
| Unknown bytes / ranges | 8,704 / 189 | **7,724 / 140** | 7,724 / 140 |
| Anomalies | 83 | **70** | 70 |

The supplied kind counts are **4,542 function + 17 runtime + 8 assembly + 17 multi_entry + 6 veneer = 4,590**.

The balances are exact:

- Function count: **4,557 + 10 − 17 − 8 = 4,542**. The two annotated multi-entry owners are not subtracted again.
- Creditable bytes: **1,129,946 + 152 + 788 − 1,286 − 200 = 1,129,400**.
- All-kind extent growth: **152 B of leading-repair growth + 788 B of new extents = 940 B**.
- Owned-map growth: **940 B of new extent coverage + 76 B of filled old holes = 1,016 B**.
- Extent holes: **1,136,930 − 1,135,444 = 1,486 B** before, versus **1,137,870 − 1,136,460 = 1,410 B** after; the difference is exactly the 76 B closed above.
- Physical map growth: **980 fewer unknown bytes + 36 fewer padding bytes = 1,016 more function-owned bytes**. Data, rodata and segment padding are unchanged.

These are denominator/map corrections, not newly authored or accepted C progress. The remaining 1,410 B of holes within function extents are still represented honestly by the map and anomalies; extent totals must not be mistaken for fully explained code coverage.

## Retained review evidence

All paths below are relative to `scratch/review/catalogue-v8/`:

- `commands.json`, `regeneration.log`, `tests.log`: actual generation/test commands, exits, timings and captured output.
- `rerun.json`: the requested independent plain-analyzer output, with the nonmatching SHA recorded above.
- `metadata-audit.json`: complete protected comparisons, source identities, record changes, coverage intervals and exact denominator balances.
- `structure-audit.json`: independently derived prefix/dependency, CFG, pointer, callback-origin and residual-boundary facts; includes the call-may-read qualification.
- `wrapper-audit.json`, `setup-safety-audit.json`: exact-tail/successor/register/flag facts and the explicit nonstoring instruction-class check. The long wrapper's `register_only` field is deliberately false, not hidden.
- `classification-audit.json`: original common-helper digests, service numbers, preserved core-catalogue comparison and retained no-frame grid/source identity checks.
- `disposition-audit.json`: all closed-hole sizes, old-unknown interval dispositions, Thumb and shape-only SVC retention.
- `protected-classification-probe.json`: the isolated synthetic V8-R02 demonstration.
- `pointer-side-effect-probe.json`: isolated synthetic memory-writing instruction admissions in both new recovery rules; distinguishes architectural memory writes from Capstone's missing operand WRITE flags.
- `packet-commands.json`, `packet-checks.json`, `packets/v7/`, `packets/v8/`: all 116 private packet invocations, exits, hashes, sizes and instruction-address coverage checks.

**Adoption recommendation:** do not adopt this exact handoff as an unqualified PASS. Resolve the prescribed-entrypoint mismatch and the two new guard-contract failures, and explicitly disposition the two reported unsplit boundaries. The independently confirmed image coverage, protected-record equality and denominator arithmetic above remain useful evidence; they do not substitute for those failed review criteria.
