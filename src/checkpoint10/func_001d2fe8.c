#include <stdbool.h>

#include "game/progress_counters.h"

static __forceinline void advance_counter(struct TimedCounter *counter)
{
    if (counter->current < counter->limit)
        ++counter->current;
}

void func_001d2fe8(struct ExtendedTimedState *state)
{
    unsigned short enabled;
    func_001d2be8(state);
    enabled = state->enabled;
    if (enabled & 1)
        advance_counter(&state->channels[0]);
    enabled >>= 1;
    if (enabled & 1)
        advance_counter(&state->channels[1]);
    enabled >>= 1;
    if (enabled & 1)
        advance_counter(&state->channels[2]);
    {
        bool last_enabled = (enabled >> 1) & 1;
        if (last_enabled)
            advance_counter(&state->channels[3]);
    }
}
