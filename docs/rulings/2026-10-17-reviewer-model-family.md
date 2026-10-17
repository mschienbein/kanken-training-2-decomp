# Ruling 2026-10-17 — every subagent, reviewers included, runs one model family

**Decision (user, 2026-10-17):** to save cost, every subagent, authors and both checkpoint reviewers included, runs
`openai-codex/gpt-6.1-sol`. The integrator session is Anthropic (Claude Opus), but it never reviews.

**Context:** `AGENTS.md` and `docs/GATES.md` ask for at least one reviewer from a model family other
than the authors' "where possible". The user rules a second family out on cost. This is the setup
every accepted checkpoint already used: all 18 review records from c001 to c004 (`accept/*/reviews/`)
are OpenAI family.

**What keeps reviews independent:**

- Each checkpoint gets two reviewers (`review-<cp>-openai`, `review-<cp>-second`), spawned as
  separate agents with fresh context. Neither is the integrator or an author of the reviewed units.
- Each re-derives every row (`docs/REVIEWING.md`) and seals its record with `tools/accept.py review`
  before reading any other review of the same checkpoint.
- Finalize requires `--min-reviews 2`.

**Effect on records:** review records state `"family": "openai"`. Known-issues files note that review
was single-family under this ruling. The gate design (`docs/GATES.md` v1.8) is unchanged: its
cross-family clause is conditional ("where possible"), and this ruling records why it is not
possible here.
