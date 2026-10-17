#include "game/resource_font.h"
#include "game/sound_controller.h"
/* Partial views agree with public NintendoWare4Ctr ResFontBase resource fields. */

extern "C" int func_001f4b2c(const ResourceFontView *font)
{
    return font->information->sheet->baseline;
}
