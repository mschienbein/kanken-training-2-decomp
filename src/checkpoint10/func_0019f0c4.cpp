#include "game/drawing_system.h"
#include "game/functions.h"
#include "game/runtime.h"

struct RenderTarget;
struct W23JDrawingBuffers {
    unsigned char reserved00[0x0c];
    int count;
    unsigned handles[2];
    int index;
};
/* INTEGRATOR: differs from src/wave3/func_0019f170.cpp: ready at +0x10 is
 * a drawing-buffer-list pointer, dereferenced here after the same guard. */
struct DrawingSystem {
    unsigned char reserved00[4];
    unsigned char initialized;
    unsigned char reserved05[7];
    unsigned mode;
    W23JDrawingBuffers *ready;
    unsigned char reserved14[0x90];
    RenderTarget *target;
};


/* INTEGRATOR: move to include/game/drawing_system.h */
extern "C" {
void func_0011de68(unsigned handle, unsigned mask, unsigned offset,
                   unsigned count, unsigned flags);

void func_0019f0c4(int requested);
}

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
