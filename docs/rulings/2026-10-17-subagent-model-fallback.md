# Ruling: subagent model fallback on Codex usage limits (2026-10-17)

**Decision (user, 2026-10-17):** all subagents — authors, integrators and reviewers — keep running
`openai-codex/gpt-6.1-sol` (see `2026-10-17-reviewer-model-family.md`). If the Codex usage quota runs
out, subagents switch to `anthropic/claude-sonnet-5-5` at `xhigh` thinking until Codex usage comes back.
The user stated Codex usage was still available at the time; the fallback is a precaution.

**Mechanism (session configuration, not persisted by the user's choice):**

- `retry.fallbackChains`: `subagent` and `openai-codex/*` → `anthropic/claude-sonnet-5-5:xhigh`.
- `retry.fallbackOn: usage-limit` — only a usage/quota limit switches models (user's choice); other
  provider failures retry on Codex.
- `retry.fallbackRevertPolicy: cooldown-expiry` — work returns to Codex when its cooldown ends.

**Review independence:** reviewer independence still comes from separate fresh agents with sealed
records. If a checkpoint's two reviews end up on different model families because of the fallback,
each review record states the model that produced it; this does not lower the review bar.
