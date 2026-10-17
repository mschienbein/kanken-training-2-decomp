#include "game/animation.h"

void func_00179e40(struct AnimationOwner *owner, int index,
                   float x, float y, float width, float height)
{
    struct Animation *panel = owner->animations[index];
    if (panel != 0)
        func_001670e4(panel->bounds, x, y, width, height);
}
