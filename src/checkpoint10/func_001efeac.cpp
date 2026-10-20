#include <preinclude.h>
#include <nw/font/font_Font.h>
#include "game/font_resource.h"

static __forceinline unsigned short resolveGlyphIndex(const FontResource *font,
                                                       unsigned short code)
{
    unsigned short index = func_001efd80(font, code);
    return index == 0xffff ? font->information->alternate_index : index;
}

void func_001efeac(const FontResource *font, nw::font::Glyph *glyph, unsigned short code)
{
    func_001efee4(font, glyph, resolveGlyphIndex(font, code));
}
