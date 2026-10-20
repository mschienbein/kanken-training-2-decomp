#include "game/motion_screen.h"
#include "game/input.h"
#include "game/runtime.h"

extern void func_0016e924(struct MotionState *state, float elapsed);
extern float func_0016e874(const struct MotionState *state);

void func_0017457c(struct MenuControlSet *screen)
{
    int i;
    for (i = 0; i < 20; ++i) {
        struct MenuGroup *widget = screen->groups[i];
        if (widget && widget->count > 0 && !func_0016ec14(&widget->motion.mode)) {
            func_0016e924(&widget->motion, -1.0f);
            if (widget->flags & 0x10)
                widget->scroll.x = func_0016e874(&widget->motion);
            if (widget->flags & 0x20)
                widget->scroll.y = func_0016e874(&widget->motion);
            widget->offset.x = widget->scroll.x + widget->bias.x;
            widget->offset.y = widget->scroll.y + widget->bias.y;
            if (func_0016ec14(&widget->motion.mode)) {
                if (widget->flags & 0x10)
                    widget->flags &= ~0x10u;
                if (widget->flags & 0x20)
                    widget->flags &= ~0x20u;
            }
        }
    }
}
