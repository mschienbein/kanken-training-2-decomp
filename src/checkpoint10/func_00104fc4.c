#include "game/material.h"
#include "game/exp_heap.h"
#include "game/runtime.h"

struct ResourceArrayEntry {
    unsigned active;
    unsigned char reserved04[0x18];
};



void func_00104fc4(void)
{
    struct ResourceArrayEntry *entries;
    int i;
    bss_002d5ea0.state = 256;
    bss_002d5ea0.count = 512;
    entries = func_0019d798(2, 4, 512 * sizeof(*entries));
    bss_002d5ea0.capacity = 0;
    bss_002d5ea0.buffer = entries;
    for (i = 0; i < (int)bss_002d5ea0.count; ++i)
        entries[i].active = 0;
}
