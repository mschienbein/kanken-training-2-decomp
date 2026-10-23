#include "game/selection.h"

int func_00174160(const struct W6ListController *controller, int list, int item)
{
    struct W6ListGroup *group = controller->lists[list];
    int value = 0;
    if (group == 0)
        return -1;
    if (group->items[item] != 0)
        value = group->items[item]->value;
    return value;
}
