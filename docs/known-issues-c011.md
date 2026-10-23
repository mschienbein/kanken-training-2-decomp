# c011-r5 candidate — known issues and qualifications

This is an **unaccepted candidate** rebased from c011-r4 `fdea2b39` onto main
`7083a0a3`: finalized **c009-r6** and **PREPARED c010-r6** (source `3caf7c4d`).
The c010-r6 reviews/finalization must finish before Main prepares c011. The
adopted denominator is **v13-r2**; frozen gates remain **v1.19**. No historical
acceptance record, canonical ledger or predecessor worktree is rewritten.

## Current source scope

- **76 candidate units / 93 claims:** **84 NEW functions / 11,464 B**, plus
  **9 explicit stable-identity moves / 872 B**. Every claimed identity remains
  an ordinary creditable function under v13-r2.
- The proposed ledger preserves all **2,693 prepared predecessor identities /
  289,852 B** and contains **2,701 units / 2,777 functions / 301,316 B**,
  including all **24 inherited helper-only units**. These are proposed numbers,
  not accepted credit.
- All **15 c011-r4 NEW-only deferrals / 2,532 B** survive unchanged. Four further
  NEW-only deferrals / **780 B** were explicitly approved by Main. Together with
  the 84 older holds, c011 has **103 deferrals / 20,540 B**. None is credited.
- Four c009-owned font-wrapper holds (`00131540`, `001f4920`, `001efcec`,
  `001f4994`; 144 B), its DisplayTarget/queue holds and all **83 c010-r6 holds**
  remain their predecessors' responsibility and are not charged to c011.

Evidence entry point: `scratch/integrator/c011/r5/HANDOFF.json`. The proposed
ledger, source/header bindings, explicit move owners, failed attempts and full
final match/rewrite scope are sealed beside it.

## Additional r5 deferrals

| NEW identity | Bytes | Reason |
| --- | ---: | --- |
| `00155a44` | 112 | Its byte publication overlaps the prepared `AudioDspParameterBlock.valuea0` halfword protocol. The old ushort/byte union would be punning; changing the witnessed access width is not a fix. The union is withdrawn. |
| `00170e58` | 200 | Display labeling assumes a `DisplayObject *` where prepared `WidgetSlot.object` is `ResourceEvent *`; no producer/tag establishes the asserted owner. |
| `001af258` | 172 | The four-slot activation path has the same unproved `ResourceEvent *` to `DisplayObject *` relationship. |
| `001b5b60` | 296 | Bound-display updating additionally casts that receiver to `BoundDisplayObject *`, without a real type witness. |

Main approved these holds rather than casts or restoring the older slot type.
The rejected sources are preserved under the r5 `deferred/` directory; the
original r4 transaction remains in history. **Prepared `00155ab4` stays in
`src/checkpoint10/func_00155ab4.c`** using its canonical `count` field, and its
unnecessary proposed move is cancelled—not its identity or credit.

## Accepted/prepared contract preservation

- The nine remaining moves explicitly name their c010-r6 prepared owners. Four
  emptied old paths are absent; every destination lists its complete claimed
  identity set. No accepted/prepared identity is dropped, renamed or re-awarded.
- FontVTable/data_00223d98 remain opaque; the c009-r6 W20 getters and single
  `float(float)` converter protocol survive. Stale native glyph completions and
  duplicate converter declarations are not reintroduced.
- The DSP mask helper uses the real halfword `valuea0`, without a byte overlay.
- `TaskRuntime` keeps the prepared sound/heap and suffix fields at their existing
  locations. Four independently witnessed playback records refine only the
  previously reserved preceding region.
- `W6ListItem` retains the prepared `id/state/frame/saved_frame` members. The new
  setter recovers the previously reserved label and active fields and uses the
  existing saved-frame/state members, rather than a second record/union. Both
  the protected cleanup and new setter MATCH in that shared declaration.
- Two unused, unclaimed context definitions (`001dc400`, `0015ad74`) and the
  latter's unused direction helper are removed from the new translation units.
  They earned no credit, had mismatching extra emissions, and are not fabricated
  helpers or alternate implementations of those identities.
- An emitted-undefined-symbol audit of every remaining candidate object, resolving
  both C names and the current native-name overlay, finds **no reference to any
  of the 83 deferred c010 identities**. This is not proof of unknown dynamic
  populations or a replacement for manual callable-table review.

## Exercised exactness and continuing qualifications

The final full proposed-primary sweep ran **2,677 sequential, niced,
single-process `tools/match.py --show 0` commands**, all **exit 0**, with frozen
per-unit profiles and explicit **armlink5-inline-branchnop-v6 /
anonymous-string-records-v1**. All **2,777 required functions MATCH** and all
24 helper-only definitions are supplied in natural contexts. The complete,
source-bound linker-rewrite projection has `scope_complete=true`.

Earlier failures are retained: 15 compile refusals and two extra-context
mismatches in the first candidate pass, plus one prepared-list member-name
compile refusal in the first full pass. Two focused repaired-list proofs passed.
Across all attempts, **5,435 matcher commands** returned **5,417 exit 0,
2 exit 1, and 16 exit 2**. No unchanged failed input was retried to claim success.

R1 removed-helper provenance remains `[INFERENCE]` and requires QUALIFIED review
rows, including every affected inherited helper/header caller. L1 establishes
only its witnessed literal span—not historical TU/pool ownership, complete
original pool extent, ordering or data credit. Native same-entry aliases,
selective-link lifetime/key-function assumptions, preserved `ORIGINAL BUG:`
behaviors, ARMCC TLS/SVC/intrinsic uses and C-only MUST-discard callback
qualifications remain. No shim, incompatible callback cast, new punning overlay,
asm or code-generation steering was introduced.

No interfaces/argcheck gate, full build, suite, prepare, review registration,
finalize or push was run by this author. Main must finalize c010-r6, integrate
this complete committed transaction, run and adjudicate the serialized
whole-ledger gates (including unchecked C++/callable contracts and unique emitted
ownership), then prepare with two fresh builds and the suite, followed by two
independent reviews. Exact argv is in
`scratch/integrator/c011/r5/REMAINING-HEAVY-COMMANDS.json`. Any source/header/
profile/name/catalogue/composition drift requires fresh affected-closure proof
under unused output names.
