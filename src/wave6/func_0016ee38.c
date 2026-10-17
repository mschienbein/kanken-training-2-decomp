#include <stddef.h>
#include <string.h>

#include "game/runtime.h"

void func_0016ee38(struct MotionState *state)
{
    if (state == 0)
        return;
    if (state->allocation) {
        func_0019dc20(state->allocation);
        state->allocation = 0;
        state->count = 0;
    }
    memset(state, 0, offsetof(struct MotionState, allocation));
    state->scale = 1.0f;
}
