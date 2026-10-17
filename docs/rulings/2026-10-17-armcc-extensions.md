# Ruling 2026-10-17 — documented ARMCC C extensions count as C

**Decision (user, 2026-10-17):** the following documented ARMCC C/C++ language extensions count as
C for credit, like any other C construct:

1. `__svc(N)` function declarations (calls compile to `svc N`), e.g. `__svc(0x28) unsigned long long
   svcGetSystemTick(void);`.
2. Named register variables bound to coprocessor registers, e.g. `register unsigned int tls
   __asm("cp15:0:c13:c0:3");` (CP15 thread-local-storage reads).
3. Compiler intrinsics, e.g. `__dmb`, `__dsb`, `__isb`, `__ldrex`, `__strex`, `__clrex`,
   `__schedule_barrier`.

**Context:** 220 functions (37,256 B) need `__svc` or CP15 named registers, and 8 more (2,220 B)
only issue barriers (evidence: scratch/integrator/residue/REPORT.md, private). 87 of them are
ordinary game functions that contain one system call, such as reading the system tick. Plain C
cannot express these instructions. CTR-SDK's own headers use these extensions to give C code
access to system calls.

**Conditions:**

- Still refused: inline `__asm` statements or blocks, embedded-assembler functions (`__asm void
  f(void) { ... }`), byte blobs and every other form of assembly standing in for C. Handwritten
  assembly is classified as non-C with evidence and never credited.
- Each `__svc` / named-register declaration lives once in a shared header under include/ (one
  declaration per SVC number or register), never as per-unit copies.
- Every credited function that uses an extension carries it as a qualification in its review row
  and in `docs/known-issues-<checkpoint>.md`.
- This ruling does not decide ownership. A function that turns out to be SDK or library code (for
  example the 4-byte kernel-ABI stubs, which C `__svc` probes do not reproduce) is classified with
  evidence, not credited.

**Effect:** replaces the open question "may `__svc(N)` and named-register CP15 variables count as
C?" raised since 2026-10-16. `docs/REVIEWING.md` step 2 and the source policy in `docs/GATES.md`
v1.9 follow this ruling.
