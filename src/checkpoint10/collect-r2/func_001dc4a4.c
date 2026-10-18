#include "game/selection.h"
#include "game/item_text.h"


int func_001dc4a4(int group)
{
    int completed = 0;
    int count = func_001698f4(group, -1);
    int index;
    for (index = 0; index < count; ++index) {
        if (func_001dc90c(group, index, -1) &&
            func_001dc574(group, index, -1))
            ++completed;
    }
    /* ORIGINAL BUG: an empty group still uses a zero divisor. */
    return (int)((float)completed / (float)count * 100.0f);
}

/* Existing helpers move here without adding new function credit. */
int func_001dc574(int group, int index, int value)
{
    return func_0016992c(group, index, 2, value);
}

int func_001dc90c(int group, int index, int value)
{
    return func_0016992c(group, index, 1, value);
}
