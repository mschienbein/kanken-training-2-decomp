#include "game/item_entries.h"

/* [INFERENCE] armlink-inlined item category accessor removed by --remove;
 * identity inferred from the signed-byte read of the item category. */
int w19_item_category(const struct W8ItemEntry *entry)
{
    return entry->first;
}

/* [INFERENCE] armlink-inlined item value accessor removed by --remove;
 * identity inferred from the word read of the item value. */
unsigned w19_item_value(const struct W8ItemEntry *entry)
{
    return entry->value0c;
}
