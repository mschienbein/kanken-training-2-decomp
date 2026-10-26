#include "game/resource_runtime.h"
#include "game/screen_operations.h"

static __inline void release_slot(struct WidgetSlot *slot)
{
    if (slot != 0 && slot->object != 0)
        func_00183208(slot->object);
}

static __inline void reset_slot(struct WidgetSlot *slot)
{
    if (slot != 0)
        slot->object = 0;
}

static void destroy_slots(struct WidgetController *controller)
{
    int i;
    for (i = 0; i < 10; ++i) {
        struct WidgetSlot *slot = widget_slots(controller)[i];
        if (slot != 0) {
            release_slot(widget_slots(controller)[i]);
            reset_slot(widget_slots(controller)[i]);
            func_001852f4(slot);
            widget_slots(controller)[i] = 0;
        }
    }
}

struct WidgetController *func_001b4870(struct WidgetController *controller)
{
    destroy_slots(controller);
    return controller;
}
