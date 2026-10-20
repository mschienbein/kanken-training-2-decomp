#include "game/render_state.h"
#include "game/screen.h"



void func_0019e414(unsigned features)
{
    struct GpuState *state = data_00242fd0;
    unsigned changed = features & ~state->excluded_flags;
    if (changed & 1) state->dirty |= 0x100000;
    if (changed & 2) state->dirty |= 0x10000;
    if (changed & 4) state->dirty |= 0x1800000;
    if (changed & 8) state->dirty |= 0x600000;
    if (changed & 0x10) state->dirty |= 0x10000;
    if (changed & 0x20) state->dirty |= 0x80000;
    if (changed & 0x40) {
        state->dirty |= 0x2004038;
        func_0019cecc(state->first_constants, 0x84);
        func_0019cecc(state->second_constants, 0x84);
    }
    if (changed & 0x80) state->dirty |= 0x1c00;
    if (changed & 0x100) state->dirty |= 1;
    if (changed & 0x200) state->dirty |= 0x80c2;
    if (changed & 0x400) state->dirty |= 4;
    if (changed & 0x800) state->dirty |= 0x100;
    if (changed & 0x1000) state->dirty |= 0x200;
    state->requested_flags |= changed;
}
