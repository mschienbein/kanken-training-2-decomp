#include "game/input_runtime.h"

void func_0013712c(struct InputSessionContext *state)
{
    if (state->active != 0) {
        state->selected = -1;
        state->active = 0;
    }
}
