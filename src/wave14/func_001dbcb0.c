#include "game/notification.h"
#include "game/notification_selection.h"
#include "game/text_renderer.h"
void func_001dbcb0(int category, int kind, int index, int *value)
{
    const struct NotificationSelection *table = &bss_00280af8.storage.selection;
    /* ORIGINAL BUG: negative category/kind/index inputs are not checked. */
    if (index < 20) {
        const short *entry = &table->history[category].lists[kind][index];
        if (value)
            *value = *entry;
    }
}
