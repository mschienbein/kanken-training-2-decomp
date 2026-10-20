#include <preinclude.h>
#include "game/display_native.h"

extern "C" void func_0014d000(DisplayObject *object,
                              const nw::math::VEC2 *coordinates)
{
    PaneDisplayAttachment *binding = display_pane_attachment(object, 0);
    func_0014e504(binding->picture, 1);
    func_0014d02c(binding->picture, 0, coordinates);
}
