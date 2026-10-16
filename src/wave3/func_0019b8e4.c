#include "game/render_state.h"

void func_0019b8e4(float red, float green, float blue, float alpha)
{
    struct GpuState *state = data_00242fd0;
    state->clear_color[0] = red <= 0.0f ? 0.0f : red >= 1.0f ? 1.0f : red;
    state->clear_color[1] = green <= 0.0f ? 0.0f : green >= 1.0f ? 1.0f : green;
    state->clear_color[2] = blue <= 0.0f ? 0.0f : blue >= 1.0f ? 1.0f : blue;
    state->clear_color[3] = alpha <= 0.0f ? 0.0f : alpha >= 1.0f ? 1.0f : alpha;
}
