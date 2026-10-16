# 漢検トレーニング2 (Kanken Training 2) — matching decompilation

A work-in-progress matching decompilation of **漢検トレーニング2** (Imagineer, 2017, Nintendo 3DS,
Japan, `CTR-P-B2KJ`). The goal is C/C++ source that the original ARM compiler turns back into a
byte-identical `code.bin`, and then a native study app for Windows and iPad built on the recovered
behaviour.

<!-- ByteMatch badges go live once the repository is public and registered (see docs/BYTEMATCH.md):
[![Code](https://bytematch.dev/mschienbein/kanken-training-2-decomp.svg?measure=matched_code_percent&label=Code)](https://bytematch.dev/mschienbein/kanken-training-2-decomp)
[![Functions](https://bytematch.dev/mschienbein/kanken-training-2-decomp.svg?measure=matched_functions&label=Functions)](https://bytematch.dev/mschienbein/kanken-training-2-decomp)
-->

**Status:** harness bootstrap. No game code is decompiled yet. See [`docs/STATUS.md`](docs/STATUS.md).

## What you need

- Your own dump of the cartridge. This repository contains no ROM data, no extracted assets, no
  Nintendo SDK, and no compiler.
- Either the encrypted cartridge dump plus a `boot9.bin` dumped from your own console (GodMode9), or
  the same dump with its content decrypted (GodMode9 "decrypt"). The accepted dumps are pinned by
  hash in [`config/title.json`](config/title.json).
- macOS (Apple Silicon) or Linux, Python 3.12, [`uv`](https://docs.astral.sh/uv/).

## Quick start

```sh
uv sync
# Encrypted dump + your console's boot9:
.venv/bin/python tools/rom.py import --rom /path/to/kanken2.3ds --boot9 /path/to/boot9.bin --romfs
# Or a decrypted dump:
.venv/bin/python tools/rom.py import --rom /path/to/kanken2-decrypted.3ds --romfs
.venv/bin/python tools/rom.py verify
```

`import` checks every byte against the NCCH hash chain and the pinned hashes, then writes the private
inputs to the git-ignored `orig/` directory:

| File | Contents |
|---|---|
| `orig/exheader.bin` | Extended header (code-set layout, BSS, dependencies) |
| `orig/exefs/code.blz` | ExeFS `.code`, backward-LZ77 compressed, as stored |
| `orig/code.bin` | Decompressed executable: text @ `0x100000`, rodata @ `0x218000`, data @ `0x237000` |
| `orig/romfs/` | RomFS files (`--romfs`) |
| `orig/manifest.json` | sha256 of every extracted file |

## The target

| Item | Value |
|---|---|
| `code.bin` | 1,327,104 B, sha256 `a367b5dcd65e3924c93cf3fbaee5d4d40f9c5205d1287912784a974d5e405361` |
| text / rodata / data | 0x1175F4 / 0x1E0B8 / 0xC908 B; BSS 0x29F9E4 B |
| Modules | No CROs; all code is in `code.bin` |
| SDK tags | CTR-SDK 11.5.2, NW4C 3.7.5 (layout, sound, font), PUX HWR 2.0.1 (handwriting recognition) |

## Rules

- Never commit ROM bytes, decrypted or extracted data, disassembly of original code, SDK files,
  compilers, or console keys. The repository history must stay publishable.
- "Matched" means the function's bytes come from compiled C/C++ in a full `code.bin` build that is
  byte-identical to the original. A split-and-relink of original bytes, an objdiff score, or a
  plausible decompilation are not matches.
- See [`AGENTS.md`](AGENTS.md) for the agent workflow and [`docs/ACCEPTANCE.md`](docs/ACCEPTANCE.md)
  for the gates.
