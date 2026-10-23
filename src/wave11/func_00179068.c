#include "game/selection.h"

void func_00179068(struct W7dMenuControl *control, int index, int enabled)
{
    unsigned *flags = control->flags[index];
    if (flags != 0) {
        if (enabled)
            *flags |= 4;
        else
            *flags &= ~4u;
    }
}
