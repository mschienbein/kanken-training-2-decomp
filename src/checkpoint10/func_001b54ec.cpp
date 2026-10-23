#include "game/screen.h"
static __forceinline int widget_has_flag(struct WidgetController *screen, int index, unsigned int mask)
{
    struct WidgetSlot *slot = widget_slots(screen)[index];
    return slot != 0 && slot->object != 0 && (slot->state.flags & mask);
}

void func_001b54ec(struct WidgetController *screen, int enabled)
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
            func_0016e7e0(screen, index, 1);
        }
    } else {
        for (; index < 27; ++index)
            func_0016e7e0(screen, index, 0);
    }
}

