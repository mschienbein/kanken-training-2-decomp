#include "game/item_pages.h"

struct ItemPage;

/* [INFERENCE] callee identity: inlined by armlink; the eligible body reads ItemPage.count. */
extern int func_001c6af4(const struct ItemPage *page);

int func_00172cf0(const struct ItemPageSet *set)
{
    int total = 0;
    int i;
    if (set->pages) {
        for (i = 0; i < set->count; ++i) {
            if (set->pages[i])
                total += func_001c6af4(set->pages[i]);
        }
    }
    return total;
}
