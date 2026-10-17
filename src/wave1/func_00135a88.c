#include "game/render_resource.h"
#include "game/runtime.h"

void func_00135a88(void)
{
    if (bss_0049d93c.allocation.buffer) {
        func_0019dc20(bss_0049d93c.allocation.buffer);
        bss_0049d93c.allocation.buffer = 0;
        bss_0049d93c.allocation.count = 0;
        bss_0049d93c.allocation.capacity = 0;
    }
}
