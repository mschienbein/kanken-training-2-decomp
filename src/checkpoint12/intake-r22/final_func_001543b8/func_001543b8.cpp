#include "game/input_runtime.h"
#include "game/exp_heap.h"

/* [INFERENCE] The planar elements are four-byte samples; their numerical
 * representation is not established by this routing function alone. */
static void process_block(int index, InputSessionContext *state, int *output)
{
    if (state->active) {
        StreamLockGuard guard(&state->mutex);
        if (state->callbacks[index]) {
            int *channels[4] = {output, output + 160, output + 320, output + 480};
            state->callbacks[index](channels, 160, state->contexts[index]);
        }
    }
}

extern "C" void func_001543b8(InputSessionContext *state, int index, int *output)
{
    process_block(index, state, output);
}
