#include "game/property_descriptor.h"

extern void func_00183208(struct DisplayObject *object);
extern void func_0017a938(struct NameTable *const *table, int index, int mode);

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
