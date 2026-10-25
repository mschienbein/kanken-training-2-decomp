#include "game/item_pages.h"
#include "game/functions.h"


int func_0015f5b0(const struct ItemPage *resource)
{
    int total = 0;
    int remaining = resource->count;
    const unsigned char *values;
    if (remaining > 0)
        values = resource->flags;
    while (remaining > 0) {
        if (*values++ == 1)
            total += resource->limit;
        --remaining;
    }
    return total;
}
