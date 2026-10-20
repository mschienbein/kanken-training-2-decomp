#include "game/sequence.h"

typedef void (AnimationTrack::*FloatTrackCallback)(float);

static void visit_tracks(SequencePlayer *player, unsigned mask,
                         const FloatTrackCallback &method, float value)
{
    for (int index = 0; index < 16; ++index, mask >>= 1) {
        if (mask == 0)
            break;
        if ((mask & 1) != 0) {
            AnimationTrack *track = index <= 15 ? player->tracks[index] : 0;
            if (track)
                (track->*method)(value);
        }
    }
}

extern "C" void func_0017e3d8(SequencePlayer *player, unsigned mask,
                              FloatTrackCallback method, float value)
{
    visit_tracks(player, mask, method, value);
}
