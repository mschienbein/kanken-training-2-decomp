#include <string.h>

/* INTEGRATOR: differs from include/game/notification.h: refines the existing
 * NotificationPayload reserved bytes with history rows, packed result flags,
 * and two four-byte progress records; all existing fields/offsets are retained. */
struct W10HistoryGrade {
    short lists[32][20];
};

struct W10CourseProgress {
    signed char grade;
    signed char stage;
    short progress;
};

struct NotificationPayload {
    unsigned int reserved00[12];
    unsigned char reserved30[2];
    unsigned char flags;
    unsigned char reserved33[0x25a5];
    struct W10HistoryGrade history[12];
    unsigned char reserved61d8[0x660];
    unsigned char result_flags[12][0x640];
    unsigned char reservedb338[0x3f0];
    struct W10CourseProgress primary;
    struct W10CourseProgress secondary;
    unsigned char reservedb730[0x10];
};

struct SavedNotification {
    int index;
    unsigned int reserved;
    struct NotificationPayload payload;
};

extern struct NotificationPayload bss_00280af8;

static __forceinline short *history_entries(struct NotificationPayload *payload,
                                             int grade, int category)
{
    return payload->history[grade].lists[category];
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
