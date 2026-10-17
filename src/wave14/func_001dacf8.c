#include "game/event_route.h"
unsigned *func_001dacf8(struct DrawCommandState *state, unsigned *commands, unsigned flags)
{
    state->field698 = 0;
    state->secondary_components = 0;
    state->mode = 0;
    state->kind = 6;
    state->flags = 1;
    commands = func_0016b27c(state, commands, 0, 0);
    return func_001dad48(state, commands, flags);
}
