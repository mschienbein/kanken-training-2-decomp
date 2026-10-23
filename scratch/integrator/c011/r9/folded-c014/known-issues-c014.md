# c014 intake candidate — known issues and qualifications

This is an **unaccepted candidate** on new branch/worktree `c014`, starting at
**c011-r8 `f9ea308e21e3d9faa439631ac56189e3b28e62d2`**, on c010-r8 `f78df906`.
Both branch and worktree were absent before creation. Gates remain **v1.19**,
denominator **v13-r2**, with explicit **armlink5-inline-branchnop-v6 /
anonymous-string-records-v1** selection. No predecessor is finalized here.

Main's required checkpoint order is **c011 → ctypes1 → c014 → c012**. The
present source/proof binds the stated pre-ctypes1 base, so Main must rebase and
refresh changed proof against the actual predecessors before preparation.

## Candidate scope

| Identity | Natural unit | Frozen profile | Bytes |
| --- | --- | --- | ---: |
| `001f29b4` | `byte_parameters.cpp` | middleware | 164 |
| `00122360` | `aligned_buffer.c` | sdk | 44 |
| `00154674` | `channel_drop.cpp` | middleware | 232 |
| `00136450` | `service_strings.c` | sdk | 192 |
| `00192a7c` | `service_handle.cpp` | sdk | 52 |
| `00157500` | `candidate_ranking.c` | middleware | 468 |
| `0018191c` | `image_activation.cpp` | game | 120 |
| `001acb38` | `menu_modes.cpp` | game | 88 |
| `0014cc1c` | `auxiliary_mode.c` | middleware | 128 |
| `0014242c` | `memory_create.cpp` | sdk | 148 |

All units are under `src/checkpoint14/`. **10 NEW functions / 1,636 B; no moves.**
All **2,761 predecessor-proposed identities / 299,660 B** survive. The composed
proposal is **2,695 units / 2,771 functions / 301,296 B**, including all **24
inherited helper-only units**. These figures are proposed, not accepted credit.

All thirteen requested identities are ordinary catalogue functions and were
checked against main, c010-r8, c011-r8, c012 `25dec844`, cmodels2-r4 and both
types-compose branches, resolving names to stable identities. None was accepted
or claimed. Matching/unmatched author companions and context ownership are
recorded separately; no whole-TU mismatch is relabeled as an all-MATCH input.

## Canonical integration

- The byte-parameter reader shares its actual reader/entry/output records in
  `game/event_route.h` and uses **pinned `Util::Table<unsigned char>` and
  `nw::ut::AddOffsetToPtr`**, not hand-computed indexed byte displacement.
  Existing signed reference count, unsigned lookup index, seven-byte output,
  narrowed count and original unchecked typed-reference dereference survive.
- Both aligned-span initializers use the one `AlignedBuffer` in
  `game/linear_buffer.h`; the accepted sibling's local copy is removed. The
  unrelated `LinearBuffer` allocator object is not aliased to it.
- The channel callback implements the actual invoking type
  **`void(AudioChannel *, AudioVoiceGroup *)`** directly, as required by both
  original runtime routes in `RUNTIME-CALLBACK-CLOSURE.json`. Its nested group
  callback retains **`void(AudioVoiceGroup *, int, void *)`**, matching the
  known supplied providers. No generic erasure or native Voice alias is used.
- Service strings share their bounded-copy/table declarations. The accepted
  single-string updater and group-string caller use the canonical declaration.
  The handle getter uses a single opaque-prefix handle object; the checked
  symbol names its actual `+4` handle word, not an independent overlapping
  service object.
- HWR candidate records have one shared header. Drawing activation and its
  accepted mask selector share one preset block and retain unsigned fields;
  only the consumed local default snapshot is signed. The existing HWR wrapper
  declaration is shared without changing its body or argument widths.
- Menu activation uses the canonical `W23FlowScreen` mode prefix, adding only
  opaque padding plus the used `+0xac` mode word. The large private
  `MenuResetStateTu605`/controller model is not imported.
- DSP settings are one real **`AudioDspSettings` subobject at parent `+4`**.
  Every prior field, width and offset, including the existing volume subobject,
  is preserved; all fourteen typed source users are migrated together. The new
  setter has no `AuxiliaryFilterMode` shadow or class cast.
- The mapping wrapper uses canonical `MappedMemoryBlock`, its real writable
  handle and all five raw service inputs. `MemoryCreateOperation` is a used
  borrowed operation, not a second mapping-record model.

No names overlay is installed or rebound. Overlays needed only by unselected
native companions remain author evidence, not live bindings.

## Coordinated TaskPool dependency — no duplicate credit

The initial new-object audit caught the channel callback's call to **`0017e764`**,
which is held in the c011 source ledger but already claimed by c012. Main
explicitly directed c014's shared type to land first and c012-r4 to adopt it.

`game/task_pool.h` is byte-identical to the existing evidenced c012 prefix:

```cpp
struct TaskPool {
    unsigned char initialized;
    unsigned char reserved01[3];
    struct PoolList active;
    struct PoolList available;
};
```

`active` is at `+4`, `available` at `+0x10`. **`0x1c` is a known prefix, not an
allocation extent.** `PoolList` is the canonical unsigned count plus sentinel;
both lists contain real `AudioVoiceGroup.link` nodes. No C/native list alias,
extra capacity fields or invented inheritance is introduced.

The two local C prefix definitions are removed from:

- `src/wave16/func_00153fec.c`
- `src/wave16/func_0015477c.c`

Both now obtain the same model through `audio_voice.h → task_pool.h`. The exact
model, paths, element contract and peer agreement are recorded under
`scratch/integrator/c014/TASK-POOL-AGREEMENT-r1.json` and in both handoffs.

**`0017e764` remains c012-owned, with no c014 definition, extra source or credit.**
A private standalone interface/layout witness MATCHes all **44 B** under game
v6/L1. This closes the real shared-type/interface gap rather than hiding a held
reference; the ordinary external call still resolves to its original function.
Its eventual source/credit adoption remains c012's responsibility.

### Inherited owner-recovery attention

Accepted **`0015477c` retains its pre-existing `offsetof`/byte-subtraction owner
recovery**. Only the local type definition was migrated; its body is unchanged.
This intake does **not** newly approve that expression. Main was explicitly
notified and independent contract research investigates it. Preserve any
ctypes1 body fix during rebase; do not overwrite it with this older body.
The c012 NEW drain must use a compliant iterator or remain held. The new c014
callback and private release-interface witness do not perform owner recovery.

## Explicit holds

| Requested identity | Bytes | Remaining boundary |
| --- | ---: | --- |
| `001ae140` | 420 | Real `ResourceDescriptor` objects are passed toward the retained `NameTable` dispatcher family. Their complete types/dispatch tails have no evidenced common hierarchy. A prototype-only retag, table cast or invented alias is not a cutover. The author's corrected cleanup receiver does not resolve this independent contract. |
| `0014aa94` | 96 | The match reinterprets game `Material *` as `nw::lyt::Material *`; its native setter companion remains nonmatching. Pane/getter/material/setter producers and all accepted consumers need one coherent model. |
| `0017df84` | 88 | The submitter calls unresolved held queue endpoint `0014a7e0` through `StreamQueue *`, unlike the known volatile unsigned native WordQueue contract. Canonical TaskPool does not repair that queue or the request's signal-storage view. |

**3 intake holds / 604 B.** `001ae140` is already inherited, so only **2 distinct
holds / 184 B** are added to the parent's 236: **238 distinct credit holds**.
`0017e764` is not credited here; its separately closed external interface is
recorded explicitly rather than silently removing c012's ownership boundary.
All other predecessor exclusions remain intact. No accepted identity is held
or withdrawn and no matching companion is defined twice.

## Exercised proof

The final coordinated closure ran **594 sequential, niced, single-process
`tools/match.py --show 0` commands**, all **exit 0**. All **602 required
functions MATCH** their complete catalogue extents under frozen per-unit
profiles and explicit v6/L1. It covers all **12 changed/added headers**, all
**20 source-revised predecessor units**, and both affected helper contexts.

There were **605 actual matcher commands, all exit 0**: ten initial target
witnesses, the private 44-byte external-context witness, and the fresh final
594-command closure. Initial byte matches are historical, not final admission
proof: they preceded the caught TaskPool interface gap and its canonical fix.

The complete proposed-ledger rewrite projection covers **2,671 primaries**:
**594 final fresh receipts plus 2,077 untouched c011-r8 receipt carries**.
This is a complete projection, not a claim that c014 reran every untouched unit.
All 24 helper-only sources survive; the private c012-owned witness is excluded.

Entry point: **`scratch/integrator/c014/HANDOFF.json`**. Final scope, receipts,
attempts, source/header bindings and rewrite evidence use **`-r2`** names.
The `-r1` initial scope and boundary disposition remain immutable history.
Published safe receipts omit instruction encodings and bind raw receipt hashes;
no ROM bytes, packets, disassembly or object files are committed.

## Self pre-review and qualifications

`SELF-PREREVIEW-r2.json` records **57 standing-source and historical c009/c010/
c011/c013 failure-class dispositions**. It is a self pre-review, not an
independent acceptance certificate. Final actual-object inspection finds no
new `_ZTV`, name binding or unresolved native/C/callable interface conflict.

- `__current_pc` in the handle getter is an approved intrinsic requiring a
  qualified review row. Matching SDK/middleware profiles establish byte
  witnesses, not authenticated historical ownership.
- Private helper/value names, historical TU grouping, partial object identity
  and field roles remain **[INFERENCE]** where indicated. All used values affect
  real operations; there is no volatile, barrier, asm, forced spill or fake call.
- Canonical one-entry serialized tables are a pinned format convention, not a
  recovered complete array extent. HWR invalid-template sentinel and fixed
  32-slot shift behavior remain; no bounds validation is invented.
- Original null-reference, fixed string-table capacity and discarded-history
  bugs remain explicit. Signed preset-read provenance does not change unsigned
  shared storage or consumer contracts.
- Inherited R1/L1, C-only MUST-discard, accepted callback attention and unknown
  dynamic population qualifications remain Main obligations. No opaque-provider
  waiver or guessed callable table is extended. The specific inherited pool
  owner-recovery attention above is not claimed closed by byte equality.

## Main-only remaining work

Main must finalize the applicable predecessors in the specified order, rebase
this transaction onto actual ctypes1 source, retain the one coordinated pool
model, adjudicate inherited body/interface attention and refresh changed proof.
Then run whole-ledger interfaces and complete-rewrite argcheck, prepare with two
fresh full builds and the suite, and obtain independent bound reviews.

Exact commands and unused output names are in `REMAINING-HEAVY-COMMANDS.json`.
**No heavy gates, full build, tests/suite, formatter, prepare, independent review,
review registration, finalization, push or subagents ran in this assignment.**
