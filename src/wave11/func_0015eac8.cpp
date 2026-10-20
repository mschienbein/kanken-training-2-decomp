#include <preinclude.h>
#include "nw/lyt/lyt_Animation.h"
#include "game/screen.h"

extern "C" void func_0015eac8(DisplayObject *object, int index, float value)
{
    LayoutDisplayAttachment *binding = display_layout_attachment(object, index);
    binding->timed.common.value = value;
    binding->target->SetFrame(value);
}
