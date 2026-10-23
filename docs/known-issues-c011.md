# c011-r7 candidate — known issues and qualifications

This is an **unaccepted candidate**, a successor of c011-r6 `91172f54` on the
unchanged **c010-r7 `819edc6f`** source/current proposed ledger, finalized c009-r6
and denominator **v13-r2**. The sealed r6 pre-review FAIL and all older failed
reviews remain immutable. Main owns predecessor finalization, whole-ledger gates,
prepare, independent review registration, finalize and pushing. Gates stay
**v1.19**, with explicit **armlink5-inline-branchnop-v6 /
anonymous-string-records-v1** selection.

## Current scope

- **68 candidate units / 85 claims:** **76 NEW functions / 10,172 B**, plus
  **9 explicit stable-identity moves / 872 B**. All claims remain ordinary
  creditable functions under the frozen catalogue.
- All **2,687 current c010-r7 retained predecessor identities / 289,152 B**
  survive. The proposed ledger has **2,687 units / 2,763 functions / 299,324 B**,
  including all **24 inherited helper-only units**. These are proposed figures,
  not accepted credit.
- All **107 r6 c011 holds / 21,440 B** remain. The sealed finding and same-class
  sweep add **four NEW-only holds / 392 B**: **111 c011 holds / 21,832 B** total.
  The **28 c009 holds / 3,588 B** and **89 c010-r7 holds / 7,832 B** remain
  predecessor-owned. All **228 distinct manifest holds** stay uncredited.
- No accepted/current predecessor-retained identity or move is withdrawn. The
  nine move origins stay current, all four emptied old paths are absent, and
  `00155ab4` remains at its original prepared c010 owner.

Evidence entry point: `scratch/integrator/c011/r7/HANDOFF.json`. The per-finding
record, full native-facade sweep, proposed ledger, current holds, exact source
bindings and complete matcher/rewrite scope are sealed beside it.

## C011R6-F01 — bank reader native member facade

**NEW `00185a88` / 100 B is held.** Its C-named call declared
`const BankBlock *(const ::BankFile *)`, while the actual retained provider is
`BankFile::FileHeader::GetInfoBlock() const`, returning native `InfoBlock *`.
The old/new records are not aliases or a genuine native receiver relationship.

A local call substitution would require a FileHeader/result cast or a second
BankReader model. The real shared reader also has accepted C consumers and a C
reference protocol; the public native BankFileReader has a different complete
extent. This is a coordinated accepted-family migration prerequisite, not a
truthful one-line fix. The NEW reader is quarantined instead. The existing
accepted `0015276c`/`00185a6c` and provider remain unchanged; their inherited
boundary is explicitly **not** declared repaired by this exclusion.

## Entire-scope same-class sweep

The sweep resolves every bound relocation/storage reference from all **71
bank-isolated candidate objects** against the actual fresh defining-symbol
inventory, not merely `_ZN` names or neighboring table addresses. It checks both
C-spelled references to native definitions and the reverse native aliases over
retained C providers. Actual receiver/argument/result declarations were read.
It also screens native-bound but uncompiled endpoints against the accepted
shared declaration, rather than treating an unavailable body as a type waiver.

Two further concrete list boundaries are NEW-only held, as Main approved:

| NEW identity | Bytes | Actual mismatch |
| --- | ---: | --- |
| `001ce998` | 80 | `LinkListImpl::erase(LinkListNode *)` is called as a native member returning an iterator, but the real `0018d868` is `PoolLink *(PoolList *, PoolLink *)`. |
| `001cc1e4` | 104 | Native `erase(iterator)` at `00191178` has the same unrelated C owner/node/result-provider problem. |

The prepared C providers remain at their exact owners. The two NEW cleanup
sources and the exclusive NEW Layout::DeleteAnimTransform binding are removed
from the active candidate, with no native/C list typedef, PMF/function cast or
incomplete accepted-family cutover. The now-unused group-cleanup declaration
is withdrawn; the remaining Pane lookup declaration is retained.

### ServiceResult constructor boundary

**NEW `00117ad8` / 108 B is held.** Its `WordPair *(WordPair *, unsigned)`
facade reaches the same address already used by accepted
`ServiceResult::ServiceResult(Result)` construction. The provider is uncompiled,
but the accepted declaration/caller is sufficient to forbid a second receiver,
argument and result model for that function.

Two honest reuses of the real accepted constructor were exercised:

1. Canonical `ServiceResult` value result: **exit 1**, **116/108 B**, first
   difference `+0x20`.
2. Placement-construct an actual `ServiceResult(Result)` in canonical typed
   caller-provided storage and return that pointer: **exit 1**, **112/108 B**,
   first difference `+0x20`.

Both source snapshots/hashes and failed receipts remain. Neither requires a
fake wrapper, cast or added allocation, but neither is exact. Main's
fix-or-hold rule therefore selects the NEW-only hold, not code-generation
steering. The original C facade and failed C++ forms are quarantined; the
unused `00117ad8` public prototype is removed. Accepted native declarations,
constructor binding and older accepted consumers are not changed.

### Remaining boundaries and limits

The final **68 candidate objects** have **zero** C-spelled references to an
actual native-defined entry, **zero** native-spelled references to a C-only
provider, **zero** C-spelled references to an uncompiled native-bound endpoint,
and **zero references to the 228 combined holds**. Stored/function-reference
relocations are included, not just branch relocations. Object/source hashes
and per-source dispositions are recorded.

The remaining external native calls use the actual shared declarations:
`ServiceResult(Result)` on real ServiceResult objects and
`ResDicPatricia::Get(const char *, unsigned) const` on the actual dictionary.
The compiler-generated Pair/Scalar lifetimes, real recursive resource/heap
functions, allocator/destructor interfaces and native FxDelay provider family
were checked separately. Uncompiled/opaque dynamic populations remain qualified;
the two C sound-table providers are not assigned to a nearby native class by
address proximity. This is not blanket proof that inherited opaque/native
families or all predecessor-only facades are closed.

## Preserved earlier remedies and predecessor forms

The original fifteen r4 exclusions, four r5 exclusions, and four r6 exclusions
remain. In particular the loader virtual lookup, texture upload/held scalar
dependency, ChannelCommand byte-storage access and nonexact honest derived-event
repair remain isolated. No byte overlay, alias or constructor shadow is revived.
Opaque FontVTable/data_00223d98, canonical W20 getter forms, `float(float)`
converters, generic `void *(void *)` callbacks, const SoundEnvelope, real
ResTextureAnim/AudioDecodeEvent construction, canonical DSP/list/runtime fields
and all c010-r7 bindings remain.

## Exercised proof and Main handoff

The final complete proposed-primary sweep ran **2,663 sequential, niced,
single-process `tools/match.py --show 0` commands**, all **exit 0**. All
**2,763 required functions MATCH**, with all **24 helper-only definitions**
supplied in natural contexts under frozen profiles and explicit v6/L1. The final
source-bound rewrite projection has **`scope_complete=true`**.

Across this remediation **5,331 commands** returned **5,329 exit 0** and
**two exit 1** (the honest ServiceResult attempts). The initial bank-isolated
sweep's matches do not excuse the subsequently found type defects. Only the
final scope is the current proof. No unchanged failed input was retried;
published metadata omits fixed replacement instruction encodings while retaining
untouched raw receipt hashes.

R1 identity/call-graph provenance stays `[INFERENCE]` and requires QUALIFIED
independent rows. L1 establishes only its witnessed literal span, not historical
pool extent, order or ownership; helpers/data earn no credit. Native aliases,
selective-link lifetime assumptions, partial owner extents, `ORIGINAL BUG:`
behavior, ARMCC extensions and C-only MUST-discard callback qualifications remain.
MATCH never supplies a missing class hierarchy or a compatible unknown callback
population.

No whole-ledger interfaces/argcheck, full build, suite, prepare, review
registration, finalize, push or subagent ran here. Main must finalize the
applicable c010-r7, integrate this full transaction, run and adjudicate serialized
whole-ledger gates including unchecked C++, callable contracts and unique
emitted ownership, then prepare with two fresh builds/the suite and obtain two
independent reviews. Exact argv is in `r7/REMAINING-HEAVY-COMMANDS.json`. Any
source/header/profile/name/catalogue/composition drift needs new affected proof
under unused immutable evidence names.
