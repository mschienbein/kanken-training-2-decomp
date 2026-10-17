#include "game/screen.h"

void func_00179964(struct WidgetController *screen)
{
    int index;
    for (index = 5; index <= 6; ++index) {
        struct WidgetSlot *slot;
        func_0016e7e0(screen, index, 0);
        slot = widget_slots(screen)[index];
        if (slot != 0 && slot->object != 0)
            slot->state.flags &= ~2u;
    }
}
