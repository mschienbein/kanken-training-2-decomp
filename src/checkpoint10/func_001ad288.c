#include "game/screen.h"
#include "game/runtime.h"

extern void func_00183208(struct DisplayObject *object);

struct W24BOffsetWidgetOwner {
    unsigned char reserved00[0x10];
    struct WidgetSlot *slots[];
};

static void release_slot(struct WidgetSlot *slot)
{
    if (slot != 0 && slot->object != 0)
        func_00183208(slot->object);
}

static void reset_slot(struct WidgetSlot *slot)
{
    if (slot != 0)
        slot->object = 0;
}

static void clear_slots(struct W24BOffsetWidgetOwner *controller)
{
    int i;
    for (i = 0; i < 10; ++i) {
        if (controller->slots[i] != 0) {
            struct WidgetSlot *slot = controller->slots[i];
            release_slot(controller->slots[i]);
            reset_slot(controller->slots[i]);
            func_001852f4(slot);
            controller->slots[i] = 0;
        }
    }
}

struct W24BOffsetWidgetOwner *func_001ad288(struct W24BOffsetWidgetOwner *controller)
{
    clear_slots(controller);
    return controller;
}
