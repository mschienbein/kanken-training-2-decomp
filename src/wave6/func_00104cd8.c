#include "game/runtime.h"

void func_00104cd8(void)
{
    if (bss_0049cbc0.allocation.buffer) {
        func_0019dc20(bss_0049cbc0.allocation.buffer);
        bss_0049cbc0.allocation.buffer = 0;
        bss_0049cbc0.allocation.count = 0;
        bss_0049cbc0.allocation.capacity = 0;
    }
}
