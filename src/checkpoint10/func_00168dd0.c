#include "game/training_screen.h"
/* [INFERENCE] The unchanged receiver reaches the L041 visibility/reset family;
 * its controls, scroll and message pointers are the same actual objects used
 * by those consumers, not aliases for the unrelated completion state. */

void func_00168dd0(struct L041InitializeScreen *state, int enabled)
{
    if (enabled) {
        func_00179d48(state->controls.animation, state->control_index, 0, 1);
        func_00179d48(state->controls.animation, state->control_index, 1, 0);
        func_001690a8(state, 0);
        func_00169018(state, 1);
        func_0016f174(state->scroll, 0);
        func_00177598(state->message, 0.0f, 0.0f);
    } else {
        func_00179d48(state->controls.animation, state->control_index, 0, 0);
        func_00179d48(state->controls.animation, state->control_index, 1, 1);
        func_001de2c0(state);
        func_00169018(state, 0);
    }
}
