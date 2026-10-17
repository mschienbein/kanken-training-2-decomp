# Candidate catalogue v7 — VFP arguments and restored-frame leaf setters

## Status and provenance

**Complete candidate; not adopted.** Branch **`catalogue-v7`**, commit **`75a395b`** (`analyze: recover VFP and restored-frame tail boundaries`), based on main **`ff42a86`**. Author: CatalogueV7, OpenAI / openai-codex/gpt-6.1-sol, 2026-10-17.

Tracked edits are only `tools/analyze.py` and `tests/test_analyze.py`, in `scratch/worktrees/catalogue-v7`. Neither `config/catalogue.json` nor `docs/denominator-history.jsonl` was changed anywhere. Main's source, headers, compiler profiles, catalogue and checkpoint state were not modified. The only main-checkout writes are this requested report and `scratch/analyze/catalogue-v7.json`.

Private metadata evidence root **V** = `scratch/worktrees/catalogue-v7/scratch/verification/`. All commands below ran from the v7 worktree with `/home/michael/code/kanken-training-2-decomp/.venv/bin/python`. `orig` and `.cache/toolchain` are symlinks to main's private inputs/toolchain. Evidence contains addresses, sizes, instruction-class facts, hashes and checks, never original bytes or disassembly.

| Input / output | SHA-256 |
| --- | --- |
| Adopted v6 `config/catalogue.json` | `0526a7574676bc4d5a840c5b891564934b36e6d7d8552be880d3d6e2ad9c8323` |
| Candidate `scratch/analyze/catalogue-v7.json` | **`b303210af5d4f8395044bb8cd31508d7ee801b4640704c1253a433e68e1c88d3`** |
| Original image | `a367b5dcd65e3924c93cf3fbaee5d4d40f9c5205d1287912784a974d5e405361` |
| `tools/analyze.py` | `66c75558fe14f622889d88d35b04d210310a63259fef65bb610d25e84ecdef06` |
| `tests/test_analyze.py` | `7e021b25fed8cb34ffc0d569f1b69e782cfda63c92ba4ec22d3cd0f6dddc9054` |

## Executive result and denominator effect

| Measure | v6 | v7 | Delta |
| --- | ---: | ---: | ---: |
| Top-level owners | 4,572 | 4,580 | +8 |
| Callable starts, including entries | 4,596 | 4,604 | +8 |
| Creditable ordinary functions | 4,549 | 4,557 | +8 |
| Creditable extent bytes | 1,129,946 | 1,129,946 | +0 |
| Sum of all extents | 1,136,930 | 1,136,930 | +0 |
| Function-owned bytes | 1,135,444 | 1,135,444 | +0 |
| Instruction bytes | 1,054,598 | 1,054,598 | +0 |
| Decoded instructions | 264,729 | 264,729 | +0 |
| Pool/table bytes | 39,988 | 39,988 | +0 |
| Literal bytes | 40,858 | 40,858 | +0 |
| Unknown bytes | 8,704 | 8,704 | +0 |
| Unknown ranges | 189 | 189 | +0 |
| Anomalies | 83 | 83 | +0 |
| Multi-entry owners | 17 | 17 | +0 |
| Veneers | 6 | 6 | +0 |
| Internal-tail cuts reproduced by the analyzer | 129 | 137 | +8 |

**Eight new boundaries: six VFP pass-through cases and two restored-frame setters.** Eight existing owner records change, eight successor records are added, no owner/start/entry is removed, and **4,564 old owner records remain byte-for-byte identical as JSON objects**. All **4,596 v6 callable starts/entries** survive in the same ISA, and all **129 v6 internal-tail cuts** are reproduced.

The creditable denominator changes from **4,549 to 4,557 ordinary functions**, but stays **1,129,946 extent bytes**. All eight old owners were already ordinary functions; unlike v6's multi-entry reclassification, this revision merely partitions their existing 480 bytes into 176 head bytes and 304 successor bytes. **No executable coverage or accepted C credit is added.** The accepted byte-progress denominator is therefore unchanged. The total all-kind extent sum also remains 1,136,930 bytes.

## 1. Diagnosis: why v6 missed the reported cases

Every reported site was already present in v6's frozen 270-site tail cohort. They were not lost during census discovery, nor rejected for LR/control/literal crossings. Each was retained as **`no_independent_successor_witness`**:

- `func_001d2c44`, NOP `0x001d2c74`, successor `0x001d2c78`, and `func_001d2cf0`, NOP `0x001d2d10`, successor `0x001d2d14`: both suffixes are 12-byte constant-field setters. Their temporary core argument register is initialized before use, so v6's core-only live-argument probe is consistent with a call. Neither suffix was an existing entry, prologue, independent pointer target, nor return-only successor. A genuine post-epilogue position alone was deliberately not an entry witness in v6. V7 adds a separately checked **restored-frame + closed leaf setter** witness, not a blanket “split after epilogue” rule.
- `func_0015e474`, NOP `0x0015e47c`, successor `0x0015e480`; `func_0015e5c4`, NOP `0x0015e5d0`, successor `0x0015e5d4`; `func_0015e684`, NOP `0x0015e694`, successor `0x0015e698`: the suffixes consume pass-through floating arguments while using an untouched object pointer. V6 only tracked `r1-r3,r12`, not VFP argument lanes. Moreover, Capstone 5.0.7 omits VSTM/VLDM register-list operands from `regs_access()` and gives those explicit operands access=0. Merely adding VFP register IDs to v6's set would still miss the three reported register-list setters.
- The whole-image census finds the same VFP omission at `func_0015274c`, `func_0015e49c` and `func_0015e624`. These three additional boundaries are included, not left as reported-case-only fixes.

The suggested second blind spot—real LR restores outside the census's POP/LDM textual heuristic—is handled generally as well. An independently matched immediate stack-slot LR load followed by balanced SP restoration can now enter the candidate inventory even if its frozen census class is `call:in_frame`. **There are no such extra candidates in this image**: all 505 excluded in-frame MOV-NOPs were examined, all remain in-frame, and all 505 pass the expanded as-if-call probe. A hand-authored synthetic non-writeback stack-load regression exercises this previously unreachable path.

## 2. General rules and retained guards

### VFP live-argument probe

The 12-instruction straight-line probe now tracks `s0-s15` / `d0-d7`, normalizes each D register to its overlapping S lanes, and explicitly accounts for VSTM/VPUSH reads and VLDM/VPOP writes. A D write initializes both halves; an S write does not falsely initialize the other half of a D register. `s16-s31` / `d8-d15` remain callee-saved and are not argument violations.

**VFP results are not mistaken for live arguments.** The probe retains two alternative legal result classes: core `r0`, or the conservative VFP HFA result block `d0-d3` (`s0-s7`). A read of `s0` alone is therefore not sufficient. An untouched `r0` object pointer together with pass-through VFP lanes is incompatible with either single result class and is the observed witness at all six newly split VFP sites. Lanes beyond the VFP result block are also eligible argument witnesses. Predicated writes cannot initialize a register unconditionally. A flags-only dependency remains non-evidence.

### Restored-frame leaf setter witness

The new witness requires a real saved LR stack slot, an unconditional restore from the matching slot, balanced SP at the NOP, and no intervening LR overwrite. Stack LR is kept separate from the old census's textual LR flag. Immediate stack loads missed by the census need a matched saved slot; arbitrary loads into LR do not clear the saved-frame guard. Conditional restores do not clear it.

The successor must be a bounded, fully owned, closed leaf setter (at most 12 instructions including its terminal return), ending exactly at the existing owner end. It must have a field store through the unchanged `r0` base, initialize its temporary core registers, and have no calls, branches, conditional operations, literal references, unknown holes, stack/LR dependencies, callee-saved register dependencies, indexed/writeback memory accesses, or extra trailing body. A setter-shaped suffix without a genuine restored frame is still not an independent section witness.

### No false-split guard was relaxed

The exact executed unconditional armlink MOV-NOP encoding remains mandatory; architectural NOP hints, predicated MOVs, data and the unexecuted switch slot are not boundaries. The final-pass design, independent successor requirement, direct forward/backward and address-switch crossing gates, PC-relative data ownership/conflict gates, saved-stack-LR gate, extra-interior-entry gate and full-record protected-owner gate remain active. Candidate admission is extended only for independently proven actual restores outside the frozen census; no actual-image candidate required that extension.

No rediscovery or global refinement runs after partitioning, no instruction/data byte is claimed or moved, and no old entry is dropped. The original census labels are retained unchanged for comparison with v6. The new `linker_nop_inventory` additionally records **every one of the 776 MOV-NOP words**, including explicit excluded in-frame and non-executed positions, so the tail cohort is no longer the only visible audit population.

## 3. Complete table of every newly changed extent

Each row replaces exactly one v6 owner with the listed head and successor. All 16 resulting records are ordinary ARM functions. No v6 secondary entry is promoted at these eight sites; all successor addresses are new callable starts. Every row has **zero crossing branches, zero crossing data references and no still-saved LR**.

| Old owner | Old bytes | Tail NOP | New head bytes | Successor | Successor bytes | Independent evidence |
| --- | ---: | --- | ---: | --- | ---: | --- |
| `0x0015274c` | 32 | `0x00152754` | 12 | `0x00152758` | 20 | AAPCS live arguments: `r0, s0` |
| `0x0015e474` | 40 | `0x0015e47c` | 12 | `0x0015e480` | 28 | AAPCS live arguments: `r0, s0, s1` |
| `0x0015e49c` | 108 | `0x0015e4a4` | 12 | `0x0015e4a8` | 96 | AAPCS live arguments: `r0, s0` |
| `0x0015e5c4` | 44 | `0x0015e5d0` | 16 | `0x0015e5d4` | 28 | AAPCS live arguments: `r0, s0, s1, s2` |
| `0x0015e624` | 96 | `0x0015e630` | 16 | `0x0015e634` | 80 | AAPCS live arguments: `r0, s0` |
| `0x0015e684` | 48 | `0x0015e694` | 20 | `0x0015e698` | 28 | AAPCS live arguments: `r0, s0, s1, s2` |
| `0x001d2c44` | 64 | `0x001d2c74` | 52 | `0x001d2c78` | 12 | Matched LR stack-slot restore at `0x001d2c70`, balanced SP, closed leaf setter |
| `0x001d2cf0` | 48 | `0x001d2d10` | 36 | `0x001d2d14` | 12 | Matched LR stack-slot restore at `0x001d2d0c`, balanced SP, closed leaf setter |

This is the full v6→v7 extent-change table: **no other old extent changes**. Full before/after records (including exact code ranges and changed-field lists) are in `V/verification-v7.json` under `changed_records`; all 137 inherited/new tail cuts and all 270 tail dispositions are also embedded in the candidate JSON as `linker_tail_extent_splits` and `linker_tail_inventory`.

The two setter restores are observed at `0x001d2c70` and `0x001d2d0c`. Their complete old extents are preserved by the 52+12 and 36+12 partitions respectively. The reported float extents become 12+28, 16+28 and 20+28 bytes, rather than padding author source to absorb the following setter.

## 4. Protected accepted/c004/c005 functions

Protection binds full adopted owner records, not just names, addresses or sizes. The generation CLI supplied all three manifests; the default accepted manifest was retained. An eligible proposal that changes a protected owner is withheld, and inability to reproduce an adopted protected record fails closed. Adoption-safe regeneration of newly split parts is covered by both the old and new-evidence regressions.

| Manifest | Functions | Extent bytes | Whole-record equality | Changed address/size/record list |
| --- | ---: | ---: | --- | --- |
| `config/matches.json` | 103 | 5,056 | 103/103 | `[]` |
| `config/candidates/c004.json` | 196 | 2,564 | 196/196 | `[]` |
| `origin/c005-wave5:config/candidates/c005-wave5.json` | 37 | 1,852 | 37/37 | `[]` |
| **Total / unique addresses** | **336** | **9,472** | **336/336 ordinary and identical** | **`[]`** |

C005 provenance is exactly `git show origin/c005-wave5:config/candidates/c005-wave5.json`, with `origin/c005-wave5` resolving to **`01c43541b0c356d3080a67e4a6950324b4fea5e8`**. The extracted manifest is frozen at `V/c005-wave5.json`; no tracked candidate configuration was written. All 336 individual source/function/address/v6-size/v7-size/equality records are in `V/verification-v7.json` under `protected_rows`. There are **zero protected-owner conflicts and no size-change exceptions**.

| Protected input | SHA-256 |
| --- | --- |
| `config/matches.json` | `1641e6e3401711ba082de514182045915d888db764ef3697acdfae1e1d54bd1a` |
| `config/candidates/c004.json` | `2626c2911fae00d433e162c87b8835cac0415fd0baf6e406610cd96666130af8` |
| `origin/c005-wave5:config/candidates/c005-wave5.json` | `e7ea894fe11ae32655d884153bd24034ddd21e886e7e24490cd9e74f2c71fb33` |

The protected check is catalogue preservation, not a fresh protected-source compile or accepted full-image build. No such build or acceptance step was run.

## 5. Systematic whole-image census

The candidate inventory accounts for every aligned MOV-NOP word in the whole text segment, not only sites already considered tail-like. Its site list was independently compared with a complete raw-word address scan. The old 270 `(site, census_class)` pairs are identical; the full class counts remain 505 in-frame, 23 after-epilogue, 20 before-prologue, 79 falling into entries, 54 in-leaf, 86 NOP entries and eight outside-catalogue. The unexecuted switch slot is separately recorded, not forced into a tail class.

| Disposition | v6 | v7 | Change |
| --- | ---: | ---: | ---: |
| Split internal tail boundary | 129 | 137 | +8 |
| Already separate | 86 | 86 | 0 |
| Crossing control flow | 31 | 31 | 0 |
| Crossing literal/data ownership | 1 | 1 | 0 |
| No independent successor witness | 15 | 7 | -8 |
| Unowned tail without an established head | 8 | 8 | 0 |
| In-frame call population, not eligible as tails | 505 | 505 | 0 |
| Unexecuted switch slot, not code | 1 | 1 | 0 |
| **Whole text MOV-NOP population** | **776** | **776** | **0** |

All **505 in-frame sites remain as-if-call consistent even with VFP tracking**; none acquires a restored-frame proof. The **21 textual-LR versus actual-stack-LR disagreements** retained in v6 are still all withheld by crossing-control flow, with the true stack LR preserved in their evidence. The guard regression covers both immediate and separated LR-base LDM forms.

The seven still-unwitnessed sites are deliberately unchanged (all have consistent probes and no other independent start evidence):

| Retained owner | Tail NOP | Unproved successor | Frozen census class |
| --- | --- | --- | --- |
| `0x0010bed4` | `0x0010bef0` | `0x0010bef4` | `tail:after_epilogue` |
| `0x00131948` | `0x0013194c` | `0x00131950` | `tail:in_leaf` |
| `0x001c5630` | `0x001c563c` | `0x001c5640` | `tail:after_epilogue` |
| `0x001d033c` | `0x001d0344` | `0x001d0348` | `tail:in_leaf` |
| `0x001d222c` | `0x001d2230` | `0x001d2234` | `tail:in_leaf` |
| `0x001efcd0` | `0x001efcd4` | `0x001efcd8` | `tail:in_leaf` |
| `0x001f4784` | `0x001f4788` | `0x001f478c` | `tail:in_leaf` |

The unchanged 31 control-crossing sites, one literal-crossing site and eight unowned tails remain individually recorded in `linker_tail_inventory`. There is no inferred fallback split for any of them. Only the eight rows in §3 change their prior disposition.

## 6. Preservation checks

**All 27 metadata/preservation checks pass.** `validate_catalogue` returns `[]`; the complete map still tiles all **1,327,104 mapped bytes**. Function code, pool and literal coverage are identical interval sets, as is every map range-kind coverage set. Unknown bytes/ranges and all 83 anomaly records are exactly unchanged, not merely equal in count. All 17 literal-conflict records are unchanged.

All **19,991 physical direct-call tuples** and **26,274 physical branch tuples** compare identically, including sites, transfer class, target and ISA. The new logical tail count increases by eight, but no instruction graph changes. Every modified head/successor is larger than eight bytes, and all other function records are identical; thus none crosses the linker's 4/8-byte tiny-callee eligibility threshold. This is static preservation evidence, not a full acceptance build.

| Check | Result |
| --- | --- |
| `all_v6_entries_preserved` | PASS |
| `all_v6_owners_preserved` | PASS |
| `anomalies_unchanged` | PASS |
| `census_pairs_unchanged` | PASS |
| `changed_parts_above_inline_size_limit` | PASS |
| `code_ranges_coverage_unchanged` | PASS |
| `data_map_coverage_unchanged` | PASS |
| `extent_sum_conserved` | PASS |
| `function_map_coverage_unchanged` | PASS |
| `image_identity` | PASS |
| `in_frame_probe_consistent` | PASS |
| `literal_conflicts_unchanged` | PASS |
| `literals_coverage_unchanged` | PASS |
| `lr_base_ambiguities_withheld` | PASS |
| `old_splits_preserved` | PASS |
| `only_new_split_owners_modified` | PASS |
| `only_new_successors_added` | PASS |
| `padding_map_coverage_unchanged` | PASS |
| `physical_edge_tuples_unchanged` | PASS |
| `pools_coverage_unchanged` | PASS |
| `protected_functions_unchanged` | PASS |
| `rodata_map_coverage_unchanged` | PASS |
| `segment_padding_map_coverage_unchanged` | PASS |
| `segments_unchanged` | PASS |
| `unknown_map_coverage_unchanged` | PASS |
| `validated` | PASS |
| `whole_text_nop_census_complete` | PASS |

## 7. Tests, commands and exits

Only **`tests.test_analyze`** was run: **109 tests, OK, exit 0**, unittest elapsed **41.671 s** (captured process wall time **41.744 s**). Thirteen new test methods cover VFP scalar/list reads, D/S aliases and partial initialization, HFA/scalar return exceptions, callee-saved VFP lanes, saved LR, non-tail inventory, crossing branches and literals, flags-only dependence, short setters, genuine/balanced versus conditional/wrong-slot/clobbered restores, outside-census actual restores, arbitrary/oversized suffix rejection, and full-record/adoption-safe protection. The original-image regression explicitly checks every reported case plus all three systematically found VFP siblings, exact eight-owner/eight-successor changes, all v6 entries, complete 776-word census, old coverage and deterministic regeneration. The existing guard regressions remain unchanged and pass.

Commands (working directory `scratch/worktrees/catalogue-v7`):

```sh
/home/michael/code/kanken-training-2-decomp/.venv/bin/python -m unittest tests.test_analyze -v
/home/michael/code/kanken-training-2-decomp/.venv/bin/python tools/analyze.py catalogue --protect-manifest config/candidates/c004.json --protect-manifest scratch/verification/c005-wave5.json --out /home/michael/code/kanken-training-2-decomp/scratch/analyze/catalogue-v7.json
/home/michael/code/kanken-training-2-decomp/.venv/bin/python scratch/verification/check_catalogue_v7.py
```

| Command | Exit | Wall seconds | Evidence |
| --- | ---: | ---: | --- |
| Analyzer test module | 0 | 41.744 | `V/tests-v7-first.json` |
| Final protected catalogue generation | 0 | 20.958 | `V/catalogue-v7-command.json` |
| Metadata/census/protection audit | 0 | 2.233 | `V/verification-v7-command.json`, `V/verification-v7.json` |

There were no failed test runs. An earlier eight-split draft generation also exited 0 and is retained as `V/catalogue-draft-command.json`; the final candidate adds the complete non-tail census metadata. No analyzer/test source was changed after the passing test run. No full suite, `tools/build.py`, match compile, formatter, denominator recording, prepare/finalize, main commit or push was run.

## 8. Retained evidence hashes

Paths are relative to V unless otherwise specified.

| Artifact | SHA-256 |
| --- | --- |
| `check_catalogue_v7.py` | `c882ea1a04d283e62e4719ddb90a314022c6b51299d9ff5c101409f4a9eb8e4e` |
| `verification-v7.json` | `c32bfeb3e8f705082a7b659aa3f32e899baa9eeee7c4916eb162115350f140ba` |
| `tests-v7-first.json` | `3ddc5209eca329638fc899182106bea78b6734415a443687f633676d61b2f78c` |
| `catalogue-v7-command.json` | `d5877e7e9ab790df4d9942fbcdb2e1a1a442fd55d1b19c518f3e6456a214efe5` |
| `verification-v7-command.json` | `aa76f20e4cf0afaf68dbe42bc24f9af7a4bec6c406ded98263b3127da684b17e` |

## Hand-off qualification

The assigned analyzer fix, regression module, committed branch, candidate and complete extent/protection report are delivered. **No guard was relaxed.** [INFERENCE] The eight recovered boundaries correspond to separate original ARMCC/armlink tail-calling sections; original source spelling and author interface semantics are not recovered by catalogue analysis alone. No new C/C++ match or accepted progress is claimed. Adoption, denominator history, main integration and checkpoint builds remain integrator-owned actions.
