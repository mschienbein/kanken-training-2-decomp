# Ruling 2026-10-25 (late): held-contract functions are documented residue

**Decision (user, 2026-10-25).** Some creditable functions match byte-exactly but stay held because their source contracts cannot be reconciled with the accepted code under the standing rules. These are the residue classes `held_contract` and `header_blocked`. They are **documented residue**.

No rule relaxation is pursued. The user was offered, and declined:
- casts between proven-equivalent callback types;
- waiving the no-punning rule where it is the only blocker;
- a VFE / null-vtable-cell linker model (armlink virtual-function elimination).

Each function is listed in STATUS's residue section with its primary blocker and evidence pointer. The machine-readable companion `2026-10-25-held-contract-residue.json` names the exact functions, taken from the final residue table after the last checkpoint finalizes.

**Context.** Reconciliation was attempted on 2026-10-25:
- checkpoint c015;
- the small-component families;
- three contract waves (W1 and W3 landed; W2's gate failed);
- surveys of native classes and of the c012 holds;
- the scene/menu and font native-class migrations (no new credit: unrecoverable key-function owners and unprovable null vtable cells).

The decision basis, from scratch/integrator/held-residue-summary/HELD-SUMMARY.md (private) at residue draft r15, 458 functions / 103,548 B:

| Primary blocker | Functions | Bytes | Could a ruling unlock it? |
|---|---:|---:|---|
| Unproved ownership, identity or provider model | 323 | 80,560 | no; evidence missing |
| ABI or scalar-contract conflicts | 31 | 6,032 | no; evidence missing |
| Union, byte-offset or storage punning | 32 | 5,520 | a punning waiver (declined) |
| Honest canonical source still mismatches | 29 | 5,256 | no; needs a lawful matching source |
| Callback/provider signature conflicts | 17 | 2,260 | proven-equivalent casts (declined) |
| Vtable emission (live D0 / null D1) | 13 | 1,048 | VFE/null-cell model (declined) |
| Canonical-header compile blocks | 3 | 960 | no |
| Companion/whole-TU mismatches | 3 | 632 | whole-TU waiver (not offered separately) |
| Unrecovered lifetime/key-function owner | 3 | 576 | no; evidence missing |
| Non-natural code-generation work | 2 | 372 | steering waiver (not offered separately) |
| Erased call-site identity (inline-NOP) | 2 | 332 | erased-call inference (not offered separately) |

**Conditions.** The ruling credits nothing and changes no gate. New evidence reopens a function through an ordinary checkpoint: an owner identity, an ABI proof, a recovered key-function TU, or a lawful matching source.
