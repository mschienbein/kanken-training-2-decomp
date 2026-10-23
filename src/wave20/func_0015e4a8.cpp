#include <preinclude.h>
#include "game/text_renderer.h"
#include "game/screen.h"


extern "C" void func_0015e4a8(DisplaySurface *surface, float character_spacing, float line_spacing)
{
    surface->text->set_character_spacing(character_spacing);
    surface->text->set_line_spacing(line_spacing);
}
