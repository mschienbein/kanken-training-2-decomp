#include "game/audio.h"
#include "game/sound_track.h"
extern "C" unsigned char func_001d018c(SoundStarter *starter, SoundTrack **track,
                                     unsigned resource, unsigned flags)
{
    unsigned char started = starter->start(track, resource, 0, flags);
    unsigned char result = 0;
    if (started != 0)
        return started;
    if (*track != 0)
        func_001d2b80(*track);
    return result;
}
