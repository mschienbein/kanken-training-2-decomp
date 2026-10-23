# c013 intake candidate — known issues and qualifications

This is an **unaccepted candidate** on branch `c013`, created in the new
`scratch/worktrees/c013` worktree from **c011-r7
`8359b8594dd53889717cca14fb0a9de0edd201e2`**. The branch and worktree were both
absent before creation. It inherits the c010-r7 plus c011-r7 source stack;
no predecessor is finalized by this work. Gates remain **v1.19**, denominator
**v13-r2**, with explicit **armlink5-inline-branchnop-v6 /
anonymous-string-records-v1** selection.

## Admitted scope

| Identity | Source | Frozen profile | Bytes |
| --- | --- | --- | ---: |
| `0012e68c` | `src/checkpoint13/language.cpp` | middleware | 296 |
| `0010aa28` | `src/checkpoint13/copy_worker.c` | game | 280 |
| `001c65e4` | `src/checkpoint13/transform_callbacks.cpp` | game | 332 |

**3 NEW functions / 908 B; no moves.** All 2,763 predecessor-proposed identities
and all 24 helper-only units survive. The composed proposal is **2,690 units /
2,766 functions / 300,232 B**. These are candidate figures, not accepted credit.
The seven requested identities were checked against the accepted ledger and
c010-r7, c011-r7, c012 `25dec844`, cmodels2-r4 and types-compose manifests,
resolving names to stable identities: none was already credited or claimed.

### Canonical integration

- `game/language.h` owns the constant configuration byte and public language
  selector declaration. The source-local `INTEGRATOR` declaration is removed.
  The regional masks are used anonymous-namespace values, not a second shared
  object model.
- `game/runtime.h` completes its existing `BufferOwner` with signed ring indices,
  256 canonical `CopyRequest` records, stop flag and existing `WorkerThread`
  tail. Existing buffer/count/owned/state/active offsets remain
  `0xc08/0xc0c/0xc10/0xc14/0xc18`; stop and thread start at `0xc1c/0xc20`.
  The decompressor and `void(void *)` worker entry are declared in that header.
  No private owner copy, storage union or byte-offset field access remains.
- The two canonical wrapped/variable `MoveArray` constructors initialize fresh
  capacity storage to zero before their existing setters. The source-local
  signal specializations are **not** retained. Native `TransformNode`, `Signal2`
  and slot types are reused; all their header dependents were re-proved.
- No names overlay is added or rebound. `TransformNode::CreateCallbacks` already
  has its canonical binding; the other admitted entries are address-named C
  interfaces. Overlays belonging only to held inputs are not installed.

## Explicit holds

All four held author MATCHes remain uncredited; no claim is made that their
unintegrated TUs pass current v6/L1 admission.

| Identity | Bytes | Unresolved integration boundary |
| --- | ---: | --- |
| `001d9820` | 156 | The pool allocation is already exposed as `AudioVoice *`, while the author models that storage as `ConstructedPaneObject` with different runtime-member, registration and timing owners. A native name, equal offsets or allocation-result cast cannot establish a second class model. A coordinated canonical voice construction/member migration is required. |
| `001d01dc` | 328 | Native `SoundArchive::GetWaveArchiveCount() const` would alias the retained C `func_001f2b90(ArchiveHandle *)` provider with an `ArchiveIndex *` receiver field, not the native reader. Its delivered TU also retains a mismatching partial Window reconstruction. The real archive provider/reader family and Window partition/layout need reconciliation, not a C facade. |
| `001f4408` | 180 | The shared bank decoder uses byte-offset chunk/reference resolution, including a synthesized header/rounded-bounds/index displacement before dereference. Existing `AddOffsetToPtr` spelling does not waive the standing byte-offset-access prohibition. Private one-entry tails also do not establish a complete array extent. |
| `001f46d0` | 180 | Uses the same unresolved bank decoder. Tag-evidenced downcasts do not by themselves discharge its separate storage-access and extent obligations. |

The intake has **4 holds / 844 B**. `001d01dc` was already held by c011, so this
adds only **3 distinct holds / 516 B** to the inherited 228: **231 distinct
combined holds**, not 232. Existing c009/c010/c011 exclusions are unchanged.

### Accepted bank accessor ownership

The nm01 TU is not placed under active `src/` and none of its names are adopted.
Accepted **`001f4500` remains solely at `src/wave9/func_001f4500.c`**, with its
original game profile and source hash. There is no native duplicate and no
implicit owner move. Any later admission retaining that accessor must use an
explicit manifest move and remove its emptied old source, or omit its definition
from the new TU. The author's nonmatching `001f44bc` context is not credited.

## Exercised proof

All **1,384 sequential, niced, single-process `tools/match.py --show 0`
commands exited 0** under the frozen per-unit profiles and explicit v6/L1.
All **1,422 required functions MATCH** at their complete catalogue extents.
The three new receipts were produced after the final header edits and reused
without an unchanged rerun. The remaining 1,381 commands re-proved dependents.

Conservative project-header reachability includes **1,375 runtime.h units**,
**29 MoveArray.h units** and the new language-header owner. Their union is
**1,394 units: 1,384 primaries and 10 affected helper-only units**. All natural
extra-source combinations are preserved; **17 helper definitions** are supplied
in the fresh contexts. All 24 inherited helper-only units remain in the proposal.
Two additional MATCH comparison rows are existing compiler runtime helpers
(`__ARM_common_memset4_5`, `__ARM_common_memcpy4_5`), not new credit.

The complete rewrite projection covers **2,666 proposed primaries**:
**1,384 fresh receipts plus 1,282 untouched c011-r7 receipt carries**. Its complete
scope is not a claim that c013 reran the whole ledger. Source/header/name/profile/
catalogue/tool hashes and unchanged inputs are bound separately. Published safe
receipts omit replacement instruction encodings and bind untouched raw receipt
hashes; no ROM bytes, disassembly, packets or object files are committed.

Evidence entry point: `scratch/integrator/c013/HANDOFF.json`.
`MATCH-SCOPE-r1.json`, `SAFE-MATCH-RECEIPTS-r1.json`, `REPROOF-ATTEMPTS-r1.json`,
`SOURCE-CONTEXT-r1.json`, `LINKER-REWRITES-r1.json` and `MATCH-SUMMARY-r1.json`
record the actual scope and results. `AUTHOR-INTAKE-r1.json`, `OWNERSHIP-r1.json`
and `HOLD-DISPOSITIONS-r1.json` bind source inputs and ownership decisions.

## Self pre-review and qualifications

`SELF-PREREVIEW-r1.json` records **48 source-policy and historical failure-class
dispositions**, including every c009/c010/c011 review failure family. This is a
self pre-review, **not an independent acceptance certificate**. Fresh admitted
object symbol/relocation inspection finds no new `_ZTV`, no new native/C facade
and no references to any of the 231 held identities.

- `language.cpp` retains approved **`__current_pc`** and the explicit
  **`ORIGINAL BUG:`** assumption that the configured language is below 32.
  Regional helper/TU identity and middleware subsystem ownership remain
  **[INFERENCE]**; other profiles were not remeasured here.
- Queue/control names and captured SignalState role remain **[INFERENCE]**.
  The worker preserves signed-length copying, clears `active` only after the
  queue drains, and returns on stop without clearing it. No new validation,
  allocation, byte overlay or total historical owner extent is invented.
- The fresh-capacity initializer's historical spelling is **[INFERENCE]**.
  The actual canonical native signal and allocator interfaces are used. Creating
  empty signal storage does not recover every external allocator or eventual
  dynamic slot-provider population.
- All inherited R1/L1 and C-only MUST-discard qualifications survive. The fresh
  closure contains 18 R1 caller rows and one L1 pool-owning unit; erased helper
  identities remain inferred and L1 proves only its witnessed span. Helpers and
  external data earn no credit. No accepted bank/list/result/scene/font family
  is declared repaired merely because a NEW caller is held or bytes MATCH.

## Remaining Main-only integration

Small c010-r7/c011-r7 follow-ups may require the requested later rebase. Recompose
against the actual predecessor ledger and re-prove any changed closure under
fresh evidence names; these records are bound to the stated starting commit.
Other candidate branches were checked for ownership, not merged into this stack.

After the applicable predecessors finalize, Main must integrate this complete
transaction, run/adjudicate whole-ledger interfaces and complete-rewrite argcheck,
and prepare with two fresh full builds and the suite, followed by independent
bound reviews. Exact commands and unused output names are in
`REMAINING-HEAVY-COMMANDS.json`. **No heavy gates, full build, suite, prepare,
review registration, finalization, push or subagents ran in this assignment.**
