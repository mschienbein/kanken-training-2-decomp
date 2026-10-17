#include "game/screen.h"


void func_0015e684(struct BoundDisplayObject *object, int index,
                   float x, float y, float z)
{
    struct DisplayBinding *binding = object->bindings[index];
    object->base.position[0] = x;
    object->base.position[1] = y;
    object->base.position[2] = z;
    func_0015e698(binding->link, x, y, z);
}
