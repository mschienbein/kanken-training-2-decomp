# Reviewing a checkpoint

You are an independent reviewer: not the integrator, not the author of the functions you review.
Re-derive; never trust `prepare.json`, author notes or prior reviews. Seal your verdict before
reading any other review of the same checkpoint.

## Inputs

- `accept/<cp>/prepare.json` (what was prepared), `accept/<cp>/matches.json` (the ledger),
  `accept/<cp>/interfaces.json` (mechanical interface results), `accept/<cp>/receipts/*.json`.
- The new units: `prepare.json` → `new_units`; their functions are listed in the ledger.
- Rules: `AGENTS.md` (source rules), `docs/ACCEPTANCE.md`, `docs/GATES.md`.

## What to check per function

1. **Exactness:** run `tools/match.py <source> --profile <unit profile> --out scratch/review/<cp>/<you>/...`
   yourself; every credited function must report `MATCH`.
2. **Source policy:** no inline/embedded assembly (`__asm` statements or blocks, `__asm` functions),
   no byte arrays or casts of data standing in for code, no dead code added only to steer codegen
   unless declared as a qualification, `ORIGINAL BUG:` comments where original misbehaviour is
   preserved. Documented ARMCC extensions count as C (`docs/rulings/2026-10-17-armcc-extensions.md`):
   `__svc(N)` declarations, CP15 named-register variables (`__asm("cp15:...")` bindings) and
   intrinsics (`__dmb`, `__ldrex`, …), each declared once in a shared header and listed as a
   qualification on the function's row.
3. **Plausibility:** the C reads as something a programmer wrote; types are consistent with use
   (field widths, signedness, pointer depth); magic offsets in padded structs are acceptable but
   must agree with every use in the unit.
4. **Interfaces:** for units listed as `unchecked` in `interfaces.json` (C++), check declarations
   against other units by hand.
5. **Credit scope:** only functions listed for the unit are credited; nothing credits a veneer or
   another classified kind.
6. **Linker rewrites:** `prepare.json` `linker_rewrites` lists every emulated armlink rewrite
   (`docs/LINKER.md`, model in `linker_model`) inside the functions you review. A call inlined to
   `mov r0, r0` proves only that the original callee was a return-only function: the source's
   choice of callee is unverifiable from bytes and must be plausible and marked `[INFERENCE]` in a
   comment when it is not otherwise evidenced (e.g. by a pointer to the same function).

## Record

Write a JSON record and submit it with `tools/accept.py review --checkpoint <cp> --record <file>`:

```json
{
  "reviewer": "your-agent-name",
  "family": "openai | anthropic | ...",
  "verdict": "PASS | PASS_WITH_QUALIFICATIONS | FAIL",
  "functions": [
    {"function": "func_00103798", "source": "src/pilot/pilot_b.c", "verdict": "PASS | QUALIFIED | FAIL",
     "method": "match.py re-run: MATCH; read source; checked struct use", "finding_ids": []}
  ],
  "findings": [
    {"id": "F01", "severity": "blocker | major | minor | note", "subject": "src/... or func_...", "summary": "..."}
  ],
  "qualifications": ["..."]
}
```

Every new function needs exactly one row. A FAIL row or a blocker finding makes the overall
verdict FAIL; QUALIFIED rows or major findings need PASS_WITH_QUALIFICATIONS (with stated
qualifications) or FAIL. `accept.py review` rejects inconsistent records.
