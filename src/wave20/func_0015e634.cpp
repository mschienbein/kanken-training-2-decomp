#include <preinclude.h>
#include "game/text_renderer.h"
#include "game/screen.h"


extern "C" void func_0015e634(DisplaySurface *surface, float width, float height)
{
    surface->text->set_size(SurfaceSize(width, height));
}
