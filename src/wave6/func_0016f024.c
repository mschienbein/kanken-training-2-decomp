#include "game/functions.h"
#include <stddef.h>
#include <string.h>

#include "game/runtime.h"

static __forceinline void reset_motion(struct MotionState *state)
{
    if (state) {
        if (state->allocation) {
            func_0019dc20(state->allocation);
            state->allocation = 0;
            state->count = 0;
        }
        memset(state, 0, offsetof(struct MotionState, allocation));
        state->scale = 1.0f;
    }
}

void func_0016f024(struct MotionState *state)
{
    reset_motion(state);
}
