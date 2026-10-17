#include "game/render_state.h"
#include "game/screen.h"
struct GpuVertexAttribute {
    unsigned offset, components, format, stride, buffer;
    unsigned char normalized, enabled, reserved16[2];
};
/* INTEGRATOR: differs from include/game/render_state.h: dirty propagation reads masks at 4/8 and clears two 0x84-byte regions at 0x10c and 0x190. */
struct GpuState {
    unsigned dirty;
    unsigned requested;
    unsigned excluded;
    unsigned char reserved0c[0x14];
    int viewport_x, viewport_y, viewport_width, viewport_height;
    unsigned char viewport_dirty;
    unsigned char reserved31[0xdb];
    unsigned first_constants[33];
    unsigned second_constants[33];
    unsigned char reserved214[0x244];
    struct GpuVertexAttribute attributes[16];
    unsigned element_buffer, array_buffer, deferred_buffer;
    unsigned char reserved5e4[0x7c];
    float clear_color[4];
    unsigned char reserved670[0x100];
    unsigned control;
};



void func_0019e414(unsigned features)
{
    struct GpuState *state = data_00242fd0;
    unsigned changed = features & ~state->excluded;
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
    state->requested |= changed;
}
