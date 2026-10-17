# Third-party sources

Source reused from other public projects, with its license and pinned upstream commit. Every
reused function still has to match this game's bytes under the project's compiler profile and
pass the normal checkpoint review before it is credited.

| Upstream | License | Pinned commit | Used for | Files here |
|---|---|---|---|---|
| [ctrdecomp/NintendoWare4Ctr](https://github.com/ctrdecomp/NintendoWare4Ctr) | GPL-3.0 | `15ddb5632db5a6f0d2f46e3827fc78df468684bd` (2026-10-11) | NW4C library code linked by the game (layout, sound, font; upstream targets 3.7.4, the game links 3.7.5) | none yet |

Policy: only sources whose license is compatible with GPL-3.0 are copied; attribution, the
upstream license and modification notices stay with the copied code. Public repositories without a
license grant (inspected 2026-10-17: standalone nnsdk, LibMessageStudio, 3DS sead) are not copied.
