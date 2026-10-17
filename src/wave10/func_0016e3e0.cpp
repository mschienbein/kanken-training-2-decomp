#include "game/screen.h"

inline void WidgetSlot::set_active(int enabled)
{
    struct DisplayObject *object = this->object;
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

extern "C" void func_0016e3e0(struct WidgetController *screen, int index, int value)
{
    struct WidgetSlot *slot = widget_slots(screen)[index];
    if (slot == 0 || slot->object == 0)
        return;
    func_0017a968(slot->object, bss_00246f4c[index], value, 0);
    slot->set_active(1);
}
