#include "game/selection.h"
/* INTEGRATOR: differs from include/game/selection.h: stores prove four signed
 * halfword values in W6CategoryItem's previously opaque eight-byte payload. */
struct W6CategoryRecord {
    short kind;
    short count;
    unsigned char values[6];
};
struct W6CategoryItem {
    int kind;
    short x, y, width, height;
};
struct W6Category {
    int reserved00;
    short count;
    short reserved06;
    struct W6CategoryRecord records[9];
    short reserved62;
    struct W6CategoryItem items[12];
};


void func_00190b1c(int category, int kind, int x, int y, int width, int height)
{
    struct W6Category *group = &bss_00290120[category];
    int index;
    for (index = 0; index < 12; ++index)
        if (group->items[index].kind == kind)
            break;
    if (index < 12) {
        group->items[index].x = x;
        group->items[index].width = width;
        group->items[index].y = y;
        group->items[index].height = height;
    }
}
