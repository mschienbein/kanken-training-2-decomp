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

## Uncredited holds and failed canonical transactions

These ten incoming functions (1,924 B) are not c016 claims. Their full
dispositions remain in `config/candidates/c016.json` → `holds`.

| Function(s) | Reason |
|---|---|
| `0014381c` | Complete native sound-manager lifetime/table/provider closure is unproved; a deleting-destructor transaction is outside this checkpoint. |
| `001d3f58` | Native stream lifetime/key and all installed capability/provider slots are not reconciled to one owner. |
| `001cbf34`, `001ce9e8` | Binary-validator contracts and native Pane/list insertion require a complete cutover without runtime byte-offset node recovery. |
| `001266e4`, `0012672c` | Both filters match in isolation, but the real co-emitted translator does not; no narrowed passing subset is imported. |
| `0017418c` | Notification storage retains incompatible word/text/field/selection union views. |
| `001b8238` | Training control storage overlays unrelated list and animation owners. |
| `001c5a08` | The honest full-word input contract remains a code-generation mismatch; no narrowed formal parameter or steering is admitted. |
| `0012f5a0` | The canonical scalar relay transaction and its accepted prerequisite repair do not both match; the accepted relay/header remain unchanged. |

## Qualifications retained by the candidate

- **[INFERENCE]** The recovered cache-line alignment and descriptive native method
  identities do not recover the historical declaration spelling or translation
  unit boundaries. SDK/middleware alignment instantiations are compile/link
  witnesses, not additional match credit.
- New native renderer calls use canonical owner/resource types. Material
  activation retains the exact native `IMaterialActivator::Activate` interface;
  framebuffer array construction retains the exact `void *(*)(void *)` runtime
  provider type.
- The established `matrix34Identity` guard-release callee identity remains
  qualified where armlink erased its call. Existing removed-helper identities
  retain their accepted inference qualifications and are re-proved with their
  complete source atoms.
- The request protocol preserves the complete event result, both aligned timeout
  words and genuinely optional outputs. Explicit byte stores are not Boolean
  normalization. Descriptive duration/payload names remain **[INFERENCE]**.
- Library `func_0019e08c` (`_ZdaPv`), c015 claims and all scene-native/scene-font
  reservations are excluded. No gate-failed W2 transaction is imported.

## Finalized-main boundary and residue ruling

c016 is composed on finalized c015 main `45d62e6d` (history sequence 14).
The accepted native caller move preserves `func_001306c8`'s existing 240 B;
it does not count as new credit. fx04 and the nc01–nc06 revisits supplied no
additional new matches.

The [held-contract residue ruling](rulings/2026-10-25-held-contract-residue.md)
keeps every held-contract function as documented residue with its exact blocker.
c016 introduces no callback-cast, punning or VFE exception and undertakes no
further reconciliation of the held functions.
