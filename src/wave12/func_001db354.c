#include "game/font_resource.h"
unsigned func_001db354(const struct BinaryFileHeader *file)
{
    const struct BinaryBlockHeader *block;
    int i;
    if (file->signature != 0x554e4643 &&
        !func_001813a0(file, 0x544e4643, 0x03000000, 2))
        return 0;
    block = (const struct BinaryBlockHeader *)((const unsigned char *)file + file->header_size);
    for (i = 0; i < file->block_count; ++i) {
        if (block->signature == 0x504c4754) {
            const struct FontGlyphInfo *glyph = (const struct FontGlyphInfo *)(block + 1);
            if (glyph)
                return glyph->sheet_count * 20;
            break;
        }
        block = (const struct BinaryBlockHeader *)((const unsigned char *)block + block->size);
    }
    return 0;
}
