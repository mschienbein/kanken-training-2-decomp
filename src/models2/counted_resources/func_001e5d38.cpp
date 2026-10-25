#include "game/item_entries.h"
#include "game/functions.h"

int func_001e5d38(const W8EntryList *list, int category)
{
    int total = 0;
    if (list->entries != 0) {
        for (int index = 0; index < list->count; ++index) {
            const ItemPage *entry = list->entries[index];
            /* [INFERENCE] The accepted signed-byte category accessor may have
             * been armlink-inlined and removed. Same-receiver callers establish
             * the entry contract, not the historical helper identity. */
            if (entry != 0 && w19_item_category(entry) == category)
                total += func_0015f5b0(entry);
        }
    }
    return total;
}
