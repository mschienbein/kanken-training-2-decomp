# Known issues — checkpoint c005

37 new functions / 1,852 B in 31 wave-5 units by OpenAI-family authors
(`openai-codex/gpt-6.1-sol`; `author` per unit in `config/candidates/c005-wave5.json`),
plus six interface revisions relative to the c004-r2 prepared ledger. Denominator v6;
gates v1.8; linker model `armlink5-inline-branchnop-v2` (`docs/LINKER.md`).
This is checkpoint content, not an acceptance or a completed independent review.

Review is single-family under
[`rulings/2026-10-17-reviewer-model-family.md`](rulings/2026-10-17-reviewer-model-family.md):
two separate, fresh-context OpenAI-family reviewers must re-derive and seal their records.
The integration pre-review does not replace those reviews.

## Linker model qualifications

- None of the 37 new functions requires an emulated linker rewrite.
- Revised `func_0011ef54` retains its c004-r2 `branchnop` rewrite: the tail target is the
  adjacent `func_0011ef58`. The shared prototype forwards a signed 64-bit nanosecond
  duration in r0/r1; its caller/callee evidence and `[INFERENCE]` comment are retained.
- The model still qualifies inherited material: an inlined return-only call proves an
  eligible callee body, not its identity; a branch-to-next rewrite proves the final
  adjacent target, not whether source order or linker reordering placed it there.
  Conditional single-instruction inlining remains manual-backed, not newly established
  by this checkpoint. See `docs/LINKER.md` and the eventual receipt's `linker_rewrites`.

## Revisions to accepted material (`revised_units` / `revised_headers`)

The baseline for integration is `accept/c004-r2/matches.json`. Four currently accepted
wave-1 units change, and two c004-r2 units will be revisions relative to c004-r2 after
that checkpoint finalizes:

- `src/wave1/func_00134f04.c`: reset the leading `NotificationState.status` halfword
  through new `game/notification.h`, rather than declaring the whole object a scalar.
- `src/wave1/func_0018fcb4.c`: return the distinct opaque `ServiceContext` at
  `data_002430e0`, not the flag-block type at `data_00242a50`.
- `src/wave1/func_00195e84.c`, `src/wave1/func_00195e94.c`: read the shared
  `ServiceState` signed flag at 0x09 and unsigned mode byte at 0x06.
- `src/wave4/func_0011ef54.c`: obtain both sleep prototypes from `game/runtime.h`.
- `src/wave4/func_00136448.c`: obtain the two-word `WordPair` layout and setter
  prototype from `game/archive.h`.

Existing headers `game/archive.h` and `game/runtime.h` are revised; new headers
`game/service_state.h` and `game/notification.h` replace local, conflicting declarations.
The self-relative reference resolver and its callers now share the archive header.

The pre-review also corrects candidate-only interfaces and partial layouts:
`func_0018fa44` reads a const service context and writes its memory output;
`func_0012fab0` reads a const handle object; `func_00186294` receives a writable
0x80-byte error record, including its handle field at 0x0c. The return-only
`func_001c0e44` is an array-element callback with a writable element pointer, not a
no-argument function; its unused return type remains explicitly `[INFERENCE]`.
The C++ animation updater has a writable object and an integer-returning virtual
advance callback taking the object and integer mode; unobserved virtual slots remain
padding rather than invented destructor declarations.

The request record keeps the evidenced 0xf80-byte extent with padding for unaccessed
synchronization storage, avoiding an unchecked packing pragma. The notification
payload uses a literal word count of the same size, avoiding an unsupported parser
expression. Both C units are now checked by the interface tool rather than waived.

## Preserved original bugs (new in this checkpoint)

- `src/wave5/func_001533b8.c`: unsupported relative-reference types resolve to null,
  but the original still copies 0x4c bytes from that source.
- `src/wave5/func_001f28cc.c`: the same failed resolution is followed by field reads
  through the null source, rather than an error return.

Both carry `ORIGINAL BUG:` comments. No null guard or substitute output was introduced.

## Not credited

- No unit was removed from the candidate list: all 31 units / 37 functions re-derived
  MATCH at their destinations under catalogue v6, before and after integration.
- Other wave-5 attempts remain outside the candidate manifest and receive no credit.
  SVC/CP15 intrinsics, exclusive-access primitives and compiler helpers are not admitted
  by this integration pass.
- No full build, suite, prepare, independent review, finalize, canonical-ledger change
  or publication was performed here. The dry ledger is private integration evidence.

## Inherited

Partial layouts and inferred source-level contracts; preserved original bugs from prior
checkpoints; no runtime execution in this preparation pass. The interface tool checks C,
C++ ODR/header-name/local-redeclaration policy and limited const use, not all aliasing or
callee writes. Fourteen C++ units in the dry ledger remain mechanically unchecked under
`--allow-unchecked-cpp`; the new animation unit was checked by hand against its callers
and virtual-call use. The other thirteen are inherited c004-r2 material.

## Preparation evidence

The private dry ledger combines 274 c004-r2 units and 31 c005 units: 305 units /
336 functions. Initial interface checking found three disagreements, five policy items
and two unchecked C constructs. All were resolved. Final checking reports 291 checked C
units, fourteen permitted C++ units, zero disagreements and zero policy items.

Final sequential destination checks report MATCH for all 62 selected units / 68 functions:
every c005 unit, all six revised baseline units and the other shared-header dependents.
These are per-unit results, not a substitute for the two fresh full builds and independent
reviews required by checkpoint acceptance.
