# Subagent concurrency — 2026-10-20

The user authorized resuming the interrupted decompilation work with **32 subagents maximum**.
This supersedes the previous 64-slot configuration and 62-worker rotation target.

The limit includes authors, integration workers, research helpers and independent reviewers.
The integrator fills only released capacity and resumes preserved work rather than restarting
completed batches. The saved runtime limit and private rotation target are both 32.

The model ruling in `2026-10-18-subagent-azure-foundry.md` is unchanged:
`azure-foundry/gpt-6.1-sol:max` for every subagent, including both independent reviewers.
The single-writer rule, serialized heavy jobs, frozen gates and review independence are unchanged.
