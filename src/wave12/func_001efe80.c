#include "game/resource_font.h"
#include "game/sound_track.h"
/* [INFERENCE] Resource names reflect the font-information/sheet layout. */

signed char func_001efe80(const struct ResourceFontView *font)
{
    return font->information->sheet->baseline;
}
