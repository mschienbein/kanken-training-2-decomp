#include "game/command_buffer.h"
#include "game/material.h"
#include "game/render_state.h"
#include <string.h>

struct GpuCommandPacket { unsigned value, header; };
/* Copy a complete hardware packet, leaving the shared cursor byte-addressed. */
static __forceinline void write_command(unsigned value, unsigned header)
{
    const struct GpuCommandPacket packet = { value, header };
    memcpy(data_00242fc8, &packet, sizeof(packet));
    data_00242fc8 += sizeof(packet);
}

/* ORIGINAL BUG: each original capacity guard checks only cursor < end before
 * emitting two words; it does not prove that eight bytes remain available. */
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
            if (data_00242fc8 < data_00242fcc) { write_command(data_00242fd8, 0x10107); }
            state->depth_enabled = enabled;
            state->dirty |= 0x100;
        }
        return;
    case 0xbf2:
        if (state->logic_enabled != enabled || state->force_update) {
            if (enabled) {
                if (data_00242fc8 < data_00242fcc) { write_command(0, 0x20100); }
            } else {
                if (data_00242fc8 < data_00242fcc) { write_command(state->blend_enabled ? data_00242fd4 : 0x1010000, 0xf0101); }
                if (data_00242fc8 < data_00242fcc) { write_command(0x100, 0x20100); }
            }
            state->logic_enabled = enabled;
            state->dirty |= 0x100;
        }
        return;
    case 0xbe2:
        if (state->blend_enabled != enabled || state->force_update) {
            if (state->logic_enabled) {
                if (data_00242fc8 < data_00242fcc) { write_command(0, 0x20100); }
            } else {
                if (data_00242fc8 < data_00242fcc) { write_command(enabled ? data_00242fd4 : 0x1010000, 0xf0101); }
                if (data_00242fc8 < data_00242fcc) { write_command(0x100, 0x20100); }
            }
            state->blend_enabled = enabled;
            state->dirty |= 0x100;
        }
        return;
    case 0x6780:
        if (state->feature6780 != enabled || state->force_update) {
            if (data_00242fc8 < data_00242fcc) { write_command(enabled != 0, 0x10062); }
            if (data_00242fc8 < data_00242fcc) { write_command(enabled != 0, 0x10118); }
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
            if (data_00242fc8 < data_00242fcc) { write_command(data_00242fdc, 0x10105); }
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
                if (data_00242fc8 < data_00242fcc) { write_command(((state->cull_face == 0x404) !=
                                    (state->front_face == 0x901)) ? 2 : 1, 0xf0040); }
            } else {
                if (data_00242fc8 < data_00242fcc) { write_command(0, 0xf0040); }
            }
            state->cull_enabled = enabled;
        }
        return;
    }
}
