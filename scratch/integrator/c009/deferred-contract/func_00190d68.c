#include <stdarg.h>
#include "game/selection.h"

void func_00190d68(int category, int count, ...)
{
    struct W6Category *state = &bss_00290120[category];
    va_list values;
    int i;
    va_start(values, count);
    state->count = count;
    /* ORIGINAL BUG: the count is not checked against the twelve-item capacity. */
    for (i = 0; i < count; ++i)
        state->items[i].kind = va_arg(values, int);
    va_end(values);
}
