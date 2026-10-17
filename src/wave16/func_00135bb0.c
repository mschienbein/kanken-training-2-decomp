#include "game/functions.h"
#include "game/graphics_resources.h"
#include "game/runtime.h"

struct BufferEntry48 {
    unsigned active;
    unsigned char reserved04[44];
};

extern void *func_0019d798(unsigned heap, unsigned alignment, unsigned size);

void func_00135bb0(int count)
{
    struct BufferEntry48 *buffer =
        func_0019d798(1, 4, count * sizeof(struct BufferEntry48));
    int i;
    bss_0049da58.buffers.capacity = 0;
    bss_0049da58.buffers.buffer = buffer;
    bss_0049da58.buffers.count = count;
    /* ORIGINAL BUG: allocation failure is unchecked before initializing entries. */
    for (i = 0; i < count; ++i)
        buffer[i].active = 0;
}
