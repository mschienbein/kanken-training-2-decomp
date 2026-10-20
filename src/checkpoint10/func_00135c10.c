#include "game/render_resource.h"
#include "game/exp_heap.h"
#include "game/runtime.h"

struct RuntimeRecord88 {
    unsigned active;
    unsigned char reserved04[0x54];
};



void func_00135c10(int count)
{
    struct RuntimeRecord88 *records = func_0019d798(1, 4, count * sizeof(*records));
    int i;
    bss_0049d93c.allocation.buffer = records;
    bss_0049d93c.allocation.count = count;
    bss_0049d93c.allocation.capacity = 0;
    for (i = 0; i < count; ++i)
        records[i].active = 0;
}
