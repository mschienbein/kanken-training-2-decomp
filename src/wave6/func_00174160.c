#include "game/selection.h"

int func_00174160(const struct MenuControlSet *controller, int list, int item)
{
    struct MenuGroup *group = controller->groups[list];
    int value = 0;
    if (group == 0)
        return -1;
    if (group->items[item] != 0)
        value = group->items[item]->result;
    return value;
}
