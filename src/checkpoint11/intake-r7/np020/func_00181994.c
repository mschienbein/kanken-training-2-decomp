#include "game/command_state.h"

struct W11BroadcastState {
    unsigned char reserved00[0x18];
    unsigned char enabled;
};

static void broadcast_targets(int first, const int *targets)
{
    int index;
    for (index = first; index < 3; ++index)
        func_00181a4c(1, targets[index], targets[index], 3, 0x80);
}

void func_00181994(const struct W11BroadcastState *const state)
{
    if (state->enabled != 1) {
        const int targets[3] = { 3, 1, 2 };
        broadcast_targets(0, targets);
        func_00186598();
    }
}
