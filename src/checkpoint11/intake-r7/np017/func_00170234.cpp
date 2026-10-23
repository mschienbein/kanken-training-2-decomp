#include "game/controller_transitions.h"

#include "game/task_runtime.h"

static void channel_range(int requested, int &first, int &end)
{
    if (requested == -1) {
        first = 0;
        end = 4;
    } else {
        first = requested;
        end = requested + 1;
    }
}

int func_00170234(int channel)
{
    TaskRuntime *state = &bss_0029f540;
    if (!state->active)
        return 0;
    int first, end;
    channel_range(channel, first, end);
    /* ORIGINAL BUG: non-sentinel channel values are not range-checked before
     * indexing the four resource records. */
    const RuntimePlaybackChannel *entry = &state->channels[first];
    for (; first < end; ++first, ++entry)
        if (entry->resource != -1)
            return 1;
    return 0;
}
