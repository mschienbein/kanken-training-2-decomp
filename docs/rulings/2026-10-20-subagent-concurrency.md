# Subagent concurrency — 2026-10-20

## Later amendment — 64 maximum

The user raised the ceiling to **64 concurrent subagents**, correcting an initial
63 to 64, and confirmed ample usage and credits. This supersedes the earlier
32-agent ceiling below. The saved runtime ceiling and private rotation target
are now 64, including reviewers, integrators, helpers and authors.

The initial ramp reached 16 active agents. The user then questioned leaving
capacity unused, so the active target was raised to **64** and 48 more independent
author jobs were dispatched. Keep useful slots full; reduce concurrency only if
fresh provider throttling recurs. Earlier Azure Foundry per-minute token failures
at 32 and 16 remain historical evidence, not a permanent cap. Available credits
are separate from per-minute throughput. Resume preserved work and avoid
duplicate assignments; the current queue and any new errors live in private state.

Every subagent remains `azure-foundry/gpt-6.1-sol:max`. Heavy jobs remain
serialized; single-writer ownership, frozen gates and independent reviews are
unchanged.

## Earlier ruling — 32 maximum

The user authorized resuming the interrupted decompilation work with **32 subagents maximum**.
This supersedes the previous 64-slot configuration and 62-worker rotation target.

The limit includes authors, integration workers, research helpers and independent reviewers.
The integrator fills only released capacity and resumes preserved work rather than restarting
completed batches. The saved runtime limit and private rotation target are both 32.

The model ruling in `2026-10-18-subagent-azure-foundry.md` is unchanged:
`azure-foundry/gpt-6.1-sol:max` for every subagent, including both independent reviewers.
The single-writer rule, serialized heavy jobs, frozen gates and review independence are unchanged.
