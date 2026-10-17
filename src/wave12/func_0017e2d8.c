#include "game/sequence.h"
#include "game/audio_voice.h"


void func_0017e2d8(struct AnimationTrack *track, const unsigned char *begin, unsigned size)
{
    track->state.begin = begin;
    track->state.input.end = begin + size;
}
