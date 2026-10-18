#include "game/scene_lifecycle.h"
#include "game/sound_track.h"
#include "game/audio_voice.h"



extern "C" AudioVoice *func_001d9198(int channels, int priority,
    void (*callback)(AudioVoice *, int, void *), void *context)
{
    AudioVoice *track = func_001d5528(func_00185cc0());
    if (!track)
        return 0;
    track->flag13a = true;
    AudioVoiceGroup *voice = func_0017e500(func_00185c58(), channels, priority,
                                         func_0018290c, track);
    if (!voice) {
        func_00182994(func_00185cc0(), track);
        return 0;
    }
    track->group = voice;
    func_001d96e8(track, callback, context);
    return track;
}
