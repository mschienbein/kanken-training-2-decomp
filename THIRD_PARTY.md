# Third-party sources

Source reused from other public projects, with its license and pinned upstream commit. Every
reused function still has to match this game's bytes under the project's compiler profile and
pass the normal checkpoint review before it is credited.

| Upstream | License | Pinned commit | Used for | Files here |
|---|---|---|---|---|
| [ctrdecomp/NintendoWare4Ctr](https://github.com/ctrdecomp/NintendoWare4Ctr) | GPL-3.0-only | `15ddb5632db5a6f0d2f46e3827fc78df468684bd` (2026-10-11) | NW4C layout, sound, font and graphics code (upstream 3.7.4; target 3.7.5) | `src/nw4c/`, curated `include/nw/`, project-owned `include/nn/` adapters and `include/preinclude.h`; per-file notices, `src/nw4c/LICENSE.txt`, `PROVENANCE.json` and `PROVENANCE-round3.json` preserve supplied origin/modifications; integration hashes are recorded with c007. |

The public Khronos GLES declaration/constant subsets in `include/GLES2/` use the
MIT license from [OpenGL-Registry](https://github.com/KhronosGroup/OpenGL-Registry),
including GLES2 declarations and GLES/GL legacy constants attested in
`src/nw4c/FACTS-KHRONOS.json`. Its license text is preserved in
`src/nw4c/LICENSE-KHRONOS.txt`. Lowercase `include/gles2/` headers are
project-owned include adapters, not an OpenGL implementation.
Policy: only sources whose license is compatible with GPL-3.0 are copied; attribution, the
upstream license and modification notices stay with the copied code. Public repositories without a
license grant (inspected 2026-10-17: standalone nnsdk, LibMessageStudio, 3DS sead) are not copied.
