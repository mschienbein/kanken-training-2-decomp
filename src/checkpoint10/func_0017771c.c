#include "game/command_state.h"
struct QueuedValueState {
    unsigned char reserved00[0x14];
    unsigned value;
};



void func_0017771c(struct QueuedValueState *state, unsigned value)
{
    state->value = value;
    func_00181a4c(14, value, value, 3, 0x80);
}
