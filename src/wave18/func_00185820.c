#include "game/sound_format.h"



extern const struct ArchiveBlockHeader *func_00138f38(const struct ArchiveFile *file);

struct WaveSoundReader *func_00185820(struct WaveSoundReader *reader,
                                   const struct ArchiveFile *file)
{
    const struct ArchiveBlockHeader *block;
    reader->file = 0;
    reader->info = 0;
    if (file->signature != 0x44535743 || file->version - 0x01000000u > 0x100)
        return reader;
    reader->file = file;
    block = func_00138f38(file);
    if (!block)
        return reader;
    reader->info = (const struct WaveSoundInfoBlock *)(block + 1);
    return reader;
}
