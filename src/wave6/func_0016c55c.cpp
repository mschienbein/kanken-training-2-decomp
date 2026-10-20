#include "game/menu_controller.h"
#include "game/selection.h"
struct W6HandlePanel10;
struct W6DisplayHandle;

extern "C" void func_0016c55c(struct ExitSelectionScene *state)
{
    func_00178e98(state->controls, 0);
    if (state->previous[0] == 1 || state->previous[0] == 2)
        func_00171000(state->objects, -1, 0.0f, 0.0f);
    if (state->current[0] == 1) {
        func_00173d8c(state->handle_panel, 27);
        func_00171000(state->objects, 8, 0.0f, 0.0f);
    } else if (state->current[0] == 2) {
        func_00173d8c(state->handle_panel, 28);
        func_00171000(state->objects, 9, 0.0f, 0.0f);
    }
}
