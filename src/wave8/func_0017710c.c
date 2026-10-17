#include "game/animation.h"

int func_0017710c(const struct AnimationOwner *owner, int index)
{
    const struct Animation *animation = owner->animations[index];
    return animation != 0 ? animation->frame : -1;
}
