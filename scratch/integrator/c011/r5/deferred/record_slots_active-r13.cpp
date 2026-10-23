#include "game/screen.h"

/* [INFERENCE] This active-state operation uses only canonical Screen slots;
 * unrelated lifecycle owners from the author TU are not imported. */
static void set_display_active(DisplayObject *object, int enabled)
{
    if (!object) return;
    switch (func_0017a9a0(object, 0)) {
    case 3: func_00195a08(object, 0, enabled); break;
    case 6: func_001826fc(object, enabled); break;
    case 5: func_0015e948(object, enabled); break;
    }
}

static void set_slot_active(WidgetSlot *slot, int enabled)
{
    if (slot) set_display_active(slot->object, enabled);
}

static void set_index_active(Screen *screen, int index, int enabled)
{
    set_slot_active(screen->slots[index], enabled);
}

static void set_four_active(Screen *screen, int enabled)
{
    for (int i = 0; i < 4; ++i) set_index_active(screen, i, enabled);
}

extern "C" void func_001af258(Screen *screen, int enabled)
{
    set_four_active(screen, enabled);
}
