#include "game/screen.h"

struct W8DisplayHandle {
    struct DisplayObject *object;
    unsigned char reserved04[8];
    float scale_x;
    float scale_y;
};

struct W8DisplaySet {
    unsigned char reserved00[0x1300];
    struct W8DisplayHandle *handles[];
};

extern void func_001816e0(struct DisplayObject *object, float x, float y);
extern void func_0015e474(struct DisplayObject *object, int index, float x, float y, float z);

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
        func_001816e0(object, object->scale[0], object->scale[1]);
        break;
    }
}
