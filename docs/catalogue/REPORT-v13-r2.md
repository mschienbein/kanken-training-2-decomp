# Catalogue v13-r2 — fail-closed absence and bounded-read dispatch

## Immutable failed predecessor

This is a distinct successor branch/worktree from
`0c3fd934009d50c18306d54df82abd5ac0da6d94`. The original `catalogue-v13`
worktree, report, proposal and sealed independent **FAIL** remain unchanged.
The failed proposal's SHA-256 is
`e333779b6f2272c70cc166f915aa925abbcb06641a11f62f4443368ab847e743`.
The independent seal SHA-256 is
`5e46697794546a96c57ab5284068e3e407e1bdd042596f757f9c706ccb7c98f3`.

Both findings are accepted as ground truth. No unchanged failed-source probe was
rerun to reconfirm them. The review's successful image reproduction does not
waive either source defect and is not a passing v13 seal.

`research/catalogue_v13_r2_review.json` retains the exact reviewed failure
metadata. `research/catalogue_v13_r2_evidence.json` binds its digest, the old v13
parent evidence, the original image and both finding identities. The generator
and review now identify **catalogue-v13-r2** and bind these new inputs without
changing any ancestor evidence or importing copied positive function records.

## V13-CODEX-F01: absence cannot supply a consumable path

The failed implementation returned a filesystem sentinel for a declared-absent
label. An absolute extra-manifest label discarded the sentinel prefix during
path joining. A materialized sentinel file/symlink could also supply live bytes.
The actual protection consumer could consequently change protected identities
without changing the frozen manifest.

The root fix separates two operations:

* `catalogue_frozen.is_present` is a **boolean-only optional-input probe**. A
  genuinely absent optional input returns false, never a candidate path.
* `catalogue_frozen.resolve` is consumption. A declared-absent label **always
  refuses consumption**, even when no file exists, whether the requested extra
  manifest was originally expressed relative or absolute.

Absence assertions must be unique, confined, normalized relative labels and
cannot also name registered inputs. Every load/probe/resolution checks that
neither the original location nor the historical sentinel location is a file,
symlink or dangling symlink. Materialization refuses; it is not treated as a new
registered input. Absolute and traversal absence declarations refuse before any
path can escape its intended root.

The protection consumer uses the boolean probe for optional names/candidate
presence, and resolves only present inputs. Mandatory and explicit manifests
still require their exact retained descriptors. A registered absolute extra
manifest remains supported, but only its hash-bound retained copy supplies
content; later live-file changes cannot change protection. Empty maps with no
consumption still pass. Unregistered, missing and hash-mismatched consumed
inputs retain their refusal behavior.

## V13-CODEX-F02: exact bounded vetoes exclude padding

The conditional-read producer correctly emitted an exact
`conditional_read_veto_only` interval, but v13's padding guard inspected only
`read_footprints`. The independently produced bounded reference was therefore
lost at admission and public validation.

The same `_no_reference` guard now examines `read_vetoes` for both external and
internal alignment rules. Every intersecting bounded veto refuses padding,
including conditional and partially mapped reads. Unknown future veto
categories also fail closed. Only the two already-qualified broad categories,
`unproved_data_endian_pool_value_veto_only` and
`unmodeled_or_unbounded_static_read_veto_only`, retain the inherited distinction:
they are not certificates of actual exact reads into every gap. This does not
turn broad uncertainty into a positive runtime-endian or indirect-read proof.

The guard is used by both public admission and successor validation. The new
validation regression supplies the actual conditional-read producer's output,
rebinds the complete synthetic baseline and exact receipt, and still requires
the bounded-overlap guard to refuse. It cannot pass merely because a stale
baseline hash or receipt was rejected earlier.

## Focused checks actually exercised

Exact argv, from this successor worktree:

```text
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -B -m unittest tests.test_catalogue_v13_r2 tests.test_catalogue_alignment tests.test_catalogue_protection tests.test_catalogue_local_entries tests.test_catalogue_entry_reassessment tests.test_catalogue_endian_cli tests.test_analyze.CatalogueClosureTests.test_frozen_baseline_and_parent_are_independently_authenticated
```

**Actual exit: 0; 45 tests; 4.779 s unittest time / 4.980619 s captured wall.**
The exact command, cwd, stdout/stderr and changed source hashes are retained in
`scratch/closure/v13-r2/focused-tests-r1.json`.

New regressions reproduce each reviewer counterexample through actual public
consumers, with positive controls:

* Absolute declared-absent external manifest, before/after live mutation: both
  attempts refuse; the frozen manifest is unchanged.
* Relative declared-absent explicit consumption: refuses even while absent.
* Materialized original/sentinel files, symlinks and dangling symlinks: loader,
  optional probe, resolver and actual protection consumer all refuse.
* Valid absence with no consumption: passes; optional presence is false.
* Registered absolute extra manifest: uses only retained content despite live
  mutation, preserving the protected identity set and consumed digest.
* Actual conditional halfword reads covering an external gap and an internal
  alignment interval: both public admissions refuse before mutation.
* Public validation with the actual conditional veto and a rebound baseline:
  refuses the exact bounded overlap.
* Nonoverlapping conditional reads and the explicitly qualified broad vetoes:
  retain the exact positive padding control.
* Partially mapped and future bounded veto categories: overlap refuses.

No full suite, build, compilation, matcher, whole-ledger audit, canonical edit,
acceptance, adoption or generation was performed for these focused checks.
Main retains the adoption-merge full-suite gate; the old v13 912-test failed
suite remains an immutable historical FAIL, not a result for this successor.

## One granted fresh generation and exhaustive metadata comparison

Main explicitly granted the exact request at source
`701ee0b16bc3d13f8a15d69e2aca6f9a7d6735b6`. Request SHA-256:
`02456ed3eca6d013d2d5ad94f18b73d093cf259da21ae9be49322585ac98555b`.
Exactly **one** public generator execution ran through
`scratch/closure/v13-r2/generate-r1.py`; both generator and runner actually
exited **0**. Captured generator time was **105.101197 s**, runner wall time
**105.32 s**. The runner atomically wrote `"released": true` with the actual
generator exit and output argv immediately after that execution, **before**
proposal hashing or comparison. No second generation or full suite was run.

Fresh proposal: `scratch/analyze/catalogue-v13-r2-r1.json`, **9,591,326 B**,
SHA-256:

`394e550406cc227550245bbbed378e7bba21f1de858fdd9310d9a737aba27b8a`

This differs from the sealed failed v13 proposal solely in revision/source/
input/review bindings: **zero complete function-record changes**, **zero
physical-range-record changes**, unchanged function/range order and unchanged
statistics, anomalies, entry identities, extents and code/pool/literal spans.
All 4,743 complete function records are identical to the sealed v13 proposal.
The exact comparison records **15 other metadata changes**, all enumerated
below. No changed catalogue record is omitted or summarized away.

| Changed metadata path | Exact change |
|---|---|
| `/generator/input_sha256/research/catalogue_v13_r2_evidence.json` | Added `2adf68b043881412092ce23c8c89ba43e64d47061e4f05ca100e45dfa15b86c0` |
| `/generator/input_sha256/research/catalogue_v13_r2_review.json` | Added `5b767baaf4965659e1e4c16e945ba3ead76e346420243348c8199d8d240a0b03` |
| `/generator/revision` | `catalogue-v13` → `catalogue-v13-r2` |
| `/generator/source_sha256/tools/analyze.py` | `490d6f5adfeee03bba171cd3b79b41a8ce8a2d281621026adab87380d9429a9a` → `39edac788a92878e5d91f69fcafbec1df13f7fb12dccbdb73942975f5caf52ad` |
| `/generator/source_sha256/tools/catalogue_alignment.py` | `8f94c2b75f3f21e0878b42c5918edf0a56a84ea5131a28b275ffc0c7fd082b07` → `766ec7da1e4a46df5db065db4207c0a5de898b3780c2d827d054ad39a33c9726` |
| `/generator/source_sha256/tools/catalogue_closure.py` | `f0da2c5b4248d09e5e27189ca44264ef546a51125b26c7f6ca62bcd30dcbc07b` → `0739bc685e3071a2249e6684e73bee84416b0a616b4659569baf3a4d2933a32d` |
| `/generator/source_sha256/tools/catalogue_frozen.py` | `0d4e6a6db8101463d3b1887b666224d9733fdb38a1e5f4713eefc3b231bfe189` → `44814fa56c0e90f6cc0e47286161347081c3addfa44cae4c6f3215a2f54ffcd0` |
| `/revision_review/fail_closed_revision_v13_r2` | Added exact `research/catalogue_v13_r2_evidence.json` repair record, including both finding IDs and frozen FAIL/seal bindings |
| `/revision_review/input_sha256/research/catalogue_v13_r2_evidence.json` | Added the same repair-evidence digest above |
| `/revision_review/input_sha256/research/catalogue_v13_r2_review.json` | Added the same reviewed-FAIL digest above |
| `/revision_review/input_sha256/tools/analyze.py` | Same old/new analyzer source digests above |
| `/revision_review/input_sha256/tools/catalogue_alignment.py` | Same old/new alignment source digests above |
| `/revision_review/input_sha256/tools/catalogue_closure.py` | Same old/new closure source digests above |
| `/revision_review/input_sha256/tools/catalogue_frozen.py` | Same old/new frozen-input source digests above |
| `/revision_review/name` | `catalogue-v13` → `catalogue-v13-r2` |

`research/catalogue_v13_r2_generation.json` commits the exhaustive comparison,
including full before/after metadata records and exact residual intervals.
`research/catalogue_v13_r2_checks.json` binds it, the grant/release and receipts.
The repair evidence was frozen before generation; its pre-generation caution
remains unchanged rather than editing a consumed generator input afterward.

Final counts remain **4,743 owners, 4,776 entries, 4,571 creditable ordinary
owners / 1,121,674 extent B**, **114 unknown B / 11 intervals**, and one anomaly,
`0010d2b8: unexplained_function_bytes`. The exact unknowns remain:
`00100020/4`, `0010012e/48`, `001005fa/16`, `00100942/2`, `00100b26/2`,
`00100d70/4`, `0010d3a8/8`, `001415b4/4`, `001a00b4/4`, `001c7748/20`,
`001cd2e6/2`. The independent review's earlier positive geometry observations
are not relabelled as a successor review or a passing old seal. Main retains
fresh independent review, adoption-merge full suite and every canonical
adoption/acceptance operation. No source/data/C credit or adoption follows here.
