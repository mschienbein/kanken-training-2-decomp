#include "game/font_resource.h"

extern "C" bool func_001f4cc4(const FontResource *font)
{
    return ((font->flags >> 2) & 1u) != 0;
}
