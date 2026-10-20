#include "game/drawing_system.h"
#include "game/functions.h"
#include "game/runtime.h"


void func_0019f0c4(int requested)
{
    if (!bss_002d3dd0.initialized)
        func_0019f568();
    if (bss_002d3dd0.mode != 0x400 && bss_002d3dd0.mode != 0x410 &&
        bss_002d3dd0.mode != 0x401)
        func_0019f568();
    if (!bss_002d3dd0.ready)
        func_0019f568();
    if (requested) {
        W23JDrawingBuffers *buffers = bss_002d3dd0.ready;
        unsigned handle = buffers->index == buffers->count - 1 ?
            buffers->handles[0] : buffers->handles[buffers->index + 1];
        func_0011de68(handle, 0x500, 0, 0, 0);
        if (func_0019ceb4())
            func_0019f568();
    }
}
