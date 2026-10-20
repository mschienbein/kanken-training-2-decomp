#include "game/item_entries.h"

int func_0015f5b0(const struct W8ItemEntry *resource)
{
    int total = 0;
    int remaining = resource->count;
    const unsigned char *values;
    if (remaining > 0)
        values = resource->values;
    while (remaining > 0) {
        if (*values++ == 1)
            total += resource->weight;
        --remaining;
    }
    return total;
}
