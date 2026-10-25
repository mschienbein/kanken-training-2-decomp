# Ruling 2026-10-25: documented unresolved catalogue bytes; Thumb library candidates wait for ARM Compiler 4.1

## Unresolved unknown bytes and anomalies

**Decision (user, 2026-10-25).** For the definition of done, a catalogue unknown range or anomaly counts as **resolved** when it is documented in STATUS's residue section as irreducible, together with the evidence of what was examined and why no reviewed rule classifies it (padding/fill, owned data, missed function, owner extension). No byte is credited or reclassified by this ruling.

**Context.** Catalogue v15 (proposal; research/catalogue_v15_unknowns.json, docs/catalogue/REPORT-v15.md on branch catalogue-v15) proves 16 of the 114 unknown bytes to be the Thumb library function `_snputc`. The remaining 98 B in 10 ranges of 2–20 B, plus the anomaly at 0x0010d2b8, have no evidence that supports a classification.

**Conditions.** Each range is listed with its address interval, size, the evidence path and a one-line reason. New evidence reopens a range through a reviewed catalogue revision.

## Thumb library candidates

**Decision (user, 2026-10-25).** The Thumb functions that look like C runtime library routines but match none of the local ARM Compiler 5.04/5.06 libraries stay **awaiting user input** until the ARM Compiler 4.1 runtime libraries are available. These are the 8 residue rows classed awaits_user_ruling (360 B) plus func_00100c9c (2 B; scratch/integrator/USER-RULING-HOLDS.json). They are not authored as C, and no Thumb compiler profile is frozen for them.

**Next step.** The user is downloading ARM Compiler 4.1 from Arm's legacy page (ACOMP5; build 894, and build 791 if listed). The libraries go under ~/code/3ds-experiments/inputs/. An exact archive identity then classifies a function as library code through a reviewed catalogue revision. A function that still does not match is revisited with the user.
