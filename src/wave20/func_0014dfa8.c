#include "game/input_runtime.h"


int func_0014dfa8(struct InputController *state, int channel,
    const struct ExtendedInputUpdate *values)
{
    if (values->flags & 1)
        state->channels[channel].value = values->value;
    if (values->flags & 4) {
        state->channels[channel].mode = values->mode;
        state->channels[channel].parameters[0] = values->parameters[0];
        state->channels[channel].parameters[1] = values->parameters[1];
        state->channels[channel].parameters[2] = values->parameters[2];
        state->channels[channel].parameters[3] = values->parameters[3];
        state->channels[channel].parameters[4] = values->parameters[4];
        state->channels[channel].parameters[5] = values->parameters[5];
        state->channels[channel].parameters[6] = values->parameters[6];
        state->channels[channel].parameters[7] = values->parameters[7];
        state->channels[channel].parameters[8] = values->parameters[8];
        state->channels[channel].parameters[9] = values->parameters[9];
        state->channels[channel].parameters[10] = values->parameters[10];
        state->channels[channel].parameters[11] = values->parameters[11];
    }
    if (values->flags & 2) {
        state->channels[channel].data[0] = values->data[0];
        state->channels[channel].data[1] = values->data[1];
        state->channels[channel].data[2] = values->data[2];
        state->channels[channel].data[3] = values->data[3];
        state->channels[channel].data[4] = values->data[4];
    }
    return func_00180e74(&bss_004b5284, channel, values);
}
