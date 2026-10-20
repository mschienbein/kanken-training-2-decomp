#include "game/runtime.h"
#include "game/material.h"

void func_00104f90(void)
{
    if (bss_002d5ea0.buffer) {
        func_0019dc20(bss_002d5ea0.buffer);
        bss_002d5ea0.buffer = 0;
        bss_002d5ea0.count = 0;
        bss_002d5ea0.state = 0;
        bss_002d5ea0.capacity = 0;
    }
}
