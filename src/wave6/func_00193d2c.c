#include "game/render_state.h"

void func_00193d2c(unsigned index)
{
    struct GpuState *state = data_00242fd0;
    if (!state->attributes[index].enabled) {
        state->attributes[index].enabled = 1;
        state->dirty |= 0x40;
    }
}
