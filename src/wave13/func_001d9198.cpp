#include "game/scene_lifecycle.h"
#include "game/sound_track.h"
#include "game/audio_voice.h"



extern "C" SoundTrack *func_001d9198(unsigned first, unsigned second,
                                     unsigned third, unsigned fourth)
{
    SoundTrack *track = func_001d5528(func_00185cc0());
    if (!track)
        return 0;
    track->pending = true;
    AudioVoiceGroup *voice = func_0017e500(func_00185c58(), first, second,
                                         func_0018290c, track);
    if (!voice) {
        func_00182994(func_00185cc0(), track);
        return 0;
    }
    track->task = voice;
    func_001d96e8(track, third, fourth);
    return track;
}
