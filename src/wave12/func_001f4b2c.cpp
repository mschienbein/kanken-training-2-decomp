#include "game/font_resource.h"

extern "C" int func_001f4b2c(const FontResource *font)
{
    return font->information->glyphs->baseline;
}
