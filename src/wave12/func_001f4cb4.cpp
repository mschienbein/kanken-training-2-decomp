#include "game/resource_font.h"
#include "game/sound_controller.h"
/* Partial views agree with public NintendoWare4Ctr ResFontBase resource fields. */

extern "C" bool func_001f4cb4(const ResourceFontView *font)
{
    return ((font->flags >> 1) & 1u) != 0;
}
