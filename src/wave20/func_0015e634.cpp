#include <preinclude.h>
#include "game/exp_heap.h"
#include "nw/lyt/lyt_TextBox.h"
#include "game/screen.h"


extern "C" void func_0015e634(ResourceEntry *surface, float width, float height)
{
    surface->object->SetFontSize(nw::lyt::Size(width, height));
}
