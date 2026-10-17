#include "game/sound_track.h"
#include "game/functions.h"



extern "C" SoundTrack *func_001d607c(SoundTrack *track)
{
    track = func_001d540c(track);
    track->vtable = data_00224964;
    return track;
}
