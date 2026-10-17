#include "game/item_pages.h"
#include "game/text.h"
#include "game/selection.h"



struct W7dSelectionState {
    unsigned char reserved00[0x4c];
    struct W7dMenuControl *control;
    unsigned char reserved50[0x64];
    int item_index;
    unsigned reservedb8;
    int page_index;
    int mode;
    unsigned reservedc4;
    int active;
    int side;
    unsigned char reservedd0[0xb0];
    unsigned char has_link;
};

extern struct MenuItem *func_0017a49c(struct ItemPage *page, int index, int *changed);

static signed char item_kind(const struct MenuItem *item)
{
    return item->text.kind;
}

void func_0015fae4(struct W7dSelectionState *state)
{
    struct ItemPageSet *set = data_0023ecf4;
    /* ORIGINAL BUG: page, item and text lookup failures are used without null checks. */
    struct ItemPage *page = func_0017a4e8(set, state->page_index);
    struct MenuItem *item = func_0017a49c(page, state->item_index, 0);
    int kind = item_kind(item);
    state->active = 0;
    state->mode = 0;
    switch (kind) {
    case 0: case 1: case 3: case 4:
    case 8: case 9: case 10: case 11:
    case 12: case 13: case 14: case 15:
    case 20: case 21: case 22: case 23:
    case 25: case 32:
        break;
    default:
        state->active = 1;
        if (func_0017adc4(func_001791e0(&item->text, 0)) == 1)
            state->mode = 1;
        else
            state->mode = 2;
        break;
    }
    state->side = func_00179148() ? 11 : 12;
    if (func_00179090(item))
        state->has_link = 1;
    else
        state->has_link = 0;
    if (state->mode)
        func_00179068(state->control, 2, 1);
    else
        func_00179068(state->control, 2, 0);
}
