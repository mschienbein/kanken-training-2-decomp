# Status

Current entry on top; older entries are kept as history.

## 2026-10-15 — harness bootstrap (H-gates in progress)

- Inputs: encrypted cartridge dump (sha256 `23ba7aaf…`) and a decrypted copy (`5874c3af…`) both
  import to identical outputs. Decryption uses pyctr 0.7.6 with the user's own `boot9.bin`; every
  byte is checked against the NCCH hash chain (exheader hash, ExeFS superblock and per-file hashes,
  RomFS superblock hash).
- `code.bin`: 1,327,104 B (`a367b5dc…`); text 0x1175F4 B @ 0x100000, rodata 0x1E0B8 B @ 0x218000,
  data 0xC908 B @ 0x237000, BSS 0x29F9E4 B. No CROs. RomFS: 46 files, 115.8 MB (archives
  `arcApp*Dat.bin`, `arcJitenPrmDat.bin`, `arcSikenPrmDat.bin`, fonts, audio, shaders).
- **ExeFS `.code` recompression is byte-exact:** our backward-LZ77 encoder (3dstool's greedy policy)
  turns `code.bin` back into the stored `.code` byte for byte.
- SDK tags from the plaintext NCCH plain region: CTR-SDK 11.5.2, NW4C 3.7.5, PUX HWR 2.0.1.
- Code is mostly ARM with heavy VFP use; Thumb is rare. No assert strings or source paths survive.
- Next: function catalogue, split/relink roundtrip, compiler identification, match loop.

Accepted progress: none (0 functions).
