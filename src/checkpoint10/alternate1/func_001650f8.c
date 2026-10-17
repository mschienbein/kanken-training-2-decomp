#include "game/archive.h"


/* The block search is an outlined leaf shared by the adjacent block queries. */
/* [INFERENCE] Preserve the full incoming tag word. The original core-word
 * comparisons do not establish a narrowing conversion or a historical
 * unsigned-short formal merely because the serialized field is 16-bit. */
__declspec(noinline) const struct ArchiveBlockReference *
func_001650f8(const struct ArchiveFile *file, unsigned type)
{
    int index;
    for (index = 0; index < file->block_count; ++index) {
        if (file->blocks[index].type == type)
            return &file->blocks[index];
    }
    return 0;
}

unsigned func_001f1f20(const struct ArchiveFile *file)
{
    return func_001650f8(file, 0x4000)->offset;
}

unsigned func_001f1f34(const struct ArchiveFile *file)
{
    return func_001650f8(file, 0x4001)->offset;
}

unsigned func_001f1f4c(const struct ArchiveFile *file)
{
    return func_001650f8(file, 0x4003)->offset;
}

bool func_001f1ebc(const struct ArchiveFile *file)
{
    return func_001650f8(file, 0x4001) != 0;
}

bool func_001f1ed8(const struct ArchiveFile *file)
{
    return func_001650f8(file, 0x4003) != 0;
}
