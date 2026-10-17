#include "game/screen.h"


void func_00179160(struct W8DisplaySet *set, int index, float x, float y)
{
    struct W8DisplayHandle *handle = set->handles[index];
    struct DisplayObject *object;
    if (handle == 0)
        return;
    handle->scale_x = x;
    handle->scale_y = y;
    object = handle->object;
    /* ORIGINAL BUG: a non-null handle is assumed to contain a non-null object. */
    object->scale[0] = x;
    object->scale[1] = y;
    object->scale[2] = 1.0f;
    switch (func_0017a9a0(object, 0)) {
    case 5:
        func_0015e474(object, 0, object->scale[0], object->scale[1], object->scale[2]);
        break;
    case 6:
        func_001816e0((struct BoundDisplayObject *)object, object->scale[0], object->scale[1]);
        break;
    }
}
