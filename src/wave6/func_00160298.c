#include "game/selection.h"
#include "game/item_pages.h"

struct W6PagedScreen {
    unsigned char reserved00[0x4c];
    struct MenuControlSet *list;
    unsigned char reserved50[0x60];
    int mode;
    int item;
    int page_count;
    int page;
};

static __inline struct ItemPage *current_page(struct ItemPageSet *set, int index)
{
    return func_0017a4e8(set, index);
}

/* [INFERENCE] callee identity: inlined by armlink; the eligible body reads ItemPage.count. */
extern int func_001c6af4(const struct ItemPage *page);

void func_00160298(struct W6PagedScreen *screen)
{
    struct ItemPage *page = current_page(data_0023ecf4, screen->page);
    func_00178e98(screen->list, 1);
    if (screen->mode == 0 || (screen->mode == 2 && func_00178144() == 31)) {
        if (screen->page != 0) {
            func_001780c4(screen->list, 1, 0, 1);
            func_001780c4(screen->list, 1, 1, 1);
        } else {
            func_001780c4(screen->list, 1, 0, 0);
            func_001780c4(screen->list, 1, 1, screen->item > 0);
        }
        if (screen->page == screen->page_count - 1) {
            func_001780c4(screen->list, 1, 3, 0);
            func_001780c4(screen->list, 1, 2, func_001c6af4(page) - 1 > screen->item);
        } else {
            func_001780c4(screen->list, 1, 3, 1);
            func_001780c4(screen->list, 1, 2, 1);
        }
    } else {
        func_001780c4(screen->list, 1, 0, 0);
        func_001780c4(screen->list, 1, 1, screen->item > 0);
        func_001780c4(screen->list, 1, 2, func_001c6af4(page) - 1 > screen->item);
        func_001780c4(screen->list, 1, 3, 0);
    }
}
