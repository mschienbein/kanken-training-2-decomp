# Catalogue v11-r2 — current-ledger inline-metadata protection handoff

## Inherited failure and exact cause

This unadopted successor starts at isolated integrated commit
`307e1bd1486e8f81f744f4b1d771a90b6171259f`. Main's first integrated suite is an
immutable **failure**, not a passing gate: exit1, 879 tests / one skip, with only
`tests.test_analyze.CatalogueTests.test_original_invariants_and_stability` failing
at the real `analyze.main` invocation. The diagnostic is
`protected function 0x203a68 cannot retain its adopted catalogue record`.
The receipt remains `scratch/integrator/catalogue-v11/integrated-suite-r1.json`;
its digest is bound in `research/catalogue_v11_r2_evidence.json`. The unchanged
failed suite was not rerun here.

The ordinary CLI correctly reads **the current accepted ledger**, not the earlier
1,602-identity frozen union. `func_00203a68` is now accepted by cmodels. The typed
inline-object witnesses already make discovery reproduce its expanded literal
metadata, but `load_protected_functions` supplied its complete **adopted v9-r2**
record. `_split_linker_tail_extents` therefore refused the different literal list
before closure, prepared protection, or review enrichment could run. The final
`check_protected_functions` and review check would reject the same disagreement.
The old v11 prepared snapshot pinned the correct proposed records, but checking
that snapshot only at the end was not an authorization/handoff for the earlier
complete-record guards. Merely lowering a protected count or moving one check
would neither fix nor authorize that mismatch.

Both inherited transitions change **only `literals`**:

| Identity | Unchanged extent | Complete adopted record SHA-256 | Complete successor record SHA-256 |
| --- | ---: | --- | --- |
| `00203a68` | 2,540 B | `c6c942b981aecf0cf7004f0619b447f3ef856750786b1a40da3f16679925371d` | `a0b0c0d0c6b4f0f5cfd04b0e63a79d0d09cdc828dbd38bdba399404367422b53` |
| `001e81fc` | 752 B | `045976405b19240872809dc2852f81b9095704561be90009d65e4c923118241f` | `dc903285a1aa31521324ebc70db684ccf134567bdc013748a1ef103c57dc7a30` |

The separate earlier review already isolatedly relinked the same retained object
under both catalogues, with v6 + L1 explicitly selected, and obtained complete
2,540/2,540 and 752/752 MATCH. External L1 mappings, linker rewrites and removed
helpers were identical. The hash-bound retained compatibility record and source,
object, receipt, outcome/mapping digests are preserved in the new evidence.
Those are **inherited observations**, not fresh compiles, checkpoint acceptance,
or approval of the sealed-failed r4 producer.

## Fail-closed fix

`research/catalogue_v11_r2_evidence.json` is a distinct immutable successor of the
unchanged v11 evidence, not a replacement for v11, r5, r4 or the existing
`research/catalogue_v11_protection.json`. Original reports and all prior failed
artifacts are unchanged. The original **r4 → r5 → v11** chain remains loaded and
hash-bound; the successor adds only the exact metadata handoff.

The new closure protection loader:

- binds the successor to its parent, image and original typed-literal evidence;
- preserves the exact prepared-protection descriptor and verifies the frozen
  catalogue, names, c009-r2 ledger, prepare and model selection artifacts;
- checks every prepared `(name,address,source,profile)` claim against the complete
  2,297-identity snapshot, including uniqueness;
- requires the transition set and complete old/new hashes to be **exactly** the
  snapshot's inherited metadata differences; no transition or snapshot may be
  silently removed;
- verifies both live affected sources against their retained compatibility hashes
  and records these digests in the generator's input closure.

The CLI adds that authenticated prepared ledger to the existing accepted,
legacy-candidate, immutable-Git-blob and explicit `--protect-manifest` inputs.
Nothing is replaced or dropped. The measured current union is **2,298** identities:
all 2,297 prepared identities plus a retained older candidate. This is not a new
standalone count substituted for complete identity protection.

Before discovery or any complete-record guard, `resolve_protected_records`
checks each full old/new record, unchanged original whole-extent digest, identical
retained source/object outcome contract, and exact typed-witness digest. It
independently derives the expected successor from the **old record** plus the
complete original UTF-16 object bounds and zero alignment. Every span must stay
inside the unchanged extent, outside owned code/pools/entries, and have owned
reference sites. The derived complete record must equal the bound successor.
Any other changed field, invented literal, image/extent mismatch, unrelated
protected-record change or failed source binding refuses.

**No proposed record is copied into the recovered catalogue.** Discovery must
still reproduce every literal and architectural reference. The partition guard,
closure protection, final complete-record guard, full prepared-record hashes and
review check remain in place. The same resolver is used by the public builder
and the CLI; review receives the same resolved expectations. Both closure and
review metadata expose the exact old/new digest pairs instead of hiding the
transition behind an exemption.

No r5 endian-effect/state proof, SVC contract, classification rule, source model,
GATES v1.19 text, canonical source/header/config/name, accepted ledger or historical
receipt is changed. The **22/10/620 B** library declarations remain fixed, never
26/20/632. The **216 B three-interval runtime closure** and separately authorized
**2 B selector entry** remain independent deltas. All 4,776 prior entries remain a
whole-proposal acceptance obligation; neither transition changes an entry or an
extent. No source/data/denominator credit is granted.

## Authored behavioral regressions

`tests/test_catalogue_protection.py` adds eight methods using hand-encoded public
synthetic ARM inputs and the real public builder/CLI, with no mocks or source-text
assertions. A trailing independently loaded pool pins the original function extent
while a genuine typed inline object expands only its literal metadata. Tests cover:

- refusal without an authorized transition, and independent reproduction of the
  exact successor without mutating adopted records;
- refusal of extent/ISA/kind/entry/code/pool/noreturn/classification changes,
  invented literal ranges, wrong image/extent/witness hashes, duplicate transitions,
  unequal retained outcomes and changed architectural references;
- unchanged complete-record guards on other protected owners;
- current accepted plus prepared-only identity protection through the actual CLI;
- fail-closed parent, transition-set, snapshot, source, prepared-ledger and typed
  evidence mutations, with no refused output written;
- retention of explicit additional protection manifests.

The existing original-image invariant now loads the authenticated prepared ledger,
resolves the two exact expectations, and checks every one of the 2,297 complete
prepared record hashes. Existing synthetic fixture/CLI revision bindings include
v11-r2 while retaining the r5/v11 inputs and endian assertions. **These tests were
authored, not run by this worker.**

## Actual bounded proof, not current-image generation

Tracked summary: `research/catalogue_v11_r2_checks.json`. Both exclusive receipts
retain the exact interpreter, working directory and complete `-c` command in
`argv`, plus actual exit/stdout/stderr. No raw original data or disassembly is
included.

| Exercised bounded check | Exit | Actual result |
| --- | ---: | --- |
| `scratch/closure/v11-r2/bounded-metadata-r1.json` | 0 | 0.434303 s captured wall; loaded current protection inputs, independently derived both exact successors from the original bounded extents, and verified all 2,297 prepared record digests within the unchanged 2,298-address protection union; every protected entry/extent unchanged |
| `scratch/closure/v11-r2/synthetic-public-cli-r1.json` | 0 | 0.537069 s captured wall; one synthetic public CLI smoke: valid invocation exit0 with actual typed-writer function equality and accepted/prepared-only protection; changed transition-set invocation exit1, exact refusal diagnostic, and no refused output |

The metadata probe reads original input privately but performs **no original-image
catalogue generation**: it checks only frozen metadata and the two exact original
function extents. The synthetic smoke uses no original bytes and executes no test
method/suite. No unchanged failed input was retried. No tests, full suite, build,
compile/matcher, current-image generation, whole-tree audit, linter or formatter
was run here.

The earlier source-only hypotheses **4,744 owners / 4,582 ordinary / 1,123,224
ordinary extent B / 142 unknown B / 32 intervals / nine anomalies** remain
hypotheses until Main's actual current-image CLI succeeds. This report does not
claim that the fresh whole-image producer passes or that v11-r2 is adopted.

## Remaining Main-only verification and adoption prerequisites

After source integration into Main's isolated integration worktree and acquisition
of the serialized heavy slot, run the following once, without piping gate output:

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -B -m unittest discover -s tests -t .
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -B tools/analyze.py catalogue --out scratch/analyze/catalogue-v11-r2.json
```

The integrated suite includes the authored regressions and the original-image
stability test; do not first run that heavy module separately. Capture actual
exits under new immutable r2 receipt names and preserve the inherited failed run.
Then obtain a **fresh independent review** of the complete generated proposal and
source delta, checking all 2,297 prepared records, all 4,776 prior entries,
accepted/candidate compatibility and exact old/new interval/anomaly accounting,
with runtime216B and selector2B deltas separate. The generation must bind r4, r5,
v11, v11-r2 and the original prepared snapshot. No merge into frozen Main,
prepare/finalize, adoption, history update, credit or push is performed here.
