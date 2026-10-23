#include "game/font.h"



int func_001f4994(const FontResource *font, unsigned short character)
{
    return font->vtable->get_widths(font, character).advance;
}
