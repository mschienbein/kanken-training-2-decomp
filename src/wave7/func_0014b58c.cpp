#include <stddef.h>

#include "game/sound_runtime.h"

static __forceinline int requested_count(const SoundFileGroup *group)
{
    return group->requested;
}

static __forceinline int playing_count(const SoundFileGroup *group)
{
    return group->playing.count;
}

extern "C" void func_0014b58c(SoundFileGroup *object, int requested)
{
    /* ORIGINAL BUG: a negative limit is retained instead of clamping to zero. */
    if (object->limit < requested)
        requested = object->limit;
    else if (requested < 0)
        requested = 0;
    object->requested = requested;
    for (;;) {
        int limit = requested_count(object);
        if (playing_count(object) <= limit)
            break;
        SoundInstance *voice = (SoundInstance *)((unsigned char *)object->pending.sentinel.next - offsetof(SoundInstance, player_link));
        voice->vtable->stop(voice);
    }
}
