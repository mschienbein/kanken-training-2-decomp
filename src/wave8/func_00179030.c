#include <string.h>

#include "game/animation.h"

void func_00179030(const struct AnimationOwner *owner, int index,
                   int child_index, float *width, float *height)
{
    const struct Animation *animation = owner->animations[index];
    if (animation != 0) {
        const struct AnimationChild *child = animation->children[child_index];
        if (child != 0) {
            memcpy(width, &child->width, sizeof *width);
            memcpy(height, &child->height, sizeof *height);
        }
    }
}
