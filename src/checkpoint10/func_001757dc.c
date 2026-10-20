#include "game/command_state.h"
#include "game/exp_heap.h"

struct ParameterBufferState {
    unsigned char reserved00[0x0c];
    float read_index;
    float write_index;
    unsigned position;
    unsigned char reserved18[0x10];
    unsigned count;
    void *buffer;
};

void func_001757dc(struct ParameterBufferState *state)
{
    state->read_index = 0.0f;
    state->write_index = 0.0f;
    state->count = 0;
    state->buffer = func_0019d798(1, 4, 0x20100);
    state->position = 0;
    func_00181a4c(14, state->position, state->position, 3, 128);
}
