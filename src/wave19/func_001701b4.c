#include "game/screen.h"

static __forceinline void set_active(struct WidgetSlot *slot, int enabled)
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

void func_001701b4(struct WidgetController *controller, int index, int enabled)
{
    struct WidgetSlot *slot = widget_slots(controller)[index];
    if (slot != 0)
        set_active(slot, enabled);
}
