#include "game/animation.h"

void func_00179e40(struct MenuControlSet *owner, int index,
                   float x, float y, float width, float height)
{
    struct MenuGroup *panel = owner->groups[index];
    if (panel != 0)
        func_001670e4(&panel->limits, x, y, width, height);
}
