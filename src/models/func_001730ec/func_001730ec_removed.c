#include "game/item_entries.h"


unsigned func_001730ec(struct W8EntryList *list, int category)
{
    unsigned total = 0;
    int i;
    if (list->entries) {
        for (i = 0; i < list->count; ++i) {
            struct ItemPage *entry = list->entries[i];
            /* [INFERENCE] callee identities: armlink-inlined item accessors
             * removed by --remove; identities inferred from the field reads. */
            if (entry && w19_item_category(entry) == category)
                total += w19_item_value(entry);
        }
    }
    return total;
}
