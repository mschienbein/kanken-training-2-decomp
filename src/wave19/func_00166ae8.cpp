#include "game/category_progress.h"

extern "C" void func_00166ae8(struct CategoryScene *state, int value, int slot)
{
    if (slot == -1) {
        state->previous_states[state->active_layer] = state->states[state->active_layer];
        state->states[state->active_layer] = value;
    } else {
        state->previous_states[slot] = state->states[slot];
        state->states[slot] = value;
    }
    state->phase = 0;
}
