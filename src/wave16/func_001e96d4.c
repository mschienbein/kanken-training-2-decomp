#include "game/motion_screen.h"
#include "game/runtime.h"

struct MotionKey;

extern void func_0016ee38(struct MotionState *state);
extern int func_0016ece8(struct MotionState *state, const struct MotionKey *keys, int heap);
extern void func_0016ec84(struct MotionState *state, int first, int last);
extern void func_0016ec6c(struct MotionState *state, int loop);
extern void func_0016ec28(struct MotionState *state, int index);

void func_001e96d4(struct MotionScreen *screen, int index, const struct MotionKey *keys)
{
    struct AnimatedWidget *widget = screen->widgets[index];
    if (widget) {
        widget->flags |= 0x10;
        widget->base[0] = 0.0f;
        func_0016ee38(&widget->motion);
        if (func_0016ece8(&widget->motion, keys, 1)) {
            func_0016ec84(&widget->motion, 0, -1);
            func_0016ec6c(&widget->motion, 0);
            func_0016ec28(&widget->motion, -1);
        }
    }
}
