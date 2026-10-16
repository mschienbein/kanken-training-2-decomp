#include "game/archive.h"

const struct ArchiveBlockHeader *func_0014a2f0(const struct ArchiveFile *file)
{
    const struct ArchiveBlockReference *reference;
    if (file->blocks[0].type == 0x6800)
        reference = &file->blocks[0];
    else if (file->blocks[1].type == 0x6800)
        reference = &file->blocks[1];
    else
        reference = 0;
    /* ORIGINAL BUG: a missing information block dereferences a null reference. */
    return (const struct ArchiveBlockHeader *)((const unsigned char *)file + reference->offset);
}
