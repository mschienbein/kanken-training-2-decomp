#include "game/progress_counters.h"

void func_001d2be8(struct ExtendedTimedState *state)
{
    if (state->first.current < state->first.limit)
        ++state->first.current;
    if (state->second.current < state->second.limit)
        ++state->second.current;
    if (state->third.current < state->third.limit)
        ++state->third.current;
}
