#include "game/motion_screen.h"
#include "game/runtime.h"

struct MotionKey;


extern int func_0016ece8(struct MotionState *state, const struct MotionKey *keys, int heap);
extern void func_0016ec84(struct MotionState *state, int first, int last);



void func_001e96d4(struct MenuControlSet *screen, int index, const struct MotionKey *keys)
{
    struct MenuGroup *widget = screen->groups[index];
    if (widget) {
        widget->flags |= 0x10;
        widget->velocity.x = 0.0f;
        func_0016ee38(&widget->motion);
        if (func_0016ece8(&widget->motion, keys, 1)) {
            func_0016ec84(&widget->motion, 0, -1);
            func_0016ec6c(&widget->motion, 0);
            func_0016ec28(&widget->motion, -1);
        }
    }
}
