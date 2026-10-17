#include "game/functions.h"
#include "game/shader.h"



void func_001175f4(struct ShaderConfiguration *state)
{
    int index;
    if (state->initialized) {
        for (index = 0; index < 4; ++index)
            func_00193af4(index);
        func_0018f0dc(1, &state->texture);
        state->initialized = 0;
        func_00197c68(0);
        func_0019b344(state->program);
    }
}
