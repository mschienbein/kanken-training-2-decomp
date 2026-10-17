#include "game/animation.h"

void func_00179e00(struct AnimationOwner *owner, int index, float x, float y)
{
    struct Animation *panel = owner->animations[index];
    if (panel != 0) {
        panel->width = func_0016702c(panel->bounds, x);
        panel->height = func_00167000(panel->bounds, y);
    }
}
