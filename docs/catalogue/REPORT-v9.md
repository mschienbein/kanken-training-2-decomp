# Candidate catalogue v9 — recovered proposal, not adopted

**Candidate only. No accepted C progress, catalogue adoption, gate change, or claim of complete recovery.** This resumes branch `catalogue-v9` at `c8bb652e16d19e7139a440b90020549d4e9a317e` and its interrupted dirty implementation; it does not restart the work. All edits and generated artifacts stay in `scratch/worktrees/catalogue-v9`. Main-checkout configuration, sources/headers, accepted receipts, progress/denominator history, and compiler profiles are unchanged.

- Adopted v8 input: `config/catalogue.json`, SHA-256 **`e33aad973846b60b5286be6becb5080c9626188fa9a64a6c0b48197b16e8adad`**.
- Final proposal: `scratch/worktrees/catalogue-v9/scratch/analyze/catalogue-v9.json`.
- Complete proposal SHA-256: **`9b13a770438c91b48ea34d8b2445f41c87ae2543a696618360a0ff550ddfe460`**.
- Independent plain-CLI output: `scratch/worktrees/catalogue-v9/scratch/verification/catalogue-v9-final-rerun.json`, the same SHA-256 and complete bytes.
- The interrupted `scratch/verification/catalogue-v9-rerun.json` is retained unchanged, SHA-256 `7f36c59ce6f4abc6f037d578f77db5f4c29a1fdde5a15cc68a5dbacd3b603be0`. Its pre-final source/evidence hashes and totals are historical, not the final proposal.
- Public evidence input: `research/catalogue_v9_evidence.json`; private retained inputs are identified by path and digest, not executable payload. There are no original bytes, disassembly, packets or secrets in this report.

## Reproduction and executed operations

Run from `scratch/worktrees/catalogue-v9`, with that worktree's recorded inputs and the pinned c008 protection blob:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out /home/michael/code/kanken-training-2-decomp/scratch/worktrees/catalogue-v9/scratch/analyze/catalogue-v9.json
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out /home/michael/code/kanken-training-2-decomp/scratch/worktrees/catalogue-v9/scratch/verification/catalogue-v9-final-rerun.json
```

Each command is the entire generation procedure: no postprocessor, active-author scan, optional classification argument, or manual candidate edit. The second command runs a separate process with a different output name. Output pathname and elapsed time are not serialized. Input/source hashes deliberately are: altered inputs produce a different proposal.

| Exercised on resume | Exit | Observed result |
| --- | ---: | --- |
| Final plain-CLI generation | **0** | 4,699 owners, 4,417 unknown B, 37 anomalies; 27.993 s captured wall time |
| Separate plain-CLI regeneration | **0** | Complete byte equality and identical SHA-256; 27.619 s captured wall time |
| Generation-time protection, entry preservation, structural and accounting guards | **0** | 1,602 complete protected records unchanged; all 4,616 prior entries retained; structural issues and unexpected entry/extent changes empty |
| Read of resumed branch tip | **0** | `c8bb652e16d19e7139a440b90020549d4e9a317e` |

Private receipts: `scratch/verification/catalogue-v9-final-generation.json`, `catalogue-v9-final-regeneration.json`, and `catalogue-v9-final-differences.json` in the assigned worktree. The differences receipt enumerates every changed function field, all remaining unknown intervals, every old anomaly's disposition, protected summaries, and both complete digest identities.

**Not run on resume:** tests (including new synthetic tests), suites, full builds, lint/formatters, compiler/ABI probes, smoke runs, prepare/finalize, adoption or push. Existing Thumb and kernel-grid execution claims below are retained evidence from interrupted work, not fresh executions. The main integrator must run the full checks once after integration, including `tests.test_analyze`, then obtain independent review before adoption.

## Totals versus adopted v8

| Measure | v8 | v9 proposal | Delta |
| --- | ---: | ---: | ---: |
| Top-level owners | 4,592 | 4,699 | +107 |
| Entry addresses/ISAs | 4,616 | 4,730 | +114; no prior loss |
| Ordinary creditable functions | 4,544 | **4,624** | **+80** |
| Creditable extent bytes | 1,129,400 | **1,131,454** | **+2,054** |
| Nonordinary/topology-excluded owners | 48 | **75** | +27 |
| Nonordinary extent bytes | 8,470 | 9,568 | +1,098 |
| All-kind extent bytes | 1,137,870 | 1,141,022 | +3,152 |
| Physically function-owned bytes | 1,136,460 | 1,139,722 | +3,262 |
| Instruction bytes | 1,055,506 | 1,057,946 | +2,440 |
| Pool bytes | 40,096 | 40,792 | +696 |
| Addressed-literal bytes | 40,858 | 40,984 | +126 |
| Shared/unowned rodata bytes in text | 0 | 93 | +93 |
| Unknown bytes | 7,724 | **4,417** | **−3,307** |
| Unknown ranges | 140 | **87** | −53 |
| Anomalies | 70 | **37** | −33 net: 35 old resolved, 2 new exposed |

Final kinds: **4,624 function + 24 multi_entry + 22 runtime + 11 assembly + 18 veneer = 4,699 owners**; 4,622 ARM and 77 Thumb. Extent bytes by nonordinary kind: multi_entry 7,760; runtime 1,404; assembly 260; veneer 144. There are **35 classification-evidence records**, including the two inherited runtime-evidenced multi-entry owners. There are **zero known-library exclusions**. Topological exclusion is not proof of non-C ownership or irrecoverable C eligibility.

Extent totals still exceed physically owned bytes by **1,300 B**. Neither those holes nor shared external data are silently credited. The full image map continues to tile 1,327,104 B exactly.

The 93 previously unknown initializer targets all become top-level owners. Five replace weak interior prologue-only owners; the other additions include four tail partitions, newly exposed Thumb continuations, six independently pointer-witnessed leaves, and one newly found veneer. Of the 4,592 v8 records, **4,536 are byte-for-byte identical as metadata; 50 change and 6 cease being top-level owners**. Every old entry survives: five become explicit legacy secondary labels and the sixth remains a genuinely observed entry inside a recovered Thumb owner.

## Complete prior-entry and accepted-identity protection

The accepted c007 population remains **1,167 identities / 73,388 B**, protected by the accepted ledger snapshot plus c007 manifest. The pinned c008 candidate contributes another 435 identities / 112,764 B. Their union is **1,602 complete records / 186,152 B**, unchanged after boundary recovery and classification. No accepted credit is withdrawn, moved or awarded by this proposal.

| Protected input | Function rows | Extent B | Complete-record changes |
| --- | ---: | ---: | ---: |
| `config/matches.json` (pre-c007 snapshot) | 870 | 50,012 | 0 |
| `config/candidates/c004.json` | 196 | 2,564 | 0 |
| `config/candidates/c005.json` | 190 | 11,668 | 0 |
| `config/candidates/c006.json` | 381 | 30,724 | 0 |
| `config/candidates/c007.json` | 297 | 23,376 | 0 |
| c008 immutable Git blob | 435 | 112,764 | 0 |

Rows overlap; they must not be summed as distinct credit. c008 reference: `c008-integrate:config/candidates/c008.json`, blob `ec64c31302ef6631530d909e0e3ec3bd8494f33a`, SHA-256 `d8f49a991dde5c659f76f833de228342ff8fce9726288f55abae3f27055596b2`. No mutable branch-tip lookup determines generation. All bundled classification conflicts would be explicitly withheld; the final withheld list is empty.

Ten newly observed incoming sites and one additional start-evidence observation on protected functions are recorded separately in `protected_entry_observations` / `protected_start_observations`, instead of changing accepted entry metadata. The entire adopted record, not just address/size, is compared after classification.

### Five legacy prologue labels retained conservatively

Each old address is an aligned reachable ARM instruction boundary inside the recovered initializer, and every old code/pool/literal span is retained. The initializer's stronger start evidence is `relative_init_array`; the old label is an explicit `secondary_entry` with **`legacy_prologue`**, no incoming sources, and no claim of independently established callability.

| Prior top-level start / size | New initializer owner / size | Preserved old label |
| --- | --- | --- |
| `0x001f5990` / 32 B | `0x001f5980` / 56 B | `0x001f5990`, secondary, legacy prologue |
| `0x002069f0` / 92 B | `0x002069ec` / 96 B | `0x002069f0`, secondary, legacy prologue |
| `0x00212684` / 68 B | `0x00212674` / 84 B | `0x00212684`, secondary, legacy prologue |
| `0x002136d0` / 44 B | `0x002136c4` / 56 B | `0x002136d0`, secondary, legacy prologue |
| `0x00214fec` / 308 B | `0x00214fe8` / 312 B | `0x00214fec`, secondary, legacy prologue |

These five new owners are conservatively `multi_entry`. Compared with the interrupted proposal that discarded those labels, preservation removes **5 functions / 604 extent B** from proposed credit; it creates no extra credit. Compared with v8, the five previous ordinary owners accounted for 544 B and their recovered prefixes add 60 uncredited B. `legacy_entry_preservation` and `revision_review.boundary_repairs` record exact mappings. Any other lost prior entry fails generation. Independent review must check these mappings, not assume that the old prologue guesses were genuine independent routines.

## Thumb: four runtime proposals, all 56 ownership-unresolved rows retained

The input remains the exact 60-function / 4,242-B Thumb cohort. The four following records meet the proposed inherited compiler-ABI admission standard, **not** known-library whole-body identity:

| Address | Extent B | Proposed runtime contract | Retained witness |
| --- | ---: | --- | --- |
| `0x001008ac` | 36 | `.init_array` startup walker; `__cpp_initialize__aeabi_` spelling [INFERENCE] | Natural C++ emits `.init_array` / `R_ARM_TARGET1`; original startup walker traversed all 160 relative entries in order |
| `0x00100c80` | 18 | `__aeabi_vec_dtor_cookie` | Natural delete-array caller matched 52/52 B on three profiles, no rewrites; 25 cookie cases plus null |
| `0x00100c9e` | 10 | `__cxa_pure_virtual` | Independent abstract-class compiler relocations; 83 static pointer sites and six signal-12 dispatch experiments |
| `0x0019f6b4` | 34 | `__aeabi_vec_dtor` | Independent member-array destruction emission; 25 original helper contract cases |

Their isolated ownership-classification delta is **−4 functions / −98 B**, never accepted C credit. Source records are copied from `scratch/integrator/thumb/proposed-v9-entries.json`, with compiler/emulation provenance preserved. The walker deliberately uses a **section ABI** witness, not an invented direct emitted-symbol binding; independent review must adjudicate that distinction.

**All other 56 / 4,144 original extent B remain explicitly ownership-unresolved.** None is relabeled runtime/library merely because matching is difficult. There is no frozen Thumb game/SDK profile: the retained eight-cell, ten-source sweep found two tiny witnesses matching all eight cells, not a discriminating compiler population; the inspected compiler archive contains no runtime `.a/.lib/.o/.obj` body source for authentication.

A stronger entry graph independently changes topology for **two** of the 56. Thus **54 remain ordinary creditable (3,952 original extent B)**; the other two remain unresolved residue whose C eligibility is topology-excluded pending review, not proven non-C ownership:

| Original extent / entry | Proposed extent / entries | Incoming-edge evidence and denominator effect |
| --- | --- | --- |
| `[0x001000a0, 0x0010010c)`, 108 B, entry `0x001000a0` | `[0x00100084, 0x0010010c)`, 136 B, Thumb `multi_entry`; entries `0x00100084` and **preserved `0x001000a0`** | Canonical veneer at `0x0010007c` exposes `0x00100084`; observed fallthrough reaches the old entry. The old BLX site **`0x0012db30`** to `0x001000a0` remains. Credit delta **−1 / −108 B**; the newly exposed 28 B are uncredited topology, not runtime proof. |
| `[0x0010030c, 0x00100360)`, 84 B, entry `0x0010030c` | Same 84-B extent, Thumb `multi_entry`; entries **`0x0010030c`** and `0x00100312` | Existing pointer witness at pool **`0x00100358`** retains the old start. Newly recovered Thumb call site **`0x00100086`** targets the interior entry `0x00100312`; the entry group is connected by fallthrough. Credit delta **−1 / −84 B**; ownership remains unresolved. |

Combined topology delta for those two is **−2 functions / −192 creditable B**; the new multi-entry extents total 220 B. Their evidence and unresolved role records remain in `thumb_v9_residue`, with `candidate_owner`, `candidate_owner_kind`, and `ownership_classified: false`. The two maps and incoming edges require independent review. Erasing the observed edges to force 56 ordinary owners would be dishonest.

## Compiler helper, kernel stubs, and author dispositions

The complete metadata-only author snapshot contains **82 distinct library_candidate rows**, each with original report path/hash, source row, extent, notes, and explicit decision. Generation does not scan changing author directories. Final dispositions are: **57 retain ordinary creditability; 9 retain prior runtime; 1 newly proven compiler helper; 11 structural veneers; 3 proposed assembly stubs; 1 preserved legacy entry in a multi-entry initializer**. The last is the old `0x001f5990` label above, not a new library ownership exclusion.

Ordinary libc/libm, unbound software-FP/private-ABI hypotheses, virtual adjustment thunks without stronger independent provenance, TLS/barrier/intrinsic operations, and merely named `strlen`/`strcmp` candidates do not become known-library exclusions. A compiler mismatch, author avoidance, or API resemblance is not identity proof.

- **`0x001847c8`, 20 B:** the retained relocation-free named ARMCC section `i.__ARM_common_memclr4_10` equals the whole extent. Object SHA-256 `ca21cdbcf33d529d8f8ac9b091b40ca49b36fb52af7734ee8789156fcd9d44b3`; full-extent digest `492eb185d4dde78c7f83992286756e363cacafdd2ccae26982bc839ee17f5187`. This is a compiler-generated helper proposal, not authored-C credit. The source object and historical equality witness are retained privately; not recompiled on resume.
- **x20 `0x0011ef2c`, 28 B:** GetProcessInfo SVC `0x2b` output adapter; proposed assembly/no-frame ABI classification.
- **x20 `0x0011ef4c`, 8 B:** GetSystemTick SVC `0x28` no-frame stub; proposed assembly classification.
- **x22 `0x001565b0`, 24 B:** DuplicateHandle SVC `0x27` output adapter; proposed assembly/no-frame ABI classification.
- **x21 `0x00145610`, 28 B:** **CreateMemoryBlock** output adapter, **unchanged ordinary creditable function**. The earlier 24-B DuplicateHandle description is stale. Its shared `__svc(0x1e)` interface is representable; three failed frozen-profile source probes do not establish non-C status.

The three proposed no-frame exclusions bind the literal kernel ABI/observed frame constraint to the inherited v8 evidence standard, with authored shared-header C probes in `research/compid/v9/`. The retained grid covers four compiler builds × four flagsets × three functions = **48 cells**, all differing at offset zero, **zero compile failures**. Historical `compid.py` exit **1** means no matching cell, not a failed compiler. Grid digest: `7525f90ecf37501f0903d2e86db7cc3f294ec7230967d3c44923318a353b7e51`.

**Qualification:** that negative grid is bounded evidence for the specified no-frame emission constraint, not a proof that no conceivable C spelling can ever match. SDK/archive/handwritten ownership is [INFERENCE], not authenticated library-body identity. Independent review must apply the existing v8 standard separately to these three; no new gate or blanket “SVC is non-C” rule is proposed. Approved ARMCC extensions remain C-authorable.

## Init-array and boundary queue disposition

The authenticated relative table is `[0x00235e38, 0x002360b8)`, **160 entries**. Every entry is rederived from its own site plus its 32-bit relative value. The walker pool-derived bounds are checked against the image. **67 targets were already callable; 93 were previously unknown** and are explicitly listed in `initializer_targets_previously_unknown`. Every target has a final entry. A bounded reviewed section witness is required: this is not a free relative-pointer scan.

| Queue item | Final disposition / evidence |
| --- | --- |
| `0x001bc29c` literal, owner `0x001bc220` | Exact shared empty NUL string is **one unowned rodata byte**. Three following nonzero/unwitnessed bytes remain unknown. No owner growth. |
| `0x0010bef4` wrapper | Old `0x0010bed4` / 36 B partitions **32 + 4 B**, exact tail site `0x0010bef0`; independently restored frame, closed unconditional transfer to known returning ARM owner `0x001135e8`, no crossing. |
| `0x001372ec` literal through `0x001375b0` | Extent **688 → 708 B**. Seven exact UTF-16LE object records have witnessed halfword-formatting use, exact reference sites and complete terminators; final literal span `[0x00137520, 0x001375b0)`. No code/entry crossing is overridden. |
| Entry `0x001d0348` | `0x001d033c` / 68 B partitions **12 + 56 B**, exact tail site `0x001d0344`; nonstoring prefix and closed scalar-memory R0-input CFG with definite initialization at joins. |
| Boundary `0x001f478c` | `0x001f4784` / 48 B partitions **8 + 40 B**, exact tail site `0x001f4788`; closed pointer/count leaf CFG. |
| Wrapper/body `0x001d2234` | `0x001d222c` / 60 B partitions **8 + 52 B**, exact tail site `0x001d2230`; closed scalar-memory leaf CFG. |
| `0x0014e510` split request | **No new split.** This is inside the existing `0x0014e50c` / 336-B owner immediately after its prologue; matching frame return and no incoming entry witness. The preceding existing tail boundary already separates the wrapper. |
| `0x0016bde4` split request | **No new split.** This is the vector-save continuation of the existing `0x0016bde0` / 416-B owner, not an independently witnessed entry. Existing preceding tail boundary is retained. |
| `0x001b8198` empty literal at `0x001b8234` | Exact **one-byte shared rodata** object; following three bytes remain unknown. No owner growth. |
| Gap/old start `0x002136d0` | Initializer owner repaired to **`0x002136c4` / 56 B**; pool `[0x002136ec, 0x002136fc)` closes the old internal pool hole. Old entry `0x002136d0` explicitly retained as a legacy secondary label. |
| Gap/old start `0x00212684` | Initializer owner repaired to **`0x00212674` / 84 B**; pool `[0x002126b4, 0x002126c8)` closes the old pool hole. Old entry `0x00212684` explicitly retained as a legacy secondary label. |

Tail partitions add **4 owners, zero extent/physical bytes**; the inventory now has 143 split sites and retains the 776-word linker-NOP census. The generalized CFG rule rejects calls, unknown/out-of-owner exits, uninitialized register reads, stack/LR dependence, atomics/exclusives, writeback, pool or crossing ownership, saved frames and protected-record changes. It is not an address whitelist or a permission to split arbitrary hard-to-match code.

Canonical PC-relative interworking recovery produces **12 additional veneers**: 11 old ordinary starts at `0x0010007c`, `0x00100114`, `0x00100520`, `0x001008dc`, `0x0010090c`, `0x00100a6c`, `0x00100af0`, `0x00100c94`, `0x0019f5bc`, `0x0019f634`, `0x0019f6ac`, plus newly owned `0x001911c4`. Each is 8 B. Four old extents shrink: `0x0010007c` 12→8, `0x001008dc` 16→8, `0x0019f5bc` 16→8, `0x0019f634` 12→8; their exposed Thumb code is recovered separately, not hidden as a literal. This is architecture/topology evidence, not vendor-library identity.

## Unknown-byte and anomaly accounting

Every one of the **140 old unknown intervals / 7,724 B** has an image-interval digest, complete byte-conserving destination partition, explicit reason, and any pointer witnesses in `revision_review.unknown_accounting`:

- **3,214 B** become evidenced instruction/pool/literal ownership.
- **93 B** become exact shared bounded strings in unowned rodata.
- **4,417 B** remain unknown; **zero** old unknown bytes are renamed padding.
- Range dispositions: **53 resolved, 8 partial, 79 open**. No new unknown bytes are introduced outside the old unknown set.
- Another **48 formerly padding B** become function-owned. Thus function ownership grows 3,214 + 48 = **3,262 B**; total unknown reduction is 3,214 + 93 = **3,307 B**.

Shared-data recognition does not invent function ownership, string suffixes do not become independent callable entries, and nonzero/unreferenced alignment remainders remain open. Three large residual regions account for 3,892 B: fixed-copy gap family `[0x00184000, 0x00184700)` **1,108 B**, copy/move region `[0x0018e000, 0x0018f000)` **1,772 B**, and the unbounded-object gaps inside owner `0x00203a68` **1,012 B**. Their algorithm resemblance is not a bounded data-format or callable-entry proof.

| Anomaly kind | v8 | Old resolved | New exposed | Final |
| --- | ---: | ---: | ---: | ---: |
| Shared addressed literal | 14 | 14 | 0 | 0 |
| Literal outside owner | 15 | 7 | 0 | 8 |
| Addressed literal crosses start | 7 | 4 | 0 | 3 |
| Addressed literal overlaps code | 5 | 4 | 0 | 1 |
| Unexplained owner bytes | 28 | 6 | 2 | 24 |
| Undecodable reachable code | 1 | 0 | 0 | 1 |
| **Total** | **70** | **35** | **2** | **37** |

The two newly visible anomalies are the non-owned halfword holes in recovered Thumb owners **`0x0019f5c4`** and **`0x0019f63c`**. They are reported rather than hidden by veneer classification. Still-open literal conflicts remain at `0x00100372`, `0x00100a9a`, `0x00100aac`; cross-owner PC-relative loads at `0x00100532`, `0x00100538`, `0x0019f958`, `0x0019f95a`, `0x0019f9ce`, `0x0019f9d8` are not stolen into the loading owner's extent. Reachable exceptional-dispatch descriptor/nonreturn semantics at **`0x001964e8`** remain unproved: plausible data or a hypothesized runtime role is not enough to suppress that issue. Some unexplained owner bytes are shared rodata or known padding, not unknown bytes; the two measures are deliberately distinct.

### Every remaining unknown interval

Exclusive end; addresses and sizes only. Every row retains its old-interval accounting/witnesses in the private proposal and differences receipt. Missing evidence is a closed executable entry/CFG, exact bounded referenced data, or a zero-alignment witness—not a source-match score.

| Start | End | B |
| --- | --- | ---: |
| `0x00100020` | `0x00100024` | 4 |
| `0x00100076` | `0x00100078` | 2 |
| `0x0010012e` | `0x0010015e` | 48 |
| `0x00100396` | `0x00100398` | 2 |
| `0x0010051e` | `0x00100520` | 2 |
| `0x001005bc` | `0x001005ca` | 14 |
| `0x001005ea` | `0x001005ec` | 2 |
| `0x001005fa` | `0x0010060a` | 16 |
| `0x001007ce` | `0x001007d0` | 2 |
| `0x001008c6` | `0x001008c8` | 2 |
| `0x0010090a` | `0x0010090c` | 2 |
| `0x00100942` | `0x00100944` | 2 |
| `0x001009d4` | `0x001009de` | 10 |
| `0x001009fe` | `0x00100a00` | 2 |
| `0x00100a6a` | `0x00100a6c` | 2 |
| `0x00100ada` | `0x00100adc` | 2 |
| `0x00100ae0` | `0x00100ae4` | 4 |
| `0x00100b26` | `0x00100b28` | 2 |
| `0x00100c7a` | `0x00100c7c` | 2 |
| `0x00100c92` | `0x00100c94` | 2 |
| `0x00100d70` | `0x00100d74` | 4 |
| `0x00104720` | `0x00104740` | 32 |
| `0x0010d3a8` | `0x0010d3b0` | 8 |
| `0x001415b4` | `0x001415b8` | 4 |
| `0x001569b0` | `0x00156a30` | 128 |
| `0x0016067c` | `0x00160684` | 8 |
| `0x0016b6a2` | `0x0016b6a4` | 2 |
| `0x00181eaa` | `0x00181eac` | 2 |
| `0x00184204` | `0x0018423c` | 56 |
| `0x00184278` | `0x001842d8` | 96 |
| `0x0018433c` | `0x001846f8` | 956 |
| `0x0018e0dc` | `0x0018e14c` | 112 |
| `0x0018e1c0` | `0x0018e208` | 72 |
| `0x0018e270` | `0x0018e764` | 1268 |
| `0x0018eaec` | `0x0018ebf8` | 268 |
| `0x0018ec60` | `0x0018ec7c` | 28 |
| `0x0018ed0c` | `0x0018ed18` | 12 |
| `0x0018eda8` | `0x0018edb4` | 12 |
| `0x001911ee` | `0x001911f0` | 2 |
| `0x00191c92` | `0x00191c94` | 2 |
| `0x001944ac` | `0x001944e8` | 60 |
| `0x001964e8` | `0x00196504` | 28 |
| `0x0019d5c2` | `0x0019d5c4` | 2 |
| `0x0019f622` | `0x0019f624` | 2 |
| `0x0019f696` | `0x0019f698` | 2 |
| `0x0019f6d6` | `0x0019f6d8` | 2 |
| `0x0019f910` | `0x0019f912` | 2 |
| `0x0019fd14` | `0x0019fd20` | 12 |
| `0x0019fd6a` | `0x0019fd6c` | 2 |
| `0x0019ffa2` | `0x0019ffa4` | 2 |
| `0x001a00b4` | `0x001a00b8` | 4 |
| `0x001ad598` | `0x001ad59c` | 4 |
| `0x001b54e5` | `0x001b54e8` | 3 |
| `0x001b5a1c` | `0x001b5a20` | 4 |
| `0x001b5a24` | `0x001b5a28` | 4 |
| `0x001b8235` | `0x001b8238` | 3 |
| `0x001bc29d` | `0x001bc2a0` | 3 |
| `0x001c3c34` | `0x001c3c48` | 20 |
| `0x001c7748` | `0x001c775c` | 20 |
| `0x001cd2e6` | `0x001cd2e8` | 2 |
| `0x001de4e0` | `0x001de4e8` | 8 |
| `0x001e84dc` | `0x001e84e8` | 12 |
| `0x001ef550` | `0x001ef554` | 4 |
| `0x001ef558` | `0x001ef55c` | 4 |
| `0x00203ce4` | `0x00203d10` | 44 |
| `0x00203d1c` | `0x00203d4c` | 48 |
| `0x00203d54` | `0x00203d84` | 48 |
| `0x00203d94` | `0x00203db4` | 32 |
| `0x00203dc0` | `0x00203dfc` | 60 |
| `0x00203e04` | `0x00203e2c` | 40 |
| `0x00203e38` | `0x00203e60` | 40 |
| `0x00203e6c` | `0x00203e94` | 40 |
| `0x00203eb8` | `0x00203ee0` | 40 |
| `0x00203eec` | `0x00203f28` | 60 |
| `0x00203f34` | `0x00203f64` | 48 |
| `0x00203f6c` | `0x00203f9c` | 48 |
| `0x00203fb4` | `0x00203fd8` | 36 |
| `0x00203ff0` | `0x00204024` | 52 |
| `0x00204034` | `0x00204064` | 48 |
| `0x00204254` | `0x0020427c` | 40 |
| `0x00204288` | `0x002042c0` | 56 |
| `0x002042e4` | `0x002042f8` | 20 |
| `0x00204324` | `0x0020435c` | 56 |
| `0x00204364` | `0x0020438c` | 40 |
| `0x00204394` | `0x002043c8` | 52 |
| `0x002043e0` | `0x002043f0` | 16 |
| `0x00204414` | `0x00204444` | 48 |

## Implementation and review handoff

- `tools/analyze.py`: retained interrupted v9 recovery, section-ABI relative-table input, canonical veneers, exact/shared bounded data, reviewed halfword strings, guarded tail CFG splits, complete checkpoint protection, and explicit prior-entry preservation. `preserve_prior_entries` fails other entry losses and never supplies new C credit.
- `tools/catalogue_review.py`: deterministic image-dependent accounting, exact preservation/extent records, original author decisions plus current owner/topology disposition, and explicit unresolved Thumb residue. No live-directory ownership inference at generation time.
- `research/catalogue_v9_evidence.json`: frozen metadata-only 35-record classification input, 82 author rows, 56 unresolved Thumb records, 93 initializer targets, exact boundary queue, proof identities and kernel-adapter corrections. Historical statements remain labeled as inherited.
- `research/compid/v9/`: authored ordinary shared-header kernel probes, exact grid configuration and retained metadata-only grid result. They are reproducibility evidence, not credited source or executable byte fixtures.
- `tests/test_analyze.py`: retained v9 synthetic coverage for veneer guards, exact/shared strings and UTF-16 bounds, relative-table bounds and protected metadata, closed graph/tail splits, plus new legacy-label preservation/refusal cases. Whole-image expectations include all prior entries, five legacy maps, two unresolved Thumb topology maps, and the unchanged 28-B CreateMemoryBlock adapter. **These tests were not executed on resume.**
- `docs/LINKER.md`: recovery contracts and provenance/qualification, including explicit legacy labels. Gate policy remains unchanged.

Independent review must rederive the four section/direct-symbol Thumb runtime contracts; named helper whole-extent equality; three bounded no-frame ABI exclusions (separately from x21); all boundary splits/UTF-16/shared-object bounds; 160 table entries and 93 recovered targets; five legacy maps; two unresolved Thumb topology maps and incoming edges; complete protected records and old entries; all 140 old unknown partitions and 70 old anomaly dispositions. Residual unknowns, topological C eligibility, source TU provenance, vendor build/library ownership and exceptional dispatch remain explicit gaps, not closed claims.

Public reference contracts (not body-identity proofs): [Arm run-time ABI](https://github.com/ARM-software/abi-aa/blob/main/rtabi32/rtabi32.rst), [Arm C++ ABI](https://github.com/ARM-software/abi-aa/blob/main/cppabi32/cppabi32.rst), [Arm library guide, including initialization and pure-virtual roles](https://documentation-service.arm.com/static/5e99ac3a9931941038dd997c), [3dbrew Kernel ABI](https://www.3dbrew.org/wiki/Kernel_ABI), and [armlink user guide](https://documentation-service.arm.com/static/5ea19ea09931941038de96d2). Retained original research: `scratch/integrator/thumb/REPORT.md`, `scratch/integrator/thumb/residue-evidence.json`, `scratch/integrator/runtime-helpers/REPORT.md`, and the named author RESULTS. See adopted [v8 report](REPORT-v8.md) for the inherited admission qualifications.

### Deterministic generator/input identities

| Input | SHA-256 |
| --- | --- |
| `config/candidates/c004.json` | `2626c2911fae00d433e162c87b8835cac0415fd0baf6e406610cd96666130af8` |
| `config/candidates/c005.json` | `514701fedc425d536f37fed07472173d99e5fd0015d837265ca3fcd04dff7df5` |
| `config/candidates/c006.json` | `b760840461a9a00f239e55b277799222c2203a7b4ff972e051aaf217690c5781` |
| `config/candidates/c007.json` | `d2ca1af4874f59745376a5b1524fbaf1d4fa5bed9c6ada25e3fab4f7f90a90dc` |
| `config/catalogue.json` | `e33aad973846b60b5286be6becb5080c9626188fa9a64a6c0b48197b16e8adad` |
| `config/matches.json` | `3aedad0519db261fc438be736f19248c1ee93897afe9bdfe06eb33dcedebc075` |
| `config/names.json` | `3d53f1d590cab742785fc12f4c22e6731b2e4f7135715bdbde19eb35c7bb818a` |
| `exheader` | `0c43847e321bb81809c7f01489f6f321695646f6ec8e7131936b2eea9b75c6d8` |
| `git:c008-integrate:config/candidates/c008.json@ec64c31302ef6631530d909e0e3ec3bd8494f33a` | `d8f49a991dde5c659f76f833de228342ff8fce9726288f55abae3f27055596b2` |
| `image` | `a367b5dcd65e3924c93cf3fbaee5d4d40f9c5205d1287912784a974d5e405361` |
| `research/catalogue_v9_evidence.json` | `7eb548f332764b50b8be38a8a017d803b30060db760c4f62e7299158b5c5dfd4` |
| `tools/analyze.py` | `312fea5f722f36326c14a4b35487e6be8801cbf10386c657409166810b18ecd0` |
| `tools/catalogue_review.py` | `73b7947339b726247914f8bd0f40772a1105dcb0b8ab8d0dee22056212f22dc0` |

Adoption and any denominator/history update remain integrator-owned, after full checks and independent review. No old checkpoint, receipt or history record is rewritten.
