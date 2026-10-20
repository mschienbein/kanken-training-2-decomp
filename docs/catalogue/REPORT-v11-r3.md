# Catalogue v11-r3 — exact reassessment of three unadopted pointer entries

## Failure, diagnostic grant and revised authority

The v11-r2 protection handoff at
`98107cdad6d917086fbc36f687fbde933c500e15` removed the original protected-record
refusal. Main's fresh integrated r2 suite nevertheless **failed**: exit1,
887 tests / one skip, only `test_original_invariants_and_stability`. The failing
assertion compared actual **4,741 owners / 4,579 ordinary / 1,123,188 ordinary B**
with the ungenerated **4,744 / 4,582 / 1,123,224** hypothesis. This failure remains
immutable in `scratch/integrator/catalogue-v11/integrated-r2-suite-r1.json`.
The unchanged failed suite was not rerun.

The test's two generated proposals were in a `TemporaryDirectory` which had
already exited before the tuple assertion, so neither artifact survived. After
that inspection and bounded metadata investigation, Main expressly granted
**exactly one standalone diagnostic generation**, not another suite or an adoption
run. Source was pinned to `98107cd`; the complete command, actual exit and hashes
are retained. The result was **exit0**, 117.121732 s captured wall, 9,501,806 B:

- `scratch/closure/v11-r2/diagnostic-proposal-r1.json`
- SHA-256 **`9b04527f05f081a07d98a2809609d2a61938f838b028f0a5cb478d6efc996a2b`**
- Receipt: `scratch/closure/v11-r2/diagnostic-generation-r1.json`
- Atomic slot release: `scratch/closure/v11-r2/DIAGNOSTIC-GENERATION-RELEASED.json`,
  `released: true`, `actual_exit_code: 0`, published immediately after completion.

This was diagnostic artifact recovery against the failed r2 source, **not a
passing integrated gate or fresh v11-r3 generation**. No further heavy job was run.

Main then explicitly decided to retain the sound r5 refusals and withdraw exactly
these three **unadopted r4 pointer-entry proposals** into unknown. The old
4,776-provisional-entry retention hypothesis was never authority to fabricate a
pointer witness. **All 4,730 adopted v9-r2 entries and 2,297 prepared complete
records remain mandatory.** Original r4/r5/v11/r2 reports, evidence, hypotheses and
failed receipts are unchanged. No accepted credit is withdrawn or added.

## Exact three-owner cause, not a count adjustment

| Withdrawn r4 proposal | Complete old span | Former source / pool definition | Current certificate refusal |
| --- | --- | --- | --- |
| `001005c0` | `[001005c0,001005ca)`, 10 B | `001005ca` / `001005d4` | Source contains an uncertified Thumb16 effect at `001005dc`; no reachable all-path LE certificate at the pool definition |
| `001005fa` | `[001005fa,0010060a)`, 16 B | `00100360` / `00100370` | Uncertified Thumb16 effects at `00100364`, `0010037e`, `0010038c`, plus an unresolved computed transfer at `00100394`; no LE pool-value certificate |
| `001009d4` | `[001009d4,001009de)`, 10 B | `001009de` / `001009e8` | Source contains an uncertified Thumb16 effect at `001009f0`; no reachable all-path LE certificate at the pool definition |

The bounded source probe decodes only the three existing source owners, **118 code
bytes**, and serializes semantic refusal categories/addresses, not instruction
words, disassembly or vendor payload. All three source records and code ranges
are unchanged between r4 and the diagnostic. Their old dominated odd PC-relative
pointer witnesses now have **no admitted pool value**. The diagnostic's actual
whole-catalogue `read_vetoes` independently records
`unproved_data_endian_pool_value_veto_only` at all three definitions; its
`pc_relative_pointer_witnesses` is empty.

R5 intentionally refuses an owner certificate for opaque effects/computed
transfers, including an apparently unreachable suffix. This is a conservative
missing-proof result, **not a claim that the target bytes cannot be instructions
or that architectural callability is disproved**. A closed-looking body cannot
supply its own incoming pointer witness. R3 does not broaden the effect inventory,
weaken joins/entry/processor-state checks, add positive address exceptions or
restore an unjustified entry.

None of these three addresses is an adopted v9-r2 entry or a prepared identity.
Their old entry evidence is pointer-only, with no direct incoming source. All
36 B remain **unknown, owner null**, not padding, rodata, another function's tail,
newly classified runtime or C credit.

## Complete measured delta

`diagnostic-complete-delta-r1.json` retains every removed/added/changed complete
function record, every entry loss/addition/change, the independent old/new map
intersection, all old/new anomalies and every prepared complete-record check.
Compared with the immutable sealed-failed r4 proposal:

- **Removed owners:** exactly `001005c0/10 B`, `001005fa/16 B`, `001009d4/10 B`.
- **Added owner:** only independently evidenced selector `0019f910/2 B`, library.
- **Changed retained owner:** only runtime `00196498`, its separate 80 → 108 B
  descriptor/continuation proof; no ordinary C extent change.
- **Entries:** 4,776 → 4,774: exactly three provisional entries removed and the
  selector added. All **4,773 retained r4 entries have identical complete entry
  metadata and ownership**; no other entry is lost, added or changed.
- **Adopted:** all **4,730** v9-r2 entries survive.
- **Prepared:** all **2,297** complete proposed record hashes survive, including
  the independently bound inherited literal metadata transitions.
- **Protected union:** all **2,298** complete expected records survive. A separate
  retained-artifact check also confirms all **1,621 accepted identities remain
  ordinary**, with no accepted/prepared protection exemption.
- **Anomalies:** remove only `001964e8 / undecodable_reachable_code` under the
  independent descriptor proof; add none. Ten → nine.

The physical map changes in exactly seven disjoint intervals:

| Interval | B | r4 → diagnostic |
| --- | ---: | --- |
| `[001005c0,001005ca)` | 10 | function → unknown, owner null |
| `[001005fa,0010060a)` | 16 | function → unknown, owner null |
| `[001009d4,001009de)` | 10 | function → unknown, owner null |
| `[001569b0,00156a30)` | 128 | unknown → unowned rodata, original runtime data proof |
| `[001944ac,001944e8)` | 60 | unknown → unowned rodata, original runtime data proof |
| `[001964e8,00196504)` | 28 | unknown → existing runtime owner, descriptor + continuation |
| `[0019f910,0019f912)` | 2 | unknown → separate exact-sized library selector entry |

Keep the original **216 B runtime closure**, separate **2 B selector**, and
**36 B reopened proposals** distinct: net unknown reduction is 182 B, not 218 B.
Measured diagnostic totals are **4,741 owners / 4,579 ordinary / 1,123,188 ordinary
extent B / 178 unknown B / 35 unknown intervals / nine anomalies**. This is **not
zero-unknown closure**, a new accepted denominator, or source/data credit.

## Distinct successor and consumer invariants

`research/catalogue_v11_r3_evidence.json` is a new hash-bound child of unchanged
`catalogue_v11_r2_evidence.json`. It freezes the complete r4 owner-identity and
entry metadata tables, old unknown intervals/anomalies, all three exact withdrawn
records, their unchanged original extent digests, unchanged source-record hashes,
former pointer witnesses and every negative certificate reason. It also binds the
r2 failure, diagnostic command/output and complete comparison. No generator reads
a live private author directory or the old scratch proposal to obtain authority.
The complete immutable chain **r4 → r5 → v11 → r2 → r3** remains in the input hashes.

The public builder/CLI now selects r3 and applies an additional **consumer check**;
it does not change discovery or classification. The loader refuses duplicated or
missing reference identities and any withdrawal of an adopted or non-pointer-only
entry. For each withdrawal the consumer requires:

1. no current owner/entry or overlap with a protected owner;
2. the unchanged original whole-span hash and exact unchanged pointer-source
   complete-record hash;
3. the actual whole-catalogue pool-value veto, absence of the old positive pointer
   witness, and complete unknown coverage;
4. the exact current bounded processor-effect/computed-transfer refusal inventory.

It then derives the **entire** owner/entry delta from the frozen reference and
separate proved runtime/selector closures. Every retained entry is compared in
full. Unknown intervals and anomaly changes must equal the independently proved
closures plus exactly the reopened spans. Statistics are computed from those
identity/range sets and checked against the writer; there is no replacement
numeric count accepted as proof. Existing multi-entry topology retains the same
classification rule as `apply_classifications`: attaching runtime authentication
does not reclassify a `multi_entry` owner.

The original-image test independently compares the exact ordinary address/size
set, all reference entries minus the three withdrawals plus the selector, and the
actual unknown address set against separate 216 B/2 B closure claims and reopened
36 B. It requires each actual pool veto and all prepared complete hashes. Old
failing tests remain in commit `98107cd`; their replacement is tied to exact
certificate evidence, not a lowered assertion.

No r5 endian/SVC rule, protected-record guard, GATES v1.19 text, accepted source,
header, catalogue/config/name, receipt, history or canonical ledger is edited.
The fixed library declarations remain **22/10/620 B**, never 26/20/632.

## Authored regressions and actual readiness checks

Five new methods in `tests/test_catalogue_entry_reassessment.py` exercise the
actual synthetic public producer/consumer and CLI: a BE-refused pool-derived
callback remains unknown; an admitted or protected entry cannot be withdrawn;
missing vetoes, changed reasons/source hashes, rodata substitution and unrelated
entry/extent changes refuse; existing multi-entry topology survives classification;
and an altered r3 parent hash produces a real nonzero CLI exit without output.
Existing fixture/CLI revision bindings and the original-image invariants are
updated. **No test method/module/suite was run by this worker after these edits.**

Tracked summary: `research/catalogue_v11_r3_checks.json`. All receipt commands,
actual exits, stdout/stderr and hashes remain under `scratch/closure/v11-r2/`.

| Exercised check | Exit | Actual result |
| --- | ---: | --- |
| Single granted source-98107cd diagnostic generation | 0 | 117.121732 s; retained complete proposal and atomic heavy-slot release |
| Bounded retained-diagnostic protection check | 0 | 0.506841 s; all 2,298 complete protected records, all 2,297 prepared records and all 1,621 accepted ordinary identities verified |
| Final r3 public consumer over that retained artifact | 0 | 1.186244 s; exact complete identity/entry/range/anomaly delta; missing certificate, rodata substitution and attempted protected retirement all refused |
| Final synthetic r3 public CLI smoke | 0 | 0.443506 s; valid CLI exit0, altered-child CLI exit1, no refused output, both accepted/prepared-only claims protected |

Two initial **author-check failures are retained**, not erased or retried
unchanged. The first consumer expectation incorrectly converted two already
`multi_entry` authenticated owners into runtime; it was corrected to use the
existing classification rule, without changing actual output. The initial CLI
fixture used adopted pre-typed-repair anomalies as its *prior proposal* reference;
the fixture now uses its independently typed public-writer proposal, matching the
real r4/r2 distinction. The production anomaly guard was not relaxed. Their
exclusive `*-r1.json` receipts remain beside corrected `*-r2.json` receipts.

The final consumer check reuses retained diagnostic metadata and decodes only the
bounded source owners. The CLI smoke is synthetic. **No additional current-image
generation, suite, build/compiler/matcher, whole-tree audit, linter or formatter
was run.** Source readiness is not integrated verification or adoption.

## Main-only remaining verification

After integration into Main's isolated worktree and a new serialized heavy-slot
grant, capture new immutable r3 receipts for these exact commands, without piped
gate output:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -B -m unittest discover -s tests -t .
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -B tools/analyze.py catalogue --out scratch/analyze/catalogue-v11-r3.json
```

Do not separately run the heavy original-image module before that integrated
suite. Obtain a fresh independent review of the complete successor source and
fresh generated proposal, including all adopted/prepared protections, exact three
unadopted withdrawals and every negative certificate reason, seven physical
interval changes and single anomaly disposition. Preserve the r2 suite failure
and diagnostic records. No frozen-Main edit, adoption, prepare/finalize, history
operation, credit or push is performed here.
