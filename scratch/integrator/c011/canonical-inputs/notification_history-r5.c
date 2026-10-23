#include "game/notification.h"

static struct NotificationHistoryTrack *history_track(struct NotificationPayload *payload,
                                            int track)
{
    return &payload->storage.fields.tracks[track];
}

static struct NotificationHistoryEntry *history_entry(struct NotificationHistoryTrack *history,
                                            int index)
{
    return &history->entries[index];
}

void func_001dbb70(int track, unsigned char category, unsigned short x,
                  unsigned short y, unsigned char kind)
{
    struct NotificationHistoryTrack *history = history_track(&bss_00280af8, track);
    struct NotificationHistoryEntry *entry;
    ++history->current;
    if (history->current >= 10)
        history->current = 0;
    entry = history_entry(history, history->current);
    entry->status = 1;
    entry->category = category;
    entry->x = x;
    entry->y = y;
    entry->kind = kind;
}
