#include "game/font_resource.h"

static __forceinline unsigned short resolveGlyphIndex(const FontResource *font,
                                                       unsigned short code)
{
    unsigned short index = func_001efd80(font, code);
    return index == 0xffff ? font->information->alternate_index : index;
}

static __forceinline const GlyphWidths *findWidths(const FontInformation *information,
                                                   unsigned index)
{
    for (const FontWidthBlock *widths = information->widths; widths; widths = widths->next)
        if (widths->first <= index && index <= widths->last)
            return &widths->entries()[index - widths->first];
    return &information->default_widths;
}

struct GlyphWidths func_001efd08(const FontResource *font, unsigned short code)
{
    unsigned index = resolveGlyphIndex(font, code);
    return *findWidths(font->information, index);
}
