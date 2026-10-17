#include "game/archive.h"

extern "C" unsigned func_001476ac(const SoundContainerFile *file)
{
    const struct ArchiveBlockReference *block;
    unsigned short block_type = 0x2001;
    if (file->blocks[0].type == block_type)
        block = &file->blocks[0];
    else if (file->blocks[1].type == block_type)
        block = &file->blocks[1];
    else if (file->blocks[2].type == block_type)
        block = &file->blocks[2];
    else
        block = 0;
    /* ORIGINAL BUG: a missing block still dereferences the null reference. */
    return block->offset;
}
