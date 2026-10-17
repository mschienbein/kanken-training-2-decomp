#include "game/functions.h"
struct CountedResource {
    unsigned char reserved00[6];
    signed char category;
    unsigned char reserved07[5];
    int count;
    unsigned char reserved10[4];
    signed char weight;
    unsigned char reserved15[7];
    const unsigned char *values;
};

int func_0015f5b0(const struct CountedResource *resource)
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
