# Candidate catalogue revision v3 — 2026-10-16

## Status and deliverables

**Complete candidate, not adopted; no canonical catalogue/history or credit was changed.**
Runtime identity: family **OpenAI**, model **openai-codex/gpt-6.1-sol**.

- Edited sources: `scratch/worktrees/catalogue-v3/tools/analyze.py` and
  `scratch/worktrees/catalogue-v3/tests/test_analyze.py` in the private worktree.
- Candidate: `scratch/analyze/catalogue-v3.json` in the main checkout.
- Report: `scratch/analyze/REPORT-v3.md` in the main checkout.
- Canonical baseline: adopted v2, SHA-256
  `65d1c56942a759901f27311e8fe722ed2f67c0a5b09b1f2e69f58225d3374e89`.
- Input: `orig/code.bin`, 1,327,104 mapped bytes, SHA-256
  `a367b5dcd65e3924c93cf3fbaee5d4d40f9c5205d1287912784a974d5e405361`; exheader-derived layout; Capstone 5.0.7.
- The public `build_catalogue`, `validate_catalogue`, `serialize_catalogue` signatures
  and schema version 1 are unchanged. New metadata is additive, except for the
  explicitly requested non-C grouping/classification and data-ownership correction.

### Isolation / G0 disposition

The integrator reported that its checkpoint suite encountered the initially
in-progress shared `tools/analyze.py` edit. That reported failure was accepted as
ground truth, not re-run to confirm. At its explicit request I created the private
worktree, copied my two files there, restored **only those two files** in the main
checkout, and linked the private inputs. The complete authorized command below
exited **0**; its final `git status --short` produced **no entries**, and Main was
notified immediately. Worktree HEAD was detached at **843b1f0**. Git warned that
clonefile fell back to a plain checkout; the operation succeeded.

All subsequent source edits and test execution used that worktree and the **main
checkout's interpreter**, with `orig` symlinked rather than copied. Only the assigned
scratch candidate/report were written back to the main checkout. There were no
later tracked-source edits there, no commits, no publication, no deletion, no build,
no full-repository test suite, and no formatter run by this worker.

```sh
git -C /Users/mooki/Code/kanken-training-2-decomp worktree add scratch/worktrees/catalogue-v3 HEAD && cp /Users/mooki/Code/kanken-training-2-decomp/tools/analyze.py /Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3/tools/analyze.py && cp /Users/mooki/Code/kanken-training-2-decomp/tests/test_analyze.py /Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3/tests/test_analyze.py && git -C /Users/mooki/Code/kanken-training-2-decomp checkout -- tools/analyze.py tests/test_analyze.py && ln -s ../../../orig /Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3/orig && git -C /Users/mooki/Code/kanken-training-2-decomp status --short
```

## Executive results

| Measure | Adopted v2 | v3 candidate | Change |
| --- | ---: | ---: | ---: |
| Top-level routine owners (`functions`) | 4,443 | 4,351 | -92 grouped, not lost entries |
| Declared callable entries (v2 starts implicit) | 4,443 | 4,472 | +29 interior entries |
| Ordinary C-eligible candidates (`kind == function`) | 4,437 | 4,231 | -206 |
| Non-C `multi_entry` owners | 0 | 114 | +114 |
| Linker veneers | 6 | 6 | 0 |
| Direct BL/BLX sites | 19,991 | 19,991 | 0 |
| Direct B sites | 26,271 | 26,271 | 0 |
| Decoded instructions | 264,698 | 264,698 | 0 |
| Instruction bytes | 1,054,474 B | 1,054,474 B | 0 B |
| Word-aligned pool/table bytes | 48,536 B | 39,968 B | -8,568 B |
| Separately recorded literal bytes | not separate | 40,858 B | includes retyping |
| Explained function-owned bytes | 1,103,010 B | 1,135,300 B | +32,290 B |
| Unknown text bytes | 39,338 B | 8,836 B | -30,502 B |
| Unknown ranges | 2,224 | 194 | -2,030 |
| Padding bytes | 1,960 B | 172 B | -1,788 B |
| Sum of routine extents | 1,136,402 B | 1,136,786 B | +384 B |
| C-eligible extent denominator | 1,136,354 B | 1,108,426 B | -27,928 B |
| Semantic anomalies | 197 | 82 | -115 |

The C-eligible decrease is **classification/denominator correction, not C matching
progress**. All 114 multi-entry owners are non-creditable. Their combined extents
are 28,312 B; six veneers account for another 48 B. No match credit was created.

All **4,443 v2 entry addresses survive** in v3, in the correct ISA. Exactly 92 move
from top-level records to explicit entries of a containing non-C group. There are
121 secondary entries: those 92 former starts plus **29 newly explicit interior
entry addresses**. Entry records comprise 4,399 ARM and 73 Thumb addresses; top-level
owners comprise 4,282 ARM and 69 Thumb records. Grouping did not change an entry's ISA.

`validate_catalogue` returns **[]** on the final v3 candidate, and also **[]** on the
legacy v2 input. The original and candidate sets of `(site, target, ISA)` are
identical for **all 19,991 direct calls and all 26,271 direct branches**. There are
zero newly decoded, lost or added direct-call/branch sites. No final extent overlaps
the next top-level start, and ranges still tile every one of the 1,327,104 mapped
bytes exactly once. Data (51,464 B), rodata (123,064 B) and segment padding (8,268 B)
remain unchanged. Switches (142), indirect exits (155) and noreturn owners (5) are unchanged.

## 1. Exclusive in-section literals

### Rules and representation

- Exact ARM ADD/SUB-PC and Thumb ADR/immediate-PC targets are retained as byte
  addresses, not just their rounded first word. Thumb register ADD-PC is resolved
  only with an adjacent decoded constant producer and no branch/entry into ADD;
  unknown register offsets are not guessed.
- A NUL-terminated printable UTF-8 or CP932 string (including tab/CR/LF and an empty
  string) is bounded through its terminator, then padded to a four-byte **end**.
  A literal's start may be unaligned; preceding bytes are not silently claimed.
- `functions[].literals` is a sorted, merged, nonoverlapping list of byte intervals,
  disjoint from instruction ranges and actual word-aligned `pools`. PC-relative
  address-only word prefixes no longer masquerade as complete pool objects.
- Recognition covers both appended strings and strings between separately decoded
  code spans. This is necessary to explain the previously unknown interiors of
  large recovered sections; it does not decode data as code or move a start.
- Other exclusively address-taken data keeps only the previously evidenced word
  span, not a guessed array length. There are **13 such address-target objects**;
  any unproved remainder stays unknown. No claim is made to have inferred unknown
  binary-array types or sizes.
- Ownership is checked against other functions' exact address references, referenced
  pool words/pointers, direct transfers and rodata/data pointer words. Suffix-overlap
  references count as sharing. Shared objects are withheld from both pools/literals;
  their NUL/zero padding is kept **unknown**, not relabelled as padding.
- A proposed span beyond the next top-level owner is rejected with
  `address_literal_crosses_start`. Code overlap and shared ownership get separate
  anomaly reasons and address-only `literal_conflicts` records. No boundary is
  crossed to make a string fit.

The result is **40,858 literal bytes in 99 owners**, with **2,115 complete string
address-target objects** and 13 bounded addressed-word objects. Distinct suffix
addresses can refer into the same underlying string, so this object count is not
a claim about original C source declarations. Of these bytes, 8,518 were already
function-owned first-word prefixes in v2 and are now correctly typed as literals.

### Reported six-string function

`0x1932E8` changes from **284 B to 300 B**, ending at **`0x193414`** rather than inside
its sixth string. Its code ranges remain unchanged; its true pool is
`[0x19331C,0x193334)`, and literals are `[0x193390,0x193414)` (132 B).
The 16-byte extent growth explains **108 formerly unknown bytes**, including
interior string holes; the final 300-byte extent has no unexplained hole.
No original string contents are reproduced here.

| PC-relative reference | String address | End including NUL | Padded end | Bytes including NUL |
| --- | --- | --- | --- | ---: |
| `0x193334` | `0x193390` | `0x19339E` | `0x1933A0` | 14 |
| `0x19333C` | `0x1933A0` | `0x1933B3` | `0x1933B4` | 19 |
| `0x193344` | `0x1933B4` | `0x1933CF` | `0x1933D0` | 27 |
| `0x19334C` | `0x1933D0` | `0x1933EA` | `0x1933EC` | 26 |
| `0x193354` | `0x1933EC` | `0x193400` | `0x193400` | 20 |
| `0x19335C` | `0x193400` | `0x193414` | `0x193414` | 20 |

The additional author report for `0x11FA38` is also covered: its existing **616 B**
extent is unchanged, with literals `[0x11FC80,0x11FC9C)` and its real pool
`[0x11FC9C,0x11FCA0)`. The former first-word-only pathname typing is gone.

### Exact ownership transfers

| Transfer | Bytes |
| --- | ---: |
| Unknown → function-owned data | 30,552 |
| Padding → function-owned data | 1,788 |
| Former function ownership → unknown (shared/conflicted/code-address prefixes) | 50 |
| Existing first-word pool prefixes → literal typing, same owner | 8,518 |
| Function-owned bytes moved between owners solely by the explicit fallthrough groups below | 13,072 |
| Literal bytes stolen from an unrelated next function | **0** |
| Unexplained function-owner reassignment | **0** |

Thus positive new ownership is 32,340 B, minus 50 B withdrawn = **32,290 B net**;
unknown decreases by 30,552 - 50 = **30,502 B**. The 13,072 B of owner reassignment
is precisely accounted for by `entry_groups`: old adjacent records now have one
non-C owner, and every old start is retained as an entry. It is not string-based
ownership theft.

### Literal-only extent changes (complete list)

There are **34 literal-grown owners (+416 B)** and **six owners that shrink (-32 B)**
after shared/code-address first-word claims are removed. These forty are separate
from the 92 owner enlargements caused solely by grouping. Overall, 126 surviving
top-level records grow and six shrink, while 92 other records become entries;
joining adjacent extents does not itself change the total extent byte sum.

| Address | v2 bytes | v3 bytes | Change |
| --- | ---: | ---: | ---: |
| `0x100078` | 20 | 16 | -4 |
| `0x104458` | 780 | 788 | +8 |
| `0x106EC4` | 56 | 60 | +4 |
| `0x107288` | 192 | 208 | +16 |
| `0x10DE68` | 528 | 556 | +28 |
| `0x112FB8` | 216 | 232 | +16 |
| `0x1178D4` | 212 | 224 | +12 |
| `0x117BB4` | 212 | 224 | +12 |
| `0x12D6E4` | 156 | 164 | +8 |
| `0x136510` | 204 | 208 | +4 |
| `0x1932E8` | 284 | 300 | +16 |
| `0x196564` | 56 | 64 | +8 |
| `0x1965A4` | 56 | 64 | +8 |
| `0x196E14` | 904 | 908 | +4 |
| `0x19F634` | 16 | 12 | -4 |
| `0x1B5374` | 372 | 368 | -4 |
| `0x1B8198` | 160 | 156 | -4 |
| `0x1BC220` | 128 | 124 | -4 |
| `0x1CD070` | 640 | 628 | -12 |
| `0x1F59B8` | 9140 | 9156 | +16 |
| `0x202CA8` | 2908 | 2920 | +12 |
| `0x205380` | 1580 | 1592 | +12 |
| `0x206AFC` | 1900 | 1912 | +12 |
| `0x207274` | 380 | 392 | +12 |
| `0x209BCC` | 752 | 764 | +12 |
| `0x20E038` | 596 | 604 | +8 |
| `0x20F858` | 416 | 432 | +16 |
| `0x210084` | 548 | 560 | +12 |
| `0x2102BC` | 852 | 868 | +16 |
| `0x210620` | 516 | 532 | +16 |
| `0x2122F4` | 308 | 320 | +12 |
| `0x2126F0` | 372 | 388 | +16 |
| `0x212874` | 360 | 368 | +8 |
| `0x212B60` | 1488 | 1504 | +16 |
| `0x2131B4` | 1128 | 1136 | +8 |
| `0x213798` | 1432 | 1444 | +12 |
| `0x213D3C` | 1004 | 1020 | +16 |
| `0x214468` | 264 | 280 | +16 |
| `0x2145F4` | 404 | 416 | +12 |
| `0x2153F4` | 1216 | 1228 | +12 |

## 2. Four-byte NOP starts — finding (a)

The literal-only candidate contains **21 exact four-byte ARM NOP owners**, all
falling directly into a start at `address + 4`. Their incoming evidence includes
**13 with observed BL callers**, seven with pointer evidence, and one tail-only
entry. They cannot safely be deleted or called padding. They are callable prefixes
of shared code in the recovered graph.

V3 keeps those starts and the following starts as entries of one **non-creditable
`multi_entry`** owner. A prefix entry with a known entry exactly four bytes later
is tagged `entries[].kind == nop_entry`. There are **28 such records** in total:
the 21 former four-byte owners plus seven already-longer v2 owners with separately
entered bodies four bytes after their NOP prefixes. All 28 prefix addresses were
already v2 starts; no NOP bytes are newly decoded or fabricated.

Example: `0x1852F4` (four-byte NOP prefix) and `0x1852F8` (64-byte body) are now one
**68-byte `multi_entry` owner at `0x1852F4`**, with both entries retained. The body
at `0x1852F8` remains a valid call target but is not independently creditable C.
The same treatment covers the author's `0x136858` → `0x13685C` finding.

Whether the original producer deliberately emitted hot-patch/prefix labels,
handwritten/library entry aliases, or whether an earlier boundary/decoder guess
was wrong is **[INFERENCE]**; no SDK/compiler identity is claimed. The observed
NOP, incoming edges and fallthrough are the classification evidence.

| NOP entry | Containing owner | Previous owner bytes | Entry evidence | Direct source sites |
| --- | --- | ---: | --- | ---: |
| `0x12D5C0` | `0x12D5C0` | 108 | bl_target | 1 |
| `0x136858` | `0x136858` | 4 | bl_target | 1 |
| `0x1378D4` | `0x1378D4` | 176 | bl_target | 14 |
| `0x141C70` | `0x141C70` | 4 | pointer | 0 |
| `0x144220` | `0x144220` | 8 | bl_target | 1 |
| `0x166EAC` | `0x166EAC` | 112 | bl_target | 3 |
| `0x166F1C` | `0x166F1C` | 4 | bl_target+tail_call | 43 |
| `0x169874` | `0x169874` | 4 | bl_target | 1 |
| `0x16F39C` | `0x16F39C` | 336 | bl_target | 1 |
| `0x17E358` | `0x17E358` | 4 | pointer | 0 |
| `0x181E40` | `0x181E40` | 4 | bl_target | 1 |
| `0x182718` | `0x182718` | 4 | bl_target | 1 |
| `0x183030` | `0x183030` | 4 | bl_target | 3 |
| `0x1852F4` | `0x1852F4` | 4 | bl_target+tail_call | 216 |
| `0x185B9C` | `0x185B9C` | 4 | pointer | 0 |
| `0x186484` | `0x186484` | 4 | bl_target | 2 |
| `0x18F7EC` | `0x18F7EC` | 8 | bl_target | 1 |
| `0x19721C` | `0x19721C` | 4 | bl_target | 1 |
| `0x19DC1C` | `0x19DC1C` | 4 | tail_call | 3 |
| `0x19EB48` | `0x19EB48` | 4 | bl_target | 21 |
| `0x19F2E4` | `0x19F2E4` | 4 | bl_target | 1 |
| `0x19FFA4` | `0x19FFA4` | 4 | bl_target | 2316 |
| `0x1C3ABC` | `0x1C3ABC` | 4 | bl_target | 1 |
| `0x1D54AC` | `0x1D54AC` | 4 | pointer | 0 |
| `0x1DACBC` | `0x1DACBC` | 4 | pointer | 0 |
| `0x1DADC0` | `0x1DADC0` | 4 | pointer | 0 |
| `0x1DB130` | `0x1DB130` | 4 | pointer+tail_call | 1 |
| `0x1E9818` | `0x1E9818` | 300 | bl_target | 23 |

## 3. Missing B targets and multi-entry routines — findings (b), (c)

### Outside-extent targets

The real image has **zero decoded direct-B targets outside every final extent**;
the scoped literal-only candidate also had zero. There are **zero undeclared
external direct-branch entries** after the final entry pass. The tool now enforces
this rather than relying on discovery luck: a decodable direct transfer beyond all
extents is promoted to a start (`tail_call` for B), then traces/ownership are rebuilt
until stable. The regression where an address-word claim initially hides a BX-return
target exercises the repair path; no repair was necessary on this actual image.

The two reported four-byte wrappers `0x11FA34` and `0x11FFDC` do target previously
uncatalogued **entry** `0x18F7F0`, but that instruction is already decoded inside the
existing eight-byte owner `0x18F7EC`. V3 therefore records the real secondary entry
at `0x18F7F0`, with both source addresses, instead of inventing overlapping owners.
The wrapper code is unchanged.

### Callable interior labels and validation

Every function consistently has an `entries` list **including its primary entry**:

```text
{address, isa, kind, evidence, sources}
```

Only **external B edges** or BL/BLX targets create additional callable entries;
ordinary branches within the same containing routine remain local control-flow
labels, not thousands of fake function entries. Entry addresses must be actual
instruction boundaries in their owner's code, and entry ISA must agree with the
decoded instruction (including the existing linker veneer mixed-ISA exception).
An owner with multiple callable entries is `multi_entry`, unless already a classified
veneer. Arbitrary entry records cannot legalize invalid code, data targets, overlaps
or the wrong ISA: the synthetic tampering tests fail those cases.

The requested change deliberately refines the old call invariant from **top-level
start** to **declared start/entry in the correct ISA**. All BL/BLX targets are still
explicitly represented and decoded. Legacy schema-v1 catalogues without `entries`
continue to validate under their original top-level-start rule; no shim aliases or
silently invented external symbols are emitted.

`0x135054` → `0x12D1D8` is represented as an entry 12 bytes into the 240-byte
`multi_entry` owner `0x12D1CC`. The leading argument setup remains code, rather than
being padded or discarded. The complete set of **29 newly explicit interior entries**:

| New interior entry | Owner | ISA | Direct external sources |
| --- | --- | --- | --- |
| `0x11774C` | `0x1176E0` | arm | `0x108F8C` |
| `0x1273A0` | `0x127398` | arm | `0x1941AC` |
| `0x12D1D8` | `0x12D1CC` | arm | `0x135054` |
| `0x12D5C4` | `0x12D5C0` | arm | `0x12D4F0` |
| `0x1378D8` | `0x1378D4` | arm | `0x18F0AC` |
| `0x141560` | `0x141554` | arm | `0x1DB100` |
| `0x144224` | `0x144220` | arm | `0x13DB3C` |
| `0x152610` | `0x1525F0` | arm | `0x14FBE8` |
| `0x166EB0` | `0x166EAC` | arm | `0x179144`, `0x1EAF9C` |
| `0x167918` | `0x1678DC` | arm | `0x1E59CC` |
| `0x16BDE0` | `0x16BDA4` | arm | `0x1C3C28`, `0x1C3CFC` |
| `0x16F3A0` | `0x16F39C` | arm | `0x1B3A38` |
| `0x17DF84` | `0x17DF50` | arm | `0x152F08`, `0x1D5E38`, `0x1D8CD4` |
| `0x182280` | `0x18221C` | arm | `0x195D8C`, `0x19EAE4` |
| `0x1864E0` | `0x1864DC` | arm | `0x18CB58` |
| `0x18D8C4` | `0x18D894` | arm | `0x196C4C`, `0x196CA0` |
| `0x18F7F0` | `0x18F7EC` | arm | `0x11FA34`, `0x11FFDC` |
| `0x196520` | `0x19650C` | arm | `0x196508` |
| `0x19CED0` | `0x19CECC` | arm | `0x1864EC`, `0x186504` |
| `0x19F70C` | `0x19F6D8` | arm | `0x186480` |
| `0x19F734` | `0x19F6D8` | arm | `0x18647C` |
| `0x19F764` | `0x19F6D8` | arm | `0x186474` |
| `0x19F798` | `0x19F6D8` | arm | `0x18646C` |
| `0x19F7BC` | `0x19F6D8` | arm | `0x186464` |
| `0x19F7E0` | `0x19F6D8` | arm | `0x18645C` |
| `0x19F962` | `0x19F912` | thumb | `0x19FA4A`, `0x19FAA6` |
| `0x19F96A` | `0x19F912` | thumb | `0x19F9A8` |
| `0x1D4174` | `0x1D4124` | arm | `0x1D4508` |
| `0x1E981C` | `0x1E9818` | arm | `0x1736F8` |

### Fallthrough grouping

All **92** remaining v2 fallthrough boundaries are grouped into **92 two-start
non-C routines** (90 ARM groups and two Thumb groups; no incompatible-ISA group).
Every original member start is an explicit entry. These account for 92 of the
114 `multi_entry` owners; **22 more** retain a single former extent but have
external interior entries. There are no residual `fallthrough_into_start` records:
this is classification of incompatible-with-separate-C-section code, not a claim
that the old boundaries were independently matching C functions.

Examples requested by authors/integrator:

- `0x186874` (16 B) falls into `0x186884`: one **148 B** owner at `0x186874`.
- `0x1963E8` (16 B) falls into `0x1963F8`: one **176 B** owner at `0x1963E8`.
- `0x1CF418` (48 B) falls into `0x1CF448`: one **236 B** owner at `0x1CF418`.

All are `multi_entry`, never creditable `function`. The complete fallthrough group
list below is also the complete set of 92 v2 records absorbed into an earlier
owner: in every row the second member is no longer top-level but remains an entry.

| Group owner | Original members and sizes | Combined v3 extent |
| --- | --- | ---: |
| `0x121410` | `0x121410` (4 B) + `0x121414` (24 B) | 28 B |
| `0x12D0A4` | `0x12D0A4` (20 B) + `0x12D0B8` (12 B) | 32 B |
| `0x136858` | `0x136858` (4 B) + `0x13685C` (28 B) | 32 B |
| `0x141C70` | `0x141C70` (4 B) + `0x141C74` (100 B) | 104 B |
| `0x143328` | `0x143328` (40 B) + `0x143350` (224 B) | 264 B |
| `0x148210` | `0x148210` (16 B) + `0x148220` (228 B) | 244 B |
| `0x14E20C` | `0x14E20C` (12 B) + `0x14E218` (692 B) | 704 B |
| `0x1526B4` | `0x1526B4` (44 B) + `0x1526E0` (108 B) | 152 B |
| `0x15E5F0` | `0x15E5F0` (28 B) + `0x15E60C` (24 B) | 52 B |
| `0x166F1C` | `0x166F1C` (4 B) + `0x166F20` (224 B) | 228 B |
| `0x169874` | `0x169874` (4 B) + `0x169878` (124 B) | 128 B |
| `0x169920` | `0x169920` (12 B) + `0x16992C` (72 B) | 84 B |
| `0x16B248` | `0x16B248` (24 B) + `0x16B260` (28 B) | 52 B |
| `0x16B58C` | `0x16B58C` (112 B) + `0x16B5FC` (96 B) | 208 B |
| `0x16DAE8` | `0x16DAE8` (220 B) + `0x16DBC4` (140 B) | 360 B |
| `0x16E1C8` | `0x16E1C8` (8 B) + `0x16E1D0` (160 B) | 168 B |
| `0x16E5D0` | `0x16E5D0` (12 B) + `0x16E5DC` (64 B) | 76 B |
| `0x16E61C` | `0x16E61C` (44 B) + `0x16E648` (336 B) | 380 B |
| `0x16E798` | `0x16E798` (72 B) + `0x16E7E0` (148 B) | 220 B |
| `0x16F174` | `0x16F174` (148 B) + `0x16F208` (128 B) | 276 B |
| `0x170298` | `0x170298` (100 B) + `0x1702FC` (140 B) | 240 B |
| `0x170DB4` | `0x170DB4` (12 B) + `0x170DC0` (120 B) | 132 B |
| `0x170E50` | `0x170E50` (8 B) + `0x170E58` (200 B) | 208 B |
| `0x1719F4` | `0x1719F4` (64 B) + `0x171A34` (84 B) | 148 B |
| `0x1737C0` | `0x1737C0` (1484 B) + `0x173D8C` (4 B) | 1488 B |
| `0x177C58` | `0x177C58` (1132 B) + `0x1780C4` (128 B) | 1260 B |
| `0x178154` | `0x178154` (3396 B) + `0x178E98` (4 B) | 3400 B |
| `0x1799B4` | `0x1799B4` (148 B) + `0x179A48` (144 B) | 292 B |
| `0x17A518` | `0x17A518` (64 B) + `0x17A558` (88 B) | 152 B |
| `0x17A6F4` | `0x17A6F4` (40 B) + `0x17A71C` (200 B) | 240 B |
| `0x17A9AC` | `0x17A9AC` (8 B) + `0x17A9B4` (220 B) | 228 B |
| `0x17E358` | `0x17E358` (4 B) + `0x17E35C` (124 B) | 128 B |
| `0x17FE60` | `0x17FE60` (56 B) + `0x17FE98` (924 B) | 980 B |
| `0x180358` | `0x180358` (48 B) + `0x180388` (72 B) | 120 B |
| `0x18075C` | `0x18075C` (12 B) + `0x180768` (88 B) | 100 B |
| `0x180860` | `0x180860` (36 B) + `0x180884` (192 B) | 228 B |
| `0x180A4C` | `0x180A4C` (148 B) + `0x180AE0` (420 B) | 568 B |
| `0x1810E4` | `0x1810E4` (28 B) + `0x181100` (184 B) | 212 B |
| `0x1812F8` | `0x1812F8` (48 B) + `0x181328` (12 B) | 60 B |
| `0x181420` | `0x181420` (40 B) + `0x181448` (160 B) | 200 B |
| `0x181724` | `0x181724` (16 B) + `0x181734` (20 B) | 36 B |
| `0x181E40` | `0x181E40` (4 B) + `0x181E44` (64 B) | 68 B |
| `0x182718` | `0x182718` (4 B) + `0x18271C` (16 B) | 20 B |
| `0x18290C` | `0x18290C` (136 B) + `0x182994` (60 B) | 196 B |
| `0x183030` | `0x183030` (4 B) + `0x183034` (60 B) | 64 B |
| `0x18371C` | `0x18371C` (8 B) + `0x183724` (52 B) | 60 B |
| `0x1838EC` | `0x1838EC` (20 B) + `0x183900` (204 B) | 224 B |
| `0x183F08` | `0x183F08` (16 B) + `0x183F18` (12 B) | 28 B |
| `0x1852F4` | `0x1852F4` (4 B) + `0x1852F8` (64 B) | 68 B |
| `0x1855FC` | `0x1855FC` (220 B) + `0x1856D8` (140 B) | 360 B |
| `0x185B9C` | `0x185B9C` (4 B) + `0x185BA0` (16 B) | 20 B |
| `0x186484` | `0x186484` (4 B) + `0x186488` (12 B) | 16 B |
| `0x186874` | `0x186874` (16 B) + `0x186884` (132 B) | 148 B |
| `0x186FBC` | `0x186FBC` (12 B) + `0x186FC8` (540 B) | 552 B |
| `0x18D860` | `0x18D860` (8 B) + `0x18D868` (44 B) | 52 B |
| `0x18EEF0` | `0x18EEF0` (20 B) + `0x18EF04` (112 B) | 132 B |
| `0x18F634` | `0x18F634` (144 B) + `0x18F6C4` (4 B) | 148 B |
| `0x190634` | `0x190634` (8 B) + `0x19063C` (60 B) | 68 B |
| `0x191144` | `0x191144` (16 B) + `0x191154` (36 B) | 52 B |
| `0x1911F0` | `0x1911F0` (92 B) + `0x19124C` (100 B) | 192 B |
| `0x1913FC` | `0x1913FC` (92 B) + `0x191458` (36 B) | 128 B |
| `0x19198C` | `0x19198C` (352 B) + `0x191AEC` (28 B) | 380 B |
| `0x192410` | `0x192410` (28 B) + `0x19242C` (44 B) | 72 B |
| `0x1944E8` | `0x1944E8` (8 B) + `0x1944F0` (20 B) | 28 B |
| `0x1949F8` | `0x1949F8` (60 B) + `0x194A34` (804 B) | 864 B |
| `0x195168` | `0x195168` (116 B) + `0x1951DC` (120 B) | 236 B |
| `0x195A54` | `0x195A54` (124 B) + `0x195AD0` (156 B) | 280 B |
| `0x195CFC` | `0x195CFC` (120 B) + `0x195D74` (32 B) | 152 B |
| `0x195E5C` | `0x195E5C` (8 B) + `0x195E64` (32 B) | 40 B |
| `0x195EA4` | `0x195EA4` (32 B) + `0x195EC4` (28 B) | 60 B |
| `0x1963E8` | `0x1963E8` (16 B) + `0x1963F8` (160 B) | 176 B |
| `0x19721C` | `0x19721C` (4 B) + `0x197220` (456 B) | 460 B |
| `0x19C9B0` | `0x19C9B0` (36 B) + `0x19C9D4` (360 B) | 396 B |
| `0x19D788` | `0x19D788` (16 B) + `0x19D798` (300 B) | 316 B |
| `0x19DC1C` | `0x19DC1C` (4 B) + `0x19DC20` (224 B) | 228 B |
| `0x19EA64` | `0x19EA64` (8 B) + `0x19EA6C` (184 B) | 192 B |
| `0x19EB48` | `0x19EB48` (4 B) + `0x19EB4C` (20 B) | 24 B |
| `0x19EC64` | `0x19EC64` (36 B) + `0x19EC88` (112 B) | 148 B |
| `0x19EDBC` | `0x19EDBC` (16 B) + `0x19EDCC` (292 B) | 308 B |
| `0x19F2E4` | `0x19F2E4` (4 B) + `0x19F2E8` (60 B) | 64 B |
| `0x19F518` | `0x19F518` (80 B) + `0x19F568` (84 B) | 164 B |
| `0x19F912` | `0x19F912` (142 B) + `0x19F9A0` (4 B) | 146 B |
| `0x19F9AA` | `0x19F9AA` (2 B) + `0x19F9AC` (252 B) | 254 B |
| `0x19FFA4` | `0x19FFA4` (4 B) + `0x19FFA8` (100 B) | 104 B |
| `0x1C3ABC` | `0x1C3ABC` (4 B) + `0x1C3AC0` (400 B) | 404 B |
| `0x1CF418` | `0x1CF418` (48 B) + `0x1CF448` (188 B) | 236 B |
| `0x1D54AC` | `0x1D54AC` (4 B) + `0x1D54B0` (120 B) | 124 B |
| `0x1DAC54` | `0x1DAC54` (48 B) + `0x1DAC84` (56 B) | 104 B |
| `0x1DACBC` | `0x1DACBC` (4 B) + `0x1DACC0` (40 B) | 44 B |
| `0x1DACF8` | `0x1DACF8` (80 B) + `0x1DAD48` (120 B) | 200 B |
| `0x1DADC0` | `0x1DADC0` (4 B) + `0x1DADC4` (40 B) | 44 B |
| `0x1DB130` | `0x1DB130` (4 B) + `0x1DB134` (4 B) | 8 B |

### Consumer handoff

The integrator reported completion of **“Resolver accepts catalogued secondary
entries”**: function symbols may target entry addresses/ISAs, C definitions still
require eligible top-level starts, and build/progress credit requires
`kind == function`; match/packet render literals as data. Those consumer edits
were not made or independently tested by this worker. The v3 schema here includes
primary entries consistently; adding the same primary start in a resolver is the
same address/ISA, not a second function or a compatibility alias.

## 4. All 17 credited/pilot identities

Every pre-existing per-function field is identical for all seventeen listed
identities: address, **size**, code_size, ISA, kind, evidence, pools, code_ranges and
noreturn fields. They remain top-level ordinary `function` records; none becomes a
secondary entry or a non-C classified group. Additive entries/literals metadata is
not counted as an identity change.

| Address | v2 bytes | v3 bytes | Status |
| --- | ---: | ---: | --- |
| `0x10E664` | 12 | 12 | unchanged, ordinary `function` |
| `0x1149A8` | 24 | 24 | unchanged, ordinary `function` |
| `0x136930` | 40 | 40 | unchanged, ordinary `function` |
| `0x140DAC` | 40 | 40 | unchanged, ordinary `function` |
| `0x1179B4` | 16 | 16 | unchanged, ordinary `function` |
| `0x1263B8` | 20 | 20 | unchanged, ordinary `function` |
| `0x122350` | 16 | 16 | unchanged, ordinary `function` |
| `0x103798` | 40 | 40 | unchanged, ordinary `function` |
| `0x1091D4` | 56 | 56 | unchanged, ordinary `function` |
| `0x1103D4` | 16 | 16 | unchanged, ordinary `function` |
| `0x103258` | 136 | 136 | unchanged, ordinary `function` |
| `0x103358` | 116 | 116 | unchanged, ordinary `function` |
| `0x105084` | 64 | 64 | unchanged, ordinary `function` |
| `0x105FA8` | 100 | 100 | unchanged, ordinary `function` |
| `0x10D604` | 64 | 64 | unchanged, ordinary `function` |
| `0x1718EC` | 168 | 168 | unchanged, ordinary `function` |
| `0x171C7C` | 168 | 168 | unchanged, ordinary `function` |

Therefore the conditional instruction to rerun `tools/match.py` for changed
credited/pilot extents **did not trigger**. No match/build command was run or claimed.
No source under `src/pilot/` was changed. The integrator still owns adoption and
accepted-ledger/denominator-history gates.

## 5. Residual anomalies and unknowns

| Anomaly | v2 | v3 | Change |
| --- | ---: | ---: | ---: |
| `address_literal_crosses_start` | 0 | 7 | +7 |
| `address_literal_overlaps_code` | 0 | 5 | +5 |
| `fallthrough_into_start` | 92 | 0 | -92 |
| `literal_outside_function` | 14 | 14 | +0 |
| `shared_address_literal` | 0 | 14 | +14 |
| `undecodable_reachable_code` | 1 | 1 | +0 |
| `unexplained_function_bytes` | 90 | 41 | -49 |
| **Total** | **197** | **82** | **-115** |

There are **17 conflicting address-target objects** (multiple references can yield
more than one anomaly). Shared data was not stolen, a candidate start was not erased
to make a string fit, and known code was not retyped as data. Conflict records contain
addresses and reasons, never bytes or string contents:

| Referencing owner | Data target | Proposed padded end | Next owner start / text limit | Reasons |
| --- | --- | --- | --- | --- |
| `0x100360` | `0x1005FB` | `0x100600` | `0x10039C` | address_literal_crosses_start, address_literal_overlaps_code |
| `0x100520` | `0x1005A7` | `0x1005AC` | `0x100528` | address_literal_crosses_start, address_literal_overlaps_code |
| `0x10090C` | `0x10092B` | `0x100930` | `0x100914` | address_literal_crosses_start, address_literal_overlaps_code |
| `0x100A74` | `0x100ADC` | `0x100AE0` | `0x100AB8` | address_literal_crosses_start |
| `0x100A74` | `0x100AE0` | `0x100AE4` | `0x100AB8` | address_literal_crosses_start |
| `0x100AF0` | `0x100B0F` | `0x100B14` | `0x100AF8` | address_literal_crosses_start, address_literal_overlaps_code |
| `0x100C94` | `0x100C9D` | `0x100CA0` | `0x100C9E` | address_literal_crosses_start, address_literal_overlaps_code, shared_address_literal |
| `0x1456E8` | `0x1457A8` | `0x1457B4` | `0x1457D4` | shared_address_literal |
| `0x1456E8` | `0x1457B4` | `0x1457C8` | `0x1457D4` | shared_address_literal |
| `0x1B5374` | `0x1B54E4` | `0x1B54E8` | `0x1B54E8` | shared_address_literal |
| `0x1B8198` | `0x1B8234` | `0x1B8238` | `0x1B8238` | shared_address_literal |
| `0x1BC220` | `0x1BC29C` | `0x1BC2A0` | `0x1BC2A0` | shared_address_literal |
| `0x1CD070` | `0x1CD2E4` | `0x1CD2E8` | `0x1CD2F0` | shared_address_literal |
| `0x1CD070` | `0x1CD2EC` | `0x1CD2F0` | `0x1CD2F0` | shared_address_literal |
| `0x1F7D7C` | `0x1F82B0` | `0x1F82C4` | `0x202CA8` | shared_address_literal |
| `0x1F7D7C` | `0x1F83B4` | `0x1F83C4` | `0x202CA8` | shared_address_literal |
| `0x202CA8` | `0x203068` | `0x203078` | `0x203814` | shared_address_literal |

Remaining unknown coverage:

| Position | v2 | v3 |
| --- | ---: | ---: |
| Within extents | 31,548 B / 2,078 ranges / 87 owners | 1,402 B / 68 ranges / 39 owners |
| Between extents | 7,790 B / 146 ranges | 7,434 B / 126 ranges |
| Total | 39,338 B / 2,224 ranges | **8,836 B / 194 ranges** |

Of 41 remaining `unexplained_function_bytes` records, 39 contain unknown holes and
two only padding holes. Unreferenced data beyond a last return is not absorbed
without address/format evidence. The undecodable reachable site at `0x1964E8`
remains an anomaly rather than a fake noreturn fix. Unresolved PC address references,
protected veneer destinations and disputed startup code remain explicit.

## 6. Exercised acceptance and exact evidence

All checks below ran **sequentially** in
`/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3`, using
`/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python` and symlinked private
inputs. The only test module was `tests.test_analyze`.

The final full module run passed **70 tests**, including original-image determinism,
hard invariants, the 300-byte string extent, both missing-entry examples and the NOP
classification. Synthetic tests cover complete/unaligned/UTF-8/CP932/empty strings,
shared/suffix/global-pointer/pool-pointer ownership, code/next-start conflicts,
Thumb ADD-PC constants versus branch entry, NOP/body grouping, conditional-return
fallthrough, secondary B/BL entries, entry tampering, legacy validation and a real
outside-extent repair fixture. The original v2 tests remain part of that module.

| Exact command | PYTHONHASHSEED | Exit | Wall time | Output path |
| --- | ---: | ---: | ---: | --- |
| `/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out /Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v3.json` | 23 | 0 | 23.471 s | `scratch/analyze/catalogue-v3.json` |
| `/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python -m unittest tests.test_analyze -v` | 17 | 0 | 39.213 s | this report, final command transcript |
| `/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --out /Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v3.json` | 71 | 0 | 18.971 s | `scratch/analyze/catalogue-v3.json` |

The first CLI byte string was retained before the second run overwrote the assigned
candidate path. Final runs under seeds **23 and 71 are byte-identical**, SHA-256:

```text
d8fc286cd6daf2b201dac18afa20f067fee983333bbb317a4123f7417e104ad7
```

The final metadata diagnostic exited **0** and reports: no hard violations for v3
or legacy v2, zero lost/added call sites, zero lost/added branch sites, zero direct-B
targets outside all extents, zero undeclared external branch entries, and six fully
bounded string references in the reported function. It did not rebuild the catalogue.

Earlier literal-only 62-test/CLI runs were passing development snapshots, not the
final expanded deliverable; they were superseded when Main added findings (a–c).
Their candidate hash was `0dd6cecc1aacdb48c431d645612af4da00edb72404b828bc3495837e6f83a6fd`.
The final candidate is the 70-test, multi-entry snapshot above.

### Artifact hashes

| Path relative to main repository | SHA-256 |
| --- | --- |
| `scratch/worktrees/catalogue-v3/tools/analyze.py` | `6bed0bc5f051b27aa1d75cdf05d05bb6ef8a70b5cc7bdd1777cc3d6d70666a3b` |
| `scratch/worktrees/catalogue-v3/tests/test_analyze.py` | `5f9edff93fa0a6a93395e8068ebb93f1bec0c637b37098d3ef86bca431684b53` |
| `config/catalogue.json` | `65d1c56942a759901f27311e8fe722ed2f67c0a5b09b1f2e69f58225d3374e89` |
| `docs/denominator-history.jsonl` | `675a9ccbd75bdc82b0b961b9c18c667dec0b811c28b86c7ae89154b5758ee36f` |
| `scratch/analyze/catalogue-v3.json` | `d8fc286cd6daf2b201dac18afa20f067fee983333bbb317a4123f7417e104ad7` |

### Final command output (persisted here)

```json
[
  {
    "argv": [
      "/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python",
      "tools/analyze.py",
      "catalogue",
      "--out",
      "/Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v3.json"
    ],
    "cwd": "/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3",
    "hash_seed": 23,
    "exit_code": 0,
    "seconds": 23.471,
    "stdout": "4351 functions; 8836 unknown bytes; 82 anomalies; 23.394s; /Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v3.json\n",
    "stderr": ""
  },
  {
    "argv": [
      "/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python",
      "-m",
      "unittest",
      "tests.test_analyze",
      "-v"
    ],
    "cwd": "/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3",
    "hash_seed": 17,
    "exit_code": 0,
    "seconds": 39.213,
    "stdout": "",
    "stderr": "test_add_sub_and_multibyte_string_literals (tests.test_analyze.CatalogueTests.test_add_sub_and_multibyte_string_literals) ... ok\ntest_adr_literal_pool (tests.test_analyze.CatalogueTests.test_adr_literal_pool) ... ok\ntest_all_six_strings_are_complete (tests.test_analyze.CatalogueTests.test_all_six_strings_are_complete) ... ok\ntest_arm_address_switch_table (tests.test_analyze.CatalogueTests.test_arm_address_switch_table) ... ok\ntest_arm_branch_switch_table (tests.test_analyze.CatalogueTests.test_arm_branch_switch_table) ... ok\ntest_arm_literal_veneer (tests.test_analyze.CatalogueTests.test_arm_literal_veneer) ... ok\ntest_arm_lo_switch_has_exclusive_bound (tests.test_analyze.CatalogueTests.test_arm_lo_switch_has_exclusive_bound) ... ok\ntest_bad_pool_alignment_is_detected (tests.test_analyze.CatalogueTests.test_bad_pool_alignment_is_detected) ... ok\ntest_bl_target_can_be_an_explicit_group_entry (tests.test_analyze.CatalogueTests.test_bl_target_can_be_an_explicit_group_entry) ... ok\ntest_bl_to_following_function (tests.test_analyze.CatalogueTests.test_bl_to_following_function) ... ok\ntest_branch_into_body_merges_weak_label (tests.test_analyze.CatalogueTests.test_branch_into_body_merges_weak_label) ... ok\ntest_branch_table_without_nearby_compare (tests.test_analyze.CatalogueTests.test_branch_table_without_nearby_compare) ... ok\ntest_call_to_bad_target_is_anomaly (tests.test_analyze.CatalogueTests.test_call_to_bad_target_is_anomaly) ... ok\ntest_call_to_infinite_loop_terminates_fallthrough (tests.test_analyze.CatalogueTests.test_call_to_infinite_loop_terminates_fallthrough) ... ok\ntest_closed_tail_cycle_is_noreturn (tests.test_analyze.CatalogueTests.test_closed_tail_cycle_is_noreturn) ... ok\ntest_conditional_noreturn_call_keeps_returning_path (tests.test_analyze.CatalogueTests.test_conditional_noreturn_call_keeps_returning_path) ... ok\ntest_conditional_return_fallthrough_is_non_c (tests.test_analyze.CatalogueTests.test_conditional_return_fallthrough_is_non_c) ... ok\ntest_cross_isa_pool_does_not_merge_blx_target (tests.test_analyze.CatalogueTests.test_cross_isa_pool_does_not_merge_blx_target) ... ok\ntest_discovery_and_table_protection_precede_exit_pruning (tests.test_analyze.CatalogueTests.test_discovery_and_table_protection_precede_exit_pruning) ... ok\ntest_embedded_string_ranges_are_data_not_code (tests.test_analyze.CatalogueTests.test_embedded_string_ranges_are_data_not_code) ... ok\ntest_external_body_branch_records_secondary_entry (tests.test_analyze.CatalogueTests.test_external_body_branch_records_secondary_entry) ... ok\ntest_external_data_pointer_prevents_string_absorption (tests.test_analyze.CatalogueTests.test_external_data_pointer_prevents_string_absorption) ... ok\ntest_external_tail_call_protects_weak_start (tests.test_analyze.CatalogueTests.test_external_tail_call_protects_weak_start) ... ok\ntest_fallthrough_without_return_merges_prologue (tests.test_analyze.CatalogueTests.test_fallthrough_without_return_merges_prologue) ... ok\ntest_function_pool_and_complete_adr_string (tests.test_analyze.CatalogueTests.test_function_pool_and_complete_adr_string) ... ok\ntest_incomplete_callee_is_not_noreturn (tests.test_analyze.CatalogueTests.test_incomplete_callee_is_not_noreturn) ... ok\ntest_internal_branch_labels_are_not_secondary_entries (tests.test_analyze.CatalogueTests.test_internal_branch_labels_are_not_secondary_entries) ... ok\ntest_internal_unknown_hole_is_explicit (tests.test_analyze.CatalogueTests.test_internal_unknown_hole_is_explicit) ... ok\ntest_isolated_pointer_can_name_an_internal_label (tests.test_analyze.CatalogueTests.test_isolated_pointer_can_name_an_internal_label) ... ok\ntest_leaf_and_padding (tests.test_analyze.CatalogueTests.test_leaf_and_padding) ... ok\ntest_legacy_catalogue_without_entries_still_validates (tests.test_analyze.CatalogueTests.test_legacy_catalogue_without_entries_still_validates) ... ok\ntest_literal_constraint_merges_multiple_weak_boundaries (tests.test_analyze.CatalogueTests.test_literal_constraint_merges_multiple_weak_boundaries) ... ok\ntest_literal_validation_rejects_overlap_and_bad_alignment (tests.test_analyze.CatalogueTests.test_literal_validation_rejects_overlap_and_bad_alignment) ... ok\ntest_map_gap_is_detected (tests.test_analyze.CatalogueTests.test_map_gap_is_detected) ... ok\ntest_mid_function_pool_does_not_merge_bl_target (tests.test_analyze.CatalogueTests.test_mid_function_pool_does_not_merge_bl_target) ... ok\ntest_mid_function_pool_merges_prologue_only_start (tests.test_analyze.CatalogueTests.test_mid_function_pool_merges_prologue_only_start) ... ok\ntest_mid_function_pool_merges_tail_only_start (tests.test_analyze.CatalogueTests.test_mid_function_pool_merges_tail_only_start) ... ok\ntest_nonzero_page_padding_is_anomaly (tests.test_analyze.CatalogueTests.test_nonzero_page_padding_is_anomaly) ... ok\ntest_nop_entry_and_body_are_one_non_c_group (tests.test_analyze.CatalogueTests.test_nop_entry_and_body_are_one_non_c_group) ... ok\ntest_noreturn_call_does_not_absorb_unreferenced_return_code (tests.test_analyze.CatalogueTests.test_noreturn_call_does_not_absorb_unreferenced_return_code) ... ok\ntest_noreturn_propagates_through_calls_to_exit_process (tests.test_analyze.CatalogueTests.test_noreturn_propagates_through_calls_to_exit_process) ... ok\ntest_original_invariants_and_stability (tests.test_analyze.CatalogueTests.test_original_invariants_and_stability) ... ok\ntest_other_or_conditional_svc_can_return (tests.test_analyze.CatalogueTests.test_other_or_conditional_svc_can_return) ... ok\ntest_pointer_function_and_prologue_gap (tests.test_analyze.CatalogueTests.test_pointer_function_and_prologue_gap) ... ok\ntest_pointer_tables_protect_weak_starts (tests.test_analyze.CatalogueTests.test_pointer_tables_protect_weak_starts) ... ok\ntest_return_forms_and_prefetch (tests.test_analyze.CatalogueTests.test_return_forms_and_prefetch) ... ok\ntest_secondary_entries_are_validated_not_assumed (tests.test_analyze.CatalogueTests.test_secondary_entries_are_validated_not_assumed) ... ok\ntest_shared_adr_string_remains_unknown (tests.test_analyze.CatalogueTests.test_shared_adr_string_remains_unknown) ... ok\ntest_string_crossing_a_start_is_not_stolen (tests.test_analyze.CatalogueTests.test_string_crossing_a_start_is_not_stolen) ... ok\ntest_string_shared_through_another_functions_pool (tests.test_analyze.CatalogueTests.test_string_shared_through_another_functions_pool) ... ok\ntest_switch_labels_are_not_callback_table_entries (tests.test_analyze.CatalogueTests.test_switch_labels_are_not_callback_table_entries) ... ok\ntest_tail_call_after_setup_and_padding (tests.test_analyze.CatalogueTests.test_tail_call_after_setup_and_padding) ... ok\ntest_tail_call_after_single_lr_restore (tests.test_analyze.CatalogueTests.test_tail_call_after_single_lr_restore) ... ok\ntest_tail_call_branch (tests.test_analyze.CatalogueTests.test_tail_call_branch) ... ok\ntest_tail_target_hidden_by_address_word_is_repaired (tests.test_analyze.CatalogueTests.test_tail_target_hidden_by_address_word_is_repaired) ... ok\ntest_thumb_add_pc_does_not_assume_branch_entry_constant (tests.test_analyze.CatalogueTests.test_thumb_add_pc_does_not_assume_branch_entry_constant) ... ok\ntest_thumb_add_pc_uses_adjacent_proven_constant (tests.test_analyze.CatalogueTests.test_thumb_add_pc_uses_adjacent_proven_constant) ... ok\ntest_thumb_adr_string_is_complete (tests.test_analyze.CatalogueTests.test_thumb_adr_string_is_complete) ... ok\ntest_thumb_bl_target (tests.test_analyze.CatalogueTests.test_thumb_bl_target) ... ok\ntest_thumb_blx_to_arm_target (tests.test_analyze.CatalogueTests.test_thumb_blx_to_arm_target) ... ok\ntest_thumb_exit_process_is_noreturn (tests.test_analyze.CatalogueTests.test_thumb_exit_process_is_noreturn) ... ok\ntest_thumb_function_reached_by_blx (tests.test_analyze.CatalogueTests.test_thumb_function_reached_by_blx) ... ok\ntest_thumb_interworking_veneer (tests.test_analyze.CatalogueTests.test_thumb_interworking_veneer) ... ok\ntest_thumb_literal_and_adr_pools (tests.test_analyze.CatalogueTests.test_thumb_literal_and_adr_pools) ... ok\ntest_thumb_prologue_in_gap (tests.test_analyze.CatalogueTests.test_thumb_prologue_in_gap) ... ok\ntest_trailing_literal_pool (tests.test_analyze.CatalogueTests.test_trailing_literal_pool) ... ok\ntest_unaligned_string_does_not_claim_its_prefix (tests.test_analyze.CatalogueTests.test_unaligned_string_does_not_claim_its_prefix) ... ok\ntest_unknown_bytes_are_not_absorbed (tests.test_analyze.CatalogueTests.test_unknown_bytes_are_not_absorbed) ... ok\ntest_unknown_indirect_exit_is_not_noreturn (tests.test_analyze.CatalogueTests.test_unknown_indirect_exit_is_not_noreturn) ... ok\ntest_vfp_double_literal_pool (tests.test_analyze.CatalogueTests.test_vfp_double_literal_pool) ... ok\n\n----------------------------------------------------------------------\nRan 70 tests in 39.131s\n\nOK\n"
  },
  {
    "argv": [
      "/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python",
      "tools/analyze.py",
      "catalogue",
      "--out",
      "/Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v3.json"
    ],
    "cwd": "/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3",
    "hash_seed": 71,
    "exit_code": 0,
    "seconds": 18.971,
    "stdout": "4351 functions; 8836 unknown bytes; 82 anomalies; 18.881s; /Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v3.json\n",
    "stderr": ""
  }
]
```

### Final metadata-diagnostic command, output and exit

The `-c` argument is the exact source passed to `subprocess.run`, not a guessed
shell reconstruction. Only metadata, counts and addresses were emitted.

```json
{
  "argv": [
    "/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python",
    "-c",
    "from pathlib import Path\nfrom tools import analyze,ctrfmt\nfrom bisect import bisect_right\nimport json\nold=json.loads(Path('config/catalogue.json').read_text()); new=json.loads(Path('/Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v3.json').read_text()); a=analyze.Analyzer(Path('orig/code.bin').read_bytes(),ctrfmt.parse_exheader(Path('orig/exheader.bin').read_bytes()))\ndef edges(c):\n calls=set(); branches=set()\n for f in c['functions']:\n  for lo,hi in f['code_ranges']:\n   while lo<hi:\n    isa='arm' if f['kind']=='veneer' and f['isa']=='thumb' and lo==f['address']+4 else f['isa']; ins=a.decode(lo,isa)\n    if ins.call and ins.target: calls.add((lo,*ins.target))\n    if ins.branch and ins.target: branches.add((lo,*ins.target))\n    lo+=ins.size\n return calls,branches\nold_calls,old_branches=edges(old); calls,branches=edges(new); starts=[f['address'] for f in new['functions']]; declared={entry['address']:entry['isa'] for f in new['functions'] for entry in f['entries']}; outside=[]; missing=[]\nfor site,target,isa in sorted(branches):\n index=bisect_right(starts,target)-1\n if index<0 or target>=starts[index]+new['functions'][index]['size']: outside.append((site,target,isa))\n else:\n  owner=new['functions'][index]; src=bisect_right(starts,site)-1\n  if index!=src and declared.get(target)!=isa: missing.append((site,target,isa))\nf=next(f for f in new['functions'] if f['address']==0x1932e8); strings=[]\nfor lo,hi in f['code_ranges']:\n while lo<hi:\n  ins=a.decode(lo,f['isa'])\n  if ins.address_target is not None:\n   target=ins.address_target; stop=a._string_end(target)\n   strings.append({'reference':lo,'address':target,'end':stop,'padded_end':(stop+3)&~3 if stop else None})\n  lo+=ins.size\nprint(json.dumps({'hard_violations':analyze.validate_catalogue(new,a.image,a.layout),'legacy_v2_hard_violations':analyze.validate_catalogue(old,a.image,a.layout),'old_calls':len(old_calls),'new_calls':len(calls),'lost_calls':sorted(old_calls-calls),'added_calls':sorted(calls-old_calls),'old_branch_sites':len(old_branches),'new_branch_sites':len(branches),'lost_branch_sites':sorted(old_branches-branches),'added_branch_sites':sorted(branches-old_branches),'outside_branch_targets':outside,'undeclared_external_branch_entries':missing,'reported_strings':strings},sort_keys=True))"
  ],
  "cwd": "/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3",
  "hash_seed": 17,
  "exit_code": 0,
  "seconds": 3.299,
  "stdout": "{\"added_branch_sites\": [], \"added_calls\": [], \"hard_violations\": [], \"legacy_v2_hard_violations\": [], \"lost_branch_sites\": [], \"lost_calls\": [], \"new_branch_sites\": 26271, \"new_calls\": 19991, \"old_branch_sites\": 26271, \"old_calls\": 19991, \"outside_branch_targets\": [], \"reported_strings\": [{\"address\": 1651600, \"end\": 1651614, \"padded_end\": 1651616, \"reference\": 1651508}, {\"address\": 1651616, \"end\": 1651635, \"padded_end\": 1651636, \"reference\": 1651516}, {\"address\": 1651636, \"end\": 1651663, \"padded_end\": 1651664, \"reference\": 1651524}, {\"address\": 1651664, \"end\": 1651690, \"padded_end\": 1651692, \"reference\": 1651532}, {\"address\": 1651692, \"end\": 1651712, \"padded_end\": 1651712, \"reference\": 1651540}, {\"address\": 1651712, \"end\": 1651732, \"padded_end\": 1651732, \"reference\": 1651548}], \"undeclared_external_branch_entries\": []}\n",
  "stderr": ""
}
```

### Development/scoping command ledger

These exact argv/cwd/exit/timing records make the development observations
reproducible without an extra throwaway script. Final verification is the separately
identified pair/module run above; scoping and earlier snapshots are not substituted
for it. Output for the scoping NOP/tail investigation was address-only metadata.

```json
[
  {
    "argv": [
      "/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python",
      "-c",
      "import unittest; from tests.test_analyze import CatalogueTests; suite=unittest.TestSuite(CatalogueTests(name) for name in unittest.defaultTestLoader.getTestCaseNames(CatalogueTests) if name != 'test_original_invariants_and_stability'); result=unittest.TextTestRunner(verbosity=2).run(suite); raise SystemExit(not result.wasSuccessful())"
    ],
    "cwd": "/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3",
    "hash_seed": 17,
    "exit_code": 0,
    "seconds": 0.137
  },
  {
    "argv": [
      "/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python",
      "tools/analyze.py",
      "catalogue",
      "--out",
      "/Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v3.json"
    ],
    "cwd": "/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3",
    "hash_seed": 23,
    "exit_code": 0,
    "seconds": 20.974
  },
  {
    "argv": [
      "/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python",
      "-m",
      "unittest",
      "tests.test_analyze",
      "-v"
    ],
    "cwd": "/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3",
    "hash_seed": 17,
    "exit_code": 0,
    "seconds": 45.558
  },
  {
    "argv": [
      "/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python",
      "tools/analyze.py",
      "catalogue",
      "--out",
      "/Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v3.json"
    ],
    "cwd": "/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3",
    "hash_seed": 71,
    "exit_code": 0,
    "seconds": 21.575
  },
  {
    "argv": [
      "/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python",
      "-c",
      "from pathlib import Path\nfrom tools import analyze, ctrfmt\nfrom bisect import bisect_right\nfrom collections import Counter\nimport json,struct\nc=json.loads(Path('/Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v3.json').read_text()); a=analyze.Analyzer(Path('orig/code.bin').read_bytes(),ctrfmt.parse_exheader(Path('orig/exheader.bin').read_bytes())); fs=c['functions']; by={f['address']:f for f in fs}; starts=sorted(by)\nnops=[]; branches={}; calls={}; pointers={}\nfor f in fs:\n if f['isa']=='arm' and f['size']==4 and f['code_size']==4 and a.word(f['address']) in (0xe1a00000,0xe320f000):\n  index=bisect_right(starts,f['address']); nops.append({'address':f['address'],'evidence':f['evidence'],'next_start':starts[index] if index<len(starts) else None,'next_size':by[starts[index]]['size'] if index<len(starts) else None})\n for lo,hi in f['code_ranges']:\n  while lo<hi:\n   isa='arm' if f['kind']=='veneer' and f['isa']=='thumb' and lo==f['address']+4 else f['isa']; ins=a.decode(lo,isa)\n   if ins.target is not None:\n    if ins.branch: branches.setdefault(ins.target,[]).append((lo,f['address']))\n    if ins.call: calls.setdefault(ins.target,[]).append((lo,f['address']))\n   lo+=ins.size\n for lo,hi in f['pools']:\n  for word in range(lo,hi,4): pointers.setdefault(a.word(word),[]).append((word,f['address'],'pool'))\nfor seg in (a.layout.rodata,a.layout.data):\n if seg.size:\n  off=a.layout.file_offset(seg.address)\n  for index in range(0,seg.size-3,4): pointers.setdefault(struct.unpack_from('<I',a.image,off+index)[0],[]).append((seg.address+index,None,seg.name))\nfor n in nops:\n n['pointer_sources']=pointers.get(n['address'],[]); n['branch_sources']=branches.get((n['address'],'arm'),[]); n['call_sources']=calls.get((n['address'],'arm'),[])\noutside=[]; interior=Counter()\nfor (target,isa),sources in sorted(branches.items()):\n if target in by: continue\n index=bisect_right(starts,target)-1; f=fs[index] if index>=0 else None\n if f is None or target>=f['address']+f['size']:\n  outside.append({'target':target,'isa':isa,'sources':sources,'decodes':a.decode(target,isa) is not None if a.inside(target,isa) else False})\n else:\n  position='code' if any(lo<=target<hi for lo,hi in f['code_ranges']) else 'pool' if any(lo<=target<hi for lo,hi in f['pools']) else 'literal' if any(lo<=target<hi for lo,hi in f.get('literals',[])) else 'hole'\n  interior[position]+=1\nprint(json.dumps({'nop_starts':nops,'uncatalogued_external_branch_targets':outside,'interior_nonstart_branch_targets':dict(interior)},sort_keys=True))"
    ],
    "cwd": "/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3",
    "hash_seed": 17,
    "exit_code": 0,
    "seconds": 1.149
  },
  {
    "argv": [
      "/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python",
      "-c",
      "from pathlib import Path\nfrom tools import analyze,ctrfmt\nfrom bisect import bisect_right\nfrom collections import Counter\nimport json\nc=json.loads(Path('/Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v3.json').read_text()); a=analyze.Analyzer(Path('orig/code.bin').read_bytes(),ctrfmt.parse_exheader(Path('orig/exheader.bin').read_bytes())); fs=c['functions']; starts=[f['address'] for f in fs]; by={f['address']:f for f in fs}\nfalls={item['address'] for item in c['anomalies'] if item['issue']=='fallthrough_into_start'}; groups=[]\nfor f in fs:\n if groups and f['address'] in falls: groups[-1].append(f['address'])\n else: groups.append([f['address']])\nowner={start:group[0] for group in groups for start in group}; external={}; examples=[]\nfor f in fs:\n for lo,hi in f['code_ranges']:\n  while lo<hi:\n   isa='arm' if f['kind']=='veneer' and f['isa']=='thumb' and lo==f['address']+4 else f['isa']; ins=a.decode(lo,isa)\n   if ins.target and (ins.branch or ins.call):\n    target,target_isa=ins.target; index=bisect_right(starts,target)-1; target_f=fs[index] if index>=0 else None\n    if target_f is not None and target<target_f['address']+target_f['size'] and owner[target_f['address']]!=owner[f['address']] and target!=owner[target_f['address']]:\n     external.setdefault(owner[target_f['address']],{}).setdefault(target,set()).add(lo)\n    if lo in (0x11fa34,0x11ffdc,0x135054): examples.append({'site':lo,'target':target,'owner':target_f['address'] if target_f else None,'in_code':any(x<=target<y for x,y in target_f['code_ranges']) if target_f else False})\n   lo+=ins.size\npilots=[0x10e664,0x1149a8,0x136930,0x140dac,0x1179b4,0x1263b8,0x122350,0x103798,0x1091d4,0x1103d4,0x103258,0x103358,0x105084,0x105fa8,0x10d604,0x1718ec,0x171c7c]\nprint(json.dumps({'fallthrough_groups':[group for group in groups if len(group)>1],'group_size_counts':dict(Counter(len(group) for group in groups if len(group)>1)),'external_entry_owner_count':len(external),'external_entry_count':sum(len(entries) for entries in external.values()),'examples':examples,'pilots_affected':[{'address':p,'owner':owner[p],'members':next(g for g in groups if p in g),'external_entries':sorted(external.get(owner[p],{}))} for p in pilots if len(next(g for g in groups if p in g))>1 or owner[p] in external],'mixed_isa_groups':[group for group in groups if len({by[p]['isa'] for p in group})>1]},sort_keys=True))"
    ],
    "cwd": "/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3",
    "hash_seed": 17,
    "exit_code": 0,
    "seconds": 1.039
  },
  {
    "argv": [
      "/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python",
      "-c",
      "import unittest; from tests.test_analyze import CatalogueTests; suite=unittest.TestSuite(CatalogueTests(name) for name in unittest.defaultTestLoader.getTestCaseNames(CatalogueTests) if name != 'test_original_invariants_and_stability'); result=unittest.TextTestRunner(verbosity=2).run(suite); raise SystemExit(not result.wasSuccessful())"
    ],
    "cwd": "/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3",
    "hash_seed": 17,
    "exit_code": 0,
    "seconds": 0.113
  },
  {
    "argv": [
      "/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python",
      "tools/analyze.py",
      "catalogue",
      "--out",
      "/Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v3.json"
    ],
    "cwd": "/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3",
    "hash_seed": 23,
    "exit_code": 0,
    "seconds": 23.471
  },
  {
    "argv": [
      "/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python",
      "-m",
      "unittest",
      "tests.test_analyze",
      "-v"
    ],
    "cwd": "/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3",
    "hash_seed": 17,
    "exit_code": 0,
    "seconds": 39.213
  },
  {
    "argv": [
      "/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python",
      "tools/analyze.py",
      "catalogue",
      "--out",
      "/Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v3.json"
    ],
    "cwd": "/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3",
    "hash_seed": 71,
    "exit_code": 0,
    "seconds": 18.971
  },
  {
    "argv": [
      "/Users/mooki/Code/kanken-training-2-decomp/.venv/bin/python",
      "-c",
      "from pathlib import Path\nfrom tools import analyze,ctrfmt\nfrom bisect import bisect_right\nimport json\nold=json.loads(Path('config/catalogue.json').read_text()); new=json.loads(Path('/Users/mooki/Code/kanken-training-2-decomp/scratch/analyze/catalogue-v3.json').read_text()); a=analyze.Analyzer(Path('orig/code.bin').read_bytes(),ctrfmt.parse_exheader(Path('orig/exheader.bin').read_bytes()))\ndef edges(c):\n calls=set(); branches=set()\n for f in c['functions']:\n  for lo,hi in f['code_ranges']:\n   while lo<hi:\n    isa='arm' if f['kind']=='veneer' and f['isa']=='thumb' and lo==f['address']+4 else f['isa']; ins=a.decode(lo,isa)\n    if ins.call and ins.target: calls.add((lo,*ins.target))\n    if ins.branch and ins.target: branches.add((lo,*ins.target))\n    lo+=ins.size\n return calls,branches\nold_calls,old_branches=edges(old); calls,branches=edges(new); starts=[f['address'] for f in new['functions']]; declared={entry['address']:entry['isa'] for f in new['functions'] for entry in f['entries']}; outside=[]; missing=[]\nfor site,target,isa in sorted(branches):\n index=bisect_right(starts,target)-1\n if index<0 or target>=starts[index]+new['functions'][index]['size']: outside.append((site,target,isa))\n else:\n  owner=new['functions'][index]; src=bisect_right(starts,site)-1\n  if index!=src and declared.get(target)!=isa: missing.append((site,target,isa))\nf=next(f for f in new['functions'] if f['address']==0x1932e8); strings=[]\nfor lo,hi in f['code_ranges']:\n while lo<hi:\n  ins=a.decode(lo,f['isa'])\n  if ins.address_target is not None:\n   target=ins.address_target; stop=a._string_end(target)\n   strings.append({'reference':lo,'address':target,'end':stop,'padded_end':(stop+3)&~3 if stop else None})\n  lo+=ins.size\nprint(json.dumps({'hard_violations':analyze.validate_catalogue(new,a.image,a.layout),'legacy_v2_hard_violations':analyze.validate_catalogue(old,a.image,a.layout),'old_calls':len(old_calls),'new_calls':len(calls),'lost_calls':sorted(old_calls-calls),'added_calls':sorted(calls-old_calls),'old_branch_sites':len(old_branches),'new_branch_sites':len(branches),'lost_branch_sites':sorted(old_branches-branches),'added_branch_sites':sorted(branches-old_branches),'outside_branch_targets':outside,'undeclared_external_branch_entries':missing,'reported_strings':strings},sort_keys=True))"
    ],
    "cwd": "/Users/mooki/Code/kanken-training-2-decomp/scratch/worktrees/catalogue-v3",
    "hash_seed": 17,
    "exit_code": 0,
    "seconds": 3.299
  }
]
```

## Residual risks / adoption boundary

- String terminators, printable encodings, exclusive recovered references and
  alignment are evidence, not original linker section headers. Hidden indirect
  references and unknown binary object lengths remain possible. Only bounded data
  is claimed; arbitrary remaining bytes do not become literals or padding.
- `multi_entry` means the **recovered graph** is incompatible with independent
  ordinary split-section C functions. It does not prove an SDK match or handwritten
  provenance. An unrecognized return instruction or earlier over/under-split could
  require a later reviewed revision. The complete groups/entry witnesses are exposed
  for that review, and no classified group receives C credit.
- Entry-aware call validation is an explicit requested model revision, not silent
  relaxation: entry order, unique owner, code boundary, correct ISA, extents, pools,
  literals and map tiling are all still checked. Secondary entry symbols do not
  authorize standalone C definitions.
- There remain 82 semantic anomalies and 8,836 unknown bytes. The catalogue is not
  claimed complete or ready for blind canonical adoption. No frozen denominator,
  accepted ledger, progress history or receipt was edited by this worker.
