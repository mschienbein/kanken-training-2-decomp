#include <preinclude.h>
#include "game/material.h"
#include "game/display_native.h"

extern "C" void func_0014aaf4(DisplayObject *object, int mode, float value)
{
    nw::lyt::Material *material = display_pane_attachment(object, 0)->picture->GetMaterial();
    /* ORIGINAL BUG: AlphaCompare's three padding bytes are not initialized. */
    nw::lyt::AlphaCompare pair((nw::lyt::AlphaTest)mode, value);
    *material->GetAlphaComparePtr() = pair;
}
