#include "game/runtime.h"

void func_00104cd8(void)
{
    if (bss_0049cbc0.buffer) {
        func_0019dc20(bss_0049cbc0.buffer);
        bss_0049cbc0.buffer = 0;
        bss_0049cbc0.count = 0;
        bss_0049cbc0.capacity = 0;
    }
}
