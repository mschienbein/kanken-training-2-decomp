#include "game/font_resource.h"

extern "C" bool func_001f4cb4(const FontResource *font)
{
    return ((font->flags >> 1) & 1u) != 0;
}
