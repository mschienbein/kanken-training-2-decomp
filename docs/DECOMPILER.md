# Private Ghidra pseudo-C author aid

`tools/decompile.py` supplements `tools/packet.py`, especially for larger functions. It is
**not** a compiler, match oracle, ABI/type authority, or source suitable for direct acceptance.
The output reconstructs expressions, loops, switches and field accesses; an author still needs
to recover real types, call signatures and source spelling, then use `match.py` and the normal
interface/review/checkpoint gates. No hosted service is contacted by the wrapper.

## Install without administrator privileges

The tested Linux x86-64/WSL versions are [Ghidra 12.1.4](https://github.com/NationalSecurityAgency/ghidra/releases/tag/Ghidra_12.1.4_build)
and [Eclipse Temurin JDK 21.0.12.1+1](https://github.com/adoptium/temurin21-binaries/releases/tag/jdk-21.0.12.1%2B1).
Ghidra is Apache-2.0; the JDK is GPLv2 with the Classpath Exception (individual bundled
components have their own notices). They are separately installed, not vendored or downloaded
by this tool. Ghidra includes its native Linux decompiler binary; no compiler build or GUI is
needed. See the upstream [runtime requirements](https://github.com/NationalSecurityAgency/ghidra/blob/Ghidra_12.1.4_build/GhidraDocs/GettingStarted.md).

Extract the published archives under `~/.local/opt/`, preserving executable permissions:

- `ghidra_12.1.4_PUBLIC_20260921.zip`: SHA-256
  `ddac49f903da9d5bac833e5cc79395098b9c33cfd3279be5f31bd00387d2d4db`.
- `OpenJDK21U-jdk_x64_linux_hotspot_21.0.12.1_1.tar.gz`: SHA-256
  `ce79869e1307ed8ee1e2baa86a412b1eb5b75d10a01006d788a6f968bcfaee94`.

The default directories are `~/.local/opt/ghidra_12.1.4_PUBLIC` and
`~/.local/opt/jdk-21.0.12.1+1`. For other installation locations set `GHIDRA_HOME` and
`JAVA_HOME`, or pass `--ghidra PATH --java-home PATH`. There are no extra Python dependencies
beyond the repository environment. The wrapper uses POSIX advisory locks and process groups;
Linux/WSL is supported, native Windows is not.

## Author CLI

From the repository root:

```sh
.venv/bin/python tools/decompile.py func_00190878
.venv/bin/python tools/decompile.py func_00184808 --seconds 180 --function-seconds 30
```

The single stdout line is the private pseudo-C path, **not the pseudo-C itself**. Read it locally
alongside the function's packet; do not publish it, attach it to a public scratch site, or commit
it. Exit status is **0** for a complete cached/generated result and **2** for invalid input,
missing installation, timeout, or headless/script failure. Names-overlay function names are
accepted as well as `func_XXXXXXXX`; classified veneers/multi-entry entries are refused as author
targets rather than represented as ordinary C functions.

Each result has a sibling JSON receipt with the input/tool fingerprint, source SHA-256,
decompiler and headless timings, basic-block count, completion flag and private warnings.
Generated `.c` files contain inferred types such as `undefined4`, guessed parameter counts and
raw offset accesses. They are not expected to compile. Ghidra does not recover C++ class layouts
or trustworthy prototypes from these catalogue seeds alone. A completed decompilation is not a
correctness proof; warnings and residual gotos require review.

## Parallel agents and bounded work

All worktrees whose `orig/` resolves to the same private dump share
`<dump-parent>/scratch/decompile/<fingerprint>/`. The fingerprint includes the verified image,
segment mapping, effective catalogue code ranges/ISA/names, Ghidra version properties and Java
script hash. Changed analysis inputs get a separate epoch/project; old output is never reused
across those inputs. Per-function `.c` plus receipt files are published by atomic rename.

- Cache hits validate the receipt/source hash and do not take the project lock or start Java.
- A miss takes one bounded advisory lock for the epoch. The first caller imports/decodes the
  catalogue **once** into one local Ghidra project; subsequent misses open it read-only.
- Concurrent misses queue rather than creating one JVM/project per agent. Waiting callers
  recheck the cache after locking, so duplicate requests do not repeat work.
- Default whole-call budget: 600 seconds including lock wait/startup/import/decompilation.
  Default per-function native decompiler budget: 60 seconds. `--seconds` and
  `--function-seconds` must be positive; the wall budget can terminate the native work earlier.
- Each JVM is limited to a 2 GiB Java heap and two active processors. A wall timeout kills the
  headless JVM/native-decompiler process group. This is not a hard OS resident-memory limit.
- Failed scripts cannot become cache hits, even if Ghidra exits zero. Temporary output is
  removed; the complete stdout/stderr and Ghidra logs remain private beside the result.

The script maps text/rodata/data by their exheader addresses and actual file offsets, adds
uninitialized BSS, applies each function's ARM/Thumb context and excludes catalogue literal
pools before bounded disassembly. It deliberately does **not** run heuristic whole-image auto
analysis or invent new function boundaries. Read-only text/rodata constants are folded, including
literal-pool loads. The catalogue remains authoritative and is never edited.

To discard a broken project, stop its users and remove only that fingerprint's directory under
`scratch/decompile/`; the next caller imports it again. Other analysis epochs remain untouched.
The tool never edits `src/`, `include/`, `config/`, packets, or candidate sources.

## Focused tests

```sh
.venv/bin/python -m unittest tests.test_decompile
DECOMPILE_INTEGRATION=1 .venv/bin/python -m unittest tests.test_decompile
```

The default tests use synthetic ARM/Thumb inputs and a fake headless process. The opt-in test
also launches the real installed Ghidra on **synthetic** ARM/Thumb functions, verifies generated
expressions and shared-project reuse, and requires no ROM. Tests cover fingerprints, literal
exclusion, discontiguous segment mapping, private paths, cache integrity, concurrent callers,
bounded lock/process timeouts and zero-exit script failures. Temporary artifacts remain under
`scratch/`. No full build or full suite is needed for this optional tool.
