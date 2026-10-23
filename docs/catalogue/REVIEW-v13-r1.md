# Catalogue-v13 independent review decision

**FAIL** — source `0c3fd934009d50c18306d54df82abd5ac0da6d94`, proposal `e333779b6f2272c70cc166f915aa925abbcb06641a11f62f4443368ab847e743`.

The supplied catalogue accounting, certificates and reproducibility checks pass, but two new public fail-closed paths accept forbidden input/reference consumption. Both must be fixed and independently re-reviewed before this frozen source/proposal pair is eligible for adoption.

## Blocking review findings

- **V13-CODEX-F01 / P2**: Reject materialized paths declared absent before resolving them. Evidence: frozen_absence_probe.py, frozen-absence-probe.json, frozen_absolute_probe_r2.py, frozen-absolute-probe-r2.json.
- **V13-CODEX-F02 / P2**: Apply exact bounded read vetoes before admitting padding. Evidence: padding_veto_probe.py, padding-veto-probe.json.

The original-image certificates/accounting are not the cause of this FAIL. The actual manifest has 13 intact retained copies and no absent paths; all 22 actual padding intervals avoid live code and exact bounded read-veto records. Both defects are nevertheless exercised violations of explicit required public fail-closed behavior.

Fresh generation exit: 0; equal supplied digest: True. Bounded regressions: 30, exit 0; no reviewer full suite or build.

See REPORT.md for complete accounting, certificate coverage, exact residue, qualifications and limits; REVIEW.json contains exact findings and machine-readable checks. All earlier sealed failures remain immutable. Main alone owns repair integration, the adoption-merge full suite and any later denominator adoption.
