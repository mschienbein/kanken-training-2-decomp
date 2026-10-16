# c004 interface unification

Agent: IntegrateC004. Runtime family: OpenAI; model: `openai-codex/gpt-6.1-sol`.
Worktree: `/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/c004-unify`.
Branch: `c004-unify`. Scope: the dry ledger's units and `include/game/` only; the accepted ledger, candidate ledger, catalogue, tooling and accepted records are unchanged.

## Evidence-based disagreement decisions

1. **`func_00122500`: return an `int`, matching wrapper `func_001224fc`.**
   Packet `func_001224fc.md` identifies its two direct callers, `func_00101784` and
   `func_00109a48`. Their packets show calls at `0x001018d4` and `0x00109aec`, followed
   by tests of the returned r0 against zero at `0x001018dc` and `0x00109af0`.
   Packet `func_00122500.md` identifies the fixed-handle tail target `func_0012fda0`.
   The body and that helper declaration now return `int`; the body returns the helper's
   result instead of discarding it. Both wrapper and body MATCH.
2. **`func_00122510`: retain `void`, and make wrapper `func_0012250c` `void` too.**
   Packet `func_0012250c.md` has exactly one direct caller, `func_00109a48`; the body
   packet has no separate direct caller. The call at `0x00109af4` is followed by a path
   through the branch at `0x00109b04` to `0x0010a694`, where r0 is overwritten before
   it is read. No observed caller consumes this result. [INFERENCE] The public contract
   is therefore `void`, not the previous unevidenced unsigned return. The body still
   calls `func_00137d0c(0x407)`; wrapper and body MATCH. Indirect callers' original
   source-level types cannot be recovered from these direct-call packets.
3. **`func_0013714c`: keep pointer identity; remove the C++ local declaration.**
   Accepted `src/wave1/func_0011f080.cpp` passes this wrapper's result directly to
   `func_001852f4` when releasing `entry->allocation`. Its wrapper returns the result
   of the adjacent return-only `func_00137150`. Both contracts now live in the
   existing C/C++-safe `game/runtime.h`; the accepted C++ unit already includes it
   and no longer redeclares the shared symbol with local `extern "C"`. All three
   affected units MATCH. This was a declaration-location disagreement, not a reason
   to change the return type.
4. **`func_0018f7f0`: keep `void *(void *)` pointer identity.**
   Its packet is a return-only body, with direct tail callers `func_0011fa34` and
   `func_0011ffdc`; both wrapper packets identify this target. Accepted
   `src/wave1/func_0019e02c_returning.c` passes their returned pointers to
   `func_001852f4`. Consequently the incoming pointer must survive the return.
   The interface comes from `game/runtime.h`. The other caller,
   `src/wave4/func_0017e7c4.c`, casts its volatile guard address to `void *` at the
   call site and ignores the result; neither the caller nor identity dereferences
   that cast pointer. Its inlined callee identity remains explicitly marked
   `[INFERENCE]`, because a linker NOP does not prove which empty callee was chosen.
   Caller, adjacent wrapper and identity body all MATCH.
5. **`func_001ef880` / `func_001ef884`: both return their `void *` argument.**
   `src/wave3/func_001748b8.c` passes `func_001ef880` to `func_00100c80` as a
   `void *(*)(void *)` element-destroy callback. The wrapper is a branch-to-next
   NOP and the adjacent body is return-only; preserving r0 satisfies that existing
   typed callback contract. Both shared prototypes now come from
   `game/runtime.h`; the old no-argument/void fallback comments and definitions
   are removed. Callback consumer, wrapper and body all MATCH.

Private packet paths: `scratch/integrator/c004-unify-evidence/packets/<function>.md`.
Original instruction dumps and ROM/build bytes remain ignored private artifacts;
none is included in the source commit. The decisions above record only their
interface/data-flow consequences.

## Shared-header policy decisions

- `game/bss_0049ecf0.h`: command-mutex helpers `func_0012fb38` and
  `func_0012fb48`, used by the existing command-queue subsystem.
- `game/runtime.h`: the existing `func_001852f4` declaration plus the shared
  allocation/element cleanup identities described above.
- New `game/functions.h`: all remaining policy-item prototypes, in address order,
  under a C++ `extern "C"` guard. It forward-declares `ObjectManager` and `Registry`;
  their layout is not exported. `Registry` stays defined only by its initializer.
- Header layout exceptions are justified by actual completeness/layout needs:
  `RegistryEntry` is embedded in `RegisteredObject` and its `link` field is accessed
  by registration; its embedded `RegistryNode` is also embedded by `Registry`.
  `TimingState` is written by its reset routine and embedded by `ExtendedTimingState`.
  These shared definitions replace local copies. The pointer-only timing wrapper
  does not retain an unnecessary layout declaration.
- The defining units include the same header as their callers. Local shared
  prototypes are removed, including the C++ local `extern "C"` declaration.

## Verification performed

All commands used the project interpreter
`/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python` from this worktree.
Exact argument vectors, exit codes and output paths for the packet, match, gate,
build and preservation commands are in `scratch/integrator/c004-unify-commands.json`.
Source/object/header hashes and every per-unit match-log/result path are in
`scratch/integrator/c004-unify-manifest.json`.

- **45/45 touched translation units MATCH**, run sequentially with
  `tools/match.py <source> --profile game --out
  scratch/integrator/c004-unify-evidence/matches/<wave>/<stem>`; every exit code 0.
- **G3 PASS**, one post-change run, exit 0:
  `tools/interfaces.py check --matches scratch/integrator/c004-dry.json
  --allow-unchecked-cpp --json scratch/integrator/c004-unify-interfaces.json`.
  274 units; 261 checked C units; **0 disagreements / 0 policy items**;
  13 C++ units remain unchecked as explicitly permitted by the assignment.
  Output: `scratch/integrator/c004-unify-interfaces.log`.
- **Dry build PASS**, exit 0:
  `tools/build.py --name c004-dry --matches scratch/integrator/c004-dry.json`.
  299 credited functions / 7,620 bytes. All credited functions exact; full
  `code.bin` and compressed ExeFS `.code` both identical. This is one requested
  non-fresh dry build, not checkpoint prepare or independent acceptance.
  Output: `scratch/integrator/c004-unify-build.log`.
- Preserved receipt: `scratch/integrator/c004-unify-receipt.json`, SHA-256
  `6e1140dffe52b093ea26c9ba2545003e8f941e016e12d31c69ed41940ce64b05`.
  `code.bin`: `a367b5dcd65e3924c93cf3fbaee5d4d40f9c5205d1287912784a974d5e405361`.
  ExeFS `.code`: `19a4445c367af066b5f14b97ca016c0e19c305fc2b4babcb087595823d0060da`.
- **`build/c004-dry` deleted** after complete lossless preservation. Its 506 regular
  files, directories and links were inventoried, every archive member was hashed,
  and the live tree was re-walked immediately before removal. Native tar also
  preserved 514 AppleDouble metadata members, separately inventoried. No accepted
  build or external binding referred to this disposable build; its own dependency
  records and command-log/inventory self-citations are disclosed in the cleanup
  record. Archive:
  `scratch/integrator/c004-unify-evidence/c004-dry.tar.gz`, SHA-256
  `a953f33a21000394c789617e6f9a6e5522770630fcc9f7d1f96030492a4f7fca`.
  Inventory and cleanup records are beside it. Approximate post-archive free-space
  increase on removal: 3,239,936 bytes; other host writers make this approximate.

No full test suite, checkpoint prepare/finalize, publication, or canonical-ledger
mutation was performed. The normal independent review, including the 13 unchecked
C++ units and recorded linker rewrites, remains the integrator's checkpoint work.
The LSP route reported no C/C++ server, so symbol uses were inspected with the
source/packet tools rather than assuming references were available.

## Revised accepted units

These five accepted units changed and must be re-reviewed as `revised_units`;
all other modified units belong to the unaccepted wave-3/4 candidate set.

| Accepted unit | Revised source SHA-256 |
|---|---|
| `src/wave1/func_0011f080.cpp` | `b3759e8d75b006e941939c2b5190159971f97a7b285edd32b3a494c5a62330e9` |
| `src/wave1/func_00195254.c` | `b6b662ee0f85b3f9c82c903bcdf5c0c5821657d725d81f72f3199f80ea20f000` |
| `src/wave2/func_00110634.c` | `a262d3accf77d6df99e8578c71986d7025476970fa06caeecb02b51881112426` |
| `src/wave2/func_0013db3c.c` | `b52de6cd04772297fe3d0e8e77ab90877041e2e28eaa2f8b0a4b66cc008f1aad` |
| `src/wave2/func_00147260.c` | `68f44ff520ffe1ce54ad4f9e32a73a6d439dd4a59f2cd1c8be34e84dae5a7c5b` |

## Changed header hashes

| Header | SHA-256 |
|---|---|
| `include/game/bss_0049ecf0.h` | `4181fc0caa63ef9583e3c00b86520784028f570ca149ef23c77454b2ed8555b3` |
| `include/game/functions.h` | `a9426df700ad3bafe5e3d8fc4b4340ed3c422fda0d006f921716a750a5ac5d71` |
| `include/game/runtime.h` | `b451c565dbb5f2771081f00b9c77fe39cd82043c732900329549e7c80c1af35a` |

Open blockers within this assignment: none.
