# cmodels2 known issues and qualifications — r5

**Not prepared; blocked on CM2-F01 and CM2-F03.** This work starts from finalized
main `5820254f`, including c010-r8 (history sequence 11, 2,682 accepted functions)
and adopted GATES v1.20. R4 history and evidence are preserved, but stale predecessor
source/header/name overlays do not replace accepted c010-r8 callback contracts.

## Reachable source scope

- **4 NEW functions / 596 B**, in four credited units and one new helper-only unit:
  `001456e4` (240 B), `00169018` (144 B), `00171208` (96 B), `001e5d38` (116 B).
- **4 revised accepted functions**, plus **2 revised accepted helper-only units**:
  typed text-resource table use, the completion receiver/result contract, and the
  same-receiver counted-item contract. Existing source owners are retained; **0 moves**.
- Existing helpers are reused uniquely. The proposed ledger has **2,686 functions /
  289,176 B**, **2,623 units**, including **25 helper-only units**. These are proposed
  counts, not accepted credit.
- All admitted candidate units and their current proof closures use explicit
  **`armlink5-inline-branchnop-v6` / `anonymous-string-records-v1`**. No U1 unit or new
  native scene binding is admitted. A future checkpoint still selects one global
  linker model; individual proof labels cannot silently override that selection.

## Native root/key decision under adopted v18

The authorized native-vtable pattern requires a virtual destructor, an actually
undefined key function, and a constructor/caller-evidenced external `_ZTV` binding.
A credited TU must not emit the table. No virtual slot may be inserted, removed or
reordered to obtain that behavior.

The preserved MenuScene layout puts its out-of-line destructor first among non-pure,
non-inline virtual functions. Defining it makes it the key and emits a table with a
live D0 reference. Main's v18 attempt006 refusal is retained, not rerun or suppressed.
The known MenuResetScene attempt008 success alone does not close the root/provider
transaction.

A changed, natural source arrangement was measured: put the actually empty root
destructor inline and retain the existing undefined `finish` key. It adds no slot,
attribute, forced reference or compiler option. The root then emits **no surviving
catalogued D1**, and all eight derived D1 comparisons are four bytes short:

| Derived class | Candidate / target B | Result under v18 + L1 |
|---|---:|---|
| CalendarScene | 188 / 192 | mismatch |
| DepartureScene | 68 / 72 | mismatch |
| DetailedTransitionScene | 68 / 72 | mismatch |
| ReturnScene | 68 / 72 | mismatch |
| CompletionScene | 68 / 72 | mismatch |
| MenuResetScene | 96 / 100 | mismatch |
| ExitSelectionScene | 84 / 88 | mismatch |
| CategoryScene | 96 / 100 | mismatch |

U1 legally removes the derived D0 sections in these comparisons; it does **not** waive
retained D1 equality or repair the missing extent. There is no demonstrated exact
native-family transaction inside the authorized pattern. All **nine accepted destructor
forms remain unchanged**. Their identities are neither deferred nor removed. Native
D1 names are not rebound to unrelated free C cleanup bodies; no synthetic base, pure
qualifier change, reference suppression, PMF decoding or source-policy exception is used.

Exact decisions, actions, source/receipt hashes and preserved accepted owners are in
`scratch/integrator/cmodels2-r5/NATIVE-FAMILY-DECISION.json`. The failed changed-source
probes remain private evidence, not admitted source or accepted credit.

## Findings and holds

- **CM2-F01 remains blocking:** the real scene lifecycle providers and their native/
  explicit table interfaces require one coherent exact migration. Keeping accepted
  forms is not a claim that the previously identified slot contracts are repaired.
- **CM2-F03 remains blocking:** the three accepted scene/member-callback relationships
  need the genuine shared native hierarchy and providers. The PMF is actually invoked;
  receiver erasure, unrelated record aliases and hand-decoding are not repairs.
- **CM2-F02:** only NEW `001e958c` (296 B), its geometry helper and its private owner
  header stay quarantined. The accepted c010 cleanup/provider forms win composition.
- **CM2-F04:** retain the R4 typed two-cell text-resource address-point table and its
  explicit extent qualification. Its live cell is `void(W22TextResources *)`; no arity
  is invented for the null/VFE cell, no native class identity or data credit is claimed.
- **10 tracked NEW holds / 4,760 B** remain: `0017a49c`, `001ab680`, `001acc88`,
  `001acf28`, `001bc5ac`, `001d5694`, `001d5864`, `001db404`, `001ef89c`, `001e958c`.
  Historical failures, v3-only qualifications and c011/audio ownership remain intact.
  The nine unchanged accepted destructor forms are not counted as NEW holds.

## Models, contracts and exercised evidence

- **86 sequential niced bounded matcher commands**, all exit 0: **93 unique functions /
  16,016 B**, covering **117 source contexts**, including all **25 helper-only units**
  through genuine callers. No helper is tested as an uncredited standalone replacement.
- Current model scope is **28 accepted R1 callers + 4 NEW R1 callers**, and **one L1
  owning function**. R1 identity, outlining, original existence and TU/call-graph claims
  remain `[INFERENCE]`/QUALIFIED. Exact bytes do not close CM2-F01/F03.
- The last accepted selector is already v6/L1, so the current proposal makes no model
  transition. If v18 becomes admissible, every listed accepted R1 caller and the L1
  owner must reopen, in addition to new/revised/U1/header/helper dependencies.
  `MODEL-SELECTION.json` lists them explicitly; no known finding is narrowed away by
  the current selector.
- C010-r8's runtime-callback closure remains authoritative. None of the four changed
  credited functions is a provider row in that closure. Old incompatible callback
  signatures are not imported; every changed invoking interface still needs Main's
  final complete review.
- L1 keeps its original pool-extent, sharing, ordering and historical-TU qualifications;
  no literal-data credit is added. The held geometry unit earns no helper/unwind credit.

No whole-ledger interfaces/argcheck, emitted-owner audit, suite, full build, prepare,
review/finalize or push was run. Bounded comparisons may reuse the compiler cache and
are not two fresh full G4 receipts. Main must resolve both blocking findings, refresh
all final dependencies and run the serialized commands in
`scratch/integrator/cmodels2-r5/REMAINING-HEAVY-COMMANDS.json`. Do not prepare this scope
or interpret this handoff as checkpoint readiness.
