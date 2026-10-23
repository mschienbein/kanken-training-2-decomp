#include "game/display_animation.h"
#include "game/material.h"
#include "game/screen.h"


void func_001810e4(struct BoundDisplayObject *object, int index)
{
    struct DisplayAnimationSlot *slot = (struct DisplayAnimationSlot *)object->bindings[index];
    slot->playback_state = 2;
    func_00181100(object, -1, 0);
}
