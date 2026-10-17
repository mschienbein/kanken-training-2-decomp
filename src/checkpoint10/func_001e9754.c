#include "game/motion_screen.h"
#include "game/runtime.h"
#include "game/text_writer.h"
/* MotionState mirrors include/game/runtime.h. */
struct MotionState {
    int mode;
    unsigned key_flags;
    unsigned flags;
    float time;
    float scale;
    float first_time;
    float last_time;
    int first;
    int last;
    int current;
    float blend;
    int count;
    struct MotionKey *allocation;
};


/* INTEGRATOR: differs from include/game/runtime.h: MotionKey has a float value
 * at 0x14, written by this function; the existing reserved tail needs refinement. */
struct MotionKey {
    float time;
    unsigned flags;
    unsigned char reserved08[0x0c];
    float value;
};







extern const struct MotionKey data_002180fc[3];


void func_001e9754(struct MotionScreen *screen, int index, float start, float finish, float duration)
{
    struct AnimatedWidget *widget = screen->widgets[index];
    float from = start;
    float to = finish;
    float seconds = duration;
    struct MotionKey keys[3];
    if (widget) {
        func_00192720(keys, data_002180fc, sizeof(keys));
        keys[0].value = from;
        keys[1].time = seconds * 1000.0f;
        keys[1].value = to;
        widget->base[1] = 0.0f;
        widget->base[0] = 0.0f;
        widget->flags |= 0x20;
        func_0016ee38(&widget->motion);
        if (func_0016ece8(&widget->motion, keys, 1)) {
            func_0016ec84(&widget->motion, 0, -1);
            func_0016ec6c(&widget->motion, 0);
            func_0016ec28(&widget->motion, -1);
        }
    }
}
