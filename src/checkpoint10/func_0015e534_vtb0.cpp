#include <preinclude.h>
#include "game/text_renderer.h"
using nw::ut::Color8;
#include "game/screen.h"
extern "C" void func_0015e534(DisplaySurface *surface, unsigned char red, unsigned char green,
                              unsigned char blue, unsigned char alpha)
{
    surface->text->SetTextColor(1, Color8(red, green, blue, alpha));
}
