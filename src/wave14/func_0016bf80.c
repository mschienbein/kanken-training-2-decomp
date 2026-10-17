#include "game/display_animation.h"
#include "game/screen.h"


void func_0016bf80(struct BoundDisplayObject *object, int index, float speed)
{
    struct DisplayAnimationSlot *slot = (struct DisplayAnimationSlot *)object->bindings[index];
    slot->speed = speed;
}
