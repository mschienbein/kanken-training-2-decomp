#include "game/render_state.h"

extern void func_001272bc(unsigned index);
void func_00193b2c(unsigned index, unsigned components, unsigned format,
                   unsigned normalized, unsigned stride, unsigned offset)
{
    struct GpuState *state = data_00242fd0;
    if (state->attributes[index].components != components) {
        state->attributes[index].components = components;
        state->dirty |= 0x40;
    }
    if (state->attributes[index].format != format) {
        state->attributes[index].format = format;
        state->dirty |= 0x40;
    }
    if (state->attributes[index].normalized != normalized) {
        state->attributes[index].normalized = normalized;
        state->dirty |= 0x40;
    }
    if (state->attributes[index].stride != stride) {
        state->attributes[index].stride = stride;
        state->dirty |= 0x40;
    }
    if (state->attributes[index].offset != offset) {
        state->attributes[index].offset = offset;
        state->dirty |= 0x40;
    }
    if (state->attributes[index].buffer != state->array_buffer) {
        state->attributes[index].buffer = state->array_buffer;
        state->dirty |= 0x40;
    }
    func_001272bc(index);
}
