#include "game/font.h"

struct FontCharWidths {
    signed char left;
    unsigned char glyphWidth, charWidth;
};
struct FontWidthBlock {
    unsigned short first, last;
    const struct FontWidthBlock *next;
    const FontCharWidths *entries() const { return reinterpret_cast<const FontCharWidths *>(this + 1); }
};
struct FontCodeMap {
    unsigned short first, last, method, reserved06;
    const struct FontCodeMap *next;
    const unsigned short *table() const { return reinterpret_cast<const unsigned short *>(this + 1); }
};
struct FontInformation {
    unsigned char type, lineFeed;
    unsigned short alternate;
    struct FontCharWidths defaultWidths;
    unsigned char encoding;
    struct FontGlyphData *glyphs;
    const struct FontWidthBlock *widths;
    const struct FontCodeMap *maps;
};
struct FontMapResource {
    struct FontResource base;
    void *decoder;
    unsigned short cachedCode, cachedIndex;
};
struct FontGlyph;
/* INTEGRATOR: move to include/game/font.h */
extern "C" {
unsigned short func_001f4a2c(struct FontMapResource *font, unsigned code);
void func_001f4b90(struct FontMapResource *font, struct FontGlyph *glyph, unsigned index);
}

static __forceinline unsigned short resolveGlyphIndex(FontMapResource *font, unsigned code)
{
    unsigned short index = func_001f4a2c(font, code);
    return index == 0xffff ? font->base.information->alternate : index;
}

extern "C" void func_001f4b58(struct FontMapResource *font, struct FontGlyph *glyph, unsigned code)
{
    func_001f4b90(font, glyph, resolveGlyphIndex(font, code));
}
