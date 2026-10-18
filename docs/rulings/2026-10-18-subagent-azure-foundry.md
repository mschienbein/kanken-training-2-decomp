# Ruling: subagents move to gpt-6.1-sol on Azure Foundry (2026-10-18)

**Decision (user):** after every Codex subagent hit the ChatGPT plan usage limit ("try again in
~9810 min"), switch subagents to the same model, `gpt-6.1-sol`, served through Azure Foundry, at
maximum reasoning: role selector `azure-foundry/gpt-6.1-sol:max`.

Runtime: `modelRoles.subagent` now resolves to `azure-foundry/gpt-6.1-sol:max`. All bundled agent
types (task, scout, reviewer, security-reviewer, sonic, seance) route through that role. A
read-only smoke subagent completed on the new route before work resumed.

The interrupted runs are interrupted work, not completed batches or failed reviews (state:
`scratch/integrator/recovery-codex-limit-20261018.json`, private). Resume from saved sources,
receipts, worktrees and integration notes; do not discard or repeat completed work. The model is
unchanged, so reviewer-family rules and author provenance are unaffected: records name the model
`gpt-6.1-sol` with the serving route. This supersedes the provider routing of
`2026-10-17-subagent-codex-resume.md` from this point; no other gate or policy changes.
