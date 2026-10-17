#include "game/graphics_resources.h"

void func_00135a58(void)
{
    if (bss_0049da58.buffers.buffers.buffer) {
        func_0019dc20(bss_0049da58.buffers.buffers.buffer);
        bss_0049da58.buffers.buffers.buffer = 0;
        bss_0049da58.buffers.buffers.count = 0;
        bss_0049da58.buffers.buffers.capacity = 0;
    }
}
