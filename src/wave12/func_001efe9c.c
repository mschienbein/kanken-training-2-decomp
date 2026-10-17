#include "game/resource_font.h"
#include "game/sound_track.h"
/* [INFERENCE] Resource names reflect the font-information/sheet layout. */

unsigned short func_001efe9c(const struct ResourceFontView *font)
{
    return font->information->sheet->sheet_count;
}
