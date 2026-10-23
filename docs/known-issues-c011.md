# c011-r6 candidate — known issues and qualifications

This is an **unaccepted candidate** rebased from c011-r5 `5172bded` onto
**c010-r7 `819edc6f`**, with finalized c009-r6 and denominator **v13-r2**.
The independently sealed c011-r5 pre-review FAIL and failed c010-r6 acceptance
reviews remain immutable. Main must finalize the applicable c010-r7 source and
recheck the exact composition before preparing c011. Gates remain **v1.19**;
selection remains explicit **armlink5-inline-branchnop-v6 /
anonymous-string-records-v1**.

## Current source scope

- **72 candidate units / 89 claims:** **80 NEW functions / 10,564 B**, plus
  **9 explicit stable-identity moves / 872 B**. Every claim is an ordinary
  creditable function under the frozen v13-r2 catalogue.
- All **2,687 retained c010-r7 predecessor identities / 289,152 B** survive.
  The proposed ledger contains **2,691 units / 2,767 functions / 299,716 B**,
  including all **24 inherited helper-only units**. These are proposed numbers,
  not accepted credit. The six NEW c010-r7 predecessor deferrals are not
  resurrected to preserve an obsolete failed-r6 count.
- All **103 prior c011 holds / 20,540 B** remain; four further NEW-only holds /
  **900 B** make **107 c011 holds / 21,440 B**. None is credited. The **28 c009
  holds / 3,588 B** and **89 c010-r7 holds / 7,832 B** remain predecessor-owned.
- The original fifteen r4 finding dispositions and four further r5 holds remain
  unchanged. Prepared `00155ab4` stays at its original c010 owner; its cancelled
  move stays cancelled. Every remaining move names the actual c010-r7 owner;
  all four emptied old paths are absent.

Evidence entry point: `scratch/integrator/c011/r6/HANDOFF.json`. The proposed
ledger, current deferrals, per-finding dispositions, source/header bindings,
complete exactness scope and rewrite projection are sealed beside it.

## Sealed c011-r5 findings

| Finding | Current disposition |
| --- | --- |
| **C011R5-F01 — loader virtual lookup** | NEW `00185934` is held; its exclusive c011 native-name binding is removed. The concrete secondary-base/thunk route reaches accepted C `001f27c0`, whose receiver, signed key and mutable result are not the native `PlayerHeapDataManager` contract. No accepted provider is aliased, cast or withdrawn. |
| **C011R5-F02 — held scalar texture dependency** | NEW `00137154` is held with contract-held `00182d9c`; its now-unused upload facade is quarantined. Excluding only the endpoint would not isolate the unresolved texture parameter/binding/catalog contract. |

Deferral byte counts use the frozen catalogue: `00185934` is **312 B** and
`00137154` is **408 B**. The pre-review's 308-B prose figure is not substituted
for the denominator's extent. The sealed review is not edited or relabeled PASS.

## Same-class c010-r7 derived-event sweep

Main explicitly approved both dispositions:

- **NEW `001911f0` / 92 B:** held because `ChannelCommand` writes a channel byte
  through `reinterpret_cast<unsigned char *>(&StreamEvent::object)`. This is
  prohibited byte-storage access. Neither a variant overlay nor an accepted
  event-hierarchy rewrite is imported to conceal the issue.
- **NEW `001d2f90` / 88 B:** an honest repair placement-constructs the actual
  `SequenceAuxiliaryEvent` directly in correctly sized arena storage, following
  c010-r7's genuine decoder-event construction. It **mismatches**, exit **1**:
  candidate **92 B** versus catalogue **88 B**, first difference at offset
  `+0x25`. The repaired attempt is held; no base-to-derived lifetime assertion,
  code-generation steering or unchanged-input retry is retained.

The rejected sources are preserved under `r6/deferred/`. Their unused endpoint
prototypes and the loader's exclusive binding are removed from the active tree.
The earlier matching but invalid source remains in immutable branch history.

## Predecessor and dependency preservation

- The generic invoking array callbacks/runtime retain **`void *(void *)`**;
  the canonical `const SoundEnvelope data_0024380a` and its real initializer
  binding survive. No obsolete initializer or BasicSound setter binding is
  restored by the names-overlay merge.
- C010-r7's genuine `ResTextureAnim(member.ptr())` value and placement-constructed
  `AudioDecodeEvent` remain unchanged. Its six further NEW holds (`001913fc`,
  `001d296c`, `001d2b8c`, `00140ecc`, `001d7e6c`, `001d9ca0`) remain unselected.
- The audio header merge retains only the two missing declarations matching the
  real accepted `AudioVoice *` providers `001d9178` and `001d9530`; the removed
  `SoundOperation` declaration is not revived. The initial full sweep's compile
  refusal exposed this mechanical omission; the final sweep includes the fix.
- Every remaining candidate object was inspected for undefined native/C symbols
  and emitted held identities. Resolving the current native overlay finds **no
  reference to any of the 224 combined manifest holds**, and **no reference to
  the six changed c010-r7 retained functions or `data_0024380a`**. All c010-r7
  bindings survive without rebinding. The audit includes c011-owned holds, not
  merely the c010 subset that previously missed the texture dependency.
- A static-symbol census is not proof of unknown virtual populations. The
  concrete incompatible loader route is eliminated by holding its only NEW
  invoking source, not declared compatible because it evades the direct census.
- Opaque FontVTable/data_00223d98, the W20 getter forms, `float(float)` converters,
  canonical DSP halfword/count storage, prepared runtime sound/heap suffix and
  canonical list member names remain. No old font/allocator/material/input/
  voice/table facade or byte overlay is reintroduced.

## Exercised reproof and continuing qualifications

The final complete proposed-primary sweep ran **2,667 sequential, niced,
single-process `tools/match.py --show 0` commands**, all **exit 0**. All
**2,767 required functions MATCH** and all **24 helper-only definitions** are
supplied in their natural contexts, using frozen profiles and explicit v6/L1.
The final source-bound rewrite projection has **`scope_complete=true`**.

Across this remediation there were **5,335 matcher commands**: **5,333 exit 0**,
**one exit 1** (the honest derived-event repair, now held), and **one exit 2**
(the subsequently fixed missing voice declarations). Every actual exit is kept;
no unchanged failed input was rerun to claim success. Published receipt metadata
omits fixed replacement instruction words while binding untouched raw hashes.

R1 helper identity/call-graph provenance remains `[INFERENCE]`, requiring
QUALIFIED independent rows for every affected caller. L1 establishes only its
witnessed literal span, not complete historical pool extent/order/TU ownership
or data credit. Native same-entry aliases, selective-link lifetime assumptions,
partial owner extents, preserved `ORIGINAL BUG:` behavior, ARMCC TLS/SVC/intrinsic
uses and C-only MUST-discard callbacks retain their stated qualifications. MATCH
never proves a missing receiver hierarchy, unknown callable population or unused
argument/result contract.

No whole-ledger interfaces/argcheck, full build, suite, prepare, review
registration, finalize or push was run here. Main must finalize c010-r7, integrate
this complete transaction, run and adjudicate serialized whole-ledger gates
(including unchecked C++, callable contracts and unique emitted ownership), then
prepare with two fresh builds and the suite and obtain two independent reviews.
Exact Main-only argv is in `r6/REMAINING-HEAVY-COMMANDS.json`. Any source/header/
profile/name/catalogue/composition drift requires fresh affected-closure proof
under unused evidence names.
