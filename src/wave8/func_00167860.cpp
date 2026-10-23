#include "game/item_entries.h"

extern "C" void func_00167860(W8EntryList *list, int count)
{
    list->count = count;
    list->entries = static_cast<ItemPage **>(func_00193110(count * sizeof(*list->entries), data_002232d5));
    /* ORIGINAL BUG: the entry array allocation is not checked before it is filled. */
    for (int i = 0; i < list->count; ++i) {
        ItemPage *entry = static_cast<ItemPage *>(func_0019d788(sizeof(ItemPage)));
        if (entry != 0)
            entry = func_0015f84c(entry);
        list->entries[i] = entry;
    }
}
