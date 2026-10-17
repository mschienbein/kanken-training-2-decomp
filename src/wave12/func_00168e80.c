#include "game/item_text.h"
#include "game/item_pages.h"
#include "game/text.h"





unsigned short *func_00168e80(struct W12cAlternateTextState *state)
{
    struct ItemPageSet *set = data_0023ecf4;
    struct ItemPage *page = func_0017a4e8(set, state->page_index);
    const struct W12cItemTextList *item =
        (const struct W12cItemTextList *)func_0017a49c(page, state->item_index, 0);
    int count = func_001eb688(item);
    int index;
    /* ORIGINAL BUG: page, item, and individual text pointers are not null-checked. */
    for (index = 0; index < count; ++index)
        func_0015de60(&state->text[index], 0x100, func_001eb694(item, index), 1);
    state->text[index] = 0;
    return state->text;
}
