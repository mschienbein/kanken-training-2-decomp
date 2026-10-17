#include "game/command_state.h"
void func_001774e0(struct W11ControlState *state, int value)
{
    if (state->status08 != 2) {
        func_00181a4c(15, 1, 1, value, 4);
        func_00186598();
        state->status08 = 1;
    }
}
