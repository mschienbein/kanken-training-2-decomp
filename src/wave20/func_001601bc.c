#include "game/item_pages.h"

struct NavigationState {
    unsigned char reserved00[0xb4];
    int item;
    int count;
    int page;
};
extern int func_00178e9c(struct NavigationState *state, int step, int select_item);
/* [INFERENCE] callee identity: inlined by armlink; this retained getter reads ItemPage.count. */
extern int func_001c6af4(const struct ItemPage *page);

static __inline struct ItemPage *current_page(struct ItemPageSet *set, int index)
{
    return func_0017a4e8(set, index);
}

int func_001601bc(struct NavigationState *state, int step)
{
    struct ItemPage *page = current_page(data_0023ecf4, state->page);
    /* ORIGINAL BUG: page lookup failures are dereferenced. */
    if ((state->page == 0 && step < 0 && state->item <= 0) ||
        (state->page == state->count - 1 && step > 0 && state->item >= func_001c6af4(page) - 1))
        return 0;
    state->item += step;
    if (step > 0) {
        if (state->item >= func_001c6af4(page))
            return func_00178e9c(state, 1, 1);
    } else if (step != 0) {
        if (state->item < 0)
            return func_00178e9c(state, -1, 1);
    }
    return 1;
}
