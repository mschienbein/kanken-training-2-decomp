# H11 agent pilot (2026-10-16)

Ten functions, one per triage feature class (`tools/triage.py stratified --per-class 1 --seed 16`),
split across two author agents (OpenAI `openai-codex/gpt-6.1-sol`), 25-attempt budget per function.
Candidate results; the four matches are re-derived by the integrator and go into a checkpoint.

| Address | Size | ISA | Class | Result | Attempts | Minutes | Note |
|---|---:|---|---|---|---:|---:|---|
| 0x1670e4 | 8 | ARM | leaf-small | MATCH | 1 | 0.3 | four VFP float stores |
| 0x18fdd4 | 40 | ARM | indirect-call | MATCH | 5 | 1.3 | C++ vtable call, shared epilogue |
| 0x138334 | 60 | ARM | svc | mismatch | 8 | 4.5 | CTR-SDK IPC wrapper: needs `__svc` + CP15 TLS (policy pending) |
| 0x11e928 | 72 | ARM | calls | MATCH | 1 | 0.3 | optional callback + wrap counter |
| 0x195d94 | 200 | ARM | loop | mismatch | 25 | 5.3 | exclusive load/store state machine (`ldrex`/`strex`), likely SDK synchronisation primitive |
| 0x100bb0 | 208 | Thumb | thumb | mismatch | 2 | 5.1 | ARM C runtime float conversion (position-independent tables): library classification |
| 0x195254 | 224 | ARM | pool-data | MATCH | 1 | 0.8 | command enqueue with critical section |
| 0x1932e8 | 284 | ARM | switch | mismatch | 25 | 8.9 | catalogue cuts the in-section format strings (real section 300 B); two register saves swapped |
| 0x170c20 | 304 | ARM | vfp | mismatch | 25 | 12.2 | one instruction (`add #0` vs `mov`) away |
| 0x106afc | 776 | ARM | leaf-medium | mismatch | 26 | 9.6 | inlined `svc 0x28` (GetSystemTick): policy pending |

Totals: **4/10 MATCH, 344 of 2,176 bytes**, 119 compiles, 48 agent-minutes.

## Lessons for wave planning

- Ordinary game C matched fast (1–5 attempts, under 2 minutes each). The misses cluster in four
  classes that are not "harder C":
  1. **Policy:** SVC and CP15 access (`__svc`, named-register variables). Needs a user ruling.
  2. **Library classification:** ARM C runtime (Thumb, position-independent) and SDK primitives
     (`ldrex`/`strex` loops) should be classified before authoring, not authored.
  3. **Catalogue:** in-section string literals, NOP entries, multi-entry routines, missing
     tail-branch targets (catalogue v3).
  4. **Near misses:** register-allocation/scheduling differences after budget; worth a second
     pass with a stronger model or a permuter later.
- Triage should down-rank `svc`, Thumb-runtime and exclusive-access functions until (1) and (2)
  are settled.
