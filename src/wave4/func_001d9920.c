#include "game/functions.h"

struct ExtendedTimingState {
    struct TimingState timing;
    unsigned int value;
    float start;
    float end;
    unsigned char enabled;
    unsigned char finished;
};

struct ExtendedTimingState *func_001d9920(struct ExtendedTimingState *state)
{
    func_0017e818(&state->timing);
    state->value = 0;
    state->start = 0.0f;
    state->end = 1.0f;
    state->enabled = 0;
    state->finished = 0;
    return state;
}
