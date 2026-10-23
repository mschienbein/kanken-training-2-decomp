#include "game/resource_runtime.h"
#include "game/screen.h"
#include "game/runtime.h"



static void reset_slot(struct WidgetSlot *slot)
{
    if (slot)
        slot->object = 0;
}

static void release_widget(struct WidgetSlot *slot,
                           struct WidgetController *controller, int index)
{
    if (slot->object)
        func_00183208(slot->object);
    reset_slot(widget_slots(controller)[index]);
    func_001852f4(slot);
    widget_slots(controller)[index] = 0;
}

static void clear_controller(struct WidgetController *controller)
{
    int index;
    for (index = 0; index < 24; ++index) {
        struct WidgetSlot *slot = widget_slots(controller)[index];
        if (slot)
            release_widget(slot, controller, index);
    }
}
struct WidgetController *func_0015eba4(struct WidgetController *controller)
{
    clear_controller(controller);
    return controller;
}
