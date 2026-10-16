#include "game/item_pages.h"

struct NavigationState {
    unsigned char reserved00[0xb4];
    int item;
    int count;
    int page;
};


int func_00178e9c(struct NavigationState *state, int step, int select_item)
{
    struct ItemPage *page;
    if (state->page == 0 && step < 0)
        return 0;
    if (state->page == state->count - 1 && step > 0)
        return 0;
    state->page += step;
    if (select_item) {
        page = func_0017a4e8(data_0023ecf4, state->page);
        if (step < 0)
            state->item = page->count - 1;
        else
            state->item = 0;
    } else {
        state->item = 0;
    }
    return 1;
}
