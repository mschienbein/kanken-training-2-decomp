#include "game/archive.h"

/* [INFERENCE] The adjacent queries share the generic binary-file block search. */
const struct ArchiveBlockReference *
func_001650f8(const struct ArchiveFile *file, unsigned type)
{
    int index;
    for (index = 0; index < file->block_count; ++index) {
        if (file->blocks[index].type == type)
            return &file->blocks[index];
    }
    return 0;
}

bool func_001f1ebc(const struct ArchiveFile *file)
{
    return func_001650f8(file, 0x4001) != 0;
}

bool func_001f1ed8(const struct ArchiveFile *file)
{
    return func_001650f8(file, 0x4003) != 0;
}

unsigned func_001f1f08(const struct ArchiveFile *file)
{
    /* ORIGINAL BUG: the required block is dereferenced even when absent. */
    return func_001650f8(file, 0x4002)->offset;
}

unsigned func_001f1f20(const struct ArchiveFile *file)
{
    /* ORIGINAL BUG: the required block is dereferenced even when absent. */
    return func_001650f8(file, 0x4000)->offset;
}

unsigned func_001f1f34(const struct ArchiveFile *file)
{
    /* ORIGINAL BUG: the required block is dereferenced even when absent. */
    return func_001650f8(file, 0x4001)->offset;
}

unsigned func_001f1f4c(const struct ArchiveFile *file)
{
    /* ORIGINAL BUG: the required block is dereferenced even when absent. */
    return func_001650f8(file, 0x4003)->offset;
}
