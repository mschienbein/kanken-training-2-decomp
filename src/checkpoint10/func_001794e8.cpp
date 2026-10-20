#include "game/screen.h"


struct W7hDisplayPanel {
    unsigned char reserved00[0x28];
    struct WidgetSlot *slot;
};


inline void WidgetSlot::set_active(int enabled)
{
    struct DisplayObject *object = widget_display(this);
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

static inline void set_index(WidgetSlot *slot, int index)
{
    if (index == 2) {
        if (slot->state.byte_flags[0] & 8)
            index = 1;
    }
    if (index == 0)
        slot->state.flags &= ~8u;
    else
        slot->state.flags |= 8;
    func_0017a968(widget_display(slot), bss_00246f4c[7], index, 0);
}

extern "C" void func_001794e8(struct W7hDisplayPanel *panel, int index)
{
    struct WidgetSlot *slot = panel->slot;
    if (slot != 0 && slot->object != 0) {
        set_index(slot, index);
        slot->set_active(1);
    }
}
