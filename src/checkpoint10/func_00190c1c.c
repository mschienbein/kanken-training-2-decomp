#include "game/selection.h"
#include <stdarg.h>



void func_00190c1c(int category, int kind, int count, ...)
{
    struct W6Category *group = &bss_00290120[category];
    int index;
    for (index = 0; index < 9; ++index)
        if (group->patterns[index].selection == kind)
            break;
    if (index < 9) {
        int item;
        va_list arguments;
        va_start(arguments, count);
        group->patterns[index].value = count;
        /* ORIGINAL BUG: counts above six overrun this record's value array. */
        for (item = 0; item < count; ++item)
            group->patterns[index].details.members[item] = va_arg(arguments, int);
        va_end(arguments);
    }
}
