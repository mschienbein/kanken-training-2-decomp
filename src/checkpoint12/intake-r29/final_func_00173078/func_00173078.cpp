#include "game/item_pages.h"

/* [INFERENCE] The source-defined signed property getter is armlink-inlined
 * and removed. Exact bytes do not prove the historical helper identity or
 * source call graph; retain R1/F03 typed-contract review qualifications. */
int func_00173078(const ItemPageSet *set, int kind)
{
    int total = 0;
    if (set->pages) {
        for (int i = 0; i < set->count; ++i) {
            const ItemPage *page = set->pages[i];
            if (page && item_page_kind(page) == kind)
                total += func_0015f6ec(page);
        }
    }
    return total;
}
