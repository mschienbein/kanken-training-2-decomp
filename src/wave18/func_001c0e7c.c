#include "game/screen.h"
#include "game/audio.h"

struct ActionSlot {
    const struct DisplayObject *object;
    int active;
};

struct ActionSlots {
    struct ActionSlot *slots[3];
};


static __inline int query_action_slot(const struct ActionSlot *slot)
{
    int result = 0;
    const struct DisplayObject *object = slot->object;
    if (object)
        result = func_00195874(object, 0);
    return result;
}

int func_001c0e7c(const struct ActionSlots *container, int index)
{
    int result = 0;
    /* ORIGINAL BUG: a negative index is not rejected before the slot lookup. */
    if (index < 3) {
        const struct ActionSlot *slot = container->slots[index];
        if (slot && slot->active)
            result = query_action_slot(slot);
    }
    return result;
}
