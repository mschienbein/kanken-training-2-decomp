#include "game/graphics_resources.h"

void *func_0018cb5c(unsigned index)
{
    if (index < 32) {
        const struct SlotValue *slot = data_00242a0c->slots[index];
        if (slot)
            return slot->value;
    }
    return 0;
}
