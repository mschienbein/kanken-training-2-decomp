# Catalogue v13 — complete-section alignment and eight non-C certificates

## Immutable predecessor and scope

This is a distinct, **unadopted** successor to independently reviewed v12-r2,
source `0a2f7a9780a8b8016b694f6d2604f8bc9ce4be0d`, reviewed
`PASS_WITH_QUALIFICATIONS`. The reviewed proposal is SHA-256
`ee64b7d69bf5b96fe26a8ada241a59eca2924ac4ab9a376a5fc449e2d1e5a942`.
The original image remains SHA-256
`a367b5dcd65e3924c93cf3fbaee5d4d40f9c5205d1287912784a974d5e405361`.
No v12 or earlier evidence, rule, declaration, API alias, source profile, accepted
ledger, GATES policy or canonical catalogue is changed by this proposal.

The independently read inputs include Main's `scratch/integrator/catalogue-v13-plan.md`,
both complete residual research reports/evidence, `REPORT-v12.md`, the r4/v12-r2
review records, and Main's additional complete-section library/Thumb research.
Old negative results remain meaningful; the two new standards below are explicit
v13-only additions rather than reinterpretations of earlier PASS claims.

The exact proposed alignment delta is **22 intervals / 44 B**, from unknown:
**13 external intervals / 26 B** to unowned padding and **nine internal
intervals / 18 B** to section-associated padding. It explains **eight** exact
`unexplained_function_bytes` anomalies. **114 B in 11 intervals and one anomaly
remain unresolved.** All 4,743 owners and 4,776 entry identities remain; there are
no new entry, extent, code/pool/literal or physical function-span claims.

Seven newly evidenced library classifications and one assembly classification
exclude **eight existing ordinary owners / 1,514 B** from the creditable
catalogue denominator. This is **zero new accepted C/source/data credit** and
zero canonical-name adoption, not an acceptance/denominator-ledger operation.

## Primary authority and limits

* [AAELF32, symbol values and mapping symbols](https://github.com/ARM-software/abi-aa/blob/main/aaelf32/aaelf32.rst#mapping-symbols)
  defines `$a`, `$t` and `$d` and their half-open section partitions. `$d` is a
  code/data distinction, **not** a sized data declaration or an ownership grant.
* [AAELF32, relocation](https://github.com/ARM-software/abi-aa/blob/main/aaelf32/aaelf32.rst#relocation)
  supplies the relocation/addend framework. The REL32 comparisons use static
  file-endian ELF arithmetic, not an all-path runtime little-endian certificate.
* [Arm armlink User Guide, DUI0474K, §3.3.3, Section alignment with the linker,
  p.3-58](https://documentation-service.arm.com/static/5ea19ea09931941038de96d2)
  states that the linker inserts padding as required so each input section begins
  at a multiple of its input-section alignment. Section ordering precedes this
  placement. This establishes the alignment mechanism, not an arbitrary gap's
  ownership or the actual code-fill value.
* The same guide's §7.10 distinguishes input-section/region overalignment;
  §12.104, `--pad=num`, p.12-368, describes padding bytes in load/execution
  regions. `--pad` alone does **not** prove the inter-section code pattern used
  by this image; v13 additionally requires the unanimous image-bound census.
* [Arm architecture reference](https://developer.arm.com/documentation/ddi0406/c)
  distinguishes word-aligned ARM-state instructions from halfword-aligned Thumb
  instructions. An observed aligned address by itself never authenticates a
  section's alignment; v13 needs ARM-state code or the complete object's actual
  `sh_addralign`.

No original bytes, instruction words, disassembly, ROM packets or licensed
archive payload are recorded. Public metadata contains addresses, section and
symbol properties, relocation relationships, hashes and conclusions only.

## Two fail-closed v13 standards

### External linker alignment fill

An admitted interval must be the **entire exact alignment deficit** between the
proven end of a complete predecessor input section and the next proven-aligned
successor. A retained matching object can supply the complete section; a partial
function, masked fragment or approximate endpoint cannot. Predecessor section
size/alignment, complete symbol/relocation inventory, complete partition hashes,
whole reencoding equality and retained catalogue-placement bindings are sealed.

Successor alignment must be architectural ARM-state word alignment or the exact
alignment from a complete object. Its complete catalogue record is bound. Every
admitted byte must remain unknown/unowned before the transition and have no
intersecting catalogue entry, owner extent, instruction/data span, relocation
field, incoming relocation/pointer, protected observation or bounded direct read.
This is an exclusion of **bounded** references, not a claim to solve every
unbounded/indirect read veto inherited from r4.

All **15** observed external halfwords participate in the image-wide fill check,
including observations whose predecessors cannot be admitted. They hold one
identical nonzero value; the value is not a Thumb NOP or a half of ARM MOV r0,r0.
Only its hash is retained. **One disagreement refuses every external gap**, before
any mutation. Uniformity never substitutes for the complete-predecessor proof.

The initial residual evidence proved only twelve predecessors complete. New
Thumb research supplied a complete relocation-free **18 B, alignment-2
`wcslen.o`** section at `0019d5b0`. This revision independently rechecked the
exact member at ar header offset `1164550`, member SHA-256
`4768014076128dc9ea33f23c28eb3d4d12fadae05f98795684a5c4fdf4287467`,
its whole section/declarations and retained original placement. That supplies
**geometry only** for `0019d5c2`; it does not authenticate a `wcslen` API alias or
classify the ordinary owner. The two character-adapter predecessors still have
refused weak-import fields and remain incomplete; their gaps remain unknown.

Exact external transitions, each 2 B, `unknown/null -> padding/null`:

| Start | Exclusive end | Complete predecessor |
|---|---|---|
| `00100076` | `00100078` | `strtoull.o`, 8 B |
| `0010051e` | `00100520` | `_strtoull.o`, 216 B |
| `0010090a` | `0010090c` | `_printf_charcount.o`, 38 B |
| `00100a6a` | `00100a6c` | `_printf_wc.o`, 86 B |
| `00100c92` | `00100c94` | `aeabi_vec_dtor_cookie.o`, 18 B |
| `0016b6a2` | `0016b6a4` | `wcscasecmp.o`, 42 B |
| `00181eaa` | `00181eac` | `_rserrno.o`, complete shared 22 B section |
| `001911ee` | `001911f0` | `aeabi_vec_ctor_nocookie_nodtor.o`, 34 B |
| `00191c92` | `00191c94` | `cxa_guard_acquire.o`, 18 B |
| `0019d5c2` | `0019d5c4` | `wcslen.o`, complete 18 B section; identity unpromoted |
| `0019f6d6` | `0019f6d8` | `aeabi_vec_dtor_noexceptions.o`, 34 B |
| `0019fd6a` | `0019fd6c` | `tolower.o`, 26 B; imported target is not a new API identity |
| `0019ffa2` | `0019ffa4` | `_printf_intcommon.o`, 176 B |

All thirteen admitted successors are bound ARM-state owners. The two excluded
census intervals are `[00100942,00100944)` and `[00100b26,00100b28)`.

### Internal zero alignment

Inside a completely reencoded input section, v13 admits only the zero-valued
interval from the exact end of a sized function declaration to the next aligned
REL32 or retained literal field. It is shorter than that field's alignment and
ends **exactly** at the field. Complete section mappings must place it in data;
no sized function/object declaration, relocation field, entry or bounded direct
reference may cover it. Complete inventory and comparison, not zero contents
alone, establish the compiler/assembler-emitted alignment interpretation.

The physical padding is associated with its retained section catalogue owner;
**no complete function record or extent changes**. A sparse-owner anomaly is
removed only when the old code/pool/literal spans plus these exact padding bytes
tile that owner's complete old extent, without a gap or overlap.

| Exact interval (2 B each) | Padding owner | Complete section / following field | Exact anomaly explained |
|---|---|---|---|
| `[00100396,00100398)` | `00100360` | `__0snprintf.o`, 60 B / REL32 | `00100360` |
| `[001005ea,001005ec)` | `001005ca` | `_printf_char_common.o`, 48 B / REL32 | `001005ca` |
| `[001007ce,001007d0)` | `0010064c` | `__printf_flags_ss_wp.o`, 392 B / REL32 | `0010064c` |
| `[001008c6,001008c8)` | `001008ac` | `init_aeabi.o`, 36 B / REL32 | `001008ac` |
| `[001009fe,00100a00)` | `001009de` | `_printf_wchar_common.o`, 48 B / REL32 | `001009de` |
| `[00100ada,00100adc)` | `00100ad0` | `_printf_oct_int_ll.o`, 112 B / separate retained 8 B mapped data tail | none |
| `[00100c7a,00100c7c)` | `00100bb0` | `bigflt0.o`, 208 B / REL32 | `00100bb0` |
| `[0019f622,0019f624)` | `0019f5c4` | `_printf_longlong_dec.o`, 112 B / retained literal | `0019f5c4` |
| `[0019f696,0019f698)` | `0019f63c` | `_printf_dec.o`, 108 B / retained literal | `0019f63c` |

`00100ad0` remains a **10-byte declaration and 10-byte catalogue extent**. Its
following two-byte padding association is section provenance, not executable
ownership or an extent extension; the separate unowned 8 B tail is unchanged.
The protected **22/10/620 B** declarations and all foreign-tail restrictions
remain unchanged. Every old R5/endian/transfer refusal remains unchanged.

## Eight independently rederived non-C transitions

`catalogue_v13_non_c_proof.json` records the selected exact members, all allocated
section indices, complete compared-section metadata, mappings, declarations,
relocations/addends, placement relations and original incoming references.
The new bounded proof reopens only those selected members. It independently
reencodes **3,022 complete-section bytes**; extra comparison tails and locale
descriptors do not become new physical ownership claims.

| Existing owner / size | Exact kind transition | Admitted evidence and unchanged limits |
|---|---|---|
| `00100cd8 / 144` | `function -> library` | Gap-free ordered suffix of **17 complete** printf collection sections, placed from the original `00100ca8` collection call and actual preceding geometry. Aggregate provenance, not one invented 144 B vendor function/alias. |
| `00107078 / 72` | `function -> library` | `_get_lc_numeric`, actual `lc_numeric_c.o`, complete 72 B code **and** 28 B descriptor, all same-object REL32 relations; original caller fixes code placement. Not RTTI/type-query identity. |
| `0012d0c4 / 40` | `function -> assembly` | Exact literal-installed LR continuation `0012d148` and terminal non-call indirect transfer; independently decoded original incoming call. Specific control-transfer certificate, not a failed-C or short-wrapper heuristic. |
| `00134128 / 72` | `function -> library` | `_get_lc_ctype`, `lc_ctype_c.o`, complete 72 B code **and** 272 B descriptor, all same-object REL32 relations and original caller. |
| `001565f4 / 244` | `function -> library` | Three complete `btod.o` sections, 56/84/104 B, placed by original Thumb call and actual inter-section branch relocations. Catalogue aggregate remains 244 B. |
| `001566e8 / 712` | `function -> library` | Actual 712 B `__btod_div_common` declaration in a complete **840 B** section. Its 128 B constant tail is comparison-only and remains separate rodata. |
| `00156a30 / 220` | `function -> library` | Complete `_e2e` section plus independent complete `_btod_ediv` wrapper. Reuses only the existing `inline_arm_call` decision for the constant-zero original callee; no `__ieee_status` target/API alias is authenticated. |
| `00181e94 / 10` | `function -> library` | Actual global `__read_errno` declaration in the complete 22 B `_rserrno.o` section, independently placed from the already authenticated 12 B `__set_errno` at offset10; all eight retained distinct setter calls freshly decoded. |

Every existing entry, code/pool/literal range, size and physical function span is
unchanged. Ordinary denominator becomes **4,571 owners / 1,121,674 B** from
4,579 / 1,123,188 B. Seven library and one assembly owner are added to the
classified-kind counts, not to the total owner count. No names are added to
`config/names.json` and no accepted ledger is edited.

No remaining triage row is silently upgraded. In particular the preceding
`00100ca8 / 48 B` printf collection owner still depends on the **unadmitted
conditional-successor branch-to-NOP** rule and a missing handler identity.
Generic zero/status/accessor and other Thumb template collisions remain
ordinary/evidenced residue. No Thumb profile is frozen by v13.

## Remaining residue — exact unknown gaps

`research/catalogue_v13_residue.json` binds each exact original interval hash and
missing prerequisite. They remain `unknown/null`; anomaly holes are a subset of
these bytes, not additional unknown bytes.

| Start / size | Why complete evidence still does not close it |
|---|---|
| `00100020 / 4` | No covering complete section or independent entry/data/fill witness; both endpoints already word-aligned. |
| `0010012e / 48` | Complete candidate 66 B `arm_exceptions_mem.o` and real 26/22 B sibling declarations are known, but historical initializer/entry authentication is absent. A natural C match for the existing 18 B initializer does not create the missing independent vendor/entry anchor. |
| `001005fa / 16` | Complete relocation-free `_snputc.o` differs at four positions. No equal complete callback or independent entry certificate. |
| `00100942 / 2` | Uniform fill observed, but complete `_printf_char.o` reconstruction still refuses its weak-import field at section+14. |
| `00100b26 / 2` | Corresponding complete `_printf_wchar.o` weak-import field remains refused. |
| `00100d70 / 4` | Already-aligned gap; terminal-return shape is not independent entry/lexical ownership. |
| `0010d3a8 / 8` | No complete original section/independent entry/data witness for the sparse continuation. Retain anomaly **`0010d2b8: unexplained_function_bytes`**; process-exit behavior is insufficient. |
| `001415b4 / 4` | Both adjacent exact alignment-4 sections exclude it; normal-return interpretation supplies no entry. |
| `001a00b4 / 4` | Complete candidate `rt_ctype_table.o` field exists, but context/getter identity and computed-continuation contract remain unproved. Newly authenticating `_get_lc_ctype` alone does not close the other binding/contract. |
| `001c7748 / 20` | No matched complete owning section, independent entry, bounded data or veneer witness; following separate padding is not borrowed. |
| `001cd2e6 / 2` | No complete original data/placement extent witness; adjacent two-byte rodata, subsequent padding and subsequent rodata remain distinct. |

## Immutable generator inputs and public-consumer contract

V11-r2 originally checked frozen descriptors against live `config`, `accept` and
source paths. Legitimate later names/ledger changes, and catalogue adoption
itself, therefore invalidated regeneration. This is addressed **only by the v13
immutable input resolver**, not by changing any ancestor's evidence contents or
hash, dropping checks or accepting a changed snapshot.

`research/catalogue_v13_frozen.json` maps every one of the **13 historically live
inputs / 7,471,417 B** to a retained `research/catalogue_v13_frozen/...` file.
Each retained copy has exactly the original frozen size and SHA-256, including
all five prepared descriptors (`adopted_catalogue`, `ledger`, `names`, `prepare`,
`selection`), historical protection manifests and the two compatibility-proof
sources. Original path labels remain provenance in the generator commitments;
actual reads are resolved only to hash-checked retained copies. Live names,
ledgers, sources and `config/catalogue.json` are not frozen-input dependencies.
Missing snapshots, mutable/escaping paths and changed retained contents refuse.
Git-blob inputs remain immutable blobs.

The builder runs the unchanged r3/r4 negative stage and complete v12 local-entry
consumer **before** v13. V13 commits every semantic baseline catalogue field,
not just counts. Its public consumer reverses only each exact complete padding
range and precisely derived kind/evidence/statistical deltas, then requires the
entire original v12 commitment. It never projects arbitrary ranges by owner,
recomputes away forged statistics, borrows unrelated bytes, or imports copied
positive function records. Earlier v12 behavior remains available unchanged.

New regressions exercise the public producer/consumer with real synthetic image
and ELF/archive inputs: exact positive controls, one disagreeing census value
with rebound hashes, a misaligned successor, bounded-read/entry/owner/relocation/
sized-declaration overlap, a partial/omitted section tail, refused relocations,
extra padding/owner injection, split spans, changed statistics/extents/anomalies,
non-C evidence/entry/extent injection, and retained-path/contents enforcement.
A real synthetic CLI also reproduces the same proposal after live names,
ledger and source changes and copying the proposal into the live catalogue.

## Bounded proof receipts and retained failures

The private complete-section producer's actual r1/r2/r3 exits were **0/0/0**.
R1 used an overwide four-byte model for short Thumb relocations; the public
consumer refused it. R2 corrected the real two-byte `THM_JUMP11/JUMP8` field
widths; r3 additionally bound the newly supplied complete `wcslen` section.
Earlier sources and outputs are historical attempts, not alternative admission
tools. The current public object checker independently reopens and compares all
selected members and every relocation/addend/partition.

The public baseline CLI's r1/r2/r3/r4 exits were **1/1/0/0**. R1 exposed the
protected-entry observation's `target` rather than `address` field. R2 refused
the overwide short-relocation overlap instead of admitting a halfword. R3
rechecked the original 1,842 complete-section bytes; r4 rechecked **1,860
alignment-proof bytes plus 3,022 non-C proof bytes**, with no payload output.

The non-C rederivation's r1 exit was **1**: its incoming-reference assertion
incorrectly required unconditional calls, whereas the trampoline has an actual
conditional direct call. R2 retained and checked the actual condition flag
rather than suppressing it, rederived every selected complete section and
control-transfer certificate, and exited **0**. This changes no linker/endian
rule or entry criterion: a real conditional direct call is still an original
incoming entry witness.

Focused regression receipts r1/r2/r3/r4 have actual exits **1/1/1/0**. The
failures and corrections are retained: misalignment test accidentally changed
the sealed interval list; generic fixtures initially committed the pre-v12
rather than actual post-v12 baseline; old tamper tests still modified now-ignored
live sources/ledgers instead of the retained snapshots. The r4 focused run
exercised **26 tests**, exit **0**, 3.076523 s captured wall. No failed receipt is
rewritten. All exact argv, cwd, source/output hashes and safe error text are under
`scratch/closure/v13-alignment/` and bound by the final check record.

The additional r5 focused run exercised **15 tests**, exit **0**, 1.017589 s
captured wall, including the new uncertified-predecessor control: its gap stays
unknown yet a disagreeing fill value there still vetoes every otherwise complete
external placement. The bounded complete-metadata projection also exited **0**,
1.367378 s, conserving the full v12 commitment while deriving exactly
114 unknown bytes / 11 intervals / one anomaly and the eight non-C deltas.

## Granted full suite and exact correction

Main granted the default-sharded `tools/test_suite.py` invocation in
`full-suite/r2/HEAVY-SLOT-REQUEST.json`. Its **actual exit was 1**:
**912 tests, one skip, one failed module**, 273.54 s captured wall. All other
modules passed. The original-image invariants/stability test, including its two
independent public generations and complete protected-record assertions, passed.

The sole error was the metadata-only synthetic
`CatalogueClosureTests.test_frozen_baseline_and_parent_are_independently_authenticated`:
that fixture has no mutable config/ledger/source files and correctly produced an
empty retained-input map. The new loader incorrectly required a nonempty map.
The correction accepts a dict, including that legitimately empty map; every
actual requested input still requires its exact retained path/size/hash.
No changed live input, missing retained input, mutable path or digest mismatch
is accepted, and no live-path fallback is added.

After releasing the heavy slot immediately with the actual exit, the failed
test and frozen-path regression were run **only as a bounded correction**:
**two tests, exit 0**, 0.093081 s captured wall. This is not relabelled as a
successful full-suite rerun. The original failed suite record and release remain
immutable. The separate generation/merge/adoption sequence requires its own
explicit heavy-slot grant.

Main accepted that failed-suite/bounded-correction disposition with an explicit
condition: an empty map is valid only if no mutable input is consumed. The final
resolver therefore refuses **every unregistered consumed input**, including an
explicit extra protection manifest, not only conventional `config/accept/src`
paths. Explicit extra manifests still work when first registered with an exact
retained copy. No mutable-path fallback remains when the v13 manifest exists.

Two additional regressions prove both directions: empty map with no consumption
passes; empty/partial map plus any attempted unregistered consumption refuses.
They also test registered-copy deletion and digest/size corruption through both
the loader and resolver. Together with the protection CLI tests and original
failed synthetic metadata test, the final bounded correction exercised
**21 tests, exit 0**, 1.649691 s captured wall. Its exact argv is retained in
`full-suite/r2/bounded-correction-r2.json`. Main explicitly prohibited a second
worker full suite and will run the final committed source's full suite in the
adoption merge before recording acceptance. **No final-source full-suite PASS
is claimed here.**
