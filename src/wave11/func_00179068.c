#include "game/selection.h"

void func_00179068(struct MenuControlSet *control, int index, int enabled)
{
    struct MenuGroup *group = control->groups[index];
    if (group != 0) {
        if (enabled)
            group->flags |= 4;
        else
            group->flags &= ~4u;
    }
}
