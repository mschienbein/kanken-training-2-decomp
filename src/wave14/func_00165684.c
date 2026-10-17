#include "game/event_route.h"
unsigned *func_00165684(struct DrawCommandState *state, unsigned *commands)
{
    state->vertices[state->vertex_count] = state->current;
    ++state->vertex_count;
    if (state->vertex_count + 1 > 26 || state->row_count + 7 > 32 || state->secondary_count + 7 > 32)
        commands = func_0016b42c(state, commands);
    return commands;
}
