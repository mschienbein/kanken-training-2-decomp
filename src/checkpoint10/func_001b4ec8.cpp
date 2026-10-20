#include "game/screen.h"

/* [INFERENCE] Descriptive controller role; field offsets are recovered. */
struct CarouselScreen {
    struct WidgetController controller;
    struct WidgetSlot *slots[18];
    int position;
    int count;
    int selection;
    __forceinline float offset(int selected) const {
        if (selected >= count) selected = count - 1;
        return (3 - selected) * -33.0f;
    }
};

static __forceinline void position_display(struct DisplayObject *object, float x)
{
    object->position[0] = x;
    object->position[1] = 0.0f;
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

extern "C" void func_001b4ec8(struct CarouselScreen *screen, int index, int position)
{
    struct WidgetSlot *slot = screen->slots[index];
    struct DisplayObject *object;
    if (slot == 0 || slot->object == 0)
        return;
    object = slot->object;
    position_display(object, screen->offset(position));
}
