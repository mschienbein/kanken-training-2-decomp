#include "game/screen.h"

static __forceinline void set_widget_active(struct WidgetSlot *slot, int active)
{
    struct ResourceEvent *event = slot->object;
    struct DisplayObject *object;
    if (event == 0)
        return;
    object = &event->base;
    switch (func_0017a9a0(object, 0)) {
    case 3:
        func_00195a08(object, 0, active);
        break;
    case 5:
        func_0015e948(object, active);
        break;
    case 6:
        func_001826fc(object, active);
        break;
    }
}

void func_001ae888(struct WidgetController *controller, int index, int active)
{
    struct WidgetSlot *slot = widget_slots(controller)[index];
    if (slot != 0)
        set_widget_active(slot, active);
}
