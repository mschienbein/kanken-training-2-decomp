#include "game/selection.h"
int func_001674e4(int category, int kind, int mode)
{
    int index;
    const struct W6Category *entry = &bss_00290120[category];
    for (index = 0; index < entry->count; ++index)
        if (entry->items[index].kind == kind)
            break;
    /* ORIGINAL BUG: an absent kind is used as a one-past-table index. */
    return entry->items[index].values[mode][0];
}
