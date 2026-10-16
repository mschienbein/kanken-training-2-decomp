#include "game/item_pages.h"

struct ItemPage *func_0017a4e8(struct ItemPageSet *set, int index)
{
    if (set->entries == 0)
        return 0;
    if (set->pages != 0)
        return set->pages[index];
    return 0;
}
