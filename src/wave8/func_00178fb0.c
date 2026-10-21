#include <string.h>

#include "game/animation.h"

void func_00178fb0(const struct MenuControlSet *owner, int index,
                   float *width, float *height)
{
    const struct MenuGroup *animation = owner->groups[index];
    if (animation != 0) {
        memcpy(width, &animation->scroll.x, sizeof *width);
        memcpy(height, &animation->scroll.y, sizeof *height);
    }
}
