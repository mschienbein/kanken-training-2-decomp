#include "game/input_runtime.h"

int func_0015341c(struct InputLatchSnapshot *state, int group, int index)
{
    if (state->current[group][index] != 0)
        return 0;
    state->current[group][index] = 1;
    return 1;
}
