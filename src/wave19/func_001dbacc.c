#include "game/notification.h"

int func_001dbacc(int track, int age, int *category, int *x, int *y,
                   unsigned char *kind)
{
    /* A typed view of the serialized notification storage's history region. */
    const struct NotificationFields *const storage = &bss_00280af8.storage.fields;
    const struct NotificationHistoryTrack *history = &storage->tracks[track];
    const struct NotificationHistoryEntry *entry;
    int index = history->current - age;
    if (0 > index)
        index += 10;
    if (10 <= (unsigned)index)
        return 0;
    entry = &history->entries[index];
    if (category != 0)
        *category = entry->category;
    if (x)
        *x = entry->x;
    if (y)
        *y = entry->y;
    if (kind)
        *kind = entry->kind;
    return entry->status;
}
