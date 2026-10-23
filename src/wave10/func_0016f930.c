#include "game/input.h"

void func_0016f930(const struct InputOwner *owner, int index)
{
    struct InputGroup *group = owner->groups[index];
    int i;
    if (group && group->count > 0) {
        group->enabled = 1;
        func_00170418(&group->input.pointer);
        for (i = 0; i < group->count; ++i) {
            struct InputItem *item = group->items[i];
            if (item) {
                item->state = 0;
                item->counter = 0;
                item->flags &= ~6;
            }
        }
    }
}
