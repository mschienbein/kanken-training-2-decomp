#include "game/animation.h"

void func_001770cc(struct MenuControlSet *owner, int index, int enabled)
{
    struct MenuGroup *animation = owner->groups[index];
    if (animation != 0) {
        if (enabled) {
            animation->flags |= 2;
            animation->selected = -1;
        } else {
            animation->flags &= ~2u;
        }
        animation->velocity.y = 0.0f;
        animation->velocity.x = 0.0f;
    }
}
