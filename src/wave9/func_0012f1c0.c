#include "game/input_runtime.h"

int func_0012f1c0(struct ServiceOutput *state)
{
    if (state->active != 0) {
        func_0013712c(&bss_004b97ac);
        state->selected = -1;
        return state->active = 0;
    }
    return 0;
}
