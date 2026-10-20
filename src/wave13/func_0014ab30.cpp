#include <preinclude.h>
#include <string.h>
#include "game/material.h"
#include "game/display_native.h"

extern "C" void func_0014ab30(DisplayObject *object, unsigned value)
{
    nw::lyt::Material *material = display_pane_attachment(object, 0)->picture->GetMaterial();
    /* The packed input supplies the four byte-valued blend settings. */
    memcpy(func_0014d1ec(material), &value, sizeof(nw::lyt::BlendMode));
}
