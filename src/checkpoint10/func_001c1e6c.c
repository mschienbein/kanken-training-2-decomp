#include "game/display_animation.h"
#include "game/property_descriptor.h"

static __forceinline void set_registration_active(struct DisplayRegistration *slot,
                                                   int active)
{
    struct DisplayObject *object = slot->object;
    if (object) {
        switch (func_0017a9a0(object, 0)) {
        case 3:
            func_00195a08(object, 0, active);
            break;
        case 6:
            func_001826fc(object, active);
            break;
        case 5:
            func_0015e948(object, active);
            break;
        }
    }
}

void func_001c1e6c(struct DisplayRegistration *const *slots, int index, int active)
{
    struct DisplayRegistration *slot = slots[index];
    if (slot) {
        set_registration_active(slot, active);
        if (slot->object) {
            const struct NameTable *table = data_0023efdc[index];
            if (table->count > 0)
                func_0017a968(slot->object, table, 0, 0);
        }
        slot->enabled = 1;
    }
}
