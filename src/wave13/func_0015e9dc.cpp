#include <preinclude.h>
#include "game/material.h"
#include "game/display_native.h"

extern "C" void func_0015e9dc(DisplayObject *object, int index, unsigned char alpha)
{
    LayoutDisplayAttachment *binding = display_layout_attachment(object, index);
    nw::lyt::Pane *pane;
    binding->timed.common.alpha = alpha;
    pane = binding->layout->GetRootPane();
    func_0017aa90(pane, binding->timed.common.red, binding->timed.common.green,
                   binding->timed.common.blue, alpha);
}
