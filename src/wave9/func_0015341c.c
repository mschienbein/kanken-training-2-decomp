#include "game/input_runtime.h"

int func_0015341c(struct InputLatches *state, int group, int index)
{
    if (state->acquired[group][index] != 0)
        return 0;
    state->acquired[group][index] = 1;
    return 1;
}
