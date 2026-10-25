# Ruling 2026-10-25: explicit typed dispatch tables are allowed

**Decision (user, 2026-10-25).** Original code that calls through a C++ vtable or a method/callback table may be written as an **explicit C dispatch table**: a struct of function pointers placed at the original table address. This applies when a genuine C++ class form (virtual destructor, an actually undefined key function, an evidenced `_ZTV` binding) cannot reproduce the original bytes. Calling through such a table is allowed.

**Context.** 87 functions (18,952 B in 10 families, mostly scene and menu classes) already match, but they were held under the previous rule, which allowed native vtables only in genuine C++ class form and allowed explicit address tables only when nothing called through them. The class form cannot reproduce these tables exactly; derived complete-object destructors come out 4 bytes short. The same pattern appears in accepted code as known issues CM2-F01/F03 (scratch/prereview/cmodels2/PREREVIEW.json). Evidence: scratch/integrator/contract-plan/FAMILIES.json (families SCENE-VIRTUAL, MENU-CONTROL, SCENE-PMF, SCENE-DISPATCH and others marked blocked_on_user_ruling).

**Conditions** (reviewers check each one):

- **One model per table.** A table is modelled exactly once, as the explicit typed struct. No genuine C++ class may describe the same table, no union or second view, and no table that is both opaque and typed.
- **Every slot has its exact type.** Each slot is the exact function type of the function the original table really holds at that position. Every provider is defined with that type, and every caller calls through it with matching arguments and results. No casts of function pointers, no `void *` slots that are invoked, and no type punning.
- **Evidence.** Table address, slot count and slot contents are evidenced from the original data (relocations or pointer cells) and from the writer that installs the table. Receiver types are evidenced by the providers' bodies and the stores that install the table. Labels without such evidence are `[INFERENCE]`.
- **Real receivers only.** An object whose table is installed at offset 0 is modelled with a table-pointer member of the explicit table type. No unrelated object is passed as a receiver, and no base subobject is invented.
- **Native C++ identity.** If the original evidence shows genuine C++ (mangled `_ZTV`, RTTI, compiler-generated destructors), a genuine class form remains preferred whenever it reproduces the bytes. An explicit table may stand in only where the class form provably cannot. That proof goes in the unit's known-issues entry.
- **Qualification.** Each credited function that defines, installs or calls through an explicit table carries it as a QUALIFIED row and appears in `docs/known-issues-<checkpoint>.md`.
- **Unchanged.** All other source rules stay in force: no inline assembly, byte blobs, steering, invented callees or unproven aliases.

**Scope.** The ruling applies to new candidates and to remediation of accepted code: the CM2-F01/F03 known issues may be rewritten to this form through the normal revised-unit review.
