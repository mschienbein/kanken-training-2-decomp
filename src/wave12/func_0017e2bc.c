#include "game/sequence.h"
#include "game/audio_voice.h"


void func_0017e2bc(struct AnimationTrack *track)
{
    track->waiting = 0;
    track->dirty = 0;
    track->delay = 0;
    track->active = 1;
}
