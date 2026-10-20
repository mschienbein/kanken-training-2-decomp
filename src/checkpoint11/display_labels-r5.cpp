#include "game/pane_descriptors.h"

static bool display_active(DisplayObject *object, int enabled)
{
    if (!object)
        return false;

    switch (func_0017a9a0(object, 0)) {
    case 3: func_00195a08(object, 0, enabled); break;
    case 6: func_001826fc(object, enabled); break;
    case 5: func_0015e948(object, enabled); break;
    }
    return true;
}

static void label_slot(WidgetSlot *slot, int index)
{
    DisplayObject *object = slot->object;
    if (object && bss_00244378[index]->count > 0)
        func_0017a968(object, bss_00244378[index], 0, 0);
}

static WidgetSlot *enable_slot(WidgetSlot *slot, int enabled)
{
    return display_active(slot->object, enabled) ? slot : 0;
}

void func_00170e58(Screen *screen, int index, int enabled)
{
    WidgetSlot *slot = screen->slots[index];
    if (slot) {
        slot = enable_slot(slot, enabled);
        if (slot)
            label_slot(slot, index);
    }
}
