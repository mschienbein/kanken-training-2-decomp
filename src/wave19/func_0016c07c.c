#include "game/screen.h"

static __forceinline void set_slot_active(struct WidgetSlot *slot, int active)
{
    struct DisplayObject *object = widget_display(slot);
    if (!object)
        return;
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

void func_0016c07c(struct WidgetSlot **slots, int index, int active)
{
    struct WidgetSlot *slot = slots[index];
    if (slot)
        set_slot_active(slot, active);
}
