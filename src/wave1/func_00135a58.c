#include "game/runtime.h"

extern BufferState bss_0049da58;

void func_00135a58(void)
{
    if (bss_0049da58.buffer) {
        func_0019dc20(bss_0049da58.buffer);
        bss_0049da58.buffer = 0;
        bss_0049da58.count = 0;
        bss_0049da58.capacity = 0;
    }
}
