#include "game/screen.h"

void func_0016c020(struct WidgetSlot **slots, int index, float x, float y)
{
    struct WidgetSlot *slot = slots[index];
    struct BoundDisplayObject *object;
    if (!slot)
        return;
    object = (struct BoundDisplayObject *)slot->object;
    object->base.position[0] = x;
    object->base.position[1] = y;
    switch (func_0017a9a0(&object->base, 0)) {
    case 5:
        func_0015e684(object, 0, object->base.position[0], object->base.position[1], object->base.position[2]);
        break;
    case 6:
        func_00181700(object, object->base.position[0], object->base.position[1], object->base.position[2]);
        break;
    }
}
