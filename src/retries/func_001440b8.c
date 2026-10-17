#include "game/archive.h"

static __inline const struct ArchiveBlockReference *
find_block(const struct ArchiveFile *file, unsigned short type)
{
    if (file->blocks[0].type == type)
        return &file->blocks[0];
    if (file->blocks[1].type == type)
        return &file->blocks[1];
    return 0;
}

const struct ArchiveBlockHeader *func_001440b8(const struct ArchiveFile *file)
{
    const struct ArchiveBlockReference *reference = find_block(file, 0x6801);
    /* ORIGINAL BUG: a missing file block dereferences a null reference. */
    return (const struct ArchiveBlockHeader *)
        ((const unsigned char *)file + reference->offset);
}
