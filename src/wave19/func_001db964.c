#include "game/notification.h"
#include <string.h>

void func_001db964(int group, int index, unsigned *value, float *x, float *y)
{
    const struct TransformEntry *entry = &bss_00280af0.transforms[group][index];
    if (value)
        *value = entry->reserved00;
    if (x)
        memcpy(x, &entry->x, sizeof(*x));
    if (y)
        memcpy(y, &entry->y, sizeof(*y));
}
