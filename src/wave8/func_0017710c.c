#include "game/animation.h"

int func_0017710c(const struct MenuControlSet *owner, int index)
{
    const struct MenuGroup *animation = owner->groups[index];
    return animation != 0 ? animation->selected : -1;
}
