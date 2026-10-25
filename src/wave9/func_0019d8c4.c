#include "game/font.h"

unsigned func_0019d8c4(const struct FontFile *file)
{
    struct FontGlyphInfo *glyph = 0;
    const struct FontBlock *block;
    int i;
    if (file->signature != 0x554e4643 &&
        !func_001149c0(file, 0x544e4643, 0x03000000, 2))
        return 0;
    block = (const struct FontBlock *)((const unsigned char *)file + file->headerSize);
    for (i = 0; i < file->blockCount; ++i) {
        if (block->signature == 0x504c4754) {
            glyph = (struct FontGlyphInfo *)block->data;
            break;
        }
        block = (const struct FontBlock *)((const unsigned char *)block + block->size);
    }
    return glyph ? glyph->sheet_count * 20 : 0;
}
