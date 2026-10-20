# Third-party sources

Source reused from other public projects, with its license and pinned upstream commit. Every
reused function still has to match this game's bytes under the project's compiler profile and
pass the normal checkpoint review before it is credited.

| Upstream | License | Pinned commit | Used for | Files here |
|---|---|---|---|---|
| [ctrdecomp/NintendoWare4Ctr](https://github.com/ctrdecomp/NintendoWare4Ctr) | GPL-3.0-only | `15ddb5632db5a6f0d2f46e3827fc78df468684bd` (2026-10-11) | NW4C layout, sound, font, graphics and utility code (upstream 3.7.4; target 3.7.5) | `src/nw4c/`, curated `include/nw/`, project-owned `include/nn/` adapters and `include/preinclude.h`; per-file notices, `src/nw4c/LICENSE.txt`, `PROVENANCE.json` and `PROVENANCE-round3.json` preserve supplied origin/modifications; integration hashes are recorded with c007. c011 additionally adapts `src/checkpoint11/func_001809f4.cpp`, `src/checkpoint11/func_001c7fa0.cpp` and `src/checkpoint11/lyt_delete_animation.cpp`, with hash-bound provenance in `scratch/integrator/c011/`. |
| [devkitPro/libctru](https://github.com/devkitPro/libctru) | zlib | `9b55eda44cf80b971503991e0c78f9bc8fe50425` (2026-09-29) | The 160-sample DSP frame length published by `ndspSync` in `libctru/source/ndsp/ndsp.c`; no implementation copied | Project-owned `NN_SND_SAMPLES_PER_FRAME` declaration in `include/nn/snd.h`; numeric-source hashes in `src/nw4c/FACTS-HEADER-GAPS.json`, license notice in `src/nw4c/LICENSE-LIBCTRU.txt`. |

c011's resumed source milestone additionally adapts the pinned NW4C delay
constructor (`src/checkpoint11/fx_delay-r3.cpp`), sound scalar helpers
(`sound_scale_lookup-r3.cpp`) and native shape setup (`shape_setup-r6.cpp`).
Their GPL-3.0-only notices and modification notes remain in each source;
`include/nw/snd/snd_FxDelay.h` records the measured retail damping-default
correction. Archive, material, DSP and text-flow reconstruction uses the
canonical declarations and retains source-bound matching/provenance receipts
under `scratch/integrator/c011/`; it is not a claim of recovered original
translation-unit boundaries or Nintendo SDK source.
The resumed c011 collection also adapts the pinned native texture image command
(`texture_image_command-r9.cpp`), primitive/index-stream cleanup
(`primitive_set_cleanup-r9.cpp`), group cleanup (`group_ownership_cleanup-r9.cpp`)
and texture setup dispatch (`texture_setup_dispatch-r12.cpp`). Their original
GPL-3.0-only notices remain in each source. The sequence auxiliary command
(`sequence_auxiliary_command-r14.cpp`) retains its supplied reconstruction notice.
Exact author/source/receipt bindings and current explicit v6/L1 destination
evidence are in `scratch/integrator/c011/CONSUMED-RESULTS-r6.json` and
`DESTINATION-MATCH-r16.json`; they grant no new library/data or accepted credit.
The r7 intake additionally adapts the pinned public ResFontBase texture-name
cleanup organization (`src/checkpoint11/intake-r7/nf169/func_0013ea40.cpp`) and
Voice refresh/update helper layering (`intake-r7/nf176/audio_sync.cpp` and
`intake-r7/nf176/func_0014f114.cpp`). Their supplied GPL-3.0-only notices,
modification notes and unrecovered historical-identity qualifications remain
in source. Exact delivery and destination bindings are recorded in
`scratch/integrator/c011/INTAKE-r7.json` and `DESTINATION-CLOSURE-r7.json`;
they grant no library/helper or accepted credit.



The public Khronos GLES declaration/constant subsets in `include/GLES2/` use the
MIT license from [OpenGL-Registry](https://github.com/KhronosGroup/OpenGL-Registry),
including GLES2 declarations and GLES/GL legacy constants attested in
`src/nw4c/FACTS-KHRONOS.json`. Its license text is preserved in
`src/nw4c/LICENSE-KHRONOS.txt`. Lowercase `include/gles2/` headers are
project-owned include adapters, not an OpenGL implementation.

The unaccepted model-qualified `src/models/func_0013f06c/func_0013f06c_unbound.cpp`
adapts `ut_FrameHeap.cpp` from the same pinned NintendoWare4Ctr commit and license.
It selects `FrameHeap::FreeByState` and externalizes two release-mode hooks; their
erased historical identities are explicitly inferred. Its per-file notice is retained,
and the upstream license remains in `src/nw4c/LICENSE.txt`. The hooks and model selection
grant no library/helper credit.
Policy: only sources whose license is compatible with GPL-3.0 are copied; attribution, the
upstream license and modification notices stay with the copied code. Public repositories without a
license grant (inspected 2026-10-17: standalone nnsdk, LibMessageStudio, 3DS sead) are not copied.

## Canonical declaration gaps (2026-10-17)

`src/nw4c/FACTS-HEADER-GAPS.json` records the public sources, private game-code
evidence locations and limitations of three standalone-header prerequisites:

- `include/nn/math.h` supplies only `PivotDirection::PIVOT_NONE`, visible as
  `nw::math::PivotDirection` / `PIVOT_NONE` through the existing NW4C import.
  **[INFERENCE]** The zero default and one-byte field follow the projection
  builder at `func_001246a4`; the public camera-updater initializer supplies the
  name. No licensed defining SDK header was found. Other directions and their
  values are deliberately not declared.
- `include/GLES2/gl2dmpconstants.h` supplies `GL_LUT_TEXTURE0_DMP = 0x6600`.
  **[INFERENCE]** The name comes from public `gfx_RenderContext.h::ToLutTexture`;
  the value follows the game's texture-target dispatch at `func_00184808`,
  independently of the Khronos constants. The token is absent from the public
  registry already surveyed in `FACTS-KHRONOS.json`. This project-owned header
  is GPL-3.0-or-later, not an addition attributed to the MIT registry subset.
- `include/nn/snd.h` supplies `NN_SND_SAMPLES_PER_FRAME = 160` from the pinned
  zlib-licensed libctru source above. **[INFERENCE]** Associating that frame
  length with the SDK spelling is corroborated by `func_001d2790`'s reverb
  delay-length initialization and the public `snd_FxReverb.h` multipliers.

These are minimal declaration/value recoveries, not a copy of Nintendo SDK
headers, proof of unrecovered SDK enum members, or additional function credit.
