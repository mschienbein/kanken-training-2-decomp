#include "game/scene_parameters.h"

template <int Count>
static void release_parameters(ParameterValue *(&parameters)[Count])
{
    for (int i = 0; i < Count; ++i) {
        if (parameters[i]) {
            /* [INFERENCE] callee identity: inlined by armlink; trivial
             * parameter finalization, as in the matching playback unit. */
            func_001852f4(func_001ef884(parameters[i]));
            parameters[i] = 0;
        }
    }
}

void func_001e722c(Tu1140ParameterScene *state)
{
    func_0019eb48();
    /* ORIGINAL BUG: failed allocations are forwarded as null parameters. */
    if (state->selection) {
        ParameterValue *values[2];
        values[0] = new (data_002232d5) ParameterSelection(0);
        ParameterValue *parameter = new (data_002232d5) ParameterValue;
        values[1] = parameter ? func_001757d0(parameter) : 0;
        func_00175654(state->selection, values);
        release_parameters(values);
    }
    if (state->sequence) {
        ParameterValue *values[3];
        values[0] = new (data_002232d5) ParameterSequence(0);
        values[1] = new (data_002232d5) ParameterSequence(1);
        ParameterValue *parameter = new (data_002232d5) ParameterValue;
        values[2] = parameter ? func_00175648(parameter) : 0;
        func_001754d0(state->sequence, values);
        release_parameters(values);
    }
    if (state->buffer)
        func_001757dc(state->buffer);
    func_00186598();
    func_00177b10(state, data_00218888);
}
