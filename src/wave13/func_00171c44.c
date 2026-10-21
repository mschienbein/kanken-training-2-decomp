#include "game/drawing.h"
#include "game/selection.h"

int func_00171c44(const struct MenuControlSet *controller, int index)
{
    const struct MenuGroup *group = controller->groups[index];
    return group ? group->count : 0;
}
