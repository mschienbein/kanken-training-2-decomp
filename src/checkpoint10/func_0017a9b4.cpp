#include "game/screen.h"
#include "game/property_descriptor.h"


static inline void set_active(struct WidgetSlot *slot, int active)
{
    struct DisplayObject *object = slot->object;
    if (object == 0)
        return;
    switch (func_0017a9a0(object, 0)) {
    case 3: func_00195a08(object, 0, active); break;
    case 6: func_001826fc(object, active); break;
    case 5: func_0015e948(object, active); break;
    }
}

static inline void set_entry_active(struct Screen *screen, int value, int index)
{
    struct WidgetSlot *slot = screen->slots[index];
    if (slot != 0) {
        set_active(slot, value);
        if (slot->object != 0) {
            const struct NameTable *table = bss_0024445c[index];
            /* ORIGINAL BUG: the table pointer is used without a null check. */
            if (table->count > 0)
                func_0017a968(slot->object, table, 0, 0);
        }
    }
}

void func_0017a9b4(struct Screen *screen, int value)
{
    int index = 0;
    while (1) {
        set_entry_active(screen, value, index);
        ++index;
        if (index >= 24)
            break;
    }
}
