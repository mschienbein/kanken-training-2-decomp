#include "game/screen.h"

void func_001b7c9c(struct Screen *screen, int index, int value, const struct NameTable *const *labels)
{
    struct WidgetSlot *slot = screen->slots[index];
    if (slot != 0) {
        if (index == 0x31 || index == 0x32) {
            if (slot->state.label.enabled != 0 && slot->state.label.label_index != value) {
                slot->state.label.label_index = value;
                func_0017a968(widget_display(slot), labels[index - 0x31], value, 1);
            }
        } else {
            func_0017a968(widget_display(slot), labels[index - 0x31], value, 0);
        }
    }
}
