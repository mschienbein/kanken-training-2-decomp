#include "game/animation.h"

int func_001770b4(const struct MenuControlSet *owner, int index)
{
    const struct MenuGroup *animation = owner->groups[index];
    return animation != 0 ? animation->mode : -1;
}
