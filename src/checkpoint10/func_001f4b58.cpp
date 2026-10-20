#include "game/font_resource.h"
#include "game/item_text.h"

/* [INFERENCE] Alternate target-font owner/constness and code width remain
 * unrecovered; retain its full-word inputs and the actual target glyph layout. */
static __forceinline unsigned short resolveGlyphIndex(FontResource *font, unsigned code)
{
    unsigned short index = func_001f4a2c(font, code);
    return index == 0xffff ? font->information->alternate_index : index;
}

void func_001f4b58(FontResource *font, W12cGlyphMetrics *glyph, unsigned code)
{
    func_001f4b90(font, glyph, resolveGlyphIndex(font, code));
}
