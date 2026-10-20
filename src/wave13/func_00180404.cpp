#include "game/material.h"
#include "game/screen.h"

extern "C" void func_00180404(DisplayObject *object, int index)
{
    display_pane_attachment(object, index)->mode = 3;
}
