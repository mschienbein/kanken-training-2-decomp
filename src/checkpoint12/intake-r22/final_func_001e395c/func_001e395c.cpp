#include "game/scene_parameters.h"

template <int Count>
static void release_parameters(ParameterValue *(&parameters)[Count])
{
    for (int i = 0; i < Count; ++i) {
        ParameterValue *parameter = parameters[i];
        if (parameter) {
            /* [INFERENCE] callee identity: inlined by armlink; trivial
             * parameter finalization, as in the matching playback unit. */
            func_001852f4(func_001ef884(parameter));
            parameters[i] = 0;
        }
    }
}

void func_001e395c(RestartTransitionScreenW19G *state)
{
    func_0019eb48();
    state->first = func_001b6bf4();
    state->second = func_001b7e00();
    func_001b6d2c(state->first);
    func_001b7f38(state->second);
    /* ORIGINAL BUG: failed allocations are forwarded as null parameters. */
    if (state->buttons) {
        ParameterValue *values[3];
        values[0] = new (data_002232d5) ParameterSelection(19);
        values[1] = new (data_002232d5) ParameterSelection(21);
        ParameterValue *parameter = new (data_002232d5) ParameterValue;
        values[2] = parameter ? func_001757d0(parameter) : 0;
        func_00175654(state->buttons, values);
        release_parameters(values);
    }
    if (state->selector) {
        ParameterValue *values[3];
        values[0] = new (data_002232d5) ParameterSequence(24);
        values[1] = new (data_002232d5) ParameterSequence(26);
        ParameterValue *parameter = new (data_002232d5) ParameterValue;
        values[2] = parameter ? func_00175648(parameter) : 0;
        func_001754d0(state->selector, values);
        release_parameters(values);
    }
    func_00186598();
    func_00177b10(state, data_002184b8);
}
