#include "game/functions.h"
#include "game/resource_runtime.h"




int func_0017731c(struct W17SecondaryState *state)
{
    if (state->active) {
        func_0019d95c(15, 1, 0);
        state->active = 0;
    }
    state->value = 0;
    return 0;
}
