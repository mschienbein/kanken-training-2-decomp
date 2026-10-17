#include "game/screen.h"

static __forceinline void set_widget_active(struct WidgetSlot *slot, int enabled)
{
    struct DisplayObject *object = slot->object;
    if (object == 0)
        return;
    switch (func_0017a9a0(object, 0)) {
    case 3:
        func_00195a08(object, 0, enabled);
        break;
    case 5:
        func_0015e948(object, enabled);
        break;
    case 6:
        func_001826fc(object, enabled);
        break;
    }
}

void func_0016e7e0(struct WidgetController *screen, int index, int enabled)
{
    struct WidgetSlot *slot = widget_slots(screen)[index];
    if (slot == 0)
        return;
    if (enabled)
        slot->state.flags &= ~4u;
    else
        slot->state.flags |= 4u;
    set_widget_active(slot, enabled);
}
