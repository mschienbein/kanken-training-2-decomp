#include "game/command_group.h"
/* All observed callers initialize writable stack storage and discard the result. */
void func_001950ec(struct ReadResultState *state)
{
    state->buffer = 0;
    state->size = 0;
    state->count = 0;
    state->flags = 0;
}
