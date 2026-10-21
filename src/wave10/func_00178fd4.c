#include "game/menu_controls.h"

bool func_00178fd4(const struct MenuControlSet *owner, int index)
{
    const struct MenuGroup *group = owner->groups[index];
    int result = 0;
    if (group) {
        if (func_0016ec14(&group->motion.mode))
            return result;
        return 1;
    }
    return result;
}
