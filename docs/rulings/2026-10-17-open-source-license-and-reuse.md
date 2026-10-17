# Ruling 2026-10-17 — GPL-3.0 license; reuse existing public work and tools

**Decision (user, 2026-10-17):** reuse public decompilation work and tools built for this kind
of project wherever possible; licensing this project as fully open source is acceptable. The user
chose "reuse under GPLv3" for the public NW4C decompilation.

**Effect:**

- The repository is licensed GPL-3.0-or-later (`LICENSE`), so GPL-3.0 sources such as
  [ctrdecomp/NintendoWare4Ctr](https://github.com/ctrdecomp/NintendoWare4Ctr) can be adapted
  with attribution, pinned upstream commits and modification notices (`THIRD_PARTY.md`).
- Reused source earns credit only like any other source: it must match under the project's
  compiler profile, pass the interface gate and two independent reviews, and be listed with its
  provenance in the checkpoint's known issues.
- Public code without a license grant is not copied (default copyright applies); public tools are
  used per their own licenses and are not vendored into the repository unless compatible.
- No ROM data, SDK files or compilers enter the repository; the license does not cover them.

**Context:** a 2026-10-17 fit test compiled five NintendoWare4Ctr functions (NW4C 3.7.4 upstream;
the game links 3.7.5) under the frozen `game` profile and all five matched the game's bytes.
