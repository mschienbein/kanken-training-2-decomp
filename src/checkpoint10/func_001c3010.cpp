#include "game/selection_cleanup.h"

static __inline void release_slot(struct TimedDisplaySlot *slot)
{
    if (slot != 0 && slot->object != 0)
        func_00183208(slot->object);
}

static __inline void reset_slot(struct TimedDisplaySlot *slot)
{
    if (slot != 0)
        slot->object = 0;
}

static void clear_owner(TimedDisplayController *controller)
{
    int i;
    for (i = 0; i < 11; ++i) {
        if (controller->slots[i] != 0) {
            struct TimedDisplaySlot *slot = controller->slots[i];
            release_slot(controller->slots[i]);
            reset_slot(controller->slots[i]);
            func_001852f4(slot);
            controller->slots[i] = 0;
        }
    }
}

extern "C" TimedDisplayController *func_001c3010(TimedDisplayController *controller)
{
    clear_owner(controller);
    return controller;
}
