#include "game/animation.h"

void func_00179e00(struct MenuControlSet *owner, int index, float x, float y)
{
    struct MenuGroup *panel = owner->groups[index];
    if (panel != 0) {
        panel->scroll.x = func_0016702c(&panel->limits, x);
        panel->scroll.y = func_00167000(&panel->limits, y);
    }
}
