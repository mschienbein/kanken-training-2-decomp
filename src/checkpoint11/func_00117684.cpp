#include "game/shader.h"


static void release_shader(ShaderConfiguration *state)
{
    for (int index = 0; index < 4; ++index)
        func_00193af4(index);
    func_0018f0dc(1, &state->texture);
    state->initialized = 0;
    func_00197c68(0);
    func_0019b344(state->program);
}

ShaderConfiguration *func_00117684(ShaderConfiguration *state)
{
    if (state->initialized)
        release_shader(state);
    return state;
}
