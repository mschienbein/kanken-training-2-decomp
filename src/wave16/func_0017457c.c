#include "game/motion_screen.h"
#include "game/input.h"
#include "game/runtime.h"

extern void func_0016e924(struct MotionState *state, float elapsed);
extern float func_0016e874(const struct MotionState *state);

void func_0017457c(struct MotionScreen *screen)
{
    int i;
    for (i = 0; i < 20; ++i) {
        struct AnimatedWidget *widget = screen->widgets[i];
        if (widget && widget->enabled > 0 && !func_0016ec14(&widget->motion.mode)) {
            func_0016e924(&widget->motion, -1.0f);
            if (widget->flags & 0x10)
                widget->position[0] = func_0016e874(&widget->motion);
            if (widget->flags & 0x20)
                widget->position[1] = func_0016e874(&widget->motion);
            widget->effective[0] = widget->position[0] + widget->displacement[0];
            widget->effective[1] = widget->position[1] + widget->displacement[1];
            if (func_0016ec14(&widget->motion.mode)) {
                if (widget->flags & 0x10)
                    widget->flags &= ~0x10u;
                if (widget->flags & 0x20)
                    widget->flags &= ~0x20u;
            }
        }
    }
}
