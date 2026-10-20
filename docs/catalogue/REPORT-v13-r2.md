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

## One fresh generation requested, not yet executed

`scratch/closure/v13-r2/generation/HEAVY-SLOT-REQUEST.json` requests one fresh
public `tools/analyze.py catalogue` execution through
`scratch/closure/v13-r2/generate-r1.py`. It requires an explicit Main grant.
The runner atomically releases the heavy slot with every actual generator exit
before output hashing or metadata adjudication. It then compares against the
hash-bound failed v13 proposal and records every changed complete function,
physical-range record and other metadata field, including order/statistic/
anomaly changes. It never modifies canonical Main or either predecessor.

**No fresh proposal hash, unchanged-current-image claim or generation PASS is
asserted here before that grant and execution.** The new revision/source/input
bindings necessarily require their own generated proposal identity; whether
any catalogue records change is reserved for the actual generation comparison.
