#include "game/screen.h"
/* INTEGRATOR: move to include/game/screen.h; revise src/wave4/func_001b54e8.c's void* wrapper declaration to Screen*. */
extern "C" { void func_001b54ec(Screen *screen, int enabled); }
static __forceinline int widget_has_flag(struct Screen *screen, int index, unsigned int mask)
{
    struct WidgetSlot *slot = screen->slots[index];
    return slot != 0 && slot->object != 0 && (slot->state.flags & mask);
}

void func_001b54ec(struct Screen *screen, int enabled)
{
    int index = 0;
    if (enabled) {
        for (; index < 27; ++index) {
            switch (index) {
            case 0:
            case 1:
            case 2:
            case 3:
                if (widget_has_flag(screen, index, 1))
                    continue;
                break;
            case 5:
            case 6:
            case 8:
            case 9:
                if (!widget_has_flag(screen, index, 2))
                    continue;
                break;
            default:
                if ((unsigned int)(index - 11) < 16) {
                    if (!widget_has_flag(screen, index, 16))
                        continue;
                }
                break;
            case 10:
                continue;
            }
            func_0016e7e0(&screen->controller, index, 1);
        }
    } else {
        for (; index < 27; ++index)
            func_0016e7e0(&screen->controller, index, 0);
    }
}

