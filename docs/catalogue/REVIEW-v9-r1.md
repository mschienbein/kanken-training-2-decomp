# Independent review — catalogue v9, revision 9b18089

## Sealed verdict

**FAIL / overall_correctness: incorrect. Confidence: 0.99.** This verdict binds commit **9b1808944d2fed376aa04f523d15c13bb8bd4dbc** and proposal SHA-256 **9b13a770438c91b48ea34d8b2445f41c87ae2543a696618360a0ff550ddfe460**. Two actionable patch-introduced findings are established below. Do not adopt this exact proposal as a correct final revision.

The specified plain CLI does reproduce the proposal exactly from its frozen v8 inputs. Its map arithmetic, all 4,616 prior entry addresses/ISAs, all 1,602 protected complete records / 186,152 B, initializer recovery, conservative entry topology, and the narrowly qualified runtime/helper/kernel classifications check out. However, a newly retained natural wide-literal object contradicts the proposed one-byte empty-string interpretation, and the new legacy-entry preservation guard fails when v9 itself becomes the adopted baseline. The green pre-adoption suite does not exercise that second transition.

This is an independent OpenAI review, not an author/integrator review. No other v9 review was read before this seal. Reviewed rules include AGENTS.md, docs/ACCEPTANCE.md, docs/GATES.md, docs/REVIEWING.md, the adopted v8 report/re-review and v8 evidence, and the complete v9 patch plus consuming paths. No source, configuration, header, gate, ledger, accepted receipt, history or compiler profile was edited. No nested agent, compiler invocation, build, test suite, linter, formatter, prepare/finalize, adoption, commit or push was run by this reviewer. Review output is restricted to ignored scratch artifacts. No original bytes, disassembly, packet contents or secrets are reproduced here.

## Findings

### V9-R01 — [P2] Honor the witnessed wide-object bound for shared literals

- Affected file: scratch/worktrees/catalogue-v9/tools/analyze.py
- Diff anchor: **1413–1421**; related new width-selection code: 1375–1380 and 1279–1295.
- Priority: 2. Confidence: 1.00.

At the shared target **0x001bc29c**, the new shared-string path treats a leading zero byte as a complete empty byte string and closes the literal conflicts, then leaves the following three bytes unknown. The retained natural wchar_t source at scratch/authors/r4023/variants/func_001bc220_wc2.c instead supplies a complete four-byte wide object at that exact target: its unchanged game/v2/none object freshly links to the original for all **128 B**, with target extent 124 B, match_extent_differs, first_diff null and no rewrites. The source and object hashes match the retained receipt, and the four-byte object equals the original target span. Existing typed func_001eafa0 consumers also require UTF-16 text, rather than a byte-string empty value. Thus the new map, boundary-queue explanation and regression expecting one rodata byte plus three unreferenced bytes are not the witnessed data interpretation. Preserve sharing: the four-byte object witness does **not** establish exclusive ownership by func_001bc220 or authorize 124→128 C-extent credit. Record the evidenced wide bound as shared/unowned data, or keep the disputed classification explicitly unresolved, and update the affected accounting/docs/tests. Merely adding a utf-16le record is not sufficient with the present parser: _utf16_string_end rejects the legitimate ideographic-space character in this authored object because Python isprintable returns false for it.

Independent evidence details:

- Source SHA-256: **6734f1cab911d8595e29d59b3104e4bc3475cb2ac43c1db669268ce2b1ff6a7d**.
- Object SHA-256: **2e2735464c80fd6abc3da10d852717255a54188491e99cf8b99aafc2bf13dcae**.
- Receipt: scratch/authors/r4023/match/func_001bc220/wc2-game/func_001bc220_wc2.result.json.
- Fresh existing-object comparison: complete 128-byte equality, no literal rebinding, no linker rewrite; this was **not** a new compilation or an accepted C match.
- The original pointer census independently shows four other pool users of 0x001bc29c. This is why the source extent witness cannot override shared ownership.
- The one-byte interpretations at **0x001b8234** and **0x001b54e4** have the same complete four-byte contents; 0x001b8234 also has three other typed text users. These cases need the same width-aware adjudication rather than a blanket exclusive-extent repair. No numeric claim is made here about a revised proposal that has not been generated.

### V9-R02 — [P1] Preserve legacy labels after v9 becomes the adopted baseline

- Affected file: scratch/worktrees/catalogue-v9/tools/analyze.py
- Diff anchor: **2053–2058**; call path: main → build_catalogue → preserve_prior_entries.
- Priority: 1. Confidence: 1.00.

Once this proposal is installed as config/catalogue.json, the ordinary CLI supplies its already-preserved initializer multi_entry records as prior_functions. Fresh discovery does not create legacy_prologue labels by itself, but this guard only accepts an old single-entry ordinary function with prologue evidence; it rejects the existing multi_entry/legacy_prologue form. A focused generation using the exact reviewed proposal as the prior baseline failed with **ValueError: prior entry 0x1f5990/arm cannot be preserved**, exit **1**, after 23.04 s, with unchanged image, evidence, protection and implementation. No canonical input was modified to reproduce it. The new test_original_invariants_and_stability calls the same CLI against the current adopted catalogue, so the normal post-adoption generation/test path cannot work. Carry forward already-reviewed legacy labels under the same exact owner/ISA/code-span/protection constraints, or explicitly pin/replay the reviewed generation baseline; do not turn other entry losses into an exemption.

Focused reproduction used analyze.load_protected_functions with the reviewed v9 catalogue and its pinned c008 descriptor, then analyze.build_catalogue with the unchanged image/layout, the eight-added-plus-inherited classification records, literal_encodings, relative_pointer_tables, and **prior_functions=v9["functions"]**. This is catalogue generation, not a compilation/full build. The saved failure is not rerun solely to reconfirm it.

## Reviewed identities and reproducibility

The adopted v8 input SHA-256 is **e33aad973846b60b5286be6becb5080c9626188fa9a64a6c0b48197b16e8adad**. The private original image identity is **a367b5dcd65e3924c93cf3fbaee5d4d40f9c5205d1287912784a974d5e405361**. All image-dependent operations used that identity; none printed its payload.

The independently executed plain command was `/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out /home/michael/code/kanken-training-2-decomp/scratch/review/catalogue-v9/rerun.json`, run from scratch/worktrees/catalogue-v9. It exited **0**, reported 4,699 owners / 4,417 unknown B / 37 anomalies in 28.810 s (28.91 s captured wall), and the entire output byte string equals the supplied proposal. Both SHA-256 values are **9b13a770438c91b48ea34d8b2445f41c87ae2543a696618360a0ff550ddfe460**. No postprocessor or active-author scan was involved.

Seven principal implementation/test/evidence/report files were compared byte-for-byte with git show 9b18089 contents. The three public kernel probe sources also match the pinned grid source hashes. All **60** explicitly pinned public/private evidence files exist and match their recorded SHA-256 values when public files are resolved in the proposal worktree and private files in the main scratch tree. This is an explicit pinned-path check, not a mutable author-directory scan.

Exact reviewed principal hashes:

| Path | SHA-256 |
| --- | --- |
| tools/analyze.py | 312fea5f722f36326c14a4b35487e6be8801cbf10386c657409166810b18ecd0 |
| tools/catalogue_review.py | 73b7947339b726247914f8bd0f40772a1105dcb0b8ab8d0dee22056212f22dc0 |
| tests/test_analyze.py | bf0ab3789d236a3fcb445930eb498a10935de451d91a914a69184c7c26faf7d8 |
| docs/LINKER.md | 47ee1d064224c0ad10329e4ecd11a83165bbe24601ffcbe4bf9d098bcc4e63cb |
| docs/catalogue/REPORT-v9.md | 6e7ed551d132393c0722256e34668a05a2cfe2b06aa02d64965d894ff2dffb75 |
| research/catalogue_v9_evidence.json | 7eb548f332764b50b8be38a8a017d803b30060db760c4f62e7299158b5c5dfd4 |
| research/compid/v9/kernel_stub_grid_compid.json | 7525f90ecf37501f0903d2e86db7cc3f294ec7230967d3c44923318a353b7e51 |

## Executed evidence, separated from inherited/main-reported checks

| Operation | Exit | Observed result |
| --- | --- | --- |
| Independent plain CLI generation | 0 | Complete proposal equality and pinned hash, as above |
| Independent map/entry/protection/accounting audits | 0 | Exact coverage, counts and whole-record equality, detailed below |
| Seven natural compiler-emission objects inspected, not recompiled | 0 | All source/object hashes agree; actual relevant sections/relocations agree |
| Three retained natural array-delete caller objects relinked, not recompiled | 0 | game/sdk/middleware each MATCH 52/52 B, no rewrites |
| Original array destruction behavior | 0 | 25 vec_dtor cases + 25 cookie cases + null-cookie case all pass the published model |
| Original pure-virtual behavior | 0 | Six inputs dispatch signal 12 with zero, preserving stack/callee-saved registers |
| Original initializer walker behavior | 0 | Returns after calling all 160 image-derived targets in exact order; constructor bodies intercepted, not executed |
| Named compiler memclr helper object vs whole original extent | 0 | Relocation-free named 20-B section and exact full-extent equality |
| Retained r4023 object vs original | 0 | 128-byte equality / 124-byte target extent; no compiler run, rewrite or data permutation |
| Focused post-adoption baseline generation | **1** | Preserved legacy label 0x001f5990 is rejected; V9-R02 |

No check was piped through output filtering. Small evidence programs decoded original instructions internally and emitted only counts, addresses, digests and predicate outcomes. No product test was rerun after a reported failure. One evidence-path audit initially resolved the new public grid receipt under main rather than the proposal worktree; resolving public paths correctly closed that review-harness location issue, with all 60 hashes agreeing. This is not a missing proposal input or a retried gate.

**Main-reported, not reviewer-executed:** Main relayed a full suite on untouched branch 9b18089: **563 tests, 1 skipped, 0 failed modules, 58.33 s, exit 0**, including 156 test_analyze tests. Main also relayed a separate plain-CLI generation to scratch/verification/catalogue-v9-main-observed.json: exit 0, 27.76 s, complete equality and the same proposal SHA-256. These are proposal-branch checks only. They do not prove the later integration into newer acceptance/R1/permuter tooling, nor the post-adoption transition in V9-R02.

## Map, denominator and complete preservation

Independent byte-interval calculations, without trusting the serialized stats, give:

| Measure | Observed proposal |
| --- | ---: |
| Top-level owners | 4,699 |
| Ordinary creditable owners / extent B | 4,624 / 1,131,454 |
| Kinds | 4,624 function; 24 multi_entry; 22 runtime; 11 assembly; 18 veneer |
| Callable/retained label address-ISA pairs | 4,730 |
| Every prior entry retained | 4,616 / 4,616; zero losses |
| Identical old complete records | 4,536 |
| Changed old complete records / removed top-level records / added records | 50 / 6 / 113 |
| Function-owned physical B | 1,139,722 |
| All-kind extent sum / unowned holes inside extents | 1,141,022 / 1,300 B |
| Instruction / pool / addressed-literal B | 1,057,946 / 40,792 / 40,984 |
| Unknown B / ranges | 4,417 / 87 |
| Anomalies | 37 |
| Complete mapped image | 1,327,104 B, exactly tiled |

There are zero overlapping function extents, zero multiply owned bytes, and exact equality between all code/pool/literal spans and physical map owner assignments. No formerly function-owned physical byte is lost. The only old owned-category transition is **22 B of formerly inferred literals becoming correctly decoded code** at recovered interworking continuations. Newly owned bytes are **2,418 code + 696 pool + 148 addressed literal = 3,262 B**. No new padding byte is introduced; no old unknown byte is renamed padding.

The complete actual current main accepted population is **1,167 identities / 73,388 B**, all inside the frozen protection union and all complete-record identical. The immutable c008 blob supplies another 435 identities / 112,764 B: union **1,602 complete records / 186,152 B**, with no differences. The six protection summaries overlap and are not additive accepted credit. c008 blob **ec64c31302ef6631530d909e0e3ec3bd8494f33a**, SHA-256 **d8f49a991dde5c659f76f833de228342ff8fce9726288f55abae3f27055596b2**, is loaded by its object identity rather than by an active branch-tip lookup. The withheld-classification list is empty.

Ten new incoming observations and one additional protected start observation are kept separately rather than editing accepted metadata. Complete-record protection is checked after classification. No new classification erases protected evidence.

## Initializers, legacy labels and Thumb topology

The relative table is exactly **[0x00235e38, 0x002360b8)**, 160 distinct targets, decoded independently from each table site plus its unsigned 32-bit relative value. Both table bounds agree with the original walker pool/base arithmetic. All target address/ISA pairs agree with generated table records and final entries. There are **67 prior callable targets and 93 recovered targets**, not 160 newly discovered functions. Fresh original-walker emulation visits those exact 160 targets in order and returns, with constructor bodies recorded/skipped. The seven emission objects include three natural global-lifetime objects with actual four-byte .init_array / R_ARM_TARGET1 metadata.

A separate original-flow audit of all 93 new initializer owners reaches **631 instructions**, with no missing call entry, escaping unowned path, reachability-set mismatch or unexplained extent in those owners. This confirms their original extents; it is not authored C credit.

The five prologue-only old starts are valid aligned reachable ARM instruction boundaries, have no old incoming sources, and retain all old code/pool/literal spans. Each is explicitly retained as legacy_prologue inside its initializer owner. Keeping the labels and excluding those owners as multi_entry is a conservative, intentional preservation choice, not proof that the labels were independent callable routines. None is protected accepted code. Their old ordinary extents total 544 B; recovered multi-entry extents total 604 B. The implementation defect is their inability to survive a subsequent baseline generation, V9-R02, not their first-generation conservative treatment.

The two ownership-unresolved Thumb topology changes are independently supported:

| Old entry | Proposed owner | Checked incoming/flow evidence | Credit effect |
| --- | --- | --- | --- |
| 0x001000a0, 108 B | 0x00100084, 136 B, multi_entry | Exact canonical veneer 0x0010007c reaches new start; fallthrough retains old entry; original BLX at 0x0012db30 still targets old entry | -1 / -108 B; new 28 B uncredited |
| 0x0010030c, 84 B | Same extent, multi_entry | Pool pointer at 0x00100358 retains old start; newly decoded call 0x00100086 targets interior 0x00100312; both entries connected by fallthrough | -1 / -84 B |

All **56 / 4,144 original B** remain ownership-unresolved records. Exactly **54 / 3,952 original B** remain ordinary creditable; two are topology-excluded, **not ownership-proven non-C**. Every residue ownership_classified field is false. Erasing the incoming edges to force 56 ordinary owners would not preserve the observed graph.

## Runtime/helper/kernel admission under the v8 standard

All **27 inherited v8 classification records are exactly unchanged**. The eight additions are the four Thumb ABI contracts, one emitted helper and three narrowly bounded no-frame kernel adapters. There are **35 classification records**, including the two inherited runtime-evidenced multi-entry owners. No known-library exclusion is introduced.

| Added identity | Address / size | Independent adjudication |
| --- | --- | --- |
| .init_array startup walker | 0x001008ac / 36 B | Qualified admission: natural compiler section emission plus fresh exact original table-walk behavior and startup role. This is a **section ABI** witness, not a direct emitted-symbol binding. Exact __cpp_initialize__aeabi_ spelling is [INFERENCE]. |
| __aeabi_vec_dtor_cookie | 0x00100c80 / 18 B | Actual retained source/object hashes and three fresh existing-object 52/52 caller matches agree; 25 cookie cases plus null reproduce reverse destruction and cookie-pointer result. |
| __cxa_pure_virtual | 0x00100c9e / 10 B | Natural abstract-class objects really contain the relevant vtable relocation under four profiles; six fresh original executions dispatch the documented signal with zero. Installed signal.h defines SIGPVFN as 12. Static pointer graph corroboration is not alone an identity proof. |
| __aeabi_vec_dtor | 0x0019f6b4 / 34 B | Actual natural member-array destructor relocations under four profiles plus 25 fresh original cases establish parameter order, reverse callbacks, cookie-pointer result and preservation. The bound cookie helper is an original caller. |
| __ARM_common_memclr4_10 | 0x001847c8 / 20 B | Actual named STT_FUNC and relocation-free named section in object ca21cdbcf33d529d8f8ac9b091b40ca49b36fb52af7734ee8789156fcd9d44b3 equal the whole original extent; full digest 492eb185d4dde78c7f83992286756e363cacafdd2ccae26982bc839ee17f5187. Not authored-C credit. |
| GetProcessInfo no-frame adapter | 0x0011ef2c / 28 B | Original exact service 0x2b, no ordinary call or LR save; retained natural shared-header probe/grid, not an ordinary mismatch alone, supports the same finite no-frame standard used in v8. |
| GetSystemTick no-frame stub | 0x0011ef4c / 8 B | Original exact service 0x28, no ordinary call or LR save; same qualified finite-grid shape evidence. |
| DuplicateHandle no-frame adapter | 0x001565b0 / 24 B | Original exact service 0x27, no ordinary call or LR save; same qualified finite-grid shape evidence; stale CreateMemoryBlock identity is not reused. |

The retained public kernel grid has exactly **48 +0 cells**, zero compile failures and no matching cell; actual source hashes and receipt digest agree. Historical compid exit 1 means no match, not compiler failure. It was inspected, not rerun. The three exclusions remain **qualified shape/ABI** admissions under the binding v8 review, **not** a proof that no imaginable C spelling could match and **not** authenticated proprietary archive/SDK/handwritten ownership. Those ownership claims remain [INFERENCE]. Merely expanding a compiler trial is not in itself an ownership proof; the service contract and observed literal no-frame structure are essential here.

**0x00145610 remains the ordinary 28-B CreateMemoryBlock adapter, service 0x1e.** Its shared raw interface is representable; the retained three frozen-profile mismatches do not prove non-C status. It is not the 24-B DuplicateHandle function. Keeping it creditable is conservative and does not claim a matching source. No blanket SVC, CP15, barrier, Thumb, libc/libm, software-FP or adjustment-thunk exclusion is justified.

The section-level initializer witness is explicitly sufficient here for the compiler-runtime **role** under the existing compiler-ABI criterion; it does not authenticate a vendor body or direct historical symbol. The nonthrowing destructor callback experiments do not cover exception propagation. The trial Thumb profile sweep is retained, not rerun, and its two tiny all-eight-cell matches do not discriminate or freeze a game/SDK Thumb profile. No runtime archive body has been authenticated.

The 82 frozen author rows are all dispositioned: 57 ordinary, nine inherited runtime, one new emitted helper, eleven structural veneers, three proposed assembly stubs, and one preserved legacy entry. The latter is the old 0x001f5990 label, not a library exclusion. Original author assertions are retained as notes rather than silently promoted into accepted classification.

## Boundaries, shared data and consumer routing

The three new object-access suffixes were independently traversed over every path/register-initialization state. All decoded suffix instructions are reached (14 / 13 / 10), all returns close, inputs are initialized on every path, prefixes are nonstoring, there is no stack/LR dependency or unsupported side effect, and no branch crosses the partition. The exact erased tail sites and complete partitions are:

| Old owner / extent | Tail site | Partition | Checked rule |
| --- | --- | --- | --- |
| 0x001d033c / 68 B | 0x001d0344 | 12 + 56 | Closed R0-input scalar-memory graph |
| 0x001d222c / 60 B | 0x001d2230 | 8 + 52 | Closed R0-input scalar-memory graph |
| 0x001f4784 / 48 B | 0x001f4788 | 8 + 40 | Closed pointer/count graph |
| 0x0010bed4 / 36 B | 0x0010bef0 | 32 + 4 | Matching stack saved/restored register lists; final single unconditional tail to known returning ARM owner 0x001135e8 |

These are qualified erased-tail-linker interpretations, not recovered original function symbols. Four splits add four owners and zero physical/extent bytes. The inventory retains 776 NOP words and 143 split sites. The unsupported split requests inside unchanged 0x0014e50c / 336 B and 0x0016bde0 / 416 B are not adopted.

All twelve newly found/reclassified canonical PC-relative interworking veneers are exactly 8 B of code, with no pool/literal, and their decoded Thumb targets all have entries. Eleven were old ordinary starts, one is the new owner 0x001911c4. The four shortened old veneer extents expose Thumb code rather than hiding it as a literal. Six new pointer-only ARM leaves have independently checked original pointer words and reachable returns: 0x00156dbc from 0x001f59b4; 0x001c4490 from 0x0021415c; 0x001c5764 from 0x00214420; 0x001c588c from 0x002159c4; 0x001c5d90 from 0x002126b8; 0x001db960 from 0x002108e4.

The seven reviewed UTF-16 objects in 0x001372ec have exact reference sites, halfword-formatting consumers, complete terminated object lengths **10, 6, 26, 22, 18, 26, 22 B** and only zero alignment to spans **12, 8, 28, 24, 20, 28, 24 B**. They tile [0x00137520, 0x001375b0); extent 688→708 is supported without stealing a start/code span. This narrow positive result does not fix the shared ideographic-space case in V9-R01.

Shared recognition properly keeps ownership null and does not grow producer extents. Its wrong width-dependent object bounds are V9-R01; byte-string plausibility alone must not close those typed wide objects. The shared records and exact current intervals are listed below so that the disputed data is not hidden by arithmetic success.

Cross-module consumers were read, not assumed: new veneer targets feed discovery/refinement; relative table targets feed pointer recovery and final entry validation; all explicit entries feed inplace.Resolver.call_targets with their ISA; secondary labels therefore remain routable. validate_catalogue consumes code/entry/range invariants. tools/image.py creditable and ci/progress_report.py deliberately count only kind=function, so topology-excluded owners cannot earn C credit. tools/catalogue_review.py reports unresolved topology separately from ownership. Metadata-only observation fields do not authorize code/data credit. No silent dispatch drop was found in those consumers.

## Unknown/anomaly accounting and remaining work

Every one of the **140 old unknown intervals / 7,724 B** was independently intersected with the actual proposed map. All start/end/kind/owner partitions and all original interval digests agree, conserving every byte:

- 3,214 old unknown B become function-owned code/pool/literal spans.
- 93 old unknown B become proposed shared/unowned rodata, subject to V9-R01 width correction.
- 4,417 B remain unknown; no new unknown byte appears outside the old set.
- 53 old ranges are resolved, eight partial and 79 open; the resulting unknown map has 87 intervals.
- A further 48 formerly padding B become owned, yielding 3,262 total newly owned physical B.

All **70 old anomaly rows** are represented: 35 resolved, 35 open. Exactly two newly exposed unexplained-byte anomalies occur at Thumb owners **0x0019f5c4** and **0x0019f63c**, giving **37** final anomalies. The current counts are 24 unexplained owner-byte issues, eight literal-outside-owner issues, three addressed-literal cross-start issues, one overlap-code issue and one undecodable reachable-code issue. Original exceptional dispatch at **0x001964e8** remains unproved and is not suppressed. The 1,300 B of extent holes, shared external objects, C eligibility of unresolved multi-entry topology, original TU provenance, and standalone all-source linker provenance remain real qualifications.

This is **intermediate v9 work, not decomp completion**. The exact current **4,417 B / 87 unknown intervals and 37 anomalies** are enumerated below, even though new bounded-data evidence means some current unknown labels now need revision. No corrected residual total or accepted C progress is invented. Full current-main integration checks and any adoption/history update remain exclusively integrator-owned; this review grants neither adoption nor credit.

## Exhaustive metadata appendices

These tables are generated from the exact reviewed v8/proposal dictionaries and contain addresses and metadata only. They enumerate every old changed/removed record, every added owner, each shared span, every remaining unknown interval and all final anomalies.

### All 50 changed existing owners

| Owner | Old→new extent B | Old→new kind | Changed fields |
| --- | --- | --- | --- |
| 0x0010007c | 12→8 | function→veneer | entries, evidence, kind, literals, size |
| 0x00100114 | 8→8 | function→veneer | entries, evidence, kind |
| 0x0010011c | 18→18 | function→function | entries, evidence |
| 0x0010030c | 84→84 | function→multi_entry | entries, evidence, kind |
| 0x00100520 | 8→8 | function→veneer | entries, evidence, kind |
| 0x00100528 | 88→88 | function→function | entries |
| 0x001008ac | 36→36 | function→runtime | classification_evidence, kind |
| 0x001008dc | 16→8 | function→veneer | entries, evidence, kind, literals, size |
| 0x0010090c | 8→8 | function→veneer | entries, evidence, kind |
| 0x00100914 | 22→22 | function→function | entries |
| 0x00100a6c | 8→8 | function→veneer | entries, evidence, kind |
| 0x00100ab8 | 24→24 | function→function | entries, evidence |
| 0x00100af0 | 8→8 | function→veneer | entries, evidence, kind |
| 0x00100af8 | 22→22 | function→function | entries |
| 0x00100c80 | 18→18 | function→runtime | classification_evidence, kind |
| 0x00100c94 | 8→8 | function→veneer | entries, evidence, kind |
| 0x00100c9e | 10→10 | function→runtime | classification_evidence, kind |
| 0x0010bed4 | 36→32 | function→function | code_ranges, code_size, size |
| 0x0011ef2c | 28→28 | function→assembly | classification_evidence, kind |
| 0x0011ef4c | 8→8 | function→assembly | classification_evidence, kind |
| 0x001372ec | 688→708 | function→function | literals, size |
| 0x001565b0 | 24→24 | function→assembly | classification_evidence, kind |
| 0x001565c8 | 44→44 | function→function | entries |
| 0x001847c8 | 20→20 | function→runtime | classification_evidence, kind |
| 0x001911cc | 34→34 | runtime→runtime | entries, evidence |
| 0x0019f5bc | 16→8 | function→veneer | entries, evidence, kind, literals, size |
| 0x0019f634 | 12→8 | function→veneer | entries, evidence, kind, literals, size |
| 0x0019f6ac | 8→8 | function→veneer | entries, evidence, kind |
| 0x0019f6b4 | 34→34 | function→runtime | classification_evidence, entries, evidence, kind |
| 0x0019fd6c | 152→152 | function→function | entries |
| 0x0019fedc | 22→22 | function→function | entries |
| 0x0019fef2 | 176→176 | function→function | entries |
| 0x001d033c | 68→12 | function→function | code_ranges, code_size, size |
| 0x001d222c | 60→8 | function→function | code_ranges, code_size, size |
| 0x001f4784 | 48→8 | function→function | code_ranges, code_size, size |
| 0x001f50d8 | 152→152 | function→function | entries, evidence |
| 0x001f5170 | 168→168 | function→function | entries, evidence |
| 0x001f5674 | 268→268 | function→function | entries, evidence |
| 0x001f5780 | 84→84 | function→function | entries, evidence |
| 0x001f580c | 96→96 | function→function | entries, evidence |
| 0x001f586c | 80→80 | function→function | entries, evidence |
| 0x00203810 | 600→600 | function→function | entries, evidence |
| 0x00205280 | 112→112 | function→function | entries, evidence |
| 0x00206938 | 116→116 | function→function | entries, evidence |
| 0x002102b4 | 876→876 | function→function | entries, evidence |
| 0x00214580 | 92→92 | function→function | entries, evidence |
| 0x00214d18 | 132→132 | function→function | entries, evidence |
| 0x00214d9c | 164→164 | function→function | entries, evidence |
| 0x00214e40 | 180→180 | function→function | entries, evidence |
| 0x00215120 | 252→252 | function→function | entries, evidence |

### All six replaced top-level records, with retained entry

| Old owner | Old B | Retaining owner | New B | Kind | Retained evidence | Incoming sources |
| --- | --- | --- | --- | --- | --- | --- |
| 0x001000a0 | 108 | 0x00100084 | 136 | multi_entry | blx_target | 0x0012db30 |
| 0x001f5990 | 32 | 0x001f5980 | 56 | multi_entry | legacy_prologue | none |
| 0x002069f0 | 92 | 0x002069ec | 96 | multi_entry | legacy_prologue | none |
| 0x00212684 | 68 | 0x00212674 | 84 | multi_entry | legacy_prologue | none |
| 0x002136d0 | 44 | 0x002136c4 | 56 | multi_entry | legacy_prologue | none |
| 0x00214fec | 308 | 0x00214fe8 | 312 | multi_entry | legacy_prologue | none |

### All 113 added owners

| Address | Extent B | Kind | ISA | Initializer target | Start evidence |
| --- | --- | --- | --- | --- | --- |
| 0x00100084 | 136 | multi_entry | thumb | no | veneer_target |
| 0x001005a6 | 22 | function | thumb | no | veneer_target |
| 0x001008e4 | 38 | function | thumb | no | veneer_target |
| 0x0010092a | 16 | function | thumb | no | veneer_target |
| 0x00100b0e | 16 | function | thumb | no | veneer_target |
| 0x00100c9c | 2 | function | thumb | no | veneer_target |
| 0x0010bef4 | 4 | function | arm | no | linker_branchnop |
| 0x00156dbc | 16 | function | arm | no | pointer |
| 0x001911c4 | 8 | veneer | arm | no | veneer_pattern |
| 0x0019f5c4 | 112 | function | thumb | no | veneer_target |
| 0x0019f63c | 108 | function | thumb | no | veneer_target |
| 0x0019fec6 | 22 | function | thumb | no | bl_target |
| 0x001c4490 | 4 | function | arm | no | pointer |
| 0x001c5764 | 4 | function | arm | no | pointer |
| 0x001c588c | 4 | function | arm | no | pointer |
| 0x001c5d90 | 4 | function | arm | no | pointer |
| 0x001d0348 | 56 | function | arm | no | linker_branchnop |
| 0x001d2234 | 52 | function | arm | no | linker_branchnop |
| 0x001db960 | 4 | function | arm | no | pointer |
| 0x001f478c | 40 | function | arm | no | linker_branchnop |
| 0x001f5218 | 64 | function | arm | yes | relative_init_array |
| 0x001f58bc | 24 | function | arm | yes | relative_init_array |
| 0x001f58d4 | 92 | function | arm | yes | relative_init_array |
| 0x001f5930 | 20 | function | arm | yes | relative_init_array |
| 0x001f5944 | 40 | function | arm | yes | relative_init_array |
| 0x001f596c | 20 | function | arm | yes | relative_init_array |
| 0x001f5980 | 56 | multi_entry | arm | yes | relative_init_array |
| 0x002052f0 | 24 | function | arm | yes | relative_init_array |
| 0x00205308 | 24 | function | arm | yes | relative_init_array |
| 0x002069ac | 20 | function | arm | yes | relative_init_array |
| 0x002069c0 | 20 | function | arm | yes | relative_init_array |
| 0x002069d4 | 24 | function | arm | yes | relative_init_array |
| 0x002069ec | 96 | multi_entry | arm | yes | relative_init_array |
| 0x00206a4c | 24 | function | arm | yes | relative_init_array |
| 0x00206a64 | 52 | function | arm | yes | relative_init_array |
| 0x0020e294 | 20 | function | arm | yes | relative_init_array |
| 0x0020e2a8 | 92 | function | arm | yes | relative_init_array |
| 0x0020e304 | 20 | function | arm | yes | relative_init_array |
| 0x0020e318 | 20 | function | arm | yes | relative_init_array |
| 0x0020e32c | 24 | function | arm | yes | relative_init_array |
| 0x0020e344 | 20 | function | arm | yes | relative_init_array |
| 0x0020e358 | 24 | function | arm | yes | relative_init_array |
| 0x0020e370 | 24 | function | arm | yes | relative_init_array |
| 0x0020e388 | 4 | function | arm | yes | relative_init_array |
| 0x0020e38c | 4 | function | arm | yes | relative_init_array |
| 0x0020e390 | 32 | function | arm | yes | relative_init_array |
| 0x00210834 | 56 | function | arm | yes | relative_init_array |
| 0x0021086c | 20 | function | arm | yes | relative_init_array |
| 0x00210880 | 24 | function | arm | yes | relative_init_array |
| 0x00210898 | 24 | function | arm | yes | relative_init_array |
| 0x002108b0 | 56 | function | arm | yes | relative_init_array |
| 0x002108e8 | 24 | function | arm | yes | relative_init_array |
| 0x00210900 | 44 | function | arm | yes | relative_init_array |
| 0x0021092c | 28 | function | arm | yes | relative_init_array |
| 0x00210948 | 28 | function | arm | yes | relative_init_array |
| 0x002124ec | 20 | function | arm | yes | relative_init_array |
| 0x00212500 | 24 | function | arm | yes | relative_init_array |
| 0x00212518 | 20 | function | arm | yes | relative_init_array |
| 0x0021252c | 24 | function | arm | yes | relative_init_array |
| 0x00212544 | 20 | function | arm | yes | relative_init_array |
| 0x00212558 | 20 | function | arm | yes | relative_init_array |
| 0x0021256c | 20 | function | arm | yes | relative_init_array |
| 0x00212580 | 24 | function | arm | yes | relative_init_array |
| 0x00212650 | 36 | function | arm | yes | relative_init_array |
| 0x00212674 | 84 | multi_entry | arm | yes | relative_init_array |
| 0x002126c8 | 20 | function | arm | yes | relative_init_array |
| 0x002126dc | 20 | function | arm | yes | relative_init_array |
| 0x00212a78 | 20 | function | arm | yes | relative_init_array |
| 0x00212a8c | 24 | function | arm | yes | relative_init_array |
| 0x00212aa4 | 24 | function | arm | yes | relative_init_array |
| 0x00212abc | 20 | function | arm | yes | relative_init_array |
| 0x00212ad0 | 24 | function | arm | yes | relative_init_array |
| 0x00212b20 | 40 | function | arm | yes | relative_init_array |
| 0x00212b48 | 24 | function | arm | yes | relative_init_array |
| 0x00213624 | 24 | function | arm | yes | relative_init_array |
| 0x002136c4 | 56 | multi_entry | arm | yes | relative_init_array |
| 0x00214138 | 44 | function | arm | yes | relative_init_array |
| 0x0021431c | 24 | function | arm | yes | relative_init_array |
| 0x00214334 | 24 | function | arm | yes | relative_init_array |
| 0x0021434c | 20 | function | arm | yes | relative_init_array |
| 0x00214360 | 24 | function | arm | yes | relative_init_array |
| 0x00214378 | 24 | function | arm | yes | relative_init_array |
| 0x00214390 | 44 | function | arm | yes | relative_init_array |
| 0x002143bc | 24 | function | arm | yes | relative_init_array |
| 0x002143d4 | 24 | function | arm | yes | relative_init_array |
| 0x002143ec | 56 | function | arm | yes | relative_init_array |
| 0x00214424 | 24 | function | arm | yes | relative_init_array |
| 0x0021443c | 44 | function | arm | yes | relative_init_array |
| 0x002145dc | 24 | function | arm | yes | relative_init_array |
| 0x00214ef4 | 172 | function | arm | yes | relative_init_array |
| 0x00214fa0 | 24 | function | arm | yes | relative_init_array |
| 0x00214fb8 | 24 | function | arm | yes | relative_init_array |
| 0x00214fd0 | 24 | function | arm | yes | relative_init_array |
| 0x00214fe8 | 312 | multi_entry | arm | yes | relative_init_array |
| 0x002158c0 | 24 | function | arm | yes | relative_init_array |
| 0x002158d8 | 20 | function | arm | yes | relative_init_array |
| 0x002158ec | 24 | function | arm | yes | relative_init_array |
| 0x00215904 | 92 | function | arm | yes | relative_init_array |
| 0x00215960 | 24 | function | arm | yes | relative_init_array |
| 0x00215978 | 24 | function | arm | yes | relative_init_array |
| 0x00215990 | 56 | function | arm | yes | relative_init_array |
| 0x002159c8 | 20 | function | arm | yes | relative_init_array |
| 0x002159dc | 24 | function | arm | yes | relative_init_array |
| 0x002159f4 | 24 | function | arm | yes | relative_init_array |
| 0x00215a0c | 24 | function | arm | yes | relative_init_array |
| 0x00215a24 | 40 | function | arm | yes | relative_init_array |
| 0x00215a4c | 24 | function | arm | yes | relative_init_array |
| 0x00215a64 | 24 | function | arm | yes | relative_init_array |
| 0x00215a7c | 24 | function | arm | yes | relative_init_array |
| 0x00215a94 | 24 | function | arm | yes | relative_init_array |
| 0x00215aac | 24 | function | arm | yes | relative_init_array |
| 0x00215ac4 | 24 | function | arm | yes | relative_init_array |
| 0x00215adc | 20 | function | arm | yes | relative_init_array |

### All ten proposed shared spans (ownership null; V9-R01 applies)

| Start | Exclusive end | B | Address-producing owners | Recorded address-reference sites |
| --- | --- | --- | --- | --- |
| 0x001457a8 | 0x001457b4 | 12 | 0x001456e4 | 0x0014570c |
| 0x001457b4 | 0x001457c8 | 20 | 0x001456e4 | 0x00145724, 0x00145744 |
| 0x001b54e4 | 0x001b54e5 | 1 | 0x001b5374 | 0x001b53e4 |
| 0x001b8234 | 0x001b8235 | 1 | 0x001b8198 | 0x001b8208 |
| 0x001bc29c | 0x001bc29d | 1 | 0x001bc220 | 0x001bc278 |
| 0x001cd2e4 | 0x001cd2e6 | 2 | 0x001cd070 | 0x001cd1a4, 0x001cd1b0, 0x001cd280 |
| 0x001cd2ec | 0x001cd2f0 | 4 | 0x001cd070 | 0x001cd234 |
| 0x001f82b0 | 0x001f82c4 | 20 | 0x001f7d7c | 0x001f7f1c |
| 0x001f83b4 | 0x001f83c4 | 16 | 0x001f7d7c | 0x001f8024 |
| 0x00203068 | 0x00203078 | 16 | 0x00202ca8 | 0x00202d84 |

The owners column lists candidate address-producing functions, not all independent pool/table users; the complete pointer census preserves those additional users.

### All 87 remaining unknown intervals (4,417 B)

| Start | Exclusive end | B |
| --- | --- | --- |
| 0x00100020 | 0x00100024 | 4 |
| 0x00100076 | 0x00100078 | 2 |
| 0x0010012e | 0x0010015e | 48 |
| 0x00100396 | 0x00100398 | 2 |
| 0x0010051e | 0x00100520 | 2 |
| 0x001005bc | 0x001005ca | 14 |
| 0x001005ea | 0x001005ec | 2 |
| 0x001005fa | 0x0010060a | 16 |
| 0x001007ce | 0x001007d0 | 2 |
| 0x001008c6 | 0x001008c8 | 2 |
| 0x0010090a | 0x0010090c | 2 |
| 0x00100942 | 0x00100944 | 2 |
| 0x001009d4 | 0x001009de | 10 |
| 0x001009fe | 0x00100a00 | 2 |
| 0x00100a6a | 0x00100a6c | 2 |
| 0x00100ada | 0x00100adc | 2 |
| 0x00100ae0 | 0x00100ae4 | 4 |
| 0x00100b26 | 0x00100b28 | 2 |
| 0x00100c7a | 0x00100c7c | 2 |
| 0x00100c92 | 0x00100c94 | 2 |
| 0x00100d70 | 0x00100d74 | 4 |
| 0x00104720 | 0x00104740 | 32 |
| 0x0010d3a8 | 0x0010d3b0 | 8 |
| 0x001415b4 | 0x001415b8 | 4 |
| 0x001569b0 | 0x00156a30 | 128 |
| 0x0016067c | 0x00160684 | 8 |
| 0x0016b6a2 | 0x0016b6a4 | 2 |
| 0x00181eaa | 0x00181eac | 2 |
| 0x00184204 | 0x0018423c | 56 |
| 0x00184278 | 0x001842d8 | 96 |
| 0x0018433c | 0x001846f8 | 956 |
| 0x0018e0dc | 0x0018e14c | 112 |
| 0x0018e1c0 | 0x0018e208 | 72 |
| 0x0018e270 | 0x0018e764 | 1268 |
| 0x0018eaec | 0x0018ebf8 | 268 |
| 0x0018ec60 | 0x0018ec7c | 28 |
| 0x0018ed0c | 0x0018ed18 | 12 |
| 0x0018eda8 | 0x0018edb4 | 12 |
| 0x001911ee | 0x001911f0 | 2 |
| 0x00191c92 | 0x00191c94 | 2 |
| 0x001944ac | 0x001944e8 | 60 |
| 0x001964e8 | 0x00196504 | 28 |
| 0x0019d5c2 | 0x0019d5c4 | 2 |
| 0x0019f622 | 0x0019f624 | 2 |
| 0x0019f696 | 0x0019f698 | 2 |
| 0x0019f6d6 | 0x0019f6d8 | 2 |
| 0x0019f910 | 0x0019f912 | 2 |
| 0x0019fd14 | 0x0019fd20 | 12 |
| 0x0019fd6a | 0x0019fd6c | 2 |
| 0x0019ffa2 | 0x0019ffa4 | 2 |
| 0x001a00b4 | 0x001a00b8 | 4 |
| 0x001ad598 | 0x001ad59c | 4 |
| 0x001b54e5 | 0x001b54e8 | 3 |
| 0x001b5a1c | 0x001b5a20 | 4 |
| 0x001b5a24 | 0x001b5a28 | 4 |
| 0x001b8235 | 0x001b8238 | 3 |
| 0x001bc29d | 0x001bc2a0 | 3 |
| 0x001c3c34 | 0x001c3c48 | 20 |
| 0x001c7748 | 0x001c775c | 20 |
| 0x001cd2e6 | 0x001cd2e8 | 2 |
| 0x001de4e0 | 0x001de4e8 | 8 |
| 0x001e84dc | 0x001e84e8 | 12 |
| 0x001ef550 | 0x001ef554 | 4 |
| 0x001ef558 | 0x001ef55c | 4 |
| 0x00203ce4 | 0x00203d10 | 44 |
| 0x00203d1c | 0x00203d4c | 48 |
| 0x00203d54 | 0x00203d84 | 48 |
| 0x00203d94 | 0x00203db4 | 32 |
| 0x00203dc0 | 0x00203dfc | 60 |
| 0x00203e04 | 0x00203e2c | 40 |
| 0x00203e38 | 0x00203e60 | 40 |
| 0x00203e6c | 0x00203e94 | 40 |
| 0x00203eb8 | 0x00203ee0 | 40 |
| 0x00203eec | 0x00203f28 | 60 |
| 0x00203f34 | 0x00203f64 | 48 |
| 0x00203f6c | 0x00203f9c | 48 |
| 0x00203fb4 | 0x00203fd8 | 36 |
| 0x00203ff0 | 0x00204024 | 52 |
| 0x00204034 | 0x00204064 | 48 |
| 0x00204254 | 0x0020427c | 40 |
| 0x00204288 | 0x002042c0 | 56 |
| 0x002042e4 | 0x002042f8 | 20 |
| 0x00204324 | 0x0020435c | 56 |
| 0x00204364 | 0x0020438c | 40 |
| 0x00204394 | 0x002043c8 | 52 |
| 0x002043e0 | 0x002043f0 | 16 |
| 0x00204414 | 0x00204444 | 48 |

### All 37 final anomalies

| Address | Issue |
| --- | --- |
| 0x00100360 | unexplained_function_bytes |
| 0x00100372 | address_literal_crosses_start |
| 0x00100372 | address_literal_overlaps_code |
| 0x00100532 | literal_outside_function |
| 0x00100538 | literal_outside_function |
| 0x001005ca | unexplained_function_bytes |
| 0x0010064c | unexplained_function_bytes |
| 0x001008ac | unexplained_function_bytes |
| 0x001009de | unexplained_function_bytes |
| 0x00100a9a | address_literal_crosses_start |
| 0x00100a9a | literal_outside_function |
| 0x00100aac | address_literal_crosses_start |
| 0x00100aac | literal_outside_function |
| 0x00100bb0 | unexplained_function_bytes |
| 0x00104458 | unexplained_function_bytes |
| 0x0010d2b8 | unexplained_function_bytes |
| 0x00145570 | unexplained_function_bytes |
| 0x001456e4 | unexplained_function_bytes |
| 0x00155208 | unexplained_function_bytes |
| 0x00160454 | unexplained_function_bytes |
| 0x0018ec7c | unexplained_function_bytes |
| 0x001964e8 | undecodable_reachable_code |
| 0x0019f5c4 | unexplained_function_bytes |
| 0x0019f63c | unexplained_function_bytes |
| 0x0019f958 | literal_outside_function |
| 0x0019f95a | literal_outside_function |
| 0x0019f9ce | literal_outside_function |
| 0x0019f9d8 | literal_outside_function |
| 0x001ad3c8 | unexplained_function_bytes |
| 0x001b55f8 | unexplained_function_bytes |
| 0x001c3ac0 | unexplained_function_bytes |
| 0x001de2c0 | unexplained_function_bytes |
| 0x001e81fc | unexplained_function_bytes |
| 0x001edbb0 | unexplained_function_bytes |
| 0x001f7d7c | unexplained_function_bytes |
| 0x00202ca8 | unexplained_function_bytes |
| 0x00203a68 | unexplained_function_bytes |

## Qualifications retained at the claim

1. The exact proposal is reproducible on its frozen **v8** input, not on an already-adopted v9 baseline; V9-R02 is a reproduced blocker, not a hypothetical missing test.
2. The exact current map arithmetic is correct, but width/format evidence for the disputed shared literals is not; V9-R01 is not permission to grow a C extent or steal shared ownership.
3. Five legacy labels preserve historical evidence conservatively without establishing independent callability. The two Thumb graph exclusions establish topology, not runtime/library ownership.
4. Runtime ABI evidence proves the narrowly described role/contract, not proprietary body identity. Initializer-walker evidence is section-level; the exact stripped vendor symbol is [INFERENCE]. Nonthrowing destructor experiments do not establish exception behavior.
5. The three no-frame exclusions use the already-adopted v8 finite-shape standard, not a new universal theorem from compiler mismatches. SDK/archive/handwritten authorship remains [INFERENCE]. CreateMemoryBlock stays C-eligible.
6. v8 historical compiler/emulator/declaration-rejection claims remain inherited evidence; no old receipt is rewritten or passed off as a fresh execution. New source/object inspections and the explicitly listed fresh original-body experiments are distinguished above.
7. No frozen Thumb profile, authenticated runtime archive, known-library body exclusion, standalone all-source build, new accepted C match, complete unknown-map closure or final decomp completion is claimed.
8. Main-reported proposal-branch suite/generation success is credited exactly as reported and does not overrule the semantic findings or prove the newer integrated harness.

Public contract references retained with the research, not newly fetched vendor-body authentication: [Arm C++ ABI](https://github.com/ARM-software/abi-aa/blob/main/cppabi32/cppabi32.rst#library-helper-functions), [Arm library guide](https://documentation-service.arm.com/static/5e99ac3a9931941038dd997c), [3dbrew Kernel ABI](https://www.3dbrew.org/wiki/Kernel_ABI), and the binding repository [v8 review](../../../docs/catalogue/REVIEW-v8.md).

**Seal:** overall_correctness **incorrect**; explanation: exact pre-adoption generation and preservation succeed, but typed shared-data bounds are contradicted by a retained matching object and legacy-entry preservation fails on the proposed adopted baseline. Confidence **0.99**. Both findings require a new reviewed revision; this exact proposal is not approved for adoption.
