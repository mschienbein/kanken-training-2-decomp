#include <string.h>

#include "game/animation.h"

void func_00178fb0(const struct AnimationOwner *owner, int index,
                   float *width, float *height)
{
    const struct Animation *animation = owner->animations[index];
    if (animation != 0) {
        memcpy(width, &animation->width, sizeof *width);
        memcpy(height, &animation->height, sizeof *height);
    }
}
