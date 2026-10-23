#include "game/item_pages.h"

int func_0015f6ec(const struct ItemPage *page)
{
    int count = page->count;
    int total = 0;
    int i;
    for (i = 0; count > i; ++i) {
        if (page->flags[i])
            ++total;
    }
    return total;
}
