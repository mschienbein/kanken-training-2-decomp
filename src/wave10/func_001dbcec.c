#include <string.h>

#include "game/notification.h"

static __forceinline short *history_entries(struct NotificationPayload *payload,
                                             int grade, int category)
{
    return payload->storage.fields.history[grade].lists[category];
}

static __forceinline void insert_history(struct NotificationPayload *payload,
                                         int grade, int category, int value)
{
    short *entry = history_entries(payload, grade, category);
    short stored = value;
    short *entries = entry;
    int index;
    for (index = 0; index < 20; index++, entry++) {
        if (*entry == value)
            return;
        if (*entry == -1) {
            *entry = stored;
            return;
        }
    }
    /* ORIGINAL BUG: the original uses memcpy for these overlapping ranges. */
    memcpy(entries, entries + 1, 19 * sizeof(short));
    entries[19] = stored;
}

void func_001dbcec(int grade, int category, int value)
{
    insert_history(&bss_00280af8, grade, category, value);
}
