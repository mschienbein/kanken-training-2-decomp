#include "game/selection.h"
#include <stdarg.h>

/* INTEGRATOR: differs from include/game/selection.h: nine ten-byte records
 * occupy the previously opaque region from 0x08 through 0x61. */
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


void func_00190c1c(int category, int kind, int count, ...)
{
    struct W6Category *group = &bss_00290120[category];
    int index;
    for (index = 0; index < 9; ++index)
        if (group->records[index].kind == kind)
            break;
    if (index < 9) {
        int item;
        va_list arguments;
        va_start(arguments, count);
        group->records[index].count = count;
        /* ORIGINAL BUG: counts above six overrun this record's value array. */
        for (item = 0; item < count; ++item)
            group->records[index].values[item] = va_arg(arguments, int);
        va_end(arguments);
    }
}
