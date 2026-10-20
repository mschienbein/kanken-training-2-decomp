#include "game/functions.h"
#include "game/input_runtime.h"
#include "game/service_runtime.h"



void func_0014cbf4(struct InputSessionContext *state, float value)
{
    if (state->active) {
        state->first_gain = value;
        func_00180e14(&bss_004b5284, state->first_gain * state->volume);
    }
}
