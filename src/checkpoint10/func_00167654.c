#include "game/notification.h"


void func_00167654(int secondary, int category, int index, int first, int second)
{
    struct NotificationSelection *records = &bss_00280af8.storage.selection;
    struct W14ValuePair *entry;
    /* ORIGINAL BUG: negative category and index values are not rejected. */
    if (!secondary) {
        if (index >= 150)
            return;
        entry = records->primary[category] + index;
    } else {
        if (index >= 50)
            return;
        entry = records->secondary[category] + index;
    }
    entry->value = first;
    entry->extra = second;
}
