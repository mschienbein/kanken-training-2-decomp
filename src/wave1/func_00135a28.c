#include "game/runtime.h"

void func_00135a28(void)
{
    if (bss_0049da8c.buffer) {
        func_0019dc20(bss_0049da8c.buffer);
        bss_0049da8c.buffer = 0;
        bss_0049da8c.count = 0;
        bss_0049da8c.capacity = 0;
    }
}
