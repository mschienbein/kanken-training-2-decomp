#include "game/shader.h"

void func_001043f8(void)
{
    int index;
    for (index = 0; index < 3; ++index) {
        if (bss_002d4250.programs[index]) {
            func_0019b3b0(bss_002d4250.programs[index], ~0u);
            func_0019b3b0(bss_002d4250.programs[index], bss_002d4250.shaders[index]);
            func_0019b344(bss_002d4250.programs[index]);
        }
        if (bss_002d4250.shaders[index])
            func_0019b254(bss_002d4250.shaders[index]);
    }
}
