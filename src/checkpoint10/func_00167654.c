#include "game/notification.h"

struct SavedEntryPair { unsigned short first, second; };
struct ShortEntryCategory { struct SavedEntryPair entries[50]; };
struct LongEntryCategory { struct SavedEntryPair entries[150]; };
struct SavedEntryView {
    unsigned char reserved00[0x58];
    struct LongEntryCategory primary[12];
    struct ShortEntryCategory secondary[12];
};

void func_00167654(int secondary, int category, int index, int first, int second)
{
    struct SavedEntryView *records = (struct SavedEntryView *)&bss_00280af8;
    struct SavedEntryPair *entry;
    /* ORIGINAL BUG: negative category and index values are not rejected. */
    if (!secondary) {
        if (index >= 150)
            return;
        entry = records->primary[category].entries + index;
    } else {
        if (index >= 50)
            return;
        entry = records->secondary[category].entries + index;
    }
    entry->first = first;
    entry->second = second;
}
