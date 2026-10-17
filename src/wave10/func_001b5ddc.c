#include "game/screen.h"

static __forceinline void clear_widget_lock(struct WidgetController *screen, int index)
{
    struct WidgetSlot *slot = widget_slots(screen)[index];
    if (slot != 0 && slot->object != 0)
        slot->state.flags &= ~2u;
}

void func_001b5ddc(struct WidgetController *screen)
{
    func_0016e7e0(screen, 8, 0);
    func_0016e7e0(screen, 9, 0);
    clear_widget_lock(screen, 8);
    clear_widget_lock(screen, 9);
}
