#include "game/screen.h"

void func_0016c020(struct WidgetSlot **slots, int index, float x, float y)
{
    struct WidgetSlot *slot = slots[index];
    struct DisplayObject *object;
    if (!slot)
        return;
    object = slot->object;
    object->position[0] = x;
    object->position[1] = y;
    switch (func_0017a9a0(object, 0)) {
    case 5:
        func_0015e684(object, 0, object->position[0], object->position[1], object->position[2]);
        break;
    case 6:
        func_00181700(object, object->position[0], object->position[1], object->position[2]);
        break;
    }
}
