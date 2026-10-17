#include "game/archive.h"

struct BinaryFileHeader {
    unsigned signature;
    unsigned short byte_order;
    unsigned short header_size;
    unsigned version;
    unsigned file_size;
    unsigned short block_count;
    unsigned short reserved12;
    struct ArchiveBlockReference blocks[];
};

/* The block search is an outlined leaf shared by the adjacent block queries. */
__declspec(noinline) const struct ArchiveBlockReference *
func_001650f8(const struct ArchiveFile *file, unsigned short type)
{
    int index;
    const struct BinaryFileHeader *header = (const struct BinaryFileHeader *)file;
    for (index = 0; index < header->block_count; ++index) {
        if (header->blocks[index].type == type)
            return &header->blocks[index];
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
