#include "game/layout_draw.h"
#include "game/matrix.h"
#include "game/screen.h"


extern "C" void func_0017a558(LayoutParameterBase *state)
{
    state->current[1] = state->colors[0];
    state->current[0] = state->colors[state->mode == 1];
    state->current[2] = state->colors[state->mode == 2];
    state->current[3] = state->colors[state->mode != 0];
}
