#include "game/audio.h"
#include "game/sound_track.h"
extern "C" void func_001d2b80(struct SoundTrack *track)
{
    track->stopped = 1;
}
