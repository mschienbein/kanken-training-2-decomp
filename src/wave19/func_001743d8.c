#include "game/item_pages.h"
#include "game/functions.h"




int func_001743d8(const struct ItemPageSet *list)
{
    int total = 0;
    int index;
    if (list->pages) {
        for (index = 0; index < list->count; ++index) {
            struct ItemPage *resource = list->pages[index];
            if (resource)
                total += func_0015f5b0(resource);
        }
    }
    return total;
}
