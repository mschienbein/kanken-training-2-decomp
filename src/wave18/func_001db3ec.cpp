#include "game/font.h"
#include "game/font_resource.h"



extern "C" FontResource *func_001db3ec(FontResource *font)
{
    font = func_001daf74(font);
    font->vtable = &data_00224d30;
    return font;
}
