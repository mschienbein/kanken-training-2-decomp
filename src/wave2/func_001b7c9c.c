#include "game/screen.h"

void func_001b7c9c(struct Screen *screen, int index, int value, const struct NameTable *const *labels)
{
    struct WidgetSlot *slot = screen->slots[index];
    if (slot != 0) {
        if (index == 0x31 || index == 0x32) {
            if (slot->enabled != 0 && slot->label_index != value) {
                slot->label_index = value;
                func_0017a968(slot->widget, labels[index - 0x31], value, 1);
            }
        } else {
            func_0017a968(slot->widget, labels[index - 0x31], value, 0);
        }
    }
}
