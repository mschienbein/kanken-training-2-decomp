#include <stdarg.h>
#include "game/selection.h"





void func_00190d0c(int category, int count, ...)
{
    struct W6Category *state =
        &bss_00290120[category];
    va_list values;
    int i;
    va_start(values, count);
    state->group_count = count;
    /* ORIGINAL BUG: no check limits the supplied count to the code capacity. */
    for (i = 0; i < count; ++i)
        state->patterns[i].selection = va_arg(values, int);
    va_end(values);
}
