# Compiler identification, round 2

Worker: `CompilerIdR2`; family **OpenAI**; runtime model **openai-codex/gpt-6.1-sol**.

## Recommendation and scope

Recommend **`armcc_504_82`, `-O3 -Otime`, ARM**, using the ordered `game` profile below.
The final grid has **one all-match cell out of twelve**, covering **11 functions / 940 bytes**
from six independently compiled sources. Four loop-bearing functions distinguish `-O3` from
`-O2`. Excluding the second, closely related enum adapter leaves **10 functions / 772 bytes**;
the numerical threshold does not depend on counting both adapters as independent evidence.

These are strict in-place **candidate source matches**, not accepted decompilation credit or a
whole-image build. They support the multi-feature compiler portion of H5. Main still owns the
profile addition and acceptance decision. The original spelling of several C++ switches and the
build provenance of prebuilt libraries remain underdetermined; they are not reported as recovered.

### Profile JSON for the integrator

Insert this member into `profiles` in `config/compiler_profiles.json`:

```json
"game": {
  "compiler": "armcc_504_82",
  "flags": [
    "--cpu=MPCore",
    "--fpmode=fast",
    "--apcs=/interwork",
    "--arm",
    "--split_sections",
    "--signed_chars",
    "-O3",
    "-Otime"
  ]
}
```

This is byte-for-byte the ordered flag list of the existing
`trial-504-82-O3-Otime`, which is the name actually exercised by the `match.py` receipts.
No profile/configuration file was edited by this worker. ARMCC selects C++ for `.cpp` files
without a separate `--cpp` flag; its help output explicitly documents that default.
Consequently this profile compiles the real C++ virtual-call probe as C++, while retaining C
as an available source language for the other controlled probes. Omitting a forced `--cpp`
does **not** mean the game's object-oriented code was originally C.

Do not add speculative vendor profiles from this round. The optional Thumb reproducer described
below is compatible with several compiler builds and is not a separately identified original build.

## Exact-match table

All source paths below are relative to `research/compid/r2/`. Every row was exercised with
`tools/match.py`, returned **MATCH** over the full catalogued extent including owned pools/tables,
and belongs to a command whose exit code was **0**. Receipts retain the actual trial profile name;
after adding the identical `game` profile, substitute `--profile game` in the same commands.

| Address | Feature | Source | Bytes | Actual result |
|---|---|---|---:|---|
| `0x103258` | Pointer-advancing loop; conditional resource cleanup; two BL targets | `loop_cleanup.c` | 136 | MATCH |
| `0x103358` | Independently sized resource loop; dynamic bound reload; calls | `loop_cleanup.c` | 116 | MATCH |
| `0x103798` | Global address literal pool; conditional BL; global store | `game_core.c` | 40 | MATCH |
| `0x105084` | Fixed-eight resource loop; indexed structure access; conditional BL | `loop_resources.c` | 64 | MATCH |
| `0x105fa8` | Fixed-eight strided initialization; pointer induction; mixed byte/word fields | `loop_state.c` | 100 | MATCH |
| `0x1091d4` | Signed-int-to-float VFP conversions; 60-byte structure indexing; store scheduling | `game_core.c` | 56 | MATCH |
| `0x10d604` | C++ class virtual allocation call; null check; constructor/helper tail call | `game_heap.cpp` | 64 | MATCH |
| `0x10e664` | Leaf structure-field store | `game_core.c` | 12 | MATCH |
| `0x1103d4` | VFP float division and float return | `game_core.c` | 16 | MATCH |
| `0x1718ec` | Twelve-entry switch jump table; relocated case targets; BL | `game_enum_switch.c` | 168 | MATCH |
| `0x171c7c` | Second enum-adapter jump table, counted separately but conservatively excluded from the independent minimum | `game_enum_switch.c` | 168 | MATCH |

The two enum adapters preserve the original out-of-range uninitialized result and carry
`ORIGINAL BUG:` comments. A valid selector may make that path unreachable, but that invariant
was not established here. This qualification is visible in the source, not concealed with an
invented fallback. The functions are reproduced with switches, not hand-written tables or assembly.

Primary evidence: `final_profile_grid_compid.json`, `game_core_match.json`, `loop_cleanup_match.json`,
`loop_state_match.json`, `loop_resources_match.json`, `game_heap_match.json`,
`game_enum_switch_match.json`. Full source and receipt hashes are in `analysis_final.json`.

## Discriminating compiler and optimization evidence

`final_grid.json` tests four archive compiler IDs against three flag lists, using the same six
source texts in every cell. `Y` means exact; `+N` means first differing byte at hexadecimal offset N.
A non-qualifying grid cell is not evidence that no imaginable source could match that compiler.

| Cell | All 11 match? | Selected discriminators |
|---|---|---|
| `armcc_40_821:O3-time` | No | `0x103258 +5`, `0x105084 +5`, `0x1091d4 +4`, `0x1103d4 +2` |
| `armcc_41_1049:O3-time` | No | `0x103258 +49`, `0x103358 +19`, `0x1091d4 +20` |
| `armcc_41_1440:O3-time` | No | Same discriminator offsets as b1049 for these sources |
| `armcc_504_82:O2-time` | No | All four loop probes differ; `0x1091d4 +4` |
| **`armcc_504_82:O3-time`** | **Yes** | **11/11 Y** |
| `armcc_504_82:O3-space` | No | Resource loops differ; `0x103798 +10`; virtual factory `+28`; switches `+e` |

No other cell in the complete twelve-cell matrix matches every function. This independently
reinforces, rather than merely repeats, the earlier 5.04 scheduling result in `docs/COMPILER.md`.

### Four loop witnesses, and the unrolling question

| Function | 5.04 O2/time | 5.04 O3/time | 5.04 O3/time, loop level 0 | 5.04 O3/time, loop level 1 |
|---|---|---|---|---|
| `0x103258` | `+19` | Y | Y | Y |
| `0x103358` | `+4` | Y | Y | Y |
| `0x105084` | `+0` | Y | Y | Y |
| `0x105fa8` | `+0` | Y | `+0` | Y |

The first two rows are in `loop_cleanup_grid_compid.json`; the fourth is in
`loop_state_grid_compid.json`. The `0x105084` row was first tested unchanged as part of
`game_loops.c` (`loops_initial_compid.json`), then isolated without changing its body in
`loop_resources.c` for the final twelve-cell matrix and direct MATCH receipt.

The documented `--help` output lists O2/O3 but no unroll switch. An explicit compile with
`--no_unroll` exits **1**, `C3900U: Unrecognized option '--no_unroll'`.
`--loop_optimization_level=0` and `=1` are accepted compiler options. Level 0 is not a solution
for the shared game profile: it breaks the exact `0x105fa8` witness. Level 1 is indistinguishable
from default O3 on these probes, so adding it would be an unsupported extra spelling.

`0x1033cc` is **not** claimed matched. Local-pointer, global-pointer, C++, and typed-resource
variants remain negative (`optimization_initial_compid.json`, `loop_alloc_*_grid_compid.json`).
The local-pointer O3 candidate is 112 bytes against an 88-byte target; its O2 and O3/loop0
variants are 84 bytes and still differ. Thus “original not unrolled” alone was misleading:
four other plausible loop sources select O3, and globally disabling the loop optimizer fails.
The remaining `0x1033cc` discrepancy is a source-shape/per-function-setting question, not
positive evidence for choosing O2 globally.

## C++ and C++-specific switches

`game_heap.cpp` declares an actual polymorphic `Heap` class with a virtual destructor and `allocate`;
the factory calls it and then conditionally tail-calls the object constructor/helper.
It exactly reproduces `0x10d604`, including the vtable slot, argument registers, null check,
saved registers, indirect call and final tail call. No fake vtable byte array or assembly is used.
The destructor's two ABI slots naturally give the observed allocation slot; no invented reserved
virtual methods are needed. Earlier `game_virtual.cpp` records are retained as a checkpoint, while
the final twelve-cell grid and table use this natural destructor-based class.
This is an exact C++ class/virtual-method witness for the game's C++ object model.

`cpp_grid.json` forces `--cpp` and compares the following six 5.04 O3/time cells:

| Additional C++ controls | `0x10d604` |
|---|---|
| None beyond `--cpp` | Y |
| `--no_rtti` | Y |
| `--no_exceptions` | Y |
| `--force_new_nothrow` | Y |
| `--no_vfe` | Y |
| All four controls together | Y |

Evidence: `heap_cpp_options_compid.json`. These options are accepted; they are **not discriminated**
by this call site. It does not emit a class vtable/typeinfo definition, use `dynamic_cast`/`typeid`,
throw/catch, or perform ordinary `new`. Its direct virtual allocation call is not a test of the
semantics of the standard allocation expression. Selecting all four options just because they
are familiar embedded defaults would overstate the evidence.

Additional [INFERENCE] favoring RTTI suppression: constructor-installed vtables at `0x223e2c`,
`0x223ee8`, `0x223ef8`, `0x223f08`, and the font table at `0x224d30` have two null predecessor
words under the ARM C++ ABI header interpretation. A restricted scan for zero offset-to-top plus
three initial function-pointer slots found 26 null-typeinfo candidates and no nonzero data-pointer
typeinfo candidates; the other 55 apparent candidates had a preceding **function** pointer and
are likely table interiors. This is a heuristic, not a complete vtable inventory, and does not
uniquely distinguish `--no_rtti`, `--no_rtti_data`, or link-time removal. Derived counts and method
are recorded in `analysis_final.json`; original table contents were not exported.

`0x10c498` remains a near match, not a MATCH: the direct C++ local-variable source has the same
92-byte extent but different ordering of its two incoming-float saves. All six C++ option cells
remain `+c` (`virtual_dimensions_cpp_options_compid.json`). Vector-constructor and local-declaration
variants also remain negative. Toggling the requested switches did not repair this source shape.

## Library components: addresses, idioms, and measured compatibility

Addresses are **component witnesses, not blanket contiguous ownership ranges**. SDK inlining and
link layout mix components; assigning every intervening function to one vendor would be unjustified.
The input's recorded tags are CTR-SDK 11.5.2, NW4C 3.7.5, and PUX HWR 2.0.1; no licensed vendor
archive or known-library binary was available for independent byte identification.

### CTR-SDK/service ABI

Strong service-wrapper witnesses include `0x100d6c` (ExitProcess), `0x1109b4` (CreateAddressArbiter),
`0x1109cc` (QueryMemory), `0x12d0ec` (ConnectToPort), `0x12d148` (ExitThread), `0x12d150`
(GetProcessId), and `0x134e7c` (Break). Their service numbers and out-register-to-pointer marshaling
agree with the public [3dbrew SVC reference](https://www.3dbrew.org/wiki/SVC).
CP15 thread-local IPC-buffer access plus SendSyncRequest is also present, for example at `0x10bc9c`.

Three small wrappers were probed with real ARMCC `__svc` intrinsics, not inline assembly:
`0x100d6c`, `0x12d148`, `0x134e7c`, source `library_ctr_wrappers.c`.
All **16** library-grid cells fail at `+0`: the tested compiler emits a stack frame while the
original stubs do not. Evidence: `ctr_wrappers_grid_compid.json`. This is consistent with
handwritten SDK service stubs [INFERENCE]; it does not identify a different ARMCC version, and
these wrappers are not C-credit candidates. No fabricated compiler profile or no-frame assembly
substitute is proposed.

`library_ctr.c`/`library_grid_compid.json` retain the initial exploratory probe, whose provisional
name/signature for service 0x09 was wrong; it is superseded for ABI interpretation by the corrected
`library_ctr_wrappers.c` and its separate grid. Both are negative, and neither is credited.

### Likely NW4C font/layout

The font parser at `0x1db144` recognizes CFNT/CFNU and FINF/CMAP records; its validation helper is
also used by the CLYT layout parser at `0x1ce9e8`. This connects the selected font component to the
layout family, rather than attributing a library solely from a generic getter. The other font
parser at `0x19da14` is a distinct related component and was not silently merged with it.

Three compact witnesses in `library_nw4c_readers.c` all directly MATCH under the game flag list:

| Address | Feature | 5.04 O3/time |
|---|---|---|
| `0x1db054` | Read unsigned 16-bit character, then advance pointer | MATCH |
| `0x1db068` | Read unsigned byte, then advance pointer | MATCH |
| `0x1db3ec` | Base constructor/helper call followed by vtable installation | MATCH |

`nw4c_readers_grid_compid.json`: all three are Y for **all four tested builds** at O2/time,
O3/time and O3/space. O0 cells fail closed because enlarged adjacent candidate extents overlap;
these are recorded failures, not non-comparable successes. The matches demonstrate compatibility
with the game reproducer, **not** a uniquely identified NW4C compiler or flags. The initial
post-increment-expression version in `library_nw4c.c` was negative; the final reader explicitly
sequences the read before pointer update, which is a meaningful aliasing/ordering distinction.

### Probable PUX HWR recognition component [INFERENCE]

Best candidates are the state-management and short-coordinate component anchored by
`0x14c338`, `0x14db18`, `0x14db3c`, `0x14dc28`, `0x14dc44`, `0x156de8`, `0x157fe8`,
`0x1582ac`, `0x15c5a4`, and `0x15cab4`. Evidence includes shared state at
`data_00243830` through `data_00243890`, buffers near `bss_004e07f0`, 16-bit coordinate/record
processing, 0xffff sentinel chains, candidate-result copying, and integer normalization kernels.
Those idioms plus the known PUX HWR tag make this a likely recognition component, but the vendor
attribution is **not authenticated** by a source path, symbol, or original library archive.
Other code is interleaved in this broad address neighborhood.

Three compact witnesses in `library_pux.c` directly MATCH under the game flag list:

| Address | Feature | 5.04 O3/time |
|---|---|---|
| `0x14dc28` | Select static recognition-state buffer; return success | MATCH |
| `0x15c5a4` | Store option into global recognition state | MATCH |
| `0x15cab4` | Clear five short state fields in observed semantic order | MATCH |

`library_grid_compid.json`: all three are Y for each tested build at O2/time and O3/time.
At O3/space, 4.0 matches all three, whereas the 4.1/5.04 cells differ in `0x15cab4` at `+1`.
This is insufficient to distinguish 4.0/space from later/time builds. Do not freeze a separate
PUX compiler profile on these leaves or classify the whole component as a proven library match.

### Self-move before return

The recurring self-move tail is present at witnesses such as `0x125b50`, `0x13714c`, `0x18f7ec`,
`0x1c5b84`, and `0x1c6ad8`; other packets show the same idiom inside routines before another body.
`library_nop.c` probes an identity return and two empty hooks without inserting an instruction.
All 16 tested build/optimization cells differ (`nop_grid_compid.json`), including O0. The idiom
therefore does **not** justify assigning these functions to RVCT 4.0, O0, or a vendor. Explicit
source intrinsics, macro expansion, other options, and catalogue boundaries remain possibilities.
No no-op was injected to manufacture a match.

## Thumb coverage

The catalogue used for the main matches has 71 Thumb entries, 47 in the startup/runtime block
below `0x100d74`. Inspected outliers include a UTF-16 length loop (`0x19d5b0`), a guard-like helper
(`0x191c80`), runtime state accessors (`0x181e94`, `0x181e9e`), and internal tail entries around
`0x19f9a0`. No game-owned Thumb body was established; this is not proof that none exists.

`runtime_thumb_get.c` exactly reproduces `0x181e94` under all nine cells in `thumb_grid.json`
(`thumb_get_grid_compid.json`). The grid uses `--thumb` in place of `--arm`, otherwise the same
base controls, for 4.0 b821 / 4.1 b1049 / 5.04 b82 and O2/time / O3/time / O3/space.
This proves the tool/relocation path can reproduce a real Thumb body but cannot identify its
original compiler. The broader `runtime_thumb.c` records three additional negative source shapes;
none was counted as a match. The Thumb result is supplementary and not part of the 11-function ARM
match total.

For optional reproduction after adding a *candidate*, not historically identified, profile:

```json
"runtime-thumb-candidate": {
  "compiler": "armcc_504_82",
  "flags": [
    "--cpu=MPCore", "--fpmode=fast", "--apcs=/interwork", "--thumb",
    "--split_sections", "--signed_chars", "-O3", "-Otime"
  ]
}
```

## Reproduction and evidence handling

Executed final matrix (exit **0**; complete output in `scratch/compid/r2/final-profile-grid.log`):

```sh
.venv/bin/python tools/compid.py research/compid/r2/game_core.c research/compid/r2/loop_cleanup.c research/compid/r2/loop_state.c research/compid/r2/loop_resources.c research/compid/r2/game_heap.cpp research/compid/r2/game_enum_switch.c --grid research/compid/r2/final_grid.json --out scratch/compid/r2/final-profile-grid
```

Executed one `match.py` command per source, all six exit **0**. For example:

```sh
.venv/bin/python tools/match.py research/compid/r2/game_heap.cpp --profile trial-504-82-O3-Otime --out scratch/compid/r2/final-match/game_heap --show 0
```

Once Main adds `game`, all six listed sources can be reproduced with:

```sh
.venv/bin/python tools/match.py research/compid/r2/<source> --profile game --out scratch/compid/r2/reproduce/<source-stem> --show 0
```

Use fresh output directories rather than overwriting the sealed records. Equivalent direct
commands for both three-function library sources also exited **0**; receipts are
`library_nw4c_readers_match.json` and `library_pux_match.json`.

`commands_final.json` records the exact argv, working directory, exit code, duration and private output
log path for all 199 recorded commands, including packet creation, negative experiments and the
help/unsupported-option queries. Each copied `*_compid.json` is the original output record, not a
reformatted success-only summary. Source hashes bind the unchanged text tested across each grid.
Object files, original disassembly, packets and diff logs remain under `scratch/` only.

One oracle prerequisite was fixed by Main during this study: ARMCC's local zero-size
`__switch$$` marker was being treated as an independently named function. The initial float-switch
grid failed closed for that reason; Main's fix permits the genuine switch experiments. The final
switch MATCH results use the corrected oracle, whose hash is recorded in `analysis_final.json`.
The float-switch source at `0x16bf90` remains a real mismatch and is not counted.

All compiles were sequential. Measured scratch usage at the end of compilation was 2,684 KiB for
`scratch/compid/r2` and 2,132 KiB for the shared packet directory, well below the 200 MB budget.
No `tools/build.py`, full test suite, git operation, deletion, publication or shared configuration
edit was performed by this worker.

## Provenance and remaining limits

- Archive ID: **`armcc_504_82`**; measured banner **`Tool: armcc [5040081]`**, not an invented b82
  banner. Executable SHA-256: `198f346b4cdf64831eafe1c379fbbbd6e28d21731f839994118b0eb76106394d`.
- Compiler archive: [decomp.me compiler release](https://github.com/decompme/compilers/releases/download/compilers/armcc.zip?2026-03-04),
  SHA-256 `bda0cea5b13a9c3f08322fa76f9f9dcda59bda290f57f1a06e05cd80044fafdb`;
  provenance/license caveat retained from `toolchain.lock.json`. No compiler or SDK files exported.
- Input `code.bin` SHA-256: `a367b5dcd65e3924c93cf3fbaee5d4d40f9c5205d1287912784a974d5e405361`.
- Catalogue bound by direct-match receipts:
  `06027f34eb7e89666c138293a31393cbafc90d9c66862add8f3d9df571aedf13`.
- Still open: exact original RTTI/exception/new/VFE controls, per-function controls/source shape at
  `0x1033cc`, float-save ordering at `0x10c498`, source shape at `0x16bf90`, authenticated vendor
  library/compiler provenance, interpretation of the self-move tails, and complete ownership of
  the remaining Thumb entries. None is silently turned into a MATCH or a blanket classification.
- H5's reproducible code-generation recommendation is supported by the recorded multi-feature
  matrix. Original build-command recovery, vendor-library certification, independent review,
  accepted receipts and whole-image reproduction are separate claims and were not performed here.
