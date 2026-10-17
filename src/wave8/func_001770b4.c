#include "game/animation.h"

int func_001770b4(const struct AnimationOwner *owner, int index)
{
    const struct Animation *animation = owner->animations[index];
    return animation != 0 ? animation->frame_count : -1;
}
