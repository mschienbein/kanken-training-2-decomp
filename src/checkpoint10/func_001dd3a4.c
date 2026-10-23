#include "game/selection.h"

int func_001dd3a4(int category, int kind)
{
    int index;
    struct W6Category *record = &bss_00290120[category];
    for (index = 0; index < record->count; ++index) {
        if (record->items[index].kind == kind)
            return index;
    }
    return -1;
}
