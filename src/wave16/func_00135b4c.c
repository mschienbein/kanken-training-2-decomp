#include "game/functions.h"
#include "game/runtime.h"
#include "game/exp_heap.h"

struct BufferEntry44 {
    unsigned active;
    unsigned char reserved04[40];
};


void func_00135b4c(int count)
{
    struct BufferEntry44 *buffer =
        func_0019d798(1, 4, count * sizeof(struct BufferEntry44));
    int i;
    bss_0049da8c.allocation.capacity = 0;
    bss_0049da8c.allocation.buffer = buffer;
    bss_0049da8c.allocation.count = count;
    /* ORIGINAL BUG: allocation failure is unchecked before initializing entries. */
    for (i = 0; i < count; ++i)
        buffer[i].active = 0;
}
