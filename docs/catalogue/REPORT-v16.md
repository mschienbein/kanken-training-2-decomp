# Catalogue v16 — independently anchored ARM array delete

## Proposal boundary and frozen parent

**UNADOPTED proposal, not an independent review, acceptance or C credit.**
The new `catalogue-v16` branch/worktree was created at Main's
`ea985bcb37576c3347debe84579993a8f228352b`, with `orig`, `.cache` and `.venv`
symlinked to the main checkout. Only this worktree and
`scratch/integrator/catalogue-v16/` were written. Main's catalogue, accepted
sources/records, names, headers, profiles and frozen GATES v1.20 were not edited.

The adopted v15 parent is the immutable Git blob bound in
`research/catalogue_v16_evidence.json`, SHA-256
`756bacb96bb8e7e59a1e97480b40557ad056819ce27aa030a8b245812fe50980`.
The passing `REVIEW-v15.md`, parent evidence, exact accepted ledger/names,
pending snapshots, strict 4.1 adjudication/report, both native caller sources and
new independent proof are frozen hash/size-bound generator inputs. All historical
failures, qualifications and refusals retain their meanings.

Proposed `config/catalogue.json`: **6,704,126 B**, SHA-256
`8fbf9c14089dc98e25a4dd918f47f08ee0bb032c16c4e06917ad85028681f3ee`.

## Exact accounting and protection

| Measure | Adopted v15 | Proposed v16 | Delta |
|---|---:|---:|---:|
| Creditable ordinary owners | 4,546 | **4,545** | **−1** |
| Creditable owner-extent bytes | 1,120,094 | **1,120,082** | **−12 B** |
| Library owners | 96 | 97 | +1 |
| Classified owners | 157 | 158 | +1 |
| All owners / entries | 4,744 / 4,777 | 4,744 / 4,777 | 0 / 0 |
| Accepted functions / bytes | 2,796 / 305,008 | 2,796 / 305,008 | **0 / 0 B** |
| Unknown bytes / intervals | 98 / 10 | 98 / 10 | 0 / 0 |
| Anomalies | 1 | 1 | 0 |

Only `func_0019e08c` changes, and only its **kind** (`function -> library`) and
added **classification_evidence**. Its address, 12-byte extent, ARM ISA, complete
code ranges, entry/source list, pools, literals and every other old field are
identical. All **4,743 other complete owner records** are identical. Physical
ranges, their ownership/order, all earlier closure records and every other image/
topology field are unchanged. Only four statistics change: creditable functions,
creditable bytes, classified functions and owner-kind counts. The new generator,
review and one closure certificate describe this successor rather than copying an
ancestor's transition list as a new claim.

`research/catalogue_v16_generation.json` records the complete before/after target,
every statistic delta, exact generator before/after, preserved unknowns/anomaly
and protection commitments. No numerator or C/source/data credit follows.

### Before-edit check and frozen pending scope

Before creating the worktree or writing files, all live accepted claims and every
existing c015/recon-small candidate manifest were resolved through the respective
names overlays. **No claim named address `0019e08c`**, including aliases. The
initial recon-small observation had no c016 manifest. Main explicitly authorized
proceeding with that observation and an adoption-time exclusion recheck.

The first c016 commit became available before input sealing, so its existing
initial manifest is additionally frozen and protected; later growth is not
silently consumed. The exact retained snapshots are:

| Input | Commit | Claims / extent bytes | Complete-record changes |
|---|---|---:|---:|
| Main accepted ledger | `ea985bcb37576c3347debe84579993a8f228352b` | 2,796 / 305,008 | 0 |
| c015 `config/candidates/c015.json` | `bb6397f4fb3590adb2483e6ddbcbf11e06f5af05` | 26 / 3,704 | 0 |
| recon-small initial `config/candidates/c016.json` | `49056a408122e5aa5e59ae13ed7dfcf1ec9abe36` | 1 / 232 | 0 |

The generator protects entire parent records for every resolved claim and refuses
if the classification target is protected. Frozen manifests/names are consumed
rather than live branch files. **Main must recheck later ledger, c015 and growing
c016 manifests when adopting v16**, as explicitly recorded in `pending_recon`.
ReconSmallCodex also explicitly reserved/excluded this library identity. No source
or candidate is adopted here.

## Independent private rederivation

The input lead is LibraryIdentity41's strict `CANDIDATES-41.json` and
`REPORT-41.md`, retained without modification under
`research/catalogue_v16_frozen/`. Their verdict was not copied as a substitute for
comparison. New project-reader reconstruction independently reopened only the four
selected `array_del.o` members, reconstructed the retained complete native
array-delete caller, and freshly compiled one natural public-`<new>` scalar-delete
caller. All relocations were applied with the unchanged strict in-place primitive;
there are **no masks, skipped fields or linker rewrites**.

### Complete definition and preferred provenance

Owner **`0019e08c / 12 B / ARM`**, Itanium ABI name **`_ZdaPv`**, global
`operator delete[](void*)`:

* Preferred: **4.1 b791 `cpprt_4.l(array_del.o)`**, ar-header offset **10774**,
  archive SHA-256
  `af94582ef9f66067a1008a93907145cd73ee9f39777838db28ae719b301db01b`,
  complete member SHA-256
  `ebd595814707965bb838feecaf85555d3137cb2ed8e30926ba1e1907b482b06b`.
* The entire executable, nonwritable `i._ZdaPv` section is **12 B**, alignment 4,
  `$a` throughout. Its actual global `STT_FUNC` declaration begins at zero and is
  exactly **12 B**, with no borrowed section tail/pool or hidden extra entry.
* Its single actual relocation is **`R_ARM_CALL` at +4**, addend **−8**, naming
  `_ZdlPv` at independently anchored **`001852f4`**, ARM. The complete relocated
  section equals the entire unchanged original owner, SHA-256
  `b8624762ba88c5a4e14e06b44617877ebc7785bdd0d2ed2ac410b5fcc440df03`.
* b791 `cpprt_4f.l` is an alternative. Its member SHA-256 is
  `d979bad977d5830202feaa3ab8e36b2e0ec90b264addbfaac1121152e0841d31`.
  The whole payload differs from the preferred variant, but its complete code
  section and strict reconstruction agree.
* The corresponding **b894 members were independently read and compared in
  memory byte-for-byte**: each complete member is identical to its corresponding
  b791 variant. The two variant hashes are not conflated. All four archive hashes,
  member offsets/sizes/digests and complete section/declaration/relocation metadata
  are retained in `research/catalogue_v16_library_proof.json` under `/variants`.

These are hash-identified private inputs, not imported licensed library payloads
or a claim to uniquely authenticate the historical compiler/archive build.

### Two independent native name/ABI anchors

**Incoming `_ZdaPv`:** tr20's source has a natural native `delete[] list->entries`
expression. Its exact retained source, object and MATCH receipt closure was
checked; the actual object was then reopened and **all 140 bytes** of
`func_00172f50` reconstructed again. The unique actual named relocation at
**+96 / `00172fb0`** calls **`0019e08c`**. No rewritten field, generic deleting-
destructor interpretation, or arbitrary address-named extern supplies that name.
Evidence: proof `/array_anchor`; the source snapshot is retained separately.

**Outgoing `_ZdlPv`:** a new, isolated C++ replay of the already accepted
**32-byte `func_00102cb0`** uses the installed public **`<new>`** declaration and
`::operator delete(bss_00497574)`, preserving the unchanged cleanup control flow.
The frozen game profile compiled it without using/writing the object cache.
Every byte of the complete 32-byte section reconstructs exactly, with **zero
rewrites**. Its sized function declaration remains **28 B**, followed by its
real mapped data field; no declaration is extended to hide that distinction.

The actual name-bearing field at **+16 / `00102cc0`** is a legacy
**`R_ARM_JUMP24`**, addend **−8**, targeting **`001852f4`**. Its actual conditional
call form is preserved, not assumed to be an unconditional tail B merely from the
relocation label. The complete +24 cleanup transfer and +28 BSS field are also
reencoded. This proves the scalar-target name independently of the array-delete
body's shape. Evidence: proof `/scalar_anchor`, including compiler/source/header
hashes, flags, exact argv, exit **0**, complete section and actual relocation list.

The temporary object and dependency file were removed by the temporary-directory
context after each fresh compile; no compiler objects or new cache entries are
retained. Public replay compares complete section metadata and all compiler/input
bindings; only the diagnostic temporary-output argv and full fresh object hash
are excluded from cross-run equality because temporary paths are not identities.
The full original research values remain recorded, not overwritten.

### Unchanged refusals and limits

Only this one owner is promoted. Every other discovery/refusal owner and all five
**PENDING-LINKER-v19** Thumb rows retain their complete old catalogue records.
The unnamed six-byte Thumb residue is not invented as a library helper either.
No v19 rule, weak-import rewrite, Thumb extent relaxation, allocator/vtable name,
SDK identity or source/type ruling is introduced.

The frozen 4.1 report predates adopted v15. Its statements about the then-unknown
`_snputc` interval remain historical statements; they do not undo v15's already
reviewed separate callback classification or newly classify the refused snprintf
parent. Likewise, no old negative is rerun or reinterpreted as a pass.

**Qualification:** known complete vendor-body identity plus native name/ABI
corroboration is established, not unique historical provenance, source ownership,
canonical name/interface adoption or C credit. No original ROM bytes, instruction
encodings, disassembly, packets or licensed object payloads are printed, stored or
committed. Buffers remain in memory; evidence contains metadata and hashes only.

## Public generation and behavioural checks

The explicit v16 successor leaves `tools/analyze.py catalogue`, v14 and v15
commands at their frozen historical semantics. `tools/catalogue_library.py`
authenticates the immutable parent and retained inputs, derives the classification
from the complete proof, checks accepted/pending protection, and changes only the
certified kind/evidence fields. Public validation recomputes the entire expected
successor from independently supplied parent/input/source bindings; it cannot
project away unrelated owner, physical-map, statistic, receipt or generator edits.

Final actual generation command:

```text
.venv/bin/python -B tools/catalogue_library.py propose-v16 --prove-objects --private-root /home/michael/code/kanken-training-2-decomp --out config/catalogue.json
```

**Exit 0; 1.346627 s captured wall.** This public replay independently reread all
four complete vendor sections and the whole retained array caller, then freshly
compiled/reconstructed the scalar anchor and removed its temporary outputs. The
certificate-only command omits `--prove-objects`; it uses the same hash-bound
inputs and deterministic successor, without consuming mutable scratch discovery.

Actually exercised targeted tests:

| Run | Modules | Observed result |
|---|---|---|
| r1 | `tests.test_catalogue_v16`, `tests.test_catalogue_v15`, `tests.test_catalogue_v14`, `tests.test_catalogue_alignment`, `tests.test_catalogue_v13_r2` | **70 tests, exit 0**, 141.008 s unittest / 141.754583 s wall |
| Final r2 | `tests.test_catalogue_v16` after the actual legacy-transfer guard/test correction | **17 tests, exit 0**, 37.525 s unittest / 37.991807 s wall |

The four ancestor modules were unchanged after r1; their 53 passing tests are
carried, not represented as rerun. New tests exercise public generation,
validation, protection and input loading on authored nonexecutable metadata.
They derive exact record/statistic deltas, never pin headline totals or assert
report prose. Positive controls accompany refusals for generic/partial definitions,
wrong ISA/mapping, rewrites, changed outgoing name/target/addend/type/binding,
missing incoming source/name/whole caller, failed/cached scalar compilation,
legacy transfer form/mode/target errors, cross-build/provenance changes, new pending
claims, extra adjudication promotions, retained-input mutation/escape, forged
parent bindings and unrelated successor/generator changes.

Two genuine preliminary failures are retained, not concealed: the initial
certificate check exited **1** because it wrongly demanded unconditional native
transfers; public generator r1 exited **1** before writing output because its
replacement guard wrongly equated legacy JUMP24 with B-only. The corrected guard
preserves strict full reencoding and checks actual direct branch/call targets;
its regression includes the real conditional-call case, the tail-branch positive
control and invalid-transfer refusal. No original input, declared extent, linker
algorithm or match criterion was relaxed to pass.

The branch appends **denominator-history sequence 13**, explicitly **UNADOPTED**,
for catalogue revision **v16**. Actual history-record exit: **0**. Required verify:

```text
.venv/bin/python -B tools/denominator.py verify
ok: denominator v13 matches config/catalogue.json
```

**Exit 0.** Exact argv/cwd/exits/output/times, preliminary failures, cleanup,
source hashes and accounting binding are committed in
`research/catalogue_v16_checks.json`. No full suite, matcher, full build, acceptance
or adoption ran. Main retains the heavy-slot full suite, independent review,
later pending-manifest reconciliation and canonical adoption operations.
