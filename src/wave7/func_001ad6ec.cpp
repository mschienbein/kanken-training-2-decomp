#include "game/messages.h"


void func_001ad6ec(struct MessageLayout *layout, WidgetPosition position)
{
    struct MessageEntry *entry = layout->entries[2];
    if (entry != 0 && entry->object != 0) {
        entry->animation_x = position.x;
        entry->animation_y = position.y;
    }
}


