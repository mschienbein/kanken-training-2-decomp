# Ruling 2026-10-25 (later): the Thumb runtime functions stay documented residue

**Decision (user, 2026-10-25).** The 9 Thumb functions that the ARM Compiler 4.1 libraries do not prove (362 B) stay in the residue table as **documented residue**. They are not authored as C, not classified as library code, and no Thumb compiler profile is frozen for them. This settles the "revisited with the user" step of `2026-10-25-unresolved-bytes-and-thumb.md`.

**Context.** A strict adjudication against the 4.1 b791 and b894 libraries (private: `scratch/integrator/library-sweep-41/REPORT-41.md`) gave:

| Function | Size | Best lead | Why it is not classified |
|---|---:|---|---|
| func_0010011c | 18 | `__ARM_exceptions_buffer_init` | the whole 66 B section reconstructs, but no independent name anchor proves the identity |
| func_00100360 | 60 | `snprintf` | the section re-encodes, but six library spellings are ambiguous and the caller receipt object is missing |
| func_00100914 | 22 | `_printf_cs_common` | 4.1-exact except one weak-undefined call slot that the linker turned into a NOP |
| func_00100af8 | 22 | `_printf_lcs_common` | 4.1-exact except one weak-undefined call slot that the linker turned into a NOP |
| func_0010080a | 36 | `mbrlen` | a single-slot near-identity only against the older 5.04-b82 libraries |
| func_0018fd68 | 80 | `mbstowcs` | a single-slot near-identity only against 5.04-b82 |
| func_0019fe04 | 116 | `mbrtowc` | a single-slot near-identity only against 5.04-b82 |
| func_0019f9a4 | 6 | shared-entry bridge | no library family |
| func_00100c9c | 2 | generic return | identical to thousands of definitions; no identity anchor |

**Options declined.**
- Accepting armlink's documented handling of unresolved weak calls (they become NOPs) as the native-emitter certificate. This was linker model v19, branch `linker-thumb`, with a GATES v1.21 proposal.
- Supplying an armlink 4.1 licence, so the NOP encoding could be observed directly. The local armlink 4.1 b791 fails with L9932E without one.

Gates stay at v1.20. The v19 proposal is not adopted; its branch is kept as evidence.

**Conditions.** Each function is listed in STATUS's residue section with its address, size, best library lead and evidence path. New evidence, such as a licensed armlink observation, reopens a function through a reviewed gate or catalogue revision.
