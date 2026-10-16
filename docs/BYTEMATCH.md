# ByteMatch progress tracking

[ByteMatch](https://bytematch.dev) (the decomp.dev replacement) tracks matching progress from objdiff
reports uploaded by GitHub Actions. **Live since 2026-10-16:**
https://bytematch.dev/mschienbein/kanken-training-2-decomp (project id 1421490781).

## Registration record (2026-10-16)

- Repository made public after a full-history audit (no ROM/asset/compiler paths, no disassembly
  or byte dumps in any of 795 blobs, no secrets, one noreply author identity).
- Added via Manage → Submit a project (owner `mschienbein`). Saved settings, verified through
  `/api/v1/projects?q=kanken`: name 漢検トレーニング2 (Kanken Training 2), short name `kanken2`,
  platform `3ds`, game 漢検トレーニング2, franchise 漢検 (Kanken), release year 2017, default branch
  `main`, workflow `progress.yml`, artifact pattern default, default version `jp` (auto).
- Backfill imported 44 history points from successful `main` runs; the latest ingested commit is
  `bbf6e7a` (103 / 4,350 functions, 0.455%). Later runs failed the ledger check while c004's
  revisions are pending, so the dashboard updates again once `c004-r2` finalizes.
- Not done: GitHub App (optional: instant ingestion and PR comments; without it new runs are polled
  every few minutes) — install it for this repository only if wanted; icon/cover art (SteamGridDB
  has no entry for this game).

## What is in place

| Piece | File | Notes |
|---|---|---|
| Report generator | `ci/progress_report.py` | objdiff report v2 JSON; needs no ROM. Denominator: `config/catalogue.json`. Numerator: functions credited in `config/matches.json`. Units are 64 KiB text blocks until functions are grouped into translation units. |
| Workflow | `.github/workflows/progress.yml` | Runs on push to `main`, on pull requests and on demand; uploads artifact `jp_report` containing `report.json`. |
| Badges | `README.md` | `measure=matched_code_percent` and `measure=matched_functions`. ByteMatch rejects decomp.dev's `code`/`functions` measure names. |

`config/matches.json` changes only when the integrator records an accepted build
(`tools/build.py` receipt `PASS`), so the dashboard shows accepted progress, never candidates.

## Requirements (read from ByteMatch's docs and source, 2026-10-15)

- **Public GitHub repository.** Self-service registration rejects private repositories ("Only public
  repositories can be tracked"). The repository admin registers it via Manage → Add project.
- History comes only from **successful push runs on the default branch** that upload an artifact
  named `<version>_report` (default pattern `^(?<version>.+)_report$`). Pull-request runs feed PR
  comments.
- Report: objdiff `report.json` (schema version 2) or protobuf.
- Platform id **`3ds`**. Version label **`jp`**.
- The ByteMatch GitHub App (`bytematchdev`) is optional: it gives instant ingestion and PR comments.
  Permissions: Actions read, Metadata read, Pull requests read/write.

## Settings to enter

| Field | Value |
|---|---|
| name | 漢検トレーニング2 (Kanken Training 2) |
| shortName | kanken2 |
| platform | 3ds |
| defaultVersion | jp |
| defaultBranch | main |
| game | 漢検トレーニング2 |
| franchise | 漢検 (Kanken) |
| releaseYear | 2017 |
| workflow (advanced) | progress.yml |

## Going live (needs the owner's approval)

1. Confirm the history holds no ROM-derived bytes, extracted assets, disassembly or compilers
   (`git log --stat`; the `.gitignore` keeps `orig/`, `build/`, `.cache/` and `scratch/` out).
2. Make the repository public.
3. Register on bytematch.dev and fill in the settings above; optionally install the App for this
   repository only.
4. Push to `main`, wait for the `progress` run, and check that
   `https://bytematch.dev/mschienbein/kanken-training-2-decomp.json` shows the pushed commit.
5. Uncomment the badges in `README.md`.

The `bytematch-project-setup` omp skill has the browser steps (settings form, icon/cover art, App
install).
