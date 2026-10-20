#include "game/training_screen.h"

void func_00175a90(struct InputOwner *owner, int index, int enabled)
{
    struct InputGroup *item = owner->groups[index];
    if (item) {
        if (enabled)
            item->flags |= 8;
        else
            item->flags &= ~8u;
    }
}
