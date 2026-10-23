#include "game/event_route.h"
#include <string.h>
unsigned char *func_00165684(struct DrawCommandState *state, unsigned char *commands)
{
    memcpy(state->vertices_storage + state->vertex_count * sizeof(state->current),
           &state->current, sizeof(state->current));
    ++state->vertex_count;
    if (state->vertex_count + 1 > 26 || state->row_count + 7 > 32 || state->secondary_count + 7 > 32)
        commands = func_0016b42c(state, commands);
    return commands;
}
