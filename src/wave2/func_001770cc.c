#include "game/animation.h"

void func_001770cc(struct AnimationOwner *owner, int index, int enabled)
{
    struct Animation *animation = owner->animations[index];
    if (animation != 0) {
        if (enabled) {
            animation->flags |= 2;
            animation->frame = -1;
        } else {
            animation->flags &= ~2u;
        }
        animation->previous = 0.0f;
        animation->elapsed = 0.0f;
    }
}
