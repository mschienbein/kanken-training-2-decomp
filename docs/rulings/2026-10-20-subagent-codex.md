# Codex Sol worker pool — 2026-10-20

The user explicitly replaced Azure Foundry with **Codex `gpt-6.1-sol:max`** and
requested another attempt at **64 active subagents**, until Codex usage runs out.
This supersedes the provider choice in `2026-10-18-subagent-azure-foundry.md` and
the Azure-specific provider wording in the earlier concurrency ruling. The
64-agent ceiling remains unchanged.

The saved `subagent` model role is now `openai-codex/gpt-6.1-sol:max`; all bundled
agent overrides continue to resolve through that role. Main's model and the
saved default role are unchanged. No fallback provider is authorized: on Codex
quota exhaustion, preserve work and pause dispatch rather than silently return
to Azure or switch models.

The Azure 64-worker attempt produced fresh eastus2 token-rate 429 failures after
ten retries. Interrupted authors are requeued; their sources, failed attempts,
receipts and worktrees remain intact. Old Azure workers are stopped before
replacement workers receive their write ownership. Unsealed review material is
preserved, not represented as an accepted verdict. Replacement reviewers must
independently satisfy the unchanged frozen review requirements.

Single-writer ownership, serialized heavy jobs, immutable failures, frozen gates,
privacy boundaries and independent full-scope reviews remain unchanged.
