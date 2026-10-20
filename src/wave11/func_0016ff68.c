#include "game/w11_control.h"
static __inline void set_object_active(struct DisplayObject *object, int enabled)
{
    if (object != 0) {
        switch (func_0017a9a0(object, 0)) {
        case 3: func_00195a08(object, 0, enabled); break;
        case 5: func_0015e948(object, enabled); break;
        case 6: func_001826fc(object, enabled); break;
        }
    }
}

void func_0016ff68(struct W11ControlScreen *screen, int index, int enabled)
{
    struct WidgetSlot *slot = screen->slots[index];
    if (slot != 0) {
        slot->state.label.enabled = enabled;
        if (enabled == 0)
            slot->state.label.label_index = -1;
        set_object_active(widget_display(slot), enabled);
    }
}
