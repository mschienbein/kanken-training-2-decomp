#include "game/item_entries.h"

int func_001743d8(const struct W8EntryList *list)
{
    int total = 0;
    int index;
    if (list->entries) {
        for (index = 0; index < list->count; ++index) {
            struct W8ItemEntry *resource = list->entries[index];
            if (resource)
                total += func_0015f5b0(resource);
        }
    }
    return total;
}
