#include "game/screen.h"

void func_00166eb0(struct W8DisplaySet *set, int index, float x, float y)
{
    struct W8DisplayHandle *handle = set->handles[index];
    if (handle != 0) {
        struct DisplayObject *object;
        int type;
        handle->position.x = x;
        handle->position.y = y;
        object = handle->object;
        object->position[0] = x;
        object->position[1] = y;
        type = func_0017a9a0(object, 0);
        if (type == 5)
            func_0015e684((struct BoundDisplayObject *)object, 0,
                         object->position[0], object->position[1], object->position[2]);
        else if (type == 6)
            func_00181700((struct BoundDisplayObject *)object,
                         object->position[0], object->position[1], object->position[2]);
    }
}
