#include "game/training_screen.h"

void func_00175a90(struct MenuControlSet *owner, int index, int enabled)
{
    struct MenuGroup *item = owner->groups[index];
    if (item) {
        if (enabled)
            item->flags |= 8;
        else
            item->flags &= ~8u;
    }
}
