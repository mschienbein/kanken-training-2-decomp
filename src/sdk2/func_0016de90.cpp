#include "game/selection_collections.h"
static inline void set_active(struct ScalarHandle *slot, int active)
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

static inline void set_entry_active(struct ScalarCollection *screen, int index, int active)
{
    struct ScalarHandle *slot = screen->entries[index];
    if (slot != 0 && slot->object != 0)
        set_active(slot, active);
}

void func_0016de90(struct ScalarCollection *screen, int index)
{
    set_entry_active(screen, index, 0);
}
