#include "game/input_runtime.h"

void func_0013712c(struct InputSessionContext *state)
{
    if (state->active != 0) {
        state->mutex.depth = -1;
        state->active = 0;
    }
}
