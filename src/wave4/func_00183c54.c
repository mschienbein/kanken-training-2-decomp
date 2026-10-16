#include "game/archive.h"

unsigned func_00183c54(const struct ArchiveFile *file)
{
    const struct ArchiveBlockReference *reference;
    unsigned short block_type = 0x6801;
    if (file->blocks[0].type == block_type)
        reference = &file->blocks[0];
    else if (file->blocks[1].type == block_type)
        reference = &file->blocks[1];
    else
        reference = 0;
    /* ORIGINAL BUG: a missing file block dereferences a null reference. */
    return reference->offset;
}
