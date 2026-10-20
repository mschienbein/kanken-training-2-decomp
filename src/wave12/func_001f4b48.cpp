#include "game/font_resource.h"

extern "C" int func_001f4b48(const FontResource *font)
{
    return font->information->glyphs->sheet_count;
}
