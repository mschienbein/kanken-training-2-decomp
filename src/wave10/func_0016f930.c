#include "game/menu_controls.h"

void func_0016f930(const struct MenuControlSet *owner, int index)
{
    struct MenuGroup *group = owner->groups[index];
    int i;
    if (group && group->count > 0) {
        group->preserve_selected = 1;
        func_00170418(&group->pointer);
        for (i = 0; i < group->count; ++i) {
            struct MenuControlItem *item = group->items[i];
            if (item) {
                item->delay = 0;
                item->result = 0;
                item->flags &= ~6;
            }
        }
    }
}
