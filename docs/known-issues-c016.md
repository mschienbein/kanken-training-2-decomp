# Known issues — c016 candidate

This is an intake record, not checkpoint acceptance. Main owns preparation,
independent review and finalization.

## Accepted matrix-command bodies: mechanical revisions deferred

- `func_00114778` (`src/wave5/func_00114778.c`) retains its pre-existing
  `Matrix44Command` union views of a matrix and command words.
- `func_001147c0` (`src/wave5/func_001147c0.c`) retains its pre-existing
  `Matrix34Command` and `ModelMatrixPacket` union views.

W1 proposed only replacing each file's identical local byte-cursor declaration
with `game/command_buffer.h`. Intake pre-review found that this would reopen the
unchanged union-punning bodies under the standing no-punning rule. Main explicitly
deferred both mechanical revisions and required both accepted source files to
remain exactly at `d93aa6c7`. Neither is a new c016 claim or a revised source in
the c016 manifest; existing acceptance credit is not withdrawn or re-awarded.

A future change requires a separately scoped, complete matrix/command owner
repair. c016 neither implements that repair nor introduces a new owner view,
representation conversion or exception to the source policy. New c016 functions
use their canonical shared/native interfaces and do not depend on the local
cursor declarations in these two files.

Private finding/disposition evidence is under `scratch/integrator/c016/`.
The public disposition is also recorded in
`config/candidates/c016.json` → `deferred_mechanical_revisions`.
