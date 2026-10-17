#include "game/screen.h"



void func_0015eac8(struct DisplayObject *object, int index, float value)
{
    struct BoundDisplayObject *bound = (struct BoundDisplayObject *)object;
    struct DisplayBinding *binding =
        (struct DisplayBinding *)bound->bindings[index];
    binding->value = value;
    binding->target->value = value;
}
