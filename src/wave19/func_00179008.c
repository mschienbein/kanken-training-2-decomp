#include "game/animation.h"

float func_00179008(const struct MenuControlSet *owner, int index, int previous)
{
    const struct MenuGroup *animation = owner->groups[index];
    if (animation == 0)
        return 0.0f;
    return previous ? animation->velocity.y : animation->velocity.x;
}
