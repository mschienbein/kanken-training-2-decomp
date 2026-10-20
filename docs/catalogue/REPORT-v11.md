# Catalogue v11 — exact-build runtime data and exceptional continuation

## Proposal boundary

This is a new, unadopted closure proposal based on fixed `3de76111a448dfaca260fab5630762edb27feb0c`.
The v10-r4 proposal remains immutable at
`scratch/review/catalogue-v10-r4/proposal.json`, SHA-256
`dacef0040a70f33bf8ec9c2f2a427a02624dcd55e960ea0844c1717321294bda`.
It contains **360 unknown B / 36 intervals / 10 anomalies**. The independent
restart review sealed **FAIL V10R4-R01**: fixed-width pool reads did not maintain
an all-path little-endian interpretation after SETEND BE. Its failed records
remain in `scratch/review/catalogue-v10-r4/restart-20261020/`. There is no claim of
an occurrence of that defect in this image, and the defect is not suppressed here.
The separate source-only v10-r5 repair
**`88ae992f75eb4195e5eef19fc9c5feef5bd5ca04`** is now source-merged into this
worktree before any new generation. The immutable evidence chain is
**r4 → r5 → v11**, with r5 child SHA
`b758574088ae4a9c85f8dd89548066dec48d060d622aa43dcef36707827d2f52`.
Only loader/provenance/fixture conflicts were reconciled; the repaired reader
and new endian regression bodies were not changed. The combined generator is
v11 and explicitly preserves r5 repair provenance. No checks or generation were
rerun during this integration. V10-r4 is never adopted by this report.

The authoritative adopted state is main's **v9-r2 / GATES v1.19**, including the
finalized cmodels identities; this feature parent's older gates/accepted files are
not copied over main. No main merge, gate revision, canonical catalogue/config/name
change, prepare/finalize/adoption or history operation is authorized. The existing
accepted `00203a68` and c009 `001e81fc` records are untouched by this delta.

## Exact target dispositions

| Investigated half-open interval | B | Proposed disposition | Complete evidence, not byte appearance |
| --- | ---: | --- | --- |
| `0x001569b0–0x00156a30` | 128 | **Resolved: unowned rodata** | `btod.o`, complete section `CL$$btod_div_common`, 840 B at independently bound `001566e8`; ARM code `[0,712)`, complete `$d` object `[712,840)`. Authenticated `_btod_ediv` at `00156b0c` binds section placement through its actual R_ARM_CALL at `00156b2c`. All 840 B and all 128 data B equal exactly. Keep the existing ordinary owner at **712 B**, with no new library/API classification. |
| `0x001944ac–0x001944e8` | 60 | **Resolved: unowned rodata** | `lludiv5.o`, complete 752-B `.text` at independently bound `001941f8`; code `[0,688)`, complete `$d` object `[688,752)` = 64 B at `001944a8`. Original compiler/ABI identity and the independently owned signed-division caller `0018d204` / call `0018d22c` bind the entry. Full752B relocation reencoding and full64B data equality pass. The existing first4B owned literal and **692-B runtime identity remain unchanged**. |
| `0x001964e8–0x00196504` | 28 | **Resolved: existing runtime owner's descriptor4B + continuation24B** | `ffixull.o`, complete108B section at independently bound `00196498`; mapping code `[0,80)`, data `[80,84)`, code `[84,108)`. Complete108B reencoding includes its actual R_ARM_CALL at `001964e4` to authenticated160B `__fpl_fnaninf` at `00145570`. The descriptor's exact4B read and every-path adjusted-LR continuation are independently proved; no normal-case behavior test substitutes for this. |

All three target intervals total **216 B**. The intended isolated delta is
unknown **360 → 144 B**, intervals **36 → 33**, anomalies **10 → 9**. These are
**proposal accounting**, not exercised generation results. Owners and ordinary
creditable denominator stay **4,743 / 4,582 / 1,123,224 B**; the runtime owner's
new physical extent adds no ordinary C credit. **This is not zero-unknown closure.**
This three-interval delta is kept separate from the subsequently authorized2B
selector entry below; it is not silently changed to218B.

## Exact private inputs and inherited metadata

Licensed **Arm Compiler 5.04 update 2 build82 only**. Objects are read privately in
memory with the existing archive/ELF parser; no extracted object, ROM payload,
instruction word/text, disassembly or vendor source is published. The bounded
probe selects four pre-existing member offsets, not an arbitrary data search or
an exhaustive archive/image rescan. Prior r5/r7 identity and relocation rows are
reused unchanged. Whole-body coincidence alone is not promoted to API identity.

| Member / selected archive | Member offset | Member SHA-256 | Complete placed section SHA-256 |
| --- | ---: | --- | --- |
| `btod.o` / `c_4.l` | 851118 | `b01721e5e77675da3c41315bd8e735210918c96aae839686ed97d5d6dd333dd0` | `9d7eb50894287249b1c65c775ce361a9f9e9c4db0aca800f91333bb6d4c0a384` |
| `lludiv5.o` / `c_4.l` | 325602 | `fd4702498a2fdaa48bbc212a859dc5436f5eb54e3e2a10fc3d11a50c38ab6d30` | `fdcdb4aafd4ee9fde2b607ee93543a02e05db75189adb76a35a0c3f2def02709` |
| `ffixull.o` / `fz_4m.l` | 104744 | `86ef9bc7d935c7e447318f05a0dd747f7f9994e216706303adb24d5d248ecfd2` | `2b63549d8c7ae77399b093bf1b38e813ae7a7f516d9f50993c9bfc9567ce6c89` |
| `fnaninf_ofz.o` / `fz_4m.l` | 118400 | `0ceae8da0432f81eba7d3cc814df3d8026c3a18910d2f1be9491c87ce6da4275` | `35f37e26b387197bd6786a836291ce2d5154d47c257d35eaeb84759d05e0ea0e` |

Archive SHA-256: `c_4.l`
`595f005c9a8b73bea5d437e7f1392569d0bd25f12feb04aab2a31629c2c86fb2`;
`fz_4m.l`
`73ae2afb134975f5d46e76740e4786253c11568dac94bdbfcd7a59363a534689`.
Data object digests are exact, unmasked:
128B `82f10a50950cb56a154a587c5c03b40c9e66c251600d7b631f0f56dea736183b`;
64B `d438dff34172d789f6ecc54f3ffd16323a223b0c1aa817564d29ff25be0a136d`;
descriptor4B `2d26b36b04043ffabec88eadc26b2102c6accfd7321a7264eb8b79ed8c88f4df`.

The unsigned division helper's sole ordinary relocation resolves to original
`00144220`, but its spelling `__aeabi_ldiv0` is explicitly
**observed external target, not authenticated API identity**. This qualification
is not erased by exact reencoding. Complete data bounds do not require asserting
that external target's source name or behavior. The other call relocation targets
a separately authenticated complete dispatcher body; the inherited `_frem`
caller (`0014a860`, R_ARM_CALL at `0014a960`) supplies its independent identity.

Inherited identities: `scratch/libid/proposal-504b82-r7/handoff.json`, research
commit `c80a50e37e74eb1d50bc0b9e9500d7a7fb8f0663`; source match indices
17539,19647,21764,21766,21770,26107,28901,28905 are copied as metadata with their
retained relocation checks. No previous scan is rerun. The r7 final
`scratch/libid/followup-r7/extent-adjudication.json` remains authoritative for the
three unrelated Thumb declarations **22/10/620 B**, never26/20/632. Their extents,
foreign-reference sites, classifications and all failed/provisional evidence are
untouched.

## Descriptor and continuation proof

The authenticated dispatcher performs exactly **one4B read at its incoming LR**
and advances LR by4. A bounded finite-set/unsigned-interval abstract proof starts
with **every possible incoming core register value and NZCV flag combination**,
not a single emulator path. It models only the observed ordinary integer effects,
exact flags when operands are finite, and all predicates otherwise. Unknown
instructions, memory/side effects, state restoration, loops, unsupported shifts,
internal computed dispatch and escaping transfers fail closed. Register shifts
use ARM's low-byte amount semantics. The public little-endian ARM entry contract
is explicit; caller and dispatcher paths admit no SETEND or state-restoring effect.

All nine descriptor category values0–8 are conservatively covered. For this exact
caller's descriptor, **every path** restores the saved original r4/LR pair and
resumes only `001964ec` or `001964f8`; no path resumes at descriptor data or escapes
the108B section. Each target owns a complete bounded12B normal-return continuation,
returning via the restored original LR. The exact entry-to-call prefix separately
proves that the one saved r4/LR pair dominates every exceptional call. Every
newly owned24B instruction is covered by those two continuations. No global
`noreturn` property is inferred: this dispatcher redirects to caller-specific
continuations, and other descriptors can select different behavior.

The `undecodable_reachable_code` anomaly at `001964e8` is explicitly reopened and
resolved by data-format/continuation evidence, not deleted because adjacent bytes
look plausible. No independent callable entry is invented at `001964ec` or
`001964f8`; they remain internal continuations of the existing `00196498` identity.
Its sole existing entry and every incoming caller source remain unchanged.

The historical **80B / 404 normal f2ul cases** classification remains nested,
unchanged, as historical evidence. It is not retroactively described as a108B
whole-body proof. A **new108B compiler-generated runtime record** binds the
complete exact section, descriptor and continuation contract. The first two
intervals become unowned data only: neither preceding function is grown.

## Preserved negative measurements

Old indexed-read probes remain immutable, both investigation commands exit0 but
leave their bounds unproved. `bounded-index-probe.json` records the btod read
starting from base `00156930`, index range0–16777215 and access end18180400;
its unsigned division bound records index0–4294967295 and access end4296623275.
Neither was bounded within the image. The exact-build section metadata now gives
an independent complete data-object bound; these old approximate indices are not
silently marked proven. The btod incoming mantissa index safety is **still not
claimed**. `indexed-read-bound-census.json` retains the real indexed-read sites,
widths and biased bases. Those negative measurements are bound in the new evidence.
The old normal f2ul cases still exclude exceptional paths.

No byte-value-based padding/table rule, foreign-tail extent borrow, fabricated
alias or whole-body coincidence as sole historical API identity is introduced.
The dispatcher itself's old code/data hole remains outside this narrow physical
delta; complete section authentication is not permission to change its owner
record without separate accounting.
## Supplemental33-interval evidence intake

The payload-free scout report `scratch/integrator/catalogue-v11/ALIGNMENT-FACTS-r1.json`
SHA `5cf178ac2aa6a255d1ee0e43802199acd234b0f255b6f0557301478d48dbeb4b`
is an evidence intake, not a classification grant. A bounded13-member ELF metadata
probe extracted actual declarations, mapping offsets, section sizes and alignment
for all12 strong leads. A separate two-member probe reencoded only the complete
`_printf_fp_dec.o`1040B and `rt_ctype_table.o`20B sections at already referenced
placements. Both probes exited0; no exhaustive archive/image scan or source/packet
search was used.

`scratch/runtime-data-v11/alignment-closure-findings.json`, SHA
`19d7ccbe9f217a686babdd91c31c7909f6222313fe103c928787b0ca10ad9656`,
explicitly accounts for all33 intervals. The two-byte `0019f910–0019f912` selector
was first sealed as **fully evidenced independent-entry findings**. Main then
explicitly authorized its separate source-only integration. It is now a new
unadopted library entry, based on actual global
`STT_FUNC __lib_sel_fp_printf`, offset0, declared2B, `$t` mapping;
complete1040B exact relocation closure; independent same-section references at
`0019fb22/0019fb44/0019fb84` bind offset2 to`0019f912`; the authenticated620B
owner at`0019faa8` is at offset408. Its2B fragment is an unconditional normal
return. This supplies a real declaration/placement witness, not generic return
similarity. The existing620B owner and rejected632B growth remain unchanged.
The new entry is separately accounted, never transferred to the ambiguous
`00100c9c` stub or used to grow the620B owner.

The other32 intervals remain open. Nine restored halfword holes have actual
4-aligned `$d` section mappings, but **no data-object declarations and no complete
negative-reference/fill proof**; historical broad read vetoes still bar padding.
The48B allocation/free fragment has exact26/22B declarations in a66B Thumb section,
but no independently authenticated original entry/caller placement: the18B
initializer's malloc import remains unproved. The4B ctype tail has a complete20B
declared mapped section and exact reencoding, but no direct original architectural
PC-addressed read; its relative computed-transfer contract/API identity is unproved.
No unsupported padding, rodata, API rename or extent rule is installed for them.

Metadata artifacts: `alignment-object-metadata.json` SHA
`c3b89be37226dc2c5922b8c0581f679953a8ecf1f1cb67dcb0e3f7db51f61008`;
the exact supplemental section hashes and every checked relocation are in
`alignment-supplement-measurements.json`. Separate entry delta: owners **+1**,
library owners **+1**, ordinary functions/bytes **0**, unknown **−2B/−1interval**.
Combined source-only proposal: **4,744 owners /81library /4,582ordinary /
1,123,224ordinary B;142unknown B/32intervals/9anomalies**. No whole-image generation
has been run. Bounded application of only these four exact intervals to the fixed
proposal metadata measured those totals, retained all4,776 prior entries, added
only`0019f910/Thumb`, and checked all **2,297 c009-r2 prepared identities**.
Its receipt `scratch/runtime-data-v11/source-integration-bounded-evidence.json`
SHA `24903c7deae02f4d6fbda44a802e1bf2b7c7dd06d7be3c1d65fd740e45760a10`
records exit0; this is not generation, suite or independent review.

Prepared protection is frozen in `research/catalogue_v11_protection.json`, SHA
`1c2cf1bf6396f5a78251cb9ef0f1a5b59eff1c77517341b47399143caf6df415`.
Generation compares every complete proposal record/entry/extent for those2297
identities, not merely the old1602 protection count. The inherited separately
reviewed literal metadata at`00203a68` and`001e81fc` is explicitly pinned, not
reinterpreted or changed by v11. Main's source/header/harness freeze is untouched.


## Artifacts and exercised investigation

Tracked evidence: `research/catalogue_v11_evidence.json`; generic fail-closed
implementation: `tools/catalogue_runtime_data.py`. `tools/analyze.py` applies the
new closure after existing classifications, then replaces only the separately
proved runtime extent record. Parent/child hashes, every existing protected
record, unknown-byte accounting and changed-extent evidence are checked by the
existing generation pipeline. No generator reads licensed archive contents or
live author directories. New regression coverage is in
`tests/test_catalogue_runtime_data.py`; mandatory fixture and exact current-image
expectations in `tests/test_analyze.py` are updated. **These tests are authored,
not run.**

Private bounded proof receipts under
`scratch/worktrees/catalogue-v11/scratch/runtime-data-v11/`:

| Artifact | SHA-256 | Exercised result |
| --- | --- | --- |
| `object-witnesses.json` | `ab198cc89b94edb9c87bc59bf66b9ecf1e190fd140501ab4776f3c58aa6a4b96` | four complete placed sections and reference/partition metadata |
| `object-witnesses-command.json` | `7b3bbe3dd5a0ce49654756020ce9d186520d85291a6247d007ea341191ff840f` | exit0; bounded four-member evidence probe only |
| `descriptor-paths-command.json` | `127bf290c204bb337e4ca2eb1aabd51858c45d4df9957cb020c0b09bbddb45c1` | exit0; sound all-path category/continuation overapproximation |
| `initial-object-inspection.json` | `b29dadda18b57a421595a20e0775a07fa8b3f9fdf21aec674051bb875b489b4d` | initial independently selected section/symbol/relocation metadata |
| `closure-findings.json` | `229a4c051bb4bba790587b2e0bcf600ce174b873ef29b58a28ce306f01792bef` | all three target dispositions and every other still-open interval; no generation claim |
| `caller-prefix-paths.json` | `2fd7b75aedc678e797d1eb7732141a38e4f74c64e8e1b695987775854ce44292` | complete80B owned prefix; original saved pair dominates both exceptional paths |
| `caller-prefix-command.json` | `5cf7209b312213ee0e1cd16005c0ad5791a4ba789f2673dc80e32ea7b90eaf99` | exit0; bounded20-instruction original caller probe only |

All executable evidence-probe outputs contain addresses, bounds, safe semantic
facts and hashes only. Their private scripts are research scaffolding, not
published vendor code or source-match candidates. The immutable receipts remain
available even after scaffolding removal.

**Not run:** tests, suite, build, compilation/matcher, linter, formatter or full
catalogue generation. Main performs clean suite, generation, exact
delta/protected-identity checks and fresh independent review of the combined
repaired source stack. No adoption or C credit follows from these findings.

## Main-only readiness commands

After granting the heavy slot, from `scratch/worktrees/catalogue-v11`:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -B -m unittest tests.test_catalogue_runtime_data tests.test_catalogue_endian tests.test_catalogue_endian_cli tests.test_analyze
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -B -m unittest discover -s tests -t .
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -B tools/analyze.py catalogue --out scratch/analyze/catalogue-v11.json
```

The first command's original-image stability test includes two generations and
must not run before that slot is granted. No command above was run by this author.
The ordinary generator always binds r4/r5/v11 evidence and the2297-identity
prepared snapshot; no main merge, adoption or gate revision is implied.
