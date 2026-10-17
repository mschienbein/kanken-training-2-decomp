#include "game/animation.h"

float func_00179008(const struct AnimationOwner *owner, int index, int previous)
{
    const struct Animation *animation = owner->animations[index];
    if (animation == 0)
        return 0.0f;
    return previous ? animation->previous : animation->elapsed;
}
