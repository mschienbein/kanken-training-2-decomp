#include "game/screen.h"

#include "game/animation.h"

void func_0015eaa8(struct AnimationDisplayObject *object, int index)
{
    struct AnimationResetSlot *slot = object->slots[index];
    slot->state = 0;
    slot->progress = 0.0f;
}
