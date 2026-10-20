#include "game/selection.h"
extern void func_00171000(struct W6DisplayHandle **handles, int index, float x, float y);
extern void func_00173d8c(struct W6HandlePanel10 *panel, int resource);

void func_0016c55c(struct W6SelectionReset *state)
{
    func_00178e98(state->list, 0);
    if (state->previous_modes[0] == 1 || state->previous_modes[0] == 2)
        func_00171000(state->handles, -1, 0.0f, 0.0f);
    if (state->modes[0] == 1) {
        func_00173d8c(state->panel, 27);
        func_00171000(state->handles, 8, 0.0f, 0.0f);
    } else if (state->modes[0] == 2) {
        func_00173d8c(state->panel, 28);
        func_00171000(state->handles, 9, 0.0f, 0.0f);
    }
}
