#include "game/sound_track.h"
#include "game/functions.h"





extern "C" void func_001d9090(SoundTrackPool *pool, void *resource)
{
    FreeListNode *storage = func_0017dfdc(&pool->free_list);
    SoundTrack *track = storage ? func_001d607c((SoundTrack *)storage) : 0;
    if (track) {
        track->resource = resource;
        track->owner = pool->owner;
    }
}
