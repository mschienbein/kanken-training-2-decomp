#include "game/animation.h"
void func_00175a5c(struct MenuControlSet *owner, int index,
                   float x, float y, float width, float height)
{
    struct MenuGroup *animation = owner->groups[index];
    if (animation != 0) {
        x -= 160.0f;
        y += 120.0f;
        func_001670e4(&animation->viewport, x, y, width, height);
    }
}
