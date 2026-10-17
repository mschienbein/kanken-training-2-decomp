#include "game/command_state.h"
void func_001772dc(struct W11ControlState *state, int value)
{
    if (state->status0c != 2) {
        func_00181a4c(15, 0, 0, value, 4);
        func_00186598();
        state->status0c = 1;
    }
}
