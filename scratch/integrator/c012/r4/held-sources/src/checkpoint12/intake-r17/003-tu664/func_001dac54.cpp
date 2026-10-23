#include "game/event_route.h"


static unsigned *flush_vertices(DrawCommandState *state, unsigned *commands)
{
    if (state->vertex_count != 0)
        commands = func_0016b42c(state, commands);
    return commands;
}

unsigned char *func_001dac54(DrawCommandState *state,
                           unsigned char *commands, unsigned flags)
{
    unsigned *end = flush_vertices(state, reinterpret_cast<unsigned *>(commands));
    return func_001dac84(state, reinterpret_cast<unsigned char *>(end), flags);
}
