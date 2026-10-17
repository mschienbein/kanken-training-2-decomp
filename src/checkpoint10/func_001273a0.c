#include "game/command_buffer.h"
#include "game/material.h"
#include "game/render_state.h"
/* INTEGRATOR: differs from include/game/render_state.h: reserved ranges
 * contain the capability flags and force-update flag named below. */
struct GpuState {
    unsigned dirty;
    unsigned char reserved04[8];
    unsigned char force_update;
    unsigned char reserved0d[0xd];
    unsigned char feature6502;
    unsigned char reserved1b[5];
    int viewport_x;
    int viewport_y;
    int viewport_width;
    int viewport_height;
    unsigned char viewport_dirty;
    unsigned char reserved31[3];
    unsigned front_face;
    unsigned cull_face;
    unsigned char cull_enabled;
    unsigned char reserved3d[0x17];
    unsigned char polygon_offset_enabled;
    unsigned char reserved55[0x403];
    struct GpuVertexAttribute attributes[16];
    unsigned element_buffer;
    unsigned array_buffer;
    unsigned deferred_buffer;
    unsigned char reserved5e4[0x34];
    unsigned source_color;
    unsigned source_alpha;
    unsigned destination_color;
    unsigned destination_alpha;
    unsigned char reserved628[0x20];
    unsigned char scissor_enabled;
    unsigned char reserved649;
    unsigned char stencil_enabled;
    unsigned char depth_enabled;
    unsigned char blend_enabled;
    unsigned char logic_enabled;
    unsigned char reserved64e;
    unsigned char feature6780;
    unsigned char reserved650[0x10];
    float clear_color[4];
    unsigned char reserved670[0x24];
    unsigned char feature67a0;
    unsigned char reserved695[0xdb];
    unsigned control;
};

/* INTEGRATOR: differs from src/wave1/func_00195254.c: command generation
 * accesses the cursor as a word pointer; byte-copy users need an explicit cast. */

extern unsigned *data_00242fcc;
extern unsigned data_00242fd4;
extern unsigned data_00242fd8;
extern unsigned data_00242fdc;

/* INTEGRATOR: differs from src/wave5/func_001941a8.c and src/wave6/func_00127398.c:
 * enabled is a GLboolean-style byte (callers pass 0/1); with an int/unsigned
 * parameter the compiler masks it to a byte once at entry, the original does not.
 * Capability enum comparisons are signed. */
void func_001273a0(int capability, unsigned char enabled)
{
    struct GpuState *state = data_00242fd0;
    switch (capability) {
    case 0x6502:
        state->feature6502 = enabled;
        return;
    case 0x67a0:
        state->feature67a0 = enabled;
        return;
    case 0xb71:
        if (state->depth_enabled != enabled || state->force_update) {
            if (enabled)
                data_00242fd8 |= 1;
            else
                data_00242fd8 &= ~1u;
            if (data_00242fc8 < data_00242fcc) {
                *data_00242fc8++ = data_00242fd8;
                *data_00242fc8++ = 0x10107;
            }
            state->depth_enabled = enabled;
            state->dirty |= 0x100;
        }
        return;
    case 0xbf2:
        if (state->logic_enabled != enabled || state->force_update) {
            if (enabled) {
                if (data_00242fc8 < data_00242fcc) {
                    *data_00242fc8++ = 0;
                    *data_00242fc8++ = 0x20100;
                }
            } else {
                if (data_00242fc8 < data_00242fcc) {
                    *data_00242fc8++ = state->blend_enabled ? data_00242fd4 : 0x1010000;
                    *data_00242fc8++ = 0xf0101;
                }
                if (data_00242fc8 < data_00242fcc) {
                    *data_00242fc8++ = 0x100;
                    *data_00242fc8++ = 0x20100;
                }
            }
            state->logic_enabled = enabled;
            state->dirty |= 0x100;
        }
        return;
    case 0xbe2:
        if (state->blend_enabled != enabled || state->force_update) {
            if (state->logic_enabled) {
                if (data_00242fc8 < data_00242fcc) {
                    *data_00242fc8++ = 0;
                    *data_00242fc8++ = 0x20100;
                }
            } else {
                if (data_00242fc8 < data_00242fcc) {
                    *data_00242fc8++ = enabled ? data_00242fd4 : 0x1010000;
                    *data_00242fc8++ = 0xf0101;
                }
                if (data_00242fc8 < data_00242fcc) {
                    *data_00242fc8++ = 0x100;
                    *data_00242fc8++ = 0x20100;
                }
            }
            state->blend_enabled = enabled;
            state->dirty |= 0x100;
        }
        return;
    case 0x6780:
        if (state->feature6780 != enabled || state->force_update) {
            if (data_00242fc8 < data_00242fcc) {
                *data_00242fc8++ = enabled != 0;
                *data_00242fc8++ = 0x10062;
            }
            if (data_00242fc8 < data_00242fcc) {
                *data_00242fc8++ = enabled != 0;
                *data_00242fc8++ = 0x10118;
            }
            state->feature6780 = enabled;
            state->dirty |= 0x100;
        }
        return;
    case 0x8037:
        if (state->polygon_offset_enabled != enabled) {
            state->polygon_offset_enabled = enabled;
            state->dirty |= 4;
        }
        return;
    case 0xb90:
        if (state->stencil_enabled != enabled || state->force_update) {
            if (enabled)
                data_00242fdc |= 1;
            else
                data_00242fdc &= ~1u;
            if (data_00242fc8 < data_00242fcc) {
                *data_00242fc8++ = data_00242fdc;
                *data_00242fc8++ = 0x10105;
            }
            state->stencil_enabled = enabled;
            state->dirty |= 0x100;
        }
        return;
    case 0xc11:
        if (state->scissor_enabled != enabled) {
            state->scissor_enabled = enabled;
            state->dirty |= 0x200;
        }
        return;
    case 0xb44:
        if (state->cull_enabled != enabled || state->force_update) {
            if (enabled) {
                if (data_00242fc8 < data_00242fcc) {
                    *data_00242fc8++ = ((state->cull_face == 0x404) !=
                                        (state->front_face == 0x901)) ? 2 : 1;
                    *data_00242fc8++ = 0xf0040;
                }
            } else {
                if (data_00242fc8 < data_00242fcc) {
                    *data_00242fc8++ = 0;
                    *data_00242fc8++ = 0xf0040;
                }
            }
            state->cull_enabled = enabled;
        }
        return;
    }
}
