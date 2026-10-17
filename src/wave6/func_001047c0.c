#include "game/runtime.h"

void func_001047c0(void)
{
    if (bss_0049da8c.buffer) {
        func_0019dc20(bss_0049da8c.buffer);
        bss_0049da8c.buffer = 0;
        bss_0049da8c.count = 0;
        bss_0049da8c.capacity = 0;
    }
}
