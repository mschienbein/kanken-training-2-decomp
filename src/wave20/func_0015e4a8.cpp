#include <preinclude.h>
#include "game/exp_heap.h"
#include "nw/lyt/lyt_TextBox.h"
#include "game/screen.h"


extern "C" void func_0015e4a8(ResourceEntry *surface, float character_spacing, float line_spacing)
{
    surface->object->SetCharSpace(character_spacing);
    surface->object->SetLineSpace(line_spacing);
}
