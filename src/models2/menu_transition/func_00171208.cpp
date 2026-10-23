#include "game/transition.h"

/* [INFERENCE] The accepted completion-setter role may have been inlined and
 * removed by armlink; its historical identity is not recovered. The helper
 * returns the very same typed receiver passed to it. */
void func_00171208(MenuTransitionState *state, int next_state)
{
    state = set_completion(state, next_state);
    state->modes[state->stack_index] = 0;
    func_0017433c(0, (int)func_0017adb4() * 60);
    func_00181b68(7, 1, 1);
    func_00177b10(state, data_00218648);
}
