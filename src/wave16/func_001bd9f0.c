#include "game/selection.h"

void func_001bd9f0(struct W6SelectionReset *state)
{
    switch (state->stack_index) {
    case 0:
        func_001bda14(state);
        break;
    case 1:
        func_001bdb70(state);
        break;
    }
}
