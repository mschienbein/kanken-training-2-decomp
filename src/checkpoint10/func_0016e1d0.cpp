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

extern "C" void func_0016e1d0(Screen *screen, int index, int enabled)
{
    WidgetSlot *slot = screen->slots[index];
    if (slot) {
        switch (index) {
        case 0x31:
        case 0x32:
            slot->state.label.enabled = enabled;
            if (enabled == 0)
                slot->state.label.label_index = -1;
            break;
        }
        slot->set_active(enabled);
    }
}
