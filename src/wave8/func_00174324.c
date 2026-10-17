#include "game/animation.h"

int func_00174324(const struct AnimationOwner *owner, int index)
{
    const struct Animation *animation = owner->animations[index];
    return animation != 0 ? (animation->flags & 1) : 0;
}
