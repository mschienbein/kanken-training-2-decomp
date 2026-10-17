#include "game/runtime.h"

void func_001047c0(void)
{
    if (bss_0049da8c.allocation.buffer) {
        func_0019dc20(bss_0049da8c.allocation.buffer);
        bss_0049da8c.allocation.buffer = 0;
        bss_0049da8c.allocation.count = 0;
        bss_0049da8c.allocation.capacity = 0;
    }
}
