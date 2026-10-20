#include "game/screen.h"
#include "game/audio.h"

struct AnimatedPanel {
    struct WidgetController controller;
    struct WidgetSlot *slots[27];
    unsigned char primary;
};


int func_001b5c88(const struct AnimatedPanel *panel)
{
    struct WidgetSlot *slot = panel->slots[panel->primary ? 8 : 9];
    if (slot == 0 || slot->object == 0)
        return 0;
    return func_00195874(widget_display(slot), 0);
}
