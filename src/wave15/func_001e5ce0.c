#include "game/item_pages.h"

struct ItemPage;

extern int func_0015f6ec(const struct ItemPage *page);

int func_001e5ce0(const struct ItemPageSet *set)
{
    int total = 0;
    int i;
    if (set->pages) {
        for (i = 0; i < set->count; ++i) {
            if (set->pages[i])
                total += func_0015f6ec(set->pages[i]);
        }
    }
    return total;
}
