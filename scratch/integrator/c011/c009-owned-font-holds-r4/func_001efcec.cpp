#include "game/font.h"



int func_001efcec(const FontResource *font, unsigned short character)
{
    return font->vtable->get_widths(font, character).advance;
}
