#include "game/material.h"

extern struct ResourceArray bss_002d5ea0;

struct ResourceQueryEntry {
    unsigned int present;
    unsigned int reserved04[3];
    unsigned int fallback;
    unsigned int preferred;
    unsigned int reserved18;
};

unsigned int func_0014ddf0(int index)
{
    const struct ResourceQueryEntry *entry = (const struct ResourceQueryEntry *)bss_002d5ea0.buffer + index;
    unsigned int value = entry->present;
    if (value == 0)
        return value;
    value = entry->preferred;
    if (value == 0)
        value = entry->fallback;
    return value;
}
