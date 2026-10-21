#include "game/animation.h"

int func_00174324(const struct MenuControlSet *owner, int index)
{
    const struct MenuGroup *animation = owner->groups[index];
    return animation != 0 ? (animation->flags & 1) : 0;
}
