#include "game/display_animation.h"
#include "game/material.h"
#include "game/screen.h"

extern "C" void func_001810e4(DisplayObject *object, int index)
{
    display_pane_attachment(object, index)->mode = 2;
    func_00181100(object, -1, 0);
}
