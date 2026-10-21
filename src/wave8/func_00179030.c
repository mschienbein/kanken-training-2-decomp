#include <string.h>

#include "game/animation.h"

void func_00179030(const struct MenuControlSet *owner, int index,
                   int child_index, float *width, float *height)
{
    const struct MenuGroup *animation = owner->groups[index];
    if (animation != 0) {
        const struct MenuControlItem *child = animation->items[child_index];
        if (child != 0) {
            memcpy(width, &child->position.x, sizeof *width);
            memcpy(height, &child->position.y, sizeof *height);
        }
    }
}
