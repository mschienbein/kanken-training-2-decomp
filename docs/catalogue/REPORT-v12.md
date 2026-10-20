# Catalogue v12 — two independently bound local runtime entries

## Scope and immutable predecessor

This is a distinct, unadopted successor to v11-r4, based on source
`32620ff561b7f3c21d7f7059be5e391615cd4ef4`. It closes only the proposed
`001005c0 / 10 B` and `001009d4 / 10 B` entries. It does not close
`001005fa / 16 B`, classify the halfword holes `001005ea` or `001009fe`, extend
any declared function, change canonical names, or grant C/source/data credit.

The complete scout report
`scratch/integrator/catalogue-v11/THUMB-CERTIFICATE-SCOUT-r2.md`, current r3/r4
reports, exact object metadata at `alignment-object-metadata.json:439-580`, and
the cited historical fragment, relocation and source-family refusal metadata
were read. Fragment equality was a lead, not the admission certificate.

Main supplied the fresh **unadopted** r4 proposal:

- `scratch/worktrees/catalogue-v11-r4/scratch/analyze/catalogue-v11-r4.json`
- SHA-256 `55168a79db0d76debddd3b55cf62beed6ab29dc73396d2d3dd29e882ac6e3451`

The bounded proof independently checked that exact proposal digest, the original
image/exheader digests, every inherited generator input binding, the four r4
source bindings, and the r4 leaf-refusal policy. It ran the **unchanged r3
consumer** over the complete retained r4 metadata, decoding only the three
bounded pointer-source owners. Its negative-stage receipt remained identical.
This is not adoption, completion of the concurrent independent r4 review, or a
new current-image generation.

## Complete pool-independent object proof

Only two exact members of the linked private 5.04-b82 archive were inspected;
there was no installation, recursive author search or matching-body search.
Archive SHA-256:
`3583239cecab63fb14cd40478ffb4d6fbedd67abd1df5acaa332d47d274d1fbe`.

| Entry | Exact member / ar header offset | Member SHA-256 | Complete relocated 48-byte section SHA-256 |
| --- | --- | --- | --- |
| `001005c0 / 10 B` | `c_5.l(_printf_char_common.o)` / `656918` | `ca69465256b5d9a9b5a7909af7b9c23d3d82b2d3fc5692645db52a265ccb1c22` | `5636d4867ce871f5beed15b8346ce6436c44a0a3e01a60e7410403166e5ab59c` |
| `001009d4 / 10 B` | `c_5.l(_printf_wchar_common.o)` / `670346` | `56e3f5ea3ef9d52b795a524349060bc7dc7ce8dd04c57ba15240abb6fb850991` | `e59082f49f725fed856405bfb3bc8485c56551b6e8a90d358d75b52815159ddd` |

Each object has exactly one nonempty allocated section, `.text`, index 1,
alignment 4, executable and not writable, exactly 48 bytes. **All** section
symbols, mapping transitions and relocations were compared with the retained
metadata, then the entire section was independently reencoded and freshly
compared with its original-image interval in memory. No original bytes, words,
disassembly or licensed payload are serialized.

The complete, gap-free composition is the same in both objects:

| Section interval | Actual declaration/mapping | Disposition |
| --- | --- | --- |
| `[0,10)` | Local `STT_FUNC`, Thumb-tagged value 1, size 10: `_printf_input_char` / `_printf_input_wchar` | New separate library entry only |
| `[10,42)` | Global `STT_FUNC`, Thumb-tagged value 11, declared size **32**: `_printf_char_common` / `_printf_wchar_common` | Unchanged independently authenticated source code |
| `[42,44)` | `$d` begins at 42 | Remains the separate **unknown** halfword; mapping alone is not physical ownership |
| `[44,48)` | Same-section `R_ARM_REL32` naming the actual local definition | Unchanged existing source pool footprint; static relocation witness only |

The complete mapping inventory is `$t` at 0, `$d` at 42. The complete relocation
inventory is:

1. Section offset **34**, `R_ARM_THM_CALL` (10), undefined `__printf`, addend
   **-4**, observed Thumb target `0010064c`. Reencoding is exact, but this is
   explicitly **observed external target, not an authenticated API identity**.
2. Section offset **44**, `R_ARM_REL32` (3), local definition in section 1,
   value **1**, size **10**, addend **16**. Its static target is section base.
   The complete REL32 arithmetic, including the addend and Thumb tag, is checked;
   it is not replaced with a bare tagged-address comparison.

The source owners at `001005ca` and `001009de` were already authenticated in
frozen v10-r3 independently of these local-entry proposals. Their complete
record hashes, frozen authentication rows, classifications, exact member
identities, complete 38-byte original extent hashes and actual incoming code
references are bound. Source placement fixes section base as source address
minus **10**, without using a disputed pool value. Actual incoming sites are
`0010037a` from owner `00100360`, and `0010085a` from owner `0010083c`.
The historical snprintf-family caller spelling remains unresolved; no caller
API identity is invented or promoted.

The source's **catalogue extent remains 38 B**, including its existing pool and
unexplained internal halfword; its actual ELF function declaration remains
**32 B**. Neither is extended to borrow section tails. The prior **22/10/620 B**
declarations and their foreign-tail constraints remain protected by complete
baseline equality.

`research/catalogue_v12_object_proof.json` records all original/member/image,
section and partition digests, declarations, mappings, relocations and independent
source placement. The source-32620ff bounded proof's actual exit was **0**,
0.590260 seconds. Its exact source and receipt are retained under
`scratch/closure/v12-local-entries/complete-section-proof-r1.{py,json}`. The script
is historical source-pinned evidence; the current reproducible checker is the
public `catalogue_local_entries.py` CLI below.
The exact original `-c` argv is retained separately in
`complete-section-proof-invocation-r1.json`; the first receipt used a concise
display label for that payload without changing its exit, output or source.

Architectural authority: [AAELF32 symbol values and mapping symbols](https://github.com/ARM-software/abi-aa/blob/main/aaelf32/aaelf32.rst#symbol-values)
and [AAELF32 relocation types](https://github.com/ARM-software/abi-aa/blob/main/aaelf32/aaelf32.rst#relocation-types).
The REL32 check uses file-endian ELF relocation arithmetic, not a runtime
little-endian load certificate. R5's processor-state and whole-owner LE refusals
therefore remain unchanged and meaningful.

## Distinct successor and public consumers

`research/catalogue_v12_evidence.json` hash-binds the unchanged v11-r4 parent,
complete object proof and exact r4 baseline. Its two transitions are explicitly
**unknown/unowned -> separate function-owned library entry**. It contains no
copied positive function records and no canonical-name changes.

`tools/catalogue_local_entries.py` reuses runtime-section hash/binding and exact
unknown-range replacement patterns, without weakening the global-selector path
or admitting local symbols generally. The public builder performs the unchanged
r3 consumer **before** applying v12. It then requires exact commitments to all
r4 complete functions, physical ranges, statistics, anomalies, pointer witnesses
and closure evidence. New records are freshly derived from the local declarations,
not imported from the historical withdrawn pointer records.

Wrong member/name/offset/hash, changed source authentication or incoming reference,
wrong mapping, omitted/changed relocation or addend, altered section composition,
wrong size/tag/hash, protected overlap, a nonexact unknown interval, and an
unrelated baseline change all refuse. The public successor consumer removes
only the freshly derived two local records and reverses only their exact
physical/statistical deltas, then requires byte-for-byte canonical metadata
commitments to the complete r4 stage. A copied old pointer record is refused.
The generator/review now identifies v12 and binds the new module/evidence inputs.

R3's three withdrawals and negative certificates remain **historical intermediate
stages**, not reinterpreted positive evidence. R4's mode-sensitive-transfer guard,
r2 protection, all earlier sealed failures, source profiles, accepted/prepared
identities, frozen GATES v1.19 and canonical ledgers are untouched.

The exact delta relative to the retained r4 stage is two library owners/entries,
20 mapped function bytes, and no ordinary-C or anomaly change. `[001005fa,0010060a)`,
`[001005ea,001005ec)` and `[001009fe,00100a00)` remain unknown. Counts derived from
retained metadata are not a fresh unchanged-image claim; a current v12 generation
and independent review are still Main-only acceptance gates.

## Independent review finding -> strict physical-span successor

The independent reviewer found a **major fail-open consumer projection** in
`fdcc391`; that commit and its original receipts are preserved, not amended or
rewritten. The sealed synthetic public-producer/consumer observations are
`scratch/review/catalogue-v12/codex/consumer-projection-probe.json` and
`consumer-projection-probe-receipt.json` in Main's repository. The probe itself
exited **0** and recorded **three unexpected acceptances**. The known failure
was accepted as ground truth, not rerun against the unchanged failed source.

The consumer reversed **every** physical range naming a new entry as its owner.
Consequently a separate unknown halfword could acquire that owner, either with
`kind=function` or still `kind=unknown`, and another unresolved interval could
be attributed as extra function bytes. All complete function records, the real
10-byte span and statistics remained unchanged. Projection erased the forged
ownership and recovered the committed r4 hashes, so hash equality alone did
not establish the claimed exact physical delta. The original smoke did not
cover this owner-injection case.

The successor first requires each freshly derived entry's exact single
`[address,address+10)` function-owned span. It reverses **only that complete
range record**. Any other range naming either new entry as owner is refused
before projection, regardless of its kind. Every unaffected range is carried
through unchanged; complete old records, statistics, anomalies, pointer
witnesses and the negative closure stage must still equal their committed r4
hashes. No ancestor evidence, declared extent, object proof, source profile,
endian rule, unresolved interval or C credit is changed.

New public-consumer regressions include an exact positive projection control
and unchanged-record/statistic owner injections into both a separate halfword
and another unresolved interval, with both `function` and `unknown` kinds.
The bounded successor smoke also checks both actual halfword holes and the
remaining 16-byte proposal against both new owner identities. These checks
consume retained metadata; they do not discover or generate a current-image
catalogue.

Successor checks actually exercised after source completion:

- `focused-projection-tests-r1.json`: exit **0**, **8 tests** in 1.097 seconds
  (1.174983 seconds captured wall), including the new exact positive control
  and all four halfword/other-interval owner-injection variants.
- `public-consumer-smoke-r4.json`: exit **0**, 2.991994 seconds captured wall;
  the original bounded smoke was reexercised against the fixed source, then
  **four synthetic and twelve retained-metadata owner injections** were all
  refused, with exact synthetic and retained-r4 positive controls passing.
- `complete-section-proof-r2.json`: the enclosed public CLI actually exited
  **0** and rechecked both complete private sections, **96 B** in total,
  against the same hash-bound unadopted r4 baseline.

`research/catalogue_v12_projection_checks.json` binds this successor's exact
sources, sealed review evidence, new commands and receipts. Every old receipt
is unchanged. No full suite, current-image generation, build, matcher, ledger
audit, linter, formatter or adoption was performed.

## Regressions and remaining Main checks

The original `tests/test_catalogue_local_entries.py` authored six public-consumer
methods, including a real minimal synthetic ELF/archive, independent source
placement, unchanged negative-stage retention, complete-object rechecking,
misbound/incomplete certificates, old positive record substitution, unrelated
changes/protected replacement and real CLI parent/proof/baseline refusals. The
projection successor adds the two explicit positive/negative methods above.
Existing synthetic revision/CLI fixtures now freeze their independently produced
predecessor rather than skipping the new baseline guard. The original-image
acceptance test derives its exact extra identities/unknown subtraction from the
new certificates while retaining the ordinary address/extent set and every
prepared complete-record check.

The initial source-`fdcc391` bounded smoke (`public-consumer-smoke-r3.json`) exited **0**
in **1.507727 seconds**. It exercised the synthetic public producer/consumer,
real minimal ELF/archive rechecking, twelve certificate/candidate refusals, and
a real synthetic CLI (valid exit **0**, changed-child exit **1**, no refused
output). The current public CLI separately returned **0** for the exact r4
baseline and both complete private 48-byte sections.

The in-memory retained-r4 metadata consumer verified all **4,741** old complete
function records unchanged, all **4,579 / 1,123,188 B** ordinary identities/extent
unchanged, and exactly the two separate 10-byte library additions. Its derived
metadata has **4,743 owners / 158 unknown B / 33 unknown intervals / nine
anomalies**. It wrote **no successor catalogue** and performed no discovery or
current-image generation; these numbers are not fresh generation/adoption
measurements.

Two author failures remain immutable: r1 failed before consumer entry because
the scratch script lacked the worktree import context; r2's synthetic LE
reference classified a zero halfword that the BE baseline correctly refused.
The historical exact-range guard rejected that extra reopening. The synthetic
fixture was corrected to use a nonzero mapped-data halfword, retaining unknown
ownership in both stages; **no production guard was relaxed**.

Before the projection successor, no unittest method/module/full suite,
current-image generation, build/compiler/matcher, whole-ledger source audit,
linter or formatter had been run. The unchanged
`research/catalogue_v12_checks.json` binds that stage's exact sources, inputs,
actual exits, retained failures and scoped results.

After integration and a serialized heavy-slot grant, Main should run the full
integrated suite and **one** fresh v12 generation, with real exits and immutable
receipts, then the retained-artifact public consumer:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -B -m unittest discover -s tests -t .
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -B tools/analyze.py catalogue --out scratch/analyze/catalogue-v12.json
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -B tools/catalogue_local_entries.py --proposal scratch/analyze/catalogue-v12.json --prove-objects
```

The bounded original/member proof can also be independently reproduced without
generating a catalogue:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -B tools/catalogue_local_entries.py --baseline /home/michael/code/kanken-training-2-decomp/scratch/worktrees/catalogue-v11-r4/scratch/analyze/catalogue-v11-r4.json --prove-objects
```

Obtain independent review of the complete successor source/evidence and fresh
v12 proposal, including the complete r4 stage, exact two transitions, every
adopted/prepared identity, preserved 16-byte proposal and halfword holes,
22/10/620 constraints and all retained R5 negative reasons. Complete the separate
r4 review without altering its frozen inputs. No adoption, prepare/finalize,
denominator/history operation, accepted credit or push is performed here.
