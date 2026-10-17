#include "game/sequence.h"

static __forceinline void advance_progress(Progress *progress)
{
    short limit = progress->limit;
    short current = progress->current;
    if (current < limit)
        progress->current = current + 1;
}

extern "C" int func_001d4b88(AnimationTrack *track, int mode)
{
    int count;
    if (!track->active)
        return 0;
    Progress *first = &track->progress[0];
    if (first->limit > first->current)
        ++first->current;
    advance_progress(&track->progress[1]);
    advance_progress(&track->progress[2]);
    advance_progress(&track->progress[3]);
    advance_progress(&track->progress[4]);
    if (track->waiting) {
        unsigned remaining = track->activity.wait_count;
        if (remaining)
            return 1;
        track->waiting = remaining;
    }
    if (track->delay > 0 && --track->delay > 0)
        return 1;
    if (track->state.input.available) {
        count = 0;
        while (track->delay == 0 && track->waiting == 0) {
            if (++count > 10000)
                break;
            if (track->vtable->advance(track, mode) == 1)
                return -1;
        }
    }
    return 1;
}
