#include "game/graphics_resources.h"


void func_00135a58(void)
{
    if (bss_0049da58.buffers.buffer) {
        func_0019dc20(bss_0049da58.buffers.buffer);
        bss_0049da58.buffers.buffer = 0;
        bss_0049da58.buffers.count = 0;
        bss_0049da58.buffers.capacity = 0;
    }
}
