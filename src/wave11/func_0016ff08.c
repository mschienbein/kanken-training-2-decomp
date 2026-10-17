#include "game/screen.h"

struct W11ControlScreen {
    unsigned char reserved00[0x0c];
    struct WidgetSlot *slots[10];
    int layout;
    int active;
    int selection;
};

extern void func_00181700(struct BoundDisplayObject *object, float x, float y, float z);
extern void func_0015e684(struct BoundDisplayObject *object, int index, float x, float y, float z);

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
