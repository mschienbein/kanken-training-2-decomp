# Ruling: resume interrupted subagents on Codex max (2026-10-17)

**Decision (user):** resume the interrupted work with every subagent, including integrators and
both independent reviewers, using `openai-codex/gpt-6.1-sol:max`.

The user reports that the prior subagents were cut off when Claude usage ran out. Those runs are
interrupted work, not completed author batches or failed acceptance reviews. Resume from saved
sources, match receipts and worktrees; do not discard or repeat completed work.

For this resumed run, the earlier session-only Sonnet fallback is not reinstated. Runtime
inspection found `modelRoles.subagent = openai-codex/gpt-6.1-sol:max` and an empty
`retry.fallbackChains`. All bundled agent types route through that explicit subagent role.
The task API rejects a per-task `model` field for non-seance agents, so the verified role
assignment supplies the selector. If Codex is unavailable, preserve the interrupted state and
report the limitation rather than switch providers without a new user instruction.

This supersedes the fallback routing in `2026-10-17-subagent-model-fallback.md` for the resumed
run, not the historical record of models that already produced evidence. Reviewer independence,
sealed records, checkpoint gates and the source policy are unchanged.
