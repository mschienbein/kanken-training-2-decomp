#include "game/screen.h"
#include "game/property_descriptor.h"


static bool set_slot_active(WidgetSlot *slot, int enabled)
{
    ResourceEvent *object = slot->object;
    if (object == 0)
        return false;
    switch (func_0017a9a0(&object->base, 0)) {
    case 3:
        func_00195a08(&object->base, 0, enabled);
        break;
    case 6:
        func_001826fc(&object->base, enabled);
        break;
    case 5:
        func_0015e948(&object->base, enabled);
        break;
    }
    return true;
}

extern "C" void func_0017a71c(Screen *screen, int index, int enabled)
{
    WidgetSlot *slot = screen->slots[index];
    if (slot == 0)
        return;
    if (!set_slot_active(slot, enabled))
        return;
    ResourceEvent *object = slot->object;
    if (object != 0) {
        const NameTable *table = bss_0024445c[index];
        if (table->count > 0)
            func_0017a968(&object->base, table, 0, 0);
    }
}

