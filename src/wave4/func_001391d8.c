#include "game/archive.h"

int func_001391d8(const struct ArchiveReader *reader)
{
    if (reader->initialized) {
        const struct ArchiveFile *file = reader->file;
        unsigned offset = func_00183c54(file);
        const struct ArchiveBlockHeader *block = (const struct ArchiveBlockHeader *)
            ((const unsigned char *)file + (int)offset);
        if (block->signature == 0x54415743)
            return 1;
    }
    return 0;
}
