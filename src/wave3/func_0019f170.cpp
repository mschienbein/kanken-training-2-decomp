#include "game/render_state.h"
#include "game/runtime.h"

struct RenderTarget;

struct RenderTargetVTable {
    void (*reserved[4])();
    const unsigned *(*handle)(RenderTarget *target);
};

struct RenderTarget {
    const RenderTargetVTable *vtable;
};

struct DrawingSystem {
    unsigned char reserved00[4];
    unsigned char initialized;
    unsigned char reserved05[7];
    unsigned mode;
    unsigned ready;
    unsigned char reserved14[0x90];
    RenderTarget *target;
};

extern DrawingSystem bss_002d3dd0;
extern "C" void func_0011d330(unsigned flags);

extern "C" void func_0019f170(unsigned red, unsigned green, unsigned blue)
{
    if (!bss_002d3dd0.initialized)
        func_0019f568();
    if (bss_002d3dd0.mode != 0x400 && bss_002d3dd0.mode != 0x410 &&
        bss_002d3dd0.mode != 0x401)
        func_0019f568();
    if (!bss_002d3dd0.ready)
        func_0019f568();
    func_0019b8e4(red * (1.0f / 255.0f), green * (1.0f / 255.0f),
                  blue * (1.0f / 255.0f), 1.0f);
    RenderTarget *target = bss_002d3dd0.target;
    const unsigned *handle = target->vtable->handle(target);
    func_00196d18(0x8d40, *handle);
    func_0011d330(0x4500);
}
