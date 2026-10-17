#include "game/command_state.h"
#include "game/resource_runtime.h"
#include "game/property_descriptor.h"




static __inline void reset_slot(struct WidgetSlot *slot)
{
    if (slot != 0) {
        slot->object = 0;
        slot->state.flags = 0;
    }
}

void func_001c1cfc(struct WidgetSlot **slots)
{
    if (slots[0] != 0) {
        if (slots[0]->object != 0)
            func_00183208(slots[0]->object);
        reset_slot(slots[0]);
    }
    func_0017a938(data_0023efdc, -1, 0);
}
