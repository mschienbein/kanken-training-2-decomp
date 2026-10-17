#include "game/w11_control.h"
void func_0016ff08(struct W11ControlScreen *screen, int index, float x, float y)
{
    struct WidgetSlot *slot = screen->slots[index];
    if (slot != 0) {
        struct DisplayObject *object = slot->object;
        /* ORIGINAL BUG: an existing slot's null object is not checked before the stores. */
        object->position[0] = x;
        object->position[1] = y;
        switch (func_0017a9a0(object, 0)) {
        case 5:
            func_0015e684((struct BoundDisplayObject *)object, 0,
                          object->position[0], object->position[1], object->position[2]);
            break;
        case 6:
            func_00181700((struct BoundDisplayObject *)object,
                          object->position[0], object->position[1], object->position[2]);
            break;
        }
    }
}
