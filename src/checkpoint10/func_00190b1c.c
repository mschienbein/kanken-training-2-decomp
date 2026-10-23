#include "game/selection.h"


void func_00190b1c(int category, int kind, int x, int y, int width, int height)
{
    struct W6Category *group = &bss_00290120[category];
    int index;
    for (index = 0; index < 12; ++index)
        if (group->items[index].kind == kind)
            break;
    if (index < 12) {
        group->items[index].values[0][0] = x;
        group->items[index].values[1][0] = width;
        group->items[index].values[0][1] = y;
        group->items[index].values[1][1] = height;
    }
}
