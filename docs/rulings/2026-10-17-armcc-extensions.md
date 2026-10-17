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

## Implementation

`tools/interfaces.py` accepts CP15 named-register variables at file/block scope with `register`
storage, retains syscall and calling-convention annotations, and still refuses assembly bodies,
assembly functions and core-register bindings. Unit-source `INTEGRATOR:` marker comments fail
the audit before preprocessing. The pinned compiler's standard C headers are supported without
discarding ABI attributes: `__int64` is normalized to `long long`, `__softfp` retains the AAPCS
contract, and non-ABI nothrow/format annotations and anonymous-union permission pragmas are
recognized. Packing and unknown pragmas/attributes still fail closed.

Shared C/C++-safe declarations live in `include/ctr/svc.h` and `include/ctr/cp15.h`; no extra
include path is required. The syscall header describes the **raw kernel register ABI**, not
userland pointer-output wrappers. ARMCC limits `__svc` arguments/results to r0–r3 and rejects
64-bit arguments, so representable 64-bit inputs are split into low/high words and multiword
outputs use `__value_in_regs`. ControlMemory, QueryMemory, CreateThread, ArbitrateAddress and
WaitSynchronizationN need r4/r5 and intentionally have no incorrect declaration. See the
[public kernel ABI](https://www.3dbrew.org/wiki/Kernel_ABI) and
[public wrapper marshaling](https://github.com/devkitPro/libctru/blob/master/libctru/source/svc.s).
Their full-ABI compile errors and affected image functions are recorded in the private
`scratch/integrator/armcc-ext/REPORT.md`; any classification remains an independent review.

For the ARMv6 MPCore profile, the pinned compiler does not recognize `__dmb`/`__dsb` as native
intrinsics. The CP15 header therefore expresses these barriers through named coprocessor
register writes, with `__schedule_barrier` preventing compiler motion around the CPU barrier.
This uses the already-approved named-register and intrinsic forms, not inline assembly.

